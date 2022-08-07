#pragma once
#include <QImage>
#include <opencv4/opencv2/imgproc.hpp>
#include "cv_img_mat.hpp"

class CVSegmentation
{
public:
	struct DetectionSize
	{
		float minWidth, maxWidth;
		float minHeight, maxHeight;
	};
	
	explicit CVSegmentation( const QImage& img );
	~CVSegmentation() = default;
	
	cv::Mat cuttedImage();
	std::vector<cv::Vec2f> houghLines( const cv::Mat& binImg );
	std::vector<cv::Point> getIntersections( const std::vector<cv::Vec2f>& mergedLines );
	std::vector<cv::Mat> cutSquares( const std::vector<cv::Point>& intersections, const cv::Mat& binImg );
	std::vector<cv::Mat> preparedSquares( const std::vector<cv::Mat>& squares, float numberFillFactor, DetectionSize d );
	
	static float toRad( float deg );
	static float getAspectRatio( const cv::Mat& img );
	
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
	const int m_nnWidth { 28 };
	const int m_nnHeight { 28 };
	
};