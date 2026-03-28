/* 
 * 📌 HOW TO FIX COMPILATION ERRORS:
 * 1. Install Arduino IDE or VS Code with PlatformIO.
 * 2. Install "Blynk" library from Library Manager.
 * 3. Install "ESP8266" board support from Board Manager.
 * 4. Select your board (e.g., NodeMCU 1.0) before uploading.
 */

/* Fill in information from Blynk Device Info here (MUST be at the TOP) */
#define BLYNK_TEMPLATE_ID           "TMPLxxxxxx"
#define BLYNK_TEMPLATE_NAME         "Smart Irrigation"
#define BLYNK_AUTH_TOKEN            "YourAuthToken"

/**
 * @file smart-irrigation-iot.ino
 * @author Atharv Pokale
 * @brief IoT-based Smart Irrigation System using ESP8266 and Blynk IoT platform.
 * @version 1.1
 * @date 2026-03-27
 */

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// --- Configuration ---
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "YourWiFiName";     // Replace with your WiFi SSID
char pass[] = "YourWiFiPassword"; // Replace with your WiFi Password

const int SOIL_SENSOR_PIN = A0;
const int RELAY_PIN = D1;

BlynkTimer timer;

// --- Functions ---

// Function to send sensor data to Blynk App
void sendSensorData() {
  int soilMoistureRaw = analogRead(SOIL_SENSOR_PIN);
  
  // Convert to Percentage (0-1024 to 0-100%)
  // Dry value is usually high (~1024), Wet value is low (~200)
  int soilMoisturePercent = map(soilMoistureRaw, 1024, 200, 0, 100);
  soilMoisturePercent = constrain(soilMoisturePercent, 0, 100);

  // Send to Blynk Virtual Pin V1 (Gauge/Value Display)
  Blynk.virtualWrite(V1, soilMoisturePercent);

  // Auto-irrigation logic
  if (soilMoisturePercent < 30) {
    digitalWrite(RELAY_PIN, HIGH); // Turn ON pump
    // Note: "soil_dry" must be created in Blynk Console -> Events
    // Blynk.logEvent("soil_dry", "Watering started! Soil is dry."); 
  } else if (soilMoisturePercent > 70) {
    digitalWrite(RELAY_PIN, LOW);  // Turn OFF pump
  }
}

void setup() {
  Serial.begin(115200);
  
  // Connect to Blynk
  // If using a different board like ESP32, you need a different library
  Blynk.begin(auth, ssid, pass);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  // Setup a function to be called every 2 seconds
  timer.setInterval(2000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
}
