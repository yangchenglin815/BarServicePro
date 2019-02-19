#include "masterCashier.h"
#include "appConfigIni.h"
#include "loginDialog.h"
#include "FramelessHelper.h"
#include "switchAccountDlg.h"
#include "webSocketAgent.h"
#include "HttpAgent.h"
#include "ViewSignal.h"
#include "verifyShopperPwdDlg.h"

#include <QTimer>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QColorDialog>
#include <webSocketSignal.h>

masterCashier::masterCashier(QWidget *parent)
	: QMainWindow(parent)
	, m_loginDlg(NULL)
	, m_switchDlg(NULL)
	, m_maskWgt(NULL)
	, m_warningGoods_timer(NULL)
	, m_verifyDlg(NULL)
	, m_faceTectionDlg(NULL)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	setWindowTitle("masterCashier");
	setWindowIcon(QIcon(":/masterCashier/Resources/masterCashier_icon.ico"));

	FramelessHelper *helper = new FramelessHelper;
	helper->activateOn(this);
	helper->setDbClickOn(false);
	helper->setWidgetResizable(false);

	m_warningGoods_timer = new QTimer(this);
	m_warningGoods_timer->setInterval(1000 * 60 * 10);

	QString path = QApplication::applicationDirPath() + "/config/config.ini";
	appConfigIni::setIniPath(path);

	initUI();
	initConnect();
	webSocketAgent::instance()->initServer();
	QTimer::singleShot(20, this, SLOT(slotOnInitLogin()));
}

masterCashier::~masterCashier()
{
	if (m_warningGoods_timer != NULL)
	{
		m_warningGoods_timer->stop();
		m_warningGoods_timer->deleteLater();
	}
}

void masterCashier::initUI()
{
	ui.btn_max->hide();
	QString classifyName = QStringLiteral("全游超市");
	QString classifyStyle = "background-image:url(:/masterCashier/Resources/supermarket.png)";
	ui.btn_qySupmarket->initButton(classifyName, classifyStyle);

	ui.wgt_right->setCurrentWidget(ui.page_qySupmarket);
	ui.page_qySupmarket->initData();

	connect(ui.btn_icon, &QPushButton::clicked, this, [&](){
		//人脸检测测试接口
		//slotOnFaceTection("1234567890", "ycl");
		//打印机测试接口
		ReceiptInfo info;
		info.nAccount = QStringLiteral("羊羊羊");
		info.nPaymentNo = "1234567890";
		info.nSeatNo = "220";
		info.sBarName = QStringLiteral("全游电竞杭州蓝钻旗舰店");
		info.strSum = "20.00";
		ProductInfo pInfo;
		pInfo.nPrice = "20.00";
		pInfo.nProductName = QStringLiteral("统一冰红茶瓶装1L");
		pInfo.nProductNum = QStringLiteral("1瓶");
		pInfo.sTasteGroup = QStringLiteral("7分甜/去冰/多奶/热");
		info.nProductList.append(pInfo);
		webSocketSignal::instance()->sigOnPrintTips(info);
	});
}

void masterCashier::initConnect()
{
	connect(ui.btn_min, SIGNAL(clicked()), this, SLOT(showMinimized()));
	connect(ui.btn_max, SIGNAL(clicked()), this, SLOT(showMaximized()));
	connect(ui.btn_close, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.btn_switch, SIGNAL(clicked()), this, SLOT(slotOnSwitchAccount()));
	connect(ui.btn_shift, SIGNAL(clicked()), this, SLOT(slotOnShowShiftPage()));
	connect(ui.btn_qySupmarket, SIGNAL(clicked()), this, SLOT(slotOnShowSalePage()));
	connect(ui.btn_settings, SIGNAL(clicked()), this, SLOT(slotOnShowSettingsPage()));
	connect(m_warningGoods_timer, SIGNAL(timeout()), this, SLOT(slotOnGetWarningGoodsList()));

	connect(ViewSignal::instance(), SIGNAL(onMask(bool)), this, SLOT(OnMask(bool)));
	connect(ViewSignal::instance(), SIGNAL(sigOnShowHomePage()), this, SLOT(slotOnShowHomePage()));
	connect(ViewSignal::instance(), SIGNAL(sigOnShowVerifyDlg(QString, int)), this, SLOT(slotOnShowVerifyDlg(QString, int)));
	connect(webSocketAgent::instance(), SIGNAL(sigOnFaceTection(QString, QString)), this, SLOT(slotOnFaceTection(QString, QString)));
}

