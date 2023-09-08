#include "ddr_monitor.h"

DdrMonitor::DdrMonitor() { }

DdrMonitor::~DdrMonitor() { }

int DdrMonitor::GetDdrCurFreq() 
{
    int ddrFreq = 0;
    if (IsPathExist("/sys/class/devfreq/ddrfreq/cur_freq")) { // Kirin
        auto ddrFreqHz = StringToLong(ReadFile("/sys/class/devfreq/ddrfreq/cur_freq")) / 1000000;
        ddrFreq = static_cast<int>(ddrFreqHz);
    } else if (IsPathExist("/sys/devices/platform/10012000.dvfsrc/helio-dvfsrc/dvfsrc_dump")) { // MediaTek
        auto lines = StrSplit(ReadFile("/sys/devices/platform/10012000.dvfsrc/helio-dvfsrc/dvfsrc_dump"), "\n");
        for (const auto &line : lines) {
            if (StrContains(line, "khz")) {
                // DDR    : 1866000  khz
                ddrFreq = StringToInteger(GetPrevString(GetPostString(line, ':'), 'k')) / 1000;
                break;
            }
        }
    } else if (IsPathExist("/sys/devices/system/cpu/bus_dcvs/DDR/cur_freq")) { // Qualcomm 
        ddrFreq = StringToInteger(ReadFile("/sys/devices/system/cpu/bus_dcvs/DDR/cur_freq")) / 1000;
    }

    return ddrFreq;
}
