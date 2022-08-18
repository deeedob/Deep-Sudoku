#include "app.hpp"
#include <filesystem>

App::App( QObject* parent )
	: QObject( parent ), m_boardModel( new BoardModel( 9, 9 ))
{
	std::filesystem::path ai_path( std::filesystem::current_path().parent_path().parent_path() += "/ai_model/fdeep_model.json" );
	m_deepSolver = std::make_unique<DeepSolver>( ai_path.string());
}

App& App::getInstance()
{
	static auto* instance = new App( nullptr );
	return *instance;
}

BoardModel* App::getModel() const
{
	return m_boardModel;
}

void App::solveGame( const QImage& src )
{
	if( src.isNull() )
		return;
		
	auto result = m_deepSolver->predictMulti( src );
	for( const auto& i : result ) {
	}
	
}