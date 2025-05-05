#pragma once

#include <includes.hpp>
#include "Keybind.hpp"
#include <vector>
#include <variant>

#define ADD_KEYBIND(keybind)\
if (!g_KeyHandler) g_KeyHandler = std::make_unique<KeyHandler>( );\
g_KeyHandler->AddKeybind(keybind);\

class KeyHandler {
public:
	explicit KeyHandler( ) = default;
	~KeyHandler( ) = default;

	auto HandleKeyInput( uint8_t key ) -> void;
	auto AddKeybind( const Keybind& keybind ) -> void { this->m_Keybinds.emplace_back( keybind ); };

private:
	std::vector<Keybind> m_Keybinds;
};

inline std::unique_ptr<KeyHandler> g_KeyHandler;