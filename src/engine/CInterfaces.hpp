#pragma once
#include <includes.hpp>
class CInterfaces {
public:
	explicit CInterfaces( ) = default;
	~CInterfaces( ) = default;

	auto CacheEngineInterfaces( ) -> bool;
	//Pushes an std::pair, into private member m_Interfaces
	auto PushInterface( std::pair<const char*, uintptr_t> interfacePair ) -> void { this->m_Interfaces[ interfacePair.first ] = interfacePair.second; }

	template<typename T>
	[[nodiscard( "GetInterface return value must be read!" )]] 
	auto GetInterface( const char* interfaceName ) -> std::optional<T>
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

private:
	typedef void* ( __fastcall* tCreateInterface )( const char* interfaceName, int* returnCode );

	std::unordered_map<const char*, uintptr_t> m_Interfaces;
	std::vector<const char*> m_ClientInterfaceNames = { "Source2Client002", "GameClientExports001", "ClientToolsInfo_001" };
};

inline std::unique_ptr<CInterfaces> g_EngineInterfaces = std::make_unique<CInterfaces>( );;
