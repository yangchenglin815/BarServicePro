#ifndef MYPROGRESSDLG_H
#define MYPROGRESSDLG_H

#include <QDialog>
#include "ui_myProgressDlg.h"

class QtMaterialCircularProgress;
class myProgressDlg : public QDialog
{
	Q_OBJECT

public:
	myProgressDlg(QWidget *parent = 0);
	~myProgressDlg();

private:
	Ui::myProgressDlg ui;
	QtMaterialCircularProgress       *const m_progress;
};

#endif // MYPROGRESSDLG_H
