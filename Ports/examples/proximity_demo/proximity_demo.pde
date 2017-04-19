//>>> The latest version of this code can be found at https://github.com/jcw/ !!

// Demo of the Proximity Plug, based on ProximityPlug class in the Ports library
// 2010-03-19 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
// $Id: proximity_demo.pde 7763 2011-12-11 01:28:16Z jcw $

#include <Ports.h>
#include <RF12.h> // needed to avoid a linker error :(

PortI2C myBus (4, PortI2C::KHZ100); // max 100 KHz!
ProximityPlug sensor (myBus);

void setup () {
    Serial.begin(57600);
    Serial.println("\n[proximity_demo]");
    sensor.begin();
    
    // sensor.send();
    // sensor.write(ProximityPlug::SINFO);
    // sensor.receive();
    // for (;;) {
    //     byte c = sensor.read(0);
    //     if (c == 0)
    //         break;
    //     if (' ' <= c && c <= '~')
    //         Serial.print(c);
    //     else {
    //         Serial.print('<');
    //         Serial.print(c, HEX);
    //         Serial.print('>');
    //     }
    // }
    // sensor.read(1);
    // sensor.stop();
    // Serial.println();
}

void loop() {
    byte v = sensor.getReg(ProximityPlug::TPSTATUS);
    if (v != 0 && v != 0xFF) {
        Serial.print("PROX ");
        Serial.println(v, HEX);
    }
}