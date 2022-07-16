
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
        bool ok;
        const int posValue = value.toInt(&ok);
        if(!ok)
            return false;
        if ((posValue < 1 || posValue > 9) && posValue != -1 )
            return false;

        m_dataGrid[index.row()][index.column()].setValue(posValue);
        roles << ValueRole;
        if(updateConflicts()) {
            roles << ConflictRole;
            topLeft = createIndex(0, 0);
            bottomRight = createIndex(8, 8);
        }
    } else if ( role == StatusRole) {
        const QString status = value.toString();
        if ( status != "none" && status != "selected" )
            return false;
        if ( status == "selected" ) {
            for( int row = 0; row < 9 ; row++ ) {
                for( int column = 0; column < 9; column++ ) {
                    if ( row == index.row() ) {
                        if ( column == index.column() )
                            m_dataGrid[row][column].setStatus("selected");
                        m_dataGrid[row][column].setStatus("same_row");
                    } else if ( column == index.column() ) {
                        m_dataGrid[ row ][ column ].setStatus("same_column");
                    } else {
                        m_dataGrid[ row ][ column ].setStatus("none");
                    }
                }
            }
            topLeft = createIndex(0, 0);
            bottomRight = createIndex(8, 8);
        }
        roles << StatusRole;
        m_dataGrid[index.row()][index.column()].setStatus(status);
    }
    emit dataChanged(topLeft, bottomRight, roles);
    return true;
}

Qt::ItemFlags SudokuBoardModel::flags( const QModelIndex &index ) const {
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> SudokuBoardModel::roleNames() const {
    QHash<int, QByteArray> names;
    names[ValueRole] = "value";
    names[StatusRole] = "status";
    names[ConflictRole] = "conflict";
    names[EditableRole] = "editable";
    return names;
}

void SudokuBoardModel::clear() {
    for( int row = 0; row < 9 ; row++ ) {
        for( int column = 0; column < 9; column++ ) {
            m_dataGrid[row][column].setValue(-1);
        }
    }
    emit dataChanged(createIndex(0, 0), createIndex(8, 8), QVector<int>() << ValueRole);
}

void SudokuBoardModel::setCellEditable( int row, int col ) {
    m_dataGrid[row][col].setEditable(true);
    emit dataChanged(createIndex(row, col), createIndex(row, col), QVector<int>() << EditableRole);
}

void SudokuBoardModel::makeAllCellsUneditable() {
    for(auto& row : m_dataGrid) {
        for(auto& cell : row)
            cell.setEditable(false);
    }
    emit dataChanged(createIndex(0, 0), createIndex(8, 8), QVector<int>() << EditableRole);
}

bool SudokuBoardModel::setData( int row, int column, int value ) {
    return setData(createIndex(row, column), value, ValueRole);
}

bool SudokuBoardModel::updateConflicts() {

    bool isChanged = false;

    for (int targetRow = 0; targetRow < 9; ++targetRow) {
        for (int targetCol = 0; targetCol < 9; ++targetCol) {
            if (m_dataGrid[targetRow][targetCol].conflict()) {
                m_dataGrid[targetRow][targetCol].setConflict(false);
                isChanged = true;
            }

            if (m_dataGrid[targetRow][targetCol] == -1)
                continue;

            int value = m_dataGrid[targetRow][targetCol].value();

            // Check same row conflict
            for (int col = 0; col < 9; ++col) {
                if (col != targetCol && m_dataGrid[targetRow][col] == value) {
                    m_dataGrid[targetRow][targetCol].setConflict(true);
                    isChanged = true;
                }
            }

            // Check same column conflict
            for (int row = 0; row < 9; ++row) {
                if (row != targetRow && m_dataGrid[row][targetCol] == value) {
                    m_dataGrid[targetRow][targetCol].setConflict(true);
                    isChanged = true;
                }
            }

            // Check same square conflict
            Index quadraticIndex = mapFromLinear({targetRow, targetCol});
            for (int col = 0; col < 9; ++col) {
                Index testIndex = mapToLinear({quadraticIndex.row, col});
                if (col != quadraticIndex.column && m_dataGrid[testIndex.row][testIndex.column] == value) {
                    m_dataGrid[targetRow][targetCol].setConflict(true);
                    isChanged = true;
                }
            }
        }
    }

    return isChanged;
}
