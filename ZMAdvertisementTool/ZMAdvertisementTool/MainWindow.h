#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include <QDateTime>
#include "HttpStructData.h"

class QGraphicsDropShadowEffect;
class QTimer;
class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	private slots:
	void slotOnClose();
	void anchorClickedSlot(const QUrl &url);
	void slotOnGetAdvertisementList(QList<AdvertisementInfo_t> advertisementInfoList, int nCode, QString sMsg);
	void slotOnCommitStatisticalData(int nCode, QString sMsg);
protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
private:
	void Init();
	void InitConnect();
	void getScreenInfo();
	void InitUi();

	void picToData(QString fromPic, QString toData, QString myformat);
	void dataToPic(QString fromData, QString toPic, QString myformat);
	QByteArray  getNetworkPic(const QString &szUrl);
	void commitStatisticalData();
private:
	Ui::MainWindow *ui;
	QGraphicsDropShadowEffect *m_shadow;
	QTimer *m_timer;
	int g_nActScreenW;
	int g_nActScreenH;
	int m_interval;
	bool m_mouseIn;
	QList<AdvertisementInfo_t> m_advertisementInfoList;
	QString m_jumpUrl;
	QDateTime m_startTime;
};

#endif // MAINWINDOW_H
