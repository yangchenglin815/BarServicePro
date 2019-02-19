#include "HttpClient.h"
#include "HttpGetBookSeatStatus.h"
#include "HttpVerifyUnlock.h"
#include "HttpGetOnlineStatus.h"
#include "HttpGetAppDownloadUrl.h"
#include "HttpGetBarName.h"

HttpClient* HttpClient::m_pInstance = NULL;

HttpClient *HttpClient::instance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new HttpClient;
	}
	return m_pInstance;
}

HttpClient::HttpClient(QObject *parent)
	: QObject(parent)
{
	m_pManager = new QNetworkAccessManager();
}

HttpClient::~HttpClient()
{

}

void HttpClient::setToken(QString sToken)
{
	m_sToken = sToken;
}

void HttpClient::RequestOnGetBookSeatStatus(QString nPcName, QString nBarId, QString nMac)
{
	HttpGetBookSeatStatus *p = new HttpGetBookSeatStatus(nPcName, nBarId, nMac);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnGetVerifyUnlockCode(QString nPcName, QString nBarId, QString nMac, QString nCode)
{
	HttpVerifyUnlock *p = new HttpVerifyUnlock(nPcName, nBarId, nMac, nCode);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnReportOnlineStatus(QString nPcName, QString nBarId, QString nMac, QString nStatus)
{
	HttpGetOnlineStatus *p = new HttpGetOnlineStatus(nPcName, nBarId, nMac, nStatus);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnGetBarName(QString nBarId)
{
	HttpGetBarName *p = new HttpGetBarName(nBarId);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnGetDownloadUrl()
{
	HttpGetAppDownloadUrl *p = new HttpGetAppDownloadUrl("getDownloadUrl");
	p->setNetManager(m_pManager);
	p->postRequest();
}
