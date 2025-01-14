#pragma once
#include "EngineClassBase.hpp"

class CBaseEntity : public EngineClassBase {
public:
	SCHEMA_FIELD( m_iHealth, "C_BaseEntity", "m_iHealth", int32_t, this );
	SCHEMA_FIELD( m_sSanitizedPlayerName, "CCSPlayerController", "m_sSanitizedPlayerName", const char*, this );

	auto IsPlayer( ) -> bool;

	const char* _GetClassName( ) override { return "C_BaseEntity"; }
};