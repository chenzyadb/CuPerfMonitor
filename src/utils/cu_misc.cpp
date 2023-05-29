#include "cu_misc.h"

void CreateFile(const std::string &filePath, const std::string &str)
{
    int fd = open(filePath.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0666);
    if (fd < 0) {
        chmod(filePath.c_str(), 0666);
        fd = open(filePath.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0666);
    }
    if (fd >= 0) {
        write(fd, str.data(), str.size());
        close(fd);
    }
}

void AppendFile(const std::string &filePath, const std::string &str)
{
    int fd = open(filePath.c_str(), O_WRONLY | O_APPEND | O_NONBLOCK | O_CLOEXEC);
    if (fd < 0) {
        chmod(filePath.c_str(), 0666);
        fd = open(filePath.c_str(), O_WRONLY | O_APPEND | O_NONBLOCK | O_CLOEXEC);
    }
    if (fd >= 0) {
        write(fd, str.data(), str.size());
        close(fd);
    }
}

void WriteFile(const std::string &filePath, const std::string &str)
{
    int fd = open(filePath.c_str(), O_WRONLY | O_NONBLOCK | O_CLOEXEC);
    if (fd < 0) {
        chmod(filePath.c_str(), 0666);
        fd = open(filePath.c_str(), O_WRONLY | O_NONBLOCK | O_CLOEXEC);
    }
    if (fd >= 0) {
        write(fd, str.data(), str.size());
        close(fd);
    }
}

std::string ReadFile(const std::string &filePath)
{
    std::string ret = "";

    int fd = open(filePath.c_str(), O_RDONLY | O_NONBLOCK | O_CLOEXEC);
    if (fd < 0) {
        chmod(filePath.c_str(), 0666);
        fd = open(filePath.c_str(), O_RDONLY | O_NONBLOCK | O_CLOEXEC);
    }
    if (fd >= 0) {
        char buffer[4096] = { 0 };
        size_t len = read(fd, buffer, sizeof(buffer));
        if (len >= 0) {
            buffer[len] = '\0';
        } else {
            buffer[0] = '\0';
        }
        ret = buffer;
        close(fd);
    }

    return ret;
}

std::string ReadFileEx(const std::string &filePath)
{
    std::string ret = "";

    int fd = open(filePath.c_str(), O_RDONLY | O_NONBLOCK | O_CLOEXEC);
    if (fd < 0) {
        chmod(filePath.c_str(), 0666);
        fd = open(filePath.c_str(), O_RDONLY | O_NONBLOCK | O_CLOEXEC);
    }
    if (fd >= 0) {
        struct stat file_stat{};
        fstat(fd, &file_stat);
        size_t file_size = file_stat.st_size;
        if (file_size == 0) {
            file_size = 64 * 1024;
        }
		ret.resize(file_size + 1);
        size_t len = read(fd, &ret[0], file_size);
        if (len >= 0) {
            ret[len] = '\0';
        } else {
            ret[0] = '\0';
        }
        ret.resize(strlen(ret.c_str()));
        close(fd);
    }

    return ret;
}

std::vector<std::string> StrSplit(const std::string &str, const std::string &delimiter)
{
    std::vector<std::string> strList{};

    size_t start_pos = 0;
    size_t pos = str.find(delimiter);
    while (pos != std::string::npos) {
        strList.emplace_back(str.substr(start_pos, pos - start_pos));
        start_pos = pos + delimiter.size();
        pos = str.find(delimiter, start_pos);
    }
    if (start_pos < str.size()) {
        strList.emplace_back(str.substr(start_pos));
    }

    return strList;
}

std::string StrDivide(const std::string &str, const int &idx)
{
    std::string ret = "";

    size_t start_pos = 0;
    size_t space_num = 0;
    for (size_t pos = 1; pos < str.size(); pos++) {
        if (str[pos] == ' ' && str[pos - 1] != ' ') {
            if (space_num == idx) {
                ret = str.substr(start_pos, pos - start_pos);
            }
            space_num++;
        } else if (str[pos] != ' ' && str[pos - 1] == ' ') {
            start_pos = pos;
        }
    }
    
    return ret;
}

std::string StrMerge(const char* format, ...)
{
    std::string str = "";
	{
		va_list arg;
		va_start(arg, format);
		int size = vsnprintf(nullptr, 0, format, arg);
		va_end(arg);
		if (size > 0) {
			str.resize((size_t)size + 1);
			va_start(arg, format);
			vsnprintf(&str[0], str.size(), format, arg);
			va_end(arg);
		}
		str.resize(strlen(str.c_str()));
	}

    return str;
}

