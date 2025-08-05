//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#pragma once

#include <cstdint>

class CBuildItem  
{
public:
	CBuildItem();
	~CBuildItem();

	char  m_cName[21];
	short m_sItemID;

	int  m_iSkillLimit;
	
	int  m_iMaterialItemID[6];
	int  m_iMaterialItemCount[6];
	int  m_iMaterialItemValue[6];
	int  m_iIndex[6];

	int	 m_iMaxValue;
	int  m_iAverageValue;	
	int   m_iMaxSkill;
	uint16_t  m_wAttribute;
};
