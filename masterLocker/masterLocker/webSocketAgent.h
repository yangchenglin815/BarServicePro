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
    void slotOnWSRebootFunc();
	void slotOnWSShutdownFunc();
signals:
	void sigOnWSLockScreenFunc();
	void sigOnWSUnLockScreenFunc();
private:
	static webSocketAgent* m_pInstance;
	AttributeServer *m_pServer;
};

#endif // WEBSOCKETAGENT_H
