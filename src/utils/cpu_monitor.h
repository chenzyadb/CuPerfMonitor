#pragma once

#include "cu_misc.h"

class CpuMonitor {
    public:
        CpuMonitor();
        ~CpuMonitor();

        std::vector<int> GetCpuCurFreq();
        std::vector<int> GetCpuUsage();
        int GetCpuTemp();

    private:
        std::string cpuThermalZonePath_;
};