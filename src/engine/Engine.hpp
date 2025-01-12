#pragma once
#include <includes.hpp>

class Engine {
public:
	Engine( );
private:
	auto InitializeEngineComponents( ) -> void;
};

inline std::unique_ptr<Engine> g_Engine;