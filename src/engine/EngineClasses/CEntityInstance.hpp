#pragma once

#include "EngineClassBase.hpp"
#include "CEntityIdentity.hpp"
#include "CHandle.hpp"

class CEntityInstance : public EngineClassBase {
public:


    auto GetRefEHandle( ) {
        CEntityIdentity* pIdentity = (CEntityIdentity*)m_pEntity( );
        auto v3 = *( uint32_t* )( pIdentity + 16 );
        auto v4 = ENT_ENTRY_MASK;
        auto v5 = ( ( v3 >> 15 ) - ( *( uint32_t* )( pIdentity + 48 ) & 1 ) ) << 15;
        if ( v3 != -1 ) {
            v4 = *( uint32_t* )( pIdentity + 16 ) & ENT_ENTRY_MASK;
        }

        return CHandle( v4 | v5 );
    }

	SCHEMA_FIELD( m_pEntity, "CEntityInstance", "m_pEntity", CEntityIdentity*, this );

	const char* _GetClassName( ) override { return "C_BaseEntity"; }
};