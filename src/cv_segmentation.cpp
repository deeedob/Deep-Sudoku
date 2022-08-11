#include "cv_segmentation.hpp"

CVSegmentation::CVSegmentation( const QImage& src )
	: m_success( true ), m_processed( false )
{
	m_orig = qt_ocv::image2Mat( src, CV_8UC( 4 ), qt_ocv::MCO_BGRA );
}

bool CVSegmentation::process()
{
	m_origBin = binarizedImg( m_orig );
	m_contour = getRectangularContour( m_origBin );
	m_binCutted = warpSelection( m_origBin, m_contour );
	m_mergedLines = mergedHoughLines( m_binCutted );
	if( m_mergedLines.size() < 20 )
		m_success = false;
	if( m_success ) {
		m_intersections = getIntersections( m_mergedLines );
		m_preparedSquares = preparedSquares( cutSquares( m_intersections, m_binCutted ), m_numbFillFactor, m_detectionSize );
	}
	m_processed = true;
	return m_success;
}

const cv::Mat& CVSegmentation::getOriginal()
{
	return m_orig;
}

const cv::Mat& CVSegmentation::getBinarizedImg()
{
	if( m_processed )
		return m_origBin;
}

const cv::Mat& CVSegmentation::getBinarizedCuttedImg()
{
	if( m_processed )
		return m_binCutted;
}

cv::Mat CVSegmentation::getMergedLinesImg()
{
	cv::Mat lines_img;
	cv::cvtColor( m_binCutted, lines_img, cv::COLOR_GRAY2BGR );
	for( auto& merged_line : m_mergedLines ) {
		float rho = merged_line[ 0 ], theta = merged_line[ 1 ];
		cv::Point pt_1, pt_2;
		double a = cos( theta ), b = sin( theta );
		double x_0 = a * rho, y_0 = b * rho;
		pt_1.x = cvRound( x_0 + m_orig.cols * ( -b ));
		pt_1.y = cvRound( y_0 + m_orig.rows * ( a ));
		pt_2.x = cvRound( x_0 - m_orig.cols * ( -b ));
		pt_2.y = cvRound( y_0 - m_orig.rows * ( a ));
		cv::line( lines_img, pt_1, pt_2, cv::Scalar( 0, 0, 255 ), 12, cv::LINE_AA );
	}
	return lines_img;
}

cv::Mat CVSegmentation::getIntersectionImg()
{
	cv::Mat intersection_img;
	cv::cvtColor( m_binCutted, intersection_img, cv::COLOR_GRAY2BGR );
	for( auto& i : m_intersections ) {
		cv::circle( intersection_img, i, 25, cv::Scalar( 255, 0, 255 ), -1 );
	}
	return intersection_img;
}

cv::Mat CVSegmentation::getPreparedSquaresImg()
{
	if( m_success )
		return drawSegmentedRectImg( m_preparedSquares, 3 );
	else
		return cv::Mat::ones( m_orig.rows, m_orig.cols, CV_8UC1);
}

void CVSegmentation::setImage( const QImage& src ) noexcept
{
	m_orig = qt_ocv::image2Mat( src, CV_8UC( 4 ), qt_ocv::MCO_BGRA );
}

const std::vector<cv::Mat>& CVSegmentation::getPreparedSquares() const noexcept
{
	return m_preparedSquares;
}

const std::vector<int>& CVSegmentation::getUsedSquares() const noexcept
{
	return m_usedSquares;
}

std::pair<size_t, size_t> CVSegmentation::getNNSize() noexcept
{
	return m_nnSize;
}

cv::Mat
CVSegmentation::binarizedImg( cv::Mat src, int gauss_value, bool dilating, bool eroding )
{
	cv::Mat out;
	auto ones = []( int width, int height ) {
		return cv::Mat::ones( cv::Size( width, height ), CV_8U );
	};
	cv::cvtColor( src, out, cv::COLOR_BGR2GRAY );
	cv::GaussianBlur( out, out, cv::Size( gauss_value, gauss_value ), 3 );
	//TODO adaptive Threshold very slow!!
	cv::adaptiveThreshold( out, out, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 199, 25 );
	if( dilating )
		cv::dilate( out, out, ones( 3, 3 ), cv::Point( 0, 0 ), 1 );
	if( eroding )
		cv::erode( out, out, ones( 3, 3 ), cv::Point( 0, 0 ), 2 );
	return out;
}

