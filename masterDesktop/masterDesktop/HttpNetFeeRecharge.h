#ifndef HTTPNETFEERECHARGE_H
#define HTTPNETFEERECHARGE_H

#include <QObject>
#include "IHttpReqRes.h"
#include "httpStructData.h"
#include "appConfigIni.h"

class HttpNetFeeRecharge : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpNetFeeRecharge(LoginUserInfo_t userInfo, DiscountPkg_t discountPkg, QString sBarId = appConfigIni::getBarId());
	~HttpNetFeeRecharge();
protected:
	void processResponse(QString strResult);
	void processError();
};

#endif // HTTPNETFEERECHARGE_H
