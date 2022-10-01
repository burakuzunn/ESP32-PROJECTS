
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

const char* ssid = "AKU";
const char* password = "Burak5334";
String BOTtoken = "5494870769:AAG6Kir2uUb6KDCDMkCo8GSAft_uHsdUAhM";   
String CHAT_ID = "1236166963";



bool sendPhoto = false;
bool flashPhoto = true;

WiFiClientSecure clientTCP;
UniversalTelegramBot bot(BOTtoken, clientTCP);

#define FLASH_LED_PIN 4
bool flashState = LOW;
bool photoregularly = false;
const int DoorSensor = 15;
int ReadedDoorSensor = 0;
const int buzzer = 14;
bool buzzerdegiskeni = false;
bool alarmDegiskeni = false;








//Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

//CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22


void configInitCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  //init with high specs to pre-allocate larger buffers
  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;  //0-63 lower number means higher quality
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;  //0-63 lower number means higher quality
    config.fb_count = 1;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    delay(1000);
    ESP.restart();
  }

  // Drop down frame size for higher initial frame rate
  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_CIF);  // UXGA|SXGA|XGA|SVGA|VGA|CIF|QVGA|HQVGA|QQVGA
}

void handleNewMessages(int numNewMessages) {
  Serial.print("Handle New Messages: ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }

    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;
    if (text == "/start") {

      String welcome = "Merhaba!! \n";
      welcome += "Aşağıdaki komutları kullanarak göreve devam edebilirsin. \n";
      welcome += "/photo5 : Anlık olarak 5 fotoğraf çeker\n";
      welcome += "/photoregularlyOn : Sürekli Fotoğraf Çeker\n";
      welcome += "/photoregularlyOff : Sürekli Fotoğraf Çekimini Durdurur\n";
      welcome += "/flash : Işığı Anlık olarak açar ve kapatır \n";
      welcome += "/flashonphoto : Flashlı fotoğraf çeker \n";
      welcome += "/flashoffphoto : Flashsız fotoğraf çeker \n";
      welcome += "/Soundon :Kapı Açıldığında Sesli Uyarı Verir \n";
      welcome += "/Soundoff :Kapı Açıldığında Sesli Uyarı vermez \n";
      welcome += "/Alarmon : Alarm Sistemini Aktif Eder \n";
      welcome += "/Alarmoff : Alarm Sistemini Kapatır. \n";

      bot.sendMessage(CHAT_ID, welcome, "");
    }

    if (text == "/Soundon") {
      buzzerdegiskeni = true;
      bot.sendMessage(CHAT_ID, "Komut Alındı. Kapı Açıldığında Alarm Çalacak.", "");
      Serial.println("Alarm çalarken ses öter açık");
    }
    if (text == "/Soundoff") {
      buzzerdegiskeni = false;
      bot.sendMessage(CHAT_ID, "Komut Alındı.Sessiz Mod", "");
      Serial.println("Alarm çalarken ses öter açık");
    }


    if (text == "/Alarmon") {
      alarmDegiskeni = true;
      bot.sendMessage(CHAT_ID, "Komut Alındı.Güvenlik Sistemi Aktif", "");
      Serial.println("Alarm açık");
    }
    if (text == "/Alarmoff") {
      alarmDegiskeni = false;
      bot.sendMessage(CHAT_ID, "Komut Alındı.Güvenlik Sistemi Pasif.", "");
      Serial.println("Alarm Kapalı");
    }
    if (text == "/photoregularlyOn") {
      photoregularly = true;
      bot.sendMessage(CHAT_ID, "Komut Alındı.Dur Komutu Gelene Kadar Anlık Fotoğraf Atılacak.", "");
      Serial.println("Anlık sürekli foto atan komut açıldı");
    }

    if (text == "/photoregularlyOff") {
      photoregularly = false;
      bot.sendMessage(CHAT_ID, "Komut Alındı.Dur Komutu Geldiğinden Dolayı Anlık Fotoğraf Atımı Durdu. ", "");
      Serial.println("Anlık sürekli foto atan komut kapandı.");
    }


    if (text == "/flash") {
      flashState = !flashState;
      String LedDurumu;
      digitalWrite(FLASH_LED_PIN, flashState);
      if (flashState == HIGH)
      {
        LedDurumu = "ACIK";

      }
      else {
        LedDurumu = "KAPALI";
      }
      bot.sendMessage(CHAT_ID, "Şu An Flash:" + LedDurumu, "");
      Serial.println("Changed flash LED state");
    }
    if (text == "/photo5") {
      sendPhoto = true;
      bot.sendMessage(CHAT_ID, "Komut Alındı.Peş Peşe 5 tane fotoğraf atılacak", "");
      Serial.println("New photo request");
    }
    if (text == "/flashonphoto") {
      flashPhoto = true;
      bot.sendMessage(CHAT_ID, "Komut Alındı.Bundan Sonraki Fotoğraflar Flashlı Olarak Çekilecek.", "");
      Serial.println("Flashlı fotograf cekılecek");
    }

    if (text == "/flashoffphoto") {
      flashPhoto = false;
      bot.sendMessage(CHAT_ID, "Komut Alındı.Bundan Sonraki Fotoğraflar Flash Olmadan çekilecek", "");
      Serial.println("Flashsız fotograf cekılecek");
    }
  }
}

