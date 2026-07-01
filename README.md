# Tabellone
Bingo (FMX)

```
git clone https://github.com/gcardi/Tabellone.git
```

> **Note:** the [Anafestica](https://github.com/gcardi/Anafestica.git) configuration
> library is no longer bundled as a submodule. It is now an **external
> prerequisite** that must be installed before building — see
> [Prerequisites](#prerequisites) below.

The control panel:

<img src="docs/assets/images/Bingo-0.png" alt="Control Panel" border="0">

The board:

<img src="docs/assets/images/Bingo-1.gif" alt="Board Panel" border="0">

## Prerequisites

- **Embarcadero C++ Builder** (RAD Studio 12.x) with FireMonkey support.
- **[Anafestica](https://github.com/gcardi/Anafestica.git)** configuration library.

### Installing Anafestica

Anafestica is required to compile this project. Download it and install it under
`$(BDSCOMMONDIR)` following the instructions in its own repository:

```
git clone https://github.com/gcardi/Anafestica.git
```

Copy/deploy the library into `$(BDSCOMMONDIR)` (the RAD Studio common directory,
e.g. `C:\Users\Public\Documents\Embarcadero\Studio\<version>`) so that its
include and library paths are resolved by the IDE. The project no longer carries
`Anafestica\...` include/library paths inside `Tombola.cbproj`; the library is
expected to be reachable through the global RAD Studio search paths.

## Project Structure

### Main Components

```
Tabellone/
├── 📋 Forms & UI Components
│   ├── FMXFormAppMain.*      # Main application window
│   ├── FMXFormPanelBase.*    # Base panel form
│   ├── FMXWinDisplayDev.*    # Display device window
│   ├── FormMain.*            # Main control form
│   ├── FormPanel.*           # Panel form (board + configurable logo)
│   ├── FormConfig.*          # Configuration form
│   ├── FrameNum.*            # Frame for number display
│   └── DataModStyleRes.*     # Style resources data module
│
├── 📦 Core Application Code
│   ├── Tombola.cpp/h         # Main application class
│   ├── AppUtils.cpp/h        # Utility functions
│   ├── CmdLineParser.*       # Command line argument parser
│   ├── CmdLineOptions.*      # Command line options
│   ├── SyncObjsFixed.*       # Synchronization objects
│   ├── TrayIcon.cpp/h        # System tray icon
│   ├── Synchronize.h         # Thread synchronization
│   └── RegexDefs.h           # Regular expression definitions
│
├── 🎨 Resources
│   ├── AllRes.rc             # Resource file
│   └── Resources/            # Additional resources (default logo, etc.)
│
├── 📖 Documentation
│   └── docs/                 # Documentation and assets
│       └── assets/           # Images and styles
│
├── 🔧 Build Artifacts
│   ├── Win64/                # 64-bit build output (classic)
│   │   ├── Debug/
│   │   └── Release/
│   └── Win64x/               # 64-bit modern (clang) build output
│
└── ⚙️ Project Files
    ├── Tombola.cbproj        # C++ Builder project file
    ├── Tombola.twopts        # IDE options
    └── LICENSE               # License file
```

### Key Features

- **FMX-based UI**: Built with FireMonkey.
- **Bingo Game**: Complete bingo management application with control panel and display board.
- **Command-line Interface**: Support for command-line arguments and options (see below).
- **Configurable Logo**: The board logo can be replaced without rebuilding (see below).
- **Configuration Management**: Uses the Anafestica library for settings management.
- **System Tray Integration**: Runs in system tray for convenience.
- **Multi-form Architecture**: Separated concerns with dedicated forms for display, panel, and configuration.

## Command-Line Options

Options are prefixed with `-`. Options that take a value use the form
`-name=value`.

| Option              | Value            | Description                                                                 |
| ------------------- | ---------------- | --------------------------------------------------------------------------- |
| `-help`             | —                | Show the help/usage message and exit.                                       |
| `-logo=<file>`      | full path        | File to use for the board logo instead of the default (see below).          |
| `-session=<name>`   | session name     | Use a separate configuration session, to keep independent settings.         |

Examples:

```
Tombola.exe -help
Tombola.exe -logo=C:\Bingo\CustomLogo.png
Tombola.exe -session=Sala2
```

## Logo Customization

The board logo (`Image1` in `FormPanel`) is resolved at startup with the
following precedence:

1. **`-logo=<file>`** command-line option, if provided and the file can be loaded.
2. **`Logo.png`** located in the same folder as the executable, if present.
3. The **default image embedded** in the form resources (fallback).

If a file does not exist or cannot be loaded (unsupported format, corrupted
file, …) the next source in the list is used. Supported image formats are
**PNG, JPEG/JPG, BMP, GIF and TIFF** (PNG is recommended because it supports
transparency).

## Technologies

- **Language**: C++
- **Framework**: FireMonkey (FMX)
- **IDE**: Embarcadero C++ Builder (RAD Studio 12.x)
- **Configuration Library**: Anafestica (external prerequisite)
- **Build Targets**: Windows 64-bit (`Win64`, `Win64x`)

## Building

The project requires Embarcadero C++ Builder with FireMonkey support and the
[Anafestica](#installing-anafestica) library installed under `$(BDSCOMMONDIR)`.

Build configurations produce their output under:
- Debug builds: `Win64/Debug/` (or `Win64x/Debug/`)
- Release builds: `Win64/Release/` (or `Win64x/Release/`)

To build the project, open `Tombola.cbproj` in C++ Builder and compile using your
preferred configuration.
