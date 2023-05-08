#include "battery_monitor.h"

BatteryMonitor::BatteryMonitor() {
    batteryPath_ = "/sys/class/power_supply/battery";
    if (IsPathExist("/sys/class/power_supply/Battery")) {
        batteryPath_ = "/sys/class/power_supply/Battery";
    }
}

BatteryMonitor::~BatteryMonitor() { }

int BatteryMonitor::GetBatteryPower() 
{
    int batteryPower = 0;      // mW.
    int batteryCurrent = 0;    // mA.
	int batteryVoltage = 0;    // mV.

    batteryCurrent = StringToInteger(ReadFile(StrMerge("%s/current_now", batteryPath_.c_str())));
    if (batteryCurrent > 10000) {
        batteryCurrent = batteryCurrent / 1000;
    }

    batteryVoltage = StringToInteger(ReadFile(StrMerge("%s/voltage_now", batteryPath_.c_str())));
    if (batteryVoltage > 10000) {
        batteryVoltage = batteryVoltage / 1000;
    }

    batteryPower = batteryCurrent * batteryVoltage / 1000;

    return batteryPower;
}

int BatteryMonitor::GetBatteryTemp() 
{
    int batteryTemp = 0;    // °C.

    batteryTemp = StringToInteger(ReadFile(StrMerge("%s/temp", batteryPath_.c_str())));
    if (batteryTemp > 100) {
        batteryTemp = batteryTemp / 10;
    }

    return batteryTemp;
}

int BatteryMonitor::GetBatteryPercent()
{
    int batteryPercent = StringToInteger(ReadFile(StrMerge("%s/capacity", batteryPath_.c_str())));

    return batteryPercent;
}
