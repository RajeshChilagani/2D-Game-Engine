#pragma once
#include "../Memory/HeapManager.h"
using namespace HeapManagerProxy;
#include <intrin.h>
#include <memory.h>
#include <stdint.h>
#ifdef WIN32
typedef uint32_t uintDyn;
#define UINTDYN_MAX UINT32_MAX
#define UINTDYN_MIN 0x00000000
#define BITSCAN _BitScanForward
#else
typedef uint64_t uintDyn;
#define UINTDYN_MAX UINT64_MAX
#define UINTDYN_MIN 0x0000000000000000
#define BITSCAN _BitScanForward64
#endif

class BitArray
{
public:
	static BitArray* Create(size_t i_numBits, HeapManager* i_pHeapManager,bool i_startClear = true);
	void Init(size_t i_numBits, HeapManager* i_pHeapManager,bool i_startClear = true);
	void Destroy(HeapManager* i_pHeapManager);
	void ClearAll(void);
	void SetAll(void);
	bool AreAllClear(void) const;
	bool AreAllSet(void) const;
	bool IsBitSet(size_t i_bitNumber) const;
	bool IsBitClear(size_t i_bitNumber) const;
	void SetBit(size_t bn);
	void ClearBit(size_t bn);
	bool GetFirstClearBit(size_t & o_bitNumber) const;
	bool GetFirstSetBit(size_t & o_bitNumber) const;

private:
	uintDyn*            m_pBits;
	size_t              bitsPerItem;
	size_t              m_numUintDyns;
	size_t              totalBits;
};

#include "BitArray_inl.h"