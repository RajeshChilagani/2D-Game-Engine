#pragma once

#include <Windows.h>
#include <d3d11_2.h>

namespace GLib
{
	extern D3D_DRIVER_TYPE			g_DriverType;
	extern D3D_FEATURE_LEVEL		g_FeatureLevel;
	extern ID3D11Device*			g_pD3DDevice;
	extern IDXGISwapChain*			g_pSwapChain;
	extern ID3D11RenderTargetView* g_pRenderTargetView;
	extern ID3D11DepthStencilView* g_pDepthStencilView;

	template<class T>
	class SafeDeviceInterface;

	HRESULT				CreateDevice(HWND i_hWnd, bool i_bDebugDevice);
	void				ReleaseDevice();

	SafeDeviceInterface<ID3D11DeviceContext> GetImmediateContext();

	unsigned int		GetDisplayWidth();
	unsigned int		GetDisplayHeight();
} // namespace GLib

#include "Device-inl.h"