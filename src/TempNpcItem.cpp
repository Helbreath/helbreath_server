//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include "TempNpcItem.h"
extern void PutLogFileList(char * cStr);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNpcItem::CNpcItem()
{
	memset(m_cName, 0, sizeof(m_cName));

	m_sItemID = 0;
	m_sFirstProbability = 0;
	m_cFirstTargetValue = 0;
	m_sSecondProbability = 0;
	m_cSecondTargetValue = 0;

}

CNpcItem::~CNpcItem()
{

}
