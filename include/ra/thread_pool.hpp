#include "ra/queue.hpp"
#include <thread>

namespace ra::concurrency {

class thread_pool
{
public:
	using size_type = std::size_t;

	thread_pool();

	thread_pool( std::size_t num_threads );

	thread_pool( const thread_pool& ) = delete;
	thread_pool& operator=( const thread_pool& ) = delete;
	thread_pool( thread_pool&& ) = delete;
	thread_pool& operator=( thread_pool&& ) = delete;

	~thread_pool();

	size_type size() const;

	void schedule( std::function<void()>&& func );

	void shutdown();

	bool is_shutdown() const;
private:
	bool off_;
	size_type size_;
	queue<std::thread> pool_;
};

}

