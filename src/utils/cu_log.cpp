#include "cu_log.h"

std::string CuLog::LOG_PATH = "";
int CuLog::OUTPUT_LOG_LEVEL = CuLog::INFO;

CuLog::CuLog() {}
CuLog::~CuLog() {}

void CuLog::InitCuLog(const std::string &logPath)
{
	LOG_PATH = logPath;
	CreateFile(LOG_PATH, "");
}

void CuLog::SetLogLevel(const int logLevel)
{
	OUTPUT_LOG_LEVEL = logLevel;
}

void CuLog::WriteLog(const int logLevel, const char* format, ...)
{
	if (logLevel > OUTPUT_LOG_LEVEL) {
		return;
	}

	char logText[4096] = { 0 };
	va_list arg;
	va_start(arg, format);
	vsprintf(logText, format, arg);
	va_end(arg);

	constexpr char logLevelChar[] = {'E', 'W', 'I', 'D'};
	AppendFile(LOG_PATH, StrMerge("%s [%c] %s\n", GetTimeInfo().c_str(), logLevelChar[logLevel], logText));
}