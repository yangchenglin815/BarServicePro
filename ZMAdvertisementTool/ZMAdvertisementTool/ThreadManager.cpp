#include "ThreadManager.h"

QThread* ThreadManager::m_pAgentThread = Q_NULLPTR;

QThread *ThreadManager::getAgentThread()
{
	if (m_pAgentThread == Q_NULLPTR) {
		m_pAgentThread = new QThread;
	}
	return m_pAgentThread;
}
