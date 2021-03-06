/*
  Emon.h - Library for openenergymonitor
  Created by Trystan Lea, April 27 2010
  GNU GPL
  modified to use up to 12 bits ADC resolution (ex. Arduino Due)
  also includes 3-phase buffer-delay algorithm for monitoring different line phases with single AC-AC voltage adapter.
  by boredman@boredomprojects.net 26.12.2013
*/

#ifndef EmonLib_3PH_h
#define EmonLib_3PH_h

#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"

#else

#include "WProgram.h"

#endif

// to enable 12-bit ADC resolution on Arduino Due, 
// include the following line in main sketch inside setup() function:
//  analogReadResolution(ADC_BITS);
// otherwise will default to 10 bits, as in regular Arduino-based boards.
#if defined(__arm__)
#define ADC_BITS    12
#else
#define ADC_BITS    10
#endif

#define ADC_COUNTS  (1<<ADC_BITS)


#define PHASE2 18     //  Number of samples delay for L2  -oryginall 65
#define PHASE3 36    //  Number  of samples delay for L3, also size of array oryginal 130
                      //  These can be adjusted if the phase correction is not adequate
//#define PHASE2 65     //  Number of samples delay for L2
//#define PHASE3 130    //  Number  of samples delay for L3, also size of array
                      //  These can be adjusted if the phase correction is not adequate

class EnergyMonitor
{
  public:

    void voltage(unsigned int _inPinV, double _VCAL, double _PHASECAL, int _PHASE);
    void current(unsigned int _inPinI, double _ICAL);

    void voltageTX(double _VCAL, double _PHASECAL, int _PHASE);
    void currentTX(unsigned int _channel, double _ICAL);

    void calcVI(unsigned int crossings, unsigned int timeout);
    double calcIrms(unsigned int NUMBER_OF_SAMPLES);
    void serialprint();

    long readVcc();
    //Useful value variables
    double realPower,
       apparentPower,
       powerFactor,
       Vrms,
       Irms;

  private:

    //Set Voltage and current input pins
    unsigned int inPinV;
    unsigned int inPinI;
    //Calibration coeficients
    //These need to be set in order to obtain accurate results
    double VCAL;
    double ICAL;
    double PHASECAL;
    int PHASE;

    //--------------------------------------------------------------------------------------
    // Variable declaration for emon_calc procedure
    //--------------------------------------------------------------------------------------
	//int lastSampleV,sampleV;   //sample_ holds the raw analog read value, lastSample_ holds the last sample
	int sampleV; //sample_ holds the raw analog read value
	//int lastSampleI,sampleI;                      
    int sampleI; 
	
	double lastFilteredV,filteredV;                   //Filtered_ is the raw analog value minus the DC offset
	//double lastFilteredI, filteredI;                  
	double filteredI;                  
	double offsetV;                          //Low-pass filter output
	double offsetI;                          //Low-pass filter output      
	
	double phaseShiftedV;                             //Holds the calibrated phase shifted voltage.

	double sqV,sumV,sqI,sumI,instP,sumP;              //sq = squared, sum = Sum, inst = instantaneous

	int startV;                                       //Instantaneous voltage at start of sample window.
    boolean lastVCross, checkVCross;                  //Used to measure number of times threshold is crossed.

};

#endif

