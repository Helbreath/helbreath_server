//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include "OccupyFlag.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COccupyFlag::COccupyFlag(int dX, int dY, char cSide, int iEKNum, int iDOI)
{
	m_sX = dX;
	m_sY = dY;

	m_cSide = cSide;
	m_iEKCount = iEKNum;

	m_iDynamicObjectIndex = iDOI;
}

COccupyFlag::~COccupyFlag()
{

}