std::string GetPrevString(const std::string &str, const char &chr)
{
    std::string prevString = str;

	size_t pos = prevString.find(chr);
	if (pos != std::string::npos) {
		prevString = prevString.substr(0, pos);
	}

	return prevString;
}

std::string GetRePrevString(const std::string &str, const char &chr)
{
    std::string prevString = str;

	size_t pos = prevString.rfind(chr);
	if (pos != std::string::npos) {
		prevString = prevString.substr(0, pos);
	}

	return prevString;
}

std::string GetPostString(const std::string &str, const char &chr)
{
	std::string postString = str;

	size_t pos = postString.find(chr);
	if (pos != std::string::npos) {
		postString = postString.substr(pos + 1);
	}

	return postString;
}

std::string GetRePostString(const std::string &str, const char &chr)
{
	std::string postString = str;

	size_t pos = postString.rfind(chr);
	if (pos != std::string::npos) {
		postString = postString.substr(pos + 1);
	}

	return postString;
}

bool StrContains(const std::string &str, const std::string &subStr)
{
    bool contains = false;
    if (str.find(subStr) != std::string::npos) {
        contains = true;
    }

    return contains;
}

bool CheckRegex(const std::string &str, const std::string &regex)
{
    bool ret = false;

    regex_t comment;
    regcomp(&comment, regex.c_str(), REG_EXTENDED | REG_NEWLINE | REG_NOSUB);
    if (regexec(&comment, str.c_str(), 0, nullptr, 0) != REG_NOMATCH) {
        ret = true;
    }
    regfree(&comment);

    return ret;
}

std::string DumpTopActivityInfo(void)
{
    std::string ret = "";

    FILE* fp = popen("/system/bin/dumpsys activity oom", "r");
    if (fp) {
        char buffer[4096] = { 0 };
        while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
            if (strstr(buffer, "top-activity")) {
                ret = buffer;
                break;
            }
        }
        pclose(fp);
    }

    return ret;
}

bool IsPathExist(const std::string &path)
{
    bool ret = false;
    if (access(path.c_str(), 0) != -1) {
        ret = true;
    }

    return ret;
}

int GetThreadPid(const int &tid)
{
    int pid = -1;

    char statusPath[128] = { 0 };
    sprintf(statusPath, "/proc/%d/status", tid);
    int fd = open(statusPath, O_RDONLY | O_NONBLOCK | O_CLOEXEC);
    if (fd >= 0) {
        char buffer[4096] = { 0 };
        size_t len = read(fd, buffer, sizeof(buffer));
        char lineStr[128] = { 0 };
        int start_p = 0;
        for (int i = 0; i < len; i++) {
            if (buffer[i] != '\n') {
                lineStr[i - start_p] = buffer[i];
            } else {
                lineStr[i - start_p] = '\0';
                if (strstr(lineStr, "Tgid:")) {
                    sscanf(lineStr, "Tgid: %d", &pid);
                    break;
                }
                start_p = i + 1;
            }
        }
        close(fd);
    }

    return pid;
}

int GetTaskType(const int &pid)
{
/*
    Android oom_adj 值类型:

    UNKNOWN_ADJ 	        16 	    预留的最低级别，一般对于缓存的进程才有可能设置成这个级别
    CACHED_APP_MAX_ADJ 	    15 	    缓存进程，空进程，在内存不足的情况下就会优先被kill
    CACHED_APP_MIN_ADJ 	    9 	    缓存进程，也就是空进程
    SERVICE_B_ADJ 	        8 	    不活跃的进程
    PREVIOUS_APP_ADJ 	    7 	    切换进程
    HOME_APP_ADJ 	        6 	    与Home交互的进程
    SERVICE_ADJ 	        5 	    有Service的进程
    HEAVY_WEIGHT_APP_ADJ 	4 	    高权重进程
    BACKUP_APP_ADJ 	        3 	    正在备份的进程
    PERCEPTIBLE_APP_ADJ 	2 	    可感知的进程，比如那种播放音乐
    VISIBLE_APP_ADJ 	    1 	    可见进程
    FOREGROUND_APP_ADJ 	    0 	    前台进程
    PERSISTENT_SERVICE_ADJ 	-11 	重要进程
    PERSISTENT_PROC_ADJ 	-12 	核心进程
    SYSTEM_ADJ 	            -16 	系统进程
    NATIVE_ADJ 	            -17 	系统起的Native进程
*/

    int taskType = TASK_OTHER;

    char oomAdjPath[128] = { 0 };
    sprintf(oomAdjPath, "/proc/%d/oom_adj", pid);
    int fd = open(oomAdjPath, O_RDONLY | O_NONBLOCK | O_CLOEXEC);
    if (fd >= 0) {
        char buffer[4096] = { 0 };
        read(fd, buffer, sizeof(buffer));
        int oom_adj = 16;
        sscanf(buffer, "%d", &oom_adj);
        if (oom_adj == 0) {
            taskType = TASK_FOREGROUND;
        } else if (oom_adj == 1) {
            taskType = TASK_VISIBLE;
        } else if (oom_adj >= 2 && oom_adj <= 8) {
            taskType = TASK_SERVICE;
        } else if (oom_adj <= -1 && oom_adj >= -17) {
            taskType = TASK_SYSTEM;
        } else if (oom_adj >= 9 && oom_adj <= 16) {
            taskType = TASK_CACHED;
        }
        close(fd);
    }

    return taskType;
}

