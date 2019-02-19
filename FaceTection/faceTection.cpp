#include "faceTection.h"
#include "appConfigIni.h"
#include "HttpAgent.h"
#include "webSocketAgent.h"
#include "logging.hpp"
#include "opencv2/opencv.hpp"
#include <stdint.h>

#include <QCloseEvent>
#include <QMessageBox>
#include <QDir>
#include <QUuid>

class Capture{
private:
	cv::VideoCapture cap;
public:
	void open()
	{
		close();
		cap.open(appConfigIni::getIndex().toInt());
	}
	void close()
	{
		cap.release();
	}
	bool isOpened() const{
		return cap.isOpened();
	}
	QImage capture(){
		if (isOpened())
		{
			cv::Mat frame;
			cap.read(frame);
			if (frame.dims == 2 && frame.channels() == 3)
			{
				int w = frame.cols;
				int h = frame.rows;
				QImage image(w, h, QImage::Format_RGB32);
				for (int i = 0; i < h; i++)
				{
					uint8_t const  *src = frame.ptr(i);
					QRgb *dst = (QRgb*)image.scanLine(i);
					for (int j = 0; j < w; j++)
					{
						dst[j] = qRgb(src[2], src[1], src[0]);
						src += 3;
					}
				}
				return image;
			}
		}
		return QImage();
	}
};

struct FaceTection::Private {
	Capture cap;
	QTimer timer;
};

FaceTection::FaceTection(QWidget *parent)
: QMainWindow(parent)
, m_counter(0)
{
	ui.setupUi(this);
	setWindowTitle(QStringLiteral("人脸检测"));
	setWindowIcon(QIcon(":/FaceTection/image/ico.ico"));

	//init Ini
	QString path = QApplication::applicationDirPath() + "/config/config.ini";
	appConfigIni::setIniPath(path);

	//init Timer
	m_counter_timer = new QTimer(this);
	m_counter_timer->setInterval(1000);

	//Init Controller
	InitStateMachine();
	InitCamera();
	InitConnect();

	webSocketAgent::instance()->initServer();
	HttpAgent::instance()->RequestOnCommitData(appConfigIni::getWSUrl(), appConfigIni::getWebPort());
}

FaceTection::~FaceTection()
{
	emit TranslateToInitState();
}

void FaceTection::slotOnGetCommitDataRespond(int nCode, QString sMsg)
{
	if (nCode != e_success)
	{
		Log::Info("Request On GetCommitData failed : %s", sMsg.toStdString().c_str());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		Log::Info("Request On GetCommitData success");
	}

	emit TranslateToInitState();
}

void FaceTection::slotOnGetFaceDetectionRespond(int nCode, QString sMsg, int nRespondCode)
{
	if (nCode != e_success)
	{
		m_counter = 0;
		ui.btn_select->setEnabled(true);
		Log::Info("Request On GetFaceDetection failed : %s", sMsg.toStdString().c_str());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		Log::Info("Request On GetFaceDetection success");
		if (nRespondCode == 0)
		{
			if (++m_counter < IMAGENUM)
			{
				//imageCapture_->capture();
				SavePicture(0, ui.lbl_render->pixmap()->toImage());
			}
		}
		else
		{
			m_counter = 0;
			ui.btn_select->setEnabled(true);
			QMessageBox::warning(NULL, "warning", QStringLiteral("人脸采集失败，请重新采集！"));
		}
	}
}

void FaceTection::slotOnGetCommitFaceDataRespond(int nCode, QString sMsg, int nRespondCode)
{
	ui.btn_select->setEnabled(true);
	if (nCode != e_success)
	{
		Log::Info("Request On GetCommitFaceData failed : %s", sMsg.toStdString().c_str());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		Log::Info("Request On GetCommitFaceData success");
		QMessageBox::warning(NULL, "warning", QStringLiteral("人脸数据上传成功！"));
		emit TranslateToInitState();
	}
}

void FaceTection::SavePicture(int, QImage image)
{
	image = image.convertToFormat(QImage::Format_RGB888);
	const QPixmap pixmap = QPixmap::fromImage(image);
	ui.lbl_capture->setPixmap(pixmap);

	QString path = QApplication::applicationDirPath() + "/image/" + QUuid::createUuid().toString() + ".jpg";
	bool ret = pixmap.save(path);
	if (ret)
	{
		HttpAgent::instance()->RequestOnFaceDetection(path);
		return;
	}
	ui.btn_select->setEnabled(true);
}

void FaceTection::slotOnCounter()
{
	QDir dir(QApplication::applicationDirPath() + "/image");
	dir.setFilter(QDir::Files);
	int fileCount = dir.count();
	if (m_counter == IMAGENUM && fileCount == IMAGENUM)
	{
		QMessageBox::about(NULL, "tip", QStringLiteral("人脸采集成功，正在上传数据！"));
		HttpAgent::instance()->RequestOnCommitFaceData(idCard_, userName_);
		m_counter = 0;
	}
}

