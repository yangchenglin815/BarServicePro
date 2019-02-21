#pragma once
#include <QString>
#include <QDateTime>
#include <QVariant>

typedef struct AdvertisementInfo{
	QString sId;
	QString sDetial;
	QString sTitle;
	QString sImage;
	QString sJumpUrl;
	int nDelayTime;
	AdvertisementInfo() {
		nDelayTime = 0;
	}
}AdvertisementInfo_t;
Q_DECLARE_METATYPE(AdvertisementInfo)