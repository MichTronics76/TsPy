"""
TsPy Plugin - Example Python Script
This script demonstrates Python integration with TeamSpeak
"""

import ts3api

def on_connect(server_id):
    """Called when connected to a server"""
    ts3api.log(f"Connected to server! ID: {server_id}", 0)
    ts3api.print_message(server_id, "✨ Python script loaded! Welcome!")

def on_disconnect(server_id):
    """Called when disconnected from a server"""
    ts3api.log(f"Disconnected from server {server_id}", 0)

def on_client_move(server_id, client_id, old_channel, new_channel):
    """Called when a client moves channels"""
    client_name = ts3api.get_client_name(server_id, client_id)
    if client_name:
        ts3api.log(f"Client {client_name} moved from channel {old_channel} to {new_channel}", 0)

def on_text_message(server_id, target_mode, to_id, from_id, from_name, from_uid, message):
    """Called when a text message is received"""
    ts3api.log(f"Message from {from_name}: {message}", 0)
    
    # Auto-respond to greetings
    if "hello" in message.lower() or "hi" in message.lower():
        ts3api.send_channel_message(server_id, f"Hi {from_name}! 👋 (auto-reply from Python)")

def on_command(server_id, command, args):
    """Called when a plugin command is executed"""
    if command == "python":
        if args == "test":
            ts3api.print_message(server_id, "🐍 Python is working!")
            ts3api.send_channel_message(server_id, "Python test successful!")
        elif args == "info":
            import sys
            info = f"Python {sys.version_info.major}.{sys.version_info.minor}.{sys.version_info.micro}"
            ts3api.print_message(server_id, f"🐍 {info}")
        else:
            ts3api.print_message(server_id, "Usage: /tspy python [test|info]")

# Startup message
ts3api.log("🐍 TsPy Python script initialized!", 0)
print("TsPy script loaded - TeamSpeak Python integration active!")
