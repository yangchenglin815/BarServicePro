#ifndef ENUMDATA_H
#define ENUMDATA_H

enum ServerHttpErrorCode{
	e_connectServerError = -1,
	e_success = 0, // 成功
	e_paramError = 1, // 参数错误，未找到参数
	e_bookingExpired = 1001,   //订座失效
	e_bookingCodeErr = 1002,  //订座验证码错误
	e_bookingUnlocked = 1003,  //已被解锁
};

enum LoginType{
	t_login = 1,   //登录
	t_switch = 2,   //切换账号
	t_shift = 3,    //交接班
};

enum ServerType{
	t_formalServer = 1,    //正式服务
	t_testServer = 2,     //测试服务
	t_localServer = 3,     //本地服务
};

//订座状态
enum BookSeatStatus{
	s_unBookSeat = 0,    //未预订
	s_bookSeat = 1,       //已预订
};

//计算机状态
enum PCStatus{
	s_offLine = 1,    //离线
	s_onLine = 2,     //在线
};

//websocket api
enum WSApi{
	w_sReboot = 10001,     //重启
	w_sShutdown = 10002,     //关机
	w_sLockScreen = 10003,    //锁屏
};

//支付类型
enum PayType{
	t_wxPay = 1,      //微信支付
	t_aliPay = 2,     //支付宝支付
	t_cashPay = 3,   //现金支付
    t_yyPay = 4,     //幽游钱包支付
};

//支付弹框类型
enum PayDlgType{
	t_payment_success = 101,     //支付成功
	t_payment_failed = 102,      //支付失败
	t_paymenting = 103,          //支付中
};

//页面
enum PageType{
	t_salePage = 0,    //销售页面
	t_shiftPage = 1,   //交接班页面
	t_returnPage = 2,   //退货单页面
	t_orderPage = 3,    //订单页面
};

//仓库类型
enum Stocktype{
	t_mainStock = 1,     //主库存
	t_barStock = 2,      //吧台库存
};

//吧台操作
enum BarOpearte{
	t_upInventory = 1,      //上架
	t_downInventory = 2,    //下架
};

//库存操作
enum MainOperate{
	t_inInventory = 1,  //进货
	t_outInventory = 2,  //退货
};

//订单支付状态
enum OrderPayStatus{
	s_unPay = 1,  //未支付
	s_pay = 2,   //已支付
	s_return = 3,  //已退款
};
#endif