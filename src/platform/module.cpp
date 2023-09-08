#include "platform/module.h"

Module::Module() { }

Module::~Module() { }

void Module::Broadcast_SetBroadcastReceiver(const std::string &broadcastName, const Broadcast::BroadcastReceiver &br)
{
	Broadcast::GetInstance()->SetBroadcastReceiver(broadcastName, br);
}

void Module::Broadcast_SendBroadcast(const std::string &broadcastName, const void* data)
{
	Broadcast::GetInstance()->SendBroadcast(broadcastName, data);
}

void Module::Timer_AddTimer(const std::string &name, const Timer::TimerTask &task, const int &intervalMs)
{
	Timer::GetInstance()->AddTimer(name, task, intervalMs);
}

void Module::Timer_DeleteTimer(const std::string &name)
{
	Timer::GetInstance()->DeleteTimer(name);
}

bool Module::Timer_IsTimerExist(const std::string &name)
{
	return Timer::GetInstance()->IsTimerExist(name);
}
