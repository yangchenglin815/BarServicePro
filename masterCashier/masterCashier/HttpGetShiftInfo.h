#ifndef HTTPGETSHIFTINFO_H
#define HTTPGETSHIFTINFO_H

#include <QObject>
#include "IHttpReqRes.h"

class HttpGetShiftInfo : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpGetShiftInfo(QString endTime);
	~HttpGetShiftInfo();
protected:
	void processResponse(QString strResult);
	void processError();
};

#endif // HTTPGETSHIFTINFO_H
