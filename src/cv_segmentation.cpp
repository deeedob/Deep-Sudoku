#include "cv_segmentation.hpp"

CVSegmentation::CVSegmentation( const QImage& img ) {
	m_mat = qt_ocv::image2Mat( img , CV_8UC( 4 ) , qt_ocv::MCO_BGRA );
}

void CVSegmentation::prepareImage( int gaussValue , bool dilating , bool eroding ) {
	auto ones = []( int width , int height ) {
		return cv::Mat::ones( cv::Size( width , height ) , CV_8U );
	};
	cv::cvtColor( m_mat , m_mat , cv::COLOR_BGR2GRAY );
	cv::GaussianBlur( m_mat , m_mat , cv::Size( gaussValue , gaussValue ) , 3 );
	cv::adaptiveThreshold( m_mat , m_mat , 255 , cv::ADAPTIVE_THRESH_GAUSSIAN_C , cv::THRESH_BINARY_INV , 199 , 25 );
	if( dilating )
		cv::dilate( m_mat , m_mat , ones( 3 , 3 ) , cv::Point( 0 , 0 ) , 1 );
	if( eroding )
		cv::erode( m_mat , m_mat , ones( 3 , 3 ) , cv::Point( 0 , 0 ) , 2 );
}

#include <iostream>

std::vector< cv::Point > CVSegmentation::getContour() {
	std::vector< std::vector< cv::Point>> contours;
	std::vector< cv::Vec4i > hierarchy;
	cv::findContours( m_mat , contours , hierarchy , cv::RETR_EXTERNAL , cv::CHAIN_APPROX_SIMPLE );
	
	struct
	{
	bool operator()( const std::vector< cv::Point >& c1 , const std::vector< cv::Point >& c2 ) {
		auto i = fabs( cv::contourArea( cv::Mat( c1 )));
		auto j = fabs( cv::contourArea( cv::Mat( c2 )));
		return i < j;
	}
	} cmpContourArea;
	
	std::sort( contours.begin() , contours.end() , cmpContourArea );
	std::vector< cv::Point > approx;
	for( auto& c : contours ) {
		cv::approxPolyDP( c , approx , 0.08 * cv::arcLength( c , true ) , true );
		if( approx.size() == 4 ) {
			std::cout << "hii" << std::endl;
			for( int i = 0; i < approx.size(); i++ ) {
				std::cout << approx[ i ].x << " " << approx[ i ].y << std::endl;
			}
			return approx;
		}
	}
	return { };
	
}

cv::Mat CVSegmentation::addPadding( const cv::Mat& approx ) {
	auto calculateDistance = []() {
	
	};
	std::cout << "rows: " << approx.rows << "cols" << approx.cols << std::endl;
	
	return cv::Mat();
}

const cv::Mat& CVSegmentation::getMat() const {
	return m_mat;
}

void CVSegmentation::setMat( const cv::Mat& mat ) {
	m_mat = mat;
}