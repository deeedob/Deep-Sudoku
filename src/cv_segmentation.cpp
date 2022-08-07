#include "cv_segmentation.hpp"

CVSegmentation::CVSegmentation( const QImage& img ) {
	m_orig = qt_ocv::image2Mat( img, CV_8UC( 4 ), qt_ocv::MCO_BGRA );
}

cv::Mat CVSegmentation::cuttedImage() {
	prepareImage();
	prepareContour();
	return warpSelection();
}

void
CVSegmentation::prepareImage( int gaussValue, bool dilating, bool eroding ) {
	auto ones = []( int width, int height ) {
		return cv::Mat::ones( cv::Size( width, height ), CV_8U );
	};
	cv::cvtColor( m_orig, m_orig, cv::COLOR_BGR2GRAY );
	cv::GaussianBlur( m_orig, m_orig, cv::Size( gaussValue, gaussValue ), 3 );
	cv::adaptiveThreshold( m_orig, m_orig, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 199, 25 );
	if( dilating )
		cv::dilate( m_orig, m_orig, ones( 3, 3 ), cv::Point( 0, 0 ), 1 );
	if( eroding )
		cv::erode( m_orig, m_orig, ones( 3, 3 ), cv::Point( 0, 0 ), 2 );
}

void CVSegmentation::prepareContour() {
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours( m_orig, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE );
	
	struct
	{
		bool operator()( const std::vector<cv::Point>& c1, const std::vector<cv::Point>& c2 ) {
			auto i = fabs( cv::contourArea( cv::Mat( c1 )));
			auto j = fabs( cv::contourArea( cv::Mat( c2 )));
			return i > j;
		}
	} cmpContourArea;
	
	std::sort( contours.begin(), contours.end(), cmpContourArea );
	
	std::vector<cv::Point> approx;
	for( auto& c : contours ) {
		cv::approxPolyDP( c, approx, 0.08 * cv::arcLength( c, true ), true );
		if( approx.size() == 4 ) {
			break;
		}
	}
	/* add padding to detected contour */
	/* sort left and right */
	for( int i = 1; i < approx.size(); i++ ) {
		cv::Point tmp = approx[ i - 1 ];
		if( approx[ i ].x < tmp.x ) {
			std::swap( approx[ i - 1 ], approx[ i ] );
		}
	}
	/* sort up and down regarding left and right */
	for( int i = 1; i < approx.size(); i += 2 ) {
		cv::Point tmp = approx[ i - 1 ];
		if( approx[ i ].y < tmp.y ) {
			std::swap( approx[ i - 1 ], approx[ i ] );
		}
	}
	/* we now know that the points are sorted with
	 * left up, left down, right up, right down
	 * for any case!
	 * eg.  0 2  we want :  0 3 in ccw!
	 *      1 3             1 2
	 * */
	std::swap( approx[ 2 ], approx[ 3 ] );
	auto padding = static_cast<int>(( approx[ 1 ].y - approx[ 0 ]
		.y ) * m_padding);
	/* add some padding */
	approx[ 0 ] += { -padding, -padding };
	approx[ 1 ] += { -padding, padding };
	approx[ 2 ] += { padding, padding };
	approx[ 3 ] += { padding, -padding };
	m_contour = approx;
}

cv::Mat CVSegmentation::warpSelection() {
	//width of the new image
	auto widthA = std::sqrt(( std::pow( m_contour[ 0 ].x - m_contour[ 3 ]
		.x, 2 ) + std::pow( m_contour[ 0 ].y - m_contour[ 3 ]
		.y, 2 )));
	auto widthB = std::sqrt(( std::pow( m_contour[ 1 ].x - m_contour[ 2 ]
		.x, 2 ) + std::pow( m_contour[ 1 ].y - m_contour[ 2 ]
		.y, 2 )));
	auto heightA = std::sqrt(( std::pow( m_contour[ 0 ].x - m_contour[ 1 ]
		.x, 2 ) + std::pow( m_contour[ 0 ].y - m_contour[ 1 ]
		.y, 2 )));
	auto heightB = std::sqrt(( std::pow( m_contour[ 3 ].x - m_contour[ 2 ]
		.x, 2 ) + std::pow( m_contour[ 3 ].y - m_contour[ 3 ]
		.y, 2 )));
	auto maxWidth = static_cast<float>(std::max( widthA, widthB ));
	auto maxHeight = static_cast<float>(std::max( heightA, heightB ));
	// warp image to new size
	std::vector<cv::Point2f> dst = {{ 0, 0 },
		{ 0, maxHeight },
		{ maxWidth, maxHeight },
		{ maxWidth, 0 }};
	std::vector<cv::Point2f> src = {{ static_cast<float>(m_contour[ 0 ].x),
		static_cast<float>(m_contour[ 0 ].y)
	},
		{ static_cast<float>(m_contour[ 1 ].x),
			static_cast<float>(m_contour[ 1 ].y)
		},
		{ static_cast<float>(m_contour[ 2 ].x),
			static_cast<float>(m_contour[ 2 ].y)
		},
		{ static_cast<float>(m_contour[ 3 ].x),
			static_cast<float>(m_contour[ 3 ].y)
		}};
	cv::Mat transform = cv::getPerspectiveTransform( src, dst );
	cv::Mat warped;
	cv::warpPerspective( m_orig, warped, transform, { (int) maxWidth,
		(int) maxHeight
	} );
	
	return warped;
}

