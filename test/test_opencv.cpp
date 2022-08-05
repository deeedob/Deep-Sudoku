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
	seg.prepareImage( 41 , true , true );
	
	cv::namedWindow( "test_opencv" , cv::WINDOW_KEEPRATIO );
	cv::Mat resizedImg;
	cv::resize( seg.getMat() , resizedImg , cv::Size( 700 , 700 ) , 0 , 0 , cv::INTER_CUBIC );
	cv::imshow( "test_opencv" , resizedImg );
	
	cv::Mat resizedImg2;
	auto approx = seg.getContour();
	cv::Mat drawing;
	cv::cvtColor( seg.getMat() , drawing , cv::COLOR_GRAY2RGB );
	cv::Scalar color = cv::Scalar( std::rand() % 255 , std::rand() % 255 , std::rand() % 255 );
	drawContours( drawing , std::vector< std::vector< cv::Point > >( 1 , approx ) , -1 , color , 5 , cv::LINE_8 );
	cv::resize( drawing , resizedImg2 , cv::Size( 700 , 700 ) , 0 , 0 , cv::INTER_CUBIC );
	cv::namedWindow( "contours" , cv::WINDOW_KEEPRATIO );
	cv::imshow( "contours" , resizedImg2 );
	
	//seg.addPadding( seg.getContour());
	
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