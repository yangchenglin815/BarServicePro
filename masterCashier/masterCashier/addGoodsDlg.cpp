#include "addGoodsDlg.h"
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QKeyEvent>

addGoodsDlg::addGoodsDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);
}

addGoodsDlg::~addGoodsDlg()
{

}

void addGoodsDlg::initUI()
{
// 	QLabel *goodsName = new QLabel(this);
// 	goodsName->setStyleSheet(QString("font-family:%1;font-size:14px;color:rgba(0,0,0,0.9);").arg(QStringLiteral("微软雅黑")));
// 	goodsName->setText(QStringLiteral("%1").arg(m_goodsInfo.sName));
// 	goodsName->move(20, 30);
	ui.lbl_goodsName->setText(QStringLiteral("%1").arg(m_goodsInfo.sName));

// 	QScrollArea *areaWgt = new QScrollArea(this);
// 	areaWgt->setStyleSheet("QScrollArea{background:#FFFFFF;border:none;}");
// 	areaWgt->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
// 	areaWgt->resize(QSize(this->width(), 210));
// 	areaWgt->move(0, 70);
// 
// 	QWidget *wgt = new QWidget;
// 	wgt->setStyleSheet("QWidget{background:#FFFFFF;border:none;}");
// 	areaWgt->setWidget(wgt);

	int num = 0;
	printf("tasteGroup size:%d\n", m_goodsInfo.nTasteGroupList.size());
	for (int i = 0; i < m_goodsInfo.nTasteGroupList.size(); ++i)
	{
		TasteGroupInfo_t tasteGroupInfo;
		tasteGroupInfo.id = m_goodsInfo.nTasteGroupList[i].id;
		tasteGroupInfo.sName = m_goodsInfo.nTasteGroupList[i].sName;

		QLabel *label = new QLabel(ui.wgt_area);
		label->setStyleSheet(QString("font-family:%1;font-size:14px;color:rgba(0,0,0,0.8);").arg(QStringLiteral("微软雅黑")));
		label->setText(QStringLiteral("%1：").arg(m_goodsInfo.nTasteGroupList[i].sName));
		label->move(20, 20 + 60*i);

		QButtonGroup *buttonGroup = new QButtonGroup(this);

		QString tasteGroups = m_goodsInfo.nTasteGroupList[i].sOpt;
		QStringList tasteList = tasteGroups.split(",");
		if (num < tasteList.size())
		{
			num = tasteList.size();
		}
		if (tasteList.size() > 0)
		{
			tasteGroupInfo.sOpt = tasteList[0];
			for (int j = 0; j < tasteList.size(); ++j)
			{
				checkButton *button = new checkButton(ui.wgt_area);
				button->setId(m_goodsInfo.nTasteGroupList[i].id);
				connect(button, SIGNAL(toggled(bool)), this, SLOT(slotOnTasteSelected(bool)));
				button->setText(QStringLiteral("%1").arg(tasteList[j]));
				if (j == 0)
				{
					button->setChecked(true);
				}
				button->move(label->x() + label->width() + 100 * j - 30, 12 + 60 * i);
				buttonGroup->addButton(button);
				m_buttonList.append(button);
			}
		}
		m_goodsInfo.nSelectedTasteList.append(tasteGroupInfo);
	}
	ui.wgt_area->setMinimumHeight(m_goodsInfo.nTasteGroupList.size() * 60);
	ui.wgt_area->setMinimumWidth((num + 1) * 90 + 20 + num * 8);

	QLabel *unit_Label = new QLabel(this);
	unit_Label->setText(QStringLiteral("¥"));
	unit_Label->setStyleSheet(QString("font-family:%1;font-size:10px;color:#EC0000;").arg(QStringLiteral("微软雅黑")));
	unit_Label->move(40, 311);

	m_sum_label = new QLabel(this);
	m_sum_label->setText(QStringLiteral("%1").arg(m_goodsInfo.dPrice));
	m_sum_label->setStyleSheet(QString("font-family:%1;font-size:18px;color:#EC0000;").arg(QStringLiteral("微软雅黑")));
	m_sum_label->move(unit_Label->x() +10, 311);

	m_reduce_button = new QPushButton(this);
	connect(m_reduce_button, SIGNAL(clicked()), this, SLOT(slotOnReduceGoods()));
	m_reduce_button->resize(QSize(20, 20));
	m_reduce_button->move(m_sum_label->x() + 52, 316);
	m_reduce_button->setStyleSheet("QPushButton{border-image:url(:/masterCashier/Resources/reduce_normal.png);}QPushButton:hover{border-image:url(:/masterCashier/Resources/reduce_hover.png);}QPushButton:pressed{border-image:url(:/masterCashier/Resources/reduce_pressed.png);}");

	m_sum_label = new QLabel(this);
	m_sum_label->setText(QStringLiteral("%1").arg(m_goodsInfo.nOrderNum));
	m_sum_label->setStyleSheet(QString("font-family:%1;font-size:16px;color:rgba(0,0,0,0.9);").arg(QStringLiteral("微软雅黑")));
	m_sum_label->move(m_reduce_button->x()+30, 310);

	m_add_button = new QPushButton(this);
	connect(m_add_button, SIGNAL(clicked()), this, SLOT(slotOnAddGoods()));
	m_add_button->resize(QSize(20, 20));
	m_add_button->move(m_sum_label->x() + 20, 316);
	m_add_button->setStyleSheet("QPushButton{border-image:url(:/masterCashier/Resources/add_normal.png);}QPushButton:hover{border-image:url(:/masterCashier/Resources/add_hover.png);}QPushButton:pressed{border-image:url(:/masterCashier/Resources/add_pressed.png);}");

	m_finshed_button = new QPushButton(this);
	m_finshed_button->resize(QSize(120, 40));
	m_finshed_button->setText(QStringLiteral("添加"));
	m_finshed_button->setStyleSheet(QString("QPushButton{font-family:%1;font-size:14px;color:#FFFFFF;background:#00B900;border:none;border-radius:4px;}QPushButton:pressed{margin:1px;}").arg(QStringLiteral("微软雅黑")));
	m_finshed_button->move(this->width() - m_finshed_button->width() - 40, 300);
	connect(m_finshed_button, SIGNAL(clicked()), this, SLOT(slotOnFinishAddGoods()));

	m_cancel_button = new QPushButton(this);
	m_cancel_button->resize(QSize(120, 40));
	m_cancel_button->setText(QStringLiteral("取消"));
	m_cancel_button->setStyleSheet(QString("QPushButton{font-family:%1;font-size:14px;color:#FFFFFF;background:#EC0000;border:none;border-radius:4px;}QPushButton:pressed{margin:1px;}").arg(QStringLiteral("微软雅黑")));
	m_cancel_button->move(m_finshed_button->x() - m_cancel_button->width() - 10, 300);
	connect(m_cancel_button, SIGNAL(clicked()), this, SIGNAL(sigOnCloseDlg()));
}

