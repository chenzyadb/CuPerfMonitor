#include "ram_monitor.h"

RamMonitor::RamMonitor() { }

RamMonitor::~RamMonitor() { }

int RamMonitor::GetRamFree()
{
    int ramFree = 0;    // MB.

    std::string meminfo = ReadFile("/proc/meminfo");
    std::vector<std::string> lines = StrSplit(meminfo, "\n");
    for (const std::string &line : lines) {
        // MemTotal:        3809036 kB
		// MemFree:          282012 kB
		// MemAvailable:     865620 kB
        if (StrContains(line, "MemAvailable:")) {
            int memAvailable = 0;
            sscanf(line.c_str(), "MemAvailable: %d kB", &memAvailable);
            ramFree = memAvailable / 1024;
            break;
        }
    }

    return ramFree;
}