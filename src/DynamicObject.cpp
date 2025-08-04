//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include "DynamicObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDynamicObject::CDynamicObject(short sOwner, char cOwnerType, short sType, char cMapIndex, short sX, short sY, uint32_t dwRegisterTime, uint32_t dwLastTime, int iV1)
{
	m_sOwner         = sOwner;
	m_cOwnerType     = cOwnerType;

	m_sType          = sType;
	
	m_cMapIndex      = cMapIndex;
	m_sX             = sX;
	m_sY             = sY;
	
	m_dwRegisterTime = dwRegisterTime;
	m_dwLastTime     = dwLastTime;

	m_iCount         = 0;
	m_iV1            = iV1;
}										   

CDynamicObject::~CDynamicObject()
{

}
