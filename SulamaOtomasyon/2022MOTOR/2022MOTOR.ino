/////////////////////////////////////////////----DESİGNED BY BURAK UZUN----///////////////////////////////////////////////
//https://dl.espressif.com/dl/package_esp32_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json

#define BLYNK_TEMPLATE_ID "TMPL5Lfvzwq2qwr4e_E"//böyle bir template yok
#define BLYNK_DEVICE_NAME "Sulama"
#define BLYNK_AUTH_TOKEN "L-PkzqADd-segTfNRwhZEzA6VrnuxxOMqxCog"//böyle bir token yok
//-----------------------------------------------------------------------------------------------------------------------
#include <BlynkSimpleEsp32.h>
#include <WiFiClient.h>
#include <TimeLib.h>//bu kütüphane hata verirse adını bizzat librarye "Timelib.h"yazıp indir. bunu yorum satırına dönüştürmek malesef bir işe yaramıyor denedin. hatayı ancak libraryye yazarak çözebilirsin
#include <WidgetRTC.h>
#include <Preferences.h>
#define BLYNK_PRINT Serial
#define r 14   //röle su motoru
#define ek 27   //röle2 yani ışık
//-----------------------------------------------------------------------------------------------------------------------
BlynkTimer timer;
WidgetRTC rtc;
WidgetLCD lcd(V16);//saati i gösteriyor
WidgetTerminal terminal (V14);
WidgetLED durum1(V4);            //KAPALI led
WidgetLED durum2(V5);            //AÇIK  led
WidgetLED ekDurum1(V10);         //KAPALI  led
WidgetLED ekDurum2(V11);         //AÇIK    led
WidgetLED sys(V18);             //internet varmı blynk üzerinden yanıp sönen led
//-----------------------------------------------------------------------------------------------------------------------
String currentTime;
String currentDate;
int rssi = 0;                   //internetin ne kadar çektiği ile ilgili bar çubuğunun değişkeni
int sayac = 0;
char auth[] = BLYNK_AUTH_TOKEN;//bilerek bir harf fazladan koydun hangisi ise onu sil
char ssid[] = "AKU";
char pass[] = "Burak5334";
int counter = 0;
//-----------------------------------------------------------------------------------------------------------------------
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
 
  pinMode(r, OUTPUT);
  
  pinMode(ek, OUTPUT);
  
  digitalWrite(r, HIGH);
  

  digitalWrite(ek, LOW);
  bekle();
  Serial.begin(115200);
  Serial.println("Designed By ----BURAK UZUN----");
  Serial.println("---SİSTEM BAŞLATILIYOR 3 SANİYE BEKLEYİNİZ---");
  delay(3000);
  Serial.println("Işık ,Güvenlik Amacı İle Sistem tarafından açıldı,motor kapandı. IŞIK,Blynk sınamasına tekrar denetlenecek");
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Aşağıdaki Ağlar ile bağlantı kuruluyor");
  Serial.println("Ağ Adı,şifresi:");
  Serial.println(ssid);
  Serial.println(pass);
  bekle();
  WiFi.begin(ssid, pass);
  bekle();
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("...ilk defa wifi.h Bağlantı aranıyor...");
    kurulum();
    restartsayaci();
  }
  bekle();
  Serial.println("wifi.h İnternete bağlandı");
  digitalWrite(LED_BUILTIN, HIGH);
  bekle();
  Serial.println("başlangıç Blynk sunucu Bağlantısı Taranıyor 5 saniye sonra blynk denenecek");
  delay(5000);
  Blynk.config(auth);  // blynk.begin kullanmadın çünkü onda while döngüsü doğru çalışmıyor.bunda istediğin gibi halledebilirsin işi
  bekle();
  Blynk.connect(10000);//10 saniyede bu bağlanacak
  Serial.println("BLYNK.CONNECT,Blynk.CONFİG ÇALIŞTI");

  if (Blynk.connected()) {
    Serial.println("Blynk sunucusuna Bağlandı");
    Serial.println("Sistemimizin İnternet bağlantısı başarılı Sistem rutine başlıyor... ");
  } else {
    Serial.println("Başlangıç bağlantısı sağlanamadı sistem tekrar başlatılıyor");
    ESP.restart();
  }
  digitalWrite(LED_BUILTIN, LOW);        // işlemler bittiğinde sistem hazır olduğunu der
  delay(100);

  Blynk.syncVirtual(V2);                  //butona göre motoru ayarlar
  //Blynk.virtualWrite(V2,LOW);           //blynk butonsal olarak motoru kapatacak böylelikle elle kapanmış gibi olacak ve sistem rutinden çıkmayacak üstteki kod eklendi bunun yerine motor butonu tanımlayacak
  //motorkapandiled();
  delay(100);
  Blynk.syncVirtual(V7);//ışığı blynk ugulamasındaki butonun verdiği komuta göre halledecek
  delay(700);
