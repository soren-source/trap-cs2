#pragma once

#include <d3d11.h>
#include <memory/hooking.hpp>

class PresentHook {
public:
	PresentHook( );
	~PresentHook( ) = default;

	static auto DetourPresent( IDXGISwapChain* swapchain, UINT syncInterval, UINT flags ) -> HRESULT;

	auto IntializeRenderingEnviroment( IDXGISwapChain* swapchain ) -> bool;
	auto RenderGui( ) -> void;

	auto Uninitialize( ) -> void;

	WNDPROC m_OriginalWndProc = nullptr;
private:
	typedef HRESULT( __fastcall* tPresent )( IDXGISwapChain*, UINT, UINT );
	Hook<tPresent> *m_HookContext;

	auto InitializeImGui( ) -> bool;

	auto BeginImGuiFrame( ) -> void;
	auto EndImGuiFrame( ) -> void;

	HWND m_Window = 0;
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
	ID3D11Texture2D* m_pBackBuffer = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;

	ID3D11ShaderResourceView* m_Logo = nullptr;
	ID3D11ShaderResourceView* m_Avatar = nullptr;
};

inline std::unique_ptr<PresentHook> g_PresentHook;