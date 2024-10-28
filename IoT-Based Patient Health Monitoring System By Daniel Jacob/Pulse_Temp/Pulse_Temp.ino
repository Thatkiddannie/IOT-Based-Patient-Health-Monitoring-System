#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into pin 8 on the Arduino board
#define ONE_WIRE_BUS 8
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Pulse sensor connected to analog pin A0
const int pulsePin = A0;
int pulseValue = 0;
int threshold = 520; // Adjust this threshold as needed based on your sensor

// Variables for BPM calculation
unsigned long lastBeatTime = 0;
int beatCount = 0;
float BPM = 0;

void setup() {
  // Start the serial communication
  Serial.begin(9600);

  // Start the temperature sensor library
  sensors.begin();

  // Initialize the last beat time
  lastBeatTime = millis();
}

void loop() {
  // Read the pulse sensor value
  pulseValue = analogRead(pulsePin);

  // Check if the pulse value exceeds the threshold (indicating a beat)
  if (pulseValue > threshold) {
    // Ensure it's a valid beat by waiting for a small interval (debouncing)
    if (millis() - lastBeatTime > 250) { // 250ms debounce period
      beatCount++;
      lastBeatTime = millis();
    }
  }

  // Calculate BPM every 60 seconds
  if (millis() - lastBeatTime >= 60000) {
    BPM = (beatCount / 60.0) * 60.0; // Beats per minute
    beatCount = 0; // Reset beat count for the next period
    lastBeatTime = millis();
  }

  // Request temperature measurements
  sensors.requestTemperatures();
  
  // Fetch the temperature in Celsius 
  float temperatureC = sensors.getTempCByIndex(0);

  // Print the pulse sensor value and BPM
  Serial.print("Pulse Value: ");
  Serial.print(pulseValue);
  Serial.print(" | BPM: ");
  Serial.println(BPM);

  // Print the temperature value
  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.println(" °C");

  // Wait for a bit before taking another reading
  delay(10000);
}
