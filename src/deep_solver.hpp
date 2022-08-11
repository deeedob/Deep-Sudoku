#pragma once
#include <fdeep/fdeep.hpp>
#include "cv_segmentation.hpp"

class DeepSolver
{
public:
	explicit DeepSolver( const std::string& path );
	std::vector<fdeep::tensors> predictMulti( const QImage& src );
private:
	fdeep::model m_model;
};