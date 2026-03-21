# Tabellone
Bingo (FMX)

Clone the repository using 'recurse-submodule'

```
git clone --recurse-submodules https://github.com/gcardi/Tabellone.git
```

The control panel:

<img src="docs/assets/images/Bingo-0.png" alt="Control Panel" border="0">

The board:

<img src="docs/assets/images/Bingo-1.gif" alt="Board Panel" border="0">

## Project Structure

### Main Components

```
Tabellone/
├── 📋 Forms & UI Components
│   ├── FMXFormAppMain.*      # Main application window
│   ├── FMXFormPanelBase.*    # Base panel form
│   ├── FMXWinDisplayDev.*    # Display device window
│   ├── FormMain.*            # Main control form
│   ├── FormPanel.*           # Panel form
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
│   └── Resources/            # Additional resources
│
├── 📚 Dependencies
│   └── Anafestica/           # Configuration library submodule
│       ├── anafestica/       # Main library headers
│       ├── App/              # Example applications (VCL, FMX)
│       ├── Demo/             # Demo projects
│       └── Test/             # Unit tests
│
├── 📖 Documentation
│   └── docs/                 # Documentation and assets
│       └── assets/           # Images and styles
│
├── 🔧 Build Artifacts
│   ├── Win64/                # 64-bit build output
│   │   ├── Debug/
│   │   └── Release/
│   └── Win64x/               # Extended 64-bit build
│
├── ⚙️ Project Files
│   ├── Tombola.cbproj        # C++ Builder project file
│   └── ProjectGroup1.groupproj # Project group
│
└── 📄 Configuration
    ├── Tombola.twopts        # IDE options
    └── LICENSE               # License file
```

### Key Features

- **FMX-based UI**: Built with FireMonkey for cross-platform development
- **Bingo Game**: Complete bingo management application with control panel and display board
- **Command-line Interface**: Support for command-line arguments and options
- **Configuration Management**: Integrated Anafestica library for settings management
- **System Tray Integration**: Runs in system tray for convenience
- **Multi-form Architecture**: Separated concerns with dedicated forms for display, panel, and configuration

### Technologies

- **Language**: C++
- **Framework**: FireMonkey (FMX)
- **IDE**: Embarcadero C++ Builder
- **Configuration Library**: Anafestica (included as submodule)
- **Build Target**: Windows 64-bit

### Building

The project requires Embarcadero C++ Builder with FireMonkey support. Build configurations are available for:
- Debug builds: `Win64/Debug/`
- Release builds: `Win64/Release/`

To build the project, open `Tombola.cbproj` in C++ Builder and compile using your preferred configuration.

