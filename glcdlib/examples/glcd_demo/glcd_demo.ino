// Demo display for the Graphics Boad
// 2010-11-14 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include <GLCD_ST7565.h>
#include <JeeLib.h>
#include <avr/pgmspace.h>
#include "utility/font_clR6x8.h"
#include "stdlib.h"
#include "utility/font_8x13B.h"

#include "utility/font_clR6x6.h"
#include "utility/font_clR5x8.h"
#include "utility/font_5x7.h"
#include "utility/font_4x6.h"
#include "utility/font_lubB24.h"

#define PIN_SID  6
#define PIN_SCLK 5
#define PIN_A0   4
#define PIN_RST  3
byte FONT_HIGHT=10; // set font 
class GraphicsBoard : public GLCD_ST7565, public Print {
    byte x, y, dirty;
    MilliTimer refreshTimer;
    
    void newline() {
        x = 0;
        if (y >= 52)
            scroll(SCROLLUP,FONT_HIGHT );
        else
            y += FONT_HIGHT;
    }
    
public:
    GraphicsBoard () : x (0), y (0), dirty (0) {}
    
    void poll(byte rate =100) {
        if (refreshTimer.poll(rate) && dirty) {
            refresh();
            dirty = 0;
        }
    }
    
    virtual size_t write(byte c) {
        if (c == '\r')
            x = 0;
        else if (c == '\n')
            x = 127;
        else {
            if (x > 122)
                newline();
            drawChar(x, y, c);
            x += 4;
            dirty = 1;
        }
        return 1;
    }
};


    //pinMode(PIN_CS,   OUTPUT);
/*
    digitalWrite(PIN_RST, LOW);
    _delay_ms(500);
    digitalWrite(PIN_RST, HIGH);
*/

static uint8_t icon_lbattery_16x8 [5] [16] PROGMEM = {
{
0x3C,0x7E,0x7F,0xFF,0xFF,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xFF,0xFF
},{
0x3C,0x7E,0x7F,0xFF,0xFF,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xFF,0xC3,0xFF,0xFF

},{
0x3C,0x7E,0x7F,0xFF,0xFF,0xC3,0xC3,0xC3,0xC3,0xC3,0xFF,0xC3,0xFF,0xC3,0xFF,0xFF
},{
0x3C,0x7E,0x7F,0xFF,0xFF,0xC3,0xC3,0xC3,0xFF,0xC3,0xFF,0xC3,0xFF,0xC3,0xFF,0xFF
},{
0x3C,0x7E,0x7F,0xFF,0xFF,0xC3,0xFF,0xC3,0xFF,0xC3,0xFF,0xC3,0xFF,0xC3,0xFF,0xFF
}
};

