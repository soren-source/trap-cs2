#include "PlayerVisuals.hpp"

#include <backend/caching/EntityCaching.hpp>

void PlayerVisuals::OnTick( )
{
	for ( const auto& entry : g_EntityCaching->GetPlayerCache( ) ) {
		if ( !entry.m_IsOnScreen || !entry.m_pEntity || entry.m_Health <= 0 ) continue;

		this->DrawBox( entry.m_BoundingBox );
		this->DrawSkeleton( entry );
		this->DrawBar( entry );
	}
}

std::string PlayerVisuals::GetName( )
{
	return std::string( "Player Visuals" );
}

bool PlayerVisuals::IsActive( )
{
	return true;
}

int PlayerVisuals::GetHotkey( )
{
	return 0;
}

void PlayerVisuals::DrawBox( BoundingBox box )
{
	ImGui::GetBackgroundDrawList( )->AddRect( { box.m_TopLeft.x - 1,  box.m_TopLeft.y - 1 }, { box.m_BotRight.x + 1, box.m_BotRight.y + 1 }, ImColor( 0, 0, 0 ) );
	ImGui::GetBackgroundDrawList( )->AddRect( { box.m_TopLeft.x + 1,  box.m_TopLeft.y + 1 }, { box.m_BotRight.x - 1, box.m_BotRight.y - 1 }, ImColor( 0, 0, 0 ) );
	ImGui::GetBackgroundDrawList( )->AddRect( box.m_TopLeft, box.m_BotRight, ImColor( 255, 255, 255 ) );
}

void PlayerVisuals::DrawSkeleton( CachedPlayer player )
{
	for ( const auto& bonePair : player.m_BonePairs ) {
		ImGui::GetBackgroundDrawList( )->AddLine( player.m_BoneScreenPositions[ bonePair.first ], player.m_BoneScreenPositions[ bonePair.second ], ImColor( 255, 255, 255 ) );
	}
}

void PlayerVisuals::DrawBar( CachedPlayer player )
{
	auto barBegin = player.m_BoundingBox.m_TopLeft; barBegin.x -= 5; barBegin.y -= 1;
	auto barEnd = player.m_BoundingBox.m_BotLeft; barEnd.x -= 5; barEnd.y += 1;

	ImGui::GetBackgroundDrawList( )->AddRectFilled( barBegin, barEnd, ImColor( 0, 0, 0, 120 ) );

	auto percentage = barEnd.y - ( ( barEnd.y - barBegin.y ) * ( player.m_Health * 0.01f ) );

	ImGui::GetBackgroundDrawList( )->AddRectFilled( { barBegin.x + 1, percentage + 1 }, { barEnd.x - 1, barEnd.y - 1 }, ImColor( 43, 255, 43 ) );
}
