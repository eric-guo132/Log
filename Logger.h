#ifndef LOGGER_H
#define LOGGER_H
#include <iostream>
#include <fstream>
#include <mutex>
#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <stdexcept>

class Logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void init(const std::string& fileName) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!fileStream_.is_open()) {
            fileStream_.open(fileName, std::ios::out | std::ios::app);
            if (!fileStream_) {
                throw std::runtime_error("Failed to open log file: " + fileName);
            }
        }
    }

    void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (fileStream_.is_open()) {
            fileStream_ << getCurrentTime() << " - " << message << std::endl;
        }
        else {
            std::cerr << "Log file is not open. Message: " << message << std::endl;
        }
    }

    template <typename... Args>
    void log(const std::string& format, Args... args) {
        std::string message = formatString(format, args...);
        log(message);
    }

private:
    Logger() = default;
    ~Logger() {
        if (fileStream_.is_open()) {
            fileStream_.close();
        }
    }

    std::string getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        struct tm timeinfo;
        localtime_s(&timeinfo, &now_c);

        std::ostringstream oss;
        oss << std::put_time(&timeinfo, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    template <typename... Args>
    std::string formatString(const std::string& format, Args... args) {
        size_t size = snprintf(nullptr, 0, format.c_str(), args...) + 1;  // Extra space for '\0'
        if (size <= 0) {
            throw std::runtime_error("Error during formatting.");
        }
        std::unique_ptr<char[]> buf(new char[size]);
        snprintf(buf.get(), size, format.c_str(), args...);
        return std::string(buf.get(), buf.get() + size - 1);  // Exclude the '\0'
    }

    std::ofstream fileStream_;
    std::mutex mutex_;
};
#endif // LOGGER_H