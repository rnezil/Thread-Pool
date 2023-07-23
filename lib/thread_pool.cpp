#include "ra/thread_pool.hpp"

namespace ra::concurrency {

	thread_pool::thread_pool(): shutdown_(false), joining_(false), jobs_(256), pool_ {},
	       clean_(false), shutting_down_(false) {
		unsigned num_supported = std::thread::hardware_concurrency();
		if( num_supported ){
			size_ = num_supported;
			for( num_supported; num_supported; --num_supported )
				pool_.emplace_back( std::thread() );
		}else{
			size_ = 2;
			pool_.emplace_back( std::thread() );
			pool_.emplace_back( std::thread() );
		}
	}

	thread_pool::thread_pool( std::size_t num_threads ): size_(num_threads),
	shutdown_(false), joining_(false), jobs_(256), pool_ {},
	clean_(false), shutting_down_(false) {
		for( unsigned i = num_threads; i; --i )
			pool_.emplace_back( std::thread() );
	}

	thread_pool::~thread_pool(){
		if( !is_shutdown() )
			shutdown();
	}

	thread_pool::size_type thread_pool::size() const { return size_; }

	void thread_pool::schedule( std::function<void()>&& func ) {
		// This function automatically
		// blocks if the job queue is
		// full. Therefore, schedule
		// function itself effectively
		// blocks if job queue is full
		jobs_.push( std::move(func) );

		// Grab the mutex
		std::unique_lock lock(m_);

		// Assign as many jobs as possible
		for( unsigned i = 0; i < size_; ++i ){
			if( !pool_.at(i).joinable() ){
				// Found a free thread
				std::function<void()> job;
				jobs_.pop(job); 
				pool_.at(i) = std::thread(job);
				return;
			}
		}

		// All worker threads are busy if this
		// point is reached, so let whatever
		// thread gets here first take on the
		// responsibility of freeing all the
		// worker threads
		if( !joining_ ){
			joining_ = true;
			lock.unlock();
			for( unsigned i = 0; i < size_; ++i )
				if( pool_.at(i).joinable() )
					pool_.at(i).join();
			lock.lock();
			joining_ = false;
		}
	}

	void thread_pool::shutdown() {
		// Stop accepting new jobs
		jobs_.close();

		// Grab the mutex
		std::unique_lock lock(m_);

		// Shutdown protocol
		if( !shutting_down_ ){
			// Indicate that thread pool is
			// in the process of shutting down
			shutting_down_ = true;

			// Temporary variables
			bool cleanup_worker_available = false;
			unsigned cleanup_worker_number;

			// Block while worker cleans up
			for( unsigned i = 0; i < size_; ++i ){
				if( !pool_.at(i).joinable() ){
					pool_.at(i) = std::thread(
							&thread_pool::cleanup, this);
					cleanup_worker_available = true;
					cleanup_worker_number = i;
					break;
				}
			}

			if( !cleanup_worker_available ){
				cleanup_worker_number = 0;
				pool_.at(0).join();
				pool_.at(0) = std::thread(
						&thread_pool::cleanup, this);
			}

			c_.wait(lock, [this](){return is_clean();});

			// Shutdown cleanup worker thread
			pool_.at(cleanup_worker_number).join();

			// Place thread pool in shutdown state
			shutdown_ = true;
		}
	}

	bool thread_pool::is_shutdown() const { return shutdown_; }

	void thread_pool::cleanup() {
		// Complete all jobs
		unsigned i = 1;
		while( !jobs_.is_empty() ){
			if( !pool_.at(i).joinable() ){
				std::function<void()> job;
				jobs_.pop(job);
				pool_.at(i) = std::thread(job);
			}else{
				pool_.at(i).join();
			}

			++i;
			if( i == size_ )
				i = 1;
		}

		// Join all workers
		i = 1;
		for( i; i < size_; ++i ){
			if( pool_.at(i).joinable() ){
				pool_.at(i).join();
			}
		}

		// Indicate cleanliness and exit
		clean_ = true;
		c_.notify_all();
	}

	bool thread_pool::is_clean() const { return clean_; }
}

