#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include "httpStructData.h"

class HttpClient : public QObject
{
	Q_OBJECT
public:
	static HttpClient* instance();
	void setToken(QString sToken);
	 
	void RequestOnGetFaceDetectionRespond();
	void RequestOnFaceDetection(QString filePath);
	void RequestOnCommitFaceData(QString sIdCard, QString sName);
private:
	explicit HttpClient(QObject *parent = NULL);
	~HttpClient();

private:
	static HttpClient* m_pInstance;
	QString m_strTokenInfo;
	QNetworkAccessManager* m_pManager;
	QString m_sToken;
};

#endif // HTTPCLIENT_H
