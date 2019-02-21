#pragma once
enum ServerHttpErrorCode {
	e_connectServerError = -1,
	e_success = 200,
	e_paramError = 1,
};

enum ServerType{
	t_formalServer = 1,    //正式服务
	t_testServer = 2,     //测试服务
	t_localServer = 3,     //本地服务
};