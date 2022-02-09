#include <stdio.h>
#include <assert.h>

enum batteryState{BATTERY_OK, OVERRCURRENT = 1, OVERVOLTAGE = 2, UNDERVOLTAGE = 4, UNDERCHARGE = 8, OVERCHARGE = 16, OVERHEATING = 32, FREEZING = 64, SHORT = 128};

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

typedef struct BMS{
  BatteryModel batteryT;
  float soc;
  float temperature;  //thermal control: hot = faster discharge
  float chargeRate; // chargin, discharging
  enum batteryState state;
  // TODO: float CellVoltage[TOTAL_CELLS]; //Cell balancing
  // TODO: float maxTravelRecord[100];     // SOH
}BMS;



int checkBatteryChargingTemperature(BMS batterySystem){
  if (batterySystem.temperature < batterySystem.batteryT.minChargingTemperature) {
    batterySystem.state = FREEZING;
  } else if (batterySystem.temperature > batterySystem.batteryT.maxChargingTemperature){
    batterySystem.state = OVERHEATING;
  }
  return batterySystem.state;
}

int checkBatterySOC(BMS batterySystem){
  if (batterySystem.soc < batterySystem.batteryT.minChargePercentage) {
    batterySystem.state = UNDERCHARGE;
  } else if (batterySystem.soc > batterySystem.batteryT.maxChargePercentage){
    batterySystem.state = OVERCHARGE;
  }
  return batterySystem.state;
}

int checkBatteryChargeRate(BMS batterySystem){
  if (batterySystem.chargeRate > batterySystem.batteryT.maxChargeRate){
    batterySystem.state = OVERVOLTAGE;
  }
  return batterySystem.state;
}

int batteryIsOk(BMS* batterySystem) {
  batterySystem->state = checkBatteryChargeRate(*batterySystem);
  batterySystem->state = checkBatteryChargingTemperature(*batterySystem);
  batterySystem->state = checkBatterySOC(*batterySystem);
  printf("Battery state: %d\n", batterySystem->state);
  return batterySystem->state;
}


int main() {
  BatteryModel liion = {45.0, 0.0, 80.0, 20.0, 0.8};
  BMS manageBattery= {liion, 70, 25, 0.7, BATTERY_OK};
  assert(batteryIsOk(&manageBattery) == BATTERY_OK);
  manageBattery.temperature = -5;
  assert(batteryIsOk(&manageBattery) == FREEZING);
  manageBattery.temperature = 60;
  assert(batteryIsOk(&manageBattery) == OVERHEATING); 
  manageBattery.temperature = 25;
  manageBattery.soc = 100;
  assert(batteryIsOk(&manageBattery) == OVERCHARGE); 
  manageBattery.soc = 10;
  assert(batteryIsOk(&manageBattery) == UNDERCHARGE); 
  manageBattery.soc = 70;
  manageBattery.chargeRate = 1.1;
  assert(batteryIsOk(&manageBattery) == OVERVOLTAGE); 
}
