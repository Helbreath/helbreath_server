//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#if !defined(AFX_MSG_H__57BA71C0_B929_11D2_B143_00001C7030A6__INCLUDED_)
#define AFX_MSG_H__57BA71C0_B929_11D2_B143_00001C7030A6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <windows.h>

#define DEF_MSGFROM_CLIENT		1
#define DEF_MSGFROM_LOGSERVER	2
#define DEF_MSGFROM_GATESERVER	3
#define DEF_MSGFROM_BOT			4

class CMsg  								 
{
public:
	void Get(char * pFrom, char * pData, uint32_t * pSize, int * pIndex, char * pKey);
	bool bPut(char cFrom, char * pData, uint32_t dwSize, int iIndex, char cKey);
	CMsg();
	virtual ~CMsg();

	char   m_cFrom;

	char * m_pData;
	uint32_t  m_dwSize;

	int    m_iIndex;
	char   m_cKey;   // v1.4
};

#endif // !defined(AFX_MSG_H__57BA71C0_B929_11D2_B143_00001C7030A6__INCLUDED_)
