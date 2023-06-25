/*
 * logger.hpp
 *
 *  Created on: 04.05.2023
 *      Author: Maik
 */
#pragma once

#include <iostream>
#include <sstream>
#include <iomanip>
#include <mutex>
#include <chrono>
#include <ctime>

using namespace std;

class Logger {
	public:
		enum class level {
			ERR = 4,
	        WARN = 3,
	        INFO = 2,
	        DEBUG = 1,
	        DATA = 0,
	    };

	    static void info(const std::string &log) {
	        Logger::getInstance().log_internal(log, level::INFO);
	    }

	    static void error(const std::string &log) {
	        Logger::getInstance().log_internal(log, level::ERR);
	    }

	    static void warn(const std::string &log) {
	        Logger::getInstance().log_internal(log, level::WARN);
	    }

	    static void data(const std::string &log) {
	        Logger::getInstance().log_internal(log, level::DATA);
	    }

	    static void debug(const std::string &log) {
	        Logger::getInstance().log_internal(log, level::DEBUG);
	    }

	    static void set_level(level log_level) {
	        getInstance().minimal_log_level = log_level;
	    }

	private:
		Logger(){}
		~Logger(){}
		std::mutex mutex{};
		level minimal_log_level{level::INFO};

		static Logger& getInstance()
		{
			static Logger instance;
			return instance;
		}

	    static std::string level_str(level l) {
	        switch (l) {
	            case level::DEBUG:
	                return "[DEBUG] ";
	            case level::ERR:
	                return "[ERROR] ";
	            case level::WARN:
	                return "[WARN] ";
	            case level::INFO:
	                return "[INFO] ";
	            case level::DATA:
	                return "[DATA] ";
	            default:
	                return "[UNKNOWN] ";
	        }
	    }

		void log_internal(const std::string &log, level log_level) {
			std::lock_guard<std::mutex> lock{mutex};
			if (log_level < this->minimal_log_level)
				return;

			std::stringstream stringstream;

			stringstream << level_str(log_level);
			{
				auto now = std::time(nullptr);
				auto currentTime = std::chrono::system_clock::now();
			    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count();

				std::time_t time = std::chrono::system_clock::to_time_t(currentTime);
			    char buffer[100];
			    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&time));

			    stringstream << '[' << buffer << "." << std::setw(3) << std::setfill('0') << milliseconds % 1000 << "] ";
			}
			stringstream << log;

			std::cout << stringstream.str() << std::endl;
		}
};
