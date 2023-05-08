#pragma once

#include "cu_misc.h"

class RamMonitor {
    public:
        RamMonitor();
        ~RamMonitor();

        int GetRamFree();
};