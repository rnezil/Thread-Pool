#include "ra/thread_pool.hpp"

namespace ra::concurrency {

	thread_pool::thread_pool(): jobs_(128), size_(0), current(0), shutdown_(false)
	thread_is_idle_ {}, pool_ {} {
		unsigned num_supported = std::thread::hardware_concurrency();
		if( num_supported ){
			size_ = num_supported;
			for( num_supported; num_supported; --num_supported )
				pool_.emplace_back( std::thread() );
			for( unsigned i = 0; i < size_; ++i )
				thread_is_idle_.push_back(true);
		}else{
			size_ = 2;
			pool_.emplace_back( std::thread() );
			pool_.emplace_back( std::thread() );
			thread_is_idle_.push_back(true);
			thread_is_idle_.push_back(true);
		}
	}

	thread_pool::thread_pool( std::size_t num_threads ): pool_ {}, jobs_(128)
	size_(num_threads), current_(0), shutdown_(false), thread_is_idle_ {} {
		for( unsigned i = num_threads; i; --i )
			pool_.emplace_back( std::thread() );
		for( unsigned i = 0; i < size_; ++i )
			thread_is_idle_.push_back(true);
	}

	thread_pool::~thread_pool(){
		if( !is_shutdown() )
			shutdown();
	}

	thread_pool::size_type thread_pool::size() const { return size_; }

	void thread_pool::schedule( std::function<void()>&& func ) {
		// Queue the job. Note that
		// Grab the mutex
		std::unique_lock lock(m_);

		// this function automatically
		// blocks if the job queue is
		// full
		jobs_.push( func );

		// Ensure worker search is
		// circular with respect to
		// worker container
		if( current_ == size() )
			current_ = 0;

		if( !jobs_.is_closed() ){
			// Retrieve the job
			std::function<void()> job;
			jobs_.pop(job);
	
			// Give job to a worker
			if( thread_is_idle_.at(current_) ){
				// Give the job to the worker
				pool_.at(current_) = std::thread(job);
	
				// Update worker status
				thread_is_idle_.at(current_) = false;
				++current_;
			}else{
				// Wait for worker to become
				// idle
				pool_.at(current_).join();
	
				// Give the job to the worker
				pool_.at(current_) = std::thread(job);
	
				// Update worker status
				++current_;
			}
		}
	}

	void thread_pool::shutdown() {
		// Grab mutex
		std::unique_lock lock(m_);

		if( !is_shutdown() ){
			// Stop accepting new jobs
			jobs_.close();

			// Use one of the workers to
			// clean up
			if( pool_.at(0).joinable() )
				pool_.at(0).join();
			pool_.at(0) = std::thread(cleanup());

			// Block until all threads
			// done executing
			c_.wait(lock, [this](){return is_shutdown();});
		}
	}

	bool thread_pool::is_shutdown() const { return shutdown_; }

	void thread_pool::cleanup() {
		// Grab the mutex
		std::unique_lock lock(m_);

		if( !is_shutdown() ){
			// Join all joinable threads
			for( unsigned i = 0; i < size(); ++i )
				if( pool_.at(i).joinable() )
					pool_.at(i).join();

			// Update shutdown state
			shutdown_ = true;
	
			// Notify anyone waiting on shutdown
			c_.notify_all();
		}
	}
}

