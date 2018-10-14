/****************************************************************************************************
  PumpDuino.cpp

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

#include "PumpDuino.h"

/**
 * Constructeur de l objet PumpDuino 
 */
PumpDuino::PumpDuino(void)
{

}

/**
 * Destructeur de l objet PumpDuino
 */
PumpDuino::~PumpDuino(void)
{
  delete &(this->motor);
}

/**
 * Getter pour récupérer l'adresse de sauvegarde de l'état de la pompe
 *
 * \param void
 */
int PumpDuino::getStateAddress(void)
{
  return motor.getStateAddress();
}

/**
 * Getter pour récupérer l'adresse de sauvegarde du volume par pas de la pompe
 *
 * \return int
 */
int PumpDuino::getVolPerStepAddress(void)
{
  return this->volPerStepAddress;
}

/**
 * Getter pour récupérer l'adresse de sauvegarde du débit de la pompe
 *
 * \return int
 */
int PumpDuino::getDebitAddress(void)
{
  return this->debitAddress;
}

/**
 * Établir une valeur de volume par pas
 *
 * \param float volume : volume par pas
 */
void PumpDuino::setVolumePerStep(float volume)
{
  volumePerStep = volume;
}

/**
 * Methode pour faire tourner le moteur en fonction du volume donne
 *
 * \param int waterVolume : volume d'eau voulu
 */
void PumpDuino::sendWaterVolume(int waterVolume)
{
  int steps = (int)roundf((waterVolume/volumePerStep)*motor.stepsPerRound);
  motor.motorTurnSteps(steps);
}

/**
 * Methode pour calculer le debit en L/h
 *
 * \param float waterVolumePerHour : volume du debit d eau en litre par heure
 */
void PumpDuino::sendWaterVolumePerHour(float waterVolumePerHour)
{  
  unsigned long int steps_value = (unsigned long int)roundf(((1000000*waterVolumePerHour)/(volumePerStep/1000)));   
  unsigned int valueStepSec = steps_value/3600;
  unsigned int nbStepRest = steps_value%3600;

  if(nbStepRest>1800)
  {
    valueStepSec++;
  }

  if(valueStepSec>0)
  {
    motor.motorTurnStepsPerSecond(valueStepSec);
  }
  else
  {
    //informer l'utilisateur qu'il a saisi un débit trop faible
  }

  //afficher le volume/heure effectif que le système va réaliser: ((float)(a*3600)/(float)(motor.stepsPerRound))*(volumePerStep/1000)
}

/**
 * Methode pour mettre en marche la pompe à eau
 */
void PumpDuino::sendWater(void)
{
  sendWaterVolumePerHour(*debitWater);
}

/**
 * Methode pour mettre en marche le moteur de la pompe
 */
void PumpDuino::sendCO2(void){
  motor.motorTurn();
}

/**
 * Calcul de la vitesse de rotation du moteur
 *
 * \param float debitWater : debit d eau voulu en sortie de la pompe
 */
void PumpDuino::calculateMotorSpeed(float debitWater)
{
  int speed = 600;
  motor.setSpeed(speed);
}

/**
 *
 */
void PumpDuino::setDbtWater(float debitWaterValue)
{
  *debitWater = debitWaterValue;
}

/**
 * Setter pour établir l'adresse de sauvegarde de l'état de la pompe
 *
 * \param int stateAddress
 */
void PumpDuino::setStateAddress(int stateAddress)
{
  motor.setStateAddress(stateAddress);
}

/**
 * Setter pour établir l'adresse de sauvegarde du volume par pas de la pompe
 *
 * \param int volPerStepAddress
 */
void PumpDuino::setVolPerStepAddress(int volPerStepAddress)
{
  this->volPerStepAddress = volPerStepAddress;
}

/**
 * Setter pour établir l'adresse de sauvegarde du debit de la pompe
 *
 * \param int debitAddress
 */
void PumpDuino::setDebitAddress(int debitAddress)
{
  this->debitAddress = debitAddress;
}

/**
 * Methode pour arreter le moteur de la pompe
 */
void PumpDuino::stop(void)
{
  motor.motorStop();
}

/**
 * Sauvegarde de l etat de la pompe dans la memoire
 * de la carte
 *
 * \param bool state : etat de la pompe a sauvegarder 
 */
void PumpDuino::saveState(bool state){
  motor.saveState(state);
}

/**
 * Sauvegarde de la valeur de volumePerStep dans la memoire
 * de la carte
 *
 * \param float volume : valeur de volumePerStep a sauvegarder
 */
void PumpDuino::saveVolumePerStep(float volume)
{
  EEPROM.put(EEPROM_VOLUME_PER_STEP, volume);
}

/**
 * Sauvegarde de la valeur de debitWater dans la memoire
 * de la carte
 *
 * \param float volume : valeur de debitWater a sauvegarder
 */
void PumpDuino::saveDebitWater(float dbtWater)
{
  EEPROM.put(EEPROM_DBT_WATER, dbtWater);
}

/**
 * Verifier si la pompe est allumee
 *
 * \return bool co2IsOn : true la pompe est allumee
 *						  false la pompe est eteinte
 */
bool PumpDuino::isOn(void)
{
  bool isOn = (readPinTimerState(motor.pinStep))? true:false;
  return isOn;
}