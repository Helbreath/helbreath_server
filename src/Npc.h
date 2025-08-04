//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#if !defined(AFX_NPC_H__6F7A5C80_A709_11D2_B143_00001C7030A6__INCLUDED_)
#define AFX_NPC_H__6F7A5C80_A709_11D2_B143_00001C7030A6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include <windows.h>
#include "Magic.h"

#define DEF_MAXWAYPOINTS			10

#define DEF_MOVETYPE_STOP			0		// Á¤ÁöÇØ ÀÖ´Â´Ù. ÃÊ±âÈ­µÈ NPCÀÇ ±âº» Çàµ¿.
#define DEF_MOVETYPE_SEQWAYPOINT	1		// ÀÌµ¿°æ·Î¸¦ ¼øÂ÷ÀûÀ¸·Î ÀÌµ¿. ¸Ç ¸¶Áö¸· ÀÌµ¿°æ·Î¿¡¼­ ÇØ´ç Çàµ¿À» °è¼ÓÇÑ´Ù.
#define DEF_MOVETYPE_RANDOMWAYPOINT	2		// ÀÌµ¿°æ·Î¸¦ ·£´ýÇÏ°Ô ¼±ÅÃÇÏ¿© ¿òÁ÷ÀÎ´Ù.
#define DEF_MOVETYPE_FOLLOW			3		// Æ¯Á¤ ´ë»óÀ» µû¶ó ¿òÁ÷ÀÎ´Ù. 
#define DEF_MOVETYPE_RANDOMAREA		4		// ÁöÁ¤µÈ ¿µ¿ª ¾È¿¡¼­ ·£´ýÇÑ °÷À¸·Î ¿òÁ÷ÀÎ´Ù.
#define DEF_MOVETYPE_RANDOM			5		// ¸Ê ³»ÀÇ ¾Æ¹«¿µ¿ªÀ¸·Î³ª ¿òÁ÷ÀÎ´Ù.
#define DEF_MOVETYPE_GUARD			6		// Ã³À½ »ý¼ºµÉ ´ç½ÃÀÇ À§Ä¡¿¡ Á¤ÁöÇØ ÀÖÀ¸³ª ¸ñÇ¥¹° ¹ß°ß½Ã´Â Ãß°ÝÇÑ´Ù.

#define DEF_BEHAVIOR_STOP			0
#define DEF_BEHAVIOR_MOVE			1
#define DEF_BEHAVIOR_ATTACK			2
#define DEF_BEHAVIOR_FLEE			3
#define DEF_BEHAVIOR_DEAD			4

class CNpc  
{
public:
	CNpc(char * pName5);
	virtual ~CNpc();

	// Auras
	char m_pMagicConfigList[100];

	char  m_cNpcName[21]; // NPCÀÇ ÀÌ¸§. »ç¶÷ ÀÌ¸§°°Àº °Í

	char  m_cName[6];	// ÀÌ ÀÌ¸§Àº ÇÁ·Î±×·¥À» À§ÇÑ ½Äº°ÀÚÀÓ
	char  m_cMapIndex;
	short m_sX, m_sY;
	short m_dX, m_dY;
	short m_vX, m_vY;
	int   m_tmp_iError;
	RECT  m_rcRandomArea;	// MOVETYPE_RANDOMAREAÀÏ¶§ ¿µ¿ªÁÂÇ¥

	char  m_cDir;
	char  m_cAction;
	char  m_cTurn;

	short m_sType;
	short m_sOriginalType;
	short m_sAppr2;
	int	  m_iStatus;

	uint32_t m_dwTime;
	uint32_t m_dwActionTime;
	uint32_t m_dwHPupTime, m_dwMPupTime;
	uint32_t m_dwDeadTime, m_dwRegenTime;

	int  m_iHP;						// Hit Point 
	int  m_iExp;                    // Á×¿´À» °æ¿ì ¾ò´Â °æÇèÄ¡. ExpDice¿¡ ÀÇÇØ °áÁ¤µÈ´Ù.

	int  m_iHitDice;				// ¸ó½ºÅÍÀÇ Hit Dice. ÀÌ°Í¿¡ ÀÇÇØ HP°¡ °áÁ¤µÈ´Ù.
	int  m_iDefenseRatio;			// Defense Ratio
	int  m_iHitRatio;				// HitRatio
	int  m_iMagicHitRatio;			// v1.4 ¸¶¹ý °ø°Ý Ratio
	int  m_iMinBravery;			    // ÃÖ¼Ò ¿ë°¨Ä¡ 
	int  m_iExpDiceMin;
	int	 m_iExpDiceMax;
	int  m_iGoldDiceMin;
	int  m_iGoldDiceMax;

