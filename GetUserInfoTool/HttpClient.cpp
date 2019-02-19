#include "HttpClient.h"

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

void HttpClient::RequestReportPCInfo(ReportPCInfo_t info)
{
	HttpReportPCInfo *p = new HttpReportPCInfo(info);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::setToken(QString sToken)
{
	m_sToken = sToken;
}