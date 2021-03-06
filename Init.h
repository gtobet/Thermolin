#ifndef _INIT_H
#define _INIT_H

#include <TFT_eSPI.h>
#include "DHTesp.h"
#include <EEPROM.h>

//##############################Defines####################################

#define HEATING_OFF  			     HIGH
#define HEATING_ON   			      LOW
#define BOUNCING_TIME_ROTATION    5


//EEPROM adresses
#define ADR_heating_mode          1
#define ADR_AUTO_MODE_DAY         2
#define ADR_man_temp              3
#define ADR_auto_temp_day         7
#define ADR_auto_temp_night      11

//#########################################################################



//##############################Variables##################################

extern bool  		      status_heating;
extern float 		      target_temperature;
extern float 		      man_target_temperature;
extern float 		      auto_target_temperature_day;
extern float 		      auto_target_temperature_night;
extern float 		      DHT_temperature_tmp;
extern float 		      DHT_humidity_tmp;
extern float 		      target_temperature_tmp;

extern int   		      heating_mode;

extern bool           HEATING_MODE_flag;
extern bool           AUTO_MODE_DAY;
extern bool           AUTO_MODE;
extern bool           CONNECTED_flag;


extern byte           PAGE_counter;
extern bool           PAGE_update_tft;
extern bool           PAGE_init;
extern unsigned long  PAGE_millis;

//Rotary Encoder
extern bool           ROTARY_Button_flag;
extern int            ROTARY_button_value;
extern int            ROTARY_pos;
extern bool           ROTARY_ROTATION_flag;

//#########################################################################



//#########################Classes#########################################

extern DHTesp                     dht;
extern TFT_eSPI                   tft;

//#########################################################################



//#########################Prototypes######################################

void HW_Init(void);
void PIN_ROTARY_A_ISR(void);
void PIN_ROTARY_B_ISR(void);
float EEPROM_read_float(int address);
void EEPROM_write_float(int address, float value);
//#########################################################################
#endif //_INIT_H
