#include "cell_data.hpp"

using namespace dsdk;

CellData::CellData( QObject* parent )
	: CellData( EMPTY_CELL, parent )
{ }

CellData::CellData( int value, QObject* parent )
	: QObject( parent ), m_value( value ), m_editable( true ), m_conflict( false ), m_solved( -1 )
{ }

int CellData::getValue() const
{
	return m_value;
}

void CellData::setValue( int mValue )
{
	if( mValue < -1 || mValue > 9 )
		return;
	m_value = mValue;
	emit valueChanged();
}

bool CellData::getEditable() const
{
	return m_editable;
}

void CellData::setEditable( bool mEditable )
{
	m_editable = mEditable;
	emit editableChanged();
}

bool CellData::getConflict() const
{
	return m_conflict;
}

void CellData::setConflict( bool mConflict )
{
	m_conflict = mConflict;
	emit conflictChanged();
}

const QString& CellData::getStatus() const
{
	return m_status;
}

void CellData::setStatus( const QString& mStatus )
{
	m_status = mStatus;
	emit statusChanged();
}

int CellData::getSolved() const
{
	return m_solved;
}

void CellData::setSolved( int solved )
{
	m_solved = solved;
	emit solvedChanged();
}