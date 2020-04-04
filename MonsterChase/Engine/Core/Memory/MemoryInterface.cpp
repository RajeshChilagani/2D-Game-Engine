#include "MemoryInterface.h"
#define FSAArraySize 3

HeapManager* pHeapManager;
FSAInitData fsaInitArray[FSAArraySize] = { { 16,100 } ,{ 32,200 },{ 96,400 } };
FixedSizeAllocator* fsaArray[FSAArraySize];
bool InitMemoryManager(void * i_pHeapMemory, size_t i_sizeHeapMemory, unsigned int i_OptionalNumDescriptors)
{
	pHeapManager = CreateHeapManager(i_pHeapMemory, i_sizeHeapMemory, i_OptionalNumDescriptors);
	for (int i = 0; i < FSAArraySize; ++i)
	{
		fsaArray[i] = FixedSizeAllocator::Create(fsaInitArray[i], pHeapManager);
	}
	return true;
}
 void DestroyMemoryManager()
{
	for (int i = 0; i < FSAArraySize; ++i)
	{
		if (fsaArray[i])
		{
			fsaArray[i]->Destroy(pHeapManager);
			free(pHeapManager, fsaArray[i]);
			fsaArray[i] = nullptr;
		}
	}
	if (pHeapManager)
	{
		Destroy(pHeapManager);
		pHeapManager = nullptr;
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
	FixedSizeAllocator* FSA = FindFixedSizeAllocator(i_size);

	if (FSA)
	{
		pReturn = FSA->FSAalloc();
	}

	if (pReturn == nullptr)
	{
		pReturn = alloc(pHeapManager, i_size);
	}
	if (pReturn == nullptr)
	{
		ICollect();
		pReturn = alloc(pHeapManager, i_size);
	}
	return pReturn;
}
 void* AllocateMemory(size_t i_size,unsigned int i_align)
{
	return alloc(pHeapManager, i_size, i_align);
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
	if (pHeapManager)
	{
		free(pHeapManager, i_ptr);
	}
}
 char* myStrdup(const char* src)
{
	char* dst = static_cast<char*>(malloc(strlen(src) + 1));
	if (dst == nullptr)
		return nullptr;
	memcpy(dst, src, strlen(src));
	dst[strlen(src)] = '\0';
	return dst;
}
 void ICollect()
{
	Collect(pHeapManager);
}