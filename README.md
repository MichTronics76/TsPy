# TsPy - TeamSpeak 3 Plugin with Python Scripting

A professional TeamSpeak 3 client plugin with embedded Python scripting engine, audio monitoring, and event system.

## 🎯 Overview

TsPy is a feature-rich TeamSpeak 3 plugin that brings Python scripting capabilities to your TeamSpeak client. Write custom scripts to automate tasks, respond to events, and create interactive audio monitoring tools.

### Key Features

- 🐍 **Python 3.10+ Integration** - Full embedded Python interpreter with custom `ts3api` module
- 🎤 **Audio Monitoring** - Real-time audio level visualization with PyGame UI
- 📡 **Event System** - React to TeamSpeak events (connections, messages, talk status, etc.)
- ⚙️ **Command System** - Extensible command handling with `/tspy` commands
- 🏗️ **Modern Architecture** - Clean, modular C11 codebase with CMake build system
- 📊 **Comprehensive Logging** - Professional logging system for debugging

## 📋 Requirements

### Build Requirements
- **CMake** 3.20 or higher
- **Visual Studio 2022** (MSVC 19.39+)
- **TeamSpeak 3 Client** (API version 26)
- **Python 3.10+** (development libraries)

### Runtime Requirements
- **TeamSpeak 3 Client**
- **Python 3.10+** (installed on system)
- **PyGame** (for audio monitor): `pip install pygame`

## 🚀 Quick Start

### Building the Plugin

```powershell
# Clone or navigate to the project directory
cd E:\Projects\HamRadio\TsPy

# Configure CMake (first time only)
cmake -S . -B build -G "Visual Studio 17 2022" -A x64

# Build the plugin
cmake --build build --config Release

# Install to TeamSpeak plugins folder
.\install.ps1
```

### Using the Plugin

1. **Start TeamSpeak** - The plugin loads automatically
2. **Check status**: `/tspy status` in chat
3. **Load a script**: `/tspy python load <script_name>`
4. **Try audio monitor**: `/tspy python load audio_monitor`

## 🐍 Python Scripting

### Available API Functions

The `ts3api` module provides these functions:

```python
import ts3api

# Information
client_id = ts3api.get_client_id(server_id)
name = ts3api.get_client_name(server_id, client_id)

# Messaging
ts3api.print_message("Hello TeamSpeak!")
ts3api.send_channel_message(server_id, "Hello channel!")
ts3api.send_server_message(server_id, "Hello server!")

# Audio (v1.5.0+)
level = ts3api.get_audio_level(server_id)  # Returns dB (-60 to 0)
ts3api.start_recording(server_id)
ts3api.stop_recording(server_id)

# Logging
ts3api.log("Debug message", level=0)  # 0=INFO, 1=WARNING, 2=ERROR
```

### Event Handlers

Your scripts can implement these event handlers:

```python
def on_connect(server_id, client_id):
    """Called when you connect to a server"""
    print(f"Connected to server {server_id}")

def on_disconnect(server_id):
    """Called when you disconnect from a server"""
    print(f"Disconnected from server {server_id}")

def on_client_move(server_id, client_id, old_channel, new_channel):
    """Called when a client moves channels"""
    print(f"Client {client_id} moved to channel {new_channel}")

def on_text_message(server_id, from_id, to_id, target_mode, message):
    """Called when a text message is received"""
    print(f"Message from {from_id}: {message}")

def on_talk_status_change(server_id, status, client_id):
    """Called when someone starts/stops talking"""
    if status == 1:  # STATUS_TALKING
        print(f"Client {client_id} started talking")
```

### Example Scripts

#### Simple Greeter
```python
# greeter.py
import ts3api

def on_connect(server_id, client_id):
    ts3api.send_channel_message(server_id, "Hello everyone! 👋")
```

#### Echo Bot
```python
# echobot.py
import ts3api

def on_text_message(server_id, from_id, to_id, target_mode, message):
    if not "(auto-reply from Python)" in message:
        reply = f"Echo: {message} (auto-reply from Python)"
        ts3api.send_channel_message(server_id, reply)
```

## 🎤 Audio Monitor

The included `audio_monitor.py` script provides a real-time audio monitoring UI:

### Features
- **VU Meter** - Real-time microphone level visualization (-60dB to 0dB)
- **Color Coding** - Green (safe), Yellow (loud), Red (clipping)
- **Recording Control** - Click button to start/stop voice recording
- **Talk Status** - Live list of currently talking clients with indicators

### Usage
```
/tspy python load audio_monitor
```

A PyGame window opens showing real-time audio monitoring. Press ESC or close the window to exit.

### Requirements
```powershell
pip install pygame
```

## ⚙️ Commands

| Command | Description |
|---------|-------------|
| `/tspy help` | Show available commands |
| `/tspy status` | Show plugin status |
| `/tspy python status` | Show Python engine status |
| `/tspy python load <script>` | Load a Python script |
| `/tspy python reload <script>` | Reload a running script |
| `/tspy python unload <script>` | Unload a script |

## 📁 Project Structure

