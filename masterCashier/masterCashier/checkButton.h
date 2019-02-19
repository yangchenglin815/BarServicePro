#ifndef CHECKBUTTON_H
#define CHECKBUTTON_H

#include <QPushButton>

class checkButton : public QPushButton
{
	Q_OBJECT

public:
	explicit checkButton(QWidget *parent = 0);
	~checkButton();

	void setId(QString id);
	QString getId();
private:
	QString m_classId;
};

#endif // CHECKBUTTON_H
