#include "Init.h"
#include "Pinout.h"

//##############################Variables##################################

bool  		    status_heating;
float 		    target_temperature =           							              19;
float 		    man_target_temperature =       						 	              15;
float 		    auto_target_temperature_day =  						 	              19;
float 		    auto_target_temperature_night =							              16;
float 		    DHT_temperature_tmp =          							               0;
float 		    DHT_humidity_tmp =           							                 0;
float 		    target_temperature_tmp =     							                 0;

int   		    heating_mode =                 						 	               2;  //1 = Auto //2 = Manuel

bool          HEATING_MODE_flag =         						 	              true;
bool          CONNECTED_flag =            						 	             false;
bool          AUTO_MODE_DAY;
bool          AUTO_MODE	=											                       false;

byte          PAGE_counter =              						     	             0;
bool          PAGE_update_tft =         						                  true;
bool          PAGE_init =               					   	   	           false;
unsigned long PAGE_millis =              						                     0;

//Rotary Encoder
bool          ROTARY_Button_flag =         							              true;
int           ROTARY_button_value =        							                 0;
int           ROTARY_pos =                 							                 0;
bool          ROTARY_ROTATION_flag =       							             false;
//#########################################################################



//#########################Classes#########################################

DHTesp 						          dht;
TFT_eSPI 					          tft = TFT_eSPI();

//#########################+++++++#########################################




void PIN_ROTARY_A_ISR()
{
  cli();
  ROTARY_ROTATION_flag = true;
  PAGE_counter = 2;
  PAGE_update_tft = true;
  	
  static unsigned long last_interrupt_timeA = 0;
  unsigned long interrupt_time = millis();
  	
  PAGE_millis = interrupt_time;
  
 	if (interrupt_time - last_interrupt_timeA > 5)
 	{
    if(digitalRead(PIN_ROTARY_B) == LOW)
    {
      if(target_temperature > 15)
      {
        if(((int)(target_temperature*10)%5))
        {
          target_temperature = (int) target_temperature;
        }
        target_temperature = target_temperature - 0.5;
      }
    }
  }
  last_interrupt_timeA = interrupt_time;
  sei();
}

void PIN_ROTARY_B_ISR()
{
  cli();
  ROTARY_ROTATION_flag = true;
  PAGE_counter = 2;
  PAGE_update_tft = true;
  
  static unsigned long last_interrupt_timeB = 0;
  unsigned long interrupt_time = millis();
  
  PAGE_millis = interrupt_time;
  
  if (interrupt_time - last_interrupt_timeB > 5)
  {
    if(digitalRead(PIN_ROTARY_A) == LOW)
    {
      if(target_temperature < 25)
      {
        if(((int)(target_temperature*10)%5))
        {
          target_temperature = (int) target_temperature;
        }
        target_temperature = target_temperature + 0.5;
      }
    }
  }
  last_interrupt_timeB = interrupt_time;
  sei();
}

void HW_Init()
{
	//Initialize Relay
  digitalWrite(PIN_RELAY, HEATING_OFF);
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_ROTARY_BUTTON,INPUT);
  	
  //Initialize Temp Sensor
  dht.setup(PIN_DHT, DHTesp::DHT22);
  delay(50);
  	
  //Initialize Display
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  	
  //Initialize Rotary Encoder
  pinMode(PIN_ROTARY_A, INPUT_PULLUP);
  pinMode(PIN_ROTARY_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_ROTARY_A), PIN_ROTARY_A_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_ROTARY_B), PIN_ROTARY_B_ISR, RISING);
}
