#include "HttpClient.h"
#include "HttpGetLoginUserInfo.h"
#include "HttpGetClassifyGroup.h"
#include "HttpGetGoodsInfo.h"
#include "HttpGetWifiInfo.h"
#include "HttpCallService.h"
#include "HttpGetSignInList.h"
#include "HttpSignIn.h"
#include "HttpGetNetBagList.h"
#include "HttpNetFeeRecharge.h"

HttpClient* HttpClient::m_pInstance = NULL;

HttpClient *HttpClient::instance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new HttpClient;
	}
	return m_pInstance;
}

HttpClient::HttpClient(QObject *parent)
	: QObject(parent)
{
	m_pManager = new QNetworkAccessManager();
}

HttpClient::~HttpClient()
{

}

void HttpClient::setToken(QString sToken)
{
	m_sToken = sToken;
}

void HttpClient::RequestOnGetLoginUserInfo(QString sMac)
{
	HttpGetLoginUserInfo *p = new HttpGetLoginUserInfo(sMac);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnGetClassifyGroups(int nPage)
{
	HttpGetClassifyGroup *p = new HttpGetClassifyGroup(nPage);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnGetGoodsInfo(QString sName, int nPage)
{
	HttpGetGoodsInfo *p = new HttpGetGoodsInfo(sName, nPage);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnGetWifiInfo()
{
	HttpGetWifiInfo *p = new HttpGetWifiInfo();
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnCallService(LoginUserInfo userInfo)
{
	HttpCallService *p = new HttpCallService(userInfo);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnGetSignInList(LoginUserInfo userInfo)
{
	HttpGetSignInList *p = new HttpGetSignInList(userInfo);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnSignIn(LoginUserInfo userInfo)
{
	HttpSignIn *p = new HttpSignIn(userInfo);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnGetDiscountPkg(RequestPageType type)
{
	HttpGetNetBagList *p = new HttpGetNetBagList(type);
	p->setNetManager(m_pManager);
	p->postRequest();
}

void HttpClient::RequestOnNetFeeRecharge(LoginUserInfo_t userInfo, DiscountPkg_t discountPkg)
{
	HttpNetFeeRecharge *p = new HttpNetFeeRecharge(userInfo, discountPkg);
	p->setNetManager(m_pManager);
	p->postRequest();
}
