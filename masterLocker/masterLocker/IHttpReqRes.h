#ifndef IHTTPREQRES_H
#define IHTTPREQRES_H

#include <QObject>
#include <QtNetwork>
#include <QSslConfiguration>
#include "appConfigIni.h"

class IHttpReqRes : public QObject
{
	Q_OBJECT

public:
	IHttpReqRes(QObject *parent = 0);
	~IHttpReqRes();

	void setNetManager(QNetworkAccessManager* pManager);
	void setReqParams(QByteArray byArr);
	void postRequest();
	QString getTestUrl(); // 测试用
	void setOverTimeLengh(int nOverTimeLengh); // 设置超时时长
	bool isOverTime(); // 请求是否超时
protected:
	virtual void processResponse(QString strResult) = 0;
	virtual void processError() = 0;
	virtual void setReqUrlAddress(QString urlAddress);
private slots:
	void slot_ReplyFinished();
	void slot_overTimeout();

private:
	void initConnect();
protected:
	QNetworkAccessManager*  m_pManager;
	QNetworkRequest			m_request;
	QNetworkReply*          m_pReply;
	QByteArray				m_reqParams;
	int						m_nReconnectTimes;
	//QString					m_strResult;
	QString m_sUrlAddress;
	QString m_sTestUrl; // 测试用
	QTimer m_overTimer; // 超时定时器
	int m_nOverTimeLen; // 超时时长
	bool m_bOverTime; // 是否超时
};

#endif // IHTTPREQRES_H