String sendPhotoTelegram() {
  const char* myDomain = "api.telegram.org";
  String getAll = "";
  String getBody = "";


  camera_fb_t * fb = NULL;
  fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    delay(1000);
    ESP.restart();
    return "Camera capture failed";
  }


  Serial.println("Connect to " + String(myDomain));


  if (clientTCP.connect(myDomain, 443)) {
    Serial.println("Connection successful");




    String head = "--RandomNerdTutorials\r\nContent-Disposition: form-data; name=\"chat_id\"; \r\n\r\n" + CHAT_ID + "\r\n--RandomNerdTutorials\r\nContent-Disposition: form-data; name=\"photo\"; filename=\"esp32-cam.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
    String tail = "\r\n--RandomNerdTutorials--\r\n";

    uint16_t imageLen = fb->len;
    uint16_t extraLen = head.length() + tail.length();
    uint16_t totalLen = imageLen + extraLen;

    clientTCP.println("POST /bot" + BOTtoken + "/sendPhoto HTTP/1.1");
    clientTCP.println("Host: " + String(myDomain));
    clientTCP.println("Content-Length: " + String(totalLen));
    clientTCP.println("Content-Type: multipart/form-data; boundary=RandomNerdTutorials");
    clientTCP.println();
    clientTCP.print(head);

    uint8_t *fbBuf = fb->buf;
    size_t fbLen = fb->len;
    for (size_t n = 0; n < fbLen; n = n + 1024) {
      if (n + 1024 < fbLen) {
        clientTCP.write(fbBuf, 1024);
        fbBuf += 1024;
      }
      else if (fbLen % 1024 > 0) {
        size_t remainder = fbLen % 1024;
        clientTCP.write(fbBuf, remainder);
      }
    }

    clientTCP.print(tail);

    esp_camera_fb_return(fb);


    int waitTime = 10000;   // timeout 10 seconds
    long startTimer = millis();
    boolean state = false;

    while ((startTimer + waitTime) > millis()) {
      Serial.print(".");
      delay(100);
      while (clientTCP.available()) {
        char c = clientTCP.read();
        if (state == true) getBody += String(c);
        if (c == '\n') {
          if (getAll.length() == 0) state = true;
          getAll = "";
        }
        else if (c != '\r')
          getAll += String(c);
        startTimer = millis();
      }
      if (getBody.length() > 0) break;
    }
    clientTCP.stop();
    Serial.println(getBody);
  }
  else {
    getBody = "Connected to api.telegram.org failed.";
    Serial.println("Connected to api.telegram.org failed.");
  }
  return getBody;
}
void alarm() {
  if (buzzerdegiskeni) {
    digitalWrite(buzzer, HIGH);
    delay(70);
    digitalWrite(buzzer, LOW);
    delay(70);
    digitalWrite(buzzer, HIGH);
    delay(70);
    digitalWrite(buzzer, LOW);
    delay(70);
    Serial.println("Alarm Çalıyor Kapı Açık");
  }
}


