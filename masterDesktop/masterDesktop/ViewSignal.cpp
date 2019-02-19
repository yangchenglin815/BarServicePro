#include "ViewSignal.h"

ViewSignal* ViewSignal::m_pInstance = NULL;

ViewSignal* ViewSignal::instance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new ViewSignal;
	}
	return m_pInstance;
}

ViewSignal::ViewSignal(QObject *parent)
	: QObject(parent)
{

}

ViewSignal::~ViewSignal()
{

}
