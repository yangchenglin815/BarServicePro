#pragma once
#include <QString>

static QString iniPath_;

class configIni
{
public:
	//获取用户名
	static QString getUserName();
	static void setUserName(QString user);
	//获取密码
	static QString getPasswd();
	static void setPasswd(QString passwd);
	//获取网吧id
	static QString getBarId();
	static void setBarId(QString barId);
    //获取服务ip
	static QString getServerIP();
	static void setServerIP(QString barId);
	//获取端口
	static QString getServerPort();
	static void setServerPort(QString barId);
	//
	static void setIniPath(QString path);
	//获取服务环境类型
	static QString getServerType();
};