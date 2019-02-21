#include "speechtool.h"
#include <QtWidgets/QApplication>
#include <QString>
#include "speech.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

char* UnicodeToUtf8(const wchar_t* unicode)
{
	int len;
	len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
	char *szUtf8 = (char*)malloc(len + 1);
	memset(szUtf8, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, unicode, -1, szUtf8, len, NULL, NULL);
	return szUtf8;
}

unsigned char ToHex(unsigned char x)
{
	return  x > 9 ? x + 55 : x + 48;
}

unsigned char FromHex(unsigned char x)
{
	unsigned char y;
	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
	else if (x >= '0' && x <= '9') y = x - '0';
	else assert(0);
	return y;
}

std::string UrlEncode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (isalnum((unsigned char)str[i]) ||
			(str[i] == '-') ||
			(str[i] == '_') ||
			(str[i] == '.') ||
			(str[i] == '~'))
			strTemp += str[i];
		else if (str[i] == ' ')
			strTemp += "+";
		else
		{
			strTemp += '%';
			strTemp += ToHex((unsigned char)str[i] >> 4);
			strTemp += ToHex((unsigned char)str[i] % 16);
		}
	}
	return strTemp;
}

std::string UrlDecode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (str[i] == '+') strTemp += ' ';
		else if (str[i] == '%')
		{
			assert(i + 2 < length);
			unsigned char high = FromHex((unsigned char)str[++i]);
			unsigned char low = FromHex((unsigned char)str[++i]);
			strTemp += high * 16 + low;
		}
		else strTemp += str[i];
	}
	return strTemp;
}

void ASR(aip::Speech* client) {
	std::map<std::string, std::string> options;
	options["lan"] = "ZH";
	std::string file_content;
	aip::get_file_content("../assets/16k_test.pcm", &file_content);
	Json::Value result = client->recognize(file_content, "pcm", 16000, options);
	std::cout << "语音识别本地文件结果:" << std::endl << result.toStyledString();
}

void ASR_url(aip::Speech* client) {
	std::map<std::string, std::string> options;
	options["lan"] = "zh";
	Json::Value result =
		client->recognize_url("http://bos.nj.bpc.baidu.com/v1/audio/8k.amr",
		"http://your_site/dump",
		"amr", 8000, options);
	std::cout << "语音识别远程文件结果:" << std::endl << result.toStyledString();
}

void TTS(aip::Speech* client) {
	std::ofstream ofile;
	std::string file_ret;
	std::string str;
	std::map<std::string, std::string> options;
	options["spd"] = "5";  //语速
	options["per"] = "0";    //人声
	ofile.open("./tts.mp3", std::ios::out | std::ios::binary);
	wchar_t* wCharUnicode = L"工作人员请注意，有新的订单消息等待处理";
	char* cCharUtf = UnicodeToUtf8(wCharUnicode);
	str += cCharUtf;
	Json::Value result = client->text2audio(str, options, file_ret);
	// 如果file_ret为不为空则说明合成成功，返回mp3文件内容
	if (!file_ret.empty())
	{
		// 合成成功保存文件
		ofile << file_ret;
		std::cout << "语音合成成功，打开目录下的tts.mp3文件听听看" << std::endl;
	}
	else {
		// 合成出错，打印错误信息
		std::cout << result.toStyledString();
	}
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
 	aip::Speech * client = new aip::Speech("11609320", "q9cc82afcdBGb5KDnok86w4X", "5t509BUViY4UQGkStES37bNST4muQVGQ");
 
 	TTS(client);

// 	SpeechTool w;
// 	w.show();
	return a.exec();
}
