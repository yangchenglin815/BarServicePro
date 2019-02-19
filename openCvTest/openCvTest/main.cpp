#include "openCvTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	openCvTest w;
	w.show();
	return a.exec();
}
