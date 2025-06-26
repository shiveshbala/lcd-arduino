#include <SoftwareSerial.h>
SoftwareSerial Dta(4,3);
void setup(){
  Serial.begin(9600);
  Dta.begin(9600);
}
void loop() {
  while(Dta.available()>0)
  {
    byte gpsData=Dta.read();// raw data from gps module
    Serial.write(gpsData);
  }
}
