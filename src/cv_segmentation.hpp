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
	std::vector<cv::Vec2f> houghLines( const cv::Mat& binImg );
	std::vector<cv::Point> getIntersections( const std::vector<cv::Vec2f>& mergedLines );
	static float toRad( float deg );
	
	[[nodiscard]] const cv::Mat& getMat() const;
	void setMat( const cv::Mat& mat );
	[[nodiscard]] const std::vector<cv::Point>& getContour() const;
	void setContour( const std::vector<cv::Point>& contour );
private:
	void prepareImage( int gaussValue = 41, bool dilating = true, bool eroding = true );
	void prepareContour();
	cv::Mat warpSelection();
	std::vector<cv::Vec2f> mergeHoughLines( const std::vector<cv::Vec2f>& lines, float thetaMax, float rhoMax );
private:
	cv::Mat m_orig;
	std::vector<cv::Point> m_contour;
	double m_padding { 0.03 };
	
};