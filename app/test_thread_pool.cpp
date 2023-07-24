#include "ra/thread_pool.hpp"
#include <iostream>

using namespace ra::concurrency;

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
	for( unsigned i = 0; i < 128; ++i )
		test.schedule( [i](){std::cout << i << ' ';} );
	test.shutdown();
	
	return 0;
}

