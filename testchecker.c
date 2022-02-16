#include <stdio.h>
#include <assert.h>
#include "checker.h"

void testBatteryOk(){
  BatteryModel liion = {45.0, 0.0, 80.0, 20.0, 0.8};
  BMS manageBattery= {liion, 70, 25, 0.7, 0, DISCHARGING};
  assert(batteryIsOk(&manageBattery) == BATTERY_OK);
}

void testFreezing() {
  BatteryModel liion = {45.0, 0.0, 80.0, 20.0, 0.8};
  BMS manageBattery= {liion, 70, 25, 0.7, 0, DISCHARGING};
  manageBattery.temperature = -5;
  assert(batteryIsOk(&manageBattery) == FREEZING);
}

void testBatteryOverheating(){
  BatteryModel liion = {45.0, 0.0, 80.0, 20.0, 0.8};
  BMS manageBattery= {liion, 70, 25, 0.7, 0, DISCHARGING};
  manageBattery.temperature = 60;
  assert(batteryIsOk(&manageBattery) == OVERHEATING); 
}

void testBatteryOvercharge() {
  BatteryModel liion = {45.0, 0.0, 80.0, 20.0, 0.8};
  BMS manageBattery= {liion, 70, 25, 0.7, 0, DISCHARGING};
  manageBattery.soc = 100;
  assert(batteryIsOk(&manageBattery) == (OVERCHARGE)); 
}

void testBatteryUndercharge() {
  BatteryModel liion = {45.0, 0.0, 80.0, 20.0, 0.8};
  BMS manageBattery= {liion, 70, 25, 0.7, 0, DISCHARGING};
  manageBattery.soc = 10;
  assert(batteryIsOk(&manageBattery) == UNDERCHARGE);
}

void testBatteryOvervoltage() {
  BatteryModel liion = {45.0, 0.0, 80.0, 20.0, 0.8};
  BMS manageBattery= {liion, 70, 25, 0.7, 0, DISCHARGING};
  manageBattery.chargeRate = 1.1;
  assert(batteryIsOk(&manageBattery) == OVERVOLTAGE); 
}

void testMultipleFailures() {
  BatteryModel liion = {45.0, 0.0, 80.0, 20.0, 0.8};
  BMS manageBattery= {liion, 70, 25, 0.7, 0, DISCHARGING};
  manageBattery.soc = 100;
  manageBattery.temperature = 60;
  assert(batteryIsOk(&manageBattery) == (OVERCHARGE|OVERHEATING)); 
}


int main() {
  testBatteryOk();
  testFreezing();
  testBatteryOverheating();
  testBatteryOvercharge();
  testBatteryUndercharge();
  testBatteryOvervoltage();
  testMultipleFailures();

}
