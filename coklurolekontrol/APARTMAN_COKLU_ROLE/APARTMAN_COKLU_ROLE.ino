//sayac eklenecek

#define BLYNK_TEMPLATE_ID "TMPLkm405M-q"
#define BLYNK_DEVICE_NAME "Apartman Aydınlatma"
#define BLYNK_AUTH_TOKEN "Jw_IP63-KiopP8ysHDvd0wVLVI3iv0Mn"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiClient.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "AKU";
char pass[] = "Burak5334";


#define R1 D5
#define R2 D6
#define R3 D7
#define R4 D0


WidgetLED ROLE1VLED(V6);
WidgetLED ROLE2VLED(V7);
WidgetLED ROLE3VLED(V8);
WidgetLED ROLE4VLED(V9);



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


  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);
  Blynk.syncVirtual(V3);



}






void loop() {
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
    Blynk.syncVirtual(V1);
    Blynk.syncVirtual(V2);
    Blynk.syncVirtual(V3);
  }
}






BLYNK_WRITE(V0) {
  if (param.asInt())
  { digitalWrite(R1, LOW);
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
