#include "EventHandler.h"
#include <stdexcept>
#include <exception>
#include <stdio.h>
#include "logging.hpp"

namespace dm {
	namespace server {

		/// 单例声明
		/// 类内 class static 除了在class 之内声明，还需要在class 之外定义.
		auto_ptr<EventHandler> EventHandler::auto_ptr_instance;
EventHandler::EventHandler()
{

}

EventHandler::~EventHandler()
{

}

EventHandler& EventHandler::GetInstance(void)
{
	if (auto_ptr_instance.get() == 0) {
		auto_ptr_instance.reset(new EventHandler());
	}
	return *(auto_ptr_instance.get());
}

void EventHandler::rebootClientHandler()
{
	char err[200];
	memset(err, 0, sizeof(err));

	std::string cmd = "shutdown /r /t 1";
	Log::Info("reboot cmd exec : %s", cmd.c_str());
	if (system(cmd.c_str()))
	{
		perror(err);
		Log::Info("reboot cmd exc failed : %s", err);
	}
}

void EventHandler::shutdownClientHandler()
{
	char err[200];
	memset(err, 0, sizeof(err));

	std::string cmd = "shutdown /s /t 1";
	Log::Info("shutdown cmd exec : %s", cmd.c_str());
	if (system(cmd.c_str()))
	{
		perror(err);
		Log::Info("shutdown cmd exc failed : %s", err);
	}
}

	}  // namespace server
} // namespace dm