void masterCashier::slotOnInitLogin()
{
	if (m_loginDlg == NULL)
	{
		m_loginDlg = new loginDialog();
		connect(m_loginDlg, SIGNAL(sigOnGetBarInfo(QString)), this, SLOT(slotOnGetBarInfo(QString)));
	}

	int r = m_loginDlg->exec();
	if (r != QDialog::Accepted)
	{
		return;
	}

	show();
	setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
	raise();
	activateWindow();
	m_warningGoods_timer->start();
	slotOnGetWarningGoodsList();
}

void masterCashier::slotOnGetBarInfo(QString nBarName)
{
	m_sBarName = nBarName;
	ui.lbl_logo->setText(QStringLiteral("%1(ID:%2)").arg(nBarName).arg(appConfigIni::getBarId()));
}

void masterCashier::slotOnSwitchAccount()
{
	if (QMessageBox::information(NULL, QStringLiteral("切换账号"), QStringLiteral("是否要切换当前账号？"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
	{
		if (m_switchDlg == NULL)
		{
			OnMask(true);
			m_switchDlg = new switchAccountDlg(this);
			m_switchDlg->setLogoText(m_sBarName);
		}

		int r = m_switchDlg->exec();
		if (r != QDialog::Accepted)
		{
			return;
		}

		m_switchDlg->deleteLater();
		m_switchDlg = NULL;
		OnMask(false);
		slotOnShowHomePage();
	}
}

void masterCashier::slotOnShowShiftPage()
{
	ui.wgt_right->setCurrentWidget(ui.page_shift);
	ui.page_shift->slotOnFrontFunc();
	HttpAgent::instance()->RequestOnGetClassifyGroups(appConfigIni::getBarId(), t_shiftPage);
}

void masterCashier::slotOnShowSalePage()
{
	ui.wgt_right->setCurrentWidget(ui.page_qySupmarket);
	ui.page_qySupmarket->initData();
}

void masterCashier::slotOnShowSettingsPage()
{
	ui.wgt_right->setCurrentWidget(ui.page_settings);
}

void masterCashier::OnMask(bool isMask)
{
	if (isMask)
	{
		if (m_maskWgt != NULL)
		{
			m_maskWgt->deleteLater();
			m_maskWgt = NULL;
		}
		m_maskWgt = new QWidget(this);
		m_maskWgt->setGeometry(0, 0, this->width(), this->height());
		m_maskWgt->setStyleSheet("QWidget{background:rgba(0,0,0,0.3);}");
		m_maskWgt->show();
	}
	else
	{
		if (m_maskWgt != NULL)
		{
			m_maskWgt->close();
			m_maskWgt->deleteLater();
			m_maskWgt = NULL;
		}
	}
}

void masterCashier::slotOnShowHomePage()
{
	slotOnShowSalePage();
}

void masterCashier::slotOnGetWarningGoodsList()
{
	QString barId = appConfigIni::getBarId();
	HttpAgent::instance()->RequestOnGetWarningGoodsList(barId);
}

void masterCashier::slotOnShowVerifyDlg(QString sOrderId, int nPageType)
{
	OnMask(true);

	if (m_verifyDlg == NULL)
	{
		m_verifyDlg = new verifyShopperPwdDlg(sOrderId, nPageType, this);
	}
	m_verifyDlg->exec();
	m_verifyDlg->deleteLater();
	m_verifyDlg = NULL;

	OnMask(false);
}

void masterCashier::slotOnFaceTection(QString sIdCard, QString sName)
{
	if (m_faceTectionDlg == NULL)
	{
		m_faceTectionDlg = new faceTectionDlg(sIdCard, sName, this);
	}
	m_faceTectionDlg->openCamera();
	m_faceTectionDlg->show();
}

void masterCashier::showEvent(QShowEvent *event)
{
	QDesktopWidget *desktop = QApplication::desktop();
	//获取桌面宽度
	int w = desktop->width();
	//获取桌面高度
	int h = desktop->height();

	this->move((w - width()) / 2, (h - height()) / 2);
}
