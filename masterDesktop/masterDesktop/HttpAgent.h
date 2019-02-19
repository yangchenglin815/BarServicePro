#ifndef HTTPAGENT_H
#define HTTPAGENT_H

#include <QObject>
#include "HttpSignal.h"

class HttpAgent : public QObject
{
	Q_OBJECT

public:
	static HttpAgent* instance();

	void RequestOnGetLoginUserInfo(QString sMac);
	void RequestOnGetClassifyGroups(int nPage);
	void RequestOnGetGoodsInfo(QString sName, int nPage);
	void RequestOnGetWifiInfo();
	void RequestOnCallService(LoginUserInfo userInfo);
	void RequestOnGetSignInList(LoginUserInfo userInfo);
	void RequestOnSignIn(LoginUserInfo userInfo);
	void RequestOnGetDiscountPkg(RequestPageType type);
	void RequestOnNetFeeRecharge(LoginUserInfo_t userInfo, DiscountPkg_t discountPkg);
private:
	explicit HttpAgent(QObject *parent = 0);
	~HttpAgent();

	void initConnect();
signals:
	void sigOnGetLoginUserInfo(int nCode, QString sMsg, LoginUserInfo loginUserInfo);
	void sigOnGetClassifyGroups(int nCode, QString sMsg, QList<ClassifyGroupInfo_t> classifyGroupList, int nPage);
	void sigOnGetGoodsInfo(int nCode, QString sMsg, QList<GoodsInfo_t> goodsInfoList);
	void sigOnGetWifiInfo(int nCode, QString sMsg, QString sWifiName, QString sWifiKey);
	void sigOnCallService(int nCode, QString sMsg);
	void sigOnGetSignInList(int nCode, QString sMsg, QList<QString> lSignInDateList);
	void sigOnSignIn(int nCode, QString sMsg);
	void sigOnGetDiscountPkgForRechargePage(int nCode, QString sMsg, QList<DiscountPkg_t> lDiscountPkgList);
	void sigOnGetDiscountPkgForPersongalCentrePage(int nCode, QString sMsg, QList<DiscountPkg_t> lDiscountPkgList);
	void sigOnNetFeeRecharge(int nCode, QString sMsg, QString sFeeCode);
private:
	static HttpAgent* m_pInstance;
};

#endif // HTTPAGENT_H
