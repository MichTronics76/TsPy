"""
TsPy Audio Monitor v1.0
Real-time audio level monitoring with PyGame UI

Features:
- Visual audio level meter (VU meter)
- Talk status indicators
- Recording control
- Client activity tracking
"""

import pygame
import sys
import time
import threading
import ts3api

# Initialize PyGame
pygame.init()

# Constants
WINDOW_WIDTH = 800
WINDOW_HEIGHT = 600
FPS = 30

# Colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
GREEN = (0, 255, 0)
YELLOW = (255, 255, 0)
RED = (255, 0, 0)
BLUE = (0, 100, 255)
GRAY = (128, 128, 128)
DARK_GRAY = (50, 50, 50)

# Audio settings
SERVER_ID = 1  # Will be set dynamically
recording = False
audio_level = 0.0
talking_clients = {}  # {client_id: client_name}

class AudioMonitor:
    def __init__(self):
        self.screen = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
        pygame.display.set_caption("TsPy Audio Monitor v1.0")
        self.clock = pygame.time.Clock()
        self.font_large = pygame.font.Font(None, 48)
        self.font_medium = pygame.font.Font(None, 32)
        self.font_small = pygame.font.Font(None, 24)
        self.running = True
        self.server_id = SERVER_ID
        
    def draw_vu_meter(self, x, y, width, height, level):
        """Draw VU meter showing audio level"""
        # Background
        pygame.draw.rect(self.screen, DARK_GRAY, (x, y, width, height))
        pygame.draw.rect(self.screen, WHITE, (x, y, width, height), 2)
        
        # Level bars (from -60dB to 0dB)
        # Normalize: -60dB = 0%, 0dB = 100%
        level_percent = max(0, min(100, (level + 60) / 60 * 100))
        bar_width = int(width * level_percent / 100)
        
        # Color based on level
        if level_percent > 80:
            color = RED
        elif level_percent > 60:
            color = YELLOW
        else:
            color = GREEN
            
        if bar_width > 0:
            pygame.draw.rect(self.screen, color, (x, y, bar_width, height))
        
        # Level markers
        for i in range(0, 101, 20):
            marker_x = x + int(width * i / 100)
            pygame.draw.line(self.screen, WHITE, (marker_x, y + height), 
                           (marker_x, y + height + 10), 2)
            
        # Level text
        level_text = self.font_small.render(f"{level:.1f} dB", True, WHITE)
        self.screen.blit(level_text, (x + width + 10, y + height // 2 - 10))
        
    def draw_recording_button(self, x, y, width, height, is_recording):
        """Draw recording control button"""
        color = RED if is_recording else GRAY
        pygame.draw.rect(self.screen, color, (x, y, width, height))
        pygame.draw.rect(self.screen, WHITE, (x, y, width, height), 2)
        
        text = "⏹ STOP REC" if is_recording else "⏺ START REC"
        button_text = self.font_medium.render(text, True, WHITE)
        text_rect = button_text.get_rect(center=(x + width // 2, y + height // 2))
        self.screen.blit(button_text, text_rect)
        
        return pygame.Rect(x, y, width, height)
        
    def draw_talking_list(self, x, y, width, height):
        """Draw list of currently talking clients"""
        pygame.draw.rect(self.screen, DARK_GRAY, (x, y, width, height))
        pygame.draw.rect(self.screen, WHITE, (x, y, width, height), 2)
        
        title = self.font_medium.render("Currently Talking:", True, WHITE)
        self.screen.blit(title, (x + 10, y + 10))
        
        y_offset = y + 50
        if not talking_clients:
            no_one = self.font_small.render("(no one)", True, GRAY)
            self.screen.blit(no_one, (x + 20, y_offset))
        else:
            for client_id, client_name in talking_clients.items():
                # Talking indicator
                pygame.draw.circle(self.screen, GREEN, (x + 20, y_offset + 10), 8)
                # Client name
                name_text = self.font_small.render(client_name, True, WHITE)
                self.screen.blit(name_text, (x + 40, y_offset))
                y_offset += 30
                
                if y_offset > y + height - 30:
                    break
    
    def update_audio_level(self):
        """Get current audio level from TeamSpeak"""
        global audio_level
        try:
            level = ts3api.get_audio_level(self.server_id)
            if level is not None:
                audio_level = level
        except Exception as e:
            ts3api.log(f"Error getting audio level: {e}", 2)
            
    def toggle_recording(self):
        """Toggle recording on/off"""
        global recording
        try:
            if recording:
                if ts3api.stop_recording(self.server_id):
                    recording = False
                    ts3api.log("Recording stopped", 0)
            else:
                if ts3api.start_recording(self.server_id):
                    recording = True
                    ts3api.log("Recording started", 0)
        except Exception as e:
            ts3api.log(f"Error toggling recording: {e}", 2)
    
    def run(self):
        """Main UI loop"""
        while self.running:
            # Event handling
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.running = False
                elif event.type == pygame.MOUSEBUTTONDOWN:
                    mouse_pos = pygame.mouse.get_pos()
                    # Check if recording button clicked
                    rec_button = pygame.Rect(550, 100, 200, 60)
                    if rec_button.collidepoint(mouse_pos):
                        self.toggle_recording()
            
            # Update audio level
            self.update_audio_level()
            
            # Draw everything
            self.screen.fill(BLACK)
            
            # Title
            title = self.font_large.render("TsPy Audio Monitor", True, BLUE)
            self.screen.blit(title, (WINDOW_WIDTH // 2 - 200, 20))
            
            # VU Meter
            vu_label = self.font_medium.render("Microphone Level:", True, WHITE)
            self.screen.blit(vu_label, (50, 100))
            self.draw_vu_meter(50, 140, 450, 60, audio_level)
            
            # Recording button
            self.draw_recording_button(550, 100, 200, 60, recording)
            
            # Recording status
            if recording:
                rec_status = self.font_small.render("🔴 RECORDING", True, RED)
                self.screen.blit(rec_status, (550, 170))
            
            # Talking list
            self.draw_talking_list(50, 250, 700, 300)
            
            # Info
            info_text = self.font_small.render(
                "Press recording button to start/stop recording | Close window to exit",
                True, GRAY
            )
            self.screen.blit(info_text, (50, 570))
            
            pygame.display.flip()
            self.clock.tick(FPS)
        
        pygame.quit()

# Event handlers for TeamSpeak integration
def on_talk_status_change(server_id, status, client_id):
    """Called when someone starts/stops talking"""
    global talking_clients
    
    if status == 1:  # Started talking
        client_name = ts3api.get_client_name(server_id, client_id)
        if client_name:
            talking_clients[client_id] = client_name
            ts3api.log(f"🎤 {client_name} started talking", 0)
    else:  # Stopped talking
        if client_id in talking_clients:
            client_name = talking_clients.pop(client_id)
            ts3api.log(f"🔇 {client_name} stopped talking", 0)

# Initialize and run
ts3api.log("🎤 TsPy Audio Monitor starting...", 0)
ts3api.log("Opening PyGame window - check taskbar!", 0)

try:
    monitor = AudioMonitor()
    monitor.run()
except Exception as e:
    ts3api.log(f"Audio Monitor error: {e}", 2)
    import traceback
    ts3api.log(traceback.format_exc(), 2)

ts3api.log("🎤 Audio Monitor closed", 0)
