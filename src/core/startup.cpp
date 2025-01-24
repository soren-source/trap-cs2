#include "startup.hpp"
#include <engine/engine.hpp>
#include <engine/CEntitiyListSystem.hpp>
#include <engine/EngineClasses/CBaseEntity.hpp>
#include <engine/EngineClasses/CEntityInstance.hpp>
#include <hooking/PresentHook.hpp>
#include <minhook/MinHook.h>

Startup::Startup( ) {
	this->AllocateConsole( );
	this->IntializeCheatStart( );
}

auto Startup::AllocateConsole( ) -> void {
	if ( !this->m_IsDebug ) return;

	if ( AllocConsole( ) )
		freopen_s( ( FILE** )stdout, "CONOUT$", "w", stdout );

}

auto Startup::IntializeCheatStart( ) -> void
{
	std::cout << "[+] Initializing Trap\n";

	MH_Initialize( );

	g_Engine = std::make_unique<Engine>( );
	g_PresentHook = std::make_unique<PresentHook>( );

	while ( !GetAsyncKeyState( VK_END ) & 1 ) {
		std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
	}
	std::cout << "[-] Unloading Trap...\n";
	MH_DisableHook( MH_ALL_HOOKS );
	std::cout << "[-] Disabled all Hooks...\n";
	MH_RemoveHook( MH_ALL_HOOKS );
	g_PresentHook->Uninitialize( );
	std::cout << "[-] Removed all Hooks...\n";
}
