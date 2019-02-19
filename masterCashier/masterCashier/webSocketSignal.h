#ifndef WEBSOCKETSIGNAL_H
#define WEBSOCKETSIGNAL_H

#include <QObject>
#include "receiptData.h"

class webSocketSignal : public QObject
{
	Q_OBJECT

public:
	static webSocketSignal* instance();
private:
	explicit webSocketSignal(QObject *parent = 0);
	~webSocketSignal();
signals:
	void sigOnPrintTips(ReceiptInfo info);
	void sigOnGoodsListChanged();
	void sigOnPaySuccess();
	void sigOnFaceTection(QString idcard, QString name);
private:
	static webSocketSignal* m_pInstance;
};

#endif // WEBSOCKETSIGNAL_H
