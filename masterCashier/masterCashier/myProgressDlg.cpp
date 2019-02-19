#include "myProgressDlg.h"
#include <qtmaterialcircularprogress.h>

myProgressDlg::myProgressDlg(QWidget *parent)
	: QDialog(parent)
	, m_progress(new QtMaterialCircularProgress)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);

	QVBoxLayout *layout = new QVBoxLayout;
	ui.widget->setLayout(layout);
	layout->addWidget(m_progress);
	layout->setAlignment(m_progress, Qt::AlignCenter);
}

myProgressDlg::~myProgressDlg()
{

}
