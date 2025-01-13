#define fan 27
#define light 14

void setup()
{
    pinMode(light, OUTPUT);
    pinMode(fan, OUTPUT);
}
void loop()
{
    digitalWrite(light, HIGH);
    digitalWrite(fan, LOW);
    delay(5000);

    digitalWrite(light, LOW);
    digitalWrite(fan, HIGH);
    delay(5000);
}