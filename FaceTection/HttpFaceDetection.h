#ifndef HTTPFACEDETECTION_H
#define HTTPFACEDETECTION_H

#include <QObject>
#include "IHttpReqRes.h"

class HttpFaceDetection : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpFaceDetection(QString filePath);
	~HttpFaceDetection();
protected:
	void processResponse(QString strResult);
	void processError();
private:
	
};

#endif // HTTPFACEDETECTION_H
