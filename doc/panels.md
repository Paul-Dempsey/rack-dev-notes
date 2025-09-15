# Panels

My usual workflow for a new module is to prototype the panel design before I even start coding a module.
Sometimes I will also write a draft of the documentation before coding.
This helps me work out whether the design is explicable and makes sense.
Other devs do it the other way around: they don't worry much about the panel design until after they have a working module, and sometimes never get around to documentation, much to the dismay of some users.

I have a collection of stock SVG components (knobs, sliders, ports) that I can import or copy/paste that get removed for the actual running panel.
After I have this initial design up and running in a Rack module, there is a bit of transitional Inkscape process to eliminate transforms (ungroup everything that is grouped, then re-group movable units).
After that I hand-edit the SVGs as text.
I edit out all traces of inkscape.
All changes after that are in the VSCode text editor, with svg text / preview side-by-side.

Using a text editor is easier for me to ensure consistent colors, stroke widths, and positioning across modules.
I can copy/paste, then adjust coordinates and dimensions.

I have a mechanism to hot-reload the svgs in a module (including widgets).
This makes it quick and easy to make fine-grained adjustments.

> **#d TODO**: Backport from pachde-CHEM to svg_theme the many improvements, including hot-reload.

## Inkscape tips

- Use px coordinates.
  Ignore the Rack guide that says to use mm SVG units. Rack itself doesn't consistently follow it's own guide here.

- Set up your SVG document like this: the most import items to set are circled (most of the others are per your preference).

  ![Inkscape document setup for VCV Rack module panel](./assets/ink-doc-setup.png)

  Now all your widget positioning code works in the same units that the Rack display system uses without `MMTOPX` everywhere. This allows for precise exact pixel positioning with a minimum of artifacts.

- Inkscape supports expressions in coordinate edit boxes. For example, if you want a 10U module: 1U = 15px, so type `10*15` in the Width edit box. (Module height is _always_ `380 px`).

- For colors, copy/paste the hex code, so your colors are exact and consistent.

- Prefer editing x,y,w,h in the toolbar over dragging with the mouse.

- The Align tab has gotten really good for aligning and distributing items. I consistently use "Last selected". The lets me mentally "Align this (or these three things) with _that_". Always using the same alignment option makes the workflow smooth because you don't have to re-think what going to happen.

- For re-usable SVG components, work in a scratch SVG in Inkscape.

  1. Create the design
  2. Move it to 0,0
  3. Ungroup/regroup to eliminate transforms.
  4. Add a `transform="translate(x, y)"` on the new group for positioning.

  Now I have a unit I can paste into panels.

| | |
|--|--|
| ![pachde (#d) logo](./assets/Logo.svg) | Copyright © Paul Chase Dempsey |
