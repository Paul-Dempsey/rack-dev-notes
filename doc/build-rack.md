# Build Rack

As you get into making more complex modules or want to understand more how Rack works and how it’s widgets work, I find it’s very useful to build Rack from source.

I make the necessary modifications to the Rack sources to build it with optimizations off for easier debugging.
Unfortunate that Rack hasn't wired their build to offer debug and release modes.

Building and running your plugin within full Rack source can be faster to iterate because you don’t have to build and copy the deployment package (.vcvplugin).
This also makes Rack source local and easy to search, navigate, and step through under a debugger.

> **#d TODO** step-by step on cloning Rack, getting everything to build, and modifying it to make debug builds.

| | |
|--|--|
| ![pachde (#d) logo](./assets/Logo.svg) | Copyright © Paul Chase Dempsey |
