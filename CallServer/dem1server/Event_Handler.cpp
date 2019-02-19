#include "Event_Handler.h"

#include <stdexcept>
#include <exception>
#include <stdio.h>
#include <tchar.h>

#include "dmconnection.hpp"
#include "logging.hpp"
#include <Windows.h>
#include <MMSystem.h>
#include <QDir>
namespace dm {
	namespace server {

		/// 单例声明
		/// 类内 class static 除了在class 之内声明，还需要在class 之外定义.
		auto_ptr<Event_Handler> Event_Handler::auto_ptr_instance;

		Event_Handler::Event_Handler()
		{
			is_thread_exit_ = false;

			if (list_cycle_thread_ && list_cycle_thread_->joinable())
			{
				list_cycle_thread_->join();
			}
			list_cycle_thread_.reset(new boost::thread(&Event_Handler::boardcastCycleThreadFunc, this));
		}


		Event_Handler::~Event_Handler()
		{
			is_thread_exit_ = true;
		}

		Event_Handler& Event_Handler::GetInstance(void)
		{
			if (auto_ptr_instance.get() == 0) {
				auto_ptr_instance.reset(new Event_Handler());
			}
			return *(auto_ptr_instance.get());
		}

		bool Event_Handler::playBoardcastMsg(const api_client_info_t &api_info, std::string name)
		{
			std::string url = (QDir::homePath() + "/AppData/Local/Host-Server/").toStdString();
			url += name;

			char szStr[256];
			memset(szStr, 0, 256);
			memcpy(szStr, url.c_str(), 256);
			WCHAR wszClassName[256];
			memset(wszClassName, 0, sizeof(wszClassName));
			MultiByteToWideChar(CP_ACP, 0, szStr, strlen(szStr) + 1, wszClassName,
				sizeof(wszClassName) / sizeof(wszClassName[0]));

			MCI_OPEN_PARMS MciOpen;
			MciOpen.lpstrElementName = wszClassName;
			MciOpen.lpstrDeviceType = NULL;


			mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&MciOpen);
			wDeviceID_ = MciOpen.wDeviceID;
			MCIPlay();
			return true;
		}

		DWORD Event_Handler::MCIStatus_music_length()
		{
			MCI_STATUS_PARMS mciStatus;
			mciStatus.dwItem = MCI_STATUS_LENGTH;
			DWORD staReturn = mciSendCommand(wDeviceID_, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatus);
			DWORD music_alltime = mciStatus.dwReturn / 1000; //音乐总时间  
			return music_alltime;
		}

		DWORD Event_Handler::MCIStatus_music_pos()
		{
			MCI_STATUS_PARMS mciStatus;
			mciStatus.dwItem = MCI_STATUS_POSITION;
			DWORD sta1Return = mciSendCommand(wDeviceID_, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatus);
			DWORD music_timeing = mciStatus.dwReturn / 1000; //音乐当前播放时间  
			return music_timeing;
		}

		void Event_Handler::close_mci()
		{
			DWORD retclose = mciSendCommand(wDeviceID_, MCI_CLOSE, NULL, NULL);
			Log::Info("close the media %d", wDeviceID_);
		}

		DWORD Event_Handler::MCIPlay()
		{
			MCI_PLAY_PARMS mciPP;
			DWORD opReturn;
			opReturn = mciSendCommand(wDeviceID_, MCI_PLAY, MCI_NOTIFY, (DWORD)&mciPP);
			return opReturn;
		}

		void Event_Handler::addBoardCastListItem(const api_boardcast_info_t &api_info)
		{
			boost::lock_guard<boost::mutex> lock(ctrl_mutex_);
			boardCast_list_copy_.append(api_info);
		}

		void Event_Handler::removeBoardCastListItem(const api_boardcast_info_t &api_info)
		{
			boost::lock_guard<boost::mutex> lock(ctrl_mutex_);
			std::string seatNo = (char*)api_info.nSeatNo;
			QList<api_boardcast_info_t>::iterator it;
			for (it = boardCast_list_copy_.begin(); it != boardCast_list_copy_.end(); ++it){
				if (it->nBoardCastType == api_info.nBoardCastType && (char*)it->nSeatNo == seatNo)
				{
					boardCast_list_copy_.erase(it);
					break;
				}
			}
		}

		void Event_Handler::boardcastCycleThreadFunc()
		{
			while (!is_thread_exit_)
			{
				ctrl_mutex_.lock();
				boardCast_list_ = boardCast_list_copy_;
				boardCast_list_copy_.clear();
				ctrl_mutex_.unlock();
	
				api_client_info_t api_info;
				for (int i = 0; i < boardCast_list_.size(); ++i)
				{
					Log::Info("current seatNo : %s", boardCast_list_[i].nSeatNo);
					playBoardcastMsg(api_info, boardCast_list_[i].getBoardCastType());
					waitforPlayFinished();
				}
				boardCast_list_.clear();
				Sleep(1000);
			}
		}

		void Event_Handler::waitforPlayFinished(int sec)
		{
			DWORD time = MCIStatus_music_length();
			DWORD curr_time;
			while (true)
			{
				curr_time = MCIStatus_music_pos();
			//	Log::Info("total time is %d, curr time is %d", time, curr_time);
				if (curr_time /*+ 10*/ == time)
				{
					close_mci();
					break;
				}
				Sleep(1000);
			}
			Sleep(sec);
		}

	}  // namespace server
} // namespace dm