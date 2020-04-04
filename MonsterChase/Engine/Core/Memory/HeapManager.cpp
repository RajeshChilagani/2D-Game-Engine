#include "HeapManager.h"
#include<iostream>
namespace HeapManagerProxy
{
	//HeapManager Class
	HeapManager* HeapManager::create(void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors)
	{
		HeapManager* HM = static_cast<HeapManager*>(i_pMemory);
		void* HeapStart = static_cast<uint8_t*>(i_pMemory) + sizeof(HeapManager);
		size_t HeapSize = i_sizeMemory - sizeof(HeapManager);
		HM->init(HeapStart, HeapSize, i_numDescriptors);
		return HM;

	}
	void HeapManager::init(void* HeapStart, size_t HeapSize, unsigned int i_numDescriptors)
	{

		BlockDesc * Node = static_cast<BlockDesc*>(HeapStart);
		Node->m_pBlockStartAdr = static_cast<uint8_t*>(HeapStart) + sizeof(BlockDesc);
		Node->m_BlockSize = HeapSize - sizeof(BlockDesc);
		Node->m_next = this->FreeList;
		this->FreeList = Node;
		this->HeapSize = HeapSize;
		this->HeapStart = static_cast<uint8_t*>(HeapStart);
	}
	BlockDesc* HeapManager::addBlockDesc(void* baseAdd, size_t addSize)
	{
		BlockDesc * Node = static_cast<BlockDesc*>(baseAdd);
		Node->m_pBlockStartAdr = static_cast<uint8_t*>(baseAdd) + sizeof(BlockDesc);
		Node->m_BlockSize = addSize;
		memset(Node->m_pBlockStartAdr+Node->m_BlockSize-guardBand, 0xFD,guardBand);
		return Node;

	}
	void HeapManager::_insertsort(BlockDesc* i_BDptr)
	{
		if (this->FreeList !=nullptr)
		{
			BlockDesc* tempFree = this->FreeList;
			BlockDesc* prevFree = nullptr;
			while (tempFree != nullptr && tempFree < i_BDptr)
			{
				prevFree = tempFree;
				tempFree = tempFree->m_next;
			}
			if (tempFree == this->FreeList)
			{
				i_BDptr->m_next = this->FreeList;
				this->FreeList = i_BDptr;
			}
			else
			{
				i_BDptr->m_next = tempFree;
				prevFree->m_next = i_BDptr;
			}
		}
		else
		{
			i_BDptr->m_next = this->FreeList;
			this->FreeList = i_BDptr;
		}
	}
	void * HeapManager::_alloc(size_t i_size, unsigned int i_alignment)
	{
		BlockDesc* tempFreeList = this->FreeList;
		BlockDesc *prev=nullptr;
		i_size += guardBand;
		size_t alignment = 0;
		uint8_t* alignedPtr=nullptr;
		while (tempFreeList!=nullptr)
		{
			alignment=this->checkAlignment(tempFreeList->m_pBlockStartAdr + tempFreeList->m_BlockSize - i_size ,i_alignment);
			alignedPtr = tempFreeList->m_pBlockStartAdr + tempFreeList->m_BlockSize - i_size - alignment;
			if (alignedPtr - sizeof(BlockDesc) < tempFreeList->m_pBlockStartAdr)
			{
				prev = tempFreeList;
				tempFreeList = tempFreeList->m_next;
			}
			else
			{
				break;
			}
			
		}
		i_size += alignment;
		if (tempFreeList!=nullptr)
		{
			BlockDesc * N = this->addBlockDesc(alignedPtr - sizeof(BlockDesc), i_size);
			N->m_next = this->AllocatedList;
			this->AllocatedList = N;
			tempFreeList->m_BlockSize -= (sizeof(BlockDesc) + i_size);

			return this->AllocatedList->m_pBlockStartAdr;
		}
		else
		{
			std::cout << "No memory available for current request\n";
			return nullptr;
		}

	}
	bool HeapManager::_free(void* i_ptr)
	{
		BlockDesc* tempAllocated = this->AllocatedList;
		BlockDesc* prevAllocated = nullptr;
		while (tempAllocated != nullptr && tempAllocated->m_pBlockStartAdr!=i_ptr)
		{
			prevAllocated = tempAllocated;
			tempAllocated= tempAllocated->m_next;
		}
		if (tempAllocated!=nullptr)
		{
			if (*(tempAllocated->m_pBlockStartAdr+tempAllocated->m_BlockSize-guardBand) == 0xFD)
			{
				if (tempAllocated == this->AllocatedList)
				{
					this->AllocatedList = this->AllocatedList->m_next;
				}
				else
				{
					prevAllocated->m_next = tempAllocated->m_next;
				}
				_insertsort(tempAllocated);
				return true;
			}
			else
			{
				assert(*(tempAllocated->m_pBlockStartAdr + tempAllocated->m_BlockSize - guardBand) == 0xFD && "memory Access voilation");
				//std::cout << "memory Access voilation\n";
				return false;
			}
		}
		else
		{
			std::cout << "undefined pointer\n";
			return false;
		}
		
	}
	void* HeapManager::_realloc(void* i_ptr, size_t i_size,unsigned int i_alignment)
	{
		void* reallocptr= _alloc(i_size,i_alignment);
		if (reallocptr != nullptr)
		{
			BlockDesc* Bd = reinterpret_cast<BlockDesc*>(static_cast<uint8_t*>(i_ptr) - sizeof(BlockDesc));
			if (Bd->m_BlockSize > i_size)
			{
				memcpy(reallocptr, i_ptr, i_size);
			}
			else
			{
				memcpy(reallocptr, i_ptr, Bd->m_BlockSize);
			}
			
			_free(i_ptr);
			return reallocptr;
		}
		else
		{
			std::cout << "No memory available for current request\n";
			return nullptr;
		}
	}
	void HeapManager::_Collect()
	{
		BlockDesc* TempFree = this->FreeList;
		BlockDesc* NextFree = nullptr;
		while (TempFree!=nullptr)
		{
			while (TempFree->m_next != nullptr && TempFree->m_pBlockStartAdr + TempFree->m_BlockSize == reinterpret_cast<uint8_t*>(TempFree->m_next))
			{
				NextFree = TempFree->m_next;
				TempFree->m_BlockSize += (NextFree->m_BlockSize + sizeof(BlockDesc));
				TempFree->m_next = NextFree->m_next;
			}
			TempFree = TempFree->m_next;
		}
	}
	bool HeapManager::_IsAllocated(void * i_ptr) const
	{
		BlockDesc* tempAllocated = this->AllocatedList;
		while (tempAllocated != nullptr && tempAllocated->m_pBlockStartAdr != i_ptr)
		{
			tempAllocated = tempAllocated->m_next;
		}
		if (tempAllocated != nullptr)
			return true;
		else
			return false;
	}
	void HeapManager::_ShowFreeBlocks() const
	{
		BlockDesc* TempFree = this->FreeList;
		int count = 0;
		std::cout << "\nFreeList ";
		while (TempFree!=nullptr)
		{
			count++;
			std::cout << "Block " << count << " SA-" << static_cast<void*>(TempFree->m_pBlockStartAdr) << " Size-" << TempFree->m_BlockSize<<"->";
			TempFree = TempFree->m_next;
		}
	}
	void HeapManager::_ShowOutstandingAllocations() const
	{
		BlockDesc* TempAllocated = this->AllocatedList;
		int count = 0;
		std::cout << "\nAllocatedList ";
		while (TempAllocated != nullptr)
		{
			count++;
			std::cout << "Block " << count << " SA-" << static_cast<void*>(TempAllocated->m_pBlockStartAdr) << " Size-" << TempAllocated->m_BlockSize << "->";
			TempAllocated = TempAllocated->m_next;
		}
	}
	size_t HeapManager::getLargestFreeBlock() const
	{
		BlockDesc* TempFree = this->FreeList;
		size_t LargestFreeBlock = 0;
		if(TempFree!=nullptr)
			LargestFreeBlock = TempFree->m_BlockSize;
		while (TempFree!=nullptr)
		{
			
			if (LargestFreeBlock < TempFree->m_BlockSize)
			{
				LargestFreeBlock = TempFree->m_BlockSize;
			}
			TempFree = TempFree->m_next;
		}
		return LargestFreeBlock;
	}
	size_t HeapManager::getTotalFreeMemory() const
	{
		BlockDesc* TempFree = this->FreeList;
		size_t TotalFreeMemory = 0;
		while (TempFree != nullptr)
		{
			TotalFreeMemory += TempFree->m_BlockSize;
			TempFree = TempFree->m_next;
		}
		return TotalFreeMemory;
	}
}
