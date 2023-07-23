#include "ra/thread_pool.hpp"
#include <iostream>

using namespace ra::concurrency;

void create_default(){
	thread_pool a;
	std::cout << a.size() << '\n';
}

void create_parametrized(){
	thread_pool b(8);
	std::cout << b.size() << '\n';
}

void job(){
	std::cout << "workin the job\n";
}

int main(){
	create_default();
	std::cout << "Default thread pool created and destroyed.\n";
	create_parametrized();
	std::cout << "Parametrized thread pool created and destroyed.\n";
	
	return 0;
}