static uint8_t icon_heating_8x16 [5][16] PROGMEM = {
{
0x00, 0xFE, 0x03, 0x55, 0x55, 0x03, 0xFE, 0x00, 0x1E, 0x63, 0x5C, 0xDD, 0xDD, 0x5C, 0x63, 0x1E
},{
0x00, 0xFE, 0x03, 0x55, 0x55, 0x03, 0xFE, 0x00, 0x1E, 0x63, 0x5C, 0xDF, 0xDF, 0x5C, 0x63, 0x1E
},{
0x00, 0xFE, 0x03, 0xD5, 0xD5, 0x03, 0xFE, 0x00, 0x1E, 0x63, 0x5C, 0xDF, 0xDF, 0x5C, 0x63, 0x1E
},{
0x00, 0xFE, 0x03, 0xF5, 0xF5, 0x03, 0xFE, 0x00, 0x1E, 0x63, 0x5C, 0xDF, 0xDF, 0x5C, 0x63, 0x1E
},{
0x0E, 0xF3, 0x01, 0xFD, 0xFD, 0x01, 0xF3, 0x0E, 0x1E, 0x63, 0x5C, 0xDF, 0xDF, 0x5C, 0x63, 0x1E
}
};
static uint8_t icon_home_13x12 [] PROGMEM = {
0x60, 0x30, 0xDF, 0xEF, 0xF6, 0xFB, 0xFD, 0xFB, 0xF6, 0xEC, 0xD8, 0x30, 0x60, 0x00, 0x00, 0x00,
0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00
};
static uint8_t icon_lines_12x12 [] PROGMEM = {
0x6C, 0x24, 0x36, 0xFE, 0x25, 0x25, 0xFE, 0x36, 0x24, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x0C, 0x06, 0x05, 0x06, 0x06, 0x05, 0x06, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static uint8_t icon_solar_12x12 [5] [32] PROGMEM = {
{
0xC0, 0x60, 0x30, 0x18, 0x08, 0x0C, 0x92, 0xA2, 0xE2, 0xC4, 0x58, 0x60, 0x00, 0x00, 0x00, 0x00,
0x01, 0x03, 0x02, 0x02, 0x02, 0x07, 0x0D, 0x08, 0x08, 0x08, 0x0D, 0x06, 0x00, 0x00, 0x00, 0x00
},{
0x00, 0x20, 0x04, 0x60, 0x90, 0x08, 0x0A, 0x90, 0x60, 0x04, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x02, 0x00, 0x00, 0x01, 0x05, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
},{
0x20, 0x20, 0x04, 0x68, 0x90, 0x08, 0x0B, 0x90, 0x68, 0x04, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x02, 0x01, 0x00, 0x01, 0x0D, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
},{
0x60, 0x60, 0x0C, 0x6C, 0x90, 0x0B, 0x0B, 0x90, 0x6C, 0x0C, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x03, 0x03, 0x00, 0x0D, 0x0D, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
},{
0x60, 0x66, 0x0E, 0x6C, 0x90, 0x0B, 0x0B, 0x90, 0x6C, 0x0E, 0x66, 0x60, 0x00, 0x00, 0x00, 0x00,
0x00, 0x06, 0x07, 0x03, 0x00, 0x0D, 0x0D, 0x00, 0x03, 0x07, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00
}
};

//GLCD_ST7565 glcd;
GraphicsBoard glcd;
static void st7565_Display_Command(byte c) {
    #include <GLCD_ST7565_cmds.h>
    bitClear(PORTD, 4);
    
    for (byte mask = 0x80; mask != 0; mask >>= 1) {
        bitWrite(PORTD, 6, c & mask);
        // this is 15% faster, but it's too fast for this chip...
        //PIND = bit(4);
        // even plain set/clear is too fast, so slow down a bit more
        bitSet(PORTD, 5);
        bitClear(PORTD, 5);
    }
}
float sensorValue;
unsigned int adcreading;  //for bang up voltage
int volts;


 long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2);
  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1125300L / result;
  return result;
};
//battery control 
int battVolts(void) {
#if defined(__AVR_ATmega168__)
 analogReference(DEFAULT);
#else
 analogReference(INTERNAL);
#endif
  double vccref = readVcc()/1000.0;
  adcreading = analogRead(14) * 2;
  double battvol = (adcreading / 1023.0) * vccref;
  return battvol * 1000;
}; 

void setup () {
  
    pinMode(PIN_SID,  OUTPUT);
    pinMode(PIN_SCLK, OUTPUT);
    pinMode(PIN_A0,   OUTPUT);
    pinMode(PIN_RST,  OUTPUT);
  pinMode(7, OUTPUT);   //("LED Zielona pin 7");  
  pinMode(8, OUTPUT);   //("LED Niebieska pin 8"); 
  pinMode(17, OUTPUT);  // ("LED Czerwona pin 17"); 
  pinMode(15, INPUT_PULLUP);  //S1
  pinMode(16, INPUT_PULLUP);  //S2
  //analogReference(DEFAULT);
  sensorValue = analogRead(14) * 2;  //pomiar napiecia
  
    rf12_initialize(31, RF12_868MHZ,210);
    rf12_sleep(RF12_SLEEP);
    
    glcd.begin(0x20);
    glcd.backLight(120);
    glcd.setFont(font_8x13B);
    
st7565_Display_Command(CMD_SET_DISP_REVERSE);
glcd.clear();
glcd.drawString_P(30,22, PSTR("ROBO LAB"));
//glcd.drawString_P(30,42, PSTR(battVolts()));
glcd.refresh();
delay (2000);
glcd.clear();
  glcd.setFont(font_clR6x8);
//st7565_Display_Command(CMD_SET_ALLPTS_NORMAL);
    // draw a string at a location, use _p variant to reduce RAM use
    glcd.drawString_P(40,  0, PSTR("GLCDlib"));
    glcd.drawString_P(10, 16, PSTR("ST7565 128x64 GLCD"));
    glcd.drawString_P(22, 32, PSTR("Graphics Board"));
    glcd.drawString_P(20, 48, PSTR("JeeLabs.org/gb1"));
    
    
    glcd.drawCircle(5, 5, 5, WHITE);
    glcd.fillCircle(121, 5, 5, WHITE);
    glcd.fillCircle(6, 58, 5, WHITE);
    glcd.drawCircle(121, 58, 5, WHITE);

    glcd.drawLine(40, 9, 81, 9, WHITE);
    glcd.drawLine(40, 11, 81, 11, WHITE);
    glcd.drawLine(0, 42, 14, 28, WHITE);
    glcd.drawLine(112, 42, 126, 28, WHITE);
    glcd.drawRect(0, 28, 127, 15, WHITE);

    glcd.refresh();

// glcd.updateDisplayArea(0,0,128,64,1);
//  glcd.refresh();
    
    //Sleepy::powerDown(); // power consumption is now only the GLCD + backlight
}

