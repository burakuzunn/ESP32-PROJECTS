//projede yapılacak yenilikler
//  REMOTE OLARAK KOD YÜKLEMESİ YAPILARAK
//  OLED DİSPLAY ÖZELLİĞNİN ALTYAPISI YAPILACAK
//proje api:https://fra1.blynk.cloud/external/api/update?token=Jw_IP63-KiopP8ysHDvd0wVLVI3iv0Mn&v4=1

#define BLYNK_TEMPLATE_ID "TMPLkm405M-q"
#define BLYNK_DEVICE_NAME "Apartman Aydınlatma"
#define BLYNK_AUTH_TOKEN "Jw_IP63-KiopP8ysHDvd0wVLVI3iv0Mn"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "**";
char pass[] = "**";

unsigned long konumTetikZaman1=0;
unsigned long konumTetikZaman2=0;
bool  KonumTetik=false;
bool KonumTetik2=false;//bu olmadığında,ışık el ile açıldığı zaman binanın konumuna biri girdiğinde elle açılsa bile otomatik kapanıyor.

#define R1 D5//duvar üstü
#define R2 D6//ön duvar altı
#define R3 D7//arka duvar altı
#define R4 D0//boş


WidgetLED ROLE1VLED(V6);
WidgetLED ROLE2VLED(V7);
WidgetLED ROLE3VLED(V8);
WidgetLED ROLE4VLED(V9);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);


void setup() {

  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
  digitalWrite(R1, HIGH);
  digitalWrite(R2, HIGH);
  digitalWrite(R3, HIGH);
  digitalWrite(R4, HIGH);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(115200);
  Serial.print("Sistem Açıldı");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(5000);
    Serial.println("...ilk defa  Bağlantı aranıyor...");
    Serial.print(ssid);
    Serial.print(pass);

  }
  Serial.println("wifi.h İnternete bağlandı");
  Blynk.config(auth);   // blynk.begin kullanmadın çünkü onda while döngüsü doğru çalışmıyor.bunda istediğin gibi halledebilirsin işi
  Blynk.connect(5000); //10 saniyede bu bağlanacak
  if (Blynk.connected()) {
    Serial.println("Blynk sunucusuna Bağlandı");
    Serial.println("Sistemimizin İnternet bağlantısı başarılı Sistem rutine başlıyor... ");
  } else {
    Serial.println("Başlangıç bağlantısı sağlanamadı sistem tekrar başlatılıyor");
    ESP.restart();
  }

  timeClient.begin();
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);
  Blynk.syncVirtual(V3);
  Blynk.syncVirtual(V4);


}






void loop() {
  while (Blynk.connected() == true)
  {

    Blynk.run();
    Blynk.sendInternal("rtc", "sync");
    isikkapat();
     
    
  }

  while (Blynk.connected() == false)
  {
    Serial.println("Blynk Sunucusu ve wifih kaybedildi tekrar bağlanmak için kod dizini çalıştırılıyor...");
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.reconnect();
      delay(5000);
      Serial.println("wifi 1. kez bağlanmaya çalışıyor...");
    }
    Serial.println("wifi kütüphanesi bağlandı  blynk kütüphanesi  tekrar bağlanmayı deneyecek");
    while (Blynk.connected() == false) {
      Serial.println("Blynk.connect  ve config  çalıştırılıyor");
      Blynk.config(auth);
      Blynk.connect(5000);
    }
    Serial.print("Sistem Rutinde :) :) ");
    
    Blynk.syncVirtual(V0);
    Blynk.syncVirtual(V1);
    Blynk.syncVirtual(V2);
    Blynk.syncVirtual(V3);
    Blynk.syncVirtual(V4);
 
  }
}






