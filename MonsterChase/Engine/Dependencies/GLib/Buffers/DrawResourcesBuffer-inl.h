#pragma once

namespace GLib
{
	inline unsigned int DrawResourcesBuffer::NextItem(unsigned int i_Stride) const
	{
		unsigned int roundedUp = m_currentOffset + (i_Stride - 1) / i_Stride;
		return roundedUp / i_Stride;
	}

	inline bool DrawResourcesBuffer::HasSpace(unsigned int i_strideData, unsigned int i_sizeData) const
	{
		return ((NextItem(i_strideData) * i_strideData) + i_sizeData) <= m_sizeBuffer;
	}

} // namespace GLib
