int ldr_sen = 35;
#define light 14

void setup()
{
    Serial.begin(9600);
    pinMode(ldr_sen, INPUT);
    pinMode(light, OUTPUT);
}

void loop()
{
    int sensorValue = analogRead(ldr_sen);
    Serial.println(sensorValue);
    if (sensorValue == 4095)
        digitalWrite(light, HIGH);
    else
        digitalWrite(light, LOW);
    delay(1000);
}