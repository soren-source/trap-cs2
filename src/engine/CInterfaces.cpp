#include "CInterfaces.hpp"

auto CInterfaces::CacheEngineInterfaces( ) -> bool
{
    auto createInterface = reinterpret_cast< tCreateInterface >( GetProcAddress( GetModuleHandleA( "client.dll" ), "CreateInterface" ) );

    std::for_each( this->m_InterfaceNames.begin( ), this->m_InterfaceNames.end( ), [ this, createInterface ] ( const char* interfaceName ) {
        int returnCode;
        uintptr_t interfaceAddress = reinterpret_cast< uintptr_t >( createInterface( interfaceName, &returnCode ) );
        this->PushInterface( std::make_pair( interfaceName, interfaceAddress ) );
        printf( "Found %s at %p\n", interfaceName, interfaceAddress );
    } );

    return true;
}

template<class T>
auto CInterfaces::GetInterface( const char* interfaceName ) -> std::optional<T> 
{
    auto isInterface = [ ] ( const char* str1, const char* str2 ) {
        return !strcmp( str1, str2 );
        };

    for ( const auto& [name, address] : this->m_Interfaces ) {
        if ( !isInterface( name, interfaceName ) ) continue;

        return reinterpret_cast< T >( address );
    }

    return std::nullopt;
}