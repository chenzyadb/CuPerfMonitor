#include "cpu_monitor.h"

CpuMonitor::CpuMonitor() 
{ 
    constexpr char cpuThermalRegex[] = "^(cpu|soc|msm)|(cpu|soc)$";
	DIR* dir = opendir("/sys/class/thermal");
	if (dir) {
		struct dirent* entry = nullptr;
		while ((entry = readdir(dir)) != nullptr) {
			if (strstr(entry->d_name, "thermal_zone")) {
				if (CheckRegex(ReadFile(StrMerge("/sys/class/thermal/%s/type", entry->d_name)).c_str(), cpuThermalRegex)) {
					cpuThermalZonePath_ = StrMerge("/sys/class/thermal/%s/temp", entry->d_name);
					break;
				}
			}
		}
		closedir(dir);
	}
}

CpuMonitor::~CpuMonitor() { }

std::vector<int> CpuMonitor::GetCpuCurFreq()
{
	std::vector<int> cpuCurFreqs{};
	for (int cpuCore = 0; cpuCore < 10; cpuCore++) {
		std::string curFreqPath = StrMerge("/sys/devices/system/cpu/cpu%d/cpufreq/scaling_cur_freq", cpuCore);
		if (IsPathExist(curFreqPath)) {
			int cpuCurFreq = StringToInteger(ReadFile(curFreqPath));
			cpuCurFreqs.emplace_back(cpuCurFreq / 1000);
		}
	}

	return cpuCurFreqs;
}

std::vector<int> CpuMonitor::GetCpuUsage()
{
	static uint64_t prevSumtime[10];
	static uint64_t prevRuntime[10];

	std::vector<int> cpuUsages{};
	std::vector<std::string> statLines = StrSplit(ReadFile("/proc/stat"), "\n");
	for (const std::string &line : statLines) {
		if (CheckRegex(line, "^cpu[0-9]")) {
			unsigned long long user, nice, sys, idle, iowait, irq, softirq;
			int curCpuCore = -1;
			sscanf(line.c_str(), "cpu%d %llu %llu %llu %llu %llu %llu %llu", &curCpuCore, &user, &nice, &sys, &idle, &iowait, &irq, &softirq);
			uint64_t nowaSumtime = user + nice + sys + idle + iowait + irq + softirq;
			uint64_t nowaRuntime = nowaSumtime - idle;
			int cpuUsage = (nowaRuntime - prevRuntime[curCpuCore]) * 100 / (nowaSumtime - prevSumtime[curCpuCore]);
			cpuUsages.emplace_back(cpuUsage);
			prevSumtime[curCpuCore] = nowaSumtime;
			prevRuntime[curCpuCore] = nowaRuntime;
		}
	}

	return cpuUsages;
}

int CpuMonitor::GetCpuTemp()
{
	int cpuTemp = StringToInteger(ReadFile(cpuThermalZonePath_));
	if (cpuTemp > 1000) {
		cpuTemp = cpuTemp / 1000;
	}

	return cpuTemp;
}
