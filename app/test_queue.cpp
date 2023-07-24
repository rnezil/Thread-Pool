#define GNUMB 1000

#include "ra/queue.hpp"
#include <iostream>
#include <thread>
#include <functional>

using namespace ra::concurrency;

int main() {
	queue<int> q(256);
	std::thread worker1( [&](){for(int i = 0; i < GNUMB; ++i){int j = i; q.push(std::move(j));}} );
	std::thread worker2( [&](){for(int i = 0; i < GNUMB; ++i){int j; q.pop(j);}} );
	std::thread worker3( [&](){for(int i = 0; i < GNUMB; ++i){int j = i; q.push(std::move(j));}} );
	std::thread worker4( [&](){for(int i = 0; i < GNUMB; ++i){int j; q.pop(j);}} );
	std::thread worker5( [&](){for(int i = 0; i < GNUMB; ++i){int j = i; q.push(std::move(j));}} );
	q.close();
	std::thread worker6( [&](){for(int i = 0; i < GNUMB; ++i){int j; q.pop(j);}} );
	q.close();
	std::thread worker7( [&](){for(int i = 0; i < GNUMB; ++i){int j = i; q.push(std::move(j));}} );
	q.close();
	std::thread worker8( [&](){for(int i = 0; i < GNUMB; ++i){int j; q.pop(j);}} );
	q.close();
	worker1.join();
	worker2.join();
	worker3.join();
	worker4.join();
	worker5.join();
	worker6.join();
	worker7.join();
	worker8.join();
	return 0;
}

