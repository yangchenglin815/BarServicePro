#ifndef HTTPGETCLASSIFYGROUP_H
#define HTTPGETCLASSIFYGROUP_H

#include <QObject>
#include "IHttpReqRes.h"

class HttpGetClassifyGroup : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpGetClassifyGroup(int nPage);
	~HttpGetClassifyGroup();
protected:
	void processResponse(QString strResult);
	void processError();
private:
	int m_page;
};

#endif // HTTPGETCLASSIFYGROUP_H
