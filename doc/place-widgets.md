# Place Widgets with SVG

One of the most challenging things when making module panels is keeping your widget positioning in sync with a changing panel design.
[Hot-reload SVGs](./hot-svg.md#hot-reload-svgs) helps with that.

The Rack Tutorial provides `helper.py`, a script that reads a panel SVG and generates source code to create and position widgets.
This code-generation approach works ok for first-time bootstrapping a plugin, but it doesn't hold up for iteration on the module and panel designs.
It also supports only a small selection of stock Rack components.

The approach here has similarities to the Rack Tutorial scheme where placeholders are introduced into the SVG.
But the similarity pretty much ends there.
Here we need no scripts or special build process.
We create a panel SVG following some simple conventions, and use runtime code to get the information we need.

The basic design here is:

1. Add a placeholder for a widget.
1. Set a unique id for the placeholder using the **id** attribute for the element.
1. At `ModuleWidget` creation time, use a helper to read out coordinates for positioning the widget.
1. Hide the svg placeholders before the panel is drawn.

## Designing a panel

A simple idea for designing a panel is to simply import the SVGs for the widget.
Rack component library widgets are mostly composed of layers of SVGs, so you'd need to make a library of design components that have the right look.

While this gives you an image that has the final look of the module,
it is a lot of inital work to create the design components, and you really can't leave them in your shipped panel.
It adds a lot of size and complexity, and in some cases, won't be perfectly hidden by actual widgets.

## Adding SVG placeholders

I recommend taking a "wireframe" approach.
Simple circles and rectangles represent the knobs, buttons, ports, and switches.
In Inkscape, drop a circle or rectangle and type the radius or dimensions in the toolbar.
It works best to use a filled shape without a stroke.

For dimensions, a simple heuristic is to refer to the `viewBox` element in the component library SVG.
It's quickest to open the SVG in a text editor and read it out.
It's the `viewBox` attribute on the `svg` element at the top of the file.

```svg
<svg ... viewBox="0 0 16 16"
```

| Widget | Reference SVG or dimension |
| -- | -- |
| knob   | a circle the radius of the background (`*_bg.svg`) |
| button | circle the radius of the "up" SVG (`*_0.svg`) |
| slider | slider background SVG |
| switch | first SVG (`*_0.svg`) |
| tiny light   | circle radius 2.9px |
| small light  | circle radius 5.8px |
| medium light | circle radius 8.8px |
| large light  | circle radius 14.7px |

The design folder in this repository includes `wireframe.svg` with sized placeholders for common VCV components.
The ids are based on the name of the Rack component they correspond to.
The wireframe file is highly simplified and amenable to copy and paste using a text editor.
If copying these in Inkscape, use Ctrl+Click to select an individual placeholder sepoarately from the group it is in.

![Wireframe asset](../design/wireframe.svg)

I often work on the same svg in inkscape and a text editor side-by-side.
After saving in the text editor, use _File_ / _Revert_ to reload it in Inkscape.
VSCode and other programming text editors will automatically reload a file after it is changed on disk (or have an option for auto-reload).

## Element ids in Inkscape

Inkscape does't make it easy to edit element ids.
Instead of the id, Inkscape uses it's own `inkscape:label` attribute to identify elements in most of it's UI, such as the _Layers and Objects_ window.
Inkscape does auto-generate ids, but these aren't very informative or useful.
The `inkscape:label` attributes don't exist in a loaded svg in Rack.

You can edit element ids in Inkscape using the _Object Properties_ window, but this is clunky and inefficient.
It turns out to be much easier in practice to edit element ids in a text editor.

Once you've edited an element id outside of Inkscape, Inkscape is good about preservingthe id you authored.

## Using the svg query helpers

SVG helper code is included here:

| File | Description |
|--|--|
| [`src/svg-query.hpp`](../src/svg-query.hpp) | SVG helper Header  |
| [`src/svg-query.cpp`](../src/svg-query.cpp) | SVG helper implementation. |

### Get the loaded panel svg

Once you've created the widget's panel, you have access to the panel svg.
There are several provided functions for retrieving the svg from the panel.

Here, while constructing the `ModuleWidget`, we get the svg from the panel.
It's named `layout` because the svg is the source of the layout for all the widgets on the panel.

```cpp
auto panel = createPanel(asset::plugin(pluginInstance, "res/my-panel.svg");
auto layout = svg_query::panelSvg(panel);
setPanel(panel);
```

### Make a bounds index

You can get the bounds of a single element at a time using the `itemBounds` function,but if you have more than a couple of widgets, it's more efficient to make a bounds index.
For creating an index, we use a prefix to identify placeholders separately from other graphical elements.

In these example we use a single prefix `k:`.
You can use any prefix you like, and it isn't required to use any kind of separator like `:` - it just has to be the common prefix.
For elements that are not used for positioning, we omit the prefix.

```cpp
svg_query::BoundsIndex bounds;
svg_query::addBounds(layout, "k:", bounds, true);
```

The last parameter to `addBounds` determines if the prefixed elements are hidden or not while building the index.
Here, we hide all positioning elements.

You can use more than one prefix if you'd like.
For example one could use a naming convention to differentiate knobs, lights, inputs, and outputs, suing the `k:`, `l:`, `in:`, and `out:` prefixes.
In this case, you'd call `addBounds` for each prefix to build a complete index for all the positioning elements.

### Position widgets based on element bounds

Whether we're using an index or querying individual elements, what we get from the svg is the bounding box for the element.
Once loaded, there is no distinction between elements - they are all reduced to paths, losing information on whether the element is a rectangle, a circle, or a path.

The bounds can then be used to calculate the position of a widget in relation to the svg element.
The bounds rect is a `::rack::math::Rect`, which has useful methods for getting coordinates and measurements including the center, height, and width.

Example:

```cpp
addChild(createParamCentered<RoundKnob>(bounds["k:freq"].getCenter(),
    module, MyModule::Params::P_FREQUENCY));
```

For a bounds index, attempting to reference an id that doesn't exist in the index crashes Rack with an assertion failure.
This forces you to ensure that you haven't made a typo in the placeholder name.

If a widget gets positioned where another widget should be, look for an unintentional duplication of element ids.
More than once, I've forgotten to update the id of a copy/pasted element, causing this symptom.

If we query for an id that isn't found, `itemBounds` returns an (Infinity,Infinity,0,0) rectangle.
If using this method, it might be a good idea to assert that the position isn't infinity with something like `assert(isFinite(r.pos.x));`.

## Repositioning when hot-swapping Svgs

Runtime positioning can be integrated with svg hot-reload.
The helper support for repositioning on hot-reload is at the end of `svg-help.hpp`.

> **#d TODO** Add examples, hot-swap svg integration, and the enhanced svg_query from WIP pachde1

| | |
|--|--|
| ![pachde (#d) logo](./assets/Logo.svg) | Copyright © Paul Chase Dempsey |
