/*
 * logger.hpp
 *
 *  Created on: 04.05.2023
 *      Author: Maik
 */
#pragma once

// ANSI escape code for changing text color
#define ANSI_FOREGROUND_BLACK		"\x1b[30m"
#define ANSI_FOREGROUND_RED			"\x1b[31m"
#define ANSI_FOREGROUND_GREEN		"\x1b[32m"
#define ANSI_FOREGROUND_YELLOW		"\x1b[33m"
#define ANSI_FOREGROUND_BLUE		"\x1b[34m"
#define ANSI_FOREGROUND_MAGENTA		"\x1b[35m"
#define ANSI_FOREGROUND_CYAN		"\x1b[36m"
#define ANSI_FOREGROUND_WHITE		"\x1b[37m"
#define ANSI_BACKGROUND_BLACK		"\x1b[40m"
#define ANSI_BACKGROUND_RED			"\x1b[41m"
#define ANSI_BACKGROUND_GREEN		"\x1b[42m"
#define ANSI_BACKGROUND_YELLOW		"\x1b[43m"
#define ANSI_BACKGROUND_BLUE		"\x1b[44m"
#define ANSI_BACKGROUND_MAGENTA		"\x1b[45m"
#define ANSI_BACKGROUND_CYAN		"\x1b[46m"
#define ANSI_BACKGROUND_WHITE		"\x1b[47m"
#define ANSI_RESET					"\x1b[0m"
#define ANSI_INTENSITY_NORMAL		"\x1b[0m"
#define ANSI_INTENSITY_DIM			"\x1b[2m"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <mutex>

#include <events/events.h>

using namespace std;

class Logger : public IEventHandler {
	public:
		enum class level {
			ERR = 5,
	        WARN = 4,
			USER = 3,
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

	    static void user_info(const std::string &msg) {
	        Logger::getInstance().log_internal(msg, level::USER);
		}

	    static void set_level(level log_level) {
	        getInstance().minimal_log_level = log_level;
	    }

	    void handleEvent(Event event) override {
	    	info("Event occurred: " + EVENT_TO_STRING(event.type));
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
	            case level::USER:
	            	return "[USER] ";
	            default:
	                return "[UNKNOWN] ";
	        }
	    }

		void log_internal(const std::string &log, level log_level) {
			std::lock_guard<std::mutex> lock{mutex};
			if (log_level < this->minimal_log_level)
				return;

			std::stringstream stringstream;

			std::string level_date = level_str(log_level);
			{
				auto now = std::time(nullptr);
				//localtime is not thread safe
				auto local_time = std::localtime(&now);
	            stringstream << '[' << std::put_time(local_time, "%Y-%m-%d %H:%M:%S") << "] ";
			}

			if(log_level == level::ERR) {
				stringstream << ANSI_FOREGROUND_RED << level_date << log << ANSI_RESET;
			} else if(log_level == level::WARN) {
				stringstream << ANSI_FOREGROUND_YELLOW << level_date << log << ANSI_RESET;
			} else if(log_level == level::USER) {
				stringstream << ANSI_FOREGROUND_GREEN << level_date << log << ANSI_RESET;
			} else if(log_level == level::DEBUG) {
				stringstream << ANSI_BACKGROUND_BLACK << ANSI_FOREGROUND_WHITE << level_date << log << ANSI_RESET;
			} else if(log_level == level::INFO) {
				stringstream << ANSI_FOREGROUND_BLUE << level_date << log << ANSI_RESET;
			} else {
				stringstream << level_date << log;
			}

			std::cout << stringstream.str() << std::endl;
		}
};
