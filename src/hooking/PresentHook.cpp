#include "PresentHook.hpp"
#include <memory/memory.hpp>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui/backend/imgui_impl_dx11.h>
#include <imgui/backend/imgui_impl_win32.h>
#include <ui/hashes.hpp>
#include <ui/bytes.hpp>
#include <ui/image.hpp>

#include <D3DX11tex.h>

#include <ui/ui.hpp>
#include <backend/caching/EntityCaching.hpp>
#include <engine/EngineClasses/CBaseEntity.hpp>

#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3d11.lib")

extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
LRESULT __stdcall WndProc( const HWND wnd, UINT msg, WPARAM param, LPARAM lparam )
{
    if ( GetActiveWindow( ) == nullptr )
        return CallWindowProc( g_PresentHook->m_OriginalWndProc, wnd, msg, param, lparam );

    if ( ImGui_ImplWin32_WndProcHandler( wnd, msg, param, lparam ) )
        return 1L;

    return CallWindowProc( g_PresentHook->m_OriginalWndProc, wnd, msg, param, lparam );
}

PresentHook::PresentHook( )
{
	this->m_HookContext = new Hook<tPresent>( reinterpret_cast<uintptr_t>( g_Memory->FindPattern( "gameoverlayrenderer64.dll", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 41 8B E8" ) ), "IDXGISwapchain::Present" );
	auto result = this->m_HookContext->EnableHook( &DetourPresent );
}

inline std::once_flag intializeFlag;

auto PresentHook::DetourPresent( IDXGISwapChain* swapchain, UINT syncInterval, UINT flags ) -> HRESULT
{
	std::call_once( intializeFlag, [ swapchain ] { g_PresentHook->IntializeRenderingEnviroment( swapchain ); } );

    g_PresentHook->BeginImGuiFrame( );

    for ( const auto& entry : g_EntityCaching->GetPlayerCache( ) )
    {
        if ( !entry.m_pEntity ) continue;

        auto worldPos = entry.m_pEntity->GetOriginPosition( );

        Vector2 screenPos { };
        if ( !worldPos.ToScreen( screenPos ) ) continue;

        ImGui::GetBackgroundDrawList( )->AddText( { screenPos.x, screenPos.y }, ImColor( 255, 0, 0 ), "PLAYER" );
    }

    //g_PresentHook->RenderGui( );
    g_PresentHook->EndImGuiFrame( );

	return g_PresentHook->m_HookContext->GetOriginal( )( swapchain, syncInterval, flags );
}

auto PresentHook::IntializeRenderingEnviroment( IDXGISwapChain* swapchain ) -> bool
{
	this->m_pSwapChain = swapchain;
	if ( FAILED( swapchain->GetDevice( __uuidof( ID3D11Device ), reinterpret_cast< void** >( &this->m_pDevice ) ) ) )
		return false;

	this->m_pDevice->GetImmediateContext( &this->m_pDeviceContext );

	DXGI_SWAP_CHAIN_DESC desc;
	this->m_pSwapChain->GetDesc( &desc );
	this->m_Window = desc.OutputWindow;

	this->m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<void**>( &this->m_pBackBuffer ) );
	if ( !this->m_pBackBuffer ) return false;

	this->m_pDevice->CreateRenderTargetView( this->m_pBackBuffer, nullptr, &m_pRenderTargetView );
	this->m_pBackBuffer->Release( );

    this->m_OriginalWndProc = (WNDPROC)SetWindowLongPtrA( this->m_Window, GWLP_WNDPROC, (LONG_PTR)WndProc );

	this->InitializeImGui( );

	printf( "[+] Rendering Enviroment successfully created!\n" );
}

auto PresentHook::InitializeImGui( ) -> bool
{
	IMGUI_CHECKVERSION( );

	ImGui::CreateContext( );

	ImGui::StyleColorsDark( );

	const auto& io = ImGui::GetIO( );

	ImGui_ImplWin32_Init( this->m_Window );
	ImGui_ImplDX11_Init( this->m_pDevice, this->m_pDeviceContext );

	auto config = ImFontConfig( );
	config.FontDataOwnedByAtlas = false;

	static const ImWchar icon_ranges [ ] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	auto icons_config = ImFontConfig( );
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.FontDataOwnedByAtlas = false;

	io.Fonts->AddFontFromMemoryTTF( font_medium_bin, sizeof( font_medium_bin ), 14, &config );
	io.Fonts->AddFontFromMemoryTTF( font_awesome_bin, sizeof( font_awesome_bin ), 13, &icons_config, icon_ranges );
	io.Fonts->AddFontFromMemoryTTF( font_bold_bin, sizeof( font_bold_bin ), 14, &config );
	io.Fonts->AddFontFromMemoryTTF( font_medium_bin, sizeof( font_medium_bin ), 17, &config );
	io.Fonts->AddFontFromMemoryTTF( font_bold_bin, sizeof( font_bold_bin ), 17, &config );

	D3DX11_IMAGE_LOAD_INFO info;
	ID3DX11ThreadPump* pump { nullptr };

	D3DX11CreateShaderResourceViewFromMemory( this->m_pDevice, logob, sizeof( logob ), &info, pump, &this->m_Logo, 0 );
	D3DX11CreateShaderResourceViewFromMemory( this->m_pDevice, avatarb, sizeof( avatarb ), &info, pump, &this->m_Avatar, 0 );

    ui::add_page( 0, [ ] ( ) { } );

	return true;
}

auto PresentHook::BeginImGuiFrame( ) -> void
{
    ID3D11Texture2D* renderTargetTexture = nullptr;
    if ( !this->m_pRenderTargetView )
    {
        if ( FAILED( this->m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast< void** >( &renderTargetTexture ) ) ) ) return;

        this->m_pDevice->CreateRenderTargetView( renderTargetTexture, nullptr, &this->m_pRenderTargetView );
        renderTargetTexture->Release( );
    }

    ImGui_ImplDX11_NewFrame( );
    ImGui_ImplWin32_NewFrame( );
    ImGui::NewFrame( );
}

auto PresentHook::EndImGuiFrame( ) -> void
{
    this->m_pDeviceContext->OMSetRenderTargets( 1, &this->m_pRenderTargetView, 0 );
    ImGui::Render( );
    if ( this->m_pRenderTargetView )
    {
        this->m_pRenderTargetView->Release( );
        this->m_pRenderTargetView = nullptr;
    }

    ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );
}

