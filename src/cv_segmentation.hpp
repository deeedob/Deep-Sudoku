#pragma once
#include <QImage>
#include <opencv4/opencv2/imgproc.hpp>
#include "cv_img_mat.hpp"

class CVSegmentation
{
public:
	explicit CVSegmentation( const QImage& img );
	void prepareImage( int gaussValue = 41 , bool dilating = true , bool eroding = true );
	void prepareContour();
	cv::Mat warpSelection();
	[[nodiscard]] const cv::Mat& getMat() const;
	void setMat( const cv::Mat& mat );
	[[nodiscard]] const std::vector< cv::Point >& getContour() const;
	void setContour( const std::vector< cv::Point >& contour );
private:
	cv::Mat m_original;
	cv::Mat m_orig;
	std::vector< cv::Point > m_contour;
};