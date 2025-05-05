#pragma once

#include <memory/virtual.hpp>

class IEngineClient {
public:
	[[nodiscard]] int GetMaxClients( )
	{
		return Virtual::CallVFunc<int, 34U>( this );
	}

	[[nodiscard]] bool IsInGame( )
	{
		return Virtual::CallVFunc<bool, 35U>( this );
	}

	[[nodiscard]] bool IsConnected( )
	{
		return Virtual::CallVFunc<bool, 36U>( this );
	}

	[[nodiscard]] void* GetNetChannelInfo( int nSplitScreenSlot = 0 )
	{
		return Virtual::CallVFunc<void*, 37U>( this, nSplitScreenSlot );
	}

	void ExecuteClientCmdUnrestricted( const char* szCommand )
	{
		Virtual::CallVFunc<void, 43U>( this, 0, szCommand, 0x7FFEF001 );
	}

	// return CBaseHandle index
	[[nodiscard]] int GetLocalPlayer( )
	{
		int nIndex = -1;

		Virtual::CallVFunc<void, 47U>( this, std::ref( nIndex ), 0 );

		return nIndex + 1;
	}

	void GetScreenSize( int& iWidth, int& iHeight )
	{
		Virtual::CallVFunc<void, 53U>( this, std::ref( iWidth ), std::ref( iHeight ) );
	}

	[[nodiscard]] const char* GetLevelName( )
	{
		return Virtual::CallVFunc<const char*, 56U>( this );
	}

	[[nodiscard]] const char* GetLevelNameShort( )
	{
		return Virtual::CallVFunc<const char*, 57U>( this );
	}

	[[nodiscard]] const char* GetProductVersionString( )
	{
		return Virtual::CallVFunc<const char*, 82U>( this );
	}
};