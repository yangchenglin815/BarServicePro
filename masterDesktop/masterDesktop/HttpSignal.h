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
	void sigOnGetLoginUserInfo(int nCode, QString sMsg, LoginUserInfo loginUserInfo);
	void sigOnGetClassifyGroups(int nCode, QString sMsg, QList<ClassifyGroupInfo_t> classifyGroupList, int nPage);
	void sigOnGetGoodsInfo(int nCode, QString sMSg, QList<GoodsInfo_t> goodsInfoList);
	void sigOnGetWifiInfo(int nCode, QString sMsg, QString sWifiName, QString sWifiKey);
	void sigOnCallService(int nCode, QString sMsg);
	void sigOnGetSignInList(int nCode, QString sMsg, QList<QString> lSignInList);
	void sigOnSignIn(int nCode, QString sMsg);
	void sigOnGetDiscountPkgForRechargePage(int nCode, QString sMsg, QList<DiscountPkg_t> lDiscountPkgList);
	void sigOnGetDiscountPkgForPersongalCentrePage(int nCode, QString sMsg, QList<DiscountPkg_t> lDiscountPkgList);
	void sigOnNetFeeRecharge(int nCode, QString sMsg, QString sFeeCode);
private:
	static HttpSignal* m_pInstance;
};

#endif // HTTPSIGNAL_H
