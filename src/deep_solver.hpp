#pragma once
#include <fdeep/fdeep.hpp>
#include "img_proc/cv_segmentation.hpp"
#include <iostream>

class DeepSolver
{

public:
	struct ReturnVal
	{
		int position;
		int value;
	};
	using Board = std::array<std::array<std::pair<int, bool>, 9>, 9>;
	using Quadratic = std::pair<u_int16_t, u_int16_t>;
	
	explicit DeepSolver( const std::string& path );
	explicit DeepSolver( QFile& path );
	
	std::vector<ReturnVal> predictMulti( const QImage& src );
	bool solveFromImage( const QImage& src );
	void printBoard();
	Board& getBoard();
private:
	/* DEEP NETWORK */
	int get_result( const fdeep::tensors& t );
	void populate_board( const std::vector<ReturnVal>& results ) noexcept;
	/* SUDOKU */
	bool solve_sudoku();
	bool is_present_in_col( int col, int num );
	bool is_present_in_row( int row, int num );
	bool is_present_in_box( int offset_row, int offset_col, int num );
	bool find_empty_place( int& row, int& col );
	bool is_valid( int row, int col, int num );
	int quad_to_lin( int row, int col );
	Quadratic lin_to_quad( int position );

private:
	fdeep::model m_model;
	Board m_solvedSudoku;
	static const int m_gridSize { 9 };
};