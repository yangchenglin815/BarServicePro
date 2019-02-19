#ifndef CONSTDATA_H
#define CONSTDATA_H

#include <string>
using namespace std;

//服务类型地址
const string c_sFormalServerUrl = "https://uu.zimonet.com";
const string c_sTestServerUrl = "http://202.91.251.122:8050";
const string c_sLocalServerUrl = "http://jinzehong.s1.natapp.cc";


//请求类型说明
const string c_sHttpGetOrderBookSeatAction = "/qynetbarsystem/api/bookingSeat/findPcSatus?";
const string c_sHttpGetOrderVerifyUnlockAction = "/qynetbarsystem/api/bookingSeat/unlockPreseat?";
const string c_sHttpGetOrderOnlineStatusAction = "/qynetbarsystem/api/bookingSeat/upPcstatus?";
const string c_sHttpGetOrderBarNameAction = "/qynetbarsystem/api/barData/getBarData?";
const string c_sHttpGetOrderQrCodeAction = "/qydj/common?";

#endif