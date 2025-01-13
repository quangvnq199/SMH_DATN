#include <WiFi.h>

const char* ssid = "Bao Tan";
const char* password = "Khunglongxanh@";

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);  //Optional
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
}

void loop() {
  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
  delay(1000);
}