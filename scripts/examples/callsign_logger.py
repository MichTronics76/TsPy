"""
TsPy Plugin - Ham Radio Call Sign Logger
Example script for logging ham radio call signs from chat
"""

import ts3api
import re

# Regular expression for call signs (basic pattern)
CALLSIGN_PATTERN = r'\b[A-Z]{1,2}\d[A-Z]{1,4}\b'

# Store call signs (in production, save to file)
callsigns = set()

def on_text_message(server_id, target_mode, to_id, from_id, from_name, from_uid, message):
    """Extract and log call signs from messages"""
    
    # Find call signs in message
    matches = re.findall(CALLSIGN_PATTERN, message.upper())
    
    for callsign in matches:
        if callsign not in callsigns:
            callsigns.add(callsign)
            ts3api.log(f"New call sign detected: {callsign} (from {from_name})", 0)

def on_command(server_id, command, args):
    """Handle /tspy callsigns command"""
    if command == "callsigns":
        if not callsigns:
            ts3api.print_message(server_id, "No call signs logged yet")
        else:
            count = len(callsigns)
            signs = ", ".join(sorted(callsigns))
            ts3api.print_message(server_id, f"Logged {count} call signs: {signs}")

ts3api.log("Call Sign Logger script loaded", 0)
