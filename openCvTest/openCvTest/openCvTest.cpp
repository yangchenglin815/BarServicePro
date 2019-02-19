#include "openCvTest.h"
#include <windows.h>
#include <QImage>
#include "commonFun.h"
#include "HttpAgent.h"

#include <QFile>
#include <QDir>
#include <QUuid>
#include <enumData.h>
#include <QMessageBox>
#include <QCheckBox>
#include <QVBoxLayout>

openCvTest::openCvTest(QWidget *parent)
	: QMainWindow(parent)
	, m_counter(0)
{
	ui.setupUi(this);
// 	camera = new QCamera;//摄像头
// 	viewfinder = new QCameraViewfinder(ui.label);
// 	imageCapture = new QCameraImageCapture(camera);//截图部件
// 	camera->setViewfinder(viewfinder);

	//group = new QButtonGroup(this);
	QVBoxLayout *layout = new QVBoxLayout;
	foreach(QCameraInfo info, QCameraInfo::availableCameras()) {
		QCheckBox *box = new QCheckBox;
		/*connect(box, &QCheckBox::stateChanged, [&](int state){
			if (state == Qt::Checked)
			{
				camera = new QCamera(info);
				viewfinder = new QCameraViewfinder(ui.label);
				imageCapture = new QCameraImageCapture(camera);//截图部件
				camera->setViewfinder(viewfinder);
				QObject::connect(imageCapture, SIGNAL(imageCaptured(int, QImage)), this, SLOT(displayImage(int, QImage)));
				imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);
				camera->setCaptureMode(QCamera::CaptureStillImage);
				camera->setViewfinder(viewfinder);
				camera->start(); //启动摄像头
			}
		});*/
		//group->addButton(box);
		box->setText(info.description());
		layout->addWidget(box);
	}
	ui.wgt_dev->setLayout(layout);

	QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(ShowTheCapture()));
	QObject::connect(ui.SaveButton, SIGNAL(clicked()), this, SLOT(SavePicture()));
	QObject::connect(ui.btn_finished, SIGNAL(clicked()), this, SLOT(slotOnFunc()));
	connect(HttpAgent::instance(), SIGNAL(sigOnGetFaceDetectionRespond(int, QString, int)), this, SLOT(slotOnGetFaceDetectionRespond(int, QString, int)));
	connect(HttpAgent::instance(), SIGNAL(sigOnGetCommitFaceDataRespond(int, QString, int)), this, SLOT(slotOnGetCommitFaceDataRespond(int, QString, int)));
// 	QObject::connect(imageCapture, SIGNAL(imageCaptured(int, QImage)), this, SLOT(displayImage(int, QImage)));
// 	imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);
// 	camera->setCaptureMode(QCamera::CaptureStillImage);
// 	camera->setViewfinder(viewfinder);
// 	camera->start(); //启动摄像头

	m_counter_timer = new QTimer(this);
	m_counter_timer->setInterval(1000);
	connect(m_counter_timer, SIGNAL(timeout()), this, SLOT(slotOnCounter()));
	//m_counter_timer->start();

	//cam = NULL;
	timer = new QTimer(this);
	imag = new QImage();         // 初始化

	/*信号和槽*/
	connect(timer, SIGNAL(timeout()), this, SLOT(readFrame()));  // 时间到，读取当前摄像头信息
	connect(ui.btn_open, SIGNAL(clicked()), this, SLOT(slotOnOpen()));
	connect(ui.btn_close, SIGNAL(clicked()), this, SLOT(slotOnClose()));
	connect(ui.btn_cut, SIGNAL(clicked()), this, SLOT(slotOnCut()));

	//HttpAgent::instance()->RequestOnGetFaceDetectionRespond();
}

openCvTest::~openCvTest()
{

}

void openCvTest::slotOnOpen()
{
	//cam = cvCreateCameraCapture(0);
	timer->start(1000);
}

void openCvTest::slotOnClose()
{
	timer->stop();
	//cvReleaseCapture(&cam);
}

void openCvTest::slotOnCut()
{
	//frame = cvQueryFrame(cam);
	//QImage image((const uchar*)frame->imageData, frame->width, frame->height, QImage::Format_RGB888);
//	ui.label_2->setPixmap(QPixmap::fromImage(image));
}

void openCvTest::readFrame()
{
	//frame = cvQueryFrame(cam);
//	QImage image((const uchar*)frame->imageData, frame->width, frame->height, QImage::Format_RGB888);
	//ui.label->setPixmap(QPixmap::fromImage(image));
	//timer->setInterval(33);
}

void openCvTest::ShowTheCapture()
{
	imageCapture->capture();
}

void openCvTest::SavePicture()
{
	const QPixmap *pixmap = ui.label_2->pixmap();
	if (pixmap)
	{
		QString path = QApplication::applicationDirPath() + "/image/" + QUuid::createUuid().toString() + ".jpg";
		bool ret = pixmap->save(path);
		if (ret)
		{
			HttpAgent::instance()->RequestOnFaceDetection(path);
		}
		qDebug() << "save success" << endl;
		qDebug() << ret;
	}
}

void openCvTest::displayImage(int, QImage image)
{
	image = image.convertToFormat(QImage::Format_RGB888);
	ui.label_2->setPixmap(QPixmap::fromImage(image));
	QImage::Format format = image.format();
	qDebug() << (int)format;

	SavePicture();
}

void openCvTest::slotOnFunc()
{
	clearFiles();

	imageCapture->capture();
}

void openCvTest::slotOnGetFaceDetectionRespond(int nCode, QString sMsg, int nRespondCode)
{
	if (nCode != e_success)
	{
		//AppConfigHandler.Info("Request On GetGoodsInfo failed : " + sMsg.toStdString());
		m_counter = 0;
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		//AppConfigHandler.Info("Request On GetGoodsInfo success");
		if (nRespondCode == 0)
		{
			if (++m_counter < 5)
			{
				imageCapture->capture();
			}
		}
		else
		{
			m_counter = 0;
			QMessageBox::warning(NULL, "warning", QStringLiteral("人脸采集失败，请重新采集！"));
		}
	}
}

void openCvTest::slotOnGetCommitFaceDataRespond(int nCode, QString sMsg, int nRespondCode)
{
	if (nCode != e_success)
	{
		//AppConfigHandler.Info("Request On GetGoodsInfo failed : " + sMsg.toStdString());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		QMessageBox::warning(NULL, "warning", QStringLiteral("人脸数据上传成功！"));
	}
}

void openCvTest::slotOnCounter()
{
	QDir dir(QApplication::applicationDirPath() + "/image");
	dir.setFilter(QDir::Files);
	int fileCount = dir.count();
	if (m_counter == 5 && fileCount == 5)
	{
		QMessageBox::about(NULL, "tip", QStringLiteral("人脸采集成功，正在上传数据！"));
		HttpAgent::instance()->RequestOnCommitFaceData("88888888", "ycl");
		m_counter = 0;
	}
}

void openCvTest::clearFiles()
{
	QDir dir(QApplication::applicationDirPath() + "/image");
	dir.setFilter(QDir::Files);
	int fileCount = dir.count();
	for (int i = 0; i < fileCount; i++)
		dir.remove(dir[i]);
}
