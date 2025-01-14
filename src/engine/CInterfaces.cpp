#include "CInterfaces.hpp"

auto CInterfaces::CacheEngineInterfaces( ) -> bool
{
    auto createInterface = reinterpret_cast< tCreateInterface >( GetProcAddress( GetModuleHandleA( "client.dll" ), "CreateInterface" ) );

    std::for_each( this->m_ClientInterfaceNames.begin( ), this->m_ClientInterfaceNames.end( ), [ this, createInterface ] ( const char* interfaceName ) {
        int returnCode;
        uintptr_t interfaceAddress = reinterpret_cast< uintptr_t >( createInterface( interfaceName, &returnCode ) );
        this->PushInterface( std::make_pair( interfaceName, interfaceAddress ) );
        printf( "Found %s at %p\n", interfaceName, interfaceAddress );
    } );

    createInterface = reinterpret_cast< tCreateInterface >( GetProcAddress( GetModuleHandleA( "engine2.dll" ), "CreateInterface" ) );
    int returnCode;
    uintptr_t interfaceAddress = reinterpret_cast< uintptr_t >( createInterface( "GameResourceServiceClientV001", &returnCode ) );
    this->PushInterface( std::make_pair( "GameResourceServiceClientV001", interfaceAddress ) );
    printf( "Found %s at %p\n", "GameResourceServiceClientV001", interfaceAddress );

    return true;
}

//template<typename T>
//auto CInterfaces::GetInterface( const char* interfaceName ) -> std::optional<T> 
//{
//
//}