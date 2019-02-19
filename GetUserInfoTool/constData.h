#ifndef CONSTDATA_H
#define CONSTDATA_H

#include <QString>

// 网络响应异常
const QString c_sNetError = QStringLiteral("网络连接异常");
const QString c_sServerJsonError = QStringLiteral("服务器数据异常");

//订座校验返回错误提示
const QString c_sBookingExpiredErr = QStringLiteral("订座失效");
const QString c_sBookingCodeErr = QStringLiteral("验证码错误");
const QString c_sBookingUnlockedErr = QStringLiteral("该电脑已被解锁");

//服务类型地址
const QString c_sFormalServerUrl = "http://ddn.quanyoudianjing.com";
const QString c_sTestServerUrl = "http://202.91.251.122:8050";
const QString c_sLocalServerUrl = "http://jinzehong.s1.natapp.cc";
const QString c_sServerUrl = "http://cuizhihu.s1.natapp.cc";


/*请求类型说明*/
//获取订座状态
const QString c_sHttpGetOrderBookSeatAction = "/qynetbarsystem/api/bookingSeat/findPcSatus?";
//校验解锁码
const QString c_sHttpGetOrderVerifyUnlockAction = "/qynetbarsystem/api/bookingSeat/unlockPreseat?";
//上报PC在线状态
const QString c_sHttpGetOrderOnlineStatusAction = "/qynetbarsystem/api/bookingSeat/upPcstatus?";
//获取BarId
const QString c_sHttpGetOrderBarNameAction = "/qynetbarsystem/api/barData/getBarData?";
//获取二维码
const QString c_sHttpGetOrderQrCodeAction = "/qydj/common?";
//登录接口
const QString c_sHttpOrderLoginAction = "/qynetbarsystem/api/login/loginCashierDesk?";
//登出接口
const QString c_sHttpOrderLogoutAction = "/qynetbarsystem/api/login/logoutCashierDesk?";
//获取商品分类
const QString c_sHttpGetOrderClassifyGroupAction = "/qynetbarsystem/api/goodsClassify/useList?";
//获取商品列表
const QString c_sHttpGetOrderGoodsListAction = "/qynetbarsystem/api/goods/useList?";
//收银下单
const QString c_sHttpOrderGoodsDetialAction = "/qynetbarsystem/api/zmDeal/GeneratePayDealToPaymentCode?";
//新增或修改客户机信息
const QString c_sHttpReportPCInfoAction = "/qynetbarsystem/api/computerInfo/addorUpdateComputerInfo?";

#endif