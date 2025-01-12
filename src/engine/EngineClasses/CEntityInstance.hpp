#pragma once

#include "EngineClassBase.hpp"

class CEntityInstance : public EngineClassBase {
public:

	SCHEMA_FIELD( m_pEntity, this->_GetClassName( ), "m_pEntity", CEntityInstance*, this );

	const char* _GetClassName( ) override { return "C_BaseEntity"; }
};