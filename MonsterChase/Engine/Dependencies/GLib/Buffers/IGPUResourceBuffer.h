#pragma once

#include <d3d11.h>

namespace GLib
{
	class IGPUResourceBuffer
	{
	public:
		enum class eUsage
		{
			USAGE_VERTICES = 0,
			USAGE_INDICES
		};

		virtual ID3D11Buffer* GetDeviceBuffer() = 0;
	};
} // namespace GLib
