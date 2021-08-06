/*
 Pin connections:

      LOLIN d32 pro            Uart to rs485 convertor
    1)   GPIO 13--------------------------TX
    2)   GPIO 12--------------------------RX
    3)   GPIO 15--------------------------RTS
    4)    3.3V----------------------------VCC
    5)    GND-----------------------------GND    
 */

#include <stdint.h>

// Include the header for the ModbusClient RTU style
#include "ModbusClientRTU.h"

//#define LOG_LEVEL LOG_LEVEL_VERBOSE
// For demonstration, use the LOG statements for output
#include "Logging.h"

const byte rxd = 12;
const byte txd = 13;
const byte rts = 15;

#define no_of_sensors 1

uint16_t addr_list[] = {0x0002,0x0004,0x0006,0x0008,0x000A}; //p1,p2,T,tob1,tob2

int addr_global;

// Create a ModbusRTU client instance
ModbusClientRTU MB(Serial2, rts);

// Setup() - initialization happens here
void setup() {
// Init Serial monitor
  Serial.begin(115200);
  while (!Serial) {}
  
  Serial.println("__ Kellar33x Modbus demo code __");
  Serial.println("");
  Serial.println("");

// Set up Serial2 connected to Modbus RTU
// (Fill in your data here!)
  Serial2.begin(9600, SERIAL_8N1, rxd, txd);

// Set up ModbusRTU client.
// - provide onData handler function
  MB.onDataHandler(&handleData);
// - provide onError handler function
  MB.onErrorHandler(&handleError);
// Set message timeout to 2000ms
  MB.setTimeout(2000);
// Start ModbusRTU background task
  MB.begin();

uint32_t Token = 1111;

//int id = 1;

addr_global = 0;
Error err0 = MB.addRequest(Token++, 1, READ_HOLD_REGISTER, 0x0002, 2);
          if (err0!=SUCCESS) 
          {
            ModbusError e(err0);
            Serial.print("Error detected at Value = !");
            Serial.println(addr_global);
            Serial.println("");
            LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
          }


addr_global = 1;
Error err1 = MB.addRequest(Token++, 1, READ_HOLD_REGISTER, 0x0004, 2);
          if (err1!=SUCCESS) 
          {
            ModbusError e(err1);
            Serial.print("Error detected at Value = !");
            Serial.println(addr_global);
            Serial.println("");
            LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
          }

addr_global = 2;
Error err2 = MB.addRequest(Token++, 1, READ_HOLD_REGISTER, 0x0006, 2);
          if (err2!=SUCCESS) 
          {
            ModbusError e(err2);
            Serial.print("Error detected at Value = !");
            Serial.println(addr_global);
            Serial.println("");
            LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
          }

addr_global = 3;
Error err3 = MB.addRequest(Token++, 1, READ_HOLD_REGISTER, 0x0008, 2);
          if (err3!=SUCCESS) 
          {
            ModbusError e(err3);
            Serial.print("Error detected at Value = !");
            Serial.println(addr_global);
            Serial.println("");
            LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
          }

addr_global = 4;
Error err4 = MB.addRequest(Token++, 1, READ_HOLD_REGISTER, 0x000A, 2);
          if (err4!=SUCCESS) 
          {
            ModbusError e(err4);
            Serial.print("Error detected at Value = !");
            Serial.println(addr_global);
            Serial.println("");
            LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
          }

addr_global = 5;
Error err5 = MB.addRequest(Token++, 1, READ_HOLD_REGISTER, 0x0002, 10);
          if (err5!=SUCCESS) 
          {
            ModbusError e(err5);
            Serial.print("Error detected at Value = !");
            Serial.println(addr_global);
            Serial.println("");
            LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
          }

addr_global = 6;
Error err6 = MB.addRequest(Token++, 1, READ_HOLD_REGISTER, 0x0200, 1);
          if (err6!=SUCCESS) 
          {
            ModbusError e(err6);
            Serial.print("Error detected at Value = !");
            Serial.println(addr_global);
            Serial.println("");
            LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
          }

addr_global = 7;
Error err7 = MB.addRequest(Token++, 1, READ_HOLD_REGISTER, 0x0202, 1);
          if (err7!=SUCCESS) 
          {
            ModbusError e(err7);
            Serial.print("Error detected at Value = !");
            Serial.println(addr_global);
            Serial.println("");
            LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
          }

addr_global = 8;
Error err8 = MB.addRequest(Token++, 1, READ_HOLD_REGISTER, 0x0203, 1);
          if (err8!=SUCCESS) 
          {
            ModbusError e(err8);
            Serial.print("Error detected at Value = !");
            Serial.println(addr_global);
            Serial.println("");
            LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
          }

}

