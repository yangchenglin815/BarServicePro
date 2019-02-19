#ifndef COMPLAINTDIALOG_H
#define COMPLAINTDIALOG_H

#include <QDialog>
#include "ui_complaintDialog.h"

class complaintDialog : public QDialog
{
	Q_OBJECT

public:
	complaintDialog(QWidget *parent = 0);
	~complaintDialog();
private:
	void InitConnect();
private:
	Ui::complaintDialog ui;
};

#endif // COMPLAINTDIALOG_H
