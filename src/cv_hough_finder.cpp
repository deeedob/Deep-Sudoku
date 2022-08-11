#include <opencv4/opencv2/imgproc.hpp>
#include "cv_hough_finder.hpp"

CVHoughFinder::CVHoughFinder( cv::Mat& bin_img )
	: m_binImg( bin_img )
{

}

bool CVHoughFinder::process()
{
	gradientImage();
}

CVHoughFinder::PairGrad CVHoughFinder::gradientImage()
{
	cv::Mat_<float> sobel_x( 3, 3 ), sobel_y( 3, 3 );
	cv::Mat grad_x, grad_y;
	sobel_x << -1 / 8, 0, 1 / 8,
		-2 / 8, 0, 2 / 8,
		-1 / 8, 0, 1 / 8;
	sobel_y << 1 / 8, 2 / 8, 1 / 8,
		0, 0, 0,
		-1 / 8, -2 / 8, -1 / 8;
	cv::filter2D( m_binImg, grad_x, -1, sobel_x );
	cv::filter2D( m_binImg, grad_y, -1, sobel_y );
	return { grad_x, grad_y };
}

std::tuple<float, float, float> CVHoughFinder::getThetaRhoMagnitude( const cv::Mat& gradients, cv::Point coords )
{
	return std::tuple<float, float, float>();
}

cv::Mat CVHoughFinder::accumulate( const CVHoughFinder::PairGrad& gradients, float theta_res, float rho_res, float min_mag, float max_angle_deviation, int minN_of_votes, bool normalize )
{
	return cv::Mat();
}

CVHoughFinder::Lines CVHoughFinder::getLines()
{
	return CVHoughFinder::Lines();
}