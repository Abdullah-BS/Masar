/*
  Masar screen bridge (ESP32, Arduino core, no extra libraries)

    control panel --USB--> this ESP32 --Wi-Fi--> phone / tablet showing the screen page

  1. Arduino IDE: board "ESP32 Dev Module", open this folder, upload.
  2. Plug the ESP32 into the computer that runs the control panel.
     In the panel: Connect screens > ESP32 over USB > Connect over USB (Chrome or Edge).
     Many boards reboot when the port opens; wait two seconds.
  3. On the phone or tablet: join the Wi-Fi "Masar-Screen" (password below) and open
         http://192.168.4.1/?s=3          (the 3 is the screen number; http://masar.local/?s=3 may work too)
     The phone may say "no internet": stay connected.

  The page is stored in screen_page.h (generated from ../../screen.html by ../make_page.py).
  Frames from the panel, one line each:  M,<seq>,<point>,<mode>,<arrows>,<defaults>
  If no frame arrives for 3 s, the page (and the optional LED matrix below) shows the default route.

  Status: written against the Arduino-ESP32 API but NOT yet run on a real board.
*/
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "screen_page.h"

const char* AP_SSID = "Masar-Screen";
const char* AP_PASS = "masar2026";            // at least 8 characters
const int   MY_ID = 1;                        // only for a physical LED matrix on this same board (optional)
const unsigned long TIMEOUT_MS = 3000;

WebServer server(80);
String line, latest;
unsigned long lastFrame = 0;
bool haveFrame = false;
char shown = 0, defaultArrow = 'U';

String field(const String& s, int n) {        // n-th comma separated field
  int a = 0;
  for (int i = 0; i < n; i++) { a = s.indexOf(',', a); if (a < 0) return ""; a++; }
  int b = s.indexOf(',', a);
  return b < 0 ? s.substring(a) : s.substring(a, b);
}

void drawArrow(char a) {                      // U R D L = arrow, X = closed, O = off
  if (a == shown) return;
  shown = a;
  // TODO (optional): draw `a` on your LED matrix here
}

bool validFrame(const String& s) {            // only digits, capital letters, comma and minus
  if (!s.startsWith("M,") || s.length() > 120) return false;
  for (unsigned i = 0; i < s.length(); i++) {
    char c = s[i];
    if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || c == ',' || c == '-')) return false;
  }
  return true;
}

void onFrame(const String& f) {
  latest = f; lastFrame = millis(); haveFrame = true;
  String arrows = field(f, 4), defs = field(f, 5);
  if ((int)defs.length() >= MY_ID) defaultArrow = defs[MY_ID - 1];
  if ((int)arrows.length() >= MY_ID) drawArrow(arrows[MY_ID - 1]);
}

void handleRoot() {
  server.sendHeader("Cache-Control", "no-store");
  server.send_P(200, "text/html", SCREEN_HTML);
}

void handleFrame() {                          // the page polls this every 250 ms
  server.sendHeader("Cache-Control", "no-store");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  String age = haveFrame ? String(millis() - lastFrame) : String(-1);
  server.send(200, "application/json", "{\"f\":\"" + latest + "\",\"age\":" + age + "}");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);
  MDNS.begin("masar");
  server.on("/", handleRoot);
  server.on("/f", handleFrame);
  server.begin();
  drawArrow(defaultArrow);
}

void loop() {
  server.handleClient();
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      if (validFrame(line)) onFrame(line);
      line = "";
    } else if (c != '\r') {
      if (line.length() < 160) line += c;
    }
  }
  if (millis() - lastFrame > TIMEOUT_MS) drawArrow(defaultArrow);   // fail-safe
}
