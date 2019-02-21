#ifndef HTTPCOMMITSTATISTICALDATA_H
#define HTTPCOMMITSTATISTICALDATA_H

#include <QObject>
#include "IHttpReqRes.h"
#include "appConfigIni.h"

class HttpCommitStatisticalData : public IHttpReqRes
{
	Q_OBJECT
public:
	explicit HttpCommitStatisticalData(int delayTime, QString mac = appConfigIni::getHostMacAddress(), QString computerName = appConfigIni::getLocalmachineName());
protected:
	void processResponse(QString strResult);
	void processError();
private:
	QByteArray createJsonData(int delayTime, QString mac, QString computerName);
};

#endif // HTTPCOMMITSTATISTICALDATA_H