const cv::Mat& CVSegmentation::getMat() const {
	return m_orig;
}

void CVSegmentation::setMat( const cv::Mat& mat ) {
	m_orig = mat;
}

const std::vector<cv::Point>& CVSegmentation::getContour() const {
	return m_contour;
}

void CVSegmentation::setContour( const std::vector<cv::Point>& contour ) {
	m_contour = contour;
}

#include <opencv4/opencv2/opencv.hpp>

std::vector<cv::Vec2f> CVSegmentation::houghLines( const cv::Mat& binImg ) {
	cv::Mat edges;
	cv::Canny( binImg, edges, 50, 200, 3 );
	
	std::vector<cv::Vec2f> lines;
	cv::HoughLines( edges, lines, 5, CV_PI / 2, 480, 0, 0 ); // runs the actual detection
	return mergeHoughLines( lines, toRad( 3 ), 50 );
	
}

float CVSegmentation::toRad( float deg ) {
	return deg * static_cast<float>(CV_PI) / 180.f;
}

std::vector<cv::Vec2f>
CVSegmentation::mergeHoughLines( const std::vector<cv::Vec2f>& lines, float thetaMax, float rhoMax ) {
	std::vector<std::vector<int >> combineIndex( lines.size());
	std::vector<cv::Vec2f> mergedLines;
	for( int i = 0; i < lines.size(); i++ ) {
		int index = i;
		for( int j = i; j < lines.size(); j++ ) {
			float distanceI = lines[ i ][ 0 ], distanceJ = lines[ j ][ 0 ];
			float slopeI = lines[ i ][ 1 ], slopeJ = lines[ j ][ 1 ];
			float disDiff = abs( distanceI - distanceJ );
			float slopeDiff = abs( slopeI - slopeJ );
			
			if( slopeDiff < thetaMax && disDiff < rhoMax ) {
				bool isCombined = false;
				for( int w = 0; w < i; w++ ) {
					for( int u = 0; u < combineIndex[ w ].size(); u++ ) {
						if( combineIndex[ w ][ u ] == j ) {
							isCombined = true;
							break;
						}
						if( combineIndex[ w ][ u ] == i )
							index = w;
					}
					if( isCombined )
						break;
				}
				if( !isCombined )
					combineIndex[ index ].push_back( j );
			}
		}
	}
	
	for( auto& i : combineIndex ) {
		if( i.empty())
			continue;
		cv::Vec2f lineTemp( 0, 0 );
		for( int j : i ) {
			lineTemp[ 0 ] += lines[ j ][ 0 ];
			lineTemp[ 1 ] += lines[ j ][ 1 ];
		}
		lineTemp[ 0 ] /= i.size();
		lineTemp[ 1 ] /= i.size();
		mergedLines.push_back( lineTemp );
	}
	return mergedLines;
}

