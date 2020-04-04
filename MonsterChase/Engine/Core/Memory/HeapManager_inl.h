#pragma once
namespace HeapManagerProxy
{
	inline void HeapManager::destroy()
	{
		this->FreeList = nullptr;
		this->AllocatedList = nullptr;
	}
	inline size_t HeapManager::checkAlignment(uint8_t* i_ptr, unsigned int i_alignment)
	{
		if (reinterpret_cast<size_t>(i_ptr) % i_alignment)
			return reinterpret_cast<size_t>(i_ptr) % i_alignment;
		return 0;
	}
	inline bool HeapManager::_Contains(void * i_ptr) const
	{
		if (i_ptr > this->HeapStart && i_ptr < (this->HeapStart + this->HeapSize))
			return true;
		else
			return false;
	}
	//HeapManager
	inline HeapManager* CreateHeapManager(void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors)
	{
		return HeapManager::create(i_pMemory, i_sizeMemory, i_numDescriptors);
	}
	inline void Destroy(HeapManager * i_pManager)
	{
		assert(i_pManager);
		i_pManager->destroy();
	}
	inline void * alloc(HeapManager * i_pManager, size_t i_size)
	{
		assert(i_pManager);
		return i_pManager->_alloc(i_size);
	}
	inline void * alloc(HeapManager * i_pManager, size_t i_size, unsigned int i_alignment)
	{
		assert(i_pManager);
		return i_pManager->_alloc(i_size, i_alignment);
	}
	inline void * realloc(HeapManager * i_pManager, void* i_ptr, size_t i_size)
	{
		assert(i_pManager);
		return i_pManager->_realloc(i_ptr,i_size);
	}
	inline bool free(HeapManager * i_pManager, void * i_ptr)
	{
		assert(i_pManager);
		return i_pManager->_free(i_ptr);
	}
	inline void Collect(HeapManager * i_pManager)
	{
		assert(i_pManager);
		i_pManager->_Collect();
	}
	inline bool Contains(const HeapManager * i_pManager, void * i_ptr)
	{
		assert(i_pManager);
		return i_pManager->_Contains(i_ptr);
	}
	inline bool IsAllocated(const HeapManager * i_pManager, void * i_ptr)
	{
		assert(i_pManager);
		return i_pManager->_IsAllocated(i_ptr);
	}
	inline size_t GetLargestFreeBlock(const HeapManager * i_pManager)
	{
		assert(i_pManager);
		return i_pManager->getLargestFreeBlock();
	}
	inline size_t GetTotalFreeMemory(const HeapManager * i_pManager)
	{
		assert(i_pManager);
		return i_pManager->getTotalFreeMemory();
	}
	inline void ShowFreeBlocks(const HeapManager * i_pManager)
	{
		assert(i_pManager);
		i_pManager->_ShowFreeBlocks();
	}
	inline void ShowOutstandingAllocations(const HeapManager * i_pManager)
	{
		assert(i_pManager);
		i_pManager->_ShowOutstandingAllocations();
	}
}

