#include "ra/thread_pool.hpp"

namespace ra::concurrency {

	thread_pool::thread_pool(): shutdown_(false), joining_(false), jobs_(256), pool_ {} {
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
	shutdown_(false), joining_(false), jobs_(256), pool_ {} {	
		for( unsigned i = num_threads; i; --i )
			pool_.emplace_back( std::thread() );
	}

	thread_pool::~thread_pool(){
		if( !is_shutdown() )
			shutdown();
	}

	thread_pool::size_type thread_pool::size() const { return size_; }

	void thread_pool::schedule( std::function<void()>&& func ) {
		// Grab the mutex
		std::unique_lock lock(m_);

		// Full job queue protocol
		if( jobs_.is_full() ){
			for( unsigned i = 0; i < size_; ++i ){
				if( pool_.at(i).joinable() ){
					pool_.at(i).join();
					std::function<void()> job;
					jobs_.pop(job); 
					pool_.at(i) = std::thread(job);
				}
			}
		}

		// Drop the mutex
		lock.unlock();

		// This function automatically
		// blocks if the job queue is
		// full. Therefore, schedule
		// function itself effectively
		// blocks if job queue is full
		if( jobs_.push( std::move(func) ) == queue<std::function<void()>>::status::closed )
			return;

		// Pick the mutex back up
		lock.lock();

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
		if( !is_shutdown() ){
			// Finish all jobs
			while( !jobs_.is_empty() ){
				for( unsigned i = 0; i < size_; ++i ){
					if( !pool_.at(i).joinable() ){
						std::function<void()> job;
						jobs_.pop(job); 
						pool_.at(i) = std::thread(job);
					}
				}

				for( unsigned i = 0; i < size_; ++i )
					if( pool_.at(i).joinable() )
						pool_.at(i).join();
			}

			// Ensure all workers are joined
			for( unsigned i = 0; i < size_; ++i )
				if( pool_.at(i).joinable() )
					pool_.at(i).join();

			// Place thread in shutdown state
			shutdown_ = true;
		}
	}

	bool thread_pool::is_shutdown() const { return shutdown_; }
}

