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

	queue( size_type max_size ): storage {}, capacity {max_size}, closed {false} {}

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

	status push( value_type&& x ){
		if( is_closed() ){
			return status::closed;
		}else{
			// Take the mutex
			std::unique_lock halt(qutex);

			// Sleep the thread until there is room to push
			// something into the queue
			pushreq.wait(halt, [this](){return is_closed() || !is_full();});

			if( is_closed() ){
				return status::closed;
			}else{
				// Do the operation
				storage.emplace(x);
				
				// Notify popper
				popreq.notify_one();

				return status::success;
			}
		}
	}

	status pop( value_type& x ){
		if( is_closed() ){
			if( is_empty() ){
				return status::closed;
			}else{
				// Grab the mutex
				std::unique_lock halt(qutex);

				// Place value in x
				x = storage.front();

				// Pop it
				storage.pop();

				// Notify pusher
				pushreq.notify_one();

				return status::success;
			}
		}else{
			// Grab the mutex
			std::unique_lock halt(qutex);

			// Wait until there is something to pop
			popreq.wait(halt, [this](){return !is_empty();});

			// Storage value in x
			x = storage.front();

			// Pop it
			storage.pop();

			// Notify pusher
			pushreq.notify_one();

			return status::success;
		}
	}

	void close(){
		if( !is_closed() ){
			// Grab the mutex
			std::unique_lock halt(qutex);

			// Close the queue
			closed = true;
		}
	}

	void clear(){
		// Grab the mutex
		std::unique_lock halt(qutex);

		// Record size before clearing
		int size = storage.size();

		// Clear the queue
		while( !storage.empty )
			storage.pop();

		// Notify pusher(s)
		while( size != 0 ){
			pushreq.notify_one();
			--size;
		}
	}



	bool is_full() const; { return storage.size() == capacity; }

	bool is_empty() const { return storage.size() == 0; }

	bool is_closed() const { return closed; }

	size_type max_size() const { return capacity; }

private:
	mutable std::mutex qutex;
	mutable std::condition_variable pushreq;
	mutable std::condition_variable popreq;
	std::queue<value_type> storage;
	bool closed;
	const size_type capacity;
};

}

