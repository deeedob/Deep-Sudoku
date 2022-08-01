#pragma once
#include <QImage>
#include <opencv4/opencv2/imgproc.hpp>
#include "cv_img_mat.hpp"

class CVSegmentation
{
public:
	explicit CVSegmentation( const QImage& img );
	void prepareImage( int gaussValue = 41 , bool dilating = true , bool eroding = true );

private:
	cv::Mat ones( int width , int height );
private:
	cv::Mat m_mat;
};