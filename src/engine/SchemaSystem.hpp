#pragma once
#include <memory>
#include <unordered_map>
#include <string>

namespace Virtual {
    template <typename T>
    inline T Get( void* instance, const unsigned int index ) {
        return ( *static_cast< T** >( static_cast< void* >( instance ) ) )[ index ];
    }

    template <typename T>
    inline T Get( const void* instance, const unsigned int index ) {
        return ( *static_cast< T* const* >( static_cast< const void* >( instance ) ) )[ index ];
    }
}

struct SchemaClassInfoData;
class SchemaSystemTypeScope;

class SchemaType
{
public:
    bool GetSizes( int* outSize1, uint8_t* unkProbablyNotSize )
    {
        return reinterpret_cast< int( __thiscall* )( void*, int*, uint8_t* ) >( m_vtable[ 3 ] )( this, outSize1, unkProbablyNotSize );
    }

    bool GetSize( int* outSize )
    {
        uint8_t smh = 0;
        return GetSizes( outSize, &smh );
    }

    const SchemaType* GetRefClass( )
    {
        if ( m_typeCategory != 1 )
            return nullptr;

        SchemaType* ptr = m_schemaType;

        while ( ptr && ptr->m_typeCategory == 1 )
            ptr = ptr->m_schemaType;

        return ptr;
    }

    typedef struct ArrayT
    {
        std::uint32_t arraySize;
        std::uint32_t unknown;
        SchemaType* elementType;
    } ArrayT;

    typedef struct AtomicT
    {
        std::uint64_t gap[ 2 ];
        SchemaType* templateTypename;
    } AtomicT;

    typedef struct AtomicTT
    {
        std::uint64_t gap[ 2 ];
        SchemaType* templates[ 2 ];
    } AtomicTT;

    typedef struct AtomicI
    {
        std::uint64_t gap[ 2 ];
        std::uint64_t integer;
    } AtomicI;

    union
    {
        SchemaType* m_schemaType;
        SchemaClassInfoData* m_classInfo;
        void* m_enumBinding;
        ArrayT m_array;
        AtomicT m_atomicT;
        AtomicTT m_atomicTT;
        AtomicI m_atomicI;
    };

public:
    std::uintptr_t* m_vtable;
    const char* m_name;
    SchemaSystemTypeScope* m_typeScope;
    std::uint8_t m_typeCategory;
    std::uint8_t m_atomicCategory;
};

struct SchemaClassFieldData
{
    const char* name;
    SchemaType* type;
    short offset;
    unsigned char pad1[ 0xE ];
};

struct SchemaClassInfoData
{
    unsigned char pad1[ 0x8 ];
    const char* name;
    char* module;
    std::int32_t size;
    std::int16_t fieldSize;
    std::int16_t staticSize;
    std::int16_t metadataSize;
    std::int16_t iUnk1;
    std::int16_t iUnk2;
    std::int16_t iUnk3;
    SchemaClassFieldData* fields;
};

struct SchemaTypeT
{
    std::uintptr_t* vtable;
    const char* name;
};

struct SchemaClassBindingT
{
    SchemaClassBindingT* parent;
    const char* binaryName;
    const char* moduleName;
    const char* className;
    void* classInfoOldSynthesized;
    void* classInfo;
    void* thisModuleBindingPointer;
    SchemaTypeT* schemaType;
};

class SchemaField
{
public:
    const char* name;
    SchemaType* type;
    uint32_t singleInheritanceOffset;
    uint32_t metadataSize;
    void* metadata;
};

class SchemaClass
{
public:
    char Padding_141[ 0x8 ];
    const char* name;
    const char* moduleName;
    uint32_t size;
    uint16_t fieldsCount;
    char Padding_146[ 0x2 ];
    uint16_t staticSize;
    uint16_t metadataSize;
    char Padding_149[ 0x4 ];
    SchemaField* fields;
};

class SchemaDeclaredClass
{
public:
    char Padding_156[ 0x8 ];
    const char* name;
    const char* moduleName;
    char Padding_159[ 0x4 ];
    SchemaClass* classPtr;
};

class SchemaDeclaredClassEntry
{
public:
    char Padding_166[ 0x10 ];
    SchemaDeclaredClass* declaratedClass;
};

typedef struct _schema_offset_t
{
    std::string class_name;
    std::string property_name;
    std::uint32_t offset;
} schema_offset_t;


class SchemaSystemTypeScope
{
public:
    char pad_0x0008[ 0x8 ];
    char m_szName[ 256 ];
    char pad_0x0338[ 0x338 ];
    SchemaDeclaredClassEntry* m_pDeclaredClasses;
    char pad_0x000E[ 0xE ];
    uint16_t m_nNumDeclaredClasses;
public:
    const void GetDeclaratedClasses( std::vector<SchemaDeclaredClass*>& declaratedClasses_ )
    {
        if ( !m_pDeclaredClasses )
            return;

        for ( int i = 0; i < m_nNumDeclaredClasses; ++i )
            declaratedClasses_.push_back( m_pDeclaredClasses[ i ].declaratedClass );
    }


    void FindDeclaredClass( SchemaClassInfoData** pReturnClass, const char* szClassName ) {
        return Virtual::Get<void( __thiscall* )( void*, SchemaClassInfoData**, const char* )>( this, 2 )( this, pReturnClass, szClassName );
    }
};

typedef void* ( *InstantiateInterfaceFn )( );
struct InterfaceReg {
    InstantiateInterfaceFn m_CreateFn;
    const char* m_pName;
    InterfaceReg* m_pNext;  // For the global list.
};

class SchemaManager {
public:
    auto CacheSchemaData( ) -> void;
    template<typename T>
    [[nodiscard]]
    auto ReadOffset( const char* className, const char* fieldName, void* instance ) -> T& { return *( T* )( ( uintptr_t )instance + GetSchema( className, fieldName ) ); }
private:

    std::unordered_map<std::string, std::unordered_map<std::string, schema_offset_t>> schema_data;
    [[nodiscard]] std::uint32_t GetSchema( std::string class_name, std::string property_name ) {
        return schema_data[ class_name ][ property_name ].offset;
    }

};

inline std::unique_ptr<SchemaManager> g_SchemaManager;

class SchemaSystem {
public:
    SchemaSystem( );
    [[nodiscard]]auto GetInterface( ) -> void*;
private:
    InterfaceReg* m_InterfaceReg;
    InstantiateInterfaceFn m_SchemaInterface;
};

inline std::unique_ptr<SchemaSystem> g_SchemaSystem;

class ISchemaSystem {
public:
    [[nodiscard]] SchemaSystemTypeScope* GlobalTypeScope( ) {
        return Virtual::Get<SchemaSystemTypeScope * ( __thiscall* )( void* )>( this, 11 )( this );
    }
    [[nodiscard]] SchemaSystemTypeScope* FindTypeScopeForModule( const char* module ) {
        return Virtual::Get<SchemaSystemTypeScope * ( __thiscall* )( void*, const char*, void* )>( this, 13 )( this, module, nullptr );
    }
};

#define SCHEMA_FIELD_OFFSET(varName, datatable, propName, extra_offset, type, ptr) \
  type varName() {                               \
                                                                              \
    const auto m_offset =                                              \
        g_SchemaManager->ReadOffset<type>(datatable, propName, ptr);    \
                                                                              \
    return m_offset;                         \
  }

#define SCHEMA_FIELD(varName, datatable, propName, type, ptr) \
  SCHEMA_FIELD_OFFSET(varName, datatable, propName, 0, type, ptr)