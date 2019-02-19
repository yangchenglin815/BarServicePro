#include "HttpGetFaceDetectionRespond.h"
#include <QScriptEngine>
#include <QScriptValue>
#include <QDebug>
#include "enumData.h"
#include "HttpSignal.h"
#include "constData.h"
#include "commonFun.h"


HttpGetFaceDetectionRespond::HttpGetFaceDetectionRespond()
{
	setReqUrlAddress("http://recognition.image.myqcloud.com/face/detect");
// 	QString allParam = QString("{\"appid\":\"1252670977\",\"mode\":1,\"url\":\"%1\"}").arg("http://image.baidu.com/search/detail?ct=503316480&z=0&ipn=d&word=%E4%BA%BA%E8%84%B8%E5%9B%BE%E7%89%87&step_word=&hs=2&pn=26&spn=0&di=143473440110&pi=0&rn=1&tn=baiduimagedetail&is=0%2C0&istype=0&ie=utf-8&oe=utf-8&in=&cl=2&lm=-1&st=undefined&cs=2571247346%2C738192935&os=3470713011%2C1885423969&simid=3406494062%2C57428020&adpicid=0&lpn=0&ln=1465&fr=&fmq=1535687257142_R&fm=&ic=undefined&s=undefined&se=&sme=&tab=0&width=undefined&height=undefined&face=undefined&ist=&jit=&cg=&bdtype=0&oriquery=&objurl=http%3A%2F%2Fimg2.app.meitudata.com%2Fmeitushop%2Fimages%2Fm2%2Fphone5_img.jpg&fromurl=ippr_z2C%24qAzdH3FAzdH3Fooo_z%26e3B4jtp7_z%26e3Bv54AzdH3F4d%3FADUIN%3D899nmd8m09%26ADSESSION%3D89d8l0nbdl%26ADTAG%3DCLIENT_z%26e3BQQ_z%26e3Bcdb8__z%26e3Ba%26ADPUBNO%3Ddmdld&gsm=0&rpstart=0&rpnum=0&islist=&querylist=");
// 	QByteArray array;
// 	array.append(allParam);
// 	setReqParams(array);
}

HttpGetFaceDetectionRespond::~HttpGetFaceDetectionRespond()
{

}

void HttpGetFaceDetectionRespond::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("message").toString();
	if (!sc.property("code").isValid())
	{
		//AppConfigHandler.Info(QStringLiteral("HttpDealPayment 返回参数json格式错误").toStdString());
		qDebug() << QStringLiteral("HttpDealPayment 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		//AppConfigHandler.Info(this->getTestUrl().toStdString());
		qDebug() << this->getTestUrl();
	}

	qDebug() << "HttpDealPayment" + strResult;
	//AppConfigHandler.Info("HttpDealPayment" + strResult.toStdString());
	QString orderId = sc.property("data").property("id").toString();
	int payStatus = sc.property("data").property("payStatus").toInt32();
	//HttpSignal::instance()->sigOnGetOrderResult(nCode, sMsg, orderId, payStatus);
	this->deleteLater();
}

void HttpGetFaceDetectionRespond::processError()
{
	//HttpSignal::instance()->sigOnGetOrderResult(e_connectServerError, c_sNetError, "", -1);
	this->deleteLater();
}
