#include <NTPClient.h>
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
//#include <WiFi.h> // for WiFi shield
//#include <WiFi101.h> // for WiFi 101 shield or MKR1000
#include <WiFiUdp.h>

const char *ssid     = "AKU";
const char *password = "Burak5334";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void setup(){
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
  
   
}

 
 


void loop() {
  Serial.println(NTPsaat());
  delay(1000);
}

int NTPsaat(){
  timeClient.update();
  String strSaatRakamlari;
  int ReferansSaat=0;
  int ReferansDakika=0;
  String formattedDate = timeClient.getFormattedDate();
  int splitT = formattedDate.indexOf("T");
  String timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);  
  strSaatRakamlari += timeStamp[0];
  strSaatRakamlari += timeStamp[1];
  ReferansSaat=strSaatRakamlari.toInt()+3;
  strSaatRakamlari="";
  strSaatRakamlari += timeStamp[3];
  strSaatRakamlari += timeStamp[4];
  ReferansDakika=strSaatRakamlari.toInt();
  //Serial.print(ReferansSaat);
  //Serial.print(":");
  //Serial.print(ReferansDakika);

  return ReferansSaat;
  }
