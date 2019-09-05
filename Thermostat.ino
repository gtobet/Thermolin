
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHTesp.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include "Fonts/FreeMonoBold18pt7b.h"
#include "Fonts/FreeMono12pt7b.h"
#include "Fonts/FreeMono9pt7b.h"
#include "Fonts/FreeMonoBold9pt7b.h"


//Pinout NodeMCU
#define PIN_DHT   D3
#define PIN_RELAY D8

#define TFT_CS    D1
#define TFT_RST   D0                                          
#define TFT_DC    D2

#define PINA      D6
#define PINB      D4
#define PIN_ROTARY_PRESSED  A0

#define HEATING_OFF  HIGH
#define HEATING_ON LOW

#define FONT_Mono_Small_B FreeMonoBold9pt7b
#define FONT_Mono_Big_B FreeMonoBold18pt7b
#define FONT_Mono_Medium FreeMono12pt7b
#define FONT_Mono_Small  FreeMono9pt7b

//Pinout Blynk VirtualPins
#define VPIN_TEMPERATURE                    V5
#define VPIN_HUMIDITY                       V6
#define VPIN_MAN_TARGET_TEMPERATURE         V1
#define VPIN_STATUS_HEATING                 V7
#define VPIN_HEATING_MODE                   V8
#define VPIN_AUTO_TARGET_TEMPERATURE_DAY   V16
#define VPIN_AUTO_TARGET_TEMPERATURE_NIGHT V15
#define VPIN_AUTO_MODE_TIMER               V10

//Globals
char auth[] = "1ccc2723f0424deb9a2e340f65359b36";
char ssid[] = "UPC3AB7B37";
char pass[] = "b5fnjmuahYGn";

//char auth[] = "1ccc2723f0424deb9a2e340f65359b36";
//char ssid[] = "hoerbstWIEN";
//char pass[] = "GSG44!!60";

float DHT_temperature;
float DHT_humidity;

bool  status_heating;

float target_temperature =            19;
float man_target_temperature =        15;
float auto_target_temperature_day =   19;
float auto_target_temperature_night = 16;
float DHT_temperature_tmp =            0;
float DHT_humidity_tmp =               0;
float target_temperature_tmp =         0;

int   heating_mode =                   2;
int   heating_mode_tmp;
bool  auto_mode_timer;
bool  HEATING_MODE_flag =           true;
bool  CONNECTED_flag =             false;

int   ROTARY_pos =                     0;
bool  ROTARY_flag =                false;

const uint16_t white = 0xFFFF;

//Classes
DHTesp dht;
BlynkTimer Blynk_timer;
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);



void Blynk_timer_read()
{
  Blynk.virtualWrite(VPIN_TEMPERATURE, DHT_temperature);
  Blynk.virtualWrite(VPIN_HUMIDITY, DHT_humidity);        
  Blynk.virtualWrite(VPIN_STATUS_HEATING, status_heating);
  
}

//Reads Target Temperature from Blynk
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

BLYNK_WRITE(VPIN_AUTO_MODE_TIMER)
{
  HEATING_MODE_flag = true;
  auto_mode_timer = param.asInt();
}

void DHT_read()
{
  static int previousMillis = 0;
  
  if(millis() - previousMillis > 2000)
  {
      DHT_temperature = dht.getTemperature();
      DHT_humidity = dht.getHumidity();
      previousMillis = millis();
  }
}

void TFT_writeTemperature(float temperature)
{
  //tft.initR(INITR_BLACK TAB);        // Initialize ST7735R screen
  //tft.setTextWrap(false);
  //tft.fillScreen(ST77XX_BLACK);
  tft.fillRect(10,35,90,35,ST7735_BLACK);
  tft.setTextColor(white);
  tft.setFont(&FONT_Mono_Big_B);
  tft.setTextSize(1);
  tft.setCursor(15,60);
  tft.print(temperature,1);

  tft.drawCircle(102, 40, 2, white);
  tft.setFont(&FONT_Mono_Medium);
  tft.setTextSize(1);
  tft.setCursor(107,48);
  tft.println("c");

}

