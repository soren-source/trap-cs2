#pragma

#include <memory/hooking.hpp>
#include <engine/EngineClasses/CEntityInstance.hpp>
#include <engine/EngineClasses/CHandle.hpp>

class OnAddPlayer {
public:
	OnAddPlayer( );

	static auto ProcessOnAddPlayer( void* rcx, CEntityInstance* entity, CHandle handle ) -> void*;

	typedef void* ( __fastcall* tAddEntity )( void* rcx, CEntityInstance* entity, CHandle handle );
	Hook< tAddEntity >* m_AddPlayerContext = NULL;
private:

};

inline std::unique_ptr<OnAddPlayer> g_OnAddPlayer;