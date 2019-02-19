#ifndef WEBSOCKETAGENT_H
#define WEBSOCKETAGENT_H

#include <QObject>
#include "attributeserver.h"

class webSocketAgent : public QObject
{
	Q_OBJECT

public:
	static webSocketAgent* instance();
private:
	explicit webSocketAgent(QObject *parent = 0);
	~webSocketAgent();

	void initConnect();
private slots:
void slotOnCallAdmin(QString nSeatNo, int nCycleNum);
void slotOnCallService(QString nSeatNo, int nCycleNum);
void slotOnCallOffLine(QString nSeatNo, int nCycleNum);
void slotOnCallOrder(QString nSeatNo, int nCycleNum);
void slotOnCallSeat(QString nSeatNo, int nCycleNum);
signals:

private:
	static webSocketAgent* m_pInstance;
	AttributeServer *m_pServer;
};

#endif // WEBSOCKETAGENT_H
