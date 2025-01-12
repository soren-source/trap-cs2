#pragma once
#include <includes.hpp>

class CEntityListSystem {
public:
	auto GetEntityByIndex( int index ) -> void*;
	
	auto GetInstance( ) -> CEntityListSystem*;

private:
	typedef void* ( __thiscall* tGetEntityByIndex )( void* instance, int index );
	tGetEntityByIndex m_GetEntityByIndex;
	bool m_Initialized = false;
};

inline std::unique_ptr<CEntityListSystem> g_EntityListSystem = std::make_unique<CEntityListSystem>( );;