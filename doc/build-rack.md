# Build Rack

As you get into making more complex modules or want to understand more how Rack works and how it’s widgets work, I find it’s very useful to build Rack from source.

I make the necessary modifications to the Rack sources to build it with optimizations off for easier debugging.
Unfortunate that Rack hasn't wired their build to offer debug and release modes.

Building and running your plugin within full Rack source can be faster to iterate because you don’t have to build and copy the deployment package (.vcvplugin).
This also makes Rack source local and easy to search, navigate, and step through in the debugger.

You will have two versions of Rack on your machine: the retail VCV Rack (Pro or Free), and your dev build of Rack.

## File names and paths

In these dev notes, you will see both Windows-style paths such as `g:\repos\Rack`, and Unix-style paths `/g/repos/Rack` (especially in MINGW64 shell examples). Both refer to the same place.

When creating files and referring to them in your plugin, you must use a consistent filename case in your code and makefile and in the actual path on disk or in git.
On Mac and Windows a difference in case won't matter, but Linux is a case-sensitive file system.
This can cause files to not be found at runtime when loading a resource or when compiling a source file on a Linux host.

## Preparing your file system

Regarding the Rack advice to disable antivirus, this really applies to 3rd-party AV.
The 3rd-party antivirus programs for Windows are generally bad and you should just uninstall them.
The built-in Windows Defender is as good as any other, included in Windows, and all you need.
I don't recommend turning off antivirus completely, unless you are particularly diligent and confident about what you download and install.

Antivirus, including Defender, can interfere with building so you should exclude all your development folders from scanning, no matter what AV you use.

Development folders should never be in OneDrive. That is, not in your "Documents" folder.
Like antivirus, OneDrive can interfere with builds.
Git with GitHub or similar git hosting is all you need.

## Setting up your development environment