```
TsPy/
├── CMakeLists.txt                 # Build configuration
├── CMakePresets.json              # VS2022 presets
├── README.md                      # This file
│
├── include/                       # TeamSpeak SDK headers
│   ├── ts3_functions.h
│   ├── plugin_definitions.h
│   └── teamspeak/
│
├── src/                           # Source code
│   ├── core/                      # Core plugin
│   │   ├── plugin_main.c/h
│   │   ├── plugin_interface.c/h
│   │   └── plugin_config.c/h
│   │
│   ├── commands/                  # Command system
│   │   ├── command_handler.c/h
│   │   └── python_commands.c/h
│   │
│   ├── events/                    # Event handlers
│   │   ├── connection_events.c/h
│   │   ├── channel_events.c/h
│   │   └── client_events.c/h
│   │
│   ├── python/                    # Python engine
│   │   ├── python_engine.c/h
│   │   ├── python_api.c/h
│   │   └── python_events.c/h
│   │
│   ├── ui/                        # User interface
│   │   ├── menu_handler.c/h
│   │   └── hotkey_handler.c/h
│   │
│   └── utils/                     # Utilities
│       ├── logging.c/h
│       └── string_utils.c/h
│
├── scripts/                       # Python scripts location
│   ├── tspy_init.py              # Auto-loaded on startup
│   └── audio_monitor.py          # Audio monitoring UI
│
└── resources/                     # Resources
    └── icons/                     # Plugin icons
```

## 🔧 Building from Source

### 1. Prerequisites

Install the required tools:
- Visual Studio 2022 with C++ desktop development workload
- CMake 3.20+ ([cmake.org](https://cmake.org/download/))
- Python 3.10+ with development libraries ([python.org](https://www.python.org/downloads/))

### 2. Configure CMake

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
```

CMake will automatically find your Python installation.

### 3. Build

```powershell
# Release build (recommended)
cmake --build build --config Release

# Debug build (for development)
cmake --build build --config Debug
```

### 4. Install

```powershell
# Automatic installation
.\install.ps1

# Manual installation
Copy-Item "build\bin\Release\tspy_plugin.dll" "$env:APPDATA\TS3Client\plugins\"
```

### 5. Verify

1. Start TeamSpeak
2. Go to **Plugins** menu
3. Look for "TsPy Plugin" with Python version in description
4. Type `/tspy status` in any chat

## 🐛 Troubleshooting

### Plugin doesn't load
- Make sure TeamSpeak is completely closed before installing
- Check the TeamSpeak client log: `%APPDATA%\TS3Client\logs\`
- Verify Python 3.10+ is installed and in PATH

### Python scripts don't work
- Verify Python engine status: `/tspy python status`
- Check plugin log: `%APPDATA%\TS3Client\plugins\tspy_plugin.log`
- Ensure scripts are in: `%APPDATA%\TS3Client\plugins\scripts\`

### Audio monitor issues
- Install pygame: `pip install pygame`
- Check microphone permissions in TeamSpeak
- Verify you're connected to a server

### Build errors
- Ensure Visual Studio 2022 is installed
- Update CMake to latest version
- Verify Python development libraries are installed
- Check Python version: `python --version`

## 📊 Version History

### v1.5.0 (Current) - Audio Monitoring
- ✅ Audio API: get_audio_level, start_recording, stop_recording
- ✅ Talk status events for real-time speaker detection
- ✅ PyGame audio monitor with VU meter and recording control
- ✅ Color-coded visualization of audio levels

### v1.4.0 - Event System Complete
- ✅ Full event dispatcher for Python scripts
- ✅ 6 event types: connect, disconnect, client_move, text_message, talk_status
- ✅ Proper DLL export of TeamSpeak callbacks
- ✅ Loop prevention for bot messages

### v1.3.1 - Python Commands
- ✅ `/tspy python` command system
- ✅ Script loading/reloading/unloading
- ✅ Python engine status reporting

### v1.3.0 - Python Integration
- ✅ Embedded Python 3.10+ interpreter
- ✅ Custom ts3api module with TeamSpeak functions
- ✅ Event callback system for Python scripts
- ✅ Auto-loading of tspy_init.py

### v1.2.0 - Modular Rewrite
- ✅ CMake build system with VS2022 support
- ✅ Modular architecture (28 files)
- ✅ Professional logging system
- ✅ Command and event handling

### v1.0.0 - Initial Release
- ✅ Basic TeamSpeak plugin functionality
- ✅ Menu and hotkey support

## 📝 License

This project is provided as-is for use with TeamSpeak 3. See TeamSpeak's plugin development guidelines for usage terms.

## 🤝 Contributing

This is a personal project, but suggestions and improvements are welcome!

## 📧 Support

For issues or questions:
1. Check the Troubleshooting section above
2. Review the plugin log file
3. Check TeamSpeak client logs

## 🔗 Resources

- [TeamSpeak 3 Client Plugin SDK](https://github.com/TeamSpeak-Systems/ts3_sdk_client)
- [Python Documentation](https://docs.python.org/3/)
- [CMake Documentation](https://cmake.org/documentation/)
- [PyGame Documentation](https://www.pygame.org/docs/)

---

**Made with ❤️ for the TeamSpeak and Ham Radio communities**
