#pragma once

#include "cu_misc.h"

class GpuMonitor {
    public:
        GpuMonitor();
        ~GpuMonitor();

        int GetGpuCurFreq();
        int GetGpuUsage();
};