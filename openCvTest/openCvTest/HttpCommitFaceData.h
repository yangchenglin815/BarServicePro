#ifndef HTTPCOMMITFACEDATA_H
#define HTTPCOMMITFACEDATA_H

#include <QObject>
#include "IHttpReqRes.h"

class HttpCommitFaceData : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpCommitFaceData();
	~HttpCommitFaceData();
protected:
	void processResponse(QString strResult);
	void processError();
private:
	
};

#endif // HTTPCOMMITFACEDATA_H
