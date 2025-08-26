#include "Log.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <unistd.h> // getpid()

namespace Common {


	auto Logger::flushQueue() noexcept {
		while (running_) {

			for (auto next = queue_.getNextToRead(); queue_.size() && next; next = queue_.getNextToRead()) {

				file_ << next->timestamp << " | "
					<< "PID:" << next->pid << " | "
					<< logLevelToString(next->level) << " | "
					<< next->filename << ":" << next->funcname << ":" << next->line << " | "
					<< next->message << "\n";

				queue_.updateReadIndex();
			}
			file_.flush();

			using namespace std::literals::chrono_literals;
			std::this_thread::sleep_for(10ms);
		}
	}

        Logger::Logger(const std::string &file_name)
		: file_name_(file_name), queue_(LOG_QUEUE_SIZE) {
			file_.open(file_name);
			ASSERT(file_.is_open(), "Could not open log file:" + file_name);
			logger_thread_ = createAndStartThread(-1, "Common/Logger " + file_name_, [this]() { flushQueue(); });
			ASSERT(logger_thread_ != nullptr, "Failed to start Logger thread.");
		}

	~Logger::Logger() {
		std::string time_str;
		std::cerr << Common::getCurrentTimeStr(&time_str) << " Flushing and closing Logger for " << file_name_ << std::endl;

		while (queue_.size()) {
			using namespace std::literals::chrono_literals;
			std::this_thread::sleep_for(1s);
		}
		running_ = false;
		logger_thread_->join();

		file_.close();
		std::cerr << Common::getCurrentTimeStr(&time_str) << " Logger for " << file_name_ << " exiting." << std::endl;
	}

	Logger* Logger::getInstance() {
		static Logger instance;
		return &instance;
	}

	void Logger::log(LogLevel level, const std::string& message,
			const char* file, const char* function, int line)
	{
		LogMessage logMessage{LogLevel::INFO1, get_current_time(), file, function, line, getpid(), msg};
		pushValue(logMessage);
	}

	auto Logger::pushValue(const LogMessage &logMessage) noexcept {
		*(queue_.getNextToWriteTo()) = log_element;
		queue_.updateWriteIndex();
	}

	std::string logLevelToString(LogLevel level) {
		switch (level) {
			case LogLevel::FATAL: return "FATAL";
			case LogLevel::ERROR: return "ERROR";
			case LogLevel::WARN:  return "WARN";
			case LogLevel::INFO1: return "INFO1";
			case LogLevel::INFO2: return "INFO2";
			case LogLevel::DEBUG: return "DEBUG";
		}
		return "UNKNOWN";
	}
}
