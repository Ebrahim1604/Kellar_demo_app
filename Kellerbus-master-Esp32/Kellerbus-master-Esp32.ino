/**

  @author thewknd
  @date 7.2012

  @copyright
  This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
*/

#include "kellerbus.h"
#include "time/TimeLib.h"
#include "SoftwareSerial.h"
/*

  RX Pin: 12
  TX Pin: 13

*/
// digital pins 12 and 13 are used as virtual RX and TX serial lines.
#define rxPin 12
#define txPin 13

SoftwareSerial mySerial;

CKellerBus kbus((Stream*)&mySerial, 9600, 2, 100);

time_t kbusTime;

void setup()
{
  //define pinmodes for rx & tx
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  Serial.begin(115200);

  mySerial.begin(9600, SWSERIAL_8N1, rxPin, txPin, false);
  if (!mySerial)
  {
    Serial.println("SoftSerial configuration not done correctly");
    while (1)
    {
      delay(1000);
    }
  }
}

void loop()
{
  uint8_t mclass, group, myear, week, cfg_p, cfg_t, cnt_t, buffer, state;
  int serial;
  double temp = 0, pressure = 0;

  Serial.println("\n-- KELLERBUS --");

  kbus.initDevice(250, &mclass, &group, &myear, &week, &buffer, &state);


  Serial.print("SN : ");
  serial = kbus.getSerialnumber();
  Serial.println(serial);
  Serial.print("DEVICE ADDRESS : ");
  kbus.writeDeviceAddress(0);
  Serial.println(kbus.getDeviceAddress(), DEC);


  Serial.println("\n\r- Measurement -");

  Serial.println(kbus.getCH0(), DEC);
  Serial.print("P1 : ");
  pressure = kbus.getP1(P_BAR);
  Serial.println(pressure);
  Serial.print("TOB1 : ");
  temp = kbus.getTOB1(T_DEGC);
  Serial.println(temp);



  delay(1000);
  //while(1) ;
}
