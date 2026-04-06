# TEC-1 Single Board Computer - Hardware Specification

## Overview

The **TEC-1** (Talking Electronics Z80 trainer) is a minimal Z80-based educational computer with simple, partially-decoded I/O. The design uses only lower address bits for I/O decoding, resulting in heavy port mirroring.

## Architecture

### I/O Decoding

Two **74LS138** (or 74HC138) 3-to-8 decoders handle peripheral access, enabled during **IORQ̅** low:

- **Y0**: Activates 74C923 keyboard encoder (reads 4×5 keypad, triggers NMI on keypress)
- **Y1**: Enables upper 74LS273 latch → controls 6 common ground connections (one per digit)
- **Y2**: Enables lower 74LS273 latch → drives 8 segment lines (a–g + dp)

### I/O Port Map

| Port (hex) | Direction | Function                | Controlled by | Description                                    |
|------------|-----------|-------------------------|---------------|------------------------------------------------|
| 00h        | Input     | Keyboard data           | Y0            | Read 5-bit key code from 74C923 encoder        |
| 01h        | Input     | Keyboard status         | -             | Poll DAV (data available) bit (often mirrored) |
| 02h        | Output    | Display digit select    | Y1            | Upper 74LS273 → 6 bits select digit (common ground) |
| 03h        | Output    | Display segment data    | Y2            | Lower 74LS273 → 8 bits drive segments a-g + dp |

**Note**: Ports often mirror every 8 addresses (e.g., 00h–07h) due to partial address decoding.

## Display System

### Configuration

- **6 common-cathode 7-segment displays** (multiplexed)
- All displays share the same 8 segment lines (from port 03h)
- Individual digit selection via port 02h (one bit per digit)

### Segment Wiring (Port 03h)

The segment-to-data mapping is non-sequential:

| Segment | Data Line | Bit Position |
|---------|-----------|--------------|
| a       | D0        | bit 0        |
| f       | D1        | bit 1        |
| g       | D2        | bit 2        |
| b       | D3        | bit 3        |
| DP      | D4        | bit 4        |
| c       | D5        | bit 5        |
| e       | D6        | bit 6        |
| d       | D7        | bit 7        |

### Hex Digit Lookup Table

Example patterns for port 03h (active high):

| Digit | Segments lit  | Binary     | Hex  |
|-------|---------------|------------|------|
| 0     | a,b,c,d,e,f   | 11101011   | 0xEB |
| 1     | b,c           | 00101000   | 0x28 |
| 2     | a,b,d,e,g     | 10010101   | 0x95 |
| 3     | a,b,c,d,g     | 10101101   | 0xAD |
| 4     | b,c,f,g       | 00101110   | 0x2E |
| 5     | a,c,d,f,g     | 10100111   | 0xA7 |
| 6     | a,c,d,e,f,g   | 11100111   | 0xE7 |
| 7     | a,b,c         | 00101001   | 0x29 |
| 8     | a,b,c,d,e,f,g | 11101111   | 0xEF |
| 9     | a,b,c,d,f,g   | 10101111   | 0xAF |
| A     | a,b,c,e,f,g   | 01101111   | 0x6F |
| B     | c,d,e,f,g     | 11100110   | 0xE6 |
| C     | a,d,e,f       | 11000011   | 0xC3 |
| D     | b,c,d,e,g     | 11010100   | 0xD4 |
| E     | a,d,e,f,g     | 11000111   | 0xC7 |
| F     | a,e,f,g       | 01000111   | 0x47 |

### Multiplexing Operation

1. Write to port **02h**: Select digit (e.g., `00000001b` = digit 1, `00000010b` = digit 2, etc.)
2. Write to port **03h**: Set segment pattern for that digit
3. Repeat for all 6 digits rapidly (persistence of vision creates illusion of all lit)

**Example:**
```asm
; Display "A" on digit 3
LD A, 00000100b    ; Select digit 3
OUT (02h), A
LD A, 0x6F         ; Pattern for "A"
OUT (03h), A
```

## Keyboard System

### 74C923 Encoder Wiring

4×5 matrix keypad with the following encoding:

| Key | Position | 5-bit Code | Hex  |
|-----|----------|------------|------|
| 0   | x1y1     | 00000      | 0x00 |
| 1   | x2y1     | 00001      | 0x01 |
| 2   | x3y1     | 00010      | 0x02 |
| 3   | x4y1     | 00011      | 0x03 |
| 4   | x1y2     | 00100      | 0x04 |
| 5   | x2y2     | 00101      | 0x05 |
| 6   | x3y2     | 00110      | 0x06 |
| 7   | x4y2     | 00111      | 0x07 |
| 8   | x1y3     | 01000      | 0x08 |
| 9   | x2y3     | 01001      | 0x09 |
| A   | x3y3     | 01010      | 0x0A |
| B   | x4y3     | 01011      | 0x0B |
| C   | x1y4     | 01100      | 0x0C |
| D   | x2y4     | 01101      | 0x0D |
| E   | x3y4     | 01110      | 0x0E |
| F   | x4y4     | 01111      | 0x0F |
| +   | x1y5     | 10000      | 0x10 |
| -   | x2y5     | 10001      | 0x11 |
| GO  | x3y5     | 10010      | 0x12 |
| AD  | x4y5     | 10011      | 0x13 |

**Note**: Hex digits 0–F map directly to codes 0x00–0x0F. Function keys (+, -, GO, AD) are 0x10–0x13.

### Keyboard Operation

- Keypress triggers **NMI** interrupt to Z80
- Reading port 00h activates Y0, enabling 74C923 to output key code
- Port 01h can be polled for DAV (Data Available) status

**Example:**
```asm
; Read key code
IN A, (00h)        ; Read 5-bit key code
AND 0x1F           ; Mask to 5 bits
```

## Expansion & Modifications

- **No onboard UART**: Serial communication typically added via bit-banged mod using D6 data line + transistor level shifting to USB-serial adapter
- **Expansion ports**: Often use addresses ~80h–87h (different decoder or higher address bits)
- **Common add-ons**: Relays, LED matrices, additional I/O

## Design Philosophy

The TEC-1 is deliberately minimal for educational purposes:
- Simple partial address decoding
- Direct hardware access via I/O ports
- No complex peripherals or abstractions
- Perfect for learning Z80 assembly and basic computer architecture
