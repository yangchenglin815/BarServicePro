#include "faceTectionDlg.h"
#include <QDir>
#include <QUuid>
#include <HttpAgent.h>
#include <QMessageBox>
#include <appconfig.h>

using namespace dm::server;

faceTectionDlg::faceTectionDlg(QString idCard, QString userName, QWidget *parent)
	: QDialog(parent)
	, m_idCard(idCard)
	, m_userName(userName)
	, m_counter(0)
{
	ui.setupUi(this);
	setWindowTitle(QStringLiteral("人脸检测"));

	ui.pushButton->setRole(Material::Primary);
	ui.pushButton->setBackgroundMode(Qt::OpaqueMode);
	ui.pushButton->setOverlayStyle(Material::TintedOverlay);
	ui.pushButton->setFixedWidth(300);
	ui.pushButton->setText(QStringLiteral("开始人脸采集"));
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slotOnFunc()));
	connect(HttpAgent::instance(), SIGNAL(sigOnGetFaceDetectionRespond(int, QString, int)), this, SLOT(slotOnGetFaceDetectionRespond(int, QString, int)));
	connect(HttpAgent::instance(), SIGNAL(sigOnGetCommitFaceDataRespond(int, QString, int)), this, SLOT(slotOnGetCommitFaceDataRespond(int, QString, int)));

	camera = new QCamera;//摄像头
	viewfinder = new QCameraViewfinder(ui.label);
	imageCapture = new QCameraImageCapture(camera);//截图部件
	connect(imageCapture, SIGNAL(imageCaptured(int, QImage)), this, SLOT(SavePicture(int, QImage)));
	imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);
	camera->setCaptureMode(QCamera::CaptureStillImage);
	camera->setViewfinder(viewfinder);

	m_counter_timer = new QTimer(this);
	m_counter_timer->setInterval(1000);
	connect(m_counter_timer, SIGNAL(timeout()), this, SLOT(slotOnCounter()));
	m_counter_timer->start();
}

faceTectionDlg::~faceTectionDlg()
{
	
}

void faceTectionDlg::openCamera()
{
	camera->start();
}

void faceTectionDlg::closeCamera()
{
	camera->stop();
}

void faceTectionDlg::slotOnFunc()
{
	clearFiles();

	imageCapture->capture();
}

void faceTectionDlg::SavePicture(int, QImage image)
{
	image = image.convertToFormat(QImage::Format_RGB888);
	const QPixmap pixmap = QPixmap::fromImage(image);
	QString path = QApplication::applicationDirPath() + "/image/" + QUuid::createUuid().toString() + ".jpg";
	bool ret = pixmap.save(path);
	if (ret)
	{
		HttpAgent::instance()->RequestOnFaceDetection(path);
	}
}

void faceTectionDlg::slotOnCounter()
{
	QDir dir(QApplication::applicationDirPath() + "/image");
	dir.setFilter(QDir::Files);
	int fileCount = dir.count();
	if (m_counter == IMAGENUM && fileCount == IMAGENUM)
	{
		QMessageBox::about(NULL, "tip", QStringLiteral("人脸采集成功，正在上传数据！"));
		HttpAgent::instance()->RequestOnCommitFaceData(m_idCard, m_userName);
		m_counter = 0;
	}
}

void faceTectionDlg::slotOnGetFaceDetectionRespond(int nCode, QString sMsg, int nRespondCode)
{
	if (nCode != e_success)
	{
		//AppConfigHandler.Info("Request On GetFaceDetectionRespond failed : " + sMsg.toStdString());
		m_counter = 0;
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		//AppConfigHandler.Info("Request On GetFaceDetectionRespond success");
		if (nRespondCode == 0)
		{
			if (++m_counter < IMAGENUM)
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

void faceTectionDlg::slotOnGetCommitFaceDataRespond(int nCode, QString sMsg, int nRespondCode)
{
	if (nCode != e_success)
	{
		//AppConfigHandler.Info("Request On GetCommitFaceDataRespond failed : " + sMsg.toStdString());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		//AppConfigHandler.Info("Request On GetCommitFaceDataRespond success");
		QMessageBox::warning(NULL, "warning", QStringLiteral("人脸数据上传成功！"));
		closeCamera();
		hide();
	}
}

void faceTectionDlg::clearFiles()
{
	QDir dir(QApplication::applicationDirPath() + "/image");
	dir.setFilter(QDir::Files);
	int fileCount = dir.count();
	for (int i = 0; i < fileCount; i++)
	{
		dir.remove(dir[i]);
	}
}

// 删除文件夹
bool faceTectionDlg::DelDir(const QString &path)
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