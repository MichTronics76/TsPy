# TsPy Example Scripts

This directory contains example Python scripts demonstrating TsPy plugin capabilities.

## 📁 Available Examples

### 🎤 audio_monitor.py
**Real-time Audio Monitoring with PyGame UI**

A comprehensive audio monitoring tool featuring:
- Real-time VU meter (-60dB to 0dB visualization)
- Color-coded audio levels (green/yellow/red)
- Recording control button
- Live list of currently talking clients
- PyGame window at 800x600 @ 30 FPS

**Requirements:**
```bash
pip install pygame
```

**Usage:**
```
/tspy python load audio_monitor
```

**Features:**
- Polls `ts3api.get_audio_level()` every frame for smooth updates
- Click recording button to start/stop voice recording
- Event handler updates talking clients list in real-time
- Press ESC or close window to exit

---

### 👋 auto_greeter.py
**Automatic Welcome Messages**

Sends a friendly greeting when you connect to a server.

**Usage:**
```
/tspy python load auto_greeter
```

**Event Handlers:**
- `on_connect()` - Sends channel message when connected

---

### 📻 callsign_logger.py
**Ham Radio Callsign Logger**

Logs ham radio callsigns from text messages (format: `W1ABC`, `K2XYZ`, etc.).

**Usage:**
```
/tspy python load callsign_logger
```

**Event Handlers:**
- `on_text_message()` - Detects and logs callsigns from messages

**Features:**
- Regex pattern matching for valid callsigns
- Logs to plugin log file with timestamp

---

### 🌍 hello_world.py
**Basic Example**

Minimal example demonstrating basic TsPy functionality.

**Usage:**
```
/tspy python load hello_world
```

**Features:**
- Simple print message on load
- Demonstrates basic ts3api usage

---

## 🔧 How to Use Examples

### 1. Copy to Scripts Directory

Copy any example to your TeamSpeak scripts folder:
```powershell
Copy-Item "examples\audio_monitor.py" "$env:APPDATA\TS3Client\plugins\scripts\"
```

### 2. Load in TeamSpeak

Use the `/tspy python load` command:
```
/tspy python load audio_monitor
```

### 3. Check Status

Verify script is running:
```
/tspy python status
```

### 4. Unload When Done

Stop a running script:
```
/tspy python unload audio_monitor
```

---

## 📝 Creating Your Own Scripts

### Basic Template

```python
import ts3api

def on_connect(server_id, client_id):
    """Called when you connect to a server"""
    ts3api.print_message("Hello from Python!")
    
def on_text_message(server_id, from_id, to_id, target_mode, message):
    """Called when a text message is received"""
    print(f"Received: {message}")
```

### Available Event Handlers

- `on_connect(server_id, client_id)` - Server connection
- `on_disconnect(server_id)` - Server disconnection
- `on_client_move(server_id, client_id, old_channel, new_channel)` - Client movement
- `on_text_message(server_id, from_id, to_id, target_mode, message)` - Text messages
- `on_talk_status_change(server_id, status, client_id)` - Talk status (1=talking, 0=stopped)

### Available API Functions

```python
# Information
client_id = ts3api.get_client_id(server_id)
name = ts3api.get_client_name(server_id, client_id)

# Messaging
ts3api.print_message("Text")
ts3api.send_channel_message(server_id, "Text")
ts3api.send_server_message(server_id, "Text")

# Audio
level = ts3api.get_audio_level(server_id)  # Returns dB
ts3api.start_recording(server_id)
ts3api.stop_recording(server_id)

# Logging
ts3api.log("Message", level=0)  # 0=INFO, 1=WARNING, 2=ERROR
```

---

## 🐛 Troubleshooting

### Script doesn't load
- Check script is in: `%APPDATA%\TS3Client\plugins\scripts\`
- Verify Python engine: `/tspy python status`
- Check plugin log: `%APPDATA%\TS3Client\plugins\tspy_plugin.log`

### Audio monitor issues
- Install pygame: `pip install pygame`
- Connect to a server first
- Check microphone is enabled in TeamSpeak

### Import errors
- Ensure you only `import ts3api` (builtin module)
- Standard library imports work: `import re`, `import json`, etc.
- External packages need to be installed: `pip install package_name`

---

## 💡 Tips

1. **Auto-loading**: Add your script to `scripts/tspy_init.py` for automatic loading
2. **Debugging**: Use `ts3api.log()` to write debug messages to plugin log
3. **Testing**: Use `/tspy python reload <script>` to reload after editing
4. **Safety**: Avoid infinite loops - they will freeze TeamSpeak
5. **Performance**: Heavy processing should be done in separate threads

---

**Happy scripting! 🚀**
