//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#if !defined(AFX_TILE_H__12609161_8060_11D2_A8E6_00001C7030A6__INCLUDED_)
#define AFX_TILE_H__12609161_8060_11D2_A8E6_00001C7030A6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <windows.h>
#include "Item.h"

#define DEF_TILE_PER_ITEMS	12

class CTile  
{												  
public:
	CTile();
	virtual ~CTile();

	char  m_cOwnerClass;		// DEF_OT_PLAYER / DEF_OT_NPC
	short m_sOwner;

	char  m_cDeadOwnerClass;	// DEF_OT_PLAYER / DEF_OT_NPC Á×¾îÀÖ´Â ÇÃ·¹ÀÌ¾î´Â ÀÌ°÷¿¡ 
	short m_sDeadOwner;

	class CItem * m_pItem[DEF_TILE_PER_ITEMS];
	char  m_cTotalItem;

	uint16_t  m_wDynamicObjectID;
	short m_sDynamicObjectType;
	uint32_t m_dwDynamicObjectRegisterTime;

	bool  m_bIsMoveAllowed, m_bIsTeleport, m_bIsWater, m_bIsFarm, m_bIsTempMoveAllowed;

	// Á¡·ÉÁö Ç¥½Ã¿ë 
	int   m_iOccupyStatus;    // AresdenÀÌ¸é -°ªÀ¸·Î, ElvineÀÌ¸é +·Î ´õÇØÁø´Ù. ÃÑ ÇÕÀÌ ÀÌ Áö¿ªÀÇ °æÇâ¼º 
	int   m_iOccupyFlagIndex; // ÀÌ Å¸ÀÏ À§¿¡ ¼­ ÀÖ´Â Á¡·É±ê¹ßÀÇ ÀÎµ¦½º 

	// Crusade
	int	  m_iAttribute;		  // Å¸ÀÏÀÇ »óÅÂ ºñÆ®: Ã¹¹øÂ° ºñÆ®(¸¶¹ý º¸È£) µÎ¹øÂ° ºñÆ®(¹°¸® °ø°Ý º¸È£) ¼¼¹øÂ° ºñÆ®(¾ÈÀüÁö´ë)
	
	
};

#endif // !defined(AFX_TILE_H__12609161_8060_11D2_A8E6_00001C7030A6__INCLUDED_)
