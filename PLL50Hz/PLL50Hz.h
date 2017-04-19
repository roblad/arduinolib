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

#ifndef PLL50Hz_h
#define PLL50Hz_h

#include "Arduino.h"

#define NUMSAMPLES 50 // number of times to sample each 50Hz cycle
#define LOAD1BIT 0 // divertFlags bit for load 1
#define LOAD2BIT 1 // divertFlags bit for load 2
#define VOLTSPIN 2
#define CT1PIN 3
#define CT2PIN 0
#define LOAD1PIN 3
#define LOAD2PIN 15
#define SYNCPIN 6 // this output will be a 50Hz square wave locked to the 50Hz input

typedef struct
{
  long cycleV1squared,cycleV2squared,cycleI1squared,cycleI2squared,cycleP1,cycleP2;
  boolean PLLlocked;
  int data;
} CycleData;

class PLL50Hz
{
	public:
		void begin(CycleData *pcd);
		boolean calculatePhaseShift(int numCycles, int *Ishift, boolean calculateI2shift);
		void updatePhaseShift(int newIshift, boolean updateI2Shift);

		boolean newCycle;
		byte divertFlags;
	private:
};

extern PLL50Hz PLL;

#endif



