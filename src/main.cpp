#include <Arduino.h>
#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin Definitions
#define DHTPIN 2
#define DHTTYPE DHT22
#define LED_RED 23
#define LED_GREEN 19
#define MQ135_PIN 4

// Thresholds
#define TEMP_MIN 60.0
#define TEMP_MAX 78.0
#define HUMIDITY_MAX 70.0
#define AIR_QUALITY_THRESHOLD 400  // Lower = worse air

// Initialize
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Try 0x27 first, if fails try 0x3F

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Initialize pins
  pinMode(DHTPIN, INPUT_PULLUP);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(MQ135_PIN, INPUT);
  
  // Turn LEDs off
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  
  // Initialize sensors
  dht.begin();
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart Window");
  lcd.setCursor(0, 1);
  lcd.print("Monitor v1.0");
  
  Serial.println("\n=== Smart Environment Monitor ===");
  Serial.println("Initializing sensors...");
  
  delay(3000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Warming up...");
  
  // MQ-135 needs warm-up time
  for(int i = 10; i > 0; i--) {
    lcd.setCursor(0, 1);
    lcd.print(i);
    lcd.print(" seconds   ");
    delay(1000);
  }
  
  lcd.clear();
  Serial.println("System ready!");
}

void loop() {
  delay(3000);
  
  // Read DHT22
  float humidity = dht.readHumidity();
  float tempC = dht.readTemperature();
  float tempF = dht.readTemperature(true);
  
  // Read MQ-135
  int airQuality = analogRead(MQ135_PIN);
  
  // Check for sensor errors
  if (isnan(humidity) || isnan(tempC)) {
    Serial.println("❌ DHT22 Error!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!");
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    return;
  }
  
  // Evaluate conditions
  bool alert = false;
  String status = "GOOD";
  
  if (tempF < TEMP_MIN || tempF > TEMP_MAX) {
    alert = true;
    status = "TEMP ALERT!";
  } else if (humidity > HUMIDITY_MAX) {
    alert = true;
    status = "HUMID ALERT!";
  } else if (airQuality < AIR_QUALITY_THRESHOLD) {
    alert = true;
    status = "AIR ALERT!";
  }
  
  // Control LEDs
  if (alert) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
  } else {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
  }
  
  // Update LCD - Line 1: Readings
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print((int)tempF);
  lcd.print("F H:");
  lcd.print((int)humidity);
  lcd.print("%");
  
  // Update LCD - Line 2: Status
  lcd.setCursor(0, 1);
  if (alert) {
    lcd.print("CLOSE WINDOW!");
  } else {
    lcd.print("Window OK");
  }
  
  // Print to Serial
  Serial.println("\n========== Readings ==========");
  Serial.print("🌡️  Temperature: ");
  Serial.print(tempF, 1);
  Serial.println("°F");
  
  Serial.print("💧 Humidity: ");
  Serial.print(humidity, 1);
  Serial.println("%");
  
  Serial.print("💨 Air Quality: ");
  Serial.print(airQuality);
  Serial.println(" (lower = worse)");
  
  if (alert) {
    Serial.println("🚨 STATUS: ALERT - CLOSE WINDOW!");
  } else {
    Serial.println("✅ STATUS: GOOD - Window OK");
  }
  Serial.println("==============================");
}