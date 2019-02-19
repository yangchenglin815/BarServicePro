#include "pageGoodsWidget.h"
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QMessageBox>

#include "orderGoodsItemWgt.h"
#include "HttpAgent.h"
#include "logging.hpp"
#include "ViewSignal.h"

pageGoodsWidget::pageGoodsWidget(QWidget *parent)
	: QWidget(parent)
	, m_curr_page(1)
	, m_total_page(1)
{
	ui.setupUi(this);

	// 分页组件的css,这个内容应该放到普通文件中然后加载
	QString qss = QString(".QLabel[page=\"true\"] { padding: 1px;font-family:%1;font-size:14px;color:#E6E6E6; }").arg(QStringLiteral("微软雅黑"))
		+ QString(".QLabel[currentPage=\"true\"] { color: #03C0EE;}")
		+ QString(".QLabel[page=\"true\"]:hover { color: white; border-radius: 4px; background-color: qlineargradient(spread:reflect, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(53, 121, 238, 255), stop:1 rgba(0, 202, 237, 255));}")
		+ QString("#label,#label_2{font-family:%1;font-size:14px;color:#656565;}").arg(QStringLiteral("微软雅黑"))
		+ QString("QLineEdit{font-family:%1;font-size:12px;color:#3A3A3A;border:1px solid rgba(0,0,0,0.05);background:#F5F5F5;border-radius:1px;}").arg(QStringLiteral("微软雅黑"))
		+ QString("#previousPageLabel,#nextPageLabel{background:#2EB4AF;border-radius:10px;color:#FFFFFF;}");
	ui.widget->setStyleSheet(qss);
	ui.widget->setType(1); 

	InitConnect();
}

pageGoodsWidget::~pageGoodsWidget()
{

}

void pageGoodsWidget::InitData()
{
	HttpAgent::instance()->RequestOnGetClassifyGroups(t_orderPage);
}

void pageGoodsWidget::slotOnGetClassifyGroups(int nCode, QString sMsg, QList<ClassifyGroupInfo_t> classifyGroupList, int nPage)
{
	if (nPage != t_orderPage)
	{
		return;
	}

	if (nCode != e_success)
	{
		Log::Info("Request On pageSale GetClassifyGroups failed : %s", sMsg.toStdString().c_str());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		Log::Info("Request On pageSale GetClassifyGroups success");
		m_classifyGroupList = classifyGroupList;
		InitClassify();
		HttpAgent::instance()->RequestOnGetGoodsInfo("", t_orderPage);
	}
}

void pageGoodsWidget::slotOnGetGoodsInfo(int nCode, QString sMsg, QList<GoodsInfo_t> goodsInfoList)
{
	if (nCode != e_success)
	{
		Log::Info("Request On GetGoodsInfo failed : %s", sMsg.toLocal8Bit().data());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		Log::Info("Request On GetGoodsInfo success");
		m_goodsInfoList = goodsInfoList;
		InitGoodsList();
	}
}

void pageGoodsWidget::slotOnCurrPageChanged(int nPage)
{
	m_curr_page = nPage;
	InitGoodsList();
}

void pageGoodsWidget::slotOnClassifyItemClicked(QListWidgetItem *item)
{
	auto var = item->data(Qt::UserRole);
	m_classifyId = var.toString();
	m_curr_page = 1;
	InitGoodsList();
}

void pageGoodsWidget::slotOnGoodsNumChanged(GoodsInfo_t goodsInfo)
{
	for (int i = 0; i < m_goodsInfoList.size(); ++i)
	{
		if (m_goodsInfoList[i].nId == goodsInfo.nId)
		{
			m_goodsInfoList[i].nOrderNum = goodsInfo.nOrderNum;
			break;
		}
	}

	for (int i = 0; i < m_goodItemGroup.size(); ++i)
   {

		if (m_goodItemGroup[i]->getData().nId == goodsInfo.nId)
		{
			m_goodItemGroup[i]->setNum(goodsInfo.nOrderNum);
			break;
		}
   }
}

