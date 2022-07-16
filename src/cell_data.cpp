#include "cell_data.hpp"

CellData::CellData( QObject *parent )
    : CellData(EMPTY_CELL, parent)
{ }

CellData::CellData( int value, QObject *parent )
    : QObject(parent), m_value(value), m_editable(true), m_conflict(false)
{ }

int CellData::getValue() const {
    return m_value;
}

void CellData::setValue( int mValue ) {
    if( mValue < 0 || mValue > 9)
        return;
    m_value = mValue;
    emit valueChanged();
}

bool CellData::getEditable() const {
    return m_editable;
}

void CellData::setEditable( bool mEditable ) {
    m_editable = mEditable;
    emit editableChanged();
}

bool CellData::getConflict() const {
    return m_conflict;
}

void CellData::setConflict( bool mConflict ) {
    m_conflict = mConflict;
    emit conflictChanged();
}

const QString &CellData::getStatus() const {
    return m_status;
}

void CellData::setStatus( const QString &mStatus ) {
    m_status = mStatus;
    emit statusChanged();
}

