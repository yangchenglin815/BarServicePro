#include "handleEvent.h"
#include "dmconnection.hpp"
#include "appconfig.h"

#include <stdexcept>
#include <exception>
#include <stdio.h>
#include <tchar.h>

#include <Windows.h>
#include <MMSystem.h>
#include <QDir>
#include <QSettings>
#include <QApplication>
#include <Digitalv.h>
#include "CallServerIni.h"

namespace dm {
	namespace server {

		void handleEvent::slotOnChanged()
		{

		}

		handleEvent* handleEvent::m_pInstance = NULL;

		handleEvent* handleEvent::instance()
{
		if (m_pInstance == NULL)
		{
			m_pInstance = new handleEvent;
		}
		return m_pInstance;
}
handleEvent::handleEvent(QObject *parent)
	: QObject(parent)
{
	is_thread_exit_ = false;
	curr_play_seatNo = "";
	curr_play_boardType = -1;

	if (list_cycle_thread_ && list_cycle_thread_->joinable())
	{
		list_cycle_thread_->join();
	}
	list_cycle_thread_.reset(new boost::thread(&handleEvent::boardcastCycleThreadFunc, this));
}

handleEvent::~handleEvent()
{
	is_thread_exit_ = true;
}

bool handleEvent::playBoardcastMsg(const api_client_info_t &api_info, std::string name)
{
	std::string url = (QDir::homePath() + "/AppData/Local/Call-Server/Audio/").toStdString();
	std::string url_ = (QApplication::applicationDirPath() + "/audio/").toStdString();
	url_ += name;

	char szStr[256];
	memset(szStr, 0, 256);
	memcpy(szStr, url_.c_str(), 256);
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
	MCISetVolume(CallServerIni::getPlayVoiceValue() * 10);
	return true;
}

DWORD handleEvent::MCIStatus_music_length()
{
	MCI_STATUS_PARMS mciStatus;
	mciStatus.dwItem = MCI_STATUS_LENGTH;
	DWORD staReturn = mciSendCommand(wDeviceID_, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatus);
	DWORD music_alltime = mciStatus.dwReturn / 1000; //音乐总时间  
	return music_alltime;
}

DWORD handleEvent::MCIStatus_music_pos()
{
	MCI_STATUS_PARMS mciStatus;
	mciStatus.dwItem = MCI_STATUS_POSITION;
	DWORD sta1Return = mciSendCommand(wDeviceID_, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatus);
	DWORD music_timeing = mciStatus.dwReturn / 1000; //音乐当前播放时间  
	return music_timeing;
}

void handleEvent::close_mci()
{
	DWORD retclose = mciSendCommand(wDeviceID_, MCI_CLOSE, NULL, NULL);
	//AppConfigHandler.Info(QString("close the media %1").arg(wDeviceID_).toStdString());
	Log::Info("close the media");
}

DWORD handleEvent::MCIPlay()
{
	MCI_PLAY_PARMS mciPP;
	DWORD opReturn;
	opReturn = mciSendCommand(wDeviceID_, MCI_PLAY, MCI_NOTIFY, (DWORD)&mciPP);
	return opReturn;
}

DWORD handleEvent::MCISetVolume(DWORD dwVol)
{
	MCI_DGV_SETAUDIO_PARMS  mciSetAudioPara;
	mciSetAudioPara.dwCallback = NULL;
	mciSetAudioPara.dwItem = MCI_DGV_SETAUDIO_VOLUME;
	mciSetAudioPara.dwValue = dwVol; //此处就是音量大小 (0--1000)  
	DWORD dwReturn = mciSendCommand(wDeviceID_, MCI_SETAUDIO, MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD)(LPVOID)&mciSetAudioPara);
	return dwReturn;
}

void handleEvent::appAutoRun(bool bAutoRun)
{
	QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
	if (bAutoRun){
		QString strAppPath = QDir::toNativeSeparators(QApplication::applicationFilePath());
		reg.setValue("Call-Server", strAppPath);
	}
	else{
		reg.setValue("Call-Server", "");
	}
}

void handleEvent::addBoardCastListItem(const api_boardcast_info_t &api_info)
{
	boost::lock_guard<boost::mutex> lock(ctrl_mutex_);
	//boardCast_list_copy_.append(api_info);
	std::string seatNo = (char*)api_info.nSeatNo;
	//AppConfigHandler.Info("add to playList:" + to_string(api_info.nBoardCastType) + "---" +seatNo);
	Log::Info("add to playList: %d ---- %s", api_info.nBoardCastType, seatNo.c_str());
	boardCast_list_.append(api_info);
}

void handleEvent::removeBoardCastListItem(const api_boardcast_info_t &api_info)
{
// 	boost::lock_guard<boost::mutex> lock(ctrl_mutex_);
// 	std::string seatNo = (char*)api_info.nSeatNo;
// 	QList<api_boardcast_info_t>::iterator it;
// 	for (it = boardCast_list_copy_.begin(); it != boardCast_list_copy_.end(); ++it){
// 		if (it->nBoardCastType == api_info.nBoardCastType && (char*)it->nSeatNo == seatNo)
// 		{
// 			boardCast_list_copy_.erase(it);
// 			break;
// 		}
// 	}
// 
// 	for (it = boardCast_list_.begin(); it != boardCast_list_.end(); ++it){
// 		if (it->nBoardCastType == api_info.nBoardCastType && (char*)it->nSeatNo == seatNo)
// 		{
// 			it->isPlay = false;
// 			break;
// 		}
// 	}
	boost::lock_guard<boost::mutex> lock(cancel_mutex_);
	if (boardCast_list_.size() == 0)
	{
		return;
	}
	std::string seatNo = (char*)api_info.nSeatNo;
	if (seatNo == curr_play_seatNo && curr_play_boardType == api_info.nBoardCastType)
	{
		return;
	}

	//AppConfigHandler.Info("remove to  playList:" + to_string(api_info.nBoardCastType) + "---" + seatNo);
	Log::Info("remove to  playList: %d --- %s", api_info.nBoardCastType, seatNo.c_str());
	boardCast_list_cancel_.append(api_info);
}

void handleEvent::boardcastCycleThreadFunc()
{
	while (!is_thread_exit_)
	{
		ctrl_mutex_.lock();
// 		boardCast_list_ = boardCast_list_copy_;
// 		boardCast_list_copy_.clear();
// 		ctrl_mutex_.unlock();

		api_client_info_t api_info;
		for (int i = 0; i < boardCast_list_.size(); ++i)
		{
			int n = 0;
			std::string seatno = (char*)boardCast_list_[i].nSeatNo;
			curr_play_seatNo = seatno;
			curr_play_boardType = boardCast_list_[i].nBoardCastType;

			cancel_mutex_.lock();
			for (QList<api_boardcast_info_t>::iterator it = boardCast_list_cancel_.begin(); it != boardCast_list_cancel_.end(); ++it)
			 {
				std::string currSeatno = (char*)(boardCast_list_[i].nSeatNo);
				std::string canclSeatno = (char*)(it->nSeatNo);
				//AppConfigHandler.Info("current seatno : " + currSeatno + ", request type : " + to_string(boardCast_list_[i].nBoardCastType));
				//AppConfigHandler.Info("current cancel seatno : " + canclSeatno + ", request type : " + to_string(it->nBoardCastType));
				if ( currSeatno == canclSeatno && boardCast_list_[i].nBoardCastType == it->nBoardCastType)
				{
					n++;
					boardCast_list_cancel_.erase(it);
					break;
				}
			 }
			cancel_mutex_.unlock();

			if (n == 0)
			{
				std::string seatNo = (char*)boardCast_list_[i].nSeatNo;
				//AppConfigHandler.Info("current play item : " + seatNo);
				Log::Info("current play item : %s", seatNo.c_str());
				QList<	api_audio_info> list_ = boardCast_list_[i].getBoardCastType();
				for (int i = 0; i < list_.size(); ++i)
				{
					playBoardcastMsg(api_info, list_[i].mAudioFileName);
					//AppConfigHandler.Info("play file : " + list_[i].mAudioFileName);
					if (list_[i].isNum)
					{
						Sleep(600);
						close_mci();
					}
					else{
						waitforPlayFinished();
					}
				}
			}
		}
		boardCast_list_.clear();
		curr_play_seatNo = "";
		curr_play_boardType = -1;
		ctrl_mutex_.unlock();
		Sleep(1000);
	}
}

void handleEvent::waitforPlayFinished(int sec /*= 500*/)
{
	DWORD time = MCIStatus_music_length();
	DWORD curr_time;
	while (true)
	{
		curr_time = MCIStatus_music_pos();
		printf("%d ----- %d\n", curr_time, time);
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