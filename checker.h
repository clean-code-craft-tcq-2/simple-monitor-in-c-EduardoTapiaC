#ifndef CHECKER_H
#define CHECKER_H

typedef struct BatteryModel{
  float maxChargingTemperature;
  float minChargingTemperature;
  float maxChargePercentage;
  float minChargePercentage;
  float maxChargeRate;
 
  /* TODO: operative temperature
  float maxDischargingTemperature;
  float minDischargingTemperature;
  */

}BatteryModel;

enum batteryState{BATTERY_OK, OVERRCURRENT = 1, OVERVOLTAGE = 2, UNDERVOLTAGE = 4, UNDERCHARGE = 8, OVERCHARGE = 16, OVERHEATING = 32, FREEZING = 64, SHORT = 128};
enum batteryChargingState{DISCHARGING, CHARGING};

typedef struct BMS{
  BatteryModel batteryT;
  float soc;
  float temperature;  //thermal control: hot = faster discharge
  float chargeRate; // charging, discharging
  unsigned int battStat;    //battery  status flags. see batteryState enum
  enum batteryChargingState chargingState;
  // TODO: float CellVoltage[TOTAL_CELLS]; //Cell balancing
  // TODO: float maxTravelRecord[100];     // SOH
}BMS;

// check battery functions return 0 for OKAY, any other value means battery issues.
int checkBatteryChargingTemperature(BMS* bms);
int checkBatterySOC(BMS* bms);
int checkBatteryChargeRate(BMS* bms);

int batteryIsOk(BMS* bms);
void resetBatteryState(BMS* bms);
void printBatteryStat(BMS* bms);

#endif