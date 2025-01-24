#include "memory.hpp"
#include <codecvt>
#include <sstream>

#pragma warning(disable : 4996)
#define FIND_NT_HEADER(x) reinterpret_cast<PIMAGE_NT_HEADERS>(uint64_t(x) + ((PIMAGE_DOS_HEADER)(x))->e_lfanew)

uint64_t Memory::FixCall( uint8_t* address )
{
    return reinterpret_cast< uint64_t >( address + *reinterpret_cast< int32_t* >( address + 1 ) + 5 );
}

uint64_t Memory::FixMov( uint8_t* patternMatch )
{
    return reinterpret_cast< uint64_t >( patternMatch + *reinterpret_cast< int32_t* >( patternMatch + 3 ) + 7 );
}

uint8_t* Memory::FindPattern( const std::string_view module, const std::string_view signature )
{
    std::vector<uint8_t> signature_bytes( signature.size( ) + 1 );

    {
        std::vector<std::string> signature_chunks {};
        std::string current_chunk {};

        std::istringstream string_stream { signature.data( ) };

        while ( std::getline( string_stream, current_chunk, ' ' ) )
            signature_chunks.push_back( current_chunk );

        std::transform( signature_chunks.cbegin( ), signature_chunks.cend( ), signature_bytes.begin( ), [ ] ( const std::string& val ) -> uint8_t
            { return val.find( '?' ) != std::string::npos ? 0ui8 : static_cast< uint8_t >( std::stoi( val, nullptr, 16 ) ); } );
    }

    uint8_t* found_bytes = nullptr;

    {
        const auto image_start = reinterpret_cast< uint8_t* >( this->GetModuleBase( module.data( ) ) );
        printf( "%s %p\n", module.data( ), image_start );
        const auto image_end = image_start + FIND_NT_HEADER( image_start )->OptionalHeader.SizeOfImage;

        const auto result = std::search( image_start, image_end, signature_bytes.cbegin( ), signature_bytes.cend( ), [ ] ( uint8_t left, uint8_t right ) -> bool
            { return right == 0ui8 || left == right; } );

        found_bytes = ( result != image_end ) ? result : nullptr;
    }

    return found_bytes;
}

uint8_t* Memory::FindPattern( const std::string_view signature )
{
    std::vector<uint8_t> signature_bytes( signature.size( ) + 1 );

    {
        std::vector<std::string> signature_chunks {};
        std::string current_chunk {};

        std::istringstream string_stream { signature.data( ) };

        while ( std::getline( string_stream, current_chunk, ' ' ) )
            signature_chunks.push_back( current_chunk );

        std::transform( signature_chunks.cbegin( ), signature_chunks.cend( ), signature_bytes.begin( ), [ ] ( const std::string& val ) -> uint8_t
            { return val.find( '?' ) != std::string::npos ? 0ui8 : static_cast< uint8_t >( std::stoi( val, nullptr, 16 ) ); } );
    }

    uint8_t* found_bytes = nullptr;

    {
        const auto image_start = reinterpret_cast< uint8_t* >( this->GetModuleBase( "" ) );
        const auto image_end = image_start + FIND_NT_HEADER( image_start )->OptionalHeader.SizeOfImage;

        const auto result = std::search( image_start, image_end, signature_bytes.cbegin( ), signature_bytes.cend( ), [ ] ( uint8_t left, uint8_t right ) -> bool
            { return right == 0ui8 || left == right; } );

        found_bytes = ( result != image_end ) ? result : nullptr;
    }

    return found_bytes;
}

uintptr_t Memory::GetModuleBase( std::string module )
{
    PPEB peb = ( PPEB )__readgsqword( 0x60 );;
    if ( !peb ) return 0;

    auto ldr = peb->Ldr;
    if ( !ldr ) return 0;

    auto list = &ldr->InMemoryOrderModuleList;
    if ( !list ) return 0;

    std::wstring w_module;
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    if ( !module.size( ) )
    {
        auto params = peb->ProcessParameters;
        auto name = params->ImagePathName;
        w_module = name.Buffer;
        size_t pos = w_module.find_last_of( L"\\" );
        w_module = w_module.substr( pos + 1 );
    }
    else
    {
        w_module = converter.from_bytes( module );
    }

    for ( auto item = list->Flink; item != list, item = item->Flink; )
    {
        auto entry = ( PLDR_DATA_TABLE_ENTRY )item;
        if ( !entry->FullDllName.Buffer ) continue;

        if ( !wcscmp( w_module.c_str( ), entry->FullDllName.Buffer ) )
            return ( uintptr_t )entry->Reserved2[ 0 ];
    }

    return uintptr_t( );
}
