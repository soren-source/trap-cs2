#include "CInterfaces.hpp"
#include "Engine.hpp"

auto CInterfaces::CacheEngineInterfaces( ) -> bool
{

    auto clientDll = g_Engine->GetInterfaceRegisterList( "client.dll" );
    auto engine = g_Engine.get( );
    std::for_each( this->m_ClientInterfaceNames.begin( ), this->m_ClientInterfaceNames.end( ), [ this, clientDll, engine ] ( const char* interfaceName ) {
        uintptr_t* interfaceAddress = engine->GetInterface<uintptr_t>( clientDll, interfaceName );
        this->PushInterface( std::make_pair( interfaceName, *interfaceAddress ) );
        printf( "[+] Found %s at %p\n", interfaceName, interfaceAddress );
    } );

    auto engineDll = g_Engine->GetInterfaceRegisterList( "engine2.dll" );
    auto gameResource = g_Engine->GetInterface( engineDll, "GameResourceServiceClientV001" );
    this->PushInterface( std::make_pair( "GameResourceServiceClientV001", gameResource ) );
    printf( "[+] Found %s at %p\n", "GameResourceServiceClientV001", gameResource );

    this->m_pEngineClient = g_Engine->GetInterface<IEngineClient>( engineDll, "Source2EngineToClient001" );

    return true;
}

//template<typename T>
//auto CInterfaces::GetInterface( const char* interfaceName ) -> std::optional<T> 
//{
//
//}