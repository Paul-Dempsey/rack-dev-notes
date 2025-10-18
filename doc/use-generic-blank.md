# Use GenericBlank

Count me as one that doesn't like VCV's `Helper.py`.
I find the code-generation approach inflexible and error-prone. Many beginners quickly run into difficulties because the script operates on pattern matching and not an understanding of SVG. Units, transforms, and variations on the color representation all trip up users when they modify the panel. If you don't already know Python, it can be hard to figure out what is tripping up the script, much less how to deal with it from your SVG editor.

Maintaining a panel and your code is also not sustainable with en evolving plugin. Suffice to say that I disliked the Rack tutorial approach enough that I created an alternative: **GenericBlank**.

## Introducing GenericBlank

**GenericBlank** is a replacement for the Rack tutorial code that I think gets beginners to a much better place for further development.
It gives you a working module immediately (if you've set up your development environment correctly).

In the forums, we've seen many beginners get stuck almost immediately just getting the plugin to build and a module to load and display correctly in Rack.

By working I mean:

1. Builds
2. Loads and displays in Rack

It also doesn't contain anything extra that you need to remove for your plugin. It's a blank canvas for you to work from.

Not only do you get a ready-to-run starting point, but when you use it as a GitHub template, you are automatically set up with a Github project and the GitHub action to build your plugin for all the supported Rack platforms -- Mac, Linux, and Windows.

See [GenericBlank: a Template for a VCV Rack Blank module](https://github.com/Paul-Dempsey/GenericBlank#generic-blank). The Readme contains complete instructions for getting up and running, so I won't repeat that here.

I've been using it as the starting template for prototyping and creating new plugins.

> **In the works** — _GenericBlank.next_: Next steps to turn the blank module into one with working widgets and maybe some DSP.

| | |
|--|--|
| ![pachde (#d) logo](./assets/Logo.svg) | Copyright © Paul Chase Dempsey |
