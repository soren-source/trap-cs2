#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../Feature.hpp"
#include <backend/caching/EntityCacheStructs.hpp>

class PlayerVisuals : public Feature
{
public:
	explicit PlayerVisuals( ) = default;

	void OnTick( ) override;
	std::string GetName( ) override;
	bool IsActive( ) override;
	int GetHotkey( ) override;

private:
	void DrawBox( BoundingBox box );
	void DrawSkeleton( CachedPlayer player );
	void DrawBar( CachedPlayer player );
};