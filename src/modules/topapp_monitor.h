#pragma once

#include <regex>
#include "platform/module.h"
#include "utils/cu_misc.h"
#include "utils/CuLogger.h"

class TopAppMonitor : public Module
{
	public:
		TopAppMonitor();
		~TopAppMonitor();
		void Start();
		
	private:
		std::condition_variable cv_;
		std::mutex mtx_;
		bool unblocked_;

		void Main_();
		void CgroupModified_(const void* data);
};
