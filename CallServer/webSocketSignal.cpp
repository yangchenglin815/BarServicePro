#include "webSocketSignal.h"

webSocketSignal* webSocketSignal::m_pInstance = NULL;

webSocketSignal *webSocketSignal::instance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new webSocketSignal;
	}
	return m_pInstance;
}

webSocketSignal::webSocketSignal(QObject *parent)
	: QObject(parent)
{

}

webSocketSignal::~webSocketSignal()
{

}
