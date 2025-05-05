#pragma once
#include <includes.hpp>
#include <engine/SchemaSystem.hpp>

class Engine {
public:
	Engine( );
	auto GetInterfaceRegisterList( const char* moduleName ) -> InterfaceReg*;

	template<typename T>
	auto GetInterface( const InterfaceReg* interfaceReg, const char* interfaceName ) -> T*
	{
		for ( const InterfaceReg* pRegister = interfaceReg; pRegister != nullptr; pRegister = pRegister->m_pNext ) {
			if ( pRegister->m_pName && !strcmp( pRegister->m_pName, interfaceName ) ) {
				void* pInterface = pRegister->m_CreateFn( );
				return static_cast< T* >( pInterface );
			}
		}

		return nullptr;
	}

	auto GetInterface( const InterfaceReg* interfaceReg, const char* interfaceName ) -> uintptr_t
	{
		for ( const InterfaceReg* pRegister = interfaceReg; pRegister != nullptr; pRegister = pRegister->m_pNext ) {
			if ( pRegister->m_pName && !strcmp( pRegister->m_pName, interfaceName ) ) {
				void* pInterface = pRegister->m_CreateFn( );
				return reinterpret_cast< uintptr_t >( pInterface );
			}
		}

		return 0;
	}
private:
	auto InitializeEngineComponents( ) -> void;

};

inline std::unique_ptr<Engine> g_Engine;