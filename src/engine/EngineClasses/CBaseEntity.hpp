#pragma once
#include "EngineClassBase.hpp"

class CBaseEntity : public EngineClassBase {
public:
	SCHEMA_FIELD( m_iHealth, "C_BaseEntity", "m_iHealth", int32_t, this );

	const char* _GetClassName( ) override { return "C_BaseEntity"; }
};