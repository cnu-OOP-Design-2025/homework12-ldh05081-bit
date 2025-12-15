#pragma once
#include <fstream>
#include <mutex>
#include <string>
#include <memory>
#include <stdexcept>

class Logger {
private:
    inline static std::unique_ptr<Logger> instance = nullptr;
    inline static std::mutex init_mtx;
    inline static std::mutex write_mtx;

    std::ofstream logFile;

    Logger(const std::string& filename) {
        logFile.open(filename, std::ios::out | std::ios::trunc);
        if (!logFile.is_open()) {
            throw std::runtime_error("Failed to open log file: " + filename);
        }
        logFile << "[Init] Logger started." << std::endl;
    }

public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    ~Logger() {
        // 파일이 정상적으로 열렸을 때만
        if (logFile.is_open()) {
            logFile << "[Shutdown] Logger closed." << std::endl;
            logFile.close();
        }
    }

    static Logger* getInstance(const std::string& filename = "Test/output2.txt") {
        std::lock_guard<std::mutex> lock(init_mtx);
        if (!instance) instance.reset(new Logger(filename));
        return instance.get();
    }

    void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(write_mtx);
        if (logFile.is_open()) {
            logFile << message << std::endl;
        }
    }
};
