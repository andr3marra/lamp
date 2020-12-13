//XX   XXXXX   XXXXXXXX  XX      XXXXXX  XXXXXX
//XX  XX   XX     XX     XX      XX      XX   XX
//XX  XX   XX     XX     XX      XXXXX   XX   XX
//XX  XX   XX     XX     XX      XX      XX   XX
//XX   XXXXX      XX     XXXXXX  XXXXXX  XXXXXX

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FastLED.h>
//#include <EEPROM.h>

unsigned long t, tLast;
int delayBreathing;
//////////////////////////////////////////// LED
#define NUM_LEDS 90
#define DATA_PIN 1
CRGB leds[NUM_LEDS];
uint8_t rgbSingleColor[3];
uint8_t rgbDoubleColor[6];
bool up = false;
uint8_t breathCount;

// EEPROM
byte eepromCycle;   // 0 1 2 3       1byte   500bytes slot
char wifiMode = 's';      // 0 ap 1 sta   1byte                             //a for ap s for st
char animationChar;
char lastAnimationChar;
char modeChar;
uint8_t brightness = 255;           // EEPROM
String ssid = "CASE";//              32bytes
String password  = "gargantua97";    //              63bytes
int r[90], g[90], b[90];

/////////////////////////////////////////// NODE MCU
#define DEFAULTSSID "NodeMCU Lamp"
#define DEFAULTPSK "12345678"

ESP8266WebServer server(80);              // instancia

void WiFiconfig(char wifiMode) {
  Serial.println(wifiMode);
  if ( wifiMode == 'a') {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(DEFAULTSSID, DEFAULTPSK);
  }
  else if ( wifiMode == 's') {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    if (MDNS.begin("ledlamp")) {
      Serial.println("MDNS responder started");
    }
    WiFi.hostname("NODE-MCU-01");
  }
  setRoutes();
  server.begin();
}

void setup(void) {
  FastLED.addLeds<WS2813, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);       // LED Strip
  WiFiconfig(wifiMode);
  Serial.begin(115200);
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
  callAnimation(animationChar);
  callMode(modeChar);
}
