#pragma once

#include <iostream>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include "singleton.h"
#include "utils/ptr_data_conversion.h"

class Broadcast : public Singleton<Broadcast>
{
	public:
		using BroadcastReceiver = std::function<void(const void*)>;

		Broadcast();
		void SetBroadcastReceiver(const std::string &broadcastName, const BroadcastReceiver &br);
		void SendBroadcast(const std::string &broadcastName, const void* data);

	private:
		std::unordered_map<std::string, std::vector<BroadcastReceiver>> broadcastMap_;
};
