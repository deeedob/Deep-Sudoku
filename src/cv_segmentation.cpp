#include "cv_segmentation.hpp"

CVSegmentation::CVSegmentation( const QImage& img ) {
	m_orig = qt_ocv::image2Mat( img , CV_8UC( 4 ) , qt_ocv::MCO_BGRA );
}

void CVSegmentation::prepareImage( int gaussValue , bool dilating , bool eroding ) {
	auto ones = []( int width , int height ) {
		return cv::Mat::ones( cv::Size( width , height ) , CV_8U );
	};
	cv::cvtColor( m_orig , m_orig , cv::COLOR_BGR2GRAY );
	cv::GaussianBlur( m_orig , m_orig , cv::Size( gaussValue , gaussValue ) , 3 );
	cv::adaptiveThreshold( m_orig , m_orig , 255 , cv::ADAPTIVE_THRESH_GAUSSIAN_C , cv::THRESH_BINARY_INV , 199 , 25 );
	if( dilating )
		cv::dilate( m_orig , m_orig , ones( 3 , 3 ) , cv::Point( 0 , 0 ) , 1 );
	if( eroding )
		cv::erode( m_orig , m_orig , ones( 3 , 3 ) , cv::Point( 0 , 0 ) , 2 );
}

void CVSegmentation::prepareContour() {
	std::vector< std::vector< cv::Point>> contours;
	std::vector< cv::Vec4i > hierarchy;
	cv::findContours( m_orig , contours , hierarchy , cv::RETR_EXTERNAL , cv::CHAIN_APPROX_SIMPLE );
	
	struct
	{
	bool operator()( const std::vector< cv::Point >& c1 , const std::vector< cv::Point >& c2 ) {
		auto i = fabs( cv::contourArea( cv::Mat( c1 )));
		auto j = fabs( cv::contourArea( cv::Mat( c2 )));
		return i > j;
	}
	} cmpContourArea;
	
	std::sort( contours.begin() , contours.end() , cmpContourArea );
	
	std::vector< cv::Point > approx;
	for( auto& c : contours ) {
		cv::approxPolyDP( c , approx , 0.08 * cv::arcLength( c , true ) , true );
		if( approx.size() == 4 ) {
			break;
		}
	}
	/* add padding to detected contour */
	/* sort left and right */
	for( int i = 1; i < approx.size(); i++ ) {
		cv::Point tmp = approx[ i - 1 ];
		if( approx[ i ].x < tmp.x ) {
			std::swap( approx[ i - 1 ] , approx[ i ] );
		}
	}
	/* sort up and down regarding left and right */
	for( int i = 1; i < approx.size(); i += 2 ) {
		cv::Point tmp = approx[ i - 1 ];
		if( approx[ i ].y < tmp.y ) {
			std::swap( approx[ i - 1 ] , approx[ i ] );
		}
	}
	/* we now know that the points are sorted with
	 * left up, left down, right up, right down
	 * for any case!
	 * eg.  0 2  we want :  0 3 in ccw!
	 *      1 3             1 2
	 * */
	std::swap( approx[ 2 ] , approx[ 3 ] );
	auto padding = static_cast<int>(( approx[ 1 ].y - approx[ 0 ].y ) * 0.03);
	/* add some padding */
	approx[ 0 ] += { -padding , -padding };
	approx[ 1 ] += { -padding , padding };
	approx[ 2 ] += { padding , padding };
	approx[ 3 ] += { padding , -padding };
	m_contour = approx;
}

cv::Mat CVSegmentation::warpSelection() {
	//width of the new image
	auto widthA = std::sqrt(( std::pow( m_contour[ 0 ].x - m_contour[ 3 ].x , 2 ) + std::pow( m_contour[ 0 ].y - m_contour[ 3 ]
		.y , 2 )));
	auto widthB = std::sqrt(( std::pow( m_contour[ 1 ].x - m_contour[ 2 ].x , 2 ) + std::pow( m_contour[ 1 ].y - m_contour[ 2 ]
		.y , 2 )));
	auto heightA = std::sqrt(( std::pow( m_contour[ 0 ].x - m_contour[ 1 ].x , 2 ) + std::pow( m_contour[ 0 ].y - m_contour[ 1 ]
		.y , 2 )));
	auto heightB = std::sqrt(( std::pow( m_contour[ 3 ].x - m_contour[ 2 ].x , 2 ) + std::pow( m_contour[ 3 ].y - m_contour[ 3 ]
		.y , 2 )));
	auto maxWidth = static_cast<float>(std::max( widthA , widthB ));
	auto maxHeight = static_cast<float>(std::max( heightA , heightB ));
	// warp image to new size
	std::vector< cv::Point2f > dst = {{ 0 ,        0 } ,
	                                  { 0 ,        maxHeight } ,
	                                  { maxWidth , maxHeight } ,
	                                  { maxWidth , 0 }};
	std::vector< cv::Point2f > src = {{ static_cast<float>(m_contour[ 0 ].x) , static_cast<float>(m_contour[ 0 ].y) } ,
	                                  { static_cast<float>(m_contour[ 1 ].x) , static_cast<float>(m_contour[ 1 ].y) } ,
	                                  { static_cast<float>(m_contour[ 2 ].x) , static_cast<float>(m_contour[ 2 ].y) } ,
	                                  { static_cast<float>(m_contour[ 3 ].x) , static_cast<float>(m_contour[ 3 ].y) }};
	cv::Mat transform = cv::getPerspectiveTransform( src , dst );
	cv::Mat warped;
	cv::warpPerspective( m_orig , warped , transform , { ( int ) maxWidth , ( int ) maxHeight } );
	
	return warped;
}

const cv::Mat& CVSegmentation::getMat() const {
	return m_orig;
}

void CVSegmentation::setMat( const cv::Mat& mat ) {
	m_orig = mat;
}

const std::vector< cv::Point >& CVSegmentation::getContour() const {
	return m_contour;
}

void CVSegmentation::setContour( const std::vector< cv::Point >& contour ) {
	m_contour = contour;
}