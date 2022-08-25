#pragma once
#include <QObject>

namespace dsdk
{
	#define EMPTY_CELL -1
	
	/*!
	 * @ingroup QT
	 * @brief A single cell used by the BoardModel
	 * @author Dennis Oberst
	 */
	class CellData : public QObject
	{
	Q_OBJECT
		Q_PROPERTY( int value READ getValue WRITE setValue NOTIFY valueChanged )
		Q_PROPERTY( int solved READ getValue WRITE setValue NOTIFY valueChanged )
		Q_PROPERTY( bool editable READ getEditable WRITE setEditable NOTIFY editableChanged )
		Q_PROPERTY( bool conflict READ getConflict WRITE setConflict NOTIFY conflictChanged )
		Q_PROPERTY( QString status READ getStatus WRITE setStatus NOTIFY statusChanged )
	
	public:
		CellData( QObject* parent = nullptr );
		CellData( int value, QObject* parent = nullptr );
		
		int getValue() const;
		void setValue( int mValue );
		bool getEditable() const;
		void setEditable( bool mEditable );
		bool getConflict() const;
		void setConflict( bool mConflict );
		const QString& getStatus() const;
		void setStatus( const QString& mStatus );
		int getSolved() const;
		void setSolved( int solved );
	
	signals:
		void valueChanged();
		void editableChanged();
		void conflictChanged();
		void statusChanged();
		void hiddenChanged();
		void solvedChanged();
	
	private:
		int m_value;
		int m_solved;
		bool m_editable;
		bool m_conflict;
		QString m_status;
	};
}