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

for(int id = 1; id<=no_of_sensors; id++)
  {   
      Serial.printf("Parameters for Sensor %d:\n", id);
      
      for(int addr = 0; addr<5; addr++)
      {
        Error err = MB.addRequest(Token++, id, READ_HOLD_REGISTER, addr_list[addr], 2);
          if (err!=SUCCESS) 
          {
          ModbusError e(err);
          LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
          }
       }
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
        Error err = MB.addRequest(Token++, id, READ_HOLD_REGISTER, addr_list[addr], 2);
          if (err!=SUCCESS) 
          {
          ModbusError e(err);
          LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
          }
       }
    }
    */
}

void handleData(ModbusMessage response, uint32_t token) 
{
  // Only print out result of the "real" example - not the request preparing the field
  if (token > 1111) {

    const uint8_t *val = response.data();
    int val_size = response.size();
    //static uint8_t val[val_size];
    
    Serial.print("Data received = ");
    for(int k=0; k<val_size; k++)
    {
      
      Serial.print(val[k], HEX);
      Serial.print(" ");
    }
    Serial.println("");
  }
}

void handleError(Error error, uint32_t token) 
{
  // ModbusError wraps the error code and provides a readable error message for it
  ModbusError me(error);
  LOG_E("Error response: %02X - %s\n", (int)me, (const char *)me);
}
