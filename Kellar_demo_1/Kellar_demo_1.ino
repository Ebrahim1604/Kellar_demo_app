/*
 Pin connections:

      LOLIN d32 pro            Uart to rs485 convertor
    1)   GPIO 13--------------------------TX
    2)   GPIO 12--------------------------RX
    3)   GPIO 15--------------------------RTS
    4)    3.3V----------------------------VCC
    5)    GND-----------------------------GND    
 */

// Include the header for the ModbusClient RTU style
#include "ModbusClientRTU.h"

//#define LOG_LEVEL LOG_LEVEL_VERBOSE
// For demonstration, use the LOG statements for output
#include "Logging.h"



const byte rxd = 12;
const byte txd = 13;
const byte rts = 15;

#define demo_addr1 0x0200 //baud rate confg

#define demo_addr2 0x0202 // S/n h bytes

#define demo_addr3 0x0203 // S/n l bytes

// Create a ModbusRTU client instance
ModbusClientRTU MB(Serial2, rts);

// Define an onData handler function to receive the regular responses
// Arguments are Modbus server ID, the function code requested, the message data and length of it, 
// plus a user-supplied token to identify the causing request
void handleData(ModbusMessage response, uint32_t token) 
{
  Serial.print("handledata() function running on core: ");
  Serial.println(xPortGetCoreID());
  Serial.println("");
  
  // Only print out result of the "real" example - not the request preparing the field
  if (token > 1111) {
    LOG_N("Response: serverID=%d, FC=%d, Token=%08X, length=%d:\n", response.getServerID(), response.getFunctionCode(), token, response.size());
    HEXDUMP_N("Data", response.data(), response.size());
  }
}

// Define an onError handler function to receive error responses
// Arguments are the error code returned and a user-supplied token to identify the causing request
void handleError(Error error, uint32_t token) 
{
  Serial.print("handle error() function running on core: ");
  Serial.println(xPortGetCoreID());
  Serial.println("");
  // ModbusError wraps the error code and provides a readable error message for it
  ModbusError me(error);
  LOG_E("Error response: %02X - %s\n", (int)me, (const char *)me);
}

// Setup() - initialization happens here
void setup() {
// Init Serial monitor
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("__ Kellar33x Modbus demo code __");
  Serial.println("");
  Serial.println("");
  Serial.print("Setup() function running on core: ");
  Serial.println(xPortGetCoreID());
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

// Create request by
// - token to match the response with the request.
// - server ID = 1
// - function code = 0x03 (read holding register)
// - address to read = word 10
// - data words to read = 1
//
// If something is missing or wrong with the call parameters, we will immediately get an error code 
// and the request will not be issued
Serial.println("_______________________________________________________________________");
Serial.println(" 1) Reading the reg addr 0x0200, confg register");
Serial.println("");
// Read register demo_addr1
 Error err = MB.addRequest(Token++, 1, READ_HOLD_REGISTER, demo_addr1, 1);
  if (err!=SUCCESS) {
    ModbusError e(err);
    LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
  }

Serial.println("_______________________________________________________________________");
Serial.println(" 2) Reading the reg addr 0x0202, H byte of serial number: 728862, H bytes: 00 0B");
Serial.println("");
  // Read register demo_addr2
  err = MB.addRequest(Token++, 1, READ_HOLD_REGISTER, demo_addr2, 1);
  if (err!=SUCCESS) {
    ModbusError e(err);
    LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
  }

Serial.println("_______________________________________________________________________");
Serial.println(" 3) Reading the reg addr 0x0203, L byte of serial number: 728862, L bytes: 1F 1E");
Serial.println("");
  // Read register demo_addr3
  err = MB.addRequest(Token++, 1, READ_HOLD_REGISTER, demo_addr3, 1);
  if (err!=SUCCESS) {
    ModbusError e(err);
    LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
  }
  
/*
// Now write 0xBEEF to it
  err = MB.addRequest(Token++, 1, WRITE_HOLD_REGISTER, 10, 0xBEEF);
  if (err!=SUCCESS) {
    ModbusError e(err);
    LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
  }

// Read it again to verify
  err = MB.addRequest(Token++, 1, READ_HOLD_REGISTER, 10, 1);
  if (err!=SUCCESS) {
    ModbusError e(err);
    LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
  }

// Provoke an error just to show it
  err = MB.addRequest(Token++, 1, USER_DEFINED_44, 10, 1);
  if (err!=SUCCESS) {
    ModbusError e(err);
    LOG_E("Error creating request: %02X - %s\n", (int)e, (const char *)e);
  }
*/
}
// loop() - nothing done here today!
void loop() 
{
}
