#ifndef SPEECHTOOL_H
#define SPEECHTOOL_H

#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_speechtool.h"
#include <QButtonGroup>

class SpeechTool : public QMainWindow
{
	Q_OBJECT

public:
	SpeechTool(QWidget *parent = 0);
	~SpeechTool();
private slots:
void slotOnButtonToggled(int id, bool checked);
void slotOnCompose();
private:
	Ui::SpeechToolClass ui;
	QButtonGroup *m_common_btnGroup;
};

#endif // SPEECHTOOL_H
