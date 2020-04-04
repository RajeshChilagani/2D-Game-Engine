#pragma once
#include "../Containers/BitArray.h"
#include <iostream>

#if _DEBUG
#define DEBUG_LOG(msg)(std::cout<<"["<<__FILE__<<"]["<<__FUNCTION__<<"][Line "<<__LINE__<<"]"<<msg<<std::endl)
#endif

struct FSAInitData
{
	size_t              sizeBlock;
	size_t              numBlocks;
};
class FixedSizeAllocator
{
public:
	static FixedSizeAllocator* Create(FSAInitData i_fsa,HeapManager* i_pHeapManager);
	void Init(FSAInitData i_fsa, HeapManager* i_pHeapManager);
	void* FSAalloc();
	void FSAfree(void* i_ptr);
	bool FSAContains(void* i_ptr) const;
	void Destroy(HeapManager* i_pHeapManager);
private:
	BitArray*           m_BitArray;
	FSAInitData         m_fsa;
	uint8_t*            m_BasePtr;
};

#include "FixedSizeAllocator_inl.h"