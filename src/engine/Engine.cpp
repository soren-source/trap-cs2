#include <engine/Engine.hpp>
#include <engine/CInterfaces.hpp>
#include <engine/CEntitiyListSystem.hpp>
#include <memory/memory.hpp>
#include "SchemaSystem.hpp"

Engine::Engine( ) {
	this->InitializeEngineComponents( );
}

auto Engine::InitializeEngineComponents( ) -> void
{
	if ( !g_EngineInterfaces->CacheEngineInterfaces( ) )
		std::cout << "[!] CInterfaces::CacheEngineInterfaces failed.\n";

	g_SchemaSystem = std::make_unique<SchemaSystem>( );
	g_SchemaManager->CacheSchemaData( );
}
