#ifndef HTTPAGENT_H
#define HTTPAGENT_H

#include <QObject>

class HttpAgent : public QObject
{
	Q_OBJECT

public:
	static HttpAgent* instance();

	void RequestOnGetBookSeatStatus(QString nPcName, QString nBarId, QString nMac);
	void RequestOnGetVerifyUnlockCode(QString nPcName, QString nBarId, QString nMac, QString nCode);
	void RequestOnReportOnlineStatus(QString nPcName, QString nBarId, QString nMac, QString nStatus);
	void RequestOnGetBarName(QString nBarId);
	void RequestOnGetDownloadUrl();
private:
	explicit HttpAgent(QObject *parent = 0);
	~HttpAgent();

	void initConnect();
signals:
	void sigOnGetBookSeatStatus(int nCode, QString sMsg, int nStatus);
	void sigOnGetVerifyUnlockCode(int nCode, QString sMsg);
	void sigOnReportOnlineStatus(int nCode, QString sMsg);
	void sigOnGetDownloadUrl(int nCode, QString sMsg, QString nUrl);
	void sigOnGetBarName(int nCode, QString sMsg, QString nBarName);
private:
	static HttpAgent* m_pInstance;
};

#endif // HTTPAGENT_H
