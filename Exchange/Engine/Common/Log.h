#pragma once
#include <string> 
#include "macros.h"
#include "lf_queue.h"
#include "thread_utils.h"
#include "time_utils.h"
//#include "enum.h"
//#include "struct.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <mutex>
#include <unistd.h> // getpid()

namespace Common{
	enum class LogLevel {
		FATAL = 0,   // System is unusable, must exit
		ERROR = 1,   // Serious error, needs attention
		WARN  = 2,   // Warning, not critical but abnormal
		INFO1 = 3,   // General information (important)
		INFO2 = 4,   // Less important informational messages
		DEBUG = 5    // Debug messages for developers
	};

#pragma once

#include <string>
#include <chrono>
#include <unistd.h> // for getpid()

	struct LogMessage {
		std::chrono::system_clock::time_point timestamp;
		int pid;
		LogLevel level;
		std::string message;
		std::string filename;
		std::string funcname;
		int line;

		LogMessage(LogLevel lvl,
				const std::string& msg,
				const std::string& file,
				const std::string& func,
				int lineNum)
			: timestamp(std::chrono::system_clock::now()),
			pid(getpid()),
			level(lvl),
			message(msg),
			filename(file),
			funcname(func),
			line(lineNum) {}
	};

	class Logger final{
		public:

			size_t LOG_QUEUE_SIZE = 8 * 1024 * 1024;
			static Logger* getInstance();
			void log(LogLevel level, const std::string& message,
					const char* file, const char* function, int line); 



			~Logger(){}

		private:			

			Logger(const Logger &) = delete;

			Logger(const Logger &&) = delete;

			Logger &operator=(const Logger &) = delete;

			Logger &operator=(const Logger &&) = delete;
			Logger();                              // private constructor

			Logger(const std::string& file_name);
			/// File to which the log entries will be written.
			const std::string file_name_;
			std::ofstream file_;

			/// Lock free queue of log elements from main logging thread to background formatting and disk writer thread.
			LFQueue<struct LogMessage> queue_;
			std::atomic<bool> running_ = {true};
			auto flushQueue() noexcept -> void;                    // add this
			auto pushValue(const LogMessage&) noexcept -> void;  
			/// Background logging thread.
			std::thread *logger_thread_ = nullptr;
	};


	//#define LOG_INTERNAL(levelName, msg, file, func, line) 
	//        Logger::getInstance().log(LogLevel::levelName, msg, file, func, line)
	//#define LOG_FATAL(msg) LOG_INTERNAL(FATAL, msg, __FILE__, __func__, __LINE__)
	//#define LOG_ERROR(msg) LOG_INTERNAL(ERROR, msg, __FILE__, __func__, __LINE__)
	//#define LOG_WARN(msg)  LOG_INTERNAL(WARN,  msg, __FILE__, __func__, __LINE__)
	//#define LOG_INFO1(msg) LOG_INTERNAL(INFO1, msg, __FILE__, __func__, __LINE__)
	//#define LOG_INFO2(msg) LOG_INTERNAL(INFO2, msg, __FILE__, __func__, __LINE__)
	//#define LOG_DEBUG(msg) LOG_INTERNAL(DEBUG, msg, __FILE__, __func__, __LINE__)

#define LOG_PTR(ptr, levelName, msg) \
	ptr->log(LogLevel::levelName, msg, __FILE__, __func__, __LINE__)

#define LOG_FATAL(ptr, msg)   LOG_PTR(ptr, FATAL,  msg)
#define LOG_ERROR(ptr, msg)   LOG_PTR(ptr, ERROR,  msg)
#define LOG_WARN(ptr, msg)    LOG_PTR(ptr, WARN,   msg)
#define LOG_INFO1(ptr, msg)   LOG_PTR(ptr, INFO1,  msg)
#define LOG_INFO2(ptr, msg)   LOG_PTR(ptr, INFO2,  msg)
#define LOG_DEBUG(ptr, msg)   LOG_PTR(ptr, DEBUG,  msg)



}