//  terminal.println(" ");
//  terminal.println("İnternet bağlandı.");
//  terminal.print("Bağlanılan Ağ;");
//  terminal.println(ssid);
//  terminal.println("Blynk bağlandı.");
//  terminal.flush();

  //terminal.println("Su Motoru:Güvenlik Amacıyla Kapatıldı");
  //delay(100);
  terminal.println("=Sistem Hazır=");
  terminal.flush();
  setSyncInterval(10 * 60);
  timer.setInterval(10000L, clockDisplay);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true); 
}
//-----------------------------------------------------------------------------------------------------------------------
void loop()
{
  while (Blynk.connected() == true)
  {
    Blynk.run();
    timer.run();
    rssi = WiFi.RSSI();
    Blynk.virtualWrite(V15, rssi); //internetin ne kadar çektiği ile ilgili bar açmıştık ona veri gönderiyor.
    whathappendsys();
    BLYNK_WRITE(V14);
  }
  while (Blynk.connected() == false)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    Serial.println("Blynk Sunucusu ve wifih  kaybedildi tekrar bağlanmak için kod dizini çalıştırılıyor...");
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.reconnect();
      delay(5000);
      kurulum();
      Serial.println("wifi.h loopta 1. kez bağlanmaya çalışıyor...");
      restartsayaci();

    }
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true); 
    delay(1000);
    Serial.println("wifi.h kütüphanesi bağlandı 1 saniye sonra blynk kütüphanesi while döngüsünde tekrar bağlanmayı deneyecek");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!eğer internette sorun çıkarsa bu değeri 5000 yap(zamandan kazanmak için bunu küçülttün)
    while (Blynk.connected() == false) {
      Serial.println("Blynk.connect  ve config  çalıştırılıyor");
      Blynk.config(auth);
      Blynk.connect(1000);
      restartsayaci();
      while (WiFi.status() != WL_CONNECTED) {
        WiFi.reconnect();
        kurulum();
        delay(5000);
        Serial.println("wifi.h loopta 2. kez bağlanmaya çalışıyor...");
        restartsayaci();
      }
    }
    Serial.println("Blynk kütüphanesi aktif");
    terminal.print("*");
    Blynk.syncVirtual(V7);//ışığı butona göre açacak
    delay(100);
    Blynk.syncVirtual(V2);//dikkat et motoru butona göre açabilir!!!!!!!!!!!
    terminal.flush();
    Serial.println("Sistem rutine oturdu devam ediliyor...");
    Serial.println("blynk işliyor...");
    digitalWrite(LED_BUILTIN, LOW);
  }
}
//-----------------------------------------------------------------------------------------------------------------------
BLYNK_WRITE(V2) {

  int motor = param.asInt();
  if (param.asInt() == 1) {
    Serial.println("su motoru buton komut durumu blynk:");
    Serial.println(motor);
    led();
    digitalWrite(r, LOW);                                                 //su motor acildi
    Serial.println("Su Motoru  açıldı");
    motoracildiled();
    terminal.println("Su Motoru Açıldı");
    counter = counter + 1;
    
    nezamanoldu();
    terminal.flush();
  }
  if (param.asInt() == 0) {
    Serial.println("su motoru buton komut durumu blynk:");
    Serial.println(motor);
    led();
    digitalWrite(r, HIGH);                                //motoru kapatır
    Serial.println("Su Motoru Kapatıldı");
    motorkapandiled();
    terminal.println("Su Motoru Kapatıldı");
    terminal.flush();
    nezamanoldu();
  }
}
BLYNK_WRITE(V6)
{
  int zamanlayicimotor = param.asInt();
  if (param.asInt() == 1) {
    Serial.println("zamanlayıcı motor  durumu");
    Serial.println(zamanlayicimotor);
    led();
    digitalWrite(r, HIGH);                                //motoru KAPATIR zamanlayıcı ile
    Serial.print("Su motoru zamanlayıcı ile kapandı");
    motorkapandiled();
    Blynk.virtualWrite(V2, LOW);
    terminal.println("Su motoru zamanlayıcı ile kapandı.");
    nezamanoldu();
    terminal.print("Gün Sonu Sayaç:");
    terminal.println(counter);
    counter = 0;
    terminal.println("Günlük Sayaç Otomatik Sıfırlandı");
    terminal.flush();
  }
}
//-----------------------------------------------------------------------------------------------------------------------
BLYNK_WRITE(V19)
{
  int ne = param.asInt();
  Serial.println("zamanlayıcı durumu:");
  Serial.println(ne);
  if (param.asInt() == 1) {
    led();
    digitalWrite(ek, LOW);                                //ek i açar ZAMANLAYICI İLE
    Serial.print("ışık zamanlayıcı ile açıldı.");
    isikacildiled();
    terminal.println("Işık ZAMANLAYICI ile AÇILDI.");
    Blynk.virtualWrite(V7, HIGH);
    nezamanoldu();
  }
  if (param.asInt() == 0) {
    Serial.println("zamanlayıcı durumu");
    Serial.println(ne);
    led();
    digitalWrite(ek, HIGH);                                //ek i kapatır zamanlayıcı ile kapar
    Serial.print("ışık zamanlayıcı ile kapatıldı");
    isikkapandiled();
    terminal.println("Işık zamanlayıcı ile KAPATILDI");
    Blynk.virtualWrite(V7, LOW);
    nezamanoldu();
  }
}
//-----------------------------------------------------------------------------------------------------------------------
BLYNK_WRITE(V7)
{
  int isik = param.asInt();

  if (param.asInt() == 1) {
    Serial.println("ışık için buton komut durumu:");
    Serial.println(isik);
    led();
    digitalWrite(ek, LOW);
    isikacildiled();                           //ek i açar
    Serial.println("ışık  açıldı");
    terminal.println("Işık Açıldı");
    nezamanoldu();
  }
  if (param.asInt() == 0) {
    Serial.println("ışık içinbuton komut durumu");
    Serial.println(isik);
    led();
    digitalWrite(ek, HIGH);                                //ek i kapar
    Serial.print("ışık  kapatıldı");
    isikkapandiled();
    terminal.println("Işık KAPATILDI");
    nezamanoldu();
  }
}
//-----------------------------------------------------------------------------------------------------------------------
BLYNK_WRITE(V12)
{
  Serial.println("RESTARTİNG...");
  terminal.println("RESTARTİNG...");
  terminal.flush();
  nezamanoldu();
  ESP.restart();
}
//-----------------------------------------------------------------------------------------------------------------------
BLYNK_WRITE(V17)
{
  terminal.println(currentTime);
  terminal.print(" ");
  terminal.println(currentDate);
  terminal.flush();
}
//-----------------------------------------------------------------------------------------------------------------------
BLYNK_WRITE(V14)
{
  String a = param.asStr();
  Serial.println(a);


  if (a == "00") {
    terminal.println(" ");
    terminal.println("01)Ağ Bilgisi");
    terminal.println("02)Restart");
    terminal.println("03)Tarih Bilgisi");
    terminal.println("04)Terminal Temizleme");
    terminal.println("05)Motor Sayaç");
    terminal.println(" 051)Sayaç Sıfırlama");
    terminal.println("06)RSSI");
    terminal.flush();
  }

  else if (a == "01")
  {
    terminal.println(" ");
    terminal.println("Bağlanılan Ağ Bilgisi");
    terminal.print("ssid:");
    terminal.println(ssid);
    terminal.print("pass:");
    terminal.println(pass);
    terminal.flush();
  }
  else if (a == "06")
  {
    terminal.println(" ");
     
    terminal.print("rssi:");
    terminal.println(rssi);
    terminal.flush();
  }


  else if (a == "02")
  {
    Serial.println("RESTARTİNG...");
    terminal.println("RESTARTİNG...");
    terminal.flush();
    nezamanoldu();
    ESP.restart();
  }

  else if (a == "03")
  {
    terminal.println("Tarih");
    terminal.println(currentTime);
    terminal.print(" ");
    terminal.println(currentDate);
    terminal.flush();
  }
  else if (a == "04")
  {
    terminal.clear();
  }
  else if (a == "05")
  {
    Serial.println("Günlük motor açılma sayısı:");
    Serial.print(counter);
    terminal.println(" ");
    terminal.print("Günlük Açılma:");
    terminal.println(counter);
    terminal.flush();

  }

  else if (a == "051")
  {
    counter = 0;
    terminal.println("Sayaç sıfırlandı");
    terminal.flush();
  }

  else
  {
    terminal.println("//*Hatalı komut*//");
    terminal.flush();
  }








}

