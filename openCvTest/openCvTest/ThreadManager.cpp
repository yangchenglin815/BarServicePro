#include "ThreadManager.h"

QThread* ThreadManager::m_pAgentThread = NULL;

QThread *ThreadManager::getAgentThread()
{
	if (m_pAgentThread == NULL)
	{
		m_pAgentThread = new QThread;
	}
	return m_pAgentThread;
}
