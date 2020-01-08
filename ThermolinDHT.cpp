#include "ThermolinDHT.h"
#include "Init.h"
#include "Pinout.h"

float  DHT_temperature;
float  DHT_humidity;

void DHT_read()
{
  static unsigned long previousMillis = 0;
  
  if(millis() - previousMillis > 2000)
  {
      //if(DHT_temperature)
      //{
      DHT_temperature = dht.getTemperature();
      DHT_humidity = dht.getHumidity();
      previousMillis = millis();
      //}
    /*else //reinitialize sensor
    {
      dht.setup(PIN_DHT, DHTesp::DHT22);
      delay(100);
      DHT_temperature = dht.getTemperature();
      DHT_humidity = dht.getHumidity();
    }*/
  }
}
