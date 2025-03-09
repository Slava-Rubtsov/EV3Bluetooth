#include "EV3Bluetooth.h"

EV3Bluetooth::EV3Bluetooth() {}
// Initialize Bluetooth with the specified device name
void EV3Bluetooth::begin(const char* deviceName) { 
    SerialBT.begin(deviceName);
}
// Send a text message to the EV3 mailbox
void EV3Bluetooth::TextMailbox(const char* addr, const char* value) { // text send to EV3
    uint8_t len1 = strlen(addr);  // Box name length
    uint8_t len2 = strlen(value); // Text length
    uint8_t totalLength = len1 + len2 + 10;  // Total message length including service bytes

    if (totalLength > 128) {  // Check for exceeding the maximum packet size
        Serial.println("Error: Message too long!");
        return;
    }

    uint8_t data[128] = {0};  // Data buffer for sending
    data[0] = totalLength;
    data[1] = 0x00;
    data[2] = 0x01;
    data[3] = 0x00;
    data[4] = 0x81;
    data[5] = 0x9E;
    data[6] = len1 + 1;
    memcpy(&data[7], addr, len1);
    data[7 + len1] = 0x00;
    data[8 + len1] = len2 + 1;
    data[9 + len1] = 0x00;
    memcpy(&data[10 + len1], value, len2);
    data[11 + len1 + len2] = 0x00;

    SerialBT.write(data, totalLength + 2); // Sending a packet including additional EV3 bytes
}
// Sending a logical value to the EV3 mailbox
void EV3Bluetooth::LogicMailbox(const char* addr, bool value) { // binary send to EV3
    uint8_t len1 = strlen(addr);
    uint8_t totalLength = len1 + 9;  

    if (totalLength > 128) {  
        Serial.println("Error: Message too long!");
        return;
    }

    uint8_t data[128] = {0};  
    data[0] = totalLength;
    data[1] = 0x00;
    data[2] = 0x01;
    data[3] = 0x00;
    data[4] = 0x81;
    data[5] = 0x9E;
    data[6] = len1 + 1;
    memcpy(&data[7], addr, len1);
    data[7 + len1] = 0x00;
    data[8 + len1] = 0x01;
    data[9 + len1] = 0x00;
    data[10 + len1] = value ? 1 : 0;  // Write a logical value (1 - true, 0 - false)

    SerialBT.write(data, totalLength + 2);
}
// Send a numeric value (float) to the EV3 mailbox
void EV3Bluetooth::NumericMailbox(const char* addr, float value) { 
    uint8_t len1 = strlen(addr);
    uint8_t len2 = 4; // The number is transmitted in 4 bytes (IEEE 754 float)
    uint8_t totalLength = len1 + len2 + 9;  // Total message length

    if (totalLength > 128) {  
        Serial.println("Error: Message too long!");
        return;
    }

    uint8_t data[128] = {0}; 

    uint8_t ieeeBytes[4]; // Buffer for storing float in IEEE 754
    memcpy(ieeeBytes, &value, sizeof(float)); // Convert float to byte array

    data[0] = totalLength;
    data[1] = 0x00;
    data[2] = 0x01;
    data[3] = 0x00;
    data[4] = 0x81;
    data[5] = 0x9E;
    data[6] = len1 + 1;
    memcpy(&data[7], addr, len1);
    data[7 + len1] = 0x00;
    data[8 + len1] = len2;
    data[9 + len1] = 0x00;
    memcpy(&data[10 + len1], ieeeBytes, len2); // Copy float value into the package

    SerialBT.write(data, totalLength+2);
}

// Read a text message from the EV3 mailbox
String EV3Bluetooth::readTextMailbox() {
    if (!SerialBT.available()) return "";

    uint8_t buffer[128] = {0};
    
    // Read the first 2 bytes (message length)
    size_t bytesRead = SerialBT.readBytes(buffer, 2);
    if (bytesRead < 2) return "";

    uint8_t totalLength = buffer[0];
    uint8_t remainingLength = totalLength;

    // Read the remaining bytes
    bytesRead += SerialBT.readBytes(buffer + 2, remainingLength);

    if (bytesRead < totalLength + 2) return "";

    // Read the length of the mailbox name
    uint8_t mailboxNameLen = buffer[6];
    uint8_t dataStart = 7 + mailboxNameLen;

    // Text length
    uint8_t textLen = buffer[dataStart];
    uint8_t textStart = dataStart + 2;

    char text[129] = {0};
    memcpy(text, &buffer[textStart], textLen - 1); // Without the EV3 zero terminator
    return String(text);
}

// Reading a logical value to the EV3 mailbox
bool EV3Bluetooth::readLogicMailbox() {
    if (!SerialBT.available()) return false;

    uint8_t buffer[128] = {0};
    
    size_t bytesRead = SerialBT.readBytes(buffer, 2);
    if (bytesRead < 2) return false;

    uint8_t totalLength = buffer[0];
    uint8_t remainingLength = totalLength;

    bytesRead += SerialBT.readBytes(buffer + 2, remainingLength);
    if (bytesRead < totalLength + 2) return false;

    uint8_t mailboxNameLen = buffer[6];
    uint8_t dataStart = 7 + mailboxNameLen;

    
    uint8_t payloadLen = buffer[dataStart];
    uint8_t valueStart = dataStart + 2;

    return buffer[valueStart] != 0;
}

// Reading a num value to the EV3 mailbox
float EV3Bluetooth::readNumericMailbox() {
    if (!SerialBT.available()) return 0.0;

    uint8_t buffer[128] = {0};
    
    size_t bytesRead = SerialBT.readBytes(buffer, 2);
    if (bytesRead < 2) return 0.0;

    uint8_t totalLength = buffer[0];
    uint8_t remainingLength = totalLength;

    bytesRead += SerialBT.readBytes(buffer + 2, remainingLength);
    if (bytesRead < totalLength + 2) return 0.0;

    uint8_t mailboxNameLen = buffer[6];
    uint8_t dataStart = 7 + mailboxNameLen;

    uint8_t payloadLen = buffer[dataStart];
    uint8_t valueStart = dataStart + 2;

    float result = 0.0;
    memcpy(&result, &buffer[valueStart], sizeof(float));

    return result;
}
