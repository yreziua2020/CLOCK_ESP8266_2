/*
 * 
 * 
 */
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Ticker.h>
#include <EEPROM.h>
#include "Timer.h"

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include "Fonts.h"

#define  MAX_DEVICES 4 
//#define CLK_PIN     D5 // or SCK
//#define DATA_PIN    D7 // or MOSI
#define CS_PIN     15 // D8 // or SS
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW


MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
#define ARRAY_SIZE(x)  (sizeof(x)/sizeof(x[0]))
 int16_t tmp_cl_pog =0;  //если истина то будет оновлять время погоду
// Global data
typedef struct
{
  uint16_t      countt;
  textEffect_t  effect;   // text effect to display
  char *        psz;      // text string nul terminated
  uint16_t      speed;    // speed multiplier of library default
  uint16_t      pause;    // pause multiplier for library default
} sCatalog;

sCatalog  catalog[] =
{
  { 0, PA_SLICE, "SLICE", 1, 1 },
  { 1, PA_MESH, "MESH", 10, 1 },
  { 2, PA_FADE, "FADE", 20, 1 },
  { 3, PA_WIPE, "WIPE", 5, 1 },
  { 4, PA_WIPE_CURSOR, "WPE_C", 4, 1 },
  { 5, PA_OPENING, "OPEN", 3, 1 },
  { 6, PA_OPENING_CURSOR, "OPN_C", 4, 1 },
  { 7, PA_CLOSING, "CLOSE", 3, 1 },
  { 8, PA_CLOSING_CURSOR, "CLS_C", 4, 1 },
  { 9, PA_BLINDS, "BLIND", 7, 1 },
  { 10, PA_DISSOLVE, "DSLVE", 7, 1 },
  { 11, PA_SCROLL_UP, "SC_U", 5, 1 },
  { 12, PA_SCROLL_DOWN, "SC_D", 5, 1 },
  { 13, PA_SCROLL_LEFT, "SC_L", 5, 1 },
  { 14, PA_SCROLL_RIGHT, "SC_R", 5, 1 },
  { 15, PA_SCROLL_UP_LEFT, "SC_UL", 7, 1 },
  { 16, PA_SCROLL_UP_RIGHT, "SC_UR", 7, 1 },
  { 17, PA_SCROLL_DOWN_LEFT, "SC_DL", 7, 1 },
  { 18, PA_SCROLL_DOWN_RIGHT, "SC_DR", 7, 1 },
  { 19, PA_SCAN_HORIZ, "SCANH", 4, 1 },
  { 20, PA_SCAN_VERT, "SCANV", 3, 1 },
  { 21, PA_GROW_UP, "GRW_U", 7, 1 },
  { 22, PA_GROW_DOWN, "GRW_D", 7, 1 },
};



Timer t;

#include "global.h"
#include "NTP.h"

// Include the HTML, STYLE and Script "Pages"

#include "Page_Admin.h"
#include "Page_Script.js.h"
#include "Page_Style.css.h"
#include "Page_NTPSettings.h"
#include "Page_Information.h"
#include "Page_General.h"
#include "Page_NetworkConfiguration.h"



extern "C" {
#include "user_interface.h"
}

Ticker ticker;


os_timer_t myTimer;


//*** Normal code definition here ...

#define LED_PIN 2
#define buttonPin 0

#define analogPIN A0



String weatherKey;
String ipstring;
String Text;
char buf[256];

String y;     // год
String mon;   // месяц
String wd;    // день недели
String d;     // дени
String h;     // часоы
String m;     // минуты
String s;     // секунды

int disp=0;
int rnd;
int lp=0;

unsigned long eventTime=0;
int buttonstate =1;


String weatherMain = "";
String weatherDescription = "";
String weatherLocation = "";
String country;
int humidity;
int pressure;
float temp;
String tempz;

float lon;
float lat;

int clouds;
float windSpeed;
int windDeg;

