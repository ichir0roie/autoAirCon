#include <WiFi.h>

const char* ssid = "GaIB22";
const char* password = "48ichg29af";


const uint16_t kIrLed = 26;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

void setup() {
  Serial.begin(115200);

  delay(200);
  setupWifi();
}

struct tm timeInfo;//時刻を格納するオブジェクト
char s[20];//文字格納用

int onTimeNight = 23;
int onTimeMorning = 6;

int coolerStartMonth = 7;
int coolerEndMonth = 9;
int heaterStartMonth = 11;
int heaterEndMonth = 3;

bool sended = false;
//int loopDist=1000*60*5;
int loopDist = 1000;

void loop() {
  //  setupWifi();
  configTime(9 * 3600L, 0, "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");//NTPの設定
  getLocalTime(&timeInfo);//tmオブジェクトのtimeInfoに現在時刻を入れ込む

  //  int month=timeInfo.tom_mon+1;
  //  int hour=timeInfo.tm_hour;
  //  int minute=timeInfo.tm_min;
  int month = 11;
  int hour = timeInfo.tm_sec / 2;
  int minute = 0;

  sprintf(s, " %02d/%02d %02d:%02d",
          month, timeInfo.tm_mday, hour, minute);
  delay(500);
  Serial.println(s);//時間をシリアルモニタへ出力


  if (!sended && (hour == onTimeNight || hour == onTimeMorning)) {
    Serial.println("hit target time");
    if (coolerStartMonth <= month && month <= coolerEndMonth) {
      sendCool();
      sended = true;
    } else if ((heaterStartMonth <= month && month <= 12)
               || (1 <= month && month <= heaterEndMonth)) {
      sendHeat();
      sended = true;
    }
  }

  if (sended && minute > 30) {
    sended = false;
    Serial.println("reset status");
  }


  delay(loopDist);
}

void setupWifi() {

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (WiFi.begin(ssid, password) != WL_DISCONNECTED) {
    ESP.restart();
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  Serial.println("Connected to the WiFi network!");
}



void sendCool() {
  Serial.println("send cooler");
}

void sendHeat() {
  Serial.println("send heater");
}

void sendOffTimer(uint8_t sendMode, uint8_t sendTemp) {
}
