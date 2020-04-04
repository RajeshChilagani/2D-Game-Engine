#pragma once
inline bool BitArray::IsBitSet(size_t i_bitNumber) const
{
	size_t index = i_bitNumber / bitsPerItem;
	size_t bitToCheck = i_bitNumber - index * bitsPerItem;
	if (((m_pBits[index] >> bitToCheck) & static_cast<uintDyn>(1)))
		return true;
	return false;

}
inline bool BitArray::IsBitClear(size_t i_bitNumber) const
{
	size_t index = i_bitNumber / bitsPerItem;
	size_t bitToCheck = i_bitNumber - index * bitsPerItem;
	if (!((m_pBits[index] >> bitToCheck) & static_cast<uintDyn>(1)))
		return true;
	return false;
	
}
inline void BitArray::SetBit(size_t i_bitNumber)
{
	if (i_bitNumber >= 0 && i_bitNumber < totalBits)
	{
		size_t index = i_bitNumber / bitsPerItem;
		size_t bitToSet = i_bitNumber - index * bitsPerItem;
		m_pBits[index] |= static_cast<uintDyn>(1) << bitToSet;
	}
}
inline void BitArray::ClearBit(size_t i_bitNumber)
{
	if (i_bitNumber >= 0 && i_bitNumber < totalBits)
	{
		size_t index = i_bitNumber / bitsPerItem;
		size_t bitToSet = i_bitNumber - index * bitsPerItem;
		m_pBits[index] &= ~(static_cast<uintDyn>(1) << bitToSet);
	}

}
