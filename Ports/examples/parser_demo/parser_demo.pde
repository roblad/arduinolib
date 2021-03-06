//>>> The latest version of this code can be found at https://github.com/jcw/ !!

// Example o how to use the InputParser class
// 2010-10-23 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
// $Id: parser_demo.pde 7763 2011-12-11 01:28:16Z jcw $

#include <Ports.h>
#include <RF12.h> // needed to avoid a linker error :(

extern InputParser::Commands cmdTab[] PROGMEM;
InputParser parser (50, cmdTab);

static void helloCmd () {
    Serial.println("Hello!");
}

static void valueCmd () {
    int v;
    parser >> v;
    Serial.print("value = ");
    Serial.println(v);
}

static void longCmd () {
    long v;
    parser >> v;
    Serial.print("long = ");
    Serial.println(v);
}

static void stringCmd () {
    const char* v;
    parser >> v;
    Serial.print("string = <");
    Serial.print(v);
    Serial.println('>');
}

static void complexCmd () {
    long v;
    const char *s, *t;
    int m, n;
    parser >> v >> s >> m >> t >> n;
    Serial.print("complex = ");
    Serial.print(v);
    Serial.print(" <");
    Serial.print(s);
    Serial.print("> ");
    Serial.print(m);
    Serial.print(" <");
    Serial.print(t);
    Serial.print("> ");
    Serial.print(n);
    Serial.println();
}

InputParser::Commands cmdTab[] = {
    { 'h', 0, helloCmd },
    { 'v', 2, valueCmd },
    { 'l', 4, longCmd },
    { 's', 1, stringCmd },
    { 'c', 7, complexCmd },
    { 0 }    
};

void setup () {
    Serial.begin(57600);
    Serial.println("\n[parser_demo]");
}

void loop () {
    parser.poll();
}
