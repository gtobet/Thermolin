#include "Bitmaps.h"
#include "Pinout.h"
#include "Init.h"

BLYNK_WRITE(VPIN_MAN_TARGET_TEMPERATURE)
{
  HEATING_MODE_flag = true;
  man_target_temperature = param.asDouble();
}

BLYNK_WRITE(VPIN_HEATING_MODE)
{
  HEATING_MODE_flag = true;
  heating_mode = param.asInt();
}

BLYNK_WRITE(VPIN_AUTO_TARGET_TEMPERATURE_DAY)
{
  HEATING_MODE_flag = true;
  auto_target_temperature_day = param.asDouble();
}

BLYNK_WRITE(VPIN_AUTO_TARGET_TEMPERATURE_NIGHT)
{
  HEATING_MODE_flag = true;
  auto_target_temperature_night = param.asDouble();
}

BLYNK_WRITE(VPIN_AUTO_MODE_DAY)
{
  HEATING_MODE_flag = true;
  AUTO_MODE_DAY = param.asInt();
  Serial.println(AUTO_MODE_DAY);
}

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
void writeTemperatures2Blynk()
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
void TFT_drawWeatherPage()
{

  //Draw Temp
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextFont(6); 
  tft.setCursor(5,110);
  tft.print(currentWeather.temp,1);

  //Draw Meteocon
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setFreeFont(&Meteocons_Regular_50);    
  tft.setTextSize(1);
  tft.fillRect(5,35,50,50,TFT_BLACK);
  tft.drawString(currentWeather.iconMeteoCon, 5, 35, GFXFF);

  //Draw unit
  if(PAGE_update_tft)
  {
    tft.drawCircle(110, 110, 6, TFT_WHITE);
    tft.drawCircle(110, 110, 5, TFT_WHITE);
    tft.drawCircle(110, 110, 4, TFT_WHITE);
    PAGE_update_tft = false;
  }

}
void TFT_drawTemperaturePage(float temperature)
{
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextFont(6); 
  tft.setCursor(5,110);
  tft.print(temperature,1);

  if(PAGE_update_tft)
  {
    
    tft.drawBitmap(5, 35, house_bitmap, 50, 50, TFT_WHITE, TFT_BLACK);
    tft.drawCircle(110, 110, 6, TFT_WHITE);
    tft.drawCircle(110, 110, 5, TFT_WHITE);
    tft.drawCircle(110, 110, 4, TFT_WHITE);
    PAGE_update_tft = false;
  }

}

void TFT_drawTargetTemperaturePage(float Target_temperature)
{
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextFont(6); 
  tft.setCursor(5,110);
  tft.print(Target_temperature,1);


  if(PAGE_update_tft)
  {
    tft.drawBitmap(5, 35, temperature_bitmap, 50, 50, TFT_WHITE, TFT_BLACK);
    tft.drawCircle(110, 110, 6, TFT_WHITE);
    tft.drawCircle(110, 110, 5, TFT_WHITE);
    tft.drawCircle(110, 110, 4, TFT_WHITE);
    PAGE_update_tft = false;
  }

}
void TFT_drawHumidityPage(float Humidity)
{
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextFont(6); 
  tft.setCursor(35,110);
  tft.fillRect(5,104,123,45,TFT_BLACK);
  tft.print(Humidity,0);

  if(PAGE_update_tft)
  {
    tft.drawBitmap(5, 35, raindrop_bitmap, 50, 50, TFT_WHITE, TFT_BLACK);
    PAGE_update_tft = false;
  }

}
void TFT_drawAutoBitmaps(uint16_t color_sun, uint16_t color_moon)
{
  tft.drawBitmap(69, 35, sun_bitmap, 28, 50, color_sun, TFT_BLACK);
  tft.drawBitmap(97, 35, moon_bitmap, 21, 50, color_moon, TFT_BLACK);
}
void TFT_drawWlanBitmap(bool visible)
{
  if(visible)
  {
    tft.drawBitmap(69, 35, wlan_bitmap, 50, 50, TFT_RED, TFT_BLACK);
  }
  else
  {
    tft.fillRect(69,35,50,50,TFT_BLACK);
  }

}
void TFT_drawWlanStatus()
{
  static unsigned long currentMillis = 0;
  static unsigned long previousMillis = 0;
  static bool ShowBitmap = false;
  
  if(WiFi.status() != WL_CONNECTED)
  {
    currentMillis = millis();
    
    if(currentMillis - previousMillis > 1000)
    {
      ShowBitmap = !ShowBitmap;
      TFT_drawWlanBitmap(ShowBitmap);
      previousMillis = currentMillis;
    }
  }
}
void TFT_drawAutoModeStatus()
{
  if(AUTO_MODE)
  {
    if(AUTO_MODE_DAY)
    {
      TFT_drawAutoBitmaps(TFT_ORANGE, TFT_WHITE);
    }
    else
    {
      TFT_drawAutoBitmaps(TFT_WHITE, TFT_ORANGE);
    }
  }
  else if(!AUTO_MODE && WiFi.status() == WL_CONNECTED)
  {
    tft.fillRect(69,35,50,50,TFT_BLACK);
  }
}

