#include "cv_segmentation.hpp"

CVSegmentation::CVSegmentation( const QImage& img ) {
	m_mat = qt_ocv::image2Mat( img );
}

void CVSegmentation::prepareImage( int gaussValue , bool dilating , bool eroding ) {
	cv::cvtColor( m_mat , m_mat , cv::COLOR_BGR2GRAY );
	cv::GaussianBlur( m_mat , m_mat , cv::Size( 3 , 3 ) , gaussValue , gaussValue , 3 );
	cv::adaptiveThreshold( m_mat , m_mat , 255 , cv::ADAPTIVE_THRESH_GAUSSIAN_C , cv::THRESH_BINARY_INV , 199 , 25 );
	if( dilating )
		cv::dilate( m_mat , m_mat , ones( 5 , 5 ) , cv::Point( -1 , -1 ) , 2 );
	if( eroding )
		cv::erode( m_mat , m_mat , ones( 5 , 5 ) , cv::Point( -1 , -1 ) , 2 );
}

cv::Mat CVSegmentation::ones( int width , int height ) {
	std::vector< std::vector< int>> tmp( height , std::vector< int >( width , 1 ));
	return cv::Mat( tmp );
}