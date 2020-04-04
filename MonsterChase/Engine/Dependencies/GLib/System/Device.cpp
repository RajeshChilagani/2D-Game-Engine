#include "Device.h"

#include "DXHelpers.h"

#include <assert.h>
#include <d3d11_1.h>

namespace GLib
{
	D3D_FEATURE_LEVEL			g_FeatureLevel;
	ID3D11Device*				g_pD3DDevice = nullptr;
	IDXGISwapChain*				g_pSwapChain = nullptr;
	ID3D11RenderTargetView*		g_pRenderTargetView = nullptr;
	ID3D11DepthStencilView*		g_pDepthStencilView = nullptr;

	unsigned int				m_DisplayWidth;
	unsigned int				m_DisplayHeight;

	ID3D11DeviceContext*		pImmediateContext = nullptr;
	CRITICAL_SECTION			ImmediateContextCS;

	HRESULT CreateDevice(HWND i_hWnd, bool i_bDebugDevice)
	{
		RECT			rc;
		GetClientRect(i_hWnd, &rc);
		UINT width = rc.right - rc.left;
		UINT height = rc.bottom - rc.top;

		UINT createDeviceFlags = i_bDebugDevice ? D3D11_CREATE_DEVICE_DEBUG : 0;

		const D3D_FEATURE_LEVEL FeatureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0
		};

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = i_hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		HRESULT hResult = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &FeatureLevels[0], _countof(FeatureLevels),
			D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pD3DDevice, &g_FeatureLevel, &pImmediateContext);

		if (hResult == E_INVALIDARG)
		{
			hResult = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &FeatureLevels[1], _countof(FeatureLevels) - 1,
				D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pD3DDevice, &g_FeatureLevel, &pImmediateContext);
		}

		if (hResult != S_OK)
			return hResult;

		ID3D11Texture2D* pBackBuffer;
		// Get a pointer to the back buffer
		hResult = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (hResult != S_OK)
			return hResult;


		// Create a render-target view
		g_pD3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);

		// Bind the view
		pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);

		// Setup the viewport
		D3D11_VIEWPORT vp;
		vp.Width = FLOAT(width);
		vp.Height = FLOAT(height);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		pImmediateContext->RSSetViewports(1, &vp);

		m_DisplayWidth = width;
		m_DisplayHeight = height;

		InitializeCriticalSection(&ImmediateContextCS);

		return S_OK;
	}

	unsigned int GetDisplayWidth()
	{
		return m_DisplayWidth;
	}
	unsigned int GetDisplayHeight()
	{
		return m_DisplayHeight;
	}

	void ReleaseDevice()
	{
		if (pImmediateContext)
			pImmediateContext->ClearState();

		if (g_pDepthStencilView)
		{
			ULONG refCount = g_pDepthStencilView->Release();
			assert(refCount == 0);
		}

		if (g_pRenderTargetView)
		{
			ULONG refCount = g_pRenderTargetView->Release();
			assert(refCount == 0);

			g_pRenderTargetView = nullptr;
		}

		if (pImmediateContext)
		{
			ULONG refCount = pImmediateContext->Release();
			assert(refCount == 0);

			pImmediateContext = nullptr;
		}

		DeleteCriticalSection(&ImmediateContextCS);

		if (g_pSwapChain)
		{
			g_pSwapChain->Release();
			g_pSwapChain = nullptr;
		}

#if defined( REPORT_LIVE_DXOBJECTS )
		ID3D11Debug* pDebug = nullptr;
		HRESULT hResult = g_pD3DDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&pDebug));
		if ((hResult == S_OK) && (pDebug))
		{
			hResult = pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
			pDebug->Release();
		}
#endif // REPORT_LIVE_OBJECTS

		if (g_pD3DDevice)
		{
			g_pD3DDevice->Release();
			g_pD3DDevice = nullptr;
		}
	}

	SafeDeviceInterface<ID3D11DeviceContext> GetImmediateContext()
	{
		return SafeDeviceInterface<ID3D11DeviceContext>(*pImmediateContext, ImmediateContextCS);
	}

} // namespace GLib
