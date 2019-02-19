#ifndef OPENCVTEST_H
#define OPENCVTEST_H

#include <QtWidgets/QMainWindow>
#include "ui_openCvTest.h"
#include <QTimer>
#include <QImage>
//#include "opencv/cv.h"
//#include "opencv/highgui.h"

#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QCameraInfo>
#include <QButtonGroup>

class openCvTest : public QMainWindow
{
	Q_OBJECT

public:
	openCvTest(QWidget *parent = 0);
	~openCvTest();
private slots:
	void slotOnOpen();
	void slotOnClose();
	void slotOnCut();
	void readFrame();

	void ShowTheCapture();
	void SavePicture();
	void displayImage(int, QImage image);
	void slotOnFunc();
	void slotOnGetFaceDetectionRespond(int nCode, QString sMsg, int nRespondCode);
	void slotOnGetCommitFaceDataRespond(int nCode, QString sMsg, int nRespondCode);
	void slotOnCounter();
private:
	void clearFiles();
private:
	Ui::openCvTestClass ui;
	QTimer    *timer;
	QTimer    *m_counter_timer;
	QImage    *imag;
	//CvCapture *cam;// 视频获取结构， 用来作为视频获取函数的一个参数
	//IplImage  *frame;//申请IplImage类型指针，就是申请内存空间来存放每一帧图像

	QCamera *camera;//摄像头
	QCameraViewfinder * viewfinder;//摄像头取景器部件
	QCameraImageCapture * imageCapture;//截图部件
	QList<QCameraInfo> infoList;
	QButtonGroup *group;
	int m_counter;
};

#endif // OPENCVTEST_H
