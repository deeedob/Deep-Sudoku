#include "../src/cv_segmentation.hpp"
#include <filesystem>
#include <iostream>
#include <opencv4/opencv2/opencv.hpp>

int main()
{
	/* get qimage and convert it to mat */
	std::filesystem::path path( std::filesystem::current_path().parent_path()
	                                                           .parent_path() += "/test/res/Training_011.jpg" );
	
	QImage img( path.c_str());
	QTransform transform;
	img = img.transformed( transform.rotate( 90 ));
	CVSegmentation seg( img );
	seg.process();
	
	/* show cutted image */
	auto cutted = seg.getBinarizedCuttedImg();
	cv::Mat resizedImg1;
	cv::resize( cutted, resizedImg1, cv::Size( 700, 700 ), 0, 0, cv::INTER_CUBIC );
	cv::namedWindow( "binCuttedImg", cv::WINDOW_KEEPRATIO );
	cv::imshow( "binCuttedImg", resizedImg1 );
	
	/* show merged lines */
	cv::Mat resizedImg2;
	auto mergedLines = seg.getMergedLinesImg();
	cv::resize( mergedLines, resizedImg2, cv::Size( 700, 700 ), 0, 0, cv::INTER_AREA );
	cv::namedWindow( "lines", cv::WINDOW_KEEPRATIO );
	cv::imshow( "lines", resizedImg2 );
	
	/* show intersections */
	auto intersections = seg.getIntersectionImg();
	cv::Mat resizedImg3;
	cv::resize( intersections, resizedImg3, cv::Size( 700, 700 ), 0, 0, cv::INTER_CUBIC );
	cv::namedWindow( "intersections", cv::WINDOW_KEEPRATIO );
	cv::imshow( "intersections", resizedImg3 );
	
	auto preparedImg = seg.getPreparedSquaresImg();
	cv::Mat resizedImg4;
	cv::resize( preparedImg, resizedImg4, cv::Size( 700, 700 ), 0, 0, cv::INTER_CUBIC );
	cv::namedWindow( "prepared", cv::WINDOW_KEEPRATIO );
	cv::imshow( "prepared", resizedImg4 );
	
	auto original = seg.getOriginal();
	cv::Mat resizedImg5;
	cv::resize( original, resizedImg5, cv::Size( 700, 700 ), 0, 0, cv::INTER_CUBIC );
	cv::namedWindow( "original", cv::WINDOW_KEEPRATIO );
	cv::imshow( "original", resizedImg5 );
	
	/* wait and destroy */
	while ( true ) {
		auto key = cv::waitKey( 0 ) & 0xff;
		std::cout << "key pressed : " << key << std::endl;
		if( key == 27 ) {
			cv::destroyAllWindows();
			break;
		}
	}
	
	return EXIT_SUCCESS;
}