	char m_cSide;					// NPC°¡ ¼ÓÇÑ ÆÀ 
	char m_cActionLimit;            // ÀÌ °ªÀÌ 1ÀÌ¸é Move¸¸ °¡´ÉÇÏ¸ç Á×Áöµµ ¾Ê´Â´Ù.  ÀÌ °ªÀÌ 2¸é ¿òÁ÷ÀÌÁöµµ ¾ÊÀ¸¸ç Á×Áöµµ ¾Ê´Â´Ù. 3ÀÌ¸é Dummy´Ù. ¸Â´Â ¿ªÇÒ, 
	                                // 4¸é ¿¡³ÊÁö ½ºÇÇ¾î, 5¸é °ÇÃà¹°   

	char m_cSize;					// 0: Small-Medium 1: Large
	char m_cAttackDiceThrow;		// °ø°ÝÄ¡ ÁÖ»çÀ§ ´øÁö´Â È¸¼ö 
	char m_cAttackDiceRange;		// °ø°ÝÄ¡ ÁÖ»çÀ§ ¹üÀ§ 
	char m_cAttackBonus;			// °ø°Ý º¸³Ê½º 
	char m_cBravery;				// NPCÀÇ ¿ë°¨µµ 
	char m_cResistMagic;			// ¸¶¹ý ÀúÇ× 
	char m_cMagicLevel;				// ¸¶¹ý ·¹º§ 
	char m_cDayOfWeekLimit;		    // »ý¼º½Ã ³¯Â¥ Á¦ÇÑ 
	char m_cChatMsgPresence;		// ¿¬°áµÈ Chat Msg Á¸ÀçÀ¯¹« 
	int  m_iMana;                   // ÃÖ´ë°ªÀº MagicLevel*30
	int  m_iMaxMana;				// v1.4 »õ·Î Ãß°¡µÈ °Íµé.
																    
	char  m_cMoveType;				// NPCÀÇ ÀÌµ¿°æ·Î ¹æ½Ä 
	char  m_cBehavior;				// NPCÀÇ Çàµ¿ 
	short m_sBehaviorTurnCount;		// Çàµ¿ Ä«¿îÆ® 
	char  m_cTargetSearchRange;		// NPC°ø°Ý¸ñÇ¥ °Ë»ö ¹üÀ§ 

	int   m_iFollowOwnerIndex;		// NPC°¡ µû¶ó°¡°í ÀÖ´Â °´Ã¼ ÀÎµ¦½º 
	char  m_cFollowOwnerType;		// µû¶ó°¡´Â °´Ã¼ÀÇ Á¾·ù (NPC or Player)
	bool  m_bIsSummoned;            // ¼ÒÈ¯µÈ NPC¶ó¸é HP°¡ ¿À¸£Áö ¾Ê´Â´Ù. 
	uint32_t m_dwSummonedTime;			// ¼ÒÈ¯¸÷ÀÌ »ý¼º½Ã°£ 

	int   m_iTargetIndex;			// NPCÀÇ °ø°Ý¸ñÇ¥ ÀÎµ¦½º
	char  m_cTargetType;			// °ø°Ý¸ñÇ¥ÀÇ Á¾·ù (NPC or Player)
	char  m_cCurWaypoint;			// ÇöÀç ÀÌµ¿À§Ä¡ 
	char  m_cTotalWaypoint;			// ÃÑ ÀÌµ¿À§Ä¡ °¹¼ö 

	int   m_iSpotMobIndex;			// spot-mob-generator¿¡¼­ ¸¸µé¾îÁ³´Ù¸é ÀÎµ¦½º¸¦ ÀúÀå 
	int   m_iWayPointIndex[DEF_MAXWAYPOINTS+1];
	char  m_cMagicEffectStatus[DEF_MAXMAGICEFFECTS];

	bool  m_bIsPermAttackMode;		// ÀÌ ÇÃ·¡±×°¡ È°¼ºÈ­ µÇ¾î ÀÖÀ¸¸é °ø°ÝµµÁß ¸ñÇ¥¸¦ ¹Ù²ÙÁö ¾Ê´Â´Ù.
   	int   m_iNoDieRemainExp;		// Á×Áö ¾Ê°í ¸íÁß½Ã¿¡ ¾òÀ» ¼ö ÀÖ´Â °æÇèÄ¡ 
	int   m_iAttackStrategy; 		// ÀüÅõ ±³¸® ÀÎ°øÁö´É Å¸ÀÔ  
	int   m_iAILevel;				// Áö´É ·¹º§ 

