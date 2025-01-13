//using ESP32
//connect dht11 to GPIO 12

#include <DHT.h>
#define cb_dht 12

DHT dht(cb_dht, DHT11);

void setup() {
  Serial.begin(9600);

  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  Serial.print("Nhiet do: ");
  Serial.println(t);
  Serial.print("Do am: ");
  Serial.println(h);

  delay(2000);
}