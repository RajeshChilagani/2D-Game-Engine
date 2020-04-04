#pragma once

namespace GLib
{

	template<class T>
	inline void SafeRelease(T*& i_ptr)
	{
		if (i_ptr)
		{
			i_ptr->Release();
			i_ptr = nullptr;
		}
	}

	template<class T>
	inline void SafeRelease_Checked(T*& i_ptr)
	{
		if (i_ptr)
		{
			ULONG refCount = i_ptr->Release();
			assert(refCount == 0);

			i_ptr = nullptr;
		}
	}

} // namespace GLib