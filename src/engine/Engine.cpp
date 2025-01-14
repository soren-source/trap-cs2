#include <engine/Engine.hpp>
#include <engine/CInterfaces.hpp>
#include <engine/CEntitiyListSystem.hpp>
#include <memory/memory.hpp>
#include "SchemaSystem.hpp"
#include <backend/events/PlayerEvents/OnAddPlayer.hpp>
#include <backend/events/PlayerEvents/OnRemovePlayer.hpp>

Engine::Engine( ) {
	this->InitializeEngineComponents( );
}

auto Engine::InitializeEngineComponents( ) -> void
{
	if ( !g_EngineInterfaces->CacheEngineInterfaces( ) )
		std::cout << "[!] CInterfaces::CacheEngineInterfaces failed.\n";

	//Call make_unique to set g_SchemaSystem and call SchemaSystem constructor.
	g_SchemaSystem = std::make_unique<SchemaSystem>( );

	//Caches all SchemaDatas used to read Schemas offsets later on.
	g_SchemaManager = std::make_unique<SchemaManager>( );
	g_SchemaManager->CacheSchemaData( );

	g_OnAddPlayer = std::make_unique<OnAddPlayer>( );
	g_OnRemovePlayer = std::make_unique<OnRemovePlayer>( );
}
