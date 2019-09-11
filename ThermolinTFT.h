#ifndef _THERMOLINTFT_H
#define _THERMOLINTFT_H

#include <OpenWeatherMapCurrent.h>
#include <JsonListener.h>



//##############################Defines####################################

#define GFXFF                      1
#define BOUNCING_TIME_BUTTON     100

//#########################################################################






//##############################Variables##################################

extern const int       UPDATE_INTERVAL_SECS;
extern String          OPEN_WEATHER_MAP_APP_ID;
extern String          OPEN_WEATHER_MAP_LOCATION_ID;
extern String          OPEN_WEATHER_MAP_LANGUAGE;
extern unsigned long   timeSinceLastWUpdate;
extern const bool      IS_METRIC;

//#########################################################################





//#########################Classes#########################################

extern OpenWeatherMapCurrentData  currentWeather;
extern OpenWeatherMapCurrent      currentWeatherClient;

//#########################################################################






//#########################Prototypes######################################

void TFT_drawWeatherPage(void);
void TFT_drawTemperaturePage(float temperature);
void TFT_drawTargetTemperaturePage(float Target_temperature);
void TFT_drawHumidityPage(float Humidity);
void TFT_drawAutoBitmaps(uint16_t color_sun, uint16_t color_moon);
void TFT_drawWlanBitmap(bool visible);
void TFT_drawWlanStatus(void);
void TFT_drawAutoModeStatus(void);
void change_page_counter(void);
void update_Weather_Data(void);
void prepare_page_update(unsigned long currentMillis);
void check_page_update(void);
void TFT_page_update(void);

//#########################################################################

#endif 