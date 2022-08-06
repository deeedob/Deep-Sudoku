#include "../src/cv_segmentation.hpp"
#include <filesystem>
#include <iostream>
#include <opencv4/opencv2/opencv.hpp>

int main() {
	std::filesystem::path path( std::filesystem::current_path().parent_path()
	                                                           .parent_path() += "/test/res/Training_001.jpg" );
	QImage img( path.c_str());
	QTransform transform;
	img = img.transformed( transform.rotate( 90 ));
	
	CVSegmentation seg( img );
	cv::Mat cutted = seg.cuttedImage();
	
	cv::namedWindow( "test_opencv" , cv::WINDOW_KEEPRATIO );
	cv::Mat resizedImg;
	cv::resize( cutted , resizedImg , cv::Size( 700 , 700 ) , 0 , 0 , cv::INTER_CUBIC );
	cv::imshow( "test_opencv" , resizedImg );
	
	cv::Mat resizedImg2 , drawing;
	auto approx = seg.getContour();
	cv::cvtColor( seg.getMat() , drawing , cv::COLOR_GRAY2RGB );
	drawContours( drawing , std::vector< std::vector< cv::Point > >( 1 , approx ) , -1 , cv::Scalar( 255 , 0 , 0 ) , 10 , cv::LINE_8 );
	cv::resize( drawing , resizedImg2 , cv::Size( 700 , 700 ) , 0 , 0 , cv::INTER_CUBIC );
	cv::namedWindow( "contours" , cv::WINDOW_KEEPRATIO );
	cv::imshow( "contours" , resizedImg2 );
	
	cv::Mat resizedImg4 , lineImg;
	//get line image
	auto lines = seg.houghLines( cutted );
	
	/* wait and destroy */
	while( true ) {
		auto key = cv::waitKey( 0 ) & 0xff;
		std::cout << "key pressed : " << key << std::endl;
		if( key == 27 ) {
			cv::destroyAllWindows();
			break;
		}
	}
	
	return EXIT_SUCCESS;
}