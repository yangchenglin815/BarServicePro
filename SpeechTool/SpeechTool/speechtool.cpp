#include "speechtool.h"
#include <QMessageBox>
#include <QDebug>

SpeechTool::SpeechTool(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_common_btnGroup = new QButtonGroup;
	m_common_btnGroup->addButton(ui.btn_man, 1);
	m_common_btnGroup->addButton(ui.btn_women, 2);
	m_common_btnGroup->addButton(ui.btn_superman, 3);
	m_common_btnGroup->addButton(ui.btn_superwomen, 4);

	connect(m_common_btnGroup, SIGNAL(buttonToggled(int, bool)), this, SLOT(slotOnButtonToggled(int, bool)));
	connect(ui.btn_compose, SIGNAL(clicked()), this, SLOT(slotOnCompose()));

}

SpeechTool::~SpeechTool()
{

}

void SpeechTool::slotOnButtonToggled(int id, bool checked)
{
	if (checked)
	{
		qDebug() << id;
	}
}

void SpeechTool::slotOnCompose()
{

}
