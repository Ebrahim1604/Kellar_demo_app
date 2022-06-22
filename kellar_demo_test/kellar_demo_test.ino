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

uint32_t Token;

uint16_t addr_list[] = {0x0002,0x0004,0x0006,0x0008,0x000A}; //p1,p2,T,tob1,tob2

int addr_global;
int val_;

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

 Token = 1111;
 
 delay(8000);

Serial.println("--------------------------Parameters for Sensor:---------------------");
      
      Error err0 = MB.addRequest(Token, 1, READ_HOLD_REGISTER, 0x0002, 2);
          if (err0!=SUCCESS) 
          {
            ModbusError e(err0);
            Serial.print("Error detected at Value = !");
            Serial.println(Token);
            Serial.println("");
            LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
          }
       Token++;

  Serial.println("-------------------------------------------------------------------------");
      
  Error err1 = MB.addRequest(Token, 1, READ_HOLD_REGISTER, 0x0004, 2);
          if (err1!=SUCCESS) 
          {
            ModbusError e(err1);
            Serial.print("Error detected at Value = !");
            Serial.println(Token);
            Serial.println("");
            LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
          }
        Token++;

   Serial.println("-------------------------------------------------------------------------");
  Error err2 = MB.addRequest(Token, 1, READ_HOLD_REGISTER, 0x0006, 2);
          if (err2!=SUCCESS) 
          {
            ModbusError e(err2);
            Serial.print("Error detected at Value = !");
            Serial.println(Token);
            Serial.println("");
            LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
          }
        Token++;

  Serial.println("-------------------------------------------------------------------------");
  Error err3 = MB.addRequest(Token, 1, READ_HOLD_REGISTER, 0x0008, 2);
          if (err3!=SUCCESS) 
          {
            ModbusError e(err3);
            Serial.print("Error detected at Value = !");
            Serial.println(Token);
            Serial.println("");
            LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
          }
        Token++;

  Serial.println("-------------------------------------------------------------------------");
  Error err4 = MB.addRequest(Token, 1, READ_HOLD_REGISTER, 0x000A, 2);
          if (err4!=SUCCESS) 
          {
            ModbusError e(err4);
            Serial.print("Error detected at Value = !");
            Serial.println(Token);
            Serial.println("");
            LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
          }

        Token = 1111;
 
}

// loop() - nothing done here today!
void loop() 
{
  /*
  for(int id = 1; id<=no_of_sensors; id++)
  {   
      Serial.printf("--------------------------Parameters for Sensor %d:---------------------\n", id);
      Serial.println("");
      
      Error err0 = MB.addRequest(Token, id, READ_HOLD_REGISTER, 0x0002, 2);
          if (err0!=SUCCESS) 
          {
            ModbusError e(err0);
            Serial.print("Error detected at Value = !");
            Serial.println(Token);
            Serial.println("");
            LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
          }
       Token++;
  
  Error err1 = MB.addRequest(Token, id, READ_HOLD_REGISTER, 0x0004, 2);
          if (err1!=SUCCESS) 
          {
            ModbusError e(err1);
            Serial.print("Error detected at Value = !");
            Serial.println(Token);
            Serial.println("");
            LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
          }
        Token++;
   
  Error err2 = MB.addRequest(Token, id, READ_HOLD_REGISTER, 0x0006, 2);
          if (err2!=SUCCESS) 
          {
            ModbusError e(err2);
            Serial.print("Error detected at Value = !");
            Serial.println(Token);
            Serial.println("");
            LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
          }
        Token++;

  Error err3 = MB.addRequest(Token, id, READ_HOLD_REGISTER, 0x0008, 2);
          if (err3!=SUCCESS) 
          {
            ModbusError e(err3);
            Serial.print("Error detected at Value = !");
            Serial.println(Token);
            Serial.println("");
            LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
          }
        Token++;

  Error err4 = MB.addRequest(Token, id, READ_HOLD_REGISTER, 0x000A, 2);
          if (err4!=SUCCESS) 
          {
            ModbusError e(err4);
            Serial.print("Error detected at Value = !");
            Serial.println(Token);
            Serial.println("");
            LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
          }

        Token = 1111;
  }

 delay(1000);
*/
}

void handleData(ModbusMessage response, uint32_t token) 
{
  const uint8_t *val = response.data();
  int val_size = response.size();
   
  if(token == 1111)
    {
      Serial.print("P1 = ");
      double val_ = cal_double(val[3],val[4],val[5],val[6]);
      Serial.print(val_);
      Serial.println(" bar"); 
      Serial.println("");
      }
    else if(token == 1112)
    {
      Serial.print("P2 = ");
      double val_ = cal_double(val[3],val[4],val[5],val[6]);
      Serial.print(val_);
      Serial.println(" bar");
      Serial.println(""); 
      }
    else if(token == 1113)
    {
      Serial.print("T = ");
      double val_ = cal_double(val[3],val[4],val[5],val[6]);
      Serial.print(val_);
      Serial.println(" degree C");
      Serial.println(""); 
      }
    else if(token == 1114)
    {
      Serial.print("Tob1 = ");
      double val_ = cal_double(val[3],val[4],val[5],val[6]);
      Serial.print(val_);
      Serial.println(" degree C");
      Serial.println("");
      }
    else if(token == 1115)
    {
      Serial.print("Tob2 = ");
      double val_ = cal_double(val[3],val[4],val[5],val[6]);
      Serial.print(val_);
      Serial.println(" degree C");
      Serial.println("");
      }
}


void handleError(Error error, uint32_t token) 
{
  // ModbusError wraps the error code and provides a readable error message for it
  ModbusError me(error);
  LOG_E("Error response: %02X - %s\n", (int)me, (const char *)me);
}

double cal_double(uint8_t a,uint8_t b,uint8_t c,uint8_t d)
{
  if (a == 0xFF && b == 0xFF && c == 0xFF && d == 0xFF)
  {
    return 0.0;
    }
  
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
