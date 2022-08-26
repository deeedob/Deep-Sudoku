#include "../src/img_proc/cv_segmentation.hpp"
#include "../src/deep_solver.hpp"
#include <filesystem>
#include <opencv4/opencv2/opencv.hpp>
#include <cassert>

using namespace dsdk;

int main()
{
	/* get qimage and convert it to mat */
	std::filesystem::path root_p( std::filesystem::current_path().parent_path().parent_path());
	
	std::string img_p = root_p.c_str();
	img_p += "/test/res/Training_002.jpg";
	std::string ai_p = root_p;
	ai_p += "/ai_model/fdeep_model.json";
	
	QImage img( img_p.c_str());
	
	DeepSolver solver( ai_p );
	
	if( solver.solveFromImage( img ))
		solver.printBoard();
	else
		std::cout << "no solution found!" << std::endl;
	
	return EXIT_SUCCESS;
}