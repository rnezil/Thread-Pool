#include "ra/thread_pool.hpp"
#include <complex>
#include <boost/multi_array.hpp>

namespace ra::fractal {

template <class Real>
void compute_julia_set( const std::complex<Real>& bottom_left,
		const std::complex<Real>& top_right, const std::complex<Real>& c,
		int max_iters, boost::multi_array<int, 2>& a, int num_threads ){
	// Ensure top right and bottom left are appropriate
	assert( top_right.real() > bottom_left.real() && top_right.imag() > bottom_left.imag() );
	
	// Fire up thread pool
	ra::concurrency::thread_pool task(num_threads);
	
	// Get width and height of image
	const int W = a.shape()[1];
	const int H = a.shape()[0];
	
	// Map region defined by multi-array to
	// region defined by top and bottom points
	const Real hori_step = (top_right.real() - bottom_left.real()) / (W - 1);
	const Real vert_step = (top_right.imag() - bottom_left.imag()) / (H - 1);
	
	// Perform the computations
	std::complex<Real> gog;
	for( int row = 0; row < H; ++row ){
		gog = bottom_left + std::complex<Real>( 0 , vert_step * row );
		task.schedule( [&a, W, row, hori_step, gog, max_iters, c](){ 
				std::complex<Real> trog;
				int n;
				for( int col = 0; col < W; ++col ){
					trog = gog + hori_step * col;
					n = 0;
					while( std::abs(trog) <= 2 && n < max_iters ){
						trog = trog * trog + c;
						++n;
					}
					a[row][col] = (n < max_iters) ? n : max_iters;
				}
				} );
	}

	// Shut down the thread pool
	task.shutdown();
}

}

