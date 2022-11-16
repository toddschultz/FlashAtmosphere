// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include <DHT.h>
#include <Wire.h>

#define DHTPIN 0     // what digital pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);


void setup() 
{
    Serial.begin(115200); 
    while (!Serial);
    Serial.println("DHTxx test!");
    Wire.begin();

    /*if using WIO link,must pull up the power pin.*/
    // pinMode(PIN_GROVE_POWER, OUTPUT);
    // digitalWrite(PIN_GROVE_POWER, 1);

    dht.begin();
}

void loop() 
{
    float temperature = 0;
    float humidity    = 0;
  //Read temperature and humidity
    do {
      humidity = dht.readHumidity();
      temperature = dht.readTemperature();
    }
    while (isnan(temperature) || isnan(humidity));
    
        Serial.print("Humidity: "); 
        Serial.print(humidity);
        Serial.print(" %\t");
        Serial.print("Temperature: "); 
        Serial.print(temperature);
        Serial.println(" °C");

   delay(1500);
}
