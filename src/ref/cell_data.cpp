#include "cell_data.hpp"

bool operator ==(const CellData &lhs, const CellData &rhs) {
    return lhs.value() == rhs.value();
}

bool operator ==(const CellData &lhs, int value) {
    return lhs.value() == value;
}

CellData::CellData( QObject *parent )
    : CellData(-1, parent)
{ }

CellData::CellData( int value, QObject *parent )
    : QObject(parent), m_value(value), m_inConflict(false)
    , m_isEditable(false), m_status("none")
{ }

int CellData::value() const {
    return m_value;
}

void CellData::setValue( int value ) {
    if((value < 1 || value > 9) && value != -1)
        return;
    m_value = value;
    emit valueChanged();
}

QString CellData::status() const {
    return m_status;
}

void CellData::setStatus( const QString &status ) {
    m_status = status;
}

bool CellData::conflict() const {
    return m_inConflict;
}

void CellData::setConflict( bool b ) {
    m_inConflict = b;
    emit conflictChanged();
}

bool CellData::editable() const {
    return m_isEditable;
}

void CellData::setEditable( bool b ) {
    m_isEditable = b;
    emit editableChanged();
}