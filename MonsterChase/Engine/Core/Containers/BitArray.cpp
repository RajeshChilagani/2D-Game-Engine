#include "BitArray.h"
BitArray* BitArray::Create(size_t i_numBits, HeapManager* i_pHeapManager,bool i_startClear)
{
	BitArray* BA = static_cast<BitArray*>(alloc(i_pHeapManager,sizeof(BitArray)));
	BA->Init(i_numBits,i_pHeapManager,i_startClear);
	return BA;
}
void BitArray::Init(size_t i_numBits, HeapManager* i_pHeapManager,bool i_startClear)
{
	bitsPerItem = sizeof(uintDyn) * 8;
	m_numUintDyns = (i_numBits / bitsPerItem) + 1;
	totalBits = i_numBits;
	m_pBits = static_cast<uintDyn*>(alloc(i_pHeapManager,m_numUintDyns*sizeof(uintDyn)));
	memset(m_pBits, i_startClear ? 0 : 1, m_numUintDyns*(bitsPerItem / 8));
}
void BitArray::Destroy(HeapManager* i_pHeapManager)
{
	free(i_pHeapManager,this->m_pBits);
}
void BitArray::ClearAll(void)
{
	for (size_t i = 0; i < m_numUintDyns; i++)
	{
		m_pBits[i] = UINTDYN_MIN;
	}
}
void BitArray::SetAll(void)
{
	for (size_t i = 0; i < m_numUintDyns; i++)
	{
		m_pBits[i] = UINTDYN_MAX;

	}
}
bool BitArray::AreAllClear(void) const
{
	for (size_t i = 0; i < m_numUintDyns; i++)
	{
		if (!(m_pBits[i] == UINTDYN_MIN))
			return false;
	}
	return true;
}
bool BitArray::AreAllSet(void) const
{
	for (size_t i = 0; i < m_numUintDyns; i++)
	{
		if (!(m_pBits[i] == (UINTDYN_MAX)))
			return false;
	}
	return true;
}
bool BitArray::GetFirstClearBit(size_t & o_bitNumber) const
{
	size_t index = 0;
	while ((m_pBits[index] == UINTDYN_MAX) && (index < m_numUintDyns))
		index++;
	if (index < m_numUintDyns)
	{
		uintDyn bits = m_pBits[index];
		size_t bit;
		for (bit = 0; bit < bitsPerItem; ++bit)
		{
			if (!((bits >> bit) & static_cast<uintDyn>(1)))
				break;
		}
		o_bitNumber = index * bitsPerItem + bit;
		if (o_bitNumber >= 0 && o_bitNumber < totalBits)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

}
bool BitArray::GetFirstSetBit(size_t & o_bitNumber) const
{
	size_t index = 0;
	while ((m_pBits[index] == UINTDYN_MIN) && (index < m_numUintDyns))
		index++;
	if (index < m_numUintDyns)
	{
		uintDyn bits = m_pBits[index];
		unsigned long bit;
		BITSCAN(&bit, bits);
		/*for (bit = 0; bit < bitsPerItem; ++bit)
		{
			if (((bits >> bit) & static_cast<uintDyn>(1)))
				break;
		}*/
		o_bitNumber = index * bitsPerItem + bit;
		if (o_bitNumber >= 0 && o_bitNumber < totalBits)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

}