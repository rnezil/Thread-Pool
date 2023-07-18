#include <threads>
#include <cstddef>
#include <mutex>
#include <queue>
#include <condition_variable>

namespace ra::concurrency {

template <class T>
class queue
{
public:

	// Type of each element stored in the queue
	using value_type = T;

	// Unsigned integral type used to represent sizes
	using size_type = std::size_t;

	// Type for status of a queue operation
	enum class status {
		success = 0, // operation successful
		empty, // queue is empty (not currently used)
		full, // queue is full (not currently used)
		closed, // queue is closed
	};

	queue() = delete;

	queue( size_type max_size ): storage {},
		size {0}, capacity {max_size}, closed {false} {}

	queue( const queue& ) = delete;
	queue& operator=( const queue& ) = delete;
	queue( queue&& ) = delete;
	queue& operator=( queue&& ) = delete;

	~queue(){
		if( !closed )
			closed = true;

		while( !storage.empty() )
			storage.pop();
	}

	status push( value_type&& x );

	status pop( value_type& x );

	void close();

	void clear();

	bool is_full() const;

	bool is_empty() const;

	bool is_closed() const;

	size_type max_size() const;

private:
	std::mutex qutex;
	std::queue<value_type> storage;
	bool closed;
	size_type size;
	size_type capacity;
};

}