void TFT_writeTargetTemperature(float Target_temperature)
{
  tft.fillRect(9,115,45,20,ST7735_BLACK);
  tft.setTextColor(white);
  tft.setFont(&FONT_Mono_Small_B);
  tft.setTextSize(1);
  tft.setCursor(10,130);
  tft.print(Target_temperature,1);

  //tft.drawCircle(29, 140, 2, white);
  //tft.setFont(&FONT_Mono_Small);
  //tft.setTextSize(1);
  //tft.setCursor(32,148);
 // tft.println("C");
}
void TFT_writeHumidity(float Humidity)
{
  tft.fillRect(84,115,40,20,ST7735_BLACK);
  tft.setTextColor(white);
  tft.setFont(&FONT_Mono_Small_B);
  tft.setTextSize(1);
  tft.setCursor(85,130);
  tft.print(Humidity,0);
  //tft.setFont(&FONT_Mono_Small);
  //tft.setTextSize(1);
 // tft.setCursor(90,148);
 // tft.println("%");
}

void TFT_writeMode(int heating_mode)
{
  tft.fillRect(0,75,30,20,ST7735_BLACK);
  tft.fillRoundRect(7,77,17,18,1,ST7735_WHITE);
  tft.setTextColor(ST7735_BLACK);
  tft.setFont(&FONT_Mono_Small_B);
  tft.setTextSize(1);
  tft.setCursor(10,90);
  if(heating_mode == 2)
  {
    tft.print("M");
  }
  else if(heating_mode == 1)
  {
    tft.print("A");
  }
}
void pinA_ISR()
{
  cli();
  ROTARY_flag = true;
  static unsigned long last_interrupt_timeA = 0;
  unsigned long interrupt_time = millis();
  
  if (interrupt_time - last_interrupt_timeA > 2)
  {
    if(digitalRead(PINB) == LOW)
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

void pinB_ISR()
{
  cli();
  ROTARY_flag = true;
  static unsigned long last_interrupt_timeB = 0;
  unsigned long interrupt_time = millis();
  
  if (interrupt_time - last_interrupt_timeB > 2)
  {
    if(digitalRead(PINA) == LOW){
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

const uint8_t wlanON_bitmap[] = {
 B00000000, B00000001, 
  B00000111, B00000001, 
  B00000001, B11000001, 
  B00000000, B01100001, 
  B00000000, B00110001, 
  B00001100, B00010001, 
  B00000111, B00011001, 
  B00000001, B00001001, 
  B00000000, B10001101, 
  B00110000, B11000101, 
  B00001000, B01000101, 
  B00000100, B01000101, 
  B01000100, B01000101, 
  B01000100, B01000101, 
  B00000000, B00000001, 
  
};
const uint8_t temperature_bitmap[] = {
 B00000000, B11110000, B00000011, 
  B00000001, B11111000, B00000011, 
  B00000001, B00001000, B00000011, 
  B00000001, B00001000, B00000011, 
  B00000001, B00001000, B00000011, 
  B00000001, B00001000, B00000011, 
  B00000001, B00001011, B11000011, 
  B00000001, B01101000, B00000011, 
  B00000001, B01101001, B11000011, 
  B00000001, B01101001, B11000011, 
  B00000001, B01101000, B00000011, 
  B00000001, B01101011, B11000011, 
  B00000001, B01101000, B00000011, 
  B00000011, B01101000, B00000011, 
  B00000111, B01101100, B00000011, 
  B00000110, B01100110, B00000011, 
  B00001100, B01100011, B00000011, 
  B00001000, B11110011, B00000011, 
  B00001001, B11111001, B00000011, 
  B00001001, B11111001, B00000011, 
  B00001001, B11110001, B00000011, 
  B00001100, B11110011, B00000011, 
  B00001100, B00000010, B00000011, 
  B00000110, B00000110, B00000011, 
  B00000011, B11111100, B00000011, 
  B00000001, B11110000, B00000011, 
  
};
const uint8_t humidity_bitmap[] = {
  B00000000, B00000000, B00000011, 
  B00000000, B00000000, B00000011, 
  B00000000, B00000000, B00000011, 
  B00000000, B00001000, B00000011, 
  B00000000, B00011000, B00000011, 
  B00000000, B00011100, B00000011, 
  B00000000, B00110100, B00000011, 
  B00000000, B00100110, B00000011, 
  B00000000, B00100010, B00000011, 
  B00000000, B00100010, B00000011, 
  B00000110, B00110110, B00000011, 
  B00000111, B00011100, B00000011, 
  B00001111, B00000000, B00000011, 
  B00001001, B00000001, B00000011, 
  B00001001, B10000001, B10000011, 
  B00011000, B10000011, B11000011, 
  B00001001, B10000010, B11000011, 
  B00001111, B00000110, B01000011, 
  B00000110, B00000110, B01100011, 
  B00000000, B00000110, B01000011, 
  B00000000, B00000011, B11000011, 
  B00000000, B00000011, B10000011, 
  B00000000, B00000000, B00000011, 
  B00000000, B00000000, B00000011, 
  B00000000, B00000000, B00000011, 
  B00000000, B00000000, B00000011, 
  
};
void setup()
{
  //Initialize Relay
  digitalWrite(PIN_RELAY, HEATING_OFF);
  pinMode(PIN_RELAY, OUTPUT);
  //digitalWrite(PIN_RELAY, HEATING_OFF);

  pinMode(PIN_ROTARY_PRESSED,INPUT);
  
  //Initialize Temp Sensor
  dht.setup(PIN_DHT, DHTesp::DHT22);

  //Initialize Blynk
  Blynk.begin(auth, ssid, pass);

  //Initialize timer for Blynk
  Blynk_timer.setInterval(2000L, Blynk_timer_read);
  
  //Initialize Display
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST77XX_BLACK);
  tft.drawLine(64,110,64,160,ST7735_WHITE);


  //Rotary Encoder
  pinMode(PINA, INPUT_PULLUP);
  pinMode(PINB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PINA), pinA_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(PINB), pinB_ISR, RISING);

  //Bitmap Symbols
  tft.drawBitmap(20, 134, temperature_bitmap, 22, 26, white);
  tft.drawBitmap(85, 134, humidity_bitmap, 22, 26, white);
  
  // Debug console
  Serial.begin(9600);
}

void loop()
{
  //Serial.println(analogRead(PIN_ROTARY_PRESSED));
  
  Blynk.run();
  Blynk_timer.run();
  DHT_read();

   if(heating_mode == 2 && HEATING_MODE_flag)
   {
      target_temperature = man_target_temperature;
      HEATING_MODE_flag = false;
   }
   else if(heating_mode == 1 && HEATING_MODE_flag)
   {
      if(auto_mode_timer)
      {
        target_temperature = auto_target_temperature_day;
      }
      else
      {
        target_temperature = auto_target_temperature_night;
      }
      HEATING_MODE_flag = false;
   }
   
  //Update TFT................................
  if(DHT_temperature_tmp != DHT_temperature)
  {
      TFT_writeTemperature(DHT_temperature);
      DHT_temperature_tmp = DHT_temperature;
  }

  if(DHT_humidity_tmp != DHT_humidity)
  {
      TFT_writeHumidity(DHT_humidity);
      DHT_humidity_tmp = DHT_humidity;
  }
  
  if(target_temperature_tmp != target_temperature)
  {
    TFT_writeTargetTemperature(target_temperature);
    target_temperature_tmp = target_temperature;
  }

  if(heating_mode_tmp != heating_mode)
  {
    TFT_writeMode(heating_mode);
    heating_mode_tmp = heating_mode;
  }
  //.............................................

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
  if(ROTARY_flag)
  {
    if(heating_mode == 2)
    {
      Blynk.virtualWrite(VPIN_MAN_TARGET_TEMPERATURE, target_temperature);
    }
    else
    {
      if(auto_mode_timer)
      {
        Blynk.virtualWrite(VPIN_AUTO_TARGET_TEMPERATURE_DAY, target_temperature);
      }
      else
      {
        Blynk.virtualWrite(VPIN_AUTO_TARGET_TEMPERATURE_NIGHT, target_temperature);
      }
    }
    ROTARY_flag = false;
  }
  
  //Check Connection.............................
  if (WiFi.status() == WL_CONNECTED)
  {
    if(!CONNECTED_flag)
    {
      tft.drawBitmap(30, 78, wlanON_bitmap, 15, 15, white);
    }
    CONNECTED_flag = true;
    
  }
  else if (WiFi.status() != WL_CONNECTED)
  {
    if(CONNECTED_flag)
    {
      tft.fillRect(29,77,17,17,ST7735_BLACK);
    }
    CONNECTED_flag = false;
  }
  //..............................................
}
