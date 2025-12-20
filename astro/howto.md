# Siril Automated Astrophotography Processing Guide

## Complete Workflow: Telescope → Linux → Processing → Viewing

### Step 1: Capture Images from Telescope Camera

From your imaging software (like SharpCap, APT, N.I.N.A., etc.):
- Capture your **light frames** (main images of your target)
- Optionally capture **darks**, **flats**, and **bias** frames for calibration

Save as: `.fit`, `.fits`, `.ser`, or raw formats

---

### Step 2: Transfer to Linux

You have two options for transferring images:

#### Option A: M100 WiFi Direct Transfer (Recommended for Live Shooting)

Transfer RAW images directly from your M100 camera via WiFi to the 3TB drive, bypassing the memory card entirely.

**Initial Setup:**

1. **M100 Software Requirements:**

   **Good News: No special software needed on the camera!**

   The M100 has built-in WiFi that works with gphoto2 out of the box. However:

   - **Firmware Update Recommended** (but not required)
   - Latest firmware improves WiFi stability and compatibility

   **To Check Your M100 Firmware Version:**
   - Turn on M100
   - Press **Menu** button
   - Navigate to: **Wrench Menu** → **Firmware Ver.**
   - Should show something like "Firmware Version 1.0.1" or "2.0.0"

   **To Update Firmware (Optional):**
   1. Visit: https://www.usa.canon.com/support
   2. Search for "EOS M100"
   3. Go to "Drivers & Downloads" → "Firmware"
   4. Download latest firmware to your computer
   5. Copy to SD card root directory
   6. Insert SD card in M100
   7. Menu → Firmware Update → Follow on-screen instructions

   **Note:** Most M100 cameras work fine with WiFi transfer on factory firmware. Only update if you experience connection issues.

