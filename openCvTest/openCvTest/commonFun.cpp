#include "commonFun.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QApplication>
#include <QDataStream>

#include <iostream>
#include <fstream>

// std::vector<unsigned char> commonFun::hmac_sha1(string& data, string& key)
// {
// 	unsigned char* result;
// 	unsigned int len = HMAC_LENGTH;
// 
// 	result = (unsigned char*)malloc(sizeof(char)* len);
// 
// 	HMAC_CTX ctx;
//  	HMAC_CTX_init(&ctx);
//  	HMAC_Init_ex(&ctx, key.c_str(), key.length(), EVP_sha1(), NULL);
//  	HMAC_Update(&ctx, (unsigned char*)data.c_str(), data.length());
//  	HMAC_Final(&ctx, result, &len);
//  	HMAC_CTX_cleanup(&ctx);
// 
// 	std::vector<unsigned char> sha1;
// 	for (int i = 0; i < len; i++){
// 		sha1.push_back(result[i]);
// 	}
// 
// 	free(result);
// 	return sha1;
// }

// std::string commonFun::base64_encode(const std::string& src){
// 	BUF_MEM * bptr = NULL;
// 	BIO* b64 = BIO_new(BIO_f_base64());
// 	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
// 	BIO* bmem = BIO_new(BIO_s_mem());
// 	if (NULL == b64 || NULL == bmem){
// 		return "";
// 	}
// 
// 	bmem = BIO_push(b64, bmem);
// 	int ret = BIO_write(bmem, src.data(), src.length());
// 	if (ret <= 0){
// 		return "";
// 	}
// 
// 	ret = BIO_flush(bmem);
// 	BIO_get_mem_ptr(bmem, &bptr);
// 	std::string res(bptr->data, bptr->length);
// 	BIO_free_all(bmem);
// 	return res;
// 
// }

// std::string commonFun::hmac_sha1_func(string& appid, string& secret_id, string& secret_key)
// {
// 	std::string sign = "";
// 	time_t now = time(NULL);
// 	long expired = (long)now + 2592000;
// 	long onceExpired = 0;
// 	long current = (long)now;
// 	int rdm = rand();
// 	std::string userid = "0";
// 
// 	std::stringstream raw_stream;
// 	raw_stream << "a=" << appid << "&k=" << secret_id << "&e=" << expired << "&t=" << current
// 		<< "&r=" << rdm;
// 	std::string raw = raw_stream.str();
// 
// 	std::vector<unsigned char> sha1 = hmac_sha1(raw, secret_key);
// 
// 	std::stringstream data_stream;
// 	for (int i = 0; i != sha1.size(); i++)
// 		data_stream << sha1[i];
// 	data_stream << raw;
// 	std::string data = data_stream.str();
// 
// 	sign = base64_encode(data);
// 
// 	printf("%s\n", sign.c_str());
// 	return sign;
// }

QString commonFun::readImageData()
{
	QString imageData;
	QFile file(QApplication::applicationDirPath() + "\\test.jpg");
	if (!file.open(QFile::ReadOnly))
	{
		qDebug() << " Could not open the file for reading";
		qDebug() << file.error();
		qDebug() << file.errorString();
		return "";
	}

	char *m_pBuff;
	//确保内存分配成功  
	int nSize = file.size();
	do
	{
		m_pBuff = new char[nSize + 1];
	} while (m_pBuff == NULL);
	QDataStream in(&file);
	in.readRawData(m_pBuff, nSize);
	imageData = QString(QLatin1String(m_pBuff));

// 	QTextStream on(&file);
// 	imageData = on.readAll();
	qDebug() << imageData;

	file.close();
	delete m_pBuff;
	return imageData;
}

QString commonFun::getImageFileName(QString filePath)
{
	QString fileName;
	QStringList list = filePath.split("/");
	if (list.size() > 0)
	{
		fileName = list.at(list.size() - 1);
	}
	return fileName;
}
