#include "IHttpReqRes.h"
#include "commonFun.h"
#include <QDebug>
#include <QApplication>
#include <QHttpMultiPart>

IHttpReqRes::IHttpReqRes(QObject *parent)
	: QObject(parent)
	, m_nReconnectTimes(0)
	, m_nOverTimeLen(10)
	, m_bOverTime(false)
{
	m_sUrlAddress = "";
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
		qDebug() << dir[i];
		QHttpPart imagePart;
		imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
		QString value = QString("form-data; name=\"files\"; filename=\"%1\"").arg(dir[i]);
		imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(value));
		QFile *file = new QFile(QApplication::applicationDirPath() + "/image/" + dir[i]);
		file->open(QIODevice::ReadOnly);
		imagePart.setBodyDevice(file);
		file->setParent(multiPart);
		multiPart->append(imagePart);
	}

	request.setUrl(QUrl(m_sUrlAddress));
	m_pReply = m_pManager->post(request, multiPart);
	connect(m_pReply, SIGNAL(finished()), this, SLOT(slot_ReplyFinished()));
	m_sTestUrl = request.url().toString().append(m_reqParams); // ≤‚ ‘”√
	if (!m_sTestUrl.isEmpty())
	{
		//AppConfigHandler.Info(m_sTestUrl.toStdString());
		qDebug() << m_sTestUrl;
	}

	m_bOverTime = false;
	if (!m_overTimer.isActive())
	{
		m_overTimer.start(m_nOverTimeLen * 1000);
	}
}

void IHttpReqRes::post()
{
	QNetworkRequest request;
	QSslConfiguration config;
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	config.setProtocol(QSsl::TlsV1SslV3);
	request.setSslConfiguration(config);
	//request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

	QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
	//multiPart->setBoundary("--------------acebdf13572468");
	QHttpPart imagePart;
	imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
	QString value = QString("form-data; name=\"file\"; filename=\"%1\"").arg(commonFun::getImageFileName(m_filePath));
	imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(value));
	QFile *file = new QFile(m_filePath);
	file->open(QIODevice::ReadOnly);
	imagePart.setBodyDevice(file);
	file->setParent(multiPart);
	multiPart->append(imagePart);


	request.setUrl(QUrl(m_sUrlAddress));
	m_pReply = m_pManager->post(request, multiPart);
	connect(m_pReply, SIGNAL(finished()), this, SLOT(slot_ReplyFinished()));
	m_sTestUrl = request.url().toString().append(m_reqParams); // ≤‚ ‘”√
	if (!m_sTestUrl.isEmpty())
	{
		//AppConfigHandler.Info(m_sTestUrl.toStdString());
		qDebug() << m_sTestUrl;
	}

	m_bOverTime = false;
	if (!m_overTimer.isActive())
	{
		m_overTimer.start(m_nOverTimeLen * 1000);
	}
}

void IHttpReqRes::postRequest()
{
	QNetworkRequest request;
	QSslConfiguration config;
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	config.setProtocol(QSsl::TlsV1SslV3);
	request.setSslConfiguration(config);
	//request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

	string appid = "1252670977";
	string secret_id = "AKIDiIix0wdW6TecP0DwLadUAh42BD7qnhsr";
	string secret_key = "xnZDkAQZWJuKm7II7kBSZTDuTs8DPTO6";
	QString sha1;// = QString::fromStdString(commonFun::hmac_sha1_func(appid, secret_id, secret_key));
	request.setRawHeader("Authorization", sha1.toLocal8Bit());
	request.setRawHeader("Content-Type", "application/json");

	request.setUrl(QUrl(m_sUrlAddress));
	m_pReply = m_pManager->post(request, m_reqParams);
	connect(m_pReply, SIGNAL(finished()), this, SLOT(slot_ReplyFinished()));
	m_sTestUrl = request.url().toString().append(m_reqParams); // ≤‚ ‘”√
	if (!m_sTestUrl.isEmpty())
	{
		//AppConfigHandler.Info(m_sTestUrl.toStdString());
		qDebug() << m_sTestUrl;
	}

	m_bOverTime = false;
	if (!m_overTimer.isActive())
	{
		m_overTimer.start(m_nOverTimeLen * 1000);
	}
}

void IHttpReqRes::postSuperRequest()
{
	QNetworkRequest request;
	QSslConfiguration config;
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	config.setProtocol(QSsl::TlsV1SslV3);
	request.setSslConfiguration(config);

	string appid = "1252670977";
	string secret_id = "AKIDkT1QgoxmX8Xntnn5bzCE7pOmLky1DcAY";
	string secret_key = "SkrnY8XlJm4CC82ykLm382lLky1ueEdr";
	QString sha1;// = QString::fromStdString(commonFun::hmac_sha1_func(appid, secret_id, secret_key));
	request.setRawHeader("authorization", sha1.toLocal8Bit());

	QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
	multiPart->setBoundary("--------------acebdf13572468");

	QHttpPart appidPart;
	appidPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"appid\""));
	appidPart.setBody("1252670977");

	QHttpPart modePart;
	modePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"mode\""));
	modePart.setBody("1");

	QHttpPart imagePart;
	imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
	imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"image\"; filename=\"test.jpg\""));
	QFile *file = new QFile(QApplication::applicationDirPath() + "\\test.jpg");
	file->open(QIODevice::ReadOnly);
	imagePart.setBodyDevice(file);
	file->setParent(multiPart);

	multiPart->append(appidPart);
	multiPart->append(modePart);
	multiPart->append(imagePart);

	request.setUrl(QUrl(m_sUrlAddress));
	m_pReply = m_pManager->post(request, multiPart);
	multiPart->setParent(m_pReply); // ”√ reply …æ≥˝ multiPart
	connect(m_pReply, SIGNAL(finished()), this, SLOT(slot_ReplyFinished()));
	m_sTestUrl = request.url().toString().append(m_reqParams); // ≤‚ ‘”√
	if (!m_sTestUrl.isEmpty())
	{
		//AppConfigHandler.Info(m_sTestUrl.toStdString());
		qDebug() << m_sTestUrl;
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

	QTextCodec *codec = QTextCodec::codecForName("utf8");

	QNetworkReply::NetworkError err = m_pReply->error();

	if (err != QNetworkReply::NoError)
	{
		//AppConfigHandler.Info(QStringLiteral("Õ¯¬Á“Ï≥£: %1; errorCod:%2; errorMsg:%3").arg(m_sTestUrl).arg(err).arg(m_pReply->errorString()).toStdString());
		qDebug() << QStringLiteral("Õ¯¬Á“Ï≥£: %1; errorCod:%2; errorMsg:%3").arg(m_sTestUrl).arg(err).arg(m_pReply->errorString());
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
	//AppConfigHandler.Info(QStringLiteral("Õ¯¬Á≥¨ ±£∫%1").arg(m_sTestUrl).toStdString());
	qDebug() << QStringLiteral("Õ¯¬Á≥¨ ±£∫%1").arg(m_sTestUrl);
	processError();
}

void IHttpReqRes::initConnect()
{
	connect(&m_overTimer, SIGNAL(timeout()), this, SLOT(slot_overTimeout()));
}
