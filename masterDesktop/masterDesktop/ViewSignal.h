#ifndef VIEWSIGNAL_H
#define VIEWSIGNAL_H

#include <QObject>
#include "httpStructData.h"

class ViewSignal : public QObject
{
	Q_OBJECT

public:
	static ViewSignal* instance();
signals:
	void sigOnGoodsNumChanged(GoodsInfo_t goodsInfo);
	void sigOnClearShoppingCart();
private:
	explicit ViewSignal(QObject *parent = 0);
	~ViewSignal();
private:
	static ViewSignal* m_pInstance;
};

#endif // VIEWSIGNAL_H
