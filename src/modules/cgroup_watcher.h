#pragma once

#include <thread>
#include "platform/module.h"
#include "utils/cu_misc.h"
#include "utils/CuLogger.h"

class CgroupWatcher : public Module
{
	public:
		CgroupWatcher();
		~CgroupWatcher();
		void Start();

	private:
		int screenState_;

		void Main_();
};