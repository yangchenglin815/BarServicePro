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
	connect(HttpSignal::instance(), SIGNAL(sigOnGetBookSeatStatus(int, QString, int)), this, SIGNAL(sigOnGetBookSeatStatus(int, QString, int)));
	connect(HttpSignal::instance(), SIGNAL(sigOnGetVerifyUnlockCode(int, QString)), this, SIGNAL(sigOnGetVerifyUnlockCode(int, QString)));
	connect(HttpSignal::instance(), SIGNAL(sigOnReportOnlineStatus(int, QString)), this, SIGNAL(sigOnReportOnlineStatus(int, QString)));
	connect(HttpSignal::instance(), SIGNAL(sigOnGetDownloadUrl(int, QString, QString)), this, SIGNAL(sigOnGetDownloadUrl(int, QString, QString)));
	connect(HttpSignal::instance(), SIGNAL(sigOnGetBarName(int, QString, QString)), this, SIGNAL(sigOnGetBarName(int, QString, QString)));
}

void HttpAgent::RequestOnGetBookSeatStatus(QString nPcName, QString nBarId, QString nMac)
{
	HttpClient::instance()->RequestOnGetBookSeatStatus(nPcName, nBarId, nMac);
}

void HttpAgent::RequestOnGetVerifyUnlockCode(QString nPcName, QString nBarId, QString nMac, QString nCode)
{
	HttpClient::instance()->RequestOnGetVerifyUnlockCode(nPcName, nBarId, nMac, nCode);
}

void HttpAgent::RequestOnReportOnlineStatus(QString nPcName, QString nBarId, QString nMac, QString nStatus)
{
	HttpClient::instance()->RequestOnReportOnlineStatus(nPcName, nBarId, nMac, nStatus);
}

void HttpAgent::RequestOnGetBarName(QString nBarId)
{
	HttpClient::instance()->RequestOnGetBarName(nBarId);
}

void HttpAgent::RequestOnGetDownloadUrl()
{
	HttpClient::instance()->RequestOnGetDownloadUrl();
}
