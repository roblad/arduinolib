//>>> The latest version of this code can be found at https://github.com/jcw/ !!

// Demo readout of the 4-channel 18-bit MCP3424 on the Analog Plug v2.
// 2009-09-28 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
// $Id: analog_demo.pde 7763 2011-12-11 01:28:16Z jcw $

#include <Ports.h>
#include <RF12.h> // needed to avoid a linker error :(

PortI2C myI2C (3);
DeviceI2C adc (myI2C, 0x68);

static void AP2init (DeviceI2C& dev, byte mode =0x1C) {
    // default mode is channel 1, continuous, 18-bit, gain x1
    dev.send();
    dev.write(mode);
    dev.stop();
}

static long AP2read (DeviceI2C& dev) {
    dev.receive();
    long raw = (long) dev.read(0) << 16;
    raw |= (word) dev.read(0) << 8;
    raw |= dev.read(0);
    byte status = adc.read(1);
    return (raw * 1000) / 64;
}

void setup () {
    Serial.begin(57600);
    Serial.print("\n[analog_demo]");
    rf12_config();
    rf12_easyInit(0);
    AP2init(adc);
}

void loop () {
    long val = AP2read(adc);
    
    rf12_easyPoll();
    rf12_easySend(&val, sizeof val);

    Serial.print("VOLT ");
    Serial.println(val);  

    delay(1000);
}
