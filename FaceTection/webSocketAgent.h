#ifndef WEBSOCKETAGENT_H
#define WEBSOCKETAGENT_H

#include <QObject>
#include "attributeserver.h"

class webSocketAgent : public QObject
{
	Q_OBJECT

public:
	static webSocketAgent* instance();

	void initServer();
private:
	explicit webSocketAgent(QObject *parent = 0);
	~webSocketAgent();

	void initConnect();
signals:
	void sigOnFaceTection(QString idcard, QString name);
private:
	static webSocketAgent* m_pInstance;
	AttributeServer *m_pServer;
};

#endif // WEBSOCKETAGENT_H
