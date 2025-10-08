# Dialogs

For complex configuration, Rack menus aren't very usable.
So, I've been prototyping how to make dialogs: UI that pops up from a button or keypress on the panel.

I've tried several ways of doing it, and they each have their benefits and tradeoffs.
They are all rooted in a Rack `Menu` (and hence a Rack `MenuOverlay` under the hood) to get the necessary semi-modal popup behavior.

A dialog widget is built a lot like a module widget except it isn't restricted to module dimensions.
It can be any size or shape and sits on top of all the modules.
It can be based on an SvgWidget for a background panel or custom-drawn.
You add child widgets for the UI details, just like a module widget.

This part is simple and familiar. The tricky part is getting good user experience when it's invoked.
We'll get to that in a moment, but just a couple of details for a dialog widget.

## Dialog widget requirements

Your dialog widget will need a couple of things at minimum:

1. An `onAction` event handler to prevent it from closing when clicking on the dialog.

   ```cpp
   void onAction(const ActionEvent& e) override {
       e.unconsume(); // don't close menu overlay
   }
   ```

2. A means of closing the dialog (optional).

   The dialog, like a menu will always close when clicking outside of the dialog.
   It's still nice to have a widget right on the dialog, so that it's recognizable as a dialog.

   This method can be called from a "close" or "ok" button on the dialog.

    ```cpp
    void close() {
        MenuOverlay* overlay = getAncestorOfType<MenuOverlay>();
        if (overlay) {
            overlay->requestDelete();
        }
    }
    ```

## Creating and displaying the dialog

Presenting the dialog works just like displaying a module menu.
In some method of your module widget, create a menu and add a child.

### Option 1: Simple menu

The simplest way to get the popup behavior is make the dialog a child of a regular Rack menu.
A standard menu is rooted in a `::Rack::ui::MenuOverlay`, which works as follows.
The menu overlay makes itself at (0.0) and the size of it's parent in it's `step()` so perfectly overlays it's parent.
It handles clicks by requesting deletion, which removes the overaly and its children.
This event isn't propagated, so whatever you clicked on (a module, knob, or whatever) doesn't receive that click.

Here's a little Rack-style creation template that does the job:

```cpp
// dialog.hpp
#include <rack.hpp>

template <typename TDialog>
TDialog* createMenuDialog()
{
    TDialog* dialog = new TDialog();

    auto menu = ::rack::createMenu();
    menu->addChild(dialog);
    return dialog;
}
```

Doing it this way we get:

- The dialog appears positioned with the top left corner at the mouse position when invoked.

  If you're opening the dialog from a small button, this is not very noticeable.
  On a bigger button it's more noticeable.
  If you're opening the dialog on a keystroke, this feels random.
  And if from a menu item, just ... weird: floating somewhere off the module.

- Same general behavior as a menu, except that (as expected) clicking on the dialog doesn't close it because we overrode `onAction`.

- Clicking outside the dialog closes it.
  This is different from a typical modal dialog in other applications, but I don't mind in the context of Rack.

- Like module menus, the dialog is at Rack's **Ui Scale** setting.
  Personally, I find it disconcerting for a dialog if your Rack is not zoomed close to 100%.
  It looks out of place — either too small or too large.

- You cannot scroll or zoom while the dialog is up.

We can fix the positioning easily, but not the scaling or scrolling.
The sample implementation in this repo adds support for additional features:
optional dialog positioning and centering, and optional Rack screening (exposing a feature in Racks `MenuOverlay`).

### Option 2: Enhanced dialog

With this option we fix some of the limitations of Option 1.

The UI-scale and the lack of scrolling and zooming is because `createMenu` adds the menu as a child of `APP->scene`.
The Scene itself doesn't scroll and zooms to the Rack _UI scale_ setting.
So, we'll parent the overlay to something that _does_ scroll: `APP->scene->rackScroll`.

This still doesn't take care of the zooming, which also affects the position.
I tried a number of ways to get the correct parent to achieve this, but I didn't find a way to make that work: the correct widget isn't readily accessible.
So, I introduce a widget that does know how to zoom, and tracks the zoom factor and the intended position relative to the module:

```cpp
// dialog.hpp
#include <rack.hpp>
...
struct TrackingZoom: ::rack::widget::ZoomWidget
{
    using Base = ::rack::widget::ZoomWidget;

    Widget * pseudo_parent{nullptr};
    Vec pseudo_pos;

    TrackingZoom(Widget *pseudo, Vec pos) : pseudo_parent(pseudo), pseudo_pos(pos) {}

    void step() override {
        Base::step();
        float current_zoom = APP->scene->rackScroll->getZoom();
        if (getZoom() != current_zoom) {
            setZoom(current_zoom);
        }
        auto abs_pos = pseudo_parent->getAbsoluteOffset(pseudo_pos);
        if (APP->scene->menuBar && APP->scene->menuBar->isVisible()) {
            abs_pos.y -= APP->scene->menuBar->box.size.y;
        }
        box.pos = APP->scene->getRelativeOffset(abs_pos, APP->scene->rackScroll);
    }
};
```

