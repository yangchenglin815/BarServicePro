#ifndef ATTRIBUTESERVER_H
#define ATTRIBUTESERVER_H

#include <QObject>
#include <QByteArray>
#include <QList>
#include <QPair>
#include <QWebSocket>


QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class AttributeServer : public QObject
{
    Q_OBJECT

public:
    explicit AttributeServer(quint16 port, int frame_count = 10, bool debug = false, QObject *parent = Q_NULLPTR);
    ~AttributeServer();

Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void socketDisconnected();

    void onGetAttribute(QString age, QString sex, QString image);
private:
	void WSAPIHandler(int nCode);
private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
	QWebSocket* m_current_clients;
    bool m_debug;
    QPair<int, long> m_last_id;
    int m_frame_count;
};


#endif // ATTRIBUTESERVER_H
