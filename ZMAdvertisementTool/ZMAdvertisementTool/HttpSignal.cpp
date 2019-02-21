#include "HttpSignal.h"

HttpSignal* HttpSignal::m_pInstance = Q_NULLPTR;

HttpSignal *HttpSignal::instance()
{
	if (m_pInstance == Q_NULLPTR) {
		m_pInstance = new HttpSignal;
	}
	return m_pInstance;
}

HttpSignal::HttpSignal(QObject *parent) : QObject(parent)
{

}

HttpSignal::~HttpSignal()
{

}
