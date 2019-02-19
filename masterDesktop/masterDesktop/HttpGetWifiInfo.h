#ifndef HTTPGETWIFIINFO_H
#define HTTPGETWIFIINFO_H

#include <QObject>
#include "IHttpReqRes.h"
#include "appConfigIni.h"

class HttpGetWifiInfo : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpGetWifiInfo(QString sBarId = appConfigIni::getBarId());
	~HttpGetWifiInfo();
protected:
	void processResponse(QString strResult);
	void processError();
};

#endif // HTTPGETWIFIINFO_H
