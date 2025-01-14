#include "CEntitiyListSystem.hpp"
#include <memory/memory.hpp>

auto CEntityListSystem::GetEntityByIndex( int index ) -> void*
{
	if ( !this->m_Initialized ) {
		this->m_GetEntityByIndex = reinterpret_cast< tGetEntityByIndex >( g_Memory.FindPattern( "client.dll", "81 FA ? ? ? ? 77 ? 8B C2 C1 F8 ? 83 F8 ? 77 ? 48 98 48 8B 4C C1 ? 48 85 C9 74 ? 8B C2 25 ? ? ? ? 48 6B C0 ? 48 03 C8 74 ? 8B 41 ? 25 ? ? ? ? 3B C2 75 ? 48 8B 01" ) );
		this->m_Initialized = true;
	}
	//return nullptr;
	return this->m_GetEntityByIndex( this->GetInstance( ), index );
}

auto CEntityListSystem::GetInstance( ) -> CEntityListSystem*
{
	static CEntityListSystem* instance;
	if ( !instance )
		instance = *reinterpret_cast< CEntityListSystem** >( g_Memory.FixMov( g_Memory.FindPattern( "client.dll", "48 8B 0D ? ? ? ? 48 8D 94 24 ? ? ? ? 33 DB" ) ) );

	return instance;
}
