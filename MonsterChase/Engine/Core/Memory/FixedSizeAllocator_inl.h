#pragma once
inline FixedSizeAllocator* FixedSizeAllocator::Create(FSAInitData i_fsa, HeapManager* i_pHeapManager)
{
	FixedSizeAllocator* FSA = static_cast<FixedSizeAllocator*>(alloc(i_pHeapManager, sizeof(FixedSizeAllocator)));
	FSA->Init(i_fsa, i_pHeapManager);
	return  FSA;
}
inline void FixedSizeAllocator::Init(FSAInitData i_fsa, HeapManager* i_pHeapManager)
{
	m_fsa = i_fsa;
	m_BitArray = BitArray::Create(m_fsa.numBlocks, i_pHeapManager);
	m_BasePtr = static_cast<uint8_t*>(alloc(i_pHeapManager, m_fsa.numBlocks*m_fsa.sizeBlock));
}
inline void FixedSizeAllocator::FSAfree(void* i_ptr)
{
	if (FSAContains(i_ptr))
	{
		size_t indexBit = static_cast<size_t>((static_cast<uint8_t*>(i_ptr) - m_BasePtr)) / m_fsa.sizeBlock;
		m_BitArray->ClearBit(indexBit);
	}
	else
	{
		#ifdef _DEBUG
			DEBUG_LOG("Invalid Free request fsa"<<this->m_fsa.sizeBlock<<" does not contain pointer - "<<i_ptr);
		#endif
	}
}
inline bool FixedSizeAllocator::FSAContains(void* i_ptr) const
{
	if (i_ptr >= m_BasePtr && i_ptr <= m_BasePtr + (m_fsa.sizeBlock*m_fsa.numBlocks))
	{
		return true;
	}
	else
	{
		return false;
	}
}