/****************************************************************************************************
  PumpDuino.h

  author : Conrad Djedjebi
  version v1.0
  License: GNU AGPLv3

  A pump library for Arduino (peristaltic pumps, etc.)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Affero General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Affero General Public License for more details.

  You should have received a copy of the GNU Affero General Public
  License along with this library; If not, see <http://www.gnu.org/licenses/>.

Version Modified By Date     Comments
------- ----------- -------- --------
0001    C DJEDJEBI    xx/xx/xx ..
****************************************************************************************************/

#ifndef PumpDuino_H_
#define PumpDuino_H_

#include <Arduino.h>
#include <string.h>
#include <math.h>
#include <Macros.h>
#include <MotorDuino.h>

class PumpDuino 
{
	public:
		PumpDuino(void);
		~PumpDuino(void);		
		/**
		 * Methods
		 */
		int getStateAddress();	
		int getVolPerStepAddress();
		int getDebitAddress();		
		void setVolumePerStep(float volume);
		void setDbtWater(float debitWaterValue);
		void setStateAddress(int stateAddress); 
		void setVolPerStepAddress(int volPerStepAddress); 
		void setDebitAddress(int debitAddress); 				
		void sendWater(void);
		void sendWaterVolume(int waterVolume);		
		void sendWaterVolumePerHour(float waterVolumePerHour);		
		void sendCO2(void);			
		void saveState(bool state);
		void saveVolumePerStep(float volume);	
		void saveDebitWater(float dbtWater);
		void calculateMotorSpeed(float debitWater);
		void stop(void);
 		bool isOn(void);			
		/**
		 * Attributs
		 */
		MotorDuino motor;
		float debitWater_value;		 
		float* debitWater = &debitWater_value;
		float volumePerStep = 120.0;
		int volPerStepAddress;
		int debitAddress;	
};

#endif