You must read and follow [VCV Rack: Setting up your development environment](https://vcvrack.com/manual/Building#Setting-up-your-development-environment) _precisely_.
When it says "restart the shell" after running `pacman -Syu`, you really, _really_ **must** start a new shell for the next step.
Close the window and open a new one before installing the development packages.

Others have run into trouble (Rack crashes) starting from an existing MINGW64 install.
This is usually solved by updating packages (`pacman -Syu` again), or reinstalling MSYS2.

I use two MINGW64 shells tabbed in Windows Terminal.
One shell is for building and running the dev build of Rack.
The other shell is for building the plugin.
You can also use the shortcut for MINGW54 shell installed by MSYS2.

Here is the JSON snippet I use for Windows Terminal settings to use a MING64 bash shell.
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

Once your tools are installed, we can clone and build Rack.

## Cloning VCV Rack

Once you've decided where your development projects live, you can clone Rack.
You can put yours anywhere you have a generous amount of fast disk space available, not under OneDrive, and no spaces in the path.
All my development projects are in `g:\repos`, so that's what you'll see in all these dev notes.

To pepare for cloning, set up something similar, open the MINGW64 shell, and cd to the parent folder of where you want Rack.
For me that's `g:\repos`, and then follow the Rack guide.
[VCV Rack: Building Rack](https://vcvrack.com/manual/Building#Building-Rack) instructions are accurate and don't need any other supplementation.

## Building Rack

If you successfully completed the Rack docs including the `make run`, you should now be looking at a running VCV Rack Free.
If this didn't work for you, get on the forums and ask for help.

## Running Rack

I like to see the Rack log in the terminal.
In the MINGW64 Terminal tab for Rack in the `g:/repos/Rack` folder, I run

```shell
./Rack -d
```

Using `-d` directs logging to the terminal, instead of the log file, and sets the user folder to our dev build of Rack.

## Other modules

This is all modular, so you it's more useful to have other modules available.
Cloning other open source modules and building them is a great reference to see how things work.
At the bare minimum, I recommend cloning _Fundamental_.

In your MINGW64 shell, `cd` to the `plugins` folder under the Rack and then follow [VCV Rack: Building Plugins](https://vcvrack.com/manual/Building#Building-Rack-plugins) up to the `make` step.

You can do the same for other open-source modules you may be interested in.
Open modules will have a link in their module right-click menu under _Info_ / _Source code_ which will take you directly to that project's repository.

Once you have some plugin source, you can build them all in one swoop using:

```shell
make plugins
```

The Rack instructions show running `make install`, which will make the distribution package and install the plugin into your retail Rack install.
When you're first getting started, you really don't need to install it to your retail Rack.
The fastest iteration on a plugin is all done right in the dev build of Rack.

Of course, when your plugin is close to ready for publishing to the library, use `make install` to make sure it plays well with the retail Rack and other modules in your library.

## Set up VSCode workspace for Rack

I use VSCode for editing and debugging. You can get it from [Download VSCode](https://code.visualstudio.com/download). Since we're writing C++, you'll need at least the _Microsoft C/C++_ extension for syntax highlighting, "intellisense", and debugger.

To set up the workspace

1. Start a new VSCode window.
1. Click _Open Folder..._
1. Choose the `g:\repos\Rack` folder (or wherever you've put _your_ Rack source).
1. Choose _File_ / _Save Workspace As..._, and accept the default.

This should create a `Rack\.vscode` folder containing a `Rack.code-workspace` file.

Once you have a workspace, I like to add each plugin I'm interested in.
Choose _File_ / _Add Folder to Workspace_, and choose the plugin's folder (under `Rack/plugins`).

## Modify Rack source for making a "dev build"

Part of the reason we're building Rack from source is so that we can step through it under the debugger.
The debugging experience works best if we turn off optimizations.

Unfortunately, Rack doesn't provide this out-of-the-box, so we need to make a couple of tweaks.
It would be possible to add the necessary conditional code and macro definitions to enable building both dev and "retail" versions.
But we don't really need that because we've already installed the official retail version of Rack downloaded from the VCV Rack site.

## turn off optimization

In `Rack/compile.mk`, comment out the line under `# Optimization`.
Then add an uncommented line that turns optimization off.
The section should end up looking like this:

```makefile
# Optimization
#FLAGS += -O3 -funsafe-math-optimizations -fno-omit-frame-pointer
FLAGS += -O0 -funsafe-math-optimizations -fno-omit-frame-pointer
```

## fix blendish to build without errors with optimization off

In `Rack\dep\oui-blendish\blendish.c`, around line 64, we again comment out an existing line and add an altered one.
The resulting code looks like this:

```cpp
#else
    //#define BND_INLINE inline
    #define BND_INLINE static
```

At this point we can build an updated rack but first we'll make the changes needed for the Rack log to appear in the VSCode Terminal window while we debug.

## change the subsystem to console to see console logs

This configuration is needed by the VSCode debugging configuration described in the [Debugging](./debug.md#debugging) dev note.

In `Rack\Makefile` (around line 94) change the linker flag from Windows subsystem to Console subsystem:

```makefile
ifdef ARCH_WIN
	STANDALONE_TARGET := Rack.exe
#	STANDALONE_LDFLAGS += -mwindows
	STANDALONE_LDFLAGS += -mconsole
```

## make a clean build of everything

Since we've changed how Rack builds, including a dependency, we'll do a clean build of everything.

1. `make cleandep` to clean dependencies.
1. `make clean` to clean Rack itself.
1. `make dep` to rebuild dependencies.
1. `make` to build rack.
1. `make CMD="make clean" plugins` to run the `clean` target of each plugin.
1. `make plugins` to rebuild all plugins.

## Begin your plugin

Up to know, we've only set up a full Rack dev build with at least one other plugin.
Now it's time to get started on _your_ plugin.

I don't recommend following the Rack tutorial.
Instead, I recommend starting from my GenericBlank template.
See [Use GenericBlank](./use-generic-blank.md#use-genericblank).

## New Rack release

When a new release of Rack comes out, I always sync to the matching source and rebuild.
From the Rack folder in the MINGW64 shell:

1. `make cleandep` to clean dependencies.
1. `make clean` to clean Rack itself.
1. `git stash` to save debug modifications.
1. `git pull` to retrieve latest sources.
1. `git submodule update --init --recursive` to get any dependency updates.
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
