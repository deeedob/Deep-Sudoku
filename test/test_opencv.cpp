#include "../src/cv_segmentation.hpp"
#include <filesystem>
#include <iostream>
#include <opencv4/opencv2/opencv.hpp>

int main() {
	/* get qimage and convert it to mat */
	std::filesystem::path path( std::filesystem::current_path().parent_path()
		                            .parent_path() += "/test/res/Training_001.jpg" );
	QImage img( path.c_str());
	QTransform transform;
	img = img.transformed( transform.rotate( 90 ));
	CVSegmentation seg( img );
	
	/* show cutted image */
	auto cutted = seg.cuttedImage();
	cv::namedWindow( "test_opencv", cv::WINDOW_KEEPRATIO );
	cv::Mat resizedImg;
	cv::resize( cutted, resizedImg, cv::Size( 700, 700 ), 0, 0, cv::INTER_CUBIC );
	cv::imshow( "test_opencv", resizedImg );
	
	/* show contours */
	cv::Mat resizedImg2, drawing;
	auto approx = seg.getContour();
	cv::cvtColor( seg.getMat(), drawing, cv::COLOR_GRAY2RGB );
	drawContours( drawing, std::vector<std::vector<cv::Point> >( 1, approx ), -1, cv::Scalar( 255, 0, 0 ), 10, cv::LINE_8 );
	cv::resize( drawing, resizedImg2, cv::Size( 700, 700 ), 0, 0, cv::INTER_CUBIC );
	cv::namedWindow( "contours", cv::WINDOW_KEEPRATIO );
	cv::imshow( "contours", resizedImg2 );
	
	/* show merged lines */
	cv::Mat resizedImg3, linesImg;
	cv::cvtColor( cutted, linesImg, cv::COLOR_GRAY2BGR );
	auto mergedLines = seg.houghLines( cutted );
	for( auto& mergedLine : mergedLines ) {
		float rho = mergedLine[ 0 ], theta = mergedLine[ 1 ];
		cv::Point pt1, pt2;
		double a = cos( theta ), b = sin( theta );
		double x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound( x0 + img.width() * ( -b ));
		pt1.y = cvRound( y0 + img.height() * ( a ));
		pt2.x = cvRound( x0 - img.width() * ( -b ));
		pt2.y = cvRound( y0 - img.height() * ( a ));
		cv::line( linesImg, pt1, pt2, cv::Scalar( 0, 0, 255 ), 12, cv::LINE_AA );
	}
	cv::resize( linesImg, resizedImg3, cv::Size( 700, 700 ), 0, 0, cv::INTER_AREA );
	cv::namedWindow( "lines", cv::WINDOW_KEEPRATIO );
	cv::imshow( "lines", resizedImg3 );
	
	/* show intersections */
	auto intersections = seg.getIntersections( mergedLines );
	cv::Mat intersectionsImg = linesImg.clone();
	cv::Mat resizedImg4;
	for( auto& i : intersections ) {
		cv::circle( intersectionsImg, i, 15, cv::Scalar( 255, 0, 0 ), -1 );
	}
	cv::resize( intersectionsImg, resizedImg4, cv::Size( 700, 700 ), 0, 0, cv::INTER_CUBIC );
	cv::namedWindow( "intersections", cv::WINDOW_KEEPRATIO );
	cv::imshow( "intersections", resizedImg4 );
	// print all intersections
	for( auto& i : intersections ) {
		std::cout << i.x << " " << i.y << std::endl;
	}
	
	
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