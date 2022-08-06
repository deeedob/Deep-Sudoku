#pragma once
#include <QImage>
#include <opencv4/opencv2/imgproc.hpp>
#include "cv_img_mat.hpp"

class CVSegmentation
{
public:
	explicit CVSegmentation( const QImage& img );
	~CVSegmentation() = default;
	cv::Mat cuttedImage();
	std::vector< cv::Vec2f > houghLines( const cv::Mat& binImg );
	[[nodiscard]] const cv::Mat& getMat() const;
	void setMat( const cv::Mat& mat );
	[[nodiscard]] const std::vector< cv::Point >& getContour() const;
	void setContour( const std::vector< cv::Point >& contour );
private:
	void prepareImage( int gaussValue = 41 , bool dilating = true , bool eroding = true );
	void prepareContour();
	cv::Mat warpSelection();
private:
	cv::Mat m_orig;
	double m_padding { 0.03 };
	std::vector< cv::Point > m_contour;
};