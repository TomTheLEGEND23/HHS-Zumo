# HHS-Zumo Control Keys Mapping

## Current Command Layout (No Overlaps Detected)

### CALIBRATION & SETUP:
- **'c'** - Calibrate colors (black/green/brown/white)
- **'v'** - View/display calibration values  
- **'t'** - Test encoders (drive 10cm)

### AUTONOMOUS MODES:
- **'k'** - Start line following
- **'p'** - Start cube pushing mode
- **'o'** - Stop all operations

### MANUAL CONTROL (when not in auto mode):
- **'w'** - Move forward
- **'s'** - Move backward
- **'a'** - Turn left
- **'d'** - Turn right
- **'x'** - Stop motors

### HELP & DIAGNOSTICS:
- **'q'** - Show command list
- **'f'** - Send diagnostics (via XBee)

### BUZZER SOUNDS (number keys):
- **'1'** - Startup sound
- **'2'** - Victory fanfare
- **'3'** - Error sound
- **'4'** - Siren
- **'5'** - Imperial March
- **'6'** - R2-D2 beeps
- **'7'** - R2-D2 scream
- **'8'** - Tetris theme
- **'9'** - Mario theme

## XBee Class Changes Made:
✅ **REMOVED** - All keyboard motor control logic from XBee class
✅ **SIMPLIFIED** - XBee now only handles:
  - Communication (update(), isButtonPressed())
  - Diagnostics ('f' command only)
  - State management (line following program state)

## Key Conflict Analysis:
✅ **NO CONFLICTS** - All commands use unique keys
✅ **CLEAN SEPARATION** - XBee handles communication, main program handles all controls
✅ **SAFE MANUAL CONTROLS** - Manual motor commands disabled during autonomous operation

## Used Keys: c, v, t, k, p, o, w, s, a, d, x, q, f, 1, 2, 3, 4, 5, 6, 7, 8, 9
## Available Keys: b, e, g, h, i, j, l, m, n, r, u, y, z, 0, symbols
