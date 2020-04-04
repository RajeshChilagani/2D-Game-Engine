#include "DrawResourcesBuffer.h"

#include <assert.h>

#include "System\Device.h"
#include "System\Helpers.h"

namespace GLib
{
	DrawResourcesBuffer::DrawResourcesBuffer(ID3D11Buffer& i_Buffer, unsigned int i_sizeBuffer, unsigned int i_startOffset) :
		m_pBuffer(&i_Buffer),
		m_sizeBuffer(i_sizeBuffer),
		m_currentOffset(i_startOffset)
	{
		assert(i_sizeBuffer);
		assert(i_startOffset < i_sizeBuffer);
	}

	DrawResourcesBuffer::~DrawResourcesBuffer()
	{
		SafeRelease_Checked(m_pBuffer);
	}

	DrawResourcesBuffer* DrawResourcesBuffer::Create(eUsage i_Usage, unsigned int i_sizeBytes, void* i_pInitialData, unsigned int i_sizeInitialData)
	{
		assert(i_sizeBytes);
		assert(g_pD3DDevice);

		static const D3D11_BIND_FLAG UsageMap[] =
		{
			D3D11_BIND_VERTEX_BUFFER,
			D3D11_BIND_INDEX_BUFFER
		};

		D3D11_BUFFER_DESC	desc;

		desc.ByteWidth = i_sizeBytes;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = UsageMap[size_t(i_Usage)];
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 1;

		ID3D11Buffer* pBuffer = nullptr;
		D3D11_SUBRESOURCE_DATA* pInitialData = nullptr;

		D3D11_SUBRESOURCE_DATA srd;

		if (i_pInitialData && i_sizeInitialData)
		{

			ZeroMemory(&srd, sizeof(srd));

			srd.pSysMem = i_pInitialData;
			srd.SysMemPitch = i_sizeInitialData;
			pInitialData = &srd;
		}
		else
			pInitialData = nullptr;

		HRESULT hResult = g_pD3DDevice->CreateBuffer(&desc, pInitialData, &pBuffer);
		assert((hResult == S_OK) || (hResult == E_OUTOFMEMORY));

		if (hResult == S_OK)
		{
			assert(pBuffer);
			return new DrawResourcesBuffer(*pBuffer, i_sizeBytes, i_sizeInitialData);
		}
		else
			return nullptr;
	}

	bool DrawResourcesBuffer::AddNewData(void* i_pData, unsigned int i_strideData, unsigned int i_sizeData, unsigned int& o_baseIndex)
	{
		unsigned int nextItem = NextItem(i_strideData);

		unsigned int endOffset = (nextItem * i_strideData) + i_sizeData;

		if (endOffset >= m_sizeBuffer)
			return false;

		D3D11_BOX	box = { m_currentOffset, 0, 0, endOffset, 1, 1 };

		GetImmediateContext()->UpdateSubresource(m_pBuffer, 0, &box, i_pData, 0, 0);

		m_currentOffset = endOffset;

		o_baseIndex = nextItem;

		return true;
	}

} // namespace GLib