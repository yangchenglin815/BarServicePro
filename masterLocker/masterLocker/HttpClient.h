#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>

class HttpClient : public QObject
{
	Q_OBJECT
public:
	static HttpClient* instance();
	void setToken(QString sToken);

	void RequestOnGetBookSeatStatus(QString nPcName, QString nBarId, QString nMac);
	void RequestOnGetVerifyUnlockCode(QString nPcName, QString nBarId, QString nMac, QString nCode);
	void RequestOnReportOnlineStatus(QString nPcName, QString nBarId, QString nMac, QString nStatus);
	void RequestOnGetBarName(QString nBarId);
	void RequestOnGetDownloadUrl();
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
