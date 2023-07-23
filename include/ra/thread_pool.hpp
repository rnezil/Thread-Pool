#include "ra/queue.hpp"
#include <functional>

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

	void cleanup();
private:
	size_type size_;
	queue<std::function<void()>> jobs_;
	std::vector<std::thread> pool_;
	bool shutdown_;
	std::mutex m_;
	std::condition_variable c_;
	std::vector<bool> thread_is_idle_;
	int current_;
};

}

