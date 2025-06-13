#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <mutex>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR,
    DEBUG
};

class Logger {
private:
    std::ofstream logFile;
    LogLevel minLevel = LogLevel::INFO;
    bool logToConsole = true;
    bool logToFile = false;
    std::string filename = "ocean_log.txt";
    std::mutex logMutex;
    bool fileOpenFailed = false;

    Logger() {
        // Попытка открыть файл при создании
        tryOpenFile();
    }

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void tryOpenFile() {
        if (logToFile && !filename.empty()) {
            logFile.open(filename, std::ios::app);
            if (!logFile.is_open()) {
                fileOpenFailed = true;
                std::cerr << "[LOGGER ERROR] Failed to open log file: " << filename << std::endl;
            } else {
                fileOpenFailed = false;
                // Запись заголовка при первом открытии
                std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                logFile << "\n\n===== Log started at: " << std::ctime(&now) << "=====\n";
            }
        }
    }

public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void configure(LogLevel level, bool console, bool file, const std::string& newFilename = "") {
        std::lock_guard<std::mutex> lock(logMutex);
        
        minLevel = level;
        logToConsole = console;
        logToFile = file;
        
        if (!newFilename.empty()) {
            filename = newFilename;
        }
        
        if (logFile.is_open()) {
            logFile.close();
        }
        
        tryOpenFile();
    }

    template <typename... Args>
    void log(LogLevel level, Args... args) {
        if (level < minLevel) return;

        std::lock_guard<std::mutex> lock(logMutex);
        
        // Получение текущего времени
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm = *std::localtime(&now_time);
        
        // Форматирование сообщения
        std::ostringstream oss;
        oss << "[" << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << "] ";
        
        switch (level) {
            case LogLevel::INFO: oss << "[INFO]    "; break;
            case LogLevel::WARNING: oss << "[WARNING] "; break;
            case LogLevel::ERROR: oss << "[ERROR]   "; break;
            case LogLevel::DEBUG: oss << "[DEBUG]   "; break;
        }
        
        // Добавление аргументов
        (oss << ... << args);
        
        const std::string logMessage = oss.str();
        
        // Вывод в консоль
        if (logToConsole) {
            std::cout << logMessage << std::endl;
        }
        
        // Запись в файл
        if (logToFile) {
            if (!logFile.is_open() && !fileOpenFailed) {
                tryOpenFile();
            }
            
            if (logFile.is_open()) {
                logFile << logMessage << std::endl;
                logFile.flush();
            }
        }
    }

    ~Logger() {
        if (logFile.is_open()) {
            // Запись завершающего сообщения
            std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            logFile << "===== Log ended at: " << std::ctime(&now) << "=====\n\n";
            logFile.close();
        }
    }
};

#endif // LOGGER_HPP