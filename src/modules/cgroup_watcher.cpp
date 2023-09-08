#include "cgroup_watcher.h"
#include <sys/inotify.h>

CgroupWatcher::CgroupWatcher() : Module(), screenState_(SCREEN_OFF) { }

CgroupWatcher::~CgroupWatcher() { }

void CgroupWatcher::Start()
{
	if (GetAndroidSDKVersion() < 29) {
		screenState_ = GetScreenStateViaWakelock();
	} else {
		screenState_ = GetScreenStateViaCgroup();
	}
	Broadcast_SendBroadcast("CgroupWatcher.ScreenStateChanged", GetDataPtr<int>(screenState_));

	std::thread thread_(std::bind(&CgroupWatcher::Main_, this));
	thread_.detach();
}

void CgroupWatcher::Main_()
{
	SetThreadName("CgroupWatcher");
	const auto &logger = CuLogger::GetLogger();
	
	int androidSDKVersion = GetAndroidSDKVersion();

	int fd = inotify_init();
	if (fd < 0) {
		logger->Error("Failed to init inotify.");
		std::exit(0);
	}

	int ta_wd = -1, fg_wd = -1, bg_wd = -1, re_wd = -1;
	if (androidSDKVersion < 29) {
		ta_wd = inotify_add_watch(fd, "/dev/cpuset/top-app/tasks", IN_MODIFY);
		if (ta_wd < 0) {
			logger->Warning("Failed to watch top-app cgroup.");
		}
		fg_wd = inotify_add_watch(fd, "/dev/cpuset/foreground/tasks", IN_MODIFY);
		if (fg_wd < 0) {
			logger->Warning("Failed to watch foreground cgroup.");
		}
		bg_wd = inotify_add_watch(fd, "/dev/cpuset/background/tasks", IN_MODIFY);
		if (bg_wd < 0) {
			logger->Warning("Failed to watch background cgroup.");
		}
	} else if (androidSDKVersion < 33) {
		ta_wd = inotify_add_watch(fd, "/dev/cpuset/top-app/tasks", IN_MODIFY);
		if (ta_wd < 0) {
			logger->Warning("Failed to watch top-app cgroup.");
		}
		fg_wd = inotify_add_watch(fd, "/dev/cpuset/foreground/tasks", IN_MODIFY);
		if (fg_wd < 0) {
			logger->Warning("Failed to watch foreground cgroup.");
		}
		bg_wd = inotify_add_watch(fd, "/dev/cpuset/background/tasks", IN_MODIFY);
		if (bg_wd < 0) {
			logger->Warning("Failed to watch background cgroup.");
		}
		re_wd = inotify_add_watch(fd, "/dev/cpuset/restricted/tasks", IN_MODIFY);
		if (re_wd < 0) {
			logger->Warning("Failed to watch restricted cgroup.");
		}
	} else {
		ta_wd = inotify_add_watch(fd, "/dev/cpuset/top-app/cgroup.procs", IN_MODIFY);
		if (ta_wd < 0) {
			logger->Warning("Failed to watch top-app cgroup.");
		}
		fg_wd = inotify_add_watch(fd, "/dev/cpuset/foreground/cgroup.procs", IN_MODIFY);
		if (fg_wd < 0) {
			logger->Warning("Failed to watch foreground cgroup.");
		}
		bg_wd = inotify_add_watch(fd, "/dev/cpuset/background/cgroup.procs", IN_MODIFY);
		if (bg_wd < 0) {
			logger->Warning("Failed to watch background cgroup.");
		}
		re_wd = inotify_add_watch(fd, "/dev/cpuset/restricted/cgroup.procs", IN_MODIFY);
		if (re_wd < 0) {
			logger->Warning("Failed to watch restricted cgroup.");
		}
	}

	for(;;) {
		struct inotify_event watchEvent{};
		read(fd, &watchEvent, sizeof(struct inotify_event));
		if (watchEvent.mask == IN_MODIFY) {
			if (screenState_ == SCREEN_ON) {
				if (watchEvent.wd == ta_wd) {
					Broadcast_SendBroadcast("CgroupWatcher.TopAppCgroupModified", nullptr);
				} else if (watchEvent.wd == fg_wd) {
					Broadcast_SendBroadcast("CgroupWatcher.ForegroundCgroupModified", nullptr);
				} else if (watchEvent.wd == bg_wd) {
					Broadcast_SendBroadcast("CgroupWatcher.BackgroundCgroupModified", nullptr);
				}

				if (androidSDKVersion < 29) {
					screenState_ = GetScreenStateViaWakelock();
				} else {
					screenState_ = GetScreenStateViaCgroup();
				}
				if (screenState_ == SCREEN_OFF) {
					Broadcast_SendBroadcast("CgroupWatcher.ScreenStateChanged", GetDataPtr<int>(screenState_));
				}
			} else {
				if (androidSDKVersion < 29) {
					screenState_ = GetScreenStateViaWakelock();
				} else {
					screenState_ = GetScreenStateViaCgroup();
				}
				if (screenState_ == SCREEN_ON) {
					Broadcast_SendBroadcast("CgroupWatcher.ScreenStateChanged", GetDataPtr<int>(screenState_));
				}
			}
		}
		usleep(10000);
	}
}
