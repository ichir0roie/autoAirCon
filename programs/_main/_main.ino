#include <WiFi.h>

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Panasonic.h>


const char* ssid = "GaIB22";
const char* password = "48ichg29af";


const uint16_t kIrLed = 26;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRPanasonicAc ac(kIrLed);  // Set the GPIO used for sending messages.

void setup() {
  Serial.begin(115200);

  delay(200);
//  setupWifi();
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
int loopDist=1000*60*5;
//int loopDist = 5000;

void loop() {
    setupWifi();
  configTime(9 * 3600L, 0, "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");//NTPの設定
  getLocalTime(&timeInfo);//tmオブジェクトのtimeInfoに現在時刻を入れ込む

    int month=timeInfo.tm_mon+1;
    int hour=timeInfo.tm_hour;
    int minute=timeInfo.tm_min;
//  int month = 11;
//  int hour = timeInfo.tm_sec / 2;
//  int minute = 0;

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



void printState() {
  // Display the settings.
  Serial.println("Panasonic A/C remote is in the following state:");
  Serial.printf("  %s\n", ac.toString().c_str());
  // Display the encoded IR sequence.
  unsigned char* ir_code = ac.getRaw();
  Serial.print("IR Code: 0x");
  for (uint8_t i = 0; i < kPanasonicAcStateLength; i++)
    Serial.printf("%02X", ir_code[i]);
  Serial.println();
  Serial.println();
  Serial.println();
}


void sendOrder(uint8_t sendMode, uint8_t sendTemp) {

  Serial.println("Setting desired state for A/C.");
  ac.setModel(kPanasonicJke);
  ac.on();
  ac.setMode(sendMode);
  ac.setTemp(sendTemp);
  ac.setFan(kPanasonicAcFanAuto);
  ac.setSwingVertical(kPanasonicAcSwingVAuto);
  ac.setSwingHorizontal(kPanasonicAcSwingHAuto);
  ac.send();
  printState();
}


void sendCool() {
  sendOrder(kPanasonicAcCool, 27);
  delay(1000);
  sendOffTimer(kPanasonicAcCool, 27);
}

void sendHeat() {
  sendOrder(kPanasonicAcHeat, 24);
  delay(1000);
  sendOffTimer(kPanasonicAcHeat, 24);
}

void sendOffTimer(uint8_t sendMode, uint8_t sendTemp) {
  ac.setOffTimer(60);
  ac.send();
  printState();
}
