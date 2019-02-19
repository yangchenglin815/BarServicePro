#pragma once

#include <QString>
#include <QDateTime>
#include "enumData.h"
#include <QVariant>

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
	int nPayType;           //支付类型   1:微信支付  2:支付宝支付   3:现金支付
	int nBarId;               //网吧id
	QString sFactPay;     //实付金额
	QString sChange;     //找零
	QString sRemark;     //备注
	QString sAuthCode;    //用户付款码
	QList<OrderGoods_t> orderGoodsList;    //下单商品列表
}PaymentOrder_t;
Q_DECLARE_METATYPE(PaymentOrder)

//商品盘点
typedef struct GoodsCheckUpInfo{
	QString nGoodsId;     //商品id
	double dPrice;   //商品单价
	int nMainStock;   //总库存
	int nBarStock;   //吧台库存
	int nCheckStock;  //盘点数量
}GoodsCheckUpInfo_t;
Q_DECLARE_METATYPE(GoodsCheckUpInfo)

//当班人信息
typedef struct ShifterInfo{
	QString nShiftId;   //当班人id
	QString nUserName;   //当班人名称
	QString nAccount;    //当班人账号
}ShifterInfo_t;
Q_DECLARE_METATYPE(ShifterInfo)

//交接班信息
typedef struct ShiftInfo
{
	double dCashSale;     //现金销售
	double dCashReturn;    //现金退货
	double dCodeSale;     //收款码销售
	double dTotalSale;     //交班销售
	double dCash;     //现金

	double dOperationTotal;   //进出货收支
	double dOperationIn;      //收货收入
	double dOperationOut;    //出货支出
	double dCheckTotal;      //盘点损益
	double dCheckLoss;      //盘点报损
	double dCheckProfit;    //盘点报溢

	double dOtherIn;      //其他收入
	double dOtherOut;     //其他支出
	double dCashIn;       //现金收入
	double dCashOut;      //现金支出
	double dCashTotal;    //现金收入支出汇总

	QString nEndTime;      //结束时间
	QString nParentId;      //必传id
	QList<ShifterInfo_t> nShifterList;  //当班人列表
}ShiftInfo_t;
Q_DECLARE_METATYPE(ShiftInfo)

//库存预警商品
typedef struct WarningGoods
{
	QString sGoodsId;
	QString sGoodsName;
	double dPrice;
	int nInventory;
	WarningGoods(){
		dPrice = 0.00;
		nInventory = 0;
	}
	int get_size(){
		return sizeof(WarningGoods);
	}
}WarningGoods_t;
Q_DECLARE_METATYPE(WarningGoods)

//订单详情列表
typedef struct OrderDetialInfo
{
	QString sGoodsId;       //商品id
	QString sGoodsName;     //商品名称
	int nOrderNum;          //下单数量
	double dPrice;         //商品单价
	OrderDetialInfo(){
		nOrderNum = 0;
		dPrice = 0.00;
	}
}OrderDetialInfo_t;
Q_DECLARE_METATYPE(OrderDetialInfo)

//订单列表
typedef struct OrderInfo
{
	QString sOrderId;    //订单id
	QString sOrderNum;         //订单号
	int nTradeType;      //交易类型    1.付款 2.退款
	int nPayStatus;       //支付状态  1.未支付 2.已支付 3.已退款
	int nPayType;    //支付类型/退款类型  1.微信 2.支付包 3.现金 4.幽游钱包
	QString sCreateTime;         //下单时间
	QString sFinishTime;         //完成时间
	QString sRemark;    //备注
	double dSum;   //订单金额
	QString seatNo;  //座位号
	QList<OrderDetialInfo_t> lOrderDetialList;  //订单商品列表
	OrderInfo(){
		nTradeType = 0;
		nPayStatus = 0;
		nPayType = 0;
	}
}OrderInfo_t;
Q_DECLARE_METATYPE(OrderInfo)