#define GNUMB 1000

#include "ra/queue.hpp"
#include <iostream>
#include <thread>
#include <functional>

using namespace ra::concurrency;

int main() {
	queue<int> q(256);
	std::thread worker1( [&](){for(int i = 0; i < GNUMB; ++i){int j = i; q.push(std::move(j));}} );
	std::cout << "Initiate worker 1\n.";
	std::thread worker2( [&](){for(int i = 0; i < GNUMB; ++i){int j; q.pop(j);}} );
	std::cout << "Initiate worker 2\n.";
	std::thread worker3( [&](){for(int i = 0; i < GNUMB; ++i){int j = i; q.push(std::move(j));}} );
	std::cout << "Initiate worker 3\n.";
	std::thread worker4( [&](){for(int i = 0; i < GNUMB; ++i){int j; q.pop(j);}} );
	std::cout << "Initiate worker 4\n.";
	std::thread worker5( [&](){for(int i = 0; i < GNUMB; ++i){int j = i; q.push(std::move(j));}} );
	std::cout << "Initiate worker 5\n.";
	q.close();
	std::cout << "First close.\n";
	std::thread worker6( [&](){for(int i = 0; i < GNUMB; ++i){int j; q.pop(j);}} );
	std::cout << "Initiate worker 6\n.";
	q.close();
	std::cout << "Second close.\n";
	std::thread worker7( [&](){for(int i = 0; i < GNUMB; ++i){int j = i; q.push(std::move(j));}} );
	std::cout << "Initiate worker 7\n.";
	q.close();
	std::cout << "Third close.\n";
	std::thread worker8( [&](){for(int i = 0; i < GNUMB; ++i){int j; q.pop(j);}} );
	std::cout << "Initiate worker 8\n.";
	q.close();
	std::cout << "Fourth close.\n";
	worker1.join();
	std::cout << "Worker 1 joined.\n";
	worker2.join();
	std::cout << "Worker 2 joined.\n";
	worker3.join();
	std::cout << "Worker 3 joined.\n";
	worker4.join();
	std::cout << "Worker 4 joined.\n";
	worker5.join();
	std::cout << "Worker 5 joined.\n";
	worker6.join();
	std::cout << "Worker 6 joined.\n";
	worker7.join();
	std::cout << "Worker 7 joined.\n";
	worker8.join();
	std::cout << "Worker 8 joined.\n";
	return 0;
}

