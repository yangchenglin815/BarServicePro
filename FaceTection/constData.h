#ifndef CONSTDATA_H
#define CONSTDATA_H

#include <QString>

// 网络响应异常
const QString c_sNetError = QStringLiteral("网络连接异常");
const QString c_sServerJsonError = QStringLiteral("服务器数据异常");

//服务类型地址
const QString c_sFormalServerUrl = "http://ddn.quanyoudianjing.com";
const QString c_sTestServerUrl = "http://202.91.251.122:8081";
const QString c_sLocalServerUrl = "http://steven.nat300.top";


/*请求类型说明*/
//人脸检测接口
const QString c_sHttpFaceTectionOrderAction = "/faceRecog/face/faceDetect";
//提交人脸数据
const QString c_sHttpCommitFaceDataOrderAction = "/faceRecog/face/facePerson";
//提交ip端口数据
const QString c_sHttpCommitDataOrderAction = "/faceRecog/thirdBack/registerIP";

#endif