#include "EntityCaching.hpp"

#include <engine/CEntitiyListSystem.hpp>
#include <engine/EngineClasses/CBaseEntity.hpp>

EntityCaching::EntityCaching( )
{
}

auto EntityCaching::ProcessEntityInformationsAndAddToCache( CEntityInstance* entity, CHandle& handle ) -> void
{
	if ( entity == nullptr ) return;

	// Credits @https://github.com/alza54/opensource2
	// Cache only networked entities "In  Source 2, specifically  S&box, the networked entity limit is 16384 edicts.".
	// https://developer.valvesoftware.com/wiki/Entity_limit#Source_2_limits
	if ( handle.GetEntryIndex( ) >= 16384 ) return;

	const auto& baseEntity = reinterpret_cast< CBaseEntity* >( entity );

	CachedPlayer cachedPlayer;
	cachedPlayer.m_pEntity = baseEntity;
	cachedPlayer.m_Health = baseEntity->m_iHealth( );
	cachedPlayer.m_WorldPosition = baseEntity->GetOriginPosition( );

	this->AddToPlayerCache( cachedPlayer );

	//cachedPlayer.m_WorldPosition = baseEntity->
}
