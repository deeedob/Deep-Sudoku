#pragma once
#include <opencv2/core/core.hpp>

class CVHoughFinder
{
	using PairGrad = std::pair<cv::Mat, cv::Mat>;
	using Lines = std::vector<cv::Vec3f>;
public:
	explicit CVHoughFinder( cv::Mat& bin_img );
	bool process();
	cv::Mat getResult();
private:
	PairGrad gradientImage();
	std::tuple<float, float, float> getThetaRhoMagnitude( const PairGrad& gradients, cv::Point coords );
	cv::Mat accumulate( float theta_res, float rho_res, float min_mag, float max_angle_deviation, int min_n_of_votes, bool normalize );
	Lines getLines();
private:
	cv::Mat m_binImg;
	PairGrad m_gradients;
	
};