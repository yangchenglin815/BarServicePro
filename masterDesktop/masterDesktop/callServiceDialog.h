#ifndef CALLSERVICEDIALOG_H
#define CALLSERVICEDIALOG_H

#include <QDialog>
#include "ui_callServiceDialog.h"
#include "httpStructData.h"

class callServiceDialog : public QDialog
{
	Q_OBJECT

public:
	callServiceDialog(QWidget *parent = 0);
	~callServiceDialog();

	void InitCall(LoginUserInfo userInfo);
private slots:
    void slotOnCallService(int nCode, QString sMsg);
private:
	void InitConnect();
private:
	Ui::callServiceDialog ui;
};

#endif // CALLSERVICEDIALOG_H
