#include "CuPerfMonitor.h"

CuPerfMonitor::CuPerfMonitor() {}

CuPerfMonitor::~CuPerfMonitor() {}

void CuPerfMonitor::Start(const std::string &configPath, const std::string &outputPath)
{
	try {
		Main(configPath, outputPath);
	} catch (const std::exception &e) {
		const auto &logger = CuLogger::GetLogger();
		logger->Error("Something went wrong while loading.");
		logger->Error("Exception Thrown: %s.", e.what());
		std::exit(0);
	}
}

void CuPerfMonitor::Main(const std::string &configPath, const std::string &outputPath)
{
	JsonObject config = JsonObject(ReadFileEx(configPath));

	modules.clear();
	modules.emplace_back(std::make_unique<MonitorMain>(config, outputPath));
	modules.emplace_back(std::make_unique<CgroupWatcher>());
	modules.emplace_back(std::make_unique<TopAppMonitor>());
	for (const auto &module : modules) {
		module->Start();
	}

	const auto &logger = CuLogger::GetLogger();
	logger->Info("Daemon Running (pid=%d).", getpid());
}
