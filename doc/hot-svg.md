# Hot-reload SVGs

One challenge to making a great-looking plugin is making great SVGs that define the look of a module.
It's the exceptional artist that gets it right the first time.
It can take many iterations of changing the SVG to see how it interacts with the position of widgets in the module to define the final appearance.
It's tedious to have to continually close Rack, update an SVG, and run it again to see changes.

If you're using the SDK, it's even worse because you have to `make install` on each iteration.
One of the reasons to [Build Rack](./build-rack.md#build-rack), is that it allows you to iterate panel graphics faster, with less tedium.

So, how can we iterate on our graphics assets faster?
We borrow a technique from games programming: **hot-reload**.
This means being able to reload the graphics from disk without restarting the program.

Now, games often have a highly engineered system with a file-system watcher to automatically detect changes and reload assets as soon as the file changes.
That would be cool, but that's a lot of complexity to engineer, especially in a cross-platform way, and too much to cover in a simple dev note.

So, we'll implement a pragmatic minimal solution where we invoke the reloading manually.

What follows is a step-by step guide, including all the code required to have SVG hot-reloading in your plugin.
This note is written for a newcomer, so I don't assume you know about writing makefiles and the like. Bear with me if the explanations cover things that "every developer should know already" :-).

> **Note** —
> I assume you're starting from a generic VCV Rack plugin build, and you're building from the command line.
> I based this exercise on a project created using the GenericBlank template (see [Use GenericBlank](./use-generic-blank.md#use-genericblank)). So this uses the structure and naming of GenericBlank.

## Hot-reload feature

Let's run down the tl;dr on the hot-reload feature:

- Light and Dark panels following the VCV Rack mechanism.
- Hot-reload doesn't make sense for end users of your plugin, so we only add it for developer builds.
- Hot-reload is initiated manually by:
  - Press F5 when the mouse is over the module.
  - Right-click menu item.
- Hot-reload affects all modules of the plugin at once.

As a side effect, this exercise shows how to differentiate developer and release builds.
There are a variety of things we might want to have only in developer builds,
so this aspect (the first step below) is a useful technique on it's own.

### Conditional compilation (developer build)

We only want hot-reload in builds for the developer and not in the plugin installled from the Rack Library.
For this we set up conditional compilation in the makefile so that we can differentiate between developer builds and other builds.

We'll also add a feature flag for the hot-reload feature.
This can allow us to differentiate this feature from other otherwise unrelated developer-only features.

In the plugin `makefile`, add the following block after the `RACK_DIR ?= ../..` line at the top.

```makefile
ifdef DEV_BUILD

# turn off optimizations
FLAGS += -O0
# include hot-swap SVG support
FLAGS += -DUSE_HOT_SVG
#SOURCES += src/hot-svg.cpp

else

# turn on max optimization
FLAGS += -O3
# define NDEBUG (weird negative, but commonly used for excluding asserts)
FLAGS += -DNDEBUG

endif
```

In our plugin code, well be able to use "if-defs" with the `USE_HOT_SVG` symbol to separate the hot-svg code from regular code in the project.
See how it's defined in the makefile above.
You'll see that used in later steps.

Since we don't have the hot-reload code yet, the inclusion of it's source is commented out.
You can try making a devloper build with just this change.
Since we've only touched the project makefile, we must first clean the project to ensure it gets rebuilt:

```shell
make clean
```

Then launch a developer build:

```shell
make DEV_BUILD=1
```

This command defines `DEV_BUILD`, so you get **if** part of the block, and not the **else** part.
To make a regular release build without the dev features as it would be for a Rack library build, run:

```shell
make clean
make
#or 'make install'
```

In other words, do the regular normal thing. That's what the Rack Library builds will do.
Becuase we haven't updated the GitHub action, the GitHub builds are not dev builds.

So what did we get from this change, given there's nothing about the main topic of this dev note here yet? We got:

- A developer build with optimizations turned off.

  This makes using a debugger easier.

- In non-dev (release) builds, we've turned off asserts by defining `NDEBUG`.

  This is different from normal Rack builds where asserts are enabled (and sometimes surprises people).

## Add hot-reload support code

Now we'll add the support code to the project.
The support code consists of a header and cpp file for an SVG cache.
We will bypass the normal Rack SVG cache which doesn't support reload, and instead use a plugin-specific cache that does support reload.
This will require some changes in how panels get created, which we'll show in later steps.

1. Add the file `src/hot-svg.hpp` to your project, containing this:

   ```hpp
   #pragma once
   #include <rack.hpp>

   namespace hot_svg {

   struct Cache
   {
       // map uses the filename as the lookup key
       std::map<std::string, std::shared_ptr<::rack::window::Svg>> svgs;
       // load and return a cached SVG shared pointer
       std::shared_ptr<::rack::window::Svg> load(const std::string &path);
       // reload all svgs in the cache
       void reload();
   };

   }
   ```

2. In the `makefile`, Un-comment the inclusion of the implementation file:

   ```makefile
   SOURCES += src/hot-svg.cpp
   ```

3. Add the file `src/hot-svg.cpp` to your project, containing this:

   ```cpp
   #include "hot-svg.hpp"
   namespace hot_svg {

   // nearly identical to Rack's cache
   std::shared_ptr<::rack::window::Svg> Cache::load(const std::string & path)
   {
       const auto & pair = svgs.find(path);
       if (pair != svgs.end()) {
           return pair->second;
       }

       std::shared_ptr<::rack::window::Svg> svg;
       try {
           svg = std::make_shared<::rack::window::Svg>();
           svg->loadFile(path);
       } catch (::rack::Exception& e) {
           WARN("%s", e.what());
           svg = nullptr;
       }
       svgs[path] = svg;
       return svg;
   }

   // Reload all SVGs in the cache
   // For changes to be visible in Rack, you must also send a dirty event
   // to affected module widgets after reloading the cache
   void Cache::reload()
   {
       for (auto pair: svgs) {
           try {
               pair.second->loadFile(pair.first);
           } catch (::rack::Exception& e) {
               WARN("%s", e.what());
           }
       }
   }

   }
   ```

At this point you can build the project in dev mode to make sure the makefile works and the code compiles. Even if we aren't using it yet.

## Add the hot-reload cache to the plugin

In `src/plugin.hpp`, include the `hot-svg.h` header when the `USE_HOT_SVG` feature is enabled (which the makefile turns on for dev builds).

```hpp
#if defined USE_HOT_SVG
#include "hot-svg.hpp"
#endif
```

And append this to the end:

```hpp
#if defined USE_HOT_SVG
extern hot_svg::Cache svg_cache;

// is module from this plugin?
inline bool isPluginModule(Module* module) {
    return module ? pluginInstance == module->model->plugin : false;
}

void hotReloadSvgs();
#endif
```

This delares the global instance of the cache, and defines a helper that can identify modules that come from this plugin.

In `src/plugin.cpp`, we'll create that cache instance and add a function that does the reloading work that we can call from a module.

```cpp
#if defined USE_HOT_SVG
hot_svg::Cache svg_cache;

void hotReloadSvgs()
{
    svg_cache.reload();

    // refresh all the plugin's module widgets
    auto module_widgets = APP->scene->rack->getModules();
    for (auto mw : module_widgets) {
        if (isPluginModule(mw->module)) {
            mw->onDirty(::rack::widget::Widget::DirtyEvent{});
        }
    }
}
#endif
```

## Add hot-reload access to the module user interface

Our design calls for a key press and a menu item to invoke SVG reloading.
So, we go to our nmodule widget and add or modify `onHoverKey` for the key press, and add/modify `appendContextMenu`.

Here's the code, assuming you don't already have overrides for these fuunctions.
If you're following from a stock GenericBlank, you won't have these yet.
If you do, the you'll need to merge the logic.

```cpp
    void onHoverKey(const HoverKeyEvent& e) override
    {
        auto mods = e.mods & RACK_MOD_MASK;
        switch (e.key) {
#if defined USE_HOT_SVG
        case GLFW_KEY_F5: {
            if ((e.action == GLFW_RELEASE) && (0 == mods)) {
                e.consume(this);
                hotReloadSvgs();
            }
        } break;
#endif
        case GLFW_KEY_MENU:
            if ((e.action == GLFW_RELEASE) && (0 == mods)){
                e.consume(this);
                createContextMenu();
            }
            break;
        }
        ModuleWidget::onHoverKey(e);
    }

    void appendContextMenu(Menu *menu) override
    {
#if defined USE_HOT_SVG
        menu->addChild(new ::rack::ui::MenuSeparator);
        menu->addChild(createMenuItem("Hot-reload plugin SVGs", "F5", []() {
            hotReloadSvgs();
        }));
#endif
    }
```

You might notice that I slipped in a handler for the **Menu** key that brings up the right click menu (if your keyboard has that key).
You can delete that case if you want, but hey, a free feature!

At this point you can check that the project builds and runs, but the feature won't work until we change our widgets to use the hot-reload cache instead of the Rack cache (next step).

## Load SVGs to the hot-svg cache

The last step is loading our SVGs into the cache.

In this example code, you'll need to change the paths to reflect your project's organization and file names.

> **NOTE** — Remember to make sure you use the exact filename case as your files on disk!
If you don't use a consistent capitalization between your code and the files, your plugin works fine on Mac and Windows, but fails to load the SVG on Linux.

In the module widget constructor, we'll load the panel SVGs into the cache as follows:

```cpp
struct MyModuleWidget : ModuleWidget
{
    MyModuleWidget(MyModule* module)
    {
        setModule(module);
#if defined USE_HOT_SVG
        auto panel = new ::rack::app::ThemedSvgPanel;
        // Load SVGs into the plugin's SVG cache
        auto light = svg_cache.load(asset::plugin(pluginInstance, "res/my-panel-light.svg"));
        auto dark = svg_cache.load(asset::plugin(pluginInstance, "res/my-panel-dark.svg"));
        panel->setBackground(light, dark);
#else
        // standard VCV Rack themed panel
        auto panel = createPanel(
            asset::plugin(pluginInstance, "res/my-panel-light.svg"),
            asset::plugin(pluginInstance, "res/my-panel-dark.svg"));
#endif
        setPanel(panel);
```

Do the same for each module in your plugin.

And we're done! You should be able to do a dev build and run it and try out the feature.

Once you're up and running, give it a try.
While running Rack with your module visible, modify the panel SVG in an easily visible way, such as changing the panel color, or adding a rectangle that won't be covered by a control.
After saving the changed SVG, click on your module and press F5.
You should immediately see the affect of your change on the module.

Now try it with multiple instances of your module, or other modules from your plugin in the patch.
All instances should update when you press F5 on any one of them.

Because all modules are afffected from any one of them, you don't actually have to implement the hot-swap UI code into all modules (even though that is most convenient).
