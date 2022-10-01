#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiClient.h>
int sayac = 0;
char auth[] = "jJzMlocJYovTR5cFqqoql6kgoepzqM7Y";
char ssid[] ="UZUNOGLU_APARTMAN"; 
char pass[] ="Burak5334"; 
#define PİN 0
//http://blynk-cloud.com/jJzMlocJYovTR5cFqqoql6kgoepzqM7Y/update/V0=1
void setup()
{
  pinMode(PİN, OUTPUT);
  digitalWrite(PİN,HIGH);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); 
  
  
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
   Blynk.syncVirtual(V0);
   digitalWrite(LED_BUILTIN, HIGH); 
}

void loop()
{
  while (Blynk.connected() == true)
  {
    Blynk.run();
    
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
    
  }
}


BLYNK_WRITE(V0) {
  if (param.asInt())
  { digitalWrite(PİN, HIGH);
    Serial.println("İşlem Gerçekleşti.b");
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(100);                       // wait for a second
    digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
    delay(100);
    
  }
  else{digitalWrite(PİN,LOW);
    Serial.println("İşlem Gerçekleşti.a");
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
  delay(100);
  sayac=sayac+1;
    Blynk.virtualWrite(V1,sayac);
  }
  
}

BLYNK_WRITE(V2) {

sayac=0;
  Blynk.virtualWrite(V1,sayac);
}

 
