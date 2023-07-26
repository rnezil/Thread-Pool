#include "ra/julia_set.hpp"
#include <iostream>

int main() {
	std::complex<double> bot( -1.25 , -1.25 );
	std::complex<double> top( 1.25 , 1.25 );
	std::complex<double> c( 0.37 , -0.16 );
	int W = 512;
	int H = 512;
	int sample = 255;
	boost::multi_array<int, 2> a( boost::extents[H][W] );
	ra::fractal::compute_julia_set( bot, top, c, sample, a, 4 );
	std::cout << "P2 " << W << ' ' << H << ' ' << sample << '\n';
	for( int m = H; m; --m ){
		for( int n = 0; n < W; ++n ){
			std::cout << a[m-1][n] << ' ';
		}
		std::cout << '\n';
	}
	return 0;
}

