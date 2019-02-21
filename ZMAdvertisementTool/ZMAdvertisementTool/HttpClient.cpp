#include "HttpClient.h"
#include "HttpGetAdvertisementList.h"
#include "HttpCommitStatisticalData.h"

HttpClient* HttpClient::m_pInstance = Q_NULLPTR;

HttpClient *HttpClient::instance()
{
	if (m_pInstance == Q_NULLPTR) {
		m_pInstance = new HttpClient;
	}
	return m_pInstance;
}

void HttpClient::setToken(QString sToken)
{
	m_sToken = sToken;
}

void HttpClient::RequestOnGetAdvertisementList()
{
	HttpGetAdvertisementList *p = new HttpGetAdvertisementList();
	p->setNetManager(m_pManager);
	p->postJsonRequest();
}

void HttpClient::RequestOnCommitStatisticalData(int delayTime)
{
	HttpCommitStatisticalData *p = new HttpCommitStatisticalData(delayTime);
	p->setNetManager(m_pManager);
	p->postJsonRequest();
}

HttpClient::HttpClient(QObject *parent) : QObject(parent)
{
	m_pManager = new QNetworkAccessManager();
}

HttpClient::~HttpClient()
{

}