std::vector<cv::Point>
CVSegmentation::getIntersections( const std::vector<cv::Vec2f>& mergedLines ) {
	// find intersections from hough lines
	std::vector<cv::Point> intersections;
	auto getIntersect = []( const cv::Vec2f& l1, const cv::Vec2f& l2 ) {
		// get intersections of perpendicular lines
		int x, y = { 0 };
		if( l1[ 1 ] == l2[ 1 ] )
			return cv::Point( -1, -1 );
		if( l1[ 1 ] == 0 ) {
			x = static_cast<int>(l1[ 0 ]);
			y = static_cast<int>(l2[ 0 ]);
		}
		if( l2[ 1 ] == 0 ) {
			x = static_cast<int>(l2[ 0 ]);
			y = static_cast<int>(l1[ 0 ]);
		}
		return cv::Point( x, y );
	};
	for( int i = 0; i < mergedLines.size(); i++ ) {
		for( int j = i + 1; j < mergedLines.size(); j++ ) {
			auto intersect = getIntersect( mergedLines[ i ], mergedLines[ j ] );
			if( intersect.x != -1 && intersect.y != -1 )
				intersections.push_back( intersect );
		}
	}
	// sort points on the same line
	auto sortLRSameRow = []( const cv::Point& p1, const cv::Point& p2 ) {
		if( p1.y == p2.y )
			return p1.x < p2.x;
		return p1.y < p2.y;
	};
	std::sort( intersections.begin(), intersections.end(), sortLRSameRow );
	
	return intersections;
}

std::vector<cv::Mat>
CVSegmentation::cutSquares( const std::vector<cv::Point>& intersections, const cv::Mat& binImg ) {
	std::vector<cv::Mat> squares;
	int cnt { 0 };
	for( int i = 0; i < intersections.size() - 11; i++, cnt++ ) {
		if( cnt > 8 ) {
			i++;
			cnt = 0;
		}
		cv::Point p1 = intersections[ i ], p2 = intersections[ i + 11 ];
		cv::Rect roi( p1.x, p1.y, p2.x - p1.x, p2.y - p1.y );
		cv::Mat croppedRef( binImg, roi );
		cv::Mat cropped;
		//croppedRef.copyTo( cropped );
		squares.push_back( croppedRef );
	}
	
	return squares;
}

std::vector<cv::Mat>
CVSegmentation::preparedSquares( const std::vector<cv::Mat>& squares, float numberFillFactor, DetectionSize d ) {
	assert(( numberFillFactor > 0 && numberFillFactor < 1 ) && "numberFillFactor must be between 0 and 1" );
	assert((( d.minWidth > 0 && d.minWidth < 1 ) && ( d.maxWidth > 0 && d.maxWidth < 1 ) &&
	        ( d.minHeight > 0 && d.minHeight < 1 ) && ( d.maxHeight > 0 && d.maxHeight < 1 )) && "DetectionSize must be between 0 and 1" );
	
	std::vector<cv::Mat> preparedSquares;
	float detectMinWidth = 0.18, detectMinHeight = 0.25, detectMaxWidth = 0.75, detectMaxHeight = 0.75;
	
	int cnt { 0 };
	for( int i = 0; i < squares.size(); i++ ) {
		std::vector<std::vector<cv::Point>> prepared;
		cv::Mat c = squares[ i ];
		cv::Mat candidate = cv::Mat::ones( cv::Size( m_nnWidth, m_nnHeight ), CV_8UC1);
		
		cv::findContours( c, prepared, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );
		
		for( auto& j : prepared ) {
			auto r = cv::boundingRect( j );
			/* if a number matches a rect size of a number ... */
			if( r.width > c.cols * d.minWidth && r.width < c.cols * d
				.maxWidth &&
			    r.height > c.rows * d.minHeight && r.height < c.cols * d
				.maxHeight ) {
				
				cv::Mat croppedRef( c, r );
				if( croppedRef.cols > candidate.cols || croppedRef
					                                        .rows > candidate
					                                        .rows ) {
					cv::Mat resized;
					// resize with aspect ratio
					auto ratio = getAspectRatio( croppedRef );
					cv::resize( croppedRef, resized, cv::Size( static_cast<int>((float) m_nnWidth * numberFillFactor), static_cast<int>((float) m_nnHeight * numberFillFactor )), ratio, ratio );
					croppedRef = resized;
				}
				
				int x = ( m_nnWidth - croppedRef.cols ) / 2;
				int y = ( m_nnHeight - croppedRef.rows ) / 2;
				//copy the croppedRef to the candidate
				croppedRef.copyTo( candidate( cv::Rect( x, y, croppedRef
					.cols, croppedRef.rows )));
				
			}
		}
		preparedSquares.push_back( candidate );
		cnt++;
	}
	return preparedSquares;
}

float CVSegmentation::getAspectRatio( const cv::Mat& img ) {
	return img.cols > img.rows ? static_cast<float>(img.cols) / img
		.rows : static_cast<float>(img.rows) / img.cols;
}