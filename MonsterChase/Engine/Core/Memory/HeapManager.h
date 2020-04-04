#pragma once
#include <stdint.h>
#include <assert.h>


namespace HeapManagerProxy
{
	struct BlockDesc
	{
		uint8_t*           m_pBlockStartAdr;
		size_t             m_BlockSize;
		BlockDesc*         m_next;
	};
	class HeapManager
	{

	public:
		static HeapManager* create(void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors);
		void destroy();
		void init(void* HeapStart, size_t HeapSize, unsigned int i_numDescriptors);
		void* _alloc(size_t i_size, unsigned int i_alignment=4);
		void* _realloc(void * i_ptr, size_t i_size, unsigned int i_alignment = 4);
		bool _free(void* i_ptr);
		void _insertsort(BlockDesc* i_BDptr);
		void _Collect();
		bool _IsAllocated(void * i_ptr) const;
		bool _Contains(void * i_ptr) const;
		void _ShowFreeBlocks() const;
		void _ShowOutstandingAllocations() const;
		size_t getLargestFreeBlock() const;
		size_t getTotalFreeMemory() const;
		BlockDesc* addBlockDesc(void* baseAdd, size_t addSize);
		size_t checkAlignment(uint8_t* i_ptr, unsigned int i_alignment);
		static const size_t guardBand = 4;
		static const int s_MinumumToLeave=sizeof(BlockDesc)+guardBand;
	private:
		BlockDesc *          FreeList = nullptr;
		BlockDesc *          AllocatedList = nullptr;
		uint8_t*             HeapStart = nullptr;
		size_t               HeapSize;
	};
	//Heap Manager Class
	HeapManager *	CreateHeapManager(void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors=0);
	void			Destroy(HeapManager * i_pManager);
	void *			alloc(HeapManager * i_pManager, size_t i_size);
	void *          alloc(HeapManager * i_pManager, size_t i_size, unsigned int i_alignment);
	void *			realloc(HeapManager * i_pManager, void* i_ptr,size_t i_size);
	bool			free(HeapManager * i_pManager, void * i_ptr);
	void			ShowFreeBlocks(const HeapManager * i_pManager);
	void			ShowOutstandingAllocations(const HeapManager * i_pManager);
	void			Collect(HeapManager * i_pManager);
	bool			Contains(const HeapManager * i_pManager, void * i_ptr);
	bool			IsAllocated(const HeapManager * i_pManager, void * i_ptr);
	size_t			GetLargestFreeBlock(const HeapManager * i_pManager);
	size_t			GetTotalFreeMemory(const HeapManager * i_pManager);
}
//HeapManager namespace

#include "HeapManager_inl.h"