std::vector<cv::Point>
CVSegmentation::getRectangularContour( cv::Mat src ) const
{
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours( src, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE );
	
	struct
	{
		bool operator()( const std::vector<cv::Point>& c_1, const std::vector<cv::Point>& c_2 )
		{
			auto i = fabs( cv::contourArea( cv::Mat( c_1 )));
			auto j = fabs( cv::contourArea( cv::Mat( c_2 )));
			return i > j;
		}
	} cmp_contour_area;
	
	std::sort( contours.begin(), contours.end(), cmp_contour_area );
	
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
CVSegmentation::warpSelection( cv::Mat src, const std::vector<cv::Point>& contours )
{
	//width of the new image
	auto width_a = std::sqrt(( std::pow( contours[ 0 ].x - contours[ 3 ]
		.x, 2 ) + std::pow( contours[ 0 ].y - contours[ 3 ]
		.y, 2 )));
	auto width_b = std::sqrt(( std::pow( contours[ 1 ].x - contours[ 2 ]
		.x, 2 ) + std::pow( contours[ 1 ].y - contours[ 2 ]
		.y, 2 )));
	auto height_a = std::sqrt(( std::pow( contours[ 0 ].x - contours[ 1 ]
		.x, 2 ) + std::pow( contours[ 0 ].y - contours[ 1 ]
		.y, 2 )));
	auto height_b = std::sqrt(( std::pow( contours[ 3 ].x - contours[ 2 ]
		.x, 2 ) + std::pow( contours[ 3 ].y - contours[ 3 ]
		.y, 2 )));
	auto max_width = static_cast<float>(std::max( width_a, width_b ));
	auto max_height = static_cast<float>(std::max( height_a, height_b ));
	// warp image to new size
	std::vector<cv::Point2f> dst = {{ 0, 0 },
	                                { 0, max_height },
	                                { max_width, max_height },
	                                { max_width, 0 }};
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
	cv::warpPerspective( src, warped, transform, { (int) max_width,
	                                               (int) max_height
	} );
	
	return warped;
}

#include <iostream>

std::vector<cv::Vec2f>
CVSegmentation::mergedHoughLines( cv::Mat bin_img )
{
	cv::Mat edges;
	cv::Canny( bin_img, edges, 50, 200, 3 );
	std::vector<cv::Vec2f> lines;
	/* rho is the radians that get checked for and theta the amount of degrees a line can have. We basically only check for perpendicular lines */
	cv::HoughLines( edges, lines, 5, CV_PI / 2, 480, 0, 0 ); // runs the actual detection
	return mergedHoughLinesImpl( lines, toRad( 3 ), 50 );
}
//TODO: implement later
#if 0
/* TODO: move to seperate class! */
std::pair<cv::Mat, cv::Mat> CVSegmentation::customGradientImage( cv::Mat binImg )
{
	cv::Mat_<float> sobelX( 3, 3 ), sobelY( 3, 3 );
	cv::Mat gradX, gradY;
	sobelX << -1 / 8, 0, 1 / 8,
		-2 / 8, 0, 2 / 8,
		-1 / 8, 0, 1 / 8;
	sobelY << 1 / 8, 2 / 8, 1 / 8,
		0, 0, 0,
		-1 / 8, -2 / 8, -1 / 8;
	cv::filter2D( binImg, gradX, -1, sobelX );
	cv::filter2D( binImg, gradY, -1, sobelY );
	return { gradX, gradY };
}

std::vector<cv::Vec2f>
CVSegmentation::customHoughLinesImpl( cv::Mat bin_img )
{
	auto gradients = customGradientImage( bin_img );
	auto get_theta_rho_magnitude = [ & ]( cv::Point coord ) {
		auto val_1 = gradients.first.at<uchar>( coord );
		auto val_2 = gradients.second.at<uchar>( coord );
		auto theta = std::atan( val_1 / val_2 );
		auto rho = std::abs( coord.y * std::cos( theta ) + coord.x * std::sin( theta ));
		auto magnitude = std::sqrt( std::pow( val_1, 2 ) + std::pow( val_2, 2 ));
		
		return std::make_tuple( theta, rho, magnitude );
	};
	auto make_accumulator = [ & ]() {
		auto theta_res = M_PI / 720;
		auto rho_res = 1;
		auto min_magnitude = 1;
		auto max_angle_deviation = 20 * M_PI / 180;
		auto min_n_of_votes = 230;
		bool normalize = true;
		
		auto hough_x = static_cast<int>(( M_PI * 2 / theta_res ));
		auto hough_y = static_cast<int>(std::pow( gradients.first.cols, 2 ) + std::pow( gradients.first.rows, 2 )) + 1;
		std::vector<std::vector<uchar>> h_accumulator( hough_y, std::vector<uchar>( hough_x, 0 ));
		for( int x = 0; x < gradients.first.rows; x++ ) {
			for( int y = 0; y < gradients.first.cols; y++ ) {
				auto trm = get_theta_rho_magnitude( { x, y } );
				auto theta = std::get<0>( trm );
				auto rho = std::get<1>( trm );
				auto mag = std::get<2>( trm );
				if( mag >= min_magnitude && !std::isnan( theta ))
					if( std::abs(( M_PI / 2 ) - theta ) <= max_angle_deviation ||
						std::abs( theta ) <= max_angle_deviation ) {
						auto hough_tx = static_cast<int>(theta / theta_res);
						auto hough_ty = static_cast<int>(rho / rho_res);
						h_accumulator[ y ][ x ]++;
					}
			}
		}
		if( min_n_of_votes != 0 ) {
			for( auto& y : h_accumulator ) {
				for( auto& x : y ) {
					if( x < min_n_of_votes )
						x = 0;
				}
			}
		}
		if( normalize ) {
			uchar max { std::numeric_limits<uchar>::min() };
			for( const auto& y : h_accumulator ) {
				max = std::max( max, *std::max_element( y.begin(), y.end()));
			}
			for( auto& y : h_accumulator ) {
				for( auto& x : y ) {
					x /= ( max * 255 );
				}
			}
		}
		return cv::Mat( h_accumulator );
	};
	
	cv::Mat hough_accumulator = make_accumulator();
	
	auto get_lines = [ & ]() {
		auto theta_res = M_PI / 180;
		auto rho_res = 1;
		cv::Mat bin_accu;
		cv::threshold( hough_accumulator, bin_accu, 1, 255, cv::THRESH_BINARY );
		cv::Mat_<uchar> erod_kern_5, erod_kern_3;
		erod_kern_5 << 0, 0, 1, 0, 0,
			0, 0, 1, 0, 0,
			0, 0, 1, 0, 0,
			0, 0, 1, 0, 0,
			0, 0, 1, 0, 0;
		// TODO: ?!?!
		cv::erode( bin_accu, bin_accu, erod_kern_5, { -1, -1 }, 6 );
		cv::dilate( bin_accu, bin_accu, 5 );
		cv::erode( bin_accu, bin_accu, 5 );
		cv::dilate( bin_accu, bin_accu, 17 );
		cv::erode( bin_accu, bin_accu, 15 );
		
		cv::Mat marked_accu = cv::Mat::zeros( bin_accu.rows, bin_accu.cols, CV_8UC1);
		auto flood_fill = [ & ]( auto&& flood_fill, cv::Point p, std::vector<cv::Vec3f>& lines ) {
			if( p.x < 0 || p.y < 0 || p.x > hough_accumulator.cols || p.y > hough_accumulator.rows )
				return;
			if( bin_accu.at<uchar>( p ) == 0 || marked_accu.at<uchar>( p ) > 0 )
				return;
			marked_accu.at<uchar>( p ) = 255;
			lines.push_back(
				{ static_cast<float>(p.x * rho_res),
				  static_cast<float>(p.y * theta_res),
				  hough_accumulator.at<float>( p )
				} );
			flood_fill( flood_fill, { p.x + 1, p.y }, lines );
		};
		
	};
}
#endif

std::vector<cv::Vec2f>
CVSegmentation::mergedHoughLinesImpl( const std::vector<cv::Vec2f>& lines, float theta_max, float rho_max )
{
	std::vector<std::vector<int >> combine_index( lines.size());
	std::vector<cv::Vec2f> merged_lines;
	for( int i = 0; i < lines.size(); i++ ) {
		int index = i;
		for( int j = i; j < lines.size(); j++ ) {
			float distance_i = lines[ i ][ 0 ], distance_j = lines[ j ][ 0 ];
			float slope_i = lines[ i ][ 1 ], slope_j = lines[ j ][ 1 ];
			float dis_diff = abs( distance_i - distance_j );
			float slope_diff = abs( slope_i - slope_j );
			
			if( slope_diff < theta_max && dis_diff < rho_max ) {
				bool is_combined = false;
				for( int w = 0; w < i; w++ ) {
					for( int u = 0; u < combine_index[ w ].size(); u++ ) {
						if( combine_index[ w ][ u ] == j ) {
							is_combined = true;
							break;
						}
						if( combine_index[ w ][ u ] == i )
							index = w;
					}
					if( is_combined )
						break;
				}
				if( !is_combined )
					combine_index[ index ].push_back( j );
			}
		}
	}
	
	for( auto& i : combine_index ) {
		if( i.empty())
			continue;
		cv::Vec2f line_temp( 0, 0 );
		for( int j : i ) {
			line_temp[ 0 ] += lines[ j ][ 0 ];
			line_temp[ 1 ] += lines[ j ][ 1 ];
		}
		line_temp[ 0 ] /= i.size();
		line_temp[ 1 ] /= i.size();
		merged_lines.push_back( line_temp );
	}
	return merged_lines;
}

std::vector<cv::Point>
CVSegmentation::getIntersections( const std::vector<cv::Vec2f>& merged_lines )
{
	// find intersections from hough lines
	std::vector<cv::Point> intersections;
	auto get_intersect = []( const cv::Vec2f& l_1, const cv::Vec2f& l_2 ) {
		// get intersections of perpendicular lines
		int x, y = { 0 };
		if( l_1[ 1 ] == l_2[ 1 ] )
			return cv::Point( -1, -1 );
		if( l_1[ 1 ] == 0 ) {
			x = static_cast<int>(l_1[ 0 ]);
			y = static_cast<int>(l_2[ 0 ]);
		}
		if( l_2[ 1 ] == 0 ) {
			x = static_cast<int>(l_2[ 0 ]);
			y = static_cast<int>(l_1[ 0 ]);
		}
		return cv::Point( x, y );
	};
	for( int i = 0; i < merged_lines.size(); i++ ) {
		for( int j = i + 1; j < merged_lines.size(); j++ ) {
			auto intersect = get_intersect( merged_lines[ i ], merged_lines[ j ] );
			if( intersect.x != -1 && intersect.y != -1 )
				intersections.push_back( intersect );
		}
	}
	// sort points on the same line
	auto sort_lr_same_row = []( const cv::Point& p_1, const cv::Point& p_2 ) {
		if( p_1.y == p_2.y )
			return p_1.x < p_2.x;
		return p_1.y < p_2.y;
	};
	std::sort( intersections.begin(), intersections.end(), sort_lr_same_row );
	
	return intersections;
}

std::vector<cv::Mat>
CVSegmentation::cutSquares( const std::vector<cv::Point>& intersections, cv::Mat bin_img )
{
	std::vector<cv::Mat> squares;
	int cnt { 0 };
	for( int i = 0; i < intersections.size() - 11; i++, cnt++ ) {
		if( cnt > 8 ) {
			i++;
			cnt = 0;
		}
		cv::Point p_1 = intersections[ i ], p_2 = intersections[ i + 11 ];
		cv::Rect roi( p_1.x, p_1.y, p_2.x - p_1.x, p_2.y - p_1.y );
		cv::Mat cropped_ref( bin_img, roi );
		cv::Mat cropped;
		//croppedRef.copyTo( cropped );
		squares.push_back( cropped_ref );
	}
	
	return squares;
}

std::vector<cv::Mat>
CVSegmentation::preparedSquares( const std::vector<cv::Mat>& squares, float number_fill_factor, DetectionSize d )
{
	assert(( number_fill_factor > 0 && number_fill_factor < 1 ) && "numberFillFactor must be between 0 and 1" );
	assert((( d.minWidth > 0 && d.minWidth < 1 ) && ( d.maxWidth > 0 && d.maxWidth < 1 ) &&
	        ( d.minHeight > 0 && d.minHeight < 1 ) && ( d.maxHeight > 0 && d.maxHeight < 1 )) && "DetectionSize must be between 0 and 1" );
	
	std::vector<cv::Mat> prepared_squares;
	
	int cnt { 0 };
	for( auto& c : squares ) {
		std::vector<std::vector<cv::Point>> prepared;
		cv::Mat candidate = cv::Mat::ones( cv::Size( m_nnSize.first, m_nnSize.second ), CV_8UC1);
		
		cv::findContours( c, prepared, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );
		
		for( auto& j : prepared ) {
			auto r = cv::boundingRect( j );
			/* if a number matches a rect size of a number ... */
			if( r.width > c.cols * d.minWidth && r.width < c.cols * d.maxWidth &&
			    r.height > c.rows * d.minHeight && r.height < c.cols * d.maxHeight ) {
				
				m_usedSquares.push_back( cnt );
				cv::Mat cropped_ref( c, r );
				/* if the numbers bounding box is bigger than the
				 * blank image crop is with respect to their aspect ratio */
				if( cropped_ref.cols > candidate.cols ||
				    cropped_ref.rows > candidate.rows ) {
					cv::Mat resized;
					// resize with aspect ratio
					auto ratio = getAspectRatio( cropped_ref );
					cv::resize( cropped_ref, resized, cv::Size( static_cast<int>((float) m_nnSize.first * number_fill_factor), static_cast<int>((float) m_nnSize.second * number_fill_factor )), ratio, ratio );
					cropped_ref = resized;
				}
				
				int x = ( static_cast<int>(m_nnSize.first) - cropped_ref.cols ) / 2;
				int y = ( static_cast<int>(m_nnSize.second) - cropped_ref.rows ) / 2;
				//copy the croppedRef to the candidate
				cropped_ref.copyTo( candidate( cv::Rect( x, y, cropped_ref.cols, cropped_ref.rows )));
			}
		}
		prepared_squares.push_back( candidate );
		cnt++;
	}
	return prepared_squares;
}

float CVSegmentation::getAspectRatio( cv::Mat img )
{
	return img.cols > img.rows ? static_cast<float>(img.cols) / img
		.rows : static_cast<float>(img.rows) / img.cols;
}

float CVSegmentation::toRad( float deg )
{
	return deg * static_cast<float>(CV_PI) / 180.f;
}

cv::Mat
CVSegmentation::drawSegmentedRectImg( const std::vector<cv::Mat>& squares, int border_width )
{
	auto change_color_border = [ & ]( auto& m_1, auto& m_2, int pos ) {
		if( pos % 2 == 0 )
			cv::copyMakeBorder( m_1, m_2, border_width, border_width, border_width, border_width, cv::BORDER_CONSTANT, cv::Scalar( 255, 0, 0 ));
		else
			cv::copyMakeBorder( m_1, m_2, border_width, border_width, border_width, border_width, cv::BORDER_CONSTANT, cv::Scalar( 0, 0, 255 ));
	};
	cv::Mat squared_img;
	std::vector<cv::Mat> squares_rows( 9 );
	int j { 0 }, next_w { 0 };
	for( int i = 0; i < squares_rows.size(); i++ ) {
		cv::Mat tmp_b_1, tmp_b_2;
		cv::Mat clr_b_1, clr_b_2;
		cv::cvtColor( squares[ j++ ], clr_b_1, cv::COLOR_GRAY2BGR );
		change_color_border( clr_b_1, tmp_b_1, j );
		cv::cvtColor( squares[ j++ ], clr_b_2, cv::COLOR_GRAY2BGR );
		change_color_border( clr_b_2, tmp_b_2, j );
		cv::hconcat( tmp_b_1, tmp_b_2, squares_rows[ i ] );
		next_w += 9;
		for( ; j < next_w; ) {
			cv::cvtColor( squares[ j++ ], clr_b_1, cv::COLOR_GRAY2BGR );
			change_color_border( clr_b_1, tmp_b_1, j );
			cv::hconcat( squares_rows[ i ], tmp_b_1, squares_rows[ i ] );
		}
		if( i == 1 ) {
			cv::vconcat( squares_rows[ i - 1 ], squares_rows[ i ], squared_img );
		}
		if( i > 1 ) {
			cv::vconcat( squared_img, squares_rows[ i ], squared_img );
		}
	}
	return squared_img;
}