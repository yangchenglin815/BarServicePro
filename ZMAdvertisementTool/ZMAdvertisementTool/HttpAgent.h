#ifndef HTTPAGENT_H
#define HTTPAGENT_H

#include <QObject>
#include "HttpSignal.h"
#include "HttpStructData.h"

class HttpAgent : public QObject
{
	Q_OBJECT
public:
	static HttpAgent* instance();

	void RequestOnGetAdvertisementList();
	void RequestOnCommitStatisticalData(int delayTime);
private:
	explicit HttpAgent(QObject *parent = nullptr);
	~HttpAgent();

	void initConnect();
signals:
	void sigOnGetAdvertisementList(QList<AdvertisementInfo_t> advertisementInfoList, int nCode, QString sMsg);
	void sigOnCommitStatisticalData(int nCode, QString sMsg);
private:
	static HttpAgent* m_pInstance;
};
#endif // HTTPAGENT_H
