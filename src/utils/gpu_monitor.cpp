#include "gpu_monitor.h"

GpuMonitor::GpuMonitor() { }

GpuMonitor::~GpuMonitor() { }

int GpuMonitor::GetGpuCurFreq() 
{
    uint64_t gpuFreq = 0;

	if (IsPathExist("/sys/kernel/gpu/gpu_clock")) { // Linux Default
		gpuFreq = StringToLong(ReadFile("/sys/kernel/gpu/gpu_clock"));
	} else if (IsPathExist("/sys/class/kgsl/kgsl-3d0/clock_mhz")) { // Qualcomm (MHz)
		gpuFreq = StringToInteger(ReadFile("/sys/class/kgsl/kgsl-3d0/clock_mhz"));
	} else if (IsPathExist("/sys/class/kgsl/kgsl-3d0/devfreq/cur_freq")) { // Qualcomm
		gpuFreq = StringToLong(ReadFile("/sys/class/kgsl/kgsl-3d0/devfreq/cur_freq"));
	} else if (IsPathExist("/sys/class/devfreq/gpufreq/cur_freq")) { // Kirin & Unisoc
		gpuFreq = StringToLong(ReadFile("/sys/class/devfreq/gpufreq/cur_freq"));
	} else if (IsPathExist("/sys/kernel/debug/ged/hal/current_freqency")) { // Old MediaTek
		sscanf(ReadFile("/sys/kernel/debug/ged/hal/current_freqency").c_str(), "%*d %ld", &gpuFreq);
	} else if (IsPathExist("/sys/kernel/ged/hal/current_freqency")) { // New MediaTek
		sscanf(ReadFile("/sys/kernel/ged/hal/current_freqency").c_str(), "%*d %ld", &gpuFreq);
	} else if (IsPathExist("/sys/class/devfreq/13000000.mali/cur_freq")) { // Mali Default
		gpuFreq = StringToLong(ReadFile("/sys/class/devfreq/13000000.mali/cur_freq"));
	} else if (IsPathExist("/sys/class/devfreq/60000000.gpu/device/devfreq/60000000.gpu/cur_freq")) { // New Unisoc
		gpuFreq = StringToLong(ReadFile("/sys/class/devfreq/60000000.gpu/device/devfreq/60000000.gpu/cur_freq"));
	}

	if (gpuFreq > 10000000) {
		gpuFreq = gpuFreq / 1000000;
	} else if (gpuFreq > 10000) {
		gpuFreq = gpuFreq / 1000;
	}

    return (int)gpuFreq;
}

int GpuMonitor::GetGpuUsage()
{
    int gpuUsage = 0;

	if (IsPathExist("/sys/kernel/gpu/gpu_busy")) { // Linux Default
		sscanf(ReadFile("/sys/kernel/gpu/gpu_busy").c_str(), "%d %%", &gpuUsage);
	} else if (IsPathExist("/sys/class/kgsl/kgsl-3d0/gpu_busy_percentage")) { // Qualcomm
		sscanf(ReadFile("/sys/class/kgsl/kgsl-3d0/gpu_busy_percentage").c_str(), "%d %%", &gpuUsage);
	} else if (IsPathExist("/sys/class/kgsl/kgsl-3d0/devfreq/gpu_load")) { // Qualcomm
		gpuUsage = StringToInteger(ReadFile("/sys/class/kgsl/kgsl-3d0/devfreq/gpu_load"));
	} else if (IsPathExist("/sys/class/devfreq/gpufreq/mali_ondemand/utilisation")) { // Kirin
		gpuUsage = StringToInteger(ReadFile("/sys/class/devfreq/gpufreq/mali_ondemand/utilisation"));
	} else if (IsPathExist("/sys/kernel/debug/ged/hal/gpu_utilization")) { // Old MediaTek
		sscanf(ReadFile("/sys/kernel/debug/ged/hal/gpu_utilization").c_str(), "%d %*d %*d", &gpuUsage);
	} else if (IsPathExist("/sys/kernel/ged/hal/gpu_utilization")) { // New MediaTek
		sscanf(ReadFile("/sys/kernel/ged/hal/gpu_utilization").c_str(), "%d %*d %*d", &gpuUsage);
	} else if (IsPathExist("/sys/module/ged/parameters/gpu_loading")) { // MediaTek Ged
		gpuUsage = StringToInteger(ReadFile("/sys/module/ged/parameters/gpu_loading"));
	} 

    return gpuUsage;
}
