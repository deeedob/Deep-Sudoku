#include "../src/cv_segmentation.hpp"
#include "../src/deep_solver.hpp"
#include <filesystem>
#include <opencv4/opencv2/opencv.hpp>

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
	auto p = solver.predictMulti( img );
	
	for( const auto& i : p ) {
		std::cout << "prediction: " << fdeep::show_tensors( i ) << std::endl;
	}
	
	return EXIT_SUCCESS;
}