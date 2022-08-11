#pragma once
#include <fdeep/fdeep.hpp>
#include "cv_segmentation.hpp"

class DeepSolver
{
public:
	
	struct ReturnVal
	{
		int position;
		int value;
	};
	
	explicit DeepSolver( const std::string& path );
	std::vector<ReturnVal> predictMulti( const QImage& src );
	int getResult( const fdeep::tensors& t );
private:
	fdeep::model m_model;
};