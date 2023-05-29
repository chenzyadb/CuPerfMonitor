#pragma once

#include <sys/inotify.h>

#include "platform/module_common.h"
#include "utils/cu_misc.h"
#include "utils/CuLogger.h"

class CgroupWatcher : public ModuleCommon
{
	public:
		CgroupWatcher();
		~CgroupWatcher();
		void Start();

	private:
		std::thread thread_;
		int screenState;

		void Main();
};