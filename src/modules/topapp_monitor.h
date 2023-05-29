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
		std::thread thread_;
		std::condition_variable cv_;
		std::mutex mtx_;
		bool unblocked_;

		void Main();
		void CgroupModified(const void* data);
};