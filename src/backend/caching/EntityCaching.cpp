#include "EntityCaching.hpp"

#include <engine/CEntitiyListSystem.hpp>
#include <engine/EngineClasses/CBaseEntity.hpp>
#include <engine/EngineClasses/CGameSceneNode.hpp>
#include <engine/EngineClasses/CSkeletonInstance.hpp>
#include <engine/CInterfaces.hpp>

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

	if ( handle.GetEntryIndex( ) == g_EngineInterfaces->m_pEngineClient->GetLocalPlayer( ) ) return;

	const auto& baseEntity = reinterpret_cast< CBaseEntity* >( entity );

	CachedPlayer cachedPlayer;
	cachedPlayer.m_pEntity = baseEntity;
	cachedPlayer.m_Health = baseEntity->m_iHealth( );
	cachedPlayer.m_WorldPosition = baseEntity->GetOriginPosition( );

	this->AddToPlayerCache( cachedPlayer );

	//cachedPlayer.m_WorldPosition = baseEntity->
}

auto EntityCaching::CacheAllEntitiesOnLoad( ) -> void
{
	for ( size_t i = 0; i < g_EntityListSystem->GetInstance( )->GetHighestEntityIndex( ); i++ ) {
		const auto& entity = reinterpret_cast<CBaseEntity*>( g_EntityListSystem->GetEntityByIndex( i ) );
		if ( !entity ) continue;
		if ( !entity->IsPlayer( ) ) continue;
		auto handle = entity->GetRefEHandle( );
		if ( handle.m_Index == 0 || !handle.IsValid( ) ) continue;

		this->ProcessEntityInformationsAndAddToCache( entity, handle );
	}
}

auto EntityCaching::RefreshCachedEntityInformations( ) -> void
{
	std::vector<CachedPlayer> tempVec;

	for ( auto& entry : this->GetPlayerCache( ) )
	{
		if ( !entry.m_pEntity ) continue;

		const auto& entity = entry.m_pEntity;
		entry.m_Health = entity->m_iHealth( );
		entry.m_WorldPosition = entity->GetOriginPosition( );
		entry.m_IsOnScreen = entry.m_WorldPosition.ToScreen( entry.m_ScreenPosition );
		entry.m_HandleIndex = entity->GetRefEHandle( ).GetEntryIndex( );

		auto gameScene = entry.m_pEntity->m_pGameSceneNode( );
		if ( !gameScene ) continue;

		auto skelInstance = gameScene->GetSkeletonInstance( );
		if ( !skelInstance ) continue;

		auto modelState = skelInstance->m_modelState( );
		if ( !modelState.bones ) continue;

		for ( const auto& bone : g_SkeletonBones )
		{
			entry.m_BoneWorldPositions[ bone ] = modelState.bones[ bone ].position;
			if ( !entry.m_BoneWorldPositions[ bone ].ToScreen( entry.m_BoneScreenPositions[ bone ] ) )
				continue;
		}

		if ( entry.m_BoneScreenPositions.size( ) != g_SkeletonBones.size( ) ) continue;

		float longest_distance_height = 0.0f;
		float longest_distance_width = 0.0f;
		std::pair<int, int> best_height_pair;
		std::pair<int, int> best_width_pair;

		for ( auto i : g_SkeletonBones )
		{
			float distance_height = 0.0f;
			float distance_width = 0.0f;

			int best_counterpiece_height = -1;
			int best_counterpiece_width = -1;

			for ( auto j : g_SkeletonBones )
			{
				if ( i == j )
					continue;

				if ( entry.m_BoneScreenPositions[ i ].x == 0 || entry.m_BoneScreenPositions[ j ].x == 0 )
					continue;

				float width_distance = ( entry.m_BoneScreenPositions[ i ].x - entry.m_BoneScreenPositions[ j ].x );
				float height_distance = ( entry.m_BoneScreenPositions[ i ].y - entry.m_BoneScreenPositions[ j ].y );

				if ( entry.m_BoneScreenPositions[ i ].x < entry.m_BoneScreenPositions[ j ].x )
				{
					float width_distance = ( entry.m_BoneScreenPositions[ j ].x - entry.m_BoneScreenPositions[ i ].x );
					float height_distance = ( entry.m_BoneScreenPositions[ j ].y - entry.m_BoneScreenPositions[ i ].y );
				}

				if ( distance_height < height_distance )
				{
					distance_height = height_distance;
					best_counterpiece_height = j;
				}

				if ( distance_width < width_distance )
				{
					distance_width = width_distance;
					best_counterpiece_width = j;
				}
			}

			if ( distance_height > longest_distance_height )
			{
				longest_distance_height = distance_height;
				best_height_pair = { static_cast< int >( i ), best_counterpiece_height };
			}

			if ( distance_width > longest_distance_width )
			{
				longest_distance_width = distance_width;
				best_width_pair = { static_cast< int >( i ), best_counterpiece_width };
			}
		}

		entry.m_BoxPairHeight = best_height_pair;
		entry.m_BoxPairWidth = best_width_pair;

		BoundingBox box;
		
		box.m_TopLeft = { entry.m_BoneScreenPositions[ entry.m_BoxPairWidth.second ].x - 3.f, entry.m_BoneScreenPositions[ entry.m_BoxPairHeight.second ].y - 3.f };
		box.m_BotRight = { entry.m_BoneScreenPositions[ entry.m_BoxPairWidth.first ].x + 3.f, entry.m_BoneScreenPositions[ entry.m_BoxPairHeight.first ].y + 3.f };

		auto box_left = box.m_TopLeft.x;
		auto box_right = box.m_BotRight.x;

		auto ped_head = ImVec2 { 0, box.m_TopLeft.y };
		auto ped_foot = ImVec2 { 0, box.m_BotRight.y };

		auto box_width = ( box_right - box_left );
		auto box_height = ( box.m_BotRight.y - box.m_TopLeft.y );

		auto box_middle = box_left + ( ( box_right - box_left ) / 2 );

		box.m_TopRight = { entry.m_BoneScreenPositions[ entry.m_BoxPairWidth.first ].x + 3.f, entry.m_BoneScreenPositions[ entry.m_BoxPairHeight.second ].y - 3.f };
		box.m_TopMid = { box_middle, entry.m_BoneScreenPositions[ entry.m_BoxPairHeight.second ].y - 3.f };
		box.m_BotMid = { box_middle, entry.m_BoneScreenPositions[ entry.m_BoxPairHeight.first ].y + 3.f };
		box.m_BotLeft = { entry.m_BoneScreenPositions[ entry.m_BoxPairWidth.second ].x, entry.m_BoneScreenPositions[ entry.m_BoxPairHeight.first ].y + 3.f };

		entry.m_BoundingBox = box;

		//entry.m_Armour = entity->armo

		tempVec.push_back( entry );
	}

	this->SetPlayerCache( tempVec );
}
