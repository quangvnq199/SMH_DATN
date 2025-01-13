
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

#define cb_dht 12
#define speaker 25
#define v_ref 32
#define mq_sensor 4
#define control 33
#define fire_sensor 39
#define fan 27
#define light 14
#define door 26
#define ldr 35
#define SS_PIN 5
#define RST_PIN 17


DHT dht(cb_dht, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 rfid(SS_PIN, RST_PIN);
Servo myservo;


byte authorizedUID1[] = {0x65, 0xdd, 0x0f, 0xad};
byte authorizedUID2[] = {0xf6, 0x8c, 0xf6, 0x03};
byte authorizedUID3[] = {0xc1, 0x2d, 0xa3, 0x7b};

void setup()
{
  Serial.begin(9600);

  pinMode(speaker, OUTPUT);
  pinMode(v_ref, INPUT);
  pinMode(mq_sensor, INPUT);
  pinMode(control, OUTPUT);
  pinMode(fire_sensor, INPUT);
  pinMode(light, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(ldr, INPUT);

  myservo.attach(door);

  SPI.begin();
  rfid.PCD_Init();

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("STARTING.....");
  digitalWrite(speaker, HIGH);
  delay(500);
  digitalWrite(speaker, LOW);
  lcd.clear();

  dht.begin();

}

void loop()
{
  charger();
  gas_check(mq_sensor);
  fire_check(fire_sensor);
  //light_check();
  float h = dht.readHumidity();
  float t = dht.readTemperature();


  String content = "";
  myservo.write(0);
  lcd.setCursor(0, 0);
  lcd.print("MOI DUA THE VAO:");

  lcd.setCursor(0, 1);
  lcd.print("t:");
  lcd.setCursor(2, 1);
  lcd.print(t, 1);
  lcd.setCursor(6, 1);
  lcd.print("'C h:");
  lcd.setCursor(11, 1);
  lcd.print(h, 1);
  lcd.setCursor(15, 1);
  lcd.print("%");

  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
  {
    return;
  }
  for (byte i = 0; i < rfid.uid.size; i++)
  {
    content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(rfid.uid.uidByte[i], HEX));
  }

  bool authorized = false;
  for (byte i = 0; i < rfid.uid.size; i++)
  {
    if (rfid.uid.uidByte[i] == authorizedUID1[i] || rfid.uid.uidByte[i] == authorizedUID2[i])
    {
      authorized = true;
      break;
    }
  }

  if (authorized)
  {
    digitalWrite(speaker, HIGH);
    delay(500);
    digitalWrite(speaker, LOW);
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.print("Welcome home...");
    myservo.write(90);
    delay(3000);
    myservo.write(0);
  }
  else
  {
    digitalWrite(speaker, HIGH);
    delay(500);
    digitalWrite(speaker, LOW);
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.print("THE KHONG HOP LE");
    myservo.write(0);
    delay(3000);
  }

  rfid.PICC_HaltA();

  delay(1000);

}

double ReadVoltage(byte pin)
{
  double reading = analogRead(pin);
  if (reading < 1 || reading > 4095)
    return 0;
  return -0.000000000000016 * pow(reading, 4) + 0.000000000118171 * pow(reading, 3) - 0.000000301211691 * pow(reading, 2) + 0.001109019271794 * reading + 0.034143524634089;
}

void fire_check(int y)
{
  int fire = analogRead(y);
  if (fire <= 150)
  {
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("PHAT HIEN LUA!!!");
    digitalWrite(speaker, HIGH);
    delay(2000);
  }
  else
  {
    digitalWrite(speaker, LOW);
  }
}

void gas_check(int x)
{
  float mqValue = analogRead(x);
  Serial.println(mqValue);
  if (mqValue >= 3000)
  {
    digitalWrite(fan, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("CANH BAO KHI GAS");
    delay(2000);
  }
  else
  {
    digitalWrite(fan, LOW);
  }
}

void charger()
{
  float voltage = ReadVoltage(v_ref) * 2;
  if (voltage > 3.5)
    digitalWrite(control, HIGH);
  else
    digitalWrite(control, LOW);
}

const int numSamples = 10;

int readFilteredValue()
{
  long sum = 0;
  for (int i = 0; i < numSamples; i++)
  {
    sum += analogRead(ldr);
    delay(5);
  }
  return sum / numSamples;
}
void light_check()
{
  int sensorValue = readFilteredValue();
  int check = 0;
  if (sensorValue >= 4000)
  {
    digitalWrite(light, HIGH);
    check = 1;
  }
  else
  {
    digitalWrite(light, LOW);
    check = 0;
  }
}
