"""
TsPy Plugin - Auto-Greeter Example
Automatically greets users who join your channel
"""

import ts3api

# Configuration
GREETING_MESSAGE = "Welcome to the channel! 👋"
ENABLE_GREETER = True

def on_client_move(server_id, client_id, old_channel, new_channel):
    """Greet users who join our channel"""
    if not ENABLE_GREETER:
        return
    
    # Get our own client ID
    my_id = ts3api.get_client_id(server_id)
    
    # Don't greet ourselves
    if client_id == my_id:
        return
    
    # Get the client's name
    client_name = ts3api.get_client_name(server_id, client_id)
    
    if client_name:
        greeting = f"{client_name}, {GREETING_MESSAGE}"
        ts3api.send_channel_message(server_id, greeting)
        ts3api.log(f"Greeted {client_name}", 0)

ts3api.log("Auto-Greeter script loaded", 0)