void pageGoodsWidget::slotOnClearShoppingCart()
{
	for (int i = 0; i < m_goodItemGroup.size(); ++i)
	{

		m_goodItemGroup[i]->setNum(0);
	}

	for (int i = 0; i < m_goodsInfoList.size(); ++i)
	{
		m_goodsInfoList[i].nOrderNum = 0;
	}
}

void pageGoodsWidget::InitConnect()
{
	qRegisterMetaType<QList<ClassifyGroupInfo_t> >("QList<ClassifyGroupInfo_t>");
	connect(HttpAgent::instance(), SIGNAL(sigOnGetClassifyGroups(int, QString, QList<ClassifyGroupInfo_t>, int)), this, SLOT(slotOnGetClassifyGroups(int, QString, QList<ClassifyGroupInfo_t>, int)));

	qRegisterMetaType<QList<GoodsInfo_t> >("QList<GoodsInfo_t>");
	connect(HttpAgent::instance(), SIGNAL(sigOnGetGoodsInfo(int, QString, QList<GoodsInfo_t>)), this, SLOT(slotOnGetGoodsInfo(int, QString, QList<GoodsInfo_t>)));

	connect(ui.widget, SIGNAL(currentPageChanged(int)), this, SLOT(slotOnCurrPageChanged(int)));
	connect(ui.listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slotOnClassifyItemClicked(QListWidgetItem*)));
	connect(ViewSignal::instance(), SIGNAL(sigOnGoodsNumChanged(GoodsInfo_t)), this, SLOT(slotOnGoodsNumChanged(GoodsInfo_t)));
	connect(ViewSignal::instance(), SIGNAL(sigOnClearShoppingCart()), this, SLOT(slotOnClearShoppingCart()));
}

void pageGoodsWidget::InitClassify()
{
	ui.listWidget->clear();
	if (m_classifyGroupList.size() > 0)
	{
		m_classifyId = m_classifyGroupList[0].id;
	}

	for (int i = 0; i < m_classifyGroupList.size(); ++i)
	{
		QListWidgetItem *item = new QListWidgetItem;
		item->setText(QStringLiteral("%1").arg(m_classifyGroupList[i].sName));
		item->setData(Qt::UserRole, QVariant(m_classifyGroupList[i].id));
		item->setSizeHint(QSize(170, 42));
		item->setTextAlignment(Qt::AlignCenter);
		ui.listWidget->addItem(item);
	}

	ui.listWidget->item(0)->setSelected(true);
}

