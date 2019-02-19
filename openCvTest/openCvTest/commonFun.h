#pragma once
#include <stdio.h>  
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
// #include <openssl/hmac.h>  
// #include <openssl/pem.h>
// #include <openssl/bio.h>
// #include <openssl/evp.h>
#include <string>
#include <vector>
#include <sstream>
#include <QString>

#define HMAC_LENGTH 20

using namespace std;

class commonFun
{
public:
// 	static vector<unsigned char> hmac_sha1(string& data, string& key);
// 	static string base64_encode(const string& src);
// 	static string hmac_sha1_func(string& appid, string& secret_id, string& secret_key);
	static QString readImageData();
	static QString getImageFileName(QString filePath);
};

