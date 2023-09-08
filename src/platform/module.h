#pragma once

#include "platform/broadcast.h"
#include "platform/timer.h"

class Module
{
	public:
		Module();
		virtual ~Module();
		virtual void Start() = 0;

	protected:
		void Broadcast_SetBroadcastReceiver(const std::string &broadcastName, const Broadcast::BroadcastReceiver &br);
		void Broadcast_SendBroadcast(const std::string &broadcastName, const void* data);
		void Timer_AddTimer(const std::string &name, const Timer::TimerTask &task, const int &intervalMs);
		void Timer_DeleteTimer(const std::string &name);
		bool Timer_IsTimerExist(const std::string &name);
};
