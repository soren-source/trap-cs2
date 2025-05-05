#pragma once

#include <includes.hpp>
#include <D3D11.h>

class Gui {
public:
	Gui( );
	~Gui( ) = default;

	auto Render( ) -> void;

	ID3D11ShaderResourceView* m_Logo = nullptr;
	ID3D11ShaderResourceView* m_Avatar = nullptr;
	bool m_IsOpened = false;
private:
};

inline std::unique_ptr<Gui> g_Gui = std::make_unique<Gui>( );