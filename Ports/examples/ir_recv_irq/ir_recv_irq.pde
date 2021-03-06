//>>> The latest version of this code can be found at https://github.com/jcw/ !!

// Infrared receiver using the InfraredPlug class (interrupt version)
// 2010-10-12 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
// $Id: ir_recv_irq.pde 7763 2011-12-11 01:28:16Z jcw $

#include <Ports.h>
#include <RF12.h> // needed to avoid a linker error :(

InfraredPlug ir (2);
byte value;

ISR(PCINT1_vect) {
    ir.poll();
}

void setup () {
    Serial.begin(57600);
    Serial.println("\n[ir_recv_irq]");

    // enable pin change interrupts on PC1
    PCMSK1 = bit(1);
    PCICR |= bit(PCIE1);
}

void loop () {
    if (Serial.available()) {
        char c = Serial.read();
        if ('0' <= c && c <= '9')
            value = 10 * value + c - '0';
        else {
            if (c == 's')
                ir.configure(value); // 20 ms end-of-data gap
            value = 0;
        }
    }
    
    byte count = ir.done();
    if (count > 0) {
        const byte* data = ir.buffer();
        Serial.print("IR ");
        switch (ir.decoder(count)) {
            case InfraredPlug::NEC:
                Serial.print("NEC");
                for (byte i = 0; i < 4; ++i) {
                    Serial.print(' ');
                    Serial.print(data[i], DEC);
                }
                break;
            case InfraredPlug::NEC_REP:
                Serial.print("NEC REPEAT");
                break;
            default:
                for (byte i = 0; i < count; ++i) {
                    byte nibble = (data[i/2] >> (i % 2) * 4) & 0x0F;
                    Serial.print(nibble, HEX);
                }
        }
        Serial.println();
    }
}
