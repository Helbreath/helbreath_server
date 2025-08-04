//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include "StrTok.h"
extern void PutLogFileList(char * cStr);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStrTok::CStrTok(char * pData, char * pSeps)
{
	m_pData = pData;
	m_pSeps = pSeps;

	m_iDataLength = strlen(pData);
	m_iCurLoc     = 0;
}

CStrTok::~CStrTok()
{

}

char * CStrTok::pGet()
{
 int i = 0;
 char cNextData;
 bool bFlag;

	memset(m_cToken, 0, sizeof(m_cToken));

	while (m_iCurLoc < m_iDataLength) {

		if (m_iCurLoc <= (m_iDataLength - 2))
			 cNextData = m_pData[m_iCurLoc+1];
		else cNextData = NULL;

		if (_bIsSeperator(m_pData[m_iCurLoc], cNextData) == false) {
			// separator°¡ ¾Æ´Ï¸é ÅäÅ«À» ÀÛ¼º  
			m_cToken[i] = m_pData[m_iCurLoc];
			i++;
			m_iCurLoc++;
		}
		else {
			// ´ÙÀ½ ÅäÅ«±îÁö Æ÷ÀÎÅÍ¸¦ ÀÌµ¿½ÃÅ²ÈÄ ÅäÅ«À» ¹ÝÈ¯ÇÑ´Ù.
			bFlag = false;
			while (bFlag == false) {
				if (m_iCurLoc <= (m_iDataLength - 2))
					 cNextData = m_pData[m_iCurLoc+1];
				else cNextData = NULL;
				if (_bIsSeperator(m_pData[m_iCurLoc], cNextData) == true) {
					m_iCurLoc++;	
				}
				else bFlag = true;

				if (m_iCurLoc >= (m_iDataLength-1)) bFlag = true;
			}
			
			return (char *)(&m_cToken);
		}
	}

	if (strlen(m_cToken) != 0) return (char *)(&m_cToken);
	return NULL;
}

bool CStrTok::_bIsSeperator(char cData, char cNextData)
{
 int i = 0;
	
	if (cData == NULL) return true;
	if ((cData == 0x0D) && (cNextData == 0x0A)) return true;
	
	while (m_pSeps[i] != NULL) {
		if (m_pSeps[i] == cData) return true;
		i++;
	}

	return false;
}

