#ifndef FACETECTIONDLG_H
#define FACETECTIONDLG_H

#include <QDialog>
#include "ui_faceTectionDlg.h"
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QCameraInfo>
#include <QTimer>

const int IMAGENUM = 5;   //单次传图片张数

class faceTectionDlg : public QDialog
{
	Q_OBJECT

public:
	explicit faceTectionDlg(QString idCard, QString userName, QWidget *parent = 0);
	~faceTectionDlg();
	void openCamera();
	void closeCamera();
private slots:
	void slotOnFunc();
	void SavePicture(int, QImage image);
	void slotOnCounter();
	void slotOnGetFaceDetectionRespond(int nCode, QString sMsg, int nRespondCode);
	void slotOnGetCommitFaceDataRespond(int nCode, QString sMsg, int nRespondCode);
private:
	void clearFiles();
	bool DelDir(const QString &path);
private:
	Ui::faceTectionDlg ui;
	QCamera *camera;//摄像头
	QCameraViewfinder * viewfinder;//摄像头取景器部件
	QCameraImageCapture * imageCapture;//截图部件
	QTimer    *m_counter_timer;
	int m_counter;
	//存储开卡人信息
	QString m_idCard;
	QString m_userName;
};

#endif // FACETECTIONDLG_H
