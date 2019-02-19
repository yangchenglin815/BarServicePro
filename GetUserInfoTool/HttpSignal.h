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
	void sigOnReportPCInfoResult(int nCode, QString sMsg);
private:
	static HttpSignal* m_pInstance;
};

#endif // HTTPSIGNAL_H
