#include "CuPerfMonitor.h"

CuPerfMonitor::CuPerfMonitor() {}

CuPerfMonitor::~CuPerfMonitor() {}

void CuPerfMonitor::Start(const std::string &configPath, const std::string &outputPath)
{
	try {
		Main(configPath, outputPath);
	} catch (const std::exception &e) {
		cuLog.WriteLog(CuLog::ERROR, "Something went wrong while loading.");
		cuLog.WriteLog(CuLog::ERROR, "Exception Thrown: %s.", e.what());
		std::exit(0);
	}
}

void CuPerfMonitor::Main(const std::string &configPath, const std::string &outputPath)
{
	cuJson cj;
	cuJson::Json json = cj.ParseJson(ReadFileEx(configPath));
	std::string config = cj.PrintJson(json);

	modules.clear();
	modules.emplace_back(std::make_unique<MonitorMain>(config, outputPath));
	modules.emplace_back(std::make_unique<CgroupWatcher>());
	modules.emplace_back(std::make_unique<TopAppMonitor>());
	for (const auto &module : modules) {
		module->Start();
	}

	cuLog.WriteLog(CuLog::INFO, "Daemon Running (pid=%d).", getpid());
}
