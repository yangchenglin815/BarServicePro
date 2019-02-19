#pragma once
#include "message.h"
#include "logging.hpp"
#define API_ARRAY_LEN	(32)
#define  API_URL_LEN   (512)

#define  HTTP_CRLF "\r\n"
#define  HTTP_CRLFCRLF "\r\n\r\n"
/********************************************
1）考虑可拓展性
2）网络转换时会变成unsigned

********************************************/

/*********************************************************************************************
APIClient处理(NVR)：
1）实现了注册、实时视频请求响应，及重连处理
1）马上重连情况：a)系统启动时 b）消息错误 c）服务器参数重置 d）发送注册请求失败
2）每10s重连情况：a）创建socket失败 b）连接失败  c）注册失败
APIServer(你要做的)：提供了3个测试程序，针对APIClient，你可抓包分析，配合你开发
**********************************************************************************************/
namespace dem1 {
    namespace server {
		// 命令号
		typedef enum
		{
			CMD_API_CLIENT_REBOOT = 1,   //重启终端设备
			CMD_API_CLIENT_REBOOT_RES,

			CMD_API_CLIENT_REMOTE_WAKEUP,      //远程唤醒
			CMD_API_CLIENT_REMOTE_WAKEUP_RES,

			CMD_API_CLIENT_BOARDCAST_MSG,    //播报语音
			CMD_API_CLIENT_BOARDCAST_MSG_RES,

			CMD_API_CLIENT_HOST_REGISTER,  //客户机注册
			CMD_API_CLIENT_HOST_REGISTER_RES,
			
			CMD_API_CLIENT_SHUTDOWN = 11,  //关闭计算机
			CMD_API_CLIENT_SHUTDOWN_RES,

			CMD_API_CLIENT_CANCEL_BOARDCAST,  //取消语音播报
			CMD_API_CLIENT_CANCEL_BOARDCAST_RES,

			CMD_API_CLIENT_CONTROL_CALLVOICE_VALUE,   //控制客户端呼叫音量大小
			CMD_API_CLIENT_CONTROL_CALLVOICE_VALUE_RES,

			CMD_API_CLIENT_REQUEST_CALLVOICE_VALUE,   //客户端请求呼叫音量大小
			CMD_API_CLIENT_REQUEST_CALLVOICE_VALUE_RES,

			CMD_API_SYNC_DATA,       //同步数据
			CMD_API_SYNC_DATA_RES,
        }command_e;

        // 返回值
        typedef enum
        {
			RESULT_SUCCESS = 0,		// 成功	
			RESULT_FAILED = -1,     //失败
        }command_result_e;

			//语音播报类型
		typedef enum
		{
			DEV_ONLINE = 10001,    //上机
			DEV_OFFLINE = 10002,   //下机
			OFFER_FOODS = 10003,  //点餐
			CALL_MANAGEMENT = 10004,  //呼叫网管
			CAREFOR_EYES = 10005,   //保护眼睛
		}boardcast_type_e;
		
			    //终端类型
		typedef enum
		{
			IPAD_CLIENT = 0,            //ipad端
			CSERVER_CLIENT = 1,    //CServer端
			WEB_CLIENT = 2,           //web端
		}client_type_e;
  

#pragma pack (1) // 指定按1字节对齐
        // 协议头组装 data = 终端信息相关json串
		//注：json字段内容都要带“” 方便C端解析
        	std::stringstream title;
			title << headTitle << " " << "ServerMessage/1.0" << HTTP_CRLF;

			int sessionId = -1;
			std::stringstream ss;
			ss << "Host: " << "localhost" << HTTP_CRLF
				<< "User-Agent: " << "server agent" << HTTP_CRLF
				<< "Connection: Keep-Alive" << HTTP_CRLF
				<< "Content-Length: " << std::dec << data.length() << HTTP_CRLF
				<< "Session-Id: " << std::dec << sessionId << HTTP_CRLF
				<< "Content-Type: application/json" << HTTP_CRLFCRLF;

			title << "Header-Length: " << std::dec << ss.str().length() << HTTP_CRLF;
			title << ss.str() << data;
			
		//组装后实例：
		CMD_API_CLIENT_REBOOT ServerMessage/1.0

        Header-Length: 138

        Host: localhost

        User-Agent: server agent

        Connection: Keep-Alive

        Content-Length: 190

        Session-Id: -1

        Content-Type: application/json



        {
            "CmdType": "1",
            "ErrorCode": "0",
            "ServerType": "0",
            "nUser": "ycl",
            "nPassWd": "123456",
            "nIPAdress": "192.168.2.46",
            "nMacAdress": "FF-FF-FF-FF-FF-FF",
            "nSeatNo": "010"
        }
			


		// 被控终端相关信息
		typedef struct api_client_info
		{
			unsigned char nUser[API_ARRAY_LEN];           //终端用户名
			unsigned char nPassWd[API_ARRAY_LEN];      //终端登录密码
			unsigned char nIPAdress[API_ARRAY_LEN];     //终端IP地址
			unsigned char nMacAdress[API_ARRAY_LEN];   //终端MAC地址
			unsigned char nSeatNo[API_ARRAY_LEN];  //座位号
		}api_client_info_t;

		//语音播报内容
		typedef struct api_boardcast_info
		{
			unsigned char nSeatNo[API_ARRAY_LEN];    //座位号
			unsigned short nBoardCastType;           //语音播报类型
			unsigned short nPlayCycleNum;            //语音播报次数
		}api_boardcast_info_t;

		//同步数据接口
		typedef struct api_syncdata_info{
			unsigned short nCallVioceValue;
		}api_syncdata_info_t;
    }  // namespace server
}  // namespace dem1