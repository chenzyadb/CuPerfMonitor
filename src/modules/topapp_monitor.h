#pragma once

#include "platform/module_common.h"
#include "utils/cu_misc.h"

class TopAppMonitor : public ModuleCommon
{
	public:
		TopAppMonitor();
		~TopAppMonitor();
		void Start();
		
	private:
		std::thread MainThread;
		std::condition_variable cv;
		std::mutex mtx;
		bool unblocked;

		void Main();
		void CgroupModified(const void* data);
};