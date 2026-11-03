#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init();          //Menyalakan LCD
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("Hello World!");
  lcd.setCursor(1, 1);
  lcd.print("Selamat Datang");

}

void loop() {
  
}
