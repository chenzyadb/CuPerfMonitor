#pragma once

#include <sys/inotify.h>

#include "platform/module_common.h"
#include "utils/cu_misc.h"
#include "utils/cu_log.h"

class CgroupWatcher : public ModuleCommon
{
	public:
		CgroupWatcher();
		~CgroupWatcher();
		void Start();

	private:
		std::thread MainThread;
		CuLog cuLog;
		int screenState;

		void Main();
};