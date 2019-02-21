#include "HttpAgent.h"
#include "HttpClient.h"
#include "ThreadManager.h"

HttpAgent* HttpAgent::m_pInstance = Q_NULLPTR;

HttpAgent *HttpAgent::instance()
{
	if (m_pInstance == Q_NULLPTR) {
		m_pInstance = new HttpAgent;
	}
	return m_pInstance;
}

void HttpAgent::RequestOnGetAdvertisementList()
{
	HttpClient::instance()->RequestOnGetAdvertisementList();
}

void HttpAgent::RequestOnCommitStatisticalData(int delayTime)
{
	HttpClient::instance()->RequestOnCommitStatisticalData(delayTime);
}

HttpAgent::HttpAgent(QObject *parent) : QObject(parent)
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
	qRegisterMetaType<QList<AdvertisementInfo_t> >("QList<AdvertisementInfo_t>");
	connect(HttpSignal::instance(), SIGNAL(sigOnGetAdvertisementList(QList<AdvertisementInfo_t>, int, QString)), this, SIGNAL(sigOnGetAdvertisementList(QList<AdvertisementInfo_t>, int, QString)));
	connect(HttpSignal::instance(), SIGNAL(sigOnCommitStatisticalData(int, QString)), this, SIGNAL(sigOnCommitStatisticalData(int, QString)));
}
