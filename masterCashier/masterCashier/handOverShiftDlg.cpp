#include "handOverShiftDlg.h"

handOverShiftDlg::handOverShiftDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	connect(ui.btn_close, SIGNAL(clicked()), this, SLOT(accept()));
}

handOverShiftDlg::~handOverShiftDlg()
{

}
