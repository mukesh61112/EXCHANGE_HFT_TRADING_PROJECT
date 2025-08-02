//============================================================================
// Name        : ThreadPool.cpp
// Author      : Mukesh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include<thread>
#include<chrono>
#include<mutex>
#include<condition_variable>
#include<vector>
#include<queue>
using namespace std;

std::vector<std::thread> VectorThreadPool;
std::queue<std::pair<void(*)(int,int),int>>TaskQueue;
//std::queue<int> TaskQueue;
std::condition_variable cv;
bool check = true;
std::mutex mtx;

void Work1(int i,int threadId)
{
	std::cout<<"Work1 on Thread:-"<<threadId<<"  ,Working on Task:-"<<i<<endl;
	std::this_thread::sleep_for(std::chrono::seconds(10));
	return;
}
void Work2(int i,int threadId)
{
	std::cout<<"Work2 on Thread:-"<<threadId<<"  ,Working on Task:-"<<i<<endl;
	std::this_thread::sleep_for(std::chrono::seconds(10));
	return;
}
void Handle(int threadId)
{

	while(true)
	{
		//void(*FPtr)(int) = nullptr;
		std::unique_lock<std::mutex>lock(mtx);
		cv.wait(lock,[]{return !check || !TaskQueue.empty();});
		if(!check && TaskQueue.empty())
			return;
		//FPtr = TaskQueue.front();
		void (*FPtr)(int,int) = nullptr;
		//int i = TaskQueue.front();
		std::pair<void(*)(int,int),int> pr = TaskQueue.front();
		FPtr = pr.first;
		int i = pr.second;
		TaskQueue.pop();
		lock.unlock();
		FPtr(i,threadId);
	}
}

int main() {
	for(int i=1;i<=5;i++){
		std::cout<<"Thread:-"<<i<<" Created"<<endl;
		VectorThreadPool.push_back(std::thread(Handle,i));
	}
	for(int i=1;i<=20;i++)
	{
		{lock_guard<std::mutex>lock(mtx);
		std::cout<<"Task:-"<<i<<" Added"<<endl;
		if(i%2==0)
		{
			void (*FPtr)(int,int) = Work2;
			std::pair<void(*)(int,int),int> pr = std::make_pair(FPtr,i);
			TaskQueue.push(pr);
		}
		else
		{
			void (*FPtr)(int,int) = Work1;
			std::pair<void(*)(int,int),int> pr = std::make_pair(FPtr,i);
			TaskQueue.push(pr);		}
		cv.notify_one();
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	std::this_thread::sleep_for(std::chrono::seconds(15));
	{
		lock_guard<std::mutex>lock(mtx);
		check = false;
		cv.notify_all();
	}
	for(int i=1;i<=5;i++)
	{
		std::cout<<"Thread:-"<<i<<" Deleted"<<endl;
		VectorThreadPool[i].join();
	}
	return 0;
}