void metinkontroler()
{
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();

  }


}

void besfotogonderici() {
  Serial.println("Preparing photo 5 times");
  for (int i = 0; i < 5; i++)
  {

    if (flashPhoto == true)
    {
      Serial.println("flash acıldı foto cekilecek");
      digitalWrite(FLASH_LED_PIN, HIGH);

    }

    sendPhotoTelegram();


  }
  if (flashState == LOW) {
    digitalWrite(FLASH_LED_PIN, LOW);
  }
  sendPhoto = false;
}


void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  Serial.println("sistem başladı");
  pinMode(DoorSensor, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);


  // Set LED Flash as output
  pinMode(FLASH_LED_PIN, OUTPUT);
  digitalWrite(FLASH_LED_PIN, flashState);

  // Config and init the camera
  configInitCamera();

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  clientTCP.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("ESP32-CAM IP Address: ");
  Serial.println(WiFi.localIP());
  String welcome = "Sistem Açıldı. Alarm=KAPALI,FLASH=AÇIK,SES=KAPALI \n";
  welcome += "Aşağıdaki komutları kullanarak göreve devam edebilirsin. \n";
  welcome += "/photo5 : Anlık olarak 5 fotoğraf çeker\n";
  welcome += "/photoregularlyOn : Sürekli Fotoğraf Çeker\n";
  welcome += "/photoregularlyOff : Sürekli Fotoğraf Çekimini Durdurur\n";
  welcome += "/flash : Işığı Anlık olarak açar ve kapatır \n";
  welcome += "/flashonphoto : Flashlı fotoğraf çeker \n";
  welcome += "/flashoffphoto : Flashsız fotoğraf çeker \n";
  welcome += "/Soundon :Kapı Açıldığında Sesli Uyarı Verir \n";
  welcome += "/Soundoff :Kapı Açıldığında Sesli Uyarı vermez \n";
  welcome += "/Alarmon : Alarm Sistemini Aktif Eder \n";
  welcome += "/Alarmoff : Alarm Sistemini Kapatır. \n";

  bot.sendMessage(CHAT_ID, welcome, "");


}

void loop() {
if(WiFi.status() != WL_CONNECTED){
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("wifiye tekrar bağlanılıyor");
    WiFi.reconnect();
    delay(3000);
    ReadedDoorSensor = digitalRead(DoorSensor);
    if (WiFi.status() != WL_CONNECTED && alarmDegiskeni && ReadedDoorSensor)
     { 
       alarm();
       
     }
  }
}

else{



  if (sendPhoto) {
    Serial.println("anlık 5 foto gönderici çalıştı");
    besfotogonderici();
  }











  while (photoregularly)
  {
    metinkontroler();

    if (!photoregularly) {
      break;
      if (flashState == LOW) {
        digitalWrite(FLASH_LED_PIN, LOW);
      }
    }
    if (flashPhoto == true)
    {
      Serial.println("flash acıldı foto cekilecek");
      digitalWrite(FLASH_LED_PIN, HIGH);
    }
    sendPhotoTelegram();
  }
  if (flashState == LOW) {
    digitalWrite(FLASH_LED_PIN, LOW);
  }
  sendPhoto = false;


  metinkontroler();

  if (alarmDegiskeni && WiFi.status() == WL_CONNECTED)

  {

    metinkontroler();
    ReadedDoorSensor = digitalRead(DoorSensor);

    if (ReadedDoorSensor&& WiFi.status() == WL_CONNECTED) {
      alarm();
      bot.sendMessage(CHAT_ID, "Kapı Açıldı 5 fotoğraf yolda", "");
      besfotogonderici();
      Serial.println("Kapı Açıldı");

    }



  }
}
  
}
