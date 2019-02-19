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
const QString c_sFormalServerUrl = "https://uu.zimonet.com";
const QString c_sTestServerUrl = "http://202.91.251.122:8050";
const QString c_sLocalServerUrl = "http://jinzehong.s1.natapp.cc";


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
//获取全部商品列表
const QString c_sHttpGetOrderAllGoodsListAction = "/qynetbarsystem/api/goods/getAll?";
//收银下单
const QString c_sHttpOrderGoodsDetialAction = "/qynetbarsystem/api/zmDeal/GeneratePayDealToPaymentCode?";
//商品盘点
const QString c_sHttpOrderGoodsCheckUpAction = "/qynetbarsystem/api/goodsCheckup/checkInventory?";
//查询交接班信息
const QString c_sHttpOrderGetShiftInfoAction = "/qynetbarsystem/api/handover/query?";
//提交交接班信息
const QString c_sHttpOrderCommitShiftInfoAction = "/qynetbarsystem/api/handover/save?";
//校验接班人登录信息
const QString c_sHttpOrderCheckAcceptor = "/qynetbarsystem/api/zmSysAdmin/checkPwd?";
//获取商品库存预警列表
const QString c_sHttpOrderGetWarningGoodsListAction = "/qynetbarsystem/api/goodsInventoryWarn/getWarnGoodsList?";
//获取订单列表
const QString c_sHttpOrderGetOrderListAction = "/qynetbarsystem/api/zmDeal/getListByCashRegister?";
//退款
const QString c_sHttpOrderRefundAction = "/qynetbarsystem/api/zmDeal/refundDeal?";
//撤销订单
const QString c_sHttpOrderCancelOrderAction = "/qynetbarsystem/api/zmDeal/reverseDeal?";
//获取订单详情列表
const QString c_sHttpOrderGetOrderDetialAction = "/qynetbarsystem/api/zmDeal/get?";

#endif