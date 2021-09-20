#include <WiFi.h>

#include "time.h"

const char* ssid = "GaIB22";
const char* password = "48ichg29af";


#define JST 3600* 9

void setup() {


  Serial.begin(115200);

  Serial.println("Finishing setup");


  Serial.println("start wifi");

  // WiFi接続性改善のため、いったん切断
  WiFi.disconnect( true, true ); //WiFi OFF, eraseAP=true
  delay(500);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println('.');
  }


  Serial.println("\r\nWiFi connected\r\nIP address: ");

  Serial.println(WiFi.localIP());

  configTime(JST, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}


struct tm timeinfo;

int hour = 0;
int minute = 0;

int month = 0;
int day = 0;

String text = "";

void loop() {
  delay(1000);
  Serial.println("test print");

  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
    hour = timeinfo.tm_hour;
    minute = timeinfo.tm_min;

    month = timeinfo.tm_mon;
    day = timeinfo.tm_mday;

  text =
    String(month)
    + "/"
    + String(day)
    + " "
    + String(hour)
    + ":"
    + String(minute);

  Serial.println(text);

}