void change_page_counter(){
  (PAGE_counter += 1) &= B0011;
}

void update_Weather_Data()
{
  currentWeatherClient.setMetric(IS_METRIC);
  currentWeatherClient.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
  currentWeatherClient.updateCurrentById(&currentWeather, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_ID);
}

void prepare_page_update(unsigned long currentMillis)
{
  static unsigned long last_time_triggerd = 0;
  unsigned long triggered_time = currentMillis;
    
  if(triggered_time - last_time_triggerd > 100)
  {
    change_page_counter();
    PAGE_millis = currentMillis;
    PAGE_update_tft = true;
  }
  last_time_triggerd = triggered_time;
}

void check_page_update()
{
  //Reads Rotary encoder
  ROTARY_button_value = analogRead(PIN_ROTARY_BUTTON);
  
  static unsigned long currentMillis = 0;
  currentMillis = millis();

  //Rotary encoder pressed
  if(ROTARY_button_value >= 1023 && ROTARY_Button_flag == true)
  {
    prepare_page_update(currentMillis);
    ROTARY_Button_flag = false;
  }
  //Rotary encoder released
  else if(ROTARY_button_value < 1023 && ROTARY_Button_flag == false)
  { 
    ROTARY_Button_flag = true;
  }

  //Go to initial page
  if(currentMillis - PAGE_millis > 8000 && currentMillis >= PAGE_millis && PAGE_init == false)
  {
    PAGE_counter = 0;
    PAGE_update_tft = true;
  }
}

void TFT_page_update()
{
  switch(PAGE_counter)
  {
    //Update TFT
    case 0:
      PAGE_init = true;
      if(DHT_temperature_tmp != DHT_temperature || PAGE_update_tft == true)
      {
        TFT_drawAutoModeStatus();
        TFT_drawTemperaturePage(DHT_temperature);
        DHT_temperature_tmp = DHT_temperature;
      }
      break;
      
    case 1:
      PAGE_init = false;
      if (millis()  - timeSinceLastWUpdate > 1000 * UPDATE_INTERVAL_SECS || PAGE_update_tft == true) 
      {
        update_Weather_Data();
        TFT_drawWeatherPage();
        timeSinceLastWUpdate = millis();
      }
      break;
          
    case 2:
      PAGE_init = false;
      if(target_temperature_tmp != target_temperature || PAGE_update_tft == true)
      {
        TFT_drawTargetTemperaturePage(target_temperature);
        target_temperature_tmp = target_temperature;
      }
      break;

    case 3:
      PAGE_init = false;
      if(DHT_humidity_tmp != DHT_humidity || PAGE_update_tft == true)
      {
        TFT_drawHumidityPage(DHT_humidity);
        DHT_humidity_tmp = DHT_humidity;
      }
      break;
   }
}

void setup()
{
  Blynk_Init();
  HW_Init();
  
  // Debug console
  Serial.begin(9600);

  //Wait 
  int counter = 0;
  while(WiFi.status() != WL_CONNECTED || counter < 5)
  {
    delay(500);
    counter++;
  }
  
  if(WiFi.status() == WL_CONNECTED)
  {
    update_Weather_Data();
  }

}

void loop()
{
  
  Blynk.run();
  Blynk_timer.run();
  
  DHT_read();               //reads temp and humidity from DHT senso
  writeTemperatures2Blynk();//writes temps to Blynk App when temps on Thermostat changes

  check_page_update();      //reads rotary encoder an checks for page update
  TFT_page_update();        //updates TFT display
  TFT_drawWlanStatus();     //Show when Wlan not connected
   
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


   
  //Control Relay................................
  if(target_temperature > DHT_temperature)
  {
    digitalWrite(PIN_RELAY, HEATING_ON);
    status_heating = true;
  }
  else
  {
    digitalWrite(PIN_RELAY, HEATING_OFF);
    status_heating = false;
  }
  //.............................................
}
