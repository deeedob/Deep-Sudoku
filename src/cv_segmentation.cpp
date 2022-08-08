#include "cv_segmentation.hpp"

CVSegmentation::CVSegmentation( const QImage& img )
	: m_success( true ), m_processed( false ) {
	m_orig = qt_ocv::image2Mat( img, CV_8UC( 4 ), qt_ocv::MCO_BGRA );
}

bool CVSegmentation::process() {
	m_origBin = binarizedImg( m_orig );
	m_contour = getRectangularContour( m_origBin );
	m_binCutted = warpSelection( m_origBin, m_contour );
	m_mergedLines = mergedHoughLines( m_binCutted );
	if( m_mergedLines.size() < 20 )
		m_success = false;
	if( !m_success ) {
		m_intersections = getIntersections( m_mergedLines );
		m_preparedSquares = preparedSquares( cutSquares( m_intersections, m_binCutted ), m_numbFillFactor, m_detectionSize );
	}
	m_processed = true;
	return m_success;
}

const cv::Mat& CVSegmentation::getBinarizedImg() {
	if( m_processed )
		return m_origBin;
}

const cv::Mat& CVSegmentation::getBinarizedCuttedImg() {
	if( m_processed )
		return m_binCutted;
}

cv::Mat CVSegmentation::getMergedLinesImg() {
	cv::Mat linesImg;
	cv::cvtColor( m_binCutted, linesImg, cv::COLOR_GRAY2BGR );
	for( auto& mergedLine : m_mergedLines ) {
		float rho = mergedLine[ 0 ], theta = mergedLine[ 1 ];
		cv::Point pt1, pt2;
		double a = cos( theta ), b = sin( theta );
		double x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound( x0 + m_orig.cols * ( -b ));
		pt1.y = cvRound( y0 + m_orig.rows * ( a ));
		pt2.x = cvRound( x0 - m_orig.cols * ( -b ));
		pt2.y = cvRound( y0 - m_orig.rows * ( a ));
		cv::line( linesImg, pt1, pt2, cv::Scalar( 0, 0, 255 ), 12, cv::LINE_AA );
	}
	return linesImg;
}

cv::Mat CVSegmentation::getIntersectionImg() {
	cv::Mat intersectionImg;
	cv::cvtColor( m_binCutted, intersectionImg, cv::COLOR_GRAY2BGR );
	for( auto& i : m_intersections ) {
		cv::circle( intersectionImg, i, 15, cv::Scalar( 255, 0, 0 ), -1 );
	}
	return intersectionImg;
}

cv::Mat CVSegmentation::getPreparedSquaresImg() {
	return drawSegmentedRectImg( m_preparedSquares, 3 );
}

cv::Mat
CVSegmentation::binarizedImg( const cv::Mat& src, int gaussValue, bool dilating, bool eroding ) {
	cv::Mat out;
	auto ones = []( int width, int height ) {
		return cv::Mat::ones( cv::Size( width, height ), CV_8U );
	};
	cv::cvtColor( src, out, cv::COLOR_BGR2GRAY );
	cv::GaussianBlur( out, out, cv::Size( gaussValue, gaussValue ), 3 );
	cv::adaptiveThreshold( out, out, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 199, 25 );
	if( dilating )
		cv::dilate( src, out, ones( 3, 3 ), cv::Point( 0, 0 ), 1 );
	if( eroding )
		cv::erode( src, out, ones( 3, 3 ), cv::Point( 0, 0 ), 2 );
	return out;
}

std::vector<cv::Point>
CVSegmentation::getRectangularContour( const cv::Mat& src ) {
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::cvtColor( src, src, cv::COLOR_BGR2GRAY ); //TODO maybe wrong?
	cv::findContours( src, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE );
	
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
		.y ) * m_contourPadding);
	/* add some padding */
	approx[ 0 ] += { -padding, -padding };
	approx[ 1 ] += { -padding, padding };
	approx[ 2 ] += { padding, padding };
	approx[ 3 ] += { padding, -padding };
	
	return approx;
}

