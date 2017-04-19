/* Phase-locked loop library for solar PV system, Martin Roberts 23/02/13
based on OpenEnergyMonitor http://openenergymonitor.org/emon/

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "PLL50Hz.h"

//#define SAMPPIN 5 // this output goes high each time the 50Hz input is sampled
#define TIMERPERIOD (20000/NUMSAMPLES) // nominal timer period in microseconds
#define TIMERTOP ((TIMERPERIOD*16)-1) // terminal count for PLL timer
#define PLLTIMERRANGE 100 // limit timer range ~+/-0.5Hz
#define ADCSHIFT 2 // shift value ADC result to improve DC offset resolution for current
#define ADCMULT (1<<ADCSHIFT) // multiplier for ADC result to improve DC offset resolution
#define PLLTIMERMAX ((TIMERTOP+PLLTIMERRANGE)<<ADCSHIFT)
#define PLLTIMERMIN ((TIMERTOP-PLLTIMERRANGE)<<ADCSHIFT)
#define PLLLOCKCOUNT 250 // number of cycles to determine if PLL is locked
#define OFFSETCENTRE (512<<ADCSHIFT)
#define PLLLOCKRANGE (40<<ADCSHIFT) // allowable ADC range to enter locked state
#define PLLUNLOCKRANGE (80<<ADCSHIFT) // allowable ADC range to remain locked
#define OFFSETFILTERSHIFT 7

PLL50Hz PLL; // the instance of PLL50Hz, only 1 allowed
int voltsOffset=OFFSETCENTRE,I1Offset=OFFSETCENTRE,I2Offset=OFFSETCENTRE; // start offsets at ADC centre
long sumV1squared,sumV2squared,sumI1squared,sumI2squared,sumP1,sumP2;
byte samples;
float V[NUMSAMPLES],I[NUMSAMPLES];
boolean sampling,samplingI2;
int sampleCount;
int I1PhaseShift;
int I2PhaseShift;
long CTtotalDelay;
int CT1delay,CT2delay;
word pllUnlocked,timerCount;
CycleData *cdp;

void setIdelay(boolean setI2delay)
{
  samplingI2=setI2delay;
  
	CTtotalDelay=0;
  sampleCount=10;
  
  while(sampleCount) delay(1);
  
	CTtotalDelay/=(10*NUMSAMPLES);
	if(setI2delay) CT2delay=(int)CTtotalDelay;
	else CT1delay=(int)CTtotalDelay;
}

void PLL50Hz::begin(CycleData *pcd)
{
  cdp=pcd;
#ifdef SYNCPIN
  pinMode(SYNCPIN, OUTPUT);
  digitalWrite(SYNCPIN, LOW);
#endif
  //pinMode(SAMPPIN, OUTPUT);
  //digitalWrite(SAMPPIN, LOW);
  pinMode(LOAD1PIN,OUTPUT);
  pinMode(LOAD2PIN,OUTPUT);

	pllUnlocked=PLLLOCKCOUNT;
  
  // change ADC prescaler to /64 = 250kHz clock
  // slightly out of spec of 200kHz but should be OK
  ADCSRA &= 0xf8;  // remove bits set by Arduino library
  ADCSRA |= 0x06; 

  //set timer 1 interrupt for required period
	timerCount=(TIMERTOP<<ADCSHIFT);
  noInterrupts();
  TCCR1A = 0; // clear control registers
  TCCR1B = 0;
  TCNT1  = 0; // clear counter
  OCR1A = TIMERTOP; // set compare reg for timer period
  bitSet(TCCR1B,WGM12); // CTC mode
  bitSet(TCCR1B,CS10); // no prescaling
  bitSet(TIMSK1,OCIE1A); // enable timer 1 compare interrupt
  bitSet(ADCSRA,ADIE); // enable ADC interrupt
  interrupts();
	setIdelay(false);
	setIdelay(true);
  updatePhaseShift(0,false);
  updatePhaseShift(0,true);
}

// timer 1 interrupt handler
ISR(TIMER1_COMPA_vect)
{
  //digitalWrite(SAMPPIN,HIGH);
  ADMUX = _BV(REFS0) | VOLTSPIN; // start ADC conversion for voltage
  ADCSRA |= _BV(ADSC);
  samples++;
  if(samples>=NUMSAMPLES) samples=0;
  //digitalWrite(SAMPPIN,LOW);
}

void updatePLL(int newV, int newI1, int newI2)
{
  static int oldV,lastV,I10,I20;
	static long fVoltsOffset,fI1Offset,fI2Offset;
  boolean rising;

  rising=(newV>lastV); // synchronise to rising zero crossing
  lastV=newV;
  
  if(samples==0) // end of one 50Hz cycle
  {
#ifdef SYNCPIN
    digitalWrite(SYNCPIN,HIGH);
#endif
    if(rising)
    {
      // if we're in the rising part of the 50Hz cycle adjust the final timer count
      // to move newV towards 0, only adjust if we're moving in the wrong direction
      if(((newV<0)&&(newV<=oldV))||((newV>0)&&(newV>=oldV))) timerCount-=newV;
      // limit range of PLL frequency
      timerCount=constrain(timerCount,PLLTIMERMIN,PLLTIMERMAX);
      if(abs(newV)>PLLUNLOCKRANGE) pllUnlocked=PLLLOCKCOUNT; // we're unlocked
      else if(pllUnlocked) pllUnlocked--;
      I10=newI1;
      I20=newI2;
    }
    else // in the falling part of the cycle
    {
      timerCount=PLLTIMERMAX; // shift out of this region fast
      pllUnlocked=PLLLOCKCOUNT; // and we can't be locked
    }
    OCR1A=timerCount>>ADCSHIFT;
    
    oldV=newV;
    if(pllUnlocked==0) cdp->PLLlocked=true;
    else
		{
			cdp->PLLlocked=false;
			voltsOffset=OFFSETCENTRE; // re-centre voltage offset
		}
  }
  else if(samples==(NUMSAMPLES/2))
  {
    // negative zero crossing
#ifdef SYNCPIN
    digitalWrite(SYNCPIN,LOW);
#endif
    if(!pllUnlocked)
    {
      // update DC offsets, voltage should be 0 here, current equal and opposite to 0 sample
			fVoltsOffset+=newV;
			voltsOffset=fVoltsOffset>>OFFSETFILTERSHIFT;
      fI1Offset+=((I10+newI1)>>1);
			I1Offset=fI1Offset>>OFFSETFILTERSHIFT;
      fI2Offset+=((I20+newI2)>>1);
			I2Offset=fI2Offset>>OFFSETFILTERSHIFT;
    }
		else
		{
			fVoltsOffset=(long)voltsOffset<<OFFSETFILTERSHIFT;
		}
  }
  else if(samples==((NUMSAMPLES/2)-4)) // fire triacs ~1.6ms before -ve zero crossing
  {
    digitalWrite(LOAD1PIN,bitRead(PLL.divertFlags,LOAD1BIT));
    digitalWrite(LOAD2PIN,bitRead(PLL.divertFlags,LOAD2BIT));
  }
  else if(samples==(NUMSAMPLES-1))
  {
    if(sampleCount)
    {
      if(!sampling) sampling=true;
      else
      {
        sampleCount--;
        if(sampleCount==0) sampling=false;
      }
    }

    cdp->cycleV1squared=sumV1squared>>(ADCSHIFT<<1);
    cdp->cycleV2squared=sumV2squared>>(ADCSHIFT<<1);
    cdp->cycleI1squared=sumI1squared>>(ADCSHIFT<<1);
    cdp->cycleI2squared=sumI2squared>>(ADCSHIFT<<1);
    cdp->cycleP1=sumP1>>(ADCSHIFT<<1);
    cdp->cycleP2=sumP2>>(ADCSHIFT<<1);
    sumV1squared=0;
    sumV2squared=0;
    sumI1squared=0;
    sumI2squared=0;
    sumP1=0;
    sumP2=0;
    PLL.newCycle=true;
  }
  //digitalWrite(SAMPPIN,LOW);
}

// ADC interrupt handler
ISR(ADC_vect)
{
  static int newV,newI1,newI2,lastV,Vtime;
  int result,timerNow;
  long phaseShiftedV;
  
//  digitalWrite(SAMPPIN,HIGH);
	timerNow=TCNT1;
  result = ADCL<<ADCSHIFT;
  result |= ADCH<<(8+ADCSHIFT);

  // determine which conversion just completed
  switch(ADMUX & 0x0f)
  {
    case VOLTSPIN:
      ADMUX = _BV(REFS0) | CT1PIN;
      ADCSRA |= _BV(ADSC); // start next conversion
      lastV=newV;
      newV=result-voltsOffset;
      if(sampling) V[samples]+=newV;
      Vtime=timerNow;
      break;
    case CT1PIN:
      ADMUX = _BV(REFS0) | CT2PIN;
      ADCSRA |= _BV(ADSC);
      if(I1PhaseShift>=0) // current leads voltage, need a later voltage value
      {
        phaseShiftedV=lastV+((((long)newV-lastV)*I1PhaseShift)>>10);
        sumP1+=(phaseShiftedV*newI1);
        sumV1squared+=(phaseShiftedV*phaseShiftedV);
      }
      newI1=result-I1Offset;
      if(I1PhaseShift<0) // current lags voltage, need an earlier voltage value
      {
        phaseShiftedV=newV+((((long)newV-lastV)*I1PhaseShift)>>10);
        sumP1+=(phaseShiftedV*newI1);
        sumV1squared+=(phaseShiftedV*phaseShiftedV);
      }
      sumI1squared+=((long)newI1*newI1);
      if(sampling && !samplingI2)
			{
				I[samples]+=newI1;
				CTtotalDelay+=(timerNow-Vtime);
			}
      break;
    case CT2PIN:
      if(I2PhaseShift>=0)
      {
        phaseShiftedV=lastV+((((long)newV-lastV)*I2PhaseShift)>>10);
        sumP2+=(phaseShiftedV*newI2);
        sumV2squared+=(phaseShiftedV*phaseShiftedV);
      }
      newI2=result-I2Offset;
      if(I2PhaseShift<0)
      {
        phaseShiftedV=newV+((((long)newV-lastV)*I2PhaseShift)>>10);
        sumP2+=(phaseShiftedV*newI2);
        sumV2squared+=(phaseShiftedV*phaseShiftedV);
      }
      sumI2squared+=((long)newI2*newI2);
      if(sampling && samplingI2)
			{
				I[samples]+=newI2;
				CTtotalDelay+=(timerNow-Vtime);
			}
      updatePLL(newV,newI1,newI2);
      break;
  }
  //digitalWrite(SAMPPIN,LOW);
}

float calcPhaseShift(float *datap)
{
  float sumVcos=0,sumVsin=0;
  int i;
  
  for(i=0; i<NUMSAMPLES; i++)
  {
    sumVcos+=datap[i]*cos(i*(2*PI/NUMSAMPLES));
    sumVsin+=datap[i]*sin(i*(2*PI/NUMSAMPLES));
  }

  return atan(sumVcos/sumVsin);
}

int floatToInt(float f)
{
  if(f<0) f-=0.5;
  else f+=0.5;
  return(int)f;
}

boolean PLL50Hz::calculatePhaseShift(int numCycles, int *Ishift, boolean calculateI2shift)
{
  int i;
  float fIshift;
  
  *Ishift=0;
  if(pllUnlocked) return false;
  
  samplingI2=calculateI2shift;
  
  for( i=0; i<NUMSAMPLES; i++)
  {
    V[i]=0;
    I[i]=0;
  }
  sampleCount=numCycles;
  
  while(sampleCount && (pllUnlocked==0)) delay(1);
  if(pllUnlocked) return false;
  
  for(int i=0; i<NUMSAMPLES; i++)
  {
    V[i]=V[i]/numCycles;
    I[i]=I[i]/(numCycles*ADCMULT);
  }

  float fVshift=calcPhaseShift(V);
  fIshift=(calcPhaseShift(I)-fVshift)*160000.0/PI;
  *Ishift=floatToInt(fIshift - (calculateI2shift?CT2delay:CT1delay));
  return abs(fIshift)<=TIMERTOP;
}

void PLL50Hz::updatePhaseShift(int newIshift, boolean updateI2Shift)
{
	if(updateI2Shift) I2PhaseShift=floatToInt((((float)newIshift+CT2delay)*64)/TIMERPERIOD);
  else I1PhaseShift=floatToInt((((float)newIshift+CT1delay)*64)/TIMERPERIOD);
}
