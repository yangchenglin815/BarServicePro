#include "attendanceDialog.h"
#include <QTableWidgetItem>
#include <QDate>
#include <QDateTime>
#include "HttpAgent.h"
#include "logging.hpp"
#include <QMessageBox>

attendanceDialog::attendanceDialog(QWidget *parent)
	: QDialog(parent)
	, m_curr_item(NULL)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint /*| Qt::WindowStaysOnTopHint*/);
	setAttribute(Qt::WA_TranslucentBackground, true);

	InitUI();
	InitConnect();
}

attendanceDialog::~attendanceDialog()
{

}

void attendanceDialog::InitSignIn(LoginUserInfo userInfo)
{
	m_loginUserInfo = userInfo;
	HttpAgent::instance()->RequestOnGetSignInList(userInfo);
}

void attendanceDialog::slotOnItemClicked(QTableWidgetItem *item)
{
	QString text = item->text();
	for (int i = 0; i < m_weekList.size(); ++i)
	{
		if (text.contains(m_weekList[i]))
		{
			return;
		}
	}

	int dayOfWeek = QDate::currentDate().day();
	if (!text.contains("\r\n") && text == QString::number(dayOfWeek))
	{
		m_curr_item = item;
		m_curr_signInDay = text;
		HttpAgent::instance()->RequestOnSignIn(m_loginUserInfo);
	}
}

void attendanceDialog::slotOnGetSignInList(int nCode, QString sMsg, QList<QString> lSignInDateList)
{
	if (nCode != e_success)
	{
		Log::Info("Request On GetSignInList failed : %s", sMsg.toLocal8Bit().data());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		Log::Info("Request On GetSignInList success");
		m_signInDateList = lSignInDateList;
		InitData();
	}
}

void attendanceDialog::slotOnSignIn(int nCode, QString sMsg)
{
	if (nCode != e_success)
	{
		Log::Info("Request On SignIn failed : %s", sMsg.toLocal8Bit().data());
		QMessageBox::warning(NULL, "warning", QStringLiteral("网络请求出现异常--错误码:%1, 错误信息:%2").arg(nCode).arg(sMsg));
	}
	else
	{
		Log::Info("Request On SignIn success");
		if (m_curr_item != NULL)
		{
			m_curr_item->setTextColor(QColor("#207C78"));
			m_curr_item->setText(m_curr_signInDay + QStringLiteral("\r\n已签"));
		}
	}
}

void attendanceDialog::InitUI()
{
	QTableWidgetItem *item[6];
	ui.tableWidget->setFocusPolicy(Qt::NoFocus);
	for (int i = 0; i < 7; ++i)
	{
		item[i] = new QTableWidgetItem();
		ui.tableWidget->setItem(0, i, item[i]);
		item[i]->setTextColor(QColor("#ACACAC"));
		item[i]->setTextAlignment(Qt::AlignCenter);
	}
	item[0]->setText(QStringLiteral("一"));
	item[1]->setText(QStringLiteral("二"));
	item[2]->setText(QStringLiteral("三"));
	item[3]->setText(QStringLiteral("四"));
	item[4]->setText(QStringLiteral("五"));
	item[5]->setText(QStringLiteral("六"));
	item[6]->setText(QStringLiteral("日"));

	for (int i = 0; i < 7; ++i)
	{
		m_weekList.append(item[i]->text());
	}
	
	QString currDate = QDate::currentDate().toString("yyyy-MM-dd");
	QString beginDate = currDate;
	beginDate.replace(8, 2, "01");
	int beginWeekofday = QDate::fromString(beginDate, "yyyy-MM-dd").dayOfWeek();
	int daysOfWeek = QDate::currentDate().daysInMonth();
	int extraDays = 7 - beginWeekofday + 1;     //第一行的天数
	int extraDays_ = (daysOfWeek - extraDays) % 7;    //最后一行的天数
	int rowDays = (daysOfWeek - extraDays) % 7 ? (daysOfWeek - extraDays) / 7 + 2 : (daysOfWeek - extraDays) / 7 + 1;
	int currDay = QDate::currentDate().day();
	int index = 1;
	for (int j = 0; j < rowDays; ++j)
	{
		int k = j ? 0 : beginWeekofday - 1;
		for (; k < 7; k++)
		{
			if (j == rowDays - 1 && k > extraDays_ - 1)
			{
				break;
			}
			QTableWidgetItem *cItem = new QTableWidgetItem();
			ui.tableWidget->setItem(j + 1, k, cItem);
			if (index == currDay)
			{
				cItem->setTextColor(QColor("#EC0000"));
			}
			else
			{
				cItem->setTextColor(QColor("#ACACAC"));
			}
			cItem->setTextAlignment(Qt::AlignCenter);
			cItem->setText(QString::number(index++));
		}
	}
}

void attendanceDialog::InitConnect()
{
	connect(ui.btn_close, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui.tableWidget, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(slotOnItemClicked(QTableWidgetItem*)));

	qRegisterMetaType<QList<QString> >("QList<QString>");
	connect(HttpAgent::instance(), SIGNAL(sigOnGetSignInList(int, QString, QList<QString>)), this, SLOT(slotOnGetSignInList(int, QString, QList<QString>)));
	connect(HttpAgent::instance(), SIGNAL(sigOnSignIn(int, QString)), this, SLOT(slotOnSignIn(int, QString)));
}

void attendanceDialog::InitData()
{
	for (int i = 0; i < m_signInDateList.size(); ++i)
	{
		int day = QDateTime::fromString(m_signInDateList[i], "yyyy-MM-dd hh:mm:ss").date().day();
		for (int j = 1; j < ui.tableWidget->rowCount(); ++j)
		{
			for (int k = 0; k < ui.tableWidget->columnCount(); ++k)
			{
				QTableWidgetItem *itemWgt = ui.tableWidget->item(j, k);
				if (itemWgt && itemWgt->text().toInt() == day)
				{
					itemWgt->setTextColor(QColor("#207C78"));
					itemWgt->setText(QString::number(day) + QStringLiteral("\r\n已签"));
				}
			}
		}
	}
}