cv::Mat
CVSegmentation::warpSelection( const cv::Mat& src, const std::vector<cv::Point>& contours ) {
	//width of the new image
	auto widthA = std::sqrt(( std::pow( contours[ 0 ].x - contours[ 3 ]
		.x, 2 ) + std::pow( contours[ 0 ].y - contours[ 3 ]
		.y, 2 )));
	auto widthB = std::sqrt(( std::pow( contours[ 1 ].x - contours[ 2 ]
		.x, 2 ) + std::pow( contours[ 1 ].y - contours[ 2 ]
		.y, 2 )));
	auto heightA = std::sqrt(( std::pow( contours[ 0 ].x - contours[ 1 ]
		.x, 2 ) + std::pow( contours[ 0 ].y - contours[ 1 ]
		.y, 2 )));
	auto heightB = std::sqrt(( std::pow( contours[ 3 ].x - contours[ 2 ]
		.x, 2 ) + std::pow( contours[ 3 ].y - contours[ 3 ]
		.y, 2 )));
	auto maxWidth = static_cast<float>(std::max( widthA, widthB ));
	auto maxHeight = static_cast<float>(std::max( heightA, heightB ));
	// warp image to new size
	std::vector<cv::Point2f> dst = {{ 0, 0 },
		{ 0, maxHeight },
		{ maxWidth, maxHeight },
		{ maxWidth, 0 }};
	std::vector<cv::Point2f> rect = {
		{ static_cast<float>(contours[ 0 ].x),
			static_cast<float>(contours[ 0 ].y)
		},
		{ static_cast<float>(contours[ 1 ].x),
			static_cast<float>(contours[ 1 ].y)
		},
		{ static_cast<float>(contours[ 2 ].x),
			static_cast<float>(contours[ 2 ].y)
		},
		{ static_cast<float>(contours[ 3 ].x),
			static_cast<float>(contours[ 3 ].y)
		}};
	cv::Mat transform = cv::getPerspectiveTransform( rect, dst );
	cv::Mat warped;
	cv::warpPerspective( src, warped, transform, { (int) maxWidth,
		(int) maxHeight
	} );
	
	return warped;
}

std::vector<cv::Vec2f>
CVSegmentation::mergedHoughLines( const cv::Mat& binImg ) {
	cv::Mat edges;
	cv::Canny( binImg, edges, 50, 200, 3 );
	
	std::vector<cv::Vec2f> lines;
	/* rho is the radians that get checked for and theta the amount of degrees a line can have. We basically only check for perpendicular lines */
	cv::HoughLines( edges, lines, 5, CV_PI / 2, 480, 0, 0 ); // runs the actual detection
	//TODO id lines < 20 failure
	return mergedHoughLinesImpl( lines, toRad( 3 ), 50 );
}

std::vector<cv::Vec2f>
CVSegmentation::mergedHoughLinesImpl( const std::vector<cv::Vec2f>& lines, float thetaMax, float rhoMax ) {
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

float CVSegmentation::toRad( float deg ) {
	return deg * static_cast<float>(CV_PI) / 180.f;
}

cv::Mat
CVSegmentation::drawSegmentedRectImg( const std::vector<cv::Mat>& squares, int borderWidth ) {
	auto changeColorBorder = [ & ]( auto& m1, auto& m2, int pos ) {
		if( pos % 2 == 0 )
			cv::copyMakeBorder( m1, m2, borderWidth, borderWidth, borderWidth, borderWidth, cv::BORDER_CONSTANT, cv::Scalar( 255, 0, 0 ));
		else
			cv::copyMakeBorder( m1, m2, borderWidth, borderWidth, borderWidth, borderWidth, cv::BORDER_CONSTANT, cv::Scalar( 0, 0, 255 ));
	};
	cv::Mat squaredImg;
	std::vector<cv::Mat> squaresRows( 9 );
	int j { 0 }, nextW { 0 };
	for( int i = 0; i < squaresRows.size(); i++ ) {
		cv::Mat tmpB1, tmpB2;
		cv::Mat clrB1, clrB2;
		cv::cvtColor( squares[ j++ ], clrB1, cv::COLOR_GRAY2BGR );
		changeColorBorder( clrB1, tmpB1, j );
		cv::cvtColor( squares[ j++ ], clrB2, cv::COLOR_GRAY2BGR );
		changeColorBorder( clrB2, tmpB2, j );
		cv::hconcat( tmpB1, tmpB2, squaresRows[ i ] );
		nextW += 9;
		for( ; j < nextW; ) {
			cv::cvtColor( squares[ j++ ], clrB1, cv::COLOR_GRAY2BGR );
			changeColorBorder( clrB1, tmpB1, j );
			cv::hconcat( squaresRows[ i ], tmpB1, squaresRows[ i ] );
		}
		if( i == 1 ) {
			cv::vconcat( squaresRows[ i - 1 ], squaresRows[ i ], squaredImg );
		}
		if( i > 1 ) {
			cv::vconcat( squaredImg, squaresRows[ i ], squaredImg );
		}
	}
	return squaredImg;
}