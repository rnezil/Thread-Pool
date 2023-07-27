#define CATCH_CONFIG_MAIN

#include "ra/thread_pool.hpp"
#include <cassert>
#include <set>
#include <catch2/catch.hpp>

constexpr std::size_t q1 = 420;
constexpr std::size_t q2 = 69;
constexpr std::size_t s1 = 16384;
constexpr std::size_t s2 = 13370;

using namespace ra::concurrency;

TEST_CASE( "Test thread pool with default construction", "[thread_pool]" ){
	thread_pool tp;
	queue<unsigned> numbers(q1);
	std::set<unsigned> trans {};
	CHECK( trans.empty() );
	std::mutex m;
	CHECK( numbers.is_empty() );
	for( unsigned i = 0; i < s1; ++i ){
		tp.schedule( [&numbers, &trans, i, &m](){
				unsigned tmp;
				numbers.pop(tmp);
				std::unique_lock lock(m);
				trans.insert(tmp);
				} );
		tp.schedule( [&numbers, i](){unsigned j = i; numbers.push(std::move(j));} );
		
	}
	tp.shutdown();
	for( unsigned i = 0; i < s1; ++i )
		CHECK( trans.contains(i) );
	CHECK( tp.is_shutdown() );
	CHECK( numbers.is_empty() );
	thread_pool t1;
	queue<unsigned> number1(q2);
	trans.clear();
	CHECK( trans.empty() );
	CHECK( number1.is_empty() );
	for( unsigned i = 0; i < s2; ++i ){
		t1.schedule( [&number1, &trans, i, &m](){
				unsigned tmp;
				number1.pop(tmp);
				std::unique_lock lock(m);
				trans.insert(tmp);
				} );
		t1.schedule( [&number1, i](){unsigned j = i; number1.push(std::move(j));} );
		
	}
	t1.shutdown();
	for( unsigned i = 0; i < s2; ++i )
		CHECK( trans.contains(i) );
	CHECK( t1.is_shutdown() );
	CHECK( number1.is_empty() );
}

TEST_CASE( "Test thread pool with parametrized construction", "[thread_pool]" ){
	thread_pool tp(128);
	queue<unsigned> numbers(q1);
	std::set<unsigned> trans {};
	CHECK( trans.empty() );
	std::mutex m;
	CHECK( numbers.is_empty() );
	for( unsigned i = 0; i < s1; ++i ){
		tp.schedule( [&numbers, &trans, i, &m](){
				unsigned tmp;
				numbers.pop(tmp);
				std::unique_lock lock(m);
				trans.insert(tmp);} );
		tp.schedule( [&numbers, i](){unsigned j = i; numbers.push(std::move(j));} );
		
	}
	tp.shutdown();
	for( unsigned i = 0; i < s1; ++i )
		CHECK( trans.contains(i) );
	CHECK( tp.is_shutdown() );
	CHECK( numbers.is_empty() );
	thread_pool t1(7);
	queue<unsigned> number1(q2);
	trans.clear();
	CHECK( trans.empty() );
	CHECK( number1.is_empty() );
	for( unsigned i = 0; i < s2; ++i ){
		t1.schedule( [&number1, &trans, i, &m](){
				unsigned tmp;
				number1.pop(tmp);
				std::unique_lock lock(m);
				trans.insert(tmp);} );
		t1.schedule( [&number1, i](){unsigned j = i; number1.push(std::move(j));} );
		
	}
	t1.shutdown();
	for( unsigned i = 0; i < s2; ++i )
		CHECK( trans.contains(i) );
	CHECK( t1.is_shutdown() );
	CHECK( number1.is_empty() );
}

TEST_CASE( "Test thread pool further with parametrized construction", "[thread_pool]" ){
	thread_pool tp(50);
	queue<unsigned> numbers(q1);
	std::set<unsigned> trans {};
	CHECK( trans.empty() );
	std::mutex m;
	CHECK( numbers.is_empty() );
	for( unsigned i = 0; i < s1; ++i ){
		tp.schedule( [&numbers, &trans, i, &m](){
				unsigned tmp;
				numbers.pop(tmp);
				std::unique_lock lock(m);
				trans.insert(tmp);
				} );
		tp.schedule( [&numbers, i](){unsigned j = i; numbers.push(std::move(j));} );
		
	}
	tp.shutdown();
	for( unsigned i = 0; i < s1; ++i )
		CHECK( trans.contains(i) );
	CHECK( tp.is_shutdown() );
	CHECK( numbers.is_empty() );
	thread_pool t1(5);
	queue<unsigned> number1(q2);
	trans.clear();
	CHECK( trans.empty() );
	CHECK( number1.is_empty() );
	for( unsigned i = 0; i < s2; ++i ){
		t1.schedule( [&number1, &trans, i, &m](){
				unsigned tmp;
				number1.pop(tmp);
				std::unique_lock lock(m);
				trans.insert(tmp);} );
		t1.schedule( [&number1, i](){unsigned j = i; number1.push(std::move(j));} );
		
	}
	t1.shutdown();
	for( unsigned i = 0; i < s2; ++i )
		CHECK( trans.contains(i) );
	CHECK( t1.is_shutdown() );
	CHECK( number1.is_empty() );
}




/*
void create_default(){
	thread_pool a;
}

void create_parametrized(){
	thread_pool b(8);
}

int main(){
	create_default();
	std::cout << "Default thread pool created and destroyed.\n";
	create_parametrized();
	std::cout << "Parametrized thread pool created and destroyed.\n";
	thread_pool test;
	for( unsigned i = 0; i < 4000; ++i )
		test.schedule( [i](){std::cout << i << '\n';} );
	test.shutdown();
	
	return 0;
}*/

