#include "ddr_monitor.h"

DdrMonitor::DdrMonitor() { }

DdrMonitor::~DdrMonitor() { }

int DdrMonitor::GetDdrCurFreq() 
{
    uint64_t ddrFreq = 0;

    if (IsPathExist("/sys/class/devfreq/ddrfreq/cur_freq")) { // Kirin
        ddrFreq = StringToLong(ReadFile("/sys/class/devfreq/ddrfreq/cur_freq"));
    } else if (IsPathExist("/sys/devices/platform/10012000.dvfsrc/helio-dvfsrc/dvfsrc_dump")) { // MediaTek
        std::string dumpStr = ReadFile("/sys/devices/platform/10012000.dvfsrc/helio-dvfsrc/dvfsrc_dump");
        std::vector<std::string> lines = StrSplit(dumpStr, "\n");
        for (const std::string &line : lines) {
            if (StrContains(line, "khz")) {
                // DDR    : 1866000  khz
                ddrFreq = StringToLong(GetPrevString(GetPostString(line, ':'), 'k'));
                break;
            }
        }
    }

    if (ddrFreq > 10000000) {
		ddrFreq = ddrFreq / 1000000;
	} else if (ddrFreq > 10000) {
		ddrFreq = ddrFreq / 1000;
	}

    return (int)ddrFreq;
}