void loop () {
  digitalWrite(PIN_A0, LOW);
  shiftOut(PIN_SID, PIN_SCLK, MSBFIRST, CMD_SET_DISP_NORMAL);
  shiftOut(PIN_SID, PIN_SCLK, MSBFIRST,CMD_SET_VOLUME_FIRST);
  shiftOut(PIN_SID, PIN_SCLK, MSBFIRST,CMD_SET_VOLUME_SECOND | (0x18));
delay(2000);
st7565_Display_Command(CMD_DISPLAY_OFF);
//glcd.refresh();
delay(2000);

st7565_Display_Command(CMD_DISPLAY_ON);
st7565_Display_Command(CMD_SET_DISP_NORMAL);
st7565_Display_Command(CMD_SET_VOLUME_FIRST);
st7565_Display_Command(CMD_SET_VOLUME_SECOND | (0x18));
st7565_Display_Command(CMD_RMW_CLEAR);
  

 
    glcd.drawLine(40, 9, 81, 9, 0);
    glcd.drawLine(40, 11, 81, 11, 0);
    glcd.drawLine(0, 42, 14, 28, 0);
    glcd.drawLine(112, 42, 126, 28, 0);
    glcd.drawRect(0, 28, 127, 15, 0);
    glcd.refresh();
//glcd.begin(0x18);
glcd.refresh();
delay (2000);
glcd.refresh();

    glcd.drawLine(40, 9, 81, 9, 1);
    glcd.drawLine(40, 11, 81, 11, 1);
    glcd.drawLine(0, 42, 14, 28, 1);
    glcd.drawLine(112, 42, 126, 28, 1);
    glcd.drawRect(0, 28, 127, 15, 1);
    glcd.refresh();
    delay (2000);
st7565_Display_Command(CMD_DISPLAY_OFF);
delay(2000);

st7565_Display_Command(CMD_SET_DISP_REVERSE);
st7565_Display_Command(CMD_DISPLAY_ON);
//glcd.begin(0x20);
st7565_Display_Command(CMD_SET_VOLUME_FIRST);
st7565_Display_Command(CMD_SET_VOLUME_SECOND | (0x20));
st7565_Display_Command(CMD_RMW_CLEAR);
//glcd.drawLine(40, 14, 81, 16, WHITE);
glcd.refresh();
    glcd.drawLine(40, 9, 81, 9, 0);
    glcd.drawLine(40, 11, 81, 11, 0);
    glcd.drawLine(0, 42, 14, 28, 0);
    glcd.drawLine(112, 42, 126, 28, 0);
    glcd.drawRect(0, 28, 127, 15, 0);
    glcd.refresh();
//glcd.begin(0x18);
glcd.refresh();
delay (2000);
    glcd.drawLine(40, 9, 81, 9, 1);
    //glcd.drawLine(41, 9, 82, 9, 1);
    glcd.drawLine(40, 11, 81, 11, 1);
    glcd.drawLine(0, 42, 14, 28, 1);
    glcd.drawLine(112, 42, 126, 28, 1);
    glcd.drawRect(0, 28, 127, 15, 1);
    glcd.refresh();
    
      //  glcd.drawCircle(5, 5, 10, WHITE);
    //glcd.fillCircle(121, 5, 10, WHITE);
    glcd.clear();
    
     glcd.setFont(font_clR6x8);
    glcd.drawChar(50, 30, ((float)sensorValue));
     glcd.refresh();
    delay(200);
    
    for (byte i=1; i<30 ;i++) {
    //glcd.drawCircle
    glcd.fillCircle(64, 32, i,1);
    st7565_Display_Command(CMD_RMW_CLEAR);
    glcd.refresh();
      //glcd.fillCircle(64, 32, i,1);
     // st7565_Display_Command(CMD_RMW_CLEAR);
    //glcd.refresh();
    delay (100);
}
  
    //glcd.drawCircle(121, 58, 10, WHITE);
    delay (100);
 glcd.drawLine(64, 0, 64, 64, WHITE); //top vertical line
 glcd.refresh();
 delay (100);
 glcd.drawLine(64, 0, 64, 64, 0);
 glcd.refresh();
 delay (100);
 glcd.drawLine(0, 32, 128, 32, WHITE); //middle horizontal line 
     glcd.refresh();
     delay (100);
     glcd.drawLine(0, 32, 128, 32, 0);
    glcd.refresh();
    delay (2000);
    glcd.clear();
    
    glcd.drawString_P(40,  0, PSTR("GLCDlib"));
    glcd.drawString_P(10, 16, PSTR("ST7565 128x64 GLCD"));
    glcd.drawString_P(22, 32, PSTR("Graphics Board"));
    glcd.drawString_P(20, 48, PSTR("JeeLabs.org/gb1"));
    
       // glcd.drawBitmap(13,0,icon_lines_12x12,16,12,1);
 
 for (byte i=0; i<5;++i){
   //glcd.setUpdateArea(26,0,38,12,0);
   glcd.clear();
   glcd.drawBitmap(26,0,icon_heating_8x16[i],8,16,1);
   glcd.updateDisplayArea(26,0,34,16,1);

   //glcd.refresh();
   glcd.drawBitmap(100,40,icon_solar_12x12[i],16,12,1);
   glcd.updateDisplayArea(100,40,116,52,1);
   glcd.drawBitmap(23,40,icon_home_13x12,16,12,1);
   glcd.updateDisplayArea(23,56,39,68,1);
   glcd.refresh();
   glcd.drawBitmap(100,20,icon_lbattery_16x8[i],16,8,1);
   glcd.updateDisplayArea(100,20,116,28,1);
   //glcd.refresh();
   
   glcd.drawBitmap(100,0,icon_lines_12x12,16,12,1);
   glcd.updateDisplayArea(100,0,116,12,1);

   //glcd.refresh();
   glcd.refresh();

delay (500);
 }

for (byte i=1;i<10;i++){
    
glcd.scroll(1,i);
glcd.refresh();
   delay (100);
}
 glcd.clear();
        glcd.drawString_P(40,  0, PSTR("GLCDlib"));
    glcd.drawString_P(10, 16, PSTR("ST7565 128x64 GLCD"));
    glcd.drawString_P(22, 32, PSTR("Graphics Board"));
    glcd.drawString_P(20, 48, PSTR("JeeLabs.org/gb1"));
    glcd.refresh();
    delay (1000);
        //glcd.drawBitmap(13,0,icon_lines_12x12,16,12,1);
 st7565_Display_Command(CMD_SET_DISP_REVERSE);
 glcd.refresh();
 for (byte i=1;i<15;i++){
    
glcd.scroll(2,i);
glcd.refresh();
   delay (100);
}

        glcd.drawString_P(40,  0, PSTR("GLCDlib"));
    glcd.drawString_P(10, 16, PSTR("ST7565 128x64 GLCD"));
    glcd.drawString_P(22, 32, PSTR("Graphics Board"));
    glcd.drawString_P(20, 48, PSTR("JeeLabs.org/gb1"));
    glcd.refresh();
    
     for (byte i=1;i<16;i++){
    
glcd.scroll(3,i);
delay (50);
 st7565_Display_Command(CMD_SET_DISP_REVERSE);
glcd.refresh();
   delay (200);
//st7565_Display_Command(CMD_RMW_CLEAR);
   st7565_Display_Command(CMD_SET_DISP_NORMAL);
glcd.refresh();
delay (100);

//st7565_Display_Command(CMD_RMW_CLEAR);

}

    for (byte i=0;i <188;++i){
    glcd.drawLine(0, i, i, 0, 1);
    delay (10);
 glcd.refresh();  
  }
  
  
 for (byte i=0;i <128;++i){
    glcd.drawLine(64, 32, i,i , 0);
    delay (10);
 glcd.refresh();  
 i++;
 i++;
 i++;
  }
    //glcd.drawLine(112, 42, 126, 28, 1);
    
    glcd.clear();
for (byte i=0;i <150;i++){    
    glcd.setPixel(128-i, 32, 1);
    glcd.setPixel((128-i)-i, 34, 1);
    glcd.setPixel((128-i)-i, 36, 1);
    glcd.setPixel((i), 38, 1);
    //glcd.setPixel(random(128), sqrt(i), 1);
    //glcd.setPixel(sqrt(random(128)),sqrt(random(64)), 1);
    glcd.setPixel(random(128),random(64), 1);
    glcd.refresh();
    delay(30);
    i++;
}
    
    glcd.clear();
    
    
        glcd.setFont(font_8x13B);
 
 
    sensorValue = analogRead(14) * 2;  //pomiar napiecia
    delay(20);
    glcd.setFont(font_clR6x8);
    //glcd.println("------------------------------");
    //glcd.println();
    glcd.print((float)(battVolts()));glcd.println("  Volt");
    glcd.refresh();
    delay(5000);
    
    
    glcd.clear();
    glcd.setFont(font_clR6x8);
    glcd.drawString_P(40,  0, PSTR("GLCDlib"));
    glcd.drawString_P(10, 16, PSTR("ST7565 128x64 GLCD"));
    glcd.drawString_P(22, 32, PSTR("Graphics Board"));
    glcd.drawString_P(20, 48, PSTR("JeeLabs.org/gb1"));
    glcd.refresh();
     
    glcd.drawCircle(5, 5, 5, WHITE);
    glcd.fillCircle(121, 5, 5, WHITE);
    glcd.fillCircle(6, 58, 5, WHITE);
    glcd.drawCircle(121, 58, 5, WHITE); 
glcd.refresh();
}
