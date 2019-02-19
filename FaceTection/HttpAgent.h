#ifndef HTTPAGENT_H
#define HTTPAGENT_H

#include <QObject>
#include "HttpSignal.h"

class HttpAgent : public QObject
{
	Q_OBJECT

public:
	static HttpAgent* instance();

	void RequestOnFaceDetection(QString filePath);
	void RequestOnCommitFaceData(QString sIdCard, QString sName);
	void RequestOnCommitData(QString sIpAdress, QString sPort);
private:
	explicit HttpAgent(QObject *parent = 0);
	~HttpAgent();

	void initConnect();
signals:
	void sigOnGetFaceDetectionRespond(int nCode, QString sMsg, int nRespondCode);
	void sigOnGetCommitFaceDataRespond(int nCode, QString sMsg, int nRespondCode);
	void sigOnGetCommitDataRespond(int nCode, QString sMsg);
private:
	static HttpAgent* m_pInstance;
};

#endif // HTTPAGENT_H
