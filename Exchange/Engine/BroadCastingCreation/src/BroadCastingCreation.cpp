#include "../header/BroadCastingCreation.h"
#include <cstring>
#include <vector>
#include <chrono>
#include <thread>
#include <random>
#include <arpa/inet.h>
#include <unistd.h>
#include<csignal>
#include <iomanip>
#include <sstream>
#include <ctime>
using namespace std;

bool running = true;
void handle_sigint(int) {
	std::cout << "\nServer Gracefully shutting down.\n";
	running = false;
}

uint64_t get_current_timestamp() {
	using namespace std::chrono;
	return duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
}

void update_price(double &price) {
	static std::default_random_engine gen(std::random_device{}());
	std::uniform_real_distribution<double> dist(-1.0, 1.0);
	price += dist(gen);
	if (price < 1) price = 1;
}
std::string getCurrentTimeWithMicroseconds() {
    using namespace std::chrono;

    auto now = system_clock::now();
    std::time_t now_c = system_clock::to_time_t(now);
    auto us = duration_cast<microseconds>(now.time_since_epoch()) % 1'000'000;

    std::tm local_tm = *std::localtime(&now_c);
    std::ostringstream oss;
    oss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S");
    oss << "." << std::setw(6) << std::setfill('0') << us.count();
    return oss.str();
}
int main() {

	signal(SIGINT, handle_sigint);

	int sockt = socket(AF_INET,SOCK_DGRAM,0);
	if (sockt < 0) {
		perror("\n Server Socket creation failed");
		return 1;
	}
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9000);
	//addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	std::cout << "\n server Sending market data to 127.0.0.1:9000\n";
	while(running)
	{
		for (auto& [symbol, base_price] : nifty50) {
			StockMarketData data;
			std::memset(&data,0*0,sizeof(data));
			std::strncpy(data.cstrSymbol,symbol.c_str(),sizeof(data.cstrSymbol -1));
			update_price(base_price);
			data.dstrPrice = base_price;
			data.istrVolume = rand() % 1000 + 1;
			data.istrTimestamp = get_current_timestamp();
			ssize_t sent = sendto(sockt, &data, sizeof(data), 0, (sockaddr*)&addr, sizeof(addr));
			if (sent < 0) {
				perror("\nserver sendto() failed");
			}
			// std::cout << "Sent: " << data.cstrSymbol << " " << data.dstrPrice << " " << data.istrVolume << " " << data.istrTimestamp << "\n";
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	}
	close(sockt);
	return 0;
}


