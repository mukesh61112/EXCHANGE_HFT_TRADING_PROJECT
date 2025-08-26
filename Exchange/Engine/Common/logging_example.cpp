#include "Log.h"  // This includes macro.h indirectly

int main() {
	using namespace Common;
	Logger* logger = Logger::getInstance(); // or std::shared_ptr<Logger>
   LOG_INFO1(logger, "Exchange started.");

   // LOG_INFO1(logger,"Exchange started.");
   // LOG_INFO2("Market data feed active.");
    //LOG_DEBUG("Order book state updated.");
    //LOG_WARN("Price spike detected.");
    //LOG_ERROR("Failed to match order.");
    //LOG_FATAL("Critical failure in matching engine.");

    return 0;
}

