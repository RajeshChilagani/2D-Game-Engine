#include "ImmutableBuffer.h"

#include <assert.h>

#include "System\Device.h"

namespace GLib
{
	ImmutableBuffer::ImmutableBuffer(ID3D11Buffer& i_Buffer) :
		m_pBuffer(&i_Buffer)
	{}

	ImmutableBuffer* ImmutableBuffer::Create(IGPUResourceBuffer::eUsage i_Usage, void* i_pInitialData, unsigned int i_sizeInitialData)
	{
		assert(i_pInitialData);
		assert(i_sizeInitialData);
		assert(g_pD3DDevice);

		static const D3D11_BIND_FLAG UsageMap[] =
		{
			D3D11_BIND_VERTEX_BUFFER,
			D3D11_BIND_INDEX_BUFFER
		};

		D3D11_BUFFER_DESC	desc;

		desc.ByteWidth = i_sizeInitialData;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = UsageMap[size_t(i_Usage)];
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		ID3D11Buffer* pBuffer = nullptr;

		D3D11_SUBRESOURCE_DATA srd;
		ZeroMemory(&srd, sizeof(srd));

		srd.pSysMem = i_pInitialData;
		srd.SysMemPitch = i_sizeInitialData;

		HRESULT hResult = g_pD3DDevice->CreateBuffer(&desc, &srd, &pBuffer);
		assert((hResult == S_OK) || (hResult == E_OUTOFMEMORY));

		if (hResult == S_OK)
		{
			assert(pBuffer);
			return new ImmutableBuffer(*pBuffer);
		}
		else
			return nullptr;
	}

} // namespace GLib