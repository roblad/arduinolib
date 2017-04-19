// phasecal_tool - example sketch for PLL50Hz library - Martin Roberts 25/2/13

// these constants are use to calculate the PHASECAL value from measured phase shift
// they are taken from here: http://openenergymonitor.org/emon/buildingblocks/explanation-of-the-phase-correction-algorithm
#define EMONLIB_PERIOD 377 // time in microseconds between samples
#define EMONLIB_0_PHASECAL 1.28 // PHASECAL value for 0 phase shift

#define LOOPCYCLES 250 // number of mains cycles for outer loop
#define LOOPSAMPLES (LOOPCYCLES * NUMSAMPLES)

#include <PLL50Hz.h>

long totalV1squared,totalV2squared,totalI1squared,totalI2squared,totalP1,totalP2;
long sumTimerCount;
float powerFactor1,powerFactor2,frequency;
word cycleCount;
boolean showCT2;
CycleData cd;

void setup()
{
  Serial.begin(9600);
  PLL.begin(&cd);
  showHelp();
}

void loop()
{
  if(PLL.newCycle) addCycle(); // a new mains cycle has been sampled
  
  if(cycleCount>=LOOPCYCLES)
  {
    calculatePFandF();
    cycleCount=0;
    showResults();
  }

  if(Serial.available())
  {
    int c=toupper(Serial.read());
    
    switch(c)
    {
      case 'H':
      case '?': showHelp(); break;
      case 'C': calcPhaseShift(); break;
      case '1': showCT2=false; break;
      case '2': showCT2=true; break;
    }
  }
}

// add data for new 50Hz cycle to total
void addCycle()
{
  totalV1squared+=cd.cycleV1squared;
  totalV2squared+=cd.cycleV2squared;
  totalI1squared+=cd.cycleI1squared;
  totalI2squared+=cd.cycleI2squared;
  totalP1+=cd.cycleP1;
  totalP2+=cd.cycleP2;
  sumTimerCount+=(OCR1A+1); // for average frequency calculation

  cycleCount++;
  PLL.newCycle=false;
}

// calculate power factors and frequency
void calculatePFandF()
{
  float Vrms,I1rms,I2rms;
  float V2rms; // need this because voltage interpolation makes it different from Vrms
  float realPower1,realPower2,apparentPower1,apparentPower2;
  
  Vrms = sqrt(((float)totalV1squared)/LOOPSAMPLES); 
  V2rms = sqrt(((float)totalV2squared)/LOOPSAMPLES); 
  I1rms = sqrt(((float)totalI1squared)/LOOPSAMPLES); 
  I2rms = sqrt(((float)totalI2squared)/LOOPSAMPLES); 

  realPower1 = (float)totalP1/LOOPSAMPLES;
  apparentPower1 = Vrms * I1rms;
  powerFactor1=abs(realPower1 / apparentPower1);
  realPower2 = (float)totalP2/LOOPSAMPLES;
  apparentPower2 = V2rms * I2rms;
  powerFactor2=abs(realPower2 / apparentPower2);
  frequency=((float)LOOPCYCLES*16000000.0)/((float)sumTimerCount*NUMSAMPLES);
 
  totalV1squared=0;
  totalV2squared=0;
  totalI1squared=0;
  totalI2squared=0;
  totalP1=0;
  totalP2=0;
  cycleCount=0;
  sumTimerCount=0;
}

void showResults()
{
  Serial.print("CT");
  Serial.write(showCT2?'2':'1');
  Serial.print(", Power Factor=");
  Serial.print(showCT2?powerFactor2:powerFactor1,4);
  Serial.print(", Frequency=");
  Serial.print(frequency);
  Serial.println("Hz, enter 'H' or '?' for help");
}

void calcPhaseShift()
{
  int Ilead;
  byte csum;
  
  Serial.println();
  Serial.println("Calculating phase shift...");
  if(!PLL.calculatePhaseShift(200, &Ilead, showCT2))
  {
    if(Ilead==0) Serial.println("Failed, PLL is unlocked");
    else Serial.println("Phase shift is out of range");
    Serial.println();
    return;
  }
  
  Serial.print("CT");
  Serial.write(showCT2?'2':'1');
  Serial.print(" current ");
  Serial.print((Ilead<0)?"lags":"leads");
  Serial.print(" voltage by ");
  Serial.print(abs((float)Ilead/16));
  Serial.print("\xb5s, ");
  Serial.print(abs((float)Ilead*360/(16.0*20000)));
  Serial.println(" degrees");
  Serial.print("PHASECAL should be ");
  Serial.println(EMONLIB_0_PHASECAL+(float)Ilead/(EMONLIB_PERIOD*16));
  Serial.println("Applying new value...");
  Serial.println();
  PLL.updatePhaseShift(Ilead,showCT2);
}

void showHelp()
{
  Serial.println();
  Serial.println("phasecal_tool");
  Serial.println("H,? - display this Help");
  Serial.println("1,2 - display data for CT1/2");
  Serial.println("C   - Calculate phase Shift");
  Serial.println();
}


