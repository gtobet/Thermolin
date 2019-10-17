#include "Bitmaps.h"
#include "Pinout.h"
#include "Init.h"
#include "ThermolinBlynk.h"
#include "ThermolinTFT.h"
#include "ThermolinDHT.h"


#define HYSTERESE 0.25

void setup()
{
  Blynk_Init();
  HW_Init();

  //Wait 2.5 sek for connecting to Wifi
  int counter = 0;
  while(WiFi.status() != WL_CONNECTED || counter < 5)
  {
    delay(500);
    counter++;
  }
  
  update_Weather_Data();
  
  //Debug console
  //Serial.begin(9600);
}

void loop()
{
  Blynk.run();
  Blynk_timer.run();
  
  DHT_read();                   //reads temp and humidity from DHT senso
  
  Blynk_updateAppTemp();        //writes temps to Blynk App when temps on Thermostat changes
  Blynk_updateThermostatTemp(); //writes temp from Blynk to Thermostat
  
  check_page_update();          //reads rotary encoder an checks for page update
  
  TFT_page_update();            //updates TFT display
  TFT_drawWlanStatus();         //Show when Wlan not connected



  //Control Relay................................
  if(DHT_temperature > target_temperature + HYSTERESE)
  {
    digitalWrite(PIN_RELAY, HEATING_OFF);
    status_heating = false;
  }
  else if(DHT_temperature < target_temperature - HYSTERESE)
  {
    digitalWrite(PIN_RELAY, HEATING_ON);
    status_heating = true;
  }
  
  //.............................................
}
