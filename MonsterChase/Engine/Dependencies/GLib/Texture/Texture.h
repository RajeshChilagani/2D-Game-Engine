#pragma once

#include <assert.h>

#include "Helpers/IReferenceCounted.h"
#include "System/Device.h"
#include "DDSLoader/DDSTextureLoader.h"

namespace GLib
{
	class Texture : public IReferenceCounted
	{
		friend class Sprite;

	public:
		Texture(ID3D11Resource* i_pTextureResource, ID3D11ShaderResourceView* i_pShaderResource) :
			IReferenceCounted(1),
			m_pTextureResource(i_pTextureResource),
			m_pShaderResource(i_pShaderResource)
		{}	

		bool GetDimensions(unsigned int& o_Width, unsigned int& o_Height, unsigned int& o_Depth);

		ID3D11Resource* GetTextureResource() {
			return m_pTextureResource;
		}		

		ID3D11ShaderResourceView* GetShaderResource() {
			return m_pShaderResource;
		}

	private:
		~Texture()
		{
			m_pShaderResource->Release();
			m_pTextureResource->Release();
		}

		ID3D11Resource* m_pTextureResource;
		ID3D11ShaderResourceView* m_pShaderResource;
	};

	Texture* CreateTexture(void* i_pTextureData, size_t i_sizeTextureData);
	void Release(Texture* i_pTexture);
} // namespace GLib