#ifndef HTTPCOMMITDATA_H
#define HTTPCOMMITDATA_H

#include <QObject>
#include "IHttpReqRes.h"

class HttpCommitData : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpCommitData(QString sIpAdress, QString sPort);
	~HttpCommitData();
protected:
	void processResponse(QString strResult);
	void processError();
private:
	QByteArray createJsonData(QString ipAdress, QString port);
};

#endif // HTTPCOMMITDATA_H
