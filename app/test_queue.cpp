#include "ra/queue.hpp"
#include <iostream>
#include <thread>

using namespace ra::concurrency;

int main(){
	queue<int> intq(3);
	queue<float> floatq(3);
	queue<char> charq(3);

	return 0;
}

