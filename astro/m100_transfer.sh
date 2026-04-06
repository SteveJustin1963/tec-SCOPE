#!/bin/bash
# =========================================================
# M100 WiFi Auto-Transfer Script
# Author: Steve Justin / Claude Code
# Purpose: Transfer RAW images from M100 camera via WiFi
#          directly to 3TB drive without using memory card
# =========================================================

TARGET_DIR="/mnt/astro-data/astro/session_$(date +%Y%m%d_%H%M)/lights"
mkdir -p "$TARGET_DIR"

echo "=========================================="
echo "M100 WiFi Transfer Started"
echo "Target: $TARGET_DIR"
echo "Time: $(date)"
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

echo "Camera detected! Starting transfer..."
echo "Press Ctrl+C to stop when done shooting."
echo ""

# Continuously monitor and download new images
gphoto2 --capture-image-and-download \
        --filename "$TARGET_DIR/light_%04n.CR2" \
        --keep-on-camera \
        --interval 1

echo ""
echo "=========================================="
echo "Transfer complete!"
echo "Images saved to: $TARGET_DIR"
echo "Total images: $(ls -1 "$TARGET_DIR" | wc -l)"
echo "End time: $(date)"
echo "=========================================="
