#pragma once

#include "cu_misc.h"

class BatteryMonitor {
    public:
        BatteryMonitor();
        ~BatteryMonitor();

        int GetBatteryPower();
        int GetBatteryTemp();
        int GetBatteryPercent();

    private:
        std::string batteryPath_;
};