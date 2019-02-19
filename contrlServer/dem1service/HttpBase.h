#pragma once
#include <iostream>  

using namespace std;


class HttpBase
{
public:
	HttpBase();

	virtual ~HttpBase();
	/*
	* 发送Post请求
	*/
	virtual int post(const std::string& url) = 0;

	/*
	* 发送get请求
	*/
	virtual int get(const std::string& url) = 0;

	virtual std::string getResponse(void) = 0;
protected:
	typedef int(*pBuildRequest)(const string&, const string&,
		std::ostream&);
	/*
	* 解析URL
	* parseUrl
	* url: 待解析的URL
	* out_server: 服务器名
	* out_port: 端口号
	* out_path: 服务器子页
	*/
	static int parseUrl(const std::string& url, std::string& out_server,
		std::string& out_port, std::string& out_path);

	/*
	* 建立Post请求
	* buildPostRequest
	*/
	static int buildPostRequest(const std::string& server, const std::string& path,
		std::ostream& out_request);
	/*
	* 建立Get请求
	* buildGetRequest
	*/
	static int buildGetRequest(const std::string& server, const std::string& path,
		std::ostream& out_request);
};

