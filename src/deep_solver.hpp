#pragma once
#include <fdeep/fdeep.hpp>
#include "cv_segmentation.hpp"

class DeepSolver
{
public:
	explicit DeepSolver( const std::string& path );
	fdeep::tensors predict( cv::Mat& square );
	int predictMulti( std::vector<cv::Mat>& squares );
private:
	fdeep::model m_model;
};