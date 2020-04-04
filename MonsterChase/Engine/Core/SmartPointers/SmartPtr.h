#pragma once
#include <assert.h>
template <typename T>
class WeakPtr;
struct RefCount
{
	int m_SmartPtrs;
	int m_WeakPtrs;
	RefCount(int i_SmartPtrs, int i_WeakPtrs) :m_SmartPtrs(i_SmartPtrs), m_WeakPtrs(i_WeakPtrs) {}
};
template <typename T>
class SmartPtr
{
	friend WeakPtr<T>;
public:
	explicit SmartPtr() {}
	explicit SmartPtr(T* i_T) :m_T(i_T), m_RefCount(i_T?new RefCount(1, 0):nullptr) {}
	explicit SmartPtr(WeakPtr<T>& i_WeakPtr) : m_T(i_WeakPtr.m_RefCount && i_WeakPtr.m_RefCount->m_SmartPtrs > 0 ? i_WeakPtr.m_T : nullptr), m_RefCount(i_WeakPtr.m_RefCount ? i_WeakPtr.m_RefCount : nullptr)
	{
		if (m_RefCount)
		{
			/*if (m_RefCount->m_SmartPtrs == 0)
			{
				m_T = nullptr;
				m_RefCount = nullptr;
			}
			else
			{
				(m_RefCount->m_SmartPtrs)++;
			}*/
			m_RefCount->m_SmartPtrs++;
		}
		
	}
	//Copy
	SmartPtr(const SmartPtr& i_Other) :m_T(i_Other.m_T), m_RefCount(i_Other.m_RefCount)
	{
		if (m_RefCount)
		{
			assert(m_T != nullptr);
			m_RefCount->m_SmartPtrs++;
		}
		else
		{
			assert(m_T == nullptr);
		}
			
	}
	//Move
	SmartPtr(SmartPtr&& i_Other) noexcept :m_T(i_Other.m_T), m_RefCount(i_Other.m_RefCount)
	{
		i_Other.m_T = nullptr;
		i_Other.m_RefCount = nullptr;
	}
	SmartPtr& operator=(const SmartPtr& i_Rhs)
	{
		if (this != &i_Rhs)
		{
			Release();
			m_T = i_Rhs.m_T;
			m_RefCount = i_Rhs.m_RefCount;
			(m_RefCount->m_SmartPtrs)++;
		}
		return *this;
	}
	SmartPtr& operator=(std::nullptr_t i_nullptr)
	{
		if (this != i_nullptr)
		{
			Release();
			m_T = nullptr;
			m_RefCount = nullptr;
		}
		*this;
	}
	T* operator->()
	{
		return m_T;
	}
	T& operator*()
	{
		return *(m_T);
	}
	operator bool()
	{
		return m_T != nullptr;
	}
	bool operator==(const SmartPtr& i_Other)
	{
		return m_T == i_Other.m_T;
	}
	~SmartPtr()
	{
		Release();
	}
private:
	T* m_T;
	RefCount* m_RefCount;
	void Release()
	{
		if (m_RefCount)
		{
			if (--(m_RefCount->m_SmartPtrs) == 0)
			{
				delete m_T;
				m_T = nullptr;
				if (m_RefCount->m_WeakPtrs == 0)
				{
					delete m_RefCount;
					m_RefCount = nullptr;
				}
			}
			
		}
	}
};
