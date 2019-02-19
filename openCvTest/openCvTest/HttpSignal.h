#ifndef HTTPSIGNAL_H
#define HTTPSIGNAL_H

#include <QObject>
#include "httpStructData.h"

class HttpSignal : public QObject
{
	Q_OBJECT
public:
	static HttpSignal* instance();
private:
	explicit HttpSignal(QObject *parent = 0);
	~HttpSignal();
signals:
	void sigOnGetFaceDetectionRespond(int nCode, QString sMsg, int nRespondCode);
	void sigOnGetCommitFaceDataRespond(int nCode, QString sMsg, int nRespondCode);
private:
	static HttpSignal* m_pInstance;
};

#endif // HTTPSIGNAL_H
