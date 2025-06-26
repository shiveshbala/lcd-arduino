/*****************************************************************
  HC-SR04 distance  ➜  I²C LCD
  ---------------------------------------------------------------
  • Ultrasonic sensor
        VCC  → 5 V
        GND  → GND
        TRIG → D9
        ECHO → D10
  • LCD backpack (PCF8574)
        SDA  → A4
        SCL  → A5
        VCC  → 5 V   GND → GND
  • Libraries (install via IDE ▶ Library Manager)
        – LiquidCrystal_I2C
 *****************************************************************/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/* ---------- user settings ---------- */
constexpr uint8_t LCD_ADDR = 0x27;     // change to 0x3F if your scanner says so
constexpr uint8_t LCD_COLS = 16;       // 20 if you have a 20 × 4
constexpr uint8_t LCD_ROWS = 2;

constexpr int trigPin = 9;
constexpr int echoPin = 10;
/* ----------------------------------- */

LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.print("Ultrasonic ready");
  delay(1500);
  lcd.clear();
}

void loop() {
  /* ---- fire 10 µs trigger ---- */
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  /* ---- read echo pulse ---- */
  unsigned long duration = pulseIn(echoPin, HIGH, 30000UL); // 30 ms timeout

  /* ---- convert to cm ---- */
  float distance = duration ? (duration / 58.0) : NAN;      // 29.1 µs /cm round-trip

  /* ---- show on LCD ---- */
  lcd.setCursor(0, 0);
  lcd.print("Distance:       ");   // pad to erase leftovers
  lcd.setCursor(10, 0);
  if (isnan(distance)) {
    lcd.print("--.-");
  } else {
    lcd.print(distance, 1);        // one decimal place
  }
  lcd.print("cm");

  delay(500);                      // update twice per second
}
