#pragma once
#include <QImage>
#include <opencv4/opencv2/imgproc.hpp>
#include "cv_img_mat.hpp"

class CVSegmentation
{
public:
	explicit CVSegmentation( const QImage& img );
	void prepareImage( int gaussValue = 41 , bool dilating = true , bool eroding = true );
	std::vector< cv::Point > getContour();
	cv::Mat addPadding( const cv::Mat& approx );
	[[nodiscard]] const cv::Mat& getMat() const;
	void setMat( const cv::Mat& mat );

private:
private:
	cv::Mat m_mat;
};