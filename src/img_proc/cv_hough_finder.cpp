#include "cv_hough_finder.hpp"
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

CVHoughFinder::CVHoughFinder( cv::Mat& bin_img )
	: m_binImg( bin_img )
{ }

bool CVHoughFinder::process()
{
	m_gradients = gradientImage();
	auto accum = accumulate( CV_PI / 720.f, 1, 1, 20 * CV_PI / 180.f, 230, true );
	
	cv::Mat resized;
	cv::resize( accum, resized, { 700, 700 }, 0, 0, cv::INTER_CUBIC );
	cv::namedWindow( "gradient", cv::WINDOW_KEEPRATIO );
	cv::imshow( "gradient", resized );
	return false;
	
}

CVHoughFinder::PairGrad CVHoughFinder::gradientImage()
{
	cv::Mat_<float> sobel_x( 3, 3 ), sobel_y( 3, 3 );
	cv::Mat grad_x, grad_y;
	sobel_x << ( -1.f / 8.f ), 0, ( 1.f / 8.f ),
		( -2.f / 8.f ), 0, ( 2.f / 8.f ),
		( -1.f / 8.f ), 0, ( 1.f / 8.f );
	sobel_y << ( 1.f / 8.f ), ( 2.f / 8.f ), ( 1.f / 8.f ),
		0, 0, 0,
		( -1.f / 8.f ), ( -2.f / 8.f ), ( -1.f / 8.f );
	
	cv::filter2D( m_binImg, grad_x, -1, sobel_x );
	cv::filter2D( m_binImg, grad_y, -1, sobel_y );
	return { grad_x, grad_y };
}

std::tuple<float, float, float> CVHoughFinder::getThetaRhoMagnitude( const PairGrad& gradients, cv::Point coords )
{
	auto val_1 = gradients.first.at<float>( coords );
	auto val_2 = gradients.second.at<float>( coords );
	
	float theta = std::atan( val_1 / val_2 );
	float rho = std::abs((float) coords.y * std::cos( theta ) + (float) coords.x * std::sin( theta ));
	float magnitude = std::sqrt( std::pow( val_2, 2 ) + std::pow( val_1, 2 ));
	
	return std::make_tuple( theta, rho, magnitude );
}

cv::Mat CVHoughFinder::accumulate( float theta_res, float rho_res, float min_mag, float max_angle_deviation, int min_n_of_votes, bool normalize )
{
	auto hough_x = static_cast<int>(CV_PI * 2.f / theta_res) + 1;
	auto hough_y = static_cast<int>(std::sqrt( std::pow( m_gradients.first.cols, 2 ) + std::pow( m_gradients.first.rows, 2 )) / rho_res) + 1;
	cv::Mat accumulator( hough_y, hough_x, CV_8UC1);
	accumulator.setTo( 0 );
	
	for( int i = 0; i < m_gradients.first.rows; i++ ) {
		for( int j = 0; j < m_gradients.first.cols; j++ ) {
			auto[theta, rho, magnitude] = getThetaRhoMagnitude( m_gradients, { j, i } );
			if( magnitude >= min_mag ) {
				if( std::abs(( M_PI / 2 ) - theta ) <= max_angle_deviation ||
				    std::abs( theta ) <= max_angle_deviation ) {
					
					int theta_idx = static_cast<int>( theta / theta_res );
					int rho_idy = static_cast<int>( rho / rho_res );
					accumulator.at<uchar>( rho_idy, theta_idx ) += 1;
					//std::cout << accumulator.at<int>( rho_idy, theta_idx ) << " ";
				}
			}
		}
	}
	
	if( min_n_of_votes > 0 ) {
		for( int i = 0; i < hough_y; i++ ) {
			for( int j = 0; j < hough_x; j++ ) {
				if( accumulator.at<int>( j, i ) < min_n_of_votes )
					accumulator.at<int>( j, i ) = 0;
			}
		}
	}
	
	//if( normalize ) {
	//	cv::normalize( accumulator, accumulator, 1.0, 0.0, cv::NORM_INF );
	//}
	cv::threshold( accumulator, accumulator, 1, 255, cv::THRESH_BINARY );
	
	return accumulator;
}

CVHoughFinder::Lines CVHoughFinder::getLines()
{
	return CVHoughFinder::Lines();
}

cv::Mat CVHoughFinder::getResult()
{
	return m_binImg;
}