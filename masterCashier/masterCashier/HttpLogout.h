#ifndef HTTPLOGOUT_H
#define HTTPLOGOUT_H

#include <QObject>
#include "IHttpReqRes.h"

class HttpLogout : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpLogout(int type);
	~HttpLogout();
protected:
	void processResponse(QString strResult);
	void processError();
private:
	int m_type;
};

#endif // HTTPLOGOUT_H
