#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <stdio.h>

#include "logging.hpp"
#include "appconfig.h"

using namespace std;

string ConfigFileRead(string key_)
{
    ifstream configFile;
    char buf[256];
    GetModuleFileName(NULL, buf, MAX_PATH);
    string path;
    string strLine;
    string strValue;
    path.assign(buf);
    path.append("\\..\\config.ini");
    configFile.open(path.c_str());
    if (configFile.is_open())
    {
        while (!configFile.eof())
        {
            getline(configFile, strLine);
            size_t pos = strLine.find('=');
            string key = strLine.substr(0, pos);

            if (key == key_)
            {
				strValue = strLine.substr(pos + 1);
            }
        }
    }
    else
    {
        printf("open file failed\n");
       // Log::Debug("open file failed\n");
		dem1::server::AppConfigHandler.Info("open file failed");
    }
	return strValue;
}