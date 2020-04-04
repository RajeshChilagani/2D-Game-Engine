#pragma once
#include "FixedSizeAllocator.h"

using namespace HeapManagerProxy;

enum NewAlignmemt
{
	NEW_ALIGN_DEFAULT,
	NEW_ALIGN_8 = 8,
	NEW_ALIGN_16 = 16,
	NEW_ALIGN_32 = 32
};
bool InitMemoryManager(void * i_pHeapMemory, size_t i_sizeHeapMemory, unsigned int i_OptionalNumDescriptors);
void DestroyMemoryManager();
void ICollect();
FixedSizeAllocator* FindFixedSizeAllocator(size_t i_size);
void* AllocateMemory(size_t i_size);
void* AllocateMemory(size_t i_size,unsigned int i_align);
void FreeMemory(void* i_ptr);
void* operator new(size_t i_size, NewAlignmemt i_align);
void operator delete(void* i_ptr, NewAlignmemt i_align);
