#ifndef HTTPAGENT_H
#define HTTPAGENT_H

#include <QObject>
#include "HttpSignal.h"

class HttpAgent : public QObject
{
	Q_OBJECT

public:
	static HttpAgent* instance();

	void RequestOnGetBarInfo(QString nBarId);
	void RequestOnLogin(QString nUser, QString nPasswd, int nType);
	void RequestOnLogout(int nType);
	void RequestOnGetClassifyGroups(QString nBarId, int nPage);
	void RequestOnGetGoodsInfo(QString nBarId, QString sName, int nPage);
	void RequestOnGetAllGoodsInfo();
	void RequestOnDealPayment(PaymentOrder orderInfo);
	void RequestOnGoodsCheckUp(int stockType, QList<GoodsCheckUpInfo_t> goodsCheckUpList, QString mark);
	void RequestOnGetShiftInfo(QString nEndTime);
	void RequestOnCommitShiftInfo(ShiftInfo_t tShiftInfo, QString sUser, QString sMark);
	void RequestOnCheckAcceptor(QString sPhone, QString sPwd);
	void RequestOnGetWarningGoodsList(QString sBarId);
	void RequestOnRefund(QString sBarId, QString sOrderId, QString sRemark);
	void RequestOnCancelOrder(QString sBarId, QString sOrderId, QString sPasswd);
	void RequestOnGetOrderDetialList(QString sSerialId);
	void RequestOnGetOrderList();
	void RequestOnFaceDetection(QString filePath);
	void RequestOnCommitFaceData(QString sIdCard, QString sName);
private:
	explicit HttpAgent(QObject *parent = 0);
	~HttpAgent();

	void initConnect();
signals:
	void sigOnGetBarInfo(int nCode, QString sMsg, QString nBarName);
	void sigOnLogin(int nCode, QString sMsg, QString sUser, QString nUserId);
	void sigOnLogout(int nCode, QString sMsg);
	void sigOnShiftLogout(int nCode, QString sMsg);
	void sigOnSwitch(int nCode, QString sMsg, QString sUser, QString nUserId);
	void sigOnShiftLogin(int nCode, QString sMsg, QString sUser, QString nUserId);
	void sigOnGetClassifyGroups(int nCode, QString sMsg, QList<ClassifyGroupInfo_t> classifyGroupList, int nPage);
	void sigOnGetGoodsInfo(int nCode, QString sMsg, QList<GoodsInfo_t> goodsInfoList);
	void sigOnGetReturnGoodsInfo(int nCode, QString sMsg, QList<GoodsInfo_t> goodsInfoList);
	void sigOnGetAllGoodsInfo(int nCode, QString sMsg, QList<GoodsInfo_t> goodsInfoList);
	void sigOnGetOrderResult(int nCode, QString sMsg, QString nOrderId, int nPayStatus);
	void sigOnGoodsCheckUpResult(int nCode, QString sMsg);
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
	static HttpAgent* m_pInstance;
};

#endif // HTTPAGENT_H
