//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include "Fish.h"
#include "Item.h"

CFish::CFish(char cMapIndex, short sX, short sY, short sType, class CItem * pItem, int iDifficulty)
{
	m_cMapIndex = cMapIndex;
	m_sX = sX;
	m_sY = sY;
	m_sType = sType;
	m_pItem = pItem;

	m_sEngagingCount = 0;
	m_iDifficulty = iDifficulty;
	
	if (m_iDifficulty <= 0) 
		m_iDifficulty = 1;
}

CFish::~CFish()
{
	if (m_pItem != nullptr) delete m_pItem;
}