void pageGoodsWidget::InitGoodsList()
{
	ui.list_goods->clear();
	m_goodItemGroup.clear();
	//获取当前分类商品集
	QList<GoodsInfo_t> goodsInfoList;
	for (int i = 0; i < m_goodsInfoList.size(); i++)
	{
		if (m_goodsInfoList[i].nClassifyId == m_classifyId)
		{
			goodsInfoList.append(m_goodsInfoList[i]);
		}
	}
	//获取分页总数
	if (goodsInfoList.size() % 10 == 0)
	{
		m_total_page = goodsInfoList.size() / 10;
	}
	else if (goodsInfoList.size() % 10 > 0)
	{
		m_total_page = goodsInfoList.size() / 10 + 1;
	}

	if (m_curr_page == m_total_page)   //最后一页
	{
		int extraNum = goodsInfoList.size() - (m_total_page - 1) * 10;   //最后一页剩余商品数量
		int rowNum;                                                     //最后一页行数
		if (extraNum % 5 == 0)
		{
			rowNum = extraNum / 5;
		}
		else
		{
			rowNum = extraNum / 5 + 1;
		}
		for (int i = 0; i < rowNum; ++i)
		{
			QListWidgetItem *item = new QListWidgetItem;
			item->setSizeHint(QSize(ui.list_goods->width(), 247));
			QWidget *wgt = new QWidget;

			QHBoxLayout *hLayout = new QHBoxLayout;
			hLayout->setSpacing(20);

		    if (rowNum == 1)   //只有一行的情况
		    {
				for (int j = 0; j < extraNum; ++j)
				{
					orderGoodsItemWgt *itemWgt = new orderGoodsItemWgt;
					connect(itemWgt, SIGNAL(sigOnAddGoodsItem(GoodsInfo_t)), this, SIGNAL(sigOnAddGoodsItem(GoodsInfo_t)));
					connect(itemWgt, SIGNAL(sigOnReduceGoodsItem(GoodsInfo_t)), this, SIGNAL(sigOnReduceGoodsItem(GoodsInfo_t)));
					itemWgt->setData(goodsInfoList[i * 5 + j + (m_curr_page - 1) * 10]);
					hLayout->addWidget(itemWgt);
					m_goodItemGroup.append(itemWgt);
				}
		    }
			else if (rowNum == 2)    //两行的情况
			{
				if (i == 0)   //第一行
				{
					for (int j = 0; j < 5; ++j)
					{
						orderGoodsItemWgt *itemWgt = new orderGoodsItemWgt;
						connect(itemWgt, SIGNAL(sigOnAddGoodsItem(GoodsInfo_t)), this, SIGNAL(sigOnAddGoodsItem(GoodsInfo_t)));
						connect(itemWgt, SIGNAL(sigOnReduceGoodsItem(GoodsInfo_t)), this, SIGNAL(sigOnReduceGoodsItem(GoodsInfo_t)));
						itemWgt->setData(goodsInfoList[i * 5 + j + (m_curr_page - 1) * 10]);
						hLayout->addWidget(itemWgt);
						m_goodItemGroup.append(itemWgt);
					}
				}
				else        //第二行
				{
					for (int j = 0; j < extraNum - 5; ++j)
					{
						orderGoodsItemWgt *itemWgt = new orderGoodsItemWgt;
						connect(itemWgt, SIGNAL(sigOnAddGoodsItem(GoodsInfo_t)), this, SIGNAL(sigOnAddGoodsItem(GoodsInfo_t)));
						connect(itemWgt, SIGNAL(sigOnReduceGoodsItem(GoodsInfo_t)), this, SIGNAL(sigOnReduceGoodsItem(GoodsInfo_t)));
						itemWgt->setData(goodsInfoList[i * 5 + j + (m_curr_page - 1) * 10]);
						hLayout->addWidget(itemWgt);
						m_goodItemGroup.append(itemWgt);
					}
				}
			}

			hLayout->addStretch();
			wgt->setLayout(hLayout);

			ui.list_goods->addItem(item);
			ui.list_goods->setItemWidget(item, wgt);
		}
	}
	else   //其他页
	{
		for (int i = 0; i < 2; ++i)
		{
			QListWidgetItem *item = new QListWidgetItem;
			item->setSizeHint(QSize(ui.list_goods->width(), 247));
			QWidget *wgt = new QWidget;

			QHBoxLayout *hLayout = new QHBoxLayout;
			hLayout->setSpacing(20);

			for (int j = 0; j < 5; j++)
			{
				orderGoodsItemWgt *itemWgt = new orderGoodsItemWgt;
				connect(itemWgt, SIGNAL(sigOnAddGoodsItem(GoodsInfo_t)), this, SIGNAL(sigOnAddGoodsItem(GoodsInfo_t)));
				connect(itemWgt, SIGNAL(sigOnReduceGoodsItem(GoodsInfo_t)), this, SIGNAL(sigOnReduceGoodsItem(GoodsInfo_t)));
				itemWgt->setData(goodsInfoList[i * 5 + j + (m_curr_page - 1) * 10]);
				hLayout->addWidget(itemWgt);
				m_goodItemGroup.append(itemWgt);
			}

			hLayout->addStretch();
			wgt->setLayout(hLayout);

			ui.list_goods->addItem(item);
			ui.list_goods->setItemWidget(item, wgt);
		}
	}

	ui.widget->setMaxPage(m_total_page);
	ui.widget->setCurrentPage(m_curr_page);
}