void addGoodsDlg::setData(const GoodsInfo_t &goodsInfo)
{
	m_goodsInfo = goodsInfo;
	initUI();
}

void addGoodsDlg::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter){
		slotOnFinishAddGoods();
	}
}

void addGoodsDlg::slotOnAddGoods()
{
	if (m_goodsInfo.nOrderNum < m_goodsInfo.nStockNum || m_goodsInfo.nType == 2)
	{
		m_goodsInfo.nOrderNum++;
		m_sum_label->setText(QString::number(m_goodsInfo.nOrderNum));
		m_goodsInfo.dTotalPrice = m_goodsInfo.nOrderNum * m_goodsInfo.dPrice;
	}
}

void addGoodsDlg::slotOnReduceGoods()
{
	if (m_goodsInfo.nOrderNum > 1)
	{
		m_goodsInfo.nOrderNum--;
		m_sum_label->setText(QString::number(m_goodsInfo.nOrderNum));
		m_goodsInfo.dTotalPrice = m_goodsInfo.nOrderNum * m_goodsInfo.dPrice;
	}
}

void addGoodsDlg::slotOnFinishAddGoods()
{
	emit sigOnFinishAddGoods(m_goodsInfo);
}

void addGoodsDlg::slotOnTasteSelected(bool checked)
{
	if (checked)
	{
		checkButton *button = (checkButton*)sender();
		for (int i = 0; i < m_goodsInfo.nSelectedTasteList.size(); ++i)
		{
			if (m_goodsInfo.nSelectedTasteList[i].id == button->getId())
			{
				m_goodsInfo.nSelectedTasteList[i].sOpt = button->text();
			}
		}
	}
}

