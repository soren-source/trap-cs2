#include "SchemaSystem.hpp"
#include <includes.hpp>
#include <memory/memory.hpp>

SchemaSystem::SchemaSystem( ) {
	uintptr_t createInterface = reinterpret_cast<uintptr_t>( GetProcAddress( GetModuleHandleA( "schemasystem.dll" ), "CreateInterface" ) );

	this->m_InterfaceReg = *reinterpret_cast< InterfaceReg** >( g_memory.fix_mov( (uint8_t*)createInterface ) );

	for ( ; this->m_InterfaceReg; this->m_InterfaceReg = this->m_InterfaceReg->m_pNext ) {
		this->m_SchemaInterface = this->m_InterfaceReg->m_CreateFn;
		printf( "Found SchemaInterface %s at %p\n", this->m_InterfaceReg->m_pName, ( uintptr_t )this->m_InterfaceReg->m_CreateFn );	
	}
}

auto SchemaSystem::GetInterface( ) -> void*
{
	return this->m_SchemaInterface( );
}

auto SchemaManager::CacheSchemaData( ) -> void
{
	std::vector<const char*> modules = { "client.dll", "engine2.dll", "schemasystem.dll" };

	ISchemaSystem* schemaSystem = reinterpret_cast<ISchemaSystem*>( g_SchemaSystem->GetInterface( ) );
	if ( !schemaSystem ) return;

	for ( const auto& module : modules ) {
		auto schemaClass = schemaSystem->FindTypeScopeForModule( module );
		if ( !schemaClass ) continue;

		std::vector<SchemaDeclaredClass*> declaredClasses;
		schemaClass->GetDeclaratedClasses( declaredClasses );

		for ( const auto& declaredClass : declaredClasses ) {
			if ( !declaredClass ) continue;

			const auto& className = declaredClass->name;

			const auto& classPtr = declaredClass->classPtr;
			if ( !classPtr ) continue;

			for ( size_t i = 0; i < classPtr->fieldsCount; i++ ) {
				const auto& field = &classPtr->fields[ i ];
				if ( !field || !field->name ) continue;

				schema_offset_t schemaOffset;
				schemaOffset.class_name = className;
				schemaOffset.property_name = field->name;
				schemaOffset.offset = field->singleInheritanceOffset;

				schema_data[ className ][ field->name ] = schemaOffset;

				//printf( "Field %s in %s found with offset %i\n", field->name, className, field->singleInheritanceOffset );
			}
		}
	}
}
