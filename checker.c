#include <stdio.h>
#include "checker.h"
#include "utils.h"

#define LANGUAGE_ENGLISH 1
#define LANGUAGE_GERMAN 2
#define DISPLAY_LANGUAGE LANGUAGE_GERMAN

#if (DISPLAY_LANGUAGE == LANGUAGE_ENGLISH)
const char* defaultBatStat[] = {"BATTERY_OK", "OVERRCURRENT", "OVERVOLTAGE" , "UNDERVOLTAGE" , "UNDERCHARGE", "OVERCHARGE" , "OVERHEATING", "FREEZING", "SHORT"};
#endif

#if (DISPLAY_LANGUAGE == LANGUAGE_GERMAN)
#include "german.h"

#endif

int numberBatStats = sizeof(defaultBatStat) / sizeof(defaultBatStat[0]);


int checkBatteryChargingTemperature(BMS* bms) {
  CLEAR_BITS(&bms->battStat, FREEZING|OVERHEATING);
  if (bms->temperature < bms->batteryT.minChargingTemperature) {
    CLEAR_BITS(&bms->battStat, OVERHEATING);
    bms->battStat |= FREEZING;
  } else if (bms->temperature > bms->batteryT.maxChargingTemperature){
    CLEAR_BITS(&bms->battStat, FREEZING);
    bms->battStat |= OVERHEATING;
  }
  return bms->battStat & (OVERHEATING|FREEZING);
}

int checkBatterySOC(BMS* bms) {
  CLEAR_BITS(&bms->battStat, UNDERCHARGE|OVERCHARGE);
  if (bms->soc < bms->batteryT.minChargePercentage) {
    CLEAR_BITS(&bms->battStat, OVERCHARGE);
    bms->battStat |= UNDERCHARGE;
  } else if (bms->soc > bms->batteryT.maxChargePercentage){
    CLEAR_BITS(&bms->battStat, UNDERCHARGE);
    bms->battStat |= OVERCHARGE;
  } 

  
  return bms->battStat & (UNDERCHARGE|OVERCHARGE);
}

int checkBatteryChargeRate(BMS* bms) {
  if (bms->chargeRate > bms->batteryT.maxChargeRate){
    bms->battStat |= OVERVOLTAGE;
  } else{
    CLEAR_BITS(&bms->battStat, OVERVOLTAGE);
  }
  return bms->battStat & (OVERVOLTAGE);
}

int batteryIsOk(BMS* bms) {
  checkBatteryChargeRate(bms);
  checkBatteryChargingTemperature(bms);
  checkBatterySOC(bms);
  if(bms->battStat == BATTERY_OK) {
    printf("STATUS: %s\n", defaultBatStat[0]);
  } else {
      printBatteryStat(bms);
  }

  return bms->battStat;
}

void resetBatteryState(BMS* bms) {
  bms->battStat = 0;
}

void printBatteryStat(BMS* bms) {

  printf("STATUS:");
  for (int i = 1; i < numberBatStats; i++)
  {
    if(bms->battStat & 1 << i) {
      printf("%s ", defaultBatStat[i]);
    }
  }
  printf("\n");

  
}