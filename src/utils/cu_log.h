#pragma once

#include "cu_misc.h"

class CuLog
{
	public:
		static constexpr int ERROR = 0;
		static constexpr int WARNING = 1;
		static constexpr int INFO = 2;
		static constexpr int DEBUG = 3; 

		CuLog();
		~CuLog();
		
		void InitCuLog(const std::string &logPath);
		void SetLogLevel(const int logLevel);
		void WriteLog(const int logLevel, const char* format, ...);

	private:
		static std::string LOG_PATH;
		static int OUTPUT_LOG_LEVEL;
};
