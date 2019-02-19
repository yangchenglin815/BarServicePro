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
	s_offLine = 1,
	s_onLine = 2,
};

//websocket api
enum WSApi{
	w_sReboot = 10001,
	w_sShutdown = 10002,
	w_sLockScreen = 10003,
};
#endif