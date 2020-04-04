#pragma once
#include <d3d11.h>

namespace GLib
{
	inline bool Succeeded( HRESULT i_Result ) { return i_Result >= 0; }
} // namespace GLib