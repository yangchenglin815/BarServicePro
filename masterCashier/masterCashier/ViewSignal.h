#ifndef VIEWSIGNAL_H
#define VIEWSIGNAL_H

#include <QObject>

class ViewSignal : public QObject
{
	Q_OBJECT

public:
	static ViewSignal* instance();
signals:
	void onMask(bool isMask);
	void sigOnShowHomePage();
	void sigOnShowVerifyDlg(QString sOrderId, int nPageType);
	void sigOnReturnWaitingPay();
	void sigOnCancelOrderSuccess();
	void sigOnShowTipLabel(bool isHiden);
private:
	explicit ViewSignal(QObject *parent = 0);
	~ViewSignal();
private:
	static ViewSignal* m_pInstance;
};

#endif // VIEWSIGNAL_H
