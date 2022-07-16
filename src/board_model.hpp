#pragma once
#include <QAbstractListModel>
#include "cell_data.hpp"

class BoardModel : public QAbstractListModel
{
    using grid = std::array<std::array<CellData,9>,9>;
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

private:
    void setSelectionStatus( const QModelIndex& index );
    std::pair<u_int16_t, u_int16_t> getQuadraticValue( int position) const;


private:
    u_int16_t m_width;
    u_int16_t m_height;
    grid m_data;
};