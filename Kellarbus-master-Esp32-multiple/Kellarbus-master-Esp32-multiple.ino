/**

  @author thewknd
  @date 7.2012

  @copyright
  This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
*/

#include "kellerbus.h"
#include "time/TimeLib.h"
#include "SoftwareSerial.h"

#define NOD 8 //number of devices connected to the kellerbus

double sensor_values[NOD][2];
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

uint8_t mclass, group, myear, week, cfg_p, cfg_t, cnt_t, buffer, state;
int serial;
double temp, pressure;
    
unsigned long time1,time2;

double get_sensor_values(int Device_no,int param);

void setup()
{
  //define pinmodes for rx & tx
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  Serial.begin(115200);

  for (int i=0; i<NOD; i++)
  {
     for(int j=0; j<2; j++)
     {
        sensor_values[i][j] = 0.0;
      }
    }

  //mySerial.begin(9600, SWSERIAL_8N1, rxPin, txPin, false);
  /*
  if (!mySerial)
  {
    Serial.println("SoftSerial configuration not done correctly");
    while (1)
    {
      delay(1000);
    }
  }
  */
}

void loop()
{
  time1 = 0;
  time2 = 0;
  
  time1 = millis();
  
  for(int i=1; i<=NOD; i++)
  {
    temp = 0;
    pressure = 0;
    
    Serial.println("\n-----------------");

    mySerial.begin(9600, SWSERIAL_8N1, rxPin, txPin, false);
    kbus.initDevice(i, &mclass, &group, &myear, &week, &buffer, &state);
   
    Serial.print("DEVICE ADDRESS : ");
    Serial.println(kbus.getDeviceAddress(), DEC);
    Serial.print("SN : ");
    serial = kbus.getSerialnumber();
    Serial.println(serial);
    
    Serial.println("\n\r- Measurement -");
    
    Serial.println(kbus.getCH0(), DEC);
    Serial.print("P1 : ");
    pressure = kbus.getP1(P_BAR);
    Serial.println(pressure);
    sensor_values[i-1][0] = pressure;
    
    Serial.print("TOB1 : ");
    temp = kbus.getTOB1(T_DEGC);
    Serial.println(temp);
    sensor_values[i-1][1] = temp;
    
    mySerial.flush();
    mySerial.end();

    delay(10);
  }
  
  time2 = millis();

  Serial.println("\n-------------------------------------------------------");

  double total_time = (((time2-time1)-(NOD*10))/1000.0); 
  Serial.printf("Time taken for reading %d sensors = ",NOD);
  Serial.print(total_time);
  Serial.println(" secs");
}

double get_sensor_values(int Device_no,int param)// param : 1---> Pressure, 2--->Temp
{
   if (Device_no > NOD)
   {
    Serial.printf("Error: Device no %d not connected, total devices connected = %d ",Device_no,NOD);
    return 0.0;
    }

    return sensor_values[Device_no - 1][param - 1];
  }
