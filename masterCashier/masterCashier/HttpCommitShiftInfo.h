#ifndef HTTPCOMMITSHIFTINFO_H
#define HTTPCOMMITSHIFTINFO_H

#include <QObject>
#include "IHttpReqRes.h"
#include "httpStructData.h"

class HttpCommitShiftInfo : public IHttpReqRes
{
	Q_OBJECT

public:
	explicit HttpCommitShiftInfo(const ShiftInfo_t &tShiftInfo, QString sUser, QString sMark);
	~HttpCommitShiftInfo();
protected:
	void processResponse(QString strResult);
	void processError();
private:
	QString createSalesInfoJson(const ShiftInfo_t &info);
	QString createGoodsInfoJson(const ShiftInfo_t &info);
	QString createOthersInfoJson(const ShiftInfo_t &info);
private:
	ShiftInfo_t m_shiftInfo;
};

#endif // HTTPCOMMITSHIFTINFO_H
