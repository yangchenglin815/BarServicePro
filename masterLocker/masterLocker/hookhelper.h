#ifndef HOOKHELPER_H
#define HOOKHELPER_H

#include <QObject>
#include "Windows.h"

/*******屏蔽热键*******/

//在注册表该目录下增加新内容
#define TASKMANAGER "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"

class HookHelper : public QObject
{
	Q_OBJECT

public:
	static HookHelper* instance();

	void setHookFunc();
	void unHookFunc();

	void OnRFreeze();
	void OnRUnfreeze();
private:
	explicit HookHelper(QObject *parent = 0);
	~HookHelper();

	DWORD GetPidByProcessName(LPCTSTR pszName);
	void EnableDebugPriv(void);
private:
	static HookHelper* m_pInstance;
	DWORD dwPID_;
};

#endif // HOOKHELPER_H
