namespace Engine
{
	namespace JobSystem
	{
		inline void SharedJobQueue::Shutdown()
		{
			EnterCriticalSection(&m_QueueAccess);
			m_ShutdownRequested = true;
			LeaveCriticalSection(&m_QueueAccess);

			WakeAllConditionVariable(&m_WakeAndCheck);
		}
		
	} // namespace JobSystem
} // namespace Engine