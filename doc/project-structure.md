# Project Structure

(originally in [this Community thread](https://community.vcvrack.com/t/getting-started-with-plugin-development/24323/8))

 You can organize the source for your project as you wish.
 Convention is for all source to be in src.

Rack itself has a separate `include` folder at the same level as `src`, with any organizing folder structure under `src` mirrored under `include`.
This makes a lot of sense for something that is consumed by others as an SDK (without source), but it’s not necessary for plugins.
I use a different structure.

The standard makefile expects all resources that will be installed to be in `res`.
This is the `DISTRIBUTABLES += res` line in the makefile.
You will always need resources if you're using SVGs for panels, and any fonts you want to use other than the ones provided by Rack.
If you have "factory" presets and selections, you can see the parallel folder convention for these types of content that can be installed with your plugin.
This is where Rack infrastructure expects them to be, so you must follow the convention.

I like to keep documentation in the same repo with the source.
Some developers keep a separate repo for documentation.
I don't publish to a site (yet), but it's not the worst thing to browse the markdown docs right in the repo on github.
Some keep the doc source elsewhere, and publish to a site.
Others don't bother creating any docs at all.

I keep working stuff separated from installed stuff, so I have folders for this.

---

My current plugin is quite complex, with extensive custom UI widgets, and modules that communicate among each other, so the structure is correspondingly more complex than a simpler plugin.
My other projects have been much flatter.

Some stuff is here that I wouldn't normally keep in source control,
but since I'm now switching between Windows, Mac, and Linux, and sometimes need to work away from home,
more goes in there so that it's easier to switch which machine I'm working from.

My elaborate folder structure for pachde CHEM is like this (no `include` folder in sight :-):

```cpp
.github/workflows  // GitHub actions for building releases
.vscode     // local vscode configuration (.gitignore'd so not checked in)
boneyard    // stuff I'm not currently using but might want later
design      // svg master files, sketches, notes
dev         // scratch files (.gitignore'd)
doc         // documentation source, starting with index.md
doc/image   // all graphics files for the docs (svgs, screen shots)
res/fonts   // fonts I use for displaying text
res/logo    // project logos
res/panels  // all the module panels
res/symbols // specialized svg graphics
res/themes  // Files for my fancy themeing system go here
res/widgets // ui widget svgs
src/em      // a major subsystem for the plugin used by many modules
src/modules/<module> // Folder for each module (14 modules)
src/services // common services used everywhere (mostly header-only) (33 files)
src/widgets // ui widgets (48 files)
```

To keep the size down, each module is usually composed of a _module.hpp_, _module.cpp_, and _module-ui.cpp_.
If a source file is getting too big (around the 1000-line mark for me), I sometimes break it down further.
So, in a few modules you can find _module-ui-create.cpp_ (ui creation separate from other ui logic), _module-file.cpp_ (file i/o), and _module-ui-events.cpp_ (when I need a lot of ui event overrides).

Some of the modules have widgets unique to that module so I sometimes have a `widgets` folder under the module.

| | |
|--|--|
| ![pachde (#d) logo](./assets/Logo.svg) | Copyright © Paul Chase Dempsey |
