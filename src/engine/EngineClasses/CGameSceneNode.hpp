#pragma once

#include "EngineClassBase.hpp"
#include <classes/Vector.hpp>

class CGameSceneNode {
public:
    SCHEMA_FIELD( m_angAbsRotation, "CGameSceneNode", "m_angAbsRotation", Vector3, this );
    SCHEMA_FIELD( m_angRotation, "CGameSceneNode", "m_angRotation", Vector3, this );
    SCHEMA_FIELD( m_bDormant, "CGameSceneNode", "m_bDormant", bool, this );
    SCHEMA_FIELD( m_vecAbsOrigin, "CGameSceneNode", "m_vecAbsOrigin", Vector3, this );
    SCHEMA_FIELD( m_vecOrigin, "CGameSceneNode", "m_vecOrigin", Vector3, this );
};