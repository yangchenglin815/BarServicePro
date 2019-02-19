#pragma once
#include <QString>

static QString CallServerIniPath_;

class CallServerIni
{
public:
	//获取服务器IP地址
	static QString getServerIP();
	static void setServerIP(QString serverIP);
	//获取服务器端口
	static QString getServerPort();
	static void setServerPort(QString serverPort);
	//设置开机自启
	static bool getAutoExecFlag();
	static void setAutoExecFlag(bool isAutoExec);
	//设置呼叫音量
	static int getCallVoiceValue();
	static void setCallVoiceValue(int value);
	//设置播放音量
	static int getPlayVoiceValue();
	static void setPlayVoiceValue(int value);

	static QString getWebPort();
	//
	static void setIniPath(QString path);
};

