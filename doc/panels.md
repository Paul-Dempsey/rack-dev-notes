# Panels

My usual workflow for a new module is to prototype the panel design before I even start coding a module.
Sometimes I will also write a draft of the documentation before coding.
This helps me work out whether the design is sensical and can be easily explained and understood.
Other devs do it the other way around: they don't worry much about the panel design until after they have a working module, and sometimes never get around to documentation, much to the dismay of some users.

I have a collection of stock SVG components (knobs, sliders, ports) that I can import or copy/paste that get removed for the actual running panel.

After I have this initial design up and running in a Rack module, there is a bit of transitional Inkscape process to eliminate transforms.
I eliminate the transforms because they make it very difficult to read and edit the SVG text.
It obscures where an object is on the panel.
I am unable to evaluate transform matrices in my head, and the calulator app on my machine doesn't do matrix math.

To eliminate most transforms, ungroup everything that is grouped, then re-group.
Unfortunately, this doesn't eliminate all transforms.
For the recalcitrant objects, there is an Inkscape extension that applies transforms.
It works, but only on objects converted to paths, it's old, and always generates ugly warning popups.

After that I hand-edit the SVGs as text.
I edit out all traces of inkscape.
All changes after that are in the VSCode text editor, with svg text and preview side-by-side.

Using a text editor is easier for me to ensure consistent colors, stroke widths, and positioning across modules.
I can copy/paste then adjust coordinates and dimensions precisely.

I use SVG hot-reloading ([Hot-reload SVGs](./hot-svg.md#hot-reload-svgs)).
This makes it quick and easy to make live, fine-grained adjustments.

## Inkscape tips

- **Use pixel (px) coordinates.**

  Ignore the Rack guide that says to use millimeter (mm) units.
  Rack's own assets don't consistently follow Rack' guidance.
  Millimeter units are only useful if you are trying to exactly replicate a specific physical Eurorack Module based on it's specs.
  Even then, the conversion is easily done because you can convert mm to px by appending the expression `* 2.95` in the toolbar (or a calculator).

- Set up your SVG document like this: the most important items to set are circled -- the others are per your preference.

  ![Inkscape document setup for VCV Rack module panel](./assets/ink-doc-setup.png)

  With these settings, all your widget positioning code works in pixels: the same units that the Rack display system uses.
  This allows for precise pixel positioning with a minimum of artifacts.
  Your code doesn't need `mm2px` everywhere.

- **Position and size using the toolbar**

  Instead of dragging with the mouse, which is always imprecise, use the toolbar.
  You can copy/paste exact locations and dimensions for consistency.

- **Calculate in the toolbar**.

  Inkscape supports entering mathematical expressions in the coordinate edit boxes.
  For example, if you want a 10HP module: knowing that 1HP = 15px, type `10*15` in the Width edit box.
  Module height is _always_ `380 px`.

- **Copy/paste colors**.

  Copy/paste the hex code in the color dialog so your colors are exact and consistent.

- **Use the Align tool**

  The Align tab has gotten really good for aligning and distributing items.
  I consistently use the "Last selected" option for most aligning.
  The lets me mentally "Align this (or these three things) with _that_".
  Always using the same alignment option makes the workflow smooth because you don't have to re-think what going to happen.

- **Make re-usable SVG components**.

  To do this, I work in a scratch SVG in Inkscape, then copy/paste in a text editor.

  1. Create the design.
  2. Move it to 0,0 by entering x,y coordinates the toolbar.
  3. Ungroup+Group to eliminate most transforms.
  4. In a text editor, add a `transform="translate(x, y)"` attribute on the new group for positioning the element.

  Now I have a unit I can copy/paste into panels and other graphics.
  After pasting, I can position precisely by changing the x,y coordinate in the transform.

| | |
|--|--|
| ![pachde (#d) logo](./assets/Logo.svg) | Copyright © Paul Chase Dempsey |
