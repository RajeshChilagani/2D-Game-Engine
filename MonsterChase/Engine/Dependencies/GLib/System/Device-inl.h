#pragma once

namespace GLib
{
	template<class T>
	class SafeDeviceInterface
	{
	public:
		SafeDeviceInterface(T& i_Interface, CRITICAL_SECTION& i_CS) :
			m_pInterface(&i_Interface),
			m_pCS(&i_CS)
		{
			m_pInterface->AddRef();
			EnterCriticalSection(m_pCS);
		}

		SafeDeviceInterface(SafeDeviceInterface& i_Other)
		{
			m_pInterface = i_Other.m_pInterface;
			m_pCS = i_Other.m_pCS;

			i_Other.m_pInterface = nullptr;
			i_Other.m_pCS = nullptr;
		}

		~SafeDeviceInterface()
		{
			if (m_pInterface)
				m_pInterface->Release();
			if (m_pCS)
				LeaveCriticalSection(m_pCS);
		}

		T* operator->()
		{
			assert(m_pInterface);
			return m_pInterface;
		}

		operator T*()
		{
			assert(m_pInterface);
			return m_pInterface;
		}
	private:
		T* m_pInterface;
		CRITICAL_SECTION* m_pCS;
	};

} // namespace GLib
