#include "CBaseEntity.hpp"
#include "CGameSceneNode.hpp"

auto CBaseEntity::IsPlayer( ) -> bool
{
    const auto& identity = this->m_pEntity( );
    if ( !identity ) return false;

    const auto& schemaName = identity->GetSchemaName( );
    if ( schemaName == "" ) return false;

    return ( !strcmp( schemaName, "C_CSPlayerPawnBase" ) );
}

auto CBaseEntity::GetOriginPosition( ) -> Vector3
{
    auto returnPosition = Vector3( );

    CGameSceneNode* pGameSceneNode = m_pGameSceneNode( );
    if ( !pGameSceneNode ) return returnPosition;

    return pGameSceneNode->m_vecAbsOrigin( );
}
