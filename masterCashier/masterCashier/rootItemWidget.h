#ifndef ROOTITEMWIDGET_H
#define ROOTITEMWIDGET_H

#include <QWidget>
#include "ui_rootItemWidget.h"
#include "httpStructData.h"

class rootItemWidget : public QWidget
{
	Q_OBJECT

public:
	rootItemWidget(QWidget *parent = 0);
	~rootItemWidget();

	void setButtonChecked(bool checked);
	void setData(const ClassifyGroupInfo_t &classifyGroupInfo);
	QString getClassifyId();
private:
	Ui::rootItemWidget ui;
	ClassifyGroupInfo_t m_classifyGroupInfo;
};

#endif // ROOTITEMWIDGET_H
