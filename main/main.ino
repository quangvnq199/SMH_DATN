#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6jvUtX6Aj"
#define BLYNK_TEMPLATE_NAME "SMART HOME BY QUANG NGUYEN"
#define BLYNK_AUTH_TOKEN "7xiLTONcgThzI2q6FT26UWHT8MHbC1-F"

#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <BlynkSimpleEsp32.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

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
#define BOTtoken "7912718575:AAHOWNU2czEGbqvV5kKRBU6Nhi9v9JDs7jQ"
#define CHAT_ID "7765402438"

char ssid[] = "Canh Ca Thai Binh";
char pass[] = "giang1992";
const String sheet_url = "https://script.google.com/macros/s/AKfycbzu8PP50TFVlS0kiUYdVpkHClNLrM7byl7WeEvdA3eQtOdBLNCZOf7cm8bkfR_EwP4dXA/exec?name=";
int blockNum = 2;
byte bufferLen = 18;
byte readBlockData[18];
String card_holder_name;
DHT dht(cb_dht, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
Servo myservo;
// BlynkTimer timer;

byte authorizedUID1[] = {0x65, 0xdd, 0x0f, 0xad};
byte authorizedUID2[] = {0xf6, 0x8c, 0xf6, 0x03}; // khoa so 1
byte authorizedUID3[] = {0xc1, 0x2d, 0xa3, 0x7b}; // khoa so 2

int botRequestDelay = 1000;

void setup()
{
  Serial.begin(9600);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

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
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  // Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop()
{

  charger();
  gas_check(mq_sensor);
  fire_check(fire_sensor);
  // light_check();
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // Blynk.virtualWrite(V3, h);
  // Blynk.virtualWrite(V4, t);

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

  rfid.PCD_Init();

  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
  {
    return;
  }
  ReadDataFromBlock(blockNum, readBlockData);
  for (byte i = 0; i < rfid.uid.size; i++)
  {
    content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(rfid.uid.uidByte[i], HEX));
  }

  bool authorized = false;
  for (byte i = 0; i < rfid.uid.size; i++)
  {
    if (rfid.uid.uidByte[i] == authorizedUID3[i] || rfid.uid.uidByte[i] == authorizedUID2[i])
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

  for (int j = 0; j < 16; j++)
  {
    Serial.write(readBlockData[j]);
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    WiFiClientSecure client;
    client.setInsecure();
    card_holder_name = sheet_url + String((char *)readBlockData);
    card_holder_name.trim();
    Serial.println(card_holder_name);
    HTTPClient https;
    Serial.print(F("[HTTPS] begin...\n"));

    if (https.begin(client, (String)card_holder_name))
    {
      Serial.print(F("[HTTPS] GET...\n"));
      // start connection and send HTTP header
      int httpCode = https.GET();
      // httpCode will be negative on error
      if (httpCode > 0)
      {
        // HTTP header has been sent and Server response header has been handled
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
        // file found at server
      }
      else
      {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      https.end();
      delay(1000);
    }
    else
    {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }

  rfid.PICC_HaltA();

  delay(1000);
  // Blynk.run();
  // timer.run();
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
  int check2 = 0;
  if (fire <= 150)
  {
    check2 = 1;
    bot.sendMessage(CHAT_ID, "DANG CO CHAY!", "");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("PHAT HIEN LUA!!!");
    digitalWrite(speaker, HIGH);
    delay(2000);
  }
  else
  {
    check2 = 0;
    digitalWrite(speaker, LOW);
  }
  Blynk.virtualWrite(V2, check2);
}

void gas_check(int x)
{
  float mqValue = analogRead(x);
  Serial.println(mqValue);
  if (mqValue >= 3000)
  {
    bot.sendMessage(CHAT_ID, "KHONG KHI CO KHA NANG XAY RA CHAY!", "");
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
  int check3 = 0;
  float voltage = ReadVoltage(v_ref) * 2;
  if (voltage > 3.5)
  {
    digitalWrite(control, HIGH);
    check3 = 1;
  }

  else
  {
    digitalWrite(control, LOW);
    check3 = 0;
  }
  Blynk.virtualWrite(V5, check3);
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
  // Blynk.virtualWrite(V1, check);
}

void ReadDataFromBlock(int blockNum, byte readBlockData[])
{
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
  status = rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(rfid.uid));
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Authentication failed for Read: ");
    Serial.println(rfid.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("Authentication success");
  }
  status = rfid.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Reading failed: ");
    Serial.println(rfid.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("Block was read successfully");
  }
}