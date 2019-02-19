#include "webSocketAgent.h"
#include "webSocketSignal.h"
#include "appConfigIni.h"

webSocketAgent* webSocketAgent::m_pInstance = NULL;

webSocketAgent *webSocketAgent::instance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new webSocketAgent;
	}
	return m_pInstance;
}

void webSocketAgent::initServer()
{
	m_pServer = new AttributeServer(appConfigIni::getWebPort().toInt(), 10, true, nullptr);
}

webSocketAgent::webSocketAgent(QObject *parent)
	: QObject(parent)
{
	initConnect();
}

webSocketAgent::~webSocketAgent()
{

}

void webSocketAgent::initConnect()
{
	connect(webSocketSignal::instance(), SIGNAL(sigOnFaceTection(QString, QString)), this, SIGNAL(sigOnFaceTection(QString, QString)));
}
