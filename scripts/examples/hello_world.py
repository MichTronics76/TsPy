"""
Hello World - Simple demonstration script
This script runs immediately when loaded and tests the ts3api module
"""

import ts3api
import sys

# Test logging
ts3api.log("=" * 60, 0)
ts3api.log("🎉 HELLO FROM PYTHON! 🎉", 0)
ts3api.log(f"Python version: {sys.version_info.major}.{sys.version_info.minor}.{sys.version_info.micro}", 0)
ts3api.log(f"Platform: {sys.platform}", 0)
ts3api.log("=" * 60, 0)

print("\n🐍 Python script executed successfully!")
print(f"   Python {sys.version}")
print(f"   ts3api module is working! ✅\n")

# Note: To test server-specific functions like print_message or send_channel_message,
# you need to be connected to a server and pass the correct serverConnectionHandlerID.
# Use /tspy python status to check if Python is running.
