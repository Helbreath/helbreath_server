//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#pragma once

class CItem;

class CFish  
{
public:
	CFish(char cMapIndex, short sX, short sY, short sType, CItem * pItem, int iDifficulty);
	~CFish();

	char  m_cMapIndex;
	short m_sX, m_sY;

	short m_sType;
	CItem * m_pItem;

    short m_sDynamicObjectHandle{};

	short m_sEngagingCount;
	int m_iDifficulty;
};
