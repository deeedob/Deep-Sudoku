#pragma once
#include <QImage>
#include <opencv4/opencv2/imgproc.hpp>
#include "cv_img_mat.hpp"

class CVSegmentation
{
	
	struct DetectionSize
	{
		float minWidth, maxWidth;
		float minHeight, maxHeight;
	};

public:
	
	explicit CVSegmentation( const QImage& img );
	CVSegmentation( const CVSegmentation& other ) = delete;
	CVSegmentation& operator=( const CVSegmentation& other ) = delete;
	~CVSegmentation() = default;
	
	bool process();
	const cv::Mat& getOriginal();
	const cv::Mat& getBinarizedImg();
	const cv::Mat& getBinarizedCuttedImg();
	cv::Mat getMergedLinesImg();
	cv::Mat getIntersectionImg();
	cv::Mat getPreparedSquaresImg();
	
	static float toRad( float deg );
	static float getAspectRatio( cv::Mat img );
private:
	cv::Mat binarizedImg( cv::Mat src, int gaussValue = 41, bool dilating = true, bool eroding = true );
	std::vector<cv::Point> getRectangularContour( cv::Mat src ) const;
	cv::Mat warpSelection( cv::Mat src, const std::vector<cv::Point>& contours );
	
	std::vector<cv::Vec2f> mergedHoughLines( cv::Mat binImg );
	std::vector<cv::Vec2f> mergedHoughLinesImpl( const std::vector<cv::Vec2f>& lines, float thetaMax, float rhoMax );
	std::vector<cv::Vec2f> customHoughLinesImpl(const std::vector<cv::Vec2f>& lines, float thetaMax, float rhoMax);
	std::vector<cv::Point> getIntersections( const std::vector<cv::Vec2f>& mergedLines );
	
	std::vector<cv::Mat> cutSquares( const std::vector<cv::Point>& intersections, cv::Mat binImg );
	std::vector<cv::Mat> preparedSquares( const std::vector<cv::Mat>& squares, float numberFillFactor, DetectionSize d );
	
	cv::Mat drawSegmentedRectImg( const std::vector<cv::Mat>& squares, int borderWidth );
private:
	cv::Mat m_orig;
	cv::Mat m_origBin;
	cv::Mat m_binCutted;
	std::vector<cv::Vec2f> m_mergedLines;
	std::vector<cv::Point> m_intersections;
	std::vector<cv::Point> m_contour;
	std::vector<cv::Mat> m_preparedSquares;
	
	double m_contourPadding { 0.03 };
	float m_numbFillFactor { 0.4 };
	DetectionSize m_detectionSize { 0.15, 0.75, 0.5, 0.75 };
	const int m_nnWidth { 28 };
	const int m_nnHeight { 28 };
	bool m_success;
	bool m_processed;
};