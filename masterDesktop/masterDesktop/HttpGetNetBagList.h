#ifndef HTTPGETNETBAGLIST_H
#define HTTPGETNETBAGLIST_H

#include <QObject>
#include "IHttpReqRes.h"
#include "appConfigIni.h"
#include "enumData.h"

class HttpGetNetBagList : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpGetNetBagList(RequestPageType type, QString sBarId = appConfigIni::getBarId());
	~HttpGetNetBagList();
protected:
	void processResponse(QString strResult);
	void processError();
private:
	RequestPageType m_pageType;
};

#endif // HTTPGETNETBAGLIST_H
