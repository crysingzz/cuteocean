#include "logger.hpp"

// Явные инстанциации для часто используемых типов
template void Logger::log(LogLevel, const char*);
template void Logger::log(LogLevel, const std::string&);
template void Logger::log(LogLevel, int);
template void Logger::log(LogLevel, float);
template void Logger::log(LogLevel, double);
template void Logger::log(LogLevel, const char*, int);
template void Logger::log(LogLevel, const char*, float);
template void Logger::log(LogLevel, const char*, const char*);