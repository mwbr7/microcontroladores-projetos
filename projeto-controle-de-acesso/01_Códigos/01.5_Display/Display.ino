#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); 

void setup() {
  Wire.begin(21, 22);            
  lcd.begin(20, 4);              
  lcd.backlight();

  lcd.setCursor(1, 0);
  lcd.print("Mathieu!");

  lcd.setCursor(4, 1);
  lcd.print("O Mior");
}

void loop() {
}