std::string GetTaskName(const int &pid)
{
    std::string ret = ""; 

    char cmdlinePath[128] = { 0 };
    sprintf(cmdlinePath, "/proc/%d/cmdline", pid);
    int fd = open(cmdlinePath, O_RDONLY | O_NONBLOCK | O_CLOEXEC);
    if (fd >= 0) {
        char buffer[4096] = { 0 };
        size_t len = read(fd, buffer, sizeof(buffer));
        if (len >= 0) {
            buffer[len] = '\0';
        } else {
            buffer[0] = '\0';
        }
        ret = buffer;
        close(fd);
    }

    return ret;
}

std::string GetTaskComm(const int &pid)
{
    std::string ret = ""; 

    char cmdlinePath[128] = { 0 };
    sprintf(cmdlinePath, "/proc/%d/comm", pid);
    int fd = open(cmdlinePath, O_RDONLY | O_NONBLOCK | O_CLOEXEC);
    if (fd >= 0) {
        char buffer[4096] = { 0 };
        size_t len = read(fd, buffer, sizeof(buffer));
        if (len >= 0) {
            buffer[len] = '\0';
        } else {
            buffer[0] = '\0';
        }
        ret = buffer;
        close(fd);
    }

    return ret;
}

unsigned long int GetThreadRuntime(const int &pid, const int &tid)
{
	unsigned long int runtime = 0;

	char statPath[128] = { 0 };
	sprintf(statPath, "/proc/%d/task/%d/stat", pid, tid);
	int fd = open(statPath, O_RDONLY | O_NONBLOCK | O_CLOEXEC);
	if (fd >= 0) {
        char buffer[4096] = { 0 };
		size_t len = read(fd, buffer, sizeof(buffer));
        if (len >= 0) {
            buffer[len] = '\0';
        } else {
            buffer[0] = '\0';
        }
        unsigned long int utime, stime, cutime, cstime;
		sscanf(buffer, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %lu %lu %*d %*d %*d %*d %*u %*lu %*ld", 
            &utime, &stime, &cutime, &cstime);
		runtime = utime + stime + cutime + cstime;
        close(fd);
	}

	return runtime;
}

int GetScreenStateViaCgroup(void)
{
    int state = SCREEN_ON;

    int fd = open("/dev/cpuset/restricted/tasks", O_RDONLY | O_NONBLOCK | O_CLOEXEC);
    if (fd >= 0) {
        char buffer[4096] = { 0 };
        size_t len = read(fd, buffer, sizeof(buffer));
        int restrictedTaskNum = 0;
        for (int i = 0; i < len; i++) {
            if (buffer[i] == '\n') {
                restrictedTaskNum++;
                if (restrictedTaskNum > 10) {
                    state = SCREEN_OFF;
                    break;
                }
            }
        }
        close(fd);
    }

    return state;
}

int GetScreenStateViaWakelock(void)
{
    int state = SCREEN_ON;

    int fd = open("/sys/power/wake_lock", O_RDONLY | O_NONBLOCK | O_CLOEXEC);
    if (fd >= 0) {
        char buffer[4096] = { 0 };
        read(fd, buffer, sizeof(buffer));
        if (strstr(buffer, "PowerManagerService.Display")) {
            state = SCREEN_ON;
        } else {
            state = SCREEN_OFF;
        }
        close(fd);
    }

    return state;
}

int GetCompileDateCode(const std::string &compileDate)
{
    int year = 0;
    int month = 0;
    int day = 0;

    char monthStr[4] = { 0 };
    sscanf(compileDate.c_str(), "%s %d %d", monthStr, &day, &year);
    if (strcmp(monthStr, "Jan") == 0) {
        month = 1;
    } else if (strcmp(monthStr, "Feb") == 0) {
        month = 2;
    } else if (strcmp(monthStr, "Mar") == 0) {
        month = 3;
    } else if (strcmp(monthStr, "Apr") == 0) {
        month = 4;
    } else if (strcmp(monthStr, "May") == 0) {
        month = 5;
    } else if (strcmp(monthStr, "Jun") == 0) {
        month = 6;
    } else if (strcmp(monthStr, "Jul") == 0) {
        month = 7;
    } else if (strcmp(monthStr, "Aug") == 0) {
        month = 8;
    } else if (strcmp(monthStr, "Sep") == 0) {
        month = 9;
    } else if (strcmp(monthStr, "Oct") == 0) {
        month = 10;
    } else if (strcmp(monthStr, "Nov") == 0) {
        month = 11;
    } else if (strcmp(monthStr, "Dec") == 0) {
        month = 12;
    }

    return year * 10000 + month * 100 + day;
}

