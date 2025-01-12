// dllmain.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.
#include "core/startup.hpp"


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if ( ul_reason_for_call != DLL_PROCESS_ATTACH )
        return TRUE;

    std::thread( [ ] {
        g_Startup = std::make_unique<Startup>( );
        } ).detach( );

    return TRUE;
}

