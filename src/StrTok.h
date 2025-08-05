//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#pragma once

class CStrTok  
{
public:
	CStrTok(char * pData, char * pSeps);
	~CStrTok() = default;

    char * pGet();
    char * m_pData{}, * m_pSeps{}, m_cToken[1024]{};
    int m_iDataLength{}, m_iCurLoc{};
	
private:
	bool _bIsSeperator(char cData, char cNextData);
};
