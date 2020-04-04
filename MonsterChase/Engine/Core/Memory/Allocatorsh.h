#pragma once
#include <inttypes.h>
_Ret_notnull_ _Post_writable_byte_size_(i_size)
_VCRT_ALLOCATOR void* __CRTDECL operator new (size_t i_size);

void operator delete(void * i_ptr);

_Ret_notnull_ _Post_writable_byte_size_(i_size)
_VCRT_ALLOCATOR void* __CRTDECL operator new[](size_t i_size);

void operator delete[](void * i_ptr);

inline void * operator new(size_t i_size, uint8_t* i_ptr);

namespace Engine
{
	void EnteringMain(void * i_pHeapMemory, size_t i_sizeHeapMemory);
	void LeavingMain();
}
