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

enum ChatType{
	t_admin = 1,    //系统消息
	t_local = 2,    //本人消息
	t_user = 3,    //其他用户消息
};

//页面
enum RequestPageType{
	t_rechargePage = 0,    //充值页面
	t_personalCentrePage = 1,    //个人中心界面
	t_orderPage = 2,
};
#endif