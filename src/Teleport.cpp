//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include "Teleport.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTeleport::CTeleport()
{

	memset(m_cTeleportNpcName, 0, sizeof(m_cTeleportNpcName));
	memset(m_cSourceMap, 0, sizeof(m_cSourceMap));
	memset(m_cTargetMap, 0, sizeof(m_cTargetMap));
	memset(m_cTeleportSide, 0, sizeof(m_cTeleportSide));

	m_sDestinationX = -1;
	m_sDestinationY = -1;
	m_sTeleportCost = -1;
	m_sTeleportMinLevel = 0;
	m_sTeleportMaxLevel = 0;
	m_bTeleportHunt = false;
	m_bTeleportNtrl = false;
	m_bTeleportCrmnl = false;
}

CTeleport::~CTeleport()
{										    

}

