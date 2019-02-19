#ifndef HTTPSIGNAL_H
#define HTTPSIGNAL_H

#include <QObject>
#include "httpStructData.h"

class HttpSignal : public QObject
{
	Q_OBJECT
public:
	static HttpSignal* instance();
private:
	explicit HttpSignal(QObject *parent = 0);
	~HttpSignal();
signals:
	void sigOnGetBarInfo(int nCode,QString sMsg, QString barName);
	void sigOnLogin(int nCode, QString sMsg, QString sUser, QString nUserId);
	void sigOnLogout(int nCode, QString sMsg);
	void sigOnShiftLogout(int nCode, QString sMsg);
	void sigOnSwitch(int nCode, QString sMsg, QString sUser, QString nUserId);
	void sigOnShiftLogin(int nCode, QString sMsg, QString sUser, QString nUserId);
	void sigOnGetClassifyGroups(int nCode, QString sMsg, QList<ClassifyGroupInfo_t> classifyGroupList, int nPage);
	void sigOnGetGoodsInfo(int nCode, QString sMSg, QList<GoodsInfo_t> goodsInfoList);
	void sigOnGetReturnGoodsInfo(int nCode, QString sMSg, QList<GoodsInfo_t> goodsInfoList);
	void sigOnGetAllGoodsInfo(int nCode, QString sMSg, QList<GoodsInfo_t> goodsInfoList);
	void sigOnGetOrderResult(int nCode, QString sMsg, QString nOrderId, int nPayStatus);
	void sigOnGetGoodsCheckUpResult(int nCode, QString sMsg);
	void sigOnGetShiftInfo(int nCode, QString sMsg, ShiftInfo_t tShiftInfo);
	void sigOnCommitShiftInfoResult(int nCode, QString sMsg);
	void sigOnCheckAcceptorResult(int nCode, QString sMsg);
	void sigOnGetWarningGoodsList(int nCode, QString sMsg, QList<WarningGoods_t> lWarningGoodsList);
	void sigOnGetRefundRespond(int nCode, QString sMsg);
	void sigOnGetCancelOrderRespond(int nCode, QString sMsg);
	void sigOnGetOrderDetialList(int nCode, QString sMsg, QList<OrderDetialInfo_t> lOrderDetialInfoList);
	void sigOnGetOrderList(int nCode, QString sMsg, QList<OrderInfo_t> lOrderInfoList);
	void sigOnGetFaceDetectionRespond(int nCode, QString sMsg, int nRespondCode);
	void sigOnGetCommitFaceDataRespond(int nCode, QString sMsg, int nRespondCode);
private:
	static HttpSignal* m_pInstance;
};

#endif // HTTPSIGNAL_H
