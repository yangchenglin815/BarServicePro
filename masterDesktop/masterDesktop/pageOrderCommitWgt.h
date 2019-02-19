#ifndef PAGEORDERCOMMITWGT_H
#define PAGEORDERCOMMITWGT_H

#include <QWidget>
#include "ui_pageOrderCommitWgt.h"

class pageOrderCommitWgt : public QWidget
{
	Q_OBJECT

public:
	pageOrderCommitWgt(QWidget *parent = 0);
	~pageOrderCommitWgt();

private:
	Ui::pageOrderCommitWgt ui;
};

#endif // PAGEORDERCOMMITWGT_H
