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
#include "enumData.h"
#include "appConfigIni.h"
#include "logging.hpp"

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

	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();

	if (!sc.property("code").isValid())
	{
		Log::Info("code is null !");
		return;
	}

	if (nCode == 0)
	{
		QString cmd = sc.property("data").property("cmd").toString();
		if (cmd == "10012")
		{
			//推送开卡人信息
			QString idcard = sc.property("data").property("idCard").toString();
			QString name = sc.property("data").property("userName").toString();
			webSocketSignal::instance()->sigOnFaceTection(idcard, name);
		}
	}
}

void AttributeServer::socketDisconnected()
{

    QWebSocket *pClient = qobject_cast<QWebSocket*>(sender());
	Log::Info("%s", pClient->errorString().toStdString().data());

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
	
	default:
		break;
	}
}
