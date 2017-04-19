#include <Wire.h> // necessary, or the application won't build properly
#include <stdio.h>
#include <PCF8583.h>
/*****************************************************************************
 *  read/write serial interface to PCF8583 RTC via I2C interface
 *
 *  Arduino analog input 5 - I2C SCL (PCF8583 pin 6)
 *  Arduino analog input 4 - I2C SDA (PCF8583 pin 5)
 *
 *  You can set the type by sending it  TYYMMddhhmmss with one space before T
 *  You can set the type by sending it Ahhmmss 
 *
 ******************************************************************************/

int correct_address = 0;
PCF8583 p (0xA0);	
void setup(void){
  Serial.begin(9600);
  Serial.print("initialization...");
  p.init();
  Serial.print("booting...");
  Serial.println(" done");

}



void loop(void){

if(Serial.available() > 0){
  
      if(Serial.read() == 'A'){
       
       //Set Alarm
     
       p.hour_A  = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
       p.minute_A = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
       p.second_A = (byte) ((Serial.read() - 48) * 10 + (Serial.read() - 48));
       
       Serial.println("Setting  Alarm done !");
       p.set_daily_alarm();
       }
       
       if(Serial.read() == 'T'){
         
       // Set Date
         
       p.year= (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48)) + 2000;
       p.month = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
       p.day = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
       p.hour  = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
       p.minute = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
       p.second = (byte) ((Serial.read() - 48) * 10 + (Serial.read() - 48)); // Use of (byte) type casting and ascii math to achieve result.  

    
       Serial.println("Setting Date done !");
       p.set_time();
  }
}
  //Get time
  
  p.get_time();
  char time[50];
  sprintf(time, "%02d/%02d/%02d %02d:%02d:%02d", p.year, p.month, p.day, p.hour, p.minute, p.second);
  Serial.print("Time (y.m.d.h.m.s) : ");
  Serial.println(time);
 
  //Get alarm
  
  p.get_alarm();
  char alarm [50];
  sprintf(alarm, "%02d:%02d:%02d",p.hour_A, p.minute_A, p.second_A);
  Serial.print("Alarm (h.m.s) : ");
  Serial.println(alarm);
  delay(3000);
}





