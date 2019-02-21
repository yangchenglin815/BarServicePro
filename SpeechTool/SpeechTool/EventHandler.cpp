#include "EventHandler.h"

EventHandler::EventHandler()
{
	m_client = new Speech(c_sAppId.toStdString(), c_sAppId.toStdString(), c_sApiKey.toStdString());
}

EventHandler::~EventHandler()
{

}
