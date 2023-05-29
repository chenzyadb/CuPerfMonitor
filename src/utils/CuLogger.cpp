#include "CuLogger.h"

CuLogger* CuLogger::instance_ = nullptr;
std::once_flag CuLogger::flag_;
std::string CuLogger::logPath_ = CuLogger::LOG_PATH_NONE;
int CuLogger::logLevel_ = CuLogger::LOG_NONE;
