#include <Arduino.h>
#include "config.h"
#include "sensors.h"
#include "display.h"
#include "logic.h"

void setup() {
  // Initialize serial communication
  Serial.begin(SERIAL_BAUD);
  delay(2000);
  
  // Print header
  printHeader();
  
  // Initialize all modules
  Serial.println("🔄 Initializing system...\n");
  initSensors();
  initDisplay();
  
  Serial.println("\n✅ System ready! Starting monitoring...\n");
  delay(2000);
}

void loop() {
  // Read all sensors
  float temperature = readTemperature();
  float humidity = readHumidity();
  int airQuality = readAirQuality();
  
  // Check if sensor data is valid
  if (!isSensorDataValid(temperature, humidity)) {
    Serial.println("❌ ERROR: Sensor read failed!\n");
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    delay(UPDATE_INTERVAL);
    return;
  }
  
  // Print readings
  printReadings(temperature, humidity, airQuality);
  
  // Analyze conditions
  bool alert = shouldCloseWindow(temperature, humidity, airQuality);
  
  // Update outputs
  updateLEDs(alert);
  printRecommendation(alert);
  
  // Wait before next reading
  delay(UPDATE_INTERVAL);
}