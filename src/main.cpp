#include "CuPerfMonitor.h"
#include "utils/CuLogger.h"
#include "utils/cu_misc.h"

constexpr char DAEMON_NAME[] = "CuPerfMonitor";
constexpr char COMPLIE_DATE[] = __DATE__;
constexpr int MIN_KERNEL_VERSION = 401000;
constexpr int MIN_ANDROID_SDK = 28;

void ResetArgv(int argc, char* argv[])
{
	int argLen = 0;
	for (int i = 0; i < argc; i++) {
		argLen += strlen(argv[i]) + 1;
	}
	memset(argv[0], 0, argLen);
	strcpy(argv[0], DAEMON_NAME);
}

void KillOldDaemon(void)
{
	int myPid = getpid();

	DIR* dir = opendir("/proc");
	if (dir) {
		struct dirent* entry = nullptr;
		while ((entry = readdir(dir)) != nullptr) {
			if (IsPathExist(StrMerge("/proc/%s/cmdline", entry->d_name))) {
				int taskPid = atoi(entry->d_name);
				if (taskPid > 0 && taskPid < 32768) {
					std::string taskName = GetTaskName(taskPid);
					if (taskName == DAEMON_NAME && taskPid != myPid) {
						kill(taskPid, SIGKILL);
					}
				}
			}
		}
		closedir(dir);
	}
}

void DaemonMain(const std::string &configPath, const std::string &outputPath)
{
	daemon(0, 0);

	CuLogger::CreateLogger(CuLogger::LOG_DEBUG, "/sdcard/Documents/cu_perf_monitor.log");
	const auto &logger = CuLogger::GetLogger();
	logger->Info("CuPerfMonitor V1.1 (%d) by chenzyadb.", GetCompileDateCode(COMPLIE_DATE));

	if (GetLinuxKernelVersion() < MIN_KERNEL_VERSION) {
		logger->Warning("Your Linux kernel is out-of-date, may have compatibility issues.");
	}
	if (GetAndroidSDKVersion() < MIN_ANDROID_SDK) {
		logger->Warning("Your Android System is out-of-date, may have compatibility issues.");
	}
	if (!IsPathExist(configPath)) {
		logger->Error("Config file doesn't exist.");
		exit(0);
	}
	if (!IsPathExist(GetRePrevString(outputPath, '/'))) {
		logger->Error("Output path doesn't exist.");
		exit(0);
	}

	CuPerfMonitor daemon;
	daemon.Start(configPath, outputPath);

	for (;;) {
		sleep(INT_MAX);
	}
}

int main(int argc, char* argv[])
{
	std::string option = "";
	std::string configPath = "";
	std::string outputPath = "";
	if (argc == 4) {
		option = argv[1];
		configPath = argv[2];
		outputPath = argv[3];
	}
	ResetArgv(argc, argv);
	SetThreadName(DAEMON_NAME);

	if (option == "-R" && argc == 4) {
		std::cout << "Daemon Start." << std::endl;
		KillOldDaemon();
		DaemonMain(configPath, outputPath);
	} else {
		std::cout << "Wrong Input." << std::endl;
	}

	return 0;
}