The tracking happens in `step`, which introduces a delay proportional to UI frame rate that results in slight jumpiness and flickering as you scroll and zoom. I guess we can't have everything.

To use the tracking widget we need a revised creation template, which I'll show in the next section.

## Dialog base class and SVG dialog template

The basic dialog requirements are the same for all dialogs, so let's make some templates to take care of the common stuff.
Most dialogs will need to refer back to the module widget or it's module.
For example, to add a knob or switch for a module parameter, so we'll provide for that as well.

In addition, we'll want an easy way to make SVG-based dialogs.

In the next section we'll wrap it all up with a common creation template to glue all the pieces together.
It will rely on the following:

```cpp
// dialog.hpp
#include <rack.hpp>
...

// Common behavior required of a dialog
struct DialogBase: OpaqueWidget
{
    ModuleWidget * source{nullptr};

    DialogBase(ModuleWidget * src) : source(src) {}

    void onAction(const ActionEvent& e) override {
        e.unconsume(); // don't close menu overlay
    }

    void close() {
        MenuOverlay* overlay = getAncestorOfType<MenuOverlay>();
        if (overlay) {
            overlay->requestDelete();
        }
    }
};

// Template for an SVG-based dialog
template<typename TSvg>
struct SvgDialog: DialogBase
{
    ::rack::widget::FramebufferWidget* fb;
    ::rack::widget::SvgWidget* sw;

    SvgDialog(ModuleWidget* src) : DialogBase(src) {
        fb = new widget::FramebufferWidget;
        addChild(fb);
        sw = new widget::SvgWidget;
        fb->addChild(sw);

        set_svg(::rack::window::Svg::load(TSvg::background()));
    }

    void set_svg(std::shared_ptr<window::Svg> svg) {
        if (!sw->svg) {
            sw->setSvg(svg);
            box.size = sw->box.size;
            fb->box.size = sw->box.size;
            fb->setDirty();
        }
    }
};
```

You might notice the `TSvg` template parameter for the SVG dialog.
Rack's widget setup always uses inheritance to add the specific SVG(s).
I prefer to use the templated approach here.

To create an SVG dialog, first define a little SVG provider,
and our dialog implementation will inherit from the templated base:

```cpp
// my_source.cpp
#include "dialog.hpp"

struct SampleDialogSvg {
    static std::string background() {
        return asset::plugin(pluginInstance, "res/sample_dlg.svg");
    }
};

struct SampleDialog : SvgDialog<SampleDialogSvg>
{
    SampleDialog(ModuleWidget* src) : Base(src) {
        // add dialog widgets here
    }
};
```

For a dialog, we may need widgets that aren't part of the Rack component library.
That includes a "close" or "ok" button.
I'll cover making dialog widgets (and other custom UI widgets) in another Dev note.

### Templated dialog creation

Finally, to wrap it up the basic dialog services, the creation template:

```cpp
// dialog.hpp
#include <rack.hpp>
...

// source: Module widget that is the origin of the dialog.
//    pos: Position relative to 'source' for the dialog.
// center: Optional. True to center the dialog at pos.
// screen: Optional. color to screen rack (should have transparency).
//   To dim Rack behind the dialog, an nvgRGBAf(0,0,0,.5) screen works well
template <typename TDialog>
TDialog* createDialog(ModuleWidget* source, Vec(pos), bool center = false, NVGcolor screen = NVGcolor{0})
{
    TDialog* dialog = new TDialog(source);
    if (center) {
        pos = pos.minus(dialog->box.size.div(2));
    }

    ui::MenuOverlay* menuOverlay = new ui::MenuOverlay;
    menuOverlay->bgColor = screen; // dim rack

    auto zoomer = new TrackingZoom(source, pos);
    menuOverlay->addChild(zoomer);
    zoomer->addChild(dialog);
    APP->scene->rackScroll->addChild(menuOverlay);

    return dialog;
}
```

## TBD

I plan to follow up this Dev note with another covering custom widgets suitable for dialogs, and a MessageBox for Rack.

| | |
|--|--|
| ![pachde (#d) logo](./assets/Logo.svg) | Copyright © Paul Chase Dempsey |
