#pragma once

#include <d3d11.h>

#include "DrawResourcesBuffer.h"

namespace GLib
{
	class CPUAccessableBuffer : public DrawResourcesBuffer
	{
	public:
		enum eCPUAccess
		{
			CPU_ACCESS_READ = 0,
			CPU_ACCESS_WRITE,
			CPU_ACCESS_READ_WRITE
		};

		static CPUAccessableBuffer* Create(eUsage i_Usage, eCPUAccess i_Access, unsigned int i_sizeBytes, void* i_pInitialData, size_t i_sizeInitialData);
		bool AddNewData(void* i_pNewData, unsigned int i_strideData, unsigned int i_sizeData, unsigned int& o_baseIndex);

	private:
		CPUAccessableBuffer(ID3D11Buffer& i_Buffer, unsigned int i_sizeBuffer) : DrawResourcesBuffer(i_Buffer, i_sizeBuffer, 0) { }
	};

} // namespace GLib