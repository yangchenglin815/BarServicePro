#ifndef IHTTPREQRES_H
#define IHTTPREQRES_H

#include <QObject>
#include <QtNetwork>

class IHttpReqRes : public QObject
{
	Q_OBJECT
public:
	explicit IHttpReqRes(QObject *parent = nullptr);
	~IHttpReqRes();

	void setNetManager(QNetworkAccessManager* pManager);
	void setReqParams(QByteArray byArr);
	void postRequest();
	void getRequest();
	void postJsonRequest();
	QString getTestUrl();
	void setOverTimeLength(int nOverTimeLength);
	bool isOverTime();
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
	QNetworkAccessManager* m_pManager;
	QNetworkRequest m_request;
	QNetworkReply* m_pReply;
	QByteArray m_reqParams;
	int m_nReconnectTimes;
	QString m_sUrlAddress;
	QString m_sTestUrl;
	QTimer m_overTimer;
	int m_nOverTimeLen;
	bool m_bOverTime;
};

#endif // IHTTPREQRES_H
