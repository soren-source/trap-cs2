#pragma once
#include <includes.hpp>

class CEntityListSystem {
public:
	auto GetEntityByIndex( int index ) -> void*;
	
	auto GetInstance( ) -> CEntityListSystem*;

	int GetHighestEntityIndex( )
	{
		return *reinterpret_cast< int* >( reinterpret_cast< std::uintptr_t >( this ) + 0x20F0 );
	}

private:
	typedef void* ( __thiscall* tGetEntityByIndex )( void* instance, int index );
	tGetEntityByIndex m_GetEntityByIndex;
	bool m_Initialized = false;
};

inline std::unique_ptr<CEntityListSystem> g_EntityListSystem = std::make_unique<CEntityListSystem>( );;