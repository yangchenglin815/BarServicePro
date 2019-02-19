#include "IHttpReqRes.h"
#include "commonFun.h"
#include <QHttpMultiPart>
#include <QApplication>
#include "logging.hpp"

IHttpReqRes::IHttpReqRes(QObject *parent)
	: QObject(parent)
	, m_nReconnectTimes(0)
	, m_nOverTimeLen(10)
	, m_bOverTime(false)
	, m_file(NULL)
{
	m_sUrlAddress = commonFun::getRequestUrl();
	for (int i = 0; i < 5; ++i)
	{
		m_fileGroup[i] = NULL;
	}
	initConnect();
}

IHttpReqRes::~IHttpReqRes()
{

}

void IHttpReqRes::setNetManager(QNetworkAccessManager* pManager)
{
	pManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
	m_pManager = pManager;
}

void IHttpReqRes::setReqParams(QByteArray byArr)
{
	byArr.replace("+", "%2B");
	byArr.replace("#", "%23");
	m_reqParams = byArr;
}

void IHttpReqRes::setFilePath(QString filePath)
{
	m_filePath = filePath;
}

void IHttpReqRes::postRequest()
{
	QNetworkRequest request;
	QSslConfiguration config;
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	config.setProtocol(QSsl::TlsV1SslV3);
	request.setSslConfiguration(config);
	//request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	request.setUrl(QUrl(m_sUrlAddress));
	m_pReply = m_pManager->post(request, m_reqParams);
	connect(m_pReply, SIGNAL(finished()), this, SLOT(slot_ReplyFinished()));
	m_sTestUrl = request.url().toString().append(m_reqParams); // ²âÊÔÓÃ
	if (!m_sTestUrl.isEmpty())
	{
		Log::Info("%s", m_sTestUrl.toStdString().c_str());
	}

	m_bOverTime = false;
	if (!m_overTimer.isActive())
	{
		m_overTimer.start(m_nOverTimeLen * 1000);
	}
}

void IHttpReqRes::postFaceTection()
{
	QNetworkRequest request;
	QSslConfiguration config;
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	config.setProtocol(QSsl::TlsV1SslV3);
	request.setSslConfiguration(config);

	QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
	QHttpPart imagePart;
	imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
	QString value = QString("form-data; name=\"file\"; filename=\"%1\"").arg(commonFun::getImageFileName(m_filePath));
	imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(value));
	m_file = new QFile(m_filePath);
	m_file->open(QIODevice::ReadOnly);
	imagePart.setBodyDevice(m_file);
	m_file->setParent(multiPart);
	multiPart->append(imagePart);

	request.setUrl(QUrl(m_sUrlAddress));
	m_pReply = m_pManager->post(request, multiPart);
	connect(m_pReply, SIGNAL(finished()), this, SLOT(slot_ReplyFinished()));
	m_sTestUrl = request.url().toString().append(m_reqParams); // ²âÊÔÓÃ
	if (!m_sTestUrl.isEmpty())
	{
		Log::Info("%s", m_sTestUrl.toStdString().c_str());
	}

	m_bOverTime = false;
	if (!m_overTimer.isActive())
	{
		m_overTimer.start(m_nOverTimeLen * 1000);
	}
}

void IHttpReqRes::postFaceData(QString sIdCard, QString sName)
{
	QNetworkRequest request;
	QSslConfiguration config;
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	config.setProtocol(QSsl::TlsV1SslV3);
	request.setSslConfiguration(config);

	QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

	QHttpPart idCardPart;
	idCardPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"idCard\""));
	idCardPart.setBody(sIdCard.toLocal8Bit());
	multiPart->append(idCardPart);

	QHttpPart namePart;
	namePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"personName\""));
	namePart.setBody(sName.toLocal8Bit());
	multiPart->append(namePart);

	QDir dir(QApplication::applicationDirPath() + "/image");
	dir.setFilter(QDir::Files);
	int fileCount = dir.count();
	for (int i = 0; i < fileCount; ++i)
	{
		QHttpPart imagePart;
		imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
		QString value = QString("form-data; name=\"files\"; filename=\"%1\"").arg(dir[i]);
		imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(value));
		m_fileGroup[i] = new QFile(QApplication::applicationDirPath() + "/image/" + dir[i]);
		m_fileGroup[i]->open(QIODevice::ReadOnly);
		imagePart.setBodyDevice(m_fileGroup[i]);
		m_fileGroup[i]->setParent(multiPart);
		multiPart->append(imagePart);
	}

	request.setUrl(QUrl(m_sUrlAddress));
	m_pReply = m_pManager->post(request, multiPart);
	connect(m_pReply, SIGNAL(finished()), this, SLOT(slot_ReplyFinished()));
	m_sTestUrl = request.url().toString().append(m_reqParams); // ²âÊÔÓÃ
	if (!m_sTestUrl.isEmpty())
	{
		Log::Info("%s", m_sTestUrl.toStdString().c_str());
	}

	m_bOverTime = false;
	if (!m_overTimer.isActive())
	{
		m_overTimer.start(m_nOverTimeLen * 1000);
	}
}

QString IHttpReqRes::getTestUrl()
{
	return m_sTestUrl;
}

void IHttpReqRes::setOverTimeLengh(int nOverTimeLengh)
{
	m_nOverTimeLen = nOverTimeLengh;
}

bool IHttpReqRes::isOverTime()
{
	return m_bOverTime;
}

void IHttpReqRes::setReqUrlAddress(QString urlAddress)
{
	m_sUrlAddress = urlAddress;
}

void IHttpReqRes::slot_ReplyFinished()
{
	m_bOverTime = false;
	m_overTimer.stop();

	if (m_file != NULL)
	{
		m_file->close();
		m_file->deleteLater();
		m_file = NULL;
	}

	for (int i = 0; i < 5; ++i)
	{
		if (m_fileGroup[i] != NULL)
		{
			m_fileGroup[i]->close();
			m_fileGroup[i]->deleteLater();
			m_fileGroup[i] = NULL;
		}
	}

	QTextCodec *codec = QTextCodec::codecForName("utf8");

	QNetworkReply::NetworkError err = m_pReply->error();

	if (err != QNetworkReply::NoError)
	{
		Log::Info("ÍøÂçÒì³£: %s; errorCod: %d; errorMsg: %s", m_sTestUrl.toStdString().c_str(), err, m_pReply->errorString().toStdString().c_str());
		processError();
	}
	else
	{
		m_nReconnectTimes = 0;
		QString strResult = codec->toUnicode(m_pReply->readAll());
		m_pReply->deleteLater();
		m_pReply = NULL;
		processResponse(strResult);
	}
}

void IHttpReqRes::slot_overTimeout()
{
	m_bOverTime = true;
	Log::Info("ÍøÂç³¬Ê±£º%s", m_sTestUrl.toStdString().c_str());
	processError();
}

void IHttpReqRes::initConnect()
{
	connect(&m_overTimer, SIGNAL(timeout()), this, SLOT(slot_overTimeout()));
}
