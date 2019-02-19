#include "HttpClient.h"
#include "HttpFaceDetection.h"
#include "HttpCommitFaceData.h"
#include "HttpCommitData.h"

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

void HttpClient::RequestOnFaceDetection(QString filePath)
{
	HttpFaceDetection *p = new HttpFaceDetection(filePath);
	p->setNetManager(m_pManager);
	p->postFaceTection();
}

void HttpClient::RequestOnCommitFaceData(QString sIdCard, QString sName)
{
	HttpCommitFaceData *p = new HttpCommitFaceData();
	p->setNetManager(m_pManager);
	p->postFaceData(sIdCard, sName);
}

void HttpClient::RequestOnCommitData(QString sIpAdress, QString sPort)
{
	HttpCommitData *p = new HttpCommitData(sIpAdress, sPort);
	p->setNetManager(m_pManager);
	p->postRequest();
}
