//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include <windows.h>

#define MSG_RECV 1
#define MSG_SEND 2

class CDebugWindow
{

public:
	CDebugWindow();
	// Startups The Debug Dialog
	void Startup(void);
	void Shutdown(void);
	void AddEventMsg(char* cMsg);
	void AddEventMsg(int cMsgType, char* cData, uint32_t dwSize, char cKey);
	void ShowWindow(bool isVisible);
private:
	bool m_isVisible;
};
