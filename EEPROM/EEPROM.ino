#include <EEPROM.h>

int yourInteger=21345;

void setup() {
// write to EEPROM
Serial.begin(9600);
EEPROM.write(0, highByte(yourInteger));
EEPROM.write(1, lowByte(yourInteger));

// read EEPROM
int bytehigh=EEPROM.read(0);
int bytelow=EEPROM.read(1);

int myInteger=word(bytehigh, bytelow);

Serial.println(myInteger);
Serial.println("test");
}

void loop() {
  // put your main code here, to run repeatedly:

}
