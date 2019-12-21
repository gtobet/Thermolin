#include "ThermolinBlynk.h"
#include "Init.h"
#include "Pinout.h"
#include "ThermolinDHT.h"

//##############################Variables##################################

char  auth[] =   "1ccc2723f0424deb9a2e340f65359b36";
char  ssid[] =                         "UPC3AB7B37";
char  pass[] =                       "b5fnjmuahYGn";

//#########################################################################




//#########################Classes#########################################

BlynkTimer Blynk_timer;
BlynkWifi  Blynk(_blynkTransport);

//#########################################################################


void Blynk_timer_read()
{
  Blynk.virtualWrite(VPIN_TEMPERATURE, DHT_temperature);
  Blynk.virtualWrite(VPIN_HUMIDITY, DHT_humidity);        
  Blynk.virtualWrite(VPIN_STATUS_HEATING, status_heating);
}

void Blynk_Init()
{
  //Initialize Blynk
  Blynk.begin(auth, ssid, pass);
  
  //Initialize timer for Blynk
  Blynk_timer.setInterval(2000L, Blynk_timer_read);
}


BLYNK_WRITE(VPIN_MAN_TARGET_TEMPERATURE)
{
  HEATING_MODE_flag = true;
  man_target_temperature = param.asDouble();
  
  EEPROM_write_float(ADR_man_temp,man_target_temperature);
  EEPROM.commit();
}

BLYNK_WRITE(VPIN_HEATING_MODE)
{
  HEATING_MODE_flag = true;
  heating_mode = param.asInt();
  
  EEPROM.write(ADR_heating_mode,heating_mode);
  EEPROM.commit();
}

BLYNK_WRITE(VPIN_AUTO_TARGET_TEMPERATURE_DAY)
{
  HEATING_MODE_flag = true;
  auto_target_temperature_day = param.asDouble();

  EEPROM_write_float(ADR_auto_temp_day, auto_target_temperature_day);
  EEPROM.commit();
}

BLYNK_WRITE(VPIN_AUTO_TARGET_TEMPERATURE_NIGHT)
{
  HEATING_MODE_flag = true;
  auto_target_temperature_night = param.asDouble();

  EEPROM_write_float(ADR_auto_temp_night, auto_target_temperature_night);
  EEPROM.commit();
}

BLYNK_WRITE(VPIN_AUTO_MODE_DAY)
{
  HEATING_MODE_flag = true;
  AUTO_MODE_DAY = param.asInt();
  
  EEPROM.write(ADR_AUTO_MODE_DAY,AUTO_MODE_DAY);
  EEPROM.commit();
}

void Blynk_updateAppTemp()
{
  if(ROTARY_ROTATION_flag)
  {
    if(heating_mode == 2)
    {
      Blynk.virtualWrite(VPIN_MAN_TARGET_TEMPERATURE, target_temperature);
    }
    else
    {
      if(AUTO_MODE_DAY)
      {
        Blynk.virtualWrite(VPIN_AUTO_TARGET_TEMPERATURE_DAY, target_temperature);
      }
      else
      {
        Blynk.virtualWrite(VPIN_AUTO_TARGET_TEMPERATURE_NIGHT, target_temperature);
      }
    }
    ROTARY_ROTATION_flag = false;
  }
}

void Blynk_updateThermostatTemp()
{
  if(heating_mode == 2 && HEATING_MODE_flag)
  {
     AUTO_MODE = false;
     PAGE_update_tft = true;
     target_temperature = man_target_temperature;
     HEATING_MODE_flag = false;
  }
  else if(heating_mode == 1 && HEATING_MODE_flag)
  {
     AUTO_MODE = true;
     PAGE_update_tft = true;
     if(AUTO_MODE_DAY)
     {
       target_temperature = auto_target_temperature_day;
     }
     else
     {
       target_temperature = auto_target_temperature_night;
     }
     HEATING_MODE_flag = false;
  }
}
