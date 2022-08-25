#pragma once
#include <QAbstractListModel>
#include "deep_solver.hpp"
#include "cell_data.hpp"

namespace dsdk
{
	/*!
	* @ingroup QT
	* @brief Serves as the main model for the Sudoku Game and is used by the QMLEngine.
	* @author Dennis Oberst
	*/
	class BoardModel : public QAbstractListModel
	{
	Q_OBJECT
	public:
		using Board = std::array<std::array<CellData, 9>, 9>;
		using Quadratic = std::pair<u_int16_t, u_int16_t>;
		enum CellRoles
		{
			ValueRole = Qt::UserRole + 1,
			StatusRole,
			ConflictRole,
			EditableRole,
			SolvedRole
		};
		/*!
		 * @brief Constructs a Sudoku-Grid model
		 * @param width  width of the sudoku
		 * @param height height of the sudoku
		 */
		BoardModel( u_int16_t width, u_int16_t height, QObject* parent = nullptr );
		
		/*!
		 * @brief we only have rows as we are using a GridLayout which uses a List internaly
		 * @return number of rows is the number of cells in the board.
		 */
		int rowCount( const QModelIndex& parent ) const override;
		QVariant data( const QModelIndex& index, int role ) const override;
		bool setData( const QModelIndex& index, const QVariant& value, int role ) override;
		QHash<int, QByteArray> roleNames() const override;
		Qt::ItemFlags flags( const QModelIndex& index ) const override;
		
		void setBoard( const DeepSolver::Board& other ) noexcept;
		Q_INVOKABLE void setCellValue( int index, int value );
		Q_INVOKABLE int getCellSolved( int index );
		Q_INVOKABLE void clearCellValue( int index );
		Q_INVOKABLE void setCellStatus( int index, const QString& status );
		Q_INVOKABLE void clearSelection();
		QModelIndex index( int row, int column, const QModelIndex& parent ) const override;
	private:
		void setSelectionStatus( const QModelIndex& index );
		Quadratic getQuadraticValue( int position ) const;
		Quadratic mapToLinear( Quadratic index );
		bool checkConflicts();
	private:
		u_int16_t m_width;
		u_int16_t m_height;
		Board m_data;
	};
}