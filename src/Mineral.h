//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#pragma once

class CMineral  
{
public:
	CMineral(char cType, char cMapIndex, int sX, int sY, int iRemain);
	~CMineral();
	
	char m_cType;

	char m_cMapIndex;
	int m_sX, m_sY;
	int m_iDifficulty;
	short m_sDynamicObjectHandle;
	
	int m_iRemain;
};