BLYNK_WRITE(V0) {
  if (param.asInt())
  { digitalWrite(R1, LOW);
    KonumTetik2=false;
    Blynk.setProperty(V6, "color", "#05f561");
    ROLE1VLED.on();

    Serial.println("İşlem Gerçekleşti.");
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(100);                       // wait for a second
    digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
    delay(100);
  }
  else {
    digitalWrite(R1, HIGH);
    KonumTetik2=true;
    Blynk.setProperty(V6, "color", "#f50505");
    ROLE1VLED.on();

    Serial.println("İşlem Gerçekleşti.");
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(100);                       // wait for a second
    digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
    delay(100);
  }
}

BLYNK_WRITE(V1) {
  if (param.asInt())

  {
    digitalWrite(R2, LOW);
    KonumTetik2=false;
    
    Blynk.setProperty(V7, "color", "#05f561");
    ROLE2VLED.on();

    Serial.println("İşlem Gerçekleşti.");
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(100);                       // wait for a second
    digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
    delay(100);
  }
  else {
    digitalWrite(R2, HIGH);
    KonumTetik2=true;
    Blynk.setProperty(V7, "color", "#f50505");
    ROLE2VLED.on();

    Serial.println("İşlem Gerçekleşti.");
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(100);                       // wait for a second
    digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
    delay(100);
  }
}

BLYNK_WRITE(V2) {
  if (param.asInt())
  { digitalWrite(R3, LOW);
  KonumTetik2=false;
    Blynk.setProperty(V8, "color", "#05f561");
    ROLE3VLED.on();

    Serial.println("İşlem Gerçekleşti.");
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(100);                       // wait for a second
    digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
    delay(100);
  }
  else {
    digitalWrite(R3, HIGH);
    KonumTetik2=true;
    Blynk.setProperty(V8, "color", "#f50505");
    ROLE3VLED.on();

    Serial.println("İşlem Gerçekleşti.");
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(100);                       // wait for a second
    digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
    delay(100);
  }
}

BLYNK_WRITE(V3) {
  if (param.asInt())
  { digitalWrite(R4, LOW);
  KonumTetik2=false;
    Blynk.setProperty(V9, "color", "#05f561");
    ROLE4VLED.on();

    Serial.println("İşlem Gerçekleşti.");
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(100);                       // wait for a second
    digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
    delay(100);
  }
  else {
    digitalWrite(R4, HIGH);
    KonumTetik2=true;
    Blynk.setProperty(V9, "color", "#f50505");
    ROLE4VLED.on();

    Serial.println("İşlem Gerçekleşti.");
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(100);                       // wait for a second
    digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
    delay(100);
  }
}

BLYNK_WRITE(V4)
{
  if (param.asInt())
  {
    KonumTetik2=false;
    digitalWrite(R1, LOW);
    Blynk.setProperty(V6, "color", "#05f561");
    ROLE1VLED.on();
    Blynk.virtualWrite(V0, HIGH);


    digitalWrite(R2, LOW);
    Blynk.setProperty(V7, "color", "#05f561");
    ROLE2VLED.on();
    Blynk.virtualWrite(V1, HIGH);

    digitalWrite(R3, LOW);
    Blynk.setProperty(V8, "color", "#05f561");
    ROLE3VLED.on();
    Blynk.virtualWrite(V2, HIGH);

    digitalWrite(R4, LOW);
    Blynk.setProperty(V9, "color", "#05f561");
    ROLE4VLED.on();
    Blynk.virtualWrite(V3, HIGH);

  }

  else
  {
    KonumTetik2=true;
    digitalWrite(R1, HIGH);
    Blynk.setProperty(V6, "color", "#f50505");
    ROLE1VLED.on();
    Blynk.virtualWrite(V0, LOW);

    digitalWrite(R2, HIGH);
    Blynk.setProperty(V7, "color", "#f50505");
    ROLE2VLED.on();
    Blynk.virtualWrite(V1, LOW);

    digitalWrite(R3, HIGH);
    Blynk.setProperty(V8, "color", "#f50505");
    ROLE3VLED.on();
    Blynk.virtualWrite(V2, LOW);

    digitalWrite(R4, HIGH);
    Blynk.setProperty(V9, "color", "#f50505");
    ROLE4VLED.on();
    Blynk.virtualWrite(V3, LOW);






  }
}

