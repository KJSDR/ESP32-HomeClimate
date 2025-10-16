#include <Arduino.h>
#include "DHT.h"

#define DHTPIN 2      // GPIO2 (D2 pin)
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  pinMode(DHTPIN, INPUT_PULLUP);
  
  Serial.println("\n\n=== AM2302 Test (GPIO2) ===");
  dht.begin();
  delay(3000);
}

void loop() {
  delay(3000);
  
  float humidity = dht.readHumidity();
  float tempC = dht.readTemperature();
  float tempF = dht.readTemperature(true);
  
  if (isnan(humidity) || isnan(tempC)) {
    Serial.println("❌ Failed to read - trying GPIO2");
    return;
  }
  
  Serial.println("\n✅ SUCCESS!");
  Serial.print("🌡️  Temp: ");
  Serial.print(tempF, 1);
  Serial.print("°F (");
  Serial.print(tempC, 1);
  Serial.println("°C)");
  Serial.print("💧 Humidity: ");
  Serial.print(humidity, 1);
  Serial.println("%");
}