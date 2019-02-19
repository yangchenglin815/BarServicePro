#include "orderGoodsDialog.h"
#include "shoppingCartDlg.h"

orderGoodsDialog::orderGoodsDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	setAttribute(Qt::WA_TranslucentBackground, true);

	InitMask();
	InitConnect();
}

orderGoodsDialog::~orderGoodsDialog()
{

}

void orderGoodsDialog::InitPage(PageType pageType)
{
	switch (pageType)
	{
	case t_OrderOnline:
		ui.page_goods->InitData();
		ui.stackedWidget->setCurrentWidget(ui.page_goods);
		break;
	case t_OrderConfirm:
		ui.stackedWidget->setCurrentWidget(ui.page_order);
		break;
	case t_OrderCommit:
		ui.stackedWidget->setCurrentWidget(ui.page_commit_order);
		break;
	case t_OrderPay:
		ui.stackedWidget->setCurrentWidget(ui.page_pay_order);
		break;
	case t_GoodsSearch:
		ui.stackedWidget->setCurrentWidget(ui.page_search);
		break;
	default:
		break;
	}
}

void orderGoodsDialog::slotOnShowShoppingDlg()
{
	m_maskWgt->setGeometry(0, 67, this->width(), this->height() - 67 * 2);
	m_maskWgt->setStyleSheet("QWidget{background:rgba(0,0,0,0.6);}");
	m_maskWgt->show();

	m_shoppingCart_btn->setCheckedButton(true);
	m_shoppingCart_btn->setFixedSize(QSize(120, 48));
	m_shoppingCart_btn->move(845, 99);
	m_shoppingCart_btn->show();

	m_shoppingCart_dlg->move(650, 164);
	m_shoppingCart_dlg->exec();
}

void orderGoodsDialog::slotOnAddGoodsItem(GoodsInfo_t goodsInfo)
{
	m_shoppingCart_dlg->addToShoppingCart(goodsInfo);
}

void orderGoodsDialog::slotOnReduceGoodsItem(GoodsInfo_t goodsInfo)
{
	m_shoppingCart_dlg->reduceToShoppongCart(goodsInfo);
}

void orderGoodsDialog::slotOnShoppingCartChanged(int totalNum, double totalPrice)
{
	m_shoppingCart_btn->setData(totalNum, totalPrice);
	ui.btn_shopping->setData(totalNum, totalPrice);
}

void orderGoodsDialog::InitMask()
{
	m_maskWgt = new QWidget(this);
	m_maskWgt->hide();
	m_shoppingCart_btn = new superShoppingButton(this);
	m_shoppingCart_btn->hide();
	m_shoppingCart_dlg = new shoppingCartDlg(this);
	m_shoppingCart_dlg->hide();
}

void orderGoodsDialog::InitConnect()
{
	connect(ui.page_goods, SIGNAL(sigOnAddGoodsItem(GoodsInfo_t)), this, SLOT(slotOnAddGoodsItem(GoodsInfo_t)));
	connect(ui.page_goods, SIGNAL(sigOnReduceGoodsItem(GoodsInfo_t)), this, SLOT(slotOnReduceGoodsItem(GoodsInfo_t)));
	connect(m_shoppingCart_dlg, SIGNAL(sigOnShoppingCartChanged(int, double)), this, SLOT(slotOnShoppingCartChanged(int, double)));
	connect(ui.btn_shopping, SIGNAL(clicked()), this, SLOT(slotOnShowShoppingDlg()));
	connect(ui.btn_close, SIGNAL(clicked()), this, SLOT(accept()));
	connect(m_shoppingCart_btn, &superShoppingButton::clicked, [&](){
		m_shoppingCart_btn->hide();
		m_shoppingCart_dlg->accept();
		m_maskWgt->hide();
	});
}
