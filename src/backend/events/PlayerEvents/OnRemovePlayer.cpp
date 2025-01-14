#include "OnRemovePlayer.hpp"
#include <engine/CInterfaces.hpp>
#include <backend/caching/EntityCaching.hpp>

OnRemovePlayer::OnRemovePlayer( )
{
	auto gameResourceServiceOpt = g_EngineInterfaces->GetInterface<void*>( "GameResourceServiceClientV001" );
	if ( !gameResourceServiceOpt || !gameResourceServiceOpt.has_value( ) ) return;

	auto gameResourceService = gameResourceServiceOpt.value( );
	if ( !gameResourceService ) return;

	auto instance = *reinterpret_cast< void** >( ( uintptr_t )gameResourceService + 0x58 );
	if ( !instance ) return;

	uint64_t* vtable = *( uint64_t** )( instance );

	this->m_RemovePlayerContext = new Hook<tRemoveEntity>( (uintptr_t)vtable[ 16 ] );
	if ( !this->m_RemovePlayerContext->EnableHook( &ProcessOnRemovePlayer ) )
		printf("OnAddPlayer Hook failed!");
}

auto OnRemovePlayer::ProcessOnRemovePlayer( void* rcx, CEntityInstance* entity, CHandle handle ) -> void*
{
	auto playerCache = g_EntityCaching->GetPlayerCache( );

	auto it = std::find_if( playerCache.begin( ), playerCache.end( ), [ entity ] ( const CachedPlayer& entry ) {
		return entry.m_pEntity == entity;
		} );

	if ( it != playerCache.end( ) ) {
		playerCache.erase( it );
		g_EntityCaching->SetPlayerCache( playerCache );
	}

	return g_OnRemovePlayer->m_RemovePlayerContext->GetOriginal()( rcx, entity, handle);
}
