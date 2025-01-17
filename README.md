# IR Fiddler
IR Fiddler is an experimental ESP32-based project to decode IR remote signals using the NEC protocol. The project leverages the ESP32's RMT (Remote Control Transceiver) peripheral for precise timing and signal analysis.

## Features
- Configures an IR receiver (VS1838) to read and process remote control signals.
- Implements NEC protocol decoding for extracting address and command data.
- Demonstrates usage of the ESP32's RMT peripheral for IR signal handling.
- Provides a modular foundation for further experimentation and customization.
  
## Components Used
- ESP-WROOM-32 Development Board
- VS1838 IR Receiver
- NEC-compatible remote control
  
## Prerequisites
- ESP-IDF installed and configured.
- Basic understanding of IR communication protocols (e.g., NEC).
- Familiarity with FreeRTOS and ESP32 development.
  
## Usage
1. Connect the VS1838 IR Receiver to the ESP32 GPIO specified in the code (IR_RECEIVE_GPIO is set to GPIO 26 by default).
2. Build and flash the code using ESP-IDF:
```bash
idf.py build flash monitor
```
3. Use an NEC-compatible remote control to send signals to the receiver. Decoded addresses and commands will be logged to the console.
   
## Code Highlights
- RMT Configuration: The RMT peripheral is configured with a 1 MHz clock for precise signal timing.
- Callback Mechanism: RMT data is processed via an ISR callback and queued for analysis.
- NEC Decoding: The print_nec_frame function extracts address and command information based on signal timing.
  
## Future Improvements
- Support for additional IR protocols.
- Enhanced signal filtering and error handling.
- Integration with other peripherals (e.g., LEDs or displays) for visual feedback.
  
## Disclaimer
This project is intended for educational purposes and is provided as-is. Contributions and suggestions are welcome!