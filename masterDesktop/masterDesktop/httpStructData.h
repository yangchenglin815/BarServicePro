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
	QString id;                            //分类id
	QString sName;          //分类名称
	int sort;                     //排序

}ClassifyGroupInfo_t;
Q_DECLARE_METATYPE(ClassifyGroupInfo)


//口味组信息
typedef struct TasteGroupInfo
{
	QString id;            //口味组id
	QString sName;    //口味组名称
	QString sOpt;     //口味组选项
}TasteGroupInfo_t;
Q_DECLARE_METATYPE(TasteGroupInfo)

//商品列表
typedef struct GoodsInfo
{
	int nFortId;   //排序id
	QString nId;    //商品id
	QString sName;      //商品名称
	int nType;             //商品类型
	double dPrice;       //商品单价
	double dTotalPrice;  //商品总价
	QString nClassifyId;    //商品分类id
	QString sClassifyName;   //商品分类名称
	QString sCode;   //商品条码
	QString sUnit;     //单位
	QString sImgUrl;   //图片地址
	int nStockNum;    //吧台库存数量
	int nMainStockNum;   //总库存数量
	int nOrderNum;   //下单数量
	QString nCheckNum;    //盘点数量
	QList<TasteGroupInfo> nTasteGroupList;   //口味组列表
	QList<TasteGroupInfo> nSelectedTasteList;  //被选口味组列表
	GoodsInfo(){
		nFortId = 0;
		nOrderNum = 0;
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

//用户上机信息
typedef struct LoginUserInfo
{
	QString sName;    //用户姓名
	QString sPhone;    //用户手机号
	QString sIdCard;   //用户身份证号
	double dNetFee;    //网费金额
	double dWalletFee;  //钱包金额
	QString sUserId;     //用户ID
	int nDiffTime;       //上机时长
	LoginUserInfo(){
		dNetFee = 0.00;
		dWalletFee = 0.00;
		nDiffTime = 0;
	}
	int get_size(){
		return sizeof(LoginUserInfo);
	}
}LoginUserInfo_t;
Q_DECLARE_METATYPE(LoginUserInfo)

//网费充值优惠包
typedef struct DiscountPkg
{
	QString sId;     //优惠包id
	double dOriginalPrice;     //原价
	DiscountPkg(){
		dOriginalPrice = 0.00;
	}
	int get_size(){
		return sizeof(DiscountPkg);
	}
	void clear(){
		sId = "";
		dOriginalPrice = 0.00;
	}
}DiscountPkg_t;
Q_DECLARE_METATYPE(DiscountPkg)