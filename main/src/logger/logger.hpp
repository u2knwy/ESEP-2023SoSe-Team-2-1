/*
 * logger.hpp
 *
 *  Created on: 04.05.2023
 *      Author: Maik
 */
#pragma once

// ANSI escape code for changing text color
#define ANSI_COLOR_BLACK      "\x1b[30m"
#define ANSI_COLOR_RED        "\x1b[31m"
#define ANSI_COLOR_GREEN      "\x1b[32m"
#define ANSI_COLOR_YELLOW     "\x1b[33m"
#define ANSI_COLOR_BLUE       "\x1b[34m"
#define ANSI_COLOR_MAGENTA    "\x1b[35m"
#define ANSI_COLOR_CYAN       "\x1b[36m"
#define ANSI_COLOR_LIGHTGREY  "\x1b[37m"
#define ANSI_BG_BLACK         "\x1b[40m"
#define ANSI_BG_RED           "\x1b[41m"
#define ANSI_BG_GREEN         "\x1b[42m"
#define ANSI_BG_YELLOW        "\x1b[43m"
#define ANSI_BG_BLUE          "\x1b[44m"
#define ANSI_BG_MAGENTA       "\x1b[45m"
#define ANSI_BG_CYAN          "\x1b[46m"
#define ANSI_BG_WHITE         "\x1b[47m"
#define ANSI_RESET            "\x1b[0m"
#define ANSI_INTENSITY_NORMAL "\x1b[0m"
#define ANSI_INTENSITY_DIM    "\x1b[2m"

#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <fstream>

#include "common/macros.h"
#include "events/EventManager.h"
#include "events/events.h"

using namespace std;

class Logger {
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

	static void to_file(const std::string &msg) {
		Logger::getInstance().log_to_file(msg);
	}

	static void set_level(level log_level) {
		getInstance().minimal_log_level = log_level;
	}

	static void logEvent(Event event) {
		std::stringstream ss;
		ss << "Event occurred: " << EVENT_TO_STRING(event.type);
		if (event.data != -1) {
			ss << " - data=" << event.data;
		}
		info(ss.str());
	}

	/**
	 * Register to all events, which will then be logged by this Logger
	 *
	 * @param eventManager reference to the EventManager where the Logger should
	 * subscribe to all events
	 */
	static void registerEvents(std::shared_ptr<EventManager> eventManager) {
		int nEvents = eventManager->subscribeToAllEvents(std::bind(&Logger::logEvent, std::placeholders::_1));
		std::stringstream ss;
		ss << "[Logger] Registered to all events (" << nEvents
				<< ") from EventManager";
		Logger::debug(ss.str());
	}

private:
	Logger() {
		auto now = std::time(nullptr);
		auto local_time = std::localtime(&now);
		std::stringstream ss;
		ss << "/tmp/esep_2.1/";
		ss << "log_" << std::put_time(local_time, "%Y%m%d-%H%M%S") << ".txt";
		std::string logFilePath = ss.str();
		system(std::string("rm " + ss.str()).c_str());
		logFile.open(logFilePath);
		if (!logFile.is_open()) {
			throw std::runtime_error("Failed to open file.");
		}
	}
	~Logger() {
		logFile.close();
	}
	std::mutex mutex{};
	level minimal_log_level{level::INFO};
	std::ofstream logFile;

	static Logger &getInstance() {
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
			return "[WARN]  ";
		case level::INFO:
			return "[INFO]  ";
		case level::DATA:
			return "[DATA]  ";
		case level::USER:
			return "[USER]  ";
		default:
			return "[UNKNOWN] ";
		}
	}

	void log_internal(const std::string &log, level log_level) {
		std::lock_guard<std::mutex> lock{mutex};
		if (log_level < this->minimal_log_level)
			return;

		std::stringstream ss;

		if (log_level == level::ERR) {
			ss << ANSI_COLOR_RED;
		} else if (log_level == level::WARN) {
			ss << ANSI_COLOR_YELLOW;
		} else if (log_level == level::USER) {
			ss << ANSI_COLOR_GREEN;
		} else if (log_level == level::DEBUG) {
			ss << ANSI_COLOR_LIGHTGREY;
		} else if (log_level == level::INFO) {
			ss << ANSI_COLOR_BLUE;
		}

		auto now = std::time(nullptr);
		// localtime is not thread safe
		auto local_time = std::localtime(&now);
		ss << '[' << std::put_time(local_time, "%Y-%m-%d %H:%M:%S") << "] ";

		ss << level_str(log_level) << log << ANSI_RESET;

		std::cout << ss.str() << std::endl;

	}

	void log_to_file(const std::string &log) {
		logFile << log << std::endl;
	}
};
