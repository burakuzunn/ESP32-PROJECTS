//bu projede nodemcu üzerindeki D1 PİNİNİ grounda çekerek 3 saniye beklemeniz gerekmektedir. ardından nodemcu kartı,
//kendini ap moduna alır ve sizin wifi ile kendisine bağlanmanızı bekler.
//WİFİ İLE bağlandıktan sonra kartın bağlanmasını istediğiniz ağı seçerek kaydedersiniz.
//artık siz tekrar değiştirene kadar kart bu ağa bağlanacaktır.
//bu projede kullanılan wifimanager kütüphanesinin html kısımları özelleştirilmiştir. Bundan dolayın kodun beraberinde gelen
//wifimanager kütüphanesini library ekle diyererek zip halinde dahil etmeniz sizin için daha iyi olur.
#include <WiFiManager.h>
bool res = false, resetle = false, button = false;
int buttonSayac = 0;
int sayac = 0;

WiFiManager wm;

void setup() {
  pinMode(D1, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println("Sistem Yeniden Başladı");
  WiFi.mode(WIFI_STA);
  res = wm.autoConnect("Home_Security_System", "t4?319Zk");
  attachInterrupt(digitalPinToInterrupt(D1), butonfonksiyonu, CHANGE); //pinin durumu CHANGE, RISING ve FALLING olabilir.
}
void loop()
{
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.reconnect();
    Serial.print("Bağlantı Koptu");
    delay(3000);

    denetleyici();

  }
denetleyici();

}


void denetleyici()
{
  if (resetle)
    {
      resetle = false;
      Serial.print("wifi bağlantısı sıfırlanıyor");
      WiFi.disconnect(true);//ESPnin bağlantısını ağdan koparıyoruz
      delay(2000);
      wm.resetSettings();//hafızadaki ağ ayarlarını temizliyoruz
      delay(3000);
      ESP.reset();//esp'ye reset atıp yeniden başlatıyoruz
      delay(5000);
    }
  
  }

ICACHE_RAM_ATTR void butonfonksiyonu() {
  if (!button) { //butona şu ana kadar hiç basılmamışsa yani ilk basışı ifade ediyoruz
    if (digitalRead(D1) == LOW) { //ve D1 pini HIGH olmuş yani butona basılmışsa
      buttonSayac = millis(); //basma süresini takip edecek bir sayac tanımlıyoruz
      button = true; //ve butona basıldı bilgisini tutan değişkeni TRUE yapıyoruz.
    }
  }
  else { // eğer butona daha öncesinde basılmış ise
    if (digitalRead(D1) == HIGH) { //ve artık buton bırakılmış ise
      button = false; //butonu tekrar false a çek
      if ((millis() - buttonSayac) >= 50) { //eğer bu basma zarfında basma süresi 50ms den büyük ise hareketi kaale al ve
        if ((millis() - buttonSayac) >= 3000) { //toplam basma süresi 3sn den fazla sürmüş i
          Serial.println("3 saniyeden fazla butona basıldı");
          resetle = true;


        } else {       //////3sn den az bir basma olmuş ise

          sayac = 0;  //işimiz bitince de buton süre sayacını sıfırla
          Serial.println("<3 sn");
        }
      }
    }
  }

}
