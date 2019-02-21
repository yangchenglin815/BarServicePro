#pragma once
#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "speech.h"
#include "constData.h"

using namespace aip;

class EventHandler
{
public:
	EventHandler();
	~EventHandler();

private:
	Speech *m_client;
};

#endif // EVENTHANDLER_H
