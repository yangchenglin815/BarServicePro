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
	explicit HttpClient(QObject *parent = NULL);
	~HttpClient();
private:
	static HttpClient* m_pInstance;
	QString m_strTokenInfo;
	QNetworkAccessManager* m_pManager;
	QString m_sToken;
};

#endif // HTTPCLIENT_H
