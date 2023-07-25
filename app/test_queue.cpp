#define GNUMB 1000
#define CATCH_CONFIG_MAIN

#include "ra/queue.hpp"
#include <iostream>
#include <thread>
#include <functional>
#include <catch2/catch.hpp>

using namespace ra::concurrency;

TEMPLATE_TEST_CASE("Test single-threaded behaviour", "[queue]",
		int, unsigned, char, float, std::size_t,
		double, int32_t, int8_t, int64_t, int16_t,
		uint32_t, uint8_t, uint64_t, uint16_t) {
	for( unsigned i = 1; i != 8192; i *= 2 ){
		queue<TestType> test(i);
		CHECK( test.max_size() == i );
		CHECK( test.is_empty() );
		CHECK( !test.is_full() );
		CHECK( !test.is_closed() );
		for( unsigned j = 0; j < i; ++j )
			test.push(TestType(0));
		CHECK( test.is_full() );
		test.clear();
		CHECK( test.is_empty() );
		test.close();
		CHECK( test.is_closed() );
		for( unsigned j = 0; j < i; ++j )
			test.push(TestType(0));
		CHECK( test.is_empty() );
	}
	for( unsigned i = 1; i != 8192; i *= 2){
		queue<TestType> test(i);
		for( unsigned j = 0; j < i; ++j )
			test.push(TestType(0));
		TestType blank {};
		TestType popinto {};
		unsigned k = test.max_size();
		while(k){
			test.pop(popinto);
			CHECK( popinto == blank );
			--k;
		}
		CHECK( test.is_empty() );
	}
}

TEST_CASE( "Test multithreaded behaviour", "[queue]" ){
	std::thread worker1;
	std::thread worker2;
	std::thread worker3;
	std::thread worker4;
	std::thread worker5;
	std::thread worker6;
	std::thread worker7;
	std::thread worker8;
	queue<int> q(1337);
	worker1 = std::move(std::thread([&](){for(int i=0; i<500; ++i) q.push(1);}));
	worker2 = std::move(std::thread([&](){for(int i=0; i<500; ++i) q.push(2);}));
	worker3 = std::move(std::thread([&](){for(int i=0; i<500; ++i) q.push(3);}));
	worker4 = std::move(std::thread([&](){for(int i=0; i<500; ++i) q.push(4);}));
	worker5 = std::move(std::thread([&](){for(int i=0; i<500; ++i){int j; q.pop(j);}}));
	worker6 = std::move(std::thread([&](){for(int i=0; i<500; ++i){int j; q.pop(j);}}));
	worker7 = std::move(std::thread([&](){for(int i=0; i<500; ++i){int j; q.pop(j);}}));
	worker8 = std::move(std::thread([&](){for(int i=0; i<500; ++i){int j; q.pop(j);}}));
	worker1.join();
	worker2.join();
	worker3.join();
	worker4.join();
	worker5.join();
	worker6.join();
	worker7.join();
	worker8.join();
	CHECK( q.is_empty() );
	worker5 = std::move(std::thread([&](){for(int i=0; i<500; ++i){int j; q.pop(j);}}));
	worker6 = std::move(std::thread([&](){for(int i=0; i<500; ++i){int j; q.pop(j);}}));
	worker7 = std::move(std::thread([&](){for(int i=0; i<500; ++i){int j; q.pop(j);}}));
	worker8 = std::move(std::thread([&](){for(int i=0; i<500; ++i){int j; q.pop(j);}}));
	worker1 = std::move(std::thread([&](){for(int i=0; i<500; ++i) q.push(1);}));
	worker2 = std::move(std::thread([&](){for(int i=0; i<500; ++i) q.push(2);}));
	worker3 = std::move(std::thread([&](){for(int i=0; i<500; ++i) q.push(3);}));
	worker4 = std::move(std::thread([&](){for(int i=0; i<500; ++i) q.push(4);}));
	worker1.join();
	worker2.join();
	worker3.join();
	worker4.join();
	worker5.join();
	worker6.join();
	worker7.join();
	worker8.join();
	CHECK ( q.is_empty() );
	worker1 = std::move(std::thread([&](){for(int i=0; i<500; ++i) q.push(1);}));
	worker2 = std::move(std::thread([&](){for(int i=0; i<500; ++i) q.push(2);}));
	worker1.join();
	worker2.join();
	CHECK( (!q.is_empty() && !q.is_full()) );
	worker1 = std::move(std::thread([&](){for(int i=0; i<500; ++i) q.push(1);}));
	worker2 = std::move(std::thread([&](){for(int i=0; i<500; ++i) q.push(2);}));
	worker3 = std::move(std::thread([&](){for(int i=0; i<500; ++i) q.push(3);}));
	worker4 = std::move(std::thread([&](){for(int i=0; i<500; ++i) q.push(4);}));
	worker5 = std::move(std::thread([&](){for(int i=0; i<500; ++i) q.push(2);}));
	worker6 = std::move(std::thread([&](){for(int i=0; i<500; ++i) q.push(3);}));
	worker7 = std::move(std::thread([&](){for(int i=0; i<500; ++i) q.push(4);}));
	worker8 = std::move(std::thread([&](){for(int i=0; i<500; ++i){int j; q.pop(j);}}));
	worker8.join();
	q.close();
	worker1.join();
	worker2.join();
	worker3.join();
	worker4.join();
	worker5.join();
	worker6.join();
	worker7.join();
	CHECK( q.is_closed() );
	queue<int> p(1337);
	worker1 = std::move(std::thread([&](){for(int i=0; i<500; ++i) p.push(1);}));
	worker2 = std::move(std::thread([&](){for(int i=0; i<500; ++i){int j; p.pop(j);}}));
	worker3 = std::move(std::thread([&](){for(int i=0; i<500; ++i){int j; p.pop(j);}}));
	worker4 = std::move(std::thread([&](){for(int i=0; i<500; ++i){int j; p.pop(j);}}));
	worker5 = std::move(std::thread([&](){for(int i=0; i<500; ++i){int j; p.pop(j);}}));
	worker6 = std::move(std::thread([&](){for(int i=0; i<500; ++i){int j; p.pop(j);}}));
	worker7 = std::move(std::thread([&](){for(int i=0; i<500; ++i){int j; p.pop(j);}}));
	worker8 = std::move(std::thread([&](){for(int i=0; i<500; ++i){int j; p.pop(j);}}));
	worker1.join();
	p.close();
	worker2.join();
	worker3.join();
	worker4.join();
	worker5.join();
	worker6.join();
	worker7.join();
	worker8.join();
	CHECK( p.is_closed() );
	CHECK( p.is_empty() );
}




/*
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
*/
