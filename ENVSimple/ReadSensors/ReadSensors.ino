/*
  MKR ENV Shield - Read Sensors

  This example reads the sensors on-board the MKR ENV Shield
  and prints them to the Serial Monitor once a second.

  The circuit:
  - Arduino MKR board
  - Arduino MKR ENV Shield attached

  This example code is in the public domain.
*/

#include <Arduino_MKRENV.h>

const float numSamples = 100;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV Shield!");
    while (1);
  }
}

void loop() {
  // read all the sensor values
  //float temperature = ENV.readTemperature();
  float temperature = 0;
  float humidity    = 0;
  float pressure    = 0;
  float illuminance = 0;
  float uva         = 0;
  float uvb         = 0;
  float uvIndex     = 0;

  //Get temperature values and average
  for (int isample = 0; isample < numSamples; ++isample) {
    temperature = temperature + ENV.readTemperature() / numSamples;
    humidity = humidity + ENV.readHumidity() / numSamples;
    pressure = pressure + ENV.readPressure() / numSamples; 
    illuminance = illuminance + ENV.readIlluminance() / numSamples;
    uva = uva + ENV.readUVA() / numSamples;
    uvb = uvb + ENV.readUVB() / numSamples;
    uvIndex = uvIndex + ENV.readUVIndex() / numSamples;
    delay(10);
  }  
  
  // print each of the sensor values
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity    = ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Pressure    = ");
  Serial.print(pressure);
  Serial.println(" kPa");

  Serial.print("Illuminance = ");
  Serial.print(illuminance);
  Serial.println(" lx");

  Serial.print("UVA         = ");
  Serial.println(uva);

  Serial.print("UVB         = ");
  Serial.println(uvb);

  Serial.print("UV Index    = ");
  Serial.println(uvIndex);

  // print an empty line
  Serial.println();

  // wait 1 second to print again
  //delay(1000);
}
