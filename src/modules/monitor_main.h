#pragma once

#include "platform/module.h"
#include "utils/fps_analyzer.h"
#include "utils/cpu_monitor.h"
#include "utils/gpu_monitor.h"
#include "utils/ddr_monitor.h"
#include "utils/battery_monitor.h"
#include "utils/ram_monitor.h"
#include "utils/cu_misc.h"
#include "utils/JsonObject.h"
#include "utils/cu_csv.h"
#include "utils/CuLogger.h"

class MonitorMain : public Module {
    public:
        MonitorMain(const JsonObject &config, const std::string &outputPath);
        ~MonitorMain();

        void Start();

    private:
        CuCsv cuCsv_;
        JsonObject config_;
        std::string outputPath_;
        int reflashIntervalMs_;
        int saveIntervalMs_;
        CuCsv::Csv csv_;
        struct monitor_item {
            bool timeMs;
            bool appPkgName;
            bool cpuCurFreq;
            bool cpuUsage;
            bool cpuTemp;
            bool gpuCurFreq;
            bool gpuUsage;
            bool ddrCurFreq;
            bool batteryPower;
            bool batteryTemp;
            bool batteryPercent;
            bool fps;
            bool jank;
            bool bigJank;
            bool maxFrameTime;
            bool ramFree;
        } monitorItems_;
        std::string topAppPkgName_;

        void Init();
        void Monitor();
        void Saver();
        void TopAppChanged(const void* data);
        void ScreenStateChanged(const void* data);
};