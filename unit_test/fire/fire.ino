// using ESP32 DEVKIT V1
// connect fire_sensor to VN - GPIO 39

// if FIRE => signal = 0
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6jvUtX6Aj"
#define BLYNK_TEMPLATE_NAME "SMART HOME BY QUANG NGUYEN"
#define BLYNK_AUTH_TOKEN "7xiLTONcgThzI2q6FT26UWHT8MHbC1-F"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <BlynkSimpleEsp32.h>
#define fire_sensor 39
char ssid[] = "Canh Ca Thai Binh";
char pass[] = "giang1992";
BlynkTimer timer;
void setup()
{
    Serial.begin(9600);
    pinMode(fire_sensor, INPUT);
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop()
{
    int sensorValue = analogRead(fire_sensor);
    Serial.print("fire Sensor Value: ");
    Serial.println(sensorValue);

    int check = 0;
    if (sensorValue <=100)
    {
        check = 1;
    }
    else
    {
        check = 0;
    }
    Blynk.virtualWrite(V2, check);
    delay(1000); // Delay for 1 second
    Blynk.run();
    timer.run();
}