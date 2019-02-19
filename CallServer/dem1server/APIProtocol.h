#pragma once
#include "message.h"
#include "appconfig.h"
#include "logging.hpp"
#include <QList>
#include <string.h>
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
namespace dm {
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
			DEV_OFFLINE = 10002,   //下机清理
			OFFER_FOODS = 10003,  //点餐
			CALL_MANAGEMENT = 10004,  //呼叫网管
			CAREFOR_EYES = 10005,   //保护眼睛
			CALL_SERVICE = 10006,  //呼叫服务员
			CALL_SEAT = 10007,   //订座呼叫
		}boardcast_type_e;
  

#pragma pack (1) // 指定按1字节对齐
        // 协议头
        typedef struct protocol_head
        {

            unsigned char	nStartFlags;			// 开始标示:0xAF
            unsigned char	nVersion;				// 版本:0x01
            unsigned short	nCmd;					// 指令:command_e 枚举结构
            unsigned int	nDataLen;				// 负载长度

            short			nResult;				// 仅用于命令返回:command_result_e 枚举结构

            unsigned char	deviceNum;				// 预留
            unsigned char	nEndFlags;				// 结束标示:oxFA	

            protocol_head(){
                nStartFlags = 0xAF;
                nVersion = 0x01;
                nCmd = 0;
                nDataLen = 0;
                nResult = 0;
                deviceNum = 0;
                nEndFlags = 0xFA;
            }
            int get_size(){

                return sizeof(protocol_head);
            }

            void read(netbuffer_t* rb){

                read_uchar(this->nStartFlags, rb);
                read_uchar(this->nVersion, rb);
                read_ushort(this->nCmd, rb);
                read_uint(this->nDataLen, rb);
                read_short(this->nResult, rb);
                read_uchar(this->deviceNum, rb);
                read_uchar(this->nEndFlags, rb);
            }

            void write(netbuffer_t* wb){

                write_uchar(this->nStartFlags, wb);
                write_uchar(this->nVersion, wb);
                write_ushort(this->nCmd, wb);
                write_uint(this->nDataLen, wb);
                write_short(this->nResult, wb);
                write_uchar(this->deviceNum, wb);
                write_uchar(this->nEndFlags, wb);
            }
        }protocol_head_t;

		// 被控终端相关信息
		typedef struct api_client_info
		{
			unsigned char nUser[API_ARRAY_LEN];           //终端用户名
			unsigned char nPassWd[API_ARRAY_LEN];      //终端登录密码
			unsigned char nIPAdress[API_ARRAY_LEN];     //终端IP地址
			unsigned char nMacAdress[API_ARRAY_LEN];   //终端MAC地址
			unsigned char nSeatNo[API_ARRAY_LEN];  //播报语音文件名  

			api_client_info(){
				memset(&nUser, 0, API_ARRAY_LEN);
				memset(&nPassWd, 0, API_ARRAY_LEN);
				memset(&nIPAdress, 0, API_ARRAY_LEN);
				memset(&nMacAdress, 0, API_ARRAY_LEN);
				memset(&nSeatNo, 0, API_ARRAY_LEN);
			}

			int get_size(){
				return sizeof(api_client_info);
			}

			void read(netbuffer_t* rb){
				readdata(this->nUser, API_ARRAY_LEN, rb);
				readdata(this->nPassWd, API_ARRAY_LEN, rb);
				readdata(this->nIPAdress, API_ARRAY_LEN, rb);
				readdata(this->nMacAdress, API_ARRAY_LEN, rb);
				readdata(this->nSeatNo, API_ARRAY_LEN, rb);
			}

			void write(netbuffer_t* wb){
				writedata(this->nUser, API_ARRAY_LEN, wb);
				writedata(this->nPassWd, API_ARRAY_LEN, wb);
				writedata(this->nIPAdress, API_ARRAY_LEN, wb);
				writedata(this->nMacAdress, API_ARRAY_LEN, wb);
				writedata(this->nSeatNo, API_ARRAY_LEN, wb);
			}
			void read_json(boost::property_tree::ptree parser){
				try
				{
					memcpy(this->nUser, parser.get<std::string>("nUser").c_str(), parser.get<std::string>("nUser").length());
					memcpy(this->nPassWd, parser.get<std::string>("nPassWd").c_str(), parser.get<std::string>("nPassWd").length());
					memcpy(this->nIPAdress, parser.get<std::string>("nIPAdress").c_str(), parser.get<std::string>("nIPAdress").length());
					memcpy(this->nMacAdress, parser.get<std::string>("nMacAdress").c_str(), parser.get<std::string>("nMacAdress").length());
					memcpy(this->nSeatNo, parser.get<std::string>("nSeatNo").c_str(), parser.get<std::string>("nSeatNo").length());
				}
				catch (boost::property_tree::ptree_error const & ec)
				{
					//AppConfigHandler.Info("struct api_register read_json error : " + string(ec.what()));
					Log::Info("struct api_register read_json error : %s", ec.what());
				}
			}
			void write_json(boost::property_tree::ptree &parser){
				parser.put<std::string>("nUser", (char*)(this->nUser));
				parser.put<std::string>("nPassWd", (char*)(this->nPassWd));
				parser.put<std::string>("nIPAdress", (char*)(this->nIPAdress));
				parser.put<std::string>("nMacAdress", (char*)(this->nMacAdress));
				parser.put<std::string>("nSeatNo", (char*)(this->nSeatNo));
			}
		}api_client_info_t;

		//音频细节
		typedef struct api_audio_info
		{
			std::string mAudioFileName;
			bool isNum;
			api_audio_info(){
				mAudioFileName = "";
				isNum = false;
			}
		}api_audio_info_t;

		//语音播报内容
		typedef struct api_boardcast_info
		{
			unsigned char nSeatNo[API_ARRAY_LEN];    //座位号
			unsigned short nBoardCastType;           //语音播报类型
			unsigned short nPlayCycleNum;            //语音播报次数
			api_boardcast_info(){
				memset(&nSeatNo, 0, API_ARRAY_LEN);
				nBoardCastType = 0;
				nPlayCycleNum = 0;
			}
			int get_size(){
				return sizeof(api_boardcast_info);
			}
			QList<api_audio_info> getBoardCastType(){
				QList<api_audio_info> list_;
				api_audio_info audioInfo_t;
				switch (nBoardCastType)
				{
				case CALL_MANAGEMENT:
				{
										audioInfo_t.mAudioFileName = "呼叫/呼叫网管_前.mp3";
										audioInfo_t.isNum = false;								
										list_.append(audioInfo_t);

										std::string groupNam = (char*)nSeatNo;
										for (int i = 0; i < groupNam.size(); ++i)
										{
											audioInfo_t.mAudioFileName = "数字/" + groupNam.substr(i, 1) + ".mp3";
											audioInfo_t.isNum = true;
											list_.append(audioInfo_t);
										}

										audioInfo_t.mAudioFileName = "呼叫/号机呼叫网管.mp3";
										audioInfo_t.isNum = false;
										list_.append(audioInfo_t);
										break;
				}
				case CALL_SERVICE:
				{
									 audioInfo_t.mAudioFileName = "呼叫/呼叫网管_前.mp3";
									 audioInfo_t.isNum = false;
									 list_.append(audioInfo_t);

									 std::string groupNam = (char*)nSeatNo;
									 for (int i = 0; i < groupNam.size(); ++i)
									 {
										 audioInfo_t.mAudioFileName = "数字/" + groupNam.substr(i, 1) + ".mp3";
										 audioInfo_t.isNum = true;
										 list_.append(audioInfo_t);
									 }

									 audioInfo_t.mAudioFileName = "呼叫/呼叫服务员_后.mp3";
									 audioInfo_t.isNum = false;
									 list_.append(audioInfo_t);
									 break;
				}
				case DEV_OFFLINE:
				{
									 audioInfo_t.mAudioFileName = "呼叫/下机清扫_前.mp3";
									 audioInfo_t.isNum = false;
									 list_.append(audioInfo_t);

									 std::string groupNam = (char*)nSeatNo;
									 for (int i = 0; i < groupNam.size(); ++i)
									 {
										 audioInfo_t.mAudioFileName = "数字/" + groupNam.substr(i, 1) + ".mp3";
										 audioInfo_t.isNum = true;
										 list_.append(audioInfo_t);
									 }

									 audioInfo_t.mAudioFileName = "呼叫/下机清扫_后.mp3";
									 audioInfo_t.isNum = false;
									 list_.append(audioInfo_t);
									 break;
				}
				case OFFER_FOODS:
				{
									audioInfo_t.mAudioFileName = "呼叫/定向呼叫_前.mp3";
									audioInfo_t.isNum = false;
									list_.append(audioInfo_t);

									audioInfo_t.mAudioFileName = "呼叫/order.mp3";
									audioInfo_t.isNum = false;
									list_.append(audioInfo_t);
									break;
				}
				case CALL_SEAT:
				{
									audioInfo_t.mAudioFileName = "呼叫/定向呼叫_前.mp3";
									audioInfo_t.isNum = false;
									list_.append(audioInfo_t);

									audioInfo_t.mAudioFileName = "呼叫/seat.mp3";
									audioInfo_t.isNum = false;
									list_.append(audioInfo_t);
									break;
				}
				default:
					break;
				}

				return list_;
			}
			std::string getSeatNo(){
				std::string seatNo = (char*)nSeatNo;
				return "audiofile_" + seatNo + ".mp3";
			}
			void read_json(boost::property_tree::ptree parser){
				try
				{
					memcpy(this->nSeatNo, parser.get<std::string>("nSeatNo").c_str(), parser.get<std::string>("nSeatNo").length());
					this->nBoardCastType = parser.get<short>("nBoardCastType");
					this->nPlayCycleNum = parser.get<short>("nBoardCastType");
				}
				catch (boost::property_tree::ptree_error const & ec)
				{
					//AppConfigHandler.Info("struct api_boardcast_info read_json error : " + string(ec.what()));
					Log::Info("struct api_boardcast_info read_json error : ", ec.what());
				}
			}
			void write_json(boost::property_tree::ptree &parser){
				parser.put<std::string>("nSeatNo", (char*)(this->nSeatNo));
				parser.put<short>("nBoardCastType", this->nBoardCastType);
				parser.put<short>("nPlayCycleNum", this->nPlayCycleNum);
			}
		}api_boardcast_info_t;

		//同步数据接口
		typedef struct api_syncdata_info{
			unsigned short nCallVioceValue;
			api_syncdata_info(){
				nCallVioceValue = 0;
			}
			int get_size(){
				return sizeof(api_syncdata_info);
			}
			void read_json(boost::property_tree::ptree parser){
				try
				{
					this->nCallVioceValue = parser.get<short>("nCallVioceValue");
				}
				catch (boost::property_tree::ptree_error const & ec)
				{
					//AppConfigHandler.Info("struct api_boardcast_info read_json error : " + string(ec.what()));
					Log::Info("struct api_boardcast_info read_json error : ", ec.what());
				}
			}
			void write_json(boost::property_tree::ptree &parser){
				parser.put<short>("nCallVioceValue", this->nCallVioceValue);
			}
		}api_syncdata_info_t;
    }  // namespace server
}  // namespace dm