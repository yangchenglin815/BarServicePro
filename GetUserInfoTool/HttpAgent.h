#ifndef HTTPAGENT_H
#define HTTPAGENT_H

#include <QObject>
#include "HttpSignal.h"

class HttpAgent : public QObject
{
	Q_OBJECT

public:
	static HttpAgent* instance();

	void RequestReportPCInfo(ReportPCInfo_t info);
private:
	explicit HttpAgent(QObject *parent = 0);
	~HttpAgent();

	void initConnect();
signals:
	void sigOnReportPCInfoResult(int nCode, QString sMsg);
private:
	static HttpAgent* m_pInstance;
};

#endif // HTTPAGENT_H
