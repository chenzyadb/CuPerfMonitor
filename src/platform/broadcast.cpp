#include "broadcast.h"

Broadcast::Broadcast() : broadcastMap_() {}

void Broadcast::SetBroadcastReceiver(const std::string &broadcastName, const BroadcastReceiver &br)
{
    auto iter = broadcastMap_.find(broadcastName);
    if (iter == broadcastMap_.end()) {
        iter = broadcastMap_.emplace(broadcastName, std::vector<BroadcastReceiver>{}).first;
    }
    iter->second.emplace_back(br);
}

void Broadcast::SendBroadcast(const std::string &broadcastName, const void* data)
{
    const auto &iter = broadcastMap_.find(broadcastName);
    if (iter != broadcastMap_.end()) {
        const auto &receivers = iter->second;
        for (const auto &receiver : receivers) {
            receiver(data);
        }
    }
}
