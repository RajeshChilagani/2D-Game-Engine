#include <d3d11.h>
#include <DirectXColors.h>

#include "System\Device.h"

namespace GLib
{
	bool BeginRendering(void)
	{
		assert(g_pSwapChain);

		SafeDeviceInterface<ID3D11DeviceContext> ImmediateContext = GetImmediateContext();

		// Just clear the backbuffer
		ImmediateContext->ClearRenderTargetView(g_pRenderTargetView, DirectX::Colors::White);

		if (g_pDepthStencilView)
			ImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		return true;
	}

	void EndRendering(void)
	{
		HRESULT hResult = g_pSwapChain->Present(0, 0);
		if (hResult == DXGI_ERROR_DEVICE_REMOVED)
		{
			hResult = g_pD3DDevice->GetDeviceRemovedReason();
		}
		assert(hResult == S_OK);
	}

} // namespace GLib