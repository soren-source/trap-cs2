#pragma

#include <memory/hooking.hpp>
#include <engine/EngineClasses/CEntityInstance.hpp>
#include <engine/EngineClasses/CHandle.hpp>

class OnRemovePlayer {
public:
	OnRemovePlayer( );

	static auto ProcessOnRemovePlayer( void* rcx, CEntityInstance* entity, CHandle handle ) -> void*;

	typedef void* ( __fastcall* tRemoveEntity )( void* rcx, CEntityInstance* entity, CHandle handle );
	Hook< tRemoveEntity >* m_RemovePlayerContext = NULL;
private:

};

inline std::unique_ptr<OnRemovePlayer> g_OnRemovePlayer;