#include "HttpAgent.h"
#include "ThreadManager.h"
#include "HttpClient.h"
#include "HttpSignal.h"

HttpAgent* HttpAgent::m_pInstance = NULL;

HttpAgent *HttpAgent::instance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new HttpAgent;
	}
	return m_pInstance;
}

void HttpAgent::RequestOnGetLoginUserInfo(QString sMac)
{
	HttpClient::instance()->RequestOnGetLoginUserInfo(sMac);
}

void HttpAgent::RequestOnGetClassifyGroups(int nPage)
{
	HttpClient::instance()->RequestOnGetClassifyGroups(nPage);
}

void HttpAgent::RequestOnGetGoodsInfo(QString sName, int nPage)
{
	HttpClient::instance()->RequestOnGetGoodsInfo(sName, nPage);
}

void HttpAgent::RequestOnGetWifiInfo()
{
	HttpClient::instance()->RequestOnGetWifiInfo();
}

void HttpAgent::RequestOnCallService(LoginUserInfo userInfo)
{
	HttpClient::instance()->RequestOnCallService(userInfo);
}

void HttpAgent::RequestOnGetSignInList(LoginUserInfo userInfo)
{
	HttpClient::instance()->RequestOnGetSignInList(userInfo);
}

void HttpAgent::RequestOnSignIn(LoginUserInfo userInfo)
{
	HttpClient::instance()->RequestOnSignIn(userInfo);
}

void HttpAgent::RequestOnGetDiscountPkg(RequestPageType type)
{
	HttpClient::instance()->RequestOnGetDiscountPkg(type);
}

void HttpAgent::RequestOnNetFeeRecharge(LoginUserInfo_t userInfo, DiscountPkg_t discountPkg)
{
	HttpClient::instance()->RequestOnNetFeeRecharge(userInfo, discountPkg);
}

HttpAgent::HttpAgent(QObject *parent)
	: QObject(parent)
{
	initConnect();
	moveToThread(ThreadManager::getAgentThread());
	ThreadManager::getAgentThread()->start();
}

HttpAgent::~HttpAgent()
{

}

void HttpAgent::initConnect()
{
	qRegisterMetaType<LoginUserInfo>("LoginUserInfo");
	connect(HttpSignal::instance(), SIGNAL(sigOnGetLoginUserInfo(int, QString, LoginUserInfo)), this, SIGNAL(sigOnGetLoginUserInfo(int, QString, LoginUserInfo)));

	qRegisterMetaType<QList<ClassifyGroupInfo_t> >("QList<ClassifyGroupInfo_t>");
	connect(HttpSignal::instance(), SIGNAL(sigOnGetClassifyGroups(int, QString, QList<ClassifyGroupInfo_t>, int)), this, SIGNAL(sigOnGetClassifyGroups(int, QString, QList<ClassifyGroupInfo_t>, int)));

	qRegisterMetaType<QList<GoodsInfo_t> >("QList<GoodsInfo_t>");
	connect(HttpSignal::instance(), SIGNAL(sigOnGetGoodsInfo(int, QString, QList<GoodsInfo_t>)), this, SIGNAL(sigOnGetGoodsInfo(int, QString, QList<GoodsInfo_t>)));

	connect(HttpSignal::instance(), SIGNAL(sigOnGetWifiInfo(int, QString, QString, QString)), this, SIGNAL(sigOnGetWifiInfo(int, QString, QString, QString)));
	connect(HttpSignal::instance(), SIGNAL(sigOnCallService(int, QString)), this, SIGNAL(sigOnCallService(int, QString)));
	connect(HttpSignal::instance(), SIGNAL(sigOnSignIn(int, QString)), this, SIGNAL(sigOnSignIn(int, QString)));
	connect(HttpSignal::instance(), SIGNAL(sigOnNetFeeRecharge(int, QString, QString)), this, SIGNAL(sigOnNetFeeRecharge(int, QString, QString)));

	qRegisterMetaType<QList<QString> >("QList<QString>");
	connect(HttpSignal::instance(), SIGNAL(sigOnGetSignInList(int, QString, QList<QString>)), this, SIGNAL(sigOnGetSignInList(int, QString, QList<QString>)));

	qRegisterMetaType<QList<DiscountPkg_t> >("QList<DiscountPkg_t>");
	connect(HttpSignal::instance(), SIGNAL(sigOnGetDiscountPkgForRechargePage(int, QString, QList<DiscountPkg_t>)), this, SIGNAL(sigOnGetDiscountPkgForRechargePage(int, QString, QList<DiscountPkg_t>)));
	connect(HttpSignal::instance(), SIGNAL(sigOnGetDiscountPkgForPersongalCentrePage(int, QString, QList<DiscountPkg_t>)), this, SIGNAL(sigOnGetDiscountPkgForPersongalCentrePage(int, QString, QList<DiscountPkg_t>)));
}
