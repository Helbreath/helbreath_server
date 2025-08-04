//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include "Client.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClient::CClient(HWND hWnd)
{
 int i;

	m_pXSock = NULL;
	m_pXSock = new class XSocket(hWnd, DEF_CLIENTSOCKETBLOCKLIMIT);
	m_pXSock->bInitBufferSize(DEF_MSGBUFFERSIZE);

	memset(m_cProfile, 0, sizeof(m_cProfile));
	strcpy(m_cProfile, "__________");

	memset(m_cCharName, 0, sizeof(m_cCharName));
	memset(m_cAccountName, 0, sizeof(m_cAccountName));
	memset(m_cAccountPassword, 0, sizeof(m_cAccountPassword));

	memset(m_cGuildName, 0, sizeof(m_cGuildName));
	memset(m_cLocation, 0, sizeof(m_cLocation));
	strcpy(m_cLocation, "NONE");
	m_iGuildRank = -1;
	m_iGuildGUID = -1;

	m_bIsInitComplete = false;

	//m_cLU_Str = m_cLU_Int = m_cLU_Vit = m_cLU_Dex = m_cLU_Mag = m_cLU_Char = 0;
	m_iLU_Pool = 0;
	m_cAura = 0;

	// v1.432 »ç¿ëÇÏÁö ¾Ê´Â´Ù.
	//m_iHitRatio_ItemEffect_SM = 0;
	//m_iHitRatio_ItemEffect_L  = 0;
	m_cVar = 0;
	m_iEnemyKillCount = 0;
	m_iPKCount = 0;
	m_iRewardGold = 0;
	m_iCurWeightLoad = 0;
	m_dwLogoutHackCheck = 0;

	// Charges
	m_iAddTransMana = 0;
	m_iAddChargeCritical = 0;

	m_bIsSafeAttackMode  = false;

	// ¾ÆÀÌÅÛ ÀåÂø »óÅÂ ÃÊ±âÈ­ÇÑ ÈÄ ¼³Á¤ÇÑ´Ù.
	for (i = 0; i < DEF_MAXITEMEQUIPPOS; i++) 
		m_sItemEquipmentStatus[i] = -1;
	
	// ¾ÆÀÌÅÛ ¸®½ºÆ® ÃÊ±âÈ­ 
	for (i = 0; i < DEF_MAXITEMS; i++) {
		m_pItemList[i]       = NULL;
		m_ItemPosList[i].x   = 40;
		m_ItemPosList[i].y   = 30;
		m_bIsItemEquipped[i] = false;
	}
	m_cArrowIndex = -1;	// È­»ì ¾ÆÀÌÅÛ ÀÎµ¦½º´Â ÇÒ´çµÇÁö ¾ÊÀº »óÅÂ 

	// ¸Ã°Ü³í ¾ÆÀÌÅÛ ¸®½ºÆ® ÃÊ±âÈ­.
	for (i = 0; i < DEF_MAXBANKITEMS; i++) {
		m_pItemInBankList[i] = NULL;
	}

	// Magic - Skill ¼÷·Ãµµ ¸®½ºÆ® ÃÊ±âÈ­ 
	for (i = 0; i < DEF_MAXMAGICTYPE; i++)
		m_cMagicMastery[i] = NULL;
	
	for (i = 0; i < DEF_MAXSKILLTYPE; i++)
		m_cSkillMastery[i] = NULL;

	for (i = 0; i < DEF_MAXSKILLTYPE; i++) {
		m_bSkillUsingStatus[i] = false;
		m_iSkillUsingTimeID[i] = NULL;
	}

	// testcode
	m_cMapIndex = -1;
	m_sX = -1;
	m_sY = -1;
	m_cDir = 5; 
	m_sType   = 0;
	m_sOriginalType = 0;
	m_sAppr1  = 0;
	m_sAppr2  = 0;
	m_sAppr3  = 0;
	m_sAppr4  = 0;
	m_iApprColor = 0; // v1.4
	m_iStatus = 0;

	m_cSex  = 0;
	m_cSkin = 0;
	m_cHairStyle  = 0;
	m_cHairColor  = 0;
	m_cUnderwear  = 0;

	m_cAttackDiceThrow_SM = 0;	// °ø°ÝÄ¡ ÁÖ»çÀ§ ´øÁö´Â È¸¼ö @@@@@@@@@@@@@
	m_cAttackDiceRange_SM = 0;
	m_cAttackDiceThrow_L = 0;	// °ø°ÝÄ¡ ÁÖ»çÀ§ ´øÁö´Â È¸¼ö @@@@@@@@@@@@@
	m_cAttackDiceRange_L = 0;
	m_cAttackBonus_SM    = 0;
	m_cAttackBonus_L     = 0;
	
	// ÇÃ·¹ÀÌ¾îÀÇ ¼Ò¼Ó ¸¶À»¿¡ µû¶ó¼­ »çÀÌµå°¡ °áÁ¤µÇ¸ç ÀÌ°ÍÀ» º¸°í NPC°¡ °ø°Ý¿©ºÎ¸¦ °áÁ¤ÇÒ °ÍÀÌ´Ù. 
	m_cSide = 0;

	m_iHitRatio = 0;
	m_iDefenseRatio = 0;
	
	for (i = 0; i < DEF_MAXITEMEQUIPPOS; i++) m_iDamageAbsorption_Armor[i]  = 0;
	m_iDamageAbsorption_Shield = 0;

	m_iHPstock = 0;
	m_bIsKilled = false;

	for (i = 0; i < DEF_MAXMAGICEFFECTS; i++) 
		m_cMagicEffectStatus[i]	= 0;

	m_iWhisperPlayerIndex = -1;
	memset(m_cWhisperPlayerName, 0, sizeof(m_cWhisperPlayerName));

	m_iHungerStatus  = 100;  // ÃÖ´ë°ªÀº 100
	
	m_bIsWarLocation = false;

	m_bIsPoisoned    = false;
	m_iPoisonLevel   = NULL;

	m_iAdminUserLevel  = 0;
	m_iRating          = 0;
	m_iTimeLeft_ShutUp = 0;
	m_iTimeLeft_Rating = 0;
	m_iTimeLeft_ForceRecall  = 0;
	m_iTimeLeft_FirmStaminar = 0;
	
	m_iRecentWalkTime  = 0;
	m_iRecentRunTime   = 0;
	m_sV1			   = 0;

	m_bIsOnServerChange  = false;
	m_bInhibition = false;

	m_iExpStock = 0;

	m_iAllocatedFish = NULL;
	m_iFishChance    = 0;

	memset(m_cIPaddress, 0, sizeof(m_cIPaddress)); 
	m_bIsOnWaitingProcess = false;

	m_iSuperAttackLeft  = 0;
	m_iSuperAttackCount = 0;

	m_sUsingWeaponSkill = 5; // ±âº»ÀûÀ¸·Î ¸Ç¼Õ°ÝÅõ 

	m_iManaSaveRatio   = 0;
	m_iAddResistMagic  = 0;
	m_iAddPhysicalDamage = 0;
	m_iAddMagicalDamage  = 0;
	m_bIsLuckyEffect     = false;
	m_iSideEffect_MaxHPdown = 0;

	m_iAddAbsAir   = 0;	// ¼Ó¼ºº° ´ë¹ÌÁö Èí¼ö
	m_iAddAbsEarth = 0;
	m_iAddAbsFire  = 0;
	m_iAddAbsWater = 0;

	m_iComboAttackCount = 0;
	m_iDownSkillIndex   = -1;
	m_bInRecallImpossibleMap = 0;

	m_iMagicDamageSaveItemIndex = -1;

	m_sCharIDnum1 = m_sCharIDnum2 = m_sCharIDnum3 = 0;

	// New 06/05/2004
	m_iPartyID = 0;
	m_iPartyStatus = 0;
	m_iReqJoinPartyClientH = 0;
	ZeroMemory(m_cReqJoinPartyName,sizeof(m_cReqJoinPartyName));

	/*m_iPartyRank = -1; // v1.42
	m_iPartyMemberCount = 0;
	m_iPartyGUID        = 0;

	for (i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
		m_stPartyMemberName[i].iIndex = 0;
		memset(m_stPartyMemberName[i].cName, 0, sizeof(m_stPartyMemberName[i].cName));
	}*/

	m_iAbuseCount     = 0;
	m_bIsBWMonitor    = false;
	m_bIsExchangeMode = false;

	//hbest
	isForceSet = false;

	// v1.4311-3 Ãß°¡ º¯¼ö ÃÊ±âÈ­ »çÅõÀå ¿¹¾à °ü·Ã º¯¼ö 
    m_iFightZoneTicketNumber =	m_iFightzoneNumber = m_iReserveTime = 0 ;            

	m_iPenaltyBlockYear = m_iPenaltyBlockMonth = m_iPenaltyBlockDay = 0; // v1.4

	m_iExchangeH = NULL;											// ±³È¯ÇÒ ´ë»óÀÇ ÀÎµ¦½º 
	memset(m_cExchangeName, 0, sizeof(m_cExchangeName));			// ±³È¯ÇÒ ´ë»óÀÇ ÀÌ¸§ 
	memset(m_cExchangeItemName, 0, sizeof(m_cExchangeItemName));	// ±³È¯ÇÒ ¾ÆÀÌÅÛ ÀÌ¸§ 

	for(i=0; i<4; i++){
		m_cExchangeItemIndex[i]  = -1; 
		m_iExchangeItemAmount[i] = 0;
	}

	m_bIsExchangeConfirm = false;

	m_iQuest		 = NULL; // ÇöÀç ÇÒ´çµÈ Quest 
	m_iQuestID       = NULL; // QuestID
	m_iAskedQuest	 = NULL; // ¹°¾îº» Äù½ºÆ® 
	m_iCurQuestCount = NULL; // ÇöÀç Äù½ºÆ® »óÅÂ 

	m_iQuestRewardType	 = NULL; // Äù½ºÆ® ÇØ°á½Ã »óÇ° Á¾·ù -> ¾ÆÀÌÅÛÀÇ ID°ªÀÌ´Ù.
	m_iQuestRewardAmount = NULL; // »óÇ° °¹¼ö 

	m_iContribution = NULL;			// °øÇåµµ 
	m_bQuestMatchFlag_Loc = false;  // Äù½ºÆ® Àå¼Ò È®ÀÎ¿ë ÇÃ·¡±×.
	m_bIsQuestCompleted   = false;

	m_cHeroArmourBonus = 0;

	m_bIsNeutral      = false;
	m_bIsObserverMode = false;

	// 2000.8.1 ÀÌº¥Æ® »óÇ° ¼ö¿© È®ÀÎ¿ë 
	m_iSpecialEventID = 200081;

	m_iSpecialWeaponEffectType  = 0;	// Èñ±Í ¾ÆÀÌÅÛ È¿°ú Á¾·ù: 0-None 1-ÇÊ»ì±â´ë¹ÌÁöÃß°¡ 2-Áßµ¶È¿°ú 3-Á¤ÀÇÀÇ 4-ÀúÁÖÀÇ
	m_iSpecialWeaponEffectValue = 0;	// Èñ±Í ¾ÆÀÌÅÛ È¿°ú °ª

	m_iAddHP = m_iAddSP = m_iAddMP = 0; 
	m_iAddAR = m_iAddPR = m_iAddDR = 0;
	m_iAddAbsPD = m_iAddAbsMD = 0;
	m_iAddCD = m_iAddExp = m_iAddGold = 0;
		
	m_iSpecialAbilityTime = DEF_SPECABLTYTIMESEC;		// DEF_SPECABLTYTIMESEC ÃÊ¸¶´Ù ÇÑ¹ø¾¿ Æ¯¼ö ´É·ÂÀ» ¾µ ¼ö ÀÖ´Ù.
	m_iSpecialAbilityType = NULL;
	m_bIsSpecialAbilityEnabled = false;
	m_iSpecialAbilityLastSec   = 0;

	m_iSpecialAbilityEquipPos  = 0;

	m_iMoveMsgRecvCount   = 0;
	m_iAttackMsgRecvCount = 0;
	m_iRunMsgRecvCount    = 0;
	m_iSkillMsgRecvCount  = 0;

	m_bIsAdminCommandEnabled = false;
	m_iAlterItemDropIndex = -1;

	m_iAutoExpAmount = 0;
	m_iWarContribution = 0;

	m_dwMoveLAT = m_dwRunLAT = m_dwAttackLAT = 0;

	m_dwInitCCTimeRcv = 0;
	m_dwInitCCTime = 0;

	memset(m_cLockedMapName, 0, sizeof(m_cLockedMapName));
	strcpy(m_cLockedMapName, "NONE");
	m_iLockedMapTime = NULL;

	m_iCrusadeDuty  = NULL;
	m_dwCrusadeGUID = NULL;
	m_dwHeldenianGUID = NULL;

	for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++) {
		m_stCrusadeStructureInfo[i].cType = NULL;
		m_stCrusadeStructureInfo[i].cSide = NULL;
		m_stCrusadeStructureInfo[i].sX = NULL;
		m_stCrusadeStructureInfo[i].sY = NULL;
	}

	m_iCSIsendPoint = NULL;

	m_bIsSendingMapStatus = false;
	memset(m_cSendingMapName, 0, sizeof(m_cSendingMapName));

	m_iConstructionPoint = NULL;

	memset(m_cConstructMapName, 0, sizeof(m_cConstructMapName));
	m_iConstructLocX = m_iConstructLocY = -1;

	m_bIsAdminOrderGoto = false;
	m_bIsInsideWarehouse = false;
	m_bIsInsideWizardTower = false;
	m_bIsInsideOwnTown = false;
	m_bIsCheckingWhisperPlayer = false;
	m_bIsOwnLocation = false;
	m_pIsProcessingAllowed = false;

	m_cHeroArmorBonus = 0;

	m_bIsBeingResurrected = false;
	m_bMagicConfirm = false;
	m_bMagicItem = false;
	m_iSpellCount = 0;
	m_bMagicPauseTime = false;
}

CClient::~CClient()
{
 int i;
	
	if (m_pXSock != NULL) delete m_pXSock;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (m_pItemList[i] != NULL) {
			delete m_pItemList[i];
			m_pItemList[i] = NULL;
		}
	for(i = 0; i < DEF_MAXBANKITEMS; i++)
		if (m_pItemInBankList[i] != NULL) {
			delete m_pItemInBankList[i];
			m_pItemInBankList[i]=NULL;
		}
}

bool CClient::bCreateNewParty()
{
 int i;

	if (m_iPartyRank != -1) return false;

	m_iPartyRank = 0;
	m_iPartyMemberCount = 0;
	m_iPartyGUID = (rand() % 999999) + timeGetTime();

	for (i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
		m_stPartyMemberName[i].iIndex = 0;
		memset(m_stPartyMemberName[i].cName, 0, sizeof(m_stPartyMemberName[i].cName));
	}

	return true;
}
