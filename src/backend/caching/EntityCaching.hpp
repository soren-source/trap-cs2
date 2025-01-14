#pragma once

#include <includes.hpp>
#include "EntityCacheStructs.hpp"
#include <mutex>

class EntityCaching {
public:
	EntityCaching( );
	~EntityCaching( ) = default;

	[[nodiscard]]
	auto GetPlayerCache( ) -> std::vector<CachedPlayer> { return this->m_PlayerCache; }
	auto SetPlayerCache( std::vector<CachedPlayer> playerCache ) { std::lock_guard<std::mutex> lockGuard( this->m_PlayerCacheMutex ); this->m_PlayerCache = playerCache; }
private:
	std::vector<CachedPlayer> m_PlayerCache = { };
	std::mutex m_PlayerCacheMutex;
};

inline std::unique_ptr<EntityCaching> g_EntityCaching = std::make_unique<EntityCaching>( );