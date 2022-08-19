#include "../src/img_proc/cv_segmentation.hpp"
#include "../src/deep_solver.hpp"
#include <filesystem>
#include <opencv4/opencv2/opencv.hpp>
#include <cassert>

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
	std::vector<DeepSolver::ReturnVal> predictions = solver.predictMulti( img );
	std::vector<DeepSolver::ReturnVal> lookup_img_t002 = {
		{ 2, 1 }, { 5, 7 }, { 15, 3 }, { 21, 5 }, { 23, 8 }, { 26, 1 }, { 30, 8 }, { 32, 2 }, { 36, 3 }, { 37, 8 },
		{ 39, 4 }, { 40, 6 }, { 43, 1 }, { 44, 9 }, { 45, 6 }, { 53, 5 }, { 56, 9 }, { 60, 6 }, { 61, 7 }, { 62, 3 },
		{ 64, 1 }, { 65, 8 }, { 66, 2 }, { 70, 9 }
	};
	
	for( int i = 0; i < predictions.size(); i++ ) {
		std::cout << "pos: " << predictions[ i ].position << " val: " << predictions[ i ].value << std::endl;
		assert(( predictions[ i ].value == lookup_img_t002[ i ].value && predictions[ i ].position == lookup_img_t002[ i ].position )
		       && "Prediction values doesn't match with actual sudoku game" );
	}
	
	return EXIT_SUCCESS;
}