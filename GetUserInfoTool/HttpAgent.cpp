#include "HttpAgent.h"
#include "ThreadManager.h"
#include "HttpClient.h"
#include "HttpSignal.h"

HttpAgent* HttpAgent::m_pInstance = NULL;

HttpAgent *HttpAgent::instance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new HttpAgent;
	}
	return m_pInstance;
}

HttpAgent::HttpAgent(QObject *parent)
	: QObject(parent)
{
	initConnect();
	moveToThread(ThreadManager::getAgentThread());
	ThreadManager::getAgentThread()->start();
}

HttpAgent::~HttpAgent()
{

}

void HttpAgent::initConnect()
{
	connect(HttpSignal::instance(), SIGNAL(sigOnReportPCInfoResult(int, QString)), this, SIGNAL(sigOnReportPCInfoResult(int, QString)));
}

void HttpAgent::RequestReportPCInfo(ReportPCInfo_t info)
{
	HttpClient::instance()->RequestReportPCInfo(info);
}
