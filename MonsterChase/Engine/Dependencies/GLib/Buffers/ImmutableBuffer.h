#pragma once

#include <d3d11.h>

#include "IGPUResourceBuffer.h"

namespace GLib
{
	class ImmutableBuffer : public IGPUResourceBuffer
	{
	public:
		static ImmutableBuffer* Create(eUsage i_Usage, void* i_pBufferData, unsigned int i_sizeBufferData);
		virtual ID3D11Buffer* GetDeviceBuffer() { return m_pBuffer; }
	private:
		ImmutableBuffer(ID3D11Buffer& i_Buffer);

		ID3D11Buffer* m_pBuffer;
	};

} // namespace GLib
