#include "platform/module_common.h"

ModuleCommon::ModuleCommon() {}
ModuleCommon::~ModuleCommon() {}

void ModuleCommon::Broadcast_SetBroadcastReceiver(const std::string &broadcastName, const Broadcast::BroadcastReceiver &br)
{
	Broadcast::GetInstance()->SetBroadcastReceiver(broadcastName, br);
}

void ModuleCommon::Broadcast_SendBroadcast(const std::string &broadcastName, const void* data)
{
	Broadcast::GetInstance()->SendBroadcast(broadcastName, data);
}
