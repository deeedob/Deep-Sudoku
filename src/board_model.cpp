
#include "board_model.hpp"

SudokuBoardModel::SudokuBoardModel( QObject *parent )
        : QAbstractTableModel(parent)
{ }

int SudokuBoardModel::rowCount( const QModelIndex &parent ) const {
    Q_UNUSED(parent)
    return 9;
}

int SudokuBoardModel::columnCount( const QModelIndex &parent ) const {
    Q_UNUSED(parent)
    return 9;
}

QVariant SudokuBoardModel::data( const QModelIndex &index, int role ) const {
    if( index.row() < 0 || index.row() > 8 || index.column() < 0 || index.row() > 8)
        return QVariant();

    if( role == ValueRole || role == Qt::DisplayRole)
        return m_dataGrid[index.row()][index.column()].value();
    else if( role == StatusRole )
        return m_dataGrid[index.row()][index.column()].status();
    else if( role == ConflictRole )
        return m_dataGrid[index.row()][index.column()].conflict();
    else if( role == EditableRole )
        return m_dataGrid[index.row()][index.column()].editable();

    return QVariant();
}

bool SudokuBoardModel::setData( const QModelIndex &index, const QVariant &value, int role ) {
    if( index.row() < 0 || index.row() > 8 || index.column() < 0 || index.row() > 8)
        return false;

    QModelIndex topLeft = index;
    QModelIndex bottomRight = index;
    QVector<int> roles;
    if( role == ValueRole || role == Qt::DisplayRole || role == Qt::EditRole ) {

    }
}

Qt::ItemFlags
SudokuBoardModel::flags( const QModelIndex &index ) const {
    return QAbstractTableModel::flags(index);
}

QHash<int, QByteArray> SudokuBoardModel::roleNames() const {
    return QAbstractItemModel::roleNames();
}

void SudokuBoardModel::clear() {

}

void SudokuBoardModel::setCellEditable( int row, int col ) {

}

void SudokuBoardModel::makeAllCellsUneditable( int row, int column,
                                               int value ) {

}

bool SudokuBoardModel::setData( int row, int column, int value ) {
    return false;
}

bool SudokuBoardModel::updateConflicts() {
    return false;
}
