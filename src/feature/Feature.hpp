#pragma once

#include <includes.hpp>

class Feature {
public:
	void virtual OnTick( ) = 0;
	std::string virtual GetName( ) = 0;
	bool virtual IsActive( ) = 0;
	int virtual GetHotkey( ) = 0;
};

class FeatureManager {
public:
	void TickAllFeatures( ) {
		std::for_each( this->m_Features.begin( ), this->m_Features.end( ), [ ] ( Feature* feature ) {
			if ( feature->IsActive( ) )
				feature->OnTick( );
			}
		);
	}
		
	void AddFeature( Feature* feature ) { this->m_Features.emplace_back( feature ); }

private:
	std::vector<Feature*> m_Features;
};

inline std::unique_ptr<FeatureManager> g_FeatureManager = std::make_unique<FeatureManager>( );