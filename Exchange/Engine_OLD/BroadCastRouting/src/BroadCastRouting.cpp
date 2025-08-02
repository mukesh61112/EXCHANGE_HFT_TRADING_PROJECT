#include"../header/BroadCastRouting.h"
#include"../header/KafkaProducer.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include<csignal>
#include <iomanip>
#include <sstream>
#include <ctime>
#include<chrono>
#include<queue>
#include<condition_variable>
#include<mutex>
#include<thread>
#include<atomic>
std::queue<StockMarketData> message_queue;
std::mutex mtx;
std::condition_variable cv;
std::vector<std::thread> WorkerThread;

using namespace std;

std::atomic<bool> running(true);

void handle_sigint(int) {
	std::cout << "\n[SERVER] Gracefully shutting down.\n";
	running = false;
}
std::string to_json(const StockMarketData& data) {
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(2); // optional: format price
	oss << "{"
	    << "\"symbol\":\"" << data.cstrSymbol << "\","
	    << "\"price\":" << data.dstrPrice << ","
	    << "\"volume\":" << data.istrVolume << ","
	    << "\"timestamp\":" << data.istrTimestamp
	    << "}";
	return oss.str();
}
void kafka_sender_thread(KafkaProducer& producer) {
	while (running) {
		std::unique_lock<std::mutex> lock(mtx);
		cv.wait(lock, [] { return !message_queue.empty() || !running; });

		if (!running && message_queue.empty()) return;

		if (!message_queue.empty()) {
			StockMarketData msg = message_queue.front();
			std::stringstream ss;
			ss << "{ \"symbol\": \"" << msg.cstrSymbol << "\", \"price\": " << msg.dstrPrice << " }";
			message_queue.pop();
           // producer.send(reinterpret_cast<const char*>(&msg), sizeof(StockMarketData));
			//producer.send(ss.str());
		}
		lock.unlock();
	}
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
}int main() {
	signal(SIGINT, handle_sigint); // Handle Ctrl+C

	KafkaProducer producer("localhost:9092", "marketdata");

	//std::thread send_thread(kafka_sender_thread, std::ref(producer));
//	int n = 5, m= 15;
//		for(int i=1;i<=n;i++)
//		{
//			WorkerThread.push_back(std::thread(kafka_sender_thread,std::ref(producer)));
//			std::cout<<"Thread Created:-"<<i<<std::endl;
//		}
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		perror("\n Client Socket creation failed");
		return 1;
	}
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9000);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
		perror("Client Bind failed");
		close(sock);
		return 1;
	}
	std::cout << "Client Listening on port 9000...\n";

	StockMarketData data{};
	std::cout<<"\n Current Time:-"<<getCurrentTimeWithMicroseconds();
	while (running) {
		ssize_t len = recv(sock, &data, sizeof(data), 0);
		if (len == sizeof(data)) {
			std::cout << "Received: " << data.cstrSymbol << " " << data.dstrPrice << " " << data.istrVolume << " " << data.istrTimestamp << "\n";
//			{
//				std::lock_guard<std::mutex> lock(mtx);
//				message_queue.push(data);
//			    cv.notify_one();
//			}
			std::stringstream ss;
						ss << "{ \"symbol\": \"" << data.cstrSymbol << "\", \"price\": " << data.dstrPrice << " }";
			           // producer.send(reinterpret_cast<const char*>(&data), sizeof(StockMarketData));
						//producer.send(ss.str());
						std::string json = to_json(data);

						producer.send(json);
		}
		else if (len < 0) {
			perror("Client recv() error");
		} else {
			std::cerr << "Client Incomplete packet: received " << len << " bytes\n";
		}
	}
	close(sock);
	running = false;
	cv.notify_all();
//	for(int i=0;i<n;i++)
//		{		std::cout<<"Thread Destroyed:-"<<i<<std::endl;
//			WorkerThread[i].join();
//		}
	std::cout<<"\n Current Time:-"<<getCurrentTimeWithMicroseconds();
	return 0;
}
