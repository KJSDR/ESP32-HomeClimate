#include <Arduino.h>
#include "DHT.h"

#define DHTPIN 4      
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n=== AM2302 Temperature & Humidity Sensor Test ===");
  dht.begin();
  delay(2000); 
}

void loop() {
  delay(3000); 
  
  // to read sens
  float humidity = dht.readHumidity();
  float tempC = dht.readTemperature();
  float tempF = dht.readTemperature(true);  
  
  // error check
  if (isnan(humidity) || isnan(tempC)) {
    Serial.println("❌ ERROR: Failed to read from sensor!");
    Serial.println("   Check your wiring:");
    Serial.println("   + pin → ESP32 3.3V");
    Serial.println("   OUT pin → ESP32 GPIO4");
    Serial.println("   - pin → ESP32 GND");
    return;
  }
  
  // print
  Serial.println("\n========== Sensor Reading ==========");
  Serial.print("🌡️  Temperature: ");
  Serial.print(tempF, 1);
  Serial.print("°F  (");
  Serial.print(tempC, 1);
  Serial.println("°C)");
  
  Serial.print("💧 Humidity:    ");
  Serial.print(humidity, 1);
  Serial.println("%");
  Serial.println("====================================");
}