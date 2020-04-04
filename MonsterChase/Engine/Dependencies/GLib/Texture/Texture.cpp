#include "Texture.h"

#include <d3d11.h>

#include "System\Device.h"
#include "System\Helpers.h"

namespace GLib
{
	bool Texture::GetDimensions(unsigned int& o_Width, unsigned int& o_Height, unsigned int& o_Depth)
	{
		if (m_pTextureResource == nullptr)
		{
			o_Width = o_Height = o_Depth = 0;
			return false;
		}

		D3D11_RESOURCE_DIMENSION rd;

		m_pTextureResource->GetType(&rd);

		switch (rd)
		{
		case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
		{
			ID3D11Texture1D* pTexture = nullptr;
			HRESULT hResult = m_pTextureResource->QueryInterface(__uuidof(ID3D11Texture1D), reinterpret_cast<void**>(&pTexture));
			assert(hResult == S_OK);
			assert(pTexture);

			D3D11_TEXTURE1D_DESC td;

			pTexture->GetDesc(&td);

			o_Width = td.Width;
			o_Height = 1;
			o_Depth = 1;

			pTexture->Release();
			return true;
		}

		case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
		{
			ID3D11Texture2D* pTexture = nullptr;
			HRESULT hResult = m_pTextureResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pTexture));
			assert(hResult == S_OK);
			assert(pTexture);

			D3D11_TEXTURE2D_DESC td;

			pTexture->GetDesc(&td);

			o_Width = td.Width;
			o_Height = td.Height;
			o_Depth = 1;

			pTexture->Release();
			return true;
		}

		case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
		{
			ID3D11Texture3D* pTexture = nullptr;
			HRESULT hResult = m_pTextureResource->QueryInterface(__uuidof(ID3D11Texture3D), reinterpret_cast<void**>(&pTexture));
			assert(hResult == S_OK);
			assert(pTexture);

			D3D11_TEXTURE3D_DESC td;

			pTexture->GetDesc(&td);

			o_Width = td.Width;
			o_Height = td.Height;
			o_Depth = td.Depth;

			pTexture->Release();
			return true;
		}
		default:
		{
			o_Width = o_Height = o_Depth = 0;
			return false;
		}
		}

	}

	Texture* CreateTexture(void* i_pTextureData, size_t i_sizeTextureData)
	{
		assert(g_pD3DDevice);

		SafeDeviceInterface<ID3D11DeviceContext> ImmediateContext = GetImmediateContext();

		ID3D11Resource* pTextureResource = nullptr;
		ID3D11ShaderResourceView* pShaderResource = nullptr;

		HRESULT result = DirectX::CreateDDSTextureFromMemory(g_pD3DDevice, ImmediateContext, reinterpret_cast<uint8_t*>(i_pTextureData),
			i_sizeTextureData, &pTextureResource, &pShaderResource);

		return (result == S_OK) ? new Texture(pTextureResource, pShaderResource) : nullptr;
	}

	bool GetDimensions(Texture& i_Texture, unsigned int& o_Width, unsigned int& o_Height, unsigned int& o_Depth)
	{
		return i_Texture.GetDimensions(o_Width, o_Height, o_Depth);
	}


	void Release(Texture* i_pTexture)
	{
		if (i_pTexture)
			i_pTexture->ReleaseRef();
	}
} // namespace GLib