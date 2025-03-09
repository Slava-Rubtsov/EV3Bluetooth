#ifndef EV3_BLUETOOTH_H
#define EV3_BLUETOOTH_H

#include "Arduino.h"
#include "BluetoothSerial.h"

class EV3Bluetooth {
public:
    EV3Bluetooth();
    void begin(const char* deviceName);
    void TextMailbox(const char* addr, const char* value);
    void LogicMailbox(const char* addr, bool value);  
	void NumericMailbox(const char* addr, float value);
	String readTextMailbox();
	bool readLogicMailbox();
	float readNumericMailbox();
private:
    BluetoothSerial SerialBT;
};

#endif