//-----------------------------------------------------------------------------------------------------------------------

BLYNK_CONNECTED()
{
  rtc.begin();
}
//-----------------------------------------------------------------------------------------------------------------------
void clockDisplay()
{
  // You can call hour(), minute(), ... at any time
  // Please see Time library examples for details
  currentTime = String(hour()) + ":" + minute();
  currentDate = String(day()) + "/" + month() + "/" + year();
  //Serial.print("Current time: ");
  //Serial.print(currentTime);
  //Serial.print(" ");
  //Serial.print(currentDate);                                      //rtc zamazingosu
  //Serial.println();
  // Send time to the App
  Blynk.virtualWrite(V16, currentTime);
  // Send date to the App
  Blynk.virtualWrite(V17, currentDate);
}
//-----------------------------------------------------------------------------------------------------------------------
void led()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);                                             //ESP ÜZERİNDEKİ (KOMUT ALMA BİLDİRİM) MAVİ LED
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}
//-----------------------------------------------------------------------------------------------------------------------
void kurulum()
{
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);                                             //ESP ÜZERİNDEKİ  MAVİ LED wifi.h ta tek tek yanıp sönecek
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
  }
}
//-----------------------------------------------------------------------------------------------------------------------

void nezamanoldu() { //buton komut gönderdiğinde ne zaman olduğu ile ilgili tarihi yazıyor
  terminal.print(" ");
  terminal.println(currentTime);
  //    terminal.print(" ");
  //    terminal.println(currentDate);
  //    terminal.println("----------");
  terminal.flush();
}
//-----------------------------------------------------------------------------------------------------------------------
void isikacildiled() {
  ekDurum1.off();
  ekDurum2.on();
}
//-----------------------------------------------------------------------------------------------------------------------
void isikkapandiled() {
  ekDurum1.on();
  ekDurum2.off();
}
//-----------------------------------------------------------------------------------------------------------------------
void motorkapandiled() {
  durum1.on();
  durum2.off();
}
//-----------------------------------------------------------------------------------------------------------------------
void motoracildiled() {
  durum1.off();
  durum2.on();
}
//-----------------------------------------------------------------------------------------------------------------------
void whathappendsys() {//sistem internete bağlımı onu gösteriyor
  sys.on();
  sys.off();
}
//-----------------------------------------------------------------------------------------------------------------------
void restartsayaci() {
  sayac = sayac + 1;
  delay(1000);
  Serial.println(sayac);
  if (sayac == 200)
  {
    Serial.println("internet bağlantısı sağlanamadığından dolayı sistem yeniden başlatılıyor");
    delay(3000);
    sayac = 0;
    ESP.restart();
  }

}


void bekle ()
{

  delay(500);


}