void FaceTection::slotOnStartToSelect()
{
	ui.btn_select->setEnabled(false);
	//imageCapture_->capture();

	SavePicture(0, ui.lbl_render->pixmap()->toImage());
}

void FaceTection::slotOnFaceTection(QString idCard, QString name)
{
	idCard_ = idCard;
	userName_ = name;

	emit TranslateToWorkState();
}

void FaceTection::onEnterInitState()
{
	Log::Info("StateMachine: InitState.");
	m_counter_timer->stop();
	CloseCamera();
	ui.lbl_capture->clear();
	clearFiles();
	hide();
}

void FaceTection::onEnterWorkState()
{
	Log::Info("StateMachine: WorkState.");
	m_counter_timer->start();
	OpenCamera();
	showNormal();
}

void FaceTection::doCapture()
{
	QImage image = pv->cap.capture();
	image = image.convertToFormat(QImage::Format_RGB888);
	const QPixmap pixmap = QPixmap::fromImage(image);
	ui.lbl_render->setPixmap(pixmap);
}

void FaceTection::closeEvent(QCloseEvent *event)
{
	emit TranslateToInitState();
	event->ignore();
}

void FaceTection::InitConnect()
{
	connect(ui.btn_select, SIGNAL(clicked()), this, SLOT(slotOnStartToSelect()));
	connect(m_counter_timer, SIGNAL(timeout()), this, SLOT(slotOnCounter()));
	//connect(imageCapture_, SIGNAL(imageCaptured(int, QImage)), this, SLOT(SavePicture(int, QImage)));
	connect(HttpAgent::instance(), SIGNAL(sigOnGetCommitDataRespond(int, QString)), this, SLOT(slotOnGetCommitDataRespond(int, QString)));
	connect(HttpAgent::instance(), SIGNAL(sigOnGetFaceDetectionRespond(int, QString, int)), this, SLOT(slotOnGetFaceDetectionRespond(int, QString, int)));
	connect(HttpAgent::instance(), SIGNAL(sigOnGetCommitFaceDataRespond(int, QString, int)), this, SLOT(slotOnGetCommitFaceDataRespond(int, QString, int)));
	connect(webSocketAgent::instance(), SIGNAL(sigOnFaceTection(QString, QString)), this, SLOT(slotOnFaceTection(QString, QString)));

	connect(&pv->timer, SIGNAL(timeout()), this, SLOT(doCapture()));
}

void FaceTection::InitCamera()
{
// 	camera_ = new QCamera;
// 	viewFinder_ = new QCameraViewfinder(ui.lbl_render);
// 	imageCapture_ = new QCameraImageCapture(camera_);
// 
// 	imageCapture_->setCaptureDestination(QCameraImageCapture::CaptureToFile);
// 	camera_->setCaptureMode(QCamera::CaptureStillImage);
// 	camera_->setViewfinder(viewFinder_);
// 
// 	camera_->start();

	pv = new Private();
	pv->timer.setInterval(0);
}

void FaceTection::InitStateMachine()
{
	stateMachine_ = new QStateMachine;
	InitState_ = new QState(stateMachine_);
	WorkState_ = new QState(stateMachine_);

	connect(InitState_, SIGNAL(entered()), this, SLOT(onEnterInitState()));
	connect(WorkState_, SIGNAL(entered()), this, SLOT(onEnterWorkState()));
	InitState_->addTransition(this, SIGNAL(TranslateToWorkState()), WorkState_);
	WorkState_->addTransition(this, SIGNAL(TranslateToInitState()), InitState_);

	stateMachine_->setInitialState(InitState_);
	stateMachine_->start();
}

void FaceTection::OpenCamera()
{
// 	if (camera_->state() != QCamera::StartingStatus)
// 	{
// 		camera_->start();
// 	}

	//opecv
	if (!pv->cap.isOpened())
	{
		pv->cap.open();
		pv->timer.start();
	}
}

void FaceTection::CloseCamera()
{
// 	if (camera_->state() != QCamera::StoppingStatus)
// 	{
// 		camera_->stop();
// 	}

	if (pv->cap.isOpened())
	{
		pv->cap.close();
		pv->timer.stop();
	}
}

void FaceTection::clearFiles()
{
	QDir dir(QApplication::applicationDirPath() + "/image");
	dir.setFilter(QDir::Files);
	int fileCount = dir.count();
	for (int i = 0; i < fileCount; i++)
	{
		dir.remove(dir[i]);
	}
}

bool FaceTection::DelDir(const QString &path)
{
	if (path.isEmpty()){
		return false;
	}
	QDir dir(path);
	if (!dir.exists()){
		return true;
	}
	dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤 
	QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息 
	foreach(QFileInfo file, fileList){ //遍历文件信息 
		if (file.isFile()){ // 是文件，删除 
			file.dir().remove(file.fileName());
		}
		else{ // 递归删除 
			DelDir(file.absoluteFilePath());
		}
	}
	return dir.rmpath(dir.absolutePath()); // 删除文件夹 
}
