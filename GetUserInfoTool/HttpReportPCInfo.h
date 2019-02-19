#ifndef HTTPREPORTPCINFO_H
#define HTTPREPORTPCINFO_H

#include <QObject>
#include "IHttpReqRes.h"
#include "httpStructData.h"

class HttpReportPCInfo : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpReportPCInfo(const ReportPCInfo_t &info);
	~HttpReportPCInfo();
protected:
	void processResponse(QString strResult);
	void processError();
private:
	ReportPCInfo_t m_info;
};

#endif // HTTPREPORTPCINFO_H
