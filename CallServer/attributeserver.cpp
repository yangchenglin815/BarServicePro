#include "attributeserver.h"

#include <iostream>
#include <algorithm>
#include <string>

#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>
#include <QDebug>
#include <QFile>
#include <QMutex>
#include <QWaitCondition>
#include <QScriptEngine>
#include <QScriptValueIterator>

#include "webSocketSignal.h"
#include "logging.hpp"
#include "appconfig.h"
#include "enumData.h"

using namespace dm::server;

QMutex g_mutex;
QWaitCondition g_condition;

std::vector<std::string> g_vecImages;

AttributeServer::AttributeServer(quint16 port, int frame_cont, bool bdebug, QObject *parent)
    : QObject(parent),
    m_frame_count(frame_cont),
    m_debug(bdebug),
    m_pWebSocketServer(new QWebSocketServer("Attribute Server", QWebSocketServer::NonSecureMode, this)),
    m_clients()
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        if (m_debug) {
			//AppConfigHandler.Info("Attribute Server listening ip " + to_string(QHostAddress::Any) + ", on port " + to_string(port));
			Log::Info("Attribute Server listening ip %d, on port %d", QHostAddress::Any, port);
        }
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
            this, &AttributeServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed,
            this, &AttributeServer::closed);
    }
    m_last_id.first = -1;
    m_last_id.second = 0;

    m_current_clients = NULL;
}

AttributeServer::~AttributeServer()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void AttributeServer::onNewConnection()
{
	Log::Info("new  connection");
	//AppConfigHandler.Info("new  connection");
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &AttributeServer::processTextMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &AttributeServer::socketDisconnected);

    quint16 sizes = pSocket->readBufferSize();
    pSocket->setReadBufferSize(640 * 480);

    m_current_clients = pSocket;
    m_clients << pSocket;
}

bool startWith(std::string src, std::string substr)
{
    if (src.empty() || src.size() < substr.size()) {
        return false;
    }

    bool bflag = true;
    for (int i = 0; i < substr.size(); i++){
        if (src[i] != substr[i]) {
            bflag = false;
            break;
        }
    }

    return bflag;
}

void AttributeServer::processTextMessage(QString message)
{
    static long count = 0;

    std::string curr_message = message.toStdString();
    bool bflag = startWith(curr_message, "data:image/jpeg;");
	QString strResult = QString::fromStdString(curr_message);
	Log::Info("receive data : %s", strResult.toStdString().c_str());
	//AppConfigHandler.Info("receive data : " + strResult.toStdString());

	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("data").property("cmd").toInt32();

	if (!sc.property("code").isValid())
	{
		Log::Info("code is null !");
		//AppConfigHandler.Info("code is null !");
		return;
	}

	QString seatNo = sc.property("data").property("seatNum").toString();
	int cycleNum = sc.property("data").property("cycleNum").toInt32();
	WSAPIHandler(nCode, seatNo, cycleNum);
}

void AttributeServer::socketDisconnected()
{

    QWebSocket *pClient = qobject_cast<QWebSocket*>(sender());

    if (m_debug) {
        qDebug() << "socketDisconnected: " << pClient;
    }
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

void AttributeServer::onGetAttribute(QString age, QString sex, QString image)
{
    QString str_send_info = QString("{\"age\":\"%1\",\"sex\":\"%2\",\"image\":\"%3\"}")
        .arg(age)
        .arg(sex)
        .arg(image);

    QByteArray postData = str_send_info.toUtf8();
    static long count = 0;
    qDebug() << "begin send attribute result " << count++;
    m_current_clients->sendTextMessage(postData);
    m_current_clients->flush();
}

void AttributeServer::WSAPIHandler(int nCode, QString nSeatNo, int nCycleNum)
{
	switch (nCode)
	{
	case w_sCallAdmin:
		//AppConfigHandler.Info("WSAPIHandler : w_sCallAdmin");
		Log::Info("WSAPIHandler : w_sCallAdmin");
		webSocketSignal::instance()->sigOnCallAdmin(nSeatNo, nCycleNum);
		break;
	case w_sCallService:
		//AppConfigHandler.Info("WSAPIHandler : w_sCallService");
		Log::Info("WSAPIHandler : w_sCallService");
		webSocketSignal::instance()->sigOnCallService(nSeatNo, nCycleNum);
		break;
	case w_sCallOffLine:
		//AppConfigHandler.Info("WSAPIHandler : w_sCallOffLine");
		Log::Info("WSAPIHandler : w_sCallOffLine");
		webSocketSignal::instance()->sigOnCallOffLine(nSeatNo, nCycleNum);
		break;
	case w_sCallOrder:
		//AppConfigHandler.Info("WSAPIHandler : w_sCallOrder");
		Log::Info("WSAPIHandler : w_sCallOrder");
		webSocketSignal::instance()->sigOnCallOrder(nSeatNo, nCycleNum);
		break;
	case w_sCallSeat:
		//AppConfigHandler.Info("WSAPIHandler : w_sCallSeat");
		Log::Info("WSAPIHandler : w_sCallSeat");
		webSocketSignal::instance()->sigOnCallSeat(nSeatNo, nCycleNum);
		break;
	default:
		break;
	}
}
