#include "CuPerfMonitor.h"

CuPerfMonitor::CuPerfMonitor() : modules_() {}

CuPerfMonitor::~CuPerfMonitor() 
{
	for (auto &module : modules_) {
		delete module;
		module = nullptr;
	}
}

void CuPerfMonitor::Start(const std::string &configPath, const std::string &outputPath)
{
	try {
		Main_(configPath, outputPath);
	} catch (const std::exception &e) {
		const auto &logger = CuLogger::GetLogger();
		logger->Error("Something went wrong while loading.");
		logger->Error("Exception Thrown: %s.", e.what());
		std::exit(0);
	}
}

void CuPerfMonitor::Main_(const std::string &configPath, const std::string &outputPath)
{
	JsonObject config = JsonObject(ReadFileEx(configPath));

	modules_.clear();
	modules_.emplace_back(new MonitorMain(config, outputPath));
	modules_.emplace_back(new CgroupWatcher());
	modules_.emplace_back(new TopAppMonitor());
	for (const auto &module : modules_) {
		module->Start();
	}

	const auto &logger = CuLogger::GetLogger();
	logger->Info("Daemon Running (pid=%d).", getpid());
}
