#pragma once

#include <cstdint>
#include <imgui_internal.h>
#include <engine/EngineClasses/CBaseEntity.hpp>
#include <classes/Vector.hpp>
#include <map>

enum CBones : std::uint8_t {
    HEAD = 6,
    NECK = 5,
    SPINE = 4,
    SPINE_1 = 2,
    HIP = 0,
    LEFT_SHOULDER = 8,
    LEFT_ARM = 9,
    LEFT_HAND = 10,
    RIGHT_SHOULDER = 13,
    RIGHT_ARM = 14,
    RIGHT_HAND = 15,

    LEFT_HIP = 22,
    LEFT_KNEE = 23,
    LEFT_FEET = 24,

    RIGHT_HIP = 25,
    RIGHT_KNEE = 26,
    RIGHT_FEET = 27,
};

extern const std::vector<std::uint8_t> g_SkeletonBones = { 6, 5, 4, 2, 0, 8, 9, 10, 13, 14, 15, 22, 23, 24, 25, 26, 27 };


struct BoundingBox {
    ImVec2 m_TopLeft = { 0.f, 0.f };
    ImVec2 m_TopMid = { 0.f, 0.f };
    ImVec2 m_TopRight = { 0.f, 0.f };
    ImVec2 m_BotLeft = { 0.f, 0.f };
    ImVec2 m_BotMid = { 0.f, 0.f };
    ImVec2 m_BotRight = { 0.f, 0.f };
};

struct CachedPlayer {
	CBaseEntity* m_pEntity = nullptr;
	int32_t m_Health = 0;
	int32_t m_Armour = 0;
	int32_t m_Ammo = 0;
	bool m_IsOnScreen = false;
	int m_HandleIndex = 0;
	ImVec2 m_ScreenPosition = { 0.f, 0.f };
	Vector3 m_WorldPosition = { 0.f, 0.f, 0.f };
    std::map<std::uint8_t, Vector3> m_BoneWorldPositions = { };
    std::map<std::uint8_t, ImVec2> m_BoneScreenPositions = { };

    std::pair<int, int> m_BoxPairWidth = { 0,0 };
    std::pair<int, int> m_BoxPairHeight = { 0,0 };

    BoundingBox m_BoundingBox;

    std::vector<std::pair<std::uint8_t, std::uint8_t>> m_BonePairs = {
        { CBones::LEFT_HAND, CBones::LEFT_ARM },
        { CBones::LEFT_ARM, CBones::LEFT_SHOULDER },
        { CBones::LEFT_SHOULDER, CBones::NECK },
        { CBones::RIGHT_HAND, CBones::RIGHT_ARM },
        { CBones::RIGHT_ARM, CBones::RIGHT_SHOULDER },
        { CBones::RIGHT_SHOULDER, CBones::NECK },
        { CBones::LEFT_FEET, CBones::LEFT_KNEE },
        { CBones::LEFT_KNEE, CBones::HIP },
        { CBones::RIGHT_FEET, CBones::RIGHT_KNEE },
        { CBones::RIGHT_KNEE, CBones::HIP },
        { CBones::HIP, CBones::SPINE_1 },
        { CBones::SPINE_1, CBones::SPINE },
        { CBones::SPINE, CBones::NECK },
        { CBones::NECK, CBones::HEAD }
    };
};