BLYNK_WRITE(V5)
///butonu yok. Konuma yaklaştığında iftt üzerinden bu linki tetikler.
///
/// Bu fonksiyon bütün ışıkları 3 dklığına açmaya yarar. 19.00-06.00 arasında 
//  eğer fonksiyon ışıkları açmaz ise saatten dolayıdır. Dalgınlığa gelip kendini sorgulama yani.
///
///
///
{ 
     if (param.asInt()){
  
    konumTetikZaman1=millis();
    KonumTetik=true;
    int refsaatblynk=0;
    refsaatblynk=NTPsaat();
   
    
    if(refsaatblynk >=19 || refsaatblynk<=6){
      digitalWrite(R1, LOW);
      Blynk.setProperty(V6, "color", "#05f561");
      ROLE1VLED.on();
      Blynk.virtualWrite(V0, HIGH);
  
      delay(200);
    
      digitalWrite(R2, LOW);
      Blynk.setProperty(V7, "color", "#05f561");
      ROLE2VLED.on();
      Blynk.virtualWrite(V1, HIGH);
  
      delay(200);
      digitalWrite(R3, LOW);
      Blynk.setProperty(V8, "color", "#05f561");
      ROLE3VLED.on();
      Blynk.virtualWrite(V2, HIGH);
    }
    else{Serial.println("işlem,belirlediğiniz saat dışında olduğu için gerçekleştirilemiyor. if komut bloğuna bakın.");}
    

  
}}


void isikkapat()//bir üst fonksiyonda açılan ışıkları 3 dakika sonra kapatır.
{
    
    konumTetikZaman2=millis();
    //Serial.println("aradaki fark");
    Serial.println(konumTetikZaman2-konumTetikZaman1);
    
    if(konumTetikZaman2-konumTetikZaman1>180000 && KonumTetik==true && KonumTetik2==true)
    {
    
    KonumTetik=false;
    
    digitalWrite(R1, HIGH);
    Blynk.setProperty(V6, "color", "#f50505");
    ROLE1VLED.on();
    Blynk.virtualWrite(V0, LOW);

    delay(500);
    
    digitalWrite(R2, HIGH);
    Blynk.setProperty(V7, "color", "#f50505");
    ROLE2VLED.on();
    Blynk.virtualWrite(V1, LOW);
    delay(500);
    
    digitalWrite(R3, HIGH);
    Blynk.setProperty(V8, "color", "#f50505");
    ROLE3VLED.on();
    Blynk.virtualWrite(V2, LOW);
    
  } 
}


BLYNK_WRITE(V10){//SELAM VERDİRİR

  if (param.asInt()){
   for(int i=0;i<3;i++)
   {
    digitalWrite(R1, LOW);
    delay(50);
   digitalWrite(R2, LOW);
   delay(50);
   digitalWrite(R3, LOW);
    delay(200);
    digitalWrite(R1, HIGH);
    delay(50);
   digitalWrite(R2, HIGH);
   delay(50);
   digitalWrite(R3, HIGH);
   delay(50);
    
    }


  
for(int i=0;i<3;i++){
  
   digitalWrite(R1, LOW);
   delay(200);
   digitalWrite(R2, LOW);
   delay(200);
   digitalWrite(R3, LOW);
   delay(200);

   digitalWrite(R1, HIGH);
   delay(200);
   digitalWrite(R1, LOW);
   digitalWrite(R2, HIGH);
   delay(200);

   digitalWrite(R2, LOW);
   delay(200);
   digitalWrite(R3, HIGH);
   delay(200);
   
   
    
  
   
}

  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);
  Blynk.syncVirtual(V3);
  Blynk.syncVirtual(V4);

  }}









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