String date;
String date1;
String currencyRates;
String weatherString;
String weatherString1;
String weatherStringz;
String weatherStringz1;
String weatherStringz2;

String cityID;
  
WiFiClient client;




String chipID;


// the loop function runs over and over again forever
void loop() {
  
  // OTA request handling
  ArduinoOTA.handle();

  //  WebServer requests handling
  server.handleClient();

   //  feed de DOG :) 
   customWatchdog = millis();

  //**** Normal Skecth code here ... 
t.update();
  if (lp >= 10) lp=0;
  if (disp ==0){  
        if (lp==0){  getWeatherData();      getWeatherDataz();  }   //getTime(); 
        
        if   (P.displayAnimate()) {  disp=1;  lp++; }
     }
   //if (disp ==1){ rnd = random(0, ARRAY_SIZE(catalog));   Text = h + ":" + m;                           displayInfo();  }

    //static uint16_t tmp_ef;
   if (disp ==1){ 
     
     //rnd = random(0, ARRAY_SIZE(catalog)); 
     Text = h + ":" + m;                          
      displayInfo(); 
      
      
       }
   
   if (disp ==2){                                         Text = wd + " " + d + " " + mon + " " + y;    scrollText();   }
   if (disp ==3){    rnd = random(0, ARRAY_SIZE(catalog));   Text = h + ":" + m;                           displayInfo1(); }
   if (disp ==4){                                           Text = weatherString;                         scrollText1();  }
   if (disp ==5){   rnd = random(0, ARRAY_SIZE(catalog));   Text = h + ":" + m;                           displayInfo2(); }
   if (disp ==6){                                            Text = weatherStringz + " " + weatherStringz1;scrollText2();  }
   
  //============длительное нажатие кнопки форматирует EEPROM
int buttonstate=digitalRead(buttonPin);
if(buttonstate==HIGH) eventTime=millis();
if(millis()-eventTime>5000){      // при нажатии 15 секунд - 
digitalWrite(16, LOW);  
 ResetAll();                 // форматируем EEPROM
Serial.println("EEPROM formatted");
ESP.restart();
}
else 
{
digitalWrite(16, HIGH); 
}

//============длительное нажатие кнопки форматирует EEPROM
}

void ResetAll(){
  //EEPROM.begin()512);
  // write a 0 to all 512 bytes of the EEPROM
 // for (int i = 0; i < 512; i++){  EEPROM.write(i, 0);  }
  //EEPROM.end();
 // ESP.reset();
}

//==========================================================
void getTime(){
  getNTPtime();
    h = String (DateTime.hour/10) + String (DateTime.hour%10);
    m = String (DateTime.minute/10) + String (DateTime.minute%10);
    s = String (DateTime.second/10 + String (DateTime.second%10));

    d = String (DateTime.day);

    y = String (DateTime.year);

    Serial.print("God=");Serial.println(DateTime.year);
     
    if (DateTime.month == 1) mon = "Января";
    if (DateTime.month == 2) mon = "Февраля";
    if (DateTime.month == 3) mon = "Марта";
    if (DateTime.month == 4) mon = "Апреля";
    if (DateTime.month == 5) mon = "Мая";
    if (DateTime.month == 6) mon = "Июня";
    if (DateTime.month == 7) mon = "Июля";
    if (DateTime.month == 8) mon = "Августа";
    if (DateTime.month == 9) mon = "Сентября";
    if (DateTime.month == 10) mon = "Октября";
    if (DateTime.month == 11) mon = "Ноября";
    if (DateTime.month == 12) mon = "Декабря";

    if (DateTime.wday == 2) wd = "Понедельник";
    if (DateTime.wday == 3) wd = "Вторник";
    if (DateTime.wday == 4) wd = "Среда";
    if (DateTime.wday == 5) wd = "Четверг";
    if (DateTime.wday == 6) wd = "Пятница";
    if (DateTime.wday == 7) wd = "Суббота";
    if (DateTime.wday == 1) wd = "Воскресенье";
    
}



