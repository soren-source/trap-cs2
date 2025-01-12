#include "startup.hpp"
#include <engine/engine.hpp>
#include <engine/CEntitiyListSystem.hpp>
#include <engine/EngineClasses/CBaseEntity.hpp>
#include <engine/EngineClasses/CEntityInstance.hpp>

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

	g_Engine = std::make_unique<Engine>( );

	while ( !GetAsyncKeyState( VK_END ) & 1 ) {
		std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
	}
}
