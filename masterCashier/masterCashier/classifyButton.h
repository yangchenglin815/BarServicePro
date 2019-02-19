#ifndef CLASSIFYBUTTON_H
#define CLASSIFYBUTTON_H

#include <QPushButton>
#include "ui_classifyButton.h"

class classifyButton : public QPushButton
{
	Q_OBJECT

public:
	classifyButton(QWidget *parent = 0);
	~classifyButton();

	void initButton(const QString &text, const QString &style);
private:
	Ui::classifyButton ui;
};

#endif // CLASSIFYBUTTON_H
