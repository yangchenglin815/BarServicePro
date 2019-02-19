#ifndef COMPLAINTSUCCESSDLG_H
#define COMPLAINTSUCCESSDLG_H

#include <QDialog>
#include "ui_complaintSuccessDlg.h"

class complaintSuccessDlg : public QDialog
{
	Q_OBJECT

public:
	complaintSuccessDlg(QWidget *parent = 0);
	~complaintSuccessDlg();

private:
	Ui::complaintSuccessDlg ui;
};

#endif // COMPLAINTSUCCESSDLG_H
