#include "ThermolinDHT.h"
#include "Init.h"

float  DHT_temperature;
float  DHT_humidity;

void DHT_read()
{
  static unsigned long previousMillis = 0;
  
  if(millis() - previousMillis > 2000)
  {
      DHT_temperature = dht.getTemperature();
      DHT_humidity = dht.getHumidity();
      previousMillis = millis();
  }
}