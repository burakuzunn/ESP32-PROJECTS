int sayi; 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET -1
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  

String Su_Motoru="ACIK";
int Isiklar_On=0;
String Isiklar_Arka="ACIK";
String Armaturler="KAPALI";
 
void setup()   {    
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
   display.setTextWrap(true);
  Wire.begin();
  display.clearDisplay(); 
  
}


void loop() {  
   scrollup();
   
   
}

void scrollup(){
  display.clearDisplay();

  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);

 
 for(int i=-50;i<35;i++){
  Isiklar_On=Isiklar_On+1; 
     delay(11);
      
     
    display.setCursor(0,i);
    display.print("Su Motoru: ");
    display.println(Su_Motoru);
    display.print("Armaturler: ");
    display.println(Isiklar_On);
    display.print("Isiklar(1): ");
    display.println(Isiklar_On);
    display.print("Isiklar(2): ");
    display.println(Isiklar_Arka);
    display.display();
    display.clearDisplay();
}

for(int k=35;k>-50;k--){
     delay(11);
     Isiklar_On=Isiklar_On+956; 
     
    display.setCursor(0,k);
    display.print("Su Motoru: ");
    display.println(Su_Motoru);
    display.print("Armaturler: ");
    display.println(Isiklar_On);
    display.print("Isiklar(1): ");
    display.println(Isiklar_On);
    display.print("Isiklar(2): ");
    display.println(Isiklar_Arka);
    display.display();
    display.clearDisplay();
}


}
  
  
   
 
