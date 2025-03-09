#include <EV3Bluetooth.h>

EV3Bluetooth ev3;

void setup() {
  Serial.begin(115200);
  ev3.begin("ESP32_BT"); // Bluetooth device name ESP32 (ESP32 is a server)

  delay(1000); 
  
  ev3.TextMailbox("status", "Ready!");
  ev3.LogicMailbox("connected", true);
  ev3.NumericMailbox("battery", 7.4);
}

void loop() {
}