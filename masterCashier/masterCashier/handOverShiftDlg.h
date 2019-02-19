#ifndef HANDOVERSHIFTDLG_H
#define HANDOVERSHIFTDLG_H

#include <QDialog>
#include "ui_handOverShiftDlg.h"

class handOverShiftDlg : public QDialog
{
	Q_OBJECT

public:
	handOverShiftDlg(QWidget *parent = 0);
	~handOverShiftDlg();

private:
	Ui::handOverShiftDlg ui;
};

#endif // HANDOVERSHIFTDLG_H
