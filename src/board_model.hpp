#pragma once
#include <QAbstractListModel>
#include "cell_data.hpp"

class BoardModel : public QAbstractListModel
{
    using grid = std::array<std::array<CellData,9>,9>;
    using mapping = std::pair<u_int16_t, u_int16_t>;
    Q_OBJECT
public:
    enum CellRoles {
        ValueRole = Qt::UserRole +1,
        StatusRole,
        ConflictRole,
        EditableRole
    };

    BoardModel(u_int16_t width, u_int16_t height, QObject* parent = nullptr);

    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;
    bool setData( const QModelIndex &index, const QVariant &value, int role ) override;
    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags( const QModelIndex &index ) const override;

    Q_INVOKABLE void setCellValue(int index, int value);
    Q_INVOKABLE void clearCellValue(int index);
    Q_INVOKABLE void setCellStatus(int index, const QString& status);
    Q_INVOKABLE void clearSelection();

private:
    void setSelectionStatus( const QModelIndex& index );
    mapping getQuadraticValue( int position) const;
    mapping mapToLinear(mapping index);
    bool checkConflicts();
private:
    u_int16_t m_width;
    u_int16_t m_height;
    grid m_data;
};