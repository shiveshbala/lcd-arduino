/*****************************************************************
   GPS → I2C LCD  (TinyGPS++)
   --------------------------------------------------------------
   • LCD backpack: PCF8574 at 0x27 (SDA-A4, SCL-A5)
   • GPS module : NEO-6M/7M/8M 9600 baud
        GPS  VCC → 5 V
        GPS  GND → GND
        GPS   TX → D4   (Arduino RX — we read from GPS here)
        GPS   RX → D3   (optional, only for sending cfg cmds)
 *****************************************************************/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

/* ---------- user settings ------------------------------- */
constexpr uint8_t LCD_ADDR   = 0x27;   // set to 0x3F if your scanner said so
constexpr uint8_t LCD_COLS   = 16;     // 20 if you have a 20×4
constexpr uint8_t LCD_ROWS   = 2;      // 4  if you have a 20×4

constexpr uint8_t GPS_RX_PIN = 4;      // Arduino pin receiving GPS TX
constexpr uint8_t GPS_TX_PIN = 3;      // Arduino pin sending   GPS RX (opt)
/* --------------------------------------------------------- */

// I²C LCD object
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

// GPS objects
SoftwareSerial gpsSerial(GPS_RX_PIN, GPS_TX_PIN);  // RX, TX
TinyGPSPlus gps;

unsigned long lastScreen = 0;   // for 1 s display refresh

void setup()
{
  lcd.init();
  lcd.backlight();
  lcd.print(F("GPS-LCD demo"));
  delay(1500);
  lcd.clear();

  gpsSerial.begin(9600);        // default baud for most NEO boards

  Serial.begin(115200);         // keep USB serial free for debug
  Serial.println(F("GPS → LCD sketch started"));
}

void loop()
{
  /* Feed bytes from GPS into TinyGPS++ -------------------- */
  while (gpsSerial.available())
  {
    char c = gpsSerial.read();
    gps.encode(c);          // PARSE here – don't display raw!
    // Optional: view raw NMEA in Serial Monitor
    // Serial.write(c);
  }

  /* Update LCD once a second ----------------------------- */
  if (millis() - lastScreen >= 1000)
  {
    lastScreen = millis();
    lcd.clear();

    if (gps.location.isValid() && gps.location.isUpdated())
    {
      // Row 0 – latitude
      lcd.setCursor(0, 0);
      lcd.print(F("Lat:"));
      lcd.print(gps.location.lat(), 5);   // ±dd.ddddd

      // Row 1 – longitude
      lcd.setCursor(0, 1);
      lcd.print(F("Lon:"));
      lcd.print(gps.location.lng(), 5);   // ±ddd.ddddd

      // Debug to USB serial
      Serial.print(F("Lat: "));
      Serial.print(gps.location.lat(), 6);
      Serial.print(F("  Lon: "));
      Serial.println(gps.location.lng(), 6);
    }
    else
    {
      lcd.setCursor(0, 0);
      lcd.print(F("Waiting for"));
      lcd.setCursor(0, 1);
      lcd.print(F("GPS fix..."));
      Serial.println(F("No fix yet"));
    }
  }
}
