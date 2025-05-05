#include "KeyHandler.hpp"

auto KeyHandler::HandleKeyInput( uint8_t key ) -> void
{
	for ( const auto& keybind : this->m_Keybinds )
	{
		if ( keybind.m_Key == key )
			keybind.m_Callback( );
	}
}
