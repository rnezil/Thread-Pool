#include "ra/thread_pool.hpp"

namespace ra::concurrency {

	thread_pool::thread_pool(): pool_(128), size_(0), off_(false) {
		unsigned num_supported = std::thread::hardware_concurrency();
		if( num_supported ){
			size_ = num_supported;
			for( num_supported; num_supported; --num_supported )
				pool_.push( std::thread() );
		}else{
			size_ = 2;
			pool_.push( std::thread() );
			pool_.push( std::thread() );
		}
	}

	thread_pool::thread_pool( std::size_t num_threads ): pool_(num_threads), size_(num_threads), off_(false) {
		for( unsigned i = num_threads; i; --i )
			pool_.push(std::thread());
	}

	thread_pool::~thread_pool();

	thread_pool::size_type size() const { return size_; }

	thread_pool::void schedule( std::function<void()>&& func );

	thread_pool::void shutdown();

	thread_pool::bool is_shutdown() const;
}

