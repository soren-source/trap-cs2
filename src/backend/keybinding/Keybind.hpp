#pragma once

#include <includes.hpp>

class Keybind {
public:
	Keybind( uint16_t key, std::function<void()> callback ) : m_Key( key ), m_Callback( callback ) {};
	~Keybind( ) = default;

	std::function<void()> m_Callback;
	uint16_t m_Key;
private:
};