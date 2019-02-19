#ifndef CONSTDATA_H
#define CONSTDATA_H

#include <QString>

const QString c_sAppDownloadUrl = "http://uu.zimonet.com/download/";

// 网络响应异常
const QString c_sNetError = QStringLiteral("网络连接异常");
const QString c_sServerJsonError = QStringLiteral("服务器数据异常");

//订座校验返回错误提示
const QString c_sBookingExpiredErr = QStringLiteral("订座失效");
const QString c_sBookingCodeErr = QStringLiteral("验证码错误");
const QString c_sBookingUnlockedErr = QStringLiteral("该电脑已被解锁");

//服务类型地址
const QString c_sFormalServerUrl = "https://uu.zimonet.com";
const QString c_sTestServerUrl = "http://202.91.251.122:8050";
const QString c_sLocalServerUrl = "http://jinzehong.s1.natapp.cc";


//请求类型说明
const QString c_sHttpGetOrderBookSeatAction = "/qynetbarsystem/api/bookingSeat/findPcSatus?";
const QString c_sHttpGetOrderVerifyUnlockAction = "/qynetbarsystem/api/bookingSeat/unlockPreseat?";
const QString c_sHttpGetOrderOnlineStatusAction = "/qynetbarsystem/api/bookingSeat/upPcstatus?";
const QString c_sHttpGetOrderBarNameAction = "/qynetbarsystem/api/barData/getBarData?";
const QString c_sHttpGetOrderQrCodeAction = "/qydj/common?";

#endif