2. **Configure M100 Camera (One-Time Setup):**

   **Step 1: Enable WiFi on M100**
   - Turn on your M100
   - Press the **Menu** button
   - Navigate to: **Wrench (Settings) Menu** → **Wireless Communication Settings**
   - Select **Wi-Fi/Bluetooth Connection**
   - Choose **Enable**

   **Step 2: Set Camera Nickname (Optional)**
   - In Wireless Communication Settings
   - Select **Nickname**
   - Set to something like "M100_Astro" (makes it easy to identify)

   **Step 3: Configure WiFi Function**
   - Press the **WiFi button** (looks like waves) on top of camera
   - Select **Connect to Smartphone** (this enables PTP/IP protocol for gphoto2)
   - Choose **Camera Access Point Mode**
   - Camera will display:
     - **SSID**: M100_XXXXXXX (or your nickname)
     - **Password**: Random password or set your own
   - **Write down the SSID and password!**

   **Step 4: Camera Settings for Astrophotography**
   - Set camera to **Manual Mode (M)**
   - Set **Image Quality** to **RAW** (not RAW+JPEG, just RAW)
   - Enable **Long Exposure Noise Reduction**: OFF (we'll stack images instead)
   - Enable **High ISO Noise Reduction**: OFF
   - Set **Auto Power Off**: 30 min or Disable (important for long sessions!)
   - Keep camera in **Shooting Mode** (not Playback)

   **Step 5: M100 Astrophotography Exposure Settings**

   **YES - You MUST increase ISO for astrophotography!**

   Astrophotography requires high ISO to capture faint light from stars, nebulae, and galaxies. Here are the recommended settings:

   **ISO Settings:**
   - **Recommended Range: ISO 1600-3200** (sweet spot for M100)
   - **Maximum: ISO 6400** (for very faint targets, but noisier)
   - **Why High ISO?** Captures faint light that's invisible at low ISO
   - **Don't Worry About Noise!** Stacking 100-1000 images will reduce noise dramatically

   **Shutter Speed:**
   - **With Star Tracker:** 60-300 seconds (depending on your mount)
   - **Without Tracker (wide field):** Use the "500 Rule"
     - Formula: 500 ÷ focal length = max seconds before star trails
     - Example: 500 ÷ 15mm = 33 seconds maximum
     - Example: 500 ÷ 50mm = 10 seconds maximum
   - **Deep Sky Objects:** Longer is better (if tracked)

   **Aperture:**
   - **Set to WIDEST (lowest f-number)** your lens allows
   - f/1.8, f/2.8, or f/3.5 typical
   - More light = better signal

   **Focus:**
   - **Manual Focus ONLY**
   - Set to **infinity (∞)** or slightly back from infinity
   - Use Live View with 10x magnification on a bright star
   - Fine-tune until star is smallest/sharpest point
   - **Tape the focus ring** so it doesn't move!

   **Other Critical Settings:**
   - **Image Stabilization: OFF** (causes blur on tripod/tracker)
   - **White Balance:** Daylight (3200K-5600K) - doesn't matter much for RAW
   - **Drive Mode:** Single shot or Intervalometer for sequences
   - **Mirror Lock-up:** Not available on M100, but electronic shutter is quiet

   **Example Settings for Different Scenarios:**

   ```
   Milky Way (wide field, no tracker):
   - ISO 3200
   - 15mm lens at f/2.8
   - 20 seconds (500÷15≈33, use slightly less)
   - Take 100-200 images

   Deep Sky with Star Tracker:
   - ISO 1600-2400
   - 50mm+ lens at widest aperture
   - 60-180 seconds per exposure
   - Take 50-100+ images

   Moon/Planets (bright targets):
   - ISO 400-800 (much lower!)
   - Fast shutter (1/125 - 1/500s)
   - Narrower aperture okay (f/5.6-f/8)
   ```

   **The Magic of Stacking:**
   - Single image at ISO 3200 = noisy
   - 100 images stacked = 10x less noise, same detail
   - 1000 images stacked = incredibly clean!
   - This is why we transfer and stack hundreds of images

   **Step 6: Automatic Image Capture - Intervalometer Setup**

   **You don't want to press the shutter 100-1000 times!** Here are your options for automatic image capture:

   **Option A: Built-in Self-Timer (Limited)**
   - M100 Menu → Self-timer → 2-second or 10-second delay
   - Only good for single shots, not continuous sequences
   - **Not recommended for astrophotography**

   **Option B: External Intervalometer (Best Option)**

   Purchase a **remote shutter release with intervalometer** (~$15-30):
   - Look for: "Canon M100 intervalometer remote" or "RS-60E3 compatible"
   - Plugs into M100's remote port (side of camera)

   **Recommended models:**
   - Canon RS-60E3 (basic wired remote, no interval)
   - but The Canon EOS M100 does not have a physical port for wired remote shutter releases like the RS-60E3 (which uses a 2.5mm E3-type connector).

This is a known limitation of the entry-level EOS M100 model—unlike higher models in the M series (e.g., M5, M6, M6 Mark II), it lacks the remote control terminal.

Reliable sources, including B&H Photo's official compatibility list for the genuine Canon RS-60E3, explicitly include the EOS M100 in some listings, but this appears to be an error, as user reports and forum discussions (e.g., on Digital Photography Review) confirm that the M100 does not support wired remotes.

Third-party RS-60E3 replacements (like JJC, Kiwifotos, or Foto&Tech) typically list compatibility with cameras that have the E3 port, such as Rebels, 60D–90D, EOS R series, M5/M6, and PowerShot G series—but not the M100.

For the M100, alternatives include:

Wireless control via the Canon Camera Connect app on your phone (Bluetooth/Wi-Fi), but limited features, such as No, not directly with the official Canon Camera Connect app. It lacks a built-in intervalometer or timer sequence feature—you can't program it to automatically take a series of shots (e.g., 100 exposures at 30 seconds each with delays between). The self-timer and continuous modes are basic: Self-timer only delays a single shot (or short burst). Continuous shooting takes rapid-fire photos (not spaced out for long exposures like astro/timelapse).

But Third-Party Apps: Try "Shutter - Canon Camera Remote" (iOS/Android, ~$5–10) or "Intervalometer for Canon" (~$5). These add interval timing over Wi-Fi/Bluetooth, letting you set 100 shots at 30-sec exposures with customizable intervals (e.g., 1-sec delay between). They support the M100 and work alongside transfers. Users report great results for astrophotography.

## USB Tethered Control with gphoto2 on Linux for Canon EOS M100

Yes, you can fully achieve this on Linux using gphoto2 (a free, open-source command-line tool) over USB—no Wi-Fi needed. The EOS M100 is well-supported by gphoto2 for tethered shooting, including remote shutter release, setting adjustments, live view (limited), and long bulb exposures (perfect for your 30-second shots). Users have successfully used it for astrophotography with exposures up to 1200 seconds+.
This gives you precise, scripted control—exactly what you need for taking 100 images at 30-second exposures (e.g., for timelapse or astro stacking).

Step 1: Setup
- Connect your M100 via USB cable.
- On the camera: Set to Manual (M) mode and Bulb shutter speed (dial to beyond 30" → Bulb).
- Install gphoto2 (if not already):
- Ubuntu/Debian: `sudo apt update && sudo apt install gphoto2`
- Fedora: `sudo dnf install gphoto2`
- Arch: `sudo pacman -S gphoto2`
- Test detection: `gphoto2 --auto-detect`
- Should show: Canon EOS M100
- List available settings: `gphoto2 --list-config`
- Key ones:  ` /main/settings/iso, /main/capturesettings/aperture, /main/status/battery`, and importantly 
`/main/actions/eosremoterelease` (for bulb control).


Step 2: Basic Tethered Shooting
- Single photo (with download to PC): `gphoto2 --capture-image-and-download`
- Preview live view: `gphoto2 --capture-preview` (or loop it for monitoring).
- For GUI: Install Entangle (`sudo apt install entangle` on Ubuntu). It uses gphoto2 backend and provides:
- Live view preview (lower resolution on M100).
- Remote shutter button.
- Basic setting tweaks.
- Session monitoring.
- Users confirm it works with the M100 for tethered shooting.

Step 3: Automated Sequence 
- 100 × 30-Second Bulb Exposures
- gphoto2 controls bulb via the eosremoterelease config (specific to Canon EOS cameras like the M100).
- Create a bash script for your sequence.
- Save this as `astro_timelapse.sh` and make executable (`chmod +x astro_timelapse.sh`):

```Bash
#!/bin/bash

# Settings
NUM_SHOTS=100          # Number of images
EXPOSURE=30            # Seconds per exposure
DELAY_BETWEEN=5        # Seconds delay between shots (adjust for gap; set to 0 for minimal)
FILENAME_PREFIX="astro_%04d.CR2"  # Filename pattern (RAW; change to .JPG if needed)

echo "Starting $NUM_SHOTS shots of ${EXPOSURE}s bulb exposures..."

for i in $(seq 1 $NUM_SHOTS); do
    printf "Shot %3d/%d: " $i $NUM_SHOTS
    
    # Press shutter (start exposure) - values like "Press Full" or numeric 5/2 vary; test yours
    gphoto2 --set-config eosremoterelease="Press Full"  # Or try "Immediate" / numeric like 5
    
    # Wait for exposure time
    gphoto2 --wait-event=${EXPOSURE}s
    
    # Release shutter (end exposure)
    gphoto2 --set-config eosremoterelease="Release Full"  # Or numeric like 11/4
    
    # Wait for camera to finish processing and download the image
    gphoto2 --wait-event-and-download=10s --filename="${FILENAME_PREFIX}" --keep
    
    # Optional delay between shots
    sleep $DELAY_BETWEEN
    
    echo "Done"
done

echo "Sequence complete!"
```

## Notes on eosremoterelease values 
- (these vary slightly by model; test with `gphoto2 --list-config` and trial shots):
- Common working pairs for M100/EOS series:
- Start: "Press Full" or numeric 5 / 2
- End: "Release Full" or numeric 11 / 4
- From user reports (including M100):
- One example uses `--set-config eosremoterelease=2` (press), wait, `=4` (release).
- If string values don't work, use numerics—run `gphoto2 --get-config eosremoterelease` to see choices.
- Alternative simpler bulb method (if your gphoto2 version supports -B flag for bulb):

```Bash
for i in $(seq 1 100); do
    gphoto2 --bulb=$EXPOSURE --capture-image-and-download --filename="astro_%04d.CR2"
    sleep 5  # Gap between shots
done
```
This works on many Canon models if the camera is in Bulb mode.

Advantages Over Wireless
- More reliable (no Wi-Fi dropouts or battery drain from hotspot).
- Faster downloads (direct USB).
- Full scripting for exact timing (e.g., minimal gaps for star trails).
- Can combine with tools like darktable for immediate RAW processing.
- If the script throws errors
- (e.g., unknown eosremoterelease value), run a single test: `gphoto2 --summary`   and share output—I can refine it.
- This setup is widely used for astro with the M100! Let me know how it goes.


# settings
   **Typical Settings for Milky Way:**
   ```
   Intervalometer Settings:
   - Interval: 25 seconds
   - Exposure: 20 seconds (or BULB mode with 20s duration)
   - Count: 200 shots
   - Delay: 0 seconds

   = 200 images captured over ~83 minutes automatically
   ```

   **Option C: Using gphoto2 via WiFi (Best for Full Automation!)**

   Control the M100 completely from your Linux laptop - **No external intervalometer needed!**

   **We've created a script for you: `m100_auto_capture.sh`**

   **To use it:**
   1. Edit the settings at the top of the script:
      ```bash
      nano ~/astro/m100_auto_capture.sh
      ```
      Change these values:
      ```bash
      ISO=3200           # Your desired ISO
      SHUTTER_SPEED="20" # Exposure time in seconds
      INTERVAL=25        # Time between shots (exposure + 3-5 seconds)
      NUM_FRAMES=100     # How many images to capture
      ```

   2. Save and run:
      ```bash
      cd ~/astro
      ./m100_auto_capture.sh
      ```

   **Benefits:**
   - **No physical remote needed** - camera controlled via WiFi
   - **No touching the camera** - fully automated
   - **Automatic download** - images save directly to 3TB drive
   - **Adjustable settings** - edit script for different targets
   - **Progress tracking** - see each image as it captures

   **Manual gphoto2 command (if you prefer):**
   ```bash
   # Take 100 images with 20-second exposures
  
   ```
   gphoto2 --set-config shutterspeed=20 \
   --set-config iso=3200 \
   --capture-image-and-download \
   --interval 25 \
   --frames 100 \
   --filename /mnt/astro-data/astro/session_$(date +%Y%m%d_%H%M)/lights/light_%04n.CR2
   ```

   **Option D: Camera Connect App (Not Recommended)**
   - Canon's smartphone app conflicts with our WiFi transfer setup
   - Would require switching WiFi connections
   - Skip this option

   **Recommended Workflow for 100-1000 Images:**

   1. **Set up camera** (focus, compose, exposure settings)
   2. **Connect external intervalometer** OR **use gphoto2 command**
   3. **Set intervalometer**:
      - Your exposure time + 3-5 seconds for buffer
      - Number of shots: 100-1000
   4. **Start WiFi transfer script** (if using remote):
      ```bash
      ./m100_transfer.sh
      ```
   5. **Start intervalometer** and walk away!
   6. Camera shoots automatically, images transfer automatically
   7. Come back in 30-90 minutes to hundreds of images ready to stack!

   **Pro Tips:**
   - **Interval = Exposure time + 3-5 seconds** (allows camera to save file)
   - For 20-second exposures, use 23-25 second intervals
   - For 180-second exposures, use 185-190 second intervals
   - Bring extra batteries! Long sessions drain power
   - Test your sequence with 5-10 images before committing to 1000

3. **Install gphoto2 on Linux (camera control tool):**
   ```bash
   sudo apt install gphoto2
   ```

4. **Connect Linux to M100 WiFi:**

   On M100: Press WiFi button, wait for "Waiting to connect..." message

   On Linux:
   ```bash
   # List available WiFi networks (find your M100)
   nmcli dev wifi list

   # Connect using SSID and password from camera screen
   nmcli dev wifi connect "M100_XXXXXXX" password "your-camera-password"
   ```

5. **Test camera connection:**
   ```bash
   gphoto2 --auto-detect
   # Should show: "Canon EOS M100" with USB/WiFi port
   ```

   If camera not detected, see Troubleshooting section below.

6. **Transfer script is already created:**
   The `m100_transfer.sh` script is already in your `~/astro/` directory and ready to use!

**For Each Shooting Session:**

After initial setup, here's what to do each time you shoot:

1. **On M100:**
   - Turn on camera
   - Press **WiFi button**
   - Select the saved connection (should auto-connect to previous settings)
   - Wait for "Waiting to connect..." or WiFi icon to appear
   - **Keep camera in shooting mode** (not playback!)

2. **On Linux:**
   ```bash
   # Connect to M100 WiFi
   nmcli dev wifi connect "M100_XXXXXXX" password "your-password"

   # Start the transfer script BEFORE shooting
   cd ~/astro
   ./m100_transfer.sh
   ```

3. **Start Shooting:**
   - The script will automatically download each image as you capture it
   - You'll see confirmation messages for each transferred image
   - Keep the terminal window open while shooting

This will:
- Automatically download each RAW image as it's captured
- Save directly to your 3TB drive
- Handle hundreds/thousands of images without filling the memory card
- Name files sequentially (light_0001.CR2, light_0002.CR2, etc.)

**Important Notes:**
- Keep your laptop within WiFi range of the M100 (5-10 meters recommended)
- Battery drain is faster with WiFi enabled - use AC adapter or fully charged battery
- If WiFi disconnects, just reconnect and restart the script - it will resume with next image number
- You can still shoot with the camera's shutter button or use intervalometer while connected

**What You Should See on M100 Screen When Ready:**
```
📱 WiFi icon displayed (top of screen)
📷 Camera in Manual (M) mode
🔴 Red dot (recording ready indicator)
Message: "Waiting to connect..." or "Connected"
```
**If you see this, you're ready to run the transfer script!**

**Alternative: Download All Images at Once**

If images are already on the camera:
```bash
TARGET_DIR="/mnt/astro-data/astro/session_$(date +%Y%m%d_%H%M)/lights"
mkdir -p "$TARGET_DIR"
gphoto2 --get-all-files --filename "$TARGET_DIR/light_%04n.CR2"
```

---

#### Option B: Manual Transfer

Copy images from memory card or other computer:

```bash
# From USB drive/memory card:
cp /media/usb/my_images/*.CR2 /mnt/astro-data/astro/session_YYYYMMDD_HHMM/lights/

# Or via network (scp example):
scp user@camera-computer:/path/to/images/*.CR2 /mnt/astro-data/astro/session_YYYYMMDD_HHMM/lights/
```

---

### Step 3: Run Siril Processing

After images are transferred to the lights/ folder:

```bash
cd ~/astro
./siril_auto.sh
```

This will:
- Create a new session directory with timestamp
- Generate the processing script
- Convert RAW/CR2 files to FITS format
- Register (align) all images
- Stack them into a single high-quality image
- Save the result to processed/

**OR** to process an existing session:
```bash
cd ~/astro
siril-cli -s /mnt/astro-data/astro/session_20251113_1528/process.ssf
```

**Note:** Siril automatically handles CR2, FITS, SER, TIFF, and most RAW formats.

---

### Step 4: View the Results

**Option A - Command line viewer:**
```bash
# Install if needed
sudo apt install feh

# View the result
feh session_20251113_1528/processed/result.fit
```

**Option B - Use Siril GUI:**
```bash
siril
# Then: File → Open → Navigate to processed/result.fit
```

**Option C - Convert to image format:**
```bash
# Siril can export to TIFF/JPG
siril-cli -c "load session_20251113_1528/processed/result.fit; save session_20251113_1528/processed/result.tif"
```

---

## Using External 3TB Hard Drive for Image Storage

Since astrophotography generates hundreds of large image files, you should mount and use your 3TB external drive.

### One-Time Setup

1. **Identify your external drive:**
```bash
lsblk
# Look for your 3TB drive (e.g., /dev/sdb1)
```

2. **Create mount point:**
```bash
sudo mkdir -p /mnt/astro-data
```

3. **Mount the drive:**
```bash
sudo mount /dev/sdb1 /mnt/astro-data
```

4. **Set ownership:**
```bash
sudo chown -R steve:steve /mnt/astro-data
```

5. **Auto-mount on boot (optional):**
```bash
# Get the UUID of your drive
sudo blkid /dev/sdb1

# Edit fstab
sudo nano /etc/fstab

# Add this line (replace UUID with your actual UUID):
UUID=your-uuid-here /mnt/astro-data ext4 defaults 0 2
```

### Update Script to Use External Drive

Edit the `siril_auto.sh` script to use the external drive:

```bash
# Change this line:
BASE_DIR=$HOME/astro/session_$(date +%Y%m%d_%H%M)

# To this:
BASE_DIR=/mnt/astro-data/astro/session_$(date +%Y%m%d_%H%M)
```

Now all your sessions will be stored on the 3TB drive!

---

## Quick Setup for Next Session

For a **completely fresh session**:

```bash
cd ~/astro
./siril_auto.sh  # Creates new dated folder
# Then copy your new images into the lights/ folder it creates
```

---

## Complete M100 WiFi Workflow (Quick Reference)

### Method 1: Fully Automated (Recommended - No Intervalometer Needed!)

```bash
# 1. Mount 3TB drive (if not auto-mounted)
sudo mount /dev/sdb1 /mnt/astro-data

# 2. Connect to M100 WiFi
nmcli dev wifi connect M100_Astro password your-password

# 3. Edit capture settings (one time or as needed)
nano ~/astro/m100_auto_capture.sh
# Set: ISO, SHUTTER_SPEED, INTERVAL, NUM_FRAMES

# 4. Run automated capture script
cd ~/astro
./m100_auto_capture.sh
# Camera will automatically:
#   - Capture 100-1000 images
#   - Transfer each to 3TB drive
#   - No touching camera needed!

# 5. After capture completes, process images
./siril_auto.sh

# 6. View results
feh /mnt/astro-data/astro/session_*/processed/result.fit
```

**Benefits:**
- **100% automated** - set it and forget it!
- No external intervalometer purchase needed
- Camera settings controlled via WiFi
- Direct save to 3TB drive
- No memory card limitations

---

### Method 2: With External Intervalometer

```bash
# 1. Mount 3TB drive (if not auto-mounted)
sudo mount /dev/sdb1 /mnt/astro-data

# 2. Connect to M100 WiFi
nmcli dev wifi connect M100_Astro password your-password

# 3. Set up external intervalometer on M100
# Configure: interval, number of shots, exposure time

# 4. Start WiFi transfer script
cd ~/astro
./m100_transfer.sh
# Leave this running

# 5. Press START on intervalometer
# Images automatically transfer as they're captured

# 6. After shooting, Ctrl+C to stop transfer, then process
./siril_auto.sh

# 7. View results
feh /mnt/astro-data/astro/session_*/processed/result.fit
```

**Use this method if:**
- You already own an intervalometer
- You prefer physical camera control
- You want to use BULB mode with very long exposures

---

## Troubleshooting

### M100 WiFi Issues

**Camera not detected by gphoto2:**
```bash
# Check if camera is connected
gphoto2 --auto-detect

# If not found, try:
sudo killall gvfsd-gphoto2  # Kill conflicting processes
gphoto2 --auto-detect
```

**WiFi connection drops:**
```bash
# Reconnect to M100
nmcli dev wifi connect M100_Astro password your-password

# Check connection status
nmcli connection show
```

**Transfer script fails:**
- Make sure 3TB drive is mounted: `df -h | grep astro-data`
- Check camera is in shooting mode (not playback)
- Ensure WiFi is stable (camera and laptop close together)
- Try: `sudo killall gvfsd-gphoto2` then reconnect

**M100 WiFi keeps disconnecting:**
- Check Auto Power Off is disabled on camera
- Keep laptop within 5-10 meters of camera
- Avoid obstacles between camera and laptop
- If using outdoors, WiFi range may be limited by interference

**Camera says "No Access Point Found":**
- You selected the wrong WiFi mode
- Go back and choose **Camera Access Point Mode** (not Infrastructure or Connect to Existing Network)
- Camera should CREATE its own network, not connect to yours

**Images not transferring even though connected:**
- Make sure camera is set to **RAW** format (not JPEG or RAW+JPEG may cause issues)
- Check that you're in **shooting mode** by half-pressing shutter
- Try disconnecting WiFi on M100, turn WiFi off/on, then reconnect
- Restart the transfer script

**Can I use intervalometer with WiFi transfer?**
- Yes! Use the M100's built-in intervalometer or external remote
- The script will detect and download each new image automatically
- Perfect for 100-1000 image sessions without touching the camera

### Siril Processing Issues

**GTK Warnings (Ignore Safely):**

When running in headless mode, you may see:
```
Gtk-CRITICAL **: gtk_builder_get_object: assertion 'GTK_IS_BUILDER (builder)' failed
```

These are **GUI-related warnings** and have no effect on processing. Siril runs fine in CLI mode despite these messages.

**"requires command is missing" Warning:**

This warning appears because we removed the `requires` line for beta compatibility. Your script still runs perfectly — this is just informational.

**Out of disk space:**

If processing fails due to space:
```bash
# Check available space on 3TB drive
df -h /mnt/astro-data

# Clean up old sessions if needed
rm -rf /mnt/astro-data/astro/session_OLD_DATE/
```

---

## System Information

- **Siril Version:** 1.2.1
- **Installation Path:** /usr/bin/siril
- **Script Location:** ~/astro/siril_auto.sh
- **Working Directory:** ~/astro/ or /mnt/astro-data/astro/
