#pragma once
#include <QString>

// 网络响应异常
const QString c_sNetError = QStringLiteral("网络连接异常");
const QString c_sServerJsonError = QStringLiteral("服务器数据异常");

//服务类型地址
const QString c_sFormalServerUrl = "http://202.91.251.125:8084";
const QString c_sTestServerUrl = "http://202.91.251.122:8085";
const QString c_sLocalServerUrl = "http://192.168.180.102:8080";

/*接口APi*/
//获取广告列表
const QString c_sHttpGetAdvertisementListAction = "/qynetad/api/ad/list";
//上报统计数据
const QString c_sHttpCommitStatisticalDataAction = "/qynetad/api/adstatistics/save";