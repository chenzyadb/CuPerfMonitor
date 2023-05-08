#pragma once

#include <iostream>
#include <thread>
#include "platform/broadcast.h"

class ModuleCommon
{
	public:
		ModuleCommon();
		virtual ~ModuleCommon();
		virtual void Start() = 0;

	protected:
		void Broadcast_SetBroadcastReceiver(const std::string &broadcastName, const Broadcast::BroadcastReceiver &br);
		void Broadcast_SendBroadcast(const std::string &broadcastName, const void* data);
};