	int   m_iAttackRange;			// °ø°Ý ¹üÀ§ 1ÀÌ»óÀÌ¸é ¿ø°Å¸® °ø°ÝÀÌ °¡´ÉÇÏ´Ù´Â °Í 
	/*
		AI-Level 
			1: º¸Åë Çàµ¿ 
			2: ¿¡³ÊÁö°¡ °¡Àå ³·Àº ¸ñÇ¥¹°À» ÃßÀû 
			3: ¸¶¹ý º¸È£ÁßÀÎ ¸ñÇ¥¹° ¸¶¹ý °ø°Ý´ë»ó¿¡¼­ Á¦¿Ü 
	*/
	int   m_iAttackCount;			// ¿¬¼Ó °ø°Ý µ¿ÀÛ È½¼ö Ä«¿îÆ® 
	bool  m_bIsKilled;
	bool  m_bIsUnsummoned;

	int   m_iLastDamage;			// ¸¶Áö¸· Á×±â Àü¿¡ ¾òÀº ´ë¹ÌÁö. ´ë¹ÌÁö°¡ ³Ê¹« Å©¸é ¿¬±Ý¼ú Àç·á°¡ ¶³¾îÁöÁö ¾Ê´Â´Ù.
	int   m_iSummonControlMode;		// ¼ÒÈ¯¸÷ Á¶Á¤ ¸ðµå: 0ÀÌ¸é Free, 1 Hold 2 Tgt 
	char  m_cAttribute;				// ¸¶¹ý ¼Ó¼º:  ¶¥ 1 ÇÏ´Ã 2 ºÒ 3 ¹° 4  
	int   m_iAbsDamage;				// ¾ç¼ö¸é ¸¶¹ý ´ë¹ÌÁö Àý°¨·ü, À½¼ö¸é ¹°¸® ´ë¹ÌÁö Àý°¨·ü 

	int   m_iItemRatio;				// Æ¯¼ö ¾ÆÀÌÅÛ È®·ü 
	int   m_iAssignedItem;			// ÇÒ´çµÈ ¾ÆÀÌÅÛ 

	char  m_cSpecialAbility;		// Æ¯¼ö ´É·Â 0: ¾øÀ½  

									/*
case 0: break;
case 1:  "Penetrating Invisibility"
case 2:  "Breaking Magic Protection"
case 3:  "Absorbing Physical Damage"
case 4:  "Absorbing Magical Damage"
case 5:  "Poisonous"
case 6:  "Extremely Poisonous"
case 7:  "Explosive"
case 8:  "Hi-Explosive" 

 ¸¸¾à ÀÌ °ªÀÌ 60º¸´Ù Å©¸é NPCÀÇ È¿°ú¿Í´Â ¹«°üÇÏ¹Ç·Î ¹«½ÃÇÑ´Ù.
									*/

	int	  m_iBuildCount;			// °¡µå Å¸¿öµî ±¸Á¶¹°ÀÎ °æ¿ì ¿Ï¼ºµÉ ¶§±îÁö ³²Àº °î±ªÀÌÁú È½¼ö¸¦ ÀÇ¹Ì. ÃÊ±â°ªÀº m_iMinBraveryÀÌ´Ù.
									// ÀÌ °ªÀÌ 0ÀÌ¾î¾ß¸¸ µ¿ÀÛÀ» ÇÑ´Ù.
	int   m_iManaStock;				// ±×·£µå ¸ÅÁ÷ Á¦³×·¹ÀÌÅÍÀÇ °æ¿ì ¸¶³ªÃàÀû·® 
	bool  m_bIsMaster;				// ¸¶½ºÅÍ ¸÷ÀÎÁöÀÇ ¿©ºÎ 
	int   m_iGuildGUID;				// ÀüÀï¿ë À¯´ÏÆ®ÀÎ °æ¿ì ¼ÒÈ¯ÀÚÀÇ ±æµå ¾ÆÀÌµð°¡ ÀÔ·ÂµÈ´Ù.
	
	char m_cCropType;
	char m_cCropSkill;

	int   m_iV1;
	char m_cArea;
	
	int m_iNpcItemType;
	int m_iNpcItemMax;
};

#endif // !defined(AFX_NPC_H__6F7A5C80_A709_11D2_B143_00001C7030A6__INCLUDED_)
