#include "webSocketAgent.h"
#include "webSocketSignal.h"
#include "CallServerIni.h"
#include "handleEvent.h"
#include "APIProtocol.h"

using namespace dm::server;

webSocketAgent* webSocketAgent::m_pInstance = NULL;

webSocketAgent *webSocketAgent::instance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new webSocketAgent;
	}
	return m_pInstance;
}

webSocketAgent::webSocketAgent(QObject *parent)
	: QObject(parent)
{
	m_pServer = new AttributeServer(CallServerIni::getWebPort().toInt(), 10, true, nullptr);
	initConnect();
}

webSocketAgent::~webSocketAgent()
{

}

void webSocketAgent::initConnect()
{
	connect(webSocketSignal::instance(), SIGNAL(sigOnCallAdmin(QString, int)), this, SLOT(slotOnCallAdmin(QString, int)));
	connect(webSocketSignal::instance(), SIGNAL(sigOnCallService(QString, int)), this, SLOT(slotOnCallService(QString, int)));
	connect(webSocketSignal::instance(), SIGNAL(sigOnCallOffLine(QString, int)), this, SLOT(slotOnCallOffLine(QString, int)));
	connect(webSocketSignal::instance(), SIGNAL(sigOnCallOrder(QString, int)), this, SLOT(slotOnCallOrder(QString, int)));
	connect(webSocketSignal::instance(), SIGNAL(sigOnCallSeat(QString, int)), this, SLOT(slotOnCallSeat(QString, int)));
}

void webSocketAgent::slotOnCallAdmin(QString nSeatNo, int nCycleNum)
{
	api_boardcast_info_t api_reg;
	memcpy(api_reg.nSeatNo, nSeatNo.toStdString().c_str(), nSeatNo.length());
	api_reg.nPlayCycleNum = nCycleNum;
	api_reg.nBoardCastType = CALL_MANAGEMENT;

	handleEvent::instance()->addBoardCastListItem(api_reg);
}

void webSocketAgent::slotOnCallService(QString nSeatNo, int nCycleNum)
{
	api_boardcast_info_t api_reg;
	memcpy(api_reg.nSeatNo, nSeatNo.toStdString().c_str(), nSeatNo.length());
	api_reg.nPlayCycleNum = nCycleNum;
	api_reg.nBoardCastType = CALL_SERVICE;

	handleEvent::instance()->addBoardCastListItem(api_reg);
}

void webSocketAgent::slotOnCallOffLine(QString nSeatNo, int nCycleNum)
{
	api_boardcast_info_t api_reg;
	memcpy(api_reg.nSeatNo, nSeatNo.toStdString().c_str(), nSeatNo.length());
	api_reg.nPlayCycleNum = nCycleNum;
	api_reg.nBoardCastType = DEV_OFFLINE;

	handleEvent::instance()->addBoardCastListItem(api_reg);
}

void webSocketAgent::slotOnCallOrder(QString nSeatNo, int nCycleNum)
{
	api_boardcast_info_t api_reg;
	memcpy(api_reg.nSeatNo, nSeatNo.toStdString().c_str(), nSeatNo.length());
	api_reg.nPlayCycleNum = nCycleNum;
	api_reg.nBoardCastType = OFFER_FOODS;

	handleEvent::instance()->addBoardCastListItem(api_reg);
}

void webSocketAgent::slotOnCallSeat(QString nSeatNo, int nCycleNum)
{
	api_boardcast_info_t api_reg;
	memcpy(api_reg.nSeatNo, nSeatNo.toStdString().c_str(), nSeatNo.length());
	api_reg.nPlayCycleNum = nCycleNum;
	api_reg.nBoardCastType = CALL_SEAT;

	handleEvent::instance()->addBoardCastListItem(api_reg);
}
