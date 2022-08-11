#include "deep_solver.hpp"
#include <filesystem>

DeepSolver::DeepSolver( const std::string& path )
	: m_model { fdeep::load_model( path ) }
{
}

std::vector<fdeep::tensors> DeepSolver::predictMulti( const QImage& src )
{
	CVSegmentation temp( src );
	temp.process();
	auto squares = temp.getPreparedSquares();
	auto used_squares = temp.getUsedSquares();
	
	auto size = CVSegmentation::getNNSize();
	fdeep::tensor_shape shape( size.first, size.second, 1 );
	
	std::vector<fdeep::tensors> input;
	
	for( int used_square : used_squares ) {
		cv::Mat flat_f;
		squares[ used_square ].convertTo( flat_f, CV_32FC1);
		std::vector<float> vals( flat_f.begin<float>(), flat_f.end<float>());
		fdeep::tensors t {{ shape, vals }};
		input.push_back( t );
	}
	auto predictions = m_model.predict_multi( input, true );
	for( const auto& i : predictions ) {
	}
	
	return m_model.predict_multi( input, true );
}