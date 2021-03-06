#include "HttpAgent.h"
#include "ThreadManager.h"
#include "HttpClient.h"

HttpAgent* HttpAgent::m_pInstance = NULL;

HttpAgent *HttpAgent::instance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new HttpAgent;
	}
	return m_pInstance;
}

void HttpAgent::RequestOnGetFaceDetectionRespond()
{
	HttpClient::instance()->RequestOnGetFaceDetectionRespond();
}

void HttpAgent::RequestOnFaceDetection(QString filePath)
{
	HttpClient::instance()->RequestOnFaceDetection(filePath);
}

void HttpAgent::RequestOnCommitFaceData(QString sIdCard, QString sName)
{
	HttpClient::instance()->RequestOnCommitFaceData(sIdCard, sName);
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
	connect(HttpSignal::instance(), SIGNAL(sigOnGetFaceDetectionRespond(int, QString, int)), this, SIGNAL(sigOnGetFaceDetectionRespond(int, QString, int)));
	connect(HttpSignal::instance(), SIGNAL(sigOnGetCommitFaceDataRespond(int, QString, int)), this, SIGNAL(sigOnGetCommitFaceDataRespond(int, QString, int)));
}
