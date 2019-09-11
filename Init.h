#ifndef _INIT_H
#define _INIT_H

#include <TFT_eSPI.h>
#include <OpenWeatherMapCurrent.h>
#include "BlynkSimpleEsp8266.h"
#include "DHTesp.h"
#include <JsonListener.h>


//##############################Defines####################################
#define HEATING_OFF  			 HIGH
#define HEATING_ON   			  LOW
#define GFXFF 						1
#define BLYNK_PRINT 		   Serial
#define BOUNCING_TIME_ROTATION 		5
#define BOUNCING_TIME_BUTTON 	  100
//#########################################################################



//##############################Variables##################################
extern char 		 auth[];
extern char 		 ssid[];
extern char 		 pass[];

extern float 		 DHT_temperature;
extern float		 DHT_humidity;

extern bool  		 status_heating;
extern float 		 target_temperature;
extern float 		 man_target_temperature;
extern float 		 auto_target_temperature_day;
extern float 		 auto_target_temperature_night;
extern float 		 DHT_temperature_tmp;
extern float 		 DHT_humidity_tmp;
extern float 		 target_temperature_tmp;

extern int   		 heating_mode;

extern bool          HEATING_MODE_flag;
extern bool          AUTO_MODE_DAY;
extern bool          AUTO_MODE;
extern bool          CONNECTED_flag;


extern byte          PAGE_counter;
extern bool          PAGE_update_tft;
extern bool          PAGE_init;
extern unsigned long PAGE_millis;

//Rotary Encoder
extern bool          ROTARY_Button_flag;
extern int           ROTARY_button_value;
extern int           ROTARY_pos;
extern bool          ROTARY_ROTATION_flag;

//Weather
extern const int     UPDATE_INTERVAL_SECS;
extern String        OPEN_WEATHER_MAP_APP_ID;
extern String        OPEN_WEATHER_MAP_LOCATION_ID;
extern String        OPEN_WEATHER_MAP_LANGUAGE;
extern unsigned long timeSinceLastWUpdate;
extern const bool	 IS_METRIC;
//#########################################################################



//#########################Classes#########################################
extern DHTesp dht;
extern BlynkTimer Blynk_timer;
extern TFT_eSPI tft;
extern OpenWeatherMapCurrentData currentWeather;
extern OpenWeatherMapCurrent currentWeatherClient;
extern BlynkWifi Blynk;
//#########################################################################



//#########################Prototypes######################################
void Blynk_timer_read(void);
void Blynk_Init(void);
void HW_Init(void);
void PIN_ROTARY_A_ISR(void);
void PIN_ROTARY_B_ISR(void);
//#########################################################################
#endif //_INIT_H