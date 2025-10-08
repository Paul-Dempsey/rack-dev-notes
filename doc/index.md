# Dev notes for VCV Rack

![pachde (#d) logo](./assets/Logo-24.svg) by pachde (#d)

This project collects pachde (**#d**) notes about developing plugins for VCV Rack.
It combines stuff I've posted on the [VCV Community](https://community.vcvrack.com/), the [VCV Rack Discord](https://discord.gg/wxa89Mh), and new information not posted elsewhere.

> **License** — All English content and images in this repo are Copyright © Paul Chase Dempsey, All rights reserved.
> All programming-language source code and configuration in this repo is MIT-licensed.

| Note | Description |
| -- | -- |
| [Use GenericBlank](./use-generic-blank.md#use-genericblank) | Use the **#d** Generic Blank template repo to start a new plugin. |
| GenericBlank DSP | (**#d** TODO) After your Blank is up and running, we can add a module that does some Digital Signal Processing (DSP). |
| [Place Widgets with SVG](./place_widgets.md#place-widgets-with-svg) | (**#d** TODO) Get widget positions from SVG. |
| [Project structure](./project-structure.md#project-structure) | File layout of Rack plugin |
| [Build Rack](./build-rack.md#build-rack) | A.K.A. No SDK. |
| [Debugging](./debug.md#debugging) | Setting up Rack debugging in VSCode on Windows. |
| [Panels](./panels.md#panels) | My Panel process, plus tips for using Inkscape. |
| [Hot-reload SVGs](./hot-svg.md#hot-reload-svgs) | For fast iteration on panel SVGs, implement hot-reloading. |
| [Make dialogs](./dialogs.md#dialogs) | Making popup dialogs for your modules. |
| [Jitter periodic updates](./jitter-periodic.md#jitter-periodic-updates) | Mitigate CPU spikes by adding jitter to periodic (control-rate) code. |
| Documentation on GitHub | (**#d** TODO) Minimal viable docs on GitHub |
| [Doc images](./doc-images.md#plugin-images-for-documentation) | Preparing module images for documentation. |
| Releasing to the Rack Library | (**#d** TODO) Checklist for releasing to the VCV library |
| Localization | (**#d** MAYBE) |

Everyone has their own preferences and techniques.
The best practice is whatever works for you.
Here, I'm sharing what _I_ do.
I hope that you might find some of these notes helpful.

I appreciate honest and productive feedback.
If you notice any problems or inaccuracies in this content, please let me know.
Feel free to open an issue in this repo or contact me.

## Source code available

This source code is MIT-licensed for your use.

| File | Description |
|--|--|
| [`src/hot-svg.hpp`](../src/hot-svg.hpp) | Header for _Hot-reload SVGs_ |
| [`src/hot-svg.cpp`](../src/hot-svg.cpp) | Cache implementation for _Hot-reload SVGs_ |
| [`src/svg-query.hpp`](../src/svg-query.hpp) | SVG helpers for _Place Widgets with SVG_ |
| [`src/svg-query.cpp`](../src/svg-query.cpp) | SVG helper impls for _Place Widgets with SVG_|

## Contact

You can find me online as *pachde* or *Paul Dempsey* in:

- [VCV Rack Community](https://community.vcvrack.com/) (Discourse)

- [VCV - Discord](https://discord.gg/rhcztE4sZd)

- The *Haken Continuum* and *Expressive E Osmose* Facebook groups

- [Expressive E community - Discord](https://discord.gg/UtMynfPqw7) -- the "official" Osmose Discord monitored by Expressive E.

- [Expressive E instruments - Discord](https://discord.gg/ZjHTgmCR9w) -- a community Discord. (Check out the Eagan Matrix channel).

If you want something more private, you can also get my email contact from the right-click menu of any **#d** VCV Rack module.

| | |
|--|--|
| ![pachde (#d) logo](./assets/Logo.svg) | Copyright © Paul Chase Dempsey |
