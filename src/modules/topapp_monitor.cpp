#include "topapp_monitor.h"

TopAppMonitor::TopAppMonitor() { }
TopAppMonitor::~TopAppMonitor() { }

void TopAppMonitor::Start()
{
	unblocked = false;
	MainThread = std::thread(std::bind(&TopAppMonitor::Main, this));
	MainThread.detach();

	using namespace std::placeholders;
	Broadcast_SetBroadcastReceiver("CgroupWatcher.TopAppCgroupModified", std::bind(&TopAppMonitor::CgroupModified, this, _1));
	Broadcast_SetBroadcastReceiver("CgroupWatcher.ForegroundCgroupModified", std::bind(&TopAppMonitor::CgroupModified, this, _1));
}

void TopAppMonitor::Main()
{
	SetThreadName("TopAppMonitor");

	int topAppPid = -1;

	for (;;) {
		{
			std::unique_lock<std::mutex> lck(mtx);
			while (!unblocked) {
				cv.wait(lck);
			}
			unblocked = false;
		}

		int prevTopAppPid = topAppPid;
		std::string topAppPkgName = "";
		{
			std::string topAppInfo = DumpTopActivityInfo();
			if (strstr(topAppInfo.c_str(), "fore")) {
				// Proc # 0: fore   T/A/TOP  trm: 0 4272:xyz.chenzyadb.cu_toolbox/u0a353 (top-activity)
				int pid = StringToInteger(GetPrevString(StrDivide(topAppInfo, 7), ':'));
				if (pid > 0 && pid < 32768) {
					topAppPid = pid;
					topAppPkgName = GetPrevString(GetPostString(StrDivide(topAppInfo, 7), ':'), '/');
				}
			} else if (strstr(topAppInfo.c_str(), "fg")) {
				// Proc # 0: fg     T/A/TOP  LCM  t: 0 4272:xyz.chenzyadb.cu_toolbox/u0a353 (top-activity)
				int pid = StringToInteger(GetPrevString(StrDivide(topAppInfo, 8), ':'));
				if (pid > 0 && pid < 32768) {
					topAppPid = pid;
					topAppPkgName = GetPrevString(GetPostString(StrDivide(topAppInfo, 8), ':'), '/');
				}
			}
		}
		if (topAppPid != prevTopAppPid) {
			Broadcast_SendBroadcast("TopAppMonitor.TopAppChanged", (void*)topAppPkgName.c_str());
		}

		usleep(500000);
	}
}

void TopAppMonitor::CgroupModified(const void* data)
{
	std::unique_lock<std::mutex> lck(mtx);
	unblocked = true;
	cv.notify_all();
}
