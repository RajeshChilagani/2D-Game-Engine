#pragma once
#include "SmartPtr.h"
template <typename T>
class WeakPtr
{
	friend SmartPtr<T>;
public:
	explicit WeakPtr():m_T(nullptr),m_RefCount(nullptr) {}
	WeakPtr(SmartPtr<T>& i_Other) :m_T(i_Other.m_T),m_RefCount(i_Other.m_RefCount)
	{
		if (m_RefCount)
		{
			m_RefCount->m_WeakPtrs++;
		}
		else
		{
			assert(m_T == nullptr);
		}
	}
	WeakPtr(const WeakPtr& i_Other) :m_T(i_Other.m_T), m_RefCount(i_Other.m_RefCount)
	{
		if (m_RefCount)
		{
			m_RefCount->m_WeakPtrs++;
		}
		else
		{
			assert(m_T == nullptr);
		}
	}
	WeakPtr& operator=(const WeakPtr& i_Rhs)
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
	bool operator==(const WeakPtr& i_Other)
	{
		return m_T == i_Other.m_T;
	}
	SmartPtr<T> Acquire()
	{
		return SmartPtr<T>(*this);
	}
	operator bool()
	{
		if (m_RefCount)
			return m_RefCount->m_SmartPtrs > 0;
		else
			return false;
	}
	~WeakPtr()
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
			if (--(m_RefCount->m_WeakPtrs) == 0 && (m_RefCount->m_SmartPtrs == 0))
			{
				delete m_RefCount;
			}
		}
	}
};
