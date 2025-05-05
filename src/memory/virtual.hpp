#pragma once

#include <memory>

namespace Virtual {
    template <typename T>
    inline T Get( void* instance, const unsigned int index ) {
        return ( *static_cast< T** >( static_cast< void* >( instance ) ) )[ index ];
    }

    template <typename T, std::size_t nIndex, class CBaseClass, typename... Args_t>
    static T CallVFunc( CBaseClass* thisptr, Args_t... argList )
    {
        using VirtualFn_t = T( __thiscall* )( const void*, decltype( argList )... );
        return ( *reinterpret_cast< VirtualFn_t* const* >( reinterpret_cast< std::uintptr_t >( thisptr ) ) )[ nIndex ]( thisptr, argList... );
    }

    template <typename T>
    inline T Get( const void* instance, const unsigned int index ) {
        return ( *static_cast< T* const* >( static_cast< const void* >( instance ) ) )[ index ];
    }
}