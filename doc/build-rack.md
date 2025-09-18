# Build Rack

As you get into making more complex modules or want to understand more how Rack works and how it’s widgets work, I find it’s very useful to build Rack from source.

I make the necessary modifications to the Rack sources to build it with optimizations off for easier debugging.
Unfortunate that Rack hasn't wired their build to offer debug and release modes.

Building and running your plugin within full Rack source can be faster to iterate because you don’t have to build and copy the deployment package (.vcvplugin).
This also makes Rack source local and easy to search, navigate, and step through in the debugger.

> **#d TODO** step-by step on cloning Rack, getting everything to build, and modifying it to make debug builds. .gitattributes (line ending control), .gitignore.

## New Rack release

When a new release of Rack comes out, I always sync to the matching source and rebuild.
From the Rack folder in the MINGW64 shell:

1. `make cleandep` to clean dependencies.
1. `make clean` to clean Rack itself.
1. `git stash` to save debug modifications.
1. `git pull` to retreive latest sources.
1. `git stash pop` to restore debug modifications.
1. `make dep` to rebuild dependencies.
1. `make` to build rack.
1. Clean and rebuild plugins.

   1. There is no direct support for cleaning plugins, probably because it can't assume that every plugin's makefile has a `clean` target.
As it happens, all the plugins I've cloned _do_ have a `clean` target, so I can use

      ```shell
      make CMD="make clean" plugins`
      ```

   1. `make plugins`

Now I'm all synced, and I can resume normal plugin work.

| | |
|--|--|
| ![pachde (#d) logo](./assets/Logo.svg) | Copyright © Paul Chase Dempsey |
