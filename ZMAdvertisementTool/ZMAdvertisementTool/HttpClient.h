#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include "HttpStructData.h"

class HttpClient : public QObject
{
	Q_OBJECT
public:
	static HttpClient* instance();
	void setToken(QString sToken);

	void RequestOnGetAdvertisementList();
	void RequestOnCommitStatisticalData(int delayTime);
private:
	explicit HttpClient(QObject *parent = nullptr);
	~HttpClient();
private:
	static HttpClient* m_pInstance;
	QString m_strTokenInfo;
	QNetworkAccessManager* m_pManager;
	QString m_sToken;
};

#endif // HTTPCLIENT_H
