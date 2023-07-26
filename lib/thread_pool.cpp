#include "ra/thread_pool.hpp"

namespace ra::concurrency {

	thread_pool::thread_pool(): shutdown_(false), jobs_(256), pool_ {} {
		unsigned num_supported = std::thread::hardware_concurrency();
		if( num_supported ){
			size_ = num_supported;
			for( num_supported; num_supported; --num_supported ){
				// Initialize threads to wait for a job to appear in
				// the work queue
				pool_.emplace_back( std::thread([this](){

							// Create variable for storing tasks
							std::function<void()> job;
	
							// Main loop
							while( jobs_.pop(job) == queue<std::function<void()>>::status::success ) { job(); }

							}
							) );
			}
		}else{
			num_supported = 2;
			size_ = num_supported;
			for( num_supported; num_supported; --num_supported ){
				// Initialize threads to wait for a job to appear in
				// the work queue
				pool_.emplace_back( std::thread([this](){

							// Create variable for storing tasks
							std::function<void()> job;
	
							// Main loop
							while( jobs_.pop(job) == queue<std::function<void()>>::status::success ) { job(); }

							}
							) );
			}
		}
	}

	thread_pool::thread_pool( std::size_t num_threads ): size_(num_threads), shutdown_(false), jobs_(256), pool_ {} {
		for( unsigned i = num_threads; i; --i ){
			// Initialize threads to wait for a job to appear in
			// the work queue
			pool_.emplace_back( std::thread([this](){

						// Create variable for storing tasks
						std::function<void()> job;

						// Main loop
						while( jobs_.pop(job) == queue<std::function<void()>>::status::success ) { job(); }

						}
						) );
		}
	}

	thread_pool::~thread_pool(){
		if( !is_shutdown() )
			shutdown();
	}

	thread_pool::size_type thread_pool::size() const { return size_; }

	void thread_pool::schedule( std::function<void()>&& func ) { jobs_.push(std::move(func)); }

	void thread_pool::shutdown() {
		// Stop accepting new jobs
		jobs_.close();

		// Grab the mutex
		std::unique_lock lock(m_);

		// Shutdown protocol
		if( !is_shutdown() ){
			// Join all threads
			for( unsigned i = 0; i < pool_.size(); ++i ) { pool_.at(i).join(); }

			// Place pool into shutdown state
			shutdown_ = true;
		}
	}

	bool thread_pool::is_shutdown() const { return shutdown_; }
}