int RoundNum(const float &num)
{
    int ret = 0;

    int dec = (int)(num * 10) % 10;
    if (dec >= 5) {
        ret = (int)num + 1;
    } else {
        ret = (int)num;
    }

    return ret;
}

void SetThreadName(const std::string &name)
{
    prctl(PR_SET_NAME, name.c_str());
}

int GetAndroidSDKVersion(void)
{
    char buffer[PROP_VALUE_MAX] = { 0 };
    __system_property_get("ro.build.version.sdk", buffer);
    return atoi(buffer);
}

int GetLinuxKernelVersion(void)
{
    // VersionCode: r.xx.yyy(3.18.140) -> 318140; r.x.yy(5.4.86) -> 504086;
    int version = 0;

    struct utsname uts{};
    if (uname(&uts) != -1) {
        int r = 0, x = 0, y = 0;
        sscanf(uts.release, "%d.%d.%d-%*s", &r, &x, &y);
        sscanf(uts.release, "%d.%d.%d.%*d-%*s", &r, &x, &y);
        version = r * 100000 + x * 1000 + y;
    }

    return version;
}

int FindTaskPid(const std::string &taskName)
{
    int pid = -1;

    DIR* dir = opendir("/proc");
    if (dir) {
        struct dirent* entry = nullptr;
		while ((entry = readdir(dir)) != nullptr) {
            char cmdlinePath[128] = { 0 };
            sprintf(cmdlinePath, "/proc/%s/cmdline", entry->d_name);
            int fd = open(cmdlinePath, O_RDONLY | O_NONBLOCK | O_CLOEXEC);
            if (fd >= 0) {
                char buffer[4096] = { 0 };
                read(fd, buffer, sizeof(buffer));
                if (strstr(buffer, taskName.c_str())) {
                    pid = atoi(entry->d_name);
                    break;
                }
                close(fd);
            }
        }
        closedir(dir);
    }

    return pid;
}

uint64_t GetTimeStampMs(void) 
{
    struct timespec ts{};
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;
}

int StringToInteger(const std::string &str)
{
    int integer = 0;
    for (const char &c : str) {
        switch (c) {
            case '0':
                integer = integer * 10;
                break;
            case '1':
                integer = integer * 10 + 1;
                break;
            case '2':
                integer = integer * 10 + 2;
                break;
            case '3':
                integer = integer * 10 + 3;
                break;
            case '4':
                integer = integer * 10 + 4;
                break;
            case '5':
                integer = integer * 10 + 5;
                break;
            case '6':
                integer = integer * 10 + 6;
                break;
            case '7':
                integer = integer * 10 + 7;
                break;
            case '8':
                integer = integer * 10 + 8;
                break;
            case '9':
                integer = integer * 10 + 9;
                break;
            default:
                break;
        }
        if (((int64_t)integer * 10 + 10) > INT_MAX) {
            break;
        }
    }

    return integer;
}

uint64_t StringToLong(const std::string &str)
{
    uint64_t integer = 0;
    for (const char &c : str) {
        switch (c) {
            case '0':
                integer = integer * 10;
                break;
            case '1':
                integer = integer * 10 + 1;
                break;
            case '2':
                integer = integer * 10 + 2;
                break;
            case '3':
                integer = integer * 10 + 3;
                break;
            case '4':
                integer = integer * 10 + 4;
                break;
            case '5':
                integer = integer * 10 + 5;
                break;
            case '6':
                integer = integer * 10 + 6;
                break;
            case '7':
                integer = integer * 10 + 7;
                break;
            case '8':
                integer = integer * 10 + 8;
                break;
            case '9':
                integer = integer * 10 + 9;
                break;
            default:
                break;
        }
    }

    return integer;
}

uint64_t String16BitToInteger(const std::string &str)
{
    uint64_t integer = 0;
    for (const char &c : str) {
        if (c >= '0' && c <= '9') {
            integer = integer * 16 + (c - '0');
        } else if (c >= 'a' && c <= 'f') {
            integer = integer * 16 + (c - 'a' + 10);
        } else if (c >= 'A' && c <= 'F') {
            integer = integer * 16 + (c - 'A' + 10);
        } else {
            break;
        }
    }

    return integer;
}
