# control_sprinkler_pump

# Build
```
platformio run
```

# Upload
```
platformio run --target upload
```

Processing megaatmega2560 (platform: atmelavr; board: megaatmega2560; framework: arduino)
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Verbose mode can be enabled via `-v, --verbose` option
CONFIGURATION: https://docs.platformio.org/page/boards/atmelavr/megaatmega2560.html
PLATFORM: Atmel AVR 2.0.0 > Arduino Mega or Mega 2560 ATmega2560 (Mega 2560)
HARDWARE: ATMEGA2560 16MHz, 8KB RAM, 248KB Flash
PACKAGES: toolchain-atmelavr 1.50400.190710 (5.4.0), framework-arduino-avr 5.0.0, tool-avrdude 1.60300.190424 (6.3.0)
Converting main.ino
LDF: Library Dependency Finder -> http://bit.ly/configure-pio-ldf
LDF Modes: Finder ~ chain, Compatibility ~ soft
Found 12 compatible libraries
Scanning dependencies...
Dependency Graph
|-- <Adafruit TouchScreen> 1.0.4
|-- <Adafruit GFX Library> 1.5.3
|   |-- <SPI> 1.0
|-- <Adafruit TFTLCD Library> 1.0.0 #94d08c2
|   |-- <Adafruit GFX Library> 1.5.3
|   |   |-- <SPI> 1.0
|-- <SD> 1.2.3 #98b9b57
|   |-- <SPI> 1.0
|-- <MCUFRIEND_kbv> 2.9.8 #fd3464e
|   |-- <Adafruit GFX Library> 1.5.3
|   |   |-- <SPI> 1.0
|   |-- <SPI> 1.0
Building in release mode
Compiling .pio\build\megaatmega2560\src\main.ino.cpp.o
Linking .pio\build\megaatmega2560\firmware.elf
Checking size .pio\build\megaatmega2560\firmware.elf
Advanced Memory Usage is available via "PlatformIO Home > Project Inspect"
DATA:    [=         ]   6.2% (used 505 bytes from 8192 bytes)
PROGRAM: [=         ]   9.8% (used 24974 bytes from 253952 bytes)
Configuring upload protocol...
AVAILABLE: wiring
CURRENT: upload_protocol = wiring
Looking for upload port...
Auto-detected: COM8
Uploading .pio\build\megaatmega2560\firmware.hex

avrdude: AVR device initialized and ready to accept instructions

Reading | ################################################## | 100% 0.01s

avrdude: Device signature = 0x1e9801 (probably m2560)
avrdude: reading input file ".pio\build\megaatmega2560\firmware.hex"
avrdude: writing flash (24974 bytes):

Writing | ################################################## | 100% 4.01s

avrdude: 24974 bytes of flash written
avrdude: verifying flash memory against .pio\build\megaatmega2560\firmware.hex:
avrdude: load data flash data from input file .pio\build\megaatmega2560\firmware.hex:
avrdude: input file .pio\build\megaatmega2560\firmware.hex contains 24974 bytes
avrdude: reading on-chip flash data:

Reading | ################################################## | 100% 3.12s

avrdude: verifying ...
avrdude: 24974 bytes of flash verified

avrdude: safemode: Fuses OK (E:FD, H:D8, L:FF)

avrdude done.  Thank you.

======================================================================================== [SUCCESS] Took 9.55 seconds ========================================================================================