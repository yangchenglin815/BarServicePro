#include "paymentDialog.h"

paymentDialog::paymentDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint /*| Qt::WindowStaysOnTopHint*/);
	setAttribute(Qt::WA_TranslucentBackground, true);

	InitConnect();
}

paymentDialog::~paymentDialog()
{

}

void paymentDialog::Init(DiscountPkg_t discountPkg, QString feeCode)
{
	m_discountPkg = discountPkg;
	m_feeCode = feeCode;

	ui.lbl_sum->setText(QString("%1").arg(discountPkg.dOriginalPrice) + tr("yuan"));
	ui.lbl_content->hide();
	ui.wgt_code->generateString(feeCode);
}

void paymentDialog::InitConnect()
{
	connect(ui.btn_close, SIGNAL(clicked()), this, SIGNAL(sigOnExit()));
}
