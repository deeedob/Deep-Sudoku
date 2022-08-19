#pragma once
#include <QImage>
#include <opencv2/imgproc.hpp>
#include "cv_img_mat.hpp"

class CVSegmentation
{
	
	struct DetectionSize
	{
		float minWidth, maxWidth;
		float minHeight, maxHeight;
	};

public:
	explicit CVSegmentation( const QImage& src );
	CVSegmentation( const CVSegmentation& other ) = delete;
	CVSegmentation& operator=( const CVSegmentation& other ) = delete;
	CVSegmentation( CVSegmentation&& other ) = default;
	CVSegmentation& operator=( CVSegmentation&& other ) = default;
	~CVSegmentation() = default;
	
	bool process();
	const cv::Mat& getOriginal() noexcept;
	cv::Mat getBinarizedImg();
	cv::Mat getBinarizedCuttedImg();
	cv::Mat getMergedLinesImg();
	cv::Mat getIntersectionImg();
	cv::Mat getPreparedSquaresImg();
	
	void setImage( const QImage& src ) noexcept;
	[[nodiscard]] const std::vector<cv::Mat>& getPreparedSquares() const noexcept;
	[[nodiscard]] const std::vector<int>& getUsedSquareNums() const noexcept;
	[[nodiscard]] static std::pair<size_t, size_t> getNNSize() noexcept;
	
	static float toRad( float deg );
	static float getAspectRatio( cv::Mat img );

private:
	/* implementations */
	cv::Mat binarizedImg( cv::Mat src, int gauss_value = 41, bool dilating = true, bool eroding = true );
	std::vector<cv::Point> getRectangularContour( cv::Mat src ) const;
	cv::Mat warpSelection( cv::Mat src, const std::vector<cv::Point>& contours );
	
	std::vector<cv::Vec2f> mergedHoughLines( cv::Mat bin_img );
	std::vector<cv::Vec2f> mergedHoughLinesImpl( const std::vector<cv::Vec2f>& lines, float theta_max, float rho_max );
	std::vector<cv::Point> getIntersections( const std::vector<cv::Vec2f>& p_1 );
	
	std::vector<cv::Mat> cutSquares( const std::vector<cv::Point>& intersections, cv::Mat bin_img );
	std::vector<cv::Mat> preparedSquares( const std::vector<cv::Mat>& squares, float number_fill_factor, DetectionSize d );
	
	cv::Mat drawSegmentedRectImg( const std::vector<cv::Mat>& m_1, int m_2 );
private:
	cv::Mat m_orig;
	cv::Mat m_origBin;
	cv::Mat m_binCutted;
	std::vector<cv::Vec2f> m_mergedLines;
	std::vector<cv::Point> m_intersections;
	std::vector<cv::Point> m_contour;
	std::vector<cv::Mat> m_preparedSquares;
	std::vector<int> m_usedSquares;
	
	double m_contourPadding { 0.03 };
	float m_numbFillFactor { 0.4 };
	DetectionSize m_detectionSize { 0.15, 0.75, 0.3, 0.75 };
	bool m_success;
	bool m_processed;
	static inline std::pair<size_t, size_t> m_nnSize { 28, 28 };
};