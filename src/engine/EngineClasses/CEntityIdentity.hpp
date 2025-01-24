#pragma once
#include "EngineClassBase.hpp"

class CEntityIdentity {
public:
    SCHEMA_FIELD( m_designerName, "CEntityIdentity", "m_designerName",
        const char*, this );
    SCHEMA_FIELD( m_flags, "CEntityIdentity", "m_flags", uint32_t, this );

    auto GetSchemaName( ) -> const char* {
        const uintptr_t entityClassInfo = *( uintptr_t* )( this + 0x8 );
        if ( !entityClassInfo ) return "";
        const uintptr_t schemaClassInfoData = *( uintptr_t* )( entityClassInfo + 0x30 );
        if ( !schemaClassInfoData ) return "";
        const uintptr_t className = *( uintptr_t* )( schemaClassInfoData + 0x8 );
        if ( !className ) return "";

        return ( const char* )( className );
    }
};