#include "topapp_monitor.h"

TopAppMonitor::TopAppMonitor() : Module(), cv_(), mtx_(), unblocked_(false) { }

TopAppMonitor::~TopAppMonitor() { }

void TopAppMonitor::Start() { 
	{
		std::thread thread_(std::bind(&TopAppMonitor::Main_, this));
		thread_.detach();
	}
	{
		using namespace std::placeholders;
		Broadcast_SetBroadcastReceiver("CgroupWatcher.TopAppCgroupModified", std::bind(&TopAppMonitor::CgroupModified_, this, _1));
		Broadcast_SetBroadcastReceiver("CgroupWatcher.ForegroundCgroupModified", std::bind(&TopAppMonitor::CgroupModified_, this, _1));
		Broadcast_SetBroadcastReceiver("CgroupWatcher.BackgroundCgroupModified", std::bind(&TopAppMonitor::CgroupModified_, this, _1));
	}
	CgroupModified_(nullptr);
}

void TopAppMonitor::Main_()
{
	SetThreadName("TopAppMonitor");

	std::vector<int> topAppPids{};
	for (;;) {
		{
			std::unique_lock<std::mutex> lck(mtx_);
			while (!unblocked_) {
				cv_.wait(lck);
			}
			unblocked_ = false;
		}
		{
			std::vector<int> newTopAppPids{};
			auto lines = StrSplit(ReadFile("/dev/cpuset/top-app/cgroup.procs"), "\n");
			for (const auto &line : lines) {
				newTopAppPids.emplace_back(StringToInteger(line));
			}
			std::sort(newTopAppPids.begin(), newTopAppPids.end());
			if (newTopAppPids != topAppPids) {
				auto topAppInfo = DumpTopActivityInfo();
				if (StrContains(topAppInfo, "fore")) {
					// Proc # 0: fore   T/A/TOP  trm: 0 4272:xyz.chenzyadb.cu_toolbox/u0a353 (top-activity)
					auto pkgName = GetPrevString(GetPostString(StrDivide(topAppInfo, 7), ':'), '/');
					if (!pkgName.empty()) {
						Broadcast_SendBroadcast("TopAppMonitor.TopAppChanged", GetDataPtr<std::string>(pkgName));
					}
				} else if (StrContains(topAppInfo, "fg")) {
					// Proc # 0: fg     T/A/TOP  LCM  t: 0 4272:xyz.chenzyadb.cu_toolbox/u0a353 (top-activity)
					auto pkgName = GetPrevString(GetPostString(StrDivide(topAppInfo, 8), ':'), '/');
					if (!pkgName.empty()) {
						Broadcast_SendBroadcast("TopAppMonitor.TopAppChanged", GetDataPtr<std::string>(pkgName));
					}
				}
				topAppPids = newTopAppPids;
			}
		}
		usleep(500000);
	}
}

void TopAppMonitor::CgroupModified_(const void* data)
{
	std::unique_lock<std::mutex> lck(mtx_);
	unblocked_ = true;
	cv_.notify_all();
}
