/**
 * @file smart-irrigation.ino
 * @author Atharv Pokale
 * @brief Smart Irrigation System using ESP8266 and Soil Moisture Sensor.
 * @version 1.0
 * @date 2026-03-27
 * 
 * @copyright Copyright (c) 2026 Atharv Pokale
 */

#include <Arduino.h>

// --- Configuration ---
const int SOIL_SENSOR_PIN = A0;   // Analog pin for soil moisture sensor
const int RELAY_PIN = D1;        // Digital pin for relay module (Pump control)

// --- Calibration Values (Adjust these based on your sensor) ---
const int DRY_THRESHOLD = 30;    // Moisture percentage below which pump turns ON
const int WET_THRESHOLD = 70;    // Moisture percentage above which pump turns OFF

// --- Global Variables ---
int soilMoistureRaw = 0;
int soilMoisturePercent = 0;

void setup() {
  // Initialize Serial communication
  Serial.begin(115200);
  delay(1000);
  
  // Set Pin Modes
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);  // Ensure pump is OFF at start

  Serial.println("------------------------------------");
  Serial.println("🌱 Smart Irrigation System Started!");
  Serial.println("------------------------------------");
}

void loop() {
  // 1. Read Soil Moisture (Analog)
  soilMoistureRaw = analogRead(SOIL_SENSOR_PIN);
  
  // 2. Convert to Percentage (0-1024 to 0-100%)
  // Note: Calibration values might need adjustment (Dry: ~1024, Wet: ~200)
  soilMoisturePercent = map(soilMoistureRaw, 1024, 200, 0, 100);
  soilMoisturePercent = constrain(soilMoisturePercent, 0, 100);

  // 3. Print Data to Serial Monitor
  Serial.print("Soil Moisture: ");
  Serial.print(soilMoisturePercent);
  Serial.println("%");

  // 4. Decision Logic (Automatic Irrigation)
  if (soilMoisturePercent < DRY_THRESHOLD) {
    Serial.println("⚠️ Soil is DRY! Turning Pump ON...");
    digitalWrite(RELAY_PIN, HIGH);  // Activate Relay
  } 
  else if (soilMoisturePercent > WET_THRESHOLD) {
    Serial.println("✅ Soil is WET! Turning Pump OFF...");
    digitalWrite(RELAY_PIN, LOW);   // Deactivate Relay
  }

  // 5. Wait for 5 seconds before next reading
  delay(5000);
}
