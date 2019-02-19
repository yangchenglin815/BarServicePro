#ifndef ADDGOODSDLG_H
#define ADDGOODSDLG_H

#include <QDialog>
#include "ui_addGoodsDlg.h"
#include "httpStructData.h"
#include "checkButton.h"

class QLabel;
class QPushButton;
class addGoodsDlg : public QDialog
{
	Q_OBJECT

public:
	addGoodsDlg(QWidget *parent = 0);
	~addGoodsDlg();
    
	void initUI();
	void setData(const GoodsInfo_t &goodsInfo);
protected:
	void keyPressEvent(QKeyEvent *event);
signals:
	void sigOnCloseDlg();
	void sigOnFinishAddGoods(GoodsInfo_t goodsInfo);
private slots:
    void slotOnAddGoods();
    void slotOnReduceGoods();
	void slotOnFinishAddGoods();
	void slotOnTasteSelected(bool checked);
private:
	Ui::addGoodsDlg ui;
	QLabel *m_sum_label;
	QLabel *m_num_label;
	QPushButton *m_reduce_button;
	QPushButton *m_add_button;
	QPushButton *m_finshed_button;
	QPushButton *m_cancel_button;
	GoodsInfo_t m_goodsInfo;
	QList<checkButton*> m_buttonList;
};

#endif // ADDGOODSDLG_H
