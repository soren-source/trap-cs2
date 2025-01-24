#include "Vector.hpp"
#include <memory/memory.hpp>
#include <imgui.h>

struct ViewMatrix {
	float* operator[ ]( int index ) {
		return matrix[ index ];
	}

	float matrix[ 4 ][ 4 ];
};

bool Vector3::ToScreen( Vector2& out ) 
{
	static uintptr_t viewMatrix = 0;
	if ( !viewMatrix )
		viewMatrix = static_cast< uintptr_t >( g_Memory->FixMov( g_Memory->FindPattern( "client.dll", "48 8D 0D ? ? ? ? 48 C1 E0 06" ) ) );

	auto& matrix = *( ViewMatrix* )( viewMatrix );

	out.x = matrix[ 0 ][ 0 ] * this->x + matrix[ 0 ][ 1 ] * this->y + matrix[ 0 ][ 2 ] * this->z + matrix[ 0 ][ 3 ];
	out.y = matrix[ 1 ][ 0 ] * this->x + matrix[ 1 ][ 1 ] * this->y + matrix[ 1 ][ 2 ] * this->z + matrix[ 1 ][ 3 ];

	float w = matrix[ 3 ][ 0 ] * this->x + matrix[ 3 ][ 1 ] * this->y + matrix[ 3 ][ 2 ] * this->z + matrix[ 3 ][ 3 ];

	if ( w < 0.01f )
		return false;

	float inv_w = 1.f / w;
	out.x *= inv_w;
	out.y *= inv_w;

	auto screen_size = ImGui::GetIO( ).DisplaySize;

	float x = screen_size.x * .5f;
	float y = screen_size.y * .5f;

	x += 0.5f * out.x * screen_size.x + 0.5f;
	y -= 0.5f * out.y * screen_size.y + 0.5f;

	out.x = x;
	out.y = y;

	return true;
}