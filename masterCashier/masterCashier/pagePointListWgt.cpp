#include "pagePointListWgt.h"
#include "shiftListItemWidget.h"
#include "HttpAgent.h"
#include "appconfig.h"
#include "commonFun.h"
#include <QMessageBox>
#include <QKeyEvent>

using namespace dm::server;

pagePointListWgt::pagePointListWgt(QWidget *parent)
: QWidget(parent)
, m_totalPage(1)
, m_currPage(1)
, m_itemNum(11)
{
	ui.setupUi(this);
	ui.checkBox->hide();
	initConnect();

	// 分页组件的css,这个内容应该放到普通文件中然后加载
	QString qss = QString(".QLabel[page=\"true\"] { padding: 1px;font-family:%1;font-size:14px;color:#656565; }").arg(QStringLiteral("微软雅黑"))
		+ QString(".QLabel[currentPage=\"true\"] { color: #03C0EE;}")
		+ QString(".QLabel[page=\"true\"]:hover { color: white; border-radius: 4px; background-color: qlineargradient(spread:reflect, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(53, 121, 238, 255), stop:1 rgba(0, 202, 237, 255));}")
		+ QString("#label,#label_2{font-family:%1;font-size:14px;color:#656565;}").arg(QStringLiteral("微软雅黑"))
		+ QString("QLineEdit{font-family:%1;font-size:12px;color:#3A3A3A;border:1px solid rgba(0,0,0,0.05);background:#F5F5F5;border-radius:1px;}").arg(QStringLiteral("微软雅黑"));
	ui.wgt_page->setStyleSheet(qss);
}

pagePointListWgt::~pagePointListWgt()
{

}

void pagePointListWgt::slotOnGetAllGoodsInfo(int nCode, QString sMsg, QList<GoodsInfo_t> goodsInfoList)
{
	if (nCode != e_success)
	{
		//AppConfigHandler.Info("Request On pageShift GetAllGoodsInfo failed : " + sMsg.toStdString());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		//AppConfigHandler.Info("Request On pageShift GetAllGoodsInfo success");
		m_goodsInfoList = goodsInfoList;
		initData(m_goodsInfoList);
	}
}

void pagePointListWgt::slotOnGetClassifyGroups(int nCode, QString sMsg, QList<ClassifyGroupInfo_t> classifyGroupList, int nPage)
{
	if (nPage != t_shiftPage)
	{
		return;
	}

	if (nCode != e_success)
	{
		//AppConfigHandler.Info("Request On pageShift GetClassifyGroups failed : " + sMsg.toStdString());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		//AppConfigHandler.Info("Request On pageShift GetClassifyGroups success");
		m_classifyGroupList = classifyGroupList;
		HttpAgent::instance()->RequestOnGetAllGoodsInfo();
	}
}

void pagePointListWgt::slotOnGoodsCheckUpResult(int nCode, QString sMsg)
{
	if (nCode != e_success)
	{
		//AppConfigHandler.Info("Request On GoodsCheckUpResult failed : " + sMsg.toStdString());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		//AppConfigHandler.Info("Request On GoodsCheckUpResult success");
		uint time = QDateTime::currentDateTime().toTime_t();
		HttpAgent::instance()->RequestOnGetShiftInfo(QString::number(time).append("000"));
		emit sigOnNextFunc();
	}
}

void pagePointListWgt::slotOnCurrentPageChanged(int page)
{
	initData(m_goodsInfoList, page);
}

void pagePointListWgt::slotOnNextFunc()
{
	QList<GoodsCheckUpInfo_t> goodsCheckUpList;
	for (int i = 0; i < m_goodsInfoList.size(); ++i)
	{
		GoodsCheckUpInfo_t goodsCheckUpInfo;
		goodsCheckUpInfo.nGoodsId = m_goodsInfoList[i].nId;
		goodsCheckUpInfo.dPrice = m_goodsInfoList[i].dPrice;
		goodsCheckUpInfo.nBarStock = m_goodsInfoList[i].nStockNum;
		goodsCheckUpInfo.nMainStock = 0;
		if (m_goodsInfoList[i].nCheckNum.isEmpty()){
			goodsCheckUpInfo.nCheckStock = m_goodsInfoList[i].nStockNum;
		}
		else{
			goodsCheckUpInfo.nCheckStock = m_goodsInfoList[i].nCheckNum.toInt();
		}
		goodsCheckUpList.append(goodsCheckUpInfo);
	}
	HttpAgent::instance()->RequestOnGoodsCheckUp(t_barStock, goodsCheckUpList, "");
}

void pagePointListWgt::slotOnUpdateCheckNum(GoodsInfo_t goodsInfo)
{
	for (int i = 0; i < m_goodsInfoList.size(); ++i)
	{
		if (m_goodsInfoList[i].nId == goodsInfo.nId)
		{
			m_goodsInfoList[i].nCheckNum = goodsInfo.nCheckNum;
			break;
		}
	}
}

