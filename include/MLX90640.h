#ifndef MLX90640_H
#define MLX90640_H

#include <Adafruit_MLX90640.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <Wire.h>

extern TwoWire I2CBUS;  // Tell the compiler I2CBUS is in another file

Adafruit_MLX90640 mlx;
float frame[32*24]; // buffer for full frame of temperatures

const int rows = 24;
const int cols = 32;
float sensorData[rows][cols];


void setupThermalCamera() {
    Serial.println("Adafruit MLX90640 Simple Test");
    if (! mlx.begin(MLX90640_I2CADDR_DEFAULT, &I2CBUS)) {
        Serial.println("MLX90640 not found!");
        while (1) delay(10);
    }
    Serial.println("Found Adafruit MLX90640");

    Serial.print("Serial number: ");
    Serial.print(mlx.serialNumber[0], HEX);
    Serial.print(mlx.serialNumber[1], HEX);
    Serial.println(mlx.serialNumber[2], HEX);

    mlx.setMode(MLX90640_INTERLEAVED);
    //mlx.setMode(MLX90640_CHESS);
    Serial.print("Current mode: ");
    if (mlx.getMode() == MLX90640_CHESS) {
        Serial.println("Chess");
    } else {
        Serial.println("Interleave");
    }

    mlx.setResolution(MLX90640_ADC_19BIT);
    Serial.print("Current resolution: ");
    mlx90640_resolution_t res = mlx.getResolution();
    switch (res) {
        case MLX90640_ADC_16BIT: Serial.println("16 bit"); break;
        case MLX90640_ADC_17BIT: Serial.println("17 bit"); break;
        case MLX90640_ADC_18BIT: Serial.println("18 bit"); break;
        case MLX90640_ADC_19BIT: Serial.println("19 bit"); break;
    }

    mlx.setRefreshRate(MLX90640_32_HZ);
    Serial.print("Current frame rate: ");
    mlx90640_refreshrate_t rate = mlx.getRefreshRate();
    switch (rate) {
        case MLX90640_0_5_HZ: Serial.println("0.5 Hz"); break;
        case MLX90640_1_HZ: Serial.println("1 Hz"); break;
        case MLX90640_2_HZ: Serial.println("2 Hz"); break;
        case MLX90640_4_HZ: Serial.println("4 Hz"); break;
        case MLX90640_8_HZ: Serial.println("8 Hz"); break;
        case MLX90640_16_HZ: Serial.println("16 Hz"); break;
        case MLX90640_32_HZ: Serial.println("32 Hz"); break;
        case MLX90640_64_HZ: Serial.println("64 Hz"); break;
    }
}

void getFrame() {
    if (mlx.getFrame(frame) != 0) {
    Serial.println("Failed to read frame");
    return;
  }

  // Copy 1D frame to 2D sensorData
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      sensorData[i][j] = frame[i * cols + j];
    }
  }
}

String getJSON() {
  StaticJsonDocument<2000> doc;
  JsonArray data = doc.to<JsonArray>();
  
  for (int i = 0; i < rows; i++) {
    JsonArray row = data.createNestedArray();
    for (int j = 0; j < cols; j++) {
      row.add(sensorData[i][j]);
    }
  }

  String jsonStr;
  serializeJson(doc, jsonStr);
  return jsonStr;
}

#endif