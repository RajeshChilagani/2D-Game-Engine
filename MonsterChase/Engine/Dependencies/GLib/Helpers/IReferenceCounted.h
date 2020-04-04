#pragma once

#include <Windows.h>

namespace GLib
{
	class IReferenceCounted
	{
	public:
		IReferenceCounted(unsigned long i_InitialRefCount = 0) :
			m_RefCount(i_InitialRefCount)
		{}
		virtual ~IReferenceCounted() {}

		unsigned long AddRef() 
		{ 
			return InterlockedIncrement(&m_RefCount);
		}

		unsigned long ReleaseRef() 
		{
			unsigned long NewRefCount = InterlockedDecrement(&m_RefCount);
			if ( NewRefCount == 0)
				delete this;

			return NewRefCount;
		}

	private:
		unsigned long m_RefCount;
	};
}