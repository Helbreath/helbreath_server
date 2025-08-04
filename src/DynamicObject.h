//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#if !defined(AFX_DYNAMICOBJECT_H__D8E4A581_0C95_11D3_8F1D_00001C7030A6__INCLUDED_)
#define AFX_DYNAMICOBJECT_H__D8E4A581_0C95_11D3_8F1D_00001C7030A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

class CDynamicObject  
{
public:
	CDynamicObject(short sOwner, char cOwnerType, short sType, char cMapIndex, short sX, short sY, uint32_t dwRegisterTime, uint32_t dwLastTime, int iV1);
	virtual ~CDynamicObject();

	short m_sOwner;
	char  m_cOwnerType;

	short m_sType;
	char  m_cMapIndex;
	short m_sX, m_sY; 
	uint32_t m_dwRegisterTime;
	uint32_t m_dwLastTime;

	int   m_iCount;			// Æ¯¼ö ¿ÀºêÁ§Æ®ÀÇ °æ¿ì »ç¿ëÇÏ´Â Ä«¿îÅÍ º¯¼ö 
	int   m_iV1;			// Ãß°¡ Á¤º¸¸¦ ³ÖÀ»¶§ »ç¿ëÇÑ´Ù.
};

#endif // !defined(AFX_DYNAMICOBJECT_H__D8E4A581_0C95_11D3_8F1D_00001C7030A6__INCLUDED_)
