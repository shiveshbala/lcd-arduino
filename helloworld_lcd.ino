#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_ADDR 0x27      
#define LCD_COLS 16
#define LCD_ROWS 2

LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

void setup() {
  lcd.init();              // or lcd.begin(LCD_COLS, LCD_ROWS); ← try this if blank
  lcd.backlight();         // backpack must support back-light control

  lcd.setCursor(0,0);
  lcd.print("Hello, World!");
  lcd.setCursor(0,1);
  lcd.print("Arduino ready");
}

void loop() {}