auto PresentHook::RenderGui( ) -> void
{
    ImGui::SetNextWindowSize( ui::size );
    ImGui::Begin( "weave", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground ); {
        ImGui::BeginChild( "nav", { 176, ImGui::GetWindowHeight( ) }, 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar ); {
            ImGui::BeginChild( "brand", { ImGui::GetWindowWidth( ), 42 }, 0, ImGuiWindowFlags_NoBackground ); {
                ImGui::GetWindowDrawList( )->AddRectFilled( ImGui::GetWindowPos( ), { ImGui::GetWindowPos( ).x + ImGui::GetWindowWidth( ), ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) - 2 }, ImGui::GetColorU32( ImGuiCol_ChildBg ), GImGui->Style.WindowRounding, ImDrawFlags_RoundCornersTop );
                ImGui::GetWindowDrawList( )->AddRectFilled( { ImGui::GetWindowPos( ).x, ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) - 2 }, { ImGui::GetWindowPos( ).x + ImGui::GetWindowWidth( ), ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) }, ImGui::GetColorU32( ImGuiCol_Scheme ) );
                ImGui::GetWindowDrawList( )->AddImage( this->m_Logo, { ImGui::GetWindowPos( ).x + ImGui::GetWindowWidth( ) / 2 - 31 / 2, ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) / 2 - 14 / 2 - 2 }, { ImGui::GetWindowPos( ).x + ImGui::GetWindowWidth( ) / 2 + 31 / 2, ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) / 2 + 14 / 2 - 2 }, { 0, 0 }, { 1, 1 }, ImColor( 1.f, 1.f, 1.f, GImGui->Style.Alpha ) );
            }
            ImGui::EndChild( );

            ImGui::SetCursorPosY( 42 );
            ImGui::BeginChild( "tabs", { ImGui::GetWindowWidth( ), ImGui::GetWindowHeight( ) - 42 }, 0, ImGuiWindowFlags_NoBackground ); {
                imgui_blur::create_blur( ImGui::GetWindowDrawList( ), ImGui::GetWindowPos( ), ImGui::GetWindowPos( ) + ImGui::GetWindowSize( ), ImColor( 1.f, 1.f, 1.f, GImGui->Style.Alpha ), GImGui->Style.WindowRounding, ImDrawFlags_RoundCornersBottom );
                ImGui::GetWindowDrawList( )->AddRectFilled( ImGui::GetWindowPos( ), ImGui::GetWindowPos( ) + ImGui::GetWindowSize( ), ImGui::GetColorU32( ImGuiCol_WindowBg, 0.875f ), GImGui->Style.WindowRounding, ImDrawFlags_RoundCornersBottom );

                ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, { 1, 1 } );
                ImGui::BeginGroup( ); {
                    for ( int i = 0; i < ui::tabs.size( ); ++i ) {
                        ui::tab( i );
                    }
                }
                ImGui::EndGroup( );
                ImGui::PopStyleVar( );
            }
            ImGui::EndChild( );

            ImGui::SetCursorPos( { 0, ImGui::GetWindowHeight( ) - 42 } );
            ImGui::BeginChild( "profile", { ImGui::GetWindowWidth( ), 42 }, 0, ImGuiWindowFlags_NoBackground ); {
                ImGui::GetWindowDrawList( )->AddRectFilled( { ImGui::GetWindowPos( ).x, ImGui::GetWindowPos( ).y + 1 }, ImGui::GetWindowPos( ) + ImGui::GetWindowSize( ), ImGui::GetColorU32( ImGuiCol_Header ), GImGui->Style.WindowRounding, ImDrawFlags_RoundCornersBottom );
                ImGui::GetWindowDrawList( )->AddLine( ImGui::GetWindowPos( ), { ImGui::GetWindowPos( ).x + ImGui::GetWindowWidth( ), ImGui::GetWindowPos( ).y }, ImGui::GetColorU32( ImGuiCol_BorderShadow ) );
                ImGui::GetWindowDrawList( )->AddImageRounded( this->m_Avatar, { ImGui::GetWindowPos( ).x + 15, ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) / 2 - 24 / 2 }, { ImGui::GetWindowPos( ).x + 39, ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) / 2 + 24 / 2 }, { 0, 0 }, { 1, 1 }, ImColor( 1.f, 1.f, 1.f, GImGui->Style.Alpha ), 100 );
                ImGui::GetWindowDrawList( )->AddText( font( 2 ), font( 2 )->FontSize, { ImGui::GetWindowPos( ).x + 47, ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) / 2 - font( 2 )->CalcTextSizeA( font( 2 )->FontSize, FLT_MAX, 0.f, "evrope" ).y / 2 }, ImGui::GetColorU32( ImGuiCol_Text ), "evrope" );
                ImGui::GetWindowDrawList( )->AddText( font( 2 ), font( 2 )->FontSize, { ImGui::GetWindowPos( ).x + ImGui::GetWindowWidth( ) - font( 2 )->CalcTextSizeA( font( 2 )->FontSize, FLT_MAX, 0.f, "28d" ).x - 15, ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) / 2 - font( 2 )->CalcTextSizeA( font( 2 )->FontSize, FLT_MAX, 0.f, "28d" ).y / 2 }, ImGui::GetColorU32( ImGuiCol_TextDisabled ), "28d" );
            }
            ImGui::EndChild( );
        }
        ImGui::EndChild( );

        ImGui::SetCursorPos( { 185, 0 } );
        ImGui::BeginChild( "main", ImGui::GetContentRegionAvail( ), 0, ImGuiWindowFlags_NoBackground ); {
            ImGui::BeginChild( "main header", { ImGui::GetWindowWidth( ), 42 }, 0, ImGuiWindowFlags_NoBackground ); {
                ImGui::GetWindowDrawList( )->AddRectFilled( ImGui::GetWindowPos( ), { ImGui::GetWindowPos( ).x + ImGui::GetWindowWidth( ), ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) - 2 }, ImGui::GetColorU32( ImGuiCol_ChildBg ), GImGui->Style.WindowRounding, ImDrawFlags_RoundCornersTop );
                ImGui::GetWindowDrawList( )->AddRectFilled( { ImGui::GetWindowPos( ).x, ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) - 2 }, { ImGui::GetWindowPos( ).x + ImGui::GetWindowWidth( ), ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) }, ImGui::GetColorU32( ImGuiCol_Scheme ) );
                ImGui::GetWindowDrawList( )->AddText( { ImGui::GetWindowPos( ).x + 15, ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) / 2 - ImGui::CalcTextSize( ui::tabs[ ui::cur_tab ].m_icon, 0, 1 ).y / 2 }, ImGui::GetColorU32( ImGuiCol_Text, ui::anim ), ui::tabs[ ui::cur_tab ].m_icon, ImGui::FindRenderedTextEnd( ui::tabs[ ui::cur_tab ].m_icon ) );
                ImGui::GetWindowDrawList( )->AddText( font( 2 ), font( 2 )->FontSize, { ImGui::GetWindowPos( ).x + 40, ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) / 2 - font( 2 )->CalcTextSizeA( font( 2 )->FontSize, FLT_MAX, 0.f, ui::tabs[ ui::cur_tab ].m_name, FindRenderedTextEnd( ui::tabs[ ui::cur_tab ].m_name ) ).y / 2 }, ImGui::GetColorU32( ui::tabs[ ui::cur_tab ].m_subtabs.size( ) > 0 ? ImGuiCol_TextDisabled : ImGuiCol_Text, ui::anim ), ui::tabs[ ui::cur_tab ].m_name, ImGui::FindRenderedTextEnd( ui::tabs[ ui::cur_tab ].m_name ) );
                if ( ui::tabs[ ui::cur_tab ].m_subtabs.size( ) > 0 ) {
                    ImGui::RenderArrow( ImGui::GetWindowDrawList( ), { ImGui::GetWindowPos( ).x + 47 + font( 2 )->CalcTextSizeA( font( 2 )->FontSize, FLT_MAX, 0.f, ui::tabs[ ui::cur_tab ].m_name, FindRenderedTextEnd( ui::tabs[ ui::cur_tab ].m_name ) ).x, ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) / 2 - 10 }, ImGui::GetColorU32( ImGuiCol_Text, ui::anim * ui::content_anim ), ImGuiDir_Right, 12 );
                    ImGui::GetWindowDrawList( )->AddText( font( 2 ), font( 2 )->FontSize, { ImGui::GetWindowPos( ).x + 60 + font( 2 )->CalcTextSizeA( font( 2 )->FontSize, FLT_MAX, 0.f, ui::tabs[ ui::cur_tab ].m_name, FindRenderedTextEnd( ui::tabs[ ui::cur_tab ].m_name ) ).x, ImGui::GetWindowPos( ).y + ImGui::GetWindowHeight( ) / 2 - font( 2 )->CalcTextSizeA( font( 2 )->FontSize, FLT_MAX, 0.f, ui::tabs[ ui::cur_tab ].m_subtabs[ ui::tabs[ ui::cur_tab ].cur_subtab ], FindRenderedTextEnd( ui::tabs[ ui::cur_tab ].m_subtabs[ ui::tabs[ ui::cur_tab ].cur_subtab ] ) ).y / 2 }, ImGui::GetColorU32( ImGuiCol_Scheme, ui::anim * ui::content_anim ), ui::tabs[ ui::cur_tab ].m_subtabs[ ui::tabs[ ui::cur_tab ].cur_subtab ], ImGui::FindRenderedTextEnd( ui::tabs[ ui::cur_tab ].m_subtabs[ ui::tabs[ ui::cur_tab ].cur_subtab ] ) );
                }
            }
            ImGui::EndChild( );

            ImGui::GetWindowDrawList( )->AddRectFilled( { ImGui::GetWindowPos( ).x, ImGui::GetWindowPos( ).y + 42 }, ImGui::GetWindowPos( ) + ImGui::GetWindowSize( ), ImGui::GetColorU32( ImGuiCol_WindowBg ), GImGui->Style.WindowRounding, ImDrawFlags_RoundCornersBottom );

            ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, { 10, 10 } );
            ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, { 15, 15 } );
            ImGui::PushStyleVar( ImGuiStyleVar_Alpha, ui::anim * ui::content_anim );
            ImGui::SetCursorPosY( 42 );
            ImGui::BeginChild( "content", ImGui::GetContentRegionAvail( ), 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysUseWindowPadding ); {
                ui::render_page( );
            }
            ImGui::EndChild( );
            ImGui::PopStyleVar( 3 );
        }
        ImGui::EndChild( );
    }
    ImGui::End( );

    ui::handle_anims( );
}


auto PresentHook::Uninitialize( ) -> void
{
    SetWindowLongPtrA( this->m_Window, GWLP_WNDPROC, (LONG_PTR)this->m_OriginalWndProc );
}