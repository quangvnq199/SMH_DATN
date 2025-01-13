//using es32
//SCL to GPIO 22
//SDA to GPIO 21

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Địa chỉ I2C của màn hình LCD, thường là 0x27 hoặc 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Khởi tạo kết nối LCD
  lcd.init();
  lcd.backlight();  // Bật đèn nền LCD

  // Hiển thị văn bản
  lcd.setCursor(0, 0);
  lcd.print("Hello, ESP32!");
  
  lcd.setCursor(0, 1);
  lcd.print("LCD 16x2 Test");
}

void loop() {
  // Không cần làm gì thêm trong vòng lặp
}