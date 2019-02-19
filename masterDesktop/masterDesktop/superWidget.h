#ifndef SUPERWIDGET_H
#define SUPERWIDGET_H

#include <QWidget>

class superWidget : public QWidget
{
	Q_OBJECT

public:
	explicit superWidget(QWidget *parent);
	~superWidget();
protected:
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
private:
	
};

#endif // SUPERWIDGET_H
