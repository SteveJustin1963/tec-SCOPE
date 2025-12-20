#!/bin/bash
# =========================================================
# Siril Automated Headless Processing Script (fixed version)
# Author: Steve Justin / ChatGPT
# Purpose: Self-reliant astrophotography workflow
# Compatible with: Siril 1.4.0-beta and later
# =========================================================

# === CONFIGURATION ===
BASE_DIR=$HOME/astro/session_$(date +%Y%m%d_%H%M)
SCRIPT_NAME=process.ssf
LOGFILE="$BASE_DIR/siril_log_$(date +%Y%m%d_%H%M).txt"

# === CREATE DIRECTORY STRUCTURE ===
mkdir -p "$BASE_DIR"/{lights,darks,flats,bias,processed}

echo "----------------------------------------------------"
echo "Siril Automated Workflow Started"
echo "Start time: $(date)"
echo "Working directory: $BASE_DIR"
echo "Log file: $LOGFILE"
echo "----------------------------------------------------"

# === GENERATE BASIC SIRIL SCRIPT ===
cat > "$BASE_DIR/$SCRIPT_NAME" <<EOF
# Siril processing script (auto-generated)
# Note: 'requires' line removed for beta compatibility

cd $BASE_DIR/lights
# Example workflow — adjust as needed
convert light_ prefix=light seq
register light
stack light rej 3 3
save $BASE_DIR/processed/result.fit
exit
EOF

# === RUN SIRIL IN HEADLESS MODE ===
echo "Running Siril CLI..."
siril-cli -o -d "$BASE_DIR" -s "$BASE_DIR/$SCRIPT_NAME" | tee "$LOGFILE"

# === REPORT ===
echo "----------------------------------------------------"
echo "Processing complete."
echo "End time: $(date)"
echo "Output saved to: $BASE_DIR/processed"
echo "Full log: $LOGFILE"
echo "----------------------------------------------------"



