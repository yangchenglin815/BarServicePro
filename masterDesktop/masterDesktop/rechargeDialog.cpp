#include "rechargeDialog.h"
#include "rechargeButton.h"
#include "discountItemWidget.h"

#include <QListWidgetItem>
#include <QTimer>
#include <QMessageBox>
#include "HttpAgent.h"
#include "logging.hpp"
#include "QFlowLayout.h"

rechargeDialog::rechargeDialog(QWidget *parent)
	: QDialog(parent)
	, m_listWidget(NULL)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint /*| Qt::WindowStaysOnTopHint*/);
	setAttribute(Qt::WA_TranslucentBackground, true);

	InitUI();
	InitConnect();
}

rechargeDialog::~rechargeDialog()
{

}

void rechargeDialog::InitDiscountPkg(LoginUserInfo_t userInfo)
{
	m_LoginUserInfo = userInfo;
	HttpAgent::instance()->RequestOnGetDiscountPkg(t_rechargePage);
}

void rechargeDialog::slotOnItemClicked(QListWidgetItem *item)
{
	for (int i = 0; i < m_listWidget->count(); ++i)
	{
		discountItemWidget *cItem = qobject_cast<discountItemWidget*>(m_listWidget->itemWidget(m_listWidget->item(i)));
		cItem->setChecked(false);
	}
	discountItemWidget *cItem_ = qobject_cast<discountItemWidget*>(m_listWidget->itemWidget(item));
	cItem_->setChecked(true);
	QTimer::singleShot(500, this, [&](){
		m_listWidget->hide();
	});
}

void rechargeDialog::slotOnShowSelectDiscount()
{
	if (m_listWidget != NULL)
	{
		m_listWidget->show();
	}
}

void rechargeDialog::slotOnGetDiscountPkg(int nCode, QString sMsg, QList<DiscountPkg_t> lDiscountPkgList)
{
	if (nCode != e_success)
	{
		Log::Info("Request On GetDiscountPkg failed : %s", sMsg.toLocal8Bit().data());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		Log::Info("Request On GetDiscountPkg success");
		m_discountPkgList = lDiscountPkgList;
		InitData();
	}
}

void rechargeDialog::slotOnNetFeeRecharge(int nCode, QString sMsg, QString sFeeCode)
{
	if (nCode != e_success)
	{
		Log::Info("Request On NetFeeRecharge failed : %s", sMsg.toLocal8Bit().data());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
		accept();
		m_paymentDlg->Init(m_curr_discountPkg, sFeeCode);
		m_paymentDlg->exec();
	}
	else
	{
		Log::Info("Request On NetFeeRecharge success");
		accept();
		m_paymentDlg->Init(m_curr_discountPkg, sFeeCode);
		m_paymentDlg->exec();
	}
}

void rechargeDialog::slotOnPayNetFee()
{
	HttpAgent::instance()->RequestOnNetFeeRecharge(m_LoginUserInfo, m_curr_discountPkg);
}

void rechargeDialog::slotOnDiscountPkgChanged(int id, bool checked)
{
	if (checked)
	{
		rechargeButton *p = static_cast<rechargeButton*>(m_buttonGroup->button(id));
		m_curr_discountPkg = p->getData();
	}
	else
	{
		m_curr_discountPkg.clear();
	}
}

void rechargeDialog::InitUI()
{
	m_buttonGroup = new QButtonGroup;
	m_paymentDlg = new paymentDialog;
	m_listWidget = new QListWidget(this);
	m_listWidget->setStyleSheet("QListWidget{background:#FFFFFF;border:1px solid #2EB4AF;outline:0px;}QListView::item:selected{background:#2EB4AF;}");
	m_listWidget->setFixedSize(QSize(160, 50 + 2));
	m_listWidget->move(264, 342);
	m_listWidget->hide();

	QListWidgetItem *item = new QListWidgetItem;
	discountItemWidget *itemWidget = new discountItemWidget;
	itemWidget->setData(QStringLiteral("不使用优惠"));
	item->setSizeHint(QSize(m_listWidget->width() - 2, 25));
	m_listWidget->addItem(item);
	m_listWidget->setItemWidget(item, itemWidget);
}

void rechargeDialog::InitConnect()
{
	connect(ui.btn_close, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui.btn_select, SIGNAL(clicked()), this, SLOT(slotOnShowSelectDiscount()));
	connect(m_listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slotOnItemClicked(QListWidgetItem*)));
	connect(ui.btn_pay, SIGNAL(clicked()), this, SLOT(slotOnPayNetFee()));
	connect(HttpAgent::instance(), SIGNAL(sigOnNetFeeRecharge(int, QString, QString)), this, SLOT(slotOnNetFeeRecharge(int, QString, QString)));
	connect(m_buttonGroup, SIGNAL(buttonToggled(int, bool)), this, SLOT(slotOnDiscountPkgChanged(int, bool)));
	connect(m_paymentDlg, &paymentDialog::sigOnExit, this, [&](){
		m_paymentDlg->accept();
	});

	qRegisterMetaType<QList<DiscountPkg_t> >("QList<DiscountPkg_t>");
	connect(HttpAgent::instance(), SIGNAL(sigOnGetDiscountPkgForRechargePage(int, QString, QList<DiscountPkg_t>)), this, SLOT(slotOnGetDiscountPkg(int, QString, QList<DiscountPkg_t>)));
}

void rechargeDialog::InitData()
{
	for (int i = 0; i < m_buttonGroup->buttons().size(); ++i)
	{
		m_buttonGroup->removeButton(m_buttonGroup->button(i));
	}

	QFlowLayout *layout = new QFlowLayout(QMargins(0, 30, 0, 30), 25, 25);
	for (int i = 0; i < m_discountPkgList.size(); ++i)
	{
		rechargeButton *button = new rechargeButton(this, m_discountPkgList[i]);
		m_buttonGroup->addButton(button, i);
		layout->addWidget(button);
	}
	ui.scrollArea->setLayout(layout);
}
