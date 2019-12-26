#include "Bitmaps.h"
#include "Pinout.h"
#include "Init.h"
#include "ThermolinBlynk.h"
#include "ThermolinTFT.h"
#include "ThermolinDHT.h"

#define HYSTERESE 0.25

//Reset Function
void(* resetFunc) (void) = 0;

void setup()
{
  
  HW_Init();    //Initialize Hardware
  Blynk_Init(); //Initialize Blynk App

  //Wait 2.5 sek for connecting to Wifi
  int counter = 0;
  while(WiFi.status() != WL_CONNECTED || counter < 5)
  {
    delay(500);
    counter++;
  }
  
  update_Weather_Data();  //get weather data from Server
  
  //Debug console
  //Serial.begin(9600);

  //****************Read out EEPROM****************
  EEPROM.begin(512);
  
  man_target_temperature = EEPROM_read_float(ADR_man_temp);             
  auto_target_temperature_day = EEPROM_read_float(ADR_auto_temp_day);     
  auto_target_temperature_night = EEPROM_read_float(ADR_auto_temp_night);

  AUTO_MODE_DAY = EEPROM.read(ADR_AUTO_MODE_DAY);
  heating_mode = EEPROM.read(ADR_heating_mode);
  //***********************************************
  
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
  
  //Serial.println("loop");


  //****************Control Relay****************
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
  //*********************************************


  //****************HW Reset*********************
  if(millis() > 7200000){
    resetFunc();
    delay(50); 
  }
  //*********************************************
}
