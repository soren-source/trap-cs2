#pragma once
#include "EngineClassBase.hpp"
#include "CEntityInstance.hpp"
#include <classes/Vector.hpp>

class CGameSceneNode;

class CBaseEntity : public CEntityInstance {
public:
	SCHEMA_FIELD( m_iHealth, "C_BaseEntity", "m_iHealth", int32_t, this );
	SCHEMA_FIELD( m_iTeamNum, "C_BaseEntity", "m_iTeamNum", uint8_t, this );
	SCHEMA_FIELD( m_pGameSceneNode, "C_BaseEntity", "m_pGameSceneNode",	CGameSceneNode*, this );
	SCHEMA_FIELD( m_sSanitizedPlayerName, "CCSPlayerController", "m_sSanitizedPlayerName", const char*, this );

	auto IsPlayer( ) -> bool;
	auto GetOriginPosition( ) -> Vector3;

	const char* _GetClassName( ) override { return "C_BaseEntity"; }
};