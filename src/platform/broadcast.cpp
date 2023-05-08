#include "broadcast.h"

Broadcast::Broadcast() {}

void Broadcast::SetBroadcastReceiver(const std::string &broadcastName, const BroadcastReceiver &br)
{
    auto iter = broadcastList.find(broadcastName);
    if (iter == broadcastList.end()) {
        iter = broadcastList.emplace(broadcastName, std::vector<BroadcastReceiver>{}).first;
    }
    iter->second.emplace_back(br);
}

void Broadcast::SendBroadcast(const std::string &broadcastName, const void* data)
{
    const auto &iter = broadcastList.find(broadcastName);
    if (iter != broadcastList.end()) {
        const auto &brList = iter->second;
        for (const auto &br : brList) {
            br(data);
        }
    }
}
