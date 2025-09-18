# Build Rack

As you get into making more complex modules or want to understand more how Rack works and how it’s widgets work, I find it’s very useful to build Rack from source.

I make the necessary modifications to the Rack sources to build it with optimizations off for easier debugging.
Unfortunate that Rack hasn't wired their build to offer debug and release modes.

Building and running your plugin within full Rack source can be faster to iterate because you don’t have to build and copy the deployment package (.vcvplugin).
This also makes Rack source local and easy to search, navigate, and step through in the debugger.

## Setting up your development environment

Regarding the Rack advice to disable antivirus, this really applies to 3rd-party AV.
The 3rd-party AV programs for Windows are generally bad and you should just uninstall them.
The built-in Windows Defender is good and all you need.
However, like any AV, Defender can interfere with building, so you should exclude all your development folders.

Development folders should never be in OneDrive. That is, not in your "Documents" folder.
Like antivirus, OneDrive can interfere with builds.
Git with GitHub or similar git hosting is all you need.

You must read and follow the [Rack instructions](https://vcvrack.com/manual/Building#Setting-up-your-development-environment) _precisely_.
When it says "restart the shell" after running `pacman -Syu`, you really, _really_ **must** start a new shell for the next step.
Close the window and open a new one before installing the development packages.

I use two MINGW64 shells tabbed in Windows Terminal.
One shell is for building and running the dev build of Rack.
The other shell is for building the plugin.
See _Windows Terminal profile for Rack_ below for a profile you can copy/paste in Windows Terminal.
You can also use the shortcut for MINGW54 shell installed by MSYS2.

Once your tools are installed, we can clone and build Rack.

> **#d TODO** step-by step on cloning Rack, getting everything to build, and modifying it to make debug builds. .gitattributes (line ending control), .gitignore.

## Windows Terminal profile for Rack

Here is the JSON snippet I use for Windows Terminal.
Open Terminal's `settings.json` and add this profile to the `"profiles"` object `"list"` array.

```json
{
 "commandline": "C:/msys64/msys2_shell.cmd -defterm -here -no-start -mingw64 -shell bash",
 "guid": "{9e41f090-2700-48ab-8de2-1f7ddee7f30f}",
 "hidden": false,
 "icon": "C:\\msys64\\mingw64.ico",
 "name": "Rack MSYS2-MING64",
 "startingDirectory": "G:\\Repos\\Rack"
},
```

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
