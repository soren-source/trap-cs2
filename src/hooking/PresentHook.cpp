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
#include <gui/gui.hpp>
#include <backend/keybinding/KeyHandler.hpp>
#include <engine/EngineClasses/CGameSceneNode.hpp>
#include <engine/EngineClasses/CSkeletonInstance.hpp>
#include <engine/CInterfaces.hpp>
#include <engine/CEntitiyListSystem.hpp>
#include <feature/Feature.hpp>

#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3d11.lib")

extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
LRESULT __stdcall WndProc( const HWND wnd, UINT msg, WPARAM param, LPARAM lparam )
{
    if ( GetActiveWindow( ) == nullptr )
        return CallWindowProc( g_PresentHook->m_OriginalWndProc, wnd, msg, param, lparam );

	if ( msg == WM_KEYDOWN )
		g_KeyHandler->HandleKeyInput( param );

    if ( ImGui_ImplWin32_WndProcHandler( wnd, msg, param, lparam ) && g_Gui->m_IsOpened )
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
	{
		g_EntityCaching->RefreshCachedEntityInformations( );	

		g_FeatureManager->TickAllFeatures( );

		g_Gui->Render( );
	}
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
	return true;
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

	D3DX11CreateShaderResourceViewFromMemory( this->m_pDevice, logob, sizeof( logob ), &info, pump, &g_Gui->m_Logo, 0 );
	D3DX11CreateShaderResourceViewFromMemory( this->m_pDevice, avatarb, sizeof( avatarb ), &info, pump, &g_Gui->m_Avatar, 0 );

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

auto PresentHook::Uninitialize( ) -> void
{
    SetWindowLongPtrA( this->m_Window, GWLP_WNDPROC, (LONG_PTR)this->m_OriginalWndProc );
}