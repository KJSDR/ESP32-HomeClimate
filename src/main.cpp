#include <Arduino.h>
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT22
#define MQ135_PIN 4

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  pinMode(DHTPIN, INPUT_PULLUP);
  pinMode(MQ135_PIN, INPUT);
  dht.begin();
  
  Serial.println("=== Sensor Test ===");
  delay(2000);
}

void loop() {
  delay(3000);
  
  float humidity = dht.readHumidity();
  float tempF = dht.readTemperature(true);
  int airQuality = analogRead(MQ135_PIN);
  
  if (isnan(humidity)) {
    Serial.println("ERROR: Sensor failed");
    return;
  }
  
  Serial.println("\n--- READINGS ---");
  Serial.print("Temperature: ");
  Serial.print(tempF);
  Serial.println("°F");
  
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
  
  Serial.print("Air Quality: ");
  Serial.println(airQuality);
  Serial.println("----------------");
}