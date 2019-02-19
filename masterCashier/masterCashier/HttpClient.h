#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include "httpStructData.h"

class HttpClient : public QObject
{
	Q_OBJECT
public:
	static HttpClient* instance();
	void setToken(QString sToken);

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
	explicit HttpClient(QObject *parent = NULL);
	~HttpClient();

private:
	static HttpClient* m_pInstance;
	QString m_strTokenInfo;
	QNetworkAccessManager* m_pManager;
	QString m_sToken;
};

#endif // HTTPCLIENT_H
