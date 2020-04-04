#pragma once

#include <d3d11.h>

#include "IGPUResourceBuffer.h"

namespace GLib
{
	class DrawResourcesBuffer : public IGPUResourceBuffer
	{
	public:
		static DrawResourcesBuffer* Create(eUsage i_Usage, unsigned int i_sizeBytes, void* i_pInitialData, unsigned int i_sizeInitialData);
		~DrawResourcesBuffer();

		ID3D11Buffer* GetDeviceBuffer() { return m_pBuffer; }

		inline bool HasSpace(unsigned int i_strideData, unsigned int i_sizeData) const;
		virtual bool AddNewData(void* i_pNewData, unsigned int i_sizeData, unsigned int i_strideData, unsigned int& o_baseIndex);
	protected:
		DrawResourcesBuffer(ID3D11Buffer& i_Buffer, unsigned int i_sizeBuffer, unsigned int m_startOffset);

		inline unsigned int NextItem(unsigned int i_Stride) const;

		ID3D11Buffer* m_pBuffer;
		unsigned int m_sizeBuffer;
		unsigned int m_currentOffset;
	};
} // namespace GLib

#include "DrawResourcesBuffer-inl.h"