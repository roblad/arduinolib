/*
*/

#include <SPI.h>
#include <MCP2515.h>
#include <OBDIICanbus.h>

#define SERIAL_DEBUG

void setup()
{
#ifdef SERIAL_DEBUG
  Serial.begin(9600);
  Serial.println("OBDIIReader");
#endif

  // Initialize the MCP2515
  if (!OBDIICanbus.init(SPEED_500_KBPS))
  {
#ifdef SERIAL_DEBUG
    Serial.println("Failure:MCP2515 Initialization");
#endif    
  }
  else
  {
#ifdef SERIAL_DEBUG
    Serial.println("MCP2515 is Active");
#endif    
  }
}
 

void loop()
{
/*    
  if(Canbus.ecu_req(ENGINE_RPM,buffer) == 1)
  {
  } 
  digitalWrite(LED3, HIGH);
   
  if(Canbus.ecu_req(VEHICLE_SPEED,buffer) == 1)
  {
    sLCD.print(COMMAND,BYTE);
    sLCD.print(LINE0 + 9,BYTE);
    sLCD.print(buffer);
  }
  
  if(Canbus.ecu_req(ENGINE_COOLANT_TEMP,buffer) == 1)
  {
    sLCD.print(COMMAND,BYTE);
    sLCD.print(LINE1,BYTE);                     
    sLCD.print(buffer);
  }
  
  if(Canbus.ecu_req(THROTTLE,buffer) == 1)
  {
    sLCD.print(COMMAND,BYTE);
    sLCD.print(LINE1 + 9,BYTE);
    sLCD.print(buffer);
  }  
  */
//  Canbus.ecu_req(O2_VOLTAGE,buffer);
}

uint32_t parsedecimal(char *str)
{
  uint32_t d = 0;
  
  while (str[0] != 0) {
   if ((str[0] > '9') || (str[0] < '0'))
     return d;
   d *= 10;
   d += str[0] - '0';
   str++;
  }
  return d;
}