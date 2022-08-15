#include "../src/img_proc/cv_segmentation.hpp"
#include "../src/img_proc/cv_hough_finder.hpp"
#include <filesystem>
#include <iostream>
#include <opencv4/opencv2/opencv.hpp>

int main()
{
	/* get qimage and convert it to mat */
	std::filesystem::path path( std::filesystem::current_path()
		                            .parent_path().parent_path() += "/test/res/Training_011.jpg" );
	
	QImage img( path.c_str());
	QTransform transform;
	img = img.transformed( transform.rotate( 90 ));
	
	CVSegmentation seg( img );
	auto cutted = seg.getBinarizedCuttedImg();
	CVHoughFinder finder( cutted );
	finder.process();
	
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