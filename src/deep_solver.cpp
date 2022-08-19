#include "deep_solver.hpp"
#include <filesystem>
#include <opencv2/highgui.hpp>

DeepSolver::DeepSolver( const std::string& path )
	: m_model { fdeep::load_model( path ) }
{
}

bool DeepSolver::solveFromImage( const QImage& src )
{
	auto ret_val = predictMulti( src );
	if( ret_val.empty())
		return false;
	
	populate_board( ret_val );
	return solve_sudoku();
}

std::vector<DeepSolver::ReturnVal> DeepSolver::predictMulti( const QImage& src )
{
	CVSegmentation temp( src );
	//TODO: remove next line in prod
	temp.process();
	//if( !temp.process())
	//	return { };
	
	auto squares = temp.getPreparedSquares();
	auto used_squares = temp.getUsedSquareNums();
	
	auto size = CVSegmentation::getNNSize();
	fdeep::tensor_shape shape( size.first, size.second, 1 );
	
	std::vector<fdeep::tensors> input;
	
	for( int used_square : used_squares ) {
		cv::Mat flat_f;
		squares[ used_square ].convertTo( flat_f, CV_32FC1);
		std::vector<float> vals( flat_f.begin<float>(), flat_f.end<float>());
		fdeep::tensors t {{ shape, vals }};
		input.push_back( t );
	}
	auto predictions = m_model.predict_multi( input, true );
	
	std::vector<ReturnVal> ret;
	ret.reserve( predictions.size());
	
	for( int i = 0; i < predictions.size(); i++ ) {
		auto val = get_result( predictions[ i ] );
		auto pos = used_squares[ i ];
		ret.push_back( { pos, val } );
	}
	
	
	/*  DEBUGGING */
	//auto cutted = temp.getBinarizedCuttedImg();
	//cv::Mat resizedImg1;
	//cv::resize( cutted, resizedImg1, cv::Size( 700, 700 ), 0, 0, cv::INTER_CUBIC );
	//cv::namedWindow( "binCuttedImg", cv::WINDOW_KEEPRATIO );
	//cv::imshow( "binCuttedImg", resizedImg1 );
	//
	///* show merged lines */
	//cv::Mat resizedImg2;
	//auto mergedLines = temp.getMergedLinesImg();
	//cv::resize( mergedLines, resizedImg2, cv::Size( 700, 700 ), 0, 0, cv::INTER_AREA );
	//cv::namedWindow( "lines", cv::WINDOW_KEEPRATIO );
	//cv::imshow( "lines", resizedImg2 );
	//
	///* show intersections */
	//auto intersections = temp.getIntersectionImg();
	//cv::Mat resizedImg3;
	//cv::resize( intersections, resizedImg3, cv::Size( 700, 700 ), 0, 0, cv::INTER_CUBIC );
	//cv::namedWindow( "intersections", cv::WINDOW_KEEPRATIO );
	//cv::imshow( "intersections", resizedImg3 );
	//
	//auto preparedImg = temp.getPreparedSquaresImg();
	//cv::Mat resizedImg4;
	//cv::resize( preparedImg, resizedImg4, cv::Size( 700, 700 ), 0, 0, cv::INTER_CUBIC );
	//cv::namedWindow( "prepared", cv::WINDOW_KEEPRATIO );
	//cv::imshow( "prepared", resizedImg4 );
	
	return ret;
}

void DeepSolver::printBoard()
{
	for( int row = 0; row < m_gridSize; row++ ) {
		for( int col = 0; col < m_gridSize; col++ ) {
			auto[val, type] = m_solvedSudoku[ row ][ col ];
			std::cout << "[" << val << ",";
			if( type )
				std::cout << "E"; //editable
			else
				std::cout << "O"; //original number
			std::cout << "]";
			if(( col + 1 ) % 3 == 0 )
				std::cout << " ";
		}
		std::cout << "\n";
		if(( row + 1 ) % 3 == 0 )
			std::cout << "\n";
	}
	std::cout << "_________________________________________\n";
}

int DeepSolver::get_result( const fdeep::tensors& t )
{
	auto v = t.at( 0 ).as_vector();
	auto old_max = v->at( 0 );
	int value = 1;
	for( int i = 1; i < v->size(); i++ ) {
		auto maybe = v->at( i );
		if( maybe > old_max ) {
			old_max = maybe;
			value = i + 1;
		}
	}
	return value;
}

void DeepSolver::populate_board( const std::vector<ReturnVal>& results ) noexcept
{
	for( const auto& i : results ) {
		auto[h, w] = lin_to_quad( i.position );
		m_solvedSudoku[ h ][ w ].first = i.value;
		m_solvedSudoku[ h ][ w ].second = false;
	}
	for( auto& i : m_solvedSudoku ) {
		for( auto& j : i ) {
			if(( j.first <= 0 || j.first > 9 )) {
				j.first = -1;
				j.second = true;
			}
		}
	}
}

DeepSolver::Board& DeepSolver::getBoard()
{
	return m_solvedSudoku;
}

bool DeepSolver::solve_sudoku()
{
	int row, col;
	if( !find_empty_place( row, col ))
		return true;                    //all numbers found!
	for( int num = 1; num <= 9; num++ ) {
		if( is_valid( row, col, num )) { //number is valid at position
			m_solvedSudoku[ row ][ col ].first = num;
			if( solve_sudoku())       //recursively search in the grid
				return true;
			m_solvedSudoku[ row ][ col ].first = -1;
		}
	}
	return false;
}

bool DeepSolver::is_present_in_col( int col, int num )
{
	
	for( int row = 0; row < m_gridSize; row++ ) {
		if( m_solvedSudoku[ row ][ col ].first == num )
			return true;
	}
	return false;
}

bool DeepSolver::is_present_in_row( int row, int num )
{
	for( int col = 0; col < m_gridSize; col++ ) {
		if( m_solvedSudoku[ row ][ col ].first == num )
			return true;
	}
	return false;
}

bool DeepSolver::is_present_in_box( int offset_row, int offset_col, int num )
{
	for( int row = 0; row < 3; row++ ) {
		for( int col = 0; col < 3; col++ ) {
			if( m_solvedSudoku[ row + offset_row ][ col + offset_col ].first == num ) {
				return true;
			}
		}
	}
	return false;
}

bool DeepSolver::find_empty_place( int& row, int& col )
{
	for( row = 0; row < m_gridSize; row++ ) {
		for( col = 0; col < m_gridSize; col++ ) {
			if( m_solvedSudoku[ row ][ col ].first == -1 )
				return true;
		}
	}
	return false;
}

bool DeepSolver::is_valid( int row, int col, int num )
{
	return !is_present_in_row( row, num )
	       && !is_present_in_col( col, num )
	       && !is_present_in_box( row - row % 3, col - col % 3, num );
}

int DeepSolver::quad_to_lin( int row, int col )
{
	return ( row * m_gridSize ) + col;
}

DeepSolver::Quadratic DeepSolver::lin_to_quad( int position )
{
	int w = ( position % m_gridSize );
	int h = ( position / m_gridSize );
	return { h, w };
}