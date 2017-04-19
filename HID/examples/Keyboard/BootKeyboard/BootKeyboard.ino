/*
  Copyright (c) 2014-2015 NicoHood
  See the readme for credit to other people.

  BootKeyboard example

  Shows that keyboard works even in bios.
  Led indicats if we are in bios.

  See HID Project documentation for more information.
  https://github.com/NicoHood/HID/wiki/Keyboard-API#boot-keyboard
*/

#include "HID-Project.h"

const int pinLed = LED_BUILTIN;
//const int pinButton = 2;

void setup() {
  pinMode(pinLed, OUTPUT);
  //pinMode(pinButton, INPUT_PULLUP);

  // Sends a clean report to the host. This is important on any Arduino type.
  BootKeyboard.begin();

    if (BootKeyboard.getProtocol() == HID_BOOT_PROTOCOL){
       // Simple debounce
}
    //System.write(SYSTEM_SLEEP);
} 

void loop() {
  // Light led if keyboard uses the boot protocol (normally while in bios)
  // Keep in mind that on a 16u2 and Arduino Micro HIGH and LOW for TX/RX Leds are inverted.
delay(1000);
 BootKeyboard.add(KEY_LEFT_WINDOWS);
    BootKeyboard.add(KEY_R);
    BootKeyboard.send();
    BootKeyboard.releaseAll();
    delay(1000);
    BootKeyboard.write(KEY_BACKSPACE);
    delay(1000);
    BootKeyboard.write(KEY_N);
            delay(150);
    BootKeyboard.write(KEY_O);
            delay(150);
    BootKeyboard.write(KEY_T);
           delay(150); 
    BootKeyboard.write(KEY_E);
            delay(150);
    BootKeyboard.write(KEY_P);
            delay(150);
    BootKeyboard.write(KEY_A);
            delay(150);
    BootKeyboard.write(KEY_D);
    delay(150);
    BootKeyboard.write('.');
    delay(150);
    BootKeyboard.write(KEY_E);
        delay(150);
    BootKeyboard.write(KEY_X);
        delay(150);
    BootKeyboard.write(KEY_E);
    
    
    delay(200);
    BootKeyboard.write(KEY_RETURN);
      delay(1000);
      BootKeyboard.write(KEY_ENTER);
    delay(1000);

   BootKeyboard.add('o');
    BootKeyboard.add('t');
    BootKeyboard.add('e');
    BootKeyboard.add('p');
    BootKeyboard.add('a');
    BootKeyboard.add('d');
    BootKeyboard.add('.');
    BootKeyboard.add('e');
    BootKeyboard.add('x');
    BootKeyboard.add('e');
    BootKeyboard.send();
    BootKeyboard.releaseAll();
   
    delay(1000);
  // Trigger caps lock manually via button
  //if (!digitalRead(pinButton)) {
   //BootKeyboard.write(KEY_MENU);
   //delay(1000);
   // BootKeyboard.write(KEY_PAGE_DOWN);
   //  delay(3000);
   //      BootKeyboard.write(KEY_PAGE_DOWN);
   //  delay(3000);
  //  BootKeyboard.write(KEY_ENTER);
  //       delay(3000);
   // BootKeyboard.write(KEY_F1);

      //       delay(3000);
    //BootKeyboard.write(KEY_APPLICATION);
     //    delay(3000);
    
    // Simple debounce
    //delay(1000);
    //System.write(SYSTEM_SLEEP);
    //BootKeyboard.add(KEY_LEFT_WINDOWS);
    //BootKeyboard.add(KEY_R);
    //BootKeyboard.send();
    //BootKeyboard.releaseAll();
    //delay(1000);
    
    //BootKeyboard.write(KEY_ENTER);
    //delay(1000);
    /*BootKeyboard.add('n');
    BootKeyboard.add('o');
    BootKeyboard.add('t');
    BootKeyboard.add('e');
    BootKeyboard.add('p');
    BootKeyboard.add('a');
    BootKeyboard.add('d');
    BootKeyboard.add('.');
    BootKeyboard.add('e');
    BootKeyboard.add('x');
    BootKeyboard.add('e');
    BootKeyboard.send();
    BootKeyboard.releaseAll();
    */
    //delay(1000);
    //BootKeyboard.write(KEY_ENTER);
    //delay(1000);
    //BootKeyboard.write(KEY_LEFT_WINDOWS);
    //delay(1000);



  // Trigger caps lock manually via button
  //if (!digitalRead(pinButton)) {
   //BootKeyboard.write(KEY_MENU);
   //delay(1000);
   // BootKeyboard.write(KEY_PAGE_DOWN);
   //  delay(3000);
   //      BootKeyboard.write(KEY_PAGE_DOWN);
   //  delay(3000);
  //  BootKeyboard.write(KEY_ENTER);
  //       delay(3000);
   // BootKeyboard.write(KEY_F1);

      //       delay(3000);
    //BootKeyboard.write(KEY_APPLICATION);
     //    delay(3000);
    

    
 // }
}






