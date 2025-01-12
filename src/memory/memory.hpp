#pragma once
#include <includes.hpp>


class memory
{
public:
    explicit memory( ) = default;
    ~memory( ) = default;

    uint64_t fix_call( uint8_t* address );
    uint64_t fix_mov( uint8_t* patternMatch );
    uint8_t* find_pattern( const std::string_view module, const std::string_view signature );
    uint8_t* find_pattern( const std::string_view signature );

    uintptr_t get_module_base( std::string module );
    std::string m_altv_module_name;

    template <typename T = uint8_t*>
    inline T sub( uint8_t* address, int offset )
    {
        return reinterpret_cast< T >( address ) - offset;
    }

    template <typename T = uint8_t*>
    inline T add( uint8_t* address, int offset )
    {
        return reinterpret_cast< T >( address ) + offset;
    }

    inline uintptr_t rip( uintptr_t address )
    {
        return address + 4;
    }

    template <typename type>
    type as_relative( type address, int offset = 3 )
    {
        return reinterpret_cast< type >( reinterpret_cast< uintptr_t >( address ) + *reinterpret_cast< int* >( reinterpret_cast< uintptr_t >( address ) + offset ) + ( offset + 4i32 ) );
    }

    template <typename type = uintptr_t>
    type as_relative( void* address, int offset = 3 )
    {
        return reinterpret_cast< type >( reinterpret_cast< uintptr_t >( address ) + *reinterpret_cast< int* >( reinterpret_cast< uintptr_t >( address ) + offset ) + ( offset + 4i32 ) );
    }

    template <typename type = uintptr_t>
    type as_relative( uintptr_t address, int offset = 3 )
    {
        return ( type )( address + *reinterpret_cast< int* >( address + offset ) + ( offset + 4i32 ) );
    }

private:
};

inline memory g_memory;

typedef struct _PEB_LDR_DATA {
    BYTE       Reserved1[ 8 ];
    PVOID      Reserved2[ 3 ];
    LIST_ENTRY InMemoryOrderModuleList;
} PEB_LDR_DATA, * PPEB_LDR_DATA;

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING, * PUNICODE_STRING;

typedef struct _RTL_USER_PROCESS_PARAMETERS {
    BYTE           Reserved1[ 16 ];
    PVOID          Reserved2[ 10 ];
    UNICODE_STRING ImagePathName;
    UNICODE_STRING CommandLine;
} RTL_USER_PROCESS_PARAMETERS, * PRTL_USER_PROCESS_PARAMETERS;

typedef struct _PEB {
    BYTE                          Reserved1[ 2 ];
    BYTE                          BeingDebugged;
    BYTE                          Reserved2[ 1 ];
    PVOID                         Reserved3[ 2 ];
    PPEB_LDR_DATA                 Ldr;
    PRTL_USER_PROCESS_PARAMETERS  ProcessParameters;
    PVOID                         Reserved4[ 3 ];
    PVOID                         AtlThunkSListPtr;
    PVOID                         Reserved5;
    ULONG                         Reserved6;
    PVOID                         Reserved7;
    ULONG                         Reserved8;
    ULONG                         AtlThunkSListPtr32;
    PVOID                         Reserved9[ 45 ];
    BYTE                          Reserved10[ 96 ];
    void* PostProcessInitRoutine;
    BYTE                          Reserved11[ 128 ];
    PVOID                         Reserved12[ 1 ];
    ULONG                         SessionId;
} PEB, * PPEB;

typedef struct _TEB {
    PVOID Reserved1[ 12 ];
    PPEB  ProcessEnvironmentBlock;
    PVOID Reserved2[ 399 ];
    BYTE  Reserved3[ 1952 ];
    PVOID TlsSlots[ 64 ];
    BYTE  Reserved4[ 8 ];
    PVOID Reserved5[ 26 ];
    PVOID ReservedForOle;
    PVOID Reserved6[ 4 ];
    PVOID TlsExpansionSlots;
} TEB, * PTEB;

typedef struct _LDR_DATA_TABLE_ENTRY {
    PVOID Reserved1[ 2 ];
    LIST_ENTRY InMemoryOrderLinks;
    PVOID Reserved2[ 2 ];
    PVOID DllBase;
    PVOID EntryPoint;
    PVOID Reserved3;
    UNICODE_STRING FullDllName;
    BYTE Reserved4[ 8 ];
    PVOID Reserved5[ 3 ];
    union {
        ULONG CheckSum;
        PVOID Reserved6;
    };
    ULONG TimeDateStamp;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;