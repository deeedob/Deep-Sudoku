#include "enhanced_engine.hpp"
#include <iostream>

using namespace dsdk;

EnhancedEngine::EnhancedEngine(
	QObject* parent )
	: QQmlApplicationEngine( parent )
{ }

void EnhancedEngine::clearCache()
{
	this->trimComponentCache();
	this->clearComponentCache();
	this->trimComponentCache();
}