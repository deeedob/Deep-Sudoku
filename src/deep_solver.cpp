#include "deep_solver.hpp"
#include <filesystem>

DeepSolver::DeepSolver( const std::string& path )
	: m_model { fdeep::load_model( path ) }
{ }

fdeep::tensors DeepSolver::predict( cv::Mat& square )
{
	auto size = CVSegmentation::getNNSize();
	fdeep::tensor_shape shape(size.first, size.second);
	
	cv::Mat flat_f;
	square.convertTo(flat_f, CV_32FC1);
	std::vector<float> vals( flat_f.begin<float>(), flat_f.end<float>());
	fdeep::tensor t( shape, vals );
	
	return m_model.predict({t});
}

int DeepSolver::predictMulti( std::vector<cv::Mat>& squares )
{
	/* convert to 32F and store in vector! */
	std::vector<fdeep::tensors> tensors;
	auto size = CVSegmentation::getNNSize();
	fdeep::tensor_shape shape( size.first, size.second );
	for(auto& i : squares) {
		cv::Mat f_flat;
		i.convertTo( f_flat, CV_32FC1);
		std::vector<float> res( f_flat.begin<float>(), f_flat.end<float>());
		fdeep::tensors ts()
	}
	
	
	m_model.predict({tensor});
	
	return 0;
}