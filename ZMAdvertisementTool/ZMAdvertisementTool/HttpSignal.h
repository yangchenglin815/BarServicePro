#ifndef HTTPSIGNAL_H
#define HTTPSIGNAL_H

#include <QObject>
#include "HttpStructData.h"

class HttpSignal : public QObject
{
	Q_OBJECT
public:
	static HttpSignal* instance();
private:
	explicit HttpSignal(QObject *parent = nullptr);
	~HttpSignal();
signals:
	void sigOnGetAdvertisementList(QList<AdvertisementInfo_t> advertisementInfoList, int nCode, QString sMsg);
	void sigOnCommitStatisticalData(int nCode, QString sMsg);
private:
	static HttpSignal* m_pInstance;
};

#endif // HTTPSIGNAL_H
