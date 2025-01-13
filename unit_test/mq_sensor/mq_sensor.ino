// using ESP32 DEVKIT V1
// connect mq_sensor to VP - GPIO 36

// if GAS => signal = 4095
// adding blynk to check
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6jvUtX6Aj"
#define BLYNK_TEMPLATE_NAME "SMART HOME BY QUANG NGUYEN"
#define BLYNK_AUTH_TOKEN "7xiLTONcgThzI2q6FT26UWHT8MHbC1-F"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <BlynkSimpleEsp32.h>
#define mq_sensor 4
int fan = 27;
char ssid[] = "Canh Ca Thai Binh";
char pass[] = "giang1992";
BlynkTimer timer;
void setup()
{
    Serial.begin(9600);
    pinMode(mq_sensor, INPUT);
    pinMode(fan, OUTPUT);
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop()
{
    float sensorValue = analogRead(mq_sensor);
    int check = 0;
    Serial.print("MQ Sensor Value: ");
    Serial.println(sensorValue);
    if (sensorValue >= 3000)
    {
        digitalWrite(fan, HIGH);
        check = 1;
    }
    else
    {
        digitalWrite(fan, LOW);
        check = 0;
    }
    Blynk.virtualWrite(V0, check);
    delay(1000); // Delay for 1 second
    Blynk.run();
    timer.run();
}
