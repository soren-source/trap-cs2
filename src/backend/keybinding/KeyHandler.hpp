#pragma once

#include <includes.hpp>

class KeyHandler {
public:
	explicit KeyHandler( ) = default;
	~KeyHandler( ) = default;

	auto HandleKeyInput( uint8_t key ) -> void;
};