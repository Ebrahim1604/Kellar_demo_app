#include "Modbusrtu.h"

#define MasterModbusAdd  0
#define SlaveModbusAdd  1
#define RS485Serial 2
Modbus ControllinoModbusMaster(MasterModbusAdd, RS485Serial, 3);
modbus_t ModbusQuery[2];

unsigned long WaitingTime;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  Serial.println("Debugging connection..................... ");
  
  int u = get_reg(0x0200);
  Serial.print("Test: Holding reg 0x0200 current value = ");
  Serial.println(u,BIN);
  last_error();

  Serial.println("");
  Serial.println("_____________Sensor values are displayed below:___________");
  
  float P1 = get_sensor_val(0x0002); //P1
  Serial.print("P1 = ");
  Serial.println(P1);
  last_error();
}

void loop() {
  // put your main code here, to run repeatedly:
  }

float floatconvert(uint16_t a,uint16_t b)
{
  union
    {
        double floatVal;
        uint16_t bytes[2];
    }floatConverter;
    
    floatConverter.bytes[0]= b; 
    floatConverter.bytes[1]= a;
    
    double fo = floatConverter.floatVal;
    return fo;
}

void last_error()
{
  Serial.println("Getting Last error value.....");
  int e = ControllinoModbusMaster.getLastError();

  if (e==255){Serial.println("Error: Time-out, no response from slave");}
  else if (e==1){Serial.println("Error: Function code not available");}
  else if (e==2){Serial.println("Error: Address beyond available space for Modbus registers ");}
  else if (e==3){Serial.println("Error: Coils or registers number beyond the available space");}
  else {Serial.println("No error in communication");}
  
  delay(4000);
  }
   
int get_reg(uint16_t addr)
{
  uint16_t curr_reg_val[1];
  
  ModbusQuery[0].u8id = SlaveModbusAdd;
  ModbusQuery[0].u8fct = 3;
  ModbusQuery[0].u16RegAdd = addr;
  ModbusQuery[0].u16CoilsNo = 1;
  ModbusQuery[0].au16reg = curr_reg_val;

  ControllinoModbusMaster.begin( 9600 );
  ControllinoModbusMaster.setTimeOut( 5000 );

  WaitingTime = millis() + 1000;

  while (millis() > WaitingTime);

  ControllinoModbusMaster.query( ModbusQuery[0] );

again1:
  ControllinoModbusMaster.poll();
  if (ControllinoModbusMaster.getState() == COM_IDLE)
  {
    Serial2.end();
    
    return (int)curr_reg_val[0];
  }
  else
  {
   goto again1;
  }
}

float get_sensor_val(int addr)
{
  uint16_t curr_reg_val[2];
  
  ModbusQuery[0].u8id = SlaveModbusAdd;
  ModbusQuery[0].u8fct = 3;
  ModbusQuery[0].u16RegAdd = addr;
  ModbusQuery[0].u16CoilsNo = 2;
  ModbusQuery[0].au16reg = curr_reg_val;

  ControllinoModbusMaster.begin( 9600 );
  ControllinoModbusMaster.setTimeOut( 5000 );

  WaitingTime = millis() + 1000;

  while (millis() > WaitingTime);

  ControllinoModbusMaster.query( ModbusQuery[0] );

again1:
  ControllinoModbusMaster.poll();
  if (ControllinoModbusMaster.getState() == COM_IDLE)
  {
    Serial2.end();
    float P =  floatconvert(curr_reg_val[0],curr_reg_val[1]);
    return P;
  }
  else
  {
   goto again1;
  }
 }

void set_reg(uint16_t addr, int reg_val)
{
  uint16_t req_reg_val[1];
  
  ModbusQuery[1].u8id = SlaveModbusAdd;
  ModbusQuery[1].u8fct = 6;
  ModbusQuery[1].u16RegAdd = addr - 1;
  ModbusQuery[1].u16CoilsNo = 1;
  ModbusQuery[1].au16reg = req_reg_val;
  req_reg_val[0] = reg_val;

  ControllinoModbusMaster.begin( 9600 );
  ControllinoModbusMaster.setTimeOut( 5000 );

  WaitingTime = millis() + 1000;

  while (millis() > WaitingTime);

  ControllinoModbusMaster.query( ModbusQuery[1] );

again2:
  ControllinoModbusMaster.poll();
  if (ControllinoModbusMaster.getState() == COM_IDLE)
  {
    Serial2.end();
    //Serial.println("Speed changed!");
    return;
  }
  else
  {
   goto again2;
  } 
}
