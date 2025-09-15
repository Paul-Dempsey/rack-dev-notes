# Plugin images for documentation

To prepare images for documentation, here's what I do.

I do my primary dev work on Windows, so all these instructions are for Windows, but other environments have equivalent commands and tools.

1. Start a patch, and place the unconnected module in isolation.
1. Zoom Rack to 150% in the View menu.
1. Take a screen shot (Win+Shift+S). In most environments you can snip using a rectangle around the module.
1. In GIMP, **File** / **Create** / **From Clipboard**  (Ctrl+Shift+V)
1. Crop as desirted
1. Scale to 50% of the size
1. Export PNG

The example here includes the Rack rails.

For a cleaner look, you can take a couple of extra steps to have an image of just the module. For this I use [DanT Purfenator](https://library.vcvrack.com/DanTModules/Purfenator) or [pachde nulll](https://library.vcvrack.com/pachde-one/pachde-null) to set up a green-screen style background to conceal the Rack skiff. pachde null has several factory presets for green-screening.

After setting up the green-screen, taka snip as above. Then, instead of manually cropping:

1. **Select** / **By Color** to select the screen background.
   You may have to expand the color range in the toolbox or shift+click multiple times to include the shadows.
1. **Delete**
1. **Image** / **Crop to Content**
   If your selection is good, you'll get just the module.

| | |
|--|--|
| ![pachde (#d) logo](./assets/Logo.svg) | Copyright © Paul Chase Dempsey |