// loop() - nothing done here today!
void loop() 
{
  /*
  for(int id = 1; id<=no_of_sensors; id++)
  {   
      Serial.printf("Parameters for Sensor %d:\n", id);
      
      for(int addr = 0; addr<5; addr++)
      {
        addr_global = addr;
        Error err = MB.addRequest(Token++, id, READ_HOLD_REGISTER, addr_list[addr], 2);
          if (err!=SUCCESS) 
          {
          ModbusError e(err);
          Serial.print("Error detected!");
          LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
          }
       }

       Serial.println("");
       Serial.println("");
    }
    */
}

void handleData(ModbusMessage response, uint32_t token) 
{
  Serial.println("_____________________________________________________________________________________________________________");
  // Only print out result of the "real" example - not the request preparing the field
  if (token > 1111) {

    const uint8_t *val = response.data();
    int val_size = response.size();
    //static uint8_t val[val_size];
    
    Serial.print("Data received for ");
    Serial.print(addr_global);
    Serial.print("= ");
    for(int k=0; k<val_size; k++)
    {
      
      Serial.print(val[k], HEX);
      Serial.print(" ");
    }
    Serial.println("");

    /*
    if(addr_global == 0)
    {
      Serial.print("P1 = ");
      double val_ = cal_double(val[3],val[4],val[5],val[6]);
      Serial.print(val_);
      Serial.println("bar"); 
      Serial.println("");
      }
    else if(addr_global == 1)
    {
      Serial.print("P2 = ");
      double val_ = cal_double(val[3],val[4],val[5],val[6]);
      Serial.print(val_);
      Serial.println("bar");
      Serial.println(""); 
      }
    else if(addr_global == 2)
    {
      Serial.print("T = ");
      double val_ = cal_double(val[3],val[4],val[5],val[6]);
      Serial.print(val_);
      Serial.println("degree C");
      Serial.println(""); 
      }
    else if(addr_global == 3)
    {
      Serial.print("Tob1 = ");
      double val_ = cal_double(val[3],val[4],val[5],val[6]);
      Serial.print(val_);
      Serial.println("degree C");
      Serial.println("");
      }
    else if(addr_global == 4)
    {
      Serial.print("Tob2 = ");
      double val_ = cal_double(val[3],val[4],val[5],val[6]);
      Serial.print(val_);
      Serial.println("degree C");
      Serial.println("");
      }
    else if(addr_global == 5)
    { 
      Serial.println("^^^^All in one values^^^^");
      Serial.println("");
      }
     else
     {
      Serial.println("Error: addr_global value out of bound");
      }
  */
  }

  else {Serial.println("Ran out of token value");}

  Serial.println("_____________________________________________________________________________________________________________");
}

void handleError(Error error, uint32_t token) 
{
  // ModbusError wraps the error code and provides a readable error message for it
  ModbusError me(error);
  LOG_E("Error response: %02X - %s\n", (int)me, (const char *)me);
}

double cal_double(uint8_t a,uint8_t b,uint8_t c,uint8_t d)
{
  union
    {
        float doubleVal;
        uint8_t bytes[4];
    }doubleConverter;
    
    doubleConverter.bytes[0]= d; 
    doubleConverter.bytes[1]= c;
    doubleConverter.bytes[2]= b;
    doubleConverter.bytes[3]= a;
    
  
    double fo = (double)doubleConverter.doubleVal;
  
    return fo;
  }
