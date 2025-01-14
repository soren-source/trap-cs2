#pragma once

#include <cstdint>
#include <imgui_internal.h>
#include <engine/EngineClasses/CEntityInstance.hpp>

struct CachedPlayer {
	CEntityInstance* m_pEntity = nullptr;
	int32_t m_Health = 0;
	int32_t m_Armour = 0;
	int32_t m_Ammo = 0;
	ImVec2 m_ScreenPosition = { 0.f, 0.f };
};