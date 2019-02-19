#ifndef FACETECTION_H
#define FACETECTION_H

#include <QtWidgets/QMainWindow>
#include "ui_faceTection.h"
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QCameraInfo>
#include <QTimer>
#include <QStateMachine>
#include <QState>

const int IMAGENUM = 5;   //单次传图片张数

class FaceTection : public QMainWindow
{
	Q_OBJECT

public:
	explicit FaceTection(QWidget *parent = 0);
	~FaceTection();
private slots:
	void slotOnGetCommitDataRespond(int nCode, QString sMsg);
	void slotOnGetFaceDetectionRespond(int nCode, QString sMsg, int nRespondCode);
	void slotOnGetCommitFaceDataRespond(int nCode, QString sMsg, int nRespondCode);

	void SavePicture(int, QImage image);
	void slotOnCounter();
	void slotOnStartToSelect();
	void slotOnFaceTection(QString idCard, QString name);

	void onEnterInitState();
	void onEnterWorkState();
	void doCapture();
signals:
	void TranslateToInitState();
	void TranslateToWorkState();
protected:
	void closeEvent(QCloseEvent *event);
private:
	void InitConnect();
	void InitCamera();
	void InitStateMachine();

	void OpenCamera();
	void CloseCamera();

	void clearFiles();
	bool DelDir(const QString &path);
private:
	Ui::FaceTectionClass ui;
	QCamera *camera_;
	QCameraViewfinder *viewFinder_;
	QCameraImageCapture *imageCapture_;

	QTimer    *m_counter_timer;
	int m_counter;
	QString idCard_;
	QString userName_;

	//状态机
	QStateMachine *stateMachine_;  
	QState *InitState_;
	QState *WorkState_;

	struct Private;
	Private *pv;
};

#endif // FACETECTION_H
