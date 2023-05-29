#include "monitor_main.h"

MonitorMain::MonitorMain(const JsonObject &config, const std::string &outputPath)
{
    config_ = config;
    outputPath_ = outputPath;
}

MonitorMain::~MonitorMain() { }

void MonitorMain::Start()
{
    Init();

    monitorThread_ = std::thread(std::bind(&MonitorMain::Monitor, this));
    monitorThread_.detach();
    saverThread_ = std::thread(std::bind(&MonitorMain::Saver, this));
    saverThread_.detach();
    topAppPkgName_ = "null";

    const auto &logger = CuLogger::GetLogger();
    logger->Info("MonitorMain started successfully.");

    using namespace std::placeholders;
    Broadcast_SetBroadcastReceiver("TopAppMonitor.TopAppChanged", std::bind(&MonitorMain::TopAppChanged, this, _1));
    Broadcast_SetBroadcastReceiver("CgroupWatcher.ScreenStateChanged", std::bind(&MonitorMain::ScreenStateChanged, this, _1));
} 

void MonitorMain::Init() 
{
    const auto &logger = CuLogger::GetLogger();

    reflashIntervalMs_ = config_.GetValueInt("reflashIntervalMs");
    saveIntervalMs_ = config_.GetValueInt("saveIntervalMs");

    JsonObject monitorItems = config_.GetValueJson("monitorItems");
    monitorItems_.timeMs = monitorItems.GetValueBoolean("timeMs");
    monitorItems_.appPkgName = monitorItems.GetValueBoolean("appPkgName");
    monitorItems_.cpuCurFreq = monitorItems.GetValueBoolean("cpuCurFreq");
    monitorItems_.cpuUsage = monitorItems.GetValueBoolean("cpuUsage");
    monitorItems_.cpuTemp = monitorItems.GetValueBoolean("cpuTemp");
    monitorItems_.gpuCurFreq = monitorItems.GetValueBoolean("gpuCurFreq");
    monitorItems_.gpuUsage = monitorItems.GetValueBoolean("gpuUsage");
    monitorItems_.ddrCurFreq = monitorItems.GetValueBoolean("ddrCurFreq");
    monitorItems_.batteryPower = monitorItems.GetValueBoolean("batteryPower");
    monitorItems_.batteryTemp = monitorItems.GetValueBoolean("batteryTemp");
    monitorItems_.batteryPercent = monitorItems.GetValueBoolean("batteryPercent");
    monitorItems_.fps = monitorItems.GetValueBoolean("fps");
    monitorItems_.jank = monitorItems.GetValueBoolean("jank");
    monitorItems_.bigJank = monitorItems.GetValueBoolean("bigJank");
    monitorItems_.maxFrameTime = monitorItems.GetValueBoolean("maxFrameTime");
    monitorItems_.ramFree = monitorItems.GetValueBoolean("ramFree");

    std::vector<std::string> columnNames{};
    if (monitorItems_.timeMs) {
        columnNames.emplace_back("timeMs");
    }
    if (monitorItems_.appPkgName) {
        columnNames.emplace_back("appPkgName");
    }
    if (monitorItems_.cpuCurFreq) {
        columnNames.emplace_back("cpuCurFreq");
    }
    if (monitorItems_.cpuUsage) {
        columnNames.emplace_back("cpuUsage");
    }
    if (monitorItems_.cpuTemp) {
        columnNames.emplace_back("cpuTemp");
    }
    if (monitorItems_.gpuCurFreq) {
        columnNames.emplace_back("gpuCurFreq");
    }
    if (monitorItems_.gpuUsage) {
        columnNames.emplace_back("gpuUsage");
    }
    if (monitorItems_.ddrCurFreq) {
        columnNames.emplace_back("ddrCurFreq");
    }
    if (monitorItems_.batteryPower) {
        columnNames.emplace_back("batteryPower");
    }
    if (monitorItems_.batteryTemp) {
        columnNames.emplace_back("batteryTemp");
    }
    if (monitorItems_.batteryPercent) {
        columnNames.emplace_back("batteryPercent");
    }
    if (monitorItems_.fps) {
        columnNames.emplace_back("fps");
    }
    if (monitorItems_.jank) {
        columnNames.emplace_back("jank");
    }
    if (monitorItems_.bigJank) {
        columnNames.emplace_back("bigJank");
    }
    if (monitorItems_.maxFrameTime) {
        columnNames.emplace_back("maxFrameTime");
    }
    if (monitorItems_.ramFree) {
        columnNames.emplace_back("ramFree");
    }
    csv_ = cuCsv.CreateCsv(columnNames);

    std::string enabledMonitorItems = "";
    for (const std::string &columnName : columnNames) {
        enabledMonitorItems += columnName + " ";
    }
    logger->Info("Enabled MonitorItems: %s.", enabledMonitorItems.c_str());

    CreateFile(outputPath_, "");
}

