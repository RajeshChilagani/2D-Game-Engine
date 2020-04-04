#include "FixedSizeAllocator.h"

void* FixedSizeAllocator::FSAalloc()
{
	size_t firstAvailable;
	if (m_BitArray->GetFirstClearBit(firstAvailable))
	{
		m_BitArray->SetBit(firstAvailable);
		return m_BasePtr + (firstAvailable*m_fsa.sizeBlock);
	}
	else
	{
		return nullptr;
	}
}
void FixedSizeAllocator::Destroy(HeapManager* i_pHeapManager)
{
	#ifdef _DEBUG
		size_t firstSetBit;
		if (m_BitArray->GetFirstSetBit(firstSetBit))
		{
			DEBUG_LOG("Free all Outstanding Allocations in fsa"<<this->m_fsa.sizeBlock<<" before destructing\n ");
		}
	#endif
	m_BitArray->Destroy(i_pHeapManager);
	free(i_pHeapManager,m_BitArray);
	free(i_pHeapManager, m_BasePtr);
}