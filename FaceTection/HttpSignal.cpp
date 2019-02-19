#include "HttpSignal.h"

HttpSignal* HttpSignal::m_pInstance = NULL;

HttpSignal *HttpSignal::instance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new HttpSignal;
	}
	return m_pInstance;
}

HttpSignal::HttpSignal(QObject *parent)
	: QObject(parent)
{

}

HttpSignal::~HttpSignal()
{

}
