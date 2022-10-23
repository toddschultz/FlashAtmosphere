/*
  MKR ENV Shield - Read Sensors

  This example reads the sensors on-board the MKR ENV Shield
  and prints them to the Serial Monitor once a second.

  The circuit:
  - Arduino MKR board
  - Arduino MKR ENV Shield attached

  This example code is in the public domain.

   WriteMultipleFields
  
  Description: Writes values to fields 1,2,3,4 and status in a single ThingSpeak update every 20 seconds.
  
  Hardware: Arduino MKR WiFi 1010
  
  !!! IMPORTANT - Modify the secrets.h file for this project with your network connection and ThingSpeak channel details. !!!
  
  Note:
  - Requires WiFiNINA library
  - This example is written for a network using WPA encryption. For WEP or WPA, change the WiFi.begin() call accordingly.
  
  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize, and 
  analyze live data streams in the cloud. Visit https://www.thingspeak.com to sign up for a free account and create a channel.  
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the README.md folder where the library was installed.
  See https://www.mathworks.com/help/thingspeak/index.html for the full ThingSpeak documentation.
  
  For licensing information, see the accompanying license file.
  
  Copyright 2020, The MathWorks, Inc.
*/

// Sensor stuff
#include <Arduino_MKRENV.h>

// ENV temperature offset correction
const float deltatempC = 11*5/9;

// Wifi stuff
#include <WiFiNINA.h>
#include "secrets.h"

// ThingSpeak stuff
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

WiFiClient client;

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PWD;   // your network passwords

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Need 20 seconds between writes to ThingSpeak
// 20000 / 100 = 200
// Number of samples for averaging
const float numSamples = 100;
// Time delay between samples in  milliseconds
const unsigned long sampletime = 200;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV Shield!");
    while (1);
  }

  // Connect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  // Initialize ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  // Connect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
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
    delay(sampletime);
  }  
  //Correct temperature bias
  temperature = temperature - deltatempC;
    
  // set the fields with the values
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, pressure);
  ThingSpeak.setField(4, illuminance);
  ThingSpeak.setField(5, uva);
  ThingSpeak.setField(6, uvb);
  ThingSpeak.setField(7, uvIndex);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  // print each of the sensor values
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" °C");
  
  Serial.print("Temperature = ");
  Serial.print(temperature*9/5+32);
  Serial.println(" °F");

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
}
