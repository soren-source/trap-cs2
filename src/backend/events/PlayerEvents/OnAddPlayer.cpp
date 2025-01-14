#include "OnAddPlayer.hpp"
#include <engine/CInterfaces.hpp>
#include <backend/caching/EntityCaching.hpp>
#include <engine/EngineClasses/CBaseEntity.hpp>

OnAddPlayer::OnAddPlayer( )
{
	auto gameResourceServiceOpt = g_EngineInterfaces->GetInterface<void*>( "GameResourceServiceClientV001" );
	if ( !gameResourceServiceOpt || !gameResourceServiceOpt.has_value( ) ) return;

	auto gameResourceService = gameResourceServiceOpt.value( );
	if ( !gameResourceService ) return;

	auto instance = *reinterpret_cast< void** >( ( uintptr_t )gameResourceService + 0x58 );
	if ( !instance ) return;

	uint64_t* vtable = *( uint64_t** )( instance );

	this->m_AddPlayerContext = new Hook<tAddEntity>( (uintptr_t)vtable[ 15 ] );
	if ( !this->m_AddPlayerContext->EnableHook( &ProcessOnAddPlayer ) )
		printf("OnAddPlayer Hook failed!");
}

auto ReadPlayerInformations( CEntityInstance* entity, CHandle handle ) -> CachedPlayer 
{
	CBaseEntity* baseEntity = reinterpret_cast< CBaseEntity* >( entity );
	//if ( baseEntity->IsPlayer( ) ) printf( "IS PLAYER %s\n", baseEntity->m_sSanitizedPlayerName( ) );

	return CachedPlayer();
}

auto OnAddPlayer::ProcessOnAddPlayer( void* rcx, CEntityInstance* entity, CHandle handle ) -> void*
{
	auto playerCache = g_EntityCaching->GetPlayerCache( );

	auto it = std::find_if( playerCache.begin( ), playerCache.end( ), [ entity ] ( const CachedPlayer& entry ) {
		return entry.m_pEntity == entity;
		} );

	if ( it == playerCache.end( ) ) {
		auto player = ReadPlayerInformations( entity, handle );
		playerCache.push_back( player );
		g_EntityCaching->SetPlayerCache( playerCache );
	}

	return g_OnAddPlayer->m_AddPlayerContext->GetOriginal()(rcx, entity, handle);
}
