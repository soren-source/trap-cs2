#pragma once

#include "EngineClassBase.hpp"
#include <classes/Vector.hpp>

enum EBoneFlags : uint32_t {
    FLAG_NO_BONE_FLAGS = 0x0,
    FLAG_BONEFLEXDRIVER = 0x4,
    FLAG_CLOTH = 0x8,
    FLAG_PHYSICS = 0x10,
    FLAG_ATTACHMENT = 0x20,
    FLAG_ANIMATION = 0x40,
    FLAG_MESH = 0x80,
    FLAG_HITBOX = 0x100,
    FLAG_BONE_USED_BY_VERTEX_LOD0 = 0x400,
    FLAG_BONE_USED_BY_VERTEX_LOD1 = 0x800,
    FLAG_BONE_USED_BY_VERTEX_LOD2 = 0x1000,
    FLAG_BONE_USED_BY_VERTEX_LOD3 = 0x2000,
    FLAG_BONE_USED_BY_VERTEX_LOD4 = 0x4000,
    FLAG_BONE_USED_BY_VERTEX_LOD5 = 0x8000,
    FLAG_BONE_USED_BY_VERTEX_LOD6 = 0x10000,
    FLAG_BONE_USED_BY_VERTEX_LOD7 = 0x20000,
    FLAG_BONE_MERGE_READ = 0x40000,
    FLAG_BONE_MERGE_WRITE = 0x80000,
    FLAG_ALL_BONE_FLAGS = 0xfffff,
    BLEND_PREALIGNED = 0x100000,
    FLAG_RIGIDLENGTH = 0x200000,
    FLAG_PROCEDURAL = 0x400000,
};

struct alignas( 16 ) CBoneData {
    Vector3 position;
    float scale;
    void* rotation;
};

class CModel {
public:
    //BitFlag GetBoneFlags( std::int32_t index );

    std::int32_t GetBoneParent( std::int32_t index ) {
        return 1;
    };

public:
    std::uint8_t padding_0[ 0x170 ];
    std::int32_t BoneCount;
};

class CModelState {
public:
    std::uint8_t padding_0[ 0x80 ];
    CBoneData* bones;
    std::uint8_t padding_1[ 0x18 ];
    //CStrongHandle<CModel> modelHandle;
    //CUtlSymbolLarge modelName;
};

class CSkeletonInstance {
public:
    SCHEMA_FIELD( m_modelState, "CSkeletonInstance", "m_modelState", CModelState, this );
    SCHEMA_FIELD( m_nHitboxSet, "CSkeletonInstance", "m_nHitboxSet", std::uint8_t, this );
};