#include "board_model.hpp"

BoardModel::BoardModel( u_int16_t width , u_int16_t height , QObject* parent )
	: QAbstractListModel( parent ) , m_width( width ) , m_height( height ) {
}

int BoardModel::rowCount( const QModelIndex& parent ) const {
	Q_UNUSED( parent )
	return m_width * m_height;
}

QVariant BoardModel::data( const QModelIndex& index , int role ) const {
	if( !index.isValid() || index.model() != this )
		return { };
	
	auto quad = getQuadraticValue( index.row());
	if( role == ValueRole )
		return m_data[ quad.first ][ quad.second ].getValue();
	else if( role == StatusRole )
		return m_data[ quad.first ][ quad.second ].getStatus();
	else if( role == ConflictRole )
		return m_data[ quad.first ][ quad.second ].getConflict();
	else if( role == EditableRole )
		return m_data[ quad.first ][ quad.second ].getEditable();
	
	return { };
}

/* TODO: other roles? */
bool BoardModel::setData( const QModelIndex& index , const QVariant& value , int role ) {
	
	if( index.row() < 0 || index.row() > m_width * m_height )
		return false;
	QVector< int > roles;
	QModelIndex left , right = index;
	auto quad = getQuadraticValue( index.row());
	/* setting numeric value */
	if( role == ValueRole ) {
		if( !m_data[ quad.first ][ quad.second ].getEditable())
			return false;
		m_data[ quad.first ][ quad.second ].setValue( value.toInt());
		roles << ValueRole;
		
		if( checkConflicts()) {
			roles << ConflictRole;
			left = createIndex( 0 , 0 );
			right = createIndex( m_width * m_height , 0 );
		}
	}
		/* setting status value */
	else if( role == StatusRole ) {
		const auto status = value.toString();
		if( status == "none" )
			return false;
		if( status == "selected" ) {
			setSelectionStatus( index );
			left = createIndex( 0 , 0 );
			right = createIndex( m_width * m_height , 0 );
		}
		roles << StatusRole;
	}
	emit dataChanged( left , right , roles );
	return true;
}

QHash< int , QByteArray > BoardModel::roleNames() const {
	static QHash< int , QByteArray > roles;
	roles[ ValueRole ] = "value";
	roles[ StatusRole ] = "status";
	roles[ ConflictRole ] = "conflict";
	roles[ EditableRole ] = "editable";
	return roles;
}

Qt::ItemFlags BoardModel::flags( const QModelIndex& index ) const {
	Q_UNUSED( index );
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

std::pair< u_int16_t , u_int16_t > BoardModel::getQuadraticValue( int position ) const {
	int w = ( position % m_width );
	int h = ( position / m_width );
	return { h , w };
}

void BoardModel::setSelectionStatus( const QModelIndex& index ) {
	auto quad = getQuadraticValue( index.row());
	
	for( int height = 0; height < m_height; height++ ) {
		for( int width = 0; width < m_width; width++ ) {
			if( width == quad.second ) {
				m_data[ height ][ width ].setStatus( "same_column" );
				continue;
			}
			if( height == quad.first ) {
				m_data[ height ][ width ].setStatus( "same_row" );
				continue;
			}
			m_data[ height ][ width ].setStatus( "none" );
		}
	}
	
	m_data[ quad.first ][ quad.second ].setStatus( "selected" );
}

void BoardModel::setCellValue( int index , int value ) {
	setData( createIndex( index , 0 ) , value , ValueRole );
}

void BoardModel::clearCellValue( int index ) {
	auto i = data( createIndex( index , 0 ) , ValueRole );
	setCellValue( index , -1 );
}

void BoardModel::clearSelection() {
	for( int i = 0; i < m_width * m_height; i++ ) {
		setCellStatus( i , "none" );
	}
}

void BoardModel::setCellStatus( int index , const QString& status ) {
	setData( createIndex( index , 0 ) , status , StatusRole );
}

bool BoardModel::checkConflicts() {
	bool isChanged = false;
	
	for( int targetRow = 0; targetRow < m_height; ++targetRow ) {
		for( int targetCol = 0; targetCol < m_width; ++targetCol ) {
			if( m_data[ targetRow ][ targetCol ].getConflict()) {
				m_data[ targetRow ][ targetCol ].setConflict( false );
				isChanged = true;
			}
			
			if( m_data[ targetRow ][ targetCol ].getValue() == EMPTY_CELL )
				continue;
			
			int value = m_data[ targetRow ][ targetCol ].getValue();
			
			// Check same row conflict
			for( int col = 0; col < m_width; ++col ) {
				if( col != targetCol && m_data[ targetRow ][ col ].getValue() == value ) {
					m_data[ targetRow ][ targetCol ].setConflict( true );
					isChanged = true;
				}
			}
			
			// Check same column conflict
			for( int row = 0; row < m_height; ++row ) {
				if( row != targetRow && m_data[ row ][ targetCol ].getValue() == value ) {
					m_data[ targetRow ][ targetCol ].setConflict( true );
					isChanged = true;
				}
			}
			
			// Check same square conflict
			auto quad = mapToLinear( { targetRow , targetCol } );
			for( int col = 0; col < 9; ++col ) {
				auto test = mapToLinear( { quad.first , col } );
				if( col != quad.second && m_data[ test.first ][ test.second ].getValue() == value ) {
					m_data[ targetRow ][ targetCol ].setConflict( true );
					isChanged = true;
				}
			}
		}
	}
	
	return isChanged;
}

BoardModel::mapping BoardModel::mapToLinear( BoardModel::mapping index ) {
	const int row = qFloor( index.first / 3 ) * 3 + qFloor( index.second / 3 );
	const int column = ( index.first % 3 ) * 3 + ( index.second % 3 );
	
	return { row , column };
}