void MonitorMain::Monitor()
{
    SetThreadName("Monitor");

    uint64_t startTimeMs = GetTimeStampMs();

    CpuMonitor cpuMonitor;
    GpuMonitor gpuMonitor;
    DdrMonitor ddrMonitor;
    BatteryMonitor batteryMonitor;
    RamMonitor ramMonitor;
    FpsAnalyzer fpsAnalyzer;
    fpsAnalyzer.StartAnalysis();
    usleep(reflashIntervalMs_ * 1000);
    
    for (;;) {
        CuCsv::CsvRow data{};

        if (monitorItems_.timeMs) {
            uint64_t timeMs = GetTimeStampMs() - startTimeMs;
            data["timeMs"] = StrMerge("%llu", timeMs);
        }

        if (monitorItems_.appPkgName) {
            data["appPkgName"] = topAppPkgName_;
        }

        if (monitorItems_.cpuCurFreq) {
            std::vector<int> cpuCurFreqs = cpuMonitor.GetCpuCurFreq();
            std::string cpuCurFreqData = "";
            for (int cpuCore = 0; cpuCore < (cpuCurFreqs.size() - 1); cpuCore++) {
                cpuCurFreqData += StrMerge("%d/", cpuCurFreqs[cpuCore]);
            }
            cpuCurFreqData += StrMerge("%d", cpuCurFreqs[cpuCurFreqs.size() - 1]);
            data["cpuCurFreq"] = cpuCurFreqData;
        }

        if (monitorItems_.cpuUsage) {
            std::vector<int> cpuUsages = cpuMonitor.GetCpuUsage();
            std::string cpuUsageData = "";
            for (int cpuCore = 0; cpuCore < (cpuUsages.size() - 1); cpuCore++) {
                cpuUsageData += StrMerge("%d/", cpuUsages[cpuCore]);
            }
            cpuUsageData += StrMerge("%d", cpuUsages[cpuUsages.size() - 1]);
            data["cpuUsage"] = cpuUsageData;
        }

        if (monitorItems_.cpuTemp) {
            data["cpuTemp"] = StrMerge("%d", cpuMonitor.GetCpuTemp());
        }

        if (monitorItems_.gpuCurFreq) {
            data["gpuCurFreq"] = StrMerge("%d", gpuMonitor.GetGpuCurFreq());
        }

        if (monitorItems_.gpuUsage) {
            data["gpuUsage"] = StrMerge("%d", gpuMonitor.GetGpuUsage());
        }

        if (monitorItems_.ddrCurFreq) {
            data["ddrCurFreq"] = StrMerge("%d", ddrMonitor.GetDdrCurFreq());
        }

        if (monitorItems_.batteryPower) {
            data["batteryPower"] = StrMerge("%d", batteryMonitor.GetBatteryPower());
        }

        if (monitorItems_.batteryTemp) {
            data["batteryTemp"] = StrMerge("%d", batteryMonitor.GetBatteryTemp());
        }

        if (monitorItems_.batteryPercent) {
            data["batteryPercent"] = StrMerge("%d", batteryMonitor.GetBatteryPercent());
        }

        if (monitorItems_.fps || monitorItems_.jank || monitorItems_.bigJank || monitorItems_.maxFrameTime) {
            FpsAnalyzer::FpsData fpsData = fpsAnalyzer.GetFpsData();

            if (monitorItems_.fps) {
                data["fps"] = StrMerge("%d", fpsData.fps);
            }

            if (monitorItems_.jank) {
                data["jank"] = StrMerge("%d", fpsData.jank);
            }

            if (monitorItems_.bigJank) {
                data["bigJank"] = StrMerge("%d", fpsData.bigJank);
            }

            if (monitorItems_.maxFrameTime) {
                data["maxFrameTime"] = StrMerge("%d", fpsData.maxFrameTime);
            }

            fpsAnalyzer.StartAnalysis();
        }

        if (monitorItems_.ramFree) {
            data["ramFree"] = StrMerge("%d", ramMonitor.GetRamFree());
        }

        csv_ = cuCsv.AppendCsvRow(csv_, data);

        usleep(reflashIntervalMs_ * 1000);
    }
}

void MonitorMain::Saver()
{
    SetThreadName("Saver");

    for (;;) {
        CuCsv::Csv saveCsv = csv_;
        WriteFile(outputPath_, cuCsv.PrintCsv(saveCsv, ","));

        usleep(saveIntervalMs_ * 1000);
    }
}

void MonitorMain::TopAppChanged(const void* data) 
{
    topAppPkgName_ = (char*)data;
}

void MonitorMain::ScreenStateChanged(const void* data)
{
    int screenState = (int)(int64_t)data;
    if (screenState == SCREEN_OFF) {
        topAppPkgName_ = "null";
    }
}
