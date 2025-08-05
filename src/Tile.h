//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#pragma once

#include <cstdint>

class CItem;

#define DEF_TILE_PER_ITEMS	12

class CTile  
{												  
public:
	CTile();
	~CTile();

	char m_cOwnerClass;		// DEF_OT_PLAYER / DEF_OT_NPC
	short m_sOwner;

	char m_cDeadOwnerClass;	// DEF_OT_PLAYER / DEF_OT_NPC
	short m_sDeadOwner;

	CItem * m_pItem[DEF_TILE_PER_ITEMS];
	char m_cTotalItem;

	uint16_t  m_wDynamicObjectID;
	short m_sDynamicObjectType;
	uint32_t m_dwDynamicObjectRegisterTime;

	bool m_bIsMoveAllowed, m_bIsTeleport, m_bIsWater, m_bIsFarm, m_bIsTempMoveAllowed;

	int m_iOccupyStatus;
	int m_iOccupyFlagIndex;

	// Crusade
	int m_iAttribute;
};
