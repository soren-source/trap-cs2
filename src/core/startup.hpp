#pragma once
#include <includes.hpp>

class Startup {
public:
	Startup( );
	~Startup( ) = default;

private:
	auto AllocateConsole( ) -> void;
	auto IntializeCheatStart( ) -> void;

	bool m_IsDebug = true;
};

inline std::unique_ptr<Startup> g_Startup;