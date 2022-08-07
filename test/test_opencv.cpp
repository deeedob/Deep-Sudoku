#include "../src/cv_segmentation.hpp"
#include <filesystem>
#include <iostream>
#include <opencv4/opencv2/opencv.hpp>

void
drawSegmentedRect( const std::vector<cv::Mat>& squares, const std::string& wName, int borderWidth );

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
	int cnt { 0 };
	for( auto& i : intersections ) {
		if( cnt == 10 ) {
			cnt = 0;
			std::cout << std::endl;
		}
		std::cout << '(' << i.x << ", " << i.y << ")  ";
		cnt++;
	}
	std::cout << std::endl;
	
	// draw squares
	auto squares = seg.cutSquares( intersections, cutted );
	drawSegmentedRect( squares, "squares", 20 );
	
	//prepare the image for the NN
	auto squaresNN = seg
		.preparedSquares( squares, 0.4, { 0.15, 0.75, 0.5, 0.75 } );
	drawSegmentedRect( squaresNN, "squaresNN", 5 );
	
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

//this bad boy reconciles all cutted squares into a new image with a border!
void
drawSegmentedRect( const std::vector<cv::Mat>& squares, const std::string& wName, int borderWidth ) {
	cv::Mat resizedImg5;
	cv::Mat squaredImg;
	std::vector<cv::Mat> squaresRows( 9 );
	int j { 0 }, nextW { 0 };
	for( int i = 0; i < squaresRows.size(); i++ ) {
		cv::Mat tmpB1, tmpB2;
		cv::Mat clrB1, clrB2;
		cv::cvtColor( squares[ j++ ], clrB1, cv::COLOR_GRAY2BGR );
		cv::cvtColor( squares[ j++ ], clrB2, cv::COLOR_GRAY2BGR );
		cv::copyMakeBorder( clrB1, tmpB1, borderWidth, borderWidth, borderWidth, borderWidth, cv::BORDER_CONSTANT, cv::Scalar( 0, 0, 255 ));
		cv::copyMakeBorder( clrB2, tmpB2, borderWidth, borderWidth, borderWidth, borderWidth, cv::BORDER_CONSTANT, cv::Scalar( 255, 0, 0 ));
		cv::hconcat( tmpB1, tmpB2, squaresRows[ i ] );
		nextW += 9;
		for( ; j < nextW; ) {
			cv::cvtColor( squares[ j++ ], clrB1, cv::COLOR_GRAY2BGR );
			if( j % 2 == 0 ) {
				cv::copyMakeBorder( clrB1, tmpB1, borderWidth, borderWidth, borderWidth, borderWidth, cv::BORDER_CONSTANT, cv::Scalar( 255, 0, 0 ));
			} else
				cv::copyMakeBorder( clrB1, tmpB1, borderWidth, borderWidth, borderWidth, borderWidth, cv::BORDER_CONSTANT, cv::Scalar( 0, 0, 255 ));
			
			cv::hconcat( squaresRows[ i ], tmpB1, squaresRows[ i ] );
		}
		if( i == 1 ) {
			cv::vconcat( squaresRows[ i - 1 ], squaresRows[ i ], squaredImg );
		}
		if( i > 1 ) {
			cv::vconcat( squaredImg, squaresRows[ i ], squaredImg );
		}
	}
	cv::resize( squaredImg, resizedImg5, cv::Size( 700, 700 ), 0, 0, cv::INTER_CUBIC );
	cv::namedWindow( wName, cv::WINDOW_KEEPRATIO );
	cv::imshow( wName, resizedImg5 );
	
}