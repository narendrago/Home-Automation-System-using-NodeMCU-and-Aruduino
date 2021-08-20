#include <OneWire.h>
#include <DallasTemperature.h>

#define D1 5 // fan assigning the ESP8266 pin to arduino pin
#define ONE_WIRE_BUS 12 // thermometer

int fanPin = 5;
int dutyCycle = 0;

// Setup a oneWire instance to communicate with OneWire device
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);

  pinMode(fanPin, OUTPUT); // sets the pins as outputs:

  sensors.begin(); // Start up the library

  analogWriteRange(100); // to have a range 1 - 100 for the fan
  analogWriteFreq(10000);
}

float readSensorTemp() {
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.print("Temperature: ");  
  Serial.println(sensors.getTempCByIndex(0));
  return sensors.getTempCByIndex(0);
}

void controlFanSpeed (int fanSpeedPercent) {
  Serial.print("Fan Speed: ");
  Serial.print(fanSpeedPercent);
  Serial.println("%");  
  analogWrite(fanPin, fanSpeedPercent); // set the fan speed
}

void loop() {
  float sensorTemp = readSensorTemp(); // Request sensor value

  // Map (change) the sensor reading of <=5 to >=60 to a value between 10 and 100
  int fanSpeedPercent = map(sensorTemp, 5, 60, 10, 100);

  controlFanSpeed (fanSpeedPercent); // Update fan speed
  delay(1000);
}
