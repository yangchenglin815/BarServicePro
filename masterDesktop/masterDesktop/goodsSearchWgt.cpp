#include "goodsSearchWgt.h"
#include "orderGoodsItemWgt.h"

goodsSearchWgt::goodsSearchWgt(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	// 分页组件的css,这个内容应该放到普通文件中然后加载
	QString qss = QString(".QLabel[page=\"true\"] { padding: 1px;font-family:%1;font-size:14px;color:#E6E6E6; }").arg(QStringLiteral("微软雅黑"))
		+ QString(".QLabel[currentPage=\"true\"] { color: #03C0EE;}")
		+ QString(".QLabel[page=\"true\"]:hover { color: white; border-radius: 4px; background-color: qlineargradient(spread:reflect, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(53, 121, 238, 255), stop:1 rgba(0, 202, 237, 255));}")
		+ QString("#label,#label_2{font-family:%1;font-size:14px;color:#656565;}").arg(QStringLiteral("微软雅黑"))
		+ QString("QLineEdit{font-family:%1;font-size:12px;color:#3A3A3A;border:1px solid rgba(0,0,0,0.05);background:#F5F5F5;border-radius:1px;}").arg(QStringLiteral("微软雅黑"))
		+ QString("#previousPageLabel,#nextPageLabel{background:#2EB4AF;border-radius:10px;color:#FFFFFF;}");
	ui.wgt_bottom->setStyleSheet(qss);
	ui.wgt_bottom->setType(1);

	ui.wgt_bottom->setMaxPage(20);
	ui.wgt_bottom->setCurrentPage(1);

	for (int i = 0; i < 2; ++i)
	{
		QListWidgetItem *item = new QListWidgetItem;
		item->setSizeHint(QSize(ui.listWidget->width(), 247));
		QWidget *wgt = new QWidget;

		QHBoxLayout *hLayout = new QHBoxLayout;
		hLayout->setSpacing(20);
		for (int i = 0; i < 6; ++i)
		{
			orderGoodsItemWgt *itemWgt = new orderGoodsItemWgt;
			hLayout->addWidget(itemWgt);
		}
		hLayout->addStretch();
		wgt->setLayout(hLayout);

		ui.listWidget->addItem(item);
		ui.listWidget ->setItemWidget(item, wgt);
	}
}

goodsSearchWgt::~goodsSearchWgt()
{

}
