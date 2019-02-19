#include "webSocketAgent.h"
#include "webSocketSignal.h"
#include "EventHandler.h"

using namespace dm::server;

webSocketAgent* webSocketAgent::m_pInstance = NULL;

webSocketAgent *webSocketAgent::instance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new webSocketAgent;
	}
	return m_pInstance;
}

webSocketAgent::webSocketAgent(QObject *parent)
	: QObject(parent)
{
	m_pServer = new AttributeServer(8089, 10, true, nullptr);
	initConnect();
}

webSocketAgent::~webSocketAgent()
{

}

void webSocketAgent::initConnect()
{
	connect(webSocketSignal::instance(), SIGNAL(sigOnWSRebootFunc()), this, SLOT(slotOnWSRebootFunc()));
	connect(webSocketSignal::instance(), SIGNAL(sigOnWSShutdownFunc()), this, SLOT(slotOnWSShutdownFunc()));
	connect(webSocketSignal::instance(), SIGNAL(sigOnWSLockScreenFunc()), this, SIGNAL(sigOnWSLockScreenFunc()));
	connect(webSocketSignal::instance(), SIGNAL(sigOnWSUnLockScreenFunc()), this, SIGNAL(sigOnWSUnLockScreenFunc()));
}

void webSocketAgent::slotOnWSRebootFunc()
{
	EventHandler::GetInstance().rebootClientHandler();
}

void webSocketAgent::slotOnWSShutdownFunc()
{
	EventHandler::GetInstance().shutdownClientHandler();
}