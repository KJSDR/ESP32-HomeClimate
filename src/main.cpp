#include <Arduino.h>
#include "DHT.h"

// Pin Definitions
#define DHTPIN 2          // DHT22 data pin connected to GPIO 2
#define DHTTYPE DHT22     // DHT22 sensor type
#define MQ135_PIN 4       // MQ-135 analog output connected to GPIO 4

// Sensor Thresholds
#define TEMP_MIN 60.0    
#define TEMP_MAX 78.0     
#define HUMIDITY_MAX 70.0 
#define AIR_QUALITY_MIN 500  

// Initialize DHT22 sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(2000);
  
  // Initialize sensor pins
  pinMode(DHTPIN, INPUT_PULLUP);
  pinMode(MQ135_PIN, INPUT);
  
  // Initialize DHT22 sensor
  dht.begin();
  
  // Print system header
  Serial.println("\n\n");
  Serial.println("╔═══════════════════════════════════════╗");
  Serial.println("║  SMART WINDOW MONITORING SYSTEM v1.0  ║");
  Serial.println("║     Multi-Sensor Environmental Demo   ║");
  Serial.println("╚═══════════════════════════════════════╝");
  Serial.println();
  Serial.println("📡 Sensors Online:");
  Serial.println("   • DHT22 - Temperature & Humidity");
  Serial.println("   • MQ-135 - Air Quality Sensor");
  Serial.println();
  
  delay(2000);
  
  // Warm-up period for MQ-135
  Serial.println("🔄 Warming up air quality sensor...");
  Serial.println("   (Note: MQ-135 requires 24-48h for full calibration)");
  
  for(int i = 10; i > 0; i--) {
    Serial.print("   ");
    Serial.print(i);
    Serial.println(" seconds remaining...");
    delay(1000);
  }
  
  Serial.println("\n✅ System ready! Starting monitoring...\n");
  delay(1000);
}

void loop() {
  // Delay between readings
  delay(3000);
  
  // Read DHT22 
  float humidity = dht.readHumidity();
  float tempC = dht.readTemperature();
  float tempF = dht.readTemperature(true); 
  
  // Read MQ-135 value 0-4095
  int airQuality = analogRead(MQ135_PIN);
  
  // Check for DHT22 sensor errors
  if (isnan(humidity) || isnan(tempC)) {
    Serial.println("❌ ERROR: DHT22 sensor read failed!");
    Serial.println("   Check wiring and connections.\n");
    return;
  }
  
  // Display sensor readings
  Serial.println("══════════════ SENSOR READINGS ══════════════");
  Serial.println();
  
  Serial.print("🌡️  Temperature:  ");
  Serial.print(tempF, 1);
  Serial.print("°F  (");
  Serial.print(tempC, 1);
  Serial.println("°C)");
  
  Serial.print("💧 Humidity:     ");
  Serial.print(humidity, 1);
  Serial.println("%");
  
  Serial.print("💨 Air Quality:  ");
  Serial.print(airQuality);
  Serial.println(" (0-4095 scale)");
  Serial.println("                 Lower values indicate worse air quality");
  
  Serial.println();
  Serial.println("─────────────────────────────────────────────");
  
  // Analyze environmental conditions
  bool tempTooHot = (tempF > TEMP_MAX);
  bool tempTooCold = (tempF < TEMP_MIN);
  bool humidityHigh = (humidity > HUMIDITY_MAX);
  bool airPoor = (airQuality < AIR_QUALITY_MIN);
  
  Serial.println("📊 CONDITION ANALYSIS:");
  Serial.println();
  
  // Temperature analysis
  if (tempTooHot) {
    Serial.print("   🔴 Temperature too high (>");
    Serial.print(TEMP_MAX, 0);
    Serial.println("°F)");
  } else if (tempTooCold) {
    Serial.print("   🔵 Temperature too low (<");
    Serial.print(TEMP_MIN, 0);
    Serial.println("°F)");
  } else {
    Serial.println("   ✅ Temperature in optimal range");
  }
  
  // Humidity analysis
  if (humidityHigh) {
    Serial.print("   🟡 Humidity too high (>");
    Serial.print(HUMIDITY_MAX, 0);
    Serial.println("%)");
  } else {
    Serial.println("   ✅ Humidity in acceptable range");
  }
  
  // Air quality analysis
  if (airPoor) {
    Serial.print("   🔴 Poor air quality detected (<");
    Serial.print(AIR_QUALITY_MIN);
    Serial.println(")");
  } else {
    Serial.println("   ✅ Air quality acceptable");
  }
  
  Serial.println();
  Serial.println("─────────────────────────────────────────────");
  
  // Generate recommendation
  Serial.print("💡 RECOMMENDATION:  ");
  
  if (tempTooHot || tempTooCold || humidityHigh || airPoor) {
    Serial.println("⚠️  CLOSE WINDOW");
    Serial.println();
    Serial.println("   Reasoning: Environmental conditions are");
    Serial.println("   outside optimal parameters. Closing the");
    Serial.println("   window will help maintain indoor comfort");
    Serial.println("   and air quality.");
  } else {
    Serial.println("✅ WINDOW CAN STAY OPEN");
    Serial.println();
    Serial.println("   Reasoning: All environmental parameters");
    Serial.println("   are within acceptable ranges. Fresh air");
    Serial.println("   circulation is beneficial.");
  }
  
  Serial.println("═════════════════════════════════════════════");
  Serial.println("\n");
}