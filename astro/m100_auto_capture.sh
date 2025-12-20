#!/bin/bash
# =========================================================
# M100 Automated Capture & Transfer Script
# Author: Steve Justin / Claude Code
# Purpose: Automatically capture images via WiFi and save
#          directly to 3TB drive using gphoto2
# No external intervalometer needed!
# =========================================================

# === CONFIGURATION ===
TARGET_DIR="/mnt/astro-data/astro/session_$(date +%Y%m%d_%H%M)/lights"
mkdir -p "$TARGET_DIR"

# User configurable settings
ISO=3200
SHUTTER_SPEED="20"  # seconds
INTERVAL=25         # seconds between shots (should be > SHUTTER_SPEED + 3)
NUM_FRAMES=100      # number of images to capture

echo "=========================================="
echo "M100 Automated Capture Started"
echo "Target: $TARGET_DIR"
echo "Time: $(date)"
echo "=========================================="
echo "Settings:"
echo "  ISO: $ISO"
echo "  Shutter Speed: ${SHUTTER_SPEED}s"
echo "  Interval: ${INTERVAL}s"
echo "  Number of Frames: $NUM_FRAMES"
echo "=========================================="

# Check if camera is connected
echo "Detecting camera..."
if ! gphoto2 --auto-detect | grep -q "Camera"; then
    echo "ERROR: Camera not detected!"
    echo "Please check:"
    echo "  1. M100 WiFi is enabled"
    echo "  2. Linux is connected to M100's WiFi network"
    echo "  3. Camera is in shooting mode (not playback)"
    exit 1
fi

echo "Camera detected! Starting automated capture..."
echo "This will take approximately $(( (INTERVAL * NUM_FRAMES) / 60 )) minutes"
echo "Press Ctrl+C to stop early."
echo ""

# Configure camera settings
echo "Configuring camera settings..."
gphoto2 --set-config iso=$ISO 2>/dev/null
gphoto2 --set-config shutterspeed=$SHUTTER_SPEED 2>/dev/null

# Start automated capture with download
gphoto2 --capture-image-and-download \
        --interval $INTERVAL \
        --frames $NUM_FRAMES \
        --filename "$TARGET_DIR/light_%04n.CR2"

echo ""
echo "=========================================="
echo "Capture complete!"
echo "Images saved to: $TARGET_DIR"
echo "Total images: $(ls -1 "$TARGET_DIR"/*.CR2 2>/dev/null | wc -l)"
echo "End time: $(date)"
echo "=========================================="
echo ""
echo "Ready to process? Run:"
echo "  cd ~/astro && ./siril_auto.sh"
