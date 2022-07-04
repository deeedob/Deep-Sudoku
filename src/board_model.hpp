#pragma once
#include <QAbstractTableModel>
#include "cell_data.hpp"

class SudokuBoardModel : public QAbstractTableModel
{
    Q_OBJECT
    using grid = std::array<std::array<CellData, 9>, 9>;
public:
    enum CellRoles {
        ValueRole = Qt::UserRole +1,
        StatusRole,
        ConflictRole,
        EditableRole
    };

    SudokuBoardModel(QObject *parent = 0);

    int rowCount( const QModelIndex &parent ) const override;
    int columnCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;
    bool setData( const QModelIndex &index, const QVariant &value, int role ) override;
    Qt::ItemFlags flags( const QModelIndex &index ) const override;
    QHash<int, QByteArray> roleNames() const override;

    void clear();
    void setCellEditable(int row, int col);
    void makeAllCellsUneditable(int row, int column, int value);

protected:
    bool setData(int row, int column, int value);
private:
    bool updateConflicts();

private:
    grid m_dataGrid;
};