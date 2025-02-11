#include <Arduino.h>
#include <Wire.h>
#include "MLX90640.h"
#include "Wifi_config.h"

const int i2cSpeed = 1000000; // 1 MHz
TwoWire I2CBUS = TwoWire(0);  // Create a new I2C instance


void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Booting up...");

  I2CBUS.begin();  
  I2CBUS.setClock(i2cSpeed);  // Set I2C speed to 1MHz

  setupThermalCamera();
  setupWifi();
}

void loop() {
  getFrame(); 
  server.handleClient();
}
