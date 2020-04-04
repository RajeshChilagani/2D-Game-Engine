#include "Allocatorsh.h"
#include "FixedSizeAllocator.h"
#include "Core/JobSystem/Syncronization/Mutex.h"
#include "Core/JobSystem/Syncronization/ScopeLock.h"
#include <assert.h>
#include <Windows.h>
#define FSAArraySize 3

namespace Engine
{
	 Mutex MemoryMutex;

	HeapManager*        GlobalHeap=nullptr;
	const size_t 		GlobalHeapSize = 1024 * 1024 * 2;
	void* pHeapMemory = HeapAlloc(GetProcessHeap(), 0, GlobalHeapSize);

	HeapManager* DefaultHeap = nullptr;

	FSAInitData fsaInitArray[FSAArraySize] = { { 16,100 } ,{ 32,200 },{ 96,400 } };
	FixedSizeAllocator* fsaArray[FSAArraySize];

	void EnteringMain(void * i_pHeapMemory, size_t i_sizeHeapMemory)
	{
		DefaultHeap= CreateHeapManager(i_pHeapMemory, i_sizeHeapMemory);
		for (int i = 0; i < FSAArraySize; ++i)
		{
			fsaArray[i] = FixedSizeAllocator::Create(fsaInitArray[i], DefaultHeap);
		}
	}
	void LeavingMain()
	{
		if (DefaultHeap)
		{
			for (int i = 0; i < FSAArraySize; ++i)
			{
				if (fsaArray[i])
				{
					fsaArray[i]->Destroy(DefaultHeap);
					free(DefaultHeap, fsaArray[i]);
					fsaArray[i] = nullptr;
				}
			}

			Destroy(DefaultHeap);
			DefaultHeap = nullptr;
		}
	
	}
	FixedSizeAllocator* FindFixedSizeAllocator(size_t i_size)
	{
		if (i_size > 0 && i_size <= 16)
		{
			std::cout << "fsa16 \n";
			return fsaArray[0];
		}
		else if (i_size > 16 && i_size <= 32)
		{
			std::cout << "fsa32 \n";
			return fsaArray[1];
		}
		else if (i_size > 32 && i_size <= 96)
		{
			std::cout << "fsa96 \n";
			return fsaArray[2];
		}
		return nullptr;
	}
	void* AllocateMemory(size_t i_size)
	{
		void* pReturn = nullptr;
		if (DefaultHeap == nullptr)
		{
			if (GlobalHeap == nullptr)
			{
				GlobalHeap = CreateHeapManager(pHeapMemory, GlobalHeapSize);
				pReturn = alloc(GlobalHeap, i_size);
			}
			else
			{
				pReturn =alloc(GlobalHeap, i_size);
			}
		}
		else
		{
			FixedSizeAllocator* FSA = FindFixedSizeAllocator(i_size);

			if (FSA)
			{
				pReturn = FSA->FSAalloc();
			}

			if (pReturn == nullptr)
			{
				pReturn = alloc(DefaultHeap, i_size);
			}
			if (pReturn == nullptr)
			{
				Collect(DefaultHeap);
				pReturn = alloc(DefaultHeap, i_size);
			}
		}
				
		return pReturn;
	}
	void FreeMemory(void* i_ptr)
	{
		int i = 0;
		while (i < FSAArraySize)
		{
			if (fsaArray[i] && fsaArray[i]->FSAContains(i_ptr))
			{
				fsaArray[i]->FSAfree(i_ptr);
				return;
			}
			i++;
		}
		if (DefaultHeap && DefaultHeap->_Contains(i_ptr))
		{
			free(DefaultHeap, i_ptr);
		}
		else if (GlobalHeap)
		{
			free(GlobalHeap, i_ptr);
		}
	}
	
}

_Ret_notnull_ _Post_writable_byte_size_(i_size)
_VCRT_ALLOCATOR void* __CRTDECL operator new (size_t i_size)
{
	Engine::ScopeLock Lock(Engine::MemoryMutex);
	printf("calling new(i_size ) for allocation: %zu\n", i_size);
	return Engine::AllocateMemory(i_size);
}

void operator delete(void * i_ptr)
{
	Engine::ScopeLock Lock(Engine::MemoryMutex);
	printf("delete 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
	return Engine::FreeMemory(i_ptr);

}

_Ret_notnull_ _Post_writable_byte_size_(i_size)
_VCRT_ALLOCATOR void* __CRTDECL operator new[](size_t i_size)
{
	Engine::ScopeLock Lock(Engine::MemoryMutex);
	printf("calling []new(i_size ) for allocation: %zu\n", i_size);
	return Engine::AllocateMemory(i_size);
}

void operator delete [](void * i_ptr)
{
	Engine::ScopeLock Lock(Engine::MemoryMutex);
	printf("delete [] 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
	return Engine::FreeMemory(i_ptr);
}
//Placement new
inline void * operator new(size_t i_size, uint8_t* i_ptr)
{
	Engine::ScopeLock Lock(Engine::MemoryMutex);
	std::cout << "Calling new(size_t i_size, uint8_t* i_ptr) for " << "\n";
	return i_ptr;
}
