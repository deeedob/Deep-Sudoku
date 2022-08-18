#pragma once
#include <QAbstractListModel>
#include "cell_data.hpp"

/*!
 * @author Dennis Oberst
 * @brief Serves as the main Model for the Sudoku Game and is used by the QMLEngine.
 */
class BoardModel : public QAbstractListModel
{
	using grid = std::array<std::array<CellData, 9>, 9>;
	using mapping = std::pair<u_int16_t, u_int16_t>;
Q_OBJECT
public:
	enum CellRoles
	{
		ValueRole = Qt::UserRole + 1,
		StatusRole,
		ConflictRole,
		EditableRole
	};
	/*!
	 * @brief Constructs a Sudoku-Grid model
	 * @param width  width of the sudoku
	 * @param height height of the sudoku
	 */
	BoardModel( u_int16_t width, u_int16_t height, QObject* parent = nullptr );
	
	/*!
	 * @brief we only have rows as we are using a GridLayout which uses a 
	 * @return number of rows is the number of cells in the board.
	 */
	int rowCount( const QModelIndex& parent ) const override;
	QVariant data( const QModelIndex& index, int role ) const override;
	bool setData( const QModelIndex& index, const QVariant& value, int role ) override;
	QHash<int, QByteArray> roleNames() const override;
	Qt::ItemFlags flags( const QModelIndex& index ) const override;
	
	Q_INVOKABLE void setCellValue( int index, int value );
	Q_INVOKABLE void clearCellValue( int index );
	Q_INVOKABLE void setCellStatus( int index, const QString& status );
	Q_INVOKABLE void clearSelection();
	QModelIndex index( int row, int column, const QModelIndex& parent ) const override;
private:
	void setSelectionStatus( const QModelIndex& index );
	mapping getQuadraticValue( int position ) const;
	mapping mapToLinear( mapping index );
	bool checkConflicts();
private:
	u_int16_t m_width;
	u_int16_t m_height;
	grid m_data;
};