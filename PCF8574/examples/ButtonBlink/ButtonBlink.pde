#include <PCF8574.h> // Required for ... all 

#include <Wire.h> // Required for I2C communication

PCF8574 expander; // Create object from PCF8574 class

void setup(){

  Serial.begin(9600); // Setup serial for read echo
  
  expander.pinMode(0,OUTPUT); // Setup pin D0 as output
  expander.pinMode(1,OUTPUT); // Setup pin D1 as output
  expander.pinMode(2,OUTPUT); // Setup pin D2 as output
  expander.pinMode(3,INPUT); // Setup pin D3 as input

  expander.begin(0x20);
  
  expander.pullUp(3); // Enable pull-up resistor on pin D3 (button)
  
  expander.enableInterrupt(8,ISRgateway); // Enable interrupt by attach interruption handler to arduino pin D8 and save ISRgateway as callback function
  
  expander.attachInterrupt(3,ISRexpander,FALLING); // Attach an virtual interrupt on the pin D3 (button) of the PCF8573
}

void ISRgateway(){
  expander.checkForInterrupt(); // ISRgateway act as an wrapper for attachInterrupt from namespace PCF8574:: to the global scope
}

void ISRexpander(){ // ISRexpander will be call when the PCF8574 pin D3 will goes from high to low
  digitalWrite(13,HIGH);  //
  delay(1000);            // Little blink for debug / test
  digitalWrite(13,LOW);   //
  
  expander.blink(0,5,200); // Blink the led on pin D0 on PCF8574 (to test TWI into an ISR called function /debug)
}

void loop(){
  expander.blink(0,5,200); // Blink led 1
  delay(1000);
  expander.blink(1,5,200); // Blink led 2
  delay(1000);

  expander.digitalWrite(1,LOW); // Turn on led 2
  delay(1000);
  expander.digitalWrite(1,HIGH); // Turn off led 2
  delay(1000);

  expander.toggle(2);  // Toggle led 3
  delay(1000);

  expander.write(255); // All led off
  delay(1000);
  expander.write(0); // All led on
  delay(1000);

  expander.detachInterrupt(3); // Temporaly disable interrupt on D3 (one button)
  delay(1000); // Let the time (or not xD) to press the button
  Serial.println(expander.digitalRead(3),DEC); // Read pin D3 state
  Serial.println(expander.read(),DEC); // Read
  delay(1000);
  expander.attachInterrupt(3,ISRexpander,FALLING); // Re-enable interrupt on pin D3

  expander.set();  // All led off
  delay(1000);
  expander.clear();  // All led on
  delay(5000);
 
} // Loop forever
