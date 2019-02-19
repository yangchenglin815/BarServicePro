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
	void sigOnFaceTection(QString idcard, QString name);
private:
	static webSocketSignal* m_pInstance;
};

#endif // WEBSOCKETSIGNAL_H
