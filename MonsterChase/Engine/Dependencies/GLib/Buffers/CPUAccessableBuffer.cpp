#include "CPUAccessableBuffer.h"

#include <assert.h>
#include <d3d11.h>
#include <stdint.h>

#include "Console\ConsolePrint.h"
#include "System\Device.h"

namespace GLib
{
	CPUAccessableBuffer* CPUAccessableBuffer::Create(IGPUResourceBuffer::eUsage i_Usage, eCPUAccess i_CPUAccess, unsigned int i_sizeBytes, void* i_pInitialData, size_t i_sizeInitialData)
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
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = UsageMap[size_t(i_Usage)];
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		switch (i_CPUAccess)
		{
		case CPU_ACCESS_READ:
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			break;
		case CPU_ACCESS_WRITE:
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			break;
		case CPU_ACCESS_READ_WRITE:
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
			break;
		}

		ID3D11Buffer* pBuffer = nullptr;

		HRESULT hResult = g_pD3DDevice->CreateBuffer(&desc, NULL, &pBuffer);
		assert((hResult == S_OK) || (hResult == E_OUTOFMEMORY));

		if (hResult == S_OK)
		{
			assert(pBuffer);
			return new CPUAccessableBuffer(*pBuffer, i_sizeBytes);
		}
		else
			return nullptr;
	}

	bool CPUAccessableBuffer::AddNewData( void * i_pData, unsigned int i_sizeData, unsigned int i_strideData, unsigned int & o_baseIndex )
	{
		unsigned int nextItem = NextItem( i_strideData );

		if( ( nextItem * i_strideData ) + i_sizeData >= m_sizeBuffer )
			return false;

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory( &mappedResource, sizeof( D3D11_MAPPED_SUBRESOURCE ) );
		
		SafeDeviceInterface<ID3D11DeviceContext> ImmediateContext = GetImmediateContext();

		HRESULT hResult = ImmediateContext->Map( m_pBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource );
		assert( hResult == S_OK );

		memcpy( reinterpret_cast<uint8_t *>( mappedResource.pData ) + ( nextItem * i_strideData ), i_pData, i_sizeData );

		ImmediateContext->Unmap( m_pBuffer, 0 );

		o_baseIndex = nextItem;

		return true;
	}

} // namespace GLib