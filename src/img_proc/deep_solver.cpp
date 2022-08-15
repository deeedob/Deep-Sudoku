#include "deep_solver.hpp"
#include <filesystem>

DeepSolver::DeepSolver( const std::string& path )
	: m_model { fdeep::load_model( path ) }
{
}

std::vector<DeepSolver::ReturnVal> DeepSolver::predictMulti( const QImage& src )
{
	CVSegmentation temp( src );
	temp.process();
	auto squares = temp.getPreparedSquares();
	auto used_squares = temp.getUsedSquareNums();
	
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
	
	std::vector<ReturnVal> ret;
	ret.reserve( predictions.size());
	
	for( int i = 0; i < predictions.size(); i++ ) {
		auto val = getResult( predictions[ i ] );
		auto pos = used_squares[ i ];
		ret.push_back( { pos, val } );
	}
	
	return ret;
}

int DeepSolver::getResult( const fdeep::tensors& t )
{
	auto v = t.at( 0 ).as_vector();
	auto old_max = v->at( 0 );
	int value = 1;
	for( int i = 1; i < v->size(); i++ ) {
		auto maybe = v->at( i );
		if( maybe > old_max ) {
			old_max = maybe;
			value = i + 1;
		}
	}
	return value;
}