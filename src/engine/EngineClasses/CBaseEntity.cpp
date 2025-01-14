#include "CBaseEntity.hpp"

auto CBaseEntity::IsPlayer( ) -> bool
{
    return Virtual::Get<bool>( this, 144 );
}
