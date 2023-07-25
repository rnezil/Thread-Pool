#include <cstddef>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <utility>

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
		// Close queue if not already closed
		if( !is_closed() ){
			close();
		}

		// Clear out underlying data structure
		while( !storage.empty() ){
			storage.pop();
		}
	}

	status push( value_type&& x ){
		// Take the mutex
		std::unique_lock halt(qutex);

		if( is_closed() ){
			return status::closed;
		}else{
			if( is_full() ){ 
				// If queue is empty, sleep the thread until
				// either the queue closes or there is room
				// to push something into the queue
				pushreq.wait(halt, [this](){return is_closed() || !is_full();});
			}

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
		// Grab the mutex
		std::unique_lock halt(qutex);

		if( is_closed() ){
			if( is_empty() ){
				return status::closed;
			}else{
				// Place value in x
				x = std::move(storage.front());

				// Pop it
				storage.pop();

				return status::success;
			}
		}else{
			if( is_empty() ){
				// If queue is empty, wait until
				// there is something to pop
				popreq.wait(halt, [this](){return !is_empty() || is_closed();});

				// If the queue has been closed
				// since the wait began and is
				// empty then just leave
				if( is_empty() && is_closed() )
					return status::closed;
			}

			// Storage value in x
			x = std::move(storage.front());

			// Pop it
			storage.pop();

			// Notify pusher
			pushreq.notify_one();

			return status::success;
		}
	}

	void close(){
		// Grab the mutex
		std::unique_lock halt(qutex);

		if( !is_closed() ){
			// Close the queue
			closed = true;

			// Notify anyone waiting to pop
			// that the queue is now closed
			popreq.notify_all();

			// Notify anyone trying to push
			// that the queue is now closed
			pushreq.notify_all();
		}
	}

	void clear(){
		// Grab the mutex
		std::unique_lock halt(qutex);

		// Record size before clearing
		int size = storage.size();

		// Clear the queue
		while( !storage.empty() )
			storage.pop();

		// Notify pusher(s)
		while( size != 0 ){
			pushreq.notify_one();
			--size;
		}
	}



	bool is_full() const { return storage.size() == capacity; }

	bool is_empty() const { return !storage.size(); }

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

