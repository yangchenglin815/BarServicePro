#ifndef HTTPSIGNAL_H
#define HTTPSIGNAL_H

#include <QObject>

class HttpSignal : public QObject
{
	Q_OBJECT
public:
	static HttpSignal* instance();
private:
	explicit HttpSignal(QObject *parent = 0);
	~HttpSignal();
signals:
	void sigOnGetBookSeatStatus(int nCode, QString sMsg, int nStatus);
	void sigOnGetVerifyUnlockCode(int nCode, QString sMsg);
	void sigOnReportOnlineStatus(int nCode, QString sMsg);
	void sigOnGetDownloadUrl(int nCode, QString sMsg, QString nUrl);
	void sigOnGetBarName(int nCode, QString sMsg, QString nBarName);
private:
	static HttpSignal* m_pInstance;
};

#endif // HTTPSIGNAL_H
