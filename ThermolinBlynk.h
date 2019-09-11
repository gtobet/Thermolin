#ifndef _THERMOLINBLYNK_H
#define _THERMOLINBLYNK_H

#include "BlynkSimpleEsp8266.h"


//##############################Defines####################################

#define BLYNK_PRINT Serial

//#########################################################################




//##############################Variables##################################

extern char auth[];
extern char ssid[];
extern char pass[];

//#########################################################################




//#########################Classes#########################################

extern BlynkTimer Blynk_timer;
extern BlynkWifi        Blynk;

//#########################################################################




//#########################Prototypes######################################
void Blynk_timer_read(void);
void Blynk_Init(void);
void Blynk_updateAppTemp(void);
void Blynk_updateThermostatTemp(void);
//#########################################################################

#endif 
