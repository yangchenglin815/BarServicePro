#ifndef HTTPGETFACEDETECTIONRESPOND_H
#define HTTPGETFACEDETECTIONRESPOND_H

#include <QObject>
#include "IHttpReqRes.h"

class HttpGetFaceDetectionRespond : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpGetFaceDetectionRespond();
	~HttpGetFaceDetectionRespond();
protected:
	void processResponse(QString strResult);
	void processError();
private:
	
};

#endif // HTTPGETFACEDETECTIONRESPOND_H
