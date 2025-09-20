# Debugging

This note describes how I'm set up for Rack development and debugging.
It's specific to my personal main Rack dev environment, which is:

- Windows.

  It's what's comfortable for me, having had a long career working at Microsoft.
  I also have Mac and Linux boxes for testing and learning those systems, but my main machine is a fairly hefty Windows desktop running two 4k monitors.

- Multiple monitors makes debugging a breeze because you can run the application on one monitor and the debugger on the other.
This is a standard industry setup for a developer. These days there are low-cost portable second monitors for laptops.

- Editing and debugging is in VSCode. My VScode configuration is in the next section of this dev note.

- I build Rack from source, with minor modifications for easier debugging.

- I use two MINGW64 MSYS2 bash shells running as tabs in Windows Terminal on the left monitor.
  One shell is for building and running the dev build of Rack.
  The other shell is for building the plugin.

  VSCode is on the right monitor.

  At one time I built and ran from within VSCode using VSCode tasks, but stopped doing that because it was too much trouble to set up and maintain for multiple plugins.
  I found working only within the VSCode app was too constraining.

  If on a laptop/single monitor configuration, building in VSCode could have advantages, and setting up the tasks is not difficult once you get the idea of how VSCode task configuration works.

**See also**

- [Build Rack](./build-rack.md#build-rack) for making a debug build of Rack.
- Microsoft [VSCode Debugging](https://code.visualstudio.com/docs/debugtest/debugging).
- Microsoft [VSCode tasks](https://code.visualstudio.com/docs/debugtest/tasks).

## Non-SDK file layout

My non-SDK file layout building Rack from source, and including my plugins:

```console
g:/repos/Rack
   .vscode
      launch.json
      Rack.code-workspace
   (Rack folders dep, docs, include, src, res,... )
   plugins
      Fundamental
      pachde*
      (Other open-source plugins ...)
```

All dev work is in folders that are _not_ sync'd to OneDrive. Backup and access on other machines is via git+GitHub.

I'm using a VSCode workspace rooted in the `Rack` folder.

As shown, the debugging configuration (`launch.json`) is in the application (Rack), and not the plugin.

For each plugin I want to work on, I add its folder to the Workspace.
This makes it convenient to use VSCode's integrated Git, and makes it much easier to navigate in the VSCode _Explorer_ window.

## VSCode configuration for debugging Rack

Debugging is configured in `launch.json`.

I have two debugging configurations in the file:

- `"name": "(gdb) Launch"` for regular debugging with my dev build of Rack.
- `"name": "(gdb) Launch retail"`for debugging my installed retail Rack.

```jsonc
{
    // Use IntelliSense to learn about possible attributes.
    // Hover to view descriptions of existing attributes.
    // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
{
    "name": "(gdb) Launch",
    "type": "cppdbg",
    "request": "launch",
    "program": "${workspaceFolder}/Rack.exe",
    "args": ["-d"],
    "stopAtEntry": true,
    "cwd": "${workspaceFolder}",
    "environment": [
        {"name": "RACK_DIR", "value":"${workspaceFolder}"}
    ],
    "externalConsole": false,
    "logging": {
        "exceptions": true,
        "moduleLoad": false,
        "programOutput": true,
        "engineLogging": false,
        "trace": false,
        "traceResponse": false
    },
    "filterStderr": true,
    "MIMode": "gdb",
    "miDebuggerPath": "C:/msys64/mingw64/bin/gdb.exe",
    "setupCommands": [
        {
            "description": "Enable pretty-printing for gdb",
            "text": "-enable-pretty-printing",
            "ignoreFailures": true
        },
        {
            "description": "Set Disassembly Flavor to Intel",
            "text": "-gdb-set disassembly-flavor intel",
            "ignoreFailures": true
        },
        {
            "description": "Set UTF-8 charset",
            "text": "set charset UTF-8",
        }
    ]
},
{
    "name": "(gdb) Launch retail",
    "type": "cppdbg",
    "request": "launch",
    "program": "C:/Program Files/VCV/Rack2Free/Rack.exe",
    "args": [],
    "stopAtEntry": true,
    "cwd": "${workspaceFolder}",
    "environment": [ {"name": "RACK_DIR", "value":"C:/Program Files/VCV/Rack2Free"} ],
    "externalConsole": false,
    "logging": {
        "exceptions": true,
        "moduleLoad": false,
        "programOutput": true,
        "engineLogging": false,
        "trace": false,
        "traceResponse": false
    },
    "filterStderr": true,
    "MIMode": "gdb",
    "miDebuggerPath": "C:/msys64/mingw64/bin/gdb.exe",
    "setupCommands": [
        {
            "description": "Point to my sources",
            "text": "-gdb-set directories G:/repos/Rack",
        },
        {
            "description": "Enable pretty-printing for gdb",
            "text": "-enable-pretty-printing",
            "ignoreFailures": true
        },
        {
            "description": "Set Disassembly Flavor to Intel",
            "text": "-gdb-set disassembly-flavor intel",
            "ignoreFailures": true
        },
        {
            "description": "Set UTF-8 charset",
            "text": "set charset UTF-8",
        }
    ]
},
    ]
}
```

- This is using MINGW64's GDB debugger.

- `-d` on the command line to see the Rack log in the VSCode _Terminal_ window.
This requires a custom build of Rack that sets `STANDALONE_LDFLAGS += -mconsole` linker flag instead of `-mwindows`.
If you don't mind just using the log file, you can omit `-d` and the custom link flags.

- `RACK_DIR` is set to `"${workspaceFolder}`

- `"stopAtEntry": true,`

  It's a little annoying to have to stop at Rack's entry every time we debug,
but it appears to be necessary in order for breakpoints set in a plugin to work.

- The retail launch configuration points back to my Rack dev build for source.
A new release of Rack requires a clean rebuild of Rack, its deps, and all the plugins you're building.
See my [Build Rack](./build-rack.md#build-rack) dev note for the step-by-step on that.

---

> **#d TODO** Write up Mac and Linux dev envionments.

| | |
|--|--|
| ![pachde (#d) logo](./assets/Logo.svg) | Copyright © Paul Chase Dempsey |
