# EV3Bluetooth

This library allows an ESP32 device to communicate with a LEGO Mindstorms EV3 brick over Bluetooth Serial. You can send and receive messages to/from EV3 mailboxes using Text, Logic, and Numeric formats.

## Features
✅ Send Text messages  
✅ Send Logic (Boolean) messages  
✅ Send Numeric (Float) messages  
✅ Read messages from EV3 mailboxes (soon...)

---

## Installation

1. Download or clone the repository.
2. Copy the `EV3Bluetooth` folder into your Arduino `libraries` directory.
3. Restart the Arduino IDE.

---

## Usage

### Include the library
```cpp
#include <EV3Bluetooth.h>

EV3Bluetooth ev3;
```

### Initialize Bluetooth
```cpp
void setup() {
  Serial.begin(115200);
  ev3.begin("ESP32_BT"); // Set your ESP32 device name
}
```

### Send Text Message
```cpp
ev3.TextMailbox("greeting", "Hello EV3!");
```

### Send Logic Message
```cpp
ev3.LogicMailbox("isReady", true);
```

### Send Numeric Message
```cpp
ev3.NumericMailbox("speed", 3.14);
```
## Links:
### [Installing EV3 MicroPython](https://education.lego.com/en-us/product-resources/mindstorms-ev3/teacher-resources/python-for-ev3/)

### [EV3 MicroPython documentation](https://pybricks.com/ev3-micropython/)


![EV3_image](https://github.com/user-attachments/assets/66056f07-e7c4-4ea8-aac9-8e5185967bec)
