#ifndef WEBSOCKETSIGNAL_H
#define WEBSOCKETSIGNAL_H

#include <QObject>

class webSocketSignal : public QObject
{
	Q_OBJECT

public:
	static webSocketSignal* instance();
private:
	explicit webSocketSignal(QObject *parent = 0);
	~webSocketSignal();
signals:
	void sigOnCallAdmin(QString nSeatNo, int nCycleNnum);
	void sigOnCallService(QString nSeatNo, int nCycleNnum);
	void sigOnCallOffLine(QString nSeatNo, int nCycleNnum);
	void sigOnCallOrder(QString nSeatNo, int nCycleNnum);
	void sigOnCallSeat(QString nSeatNo, int nCycleNnum);
private:
	static webSocketSignal* m_pInstance;
};

#endif // WEBSOCKETSIGNAL_H
