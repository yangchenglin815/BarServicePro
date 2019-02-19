#pragma once

#include <QString>
#include <QDateTime>
#include "enumData.h"
#include <QVariant>

//上报客户机信息
typedef struct ReportPCInfo
{
	int nBarId;   //网吧id
	QString sIp;   //ip地址
	QString sMac;   //mac地址
	QString sPCName;    //电脑名
}ReportPCInfo_t;
Q_DECLARE_METATYPE(ReportPCInfo)


//分类信息
typedef struct ClassifyGroupInfo
{
	int id;                            //分类id
	QString sName;          //分类名称
	int sort;                     //排序

}ClassifyGroupInfo_t;
Q_DECLARE_METATYPE(ClassifyGroupInfo)


//口味组信息
typedef struct TasteGroupInfo
{
	int id;            //口味组id
	QString sName;    //口味组名称
	QString sOpt;     //口味组选项
}TasteGroupInfo_t;
Q_DECLARE_METATYPE(TasteGroupInfo)

//商品列表
typedef struct GoodsInfo
{
	int nId;    //商品id
	QString sName;      //商品名称
	int nType;             //商品类型
	double dPrice;       //商品单价
	double dTotalPrice;  //商品总价
	int nClassifyId;    //商品分类id
	QString sCode;   //商品条码
	QString sUnit;     //单位
	QString sImgUrl;   //图片地址
	int nStockNum;    //库存数量
	int nOrderNum;   //下单数量
	QList<TasteGroupInfo> nTasteGroupList;   //口味组列表
	QList<TasteGroupInfo> nSelectedTasteList;  //被选口味组列表
	GoodsInfo(){
		nOrderNum = 1;
		dTotalPrice = 0.00;
	}
}GoodsInfo_t;
Q_DECLARE_METATYPE(GoodsInfo)

//下单商品详情
typedef struct OrderGoods{
	QString nGoodsId;      //商品id
	int nNum;                   //商品数量
	int nType;                  //商品类型  1:普通商品 2:自制商品
	QList<TasteGroupInfo> nTasteGroupList;     //所选口味组列表
	QString sPrice;                 //商品单价
}OrderGoods_t;
Q_DECLARE_METATYPE(OrderGoods)

//支付订单
typedef struct PaymentOrder
{
	int nPayType;           //支付类型   1:支付宝支付   3:现金支付
	int nBarId;               //网吧id
	QString sFactPay;     //实付金额
	QString sChange;     //找零
	QString sRemark;     //备注
	QString sAuthCode;    //用户付款码
	QList<OrderGoods_t> orderGoodsList;    //下单商品列表
}PaymentOrder_t;
Q_DECLARE_METATYPE(PaymentOrder)