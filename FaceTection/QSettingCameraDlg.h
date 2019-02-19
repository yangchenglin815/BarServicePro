#ifndef QSETTINGCAMERADLG_H
#define QSETTINGCAMERADLG_H

#include <QDialog>
#include <QCameraInfo>
#include "ui_QSettingCameraDlg.h"

class QSettingCameraDlg : public QDialog
{
	Q_OBJECT

public:
	QSettingCameraDlg(QWidget *parent = 0);
	~QSettingCameraDlg();

private:
	Ui::QSettingCameraDlg ui;
};

#endif // QSETTINGCAMERADLG_H
