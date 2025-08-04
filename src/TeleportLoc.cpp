//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include "TeleportLoc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTeleportLoc::CTeleportLoc()
{

	memset(m_cDestMapName, 0, sizeof(m_cDestMapName));
	//memset(m_cDestMapName2, 0, sizeof(m_cDestMapName2));
	m_sSrcX   = -1;
	m_sSrcY	  = -1;
	m_sDestX  = -1;								    
	m_sDestY  = -1;
	m_sDestX2 = -1;
	m_sDestY2 = -1;

	m_iV1     = NULL;
	m_iV2     = NULL;
	m_dwTime  = NULL;
	m_dwTime2 = NULL;

}

CTeleportLoc::~CTeleportLoc()
{

}
