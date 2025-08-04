//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include "BuildItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBuildItem::CBuildItem()
{
 int i;

	memset(m_cName, 0, sizeof(m_cName));
	m_sItemID = -1;

	m_iSkillLimit = 0;
	
	for (i = 0; i < 6; i++) {
		m_iMaterialItemID[i]    = NULL;
		m_iMaterialItemCount[i] = NULL;
		m_iMaterialItemValue[i] = NULL;
		m_iIndex[i]            = -1;
	}

	m_iMaxValue     = 0;
	m_iAverageValue = 0;
	m_iMaxSkill     = 0;
	m_wAttribute    = 0;
}

CBuildItem::~CBuildItem()
{

}
