#include "board_model.hpp"


BoardModel::BoardModel(u_int16_t width, u_int16_t height, QObject *parent )
    : QAbstractListModel(parent), m_width(width), m_height(height)
{
}

int BoardModel::rowCount( const QModelIndex &parent ) const {
    Q_UNUSED(parent)
    return m_width*m_height;
}

QVariant BoardModel::data( const QModelIndex &index, int role ) const {
    if(!index.isValid() || index.model() != this)
        return {};

    auto quad = getQuadraticValue(index.row());
    if( role == ValueRole )
        return m_data[quad.first][quad.second].getValue();
    else if( role == StatusRole )
        return m_data[quad.first][quad.second].getStatus();
    else if( role == ConflictRole )
        return m_data[quad.first][quad.second].getConflict();
    else if( role == EditableRole )
        return m_data[quad.first][quad.second].getEditable();

    return {};
}

/* TODO: other roles? */
bool BoardModel::setData( const QModelIndex &index, const QVariant &value, int role ) {

    if(index.row() < 0 || index.row() > m_width*m_height)
        return false;

    auto quad = getQuadraticValue(index.row());
    if( role == ValueRole )
        m_data[quad.first][quad.second].setValue(value.toInt());
    else if( role == StatusRole ) {
        const auto status = value.toString();
        if( status == "selected" ) {
            setSelectionStatus(index);
        }
    }
    else if( role == ConflictRole )
        return m_data[quad.first][quad.second].getConflict();
    else if( role == EditableRole )
        return m_data[quad.first][quad.second].getEditable();

    emit dataChanged(index, index, QVector<int>() << role );
    return true;
}

QHash<int, QByteArray> BoardModel::roleNames() const {
    static QHash<int, QByteArray> roles;
    roles[ValueRole] = "value";
    roles[StatusRole] = "status";
    roles[ConflictRole] = "conflict";
    roles[EditableRole] = "editable";
    return roles;
}

Qt::ItemFlags BoardModel::flags( const QModelIndex &index ) const {
    Q_UNUSED(index);
    return Qt::ItemIsSelectable|Qt::ItemIsEnabled;
}

std::pair<u_int16_t, u_int16_t> BoardModel::getQuadraticValue( int position ) const {
    int w = (position % m_width);
    int h = (position / m_width);
    return { h, w };
}

void BoardModel::setSelectionStatus( const QModelIndex &index ) {
    auto quad = getQuadraticValue(index.row());
    m_data[quad.first][quad.second].setStatus("selected");

    for( int width = 0; width < m_width; width++ ) {
        m_data[quad.first][width].setStatus("same_row");
    }
    for( int height = 0; height < m_height; height++ ) {
        m_data[height][quad.second].setStatus("same_column");
    }
}

