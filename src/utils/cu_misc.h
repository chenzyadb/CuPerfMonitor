#pragma once

#include <iostream>
#include <stdexcept>
#include <utility>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <ctime>
#include <cstdarg>
#include <cstring>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/system_properties.h>
#include <regex.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#define SCREEN_ON 1
#define SCREEN_OFF 0

#define TASK_OTHER -1
#define TASK_FOREGROUND 0
#define TASK_VISIBLE 1
#define TASK_SERVICE 2
#define TASK_SYSTEM 3
#define TASK_BACKGROUND 4
#define TASK_KILLABLE 5

void CreateFile(const std::string &filePath, const std::string &str);
void AppendFile(const std::string &filePath, const std::string &str);
void WriteFile(const std::string &filePath, const std::string &str);
std::string ReadFile(const std::string &filePath);
std::string ReadFileEx(const std::string &filePath);
std::vector<std::string> StrSplit(const std::string &str, const std::string &delimiter);
std::string StrDivide(const std::string &str, const int &idx);
std::string StrMerge(const char* format, ...);
std::string GetPrevString(const std::string &str, const char &chr);
std::string GetRePrevString(const std::string &str, const char &chr);
std::string GetPostString(const std::string &str, const char &chr);
std::string GetRePostString(const std::string &str, const char &chr);
bool StrContains(const std::string &str, const std::string &subStr);
bool CheckRegex(const std::string &str, const std::string &regex);
std::string DumpTopActivityInfo(void);
bool IsPathExist(const std::string &path);
int GetThreadPid(const int &tid);
int GetTaskType(const int &pid);
std::string GetTaskName(const int &pid);
std::string GetTaskComm(const int &pid);
unsigned long int GetThreadRuntime(const int &pid, const int &tid);
int GetScreenStateViaCgroup(void);
int GetScreenStateViaWakelock(void);
int GetCompileDateCode(const std::string &compileDate);
int RoundNum(const float &num);
void SetThreadName(const std::string &name);
int GetAndroidSDKVersion(void);
int GetLinuxKernelVersion(void);
int FindTaskPid(const std::string &taskName);
uint64_t GetTimeStampMs(void);
int StringToInteger(const std::string &str);
uint64_t StringToLong(const std::string &str);
uint64_t String16BitToInteger(const std::string &str);
std::string TrimStr(const std::string &str);
int AbsInt(const int &num);
