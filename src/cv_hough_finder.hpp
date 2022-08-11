#pragma once
#include <opencv4/opencv2/core/core.hpp>

class CVHoughFinder
{
	using PairGrad = std::pair<cv::Mat, cv::Mat>;
	using Lines = std::vector<cv::Vec3f>;
public:
	explicit CVHoughFinder( cv::Mat& bin_img );
	bool process();
private:
	CVHoughFinder::PairGrad gradientImage();
	std::tuple<float, float, float> getThetaRhoMagnitude( const cv::Mat& gradients, cv::Point coords );
	cv::Mat accumulate( const PairGrad& gradients, float theta_res, float rho_res, float min_mag, float max_angle_deviation, int minN_of_votes, bool normalize );
	Lines getLines();
private:
	cv::Mat m_binImg;
};