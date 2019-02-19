#include "HttpClient.h"
#include "HttpGetFaceDetectionRespond.h"
#include "HttpFaceDetection.h"
#include "HttpCommitFaceData.h"

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

void HttpClient::RequestOnGetFaceDetectionRespond()
{
	HttpGetFaceDetectionRespond *p = new HttpGetFaceDetectionRespond();
	p->setNetManager(m_pManager);
	p->postSuperRequest();
}

void HttpClient::RequestOnFaceDetection(QString filePath)
{
	HttpFaceDetection *p = new HttpFaceDetection(filePath);
	p->setNetManager(m_pManager);
	p->post();
}

void HttpClient::RequestOnCommitFaceData(QString sIdCard, QString sName)
{
	HttpCommitFaceData *p = new HttpCommitFaceData();
	p->setNetManager(m_pManager);
	p->postFaceData(sIdCard, sName);
}

