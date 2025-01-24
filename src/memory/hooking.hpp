#pragma once

#include <includes.hpp>
#include <minhook/MinHook.h>

template <typename T>
class Hook {
public:
	Hook( ) { };
	Hook( uintptr_t address, const char* name ) : m_FunctionAddress( address ), m_HookStatus( -1 ), m_OriginalFunction( NULL ), m_HookedFunctionName( name ) { };
	~Hook( ) { delete this;	}

	[[nodiscard]]
	auto EnableHook( void* detourFunction ) -> bool
	{
		auto status = MH_CreateHook( reinterpret_cast< PVOID >( this->m_FunctionAddress ), detourFunction, reinterpret_cast< void** >( &this->m_OriginalFunction ) );
		if ( status != MH_OK ) { std::cout << MH_StatusToString( status ) << std::endl; return false; }
		status = MH_EnableHook( reinterpret_cast< PVOID >( this->m_FunctionAddress ) );
		if ( status != MH_OK ) { std::cout << MH_StatusToString( status ) << std::endl; return false; }

		printf( "[+] Hook %s at %p placed successfully\n", this->m_HookedFunctionName, this->m_FunctionAddress );

		return true;
	}

	[[nodiscard]]
	auto DisableHook( ) -> bool;

	[[nodiscard]]
	inline auto GetOriginal( ) -> T
	{
		return m_OriginalFunction;
	}
private:
	const char* m_HookedFunctionName = "";
	uint8_t m_HookStatus = -1;
	uintptr_t m_FunctionAddress = 0;
	T m_OriginalFunction = NULL;
};


