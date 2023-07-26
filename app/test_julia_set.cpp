#include "ra/julia_set.hpp"
#include <iostream>
#include <chrono>

int main() {
	std::complex<double> bot( -1.25 , -1.25 );
	std::complex<double> top( 1.25 , 1.25 );
	std::complex<double> c( 0.37 , -0.16 );
	int W = 512;
	int H = 512;
	int sample = 255;
	boost::multi_array<int, 2> a( boost::extents[H][W] );
	/*ra::fractal::compute_julia_set( bot, top, c, sample, a, 4 );
	std::cout << "P2 " << W << ' ' << H << ' ' << sample << '\n';
	for( int m = H; m; --m ){
		for( int n = 0; n < W; ++n ){
			std::cout << a[m-1][n] << ' ';
		}
		std::cout << '\n';
	}*/



	boost::multi_array<int, 2> one( boost::extents[H][W] );
	const auto start_one = std::chrono::high_resolution_clock::now();
	ra::fractal::compute_julia_set( bot, top, c, sample, one, 1 );
	const auto stop_one = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double> time_one = stop_one - start_one;
	std::cout << "Time for one:\t" << time_one << '\n';
	


	boost::multi_array<int, 2> two( boost::extents[H][W] );
	const auto start_two = std::chrono::high_resolution_clock::now();
	ra::fractal::compute_julia_set( bot, top, c, sample, two, 2 );
	const auto stop_two = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double> time_two = stop_two - start_two;
	std::cout << "Time for two:\t" << time_two << '\n';
	


	boost::multi_array<int, 2> four( boost::extents[H][W] );
	const auto start_four = std::chrono::high_resolution_clock::now();
	ra::fractal::compute_julia_set( bot, top, c, sample, four, 4 );
	const auto stop_four = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double> time_four = stop_four - start_four;
	std::cout << "Time for four:\t" << time_four << '\n';


	
	boost::multi_array<int, 2> eight( boost::extents[H][W] );
	const auto start_eight = std::chrono::high_resolution_clock::now();
	ra::fractal::compute_julia_set( bot, top, c, sample, eight, 8 );
	const auto stop_eight = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double> time_eight = stop_eight - start_eight;
	std::cout << "Time for eight:\t" << time_eight << '\n';


	return 0;
}