void pagePointListWgt::slotOnSearchGoods(QString sIndex)
{
	int index = 0;
	QList<GoodsInfo_t> goodsInfoList;
	for (int i = 0; i < m_goodsInfoList.size(); ++i)
	{
		if (commonFun::getSearchResult(sIndex, m_goodsInfoList[i].sName, m_goodsInfoList[i].sCode))
		{
			index++;
			goodsInfoList.append(m_goodsInfoList[i]);
		}
	}

	if (index)
	{
		initData(goodsInfoList);
	}
	else
	{
		initData(m_goodsInfoList);
	}
}

void pagePointListWgt::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter){
		if (!ui.lineEdit->text().isEmpty())
		{
			slotOnSearchGoods(ui.lineEdit->text());
		}
	}
}

void pagePointListWgt::initConnect()
{
	connect(ui.wgt_page, SIGNAL(currentPageChanged(int)), this, SLOT(slotOnCurrentPageChanged(int)));
	connect(ui.btn_next, SIGNAL(clicked()), this, SLOT(slotOnNextFunc()));
	connect(ui.lineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotOnSearchGoods(QString)));

	connect(HttpAgent::instance(), SIGNAL(sigOnGoodsCheckUpResult(int, QString)), this, SLOT(slotOnGoodsCheckUpResult(int, QString)));

	qRegisterMetaType<QList<GoodsInfo_t> >("QList<GoodsInfo_t>");
	connect(HttpAgent::instance(), SIGNAL(sigOnGetAllGoodsInfo(int, QString, QList<GoodsInfo_t>)), this, SLOT(slotOnGetAllGoodsInfo(int, QString, QList<GoodsInfo_t>)));

	qRegisterMetaType<QList<ClassifyGroupInfo_t> >("QList<ClassifyGroupInfo_t>");
	connect(HttpAgent::instance(), SIGNAL(sigOnGetClassifyGroups(int, QString, QList<ClassifyGroupInfo_t>, int)), this, SLOT(slotOnGetClassifyGroups(int, QString, QList<ClassifyGroupInfo_t>, int)));
}

void pagePointListWgt::initData(QList<GoodsInfo_t> goodsInfoList, int pageNum /*= 1*/)
{
	if (goodsInfoList.size() == 0)
	{
		return;
	}

	ui.listWidget->clear();
	m_currPage = pageNum;
	m_totalPage = goodsInfoList.size() / m_itemNum;
	if (goodsInfoList.size() % m_itemNum != 0)
	{
		m_totalPage++;
	}

	if (m_currPage == m_totalPage)
	{
		int extraNum = goodsInfoList.size() % m_itemNum;
		for (int i = 0; i < extraNum; ++i){
			int j = i + (m_currPage - 1) * m_itemNum;
			goodsInfoList[j].sClassifyName = getClassifyNameById(goodsInfoList[j].nClassifyId);
			goodsInfoList[j].nFortId = i + 1;

			shiftListItemWidget *wgt = new shiftListItemWidget();
			qRegisterMetaType<GoodsInfo_t>("GoodsInfo_t");
			connect(wgt, SIGNAL(sigOnEditFinished(GoodsInfo_t)), this, SLOT(slotOnUpdateCheckNum(GoodsInfo_t)));
			wgt->setData(goodsInfoList[j]);
			QListWidgetItem *item = new QListWidgetItem();
			item->setSizeHint(QSize(ui.listWidget->width(), 36));
			ui.listWidget->addItem(item);
			ui.listWidget->setItemWidget(item, wgt);
		}
	}
	else
	{
		for (int i = 0; i < m_itemNum; ++i)
		{
			int j = i + (m_currPage - 1) * m_itemNum;
			goodsInfoList[j].sClassifyName = getClassifyNameById(goodsInfoList[j].nClassifyId);
			goodsInfoList[j].nFortId = i + 1;

			shiftListItemWidget *wgt = new shiftListItemWidget();
			qRegisterMetaType<GoodsInfo_t>("GoodsInfo_t");
			connect(wgt, SIGNAL(sigOnEditFinished(GoodsInfo_t)), this, SLOT(slotOnUpdateCheckNum(GoodsInfo_t)));
			wgt->setData(goodsInfoList[j]);
			QListWidgetItem *item = new QListWidgetItem();
			item->setSizeHint(QSize(ui.listWidget->width(), 36));
			ui.listWidget->addItem(item);
			ui.listWidget->setItemWidget(item, wgt);
		}
	}

	ui.wgt_page->setMaxPage(m_totalPage);
	ui.wgt_page->setCurrentPage(m_currPage);
}

QString pagePointListWgt::getClassifyNameById(QString id)
{
	for (int i = 0; i < m_classifyGroupList.size(); ++i)
	{
		if (m_classifyGroupList[i].id == id)
		{
			return m_classifyGroupList[i].sName;
		}
	}
	return "";
}
