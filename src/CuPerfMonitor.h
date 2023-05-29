#pragma once

#include <iostream>
#include <exception>

#include "platform/module_common.h"
#include "platform/singleton.h"
#include "modules/monitor_main.h"
#include "modules/cgroup_watcher.h"
#include "modules/topapp_monitor.h"
#include "utils/cu_misc.h"
#include "utils/CuLogger.h"
#include "utils/JsonObject.h"

class CuPerfMonitor
{
	public:
		CuPerfMonitor();
		~CuPerfMonitor();
		void Start(const std::string &configPath, const std::string &modePath);

	private:
		std::vector<std::unique_ptr<ModuleCommon>> modules;

		void Main(const std::string &configPath, const std::string &modePath);
};
