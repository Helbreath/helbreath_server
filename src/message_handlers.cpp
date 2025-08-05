//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include "Game.h"
#include "string_utils.h"

void CGame::NpcTalkHandler(int iClientH, int iWho)
{
    char cRewardName[21], cTargetName[21];
    int iResMode, iQuestNum, iQuestType, iRewardType, iRewardAmount, iContribution, iX, iY, iRange, iTargetType, iTargetCount;

    iQuestNum = 0;
    memset(cTargetName, 0, sizeof(cTargetName));
    if (m_pClientList[iClientH] == nullptr) return;
    switch (iWho)
    {
        case 1: break;
        case 2:	break;
        case 3:	break;
        case 4:
            iQuestNum = _iTalkToNpcResult_Cityhall(iClientH, &iQuestType, &iResMode, &iRewardType, &iRewardAmount, &iContribution, cTargetName, &iTargetType, &iTargetCount, &iX, &iY, &iRange);
            break;
        case 5: break;
        case 6:	break;
        case 32: break;
        case 21:
            iQuestNum = _iTalkToNpcResult_Guard(iClientH, &iQuestType, &iResMode, &iRewardType, &iRewardAmount, &iContribution, cTargetName, &iTargetType, &iTargetCount, &iX, &iY, &iRange);
            if (iQuestNum >= 1000) return;
            break;
    }

    memset(cRewardName, 0, sizeof(cRewardName));
    if (iQuestNum > 0)
    {
        if (iRewardType > 1)
        {
            strcpy(cRewardName, m_pItemConfigList[iRewardType]->m_cName);
        }
        else
        {
            switch (iRewardType)
            {
                case -10: strcpy(cRewardName, "¦µ¦F-í"); break;
            }
        }

        m_pClientList[iClientH]->m_iAskedQuest = iQuestNum;
        m_pClientList[iClientH]->m_iQuestRewardType = iRewardType;
        m_pClientList[iClientH]->m_iQuestRewardAmount = iRewardAmount;

        SendNotifyMsg(0, iClientH, DEF_NOTIFY_NPCTALK, iQuestType, iResMode, iRewardAmount, cRewardName, iContribution,
            iTargetType, iTargetCount, iX, iY, iRange, cTargetName);
    }
    else
    {
        switch (iQuestNum)
        {
            case  0: SendNotifyMsg(0, iClientH, DEF_NOTIFY_NPCTALK, (iWho + 130), 0, 0, 0, 0); break;
            case -1:
            case -2:
            case -3:
            case -4: SendNotifyMsg(0, iClientH, DEF_NOTIFY_NPCTALK, abs(iQuestNum) + 100, 0, 0, 0, 0); break;
            case -5: break;
        }
    }
}

void CGame::_TamingHandler(int iClientH, int iSkillNum, char cMapIndex, int dX, int dY)
{
    int iSkillLevel, iRange, iTamingLevel, iResult, iX, iY;
    short sOwnerH;
    char  cOwnerType;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pMapList[cMapIndex] == 0) return;

    iSkillLevel = (int)m_pClientList[iClientH]->m_cSkillMastery[iSkillNum];
    iRange = iSkillLevel / 12; // ÃƒÃ–Â´Ã« Â¹Ã¼Ã€Â§Â´Ã‚ 8

    for (iX = dX - iRange; iX <= dX + iRange; iX++)
        for (iY = dY - iRange; iY <= dY + iRange; iY++)
        {
            sOwnerH = 0;
            if ((iX > 0) && (iY > 0) && (iX < m_pMapList[cMapIndex]->m_sSizeX) && (iY < m_pMapList[cMapIndex]->m_sSizeY))
                m_pMapList[cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, iX, iY);

            if (sOwnerH != 0)
            {
                switch (cOwnerType)
                {
                    case DEF_OWNERTYPE_PLAYER:
                        if (m_pClientList[sOwnerH] == 0) break;
                        break;

                    case DEF_OWNERTYPE_NPC:
                        if (m_pNpcList[sOwnerH] == 0) break;
                        // Â¸Ã³Â½ÂºÃ…ÃÃ€Ã‡ ÃÂ¾Â·Ã¹Â¿Â¡ ÂµÃ»Â¶Ã³ Â±Ã¦ÂµÃ©Â¿Â©ÃÃº Â°ÃÃ€ÃŽÃÃ¶Â¸Â¦ Â°Ã¡ÃÂ¤Ã‡Ã‘Â´Ã™.
                        iTamingLevel = 10;
                        switch (m_pNpcList[sOwnerH]->m_sType)
                        {
                            case 10:							// Â½Â½Â¶Ã³Ã€Ã“  
                            case 16: iTamingLevel = 1; break;	// Â°Â³Â¹ÃŒ
                            case 22: iTamingLevel = 2; break;	// Â¹Ã¬
                            case 17:							// Ã€Ã¼Â°Â¥
                            case 14: iTamingLevel = 3; break;	// Â¿Ã€Ã…Â©
                            case 18: iTamingLevel = 4; break;   // ÃÂ»ÂºÃ±
                            case 11: iTamingLevel = 5; break;	// Â½ÂºÃ„ÃŒÂ·Â¹Ã…Ã¦
                            case 23:
                            case 12: iTamingLevel = 6; break;	// Â°Ã±Â·Â½
                            case 28: iTamingLevel = 7; break;	// Ã†Â®Â·Ã‘
                            case 13:							// Â½ÃŽÃ€ÃŒÃ…Â¬Â·Ã“Â½Âº
                            case 27: iTamingLevel = 8; break;	// Ã‡Ã¯Ã‡ÃÂ¿Ã®ÂµÃ¥
                            case 29: iTamingLevel = 9; break;	// Â¿Ã€Â¿Ã¬Â°Ã…
                            case 33: iTamingLevel = 9; break;	// Â¿Ã¾Â¾Ã®Â¿Ã¯Ã‡Ã
                            case 30: iTamingLevel = 9; break;  // Â¸Â®Ã„Â¡
                            case 31:
                            case 32: iTamingLevel = 10; break;  // ÂµÂ¥Â¸Ã³, Ã€Â¯Â´ÃÃ„Ãœ
                        }

                        iResult = (iSkillLevel / 10);

                        // Â½ÂºÃ…Â³Ã€Ã‡ ÂµÃ®Â±ÃžÂ¿Â¡ ÂµÃ»Â¶Ã³ Â±Ã¦ÂµÃ©Ã€Ã Â¼Ã¶ Ã€Ã–Â´Ã‚ Â°Â¹Â¼Ã¶Â°Â¡ Â´ÃžÂ¶Ã³ÃÃ¸Â´Ã™.
                        if (iResult < iTamingLevel) break;

                        break;
                }
            }
        }
}

void CGame::GetMagicAbilityHandler(int iClientH)
{
    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_cSkillMastery[4] != 0) return;

    // Â¸Â¶Â¹Ã½ Â´Ã‰Â·Ã‚ Â½ÂºÃ…Â³Ã€Â» 20Ã€Â¸Â·ÃŽ ÂºÂ¯Â°Ã¦ÃˆÃ„ Â¾Ã‹Â·ÃÃÃœ
    m_pClientList[iClientH]->m_cSkillMastery[4] = 20;
    SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILL, 4, m_pClientList[iClientH]->m_cSkillMastery[4], 0, 0);
    // v1.4311 Â½ÂºÃ…Â³Ã€Ã‡ ÃƒÃ‘ Ã‡Ã•Ã€Â» ÃƒÂ¼Ã…Â©
    bCheckTotalSkillMasteryPoints(iClientH, 4);
}

void CGame::ActivateSpecialAbilityHandler(int iClientH)
{
    uint32_t dwTime = timeGetTime();
    short sTemp;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_iSpecialAbilityTime != 0) return;
    if (m_pClientList[iClientH]->m_iSpecialAbilityType == 0) return;
    if (m_pClientList[iClientH]->m_bIsSpecialAbilityEnabled == true) return;

    m_pClientList[iClientH]->m_bIsSpecialAbilityEnabled = true;
    m_pClientList[iClientH]->m_dwSpecialAbilityStartTime = dwTime;

    m_pClientList[iClientH]->m_iSpecialAbilityTime = DEF_SPECABLTYTIMESEC;

    sTemp = m_pClientList[iClientH]->m_sAppr4;
    sTemp = sTemp & 0xFF0F;
    switch (m_pClientList[iClientH]->m_iSpecialAbilityType)
    {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            sTemp = sTemp | 0x0010;
            break;
        case 50:
        case 51:
        case 52:
        case 53:
        case 54:
            sTemp = sTemp | 0x0020;
            break;
    }
    m_pClientList[iClientH]->m_sAppr4 = sTemp;

    SendNotifyMsg(0, iClientH, DEF_NOTIFY_SPECIALABILITYSTATUS, 1, m_pClientList[iClientH]->m_iSpecialAbilityType, m_pClientList[iClientH]->m_iSpecialAbilityLastSec, 0);
    SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
}

void CGame::CancelQuestHandler(int iClientH)
{
    if (m_pClientList[iClientH] == nullptr) return;

    // Ã„Ã¹Â½ÂºÃ†Â® Â»Ã³Ã…Ã‚Â¸Â¦ Ã…Â¬Â¸Â®Â¾Ã®Ã‡Ã‘Â´Ã™.
    _ClearQuestStatus(iClientH);
    // Ã„Ã¹Â½ÂºÃ†Â®Â°Â¡ ÃƒÃ«Â¼Ã’ÂµÃ‡Â¾ÃºÃ€Â½Ã€Â» Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
    SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTABORTED, 0, 0, 0, 0);
}

bool CGame::bEquipItemHandler(int iClientH, short sItemIndex, bool bNotify)
{
    char  cEquipPos, cHeroArmorType;
    short   sSpeed;
    short sTemp;
    int iTemp;

    if (m_pClientList[iClientH] == nullptr) return false;
    if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return false;
    if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == 0) return false;
    if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType != DEF_ITEMTYPE_EQUIP) return false;

    if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan == 0) return false;

    if (((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x00000001) == 0) &&
        (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sLevelLimit > m_pClientList[iClientH]->m_iLevel)) return false;


    if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cGenderLimit != 0)
    {
        switch (m_pClientList[iClientH]->m_sType)
        {
            case 1:
            case 2:
            case 3:
                if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cGenderLimit != 1) return false;
                break;
            case 4:
            case 5:
            case 6:
                if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cGenderLimit != 2) return false;
                break;
        }
    }

    if (iGetItemWeight(m_pClientList[iClientH]->m_pItemList[sItemIndex], 1) > m_pClientList[iClientH]->m_iStr * 100) return false;

    cEquipPos = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos;

    if ((cEquipPos == DEF_EQUIPPOS_BODY) || (cEquipPos == DEF_EQUIPPOS_LEGGINGS) ||
        (cEquipPos == DEF_EQUIPPOS_ARMS) || (cEquipPos == DEF_EQUIPPOS_HEAD))
    {
        switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue4)
        {
            case 10: // Str
                if (m_pClientList[iClientH]->m_iStr < m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue5)
                {
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, 0, 0);
                    ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos], true);
                    return false;
                }
                break;
            case 11: // Dex
                if (m_pClientList[iClientH]->m_iDex < m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue5)
                {
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, 0, 0);
                    ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos], true);
                    return false;
                }
                break;
            case 12: // Vit
                if (m_pClientList[iClientH]->m_iVit < m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue5)
                {
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, 0, 0);
                    ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos], true);
                    return false;
                }
                break;
            case 13: // Int
                if (m_pClientList[iClientH]->m_iInt < m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue5)
                {
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, 0, 0);
                    ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos], true);
                    return false;
                }
                break;
            case 14: // Mag
                if (m_pClientList[iClientH]->m_iMag < m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue5)
                {
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, 0, 0);
                    ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos], true);
                    return false;
                }
                break;
            case 15: // Chr
                if (m_pClientList[iClientH]->m_iCharisma < m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue5)
                {
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, 0, 0);
                    ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos], true);
                    return false;
                }
                break;
        }
    }

    if (cEquipPos == DEF_EQUIPPOS_TWOHAND)
    {
        // Stormbringer
        if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 845)
        {
            if (m_pClientList[iClientH]->m_iInt < 65)
            {
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_iSpecialAbilityEquipPos, sItemIndex, 0, 0);
                ReleaseItemHandler(iClientH, sItemIndex, true);
                return false;
            }
        }
    }

    if (cEquipPos == DEF_EQUIPPOS_RHAND)
    {
        // Resurrection wand(MS.10) or Resurrection wand(MS.20)
        if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 865) || (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 866))
        {
            if (m_pClientList[iClientH]->m_iInt > 99 && m_pClientList[iClientH]->m_iMag > 99 && m_pClientList[iClientH]->m_iSpecialAbilityTime < 1)
            {
                m_pClientList[iClientH]->m_cMagicMastery[94] = true;
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_SUCCESS, 0, 0, 0, 0);
            }
        }
    }

    if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY) ||
        (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY))
    {

        if ((m_pClientList[iClientH]->m_iSpecialAbilityType != 0))
        {
            if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos != m_pClientList[iClientH]->m_iSpecialAbilityEquipPos)
            {
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_iSpecialAbilityEquipPos, m_pClientList[iClientH]->m_sItemEquipmentStatus[m_pClientList[iClientH]->m_iSpecialAbilityEquipPos], 0, 0);
                ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[m_pClientList[iClientH]->m_iSpecialAbilityEquipPos], true);
            }
        }
    }


    if (cEquipPos == DEF_EQUIPPOS_NONE) return false;

    if (cEquipPos == DEF_EQUIPPOS_TWOHAND)
    {
        if (m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos] != -1)
            ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos], false);
        else
        {
            if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND] != -1)
                ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND], false);
            if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LHAND] != -1)
                ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LHAND], false);
        }
    }
    else
    {
        if ((cEquipPos == DEF_EQUIPPOS_LHAND) || (cEquipPos == DEF_EQUIPPOS_RHAND))
        {
            if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1)
                ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND], false);
        }

        if (m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos] != -1)
            ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos], false);
    }


    if (cEquipPos == DEF_EQUIPPOS_RELEASEALL)
    {
        if (m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos] != -1)
        {
            ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos], false);
        }
        if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_HEAD] != -1)
        {
            ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_HEAD], false);
        }
        if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_BODY] != -1)
        {
            ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_BODY], false);
        }
        if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_ARMS] != -1)
        {
            ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_ARMS], false);
        }
        if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LEGGINGS] != -1)
        {
            ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LEGGINGS], false);
        }
        if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_PANTS] != -1)
        {
            ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_PANTS], false);
        }
        if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_BACK] != -1)
        {
            ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_BACK], false);
        }
    }
    else
    {
        if (cEquipPos == DEF_EQUIPPOS_HEAD || cEquipPos == DEF_EQUIPPOS_BODY || cEquipPos == DEF_EQUIPPOS_ARMS ||
            cEquipPos == DEF_EQUIPPOS_LEGGINGS || cEquipPos == DEF_EQUIPPOS_PANTS || cEquipPos == DEF_EQUIPPOS_BACK)
        {
            if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RELEASEALL] != -1)
            {
                ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RELEASEALL], false);
            }
        }
        if (m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos] != -1)
            ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos], false);
    }


    m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos] = sItemIndex;
    m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] = true;

    switch (cEquipPos)
    {

        case DEF_EQUIPPOS_HEAD:
            sTemp = m_pClientList[iClientH]->m_sAppr3;
            sTemp = sTemp & 0xFF0F;
            sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) << 4);
            m_pClientList[iClientH]->m_sAppr3 = sTemp;

            iTemp = m_pClientList[iClientH]->m_iApprColor;
            iTemp = iTemp & 0xFFFFFFF0;
            iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor));
            m_pClientList[iClientH]->m_iApprColor = iTemp;
            break;

        case DEF_EQUIPPOS_PANTS:
            sTemp = m_pClientList[iClientH]->m_sAppr3;
            sTemp = sTemp & 0xF0FF;
            sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) << 8);
            m_pClientList[iClientH]->m_sAppr3 = sTemp;

            iTemp = m_pClientList[iClientH]->m_iApprColor;
            iTemp = iTemp & 0xFFFFF0FF;
            iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor) << 8);
            m_pClientList[iClientH]->m_iApprColor = iTemp;
            break;

        case DEF_EQUIPPOS_LEGGINGS:
            sTemp = m_pClientList[iClientH]->m_sAppr4;
            sTemp = sTemp & 0x0FFF;
            sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) << 12); // Appr Â°ÂªÃ€Â» Â¼Â¼Ã†Ãƒ. 
            m_pClientList[iClientH]->m_sAppr4 = sTemp;

            iTemp = m_pClientList[iClientH]->m_iApprColor;
            iTemp = iTemp & 0xFFFFFF0F;
            iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor) << 4);
            m_pClientList[iClientH]->m_iApprColor = iTemp;
            break;

        case DEF_EQUIPPOS_BODY:
            sTemp = m_pClientList[iClientH]->m_sAppr3;
            sTemp = sTemp & 0x0FFF;

            if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue < 100)
            {
                sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) << 12);
                m_pClientList[iClientH]->m_sAppr3 = sTemp;
            }
            else
            {
                sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue - 100) << 12);
                m_pClientList[iClientH]->m_sAppr3 = sTemp;
                sTemp = m_pClientList[iClientH]->m_sAppr4;
                sTemp = sTemp | 0x080;
                m_pClientList[iClientH]->m_sAppr4 = sTemp;
            }

            iTemp = m_pClientList[iClientH]->m_iApprColor;
            iTemp = iTemp & 0xFF0FFFFF;
            iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor) << 20);
            m_pClientList[iClientH]->m_iApprColor = iTemp;
            break;

        case DEF_EQUIPPOS_ARMS:
            sTemp = m_pClientList[iClientH]->m_sAppr3;
            sTemp = sTemp & 0xFFF0;
            sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue));
            m_pClientList[iClientH]->m_sAppr3 = sTemp;

            iTemp = m_pClientList[iClientH]->m_iApprColor;
            iTemp = iTemp & 0xFFFFFFFF;
            iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor) << 12);
            m_pClientList[iClientH]->m_iApprColor = iTemp;
            break;

        case DEF_EQUIPPOS_LHAND:
            sTemp = m_pClientList[iClientH]->m_sAppr2;
            sTemp = sTemp & 0xFFF0;
            sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue));
            m_pClientList[iClientH]->m_sAppr2 = sTemp;

            iTemp = m_pClientList[iClientH]->m_iApprColor;
            iTemp = iTemp & 0xF0FFFFFF;
            iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor) << 24);
            m_pClientList[iClientH]->m_iApprColor = iTemp;
            break;

        case DEF_EQUIPPOS_RHAND:
            sTemp = m_pClientList[iClientH]->m_sAppr2;
            sTemp = sTemp & 0xF00F;
            sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) << 4);
            m_pClientList[iClientH]->m_sAppr2 = sTemp;

            iTemp = m_pClientList[iClientH]->m_iApprColor;
            iTemp = iTemp & 0xFFFFFFF;
            iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor) << 28);
            m_pClientList[iClientH]->m_iApprColor = iTemp;

            iTemp = m_pClientList[iClientH]->m_iStatus;
            iTemp = iTemp & 0xFFFFFFF0;
            sSpeed = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cSpeed);
            sSpeed -= (m_pClientList[iClientH]->m_iStr / 13);
            if (sSpeed < 0) sSpeed = 0;
            iTemp = iTemp | (int)sSpeed;
            m_pClientList[iClientH]->m_iStatus = iTemp;
            m_pClientList[iClientH]->m_iComboAttackCount = 0;
            break;

        case DEF_EQUIPPOS_TWOHAND:
            sTemp = m_pClientList[iClientH]->m_sAppr2;
            sTemp = sTemp & 0xF00F;
            sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) << 4); // Appr Â°ÂªÃ€Â» Â¼Â¼Ã†Ãƒ. 
            m_pClientList[iClientH]->m_sAppr2 = sTemp;

            iTemp = m_pClientList[iClientH]->m_iApprColor;
            iTemp = iTemp & 0xFFFFFFF;
            iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor) << 28);
            m_pClientList[iClientH]->m_iApprColor = iTemp;

            iTemp = m_pClientList[iClientH]->m_iStatus;
            iTemp = iTemp & 0xFFFFFFF0;
            sSpeed = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cSpeed);
            sSpeed -= (m_pClientList[iClientH]->m_iStr / 13);
            if (sSpeed < 0) sSpeed = 0;
            iTemp = iTemp | (int)sSpeed;
            m_pClientList[iClientH]->m_iStatus = iTemp;
            m_pClientList[iClientH]->m_iComboAttackCount = 0;
            break;

        case DEF_EQUIPPOS_BACK:
            sTemp = m_pClientList[iClientH]->m_sAppr4;
            sTemp = sTemp & 0xF0FF;
            sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) << 8); // Appr Â°ÂªÃ€Â» Â¼Â¼Ã†Ãƒ. 
            m_pClientList[iClientH]->m_sAppr4 = sTemp;

            iTemp = m_pClientList[iClientH]->m_iApprColor;
            iTemp = iTemp & 0xFFF0FFFF;
            iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor) << 16);
            m_pClientList[iClientH]->m_iApprColor = iTemp;
            break;

        case DEF_EQUIPPOS_RELEASEALL:
            sTemp = m_pClientList[iClientH]->m_sAppr3;
            sTemp = sTemp & 0x0FFF;
            sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) << 12); // Appr Â°ÂªÃ€Â» Â¼Â¼Ã†Ãƒ. 
            m_pClientList[iClientH]->m_sAppr3 = sTemp;

            iTemp = m_pClientList[iClientH]->m_iApprColor;
            iTemp = iTemp & 0xFFF0FFFF;
            m_pClientList[iClientH]->m_iApprColor = iTemp;
            break;
    }

    if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY)
    {
        m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 & 0xFFF3;
        switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpecialEffect)
        {
            case 0: break;
            case 1:
                m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 | 0x0004;
                break;

            case 2:
                m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 | 0x000C;
                break;

            case 3:
                m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 | 0x0008;
                break;
        }
    }

    if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY)
    {
        m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 & 0xFFFC;
        switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpecialEffect)
        {
            case 0:
                break;
            case 50:
            case 51:
            case 52:
                m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 | 0x0002;
                break;
            default:
                if (m_pClientList[iClientH]->m_iAdminUserLevel > 0)
                    m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 | 0x0001;
                // m_sAppr4
                // 0x0001 GM	
                // 0x0002 Green	
                // 0x0003 ice element
                // 0x0004 sparkle
                // 0x0005 sparkle green gm
                // 0x0006 sparkle green
                break;
        }
    }

    cHeroArmorType = _cCheckHeroItemEquipped(iClientH);
    if (cHeroArmorType != 0x0FFFFFFFF) m_pClientList[iClientH]->m_cHeroArmourBonus = cHeroArmorType;

    SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
    CalcTotalItemEffect(iClientH, sItemIndex, bNotify);
    return true;

}

void CGame::BuildItemHandler(int iClientH, char * pData)
{
    char * cp, cName[21], cElementItemID[6];
    int    i, x, z, iMatch, iCount, iPlayerSkillLevel, iResult, iTotalValue, iResultValue, iTemp, iItemCount[DEF_MAXITEMS];
    class  CItem * pItem;
    bool   bFlag, bItemFlag[6];
    double dV1, dV2, dV3;
    uint32_t  dwTemp, dwTemp2;
    uint16_t   wTemp;

    // »ç¿ëÀÚ°¡ ¾ÆÀÌÅÛ Á¦ÀÛÀ» ½ÅÃ»Çß´Ù. 
    if (m_pClientList[iClientH] == nullptr) return;
    m_pClientList[iClientH]->m_iSkillMsgRecvCount++;

    cp = (char *)(pData + 11);
    memset(cName, 0, sizeof(cName));
    memcpy(cName, cp, 20);
    cp += 20;

    //testcode
    //log->info(cName);

    memset(cElementItemID, 0, sizeof(cElementItemID));
    cElementItemID[0] = *cp;
    cp++;
    cElementItemID[1] = *cp;
    cp++;
    cElementItemID[2] = *cp;
    cp++;
    cElementItemID[3] = *cp;
    cp++;
    cElementItemID[4] = *cp;
    cp++;
    cElementItemID[5] = *cp;
    cp++;

    // ¾ÕºÎºÐÀÇ ºó°ø°£À» ¾ø¾Ø´Ù.
    bFlag = true;
    while (bFlag == true)
    {
        bFlag = false;
        for (i = 0; i <= 4; i++)
            if ((cElementItemID[i] == -1) && (cElementItemID[i + 1] != -1))
            {
                cElementItemID[i] = cElementItemID[i + 1];
                cElementItemID[i + 1] = -1;
                bFlag = true;
            }
    }

    for (i = 0; i < 6; i++) bItemFlag[i] = false;

    //testcode
    //copy_string(G_cTxt, "%d %d %d %d %d %d", cElementItemID[0], cElementItemID[1], cElementItemID[2],
    //	     cElementItemID[3], cElementItemID[4], cElementItemID[5]);
    //log->info(G_cTxt);

    iPlayerSkillLevel = m_pClientList[iClientH]->m_cSkillMastery[13];
    iResult = iDice(1, 100);

    if (iResult > iPlayerSkillLevel)
    {
        // ½ÇÆÐ´Ù. 
        // ¾ÆÀÌÅÛ Á¦ÀÛ¿¡ ½ÇÆÐÇÏ¿´´Ù.
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_BUILDITEMFAIL, 0, 0, 0, 0);
        return;
    }

    // °¢°¢ÀÇ ¾ÆÀÌÅÛÀÌ ´Ù Á¸ÀçÇÏ´ÂÁö °Ë»ç.
    for (i = 0; i < 6; i++)
        if (cElementItemID[i] != -1)
        {
            // Àß¸øµÈ Item ID°ªÀÌ´Ù. ¹«½Ã
            if ((cElementItemID[i] < 0) || (cElementItemID[i] > DEF_MAXITEMS)) return;
            if (m_pClientList[iClientH]->m_pItemList[cElementItemID[i]] == 0) return;
        }

    // ÇØ´ç ÀÌ¸§À» °¡Áø ¾ÆÀÌÅÛÀ» ºôµå ¾ÆÀÌÅÛ ¸®½ºÆ®¿¡¼­ Ã£´Â´Ù.
    for (i = 0; i < DEF_MAXBUILDITEMS; i++)
        if (m_pBuildItemList[i] != 0)
        {
            if (memcmp(m_pBuildItemList[i]->m_cName, cName, 20) == 0)
            {
                // ¾ÆÀÌÅÛÀ» Ã£¾Ò´Ù. Á¦ÀÛ Á¶°Ç°ú ¾ÆÀÌÅÛÀÌ ÀÏÄ¡ÇÏ´ÂÁö °Ë»ç.

                // ½ºÅ³ÀÌ ³·¾Æ¼­ Á¦ÀÛ ºÒ°¡´É. ¿ø·¡ Å¬¶óÀÌ¾ðÆ®¿¡¼­ °É·¯ Áø´Ù. 
                if (m_pBuildItemList[i]->m_iSkillLimit > m_pClientList[iClientH]->m_cSkillMastery[13]) return;

                for (x = 0; x < DEF_MAXITEMS; x++)
                    if (m_pClientList[iClientH]->m_pItemList[x] != 0)
                        iItemCount[x] = m_pClientList[iClientH]->m_pItemList[x]->m_dwCount;
                    else iItemCount[x] = 0;

                // ÇØ´ç ºôµå ¾ÆÀÌÅÛÀÇ Àç·á°¡ ÃæºÐÇÑÁö¸¦ °Ë»çÇÑ´Ù. ´õºÒ¾î ºôµå ¾ÆÀÌÅÛÀÇ °¡ÁßÄ¡ °ªµµ °è»ê.
                iMatch = 0;
                iTotalValue = 0;

                for (x = 0; x < 6; x++)
                {
                    if (m_pBuildItemList[i]->m_iMaterialItemCount[x] == 0)
                    {
                        iMatch++;
                    }
                    else
                    {
                        for (z = 0; z < 6; z++)
                            if ((cElementItemID[z] != -1) && (bItemFlag[z] == false))
                            {

                                if ((m_pClientList[iClientH]->m_pItemList[cElementItemID[z]]->m_sIDnum == m_pBuildItemList[i]->m_iMaterialItemID[x]) &&
                                    (m_pClientList[iClientH]->m_pItemList[cElementItemID[z]]->m_dwCount >= m_pBuildItemList[i]->m_iMaterialItemCount[x]) &&
                                    (iItemCount[cElementItemID[z]] > 0))
                                {
                                    // Àç·áÁß ÇÏ³ª¸¦ Ã£¾Ò´Ù. 
                                    iTemp = m_pClientList[iClientH]->m_pItemList[cElementItemID[z]]->m_sItemSpecEffectValue2;
                                    if (iTemp > m_pClientList[iClientH]->m_cSkillMastery[13])
                                    {
                                        // Àç·áÀÇ ¼øµµ°¡ ³» ½ºÅ³ ¼öÁØÀ» »óÈ¸ÇÑ´Ù¸é ¼øµµ¸¦ ³·Ãá´Ù. 
                                        iTemp = iTemp - (iTemp - m_pClientList[iClientH]->m_cSkillMastery[13]) / 2;
                                    }

                                    iTotalValue += (iTemp * m_pBuildItemList[i]->m_iMaterialItemValue[x]);
                                    iItemCount[cElementItemID[z]] -= m_pBuildItemList[i]->m_iMaterialItemCount[x];
                                    iMatch++;
                                    bItemFlag[z] = true;

                                    goto BIH_LOOPBREAK;
                                }
                            }
                        BIH_LOOPBREAK:;
                    }
                }

                // ¿©±â¼­ iMatch°¡ 6ÀÌ¸é ¾ÆÀÌÅÛ Á¦ÀÛ Á¶°ÇÀÌ ¸¸Á·µÈ °ÍÀÌ´Ù. 
                if (iMatch != 6)
                {
                    // Àç·á ºÎÁ·. ¾ÆÀÌÅÛ Á¦ÀÛ ºÒ°¡ 
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_BUILDITEMFAIL, 0, 0, 0, 0);
                    return;
                }

                // ¼øµµ °è»ê 
                dV2 = (double)m_pBuildItemList[i]->m_iMaxValue;
                if (iTotalValue <= 0)
                    dV3 = 1.0f;
                else dV3 = (double)iTotalValue;
                dV1 = (double)(dV3 / dV2) * 100.0f;

                // ¹éºÐ·ü·Î ¹Ù²ï °ª.
                iTotalValue = (int)dV1;

                // ¾ÆÀÌÅÛ »ý¼º 
                pItem = new class CItem;
                if (_bInitItemAttr(pItem, m_pBuildItemList[i]->m_cName) == false)
                {
                    delete pItem;
                    return;
                }

                // ¾ÆÀÌÅÛÀÌ Custom-MadeÀÓÀ» ³ªÅ¸³»´Â ÇÃ·¡±×¸¦ ÀÔ·Â 
                dwTemp = pItem->m_dwAttribute;
                dwTemp = dwTemp & 0xFFFFFFFE;
                dwTemp = dwTemp | 0x00000001;
                pItem->m_dwAttribute = dwTemp;

                if (pItem->m_cItemType == DEF_ITEMTYPE_MATERIAL)
                {
                    // ¸¸µé¾îÁø ¾ÆÀÌÅÛÀÌ ¹«±â Á¦ÀÛ Àç·á(À×°÷)ÀÌ¶ó¸é ±â¼ú ¼öÁØ¿¡ µû¸¥ ¼øµµ¸¦ ÀÔ·ÂÇÑ´Ù. 
                    iTemp = iDice(1, (iPlayerSkillLevel / 2) + 1) - 1;
                    pItem->m_sItemSpecEffectValue2 = (iPlayerSkillLevel / 2) + iTemp;
                    // v2.15 Á¦ÀÛ ¾ÆÀÌÅÛ¿¡ ¾ÆÀÌÅÛ °íÀ¯¹øÈ£ Ãß°¡ 
                    pItem->m_sTouchEffectType = DEF_ITET_ID;
                    pItem->m_sTouchEffectValue1 = iDice(1, 100000);
                    pItem->m_sTouchEffectValue2 = iDice(1, 100000);
                    pItem->m_sTouchEffectValue3 = timeGetTime();

                }
                else
                {
                    // ¹«±â Á¦ÀÛ Àç·á°¡ ¾Æ´Ï°í ¹«±â³ª ¹æ¾î±¸·ù¶ó¸é 
                    // ¾ÆÀÌÅÛ ¼Ó¼º°ª ÀÔ·Â 
                    dwTemp = pItem->m_dwAttribute;
                    dwTemp = dwTemp & 0x0000FFFF;

                    dwTemp2 = (uint16_t)m_pBuildItemList[i]->m_wAttribute;
                    dwTemp2 = dwTemp2 << 16;

                    dwTemp = dwTemp | dwTemp2;
                    pItem->m_dwAttribute = dwTemp;

                    iResultValue = (iTotalValue - m_pBuildItemList[i]->m_iAverageValue);
                    //Àç·á ¼øµµ¿¡ µû¸¥ ¼º´É ÀÔ·Â: SpecEffectValue1Àº ¼ö¸í, SpecEffectValue2´Â ¼º´É °¡ÁßÄ¡ 

                    // 1. ¼º´É °¡ÁßÄ¡ °è»ê(¹éºÐÀ²)
                    if (iResultValue > 0)
                    {
                        dV2 = (double)iResultValue;
                        dV3 = (double)(100 - m_pBuildItemList[i]->m_iAverageValue);
                        dV1 = (dV2 / dV3) * 100.0f;
                        pItem->m_sItemSpecEffectValue2 = (int)dV1;
                    }
                    else if (iResultValue < 0)
                    {
                        dV2 = (double)(iResultValue);
                        dV3 = (double)(m_pBuildItemList[i]->m_iAverageValue);
                        dV1 = (dV2 / dV3) * 100.0f;
                        pItem->m_sItemSpecEffectValue2 = (int)dV1;
                    }
                    else pItem->m_sItemSpecEffectValue2 = 0;

                    // 2. »õ·Î¿î ÃÖ´ë ¼ö¸í °è»ê 
                    dV2 = (double)pItem->m_sItemSpecEffectValue2;
                    dV3 = (double)pItem->m_wMaxLifeSpan;
                    dV1 = (dV2 / 100.0f) * dV3;

                    iTemp = (int)pItem->m_wMaxLifeSpan;
                    iTemp += (int)dV1;

                    // v2.15 Á¦ÀÛ ¾ÆÀÌÅÛ¿¡ ¾ÆÀÌÅÛ °íÀ¯¹øÈ£ Ãß°¡ 
                    pItem->m_sTouchEffectType = DEF_ITET_ID;
                    pItem->m_sTouchEffectValue1 = iDice(1, 100000);
                    pItem->m_sTouchEffectValue2 = iDice(1, 100000);
                    pItem->m_sTouchEffectValue3 = timeGetTime();

                    if (iTemp <= 0)
                        wTemp = 1;
                    else wTemp = (uint16_t)iTemp;

                    if (wTemp <= pItem->m_wMaxLifeSpan * 2)
                    {
                        // ¿¡·¯·Î ÀÎÇØ ³Ê¹« ¸¹Àº ¼ö¸í°ªÀÌ ³ª¿ÈÀ» ¸·±âÀ§ÇÔ 
                        pItem->m_wMaxLifeSpan = wTemp;
                        pItem->m_sItemSpecEffectValue1 = (short)wTemp;
                        pItem->m_wCurLifeSpan = pItem->m_wMaxLifeSpan;
                    }
                    else pItem->m_sItemSpecEffectValue1 = (short)pItem->m_wMaxLifeSpan;

                    //Custom-ItemÀº »ö»óÀÌ 2¹ø. 
                    pItem->m_cItemColor = 2;
                }

                //testcode
                log->info("Custom-Item({}) Value({}) Life({}/{})", pItem->m_cName, pItem->m_sItemSpecEffectValue2, pItem->m_wCurLifeSpan, pItem->m_wMaxLifeSpan);

                // ¾ÆÀÌÅÛ Àü´Þ 
                bAddItem(iClientH, pItem, 0);
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_BUILDITEMSUCCESS, pItem->m_sItemSpecEffectValue2, pItem->m_cItemType, 0, 0); // Integer¸¦ Àü´ÞÇÏ±â À§ÇØ 

                for (x = 0; x < 6; x++)
                    if (cElementItemID[x] != -1)
                    {
                        if (m_pClientList[iClientH]->m_pItemList[cElementItemID[x]] == 0)
                        {
                            // ### BUG POINT!!!
                            // ¹ö±×ÀÇ ¿øÀÎÀ» ¹àÈù´Ù.
                            log->info("(?) Char({}) ElementItemID({})", m_pClientList[iClientH]->m_cCharName, cElementItemID[x]);
                        }
                        else
                        {
                            iCount = m_pClientList[iClientH]->m_pItemList[cElementItemID[x]]->m_dwCount - m_pBuildItemList[i]->m_iMaterialItemCount[x];
                            if (iCount < 0) iCount = 0;
                            SetItemCount(iClientH, cElementItemID[x], iCount);
                        }
                    }

                // ÀÌÁ¦ ½ºÅ³À» Ä«¿îÆ® ÇÑ´Ù. ´Ü ¾ÆÀÌÅÛÀÇ ÃÖ´ë ¼ºÀå ½ºÅ³ ÇÑµµ ³»¿¡¼­¸¸ °¡´ÉÇÏ´Ù.  
                if (m_pBuildItemList[i]->m_iMaxSkill > m_pClientList[iClientH]->m_cSkillMastery[13])
                    CalculateSSN_SkillIndex(iClientH, 13, 1);

                // v1.41 ¼Ò·®ÀÇ °æÇèÄ¡ Áõ°¡ 
                GetExp(iClientH, iDice(1, (m_pBuildItemList[i]->m_iSkillLimit / 4))); //m_pClientList[iClientH]->m_iExpStock += iDice(1, (m_pBuildItemList[i]->m_iSkillLimit/4));

                return;
            }
        }

}

void CGame::RequestNoticementHandler(int iClientH, char * pData)
{
    char * cp, cData[120];
    int * ip, iRet, iClientSize;
    uint32_t * dwp;
    uint16_t * wp;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_dwNoticementDataSize < 10) return;

    ip = (int *)(pData + DEF_INDEX2_MSGTYPE + 2);
    iClientSize = *ip;

    if (iClientSize != m_dwNoticementDataSize)
    {
        // Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â°Â¡ Â°Â®Â°Ã­ Ã€Ã–Â´Ã‚ Ã†Ã„Ã€Ã Â»Ã§Ã€ÃŒÃÃ®Â¿Ã Â´Ã™Â¸Â£Â¸Ã© Â³Â»Â¿Ã«Ã€Â» Â¸Ã°ÂµÃŽ ÂºÂ¸Â³Â½Â´Ã™.
        cp = new char[m_dwNoticementDataSize + 2 + DEF_INDEX2_MSGTYPE + 2];
        memset(cp, 0, m_dwNoticementDataSize + 2 + DEF_INDEX2_MSGTYPE + 2);
        memcpy((cp + DEF_INDEX2_MSGTYPE + 2), m_pNoticementData, m_dwNoticementDataSize);

        dwp = (uint32_t *)(cp + DEF_INDEX4_MSGID);
        *dwp = MSGID_RESPONSE_NOTICEMENT;
        wp = (uint16_t *)(cp + DEF_INDEX2_MSGTYPE);
        *wp = DEF_MSGTYPE_REJECT;

        iRet = m_pClientList[iClientH]->send_msg(cp, m_dwNoticementDataSize + 2 + DEF_INDEX2_MSGTYPE + 2);

        delete cp;
    }
    else
    {
        memset(cData, 0, sizeof(cData));

        dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
        *dwp = MSGID_RESPONSE_NOTICEMENT;
        wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
        *wp = DEF_MSGTYPE_CONFIRM;

        iRet = m_pClientList[iClientH]->send_msg(cData, 6);
    }
    // Â¿Â¡Â·Â¯ Â¹ÃŸÂ»Ã½Ã‡Ã˜ÂµÂµ Â²Ã·ÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™.	
}

void CGame::RequestCheckAccountPasswordHandler(char * pData, uint32_t dwMsgSize)
{
    int * ip, i, iLevel;
    char * cp, cAccountName[11], cAccountPassword[11];

    cp = (char *)(pData + 6);

    memset(cAccountName, 0, sizeof(cAccountName));
    memset(cAccountPassword, 0, sizeof(cAccountPassword));

    memcpy(cAccountName, cp, 10);
    cp += 10;

    memcpy(cAccountPassword, cp, 10);
    cp += 10;

    ip = (int *)cp;
    iLevel = *ip;
    cp += 4;

    for (i = 0; i < DEF_MAXCLIENTS; i++)
        if ((m_pClientList[i] != 0) && (strcmp(m_pClientList[i]->m_cAccountName, cAccountName) == 0))
        {
            // Â°Â°Ã€Âº Â°Ã¨ÃÂ¤Ã€Â» ÃƒÂ£Â¾Ã’Â´Ã™. Â¸Â¸Â¾Ã  Ã†ÃÂ½ÂºÂ¿Ã¶ÂµÃ¥Â³Âª Â·Â¹ÂºÂ§Ã€ÃŒ Â´Ã™Â¸Â£Â¸Ã© ÂµÂ¥Ã€ÃŒÃ…Ã Ã€ÃºÃ€Ã¥Ã€Â» Ã‡ÃÃÃ¶ Â¾ÃŠÂ°Ã­ ÃÂ¢Â¼Ã“Ã€Â» Â²Ã·Â´Ã‚Â´Ã™. 
            if ((strcmp(m_pClientList[i]->m_cAccountPassword, cAccountPassword) != 0) || (m_pClientList[i]->m_iLevel != iLevel))
            {
                copy_string(G_cTxt, "(TestLog) Error! Account(%s)-Level(%d) password(or level) mismatch! Disconnect.", cAccountName, iLevel);
                log->info(G_cTxt);
                // ÂµÂ¥Ã€ÃŒÃ…Ã Ã€ÃºÃ€Ã¥Ã€Â» Ã‡ÃÃÃ¶ Â¾ÃŠÂ°Ã­ Â²Ã·Â´Ã‚Â´Ã™.
                DeleteClient(i, false, true);
                return;
            }
        }
}

void CGame::RequestRestartHandler(int iClientH)
{
    char  cTmpMap[32];

    if (m_pClientList[iClientH] == nullptr) return;

    if (m_pClientList[iClientH]->m_bIsKilled == true)
    {

        strcpy(cTmpMap, m_pClientList[iClientH]->m_cMapName);
        memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof(m_pClientList[iClientH]->m_cMapName));

        if (strcmp(m_pClientList[iClientH]->m_cLocation, "NONE") == 0)
        {
            // ¿©ÇàÀÚ¶ó¸é  default¸ÊÀ¸·Î °£´Ù.
            strcpy(m_pClientList[iClientH]->m_cMapName, "default");
        }
        else
        {
            // ¼Ò¼ÓÀÌ ÀÖ´Ù¸é ¼Ò¼Ó ¸¶À»ÀÇ ºÎÈ°Á¸À¸·Î °£´Ù.
            if ((strcmp(m_pClientList[iClientH]->m_cLocation, "aresden") == 0) || (strcmp(m_pClientList[iClientH]->m_cLocation, "arehunter") == 0))
            {
                if (m_bIsCrusadeMode == true)
                {
                    // Å©·ç¼¼ÀÌµå ¸ðµå¿¡¼­ »ç¸ÁÇÑ °æ¿ì: ¸¶À» ¸Ê¿¡¼­ ÀÏÁ¤½Ã°£ ³ª°¥ ¼ö ¾ø´Ù.
                    if (m_pClientList[iClientH]->m_iDeadPenaltyTime > 0)
                    {
                        memset(m_pClientList[iClientH]->m_cLockedMapName, 0, sizeof(m_pClientList[iClientH]->m_cLockedMapName));
                        strcpy(m_pClientList[iClientH]->m_cLockedMapName, "aresden");
                        m_pClientList[iClientH]->m_iLockedMapTime = 60 * 5;
                        m_pClientList[iClientH]->m_iDeadPenaltyTime = 60 * 10; // v2.04
                    }
                    else
                    {
                        memcpy(m_pClientList[iClientH]->m_cMapName, "resurr1", 7);
                        m_pClientList[iClientH]->m_iDeadPenaltyTime = 60 * 10; // v2.04 10ºÐ ¾È¿¡ ¶Ç Á×À¸¸é ¸¶À»¿¡ °®Èù´Ù.
                    }
                }
                // v2.16 2002-5-31
                if (strcmp(cTmpMap, "elvine") == 0)
                {
                    memcpy(m_pClientList[iClientH]->m_cMapName, "elvjail", 7);
                    strcpy(m_pClientList[iClientH]->m_cLockedMapName, "elvjail");
                    m_pClientList[iClientH]->m_iLockedMapTime = 60 * 3; // 3ºÐ 
                }
                else if (m_pClientList[iClientH]->m_iLevel > 80)
                    memcpy(m_pClientList[iClientH]->m_cMapName, "resurr1", 7);
                else memcpy(m_pClientList[iClientH]->m_cMapName, "arefarm", 7);
            }
            else
            {
                if (m_bIsCrusadeMode == true)
                {
                    // Å©·ç¼¼ÀÌµå ¸ðµå¿¡¼­ »ç¸ÁÇÑ °æ¿ì: ¸¶À» ¸Ê¿¡¼­ ÀÏÁ¤½Ã°£ ³ª°¥ ¼ö ¾ø´Ù.
                    if (m_pClientList[iClientH]->m_iDeadPenaltyTime > 0)
                    {
                        memset(m_pClientList[iClientH]->m_cLockedMapName, 0, sizeof(m_pClientList[iClientH]->m_cLockedMapName));
                        strcpy(m_pClientList[iClientH]->m_cLockedMapName, "elvine");
                        m_pClientList[iClientH]->m_iLockedMapTime = 60 * 5;
                        m_pClientList[iClientH]->m_iDeadPenaltyTime = 60 * 10; // v2.04
                    }
                    else
                    {
                        memcpy(m_pClientList[iClientH]->m_cMapName, "resurr2", 7);
                        m_pClientList[iClientH]->m_iDeadPenaltyTime = 60 * 10; // v2.04 10ºÐ ¾È¿¡ ¶Ç Á×À¸¸é ¸¶À»¿¡ °®Èù´Ù.
                    }
                }
                if (strcmp(cTmpMap, "aresden") == 0)
                {
                    memcpy(m_pClientList[iClientH]->m_cMapName, "arejail", 7);
                    strcpy(m_pClientList[iClientH]->m_cLockedMapName, "arejail");
                    m_pClientList[iClientH]->m_iLockedMapTime = 60 * 3; // 3ºÐ 

                }
                else if (m_pClientList[iClientH]->m_iLevel > 80)
                    memcpy(m_pClientList[iClientH]->m_cMapName, "resurr2", 7);
                else memcpy(m_pClientList[iClientH]->m_cMapName, "elvfarm", 7);
            }
        }

        // v2.04 ´Ù½Ã »ì¸®´Â Ã³¸®¸¦ ÇÑ´Ù. HP, ¹è°íÇÄ Áö¼ö ¸¸¶¥
        m_pClientList[iClientH]->m_bIsKilled = false;
        m_pClientList[iClientH]->m_iHP = (3 * m_pClientList[iClientH]->m_iVit) + (2 * m_pClientList[iClientH]->m_iLevel) + (m_pClientList[iClientH]->m_iStr / 2);
        m_pClientList[iClientH]->m_iHungerStatus = 100;

        memset(cTmpMap, 0, sizeof(cTmpMap));
        strcpy(cTmpMap, m_pClientList[iClientH]->m_cMapName);
        // !!! RequestTeleportHandler³»¿¡¼­ m_cMapNameÀ» ¾²±â ¶§¹®¿¡ ±×´ë·Î ÆÄ¶ó¹ÌÅÍ·Î ³Ñ°ÜÁÖ¸é ¿Àµ¿ÀÛ
        RequestTeleportHandler(iClientH, "2   ", cTmpMap, -1, -1);
    }
}

void CGame::CreateNewPartyHandler(int iClientH)
{
    bool bFlag;

    if (m_pClientList[iClientH] == nullptr) return;

    bFlag = m_pClientList[iClientH]->bCreateNewParty();
    SendNotifyMsg(0, iClientH, DEF_NOTIFY_RESPONSE_CREATENEWPARTY, (int)bFlag, 0, 0, 0);
}

void CGame::RequestSellItemListHandler(int iClientH, char * pData)
{
    int i, * ip, iAmount;
    char * cp, cIndex;
    struct
    {
        char cIndex;
        int  iAmount;
    } stTemp[12];

    if (m_pClientList[iClientH] == nullptr) return;

    cp = (char *)(pData + 6);
    for (i = 0; i < 12; i++)
    {
        stTemp[i].cIndex = *cp;
        cp++;

        ip = (int *)cp;
        stTemp[i].iAmount = *ip;
        cp += 4;
    }

    // Â³Â»Â¿Ã«Ã€Â» Â´Ã™ Ã€ÃÂ¾ÃºÂ´Ã™. Â¼Ã¸Â¼Â­Â´Ã«Â·ÃŽ Ã†ÃˆÂ¾Ã†Ã„Â¡Â¿Ã®Â´Ã™.
    for (i = 0; i < 12; i++)
    {
        cIndex = stTemp[i].cIndex;
        iAmount = stTemp[i].iAmount;

        if ((cIndex == -1) || (cIndex < 0) || (cIndex >= DEF_MAXITEMS)) return;
        if (m_pClientList[iClientH]->m_pItemList[cIndex] == 0) return;

        // cIndexÂ¿Â¡ Ã‡Ã˜Â´Ã§Ã‡ÃÂ´Ã‚ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Ã†Ã‡Â´Ã™.
        ReqSellItemConfirmHandler(iClientH, cIndex, iAmount, 0);
        // Ã€ÃŒ Â·Ã§Ã†Â¾Ã€Â» Â¼Ã¶Ã‡Ã Ã‡Ã‘ Â´Ã™Ã€Â½ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â°Â¡ Â»Ã¨ÃÂ¦ÂµÃ‡Â¾ÃºÃ€Â» Â¼Ã¶ Ã€Ã–Ã€Â¸Â´Ã ÃÃ–Ã€Ã‡!
        if (m_pClientList[iClientH] == nullptr) return;
    }
}

void CGame::RequestSetGuildConstructLocHandler(int iClientH, int dX, int dY, int iGuildGUID, char * pMapName)
{
    char * cp, cData[120];
    int i;
    int * ip, iIndex;
    uint32_t dwTemp, dwTime;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsOnServerChange == true) return;

    // Â°Ã”Ã€ÃŒÃ†Â® Â¼Â­Â¹Ã¶ Â¸ÃžÂ½ÃƒÃÃ¶ Ã€Ã›Â¼Âº 
    memset(cData, 0, sizeof(cData));
    cp = (char *)cData;
    *cp = GSM_SETGUILDCONSTRUCTLOC;
    cp++;

    ip = (int *)cp;
    *ip = iGuildGUID;
    cp += 4;

    ip = (int *)cp;
    *ip = dX;
    cp += 4;

    ip = (int *)cp;
    *ip = dY;
    cp += 4;

    memcpy(cp, pMapName, 10);
    cp += 10;
    //

    dwTime = timeGetTime();

    //testcode
    copy_string(G_cTxt, "SetGuildConstructLoc: %d %s %d %d", iGuildGUID, pMapName, dX, dY);
    log->info(G_cTxt);

    // Â¸Ã•Ã€Ãº Â°Â°Ã€Âº GUIDÂ¸Â¦ Â°Â¡ÃÃ¸ Â±Ã¦ÂµÃ¥ Ã„ÃÂ½ÂºÃ†Â®Â·Â°Ã†Â® ÃÃ‚Ã‡Â¥Â°Â¡ Â¼Â³ÃÂ¤ÂµÃ‡Â¾Ã® Ã€Ã–Â´Ã‚ÃÃ¶ ÃƒÂ£Â´Ã‚Â´Ã™. 
    // Ã…ÃšÂ·Â¹Ã†Ã·Ã†Â® Ã€Â§Ã„Â¡Ã€Ã‡ 2Â¹Ã¸ ÂºÂ¯Â¼Ã¶ÂµÃ©Ã€Â» Â»Ã§Â¿Ã«Ã‡Ã‘Â´Ã™.
    for (i = 0; i < DEF_MAXGUILDS; i++)
        if (m_pGuildTeleportLoc[i].m_iV1 == iGuildGUID)
        {
            // Ã€ÃŒÂ¹ÃŒ Â±Ã¦ÂµÃ¥Â°Â¡ Ã€Ã–Â´Ã™.
            if ((m_pGuildTeleportLoc[i].m_sDestX2 == dX) && (m_pGuildTeleportLoc[i].m_sDestY2 == dY) && (strcmp(m_pGuildTeleportLoc[i].m_cDestMapName2, pMapName) == 0))
            {
                // Â³Â»Â¿Ã«Ã€ÃŒ Ã€ÃÃ„Â¡Ã‡Ã‘Â´Ã™. Ã…Â¸Ã€Ã“ Ã€Ã”Â·Ã‚ÃˆÃ„ Â½ÂºÃ…Âµ.
                m_pGuildTeleportLoc[i].m_dwTime2 = dwTime;
                return;
            }
            else
            {
                // Â³Â»Â¿Ã« Â°Â»Â½Ã…ÃˆÃ„ 
                m_pGuildTeleportLoc[i].m_sDestX2 = dX;
                m_pGuildTeleportLoc[i].m_sDestY2 = dY;
                memset(m_pGuildTeleportLoc[i].m_cDestMapName2, 0, sizeof(m_pGuildTeleportLoc[i].m_cDestMapName2));
                strcpy(m_pGuildTeleportLoc[i].m_cDestMapName2, pMapName);
                m_pGuildTeleportLoc[i].m_dwTime2 = dwTime;

                //Â°Ã”Ã€ÃŒÃ†Â® Â¼Â­Â¹Ã¶Â¸Â¦ Ã…Ã«Ã‡Ã˜ Â´Ã™Â¸Â¥ Â¼Â­Â¹Ã¶Â·ÃŽ ÃÂ¤ÂºÂ¸ Ã€Ã¼Â¼Ã›
                bStockMsgToGateServer(cData, 23);
                return;
            }
        }

    // Â¼Â³ÃÂ¤ÂµÃˆ Â³Â»Â¿Ã«Ã€ÃŒ Â¾Ã¸Â´Ã™. Â»ÃµÂ·ÃŽ Â¼Â³ÃÂ¤Ã‡Ã‘Â´Ã™.
    dwTemp = 0;
    iIndex = -1;
    for (i = 0; i < DEF_MAXGUILDS; i++)
    {
        if (m_pGuildTeleportLoc[i].m_iV1 == 0)
        {

            m_pGuildTeleportLoc[i].m_iV1 = iGuildGUID;
            m_pGuildTeleportLoc[i].m_sDestX2 = dX;
            m_pGuildTeleportLoc[i].m_sDestY2 = dY;
            memset(m_pGuildTeleportLoc[i].m_cDestMapName2, 0, sizeof(m_pGuildTeleportLoc[i].m_cDestMapName2));
            strcpy(m_pGuildTeleportLoc[i].m_cDestMapName2, pMapName);
            m_pGuildTeleportLoc[i].m_dwTime2 = dwTime;

            //Â°Ã”Ã€ÃŒÃ†Â® Â¼Â­Â¹Ã¶Â¸Â¦ Ã…Ã«Ã‡Ã˜ Â´Ã™Â¸Â¥ Â¼Â­Â¹Ã¶Â·ÃŽ ÃÂ¤ÂºÂ¸ Ã€Ã¼Â¼Ã›  
            bStockMsgToGateServer(cData, 23);
            return;
        }
        else
        {
            // Â°Â¡Ã€Ã¥ Â¿Ã€Â·Â§ÂµÂ¿Â¾Ãˆ Ã‚Ã¼ÃÂ¶ÂµÃ‡ÃÃ¶ Â¾ÃŠÃ€Âº Â¸Â®Â½ÂºÃ†Â®Ã€Ã‡ Ã€ÃŽÂµÂ¦Â½ÂºÂ¸Â¦ ÃƒÂ£Â´Ã‚Â´Ã™.
            if (dwTemp < (dwTime - m_pGuildTeleportLoc[i].m_dwTime2))
            {
                dwTemp = (dwTime - m_pGuildTeleportLoc[i].m_dwTime2);
                iIndex = i;
            }
        }
    }

    // Ã„Â³Â½Ãƒ Â°Ã¸Â°Â£Ã€ÃŒ Â´Ã™ ÃƒÂ¡Â´Ã™. Ã€ÃŒÂ·Â± Â°Ã¦Â¿Ã¬ Â¸Â®Â½ÂºÃ†Â® ÃÃŸ Â°Â¡Ã€Ã¥ Â¿Ã€Â·Â§ÂµÂ¿Â¾Ãˆ Â»Ã§Â¿Ã«Ã‡ÃÃÃ¶ Â¾ÃŠÃ€Âº Â¸Â®Â½ÂºÃ†Â®(iIndex)Â¸Â¦ Â»Ã¨ÃÂ¦Ã‡ÃÂ°Ã­ Â¾Ã·ÂµÂ¥Ã€ÃŒÃ†Â® Ã‡Ã‘Â´Ã™.
    if (iIndex == -1) return;

    //testcode
    log->info("(X) No more GuildConstructLoc Space! Replaced.");

    m_pGuildTeleportLoc[i].m_iV1 = iGuildGUID;
    m_pGuildTeleportLoc[i].m_sDestX2 = dX;
    m_pGuildTeleportLoc[i].m_sDestY2 = dY;
    memset(m_pGuildTeleportLoc[i].m_cDestMapName2, 0, sizeof(m_pGuildTeleportLoc[i].m_cDestMapName2));
    strcpy(m_pGuildTeleportLoc[i].m_cDestMapName, pMapName);
    m_pGuildTeleportLoc[i].m_dwTime2 = dwTime;

    //Â°Ã”Ã€ÃŒÃ†Â® Â¼Â­Â¹Ã¶Â¸Â¦ Ã…Ã«Ã‡Ã˜ Â´Ã™Â¸Â¥ Â¼Â­Â¹Ã¶Â·ÃŽ ÃÂ¤ÂºÂ¸ Ã€Ã¼Â¼Ã›
    bStockMsgToGateServer(cData, 23);
}

void CGame::RequestSummonWarUnitHandler(int iClientH, int dX, int dY, char cType, char cNum, char cMode)
{
    char cName[6], cNpcName[21], cMapName[11], cNpcWayPoint[11], cOwnerType;
    int i, x;
    int iNamingValue, tX, tY, ix, iy;
    bool bRet;
    short sOwnerH;
    uint32_t dwTime = timeGetTime();

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    //hbest - crusade units summon mapcheck
    if (((strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "toh3") == 0) || (strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "icebound") == 0)) && (m_pClientList[iClientH]->m_iAdminUserLevel < 1))
    {
        return;
    }

    memset(cNpcWayPoint, 0, sizeof(cNpcWayPoint));
    memset(cNpcName, 0, sizeof(cNpcName));
    memset(cMapName, 0, sizeof(cMapName));

    // Â¿Â¡Â·Â¯ ÃƒÂ¼Ã…Â©Â¿Ã« 
    if (cType < 0) return;
    if (cType >= DEF_MAXNPCTYPES) return;
    if (cNum > 10) return;

    // Â°Ã‡Â¼Â³Ã‡ÃÂ±Ã¢Â¿Â¡ Ã†Ã·Ã€ÃŽÃ†Â®Â°Â¡ Â¸Ã°Ã€ÃšÂ¶ÃµÂ´Ã™.
    if (m_pClientList[iClientH]->m_iConstructionPoint < m_iNpcConstructionPoint[cType]) return;
    // Â°Ã‡Â¹Â° Â³Â»ÂºÃŽÂ¸Ã© Ã€Ã¼Ã€Ã¯ Ã€Â¯Â´ÃÃ†Â® Â¼Ã’ÃˆÂ¯ ÂºÃ’Â°Â¡.
    if ((m_pMapList[m_pClientList[iClientH]->m_cMapIndex] != 0) && (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFixedDayMode == true)) return;

    // Ã€ÃÂ´Ãœ Â¿Â¡Â·Â¯Â¹Ã¦ÃÃ¶Â¿Ã«
    cNum = 1;

    // ConstructionPoint ÃÂ¦Ã‡Ã‘Ã€Â» Â°Ã¨Â»ÃªÃ‡ÃÂ°Ã­ Ã€Â¯Â´ÃÃ†Â® Â°Â³Â¼Ã¶Â¸Â¸Ã…Â­ ÃÃ™Ã€ÃŽÂ´Ã™.
    for (x = 1; x <= cNum; x++)
    {
        // Â°Â³Â¼Ã¶Â¸Â¸Ã…Â­  Ã€Ã¼Ã€Ã¯ Ã€Â¯Â´ÃÃ†Â® Â»Ã½Â¼Âº 
        iNamingValue = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->iGetEmptyNamingValue();
        if (iNamingValue == -1)
        {
            // Â´ÃµÃ€ÃŒÂ»Ã³ Ã€ÃŒ Â¸ÃŠÂ¿Â¡ NPCÂ¸Â¦ Â¸Â¸ÂµÃ©Â¼Ã¶ Â¾Ã¸Â´Ã™. Ã€ÃŒÂ¸Â§Ã€Â» Ã‡Ã’Â´Ã§Ã‡Ã’ Â¼Ã¶ Â¾Ã¸Â±Ã¢ Â¶Â§Â¹Â®.
            // Ã€ÃŒÂ·Â± Ã€ÃÃ€ÃŒ?
        }
        else
        {
            // NPCÂ¸Â¦ Â»Ã½Â¼ÂºÃ‡Ã‘Â´Ã™.
            memset(cName, 0, sizeof(cName));
            copy_string(cName, "XX%d", iNamingValue);
            cName[0] = '_';
            cName[1] = m_pClientList[iClientH]->m_cMapIndex + 65;

            // ÂµÂ¿Ã€ÃÃ‡Ã‘ Ã…Â¸Ã€Ã”Ã€Â» Â°Â®ÃÃ¶Â¸Â¸ Â´Ã™Â¸Â¥ Â»Ã§Ã€ÃŒÂµÃ¥Â¸Â¦ Â°Â®Â´Ã‚ Â°Ã¦Â¿Ã¬ 

            switch (cType)
            {
                case 43: // Light War Beetle
                    switch (m_pClientList[iClientH]->m_cSide)
                    {
                        case 1: strcpy(cNpcName, "LWB-Aresden"); break;
                        case 2: strcpy(cNpcName, "LWB-Elvine"); break;
                    }
                    break;

                case 36: // Arrow Guard Tower
                    switch (m_pClientList[iClientH]->m_cSide)
                    {
                        case 1: strcpy(cNpcName, "AGT-Aresden"); break;
                        case 2: strcpy(cNpcName, "AGT-Elvine"); break;
                    }
                    break;

                case 37: // Cannon Guard Tower
                    switch (m_pClientList[iClientH]->m_cSide)
                    {
                        case 1: strcpy(cNpcName, "CGT-Aresden"); break;
                        case 2: strcpy(cNpcName, "CGT-Elvine"); break;
                    }
                    break;

                case 38: // Mana Collector
                    switch (m_pClientList[iClientH]->m_cSide)
                    {
                        case 1: strcpy(cNpcName, "MS-Aresden"); break;
                        case 2: strcpy(cNpcName, "MS-Elvine"); break;
                    }
                    break;

                case 39: // Detector
                    switch (m_pClientList[iClientH]->m_cSide)
                    {
                        case 1: strcpy(cNpcName, "DT-Aresden"); break;
                        case 2: strcpy(cNpcName, "DT-Elvine"); break;
                    }
                    break;

                case 51: // Catapult
                    switch (m_pClientList[iClientH]->m_cSide)
                    {
                        case 1: strcpy(cNpcName, "CP-Aresden"); break;
                        case 2: strcpy(cNpcName, "CP-Elvine"); break;
                    }
                    break;

                case 44:
                    strcpy(cNpcName, "GHK");
                    break;

                case 45:
                    strcpy(cNpcName, "GHKABS");
                    break;

                case 46:
                    strcpy(cNpcName, "TK");
                    break;

                case 47:
                    strcpy(cNpcName, "BG");
                    break;

                case 82:
                    switch (m_pClientList[iClientH]->m_cSide)
                    {
                        case 1: strcpy(cNpcName, "Sor-Aresden"); break;
                        case 2: strcpy(cNpcName, "Sor-Elvine"); break;
                    }
                    break;

                case 83:
                    switch (m_pClientList[iClientH]->m_cSide)
                    {
                        case 1: strcpy(cNpcName, "ATK-Aresden"); break;
                        case 2: strcpy(cNpcName, "ATK-Elvine"); break;
                    }
                    break;

                case 84:
                    switch (m_pClientList[iClientH]->m_cSide)
                    {
                        case 1: strcpy(cNpcName, "Elf-Aresden"); break;
                        case 2: strcpy(cNpcName, "Elf-Elvine"); break;
                    }
                    break;

                case 85:
                    switch (m_pClientList[iClientH]->m_cSide)
                    {
                        case 1: strcpy(cNpcName, "DSK-Aresden"); break;
                        case 2: strcpy(cNpcName, "DSK-Elvine"); break;
                    }
                    break;

                case 86:
                    switch (m_pClientList[iClientH]->m_cSide)
                    {
                        case 1: strcpy(cNpcName, "HBT-Aresden"); break;
                        case 2: strcpy(cNpcName, "HBT-Elvine"); break;
                    }
                    break;

                case 87:
                    switch (m_pClientList[iClientH]->m_cSide)
                    {
                        case 1: strcpy(cNpcName, "CT-Aresden"); break;
                        case 2: strcpy(cNpcName, "CT-Elvine"); break;
                    }
                    break;

                case 88:
                    switch (m_pClientList[iClientH]->m_cSide)
                    {
                        case 1: strcpy(cNpcName, "Bar-Aresden"); break;
                        case 2: strcpy(cNpcName, "Bar-Elvine"); break;
                    }
                    break;

                case 89:
                    switch (m_pClientList[iClientH]->m_cSide)
                    {
                        case 1: strcpy(cNpcName, "AGC-Aresden"); break;
                        case 2: strcpy(cNpcName, "AGC-Elvine"); break;
                    }
                    break;
            }

            //testcode
            copy_string(G_cTxt, "(!) Request Summon War Unit (%d) (%s)", cType, cNpcName);
            log->info(G_cTxt);

            tX = (int)dX;
            tY = (int)dY;

            // Â¸Â¸Â¾Ã  Ã€Ã¼Ã€Ã¯ Â°Ã‡ÃƒÃ Â¹Â°Ã€ÃŒÂ¶Ã³Â¸Ã© Â°Ã‡ÃƒÃ  Ã€Â§Ã„Â¡Â·ÃŽÂºÃŽÃ…Ã Ã€ÃŽÃÂ¢Ã‡Ã˜Â¾ÃŸ Ã‡Ã‘Â´Ã™.
            bRet = false;
            switch (cType)
            {
                case 36:
                case 37:
                case 38:
                case 39:
                    // Â°Ã‡Â¼Â³ Ã€Â§Ã„Â¡Â¸Â¦ ÂºÃ±Â±Â³
                    if (strcmp(m_pClientList[iClientH]->m_cConstructMapName, m_pClientList[iClientH]->m_cMapName) != 0) bRet = true;
                    if (abs(m_pClientList[iClientH]->m_sX - m_pClientList[iClientH]->m_iConstructLocX) > 10) bRet = true;
                    if (abs(m_pClientList[iClientH]->m_sY - m_pClientList[iClientH]->m_iConstructLocY) > 10) bRet = true;

                    if (bRet == true)
                    {
                        // Â°Ã‡Â¼Â³ Ã€Â§Ã„Â¡Â¿Ã Â³ÃŠÂ¹Â« Â¸Ã–Â¸Â® Â¹Ã¾Â¾Ã®Â³Âª Ã€Ã–Ã€Â¸Â¸Ã© Â°Ã‡ÃƒÃ  ÂºÃ’Â°Â¡.
                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetNamingValueEmpty(iNamingValue);
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_CANNOTCONSTRUCT, 2, 0, 0, 0);
                        return;
                    }

                    // Â±Ã¦ÂµÃ¥Â¿Â¡Â¼Â­ Â°Ã‡Â¼Â³Ã‡Ã‘ Â°Ã‡ÃƒÃ Â¹Â°Ã€Ã‡ Â°Â¹Â¼Ã¶Â¸Â¦ Ã„Â«Â¿Ã®Ã†Â®Ã‡Ã‘Â´Ã™. Ã€ÃÃÂ¤ Â°Â¹Â¼Ã¶ Ã€ÃŒÂ»Ã³Ã€Âº ÃÃ¶Ã€Â» Â¼Ã¶ Â¾Ã¸Â´Ã™.
                    /////
                    for (i = 0; i < DEF_MAXGUILDS; i++)
                        if (m_pGuildTeleportLoc[i].m_iV1 == m_pClientList[iClientH]->m_iGuildGUID)
                        {
                            m_pGuildTeleportLoc[i].m_dwTime = dwTime;
                            if (m_pGuildTeleportLoc[i].m_iV2 >= DEF_MAXCONSTRUCTNUM)
                            {
                                // Â±Ã¦ÂµÃ¥ Â°Ã‡Â¼Â³ Â°Â³Â¼Ã¶ ÃƒÃŠÂ°Ãº. ÃÃ¶Ã€Â»Â¼Ã¶ Â¾Ã¸Â´Ã™.
                                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetNamingValueEmpty(iNamingValue);
                                SendNotifyMsg(0, iClientH, DEF_NOTIFY_CANNOTCONSTRUCT, 3, 0, 0, 0);
                                return;
                            }
                            else
                            {
                                // Â°Ã‡Â¼Â³ÂµÃˆ Â°Ã‡Â¹Â° Â°Â¹Â¼Ã¶ ÃÃµÂ°Â¡.
                                m_pGuildTeleportLoc[i].m_iV2++;
                                goto RSWU_LOOPBREAK;
                            }
                        }

                    // Â±Ã¦ÂµÃ¥ Â¼Â³ÃÂ¤ÂµÃ‡Â¾Ã® Ã€Ã–ÃÃ¶ Â¾ÃŠÃ€Â½. ÃÃ¶Ã€Â»Â¼Ã¶ Â¾Ã¸Â´Ã™.
                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetNamingValueEmpty(iNamingValue);
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_CANNOTCONSTRUCT, 3, 0, 0, 0);
                    return;
                    /////
                    break;
                case 43:
                case 44:
                case 45:
                case 46:
                case 47:
                case 51:
                    break;

                case 40:
                case 41:
                case 42:
                case 48:
                case 49:
                case 50:
                    break;
            }

            RSWU_LOOPBREAK:;

            // Â¸Â¸Â¾Ã  Ã€Ã¼Ã€Ã¯ Â°Ã‡ÃƒÃ Â¹Â°ÃÃŸ Â°Â¡ÂµÃ¥Ã…Â¸Â¿Ã¶Â°Â°Ã€ÃŒ Â°Ã¸Â°ÃÃ‡Ã¼Ã€ÃŒÂ¶Ã³Â¸Ã© Â³ÃŠÂ¹Â« Ã€ÃŽÃÂ¢Ã‡Ã˜Â¼Â­ Â°Ã‡Â¼Â³Ã‡Ã’ Â¼Ã¶ Â¾Ã¸Â´Ã™.
            bRet = false;
            switch (cType)
            {
                case 36:
                case 37:
                    for (ix = tX - 2; ix <= tX + 2; ix++)
                        for (iy = tY - 2; iy <= tY + 2; iy++)
                        {
                            m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                            if ((sOwnerH != 0) && (cOwnerType == DEF_OWNERTYPE_NPC))
                            {
                                switch (m_pNpcList[sOwnerH]->m_sType)
                                {
                                    case 36:
                                    case 37:
                                        bRet = true;
                                        break;
                                }
                            }
                        }

                    // ÃÃ¶Ã€Â» Â¼Ã¶ Â¾Ã¸Â´Ã‚ Â»Ã³Ã€Â§ÃÃ‚Ã‡Â¥
                    if ((dY <= 32) || (dY >= 783)) bRet = true;
                    break;
            }

            if (bRet == true)
            {
                // Â±Ã™ÃƒÂ³Â¿Â¡ Â°Â¡ÂµÃ¥Ã…Â¸Â¿Ã¶ÂµÃ©Ã€ÃŒ Ã€Ã–Â¾Ã®Â¼Â­ Â°Ã‡ÃƒÃ ÂºÃ’Â°Â¡.
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetNamingValueEmpty(iNamingValue);
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_CANNOTCONSTRUCT, 1, 0, 0, 0);
                return;
            }

            // Â°Â¡ÂµÃ¥ Â¸Ã°ÂµÃ¥Ã€ÃŽÂ°Â¡ ÃƒÃŸÃÂ¾ Â¸Ã°ÂµÃ¥Ã€ÃŽÂ°Â¡ Â¼Â±Ã…Ãƒ 
            if (cMode == 0)
            {
                bRet = bCreateNewNpc(cNpcName, cName, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, 0, 0, DEF_MOVETYPE_FOLLOW, &tX, &tY, cNpcWayPoint, 0, 0, -1, false, false, false, false, m_pClientList[iClientH]->m_iGuildGUID);
                bSetNpcFollowMode(cName, m_pClientList[iClientH]->m_cCharName, DEF_OWNERTYPE_PLAYER);
            }
            else bRet = bCreateNewNpc(cNpcName, cName, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, 0, 0, DEF_MOVETYPE_GUARD, &tX, &tY, cNpcWayPoint, 0, 0, -1, false, false, false, false, m_pClientList[iClientH]->m_iGuildGUID);

            if (bRet == false)
            {
                // Â½Ã‡Ã†ÃÃ‡ÃŸÃ€Â¸Â¹Ã‡Â·ÃŽ Â¿Â¹Â¾Ã ÂµÃˆ NameValueÂ¸Â¦ Ã‡Ã˜ÃÂ¦Â½ÃƒÃ…Â²Â´Ã™.
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetNamingValueEmpty(iNamingValue);
            }
            else
            {
                // Â°Ã‡Â¼Â³Â¿Â¡ Â¼ÂºÂ°Ã¸Ã‡ÃŸÃ€Â¸Â¹Ã‡Â·ÃŽ Â°Ã‡Â¼Â³ Ã†Ã·Ã€ÃŽÃ†Â® Â°Â¨Â¼Ã’ Â½ÃƒÃ…Â²Â´Ã™.
                m_pClientList[iClientH]->m_iConstructionPoint -= m_iNpcConstructionPoint[cType];
                if (m_pClientList[iClientH]->m_iConstructionPoint < 0) m_pClientList[iClientH]->m_iConstructionPoint = 0;
                // ÃÃ¶ÃˆÃ–Â°Ã¼Â¿Â¡Â°Ã” Â¹Ã™Â·ÃŽ Ã…Ã«ÂºÂ¸.
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[iClientH]->m_iConstructionPoint, m_pClientList[iClientH]->m_iWarContribution, 0, 0);
            }
        }
    }
}

void CGame::CheckConnectionHandler(int iClientH, char * pData)
{
    char * cp;
    uint32_t * dwp, dwTimeRcv, dwTime, dwTimeGapClient, dwTimeGapServer;

    if (m_pClientList[iClientH] == nullptr) return;
    //m_pClientList[iClientH]->m_cConnectionCheck = 0;

    dwTime = timeGetTime();
    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);
    dwp = (uint32_t *)cp;
    dwTimeRcv = *dwp;

    if (m_pClientList[iClientH]->m_dwInitCCTimeRcv == 0)
    {
        m_pClientList[iClientH]->m_dwInitCCTimeRcv = dwTimeRcv;
        m_pClientList[iClientH]->m_dwInitCCTime = dwTime;
    }
    else
    {
        dwTimeGapClient = (dwTimeRcv - m_pClientList[iClientH]->m_dwInitCCTimeRcv);
        dwTimeGapServer = (dwTime - m_pClientList[iClientH]->m_dwInitCCTime);

        if (dwTimeGapClient < dwTimeGapServer) return;
        if ((abs(dwTimeGapClient - dwTimeGapServer)) >= (DEF_CLIENTTIMEOUT))
        {
            DeleteClient(iClientH, true, true);
            return;
        }
    }
}

void CGame::SelectCrusadeDutyHandler(int iClientH, int iDuty)
{

    if (m_pClientList[iClientH] == nullptr) return;
    if ((m_pClientList[iClientH]->m_iGuildRank != 0) && (iDuty == 3)) return;

    if (m_iLastCrusadeWinner == m_pClientList[iClientH]->m_cSide &&
        m_pClientList[iClientH]->m_dwCrusadeGUID == 0 && iDuty == 3)
    {
        m_pClientList[iClientH]->m_iConstructionPoint = 3000;
    }
    m_pClientList[iClientH]->m_iCrusadeDuty = iDuty;

    SendNotifyMsg(0, iClientH, DEF_NOTIFY_CRUSADE, (uint32_t)m_bIsCrusadeMode, m_pClientList[iClientH]->m_iCrusadeDuty, 0, 0);
    if (iDuty == 1)
        _bCrusadeLog(DEF_CRUSADELOG_SELECTDUTY, iClientH, 0, "Fighter");
    else if (iDuty == 2)
        _bCrusadeLog(DEF_CRUSADELOG_SELECTDUTY, iClientH, 0, "Constructor");
    else
        _bCrusadeLog(DEF_CRUSADELOG_SELECTDUTY, iClientH, 0, "Commander");
}

void CGame::MapStatusHandler(int iClientH, int iMode, char * pMapName)
{
    int i;

    if (m_pClientList[iClientH] == nullptr) return;

    switch (iMode)
    {
        case 1:
            if (m_pClientList[iClientH]->m_iCrusadeDuty == 0) return;

            for (i = 0; i < DEF_MAXGUILDS; i++)
                if ((m_pGuildTeleportLoc[i].m_iV1 != 0) && (m_pGuildTeleportLoc[i].m_iV1 == m_pClientList[iClientH]->m_iGuildGUID))
                {
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_TCLOC, m_pGuildTeleportLoc[i].m_sDestX, m_pGuildTeleportLoc[i].m_sDestY,
                        0, m_pGuildTeleportLoc[i].m_cDestMapName, m_pGuildTeleportLoc[i].m_sDestX2, m_pGuildTeleportLoc[i].m_sDestY2,
                        0, 0, 0, 0, m_pGuildTeleportLoc[i].m_cDestMapName2);
                    // °Ç¼³ À§Ä¡¸¦ ÀúÀåÇØ ³õ´Â´Ù.
                    memset(m_pClientList[iClientH]->m_cConstructMapName, 0, sizeof(m_pClientList[iClientH]->m_cConstructMapName));
                    memcpy(m_pClientList[iClientH]->m_cConstructMapName, m_pGuildTeleportLoc[i].m_cDestMapName2, 10);
                    m_pClientList[iClientH]->m_iConstructLocX = m_pGuildTeleportLoc[i].m_sDestX2;
                    m_pClientList[iClientH]->m_iConstructLocY = m_pGuildTeleportLoc[i].m_sDestY2;
                    return;
                }

            // ±æµå ¼Ò¼ÓÀÌ ¾Æ´Ñ ±ºÀÎÀÇ ¿äÃ»ÀÌ´Ù.
            break;

        case 3:
            //if (m_pClientList[iClientH]->m_iCrusadeDuty != 3) return;
            // µ¥ÀÌÅÍ Ä³½Ã ±¸Á¶Ã¼ ÃÊ±âÈ­ 
            for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++)
            {
                m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cType = 0;
                m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cSide = 0;
                m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sX = 0;
                m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sY = 0;
            }
            m_pClientList[iClientH]->m_iCSIsendPoint = 0;
            memset(m_pClientList[iClientH]->m_cSendingMapName, 0, sizeof(m_pClientList[iClientH]->m_cSendingMapName));

            if (strcmp(pMapName, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName) == 0)
            {
                // ÇöÀç »ç¿ëÀÚ°¡ À§Ä¡ÇÑ ¸ÊÀÇ Á¤º¸¸¦ ¿ä±¸Çß´Ù.
                // ¸Ê µ¥ÀÌÅÍ¸¦ ¿äÃ»ÇÒ ´ç½ÃÀÇ ³»¿ëÀ» ±×´ë·Î Ä«ÇÇÇÑ´Ù.
                for (i = 0; i < m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_iTotalCrusadeStructures; i++)
                {
                    // v2.15 »ç¿ëÀÚ°¡ Commander °¡ ¾Æ´Ï¾îµµ °Ç¹°ÀÇ À§Ä¡¸¦ º¸¿©ÁØ´Ù.
                    if (m_pClientList[iClientH]->m_iCrusadeDuty == 3)
                    {
                        m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cType = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stCrusadeStructureInfo[i].cType;
                        m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cSide = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stCrusadeStructureInfo[i].cSide;
                        m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sX = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stCrusadeStructureInfo[i].sX;
                        m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sY = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stCrusadeStructureInfo[i].sY;
                    }
                    else if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stCrusadeStructureInfo[i].cType == 42)
                    {
                        m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cType = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stCrusadeStructureInfo[i].cType;
                        m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cSide = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stCrusadeStructureInfo[i].cSide;
                        m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sX = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stCrusadeStructureInfo[i].sX;
                        m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sY = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stCrusadeStructureInfo[i].sY;
                    }
                }
                memcpy(m_pClientList[iClientH]->m_cSendingMapName, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, 10);
            }
            else
            {
                // ´Ù¸¥ ¸ÊÀÇ »óÈ²À» ¿äÃ»Çß´Ù. ¹Ìµé·£µåÀÇ °æ¿ì´Â °¡´É 
                if (strcmp(pMapName, "middleland") == 0)
                {
                    for (i = 0; i < m_iTotalMiddleCrusadeStructures; i++)
                    {
                        // v2.15 »ç¿ëÀÚ°¡ Commander °¡ ¾Æ´Ï¾îµµ °Ç¹°ÀÇ À§Ä¡¸¦ º¸¿©ÁØ´Ù.
                        if (m_pClientList[iClientH]->m_iCrusadeDuty == 3)
                        {
                            m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cType = m_stMiddleCrusadeStructureInfo[i].cType;
                            m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cSide = m_stMiddleCrusadeStructureInfo[i].cSide;
                            m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sX = m_stMiddleCrusadeStructureInfo[i].sX;
                            m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sY = m_stMiddleCrusadeStructureInfo[i].sY;
                        }
                        else if (m_stMiddleCrusadeStructureInfo[i].cType == 42)
                        {
                            m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cType = m_stMiddleCrusadeStructureInfo[i].cType;
                            m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cSide = m_stMiddleCrusadeStructureInfo[i].cSide;
                            m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sX = m_stMiddleCrusadeStructureInfo[i].sX;
                            m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sY = m_stMiddleCrusadeStructureInfo[i].sY;
                        }
                    }
                    strcpy(m_pClientList[iClientH]->m_cSendingMapName, "middleland");
                }
                else
                {
                    // ¾ÆÁ÷ Áö¿øµÇÁö ¾Ê´Â ±â´ÉÀÌ´Ù.
                }
            }

            _SendMapStatus(iClientH);
            break;
    }
}

void CGame::RequestHelpHandler(int iClientH)
{
    // Ã‡Ã¯Ã‡Ã Â¿Ã¤ÃƒÂ»Ã€ÃŒÂ´Ã™. Â±Ã¦ÂµÃ¥ Â¸Â¶Â½ÂºÃ…ÃÂ¿Â¡Â°Ã” ÃÃ‚Ã‡Â¥Â¸Â¦ Ã€Ã¼Â´ÃžÃ‡Ã˜ ÃÃ˜Â´Ã™.
    int i;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_iGuildRank == -1) return;
    if (m_pClientList[iClientH]->m_iCrusadeDuty != 1) return;

    for (i = 1; i < DEF_MAXCLIENTS; i++)
        if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_iGuildRank == 0) &&
            (m_pClientList[i]->m_iCrusadeDuty == 3) && (m_pClientList[i]->m_iGuildGUID == m_pClientList[iClientH]->m_iGuildGUID))
        {
            // Â°Â°Ã€Âº Â±Ã¦ÂµÃ¥ Â¸Â¶Â½ÂºÃ…ÃÃ€ÃŒÂ°Ã­ ÃÃ¶ÃˆÃ–Â°Ã¼ Â¿ÂªÃˆÂ°Ã€Â» Â¸ÃƒÃ€Âº Â»Ã³Ã…Ã‚Ã€ÃŒÂ´Ã™.
            SendNotifyMsg(0, i, DEF_NOTIFY_HELP, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, m_pClientList[iClientH]->m_iHP, m_pClientList[iClientH]->m_cCharName);
            return;
        }

    // Ã‡Ã¶Ã€Ã§ Â¸ÃŠÂ¿Â¡ ÃÃ¶ÃˆÃ–Â°Ã¼Ã€ÃŒ Â¾Ã¸Ã€Â½Ã€Â» Ã…Ã«ÂºÂ¸Ã‡Ã˜ ÃÃ˜Â´Ã™.
    SendNotifyMsg(0, iClientH, DEF_NOTIFY_HELPFAILED, 0, 0, 0, 0);
}

void CGame::DoMeteorStrikeDamageHandler(int iMapIndex)
{
    int i, iDamage;

    // Ã‡Ã˜Â´Ã§ Â¸ÃŠÃ€Ã‡ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®ÂµÃ©Ã€Ã‡ HPÂ¸Â¦ Ã€ÃÂ°Ã½ Â»Ã¨Â°Â¨
    for (i = 1; i < DEF_MAXCLIENTS; i++)
        if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_cSide != 0) && (m_pClientList[i]->m_cMapIndex == iMapIndex))
        {
            if (m_pClientList[i]->m_iLevel < 80)
                iDamage = m_pClientList[i]->m_iLevel + iDice(1, 10);
            else iDamage = m_pClientList[i]->m_iLevel * 2 + iDice(1, 10);
            iDamage = iDice(1, m_pClientList[i]->m_iLevel) + m_pClientList[i]->m_iLevel;
            // ÃƒÃ–Â´Ã« 255Ã€ÃŒÂ»Ã³Ã€Ã‡ Â´Ã«Â¹ÃŒÃÃ¶Â´Ã‚ Â¹ÃžÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™.
            if (iDamage > 255) iDamage = 255;

            if (m_pClientList[i]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 2)
            { //magic cut in half
                iDamage = (iDamage / 2) - 2;
            }

            if (m_pClientList[i]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 5)
            {
                iDamage = 0;
            }

            if (m_pClientList[i]->m_iAdminUserLevel > 0)
            { // Admins get no damage
                iDamage = 0;
            }

            m_pClientList[i]->m_iHP -= iDamage;
            if (m_pClientList[i]->m_iHP <= 0)
            {
                // Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â°Â¡ Â»Ã§Â¸ÃÃ‡ÃŸÂ´Ã™.
                ClientKilledHandler(i, 0, 0, iDamage);
                m_stMeteorStrikeResult.iCasualties++;
            }
            else
            {
                if (iDamage > 0)
                {
                    // Â¹ÃžÃ€Âº Â´Ã«Â¹ÃŒÃÃ¶Â¸Â¦ Ã…Ã«ÂºÂ¸Ã‡Ã‘Â´Ã™. <- HPÂ¸Â¦ Â±Ã—Â´Ã«Â·ÃŽ Â¾Ã‹Â¸Â°Â´Ã™.
                    SendNotifyMsg(0, i, DEF_NOTIFY_HP, 0, 0, 0, 0);
                    // ÃƒÃ¦Â°ÃÃ€Â» Â¹ÃžÂ¾Ã’Â´Ã™Â¸Ã© ÃƒÃ¦Â°ÃÂµÂ¿Ã€Ã› Ã€Ã¼Â¼Ã› 
                    SendEventToNearClient_TypeA(i, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);

                    // v1.4 ÃÃ—Ã€ÂºÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Â°Ã¦Â¿Ã¬Â´Ã‚ Owner Ã€Â§Ã„Â¡Â¸Â¦ Â¿Ã…Â±Ã¤Â´Ã™. 
                    if (m_pClientList[i]->m_bSkillUsingStatus[19] != true)
                    {
                        m_pMapList[m_pClientList[i]->m_cMapIndex]->ClearOwner(0, i, DEF_OWNERTYPE_PLAYER, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY);
                        m_pMapList[m_pClientList[i]->m_cMapIndex]->SetOwner(i, DEF_OWNERTYPE_PLAYER, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY);
                    }

                    if (m_pClientList[i]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0)
                    {
                        // Hold-Person ÂµÃ‡Â¾Ã® Ã€Ã–Â¾ÃºÂ´Ã¸ Â»Ã³Ã…Ã‚Â¶Ã³Â¸Ã© Ã‡Â®Â¸Â°Â´Ã™. Â¸Â¶Â¹Ã½Â°Ã¸Â°Ã Ã†ÃÂ·Â²Â¶Ã³Ã€ÃŒÃÃ® ÂµÃˆ Â°ÃÂµÂµ Ã‡Â®Â¸Â°Â´Ã™.
                        // 1: Hold-Person 
                        // 2: Paralize
                        SendNotifyMsg(0, i, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_HOLDOBJECT, m_pClientList[i]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT], 0, 0);

                        m_pClientList[i]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
                        bRemoveFromDelayEventList(i, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_HOLDOBJECT);
                    }
                }
            }
        }
}

void CGame::RequestGuildNameHandler(int iClientH, int iObjectID, int iIndex)
{
    if (m_pClientList[iClientH] == nullptr) return;
    if ((iObjectID <= 0) || (iObjectID >= DEF_MAXCLIENTS)) return;

    if (m_pClientList[iObjectID] == 0)
    {
        // Â¿Ã¤ÃƒÂ» Â¹ÃžÃ€Âº ObjectÂ°Â¡ Â¾Ã¸Â´Ã™.

    }
    else
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_REQGUILDNAMEANSWER, m_pClientList[iObjectID]->m_iGuildRank, iIndex, 0, m_pClientList[iObjectID]->m_cGuildName);
    }
}

void CGame::ReqCreateSlateHandler(int iClientH, char * pData)
{
    int i, iRet;
    short * sp;
    char cItemID[4], ctr[4];
    char * cp, cSlateColour, cData[120];
    bool bIsSlatePresent = false;
    CItem * pItem;
    int iSlateType, iEraseReq;
    uint32_t * dwp;
    uint16_t * wp;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsOnServerChange == true) return;

    for (i = 0; i < 4; i++)
    {
        cItemID[i] = 0;
        ctr[i] = 0;
    }

    cp = (char *)pData;
    cp += 11;

    // 14% chance of creating slates
    if (iDice(1, 100) < m_sSlateSuccessRate) bIsSlatePresent = true;

    try
    {
        // make sure slates really exist
        for (i = 0; i < 4; i++)
        {
            cItemID[i] = *cp;
            cp++;

            if (m_pClientList[iClientH]->m_pItemList[cItemID[i]] == 0 || cItemID[i] > DEF_MAXITEMS)
            {
                bIsSlatePresent = false;
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_SLATE_CREATEFAIL, 0, 0, 0, 0);
                return;
            }

            //No duping
            if (m_pClientList[iClientH]->m_pItemList[cItemID[i]]->m_sIDnum == 868) // LU
                ctr[0] = 1;
            else if (m_pClientList[iClientH]->m_pItemList[cItemID[i]]->m_sIDnum == 869) // LD
                ctr[1] = 1;
            else if (m_pClientList[iClientH]->m_pItemList[cItemID[i]]->m_sIDnum == 870) // RU
                ctr[2] = 1;
            else if (m_pClientList[iClientH]->m_pItemList[cItemID[i]]->m_sIDnum == 871) // RD
                ctr[3] = 1;
        }
    }
    catch (...)
    {
        //Crash Hacker Caught
        bIsSlatePresent = false;
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SLATE_CREATEFAIL, 0, 0, 0, 0);
        copy_string(G_cTxt, "TSearch Slate Hack: (%s) Player: (%s) - creating slates without correct item!", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
        PutHackLogFileList(G_cTxt);
        DeleteClient(iClientH, true, true);
        return;
    }

    // Are all 4 slates present ??
    if (ctr[0] != 1 || ctr[1] != 1 || ctr[2] != 1 || ctr[3] != 1)
    {
        bIsSlatePresent = false;
        return;
    }

    if (m_pClientList[iClientH]->m_iAdminUserLevel > 3) bIsSlatePresent = true;

    // if we failed, kill everything
    if (!bIsSlatePresent)
    {
        for (i = 0; i < 4; i++)
        {
            if (m_pClientList[iClientH]->m_pItemList[cItemID[i]] != 0)
            {
                ItemDepleteHandler(iClientH, cItemID[i], false);
            }
        }
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SLATE_CREATEFAIL, 0, 0, 0, 0);
        return;
    }

    // make the slates
    for (i = 0; i < 4; i++)
    {
        if (m_pClientList[iClientH]->m_pItemList[cItemID[i]] != 0)
        {
            ItemDepleteHandler(iClientH, cItemID[i], false);
        }
    }

    pItem = new class CItem;

    i = iDice(1, 1000);

    if (i < 50)
    { // Hp slate
        iSlateType = 1;
        cSlateColour = 32;
    }
    else if (i < 250)
    { // Bezerk slate
        iSlateType = 2;
        cSlateColour = 3;
    }
    else if (i < 750)
    { // Exp slate
        iSlateType = 4;
        cSlateColour = 7;
    }
    else if (i < 950)
    { // Mana slate
        iSlateType = 3;
        cSlateColour = 37;
    }
    else if (i < 1001)
    { // Hp slate
        iSlateType = 1;
        cSlateColour = 32;
    }

    // Notify client
    SendNotifyMsg(0, iClientH, DEF_NOTIFY_SLATE_CREATESUCCESS, iSlateType, 0, 0, 0);

    ZeroMemory(cData, sizeof(cData));

    // Create slates
    if (_bInitItemAttr(pItem, 867) == false)
    {
        delete pItem;
        return;
    }
    else
    {
        pItem->m_sTouchEffectType = DEF_ITET_ID;
        pItem->m_sTouchEffectValue1 = iDice(1, 100000);
        pItem->m_sTouchEffectValue2 = iDice(1, 100000);
        pItem->m_sTouchEffectValue3 = (short)timeGetTime();

        _bItemLog(DEF_ITEMLOG_GET, iClientH, -1, pItem);

        pItem->m_sItemSpecEffectValue2 = iSlateType;
        pItem->m_cItemColor = cSlateColour;
        if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true)
        {
            ZeroMemory(cData, sizeof(cData));
            dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
            *dwp = MSGID_NOTIFY;
            wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
            *wp = DEF_NOTIFY_ITEMOBTAINED;

            cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);
            *cp = 1;
            cp++;

            memcpy(cp, pItem->m_cName, 20);
            cp += 20;

            dwp = (uint32_t *)cp;
            *dwp = pItem->m_dwCount;
            cp += 4;

            *cp = pItem->m_cItemType;
            cp++;

            *cp = pItem->m_cEquipPos;
            cp++;

            *cp = (char)0; // ¾òÀº ¾ÆÀÌÅÛÀÌ¹Ç·Î ÀåÂøµÇÁö ¾Ê¾Ò´Ù.
            cp++;

            sp = (short *)cp;
            *sp = pItem->m_sLevelLimit;
            cp += 2;

            *cp = pItem->m_cGenderLimit;
            cp++;

            wp = (uint16_t *)cp;
            *wp = pItem->m_wCurLifeSpan;
            cp += 2;

            wp = (uint16_t *)cp;
            *wp = pItem->m_wWeight;
            cp += 2;

            sp = (short *)cp;
            *sp = pItem->m_sSprite;
            cp += 2;

            sp = (short *)cp;
            *sp = pItem->m_sSpriteFrame;
            cp += 2;

            *cp = pItem->m_cItemColor;
            cp++;

            *cp = (char)pItem->m_sItemSpecEffectValue2;
            cp++;

            dwp = (uint32_t *)cp;
            *dwp = pItem->m_dwAttribute;
            cp += 4;

            if (iEraseReq == 1) delete pItem;

            // ¾ÆÀÌÅÛ Á¤º¸ Àü¼Û 
            iRet = m_pClientList[iClientH]->send_msg(cData, 53);
            switch (iRet)
            {
                case DEF_XSOCKEVENT_QUENEFULL:
                case DEF_XSOCKEVENT_SOCKETERROR:
                case DEF_XSOCKEVENT_CRITICALERROR:
                case DEF_XSOCKEVENT_SOCKETCLOSED:
                    // ¸Þ½ÃÁö¸¦ º¸³¾¶§ ¿¡·¯°¡ ¹ß»ýÇß´Ù¸é Á¦°ÅÇÑ´Ù.
                    DeleteClient(iClientH, true, true);
                    return;
            }
        }
        else
        {
            m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, pItem);
            SendEventToNearClient_TypeB(MSGID_MAGICCONFIGURATIONCONTENTS, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
                m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, pItem->m_sSprite, pItem->m_sSpriteFrame,
                pItem->m_cItemColor);
            dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
            *dwp = MSGID_NOTIFY;
            wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
            *wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;

            iRet = m_pClientList[iClientH]->send_msg(cData, 6);
            switch (iRet)
            {
                case DEF_XSOCKEVENT_QUENEFULL:
                case DEF_XSOCKEVENT_SOCKETERROR:
                case DEF_XSOCKEVENT_CRITICALERROR:
                case DEF_XSOCKEVENT_SOCKETCLOSED:
                    // ¸Þ½ÃÁö¸¦ º¸³¾¶§ ¿¡·¯°¡ ¹ß»ýÇß´Ù¸é Á¦°ÅÇÑ´Ù.
                    DeleteClient(iClientH, true, true);
                    break;
            }
        }
    }
    return;
}

void CGame::RequestGuildTeleportHandler(int iClientH)
{
    int i;
    char cMapName[11];

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_iLockedMapTime != 0)
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_LOCKEDMAP, m_pClientList[iClientH]->m_iLockedMapTime, 0, 0, m_pClientList[iClientH]->m_cLockedMapName);
        return;
    }

    // if a guild teleport is set when its not a crusade, log the hacker
    if (!m_bIsCrusadeMode)
    {
        try
        {
            copy_string(G_cTxt, "Accessing crusade teleport: (%s) Player: (%s) - setting teleport location when crusade is disabled.",
                m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
            PutHackLogFileList(G_cTxt);
            DeleteClient(iClientH, true, true);
        }
        catch (...)
        {
        }
        return;
    }

    // if a player is using guild teleport and he is not in a guild, log the hacker
    if (m_pClientList[iClientH]->m_iCrusadeDuty == 0)
    {
        try
        {
            copy_string(G_cTxt, "Accessing crusade teleport: (%s) Player: (%s) - teleporting when not in a guild",
                m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
            PutHackLogFileList(G_cTxt);
            DeleteClient(iClientH, true, true);
        }
        catch (...)
        {
        }
        return;
    }

    if ((m_pClientList[iClientH]->m_cMapIndex == m_iMiddlelandMapIndex) &&
        m_iMiddlelandMapIndex != -1)
        return;

    for (i = 0; i < DEF_MAXGUILDS; i++)
        if (m_pGuildTeleportLoc[i].m_iV1 == m_pClientList[iClientH]->m_iGuildGUID)
        {
            // ±æµå¸¦ Ã£¾Ò´Ù.
            memset(cMapName, 0, sizeof(cMapName));
            strcpy(cMapName, m_pGuildTeleportLoc[i].m_cDestMapName);

            //testcode
            copy_string(G_cTxt, "ReqGuildTeleport: %d %d %d %s", m_pClientList[iClientH]->m_iGuildGUID, m_pGuildTeleportLoc[i].m_sDestX, m_pGuildTeleportLoc[i].m_sDestY, cMapName);
            log->info(G_cTxt);

            // !!! RequestTeleportHandler³»¿¡¼­ m_cMapNameÀ» ¾²±â ¶§¹®¿¡ ±×´ë·Î ÆÄ¶ó¹ÌÅÍ·Î ³Ñ°ÜÁÖ¸é ¿Àµ¿ÀÛ
            RequestTeleportHandler(iClientH, "2   ", cMapName, m_pGuildTeleportLoc[i].m_sDestX, m_pGuildTeleportLoc[i].m_sDestY);
            return;
        }

    // ±æµå¿¡ ÇÒ´çµÈ ÅÚ·¹Æ÷Æ® ÁÂÇ¥°¡ ¾ø´Ù. ±æµå ¼Ò¼ÓÀÌ ¾Æ´Ñ °æ¿ì
    switch (m_pClientList[iClientH]->m_cSide)
    {
        case 1: // ¾Æ·¹½ºµ§ ¼Ò¼Ó 
            break;
        case 2: // ¿¤¹ÙÀÎ ¼Ò¼Ó
            break;
    }
}

void CGame::RequestSetGuildTeleportLocHandler(int iClientH, int dX, int dY, int iGuildGUID, char * pMapName)
{
    char * cp, cData[120];
    int i;
    int * ip, iIndex;
    uint32_t dwTemp, dwTime;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsOnServerChange == true) return;

    // if a player is teleporting and its not a crusade, log the hacker
    if (!m_bIsCrusadeMode)
    {
        try
        {
            copy_string(G_cTxt, "Accessing Crusade Set Teleport:(%s) Player: (%s) - setting point when not a crusade.",
                m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
            PutHackLogFileList(G_cTxt);
            DeleteClient(iClientH, true, true);
        }
        catch (...)
        {

        }
        return;
    }

    // if a player is teleporting and its not a crusade, log the hacker
    if (m_pClientList[iClientH]->m_iCrusadeDuty != 3)
    {
        try
        {
            copy_string(G_cTxt, "Accessing Crusade Set Teleport: (%s) Player: (%s) - setting point when not a guildmaster.",
                m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
            PutHackLogFileList(G_cTxt);
            DeleteClient(iClientH, true, true);
        }
        catch (...)
        {

        }
        return;
    }

    // v2.04 ÁÂÇ¥ º¸Á¤ 
    if (dY < 100) dY = 100;
    if (dY > 600) dY = 600;

    // °ÔÀÌÆ® ¼­¹ö ¸Þ½ÃÁö ÀÛ¼º 
    memset(cData, 0, sizeof(cData));
    cp = (char *)cData;
    *cp = GSM_SETGUILDTELEPORTLOC;
    cp++;

    ip = (int *)cp;
    *ip = iGuildGUID;
    cp += 4;

    ip = (int *)cp;
    *ip = dX;
    cp += 4;

    ip = (int *)cp;
    *ip = dY;
    cp += 4;

    memcpy(cp, pMapName, 10);
    cp += 10;
    //

    dwTime = timeGetTime();

    //testcode
    copy_string(G_cTxt, "SetGuildTeleportLoc: %d %s %d %d", iGuildGUID, pMapName, dX, dY);
    log->info(G_cTxt);

    // ¸ÕÀú °°Àº GUID¸¦ °¡Áø ±æµå ÅÚ·¹Æ÷Æ® ÁÂÇ¥°¡ ¼³Á¤µÇ¾î ÀÖ´ÂÁö Ã£´Â´Ù.
    for (i = 0; i < DEF_MAXGUILDS; i++)
        if (m_pGuildTeleportLoc[i].m_iV1 == iGuildGUID)
        {
            // ÀÌ¹Ì ±æµå°¡ ÀÖ´Ù.
            if ((m_pGuildTeleportLoc[i].m_sDestX == dX) && (m_pGuildTeleportLoc[i].m_sDestY == dY) && (strcmp(m_pGuildTeleportLoc[i].m_cDestMapName, pMapName) == 0))
            {
                // ³»¿ëÀÌ ÀÏÄ¡ÇÑ´Ù. Å¸ÀÓ ÀÔ·ÂÈÄ ½ºÅµ.
                m_pGuildTeleportLoc[i].m_dwTime = dwTime;
                return;
            }
            else
            {
                // ³»¿ë °»½ÅÈÄ 
                m_pGuildTeleportLoc[i].m_sDestX = dX;
                m_pGuildTeleportLoc[i].m_sDestY = dY;
                memset(m_pGuildTeleportLoc[i].m_cDestMapName, 0, sizeof(m_pGuildTeleportLoc[i].m_cDestMapName));
                strcpy(m_pGuildTeleportLoc[i].m_cDestMapName, pMapName);
                m_pGuildTeleportLoc[i].m_dwTime = dwTime;

                //°ÔÀÌÆ® ¼­¹ö¸¦ ÅëÇØ ´Ù¸¥ ¼­¹ö·Î Á¤º¸ Àü¼Û
                bStockMsgToGateServer(cData, 23);
                return;
            }
        }

    // ¼³Á¤µÈ ³»¿ëÀÌ ¾ø´Ù. »õ·Î ¼³Á¤ÇÑ´Ù.
    dwTemp = 0;
    iIndex = -1;
    for (i = 0; i < DEF_MAXGUILDS; i++)
    {
        if (m_pGuildTeleportLoc[i].m_iV1 == 0)
        {

            m_pGuildTeleportLoc[i].m_iV1 = iGuildGUID;
            m_pGuildTeleportLoc[i].m_sDestX = dX;
            m_pGuildTeleportLoc[i].m_sDestY = dY;
            memset(m_pGuildTeleportLoc[i].m_cDestMapName, 0, sizeof(m_pGuildTeleportLoc[i].m_cDestMapName));
            strcpy(m_pGuildTeleportLoc[i].m_cDestMapName, pMapName);
            m_pGuildTeleportLoc[i].m_dwTime = dwTime;

            //°ÔÀÌÆ® ¼­¹ö¸¦ ÅëÇØ ´Ù¸¥ ¼­¹ö·Î Á¤º¸ Àü¼Û  
            bStockMsgToGateServer(cData, 23);
            return;
        }
        else
        {
            // °¡Àå ¿À·§µ¿¾È ÂüÁ¶µÇÁö ¾ÊÀº ¸®½ºÆ®ÀÇ ÀÎµ¦½º¸¦ Ã£´Â´Ù.
            if (dwTemp < (dwTime - m_pGuildTeleportLoc[i].m_dwTime))
            {
                dwTemp = (dwTime - m_pGuildTeleportLoc[i].m_dwTime);
                iIndex = i;
            }
        }
    }

    // Ä³½Ã °ø°£ÀÌ ´Ù Ã¡´Ù. ÀÌ·± °æ¿ì ¸®½ºÆ® Áß °¡Àå ¿À·§µ¿¾È »ç¿ëÇÏÁö ¾ÊÀº ¸®½ºÆ®(iIndex)¸¦ »èÁ¦ÇÏ°í ¾÷µ¥ÀÌÆ® ÇÑ´Ù.
    if (iIndex == -1) return;

    //testcode
    log->info("(X) No more GuildTeleportLoc Space! Replaced.");

    m_pGuildTeleportLoc[i].m_iV1 = iGuildGUID;
    m_pGuildTeleportLoc[i].m_sDestX = dX;
    m_pGuildTeleportLoc[i].m_sDestY = dY;
    memset(m_pGuildTeleportLoc[i].m_cDestMapName, 0, sizeof(m_pGuildTeleportLoc[i].m_cDestMapName));
    strcpy(m_pGuildTeleportLoc[i].m_cDestMapName, pMapName);
    m_pGuildTeleportLoc[i].m_dwTime = dwTime;

    //°ÔÀÌÆ® ¼­¹ö¸¦ ÅëÇØ ´Ù¸¥ ¼­¹ö·Î Á¤º¸ Àü¼Û
    bStockMsgToGateServer(cData, 23);
}

void CGame::MeteorStrikeHandler(int iMapIndex)
{
    int i, ix, iy, dX, dY, iIndex, iTargetIndex, iTotalESG, iEffect;
    int iTargetArray[DEF_MAXSTRIKEPOINTS];
    short sOwnerH;
    char  cOwnerType;
    uint32_t dwTime = timeGetTime();

    log->info("(!) Beginning Meteor Strike Procedure...");

    if (iMapIndex == -1)
    {
        log->info("(X) MeteorStrikeHandler Error! MapIndex -1!");
        return;
    }

    if (m_pMapList[iMapIndex] == 0)
    {
        log->info("(X) MeteorStrikeHandler Error! 0 Map!");
        return;
    }

    if (m_pMapList[iMapIndex]->m_iTotalStrikePoints == 0)
    {
        log->info("(X) MeteorStrikeHandler Error! No Strike Points!");
        return;
    }

    for (i = 0; i < DEF_MAXSTRIKEPOINTS; i++) iTargetArray[i] = -1;

    iIndex = 0;
    for (i = 1; i <= m_pMapList[iMapIndex]->m_iTotalStrikePoints; i++)
    {
        if (m_pMapList[iMapIndex]->m_stStrikePoint[i].iHP > 0)
        {
            iTargetArray[iIndex] = i; // Â½ÂºÃ†Â®Â¶Ã³Ã€ÃŒÃ…Â© Ã†Ã·Ã€ÃŽÃ†Â®Ã€Ã‡ Ã€ÃŽÂµÂ¦Â½ÂºÂ¸Â¦ Â³Ã–Â´Ã‚Â´Ã™.
            iIndex++;
        }
    }

    //testcode
    copy_string(G_cTxt, "(!) Map(%s) has %d available strike points", m_pMapList[iMapIndex]->m_cName, iIndex);
    log->info(G_cTxt);

    // Â°Ã¡Â°Ãº ÂºÂ¸Â°Ã­Â¿Ã« Â½ÂºÃ†Â®Â·Â°ÃƒÃ§ Ã…Â¬Â¸Â®Â¾Ã®
    m_stMeteorStrikeResult.iCasualties = 0;
    m_stMeteorStrikeResult.iCrashedStructureNum = 0;
    m_stMeteorStrikeResult.iStructureDamageAmount = 0;

    if (iIndex == 0)
    {
        // Ã†Ã¸Â°ÃÃ‡Ã’ Â°Ã‡Â¹Â°Ã€ÃŒ Â¾Ã¸Â´Ã™. Â¸Ã°ÂµÃŽ Ã†Ã„Â±Â«ÂµÃ‡Â¾ÃºÂ´Ã™.
        log->info("(!) No strike points!");
        bRegisterDelayEvent(DEF_DELAYEVENTTYPE_CALCMETEORSTRIKEEFFECT, 0, dwTime + 6000, 0, 0, iMapIndex, 0, 0, 0, 0, 0);
    }
    else
    {

        // Ã‡Ã˜Â´Ã§ Â¸ÃŠÃ€Ã‡ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®ÂµÃ©Â¿Â¡Â°Ã” Â¸ÃžÃ…Ã—Â¿Ã€ Â°Ã¸Â°Ã ÃˆÂ¿Â°ÃºÂ¸Â¦ ÂºÂ¸Â¿Â©ÃÃ–Â¶Ã³Â´Ã‚ Â¸ÃžÂ½ÃƒÃÃ¶ Ã€Ã¼Â¼Ã›
        for (i = 1; i < DEF_MAXCLIENTS; i++)
            if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_bIsInitComplete == true) && (m_pClientList[i]->m_cMapIndex == iMapIndex))
            {
                SendNotifyMsg(0, i, DEF_NOTIFY_METEORSTRIKEHIT, 0, 0, 0, 0);
            }

        for (i = 0; i < iIndex; i++)
        {
            // Â¸Ã°ÂµÃ§ Ã†Ã¸Â°Ã Â¸Ã±Ã‡Â¥Â¿Â¡ Â´Ã«Ã‡Ã˜ ÂµÂ¿Ã€ÃÃ‡Ã‘ Â°Ã¸Â°ÃÃ€Â» Â°Â¡Ã‡Ã‘Â´Ã™.
            iTargetIndex = iTargetArray[i];

            if (iTargetIndex == -1)
            {
                log->info("(X) Strike Point MapIndex: -1!");
                goto MSH_SKIP_STRIKE;
            }

            dX = m_pMapList[iMapIndex]->m_stStrikePoint[iTargetIndex].dX;
            dY = m_pMapList[iMapIndex]->m_stStrikePoint[iTargetIndex].dY;

            // dX, dY Ã€Â§Ã„Â¡ ÃÃ–Ã€Â§Â¿Â¡ ÃƒÃ–Â¼Ã’ 2Â°Â³ Ã€ÃŒÂ»Ã³Ã€Ã‡ Energy Shield GeneratorÂ°Â¡ ÃÂ¸Ã€Ã§Ã‡ÃÂ¸Ã© Â½ÂºÃ†Â®Â¶Ã³Ã€ÃŒÃ…Â© Ã†Ã·Ã€ÃŽÃ†Â®Â´Ã‚ Â¹Â«Â»Ã§Ã‡ÃÂ´Ã™. Â±Ã—Â·Â¯Â³Âª 1Â°Â³ Ã€ÃŒÃ‡ÃÃ€ÃŒÂ¸Ã© Â°Ã¸Â°ÃÂ¹ÃžÂ¾Ã’Ã€Â»Â¶Â§ HPÂ°Â¡ ÃÃ™Â¾Ã®ÂµÃ§Â´Ã™.
            // NPC ÃÃ–ÂºÂ¯Â¿Â¡ Ã€Ã–Â´Ã‚ Ã€Ã» Ã„Â³Â¸Â¯Ã…ÃÂ¸Â¦ Ã…Â½ÃÃ¶Ã‡Ã˜ Â°Ã¦ÂºÂ¸Â¸Â¦ Â¿Ã¯Â¸Â°Â´Ã™.
            iTotalESG = 0;
            for (ix = dX - 10; ix <= dX + 10; ix++)
                for (iy = dY - 10; iy <= dY + 10; iy++)
                {
                    m_pMapList[iMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                    if ((cOwnerType == DEF_OWNERTYPE_NPC) && (m_pNpcList[sOwnerH] != 0) && (m_pNpcList[sOwnerH]->m_sType == 40))
                    {
                        iTotalESG++;
                    }
                }

            // testcode
            copy_string(G_cTxt, "(!) Meteor Strike Target(%d, %d) ESG(%d)", dX, dY, iTotalESG);
            log->info(G_cTxt);

            if (iTotalESG < 2)
            {

                m_pMapList[iMapIndex]->m_stStrikePoint[iTargetIndex].iHP -= (2 - iTotalESG);
                if (m_pMapList[iMapIndex]->m_stStrikePoint[iTargetIndex].iHP <= 0)
                {
                    // Â°Ã‡Â¹Â°Ã€ÃŒ Ã†Ã„Â±Â«ÂµÃ‡Â¾ÃºÂ´Ã™.
                    m_pMapList[iMapIndex]->m_stStrikePoint[iTargetIndex].iHP = 0;
                    // Â°Ã‡Â¹Â° Â±Ã¢Â´Ã‰ Â»Ã³Â½Ã‡
                    m_pMapList[m_pMapList[iMapIndex]->m_stStrikePoint[iTargetIndex].iMapIndex]->m_bIsDisabled = true;
                    m_stMeteorStrikeResult.iCrashedStructureNum++;
                }
                else
                {
                    // Â°Ã‡Â¹Â°Ã€ÃŒ Â´Ã«Â¹ÃŒÃÃ¶Â¸Â¦ Â¾Ã²Â¾ÃºÂ´Ã™.
                    m_stMeteorStrikeResult.iStructureDamageAmount += (2 - iTotalESG);
                    // Â°Ã‡Â¹Â° Â´Ã«Â¹ÃŒÃÃ¶ Ã€Ã”Ã€Âº ÃˆÃ„ ÂºÃ’Ã…Â¸Â´Ã‚ Ã€ÃŒÂ¹ÃŒÃÃ¶ Â»Ã°Ã€Ã” 
                    iEffect = iDice(1, 5) - 1;
                    iAddDynamicObjectList(0, DEF_OWNERTYPE_PLAYER_INDIRECT, DEF_DYNAMICOBJECT_FIRE2, iMapIndex,
                        m_pMapList[iMapIndex]->m_stStrikePoint[iTargetIndex].iEffectX[iEffect] + (iDice(1, 3) - 2),
                        m_pMapList[iMapIndex]->m_stStrikePoint[iTargetIndex].iEffectY[iEffect] + (iDice(1, 3) - 2), 60 * 1000 * 50);
                }
            }
            MSH_SKIP_STRIKE:;
        }

        // 1ÃƒÃŠ ÃˆÃ„ Â¸ÃžÃ…Ã—Â¿Ã€ Â½ÂºÃ†Â®Â¶Ã³Ã€ÃŒÃ…Â© Â°Ã¸Â°Ã Â´Ã«Â¹ÃŒÃÃ¶ ÃˆÂ¿Â°Ãº
        bRegisterDelayEvent(DEF_DELAYEVENTTYPE_DOMETEORSTRIKEDAMAGE, 0, dwTime + 1000, 0, 0, iMapIndex, 0, 0, 0, 0, 0);
        // 4ÃƒÃŠ ÃˆÃ„ Â¸ÃžÃ…Ã—Â¿Ã€ Â½ÂºÃ†Â®Â¶Ã³Ã€ÃŒÃ…Â© Â°Ã¸Â°Ã Â´Ã«Â¹ÃŒÃÃ¶ ÃˆÂ¿Â°Ãº
        bRegisterDelayEvent(DEF_DELAYEVENTTYPE_DOMETEORSTRIKEDAMAGE, 0, dwTime + 4000, 0, 0, iMapIndex, 0, 0, 0, 0, 0);
        // 6ÃƒÃŠ ÃˆÃ„ Â°Ã¸Â°Ã Â°Ã¡Â°Ãº Ã†Ã‡Â´Ãœ.
        bRegisterDelayEvent(DEF_DELAYEVENTTYPE_CALCMETEORSTRIKEEFFECT, 0, dwTime + 6000, 0, 0, iMapIndex, 0, 0, 0, 0, 0);
    }
}

void CGame::CollectedManaHandler(uint16_t wAresdenMana, uint16_t wElvineMana)
{
    if (m_iAresdenMapIndex != -1)
    {
        m_iAresdenMana += wAresdenMana;
        //testcode
        if (wAresdenMana > 0)
        {
            copy_string(G_cTxt, "Aresden Mana: %d Total:%d", wAresdenMana, m_iAresdenMana);
            log->info(G_cTxt);
        }
    }

    if (m_iElvineMapIndex != -1)
    {
        m_iElvineMana += wElvineMana;
        //testcode
        if (wElvineMana > 0)
        {
            copy_string(G_cTxt, "Elvine Mana: %d Total:%d", wElvineMana, m_iElvineMana);
            log->info(G_cTxt);
        }
    }
}

void CGame::CalcMeteorStrikeEffectHandler(int iMapIndex)
{
    int i, iActiveStructure, iStructureHP[DEF_MAXSTRIKEPOINTS];
    char * cp, * cp2, cData[120], cWinnerSide, cTempData[120];
    uint16_t * wp;

    if (m_bIsCrusadeMode == false) return;

    // »ç¿ëÀÚ¿¡°Ô ³²Àº °Ç¹°ÀÇ HP¸¦ º¸¿©ÁÖ±â À§ÇÑ º¯¼ö ÃÊ±âÈ­ 
    for (i = 0; i < DEF_MAXSTRIKEPOINTS; i++)
        iStructureHP[i] = 0;

    // Æø°ÝÀÇ È¿°ú¸¦ °è»êÇÑ´Ù. ¸ðµç °Ç¹°ÀÌ ÆÄ±«µÇ¾ú´Ù¸é Àü¸éÀü ¸ðµå Á¾·á. ±×·¸Áö ¾ÊÀ¸¸é °á°ú¸¦ ¸ðµç ¼­¹ö¿¡ ¸®Æ÷Æ®.
    iActiveStructure = 0;
    for (i = 1; i <= m_pMapList[iMapIndex]->m_iTotalStrikePoints; i++)
    {
        if (m_pMapList[iMapIndex]->m_stStrikePoint[i].iHP > 0)
        {
            iActiveStructure++;
            iStructureHP[i] = m_pMapList[iMapIndex]->m_stStrikePoint[i].iHP;
        }
    }

    //testcode
    copy_string(G_cTxt, "ActiveStructure:%d  MapIndex:%d AresdenMap:%d ElvineMap:%d", iActiveStructure, iMapIndex, m_iAresdenMapIndex, m_iElvineMapIndex);
    log->info(G_cTxt);

    if (iActiveStructure == 0)
    {
        // ÀÛµ¿ÁßÀÎ °Ç¹°ÀÌ ¾ø´Ù. Å©·ç¼¼ÀÌµå ¸ðµå Á¾·á: ´Ù¸¥ ¼­¹ö¿¡ ¾Ë·ÁÁØ´Ù.
        // Å©·ç¼¼ÀÌµå ¸ðµå Á¾·áµÊÀ» ÇöÀç ¼­¹öÀÇ Å¬¶óÀÌ¾ðÆ®µé¿¡°Ô ¹Ù·Î Àü´Þ.
        if (iMapIndex == m_iAresdenMapIndex)
        {
            cWinnerSide = 2;
            LocalEndCrusadeMode(2); // ÀÌ±ä ÂÊÀº ¿¤¹ÙÀÎ
        }
        else if (iMapIndex == m_iElvineMapIndex)
        {
            cWinnerSide = 1;
            LocalEndCrusadeMode(1); // ÀÌ±ä ÂÊÀº ¾Æ·¹½ºµ§
        }
        else
        {
            cWinnerSide = 0;
            LocalEndCrusadeMode(0);
        }

        // ´Ù¸¥ ¼­¹ö¿¡ Å©·ç¼¼ÀÌµå Á¾·á¸¦ ¾Ë¸².
        memset(cData, 0, sizeof(cData));
        cp = (char *)(cData);
        *cp = GSM_ENDCRUSADE;
        cp++;

        *cp = cWinnerSide;
        cp++;

        wp = (uint16_t *)cp;
        *wp = m_stMeteorStrikeResult.iCrashedStructureNum;
        cp += 2;

        wp = (uint16_t *)cp;
        *wp = m_stMeteorStrikeResult.iStructureDamageAmount;
        cp += 2;

        wp = (uint16_t *)cp;
        *wp = m_stMeteorStrikeResult.iCasualties;
        cp += 2;

        memcpy(cp, m_pMapList[iMapIndex]->m_cName, 10);
        cp += 10;

        bStockMsgToGateServer(cData, 18);

    }
    else
    {
        // ÇÇÇØ »óÈ²À» ¸ðµç ¼­¹ö·Î ¸®Æ÷Æ®.
        memset(cData, 0, sizeof(cData));
        cp = (char *)(cData);
        *cp = GSM_GRANDMAGICRESULT;
        cp++;

        memcpy(cp, m_pMapList[iMapIndex]->m_cName, 10);
        cp += 10;

        wp = (uint16_t *)cp;
        *wp = m_stMeteorStrikeResult.iCrashedStructureNum;
        cp += 2;

        wp = (uint16_t *)cp;
        *wp = m_stMeteorStrikeResult.iStructureDamageAmount;
        cp += 2;

        wp = (uint16_t *)cp;
        *wp = m_stMeteorStrikeResult.iCasualties;
        cp += 2;

        wp = (uint16_t *)cp;
        *wp = (uint16_t)iActiveStructure;
        cp += 2;

        //v2.15 Ãß°¡  ÅäÅ» °Ç¹°ÀÇ °¹¼ö 
        memset(cTempData, 0, sizeof(cTempData));
        cp2 = (char *)(cTempData);

        wp = (uint16_t *)cp2;
        *wp = (uint16_t)m_pMapList[iMapIndex]->m_iTotalStrikePoints;
        cp2 += 2;

        for (i = 1; i <= m_pMapList[iMapIndex]->m_iTotalStrikePoints; i++)
        {
            wp = (uint16_t *)cp2;
            *wp = (uint16_t)iStructureHP[i];
            cp2 += 2;
        }

        memcpy(cp, cTempData, 2 * (m_pMapList[iMapIndex]->m_iTotalStrikePoints + 1));

        // v2.15 
        bStockMsgToGateServer(cData, 18 + (m_pMapList[iMapIndex]->m_iTotalStrikePoints + 1) * 2);

        // ÇöÀç ¼­¹ö¿¡´Â ½ºÅå ¸Þ½ÃÁö°¡ ¿ÀÁö ¾ÊÀ¸¹Ç·Î Á÷Á¢ Àü´ÞÇÑ´Ù.
        // v2.15 °Ç¹°ÀÇ °¹¼ö¿Í HP¸¦ º¸³½´Ù.
        GrandMagicResultHandler(m_pMapList[iMapIndex]->m_cName, m_stMeteorStrikeResult.iCrashedStructureNum, m_stMeteorStrikeResult.iStructureDamageAmount, m_stMeteorStrikeResult.iCasualties, iActiveStructure, m_pMapList[iMapIndex]->m_iTotalStrikePoints, cTempData);
    }

    // °á°ú º¸°í¿ë ½ºÆ®·°Ãç Å¬¸®¾î
    m_stMeteorStrikeResult.iCasualties = 0;
    m_stMeteorStrikeResult.iCrashedStructureNum = 0;
    m_stMeteorStrikeResult.iStructureDamageAmount = 0;
}

void CGame::GrandMagicResultHandler(char * cMapName, int iCrashedStructureNum, int iStructureDamageAmount, int iCasualities, int iActiveStructure, int iTotalStrikePoints, char * cData)
{
    int i;

    for (i = 1; i < DEF_MAXCLIENTS; i++)
        if (m_pClientList[i] != 0)
        {
            SendNotifyMsg(0, i, DEF_NOTIFY_GRANDMAGICRESULT, iCrashedStructureNum, iStructureDamageAmount, iCasualities, cMapName, iActiveStructure, 0, 0, 0, 0, iTotalStrikePoints, cData);
        }
}

void CGame::RequestCreatePartyHandler(int iClientH)
{
    char * cp, cData[120];
    uint32_t * dwp;
    uint16_t * wp;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

    if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_NULL)
    {
        // Ã†Ã„Ã†Â¼ Â»Ã³Ã…Ã‚Â°Â¡ Ã€ÃŒÂ¹ÃŒ ÃÂ¸Ã€Ã§Ã‡ÃÂ¸Ã© Ã†Ã„Ã†Â¼Â¸Â¦ Â¸Â¸ÂµÃ© Â¼Ã¶ Â¾Ã¸Â´Ã™.
        return;
    }

    m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_PROCESSING;

    // Gate ServerÂ¿Â¡ Ã†Ã„Ã†Â¼ Â»Ã½Â¼ÂºÃˆÃ„ PartyIDÂ¸Â¦ Â¾Ã‹Â·ÃÃÃ™ Â°ÃÃ€Â» Â¿Ã¤ÃƒÂ»Ã‡Ã‘Â´Ã™. 
    memset(cData, 0, sizeof(cData));
    cp = (char *)cData;

    dwp = (uint32_t *)cp;
    *dwp = MSGID_PARTYOPERATION;
    cp += 4;
    wp = (uint16_t *)cp;
    *wp = 1; // 1, request
    cp += 2;

    wp = (uint16_t *)cp;
    *wp = iClientH;
    cp += 2;

    memcpy(cp, m_pClientList[iClientH]->m_cCharName, 10);
    cp += 10;

    SendMsgToGateServer(MSGID_PARTYOPERATION, iClientH, cData);

    //testcode
    copy_string(G_cTxt, "Request Create Party: %d", iClientH);
    log->info(G_cTxt);
}

void CGame::PartyOperationResultHandler(char * pData)
{
    char * cp, cResult, cName[12];
    uint16_t * wp;
    int i, iClientH, iPartyID, iTotal;

    cp = (char *)(pData + 4);
    wp = (uint16_t *)cp;
    cp += 2;

    switch (*wp)
    {
        case 1:
            cResult = *cp;
            cp++;

            wp = (uint16_t *)cp;
            iClientH = (int)*wp;
            cp += 2;

            memset(cName, 0, sizeof(cName));
            memcpy(cName, cp, 10);
            cp += 10;

            wp = (uint16_t *)cp;
            iPartyID = (int)*wp;
            cp += 2;

            PartyOperationResult_Create(iClientH, cName, cResult, iPartyID);

            copy_string(G_cTxt, "party Operation Result: Create(ClientH:%d PartyID:%d)", iClientH, iPartyID);
            log->info(G_cTxt);
            break;

        case 2:
            wp = (uint16_t *)cp;
            iPartyID = *wp;
            cp += 2;

            PartyOperationResult_Delete(iPartyID);

            copy_string(G_cTxt, "party Operation Result: Delete(PartyID:%d)", iPartyID);
            log->info(G_cTxt);
            break;

        case 3:
            wp = (uint16_t *)cp;
            iClientH = *wp;
            cp += 2;

            memset(cName, 0, sizeof(cName));
            memcpy(cName, cp, 10);
            cp += 10;

            if ((iClientH < 0) && (iClientH > DEF_MAXCLIENTS)) return;
            if (m_pClientList[iClientH] == nullptr) return;
            if (strcmp(m_pClientList[iClientH]->m_cCharName, cName) != 0) return;

            for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
                if (m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] == iClientH)
                {
                    m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] = 0;
                    m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers--;

                    copy_string(G_cTxt, "PartyID:%d member:%d Out(Clear) Total:%d", m_pClientList[iClientH]->m_iPartyID, iClientH, m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers);
                    log->info(G_cTxt);
                    goto PORH_LOOPBREAK1;
                }
            PORH_LOOPBREAK1:;

            for (i = 0; i < DEF_MAXPARTYMEMBERS - 1; i++)
                if ((m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] == 0) && (m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i + 1] != 0))
                {
                    m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] = m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i + 1];
                    m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i + 1] = 0;
                }

            m_pClientList[iClientH]->m_iPartyID = 0;
            m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;

            copy_string(G_cTxt, "Party Status 0: %s", m_pClientList[iClientH]->m_cCharName);
            log->info(G_cTxt);

            SendNotifyMsg(0, iClientH, DEF_NOTIFY_PARTY, 8, 0, 0, 0);
            break;

        case 4:
            cResult = *cp;
            cp++;

            wp = (uint16_t *)cp;
            iClientH = (int)*wp;
            cp += 2;

            memset(cName, 0, sizeof(cName));
            memcpy(cName, cp, 10);
            cp += 10;

            wp = (uint16_t *)cp;
            iPartyID = (int)*wp;
            cp += 2;

            PartyOperationResult_Join(iClientH, cName, cResult, iPartyID);

            copy_string(G_cTxt, "party Operation Result: Join(ClientH:%d PartyID:%d)", iClientH, iPartyID);
            log->info(G_cTxt);
            break;

        case 5:
            wp = (uint16_t *)cp;
            iClientH = (int)*wp;
            cp += 2;

            memset(cName, 0, sizeof(cName));
            memcpy(cName, cp, 10);
            cp += 10;

            wp = (uint16_t *)cp;
            iTotal = (int)*wp;
            cp += 2;

            PartyOperationResult_Info(iClientH, cName, iTotal, cp);

            copy_string(G_cTxt, "party Operation Result: Info(ClientH:%d Total:%d)", iClientH, iTotal);
            log->info(G_cTxt);
            break;

        case 6:
            cResult = *cp;
            cp++;

            wp = (uint16_t *)cp;
            iClientH = (int)*wp;
            cp += 2;

            memset(cName, 0, sizeof(cName));
            memcpy(cName, cp, 10);
            cp += 10;

            wp = (uint16_t *)cp;
            iPartyID = (int)*wp;
            cp += 2;

            PartyOperationResult_Dismiss(iClientH, cName, cResult, iPartyID);

            copy_string(G_cTxt, "party Operation Result: Dismiss(ClientH:%d PartyID:%d)", iClientH, iPartyID);
            log->info(G_cTxt);
            break;
    }
}

void CGame::RequestJoinPartyHandler(int iClientH, char * pData, uint32_t dwMsgSize)
{
    char   seps[] = "= \t\n";
    char * cp, * token, cBuff[256], cData[120], cName[12];
    class  CStrTok * pStrTok;
    uint32_t * dwp;
    uint16_t * wp;
    int i;

    // Ã†Ã„Ã†Â¼ Â°Â¡Ã€Ã” Â¿Â©ÂºÃŽÂ¸Â¦ Â¹Â¯Â°Ã­ Ã€Ã–Â´Ã™.
    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_NULL) return;
    if ((dwMsgSize) <= 0) return;
    if ((m_bAdminSecurity == true) && (m_pClientList[iClientH]->m_iAdminUserLevel > 0)) return;

    memset(cBuff, 0, sizeof(cBuff));
    memcpy(cBuff, pData, dwMsgSize);

    pStrTok = new class CStrTok(cBuff, seps);
    token = pStrTok->pGet();

    token = pStrTok->pGet();
    if (token != 0)
    {
        memset(cName, 0, sizeof(cName));
        strcpy(cName, token);
    }
    else return;

    for (i = 1; i < DEF_MAXCLIENTS; i++)
        if ((m_pClientList[i] != 0) && (strcmp(m_pClientList[i]->m_cCharName, cName) == 0))
        {
            // Â°Ã”Ã€ÃŒÃ†Â® Â¼Â­Â¹Ã¶Â·ÃŽ Ã†Ã„Ã†Â¼ Â¸Ã¢Â¹Ã¶ Â°Â¡Ã€Ã”Ã€Â» Â½Ã…ÃƒÂ»Ã‡Ã‘Â´Ã™.
            if ((m_pClientList[i]->m_iPartyID == 0) || (m_pClientList[i]->m_iPartyStatus != DEF_PARTYSTATUS_CONFIRM))
            {
                // Ã†Ã„Ã†Â¼Â¿Â¡ Â°Â¡Ã€Ã”Ã‡ÃÂ°Ã­Ã€Ãš Ã‡Ã‘ Ã„Â³Â¸Â¯Ã…ÃÂ°Â¡ Ã†Ã„Ã†Â¼ Â¸Â¶Â½ÂºÃ…ÃÂ°Â¡ Â¾Ã†Â´ÃÂ´Ã™.
                return;
            }

            memset(cData, 0, sizeof(cData));

            cp = (char *)cData;
            dwp = (uint32_t *)cp;
            *dwp = MSGID_PARTYOPERATION;
            cp += 4;
            wp = (uint16_t *)cp;
            *wp = 3; // Ã†Ã„Ã†Â¼ Â¸Ã¢Â¹Ã¶ ÃƒÃŸÂ°Â¡ Â¿Ã¤ÃƒÂ»
            cp += 2;
            wp = (uint16_t *)cp;
            *wp = iClientH;
            cp += 2;
            memcpy(cp, m_pClientList[iClientH]->m_cCharName, 10);
            cp += 10;
            wp = (uint16_t *)cp;
            *wp = m_pClientList[i]->m_iPartyID;
            cp += 2;
            SendMsgToGateServer(MSGID_PARTYOPERATION, iClientH, cData);
            return;
        }

    // Â°Â¡Ã€Ã”Ã‡ÃÂ°Ã­Ã€Ãš Ã‡ÃÂ´Ã‚ Ã†Ã„Ã†Â¼ Â¸Â¶Â½ÂºÃ…ÃÂ°Â¡ Ã‡Ã¶Ã€Ã§ Â¼Â­Â¹Ã¶Â¿Â¡ ÃÂ¸Ã€Ã§Ã‡ÃÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™.
    SendNotifyMsg(0, iClientH, DEF_NOTIFY_PLAYERNOTONGAME, 0, 0, 0, cName);
}

void CGame::RequestDismissPartyHandler(int iClientH)
{
    char * cp, cData[120];
    uint32_t * dwp;
    uint16_t * wp;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_CONFIRM) return;

    memset(cData, 0, sizeof(cData));
    cp = (char *)cData;
    dwp = (uint32_t *)cp;
    *dwp = MSGID_PARTYOPERATION;
    cp += 4;
    wp = (uint16_t *)cp;
    *wp = 4; // Â¸Ã¢Â¹Ã¶ ÃÂ¦Â°Ã… Â¿Ã¤ÃƒÂ»
    cp += 2;
    wp = (uint16_t *)cp;
    *wp = iClientH;
    cp += 2;
    memcpy(cp, m_pClientList[iClientH]->m_cCharName, 10);
    cp += 10;
    wp = (uint16_t *)cp;
    *wp = m_pClientList[iClientH]->m_iPartyID;
    cp += 2;
    SendMsgToGateServer(MSGID_PARTYOPERATION, iClientH, cData);

    m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_PROCESSING;
}

void CGame::GetPartyInfoHandler(int iClientH)
{
    char * cp, cData[120];
    uint32_t * dwp;
    uint16_t * wp;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_CONFIRM) return;

    memset(cData, 0, sizeof(cData));
    cp = (char *)cData;
    dwp = (uint32_t *)cp;
    *dwp = MSGID_PARTYOPERATION;
    cp += 4;
    wp = (uint16_t *)cp;
    *wp = 5; // Ã†Ã„Ã†Â¼ ÃÂ¤ÂºÂ¸ Â¿Ã¤ÃƒÂ»
    cp += 2;
    wp = (uint16_t *)cp;
    *wp = iClientH;
    cp += 2;
    memcpy(cp, m_pClientList[iClientH]->m_cCharName, 10);
    cp += 10;
    wp = (uint16_t *)cp;
    *wp = m_pClientList[iClientH]->m_iPartyID;
    cp += 2;
    SendMsgToGateServer(MSGID_PARTYOPERATION, iClientH, cData);
}

void CGame::RequestDeletePartyHandler(int iClientH)
{
    char * cp, cData[120];
    uint32_t * dwp;
    uint16_t * wp;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_iPartyID != 0)
    {
        memset(cData, 0, sizeof(cData));
        cp = (char *)cData;
        dwp = (uint32_t *)cp;
        *dwp = MSGID_PARTYOPERATION;
        cp += 4;
        wp = (uint16_t *)cp;
        *wp = 4; // Â¸Ã¢Â¹Ã¶ ÃÂ¦Â°Ã… Â¿Ã¤ÃƒÂ»
        cp += 2;
        wp = (uint16_t *)cp;
        *wp = iClientH;
        cp += 2;
        memcpy(cp, m_pClientList[iClientH]->m_cCharName, 10);
        cp += 10;
        wp = (uint16_t *)cp;
        *wp = m_pClientList[iClientH]->m_iPartyID;
        cp += 2;
        SendMsgToGateServer(MSGID_PARTYOPERATION, iClientH, cData);
        // Â»Ã³Ã…Ã‚ ÂºÂ¯ÃˆÂ¯
        m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_PROCESSING;
    }
}

void CGame::RequestAcceptJoinPartyHandler(int iClientH, int iResult)
{
    char * cp, cData[120];
    uint32_t * dwp;
    uint16_t * wp;
    int iH;

    if (m_pClientList[iClientH] == nullptr) return;

    switch (iResult)
    {
        case 0: // Ã†Ã„Ã†Â¼ Â°Â¡Ã€Ã” Â½Ã…ÃƒÂ» Â°Ã…ÂºÃŽÂµÃ‡Â¾ÃºÃ€Â½Ã€Â» Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
            iH = m_pClientList[iClientH]->m_iReqJoinPartyClientH;
            if (m_pClientList[iH] == 0)
            {
                // Â°Â¡Ã€Ã”Ã€Â» Â½Ã…ÃƒÂ»Ã‡ÃŸÂ´Ã¸ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â°Â¡ Â¼Â­Â¹Ã¶Â»Ã³Â¿Â¡ Â¾Ã¸Â´Ã™.
                return;
            }
            if (strcmp(m_pClientList[iH]->m_cCharName, m_pClientList[iClientH]->m_cReqJoinPartyName) != 0)
            {
                // Â°Â¡Ã€Ã”Ã€Â» Â½Ã…ÃƒÂ»Ã‡ÃŸÂ´Ã¸ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â°Â¡ Â¼Â­Â¹Ã¶Â»Ã³Â¿Â¡ Â¾Ã¸Â´Ã™.
                return;
            }
            if (m_pClientList[iH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING)
            {
                // Â°Â¡Ã€Ã”Ã€Â» Â½Ã…ÃƒÂ»Ã‡ÃŸÂ´Ã¸ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Ã€Ã‡ Â»Ã³Ã…Ã‚Â°Â¡ ÂºÂ¯Â°Ã¦ÂµÃ‡Â¾ÃºÂ´Ã™.
                return;
            }
            if ((m_pClientList[iH]->m_iReqJoinPartyClientH != iClientH) || (strcmp(m_pClientList[iH]->m_cReqJoinPartyName, m_pClientList[iClientH]->m_cCharName) != 0))
            {
                // Â°Â¡Ã€Ã”Ã€Â» Â½Ã…ÃƒÂ»Ã‡ÃŸÂ´Ã¸ Â±Ã—Â¶Â§ Â±Ã— Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â°Â¡ Â¾Ã†Â´ÃÂ´Ã™.
                return;
            }

            SendNotifyMsg(0, iH, DEF_NOTIFY_PARTY, 7, 0, 0, 0);
            //testcode
            copy_string(G_cTxt, "Party join reject(3) ClientH:%d ID:%d", iH, m_pClientList[iH]->m_iPartyID);
            log->info(G_cTxt);

            m_pClientList[iH]->m_iPartyID = 0;
            m_pClientList[iH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
            m_pClientList[iH]->m_iReqJoinPartyClientH = 0;
            memset(m_pClientList[iH]->m_cReqJoinPartyName, 0, sizeof(m_pClientList[iH]->m_cReqJoinPartyName));

            m_pClientList[iClientH]->m_iReqJoinPartyClientH = 0;
            memset(m_pClientList[iClientH]->m_cReqJoinPartyName, 0, sizeof(m_pClientList[iClientH]->m_cReqJoinPartyName));
            break;

        case 1: // Ã†Ã„Ã†Â¼ Â°Â¡Ã€Ã” Â½Ã…ÃƒÂ» Â½Ã‚Ã€ÃŽ
            if ((m_pClientList[iClientH]->m_iPartyStatus == DEF_PARTYSTATUS_CONFIRM) && (m_pClientList[iClientH]->m_iPartyID != 0))
            {
                iH = m_pClientList[iClientH]->m_iReqJoinPartyClientH;
                if (m_pClientList[iH] == 0)
                {
                    // Â°Â¡Ã€Ã”Ã€Â» Â½Ã…ÃƒÂ»Ã‡ÃŸÂ´Ã¸ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â°Â¡ Â¼Â­Â¹Ã¶Â»Ã³Â¿Â¡ Â¾Ã¸Â´Ã™.
                    return;
                }
                if (strcmp(m_pClientList[iH]->m_cCharName, m_pClientList[iClientH]->m_cReqJoinPartyName) != 0)
                {
                    // Â°Â¡Ã€Ã”Ã€Â» Â½Ã…ÃƒÂ»Ã‡ÃŸÂ´Ã¸ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â°Â¡ Â¼Â­Â¹Ã¶Â»Ã³Â¿Â¡ Â¾Ã¸Â´Ã™.
                    return;
                }
                if (m_pClientList[iH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING)
                {
                    // Â°Â¡Ã€Ã”Ã€Â» Â½Ã…ÃƒÂ»Ã‡ÃŸÂ´Ã¸ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Ã€Ã‡ Â»Ã³Ã…Ã‚Â°Â¡ ÂºÂ¯Â°Ã¦ÂµÃ‡Â¾ÃºÂ´Ã™.
                    return;
                }
                if ((m_pClientList[iH]->m_iReqJoinPartyClientH != iClientH) || (strcmp(m_pClientList[iH]->m_cReqJoinPartyName, m_pClientList[iClientH]->m_cCharName) != 0))
                {
                    // Â°Â¡Ã€Ã”Ã€Â» Â½Ã…ÃƒÂ»Ã‡ÃŸÂ´Ã¸ Â±Ã—Â¶Â§ Â±Ã— Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â°Â¡ Â¾Ã†Â´ÃÂ´Ã™.
                    return;
                }

                // Â°Â¡Ã€Ã” ÃƒÂ³Â¸Â® Ã‡Ã‘Â´Ã™.
                memset(cData, 0, sizeof(cData));
                cp = (char *)cData;
                dwp = (uint32_t *)cp;
                *dwp = MSGID_PARTYOPERATION;
                cp += 4;
                wp = (uint16_t *)cp;
                *wp = 3; // Ã†Ã„Ã†Â¼ Â¸Ã¢Â¹Ã¶ ÃƒÃŸÂ°Â¡ Â¿Ã¤ÃƒÂ»
                cp += 2;
                wp = (uint16_t *)cp;
                *wp = m_pClientList[iClientH]->m_iReqJoinPartyClientH;
                cp += 2;
                memcpy(cp, m_pClientList[iClientH]->m_cReqJoinPartyName, 10);
                cp += 10;
                wp = (uint16_t *)cp;
                *wp = m_pClientList[iClientH]->m_iPartyID;
                cp += 2;
                SendMsgToGateServer(MSGID_PARTYOPERATION, iClientH, cData);
            }
            else
            {
                iH = m_pClientList[iClientH]->m_iReqJoinPartyClientH;
                if (m_pClientList[iH] == 0)
                {
                    // Â°Â¡Ã€Ã”Ã€Â» Â½Ã…ÃƒÂ»Ã‡ÃŸÂ´Ã¸ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â°Â¡ Â¼Â­Â¹Ã¶Â»Ã³Â¿Â¡ Â¾Ã¸Â´Ã™.
                    return;
                }
                if (strcmp(m_pClientList[iH]->m_cCharName, m_pClientList[iClientH]->m_cReqJoinPartyName) != 0)
                {
                    // Â°Â¡Ã€Ã”Ã€Â» Â½Ã…ÃƒÂ»Ã‡ÃŸÂ´Ã¸ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â°Â¡ Â¼Â­Â¹Ã¶Â»Ã³Â¿Â¡ Â¾Ã¸Â´Ã™.
                    return;
                }
                if (m_pClientList[iH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING)
                {
                    // Â°Â¡Ã€Ã”Ã€Â» Â½Ã…ÃƒÂ»Ã‡ÃŸÂ´Ã¸ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Ã€Ã‡ Â»Ã³Ã…Ã‚Â°Â¡ ÂºÂ¯Â°Ã¦ÂµÃ‡Â¾ÃºÂ´Ã™.
                    return;
                }
                if ((m_pClientList[iH]->m_iReqJoinPartyClientH != iClientH) || (strcmp(m_pClientList[iH]->m_cReqJoinPartyName, m_pClientList[iClientH]->m_cCharName) != 0))
                {
                    // Â°Â¡Ã€Ã”Ã€Â» Â½Ã…ÃƒÂ»Ã‡ÃŸÂ´Ã¸ Â±Ã—Â¶Â§ Â±Ã— Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â°Â¡ Â¾Ã†Â´ÃÂ´Ã™.
                    return;
                }

                if (m_pClientList[iClientH]->m_iPartyStatus == DEF_PARTYSTATUS_NULL)
                {
                    // Â½Ã…ÃƒÂ» Â¹ÃžÃ€Âº Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â°Â¡ Â¾Ã†Â¹Â«Â·Â± Ã†Ã„Ã†Â¼Â¿Â¡ÂµÂµ Â°Â¡Ã€Ã”Ã‡ÃÂ°Ã­ Ã€Ã–ÃÃ¶ Â¾ÃŠÂ´Ã™. Ã€ÃŒ Ã„Â³Â¸Â¯Ã…ÃÂ¸Â¦ Ã†Ã„Ã†Â¼ Â¸Â¶Â½ÂºÃ…ÃÂ·ÃŽ Â½Ã…ÃƒÂ»Ã‡Ã‘Â´Ã™.
                    RequestCreatePartyHandler(iClientH);
                }
                else
                {
                    // ÃƒÂ³Â¸Â® ÂºÃ’Â°Â¡
                }
            }
            break;

        case 2: // Ã†Ã„Ã†Â¼ Â°Â¡Ã€Ã” Â½Ã…ÃƒÂ» ÃƒÃ«Â¼Ã’
            // Ã€ÃŒ Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ Â¹ÃžÂ±Ã¢ Ã€Ã¼Â¿Â¡ Ã€ÃŒÂ¹ÃŒ Ã†Ã„Ã†Â¼Â¿Â¡ Â°Â¡Ã€Ã”ÂµÃ‡Â¾ÃºÂ´Ã™Â¸Ã© Ã…Â»Ã…Ã° ÃƒÂ³Â¸Â®Â¸Â¦ Ã‡Ã‘Â´Ã™.
            if ((m_pClientList[iClientH]->m_iPartyID != 0) && (m_pClientList[iClientH]->m_iPartyStatus == DEF_PARTYSTATUS_CONFIRM))
            {
                // Â¸Ã¢Â¹Ã¶ ÃÂ¦Â°Ã…Â¸Â¦ Â¿Ã¤ÃƒÂ»Ã‡Ã‘Â´Ã™.
                RequestDismissPartyHandler(iClientH);
            }
            else
            {
                // Â°Â¡Ã€Ã” Â½Ã…ÃƒÂ» Â»Ã³Ã…Ã‚Â¿Â´Â´Ã¸ Â°ÂªÂµÃ©Ã€Â» ÃƒÃŠÂ±Ã¢ÃˆÂ­ Ã‡Ã‘Â´Ã™.
                iH = m_pClientList[iClientH]->m_iReqJoinPartyClientH;

                // Â½Ã…ÃƒÂ»Ã‡ÃŸÂ´Ã¸ Ã„Â³Â¸Â¯Ã…ÃÂ°Â¡ ÃÂ¢Â¼Ã“Ã€Â» Â²Ã·Ã€Âº Â»Ã³Ã…Ã‚Â¶Ã³Â¸Ã© NULLÃ€Ã Â¼Ã¶ Ã€Ã–Â´Ã™.
                if ((m_pClientList[iH] != 0) && (m_pClientList[iH]->m_iReqJoinPartyClientH == iClientH) &&
                    (strcmp(m_pClientList[iH]->m_cReqJoinPartyName, m_pClientList[iClientH]->m_cCharName) == 0))
                {
                    // Â½Ã…ÃƒÂ»Ã‡ÃŸÂ´Ã¸ Ã„Â³Â¸Â¯Ã…ÃÃ€Ã‡ Ã†Ã„Ã†Â¼ Â¾Ã†Ã€ÃŒÂµÃ°Â¿Ã Â»Ã³Ã…Ã‚Â´Ã‚ Â°Ã‡ÂµÃ¥Â¸Â®ÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™.
                    m_pClientList[iH]->m_iReqJoinPartyClientH = 0;
                    memset(m_pClientList[iH]->m_cReqJoinPartyName, 0, sizeof(m_pClientList[iH]->m_cReqJoinPartyName));
                }

                m_pClientList[iClientH]->m_iPartyID = 0;
                m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
                m_pClientList[iClientH]->m_iReqJoinPartyClientH = 0;
                memset(m_pClientList[iClientH]->m_cReqJoinPartyName, 0, sizeof(m_pClientList[iClientH]->m_cReqJoinPartyName));
            }
            break;
    }
}

void CGame::RequestItemUpgradeHandler(int iClientH, int iItemIndex)
{
    int i, iItemX, iItemY, iSoM, iSoX, iSomH, iSoxH, iValue; // v2.172
    uint32_t dwTemp, dwSWEType;
    double dV1, dV2, dV3;
    short sItemUpgrade = 2;

    //hbest
    int bugint = 0;

    if (m_pClientList[iClientH] == nullptr) return;
    if ((iItemIndex < 0) || (iItemIndex >= DEF_MAXITEMS)) return;
    if (m_pClientList[iClientH]->m_pItemList[iItemIndex] == 0) return;

    iValue = (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
    if (iValue >= 15 || iValue < 0)
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 1, 0, 0, 0);
        return;
    }

    switch (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cCategory)
    {
        case 1: // weapons upgrade
            switch (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum)
            {
                case 703: // Â»Ã³Â¾Ã®Ã€Ã‡ Ã‡ÃƒÂ·Â¥Â¹Ã¶Â±Ã— 
                case 709: // DarkKnightFlameberge 
                case 718: // DarkKnightGreatSword
                case 727: // DarkKnightFlamebergW
                case 736: // Â»ÃµÂ·ÃŽÂ¿Ã® Â»Ã³Â¾Ã®Ã€Ã‡ Ã€ÃšÃ€ÃŒÂ¾Ã°Ã†Â®Â¼Ã’ÂµÃ¥
                case 737: // DarkKnightAxe
                case 745: // DarkKnightHammer
                    if (m_pClientList[iClientH]->m_iGizonItemUpgradeLeft <= 0)
                    {
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 3, 0, 0, 0); // Ã‡ÃŠÂ¿Ã¤Ã‡Ã‘ Â¾Ã†Ã€ÃŒÃ…Ã› Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ Â¼Ã¶Ã„Â¡Â°Â¡ Â¾Ã¸Â½Ã€Â´ÃÂ´Ã™.
                        return;
                    }
                    // Â¾Ã†Ã€ÃŒÃ…Ã› Â·Â¹ÂºÂ§Â¾Ã·Ã€ÃŒ Â³Ã´Â¾Ã†ÃÃºÂ¼Ã¶Â·Ã Â¾Ã†Ã€ÃŒÃ…Ã› Â·Â¹ÂºÂ§Â¾Ã·Ã‡ÃÂ±Ã¢Â°Â¡ Â¾Ã®Â·Ã†Â´Ã™.
                    // v2.15 ÃÃ¶ÃÂ¸ Â¾Ã†Ã€ÃŒÃ…Ã› Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ Â°Ã¸Â½Ã„ x(x+6)/8 +2 

                    sItemUpgrade = (iValue * (iValue + 6) / 8) + 2;

                    if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 != m_pClientList[iClientH]->m_sCharIDnum1) ||
                        (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 != m_pClientList[iClientH]->m_sCharIDnum2) ||
                        (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 != m_pClientList[iClientH]->m_sCharIDnum3))
                    {
                        if (iValue != 0)
                        {
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, 0, 0, 0); // Â¾Ã†Ã€ÃŒÃ…Ã› Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ ÂºÃ’Â°Â¡Â´Ã‰Ã‡Ã‘ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã”Â´ÃÂ´Ã™.
                            return;
                        }
                    }

                    if ((m_pClientList[iClientH]->m_iGizonItemUpgradeLeft - sItemUpgrade) < 0)
                    {
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 3, 0, 0, 0); // Ã‡ÃŠÂ¿Ã¤Ã‡Ã‘ Â¾Ã†Ã€ÃŒÃ…Ã› Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ Â¼Ã¶Ã„Â¡Â°Â¡ Â¾Ã¸Â½Ã€Â´ÃÂ´Ã™.
                        return;
                    }

                    m_pClientList[iClientH]->m_iGizonItemUpgradeLeft -= sItemUpgrade;

                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIZONITEMUPGRADELEFT, m_pClientList[iClientH]->m_iGizonItemUpgradeLeft, 0, 0, 0);

                    if ((iValue == 0) && m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum == 703)
                    {
                        iItemX = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x;
                        iItemY = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y;

                        // Â±Ã¢ÃÂ¸Ã€Ã‡ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â»Ã¨ÃÂ¦Ã‡Ã‘Â´Ã™.
                        delete m_pClientList[iClientH]->m_pItemList[iItemIndex];
                        m_pClientList[iClientH]->m_pItemList[iItemIndex] = 0;

                        m_pClientList[iClientH]->m_pItemList[iItemIndex] = new class CItem;
                        m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x = iItemX;
                        m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y = iItemY;

                        if (_bInitItemAttr(m_pClientList[iClientH]->m_pItemList[iItemIndex], 709) == false)
                        {
                            // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ Ã‡ÃÂ°Ã­Ã€Ãš Ã‡ÃÂ´Ã‚ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¾Ã†Ã€ÃŒÃ…Ã› Â¸Â®Â½ÂºÃ†Â®Â»Ã³Â¿Â¡ Â¾Ã¸Â´Ã™. Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥Â°Â¡ ÂºÃ’Â°Â¡Â´Ã‰Ã‡ÃÂ´Ã™.
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, 0, 0);
                            return;
                        }

                        // Â¾Ã†Ã€ÃŒÃ…Ã›Â¿Â¡ Â»Ã§Â¿Ã«Ã€Ãš Â°Ã­Ã€Â¯ Â¹Ã¸ÃˆÂ£Â¸Â¦ Ã€Ã”Â·Ã‚Ã‡Ã‘Â´Ã™. Â´Ã™Â¸Â¥ Ã„Â³Â¸Â¯Ã…ÃÂ´Ã‚ Ã€ÃŒ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â»Ã§Â¿Ã«Ã‡Ã’ Â¼Ã¶Â°Â¡ Â¾Ã¸Â´Ã™.
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;

                        iValue += 1;
                        if (iValue > 15) iValue = 15;
                        dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
                        dwTemp = dwTemp & 0x0FFFFFFF; // ÂºÃ±Ã†Â® Ã…Â¬Â¸Â®Â¾Ã® 
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28); // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ÂµÃˆ ÂºÃ±Ã†Â®Â°Âª Ã€Ã”Â·Ã‚

                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemType,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSprite,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSpriteFrame,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute);
                        _bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);
                        break;

                    }
                    else if ((iValue == 0) && ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum == 709) || (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum == 709)))
                    {

                        iItemX = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x;
                        iItemY = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y;

                        // Â±Ã¢ÃÂ¸Ã€Ã‡ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â»Ã¨ÃÂ¦Ã‡Ã‘Â´Ã™.
                        delete m_pClientList[iClientH]->m_pItemList[iItemIndex];
                        m_pClientList[iClientH]->m_pItemList[iItemIndex] = 0;

                        m_pClientList[iClientH]->m_pItemList[iItemIndex] = new class CItem;
                        m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x = iItemX;
                        m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y = iItemY;

                        if (_bInitItemAttr(m_pClientList[iClientH]->m_pItemList[iItemIndex], 709) == false)
                        {
                            // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ Ã‡ÃÂ°Ã­Ã€Ãš Ã‡ÃÂ´Ã‚ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¾Ã†Ã€ÃŒÃ…Ã› Â¸Â®Â½ÂºÃ†Â®Â»Ã³Â¿Â¡ Â¾Ã¸Â´Ã™. Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥Â°Â¡ ÂºÃ’Â°Â¡Â´Ã‰Ã‡ÃÂ´Ã™.
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, 0, 0);
                            return;
                        }

                        // Â¾Ã†Ã€ÃŒÃ…Ã›Â¿Â¡ Â»Ã§Â¿Ã«Ã€Ãš Â°Ã­Ã€Â¯ Â¹Ã¸ÃˆÂ£Â¸Â¦ Ã€Ã”Â·Ã‚Ã‡Ã‘Â´Ã™. Â´Ã™Â¸Â¥ Ã„Â³Â¸Â¯Ã…ÃÂ´Ã‚ Ã€ÃŒ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â»Ã§Â¿Ã«Ã‡Ã’ Â¼Ã¶Â°Â¡ Â¾Ã¸Â´Ã™.
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;

                        iValue += 1;
                        if (iValue > 15) iValue = 15;
                        dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
                        dwTemp = dwTemp & 0x0FFFFFFF; // ÂºÃ±Ã†Â® Ã…Â¬Â¸Â®Â¾Ã® 
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28); // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ÂµÃˆ ÂºÃ±Ã†Â®Â°Âª Ã€Ã”Â·Ã‚

                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemType,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSprite,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSpriteFrame,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute);

                        _bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);
                        break;
                    }
                    else if ((iValue == 0) && (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum == 745))
                    {

                        iItemX = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x;
                        iItemY = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y;

                        // Â±Ã¢ÃÂ¸Ã€Ã‡ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â»Ã¨ÃÂ¦Ã‡Ã‘Â´Ã™.
                        delete m_pClientList[iClientH]->m_pItemList[iItemIndex];
                        m_pClientList[iClientH]->m_pItemList[iItemIndex] = 0;

                        m_pClientList[iClientH]->m_pItemList[iItemIndex] = new class CItem;
                        m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x = iItemX;
                        m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y = iItemY;

                        if (_bInitItemAttr(m_pClientList[iClientH]->m_pItemList[iItemIndex], 745) == false)
                        {
                            // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ Ã‡ÃÂ°Ã­Ã€Ãš Ã‡ÃÂ´Ã‚ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¾Ã†Ã€ÃŒÃ…Ã› Â¸Â®Â½ÂºÃ†Â®Â»Ã³Â¿Â¡ Â¾Ã¸Â´Ã™. Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥Â°Â¡ ÂºÃ’Â°Â¡Â´Ã‰Ã‡ÃÂ´Ã™.
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, 0, 0);
                            return;
                        }

                        // Â¾Ã†Ã€ÃŒÃ…Ã›Â¿Â¡ Â»Ã§Â¿Ã«Ã€Ãš Â°Ã­Ã€Â¯ Â¹Ã¸ÃˆÂ£Â¸Â¦ Ã€Ã”Â·Ã‚Ã‡Ã‘Â´Ã™. Â´Ã™Â¸Â¥ Ã„Â³Â¸Â¯Ã…ÃÂ´Ã‚ Ã€ÃŒ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â»Ã§Â¿Ã«Ã‡Ã’ Â¼Ã¶Â°Â¡ Â¾Ã¸Â´Ã™.
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;

                        iValue += 1;
                        if (iValue > 15) iValue = 15;
                        dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
                        dwTemp = dwTemp & 0x0FFFFFFF; // ÂºÃ±Ã†Â® Ã…Â¬Â¸Â®Â¾Ã® 
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28); // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ÂµÃˆ ÂºÃ±Ã†Â®Â°Âª Ã€Ã”Â·Ã‚

                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemType,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSprite,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSpriteFrame,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute);

                        _bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);
                        break;
                    }
                    else if ((iValue == 0) && (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum == 737))
                    {

                        iItemX = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x;
                        iItemY = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y;

                        // Â±Ã¢ÃÂ¸Ã€Ã‡ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â»Ã¨ÃÂ¦Ã‡Ã‘Â´Ã™.
                        delete m_pClientList[iClientH]->m_pItemList[iItemIndex];
                        m_pClientList[iClientH]->m_pItemList[iItemIndex] = 0;

                        m_pClientList[iClientH]->m_pItemList[iItemIndex] = new class CItem;
                        m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x = iItemX;
                        m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y = iItemY;

                        if (_bInitItemAttr(m_pClientList[iClientH]->m_pItemList[iItemIndex], 737) == false)
                        {
                            // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ Ã‡ÃÂ°Ã­Ã€Ãš Ã‡ÃÂ´Ã‚ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¾Ã†Ã€ÃŒÃ…Ã› Â¸Â®Â½ÂºÃ†Â®Â»Ã³Â¿Â¡ Â¾Ã¸Â´Ã™. Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥Â°Â¡ ÂºÃ’Â°Â¡Â´Ã‰Ã‡ÃÂ´Ã™.
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, 0, 0);
                            return;
                        }

                        // Â¾Ã†Ã€ÃŒÃ…Ã›Â¿Â¡ Â»Ã§Â¿Ã«Ã€Ãš Â°Ã­Ã€Â¯ Â¹Ã¸ÃˆÂ£Â¸Â¦ Ã€Ã”Â·Ã‚Ã‡Ã‘Â´Ã™. Â´Ã™Â¸Â¥ Ã„Â³Â¸Â¯Ã…ÃÂ´Ã‚ Ã€ÃŒ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â»Ã§Â¿Ã«Ã‡Ã’ Â¼Ã¶Â°Â¡ Â¾Ã¸Â´Ã™.
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;

                        iValue += 1;
                        if (iValue > 15) iValue = 15;
                        dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
                        dwTemp = dwTemp & 0x0FFFFFFF; // ÂºÃ±Ã†Â® Ã…Â¬Â¸Â®Â¾Ã® 
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28); // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ÂµÃˆ ÂºÃ±Ã†Â®Â°Âª Ã€Ã”Â·Ã‚

                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemType,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSprite,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSpriteFrame,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute);

                        _bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);
                        break;
                    }
                    else
                    {
                        iValue += 1;
                        if (iValue > 15) iValue = 15;
                        dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
                        dwTemp = dwTemp & 0x0FFFFFFF; // ÂºÃ±Ã†Â® Ã…Â¬Â¸Â®Â¾Ã® 
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28); // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ÂµÃˆ ÂºÃ±Ã†Â®Â°Âª Ã€Ã”Â·Ã‚
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, 0, 0);
                        _bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);
                    }
                    break;

                default:

                    // v2.16 2002-5-21 Â°Ã­Â±Â¤Ã‡Ã¶Â¼Ã¶ÃÂ¤
                    // Â°Ã­Â´Ã«Ã€Ã‡~ ÃÂ¢ÂµÃŽÂ»Ã§Â°Â¡ ÂºÃ™Â´Ã‚ Â¹Â«Â±Ã¢Â´Ã‚ Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ ÂºÃ’Â°Â¡Â´Ã‰
                    if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00F00000) != 0)
                    {
                        dwSWEType = (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00F00000) >> 20;
                        if (dwSWEType == 9)
                        {
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, 0, 0, 0); // Â¾Ã†Ã€ÃŒÃ…Ã› Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ ÂºÃ’Â°Â¡Â´Ã‰Ã‡Ã‘ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã”Â´ÃÂ´Ã™.
                            return;
                        }
                    }
                    // Ã€ÃÂ¹Ã Â¹Â«Â±Ã¢ Â¾Ã†Ã€ÃŒÃ…Ã›
                    // v2.16 2002-5-21 Â°Ã­Â±Â¤Ã‡Ã¶Â¼Ã¶ÃÂ¤
                    iSoX = iSoM = 0;
                    for (i = 0; i < DEF_MAXITEMS; i++)
                        if (m_pClientList[iClientH]->m_pItemList[i] != 0)
                        {
                            switch (m_pClientList[iClientH]->m_pItemList[i]->m_sIDnum)
                            {
                                case 656: iSoX++; iSoxH = i; break; // Â½ÂºÃ…Ã¦ Â¿Ã€ÂºÃª ÃÂ¦Â¸Â®Â¸Â¶ 
                                case 657: iSoM++; iSomH = i; break; // Â½ÂºÃ…Ã¦ Â¿Ã€ÂºÃª Â¸ÃžÂ¸Â®Â¿Â£ 
                            }
                        }
                    // Â½ÂºÃ…Ã¦ Â¿Ã€ÂºÃª ÃÂ¦Â¸Â®Â¸Â¶Â°Â¡ Ã€Ã–Â´Ã™.
                    if (iSoX > 0)
                    {
                        if (bCheckIsItemUpgradeSuccess(iClientH, iItemIndex, iSoxH) == false)
                        {
                            // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ Â½Ã‡Ã†Ã 
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, 0, 0);
                            // Â½Ã‡Ã†ÃÃ‡Ã‘ Â¾Ã†Ã€ÃŒÃ…Ã› Â¾Ã¸Â¾Ã˜Â´Ã™.
                            iValue = (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0xF0000000) >> 28; // v2.172
                            if (iValue >= 1) ItemDepleteHandler(iClientH, iItemIndex, false); // v2.172 +1 -> +2 Â´ÃœÂ°Ã¨Â¿Â¡Â¼Â­ Â½Ã‡Ã†ÃÃ‡ÃÂ¸Ã© Â»Ã§Â¶Ã³ÃÃ¼ 
                            // Â½ÂºÃ…Ã¦ Â¿Ã€ÂºÃª ÃÂ¦Â¸Â®Â¸Â¶ Â¾Ã¸Â¾Ã˜Â´Ã™.
                            ItemDepleteHandler(iClientH, iSoxH, false);
                            return;
                        }

                        if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00000001) != 0)
                        {
                            // ÃÂ¦Ã€Ã›ÂµÃˆ Â¹Â«Â±Ã¢ Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ ÃƒÃ–Â´Ã« +10
                            iValue++;
                            if (iValue > 10)
                                iValue = 10;
                            else
                            {
                                // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ Â¼ÂºÂ°Ã¸. Â¾Ã†Ã€ÃŒÃ…Ã› Ã†Â¯Â¼Âº Â¹Ã™Â²Ã™Â°Ã­
                                dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
                                dwTemp = dwTemp & 0x0FFFFFFF; // ÂºÃ±Ã†Â® Ã…Â¬Â¸Â®Â¾Ã® 
                                m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28); // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ÂµÃˆ ÂºÃ±Ã†Â®Â°Âª Ã€Ã”Â·Ã‚	
                                // Â½ÂºÃ…Ã¦ Â¿Ã€ÂºÃª ÃÂ¦Â¸Â®Â¸Â¶ Â¾Ã¸Â¾Ã˜Â´Ã™.
                                ItemDepleteHandler(iClientH, iSoxH, false);
                            }
                        }
                        else
                        {
                            // Ã€ÃÂ¹Ã Â¹Â«Â±Ã¢ Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ ÃƒÃ–Â´Ã« +7
                            iValue++;
                            if (iValue > 7)
                                iValue = 7;
                            else
                            {
                                // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ Â¼ÂºÂ°Ã¸. Â¾Ã†Ã€ÃŒÃ…Ã› Ã†Â¯Â¼Âº Â¹Ã™Â²Ã™Â°Ã­
                                dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
                                dwTemp = dwTemp & 0x0FFFFFFF; // ÂºÃ±Ã†Â® Ã…Â¬Â¸Â®Â¾Ã® 
                                m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28); // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ÂµÃˆ ÂºÃ±Ã†Â®Â°Âª Ã€Ã”Â·Ã‚	
                                // Â½ÂºÃ…Ã¦ Â¿Ã€ÂºÃª ÃÂ¦Â¸Â®Â¸Â¶ Â¾Ã¸Â¾Ã˜Â´Ã™.
                                ItemDepleteHandler(iClientH, iSoxH, false);
                            }
                        }
                    }

                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, 0, 0);
                    break;
            }
            break;

        case 3: // ÃˆÂ° 
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, 0, 0);
            break;

        case 5: // Â¹Ã¦Ã†Ã
            // Â¸Ã°ÂµÃŽ Ã€ÃÂ¹Ã Â¾Ã†Ã€ÃŒÃ…Ã›
            // v2.16 2002-5-21 Â°Ã­Â±Â¤Ã‡Ã¶Â¼Ã¶ÃÂ¤
            // Â°Â­ÃˆÂ­ÂµÃˆ~ ÃÂ¢ÂµÃŽÂ»Ã§Â°Â¡ ÂºÃ™Â´Ã‚ Â¹Ã¦Â¾Ã®Â±Â¸Â´Ã‚ Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ ÂºÃ’Â°Â¡Â´Ã‰
            if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00F00000) != 0)
            {
                dwSWEType = (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00F00000) >> 20;
                if (dwSWEType == 8)
                {
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, 0, 0, 0); // Â¾Ã†Ã€ÃŒÃ…Ã› Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ ÂºÃ’Â°Â¡Â´Ã‰Ã‡Ã‘ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã”Â´ÃÂ´Ã™.
                    return;
                }
            }
            switch (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum)
            {
                case 620: // Â¸ÃžÂ¸Â®Â¿Â£-Â½Ã‡ÂµÃ¥
                case 623: // GM-Â½Ã‡ÂµÃ¥
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, 0, 0, 0); // Â¾Ã†Ã€ÃŒÃ…Ã› Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ ÂºÃ’Â°Â¡Â´Ã‰Ã‡Ã‘ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã”Â´ÃÂ´Ã™.
                    return;
                default: break;
            }


            iSoX = iSoM = 0;
            for (i = 0; i < DEF_MAXITEMS; i++)
                if (m_pClientList[iClientH]->m_pItemList[i] != 0)
                {
                    switch (m_pClientList[iClientH]->m_pItemList[i]->m_sIDnum)
                    {
                        case 656: iSoX++; iSoxH = i; break; // Â½ÂºÃ…Ã¦ Â¿Ã€ÂºÃª ÃÂ¦Â¸Â®Â¸Â¶ 
                        case 657: iSoM++; iSomH = i; break; // Â½ÂºÃ…Ã¦ Â¿Ã€ÂºÃª Â¸ÃžÂ¸Â®Â¿Â£ 
                    }
                }

            // Â½ÂºÃ…Ã¦ Â¿Ã€ÂºÃª Â¸ÃžÂ¸Â®Â¿Â£Ã€ÃŒ Ã€Ã–Â´Ã™.			
            if (iSoM > 0)
            {
                // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ Â¼ÂºÂ°Ã¸ ÃˆÂ®Â·Ã¼ Â°Ã¨Â»Ãª.
                if (bCheckIsItemUpgradeSuccess(iClientH, iItemIndex, iSomH, true) == false)
                {
                    // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ Â½Ã‡Ã†Ã 
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, 0, 0);
                    iValue = (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0xF0000000) >> 28; // v2.172
                    if (iValue >= 1) ItemDepleteHandler(iClientH, iItemIndex, false); // v2.172 +1 -> +2 Â´ÃœÂ°Ã¨Â¿Â¡Â¼Â­ Â½Ã‡Ã†ÃÃ‡ÃÂ¸Ã© Â»Ã§Â¶Ã³ÃÃ¼ 
                    // Â½ÂºÃ…Ã¦ Â¿Ã€ÂºÃª Â¸ÃžÂ¸Â®Â¿Â£ Â¾Ã¸Â¾Ã˜Â´Ã™.
                    ItemDepleteHandler(iClientH, iSomH, false);
                    return;
                }

                // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ Â¼ÂºÂ°Ã¸!
                iValue++;
                if (iValue > 10)
                    iValue = 10; // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ Ã‡Ã‘Â°Ã¨ 
                else
                {
                    dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
                    dwTemp = dwTemp & 0x0FFFFFFF; // ÂºÃ±Ã†Â® Ã…Â¬Â¸Â®Â¾Ã® 
                    m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28); // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ÂµÃˆ ÂºÃ±Ã†Â®Â°Âª Ã€Ã”Â·Ã‚	

                    if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00000001) != 0)
                    {
                        // ÃÂ¦Ã€Ã› Â¹Ã¦Â¾Ã®Â±Â¸ Ã€ÃŒÂ¹Ã‡Â·ÃŽ Â¼Ã¶Â¸Ã­ +20%
                        dV1 = (double)m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan;
                        dV2 = 0.2f * dV1;
                        dV3 = dV1 + dV2;
                    }
                    else
                    {
                        // Ã€ÃÂ¹Ã Â¹Ã¦Â¾Ã®Â±Â¸ Ã€ÃŒÂ¹Ã‡Â·ÃŽ Â¼Ã¶Â¸Ã­ +15%
                        dV1 = (double)m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan;
                        dV2 = 0.15f * dV1;
                        dV3 = dV1 + dV2;
                    }
                    m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1 = (short)dV3;
                    // Â¿Â¡Â·Â¯ Â¹Ã¦ÃÃ¶Â¿Ã« Ã„ÃšÂµÃ¥ 
                    if (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1 < 0)
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1 = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan;

                    m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1;
                    // Â½ÂºÃ…Ã¦ Â¿Ã€ÂºÃª Â¸ÃžÂ¸Â®Â¿Â£ Â¾Ã¸Â¾Ã˜Â´Ã™.
                    ItemDepleteHandler(iClientH, iSomH, false);
                }
            }
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1, 0, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2);
            break;

        case 6: // armors upgrade
            switch (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum)
            {
                case 621: // 
                case 622: // 

                case 700: // 
                case 701: // 
                case 702: //
                case 704: //  
                case 706: // 
                case 707:
                case 708:
                case 710:
                case 711: // 
                case 712: // 
                case 713: // 
                case 724: // 
                case 725:
                case 726:
                case 728:
                case 729: //
                case 730:
                case 731:
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, 0, 0, 0);
                    return;

                default:
                    if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00F00000) != 0)
                    {
                        dwSWEType = (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00F00000) >> 20;
                        if (dwSWEType == 8)
                        {
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, 0, 0, 0);
                            return;
                        }
                    }
                    iSoX = iSoM = 0;
                    for (i = 0; i < DEF_MAXITEMS; i++)
                        if (m_pClientList[iClientH]->m_pItemList[i] != 0)
                        {
                            switch (m_pClientList[iClientH]->m_pItemList[i]->m_sIDnum)
                            {
                                case 656: iSoX++; iSoxH = i; break;
                                case 657: iSoM++; iSomH = i; break;
                            }
                        }
                    if (iSoM > 0)
                    {
                        if (bCheckIsItemUpgradeSuccess(iClientH, iItemIndex, iSomH, true) == false)
                        {
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, 0, 0);
                            iValue = (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
                            if (iValue >= 1) ItemDepleteHandler(iClientH, iItemIndex, false);
                            ItemDepleteHandler(iClientH, iSomH, false);
                            return;
                        }
                        iValue++;
                        if (iValue > 10)
                            iValue = 10;
                        else
                        {
                            dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
                            dwTemp = dwTemp & 0x0FFFFFFF;
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28);

                            if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00000001) != 0)
                            {
                                dV1 = (double)m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan;
                                dV2 = 0.2f * dV1;
                                dV3 = dV1 + dV2;
                            }
                            else
                            {
                                dV1 = (double)m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan;
                                dV2 = 0.15f * dV1;
                                dV3 = dV1 + dV2;
                            }
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1 = (short)dV3;
                            if (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1 < 0)
                                m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1 = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan;

                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1;
                            ItemDepleteHandler(iClientH, iSomH, false);
                        }
                    }
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1, 0, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2);
                    break;
            }
            break;

        case 8: // wands upgrade 
            switch (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum)
            {
                case 291: // MagicWand(LLF)

                case 714: // ÃˆÃ¦Â¸Â¶Â¹Ã½Â»Ã§Ã€Ã‡ ÃÃ¶Ã†ÃŽÃ€ÃŒ 
                case 732: // ÃˆÃ¦Â¿Â©Â¸Â¶Â¹Ã½Â»Ã§Ã€Ã‡ ÃÃ¶Ã†ÃŽÃ€ÃŒ
                case 738: // ÃˆÃ¦Â¸Â¶Â¹Ã½Â»Ã§Ã€Ã‡ Â¸Ã…ÃÃ·Â¿Ã¸ÂµÃ¥
                case 746:

                    if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 != m_pClientList[iClientH]->m_sCharIDnum1) ||
                        (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 != m_pClientList[iClientH]->m_sCharIDnum2) ||
                        (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 != m_pClientList[iClientH]->m_sCharIDnum3))
                    {
                        if (iValue != 0)
                        {
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, 0, 0, 0); // Â¾Ã†Ã€ÃŒÃ…Ã› Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ ÂºÃ’Â°Â¡Â´Ã‰Ã‡Ã‘ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã”Â´ÃÂ´Ã™.
                            return;
                        }
                    }

                    if (m_pClientList[iClientH]->m_iGizonItemUpgradeLeft <= 0)
                    {
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 3, 0, 0, 0); // Ã‡ÃŠÂ¿Ã¤Ã‡Ã‘ Â¾Ã†Ã€ÃŒÃ…Ã› Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ Â¼Ã¶Ã„Â¡Â°Â¡ Â¾Ã¸Â½Ã€Â´ÃÂ´Ã™.
                        return;
                    }
                    // Â¾Ã†Ã€ÃŒÃ…Ã› Â·Â¹ÂºÂ§Â¾Ã·Ã€ÃŒ Â³Ã´Â¾Ã†ÃÃºÂ¼Ã¶Â·Ã Â¾Ã†Ã€ÃŒÃ…Ã› Â·Â¹ÂºÂ§Â¾Ã·Ã‡ÃÂ±Ã¢Â°Â¡ Â¾Ã®Â·Ã†Â´Ã™.
                    // v2.15 ÃÃ¶ÃÂ¸ Â¾Ã†Ã€ÃŒÃ…Ã› Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ Â°Ã¸Â½Ã„ x(x+6)/8 +2 
                    sItemUpgrade = (iValue * (iValue + 6) / 8) + 2;

                    if ((m_pClientList[iClientH]->m_iGizonItemUpgradeLeft - sItemUpgrade) < 0)
                    {
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 3, 0, 0, 0); // Ã‡ÃŠÂ¿Ã¤Ã‡Ã‘ Â¾Ã†Ã€ÃŒÃ…Ã› Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ Â¼Ã¶Ã„Â¡Â°Â¡ Â¾Ã¸Â½Ã€Â´ÃÂ´Ã™.
                        return;
                    }

                    m_pClientList[iClientH]->m_iGizonItemUpgradeLeft -= sItemUpgrade;
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIZONITEMUPGRADELEFT, m_pClientList[iClientH]->m_iGizonItemUpgradeLeft, 0, 0, 0);

                    if (iValue == 0)
                    {
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;
                    }

                    if ((iValue == 11) && ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum == 714) || (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum == 738)))
                    {
                        iItemX = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x;
                        iItemY = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y;

                        // Â±Ã¢ÃÂ¸Ã€Ã‡ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â»Ã¨ÃÂ¦Ã‡Ã‘Â´Ã™.
                        delete m_pClientList[iClientH]->m_pItemList[iItemIndex];
                        m_pClientList[iClientH]->m_pItemList[iItemIndex] = 0;

                        m_pClientList[iClientH]->m_pItemList[iItemIndex] = new class CItem;

                        m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x = iItemX;
                        m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y = iItemY;

                        if (_bInitItemAttr(m_pClientList[iClientH]->m_pItemList[iItemIndex], 738) == false)
                        {
                            // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ Ã‡ÃÂ°Ã­Ã€Ãš Ã‡ÃÂ´Ã‚ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¾Ã†Ã€ÃŒÃ…Ã› Â¸Â®Â½ÂºÃ†Â®Â»Ã³Â¿Â¡ Â¾Ã¸Â´Ã™. Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥Â°Â¡ ÂºÃ’Â°Â¡Â´Ã‰Ã‡ÃÂ´Ã™.
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, 0, 0);
                            return;
                        }

                        // Â¾Ã†Ã€ÃŒÃ…Ã›Â¿Â¡ Â»Ã§Â¿Ã«Ã€Ãš Â°Ã­Ã€Â¯ Â¹Ã¸ÃˆÂ£Â¸Â¦ Ã€Ã”Â·Ã‚Ã‡Ã‘Â´Ã™. Â´Ã™Â¸Â¥ Ã„Â³Â¸Â¯Ã…ÃÂ´Ã‚ Ã€ÃŒ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â»Ã§Â¿Ã«Ã‡Ã’ Â¼Ã¶Â°Â¡ Â¾Ã¸Â´Ã™.
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;

                        iValue += 1;
                        if (iValue > 15) iValue = 15;
                        dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
                        dwTemp = dwTemp & 0x0FFFFFFF; // ÂºÃ±Ã†Â® Ã…Â¬Â¸Â®Â¾Ã® 
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28); // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ÂµÃˆ ÂºÃ±Ã†Â®Â°Âª Ã€Ã”Â·Ã‚

                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemType,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSprite,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSpriteFrame,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute);
                        _bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);
                        break;


                    }
                    else if ((iValue == 15) && (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum == 738))
                    {
                        iItemX = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x;
                        iItemY = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y;

                        // Â±Ã¢ÃÂ¸Ã€Ã‡ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â»Ã¨ÃÂ¦Ã‡Ã‘Â´Ã™.
                        delete m_pClientList[iClientH]->m_pItemList[iItemIndex];
                        m_pClientList[iClientH]->m_pItemList[iItemIndex] = 0;

                        m_pClientList[iClientH]->m_pItemList[iItemIndex] = new class CItem;

                        m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x = iItemX;
                        m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y = iItemY;

                        if (_bInitItemAttr(m_pClientList[iClientH]->m_pItemList[iItemIndex], 746) == false)
                        {
                            // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ Ã‡ÃÂ°Ã­Ã€Ãš Ã‡ÃÂ´Ã‚ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¾Ã†Ã€ÃŒÃ…Ã› Â¸Â®Â½ÂºÃ†Â®Â»Ã³Â¿Â¡ Â¾Ã¸Â´Ã™. Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥Â°Â¡ ÂºÃ’Â°Â¡Â´Ã‰Ã‡ÃÂ´Ã™.
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, 0, 0);
                            return;
                        }

                        // Â¾Ã†Ã€ÃŒÃ…Ã›Â¿Â¡ Â»Ã§Â¿Ã«Ã€Ãš Â°Ã­Ã€Â¯ Â¹Ã¸ÃˆÂ£Â¸Â¦ Ã€Ã”Â·Ã‚Ã‡Ã‘Â´Ã™. Â´Ã™Â¸Â¥ Ã„Â³Â¸Â¯Ã…ÃÂ´Ã‚ Ã€ÃŒ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â»Ã§Â¿Ã«Ã‡Ã’ Â¼Ã¶Â°Â¡ Â¾Ã¸Â´Ã™.
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;

                        iValue += 1;
                        if (iValue > 15) iValue = 15;
                        dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
                        dwTemp = dwTemp & 0x0FFFFFFF; // ÂºÃ±Ã†Â® Ã…Â¬Â¸Â®Â¾Ã® 
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28); // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ÂµÃˆ ÂºÃ±Ã†Â®Â°Âª Ã€Ã”Â·Ã‚

                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemType,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSprite,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSpriteFrame,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute);
                        _bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);
                        break;


                    }
                    else if ((iValue == 15) && (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum == 746))
                    {
                        iItemX = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x;
                        iItemY = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y;

                        // Â±Ã¢ÃÂ¸Ã€Ã‡ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â»Ã¨ÃÂ¦Ã‡Ã‘Â´Ã™.
                        delete m_pClientList[iClientH]->m_pItemList[iItemIndex];
                        m_pClientList[iClientH]->m_pItemList[iItemIndex] = 0;

                        m_pClientList[iClientH]->m_pItemList[iItemIndex] = new class CItem;

                        m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x = iItemX;
                        m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y = iItemY;

                        if (_bInitItemAttr(m_pClientList[iClientH]->m_pItemList[iItemIndex], 892) == false)
                        {
                            // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ Ã‡ÃÂ°Ã­Ã€Ãš Ã‡ÃÂ´Ã‚ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¾Ã†Ã€ÃŒÃ…Ã› Â¸Â®Â½ÂºÃ†Â®Â»Ã³Â¿Â¡ Â¾Ã¸Â´Ã™. Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥Â°Â¡ ÂºÃ’Â°Â¡Â´Ã‰Ã‡ÃÂ´Ã™.
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, 0, 0);
                            return;
                        }

                        // Â¾Ã†Ã€ÃŒÃ…Ã›Â¿Â¡ Â»Ã§Â¿Ã«Ã€Ãš Â°Ã­Ã€Â¯ Â¹Ã¸ÃˆÂ£Â¸Â¦ Ã€Ã”Â·Ã‚Ã‡Ã‘Â´Ã™. Â´Ã™Â¸Â¥ Ã„Â³Â¸Â¯Ã…ÃÂ´Ã‚ Ã€ÃŒ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â»Ã§Â¿Ã«Ã‡Ã’ Â¼Ã¶Â°Â¡ Â¾Ã¸Â´Ã™.
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;

                        iValue += 1;
                        if (iValue > 15) iValue = 15;
                        dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
                        dwTemp = dwTemp & 0x0FFFFFFF; // ÂºÃ±Ã†Â® Ã…Â¬Â¸Â®Â¾Ã® 
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28); // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ÂµÃˆ ÂºÃ±Ã†Â®Â°Âª Ã€Ã”Â·Ã‚

                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemType,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSprite,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSpriteFrame,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute);
                        _bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);
                        break;


                    }
                    else
                    {
                        iValue += 1;
                        if (iValue > 15) iValue = 15;
                        dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
                        dwTemp = dwTemp & 0x0FFFFFFF; // ÂºÃ±Ã†Â® Ã…Â¬Â¸Â®Â¾Ã® 
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28); // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ÂµÃˆ ÂºÃ±Ã†Â®Â°Âª Ã€Ã”Â·Ã‚
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, 0, 0);
                        _bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);
                        break;
                    }

                default:
                    // Ã€ÃÂ¹Ã Â¾Ã†Ã€ÃŒÃ…Ã›
                    // v2.16 2002-5-21 Â°Ã­Â±Â¤Ã‡Ã¶Â¼Ã¶ÃÂ¤
                    iSoX = iSoM = 0;
                    for (i = 0; i < DEF_MAXITEMS; i++)
                        if (m_pClientList[iClientH]->m_pItemList[i] != 0)
                        {
                            switch (m_pClientList[iClientH]->m_pItemList[i]->m_sIDnum)
                            {
                                case 656: iSoX++; iSoxH = i; break; // Â½ÂºÃ…Ã¦ Â¿Ã€ÂºÃª ÃÂ¦Â¸Â®Â¸Â¶ 
                                case 657: iSoM++; iSomH = i; break; // Â½ÂºÃ…Ã¦ Â¿Ã€ÂºÃª Â¸ÃžÂ¸Â®Â¿Â£ 
                            }
                        }
                    // Â½ÂºÃ…Ã¦ Â¿Ã€ÂºÃª ÃÂ¦Â¸Â®Â¸Â¶Â°Â¡ Ã€Ã–Â´Ã™.
                    if (iSoX > 0)
                    {
                        // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ Â¼ÂºÂ°Ã¸ ÃˆÂ®Â·Ã¼ Â°Ã¨Â»Ãª.
                        if (bCheckIsItemUpgradeSuccess(iClientH, iItemIndex, iSoxH) == false)
                        {
                            // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ Â½Ã‡Ã†Ã 
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, 0, 0);
                            iValue = (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0xF0000000) >> 28; // v2.172
                            if (iValue >= 1) ItemDepleteHandler(iClientH, iItemIndex, false); // v2.172 +1 -> +2 Â´ÃœÂ°Ã¨Â¿Â¡Â¼Â­ Â½Ã‡Ã†ÃÃ‡ÃÂ¸Ã© Â»Ã§Â¶Ã³ÃÃ¼ 
                            // Â½ÂºÃ…Ã¦ Â¿Ã€ÂºÃª ÃÂ¦Â¸Â®Â¸Â¶ Â¾Ã¸Â¾Ã˜Â´Ã™.
                            ItemDepleteHandler(iClientH, iSoxH, false);
                            return;
                        }

                        iValue++;
                        if (iValue > 7)
                            iValue = 7;
                        else
                        {
                            // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ Â¼ÂºÂ°Ã¸. Â¾Ã†Ã€ÃŒÃ…Ã› Ã†Â¯Â¼Âº Â¹Ã™Â²Ã™Â°Ã­
                            dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
                            dwTemp = dwTemp & 0x0FFFFFFF; // ÂºÃ±Ã†Â® Ã…Â¬Â¸Â®Â¾Ã® 
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28); // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ÂµÃˆ ÂºÃ±Ã†Â®Â°Âª Ã€Ã”Â·Ã‚	
                            // Â½ÂºÃ…Ã¦ Â¿Ã€ÂºÃª ÃÂ¦Â¸Â®Â¸Â¶ Â¾Ã¸Â¾Ã˜Â´Ã™.
                            ItemDepleteHandler(iClientH, iSoxH, false);
                        }
                    }

                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, 0, 0);

                    break;
            }
            break;

            //hbest hero cape upgrade
        case 13:
            switch (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum)
            {
                case 400:
                case 401:
                    iSoX = iSoM = 0;
                    for (i = 0; i < DEF_MAXITEMS; i++)
                        if (m_pClientList[iClientH]->m_pItemList[i] != 0)
                        {
                            switch (m_pClientList[iClientH]->m_pItemList[i]->m_sIDnum)
                            {
                                case 656: iSoX++; iSoxH = i; break; // Â½ÂºÃ…Ã¦ Â¿Ã€ÂºÃª ÃÂ¦Â¸Â®Â¸Â¶ 
                                case 657: iSoM++; iSomH = i; break; // Â½ÂºÃ…Ã¦ Â¿Ã€ÂºÃª Â¸ÃžÂ¸Â®Â¿Â£ 
                            }
                        }

                    if (iSoM < 1)
                    {
                        return;
                    }

                    bugint = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum;
                    if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 != m_pClientList[iClientH]->m_sCharIDnum1) ||
                        (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 != m_pClientList[iClientH]->m_sCharIDnum2) ||
                        (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 != m_pClientList[iClientH]->m_sCharIDnum3))
                    {
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, 0, 0, 0); // Â¾Ã†Ã€ÃŒÃ…Ã› Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ ÂºÃ’Â°Â¡Â´Ã‰Ã‡Ã‘ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã”Â´ÃÂ´Ã™.
                        return;
                    }

                    if ((m_pClientList[iClientH]->m_iContribution < 50) || (m_pClientList[iClientH]->m_iEnemyKillCount < 50))
                    {
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 3, 0, 0, 0); // Ã‡ÃŠÂ¿Ã¤Ã‡Ã‘ Â¾Ã†Ã€ÃŒÃ…Ã› Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ Â¼Ã¶Ã„Â¡Â°Â¡ Â¾Ã¸Â½Ã€Â´ÃÂ´Ã™.
                        return;
                    }

                    m_pClientList[iClientH]->m_iContribution -= 50;
                    m_pClientList[iClientH]->m_iEnemyKillCount -= 50;
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_ENEMYKILLS, m_pClientList[iClientH]->m_iEnemyKillCount, 0, 0, 0);

                    if (iValue == 0)
                    {
                        iItemX = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x;
                        iItemY = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y;

                        delete m_pClientList[iClientH]->m_pItemList[iItemIndex];
                        m_pClientList[iClientH]->m_pItemList[iItemIndex] = 0;

                        m_pClientList[iClientH]->m_pItemList[iItemIndex] = new class CItem;

                        m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x = iItemX;
                        m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y = iItemY;

                        if (bugint == 400)
                        {
                            if (_bInitItemAttr(m_pClientList[iClientH]->m_pItemList[iItemIndex], 427) == false)
                            {
                                SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, 0, 0);
                                return;
                            }
                        }
                        else
                        {
                            if (_bInitItemAttr(m_pClientList[iClientH]->m_pItemList[iItemIndex], 428) == false)
                            {
                                SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, 0, 0);
                                return;
                            }
                        }

                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;

                        ItemDepleteHandler(iClientH, iSomH, false);

                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemType,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSprite,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSpriteFrame,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2,
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute);
                        _bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);
                        break;


                    }

                default: break;
            }
            break;

        default:
            // Â¾Ã·Â±Ã—Â·Â¹Ã€ÃŒÂµÃ¥ ÂµÃˆ Â°Ã Â¾Ã¸Ã€Â½.
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, 0, 0);
            break;
    }
}

void CGame::MeteorStrikeMsgHandler(char cAttackerSide)
{
    int i;
    uint32_t dwTime = timeGetTime();

    switch (cAttackerSide)
    {
        case 1:
            if (m_iElvineMapIndex != -1)
            {
                for (i = 1; i < DEF_MAXCLIENTS; i++)
                    if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_bIsInitComplete == true))
                    {
                        if (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cLocationName, "elvine") == 0)
                        {
                            SendNotifyMsg(0, i, DEF_NOTIFY_METEORSTRIKECOMING, 1, 0, 0, 0);
                        }
                        else
                        {
                            SendNotifyMsg(0, i, DEF_NOTIFY_METEORSTRIKECOMING, 2, 0, 0, 0);
                        }
                    }
                bRegisterDelayEvent(DEF_DELAYEVENTTYPE_METEORSTRIKE, 0, dwTime + 5000, 0, 0, m_iElvineMapIndex, 0, 0, 0, 0, 0);
            }
            else
            {
                for (i = 1; i < DEF_MAXCLIENTS; i++)
                    if (m_pClientList[i] != 0)
                    {
                        SendNotifyMsg(0, i, DEF_NOTIFY_METEORSTRIKECOMING, 2, 0, 0, 0);
                    }
            }
            break;

        case 2:
            if (m_iAresdenMapIndex != -1)
            {
                for (i = 1; i < DEF_MAXCLIENTS; i++)
                    if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_bIsInitComplete == true))
                    {
                        if (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cLocationName, "aresden") == 0)
                        {
                            SendNotifyMsg(0, i, DEF_NOTIFY_METEORSTRIKECOMING, 3, 0, 0, 0);
                        }
                        else
                        {
                            SendNotifyMsg(0, i, DEF_NOTIFY_METEORSTRIKECOMING, 4, 0, 0, 0);
                        }
                    }
                bRegisterDelayEvent(DEF_DELAYEVENTTYPE_METEORSTRIKE, 0, dwTime + 1000 * 5, 0, 0, m_iAresdenMapIndex, 0, 0, 0, 0, 0);
            }
            else
            {
                for (i = 1; i < DEF_MAXCLIENTS; i++)
                    if (m_pClientList[i] != 0)
                    {
                        SendNotifyMsg(0, i, DEF_NOTIFY_METEORSTRIKECOMING, 4, 0, 0, 0);
                    }
            }
            break;
    }
}

void CGame::ClientMotionHandler(int iClientH, char * pData)
{
    uint32_t * dwp, dwClientTime;
    uint16_t * wp, wCommand, wTargetObjectID;
    short * sp, sX, sY, dX, dY, wType;
    char * cp, cDir;
    int   iRet, iTemp;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    if (m_pClientList[iClientH]->m_bIsKilled == true) return;

    /*m_pClientList[iClientH]->m_cConnectionCheck++;
    if (m_pClientList[iClientH]->m_cConnectionCheck > 50) {
        copy_string(G_cTxt, "Hex: (%s) Player: (%s) - removed 03203203h, vital to hack detection.", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
        PutHackLogFileList(G_cTxt);
        DeleteClient(iClientH, true, true);
        return;
    }*/
    wp = (uint16_t *)(pData + DEF_INDEX2_MSGTYPE);
    wCommand = *wp;

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

    sp = (short *)cp;
    sX = *sp;
    cp += 2;

    sp = (short *)cp;
    sY = *sp;
    cp += 2;

    cDir = *cp;
    cp++;

    sp = (short *)cp;
    dX = *sp;
    cp += 2;

    sp = (short *)cp;
    dY = *sp;
    cp += 2;

    sp = (short *)cp;
    wType = *sp;
    cp += 2;

    if ((wCommand == DEF_OBJECTATTACK) || (wCommand == DEF_OBJECTATTACKMOVE))
    { // v1.4
        wp = (uint16_t *)cp;
        wTargetObjectID = *wp;
        cp += 2;
    }

    // v2.171
    dwp = (uint32_t *)cp;
    dwClientTime = *dwp;
    cp += 4;

    switch (wCommand)
    {
        case DEF_OBJECTSTOP:
            iRet = iClientMotion_Stop_Handler(iClientH, sX, sY, cDir);
            if (iRet == 1)
            {
                SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTSTOP, 0, 0, 0);
            }
            else if (iRet == 2) SendObjectMotionRejectMsg(iClientH);
            break;

        case DEF_OBJECTRUN:
            iRet = iClientMotion_Move_Handler(iClientH, sX, sY, cDir, 1);
            if (iRet == 1)
            {
                SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTRUN, 0, 0, 0);
            }
            if ((m_pClientList[iClientH] != 0) && (m_pClientList[iClientH]->m_iHP <= 0)) ClientKilledHandler(iClientH, 0, 0, 1); // v1.4
            // v2.171
            bCheckClientMoveFrequency(iClientH, dwClientTime);
            break;

        case DEF_OBJECTMOVE:
            iRet = iClientMotion_Move_Handler(iClientH, sX, sY, cDir, 2);
            if (iRet == 1)
            {
                SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTMOVE, 0, 0, 0);
            }
            if ((m_pClientList[iClientH] != 0) && (m_pClientList[iClientH]->m_iHP <= 0)) ClientKilledHandler(iClientH, 0, 0, 1); // v1.4
            // v2.171
            bCheckClientMoveFrequency(iClientH, dwClientTime);
            break;

        case DEF_OBJECTDAMAGEMOVE:
            iRet = iClientMotion_Move_Handler(iClientH, sX, sY, cDir, 0);
            if (iRet == 1)
            {
                SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGEMOVE, m_pClientList[iClientH]->m_iLastDamage, 0, 0);
            }
            if ((m_pClientList[iClientH] != 0) && (m_pClientList[iClientH]->m_iHP <= 0)) ClientKilledHandler(iClientH, 0, 0, 1); // v1.4
            break;

        case DEF_OBJECTATTACKMOVE:
            iRet = iClientMotion_Move_Handler(iClientH, sX, sY, cDir, 0);
            if ((iRet == 1) && (m_pClientList[iClientH] != 0))
            {
                SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTATTACKMOVE, 0, 0, 0);
                iClientMotion_Attack_Handler(iClientH, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, dX, dY, wType, cDir, wTargetObjectID, false, true); // v1.4
            }
            if ((m_pClientList[iClientH] != 0) && (m_pClientList[iClientH]->m_iHP <= 0)) ClientKilledHandler(iClientH, 0, 0, 1); // v1.4
            // v2.171
            bCheckClientAttackFrequency(iClientH, dwClientTime);
            break;

        case DEF_OBJECTATTACK:
            _CheckAttackType(iClientH, &wType);
            iRet = iClientMotion_Attack_Handler(iClientH, sX, sY, dX, dY, wType, cDir, wTargetObjectID); // v1.4
            if (iRet == 1)
            {
                if (wType >= 20)
                {
                    m_pClientList[iClientH]->m_iSuperAttackLeft--;
                    if (m_pClientList[iClientH]->m_iSuperAttackLeft < 0) m_pClientList[iClientH]->m_iSuperAttackLeft = 0;
                }

                SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTATTACK, dX, dY, wType);
            }
            else if (iRet == 2) SendObjectMotionRejectMsg(iClientH);
            // v2.171
            bCheckClientAttackFrequency(iClientH, dwClientTime);
            break;

        case DEF_OBJECTGETITEM:
            iRet = iClientMotion_GetItem_Handler(iClientH, sX, sY, cDir);
            if (iRet == 1)
            {
                SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTGETITEM, 0, 0, 0);
            }
            else if (iRet == 2) SendObjectMotionRejectMsg(iClientH);
            break;

        case DEF_OBJECTMAGIC:
            iRet = iClientMotion_Magic_Handler(iClientH, sX, sY, cDir);
            //client hp recorded here ONLY if its less than
            if (iRet == 1)
            {
                if (m_pClientList[iClientH]->m_bMagicPauseTime == false)
                {
                    m_pClientList[iClientH]->m_bMagicPauseTime = true;
                    iTemp = 10;
                    SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTMAGIC, dX, iTemp, 0);
                    m_pClientList[iClientH]->m_iSpellCount++;
                    bCheckClientMagicFrequency(iClientH, dwClientTime);
                }
                else if (m_pClientList[iClientH]->m_bMagicPauseTime == true)
                {
                    try
                    {
                        copy_string(G_cTxt, "Cast Delay Hack: (%s) Player: (%s) - player casting too fast.", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
                        PutHackLogFileList(G_cTxt);
                        DeleteClient(iClientH, true, true);
                    }
                    catch (...)
                    {
                    }
                }
            }
            else if (iRet == 2) SendObjectMotionRejectMsg(iClientH);
            break;

        default:
            break;
    }
}

/////////////////////////////////////////////////////////////////////////////////////
//  int CGame::iClientMotion_Move_Handler(int iClientH, short sX, short sY, char cDir, char cMoveType)
//  description			:: Handles how player or npc run, walk, attack, or get flown by attack
//  last updated		:: October 30, 2004; 1:52 AM; Hypnotoad
//	return value		:: int
//  commentary			:: - contains speed hack detection previously unavailable
//						   - changed variable 5 to char, prior bool.
//								1 = object run
//								2 = object malk
//								0 = object damage move, object attack move
//						   - fixed bump bug removing aura
/////////////////////////////////////////////////////////////////////////////////////
// Missing 3.51: 
//			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->3CA18h
// 			bRet = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->sub_4C0F20(dX, dY, cTemp, wV1, wV2);
//
/////////////////////////////////////////////////////////////////////////////////////
int CGame::iClientMotion_Move_Handler(int iClientH, short sX, short sY, char cDir, char cMoveType)
{
    char * cp, cData[3000];
    class CTile * pTile;
    uint32_t * dwp, dwTime;
    uint16_t * wp, wObjectID;
    short * sp, dX, dY, sTemp, sTemp2, sDOtype, pTopItem;
    int * ip, iRet, iSize, iDamage;
    bool  bRet, bIsBlocked = false;

    if (m_pClientList[iClientH] == nullptr) return 0;
    if ((cDir <= 0) || (cDir > 8))       return 0;
    if (m_pClientList[iClientH]->m_bIsKilled == true) return 0;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return 0;

    if ((sX != m_pClientList[iClientH]->m_sX) || (sY != m_pClientList[iClientH]->m_sY)) return 2;

    //locobans
    dwTime = timeGetTime();
    /*m_pClientList[iClientH]->m_dwLastActionTime = dwTime;
    if (cMoveType == 2) {
        if (m_pClientList[iClientH]->m_iRecentWalkTime > dwTime) {
            m_pClientList[iClientH]->m_iRecentWalkTime = dwTime;
            if (m_pClientList[iClientH]->m_sV1 < 1) {
                if (m_pClientList[iClientH]->m_iRecentWalkTime < dwTime) {
                    m_pClientList[iClientH]->m_sV1++;
                }
                else {
                    bIsBlocked = true;
                    m_pClientList[iClientH]->m_sV1 = 0;
                }
            }
        m_pClientList[iClientH]->m_iRecentWalkTime = dwTime;
        }
        if (bIsBlocked == false) m_pClientList[iClientH]->m_iMoveMsgRecvCount++;
        if (m_pClientList[iClientH]->m_iMoveMsgRecvCount >= 3) {
            if (m_pClientList[iClientH]->m_dwMoveLAT != 0) {
                if ((dwTime - m_pClientList[iClientH]->m_dwMoveLAT) < (590)) {
                    //copy_string(G_cTxt, "3.51 Walk Speeder: (%s) Player: (%s) walk difference: %d. Speed Hack?", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName, dwTime - m_pClientList[iClientH]->m_dwMoveLAT);
                    //PutHackLogFileList(G_cTxt);
                    bIsBlocked = true;
                }
            }
            m_pClientList[iClientH]->m_dwMoveLAT = dwTime;
            m_pClientList[iClientH]->m_iMoveMsgRecvCount = 0;
        }
    }
    else if (cMoveType == 1) {
        if (m_pClientList[iClientH]->m_iRecentRunTime > dwTime) {
            m_pClientList[iClientH]->m_iRecentRunTime = dwTime;
            if (m_pClientList[iClientH]->m_sV1 < 1) {
                if (m_pClientList[iClientH]->m_iRecentRunTime < dwTime) {
                    m_pClientList[iClientH]->m_sV1++;
                }
                else {
                    bIsBlocked = true;
                    m_pClientList[iClientH]->m_sV1 = 0;
                }
            }
        m_pClientList[iClientH]->m_iRecentRunTime = dwTime;
        }
        if (bIsBlocked == false) m_pClientList[iClientH]->m_iRunMsgRecvCount++;
        if (m_pClientList[iClientH]->m_iRunMsgRecvCount >= 3) {
            if (m_pClientList[iClientH]->m_dwRunLAT != 0) {
                if ((dwTime - m_pClientList[iClientH]->m_dwRunLAT) < (290)) {
                    //copy_string(G_cTxt, "3.51 Run Speeder: (%s) Player: (%s) run difference: %d. Speed Hack?", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName, dwTime - m_pClientList[iClientH]->m_dwRunLAT);
                    //PutHackLogFileList(G_cTxt);
                    bIsBlocked = true;
                }
            }
            m_pClientList[iClientH]->m_dwRunLAT	= dwTime;
            m_pClientList[iClientH]->m_iRunMsgRecvCount = 0;
        }
    }*/

    int iStX, iStY;
    if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex] != 0)
    {
        iStX = m_pClientList[iClientH]->m_sX / 20;
        iStY = m_pClientList[iClientH]->m_sY / 20;
        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iPlayerActivity++;

        switch (m_pClientList[iClientH]->m_cSide)
        {
            case 0: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iNeutralActivity++; break;
            case 1: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iAresdenActivity++; break;
            case 2: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iElvineActivity++;  break;
        }
    }

    ClearSkillUsingStatus(iClientH);

    dX = m_pClientList[iClientH]->m_sX;
    dY = m_pClientList[iClientH]->m_sY;

    switch (cDir)
    {
        case 1:	dY--; break;
        case 2:	dX++; dY--;	break;
        case 3:	dX++; break;
        case 4:	dX++; dY++;	break;
        case 5: dY++; break;
        case 6:	dX--; dY++;	break;
        case 7:	dX--; break;
        case 8:	dX--; dY--;	break;
    }

    pTopItem = 0;
    bRet = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bGetMoveable(dX, dY, &sDOtype, &pTopItem);

    if (m_pClientList[iClientH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0)
        bRet = false;

    if ((bRet == true) && (bIsBlocked == false))
    {
        if (m_pClientList[iClientH]->m_iQuest != 0) _bCheckIsQuestCompleted(iClientH);

        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->ClearOwner(1, iClientH, DEF_OWNERTYPE_PLAYER, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY);

        m_pClientList[iClientH]->m_sX = dX;
        m_pClientList[iClientH]->m_sY = dY;
        m_pClientList[iClientH]->m_cDir = cDir;

        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetOwner((short)iClientH,
            DEF_OWNERTYPE_PLAYER,
            dX, dY);

        if (sDOtype == DEF_DYNAMICOBJECT_SPIKE)
        {
            if ((m_pClientList[iClientH]->m_bIsNeutral == true) && ((m_pClientList[iClientH]->m_sAppr2 & 0xF000) == 0))
            {

            }
            else
            {
                iDamage = iDice(2, 4);

                if (m_pClientList[iClientH]->m_iAdminUserLevel == 0)
                    m_pClientList[iClientH]->m_iHP -= iDamage;
            }
        }

        if (m_pClientList[iClientH]->m_iHP <= 0) m_pClientList[iClientH]->m_iHP = 0;

        dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
        *dwp = MSGID_RESPONSE_MOTION;
        wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
        *wp = DEF_OBJECTMOVE_CONFIRM;

        cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);

        sp = (short *)cp;
        *sp = (short)(dX - 10);
        cp += 2;

        sp = (short *)cp;
        *sp = (short)(dY - 7);
        cp += 2;

        *cp = cDir;
        cp++;

        if (cMoveType == 1)
        {
            if (m_pClientList[iClientH]->m_iSP > 0)
            {
                *cp = 0;
                if (m_pClientList[iClientH]->m_iTimeLeft_FirmStaminar == 0)
                {
                    m_pClientList[iClientH]->m_iSP--;
                    *cp = 1;
                }
            }
            else
            {
                *cp = 0;
                if (m_pClientList[iClientH]->m_iTimeLeft_FirmStaminar == 0)
                {
                    m_pClientList[iClientH]->m_iSP--;
                    *cp = 1;
                }
                if (m_pClientList[iClientH]->m_iSP < -10)
                {
                    m_pClientList[iClientH]->m_iSP = 0;
                    DeleteClient(iClientH, true, true);
                    return 0;
                }
            }
        }
        else *cp = 0;
        cp++;

        pTile = (class CTile *)(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_pTile + dX + dY * m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_sSizeY);
        *cp = (char)pTile->m_iOccupyStatus;
        cp++;

        ip = (int *)cp;
        *ip = m_pClientList[iClientH]->m_iHP;
        cp += 4;

        iSize = iComposeMoveMapData((short)(dX - 10), (short)(dY - 7), iClientH, cDir, cp);
        iRet = m_pClientList[iClientH]->send_msg(cData, iSize + 12 + 1 + 4);
        switch (iRet)
        {
            case DEF_XSOCKEVENT_QUENEFULL:
            case DEF_XSOCKEVENT_SOCKETERROR:
            case DEF_XSOCKEVENT_CRITICALERROR:
            case DEF_XSOCKEVENT_SOCKETCLOSED:
                DeleteClient(iClientH, true, true);
                return 0;
        }
        /*if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->3CA18h == true) {

            .text:00406037                 mov     [ebp+var_C1C], 0
            .text:0040603E                 xor     edx, edx
            .text:00406040                 mov     [ebp+var_C1B], edx
            .text:00406046                 mov     [ebp+var_C17], edx
            .text:0040604C                 mov     [ebp+var_C13], dx

            bRet = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->sub_4C0F20(dX, dY, cTemp, wV1, wV2);
            if (bRet == 1) {
                RequestTeleportHandler(iClientH, "2   ", cTemp, wV1, wV2);
            }
        }*/
    }
    else
    {
        m_pClientList[iClientH]->m_bIsMoveBlocked = true;

        dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
        *dwp = MSGID_RESPONSE_MOTION;
        wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
        *wp = DEF_OBJECTMOVE_REJECT;
        if (bIsBlocked == true)
        {
            m_pClientList[iClientH]->m_dwAttackLAT = 1050;
        }
        m_pClientList[iClientH]->m_dwAttackLAT = 1010;

        wObjectID = (uint16_t)iClientH;

        cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);

        wp = (uint16_t *)cp;
        *wp = wObjectID;
        cp += 2;

        sp = (short *)cp;
        sX = m_pClientList[wObjectID]->m_sX;
        *sp = sX;
        cp += 2;

        sp = (short *)cp;
        sY = m_pClientList[wObjectID]->m_sY;
        *sp = sY;
        cp += 2;

        sp = (short *)cp;
        *sp = m_pClientList[wObjectID]->m_sType;
        cp += 2;

        *cp = m_pClientList[wObjectID]->m_cDir;
        cp++;

        memcpy(cp, m_pClientList[wObjectID]->m_cCharName, 10);
        cp += 10;

        sp = (short *)cp;
        *sp = m_pClientList[wObjectID]->m_sAppr1;
        cp += 2;

        sp = (short *)cp;
        *sp = m_pClientList[wObjectID]->m_sAppr2;
        cp += 2;

        sp = (short *)cp;
        *sp = m_pClientList[wObjectID]->m_sAppr3;
        cp += 2;

        sp = (short *)cp;
        *sp = m_pClientList[wObjectID]->m_sAppr4;
        cp += 2;

        ip = (int *)cp;
        *ip = m_pClientList[wObjectID]->m_iApprColor;
        cp += 4;

        ip = (int *)cp;
        sTemp = m_pClientList[wObjectID]->m_iStatus;
        sTemp = 0x0FFFFFFF & sTemp;
        sTemp2 = iGetPlayerABSStatus(wObjectID, iClientH);
        sTemp = (sTemp | (sTemp2 << 28));
        *ip = sTemp;
        cp += 4;

        iRet = m_pClientList[iClientH]->send_msg(cData, 42);

        switch (iRet)
        {
            case DEF_XSOCKEVENT_QUENEFULL:
            case DEF_XSOCKEVENT_SOCKETERROR:
            case DEF_XSOCKEVENT_CRITICALERROR:
            case DEF_XSOCKEVENT_SOCKETCLOSED:
                DeleteClient(iClientH, true, true);
                return 0;
        }
        // locobans
        SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
        return 0;
    }

    return 1;
}

void CGame::RequestInitPlayerHandler(int iClientH, char * pData, char cKey)
{
    int i;
    char * cp, cCharName[11], cAccountName[11], cAccountPassword[11], cTxt[120];
    bool bIsObserverMode;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == true) return;


    memset(cCharName, 0, sizeof(cCharName));
    memset(cAccountName, 0, sizeof(cAccountName));
    memset(cAccountPassword, 0, sizeof(cAccountPassword));

    memset(m_pClientList[iClientH]->m_cCharName, 0, sizeof(m_pClientList[iClientH]->m_cCharName));
    memset(m_pClientList[iClientH]->m_cAccountName, 0, sizeof(m_pClientList[iClientH]->m_cAccountName));
    memset(m_pClientList[iClientH]->m_cAccountPassword, 0, sizeof(m_pClientList[iClientH]->m_cAccountPassword));

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

    memcpy(cCharName, cp, 10);
    cp += 10;

    memset(cTxt, 0, sizeof(cTxt)); // v1.4
    memcpy(cTxt, cCharName, 10);
    m_Misc.bDecode(cKey, cTxt);
    memset(cCharName, 0, sizeof(cCharName));
    memcpy(cCharName, cTxt, 10);

    //testcode
    if (strlen(cTxt) == 0) log->error("RIPH - cTxt: Char 0!");

    memcpy(cAccountName, cp, 10);
    cp += 10;

    memset(cTxt, 0, sizeof(cTxt));
    memcpy(cTxt, cAccountName, 10);
    m_Misc.bDecode(cKey, cTxt);
    memset(cAccountName, 0, sizeof(cAccountName));
    memcpy(cAccountName, cTxt, 10);

    memcpy(cAccountPassword, cp, 10);
    cp += 10;

    memset(cTxt, 0, sizeof(cTxt));
    memcpy(cTxt, cAccountPassword, 10);
    m_Misc.bDecode(cKey, cTxt);
    memset(cAccountPassword, 0, sizeof(cAccountPassword));
    memcpy(cAccountPassword, cTxt, 10);

    bIsObserverMode = (bool)*cp;
    cp++;

    for (i = 1; i < DEF_MAXCLIENTS; i++)
        if ((m_pClientList[i] != 0) && (iClientH != i) && (memcmp(m_pClientList[i]->m_cAccountName, cAccountName, 10) == 0))
        {
            if (memcmp(m_pClientList[i]->m_cAccountPassword, cAccountPassword, 10) == 0)
            {
                log->info("<{}> Duplicate account player! Deleted with data save : CharName({}}) AccntName({{}) IP({})", i, m_pClientList[i]->m_cCharName, m_pClientList[i]->m_cAccountName, m_pClientList[i]->m_cIPaddress);
                DeleteClient(i, true, true, false);
            }
            else
            {
                memcpy(m_pClientList[iClientH]->m_cCharName, cCharName, 10);
                memcpy(m_pClientList[iClientH]->m_cAccountName, cAccountName, 10);
                memcpy(m_pClientList[iClientH]->m_cAccountPassword, cAccountPassword, 10);

                DeleteClient(iClientH, false, false, false);
                return;
            }
        }

    for (i = 1; i < DEF_MAXCLIENTS; i++)
        if ((m_pClientList[i] != 0) && (iClientH != i) && (memcmp(m_pClientList[i]->m_cCharName, cCharName, 10) == 0))
        {
            if (memcmp(m_pClientList[i]->m_cAccountPassword, cAccountPassword, 10) == 0)
            {
                log->info("<{}> Duplicate player! Deleted with data save : CharName({}) IP({})", i, m_pClientList[i]->m_cCharName, m_pClientList[i]->m_cIPaddress);
                DeleteClient(i, true, true, false);
            }
            else
            {
                memcpy(m_pClientList[iClientH]->m_cCharName, cCharName, 10);
                memcpy(m_pClientList[iClientH]->m_cAccountName, cAccountName, 10);
                memcpy(m_pClientList[iClientH]->m_cAccountPassword, cAccountPassword, 10);

                DeleteClient(iClientH, false, false);
                return;
            }
        }

    memcpy(m_pClientList[iClientH]->m_cCharName, cCharName, 10);
    memcpy(m_pClientList[iClientH]->m_cAccountName, cAccountName, 10);
    memcpy(m_pClientList[iClientH]->m_cAccountPassword, cAccountPassword, 10);

    m_pClientList[iClientH]->m_bIsObserverMode = bIsObserverMode;

    bSendMsgToLS(MSGID_REQUEST_PLAYERDATA, iClientH);
}

void CGame::RequestInitDataHandler(int iClientH, char * pData, char cKey)
{
    char * pBuffer = 0;
    short * sp;
    uint32_t * dwp;
    uint16_t * wp;
    char * cp, cPlayerName[11], cTxt[120], sSummonPoints;
    int * ip, i, iTotalItemA, iTotalItemB, iSize, iRet, iStats;
    SYSTEMTIME SysTime;

    if (m_pClientList[iClientH] == nullptr) return;

    pBuffer = new char[DEF_MSGBUFFERSIZE + 1];
    memset(pBuffer, 0, DEF_MSGBUFFERSIZE + 1);

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);
    memset(cPlayerName, 0, sizeof(cPlayerName));
    memcpy(cPlayerName, cp, 10);

    memset(cTxt, 0, sizeof(cTxt)); // v1.4
    memcpy(cTxt, cPlayerName, 10);
    m_Misc.bDecode(cKey, cTxt);
    memset(cPlayerName, 0, sizeof(cPlayerName));
    memcpy(cPlayerName, cTxt, 10);

    if (memcmp(m_pClientList[iClientH]->m_cCharName, cPlayerName, 10) != 0)
    {
        DeleteClient(iClientH, false, true);
        return;
    }

    dwp = (uint32_t *)(pBuffer + DEF_INDEX4_MSGID);
    *dwp = MSGID_PLAYERCHARACTERCONTENTS; // 0x0Fm_cHeldenianVictoryType000 = 262406144
    wp = (uint16_t *)(pBuffer + DEF_INDEX2_MSGTYPE);
    *wp = DEF_MSGTYPE_CONFIRM;

    cp = (char *)(pBuffer + DEF_INDEX2_MSGTYPE + 2);

    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iHP;
    cp += 4;

    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iMP;
    cp += 4;

    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iSP;
    cp += 4;

    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iDefenseRatio;
    cp += 4;

    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iHitRatio;
    cp += 4;

    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iLevel;
    cp += 4;

    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iStr;
    cp += 4;

    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iInt;
    cp += 4;

    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iVit;
    cp += 4;

    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iDex;
    cp += 4;

    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iMag;
    cp += 4;

    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iCharisma;
    cp += 4;

    iStats = (m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iDex + m_pClientList[iClientH]->m_iVit +
        m_pClientList[iClientH]->m_iInt + m_pClientList[iClientH]->m_iMag + m_pClientList[iClientH]->m_iCharisma);

    m_pClientList[iClientH]->m_iLU_Pool = m_pClientList[iClientH]->m_iLevel * 3 - (iStats - 70);
    wp = (uint16_t *)cp;
    //*wp = m_pClientList[iClientH]->m_iLevel*3 - (iStats - 70); 
    *wp = m_pClientList[iClientH]->m_iLU_Pool;
    cp += 2;

    *cp = m_pClientList[iClientH]->m_cVar;
    cp++;

    *cp = 0;
    cp++;

    *cp = 0;
    cp++;

    *cp = 0;
    cp++;

    *cp = 0;
    cp++;

    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iExp;
    cp += 4;

    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iEnemyKillCount;
    cp += 4;

    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iPKCount;
    cp += 4;

    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iRewardGold;
    cp += 4;

    memcpy(cp, m_pClientList[iClientH]->m_cLocation, 10);
    cp += 10;

    memcpy(cp, m_pClientList[iClientH]->m_cGuildName, 20);
    cp += 20;

    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iGuildRank;
    cp += 4;

    // v1.4311
    *cp = (char)m_pClientList[iClientH]->m_iSuperAttackLeft;
    cp++;

    // v1.4311-3 
    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iFightzoneNumber;
    cp += 4;

    //hbest
    m_pClientList[iClientH]->isForceSet = false;
    m_pClientList[iClientH]->m_iPartyID = 0;
    m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
    m_pClientList[iClientH]->m_iReqJoinPartyClientH = 0;

    //Syntax : ======HP==MP==SP==DRatHRatLVL=STR=INT=VIT=DEX=MAG=CHR=LUstatEXP=EK==PK==RewaLocation==GuildName=RankAF
    //Syntax : 1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345
    //Syntax : ......145212521152........376.200=200=200=200=200=195=......big.8...17......aresden...NONE......NONE30
    // 0x0Fm_cHeldenianVictoryType000 = 262406144
    // Ã¬Â¨ÂÃ­Å¡Â§Ã¬Â©ÂÃ­Å¡â€žÃ­Å¡ÂÃ¬Â²Â  ?Ã¬Â²Â´Ã¬Â©Å’Ã­Å¡Â¤ 

    //Debug Event
    //DbgWnd->AddEventMsg(MSG_SEND,pBuffer,180,0);

    iRet = m_pClientList[iClientH]->send_msg(pBuffer, 118);
    switch (iRet)
    {
        case DEF_XSOCKEVENT_QUENEFULL:
        case DEF_XSOCKEVENT_SOCKETERROR:
        case DEF_XSOCKEVENT_CRITICALERROR:
        case DEF_XSOCKEVENT_SOCKETCLOSED:
            // Ã¬Â¨ÂÃ­Å¡Â§Ã¬Â©ÂÃ­Å¡â€žÃ­Å¡ÂÃ¬Â²Â Ã¬Â¨ÂÃ¬Â§Â Ã¬Â¨ËœÃ¬Â¨ÂÃ¬Â¨â‚¬Ã¬Â©ÂÃ¬Â¨â€¹Ã¬Â§Â  Ã¬Â©â€Ã¬Â§â€¢Ã¬Â¨Å’Ã¬Â§Â±Ã¬Â§Â¸Ã¬Â§â€¢ Ã¬Â¨â€Ã­Å¡Â©Ã¬Â¨Â©Ã¬Â²ÂµÃ­Å¡â€°Ã­Å¡Â©Ã¬Â¨Ë†Ã­Å¡Â¢Ã¬Â¨ÂÃ¬Â±â€¢ Ã­Å¡ÂÃ¬Â§ÂÃ¬Â§Â¸Ã­Å¡â€¡Ã­Å¡â€°Ã­Å¡â€”Ã¬Â¨Ë†Ã­Å¡Â¢.
            DeleteClient(iClientH, true, true);
            if (pBuffer != 0) delete pBuffer;
            return;
    }

    dwp = (uint32_t *)(pBuffer + DEF_INDEX4_MSGID);
    *dwp = MSGID_PLAYERITEMLISTCONTENTS;
    wp = (uint16_t *)(pBuffer + DEF_INDEX2_MSGTYPE);
    *wp = DEF_MSGTYPE_CONFIRM;

    iTotalItemA = 0;
    for (i = 0; i < DEF_MAXITEMS; i++)
        if (m_pClientList[iClientH]->m_pItemList[i] != 0)
            iTotalItemA++;

    cp = (char *)(pBuffer + DEF_INDEX2_MSGTYPE + 2);

    *cp = iTotalItemA;
    cp++;

    for (i = 0; i < iTotalItemA; i++)
    {
        // ### ERROR POINT!!!
        if (m_pClientList[iClientH]->m_pItemList[i] == 0)
        {
            log->info("RequestInitDataHandler error: Client({}) Item({})", m_pClientList[iClientH]->m_cCharName, i);

            DeleteClient(iClientH, false, true);
            if (pBuffer != 0) delete pBuffer;
            return;
        }
        memcpy(cp, m_pClientList[iClientH]->m_pItemList[i]->m_cName, 20);
        cp += 20;
        dwp = (uint32_t *)cp;
        *dwp = m_pClientList[iClientH]->m_pItemList[i]->m_dwCount;
        cp += 4;
        *cp = m_pClientList[iClientH]->m_pItemList[i]->m_cItemType;
        cp++;
        *cp = m_pClientList[iClientH]->m_pItemList[i]->m_cEquipPos;
        cp++;
        *cp = (char)m_pClientList[iClientH]->m_bIsItemEquipped[i];
        cp++;
        sp = (short *)cp;
        *sp = m_pClientList[iClientH]->m_pItemList[i]->m_sLevelLimit;
        cp += 2;
        *cp = m_pClientList[iClientH]->m_pItemList[i]->m_cGenderLimit;
        cp++;
        wp = (uint16_t *)cp;
        *wp = m_pClientList[iClientH]->m_pItemList[i]->m_wCurLifeSpan;
        cp += 2;
        wp = (uint16_t *)cp;
        *wp = m_pClientList[iClientH]->m_pItemList[i]->m_wWeight;
        cp += 2;
        sp = (short *)cp;
        *sp = m_pClientList[iClientH]->m_pItemList[i]->m_sSprite;
        cp += 2;
        sp = (short *)cp;
        *sp = m_pClientList[iClientH]->m_pItemList[i]->m_sSpriteFrame;
        cp += 2;
        *cp = m_pClientList[iClientH]->m_pItemList[i]->m_cItemColor;
        cp++;
        *cp = (char)m_pClientList[iClientH]->m_pItemList[i]->m_sItemSpecEffectValue2;
        cp++;
        dwp = (uint32_t *)cp;
        *dwp = m_pClientList[iClientH]->m_pItemList[i]->m_dwAttribute;
        cp += 4;
    }

    iTotalItemB = 0;
    for (i = 0; i < DEF_MAXBANKITEMS; i++)
        if (m_pClientList[iClientH]->m_pItemInBankList[i] != 0)
            iTotalItemB++;

    *cp = iTotalItemB;
    cp++;

    for (i = 0; i < iTotalItemB; i++)
    {
        if (m_pClientList[iClientH]->m_pItemInBankList[i] == 0)
        {
            log->info("RequestInitDataHandler error: Client({}) Bank-Item({})", m_pClientList[iClientH]->m_cCharName, i);

            DeleteClient(iClientH, false, true);
            if (pBuffer != 0) delete pBuffer;
            return;
        }
        memcpy(cp, m_pClientList[iClientH]->m_pItemInBankList[i]->m_cName, 20);
        cp += 20;
        dwp = (uint32_t *)cp;
        *dwp = m_pClientList[iClientH]->m_pItemInBankList[i]->m_dwCount;
        cp += 4;
        *cp = m_pClientList[iClientH]->m_pItemInBankList[i]->m_cItemType;
        cp++;
        *cp = m_pClientList[iClientH]->m_pItemInBankList[i]->m_cEquipPos;
        cp++;
        sp = (short *)cp;
        *sp = m_pClientList[iClientH]->m_pItemInBankList[i]->m_sLevelLimit;
        cp += 2;
        *cp = m_pClientList[iClientH]->m_pItemInBankList[i]->m_cGenderLimit;
        cp++;
        wp = (uint16_t *)cp;
        *wp = m_pClientList[iClientH]->m_pItemInBankList[i]->m_wCurLifeSpan;
        cp += 2;
        wp = (uint16_t *)cp;
        *wp = m_pClientList[iClientH]->m_pItemInBankList[i]->m_wWeight;
        cp += 2;
        sp = (short *)cp;
        *sp = m_pClientList[iClientH]->m_pItemInBankList[i]->m_sSprite;
        cp += 2;
        sp = (short *)cp;
        *sp = m_pClientList[iClientH]->m_pItemInBankList[i]->m_sSpriteFrame;
        cp += 2;
        *cp = m_pClientList[iClientH]->m_pItemInBankList[i]->m_cItemColor;
        cp++;
        *cp = (char)m_pClientList[iClientH]->m_pItemInBankList[i]->m_sItemSpecEffectValue2;
        cp++;
        dwp = (uint32_t *)cp;
        *dwp = m_pClientList[iClientH]->m_pItemInBankList[i]->m_dwAttribute;
        cp += 4;
    }

    for (i = 0; i < DEF_MAXMAGICTYPE; i++)
    {
        *cp = m_pClientList[iClientH]->m_cMagicMastery[i];
        cp++;
    }

    for (i = 0; i < DEF_MAXSKILLTYPE; i++)
    {
        *cp = m_pClientList[iClientH]->m_cSkillMastery[i];
        cp++;
    }

    iRet = m_pClientList[iClientH]->send_msg(pBuffer, 6 + 1 + iTotalItemA * 44 + iTotalItemB * 43 + DEF_MAXMAGICTYPE + DEF_MAXSKILLTYPE);
    switch (iRet)
    {
        case DEF_XSOCKEVENT_QUENEFULL:
        case DEF_XSOCKEVENT_SOCKETERROR:
        case DEF_XSOCKEVENT_CRITICALERROR:
        case DEF_XSOCKEVENT_SOCKETCLOSED:
            DeleteClient(iClientH, true, true);
            if (pBuffer != 0) delete pBuffer;
            return;
    }

    dwp = (uint32_t *)(pBuffer + DEF_INDEX4_MSGID);
    *dwp = MSGID_RESPONSE_INITDATA;
    wp = (uint16_t *)(pBuffer + DEF_INDEX2_MSGTYPE);
    *wp = DEF_MSGTYPE_CONFIRM;

    cp = (char *)(pBuffer + DEF_INDEX2_MSGTYPE + 2);

    if (m_pClientList[iClientH]->m_bIsObserverMode == false)
        bGetEmptyPosition(&m_pClientList[iClientH]->m_sX, &m_pClientList[iClientH]->m_sY, m_pClientList[iClientH]->m_cMapIndex);
    else GetMapInitialPoint(m_pClientList[iClientH]->m_cMapIndex, &m_pClientList[iClientH]->m_sX, &m_pClientList[iClientH]->m_sY);

    // ObjectID
    wp = (uint16_t *)cp;
    *wp = iClientH;
    cp += 2;

    sp = (short *)cp;
    *sp = m_pClientList[iClientH]->m_sX - 14 - 5;
    cp += 2;

    sp = (short *)cp;
    *sp = m_pClientList[iClientH]->m_sY - 12 - 5;
    cp += 2;

    sp = (short *)cp;
    *sp = m_pClientList[iClientH]->m_sType;
    cp += 2;

    sp = (short *)cp;
    *sp = m_pClientList[iClientH]->m_sAppr1;
    cp += 2;

    sp = (short *)cp;
    *sp = m_pClientList[iClientH]->m_sAppr2;
    cp += 2;

    sp = (short *)cp;
    *sp = m_pClientList[iClientH]->m_sAppr3;
    cp += 2;

    sp = (short *)cp;
    *sp = m_pClientList[iClientH]->m_sAppr4;
    cp += 2;

    ip = (int *)cp; // v1.4
    *ip = m_pClientList[iClientH]->m_iApprColor;
    cp += 4;

    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iStatus;
    cp += 4;// Original : 2

    memcpy(cp, m_pClientList[iClientH]->m_cMapName, 10);
    cp += 10;

    memcpy(cp, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, 10);
    cp += 10;

    if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFixedDayMode == true)
        *cp = 1;
    else *cp = m_cDayOrNight;
    cp++;

    if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFixedDayMode == true)
        *cp = 0;
    else *cp = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cWhetherStatus;
    cp++;

    // v1.4 Contribution
    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iContribution;
    cp += 4;


    if (m_pClientList[iClientH]->m_bIsObserverMode == false)
    {
        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetOwner(iClientH,
            DEF_OWNERTYPE_PLAYER,
            m_pClientList[iClientH]->m_sX,
            m_pClientList[iClientH]->m_sY);
    }

    // v1.41
    *cp = (char)m_pClientList[iClientH]->m_bIsObserverMode;
    cp++;
    // catches debug on player load up

    // v1.41 
    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iRating;
    cp += 4;

    // v1.44
    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iHP;
    cp += 4;

    //Unknown variable
    *cp = 0;
    cp++;

    iSize = iComposeInitMapData(m_pClientList[iClientH]->m_sX - 10, m_pClientList[iClientH]->m_sY - 7, iClientH, cp);

    iRet = m_pClientList[iClientH]->send_msg(pBuffer, 46 + iSize + 4 + 4 + 1 + 4 + 4 + 3); // Zabuza fix
    //iRet = m_pClientList[iClientH]->send_msg(pBuffer, 59 + iSize +4 +4 +1 +4 +4); // v1.41
    //	iRet = m_pClientList[iClientH]->send_msg(pBuffer, 46 + iSize +4 +4 +1 +4 +4); // v1.41
    switch (iRet)
    {
        case DEF_XSOCKEVENT_QUENEFULL:
        case DEF_XSOCKEVENT_SOCKETERROR:
        case DEF_XSOCKEVENT_CRITICALERROR:
        case DEF_XSOCKEVENT_SOCKETCLOSED:
            DeleteClient(iClientH, true, true);
            if (pBuffer != 0) delete pBuffer;
            return;
    }

    if (pBuffer != 0) delete pBuffer;

    SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_LOG, DEF_MSGTYPE_CONFIRM, 0, 0, 0);

    // v2.13 
    if ((memcmp(m_pClientList[iClientH]->m_cLocation, "are", 3) == 0) &&
        (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "elvine", 6) == 0)
        && (m_pClientList[iClientH]->m_iAdminUserLevel == 0))
    {

        m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();
        m_pClientList[iClientH]->m_bIsWarLocation = true;
        // v2.17 2002-7-15
        SetForceRecallTime(iClientH);
    }
    // v2.13 
    else if ((memcmp(m_pClientList[iClientH]->m_cLocation, "elv", 3) == 0) &&
        (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "aresden", 7) == 0)
        && (m_pClientList[iClientH]->m_iAdminUserLevel == 0))
    {

        m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();
        m_pClientList[iClientH]->m_bIsWarLocation = true;

        // v2.17 2002-7-15
        SetForceRecallTime(iClientH);
    }
    else if (((memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "arejail", 7) == 0) ||
        (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "elvjail", 7) == 0))
        && (m_pClientList[iClientH]->m_iAdminUserLevel == 0))
    {
        m_pClientList[iClientH]->m_bIsWarLocation = true;
        m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();

        // v2.17 2002-7-15 
        if (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall == 0)
        {
            m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 20 * 5;
        }
        else if (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall > 20 * 5)
        {
            m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 20 * 5;  // 5ºÐ
        }

    }
    else if ((m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFightZone == true) &&
        (m_iFightzoneNoForceRecall == 0) && (m_pClientList[iClientH]->m_iAdminUserLevel == 0))
    {

        m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();
        m_pClientList[iClientH]->m_bIsWarLocation = true;

        GetLocalTime(&SysTime);
        m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 2 * 60 * 20 - ((SysTime.wHour % 2) * 20 * 60 + SysTime.wMinute * 20) - 2 * 20;
    }
    else
    {
        m_pClientList[iClientH]->m_bIsWarLocation = false;
        // v1.42
        m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 0;
        // 06/11/2004
        SetForceRecallTime(iClientH);
    }

    // v2.17 2002-7-15
    //hbest...
    if ((m_pClientList[iClientH]->m_iTimeLeft_ForceRecall > 0) && (m_pClientList[iClientH]->m_bIsWarLocation == true) && IsEnemyZone(iClientH))
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_FORCERECALLTIME, m_pClientList[iClientH]->m_iTimeLeft_ForceRecall, 0, 0, 0);
        //copy_string(G_cTxt,"(!) Game Server Force Recall Time  %d (%d)min", m_pClientList[iClientH]->m_iTimeLeft_ForceRecall, m_pClientList[iClientH]->m_iTimeLeft_ForceRecall/20) ;
        //log->info(G_cTxt) ;
    }

    if (m_pClientList[iClientH]->m_iGizonItemUpgradeLeft < 0)
    {
        m_pClientList[iClientH]->m_iGizonItemUpgradeLeft = 0;
    }

    // No entering enemy shops
    int iMapside, iMapside2;

    iMapside = iGetMapLocationSide(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName);
    if (iMapside > 3) iMapside2 = iMapside - 2;
    else iMapside2 = iMapside;
    m_pClientList[iClientH]->m_bIsInsideOwnTown = false;
    if ((m_pClientList[iClientH]->m_cSide != iMapside2) && (iMapside != 0))
    {
        if ((iMapside <= 2) && (m_pClientList[iClientH]->m_iAdminUserLevel < 1))
        {
            if (m_pClientList[iClientH]->m_cSide != 0)
            {
                m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();
                m_pClientList[iClientH]->m_bIsWarLocation = true;
                m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 1;
                m_pClientList[iClientH]->m_bIsInsideOwnTown = true;
            }
        }
    }
    else
    {
        if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFightZone == true &&
            m_iFightzoneNoForceRecall == false &&
            m_pClientList[iClientH]->m_iAdminUserLevel == 0)
        {
            m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();
            m_pClientList[iClientH]->m_bIsWarLocation = true;
            GetLocalTime(&SysTime);
            m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 2 * 60 * 20 - ((SysTime.wHour % 2) * 20 * 60 + SysTime.wMinute * 20) - 2 * 20;
        }
        else
        {
            if (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "arejail", 7) == 0 ||
                memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "elvjail", 7) == 0)
            {
                if (m_pClientList[iClientH]->m_iAdminUserLevel == 0)
                {
                    m_pClientList[iClientH]->m_bIsWarLocation = true;
                    m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();
                    if (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall == 0)
                        m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 100;
                    else if (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall > 100)
                        m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 100;
                }
            }
        }
    }

    /*if ((m_pClientList[iClientH]->m_iTimeLeft_ForceRecall > 0) &&
        (m_pClientList[iClientH]->m_bIsWarLocation == true)) {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_FORCERECALLTIME, m_pClientList[iClientH]->m_iTimeLeft_ForceRecall, 0, 0, 0);
    }*/

    SendNotifyMsg(0, iClientH, DEF_NOTIFY_SAFEATTACKMODE, 0, 0, 0, 0);
    SendNotifyMsg(0, iClientH, DEF_NOTIFY_DOWNSKILLINDEXSET, m_pClientList[iClientH]->m_iDownSkillIndex, 0, 0, 0);
    SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMPOSLIST, 0, 0, 0, 0);

    _SendQuestContents(iClientH);
    _CheckQuestEnvironment(iClientH);

    // v1.432
    if (m_pClientList[iClientH]->m_iSpecialAbilityTime == 0)
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SPECIALABILITYENABLED, 0, 0, 0, 0);
    }

    // Crusade 
    if (m_bIsCrusadeMode == true)
    {
        if (m_pClientList[iClientH]->m_dwCrusadeGUID == 0)
        {
            m_pClientList[iClientH]->m_iCrusadeDuty = 0;
            m_pClientList[iClientH]->m_iConstructionPoint = 0;
            m_pClientList[iClientH]->m_dwCrusadeGUID = m_dwCrusadeGUID;
        }
        else if (m_pClientList[iClientH]->m_dwCrusadeGUID != m_dwCrusadeGUID)
        {
            m_pClientList[iClientH]->m_iCrusadeDuty = 0;
            m_pClientList[iClientH]->m_iConstructionPoint = 0;
            m_pClientList[iClientH]->m_iWarContribution = 0;
            m_pClientList[iClientH]->m_dwCrusadeGUID = m_dwCrusadeGUID;
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_CRUSADE, (uint32_t)m_bIsCrusadeMode, 0, 0, 0, -1);
        }
        m_pClientList[iClientH]->m_cVar = 1;
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_CRUSADE, (uint32_t)m_bIsCrusadeMode, m_pClientList[iClientH]->m_iCrusadeDuty, 0, 0);
    }
    else if (m_bIsHeldenianMode == true)
    {
        sSummonPoints = m_pClientList[iClientH]->m_iCharisma * 300;
        if (sSummonPoints > DEF_MAXSUMMONPOINTS) sSummonPoints = DEF_MAXSUMMONPOINTS;
        if (m_pClientList[iClientH]->m_dwHeldenianGUID == 0)
        {
            m_pClientList[iClientH]->m_dwHeldenianGUID = m_dwHeldenianGUID;
            m_pClientList[iClientH]->m_iConstructionPoint = sSummonPoints;
        }
        else if (m_pClientList[iClientH]->m_dwHeldenianGUID != m_dwHeldenianGUID)
        {
            m_pClientList[iClientH]->m_iConstructionPoint = sSummonPoints;
            m_pClientList[iClientH]->m_iWarContribution = 0;
            m_pClientList[iClientH]->m_dwHeldenianGUID = m_dwHeldenianGUID;
        }
        m_pClientList[iClientH]->m_cVar = 2;
        if (m_bIsHeldenianMode == true)
        {
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_CRUSADE, 0, 0, 0, 0);
            if (m_bHeldenianInitiated == false)
            {
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_HELDENIANSTART, 0, 0, 0, 0);
            }
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[iClientH]->m_iConstructionPoint, m_pClientList[iClientH]->m_iWarContribution, 0, 0);
            UpdateHeldenianStatus();
        }
    }
    else if ((m_pClientList[iClientH]->m_cVar == 1) && (m_pClientList[iClientH]->m_dwCrusadeGUID == m_dwCrusadeGUID))
    {
        m_pClientList[iClientH]->m_iCrusadeDuty = 0;
        m_pClientList[iClientH]->m_iConstructionPoint = 0;
    }
    else
    {
        if (m_pClientList[iClientH]->m_dwCrusadeGUID == m_dwCrusadeGUID)
        {
            if (m_pClientList[iClientH]->m_cVar == 1)
            {
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_CRUSADE, (uint32_t)m_bIsCrusadeMode, 0, 0, 0, -1);
            }
        }
        else
        {
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_CRUSADE, (uint32_t)m_bIsCrusadeMode, 0, 0, 0, -1);
            m_pClientList[iClientH]->m_dwCrusadeGUID = 0;
            m_pClientList[iClientH]->m_iWarContribution = 0;
            m_pClientList[iClientH]->m_dwCrusadeGUID = 0;
        }
    }

    // v1.42
    if (memcmp(m_pClientList[iClientH]->m_cMapName, "fightzone", 9) == 0)
    {
        copy_string(G_cTxt, "Char(%s)-Enter(%s) Observer(%d)", m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_cMapName, m_pClientList[iClientH]->m_bIsObserverMode);
        PutLogEventFileList(G_cTxt);
    }

    if (m_bIsHeldenianMode == true) SendNotifyMsg(0, iClientH, DEF_NOTIFY_HELDENIANTELEPORT, 0, 0, 0, 0, 0);
    if (m_bHeldenianInitiated == true) SendNotifyMsg(0, iClientH, DEF_NOTIFY_HELDENIANSTART, 0, 0, 0, 0, 0);

    // Crusade
    SendNotifyMsg(0, iClientH, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[iClientH]->m_iConstructionPoint, m_pClientList[iClientH]->m_iWarContribution, 1, 0);
    //Fix Sprite Bug
    //			SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
    //Gizon point lefT???
    SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIZONITEMUPGRADELEFT, m_pClientList[iClientH]->m_iGizonItemUpgradeLeft, 0, 0, 0);

    if ((m_bIsApocalypseMode == true) && (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsApocalypseMap == true))
    {
        RequestTeleportHandler(iClientH, "1   ");
    }

    if (m_bIsApocalypseMode == true)
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_APOCGATESTARTMSG, 0, 0, 0, 0, 0);
    }
}

void CGame::ResponsePlayerDataHandler(char * pData, uint32_t dwSize)
{
    uint16_t * wp;
    char * cp, cCharName[11], cTxt[120];
    int  i;

    memset(cCharName, 0, sizeof(cCharName));
    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

    memcpy(cCharName, cp, 10);
    cp += 10;

    for (i = 1; i < DEF_MAXCLIENTS; i++)
        if (m_pClientList[i] != 0)
        {
            if (memcmp(m_pClientList[i]->m_cCharName, cCharName, 10) == 0)
            {
                wp = (uint16_t *)(pData + DEF_INDEX2_MSGTYPE);
                switch (*wp)
                {
                    case DEF_LOGRESMSGTYPE_CONFIRM:
                        InitPlayerData(i, pData, dwSize);
                        break;

                    case DEF_LOGRESMSGTYPE_REJECT:
                        log->info("(HACK?) Not existing character({}) data request! Rejected!", m_pClientList[i]->m_cCharName);

                        DeleteClient(i, false, false);
                        break;

                    default:
                        break;
                }

                return;
            }
        }

    log->info("(!)Non-existing player data received from Log server: CharName({})", cCharName);
}

void CGame::ChatMsgHandler(int iClientH, char * pData, uint32_t dwMsgSize)
{
    int i, iRet;
    uint16_t * wp;
    int * ip;
    char * cp, * cp2;
    char   cBuffer[256], cTemp[256]{}, cSendMode = 0;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    if (dwMsgSize > 83 + 30) return;

    if (m_pClientList[iClientH]->m_iTimeLeft_ShutUp > 0) return;

    if (memcmp((pData + 10), m_pClientList[iClientH]->m_cCharName, strlen(m_pClientList[iClientH]->m_cCharName)) != 0) return;

    if ((m_pClientList[iClientH]->m_bIsObserverMode == true) && (m_pClientList[iClientH]->m_iAdminUserLevel == 0)) return;

    int iStX, iStY;
    if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex] != 0)
    {
        iStX = m_pClientList[iClientH]->m_sX / 20;
        iStY = m_pClientList[iClientH]->m_sY / 20;
        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iPlayerActivity++;

        switch (m_pClientList[iClientH]->m_cSide)
        {
            case 0: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iNeutralActivity++; break;
            case 1: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iAresdenActivity++; break;
            case 2: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iElvineActivity++;  break;
        }
    }
    cp = (char *)(pData + 21);

    switch (*cp)
    {
        case '@':
            *cp = 32;

            if ((strlen(cp) < 90) && (m_pClientList[iClientH]->m_iGuildRank == 0))
            {
                memset(cTemp, 0, sizeof(cTemp));
                cp2 = (char *)cTemp;
                *cp2 = GSM_CHATMSG;
                cp2++;
                *cp2 = 1;
                cp2++;
                ip = (int *)cp2;
                *ip = m_pClientList[iClientH]->m_iGuildGUID;
                cp2 += 4;
                memcpy(cp2, m_pClientList[iClientH]->m_cCharName, 10);
                cp2 += 10;
                wp = (uint16_t *)cp2;
                *wp = (uint16_t)strlen(cp);
                cp2 += 2;
                strcpy(cp2, cp);
                cp2 += strlen(cp);
                bStockMsgToGateServer(cTemp, strlen(cp) + 18);
            }

            if ((m_pClientList[iClientH]->m_iTimeLeft_ShutUp == 0) && (m_pClientList[iClientH]->m_iLevel > 1) &&
                (m_pClientList[iClientH]->m_iSP >= 3))
            {
                if (m_pClientList[iClientH]->m_iTimeLeft_FirmStaminar == 0)
                {
                    m_pClientList[iClientH]->m_iSP -= 3;
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_SP, 0, 0, 0, 0);
                }
                cSendMode = 1;
            }
            else cSendMode = 0;

            if (m_pClientList[iClientH]->m_iTimeLeft_ShutUp > 0) cSendMode = 0;
            break;

            // Party chat
        case '$':
            *cp = 32;

            if ((m_pClientList[iClientH]->m_iTimeLeft_ShutUp == 0) && (m_pClientList[iClientH]->m_iSP >= 3))
            {
                if (m_pClientList[iClientH]->m_iTimeLeft_FirmStaminar == 0)
                {
                    m_pClientList[iClientH]->m_iSP -= 3;
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_SP, 0, 0, 0, 0);
                }
                cSendMode = 4;
            }
            else
            {
                cSendMode = 0;
            }

            if (m_pClientList[iClientH]->m_iTimeLeft_ShutUp > 0)
            {
                cSendMode = 0;
            }
            break;

        case '^':
            *cp = 32;

            if ((strlen(cp) < 90) && (m_pClientList[iClientH]->m_iAdminUserLevel > 0))
            {
                memset(cTemp, 0, sizeof(cTemp));
                cp2 = (char *)cTemp;
                *cp2 = GSM_CHATMSG;
                cp2++;
                *cp2 = 10;
                cp2++;
                ip = (int *)cp2;
                *ip = 0;
                cp2 += 4;
                memcpy(cp2, m_pClientList[iClientH]->m_cCharName, 10);
                cp2 += 10;
                wp = (uint16_t *)cp2;
                *wp = (uint16_t)strlen(cp);
                cp2 += 2;
                strcpy(cp2, cp);
                cp2 += strlen(cp);
                bStockMsgToGateServer(cTemp, strlen(cp) + 18);
            }

            if ((strlen(cp) < 90) && (m_pClientList[iClientH]->m_iGuildRank != -1))
            {
                memset(cTemp, 0, sizeof(cTemp));
                cp2 = (char *)cTemp;
                *cp2 = GSM_CHATMSG;
                cp2++;
                *cp2 = 1;
                cp2++;
                ip = (int *)cp2;
                *ip = m_pClientList[iClientH]->m_iGuildGUID;
                cp2 += 4;
                memcpy(cp2, m_pClientList[iClientH]->m_cCharName, 10);
                cp2 += 10;
                wp = (uint16_t *)cp2;
                *wp = (uint16_t)strlen(cp);
                cp2 += 2;
                strcpy(cp2, cp);
                cp2 += strlen(cp);
                bStockMsgToGateServer(cTemp, strlen(cp) + 18);
            }

            if ((m_pClientList[iClientH]->m_iTimeLeft_ShutUp == 0) && (m_pClientList[iClientH]->m_iLevel > 10) &&
                (m_pClientList[iClientH]->m_iSP > 5) && m_pClientList[iClientH]->m_iGuildRank != -1)
            {
                if (m_pClientList[iClientH]->m_iTimeLeft_FirmStaminar == 0)
                {
                    m_pClientList[iClientH]->m_iSP -= 3;
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_SP, 0, 0, 0, 0);
                }
                cSendMode = 1;
            }
            else cSendMode = 0;

            if (m_pClientList[iClientH]->m_iTimeLeft_ShutUp > 0) cSendMode = 0;

            if (m_pClientList[iClientH]->m_iHP < 0) cSendMode = 0;

            if (m_pClientList[iClientH]->m_iAdminUserLevel > 0) cSendMode = 10;
            break;

        case '!':
            *cp = 32;

            if ((strlen(cp) < 90) && (m_pClientList[iClientH]->m_iAdminUserLevel > 0))
            {
                memset(cTemp, 0, sizeof(cTemp));
                cp2 = (char *)cTemp;
                *cp2 = GSM_CHATMSG;
                cp2++;
                *cp2 = 10;
                cp2++;
                ip = (int *)cp2;
                *ip = 0;
                cp2 += 4;
                memcpy(cp2, m_pClientList[iClientH]->m_cCharName, 10);
                cp2 += 10;
                wp = (uint16_t *)cp2;
                *wp = (uint16_t)strlen(cp);
                cp2 += 2;
                strcpy(cp2, cp);
                cp2 += strlen(cp);
                bStockMsgToGateServer(cTemp, strlen(cp) + 18);
            }

            if ((m_pClientList[iClientH]->m_iTimeLeft_ShutUp == 0) && (m_pClientList[iClientH]->m_iLevel > 10) &&
                (m_pClientList[iClientH]->m_iSP >= 5))
            {
                if (m_pClientList[iClientH]->m_iTimeLeft_FirmStaminar == 0)
                {
                    m_pClientList[iClientH]->m_iSP -= 5;
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_SP, 0, 0, 0, 0);
                }
                cSendMode = 2;
            }
            else cSendMode = 0;

            if (m_pClientList[iClientH]->m_iTimeLeft_ShutUp > 0) cSendMode = 0;

            if (m_pClientList[iClientH]->m_iHP <= 0) cSendMode = 0;

            if (m_pClientList[iClientH]->m_iAdminUserLevel > 0) cSendMode = 10;
            break;

        case '~':
            *cp = 32;
            if ((m_pClientList[iClientH]->m_iTimeLeft_ShutUp == 0) && (m_pClientList[iClientH]->m_iLevel > 1) &&
                (m_pClientList[iClientH]->m_iSP >= 3))
            {
                if (m_pClientList[iClientH]->m_iTimeLeft_FirmStaminar == 0)
                {
                    m_pClientList[iClientH]->m_iSP -= 3;
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_SP, 0, 0, 0, 0);
                }
                cSendMode = 3;
            }
            else cSendMode = 0;

            if (m_pClientList[iClientH]->m_iTimeLeft_ShutUp > 0) cSendMode = 0;
            if (m_pClientList[iClientH]->m_iHP <= 0) cSendMode = 0;
            break;

        case '/':
            memset(cBuffer, 0, sizeof(cBuffer));
            memcpy(cBuffer, cp, dwMsgSize - 21);
            cp = (char *)(cBuffer);

            if (memcmp(cp, "/version", 8) == 0)
            {
                ShowVersion(iClientH);
                return;
            }

            if (memcmp(cp, "/begincrusadetotalwar", 21) == 0)
            {
                if (m_pClientList[iClientH]->m_iAdminUserLevel > 3)
                {
                    GlobalStartCrusadeMode();
                    copy_string(cTemp, "(%s) GM Order(%s): begincrusadetotalwar", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
                    bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
                }
                return;
            }

            if (memcmp(cp, "/endcrusadetotalwar", 19) == 0)
            {
                if (m_pClientList[iClientH]->m_iAdminUserLevel > 3)
                {
                    ManualEndCrusadeMode(0);
                    copy_string(cTemp, "(%s) GM Order(%s): endcrusadetotalwar", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
                    bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
                }
                return;
            }

            if (memcmp(cp, "/goto ", 6) == 0)
            {
                AdminOrder_GoTo(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/unsummonboss", 13) == 0)
            {
                AdminOrder_UnsummonBoss(iClientH);
                return;
            }

            if (memcmp(cp, "/clearnpc", 9) == 0)
            {
                AdminOrder_ClearNpc(iClientH);
                return;
            }

            /*if (memcmp(cp, "/clearmap", 9) == 0) {
                if (m_pClientList[iClientH]->m_iAdminUserLevel > 1) {
                AdminOrder_CleanMap(iClientH, cp, dwMsgSize);
                }
                return;
            }*/

            if (memcmp(cp, "/setforcerecalltime ", 20) == 0)
            {
                AdminOrder_SetForceRecallTime(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/enableadmincommand ", 20) == 0)
            {
                AdminOrder_EnableAdminCommand(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/monstercount", 13) == 0)
            {
                AdminOrder_MonsterCount(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/createparty", 12) == 0)
            {
                if (m_pClientList[iClientH]->m_iAdminUserLevel > 1)
                {
                    RequestCreatePartyHandler(iClientH);
                }
                return;
            }

            if (memcmp(cp, "/joinparty ", 11) == 0)
            {
                RequestJoinPartyHandler(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/dismissparty", 13) == 0)
            {
                RequestDismissPartyHandler(iClientH);
                return;
            }

            if (memcmp(cp, "/blueball", 9) == 0)
            {
                Command_BlueBall(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/redball", 8) == 0)
            {
                Command_RedBall(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/yellowball ", 12) == 0)
            {
                Command_YellowBall(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/getpartyinfo", 13) == 0)
            {
                GetPartyInfoHandler(iClientH);
                return;
            }

            if (memcmp(cp, "/deleteparty", 12) == 0)
            {
                RequestDeletePartyHandler(iClientH);
                return;
            }

            if (memcmp(cp, "/who", 4) == 0)
            {
                if (m_pClientList[iClientH]->m_iAdminUserLevel >= m_iAdminLevelWho)
                {
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_TOTALUSERS, 0, 0, 0, 0);
                }
                return;
            }

            if (memcmp(cp, "/fi ", 4) == 0)
            {
                CheckAndNotifyPlayerConnection(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/to", 3) == 0)
            {
                ToggleWhisperPlayer(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/setpf ", 7) == 0)
            {
                SetPlayerProfile(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/weather", 8) == 0)
            {
                AdminOrder_Weather(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/pf ", 4) == 0)
            {
                GetPlayerProfile(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/shutup ", 8) == 0)
            {
                ShutUpPlayer(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/rep+ ", 6) == 0)
            {
                SetPlayerReputation(iClientH, cp, 1, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/time ", 6) == 0)
            {
                AdminOrder_Time(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/getskills", 10) == 0)
            {
                SetSkillAll(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/checkrep", 9) == 0)
            {
                AdminOrder_CheckRep(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/checkstatus ", 13) == 0)
            {
                AdminOrder_CheckStats(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/send ", 5) == 0)
            {
                AdminOrder_Pushplayer(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/rep- ", 6) == 0)
            {
                SetPlayerReputation(iClientH, cp, 0, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/hold", 5) == 0)
            {
                SetSummonMobAction(iClientH, 1, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/tgt ", 5) == 0)
            {
                SetSummonMobAction(iClientH, 2, dwMsgSize - 21, cp);
                return;
            }

            if (memcmp(cp, "/free", 5) == 0)
            {
                SetSummonMobAction(iClientH, 0, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/summonall ", 11) == 0)
            {
                AdminOrder_SummonAll(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/summonguild", 12) == 0)
            {
                if ((!m_bIsCrusadeMode) || (m_pClientList[iClientH]->m_iGuildRank == 0))
                {
                    if (m_pClientList[iClientH]->m_iGuildRank == 0)
                    {
                        if (dwGetItemCount(iClientH, "Gold") >= 100000)
                        {
                            SetItemCount(iClientH, "Gold", dwGetItemCount(iClientH, "Gold") - 50000);
                            AdminOrder_SummonGuild(iClientH, cp, dwMsgSize - 21);
                            return;
                        }
                    }
                }
            }

            if (memcmp(cp, "/summonplayer ", 14) == 0)
            {
                AdminOrder_SummonPlayer(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/storm ", 7) == 0)
            {
                AdminOrder_SummonStorm(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/summondeath ", 13) == 0)
            {
                AdminOrder_SummonDeath(iClientH);
                return;
            }

            if (memcmp(cp, "/kill ", 6) == 0)
            {
                AdminOrder_Kill(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/revive ", 8) == 0)
            {
                AdminOrder_Revive(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/closeconn ", 11) == 0)
            {
                AdminOrder_CloseConn(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/ban", 4) == 0)
            {
                UserCommand_BanGuildsman(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/reservefightzone", 17) == 0)
            {
                AdminOrder_ReserveFightzone(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/dissmiss ", 9) == 0)
            {
                UserCommand_DissmissGuild(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/attack ", 8) == 0)
            {
                AdminOrder_CallGuard(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/createfish ", 12) == 0)
            {
                AdminOrder_CreateFish(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/teleport ", 10) == 0 || memcmp(cp, "/tp ", 4) == 0)
            {
                AdminOrder_Teleport(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/summondemon ", 13) == 0)
            {
                AdminOrder_SummonDemon(iClientH);
                return;
            }

            if (memcmp(cp, "/unsummonall ", 13) == 0)
            {
                AdminOrder_UnsummonAll(iClientH);
                return;
            }

            if (memcmp(cp, "/unsummondemon ", 15) == 0)
            {
                AdminOrder_UnsummonDemon(iClientH);
                return;
            }

            if (memcmp(cp, "/checkip ", 9) == 0)
            {
                AdminOrder_CheckIP(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/polymorph ", 11) == 0)
            {
                AdminOrder_Polymorph(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/setinvi ", 9) == 0)
            {
                AdminOrder_SetInvi(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/gns ", 4) == 0)
            {
                AdminOrder_GetNpcStatus(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/setattackmode ", 15) == 0)
            {
                AdminOrder_SetAttackMode(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/summon ", 8) == 0)
            {
                AdminOrder_Summon(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/setzerk ", 9) == 0)
            {
                AdminOrder_SetZerk(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/setfreeze ", 11) == 0)
            {
                AdminOrder_SetFreeze(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/setstatus ", 11) == 0)
            {
                AdminOrder_SetStatus(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/disconnectall ", 15) == 0)
            {
                AdminOrder_DisconnectAll(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/createitem ", 12) == 0)
            {
                AdminOrder_CreateItem(iClientH, cp, dwMsgSize - 21);
                return;
            }

            if (memcmp(cp, "/energysphere ", 14) == 0)
            {
                if (m_pClientList[iClientH]->m_iAdminUserLevel >= m_iAdminLevelEnergySphere) EnergySphereProcessor(true, iClientH);
                return;
            }

            if ((memcmp(cp, "/shutdownthisserverrightnow ", 28) == 0) && (m_pClientList[iClientH]->m_iAdminUserLevel >= m_iAdminLevelShutdown))
            {
                m_cShutDownCode = 2;
                m_bOnExitProcess = true;
                m_dwExitProcessTime = timeGetTime();
                log->info("(!) GAME SERVER SHUTDOWN PROCESS BEGIN(by Admin-Command)!!!");
                bSendMsgToLS(MSGID_GAMESERVERSHUTDOWNED, 0);
                // OccupyFlag
                if (m_iMiddlelandMapIndex > 0)
                {
                    // Crusade
                    SaveOccupyFlagData();
                    //bSendMsgToLS(MSGID_REQUEST_SAVEARESDENOCCUPYFLAGDATA, 0, 0);
                    //bSendMsgToLS(MSGID_REQUEST_SAVEELVINEOCCUPYFLAGDATA, 0, 0);
                }
                return;
            }

            if ((memcmp(cp, "/setobservermode ", 17) == 0) && (m_pClientList[iClientH]->m_iAdminUserLevel >= m_iAdminLevelObserver))
            {
                AdminOrder_SetObserverMode(iClientH);
                return;
            }

            if ((memcmp(cp, "/getticket ", 11) == 0) && (m_pClientList[iClientH]->m_iAdminUserLevel >= 2))
            {
                AdminOrder_GetFightzoneTicket(iClientH);
                return;
            }

            /*if (memcmp(cp, "/beginapocalypse ", 17) == 0) {
                if (m_pClientList[iClientH]->m_iAdminUserLevel > 2) {
                    GlobalStartApocalypseMode(iClientH, cp, dwMsgSize - 21);
                }
                return;
            }

            if (memcmp(cp, "/endapocalypse", 14) == 0) {
                if (m_pClientList[iClientH]->m_iAdminUserLevel > 3) {
                    GlobalEndApocalypseMode();
                    copy_string(cTemp, "(%s) GM Order(%s): endapocalypse", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
                    bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
                }
                return;
            }*/

            if (memcmp(cp, "/beginheldenian ", 16) == 0)
            {
                if (m_pClientList[iClientH]->m_iAdminUserLevel > 2)
                {
                    ManualStartHeldenianMode(iClientH, cp, dwMsgSize - 21);
                }
                return;
            }

            if (memcmp(cp, "/endheldenian ", 14) == 0)
            {
                if (m_pClientList[iClientH]->m_iAdminUserLevel > 2)
                {
                    ManualEndHeldenianMode(iClientH, cp, dwMsgSize - 21);
                }
                return;
            }

            return;
    }

    pData[dwMsgSize - 1] = 0;

    if ((m_pClientList[iClientH]->m_cMagicEffectStatus[DEF_MAGICTYPE_CONFUSE] == 1) && (iDice(1, 3) != 2))
    {
        // Confuse Language
        cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 17);

        while (*cp != 0)
        {
            if ((cp[0] != 0) && (cp[0] != ' ') && (cp[1] != 0) && (cp[1] != ' '))
            {
                switch (iDice(1, 3))
                {
                    case 1:	memcpy(cp, "Â¿Ã¶", 2); break;
                    case 2:	memcpy(cp, "Â¿Ã¬", 2); break;
                    case 3:	memcpy(cp, "Â¿Ã¹", 2); break;
                }
                cp += 2;
            }
            else cp++;
        }
    }

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 17);

    if ((cSendMode == 0) && (m_pClientList[iClientH]->m_iWhisperPlayerIndex != -1))
    {
        cSendMode = 20;

        if (*cp == '#') cSendMode = 0;

        if (m_pClientList[iClientH]->m_iTimeLeft_ShutUp > 0) cSendMode = 0;
    }

    wp = (uint16_t *)(pData + DEF_INDEX2_MSGTYPE);
    *wp = (uint16_t)iClientH;
    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 16);
    *cp = cSendMode;

    if (cSendMode != 20)
    {
        for (i = 1; i < DEF_MAXCLIENTS; i++)
            if (m_pClientList[i] != 0)
            {
                switch (cSendMode)
                {
                    case 0:
                        if (m_pClientList[i]->m_bIsInitComplete == false) break;

                        if ((m_pClientList[i]->m_cMapIndex == m_pClientList[iClientH]->m_cMapIndex) &&
                            (m_pClientList[i]->m_sX > m_pClientList[iClientH]->m_sX - 10) &&
                            (m_pClientList[i]->m_sX < m_pClientList[iClientH]->m_sX + 10) &&
                            (m_pClientList[i]->m_sY > m_pClientList[iClientH]->m_sY - 7) &&
                            (m_pClientList[i]->m_sY < m_pClientList[iClientH]->m_sY + 7))
                        {

                            // Crusade
                            if (m_bIsCrusadeMode == true)
                            {
                                if ((m_pClientList[iClientH]->m_cSide != 0) && (m_pClientList[i]->m_cSide != 0) &&
                                    (m_pClientList[i]->m_cSide != m_pClientList[iClientH]->m_cSide))
                                {
                                }
                                else iRet = m_pClientList[i]->send_msg(pData, dwMsgSize);
                            }
                            else iRet = m_pClientList[i]->send_msg(pData, dwMsgSize);
                        }
                        break;

                    case 1:
                        if (m_pClientList[i]->m_bIsInitComplete == false) break;

                        if ((memcmp(m_pClientList[i]->m_cGuildName, m_pClientList[iClientH]->m_cGuildName, 20) == 0) &&
                            (memcmp(m_pClientList[i]->m_cGuildName, "NONE", 4) != 0))
                        {

                            // Crusade
                            if (m_bIsCrusadeMode == true)
                            {
                                if ((m_pClientList[iClientH]->m_cSide != 0) && (m_pClientList[i]->m_cSide != 0) &&
                                    (m_pClientList[i]->m_cSide != m_pClientList[iClientH]->m_cSide))
                                {
                                }
                                else iRet = m_pClientList[i]->send_msg(pData, dwMsgSize);
                            }
                            else iRet = m_pClientList[i]->send_msg(pData, dwMsgSize);
                        }
                        break;

                    case 2:
                    case 10:
                        // Crusade
                        if (m_bIsCrusadeMode == true)
                        {
                            if ((m_pClientList[iClientH]->m_cSide != 0) && (m_pClientList[i]->m_cSide != 0) &&
                                (m_pClientList[i]->m_cSide != m_pClientList[iClientH]->m_cSide))
                            {
                            }
                            else iRet = m_pClientList[i]->send_msg(pData, dwMsgSize);
                        }
                        else iRet = m_pClientList[i]->send_msg(pData, dwMsgSize);
                        break;

                    case 3:
                        if (m_pClientList[i]->m_bIsInitComplete == false) break;

                        if ((m_pClientList[i]->m_cSide == m_pClientList[iClientH]->m_cSide))
                            iRet = m_pClientList[i]->send_msg(pData, dwMsgSize);
                        break;

                    case 4:
                        if (m_pClientList[i]->m_bIsInitComplete == false) break;
                        if ((m_pClientList[i]->m_iPartyID != 0) && (m_pClientList[i]->m_iPartyID == m_pClientList[iClientH]->m_iPartyID))
                            iRet = m_pClientList[i]->send_msg(pData, dwMsgSize);
                        break;
                }

                switch (iRet)
                {
                    case DEF_XSOCKEVENT_QUENEFULL:
                    case DEF_XSOCKEVENT_SOCKETERROR:
                    case DEF_XSOCKEVENT_CRITICALERROR:
                    case DEF_XSOCKEVENT_SOCKETCLOSED:
                        //DeleteClient(i, true, true);
                        break;
                }
            }
    }
    else
    {
        // New 16/05/2004
        iRet = m_pClientList[iClientH]->send_msg(pData, dwMsgSize);
        if (m_pClientList[iClientH]->m_iWhisperPlayerIndex == 10000)
        {
            memset(cBuffer, 0, sizeof(cBuffer));
            cp = (char *)cBuffer;
            *cp = GSM_WHISFERMSG;
            cp++;
            memcpy(cp, m_pClientList[iClientH]->m_cWhisperPlayerName, 10);
            cp += 10;

            wp = (uint16_t *)cp;
            *wp = (uint16_t)dwMsgSize;
            cp += 2;
            memcpy(cp, pData, dwMsgSize);
            cp += dwMsgSize;
            bStockMsgToGateServer(cBuffer, dwMsgSize + 13);

            //testcode
            //copy_string(G_cTxt, "Sending Whisper Msg: %s %d", m_pClientList[iClientH]->m_cWhisperPlayerName, (13 +dwMsgSize));
            //log->info(G_cTxt);
        }
        else
        {
            if (m_pClientList[m_pClientList[iClientH]->m_iWhisperPlayerIndex] != 0 &&
                strcmp(m_pClientList[iClientH]->m_cWhisperPlayerName, m_pClientList[m_pClientList[iClientH]->m_iWhisperPlayerIndex]->m_cCharName) == 0)
            {
                iRet = m_pClientList[m_pClientList[iClientH]->m_iWhisperPlayerIndex]->send_msg(pData, dwMsgSize);
                switch (m_bLogChatOption)
                {

                    case 1:
                        if (m_pClientList[m_pClientList[iClientH]->m_iWhisperPlayerIndex]->m_iAdminUserLevel == 0)
                        {
                            ZeroMemory(cTemp, sizeof(cTemp));
                            copy_string(cTemp, "GM Whisper   (%s):\"%s\"\tto Player(%s)", m_pClientList[iClientH]->m_cCharName, pData + 21, m_pClientList[iClientH]->m_cWhisperPlayerName);
                            bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
                        }
                        break;

                    case 2:
                        if (m_pClientList[m_pClientList[iClientH]->m_iWhisperPlayerIndex]->m_iAdminUserLevel > 0)
                        {
                            ZeroMemory(cTemp, sizeof(cTemp));
                            copy_string(cTemp, "GM Whisper   (%s):\"%s\"\tto GM(%s)", m_pClientList[iClientH]->m_cCharName, pData + 21, m_pClientList[iClientH]->m_cWhisperPlayerName);
                            bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
                        }
                        break;

                    case 3:
                        if (m_pClientList[m_pClientList[iClientH]->m_iWhisperPlayerIndex]->m_iAdminUserLevel > 0)
                        {
                            ZeroMemory(cTemp, sizeof(cTemp));
                            copy_string(cTemp, "GM Whisper   (%s):\"%s\"\tto GM(%s)", m_pClientList[iClientH]->m_cCharName, pData + 21, m_pClientList[iClientH]->m_cWhisperPlayerName);
                            bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
                        }
                        else
                        {
                            ZeroMemory(cTemp, sizeof(cTemp));
                            copy_string(cTemp, "Player Whisper   (%s):\"%s\"\tto Player(%s)", m_pClientList[iClientH]->m_cCharName, pData + 21, m_pClientList[iClientH]->m_cWhisperPlayerName);
                            bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
                        }
                        break;

                    case 4:
                        break;
                }
            }
        }

        switch (iRet)
        {
            case DEF_XSOCKEVENT_QUENEFULL:
            case DEF_XSOCKEVENT_SOCKETERROR:
            case DEF_XSOCKEVENT_CRITICALERROR:
            case DEF_XSOCKEVENT_SOCKETCLOSED:
                //DeleteClient(i, true, true);
                break;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////
//  int CGame::iClientMotion_Attack_Handler(int iClientH, short sX, short sY, short dX, short dY, short wType, char cDir, uint16_t wTargetObjectID, bool bResponse, bool bIsDash)
//  description			:: controls player attack
//	return value		:: int
//  last updated		:: October 29, 2004; 8:06 PM; Hypnotoad
//  commentary			:: - contains attack hack detection
//						   - added checks for Firebow and Directionbow to see if player is m_bIsInsideWarehouse, m_bIsInsideWizardTower, m_bIsInsideOwnTown 
//						   - added ability to attack moving object
//						   - fixed attack unmoving object
/////////////////////////////////////////////////////////////////////////////////////
// Incomplete: 
//			- Direction Bow damage disabled
//
/////////////////////////////////////////////////////////////////////////////////////
int CGame::iClientMotion_Attack_Handler(int iClientH, short sX, short sY, short dX, short dY, short wType, char cDir, uint16_t wTargetObjectID, bool bResponse, bool bIsDash)
{
    char cData[100];
    uint32_t * dwp, dwTime;
    uint16_t * wp;
    int     iRet, iExp, tdX = 0, tdY = 0, i;
    short   sOwner, sAbsX, sAbsY;
    char    cOwnerType;
    bool    bNearAttack = false, var_AC = false;
    short sItemIndex;
    int tX, tY, iErr, iStX, iStY;

    if (m_pClientList[iClientH] == nullptr) return 0;
    if ((cDir <= 0) || (cDir > 8))       return 0;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return 0;
    if (m_pClientList[iClientH]->m_bIsKilled == true) return 0;

    dwTime = timeGetTime();
    m_pClientList[iClientH]->m_dwLastActionTime = dwTime;
    m_pClientList[iClientH]->m_iAttackMsgRecvCount++;
    if (m_pClientList[iClientH]->m_iAttackMsgRecvCount >= 7)
    {
        if (m_pClientList[iClientH]->m_dwAttackLAT != 0)
        {
            if ((dwTime - m_pClientList[iClientH]->m_dwAttackLAT) < (3500))
            {
                DeleteClient(iClientH, true, true, true);
                return 0;
            }
        }
        m_pClientList[iClientH]->m_dwAttackLAT = dwTime;
        m_pClientList[iClientH]->m_iAttackMsgRecvCount = 0;
    }

    if ((wTargetObjectID != 0) && (wType != 2))
    {
        if (wTargetObjectID < DEF_MAXCLIENTS)
        {
            if (m_pClientList[wTargetObjectID] != 0)
            {
                tdX = m_pClientList[wTargetObjectID]->m_sX;
                tdY = m_pClientList[wTargetObjectID]->m_sY;
            }
        }
        else if ((wTargetObjectID > 10000) && (wTargetObjectID < (10000 + DEF_MAXNPCS)))
        {
            if (m_pNpcList[wTargetObjectID - 10000] != 0)
            {
                tdX = m_pNpcList[wTargetObjectID - 10000]->m_sX;
                tdY = m_pNpcList[wTargetObjectID - 10000]->m_sY;
            }
        }

        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwner, &cOwnerType, dX, dY);
        if (sOwner == (wTargetObjectID - 10000))
        {
            tdX = m_pNpcList[sOwner]->m_sX;
            dX = tdX;
            tdY = m_pNpcList[sOwner]->m_sY;
            dY = tdY;
            bNearAttack = false;
            var_AC = true;
        }
        if (var_AC != true)
        {
            if ((tdX == dX) && (tdY == dY))
            {
                bNearAttack = false;
            }
            else if ((abs(tdX - dX) <= 1) && (abs(tdY - dY) <= 1))
            {
                dX = tdX;
                dY = tdY;
                bNearAttack = true;
            }
        }
    }

    if ((dX < 0) || (dX >= m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_sSizeX) ||
        (dY < 0) || (dY >= m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_sSizeY)) return 0;

    if ((sX != m_pClientList[iClientH]->m_sX) || (sY != m_pClientList[iClientH]->m_sY)) return 2;

    if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex] != 0)
    {
        iStX = m_pClientList[iClientH]->m_sX / 20;
        iStY = m_pClientList[iClientH]->m_sY / 20;
        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iPlayerActivity++;

        switch (m_pClientList[iClientH]->m_cSide)
        {
            case 0: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iNeutralActivity++; break;
            case 1: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iAresdenActivity++; break;
            case 2: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iElvineActivity++;  break;
        }
    }

    sAbsX = abs(sX - dX);
    sAbsY = abs(sY - dY);
    if ((wType != 2) && (wType < 20))
    {
        if (var_AC == false)
        {
            sItemIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
            if (sItemIndex != -1)
            {
                if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == 0) return 0;
                if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 845)
                {
                    if ((sAbsX > 4) || (sAbsY > 4)) wType = 0;
                }
                else
                {
                    if ((sAbsX > 1) || (sAbsY > 1)) wType = 0;
                }
            }
            else
            {
                if ((sAbsX > 1) || (sAbsY > 1)) wType = 0;
            }
        }
        else
        {
            cDir = m_Misc.cGetNextMoveDir(sX, sY, dX, dY);
            if ((m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bCheckFlySpaceAvailable(sX, sY, cDir, sOwner)) != false)
                wType = 0;
        }
    }

    ClearSkillUsingStatus(iClientH);
    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->ClearOwner(0, iClientH, DEF_OWNERTYPE_PLAYER, sX, sY);
    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetOwner(iClientH, DEF_OWNERTYPE_PLAYER, sX, sY);

    m_pClientList[iClientH]->m_cDir = cDir;

    iExp = 0;
    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwner, &cOwnerType, dX, dY);

    if (sOwner != 0)
    {
        if ((wType != 0) && ((dwTime - m_pClientList[iClientH]->m_dwRecentAttackTime) > 100))
        {
            if ((m_pClientList[iClientH]->m_pIsProcessingAllowed == false) && (m_pClientList[iClientH]->m_bIsInsideWarehouse == false)
                && (m_pClientList[iClientH]->m_bIsInsideWizardTower == false) && (m_pClientList[iClientH]->m_bIsInsideOwnTown == false))
            {
                sItemIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
                if (sItemIndex != -1 && m_pClientList[iClientH]->m_pItemList[sItemIndex] != 0)
                {
                    if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 874)
                    { // Directional bow
                        for (i = 2; i < 10; i++)
                        {
                            iErr = 0;
                            m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);
                            m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwner, &cOwnerType, tX, tY);
                            //iExp += iCalculateAttackEffect(sOwner, cOwnerType, iClientH, DEF_OWNERTYPE_PLAYER, tX, tY, wType, bNearAttack, bIsDash, true); // 1
                            if ((abs(tdX - dX) <= 1) && (abs(tdY - dY) <= 1))
                            {
                                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwner, &cOwnerType, dX, dY);
                                //iExp += iCalculateAttackEffect(sOwner, cOwnerType, iClientH, DEF_OWNERTYPE_PLAYER, dX, dY, wType, bNearAttack, bIsDash, false); // 0
                            }
                        }
                    }
                    else if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 873)
                    { // Firebow
                        if ((m_pClientList[iClientH]->m_sAppr2 & 0xF000) != 0)
                        {
                            if (m_bHeldenianInitiated != 1)
                            {
                                iAddDynamicObjectList(iClientH, DEF_OWNERTYPE_PLAYER_INDIRECT, DEF_DYNAMICOBJECT_FIRE3, m_pClientList[iClientH]->m_cMapIndex, dX, dY, (iDice(1, 7) + 3) * 1000, 8);
                            }
                            iExp += iCalculateAttackEffect(sOwner, cOwnerType, iClientH, DEF_OWNERTYPE_PLAYER, dX, dY, wType, bNearAttack, bIsDash, false); // 0
                        }
                    }
                    else
                    {
                        iExp += iCalculateAttackEffect(sOwner, cOwnerType, iClientH, DEF_OWNERTYPE_PLAYER, dX, dY, wType, bNearAttack, bIsDash, false); // 0
                    }
                }
                else
                {
                    iExp += iCalculateAttackEffect(sOwner, cOwnerType, iClientH, DEF_OWNERTYPE_PLAYER, dX, dY, wType, bNearAttack, bIsDash, false); // 0
                }
            }
            else
            {
                iExp += iCalculateAttackEffect(sOwner, cOwnerType, iClientH, DEF_OWNERTYPE_PLAYER, dX, dY, wType, bNearAttack, bIsDash, false); // 0
            }
            if (m_pClientList[iClientH] == nullptr) return 0;
            m_pClientList[iClientH]->m_dwRecentAttackTime = dwTime;
        }
    }
    else _CheckMiningAction(iClientH, dX, dY);

    if (iExp != 0)
    {
        GetExp(iClientH, iExp, true);
    }

    if (bResponse == true)
    {
        dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
        *dwp = MSGID_RESPONSE_MOTION;
        wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
        *wp = DEF_OBJECTMOTION_ATTACK_CONFIRM;

        iRet = m_pClientList[iClientH]->send_msg(cData, 6);
        switch (iRet)
        {
            case DEF_XSOCKEVENT_QUENEFULL:
            case DEF_XSOCKEVENT_SOCKETERROR:
            case DEF_XSOCKEVENT_CRITICALERROR:
            case DEF_XSOCKEVENT_SOCKETCLOSED:
                DeleteClient(iClientH, true, true);
                return 0;
        }
    }

    return 1;
}

void CGame::NpcKilledHandler(short sAttackerH, char cAttackerType, int iNpcH, short sDamage)
{
    short  sAttackerWeapon;
    int * ip, i, iQuestIndex, iExp, iConstructionPoint, iWarContribution, iMapIndex;
    double dTmp1, dTmp2, dTmp3;
    char * cp, cData[120], cQuestRemain;

    if (m_pNpcList[iNpcH] == 0) return;
    if (m_pNpcList[iNpcH]->m_bIsKilled == true) return;

    m_pNpcList[iNpcH]->m_bIsKilled = true;
    m_pNpcList[iNpcH]->m_iHP = 0;
    m_pNpcList[iNpcH]->m_iLastDamage = sDamage;

    m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_iTotalAliveObject--;

    RemoveFromTarget(iNpcH, DEF_OWNERTYPE_NPC);

    ReleaseFollowMode(iNpcH, DEF_OWNERTYPE_NPC);

    m_pNpcList[iNpcH]->m_iTargetIndex = 0;
    m_pNpcList[iNpcH]->m_cTargetType = 0;
    if (cAttackerType == DEF_OWNERTYPE_PLAYER)
    {
        sAttackerWeapon = ((m_pClientList[sAttackerH]->m_sAppr2 & 0x0FF0) >> 4);
    }
    else sAttackerWeapon = 1;
    SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTDYING, sDamage, sAttackerWeapon, 0);
    m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearOwner(10, iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY);
    m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->SetDeadOwner(iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY);
    m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_DEAD;

    m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
    m_pNpcList[iNpcH]->m_dwDeadTime = timeGetTime();

    if (m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_cType == DEF_MAPTYPE_NOPENALTY_NOREWARD) return;

    NpcDeadItemGenerator(iNpcH, sAttackerH, cAttackerType);

    if ((m_pNpcList[iNpcH]->m_bIsSummoned != true) && (cAttackerType == DEF_OWNERTYPE_PLAYER) &&
        (m_pClientList[sAttackerH] != 0))
    {
        iExp = (m_pNpcList[iNpcH]->m_iExp / 3);
        if (m_pNpcList[iNpcH]->m_iNoDieRemainExp > 0)
            iExp += m_pNpcList[iNpcH]->m_iNoDieRemainExp;

        if (m_pClientList[sAttackerH]->m_iAddExp != 0)
        {
            dTmp1 = (double)m_pClientList[sAttackerH]->m_iAddExp;
            dTmp2 = (double)iExp;
            dTmp3 = (dTmp1 / 100.0f) * dTmp2;
            iExp += (int)dTmp3;
        }

        if (m_pNpcList[iNpcH]->m_sType == 81)
        {
            for (i = 1; i < DEF_MAXCLIENTS; i++)
            {
                if (m_pClientList[i] != 0)
                {
                    SendNotifyMsg(sAttackerH, i, DEF_NOTIFY_ABADDONKILLED, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
                }
            }
        }

        if (m_bIsCrusadeMode == true)
        {
            if (iExp > 10) iExp = iExp / 3;
        }

        // New 09/05/2004
        //m_pClientList[sAttackerH]->m_iExpStock += iExp;
        GetExp(sAttackerH, iExp, true);

        // Quest
        iQuestIndex = m_pClientList[sAttackerH]->m_iQuest;
        if (iQuestIndex != 0)
        {
            if (m_pQuestConfigList[iQuestIndex] != 0)
            {
                switch (m_pQuestConfigList[iQuestIndex]->m_iType)
                {
                    case DEF_QUESTTYPE_MONSTERHUNT:
                        if ((m_pClientList[sAttackerH]->m_bQuestMatchFlag_Loc == true) &&
                            (m_pQuestConfigList[iQuestIndex]->m_iTargetType == m_pNpcList[iNpcH]->m_sType))
                        {
                            m_pClientList[sAttackerH]->m_iCurQuestCount++;
                            cQuestRemain = (m_pQuestConfigList[m_pClientList[sAttackerH]->m_iQuest]->m_iMaxCount - m_pClientList[sAttackerH]->m_iCurQuestCount);
                            SendNotifyMsg(0, sAttackerH, DEF_NOTIFY_QUESTCOUNTER, cQuestRemain, 0, 0, 0);
                            _bCheckIsQuestCompleted(sAttackerH);
                        }
                        break;
                }
            }
        }
    }

    // v1.41
    if (cAttackerType == DEF_OWNERTYPE_PLAYER)
    {
        switch (m_pNpcList[iNpcH]->m_sType)
        {
            case 32:
                m_pClientList[sAttackerH]->m_iRating -= 5;
                if (m_pClientList[sAttackerH]->m_iRating < -10000) m_pClientList[sAttackerH]->m_iRating = 0;
                if (m_pClientList[sAttackerH]->m_iRating > 10000) m_pClientList[sAttackerH]->m_iRating = 0;
                break;

            case 33:
                break;
        }
    }

    // Crusade
    iConstructionPoint = 0;
    switch (m_pNpcList[iNpcH]->m_sType)
    {
        case 1:  iConstructionPoint = 50; iWarContribution = 100; break;
        case 2:  iConstructionPoint = 50; iWarContribution = 100; break;
        case 3:  iConstructionPoint = 50; iWarContribution = 100; break;
        case 4:  iConstructionPoint = 50; iWarContribution = 100; break;
        case 5:  iConstructionPoint = 50; iWarContribution = 100; break;
        case 6:  iConstructionPoint = 50; iWarContribution = 100; break;
        case 36: iConstructionPoint = 700; iWarContribution = 4000; break;
        case 37: iConstructionPoint = 700; iWarContribution = 4000; break;
        case 38: iConstructionPoint = 500; iWarContribution = 2000; break;
        case 39: iConstructionPoint = 500; iWarContribution = 2000; break;
        case 40: iConstructionPoint = 1500; iWarContribution = 5000; break;
        case 41: iConstructionPoint = 5000; iWarContribution = 10000; break;
        case 43: iConstructionPoint = 500; iWarContribution = 1000; break;
        case 44: iConstructionPoint = 1000; iWarContribution = 2000; break;
        case 45: iConstructionPoint = 1500; iWarContribution = 3000; break;
        case 46: iConstructionPoint = 1000; iWarContribution = 2000; break;
        case 47: iConstructionPoint = 1500; iWarContribution = 3000; break;
        case 64: m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->bRemoveCropsTotalSum(); break;

    }

    // Crusade
    if (iConstructionPoint != 0)
    {
        switch (cAttackerType)
        {
            case DEF_OWNERTYPE_PLAYER:
                if (m_pClientList[sAttackerH]->m_cSide != m_pNpcList[iNpcH]->m_cSide)
                {
                    m_pClientList[sAttackerH]->m_iConstructionPoint += iConstructionPoint;

                    if (m_pClientList[sAttackerH]->m_iConstructionPoint > DEF_MAXCONSTRUCTIONPOINT)
                        m_pClientList[sAttackerH]->m_iConstructionPoint = DEF_MAXCONSTRUCTIONPOINT;

                    m_pClientList[sAttackerH]->m_iWarContribution += iWarContribution;
                    if (m_pClientList[sAttackerH]->m_iWarContribution > DEF_MAXWARCONTRIBUTION)
                        m_pClientList[sAttackerH]->m_iWarContribution = DEF_MAXWARCONTRIBUTION;

                    //testcode
                    copy_string(G_cTxt, "Enemy Npc Killed by player! Construction: +%d WarContribution: +%d", iConstructionPoint, iWarContribution);
                    log->info(G_cTxt);

                    SendNotifyMsg(0, sAttackerH, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[sAttackerH]->m_iConstructionPoint, m_pClientList[sAttackerH]->m_iWarContribution, 0, 0);
                }
                else
                {
                    m_pClientList[sAttackerH]->m_iWarContribution -= (iWarContribution * 2);
                    if (m_pClientList[sAttackerH]->m_iWarContribution < 0)
                        m_pClientList[sAttackerH]->m_iWarContribution = 0;

                    //testcode
                    copy_string(G_cTxt, "Friendly Npc Killed by player! WarContribution: -%d", iWarContribution);
                    log->info(G_cTxt);

                    SendNotifyMsg(0, sAttackerH, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[sAttackerH]->m_iConstructionPoint, m_pClientList[sAttackerH]->m_iWarContribution, 0, 0);
                }
                break;

            case DEF_OWNERTYPE_NPC:
                if (m_pNpcList[sAttackerH]->m_iGuildGUID != 0)
                {
                    if (m_pNpcList[sAttackerH]->m_cSide != m_pNpcList[iNpcH]->m_cSide)
                    {
                        for (i = 1; i < DEF_MAXCLIENTS; i++)
                            if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_iGuildGUID == m_pNpcList[sAttackerH]->m_iGuildGUID) &&
                                (m_pClientList[i]->m_iCrusadeDuty == 3))
                            {

                                m_pClientList[i]->m_iConstructionPoint += iConstructionPoint;
                                if (m_pClientList[i]->m_iConstructionPoint > DEF_MAXCONSTRUCTIONPOINT)
                                    m_pClientList[i]->m_iConstructionPoint = DEF_MAXCONSTRUCTIONPOINT;

                                //testcode
                                copy_string(G_cTxt, "Enemy Npc Killed by Npc! Construct point +%d", iConstructionPoint);
                                log->info(G_cTxt);
                                SendNotifyMsg(0, i, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[i]->m_iConstructionPoint, m_pClientList[i]->m_iWarContribution, 0, 0);
                                goto NKH_GOTOPOINT1;
                            }

                        memset(cData, 0, sizeof(cData));
                        cp = (char *)cData;
                        *cp = GSM_CONSTRUCTIONPOINT;
                        cp++;
                        ip = (int *)cp;
                        *ip = m_pNpcList[sAttackerH]->m_iGuildGUID;
                        cp += 4;
                        ip = (int *)cp;
                        *ip = iConstructionPoint;
                        cp += 4;
                        bStockMsgToGateServer(cData, 9);
                    }
                }
                break;
        }
    }

    NKH_GOTOPOINT1:;

    // v1.411 Explosive
    if (m_pNpcList[iNpcH]->m_cSpecialAbility == 7)
    {
        m_pNpcList[iNpcH]->m_iMana = 100;
        m_pNpcList[iNpcH]->m_iMagicHitRatio = 100;
        NpcMagicHandler(iNpcH, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, 30);
    }
    else if (m_pNpcList[iNpcH]->m_cSpecialAbility == 8)
    {
        m_pNpcList[iNpcH]->m_iMana = 100;
        m_pNpcList[iNpcH]->m_iMagicHitRatio = 100;
        NpcMagicHandler(iNpcH, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, 61);
    }

    /*if (m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_iTotalAliveObject == 0) && (m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_iApocalypseMobGenType  == 2) {
        //sub_428CD0
    }

    if (m_pNpcList[]->m_bIsApocalypseBoss == true) {
        for (i = 1; i < DEF_MAXCLIENTS; i++)
        var_BC = m_pNpcList[iNpcH]->m_cMapIndex;
        if (m_pClientList[i] != 0) && (m_pClientList[i]->m_bIsInitComplete == true) && (var_BC >= 0) && (var_BC < DEF_MAXMAPS) {
            if ((m_pMapList[var_BC] != 0) && (m_pMapList[var_BC]->m_bRecallImpossible == true)) {
                m_pClientList[]->m_iTimeLeft_ForceRecall = 200;
                m_pClientList[i]->m_bIsImpossibleZone = true;
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_FORCERECALLTIME,  m_pClientList[iClientH]->m_iTimeLeft_ForceRecall , 0, 0, 0);
            }
        }
    }*/

    if ((m_bIsHeldenianMode == true) && (m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_bIsHeldenianMap == true) && (m_cHeldenianModeType == 1))
    {
        iMapIndex = 0;
        iMapIndex = m_pNpcList[m_pNpcList[iNpcH]->m_cMapIndex]->m_cMapIndex;
        if ((m_pNpcList[iNpcH]->m_sType == 87) || (m_pNpcList[iNpcH]->m_sType == 89))
        {
            if (m_pNpcList[m_pNpcList[iNpcH]->m_cMapIndex]->m_cSide == 1)
            {
                m_iHeldenianAresdenLeftTower--;
                copy_string(G_cTxt, "Aresden Tower Broken, Left TOWER %d", m_iHeldenianAresdenLeftTower);
            }
            else if (m_pNpcList[m_pNpcList[iNpcH]->m_cMapIndex]->m_cSide == 2)
            {
                m_iHeldenianElvineLeftTower--;
                copy_string(G_cTxt, "Elvine Tower Broken, Left TOWER %d", m_iHeldenianElvineLeftTower);
            }
            log->info(G_cTxt);
            UpdateHeldenianStatus();
        }
        if ((m_iHeldenianElvineLeftTower == 0) || (m_iHeldenianAresdenLeftTower == 0))
        {
            GlobalEndHeldenianMode();
        }
    }
}

void CGame::ClientCommonHandler(int iClientH, char * pData)
{
    uint16_t * wp, wCommand;
    short * sp, sX, sY;
    int * ip, iV1, iV2, iV3, iV4;
    char * cp, cDir, * pString;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    if (m_pClientList[iClientH]->m_bIsKilled == true) return;

    wp = (uint16_t *)(pData + DEF_INDEX2_MSGTYPE);
    wCommand = *wp;

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

    sp = (short *)cp;
    sX = *sp;
    cp += 2;

    sp = (short *)cp;
    sY = *sp;
    cp += 2;

    cDir = *cp;
    cp++;

    ip = (int *)cp;
    iV1 = *ip;
    cp += 4;

    ip = (int *)cp;
    iV2 = *ip;
    cp += 4;

    ip = (int *)cp;
    iV3 = *ip;
    cp += 4;

    pString = cp;
    cp += 30;

    ip = (int *)cp;
    iV4 = *ip;
    cp += 4;

    switch (wCommand)
    {

        // New 15/05/2004
        case DEF_COMMONTYPE_REQ_CREATESLATE:
            ReqCreateSlateHandler(iClientH, pData);
            break;

            // 2.06 - by KLKS
        case DEF_COMMONTYPE_REQ_CHANGEPLAYMODE:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> MSGID_REQUEST_CIVILRIGHT");
            RequestChangePlayMode(iClientH);
            break;
            //

        case DEF_COMMONTYPE_SETGUILDTELEPORTLOC:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_SETGUILDTELEPORTLOC");
            RequestSetGuildTeleportLocHandler(iClientH, iV1, iV2, m_pClientList[iClientH]->m_iGuildGUID, "middleland");
            break;

        case DEF_COMMONTYPE_SETGUILDCONSTRUCTLOC:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_SETGUILDCONSTRUCTLOC");
            RequestSetGuildConstructLocHandler(iClientH, iV1, iV2, m_pClientList[iClientH]->m_iGuildGUID, pString);
            break;

        case DEF_COMMONTYPE_GUILDTELEPORT:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_GUILDTELEPORT");
            RequestGuildTeleportHandler(iClientH);
            break;

        case DEF_COMMONTYPE_SUMMONWARUNIT:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_SUMMONWARUNIT");
            RequestSummonWarUnitHandler(iClientH, sX, sY, iV1, iV2, iV3);
            break;

        case DEF_COMMONTYPE_REQUEST_HELP:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_HELP");
            RequestHelpHandler(iClientH);
            break;

        case DEF_COMMONTYPE_REQUEST_MAPSTATUS:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_MAPSTATUS");
            MapStatusHandler(iClientH, iV1, pString);
            break;

        case DEF_COMMONTYPE_REQUEST_SELECTCRUSADEDUTY:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_SELECTCRUSADEDUTY");
            SelectCrusadeDutyHandler(iClientH, iV1);
            break;

        case DEF_COMMONTYPE_REQUEST_CANCELQUEST:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_CANCELQUEST");
            CancelQuestHandler(iClientH);
            break;

        case DEF_COMMONTYPE_REQUEST_ACTIVATESPECABLTY:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_ACTIVATESPECABLTY");
            ActivateSpecialAbilityHandler(iClientH);
            break;

        case DEF_COMMONTYPE_REQUEST_JOINPARTY:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_JOINPARTY");
            JoinPartyHandler(iClientH, iV1, pString);
            break;

        case DEF_COMMONTYPE_GETMAGICABILITY:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_GETMAGICABILITY");
            GetMagicAbilityHandler(iClientH);
            break;

        case DEF_COMMONTYPE_BUILDITEM:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_BUILDITEM");
            BuildItemHandler(iClientH, pData);
            break;

        case DEF_COMMONTYPE_QUESTACCEPTED:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_QUESTACCEPTED");
            QuestAcceptedHandler(iClientH);
            break;

        case DEF_COMMONTYPE_CANCELEXCHANGEITEM:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_CANCELEXCHANGEITEM");
            CancelExchangeItem(iClientH);
            break;

        case DEF_COMMONTYPE_CONFIRMEXCHANGEITEM:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_CONFIRMEXCHANGEITEM");
            ConfirmExchangeItem(iClientH);
            break;

        case DEF_COMMONTYPE_SETEXCHANGEITEM:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_SETEXCHANGEITEM");
            SetExchangeItem(iClientH, iV1, iV2);
            break;

        case DEF_COMMONTYPE_REQ_GETHEROMANTLE:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_GETHEROMANTLE");
            GetHeroMantleHandler(iClientH, iV1, pString);
            break;

        case DEF_COMMONTYPE_REQ_GETOCCUPYFLAG:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_GETOCCUPYFLAG");
            GetOccupyFlagHandler(iClientH);
            break;

        case DEF_COMMONTYPE_REQ_SETDOWNSKILLINDEX:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_SETDOWNSKILLINDEX");
            SetDownSkillIndexHandler(iClientH, iV1);
            break;

        case DEF_COMMONTYPE_TALKTONPC:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_TALKTONPC");
            // works for client, but for npc it returns middleland
            // if ((m_pMapList[m_pNpcList[iV1]->m_cMapIndex]->m_cName) != (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName)) break;
            NpcTalkHandler(iClientH, iV1);
            break;

        case DEF_COMMONTYPE_REQ_CREATEPORTION:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_CREATEPORTION");
            ReqCreatePortionHandler(iClientH, pData);
            break;

        case DEF_COMMONTYPE_REQ_GETFISHTHISTIME:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_GETFISHTHISTIME");
            ReqGetFishThisTimeHandler(iClientH);
            break;

        case DEF_COMMONTYPE_REQ_REPAIRITEMCONFIRM:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_REPAIRITEMCONFIRM");
            ReqRepairItemCofirmHandler(iClientH, iV1, pString);
            break;

        case DEF_COMMONTYPE_REQ_REPAIRITEM:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_REPAIRITEM");
            ReqRepairItemHandler(iClientH, iV1, iV2, pString);
            break;

        case DEF_COMMONTYPE_REQ_SELLITEMCONFIRM:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_SELLITEMCONFIRM");
            ReqSellItemConfirmHandler(iClientH, iV1, iV2, pString);
            break;

        case DEF_COMMONTYPE_REQ_SELLITEM:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_SELLITEM");
            ReqSellItemHandler(iClientH, iV1, iV2, iV3, pString);
            break;

        case DEF_COMMONTYPE_REQ_USESKILL:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_USESKILL");
            UseSkillHandler(iClientH, iV1, iV2, iV3);
            break;

        case DEF_COMMONTYPE_REQ_USEITEM:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_USEITEM");
            UseItemHandler(iClientH, iV1, iV2, iV3, iV4);
            break;

        case DEF_COMMONTYPE_REQ_GETREWARDMONEY:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_GETREWARDMONEY");
            GetRewardMoneyHandler(iClientH);
            break;

        case DEF_COMMONTYPE_ITEMDROP:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_ITEMDROP");
            DropItemHandler(iClientH, iV1, iV2, pString, true);
            break;

        case DEF_COMMONTYPE_EQUIPITEM:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_EQUIPITEM");
            // Â¿Â©Â±Ã¢Â¼Â­Â´Ã‚ Ã†Â¯Â¼ÂºÃ„Â¡ ÂºÂ¯ÃˆÂ­Â¸Â¸Ã€Â» Â´Ã™Â·Ã©Â´Ã™.
            bEquipItemHandler(iClientH, iV1);
            // Â¹Ã™Â²Ã¯ Â¿ÃœÃ‡Ã¼Â¿Â¡ Â´Ã«Ã‡Ã‘ ÃÂ¤ÂºÂ¸Â¸Â¦ Â¿Â©Â±Ã¢Â¼Â­ Ã€Ã¼Â¼Ã›Ã‡Ã‘Â´Ã™.
            // .....
            break;

        case DEF_COMMONTYPE_REQ_PURCHASEITEM:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_PURCHASEITEM");
            RequestPurchaseItemHandler(iClientH, pString, iV1);
            break;

        case DEF_COMMONTYPE_REQ_STUDYMAGIC:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_STUDYMAGIC");
            RequestStudyMagicHandler(iClientH, pString);
            break;

        case DEF_COMMONTYPE_REQ_TRAINSKILL:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_TRAINSKILL");
            //RequestTrainSkillHandler(iClientH, pString);
            break;

        case DEF_COMMONTYPE_GIVEITEMTOCHAR:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_GIVEITEMTOCHAR");
            GiveItemHandler(iClientH, cDir, iV1, iV2, iV3, iV4, pString);
            break;

        case DEF_COMMONTYPE_EXCHANGEITEMTOCHAR:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_EXCHANGEITEMTOCHAR");
            ExchangeItemHandler(iClientH, cDir, iV1, iV2, iV3, iV4, pString);
            break;

        case DEF_COMMONTYPE_JOINGUILDAPPROVE:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_JOINGUILDAPPROVE");
            JoinGuildApproveHandler(iClientH, pString);
            break;

        case DEF_COMMONTYPE_JOINGUILDREJECT:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_JOINGUILDREJECT");
            JoinGuildRejectHandler(iClientH, pString);
            break;

        case DEF_COMMONTYPE_DISMISSGUILDAPPROVE:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_DISMISSGUILDAPPROVE");
            DismissGuildApproveHandler(iClientH, pString);
            break;

        case DEF_COMMONTYPE_DISMISSGUILDREJECT:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_DISMISSGUILDREJECT");
            DismissGuildRejectHandler(iClientH, pString);
            break;

        case DEF_COMMONTYPE_RELEASEITEM:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_RELEASEITEM");
            ReleaseItemHandler(iClientH, iV1, true);
            break;

        case DEF_COMMONTYPE_TOGGLECOMBATMODE:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_TOGGLECOMBATMODE");
            ToggleCombatModeHandler(iClientH);
            break;

        case DEF_COMMONTYPE_MAGIC:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_MAGIC");
            PlayerMagicHandler(iClientH, iV1, iV2, (iV3 - 100));
            break;

        case DEF_COMMONTYPE_TOGGLESAFEATTACKMODE:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_TOGGLESAFEATTACKMODE");
            ToggleSafeAttackModeHandler(iClientH);
            break;

        case DEF_COMMONTYPE_REQ_GETOCCUPYFIGHTZONETICKET:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQ_GETOCCUPYFIGHTZONETICKET");
            GetFightzoneTicketHandler(iClientH);
            break;

            // Upgrade Item
        case DEF_COMMONTYPE_UPGRADEITEM:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_UPGRADEITEM");
            RequestItemUpgradeHandler(iClientH, iV1);
            break;

        case DEF_COMMONTYPE_REQGUILDNAME:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQGUILDNAME");
            RequestGuildNameHandler(iClientH, iV1, iV2);
            break;

        case DEF_COMMONTYPE_REQUEST_ACCEPTJOINPARTY:
            //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_CLIENT -> MSGID_COMMAND_COMMON -> DEF_COMMONTYPE_REQUEST_ACCEPTJOINPARTY");
            RequestAcceptJoinPartyHandler(iClientH, iV1);
            break;

        default:
            copy_string(G_cTxt, "Unknown message received! (0x%.8X)", wCommand);
            log->info(G_cTxt);
            break;
    }
}

void CGame::DropItemHandler(int iClientH, short sItemIndex, int iAmount, char * pItemName, bool bByPlayer)
{
    class CItem * pItem;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsOnServerChange == true) return;
    if ((m_bAdminSecurity == true) && (m_pClientList[iClientH]->m_iAdminUserLevel > 0)) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return;
    if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == 0) return;
    if ((iAmount != -1) && (iAmount < 0)) return;

    if (((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
        (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW)) &&
        (iAmount == -1))
        iAmount = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount;


    if (memcmp(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cName, pItemName, 20) != 0) return;

    if (((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
        (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW)) &&
        (((int)m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount - iAmount) > 0))
    {
        pItem = new class CItem;
        if (_bInitItemAttr(pItem, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cName) == false)
        {
            delete pItem;
            return;
        }
        else
        {
            if (iAmount <= 0)
            {
                delete pItem;
                return;
            }
            pItem->m_dwCount = (uint32_t)iAmount;
        }

        if ((uint32_t)iAmount > m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount)
        {
            delete pItem;
            return;
        }

        m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount -= iAmount;

        // v1.41 !!!
        SetItemCount(iClientH, sItemIndex, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount);

        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
            m_pClientList[iClientH]->m_sY, pItem);

        // v1.411 
        // v2.17 2002-7-31
        if (bByPlayer == true)
            _bItemLog(DEF_ITEMLOG_DROP, iClientH, (int)-1, pItem);
        else
            _bItemLog(DEF_ITEMLOG_DROP, iClientH, (int)-1, pItem, true);

        SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
            m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
            pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4 color

        SendNotifyMsg(0, iClientH, DEF_NOTIFY_DROPITEMFIN_COUNTCHANGED, sItemIndex, iAmount, 0, 0);
    }
    else
    {

        ReleaseItemHandler(iClientH, sItemIndex, true);

        // v2.17
        if (m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] == true)
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, 0, 0);

        // v1.432
        if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ALTERITEMDROP) &&
            (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan == 0))
        {
            delete m_pClientList[iClientH]->m_pItemList[sItemIndex];
            m_pClientList[iClientH]->m_pItemList[sItemIndex] = 0;
        }
        else
        {
            m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
                m_pClientList[iClientH]->m_sY,
                m_pClientList[iClientH]->m_pItemList[sItemIndex]);

            // v1.41
            // v2.17 2002-7-31
            if (bByPlayer == true)
                _bItemLog(DEF_ITEMLOG_DROP, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[sItemIndex]);
            else
                _bItemLog(DEF_ITEMLOG_DROP, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[sItemIndex], true);

            SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
                m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
                m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSprite,
                m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpriteFrame,
                m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor); //v1.4 color
        }

        m_pClientList[iClientH]->m_pItemList[sItemIndex] = 0;
        m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] = false;

        SendNotifyMsg(0, iClientH, DEF_NOTIFY_DROPITEMFIN_ERASEITEM, sItemIndex, iAmount, 0, 0);

        m_pClientList[iClientH]->m_cArrowIndex = _iGetArrowItemIndex(iClientH);
    }

    iCalcTotalWeight(iClientH);
}

/////////////////////////////////////////////////////////////////////////////////////
//  int CGame::iClientMotion_GetItem_Handler(int iClientH, short sX, short sY, char cDir)
//  description			:: check if player is dropping item or picking up item
//  last updated		:: October 29, 2004; 7:12 PM; Hypnotoad
//	return value		:: int
/////////////////////////////////////////////////////////////////////////////////////
int CGame::iClientMotion_GetItem_Handler(int iClientH, short sX, short sY, char cDir)
{
    uint32_t * dwp;
    uint16_t * wp;
    char * cp;
    short * sp, sRemainItemSprite, sRemainItemSpriteFrame;
    char  cRemainItemColor, cData[100];
    int   iRet, iEraseReq;
    class CItem * pItem;

    if (m_pClientList[iClientH] == nullptr) return 0;
    if ((cDir <= 0) || (cDir > 8))       return 0;
    if (m_pClientList[iClientH]->m_bIsKilled == true) return 0;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return 0;

    if ((sX != m_pClientList[iClientH]->m_sX) || (sY != m_pClientList[iClientH]->m_sY)) return 2;

    int iStX, iStY;
    if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex] != 0)
    {
        iStX = m_pClientList[iClientH]->m_sX / 20;
        iStY = m_pClientList[iClientH]->m_sY / 20;
        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iPlayerActivity++;

        switch (m_pClientList[iClientH]->m_cSide)
        {
            case 0: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iNeutralActivity++; break;
            case 1: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iAresdenActivity++; break;
            case 2: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iElvineActivity++;  break;
        }
    }

    ClearSkillUsingStatus(iClientH);

    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->ClearOwner(0, iClientH, DEF_OWNERTYPE_PLAYER, sX, sY);
    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetOwner(iClientH, DEF_OWNERTYPE_PLAYER, sX, sY);

    pItem = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->pGetItem(sX, sY, &sRemainItemSprite, &sRemainItemSpriteFrame, &cRemainItemColor);
    if (pItem != 0)
    {
        if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true)
        {

            _bItemLog(DEF_ITEMLOG_GET, iClientH, 0, pItem);

            dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
            *dwp = MSGID_NOTIFY;
            wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
            *wp = DEF_NOTIFY_ITEMOBTAINED;

            cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);

            *cp = 1;
            cp++;

            memcpy(cp, pItem->m_cName, 20);
            cp += 20;

            dwp = (uint32_t *)cp;
            *dwp = pItem->m_dwCount;
            cp += 4;

            *cp = pItem->m_cItemType;
            cp++;

            *cp = pItem->m_cEquipPos;
            cp++;

            *cp = (char)0;
            cp++;

            sp = (short *)cp;
            *sp = pItem->m_sLevelLimit;
            cp += 2;

            *cp = pItem->m_cGenderLimit;
            cp++;

            wp = (uint16_t *)cp;
            *wp = pItem->m_wCurLifeSpan;
            cp += 2;

            wp = (uint16_t *)cp;
            *wp = pItem->m_wWeight;
            cp += 2;

            sp = (short *)cp;
            *sp = pItem->m_sSprite;
            cp += 2;

            sp = (short *)cp;
            *sp = pItem->m_sSpriteFrame;
            cp += 2;

            *cp = pItem->m_cItemColor;
            cp++;

            *cp = (char)pItem->m_sItemSpecEffectValue2;
            cp++;

            dwp = (uint32_t *)cp;
            *dwp = pItem->m_dwAttribute;
            cp += 4;

            if (iEraseReq == 1) delete pItem;

            SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_SETITEM, m_pClientList[iClientH]->m_cMapIndex,
                m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
                sRemainItemSprite, sRemainItemSpriteFrame, cRemainItemColor);

            iRet = m_pClientList[iClientH]->send_msg(cData, 53);
            switch (iRet)
            {
                case DEF_XSOCKEVENT_QUENEFULL:
                case DEF_XSOCKEVENT_SOCKETERROR:
                case DEF_XSOCKEVENT_CRITICALERROR:
                case DEF_XSOCKEVENT_SOCKETCLOSED:
                    DeleteClient(iClientH, true, true);
                    return 0;
            }
        }
        else
        {
            m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(sX, sY, pItem);

            dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
            *dwp = MSGID_NOTIFY;
            wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
            *wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;

            iRet = m_pClientList[iClientH]->send_msg(cData, 6);
            switch (iRet)
            {
                case DEF_XSOCKEVENT_QUENEFULL:
                case DEF_XSOCKEVENT_SOCKETERROR:
                case DEF_XSOCKEVENT_CRITICALERROR:
                case DEF_XSOCKEVENT_SOCKETCLOSED:
                    DeleteClient(iClientH, true, true);
                    return 0;
            }
        }
    }

    dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
    *dwp = MSGID_RESPONSE_MOTION;
    wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
    *wp = DEF_OBJECTMOTION_CONFIRM;

    iRet = m_pClientList[iClientH]->send_msg(cData, 6);
    switch (iRet)
    {
        case DEF_XSOCKEVENT_QUENEFULL:
        case DEF_XSOCKEVENT_SOCKETERROR:
        case DEF_XSOCKEVENT_CRITICALERROR:
        case DEF_XSOCKEVENT_SOCKETCLOSED:
            DeleteClient(iClientH, true, true);
            return 0;
    }

    return 1;
}

/////////////////////////////////////////////////////////////////////////////////////
//  int CGame::iClientMotion_Stop_Handler(int iClientH, short sX, short sY, char cDir)
//  description			:: checks if player is stopped
//  last updated		:: October 29, 2004; 6:46 PM; Hypnotoad
//	return value		:: int
/////////////////////////////////////////////////////////////////////////////////////
int CGame::iClientMotion_Stop_Handler(int iClientH, short sX, short sY, char cDir)
{
    char cData[100];
    uint32_t * dwp;
    uint16_t * wp;
    int     iRet;
    short   sOwnerH;
    char    cOwnerType;

    if (m_pClientList[iClientH] == nullptr) return 0;
    if ((cDir <= 0) || (cDir > 8))       return 0;
    if (m_pClientList[iClientH]->m_bIsKilled == true) return 0;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return 0;

    if ((sX != m_pClientList[iClientH]->m_sX) || (sY != m_pClientList[iClientH]->m_sY)) return 2;

    if (m_pClientList[iClientH]->m_bSkillUsingStatus[19] == true)
    {
        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, sX, sY);
        if (sOwnerH != 0)
        {
            DeleteClient(iClientH, true, true);
            return 0;
        }
    }

    ClearSkillUsingStatus(iClientH);

    int iStX, iStY;
    if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex] != 0)
    {
        iStX = m_pClientList[iClientH]->m_sX / 20;
        iStY = m_pClientList[iClientH]->m_sY / 20;
        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iPlayerActivity++;

        switch (m_pClientList[iClientH]->m_cSide)
        {
            case 0: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iNeutralActivity++; break;
            case 1: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iAresdenActivity++; break;
            case 2: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iElvineActivity++;  break;
        }
    }

    m_pClientList[iClientH]->m_cDir = cDir;

    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->ClearOwner(0, iClientH, DEF_OWNERTYPE_PLAYER, sX, sY);
    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetOwner(iClientH, DEF_OWNERTYPE_PLAYER, sX, sY);

    dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
    *dwp = MSGID_RESPONSE_MOTION;
    wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
    *wp = DEF_OBJECTMOTION_CONFIRM;

    iRet = m_pClientList[iClientH]->send_msg(cData, 6);
    switch (iRet)
    {
        case DEF_XSOCKEVENT_QUENEFULL:
        case DEF_XSOCKEVENT_SOCKETERROR:
        case DEF_XSOCKEVENT_CRITICALERROR:
        case DEF_XSOCKEVENT_SOCKETCLOSED:
            DeleteClient(iClientH, true, true);
            return 0;
    }

    return 1;
}

void CGame::ResponseCreateNewGuildHandler(char * pData, uint32_t dwMsgSize)
{
    int i;
    uint16_t * wp, wResult;
    uint32_t * dwp;
    char * cp, cCharName[11], cData[100], cTxt[120];
    int iRet;

    // Â·ÃŽÂ±Ã— Â¼Â­Â¹Ã¶Â·ÃŽÂºÃŽÃ…Ã Â±Ã¦ÂµÃ¥ Â»Ã½Â¼Âº Â¿Ã¤ÃƒÂ»Â¿Â¡ Â´Ã«Ã‡Ã‘ Ã€Ã€Â´Ã¤ÂµÂ¥Ã€ÃŒÃ…ÃÂ°Â¡ ÂµÂµÃ‚Ã¸Ã‡ÃŸÂ´Ã™. 
    memset(cCharName, 0, sizeof(cCharName));
    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);
    memcpy(cCharName, cp, 10);
    cp += 10;

    // Ã€ÃŒÂ¸Â§Ã€ÃŒ Ã€ÃÃ„Â¡Ã‡ÃÂ´Ã‚ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¸Â¦ ÃƒÂ£Â´Ã‚Â´Ã™.
    for (i = 1; i < DEF_MAXCLIENTS; i++)
        if ((m_pClientList[i] != 0) && (memcmp(m_pClientList[i]->m_cCharName, cCharName, 10) == 0) &&
            (m_pClientList[i]->m_iLevel >= 20) && (m_pClientList[i]->m_iCharisma >= 20))
        {

            wp = (uint16_t *)(pData + DEF_INDEX2_MSGTYPE);
            switch (*wp)
            {
                case DEF_LOGRESMSGTYPE_CONFIRM:
                    // Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Ã€Ã‡ Â±Ã¦ÂµÃ¥ Â»Ã½Â¼Âº Â¿Ã¤Â±Â¸Â°Â¡ Â¼ÂºÂ°Ã¸Ã‡ÃÂ¿Â´Â´Ã™. 
                    wResult = DEF_MSGTYPE_CONFIRM;
                    // Â±Ã¦ÂµÃ¥ Ã€ÃŒÂ¸Â§Ã€Âº Ã€ÃŒÂ¹ÃŒ Ã€ÃºÃ€Ã¥ÂµÃ‡Â¾Ã® Ã€Ã–Ã€Â¸Â¹Ã‡Â·ÃŽ Â·Â©Ã…Â©Â¸Â¦ 0Ã€Â¸Â·ÃŽ Â¹Ã™Â²Ã£ Ã€Â¯ÃˆÂ¿ÃˆÂ­ Ã‡Ã‘Â´Ã™.
                    m_pClientList[i]->m_iGuildRank = 0;	// Â±Ã¦ÂµÃ¥ Â·Â©Ã…Â©Â´Ã‚ 0. Â±Ã¦ÂµÃ¥Â¸Â¶Â½ÂºÃ…ÃÃ€Ã“ 
                    copy_string(cTxt, "(!) New guild(%s) creation success! : character(%s)", m_pClientList[i]->m_cGuildName, m_pClientList[i]->m_cCharName);
                    log->info(cTxt);
                    break;

                case DEF_LOGRESMSGTYPE_REJECT:
                    // Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Ã€Ã‡ Â±Ã¦ÂµÃ¥ Â»Ã½Â¼Âº Â¿Ã¤Â±Â¸Â°Â¡ Â½Ã‡Ã†ÃÃ‡ÃÂ¿Â´Â´Ã™.
                    // Ã‡Ã˜Â´Ã§ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Ã€Ã‡ Â±Ã¦ÂµÃ¥Ã€ÃŒÂ¸Â§Ã€Â» ÃƒÃŠÂ±Ã¢ÃˆÂ­Ã‡Ã‘Â´Ã™ "NONE".
                    wResult = DEF_MSGTYPE_REJECT;
                    memset(m_pClientList[i]->m_cGuildName, 0, sizeof(m_pClientList[i]->m_cGuildName));
                    memcpy(m_pClientList[i]->m_cGuildName, "NONE", 4);
                    m_pClientList[i]->m_iGuildRank = -1;
                    m_pClientList[i]->m_iGuildGUID = -1;
                    copy_string(cTxt, "(!) New guild(%s) creation Fail! : character(%s)", m_pClientList[i]->m_cGuildName, m_pClientList[i]->m_cCharName);
                    log->info(cTxt);
                    break;
            }

            dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
            *dwp = MSGID_RESPONSE_CREATENEWGUILD;
            wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
            *wp = wResult;

            // Â±Ã¦ÂµÃ¥ Â»Ã½Â¼Âº Â¿Ã¤Â±Â¸ Ã€Ã€Â´Ã¤ Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¿Â¡Â°Ã” Ã€Ã¼Â¼Ã›
            iRet = m_pClientList[i]->send_msg(cData, 6);
            switch (iRet)
            {
                case DEF_XSOCKEVENT_QUENEFULL:
                case DEF_XSOCKEVENT_SOCKETERROR:
                case DEF_XSOCKEVENT_CRITICALERROR:
                case DEF_XSOCKEVENT_SOCKETCLOSED:
                    // Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ ÂºÂ¸Â³Â¾Â¶Â§ Â¿Â¡Â·Â¯Â°Â¡ Â¹ÃŸÂ»Ã½Ã‡ÃŸÂ´Ã™Â¸Ã© ÃÂ¦Â°Ã…Ã‡Ã‘Â´Ã™.
                    DeleteClient(i, true, true);
                    return;
            }

            return;
        }

    // Ã€ÃŒÂ¸Â§Ã€ÃŒ Ã€ÃÃ„Â¡Ã‡ÃÂ´Ã‚ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¸Â¦ ÃƒÂ£Ã€Â» Â¼Ã¶ Â¾Ã¸Â´Ã™.
    copy_string(cTxt, "(!)Non-existing player data received from Log server(2): CharName(%s)", cCharName);
    log->info(cTxt);
}

void CGame::RequestCreateNewGuildHandler(int iClientH, char * pData, uint32_t dwMsgSize)
{
    char * cp, cGuildName[21], cTxt[120], cData[100];
    uint32_t * dwp;
    uint16_t * wp;
    int     iRet;
    SYSTEMTIME SysTime;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    if (m_bIsCrusadeMode == true) return;

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

    cp += 10;
    cp += 10;
    cp += 10;

    memset(cGuildName, 0, sizeof(cGuildName));
    memcpy(cGuildName, cp, 20);
    cp += 20;

    if (m_pClientList[iClientH]->m_iGuildRank != -1)
    {
        // Ã€ÃŒ Ã„Â³Â¸Â¯Ã…ÃÂ´Ã‚ Ã€ÃŒÂ¹ÃŒ Â±Ã¦ÂµÃ¥Â¿Â¡ Â°Â¡Ã€Ã”Ã‡ÃÂ¿Â© Ã€Ã–Ã€Â¸Â¹Ã‡Â·ÃŽ Â±Ã¦ÂµÃ¥Â¸Â¦ Â¸Â¸ÂµÃ© Â¼Ã¶ Â¾Ã¸Â´Ã™.
        copy_string(cTxt, "(!)Cannot create guild! Already guild member.: CharName(%s)", m_pClientList[iClientH]->m_cCharName);
        log->info(cTxt);
    }
    else
    {
        if ((m_pClientList[iClientH]->m_iLevel < 20) || (m_pClientList[iClientH]->m_iCharisma < 20) ||
            (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) ||
            (memcmp(m_pClientList[iClientH]->m_cLocation, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, 10) != 0))
        { // v1.4
// Ã€ÃšÂ°ÃÂ¿Ã¤Â°Ã‡Ã€ÃŒ Â¸Ã‚ÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™. Ã†Â¯Â¼ÂºÃ„Â¡Â°Â¡ Â³Â·Â°Ã…Â³Âª Â¸Â¶Ã€Â»Ã€Ã‡ Ã€Â§Ã„Â¡Â°Â¡ Â´Ã™Â¸Â£Â°Ã…Â³Âª Â½ÃƒÂ¹ÃŽÃ€ÃŒ Â¾Ã†Â´Ã‘ Â°Ã¦Â¿Ã¬  
            memset(cData, 0, sizeof(cData));

            dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
            *dwp = MSGID_RESPONSE_CREATENEWGUILD;
            wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
            *wp = DEF_MSGTYPE_REJECT;

            // Â±Ã¦ÂµÃ¥ Â»Ã½Â¼Âº Â¿Ã¤Â±Â¸ Ã€Ã€Â´Ã¤ Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¿Â¡Â°Ã” Ã€Ã¼Â¼Ã›
            iRet = m_pClientList[iClientH]->send_msg(cData, 6);
            switch (iRet)
            {
                case DEF_XSOCKEVENT_QUENEFULL:
                case DEF_XSOCKEVENT_SOCKETERROR:
                case DEF_XSOCKEVENT_CRITICALERROR:
                case DEF_XSOCKEVENT_SOCKETCLOSED:
                    // Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ ÂºÂ¸Â³Â¾Â¶Â§ Â¿Â¡Â·Â¯Â°Â¡ Â¹ÃŸÂ»Ã½Ã‡ÃŸÂ´Ã™Â¸Ã© ÃÂ¦Â°Ã…Ã‡Ã‘Â´Ã™.
                    DeleteClient(iClientH, true, true);
                    return;
            }
        }
        else
        {
            // Â±Ã¦ÂµÃ¥ Ã€ÃŒÂ¸Â§Ã€Â» Ã€Ã“Â½ÃƒÂ·ÃŽ Ã€ÃºÃ€Ã¥Ã‡Ã‘Â´Ã™. -> Â¾Ã®Ã‚Ã·Ã‡Ã‡ Â±Ã¦ÂµÃ¥ Ã€ÃŒÂ¸Â§Ã€Âº RankÂ°Â¡ -1Ã€ÃÂ¶Â§ Â¹Â«Ã€Ã‡Â¹ÃŒÃ‡ÃÂ¹Ã‡Â·ÃŽ .
            memset(m_pClientList[iClientH]->m_cGuildName, 0, sizeof(m_pClientList[iClientH]->m_cGuildName));
            strcpy(m_pClientList[iClientH]->m_cGuildName, cGuildName);
            // Â±Ã¦ÂµÃ¥Ã€Ã‡ Â¼Ã’Â¼Ã“ Â¸Â¶Ã€Â» Ã€ÃŒÂ¸Â§Ã€Â» Ã€ÃºÃ€Ã¥Ã‡Ã‘Â´Ã™.
            memset(m_pClientList[iClientH]->m_cLocation, 0, sizeof(m_pClientList[iClientH]->m_cLocation));
            strcpy(m_pClientList[iClientH]->m_cLocation, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName);
            // Â±Ã¦ÂµÃ¥Ã€Ã‡ GUIDÂ¸Â¦ Â»Ã½Â¼ÂºÃ‡ÃÂ¿Â© Ã€Ã”Â·Ã‚Ã‡Ã‘Â´Ã™. 
            GetLocalTime(&SysTime);
            m_pClientList[iClientH]->m_iGuildGUID = (int)(SysTime.wYear + SysTime.wMonth + SysTime.wDay + SysTime.wHour + SysTime.wMinute + timeGetTime());

            // Â±Ã¦ÂµÃ¥ Â»Ã½Â¼ÂºÂ¿Ã¤ÃƒÂ» Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ Â·ÃŽÂ±Ã—Â¼Â­Â¹Ã¶Â·ÃŽ Ã€Ã¼Â¼Ã›Ã‡Ã‘Â´Ã™.
            bSendMsgToLS(MSGID_REQUEST_CREATENEWGUILD, iClientH);
        }
    }
}

void CGame::RequestDisbandGuildHandler(int iClientH, char * pData, uint32_t dwMsgSize)
{
    char * cp, cGuildName[21], cTxt[120];

    if (m_bIsCrusadeMode == true) return;

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);
    memset(cGuildName, 0, sizeof(cGuildName));

    cp += 10;
    cp += 10;
    cp += 10;

    memcpy(cGuildName, cp, 20);
    cp += 20;

    if ((m_pClientList[iClientH]->m_iGuildRank != 0) || (memcmp(m_pClientList[iClientH]->m_cGuildName, cGuildName, 20) != 0))
    {
        // Â±Ã¦ÂµÃ¥Â¸Â¶Â½ÂºÃ…ÃÂ°Â¡ Â¾Ã†Â´ÃÂ°Ã…Â³Âª Â±Ã¦ÂµÃ¥Ã€Ã‡ Ã€ÃŒÂ¸Â§Ã€ÃŒ Â´Ã™Â¸Â£Â¹Ã‡Â·ÃŽ Â±Ã¦ÂµÃ¥Ã‡Ã˜Â»ÃªÃ€Ã‡ Â±Ã‡Ã‡Ã‘Ã€ÃŒ Â¾Ã¸Â´Ã™.
        copy_string(cTxt, "(!)Cannot Disband guild! Not guildmaster.: CharName(%s)", m_pClientList[iClientH]->m_cCharName);
        log->info(cTxt);
    }
    else
    {
        // Â±Ã¦ÂµÃ¥ Ã‡Ã˜Â»Ãª Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ Â·ÃŽÂ±Ã—Â¼Â­Â¹Ã¶Â·ÃŽ Ã€Ã¼Â¼Ã›Ã‡Ã‘Â´Ã™.
        bSendMsgToLS(MSGID_REQUEST_DISBANDGUILD, iClientH);
    }
}

void CGame::ResponseDisbandGuildHandler(char * pData, uint32_t dwMsgSize)
{
    int i;
    uint16_t * wp, wResult;
    uint32_t * dwp;
    char * cp, cCharName[11], cData[100], cTxt[120];
    int iRet;

    // ·Î±× ¼­¹ö·ÎºÎÅÍ ±æµå ÇØ»ê ¿äÃ»¿¡ ´ëÇÑ ÀÀ´äµ¥ÀÌÅÍ°¡ µµÂøÇß´Ù. 
    memset(cCharName, 0, sizeof(cCharName));
    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);
    memcpy(cCharName, cp, 10);
    cp += 10;

    // ÀÌ¸§ÀÌ ÀÏÄ¡ÇÏ´Â Å¬¶óÀÌ¾ðÆ®¸¦ Ã£´Â´Ù.
    for (i = 1; i < DEF_MAXCLIENTS; i++)
        if ((m_pClientList[i] != 0) && (memcmp(m_pClientList[i]->m_cCharName, cCharName, 10) == 0))
        {

            wp = (uint16_t *)(pData + DEF_INDEX2_MSGTYPE);
            switch (*wp)
            {
                case DEF_LOGRESMSGTYPE_CONFIRM:
                    // Å¬¶óÀÌ¾ðÆ®ÀÇ ±æµå ÇØ»ê ¿ä±¸°¡ ¼º°øÇÏ¿´´Ù. 
                    wResult = DEF_MSGTYPE_CONFIRM;
                    copy_string(cTxt, "(!) Disband guild(%s) success! : character(%s)", m_pClientList[i]->m_cGuildName, m_pClientList[i]->m_cCharName);
                    log->info(cTxt);

                    // Á¢¼ÓÁßÀÎ ±æµå¿øµé¿¡°Ô ±æµå°¡ ÇØ»êµÇ¾úÀ½À» ¾Ë¸®´Â ¸Þ½ÃÁö¸¦ Àü¼ÛÇÑ´Ù. 
                    SendGuildMsg(i, DEF_NOTIFY_GUILDDISBANDED, 0, 0, 0);

                    // ±æµåÀÌ¸§ Å¬¸®¾î
                    memset(m_pClientList[i]->m_cGuildName, 0, sizeof(m_pClientList[i]->m_cGuildName));
                    memcpy(m_pClientList[i]->m_cGuildName, "NONE", 4);
                    m_pClientList[i]->m_iGuildRank = -1;		// ±æµå ·©Å©´Â -1. ±æµå¿øÀÌ ¾Æ´Ï´Ù. 
                    m_pClientList[i]->m_iGuildGUID = -1;
                    break;

                case DEF_LOGRESMSGTYPE_REJECT:
                    // Å¬¶óÀÌ¾ðÆ®ÀÇ ±æµå ÇØ»ê ¿ä±¸°¡ ½ÇÆÐÇÏ¿´´Ù.
                    wResult = DEF_MSGTYPE_REJECT;
                    copy_string(cTxt, "(!) Disband guild(%s) Fail! : character(%s)", m_pClientList[i]->m_cGuildName, m_pClientList[i]->m_cCharName);
                    log->info(cTxt);
                    break;
            }

            dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
            *dwp = MSGID_RESPONSE_DISBANDGUILD;
            wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
            *wp = wResult;

            // ±æµå ÇØ»ê ¿ä±¸ ÀÀ´ä ¸Þ½ÃÁö¸¦ Å¬¶óÀÌ¾ðÆ®¿¡°Ô Àü¼Û
            iRet = m_pClientList[i]->send_msg(cData, 6);
            switch (iRet)
            {
                case DEF_XSOCKEVENT_QUENEFULL:
                case DEF_XSOCKEVENT_SOCKETERROR:
                case DEF_XSOCKEVENT_CRITICALERROR:
                case DEF_XSOCKEVENT_SOCKETCLOSED:
                    // ¸Þ½ÃÁö¸¦ º¸³¾¶§ ¿¡·¯°¡ ¹ß»ýÇß´Ù¸é Á¦°ÅÇÑ´Ù.
                    DeleteClient(i, true, true);
                    return;
            }
            return;
        }

    // ÀÌ¸§ÀÌ ÀÏÄ¡ÇÏ´Â Å¬¶óÀÌ¾ðÆ®¸¦ Ã£À» ¼ö ¾ø´Ù.
    copy_string(cTxt, "(!)Non-existing player data received from Log server(2): CharName(%s)", cCharName);
    log->info(cTxt);
}

void CGame::RequestPurchaseItemHandler(int iClientH, char * pItemName, int iNum)
{
    class CItem * pItem;
    char * cp, cItemName[21], cData[100];
    short * sp;
    uint32_t * dwp, dwGoldCount, dwItemCount;
    uint16_t * wp, wTempPrice;
    int   i, iRet, iEraseReq, iGoldWeight;
    int   iCost, iDiscountRatio, iDiscountCost;
    double dTmp1, dTmp2, dTmp3;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    // Â¸Â¸Â¾Ã  Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â±Â¸Ã€Ã”Ã‡ÃÂ°Ã­Ã€Ãš Ã‡ÃÂ´Ã‚ Â°Ã·Ã€ÃŒ Ã€ÃšÂ½Ã…Ã€Ã‡ Â¸Â¶Ã€Â»Ã€ÃŒ Â¾Ã†Â´ÃÂ¶Ã³Â¸Ã© Â±Â¸Ã€Ã”Ã‡Ã’ Â¼Ã¶ Â¾Ã¸Â´Ã™. 
    //if ( (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) != 0) &&
    //	 (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, m_pClientList[iClientH]->m_cLocation, 10) != 0) ) return;

    if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) != 0)
    {
        if (memcmp(m_pClientList[iClientH]->m_cLocation, "are", 3) == 0)
        {
            if ((memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "aresden", 7) == 0) ||
                (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "arefarm", 7) == 0))
            {

            }
            else return;
        }

        if (memcmp(m_pClientList[iClientH]->m_cLocation, "elv", 3) == 0)
        {
            if ((memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "elvine", 6) == 0) ||
                (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "elvfarm", 7) == 0))
            {

            }
            else return;
        }
    }


    // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â±Â¸Ã€Ã”Ã‡Ã‘Â´Ã™. 
    memset(cData, 0, sizeof(cData));
    memset(cItemName, 0, sizeof(cItemName));

    // New 18/05/2004
    if (m_pClientList[iClientH]->m_pIsProcessingAllowed == false) return;

    // Ã€Ã“Â½ÃƒÃ„ÃšÂµÃ¥Â´Ã™. 
    if (memcmp(pItemName, "10Arrows", 8) == 0)
    {
        strcpy(cItemName, "Arrow");
        dwItemCount = 10;
    }
    else if (memcmp(pItemName, "100Arrows", 9) == 0)
    {
        strcpy(cItemName, "Arrow");
        dwItemCount = 100;
    }
    else
    {
        memcpy(cItemName, pItemName, 20);
        dwItemCount = 1;
    }

    for (i = 1; i <= iNum; i++)
    {

        pItem = new class CItem;
        if (_bInitItemAttr(pItem, cItemName) == false)
        {
            delete pItem;
        }
        else
        {

            if (pItem->m_bIsForSale == false)
            {
                delete pItem;
                return;
            }

            pItem->m_dwCount = dwItemCount;

            iCost = pItem->m_wPrice * pItem->m_dwCount;


            dwGoldCount = dwGetItemCount(iClientH, "Gold");

            iDiscountRatio = ((m_pClientList[iClientH]->m_iCharisma - 10) / 4);

            // 2.03 Discount Method
            // Charisma
            // iDiscountRatio = (m_pClientList[iClientH]->m_iCharisma / 4) -1;
            // if (iDiscountRatio == 0) iDiscountRatio = 1;

            dTmp1 = (double)(iDiscountRatio);
            dTmp2 = dTmp1 / 100.0f;
            dTmp1 = (double)iCost;
            dTmp3 = dTmp1 * dTmp2;
            iDiscountCost = (int)dTmp3;

            if (iDiscountCost >= (iCost / 2)) iDiscountCost = (iCost / 2) - 1;

            if (dwGoldCount < (uint32_t)(iCost - iDiscountCost))
            {
                delete pItem;

                dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
                *dwp = MSGID_NOTIFY;
                wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
                *wp = DEF_NOTIFY_NOTENOUGHGOLD;
                cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);
                *cp = -1;
                cp++;

                iRet = m_pClientList[iClientH]->send_msg(cData, 7);
                switch (iRet)
                {
                    case DEF_XSOCKEVENT_QUENEFULL:
                    case DEF_XSOCKEVENT_SOCKETERROR:
                    case DEF_XSOCKEVENT_CRITICALERROR:
                    case DEF_XSOCKEVENT_SOCKETCLOSED:
                        // Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ ÂºÂ¸Â³Â¾Â¶Â§ Â¿Â¡Â·Â¯Â°Â¡ Â¹ÃŸÂ»Ã½Ã‡ÃŸÂ´Ã™Â¸Ã© ÃÂ¦Â°Ã…Ã‡Ã‘Â´Ã™.
                        DeleteClient(iClientH, true, true);
                        return;
                }
                return;
            }

            if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true)
            {
                // Â¿Â¡Â·Â¯ Â¹Ã¦ÃÃ¶Â¿Ã« Ã„ÃšÂµÃ¥
                if (m_pClientList[iClientH]->m_iCurWeightLoad < 0) m_pClientList[iClientH]->m_iCurWeightLoad = 0;

                // Â¾Ã†Ã€ÃŒÃ…Ã› Â»Ã²Â´Ã™Â´Ã‚ Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ Ã€Ã¼Â¼Ã›Ã‡Ã‘Â´Ã™.
                dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
                *dwp = MSGID_NOTIFY;
                wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
                *wp = DEF_NOTIFY_ITEMPURCHASED;

                cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);
                // 1Â°Â³ ÃˆÂ¹ÂµÃ¦Ã‡ÃŸÂ´Ã™.
                *cp = 1;
                cp++;

                memcpy(cp, pItem->m_cName, 20);
                cp += 20;

                dwp = (uint32_t *)cp;
                *dwp = pItem->m_dwCount;
                cp += 4;

                *cp = pItem->m_cItemType;
                cp++;

                *cp = pItem->m_cEquipPos;
                cp++;

                *cp = (char)0; // Â¾Ã²Ã€Âº Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒÂ¹Ã‡Â·ÃŽ Ã€Ã¥Ã‚Ã¸ÂµÃ‡ÃÃ¶ Â¾ÃŠÂ¾Ã’Â´Ã™.
                cp++;

                sp = (short *)cp;
                *sp = pItem->m_sLevelLimit;
                cp += 2;

                *cp = pItem->m_cGenderLimit;
                cp++;

                wp = (uint16_t *)cp;
                *wp = pItem->m_wCurLifeSpan;
                cp += 2;

                wp = (uint16_t *)cp;
                *wp = pItem->m_wWeight;
                cp += 2;

                sp = (short *)cp;
                *sp = pItem->m_sSprite;
                cp += 2;

                sp = (short *)cp;
                *sp = pItem->m_sSpriteFrame;
                cp += 2;

                *cp = pItem->m_cItemColor;
                cp++;

                wp = (uint16_t *)cp;
                *wp = (iCost - iDiscountCost);
                wTempPrice = (iCost - iDiscountCost);
                cp += 2;

                if (iEraseReq == 1) delete pItem;

                // Â¾Ã†Ã€ÃŒÃ…Ã› ÃÂ¤ÂºÂ¸ Ã€Ã¼Â¼Ã› 
                iRet = m_pClientList[iClientH]->send_msg(cData, 48);

                // GoldÃ€Ã‡ Â¼Ã¶Â·Â®Ã€Â» Â°Â¨Â¼Ã’Â½ÃƒÃ…Â²Â´Ã™. Â¹ÃÂµÃ¥Â½Ãƒ Â¿Â©Â±Ã¢Â¼Â­ Â¼Â¼Ã†ÃƒÃ‡Ã˜Â¾ÃŸ Â¼Ã¸Â¼Â­Â°Â¡ Â¹Ã™Â²Ã®ÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™.
                iGoldWeight = SetItemCount(iClientH, "Gold", dwGoldCount - wTempPrice);
                // Â¼Ã’ÃÃ¶Ã‡Â° ÃƒÃ‘ ÃÃŸÂ·Â® Ã€Ã§ Â°Ã¨Â»Ãª 
                iCalcTotalWeight(iClientH);

                //v1.4 Â¸Â¶Ã€Â»Ã€Ã‡ Ã€ÃšÂ±ÃÂ¿Â¡ Â´ÃµÃ‡Ã‘Â´Ã™. 
                m_stCityStatus[m_pClientList[iClientH]->m_cSide].iFunds += wTempPrice;

                switch (iRet)
                {
                    case DEF_XSOCKEVENT_QUENEFULL:
                    case DEF_XSOCKEVENT_SOCKETERROR:
                    case DEF_XSOCKEVENT_CRITICALERROR:
                    case DEF_XSOCKEVENT_SOCKETCLOSED:
                        // Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ ÂºÂ¸Â³Â¾Â¶Â§ Â¿Â¡Â·Â¯Â°Â¡ Â¹ÃŸÂ»Ã½Ã‡ÃŸÂ´Ã™Â¸Ã© ÃÂ¦Â°Ã…Ã‡Ã‘Â´Ã™.
                        DeleteClient(iClientH, true, true);
                        return;
                }
            }
            else
            {
                // Â°Ã¸Â°Â£Ã€ÃŒ ÂºÃŽÃÂ·Ã‡Ã˜ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â¾Ã²Ã€Â» Â¼Ã¶ Â¾Ã¸Â´Ã™.
                delete pItem;

                // Â¼Ã’ÃÃ¶Ã‡Â° ÃƒÃ‘ ÃÃŸÂ·Â® Ã€Ã§ Â°Ã¨Â»Ãª 
                iCalcTotalWeight(iClientH);

                dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
                *dwp = MSGID_NOTIFY;
                wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
                *wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;

                iRet = m_pClientList[iClientH]->send_msg(cData, 6);
                switch (iRet)
                {
                    case DEF_XSOCKEVENT_QUENEFULL:
                    case DEF_XSOCKEVENT_SOCKETERROR:
                    case DEF_XSOCKEVENT_CRITICALERROR:
                    case DEF_XSOCKEVENT_SOCKETCLOSED:
                        // Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ ÂºÂ¸Â³Â¾Â¶Â§ Â¿Â¡Â·Â¯Â°Â¡ Â¹ÃŸÂ»Ã½Ã‡ÃŸÂ´Ã™Â¸Ã© ÃÂ¦Â°Ã…Ã‡Ã‘Â´Ã™.
                        DeleteClient(iClientH, true, true);
                        return;
                }
            }
        }
    }
}

void CGame::GiveItemHandler(int iClientH, short sItemIndex, int iAmount, short dX, short dY, uint16_t wObjectID, char * pItemName)
{
    int iRet, iEraseReq;
    short * sp, sOwnerH;
    char * cp, cOwnerType, cData[100], cCharName[21];
    uint32_t * dwp;
    uint16_t * wp;
    class CItem * pItem;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsOnServerChange == true) return;
    if ((m_bAdminSecurity == true) && (m_pClientList[iClientH]->m_iAdminUserLevel > 0)) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == 0) return;
    if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return;
    if (iAmount <= 0) return;

    // Â¾Ã†Ã€ÃŒÃ…Ã› Ã€ÃŒÂ¸Â§Ã€ÃŒ Ã€ÃÃ„Â¡Ã‡ÃÃÃ¶ Â¾ÃŠÂ¾Ã†ÂµÂµ Â¹Â«Â½ÃƒÂµÃˆÂ´Ã™.
    if (memcmp(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cName, pItemName, 20) != 0)
    {
        log->info("GiveItemHandler - Not matching Item name");
        return;
    }

    memset(cCharName, 0, sizeof(cCharName));

    if (((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
        (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW)) &&
        (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount > (uint32_t)iAmount))
    {
        // Â¼Ã’ÂºÃ±Â¼Âº Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒÂ¾ÃºÂ°Ã­ Â¼Ã¶Â·Â®Â¸Â¸Ã…Â­ Â°Â¨Â¼Ã’Â½ÃƒÃ…Â°Â°Ã­ Â³Â²Ã€Âº Â°Ã” Ã€Ã–Â´Ã™Â¸Ã© 

        pItem = new class CItem;
        if (_bInitItemAttr(pItem, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cName) == false)
        {
            // ÂºÃÃ‡Ã’Ã‡ÃÂ°Ã­Ã€Ãš Ã‡ÃÂ´Ã‚ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¸Â®Â½ÂºÃ†Â®Â¿Â¡ Â¾Ã¸Â´Ã‚ Â°Ã…Â´Ã™. Ã€ÃŒÂ·Â± Ã€ÃÃ€Âº Ã€ÃÂ¾Ã®Â³Â¯ Â¼Ã¶Â°Â¡ Â¾Ã¸ÃÃ¶Â¸Â¸ 
            delete pItem;
            return;
        }
        else
        {
            pItem->m_dwCount = iAmount;
        }

        // Â¼Ã¶Â·Â® Â°Â¨Â¼Ã’: 0ÂºÂ¸Â´Ã™ Ã…Â©Â´Ã™.
        m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount -= iAmount;

        // ÂºÂ¯Â°Ã¦ÂµÃˆ Â¼Ã¶Â·Â®Ã€Â» Â¼Â³ÃÂ¤Ã‡ÃÂ°Ã­ Â¾Ã‹Â¸Â°Â´Ã™.
        // v1.41 !!! Â¾Ã†Ã€ÃŒÃ…Ã› Ã€ÃŒÂ¸Â§Â¿Â¡Â¼Â­ Ã€ÃŽÂµÂ¦Â½ÂºÂ·ÃŽ ÂºÂ¯Â°Ã¦ÂµÃ‡Â¾ÃºÂ´Ã™. 
        SetItemCount(iClientH, sItemIndex, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount);

        //Ã€ÃŒÃÂ¦ dX, dYÂ¿Â¡ Ã€Ã–Â´Ã‚ Â¿Ã€ÂºÃªÃÂ§Ã†Â®Â¿Â¡Â°Ã” Â¼Ã’ÂºÃ±Â¼Âº Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â°Ã‡Â³Ã—ÃÃ˜Â´Ã™. 
        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);

        // v1.4 ÃÃ–Â°Ã­Ã€Ãš Ã‡Ã‘ Â°Â´ÃƒÂ¼Â¿Ã Â¸Ã‚Â´Ã‚ÃÃ¶ Ã†Ã‡Â´ÃœÃ‡Ã‘Â´Ã™.
        if (wObjectID != 0)
        {
            if (wObjectID < 10000)
            {
                // Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã® 
                if ((wObjectID > 0) && (wObjectID < DEF_MAXCLIENTS))
                {
                    if (m_pClientList[wObjectID] != 0)
                    {
                        if ((uint16_t)sOwnerH != wObjectID) sOwnerH = 0;
                    }
                }
            }
            else
            {
                // NPC
                if ((wObjectID - 10000 > 0) && (wObjectID - 10000 < DEF_MAXNPCS))
                {
                    if (m_pNpcList[wObjectID - 10000] != 0)
                    {
                        if ((uint16_t)sOwnerH != (wObjectID - 10000)) sOwnerH = 0;
                    }
                }
            }
        }

        if (sOwnerH == 0)
        {
            // ÃÃ–Â°Ã­Ã€Ãš Ã‡ÃÂ´Ã‚ Ã€Â§Ã„Â¡Â¿Â¡ Â¾Ã†Â¹Â«ÂµÂµ Â¾Ã¸Â´Ã™.
            // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â¼Â­Ã€Ã–Â´Ã‚ Ã€Â§Ã„Â¡Â¿Â¡ Â¹Ã¶Â¸Â°Â´Ã™. 
            m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, pItem);

            // v1.411  
            _bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, pItem);

            // Â´Ã™Â¸Â¥ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¿Â¡Â°Ã” Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¶Â³Â¾Ã®ÃÃ¸ Â°ÃÃ€Â» Â¾Ã‹Â¸Â°Â´Ã™. 
            SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
                m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
                pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); //v1.4 color
        }
        else
        {
            // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» ÃÃ˜Â´Ã™.
            if (cOwnerType == DEF_OWNERTYPE_PLAYER)
            {
                // Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â¿Â¡Â°Ã” ÃÃ–Â¾ÃºÂ´Ã™.
                memcpy(cCharName, m_pClientList[sOwnerH]->m_cCharName, 10);

                if (sOwnerH == iClientH)
                {
                    // Â¸Â¸Â¾Ã  Ã€ÃšÂ±Ã¢ Ã€ÃšÂ½Ã…Â¿Â¡Â°Ã” ÃÃ–Â´Ã‚ Â°Ã…Â¶Ã³Â¸Ã© Â¹Â«Â½ÃƒÃ‡Ã‘Â´Ã™. Ã‡Ã˜Ã…Â·Ã€Ã‡ Â¼Ã’ÃÃ¶Â°Â¡ Ã€Ã–Â´Ã™.
                    delete pItem;
                    return;
                }

                if (_bAddClientItemList(sOwnerH, pItem, &iEraseReq) == true)
                {
                    // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» ÃˆÂ¹ÂµÃ¦Ã‡ÃŸÂ´Ã™.
                    dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
                    *dwp = MSGID_NOTIFY;
                    wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
                    *wp = DEF_NOTIFY_ITEMOBTAINED;

                    cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);

                    // 1Â°Â³ ÃˆÂ¹ÂµÃ¦Ã‡ÃŸÂ´Ã™. AmountÂ°Â¡ Â¾Ã†Â´ÃÂ´Ã™!
                    *cp = 1;
                    cp++;

                    memcpy(cp, pItem->m_cName, 20);
                    cp += 20;

                    dwp = (uint32_t *)cp;
                    *dwp = pItem->m_dwCount;	// Â¼Ã¶Â·Â®Ã€Â» Ã€Ã”Â·Ã‚ 
                    cp += 4;

                    *cp = pItem->m_cItemType;
                    cp++;

                    *cp = pItem->m_cEquipPos;
                    cp++;

                    *cp = (char)0; // Â¾Ã²Ã€Âº Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒÂ¹Ã‡Â·ÃŽ Ã€Ã¥Ã‚Ã¸ÂµÃ‡ÃÃ¶ Â¾ÃŠÂ¾Ã’Â´Ã™.
                    cp++;

                    sp = (short *)cp;
                    *sp = pItem->m_sLevelLimit;
                    cp += 2;

                    *cp = pItem->m_cGenderLimit;
                    cp++;

                    wp = (uint16_t *)cp;
                    *wp = pItem->m_wCurLifeSpan;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    *wp = pItem->m_wWeight;
                    cp += 2;

                    sp = (short *)cp;
                    *sp = pItem->m_sSprite;
                    cp += 2;

                    sp = (short *)cp;
                    *sp = pItem->m_sSpriteFrame;
                    cp += 2;

                    *cp = pItem->m_cItemColor;
                    cp++;

                    *cp = (char)pItem->m_sItemSpecEffectValue2; // v1.41 
                    cp++;

                    dwp = (uint32_t *)cp;
                    *dwp = pItem->m_dwAttribute;
                    cp += 4;
                    /*
                    *cp = (char)(pItem->m_dwAttribute & 0x00000001); // Custom-Item
                    cp++;
                    */

                    if (iEraseReq == 1) delete pItem;

                    iRet = m_pClientList[sOwnerH]->send_msg(cData, 53);
                    switch (iRet)
                    {
                        case DEF_XSOCKEVENT_QUENEFULL:
                        case DEF_XSOCKEVENT_SOCKETERROR:
                        case DEF_XSOCKEVENT_CRITICALERROR:
                        case DEF_XSOCKEVENT_SOCKETCLOSED:
                            DeleteClient(sOwnerH, true, true);
                            break;
                    }

                    // v1.4
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIVEITEMFIN_COUNTCHANGED, sItemIndex, iAmount, 0, cCharName);
                }
                else
                {
                    // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Ã€Ã¼Â´ÃžÂ¹ÃžÃ€Âº Ã„Â³Â¸Â¯Ã…ÃÂ°Â¡ Â´ÃµÃ€ÃŒÂ»Ã³ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» ÂºÂ¸Â°Ã¼Ã‡Ã’ Â¼Ã¶ Â¾Ã¸Â´Ã‚ Â»Ã³Ã…Ã‚Ã€ÃŒÂ´Ã™.
                    // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â¼Â­Ã€Ã–Â´Ã‚ Ã€Â§Ã„Â¡Â¿Â¡ Â¹Ã¶Â¸Â°Â´Ã™. 
                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
                        m_pClientList[iClientH]->m_sY,
                        pItem);

                    // v1.411  
                    _bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, pItem);

                    // Â´Ã™Â¸Â¥ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¿Â¡Â°Ã” Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¶Â³Â¾Ã®ÃÃ¸ Â°ÃÃ€Â» Â¾Ã‹Â¸Â°Â´Ã™. 
                    SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
                        m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
                        pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); //v1.4 color

                    // Â´ÃµÃ€ÃŒÂ»Ã³ Â°Â¡ÃÃºÂ¼Ã¶ Â¾Ã¸Â´Ã™Â´Ã‚ Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ ÂºÂ¸Â³Â½Â´Ã™.
                    dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
                    *dwp = MSGID_NOTIFY;
                    wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
                    *wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;

                    iRet = m_pClientList[sOwnerH]->send_msg(cData, 6);
                    switch (iRet)
                    {
                        case DEF_XSOCKEVENT_QUENEFULL:
                        case DEF_XSOCKEVENT_SOCKETERROR:
                        case DEF_XSOCKEVENT_CRITICALERROR:
                        case DEF_XSOCKEVENT_SOCKETCLOSED:
                            // Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ ÂºÂ¸Â³Â¾Â¶Â§ Â¿Â¡Â·Â¯Â°Â¡ Â¹ÃŸÂ»Ã½Ã‡ÃŸÂ´Ã™Â¸Ã© ÃÂ¦Â°Ã…Ã‡Ã‘Â´Ã™.
                            DeleteClient(sOwnerH, true, true);
                            break;
                    }

                    // v1.4 Â¼Ã¶Â·Â®Â´ÃœÃ€Â§Ã€Ã‡ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Ã€Ã¼Â´ÃžÂ¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÃ€Â½Ã€Â» Â¾Ã‹Â¸Â°Â´Ã™.
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_CANNOTGIVEITEM, sItemIndex, iAmount, 0, cCharName);
                }

            }
            else
            {
                // NPCÂ¿Â¡Â°Ã” Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» ÃÃ–Â¾ÃºÂ´Ã™.
                memcpy(cCharName, m_pNpcList[sOwnerH]->m_cNpcName, 20);

                if (memcmp(m_pNpcList[sOwnerH]->m_cNpcName, "Howard", 6) == 0)
                {
                    // NPCÂ°Â¡ ÃƒÂ¢Â°Ã­ ÃÃ–Ã€ÃŽÃ€ÃŒÂ¾ÃºÂ´Ã™Â¸Ã© Â¹Â°Â°Ã‡Ã€Â» ÂºÂ¸Â°Ã¼Ã‡ÃÂ°ÃšÂ´Ã™Â´Ã‚ Ã€Ã‡Â¹ÃŒÃ€ÃŒÂ´Ã™. 
                    if (bSetItemToBankItem(iClientH, pItem) == false)
                    {
                        // Â¹Â°Â°Ã‡Ã€Â» Â¸ÃƒÂ±Ã¢Â´Ã‚ÂµÂ¥ Â½Ã‡Ã†ÃÃ‡ÃÂ¿Â´Â´Ã™.	
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_CANNOTITEMTOBANK, 0, 0, 0, 0);

                        // Â½Ã‡Ã†ÃÃ‡ÃŸÃ€Â¸Â¹Ã‡Â·ÃŽ Â¹Ã™Â´ÃšÂ¿Â¡ Â¶Â³Â±ÂºÂ´Ã™.
                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, pItem);

                        // v1.411  
                        _bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, pItem);

                        // Â´Ã™Â¸Â¥ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¿Â¡Â°Ã” Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¶Â³Â¾Ã®ÃÃ¸ Â°ÃÃ€Â» Â¾Ã‹Â¸Â°Â´Ã™. 
                        SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
                            m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
                            pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4 color
                    }
                }
                else
                {
                    // Ã€ÃÂ¹Ã NPCÂ¿Â¡Â°Ã” Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» ÃÃ–Â¸Ã© Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â¼Â­Ã€Ã–Â´Ã‚ Ã€Â§Ã„Â¡Â¿Â¡ Â¹Ã¶Â·ÃÂ¾ÃŸ Ã‡Ã‘Â´Ã™. 
                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, pItem);

                    // v1.411  
                    _bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, pItem);

                    // Â´Ã™Â¸Â¥ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¿Â¡Â°Ã” Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¶Â³Â¾Ã®ÃÃ¸ Â°ÃÃ€Â» Â¾Ã‹Â¸Â°Â´Ã™. 
                    SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
                        m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
                        pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4 color
                }
            }
        }
    }
    else
    {
        // Â¾Ã†Ã€ÃŒÃ…Ã› Ã€Ã¼ÂºÃŽÂ¸Â¦ ÃÃ–Â¾ÃºÂ´Ã™.

        // Ã€Ã¥Ã‚Ã¸ÃÃŸÃ€ÃŽ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» ÃÃ–Â¾ÃºÂ´Ã™Â¸Ã© Â¾Ã†Ã€ÃŒÃ…Ã› Ã€Ã¥Ã‚Ã¸ÃˆÂ¿Â°ÃºÂ¸Â¦ Ã‡Ã˜ÃÂ¦Ã‡Ã˜Â¾ÃŸ Ã‡ÃÂ¹Ã‡Â·ÃŽ.
        ReleaseItemHandler(iClientH, sItemIndex, true);

        // ÃƒÂ³Â¸Â®ÂµÂµÃÃŸ Â¿Â¡Â·Â¯Â°Â¡ Â¹ÃŸÂ»Ã½Ã‡Ã’Â¶Â§Â¸Â¦ Â´Ã«ÂºÃ±Ã‡Ã˜Â¼Â­ -1Â·ÃŽ Ã‡Ã’Â´Ã§Ã‡Ã˜ Â³ÃµÂ´Ã‚Â´Ã™.
        if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW)
            m_pClientList[iClientH]->m_cArrowIndex = -1;

        //Ã€ÃŒÃÂ¦ dX, dYÂ¿Â¡ Ã€Ã–Â´Ã‚ Â¿Ã€ÂºÃªÃÂ§Ã†Â®Â¿Â¡Â°Ã” Â¼Ã’ÂºÃ±Â¼Âº Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â°Ã‡Â³Ã—ÃÃ˜Â´Ã™. 
        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY); // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» dX, dYÂ¿Â¡ Ã€Ã–Â´Ã‚ Ã„Â³Â¸Â¯Ã…ÃÂ¿Â¡Â°Ã” ÃÃ˜Â´Ã™. Â¸Â¸Â¾Ã  Â¹ÃžÃ€Â» Ã„Â³Â¸Â¯Ã…ÃÂ°Â¡ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â¹ÃžÃÃ¶ Â¸Ã¸Ã‡Ã’ Â»Ã³ÃˆÂ²Ã€ÃŒÂ¶Ã³Â¸Ã© Â¶Â¥Â¿Â¡ Â¶Â³Â¾Ã®ÃÃ¸Â´Ã™.  

        // v1.4 ÃÃ–Â°Ã­Ã€Ãš Ã‡Ã‘ Â°Â´ÃƒÂ¼Â¿Ã Â¸Ã‚Â´Ã‚ÃÃ¶ Ã†Ã‡Â´ÃœÃ‡Ã‘Â´Ã™.
        if (wObjectID != 0)
        {
            if (wObjectID < 10000)
            {
                // Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã® 
                if ((wObjectID > 0) && (wObjectID < DEF_MAXCLIENTS))
                {
                    if (m_pClientList[wObjectID] != 0)
                    {
                        if ((uint16_t)sOwnerH != wObjectID) sOwnerH = 0;
                    }
                }
            }
            else
            {
                // NPC
                if ((wObjectID - 10000 > 0) && (wObjectID - 10000 < DEF_MAXNPCS))
                {
                    if (m_pNpcList[wObjectID - 10000] != 0)
                    {
                        if ((uint16_t)sOwnerH != (wObjectID - 10000)) sOwnerH = 0;
                    }
                }
            }
        }

        if (sOwnerH == 0)
        {
            // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» ÃÃ–Â°Ã­Ã€Ãš Ã‡ÃÂ´Ã‚ Ã€Ã¥Â¼Ã’Â¿Â¡ Ã„Â³Â¸Â¯Ã…ÃÂ°Â¡ Â¾Ã¸Â´Ã™. 
            // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â¼Â­Ã€Ã–Â´Ã‚ Ã€Â§Ã„Â¡Â¿Â¡ Â¹Ã¶Â¸Â°Â´Ã™. 
            m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
                m_pClientList[iClientH]->m_sY,
                m_pClientList[iClientH]->m_pItemList[sItemIndex]);
            // v1.411  
            _bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, m_pClientList[iClientH]->m_pItemList[sItemIndex]);

            // Â´Ã™Â¸Â¥ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¿Â¡Â°Ã” Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¶Â³Â¾Ã®ÃÃ¸ Â°ÃÃ€Â» Â¾Ã‹Â¸Â°Â´Ã™. 
            SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
                m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
                m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSprite,
                m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpriteFrame,
                m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor); // v1.4 color

            // Ã€ÃŒÃÂ¦ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¶Â³Â¾Ã®ÃÂ³Ã€Â¸Â¹Ã‡Â·ÃŽ Â¸Â®Â½ÂºÃ†Â®Â¿Â¡Â¼Â­ Â»Ã¨ÃÂ¦Ã‡Ã’Â°ÃÃ€Â» Ã…Ã«ÂºÂ¸Ã‡Ã‘Â´Ã™.
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_DROPITEMFIN_ERASEITEM, sItemIndex, iAmount, 0, 0);
        }
        else
        {
            // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â´Ã™Â¸Â¥ Ã„Â³Â¸Â¯Ã…ÃÂ¿Â¡Â°Ã” ÃÃ˜Â´Ã™. @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

            if (cOwnerType == DEF_OWNERTYPE_PLAYER)
            {
                // Ä³¸¯ÅÍ¿¡°Ô ¾ÆÀÌÅÛÀ» ÁÖ¾ú´Ù.	
                memcpy(cCharName, m_pClientList[sOwnerH]->m_cCharName, 10);
                pItem = m_pClientList[iClientH]->m_pItemList[sItemIndex];

                // v2.03 Å©·ç¼¼ÀÌµå ¸ðµåÀÎ°æ¿ìµµ ±æµå °¡ÀÔÀº °¡´ÉÇÏ´Ù.

                // v2.17 2002-7-31 ¾ÆÀÌÅÛÀ» ¾ÆÀÌÅÛ °íÀ¯¹øÈ£·Î »ý¼ºÇÒ ¼ö ÀÖ°Ô ÇÑ´Ù.
                if (pItem->m_sIDnum == 88)
                {

                    // ÇÃ·¹ÀÌ¾î iClientH °¡ sOwnerH¿¡°Ô ±æµå°¡ÀÔ ½ÅÃ»¼­¸¦ ÁÖ¾ú´Ù. ¸¸¾à 
                    // sOwnerH°¡ ±æµå¸¶½ºÅÍ¶ó¸é ¾ÆÀÌÅÛÀ» ¹Þ´Â °ÍÀÌ ¾Æ´Ï¶ó È®ÀÎÀ» ÇØ ÁÖ¾î¾ß ÇÑ´Ù.
                                // v2.17 2002-7-31 ¾ÆÀÌÅÛÀ» ¾ÆÀÌÅÛ °íÀ¯¹øÈ£·Î »ý¼ºÇÒ ¼ö ÀÖ°Ô ÇÑ´Ù.
                    if ((m_pClientList[iClientH]->m_iGuildRank == -1) &&
                        (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) != 0) &&
                        (memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[sOwnerH]->m_cLocation, 10) == 0) &&
                        (m_pClientList[sOwnerH]->m_iGuildRank == 0))
                    {
                        // ±æµå ¸¶½ºÅÍÀÌ´Ù.	±æµå ¸¶½ºÅÍ¿¡°Ô´Â °¡ÀÔÈ®ÀÎ ¿äÃ» ¸Þ½ÃÁö¸¦ Àü¼ÛÇÑ´Ù.
                        SendNotifyMsg(iClientH, sOwnerH, DEF_NOTIFY_QUERY_JOINGUILDREQPERMISSION, 0, 0, 0, 0);
                        // ÀÌÁ¦ ¾ÆÀÌÅÛÀ» ÁØ º»ÀÎ¿¡°Ô ¾ÆÀÌÅÛÀ» ÁÖ¾úÀ¸¹Ç·Î ¸®½ºÆ®¿¡¼­ »èÁ¦ÇÒ°ÍÀ» Åëº¸ÇÑ´Ù.
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIVEITEMFIN_ERASEITEM, sItemIndex, 1, 0, cCharName);

                        _bItemLog(DEF_ITEMLOG_DEPLETE, iClientH, (int)-1, pItem);

                        goto REMOVE_ITEM_PROCEDURE;
                    }
                }

                // v2.17 2002-7-31 ¾ÆÀÌÅÛÀ» ¾ÆÀÌÅÛ °íÀ¯¹øÈ£·Î »ý¼ºÇÒ ¼ö ÀÖ°Ô ÇÑ´Ù.
                if ((m_bIsCrusadeMode == false) && (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 89))
                {

                    // ÇÃ·¹ÀÌ¾î iClientH °¡ sOwnerH¿¡°Ô ±æµåÅ»Åð ½ÅÃ»¼­¸¦ ÁÖ¾ú´Ù. ¸¸¾à 
                    // sOwnerH°¡ ±æµå¸¶½ºÅÍÀÌ°í iClientH¿Í ±æµå ÀÌ¸§ÀÌ °°°í iClientH°¡ ±æµå¿øÀÌ¶ó¸é 
                    // ¾ÆÀÌÅÛÀ» ¹Þ´Â °ÍÀÌ ¾Æ´Ï¶ó È®ÀÎÀ» ÇØ ÁÖ¾î¾ß ÇÑ´Ù.
                    if ((memcmp(m_pClientList[iClientH]->m_cGuildName, m_pClientList[sOwnerH]->m_cGuildName, 20) == 0) &&
                        (m_pClientList[iClientH]->m_iGuildRank != -1) &&
                        (m_pClientList[sOwnerH]->m_iGuildRank == 0))
                    {
                        // ±æµå ¸¶½ºÅÍÀÌ´Ù.	±æµå ¸¶½ºÅÍ¿¡°Ô´Â Å»ÅðÈ®ÀÎ ¿äÃ» ¸Þ½ÃÁö¸¦ Àü¼ÛÇÑ´Ù.
                        SendNotifyMsg(iClientH, sOwnerH, DEF_NOTIFY_QUERY_DISMISSGUILDREQPERMISSION, 0, 0, 0, 0);
                        // ÀÌÁ¦ ¾ÆÀÌÅÛÀ» ÁØ º»ÀÎ¿¡°Ô ¾ÆÀÌÅÛÀ» ÁÖ¾úÀ¸¹Ç·Î ¸®½ºÆ®¿¡¼­ »èÁ¦ÇÒ°ÍÀ» Åëº¸ÇÑ´Ù.
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIVEITEMFIN_ERASEITEM, sItemIndex, 1, 0, cCharName);

                        _bItemLog(DEF_ITEMLOG_DEPLETE, iClientH, (int)-1, pItem);

                        goto REMOVE_ITEM_PROCEDURE;
                    }
                }

                // Ã€ÃÂ¹ÃÃ€Ã»Ã€ÃŽ Â°Ã¦Â¿Ã¬ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â±Ã—Ã€Ãº Â¹ÃžÃ€Â» Â»Ã“...
                if (_bAddClientItemList(sOwnerH, pItem, &iEraseReq) == true)
                {

                    // v1.41 ÃˆÃ±Â±Ã Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Ã€Ã¼Â´ÃžÃ‡Ã‘ Â°ÃÃ€ÃŒÂ¶Ã³Â¸Ã© Â·ÃŽÂ±Ã—Â¸Â¦ Â³Â²Â±Ã¤Â´Ã™. 
                    _bItemLog(DEF_ITEMLOG_GIVE, iClientH, sOwnerH, pItem);

                    // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» ÃˆÂ¹ÂµÃ¦Ã‡ÃŸÂ´Ã™.
                    dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
                    *dwp = MSGID_NOTIFY;
                    wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
                    *wp = DEF_NOTIFY_ITEMOBTAINED;

                    cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);

                    // 1Â°Â³ ÃˆÂ¹ÂµÃ¦Ã‡ÃŸÂ´Ã™.
                    *cp = 1;
                    cp++;

                    memcpy(cp, pItem->m_cName, 20);
                    cp += 20;

                    dwp = (uint32_t *)cp;
                    *dwp = pItem->m_dwCount;
                    cp += 4;

                    *cp = pItem->m_cItemType;
                    cp++;

                    *cp = pItem->m_cEquipPos;
                    cp++;

                    *cp = (char)0; // Â¾Ã²Ã€Âº Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒÂ¹Ã‡Â·ÃŽ Ã€Ã¥Ã‚Ã¸ÂµÃ‡ÃÃ¶ Â¾ÃŠÂ¾Ã’Â´Ã™.
                    cp++;

                    sp = (short *)cp;
                    *sp = pItem->m_sLevelLimit;
                    cp += 2;

                    *cp = pItem->m_cGenderLimit;
                    cp++;

                    wp = (uint16_t *)cp;
                    *wp = pItem->m_wCurLifeSpan;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    *wp = pItem->m_wWeight;
                    cp += 2;

                    sp = (short *)cp;
                    *sp = pItem->m_sSprite;
                    cp += 2;

                    sp = (short *)cp;
                    *sp = pItem->m_sSpriteFrame;
                    cp += 2;

                    *cp = pItem->m_cItemColor;
                    cp++;

                    *cp = (char)pItem->m_sItemSpecEffectValue2; // v1.41 
                    cp++;

                    dwp = (uint32_t *)cp;
                    *dwp = pItem->m_dwAttribute;
                    cp += 4;
                    /*
                    *cp = (char)(pItem->m_dwAttribute & 0x00000001); // Custom-ItemÃ€ÃŽÃÃ¶Ã€Ã‡ Â¿Â©ÂºÃŽ
                    cp++;
                    */

                    if (iEraseReq == 1) delete pItem;

                    iRet = m_pClientList[sOwnerH]->send_msg(cData, 53);
                    switch (iRet)
                    {
                        case DEF_XSOCKEVENT_QUENEFULL:
                        case DEF_XSOCKEVENT_SOCKETERROR:
                        case DEF_XSOCKEVENT_CRITICALERROR:
                        case DEF_XSOCKEVENT_SOCKETCLOSED:
                            DeleteClient(sOwnerH, true, true);
                            break;
                    }
                }
                else
                {
                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
                        m_pClientList[iClientH]->m_sY,
                        m_pClientList[iClientH]->m_pItemList[sItemIndex]);
                    _bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, m_pClientList[iClientH]->m_pItemList[sItemIndex]);

                    SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
                        m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
                        m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSprite,
                        m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpriteFrame,
                        m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor); // v1.4 color

                    dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
                    *dwp = MSGID_NOTIFY;
                    wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
                    *wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;

                    iRet = m_pClientList[sOwnerH]->send_msg(cData, 6);
                    switch (iRet)
                    {
                        case DEF_XSOCKEVENT_QUENEFULL:
                        case DEF_XSOCKEVENT_SOCKETERROR:
                        case DEF_XSOCKEVENT_CRITICALERROR:
                        case DEF_XSOCKEVENT_SOCKETCLOSED:
                            DeleteClient(sOwnerH, true, true);
                            break;
                    }

                    memset(cCharName, 0, sizeof(cCharName));
                }
            }
            else
            {
                memcpy(cCharName, m_pNpcList[sOwnerH]->m_cNpcName, 20);

                if (memcmp(m_pNpcList[sOwnerH]->m_cNpcName, "Howard", 6) == 0)
                {
                    if (bSetItemToBankItem(iClientH, sItemIndex) == false)
                    {
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_CANNOTITEMTOBANK, 0, 0, 0, 0);

                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
                            m_pClientList[iClientH]->m_sY,
                            m_pClientList[iClientH]->m_pItemList[sItemIndex]);

                        _bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, m_pClientList[iClientH]->m_pItemList[sItemIndex]);

                        SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
                            m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
                            m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSprite,
                            m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpriteFrame,
                            m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor); // v1.4 color
                    }
                }
                else if (memcmp(m_pNpcList[sOwnerH]->m_cNpcName, "Kennedy", 7) == 0)
                {
                    if ((m_bIsCrusadeMode == false) && (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 89))
                    {

                        if ((m_pClientList[iClientH]->m_iGuildRank != 0) && (m_pClientList[iClientH]->m_iGuildRank != -1))
                        {
                            SendNotifyMsg(iClientH, iClientH, DEF_COMMONTYPE_DISMISSGUILDAPPROVE, 0, 0, 0, 0);

                            memset(m_pClientList[iClientH]->m_cGuildName, 0, sizeof(m_pClientList[iClientH]->m_cGuildName));
                            memcpy(m_pClientList[iClientH]->m_cGuildName, "NONE", 4);
                            m_pClientList[iClientH]->m_iGuildRank = -1;
                            m_pClientList[iClientH]->m_iGuildGUID = -1;

                            SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);

                            m_pClientList[iClientH]->m_iExp -= 300;
                            if (m_pClientList[iClientH]->m_iExp < 0) m_pClientList[iClientH]->m_iExp = 0;
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_EXP, 0, 0, 0, 0);
                        }

                        delete m_pClientList[iClientH]->m_pItemList[sItemIndex];
                    }
                    else
                    {
                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
                            m_pClientList[iClientH]->m_sY,
                            m_pClientList[iClientH]->m_pItemList[sItemIndex]);

                        _bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, m_pClientList[iClientH]->m_pItemList[sItemIndex]);

                        SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
                            m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
                            m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSprite,
                            m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpriteFrame,
                            m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor); // v1.4 color

                        // v1.4 Â¾Ã†Ã€ÃŒÃ…Ã› Ã€Ã¼Â´ÃžÃ€ÃŒ Â½Ã‡Ã†ÃÃ‡ÃŸÃ€Â½Ã€Â» Â¾Ã‹Â¸Â®Â´Ã‚ Â¹Ã¦Â¹Ã½ 
                        memset(cCharName, 0, sizeof(cCharName));

                    }
                }
                else
                {
                    // Ã€ÃÂ¹Ã NPCÂ¿Â¡Â°Ã” Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» ÃÃ–Â¸Ã© Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â¼Â­Ã€Ã–Â´Ã‚ Ã€Â§Ã„Â¡Â¿Â¡ Â¹Ã¶Â·ÃÂ¾ÃŸ Ã‡Ã‘Â´Ã™. 

                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
                        m_pClientList[iClientH]->m_sY,
                        m_pClientList[iClientH]->m_pItemList[sItemIndex]);

                    // v1.41 ÃˆÃ±Â±Ã Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â¶Â³Â¾Ã®Â¶ÃŸÂ¸Â° Â°ÃÃ€ÃŒÂ¶Ã³Â¸Ã© Â·ÃŽÂ±Ã—Â¸Â¦ Â³Â²Â±Ã¤Â´Ã™. 
                    _bItemLog(DEF_ITEMLOG_DROP, iClientH, 0, m_pClientList[iClientH]->m_pItemList[sItemIndex]);

                    // Â´Ã™Â¸Â¥ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¿Â¡Â°Ã” Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¶Â³Â¾Ã®ÃÃ¸ Â°ÃÃ€Â» Â¾Ã‹Â¸Â°Â´Ã™. 
                    SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
                        m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
                        m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSprite,
                        m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpriteFrame,
                        m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor); // v1.4 color

                    // v1.4 Â¾Ã†Ã€ÃŒÃ…Ã› Ã€Ã¼Â´ÃžÃ€ÃŒ Â½Ã‡Ã†ÃÃ‡ÃŸÃ€Â½Ã€Â» Â¾Ã‹Â¸Â®Â´Ã‚ Â¹Ã¦Â¹Ã½ 
                    memset(cCharName, 0, sizeof(cCharName));
                }
            }

            // Ã€ÃŒÃÂ¦ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» ÃÃ˜ ÂºÂ»Ã€ÃŽÂ¿Â¡Â°Ã” Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» ÃÃ–Â¾ÃºÃ€Â¸Â¹Ã‡Â·ÃŽ Â¸Â®Â½ÂºÃ†Â®Â¿Â¡Â¼Â­ Â»Ã¨ÃÂ¦Ã‡Ã’Â°ÃÃ€Â» Ã…Ã«ÂºÂ¸Ã‡Ã‘Â´Ã™.
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIVEITEMFIN_ERASEITEM, sItemIndex, iAmount, 0, cCharName);
        }

        REMOVE_ITEM_PROCEDURE:;

        // Â³Ã—Ã†Â®Â¿Ã¶Ã…Â© Â¿Ã€Â·Ã¹Â·ÃŽ ÃƒÂ³Â¸Â®ÂµÂµÃÃŸ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â°Â¡ ÃÂ¦Â°Ã…ÂµÃ‡Â¾ÃºÂ´Ã™Â¸Ã© Â´ÃµÃ€ÃŒÂ»Ã³ ÃÃ¸Ã‡Ã Ã‡Ã’ Â¼Ã¶ Â¾Ã¸Â´Ã™. 
        if (m_pClientList[iClientH] == nullptr) return;

        // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» ÃÃ–Â°Ã…Â³Âª Â¹Ã¶Â·ÃˆÃ€Â¸Â¹Ã‡Â·ÃŽ ÃÃ¶Â¿Ã®Â´Ã™. deleteÃ‡Ã˜Â¼Â­Â´Ã‚ Â¾ÃˆÂµÃˆÂ´Ã™! 
        m_pClientList[iClientH]->m_pItemList[sItemIndex] = 0;
        m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] = false;

        // ÃˆÂ­Â»Ã¬ Ã€ÃŽÂµÂ¦Â½ÂºÂ¸Â¦ Ã€Ã§ Ã‡Ã’Â´Ã§
        m_pClientList[iClientH]->m_cArrowIndex = _iGetArrowItemIndex(iClientH);
    }

    // Â¼Ã’ÃÃ¶Ã‡Â° ÃƒÃ‘ ÃÃŸÂ·Â® Ã€Ã§ Â°Ã¨Â»Ãª 
    iCalcTotalWeight(iClientH);
}

void CGame::JoinGuildApproveHandler(int iClientH, char * pName)
{
    int i;
    bool bIsExist = false;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

    // pNameÃ€Â» Â°Â®Â´Ã‚ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Ã€Ã‡ iClientH Â±Ã¦ÂµÃ¥Â¿Â¡ Â´Ã«Ã‡Ã‘ Â°Â¡Ã€Ã”Â¿Ã¤Â±Â¸Â°Â¡ Â¼ÂºÂ°Ã¸Ã‡ÃÂ¿Â´Â´Ã™.

    // pNameÃ€Ã‡ Ã€ÃŒÂ¸Â§Ã€Â» Â°Â®Â´Ã‚ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â® Â±Â¸ÃÂ¶ÃƒÂ¼Â¸Â¦ Â°Ã‹Â»Ã¶Ã‡Ã‘Â´Ã™.
    for (i = 1; i < DEF_MAXCLIENTS; i++)
        if ((m_pClientList[i] != 0) && (memcmp(m_pClientList[i]->m_cCharName, pName, 10) == 0))
        {
            // v1.4 Â¼Ã’Â¼Ã“ Â¸Â¶Ã€Â»Ã€ÃŒ Â´ÃžÂ¶Ã³ÂµÂµ Â¹Â«Â½ÃƒÂµÃˆÂ´Ã™.
            if (memcmp(m_pClientList[i]->m_cLocation, m_pClientList[iClientH]->m_cLocation, 10) != 0) return;

            // Â±Ã¦ÂµÃ¥Ã€Ã‡ Ã€ÃŒÂ¸Â§Ã€Â» ÂºÂ¹Â»Ã§Ã‡ÃÂ°Ã­ Â¼Ã¶Ã„Â¡Â¸Â¦ ÃƒÃŠÂ±Ã¢ÃˆÂ­Ã‡Ã˜ ÃÃ˜Â´Ã™.
            memset(m_pClientList[i]->m_cGuildName, 0, sizeof(m_pClientList[i]->m_cGuildName));
            strcpy(m_pClientList[i]->m_cGuildName, m_pClientList[iClientH]->m_cGuildName);

            // Â±Ã¦ÂµÃ¥ GUIDÂºÂ¹Â»Ã§Ã‡Ã‘Â´Ã™.
            m_pClientList[i]->m_iGuildGUID = m_pClientList[iClientH]->m_iGuildGUID;

            // Â±Ã¦ÂµÃ¥Ã€Ã‡ Â»Ã½Â¼ÂºÃ€Â§Ã„Â¡Â¸Â¦ ÃƒÃŠÂ±Ã¢ÃˆÂ­Ã‡Ã‘Â´Ã™.
            memset(m_pClientList[i]->m_cLocation, 0, sizeof(m_pClientList[i]->m_cLocation));
            strcpy(m_pClientList[i]->m_cLocation, m_pClientList[iClientH]->m_cLocation);

            m_pClientList[i]->m_iGuildRank = DEF_GUILDSTARTRANK; //@@@  GuildRankÃ€Ã‡ Â½ÃƒÃ€Ã›Ã€Âº DEF_GUILDSTARTRANK

            // Â°Â¡Ã€Ã” Â½Ã…ÃƒÂ»Ã€ÃšÂ¿Â¡Â°Ã” Â°Â¡Ã€Ã”Ã€ÃŒ Â¼ÂºÂ°Ã¸Ã‡ÃŸÃ€Â½Ã€Â» Â¾Ã‹Â¸Â®Â´Ã‚ Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ ÂºÂ¸Â³Â»ÃÃ˜Â´Ã™.
            SendNotifyMsg(iClientH, i, DEF_COMMONTYPE_JOINGUILDAPPROVE, 0, 0, 0, 0);

            // Ã†Â¯Â¼ÂºÃ€ÃŒ Â¹Ã™Â²Ã®Â¹Ã‡Â·ÃŽ Â¿ÃœÂ¾Ã§Ã€Â» Â»ÃµÂ·ÃŽ ÂºÂ¸Â³Â½Â´Ã™. 
            SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);

            // Â´Ã™Â¸Â¥ Â±Ã¦ÂµÃ¥Â¿Ã¸ÂµÃ©Â¿Â¡Â°Ã” Â»Ãµ Â±Ã¦ÂµÃ¥Â¿Ã¸Ã€ÃŒ Ã€Ã–Ã€Â½Ã€Â» Â¾Ã‹Â¸Â°Â´Ã™.
            SendGuildMsg(i, DEF_NOTIFY_NEWGUILDSMAN, 0, 0, 0);

            // Â±Ã¦ÂµÃ¥ÃÂ¤ÂºÂ¸ÃˆÂ­Ã€ÃÂ¿Â¡ Â»Ãµ Â±Ã¦ÂµÃ¥Â¿Ã¸Ã€Ã‡ Ã€ÃŒÂ¸Â§Ã€Â» Â±Ã¢Â·ÃÃ‡Ã‘Â´Ã™.
            bSendMsgToLS(MSGID_REQUEST_UPDATEGUILDINFO_NEWGUILDSMAN, i);
            return;
        }

    // Â°Â¡Ã€Ã”Ã€Â» Â½Ã…ÃƒÂ»Ã‡Ã‘ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¸Â¦ ÃƒÂ£Ã€Â»Â¼Ã¶ Â¾Ã¸Â´Ã™.(ÃÂ¢Â¼Ã“Ã€ÃŒ Â±Ã—Â»Ã§Ã€ÃŒ Â²Ã·Â°Ã¥Â´Ã™Â´Ã¸ÃÃ¶) Â¹Â«ÃˆÂ¿Ã€Ã“ 
}

void CGame::JoinGuildRejectHandler(int iClientH, char * pName)
{
    int i;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

    // pNameÃ€Â» Â°Â®Â´Ã‚ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Ã€Ã‡ iClientH Â±Ã¦ÂµÃ¥Â¿Â¡ Â´Ã«Ã‡Ã‘ Â°Â¡Ã€Ã” Â¿Ã¤Â±Â¸Â°Â¡ Â½Ã‡Ã†Ã Ã‡ÃÂ¿Â´Â´Ã™.

    // pNameÃ€Ã‡ Ã€ÃŒÂ¸Â§Ã€Â» Â°Â®Â´Ã‚ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â® Â±Â¸ÃÂ¶ÃƒÂ¼Â¸Â¦ Â°Ã‹Â»Ã¶Ã‡Ã‘Â´Ã™.
    for (i = 1; i < DEF_MAXCLIENTS; i++)
        if ((m_pClientList[i] != 0) && (memcmp(m_pClientList[i]->m_cCharName, pName, 10) == 0))
        {

            // Â°Â¡Ã€Ã” Â½Ã…ÃƒÂ»Ã€ÃšÂ¿Â¡Â°Ã” Â°Â¡Ã€Ã”Ã€ÃŒ Â½Ã‡Ã†ÃÃ‡ÃŸÃ€Â½Ã€Â» Â¾Ã‹Â¸Â®Â´Ã‚ Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ ÂºÂ¸Â³Â»ÃÃ˜Â´Ã™.
            SendNotifyMsg(iClientH, i, DEF_COMMONTYPE_JOINGUILDREJECT, 0, 0, 0, 0);
            return;
        }

    // Â°Â¡Ã€Ã”Ã€Â» Â½Ã…ÃƒÂ»Ã‡Ã‘ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¸Â¦ ÃƒÂ£Ã€Â»Â¼Ã¶ Â¾Ã¸Â´Ã™.(ÃÂ¢Â¼Ã“Ã€ÃŒ Â±Ã—Â»Ã§Ã€ÃŒ Â²Ã·Â°Ã¥Â´Ã™Â´Ã¸ÃÃ¶) Â¹Â«ÃˆÂ¿Ã€Ã“ 
}

void CGame::DismissGuildApproveHandler(int iClientH, char * pName)
{
    int i;


    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    for (i = 1; i < DEF_MAXCLIENTS; i++)
        if ((m_pClientList[i] != 0) && (memcmp(m_pClientList[i]->m_cCharName, pName, 10) == 0))
        {

            bSendMsgToLS(MSGID_REQUEST_UPDATEGUILDINFO_DELGUILDSMAN, i);
            //_bItemLog(DEF_ITEMLOG_BANGUILD,i,(char *)0,0) ;
            SendGuildMsg(i, DEF_NOTIFY_DISMISSGUILDSMAN, 0, 0, 0);

            memset(m_pClientList[i]->m_cGuildName, 0, sizeof(m_pClientList[i]->m_cGuildName));
            strcpy(m_pClientList[i]->m_cGuildName, "NONE");
            m_pClientList[i]->m_iGuildRank = -1;
            m_pClientList[i]->m_iGuildGUID = -1;

            SendNotifyMsg(iClientH, i, DEF_COMMONTYPE_DISMISSGUILDAPPROVE, 0, 0, 0, 0);

            SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
            return;
        }

}

void CGame::DismissGuildRejectHandler(int iClientH, char * pName)
{
    int i;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

    // pNameÃ€Â» Â°Â®Â´Ã‚ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Ã€Ã‡ iClientH Â±Ã¦ÂµÃ¥Â¿Â¡ Â´Ã«Ã‡Ã‘ Ã…Â»Ã…Ã° Â¿Ã¤Â±Â¸Â°Â¡ Â½Ã‡Ã†Ã Ã‡ÃÂ¿Â´Â´Ã™.

    // pNameÃ€Ã‡ Ã€ÃŒÂ¸Â§Ã€Â» Â°Â®Â´Ã‚ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â® Â±Â¸ÃÂ¶ÃƒÂ¼Â¸Â¦ Â°Ã‹Â»Ã¶Ã‡Ã‘Â´Ã™.
    for (i = 1; i < DEF_MAXCLIENTS; i++)
        if ((m_pClientList[i] != 0) && (memcmp(m_pClientList[i]->m_cCharName, pName, 10) == 0))
        {

            // Â°Â¡Ã€Ã” Â½Ã…ÃƒÂ»Ã€ÃšÂ¿Â¡Â°Ã” Ã…Â»Ã…Ã°Â°Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÃ€Â½Ã€Â» Â¾Ã‹Â¸Â®Â´Ã‚ Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ ÂºÂ¸Â³Â»ÃÃ˜Â´Ã™.
            SendNotifyMsg(iClientH, i, DEF_COMMONTYPE_DISMISSGUILDREJECT, 0, 0, 0, 0);
            return;
        }

    // Ã…Â»Ã…Ã°Â¸Â¦ Â½Ã…ÃƒÂ»Ã‡Ã‘ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¸Â¦ ÃƒÂ£Ã€Â»Â¼Ã¶ Â¾Ã¸Â´Ã™.(ÃÂ¢Â¼Ã“Ã€ÃŒ Â±Ã—Â»Ã§Ã€ÃŒ Â²Ã·Â°Ã¥Â´Ã™Â´Ã¸ÃÃ¶) Â¹Â«ÃˆÂ¿Ã€Ã“ 
}

void CGame::ClientKilledHandler(int iClientH, int iAttackerH, char cAttackerType, short sDamage)
{
    char * cp, cAttackerName[21], cData[120];
    short sAttackerWeapon;
    int * ip, i, iExH;
    bool  bIsSAattacked = false;


    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    if (m_pClientList[iClientH]->m_bIsKilled == true) return;

    // »çÅõÀåÀÎ °æ¿ì ½Ã°£À» ÀÔ·ÂÇÑ´Ù.
    // 2002-7-4 »çÅõÀåÀÇ °¹¼ö¸¦ ´Ã¸± ¼ö ÀÖµµ·Ï 
    if (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "fight", 5) == 0)
    {
        m_pClientList[iClientH]->m_dwFightzoneDeadTime = timeGetTime();
        copy_string(G_cTxt, "Fightzone Dead Time: %d", m_pClientList[iClientH]->m_dwFightzoneDeadTime);
        log->info(G_cTxt);
    }

    m_pClientList[iClientH]->m_bIsKilled = true;
    // HP´Â 0ÀÌ´Ù.
    m_pClientList[iClientH]->m_iHP = 0;

    // ¸¸¾à ±³È¯ ¸ðµå¶ó¸é ±³È¯À» Ãë¼ÒÇÑ´Ù.
    if (m_pClientList[iClientH]->m_bIsExchangeMode == true)
    {
        iExH = m_pClientList[iClientH]->m_iExchangeH;
        _ClearExchangeStatus(iExH);
        _ClearExchangeStatus(iClientH);
    }

    // ÇöÀç ÀÌ NPC¸¦ °ø°Ý´ë»óÀ¸·Î »ï°íÀÖ´Â °´Ã¼µéÀ» ÇØÁ¦ÇÑ´Ù.
    RemoveFromTarget(iClientH, DEF_OWNERTYPE_PLAYER);

    memset(cAttackerName, 0, sizeof(cAttackerName));
    switch (cAttackerType)
    {
        case DEF_OWNERTYPE_PLAYER_INDIRECT:
        case DEF_OWNERTYPE_PLAYER:
            if (m_pClientList[iAttackerH] != 0)
                memcpy(cAttackerName, m_pClientList[iAttackerH]->m_cCharName, 10);
            break;
        case DEF_OWNERTYPE_NPC:
            if (m_pNpcList[iAttackerH] != 0)
#ifdef DEF_LOCALNPCNAME     // v2.14 NPC ÀÌ¸§ Áß¹®È­¸¦ À§ÇÑ ¼±¾ð 
                copy_string(cAttackerName, "NPCNPCNPC@%d", m_pNpcList[iAttackerH]->m_sType);
#else 
                memcpy(cAttackerName, m_pNpcList[iAttackerH]->m_cNpcName, 20);
#endif
            break;
        default:
            break;
    }

    SendNotifyMsg(0, iClientH, DEF_NOTIFY_KILLED, 0, 0, 0, cAttackerName);
    // ´Ù¸¥ Å¬¶óÀÌ¾ðÆ®¿¡°Ô Á×´Â µ¿ÀÛ Àü¼Û.
    if (cAttackerType == DEF_OWNERTYPE_PLAYER)
    {
        sAttackerWeapon = ((m_pClientList[iAttackerH]->m_sAppr2 & 0x0FF0) >> 4);
    }
    else sAttackerWeapon = 1;
    SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDYING, sDamage, sAttackerWeapon, 0);
    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->ClearOwner(12, iClientH, DEF_OWNERTYPE_PLAYER, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY);
    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetDeadOwner(iClientH, DEF_OWNERTYPE_PLAYER, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY);
    if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cType == DEF_MAPTYPE_NOPENALTY_NOREWARD) return;
    if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsHeldenianMap == true)
    {
        if (m_pClientList[iClientH]->m_cSide == 1)
        {
            m_iHeldenianAresdenDead++;
        }
        else if (m_pClientList[iClientH]->m_cSide == 2)
        {
            m_iHeldenianElvineDead++;
        }
        UpdateHeldenianStatus();
    }

    if (cAttackerType == DEF_OWNERTYPE_PLAYER)
    {
        // v1.432
        // Æ¯¼ö ´É·ÂÀÌ ÀÖ´Â ¹«±â·Î °ø°ÝÀ» ´çÇß´Ù.
        switch (m_pClientList[iAttackerH]->m_iSpecialAbilityType)
        {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                bIsSAattacked = true;
                break;
        }

        if (iAttackerH == iClientH) return; // ÀÚÆøÀÌ´Ù.
        // ÇÃ·¹ÀÌ¾î°¡ ÇÃ·¹ÀÌ¾î¸¦ Á×¿´´Ù¸é PKÀÎÁö ÀüÅõÁß ½Â¸®ÀÎÁö¸¦ ÆÇº°ÇÏ¿© °æÇèÄ¡¸¦ ¿Ã¸°´Ù. 
        if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0)
        {
            // Èñ»ýÀÚ°¡ ¿©ÇàÀÚÀÌ´Ù. 
            if (m_pClientList[iClientH]->m_iPKCount == 0)
            {


                // ¹«°íÇÑ ¿©ÇàÀÚ¿´´Ù. °ø°ÝÀÚ´Â PK ºÒÀÌÀÍÀ» ¹Þ´Â´Ù.
                ApplyPKpenalty(iAttackerH, iClientH);
            }
            else
            {

                // ¹üÁË¸¦ ÀúÁö¸¥ ¿©ÇàÀÚ¿´´Ù. °ø°ÝÀÚ´Â PK¸¦ ÀâÀº°Í¿¡ ´ëÇÑ Æ÷»óÀ» ¹Þ´Â´Ù. 
                PK_KillRewardHandler(iAttackerH, iClientH);
            }
        }
        else
        {
            // Èñ»ýÀÚ°¡ ¿©ÇàÀÚ°¡ ¾Æ´Ï¶ó ÇÑ ¸¶À» ¼Ò¼ÓÀÌ´Ù. 
            if (m_pClientList[iClientH]->m_iGuildRank == -1)
            {
                // Èñ»ýÀÚ´Â ½Ã¹ÎÀÌ´Ù.
                // °ø°ÝÀÚ°¡ ¿©ÇàÀÚ, °°Àº ¸¶À» ½Ã¹Î, °°Àº ¸¶À» ±æµå¿ø -> PK°¡ µÊ. ´Ù¸¥ ¸¶À» ½Ã¹Î, ±æµå¿ø -> Á¤´çÇÑ °ø°Ý
                if (memcmp(m_pClientList[iAttackerH]->m_cLocation, "NONE", 4) == 0)
                {
                    // °ø°ÝÀÚ°¡ ¿©ÇàÀÚÀÌ´Ù. 				
                    if (m_pClientList[iClientH]->m_iPKCount == 0)
                    {
                        // ¹«°íÇÑ ½Ã¹ÎÀ» ¿©ÇàÀÚ°¡ Á×¿´´Ù. ¿©ÇàÀÚ´Â PK°¡ µÈ´Ù.
                        ApplyPKpenalty(iAttackerH, iClientH);
                    }
                    else
                    {
                        // ¿©ÇàÀÚ´Â PK¸¦ Àâ¾Æµµ º¸»óÀ» ¹ÞÁö ¸øÇÑ´Ù.

                    }
                }
                else
                {
                    // °ø°ÝÀÚ°¡ ½Ã¹Î, È¤Àº ±æµå¿ø -> ¸¶À»ÀÌ °°À¸¸é PK, ´Ù¸¥ ¸¶À»ÀÌ¶ó¸é Á¤´çÇÑ °ø°Ý 
                    if (memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[iAttackerH]->m_cLocation, 10) == 0)
                    {
                        // °°Àº ¸¶À» ½Ã¹ÎÀ» °°Àº¸¶À» ½Ã¹Î È¤Àº ±æµå¿øÀÌ °ø°ÝÇÑ °ÍÀÌ´Ù.  
                        if (m_pClientList[iClientH]->m_iPKCount == 0)
                        {
                            // Èñ»ýÀÚ°¡ ¹«°íÇÑ »óÅÂ. PKÀÌ´Ù.
                            ApplyPKpenalty(iAttackerH, iClientH);
                        }
                        else
                        {
                            // ¹üÁËÀÚ¸¦ Àâ¾Ò´Ù. 
                            PK_KillRewardHandler(iAttackerH, iClientH);
                        }
                    }
                    else
                    {
                        // °ø°ÝÀÚ°¡ ´Ù¸¥ ¸¶À» ¼Ò¼Ó. Á¤´çÇÑ °ø°ÝÇàÀ§
                        EnemyKillRewardHandler(iAttackerH, iClientH);
                    }
                }
            }
            else
            {
                // Èñ»ýÀÚ´Â ±æµå¿øÀÌ´Ù. 
                // °ø°ÝÇÑ ÀÚ°¡ ¿©ÇàÀÚ, ½Ã¹Î, ÀüÀï»óÅÂ°¡ ¾Æ´Ñ ±æµå¿ø-> PK / Àü»ý»óÅÂÁßÀÎ ±æµå¿ø -> Á¤´çÇÑ ÀüÅõÇàÀ§
                if (memcmp(m_pClientList[iAttackerH]->m_cLocation, "NONE", 4) == 0)
                {
                    // °ø°ÝÀÚ°¡ ¿©ÇàÀÚÀÌ´Ù.
                    if (m_pClientList[iClientH]->m_iPKCount == 0)
                    {
                        // ¹«°íÇÑ ±æµå¿øÀ» ¿©ÇàÀÚ°¡ Á×¿´´Ù. ¿©ÇàÀÚ´Â PK°¡ µÈ´Ù.
                        ApplyPKpenalty(iAttackerH, iClientH);
                    }
                    else
                    {
                        // ¿©ÇàÀÚ´Â PK¸¦ Àâ¾Æµµ º¸»óÀ» ¹ÞÁö ¸øÇÑ´Ù.

                    }
                }
                else
                {
                    // °ø°ÝÀÚ´Â ½Ã¹Î È¤Àº ±æµå¿ø -> ¸¶À»ÀÌ °°À¸¸é PK, ´Ù¸¥ ¸¶À»ÀÌ¶ó¸é Á¤´çÇÑ °ø°Ý 
                    if (memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[iAttackerH]->m_cLocation, 10) == 0)
                    {
                        // °°Àº ¸¶À» ±æµå¿øÀ» °°Àº¸¶À» ½Ã¹Î È¤Àº ±æµå¿øÀÌ °ø°ÝÇÑ °ÍÀÌ´Ù.  
                        if (m_pClientList[iClientH]->m_iPKCount == 0)
                        {
                            // Èñ»ýÀÚ°¡ ¹«°íÇÑ »óÅÂ. PKÀÌ´Ù.
                            ApplyPKpenalty(iAttackerH, iClientH);
                        }
                        else
                        {
                            // ¹üÁËÀÚ¸¦ Àâ¾Ò´Ù. 
                            PK_KillRewardHandler(iAttackerH, iClientH);
                        }
                    }
                    else
                    {
                        // °ø°ÝÀÚ°¡ ´Ù¸¥ ¸¶À» ¼Ò¼Ó. Á¤´çÇÑ °ø°ÝÇàÀ§
                        EnemyKillRewardHandler(iAttackerH, iClientH);
                    }
                }
            }
        }

        // Èñ»ý´çÇÑ ÇÃ·¹ÀÌ¾îÀÇ µî±Þ¿¡ ¸Â´Â Æä³ÎÆ¼¸¦ ÃëÇÑ´Ù. 
        if (m_pClientList[iClientH]->m_iPKCount == 0)
        {
            // Innocent
            if (memcmp(m_pClientList[iAttackerH]->m_cLocation, "NONE", 4) == 0)
            {
                //¿©ÇàÀÚ¿¡°Ô °ø°Ý¹Þ¾Æ Á×¾ú´Ù. 
                //PK´çÇßÀ¸¹Ç·Î °æÇèÄ¡°¡ ÁÙÁö ¾Ê´Â´Ù.
                //m_pClientList[iClientH]->m_iExp -= iDice(1, 100);
                //if (m_pClientList[iClientH]->m_iExp < 0) m_pClientList[iClientH]->m_iExp = 0;
                //SendNotifyMsg(0, iClientH, DEF_NOTIFY_EXP, 0, 0, 0, 0);
            }
            else
            {
                if (memcmp(m_pClientList[iAttackerH]->m_cLocation, m_pClientList[iClientH]->m_cLocation, 10) == 0)
                {
                    //°°Àº ¸¶À» ½Ã¹Î È¤Àº ±æµå¿ø¿¡°Ô Á×¾ú´Ù. 
                    //PK´çÇßÀ¸¹Ç·Î °æÇèÁö°¡ ÁÙÁö ¾Ê´Â´Ù. 
                    //m_pClientList[iClientH]->m_iExp -= iDice(1, 100);
                    //if (m_pClientList[iClientH]->m_iExp < 0) m_pClientList[iClientH]->m_iExp = 0;
                    //SendNotifyMsg(0, iClientH, DEF_NOTIFY_EXP, 0, 0, 0, 0);
                }
                else
                {
                    // ´Ù¸¥ ¸¶À» ½Ã¹Î È¤Àº ±æµå¿ø¿¡°Ô Á×¾ú´Ù. ÀüÀïÁß Á×¾úÀ¸¹Ç·Î °æÇèÄ¡¿Í ¾ÆÀÌÅÛÀÌ ¶³¾îÁø´Ù. 
                    ApplyCombatKilledPenalty(iClientH, 2, bIsSAattacked);
                }
            }
        }
        else if ((m_pClientList[iClientH]->m_iPKCount >= 1) && (m_pClientList[iClientH]->m_iPKCount <= 3))
        {
            // Criminal 
            ApplyCombatKilledPenalty(iClientH, 3, bIsSAattacked);
        }
        else if ((m_pClientList[iClientH]->m_iPKCount >= 4) && (m_pClientList[iClientH]->m_iPKCount <= 11))
        {
            // Murderer 
            ApplyCombatKilledPenalty(iClientH, 6, bIsSAattacked);
        }
        else if ((m_pClientList[iClientH]->m_iPKCount >= 12))
        {
            // Slaughterer 
            ApplyCombatKilledPenalty(iClientH, 12, bIsSAattacked);
        }
    }
    else if (cAttackerType == DEF_OWNERTYPE_NPC)
    {

        _bPKLog(DEF_PKLOG_BYNPC, iClientH, 0, cAttackerName);

        // ÇÃ·¹ÀÌ¾î°¡ ¸÷¿¡°Ô Á×¾ú´Ù. Èñ»ý´çÇÑ ÇÃ·¹ÀÌ¾îÀÇ µî±Þ¿¡ ¸Â´Â Æä³ÎÆ¼¸¦ ÃëÇÑ´Ù.
        if (m_pClientList[iClientH]->m_iPKCount == 0)
        {
            // Innocent
            ApplyCombatKilledPenalty(iClientH, 1, bIsSAattacked);
        }
        else if ((m_pClientList[iClientH]->m_iPKCount >= 1) && (m_pClientList[iClientH]->m_iPKCount <= 3))
        {
            // Criminal 
            ApplyCombatKilledPenalty(iClientH, 3, bIsSAattacked);
        }
        else if ((m_pClientList[iClientH]->m_iPKCount >= 4) && (m_pClientList[iClientH]->m_iPKCount <= 11))
        {
            // Murderer 
            ApplyCombatKilledPenalty(iClientH, 6, bIsSAattacked);
        }
        else if ((m_pClientList[iClientH]->m_iPKCount >= 12))
        {
            // Slaughterer 
            ApplyCombatKilledPenalty(iClientH, 12, bIsSAattacked);
        }
        // ¸¸¾à °ø°ÝÇÑ NPC°¡ ÀüÀï¿ë À¯´ÏÆ®¶ó¸é ÁöÈÖ°ü¿¡°Ô °Ç¼³ Æ÷ÀÎÆ® ºÎ°¡
        if (m_pNpcList[iAttackerH]->m_iGuildGUID != 0)
        {

            if (m_pNpcList[iAttackerH]->m_cSide != m_pClientList[iClientH]->m_cSide)
            {
                // ÀüÀï¿ë ±¸Á¶¹° È¤Àº À¯´ÏÆ®°¡ Àû ÇÃ·¹ÀÌ¾î¸¦ Á×¿´´Ù. ¹Ù·Î Åëº¸ÇÑ´Ù.
                // ÇöÀç ¼­¹ö¿¡ ÁöÈÖ°üÀÌ ÀÖ´Ù¸é °ð¹Ù·Î Åëº¸. ¾øÀ¸¸é ´Ù¸¥ ¼­¹ö·Î ¾Ë·ÁÁÜ.
                for (i = 1; i < DEF_MAXCLIENTS; i++)
                    if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_iGuildGUID == m_pNpcList[iAttackerH]->m_iGuildGUID) &&
                        (m_pClientList[i]->m_iCrusadeDuty == 3))
                    {
                        m_pClientList[i]->m_iConstructionPoint += (m_pClientList[iClientH]->m_iLevel / 2);

                        if (m_pClientList[i]->m_iConstructionPoint > DEF_MAXCONSTRUCTIONPOINT)
                            m_pClientList[i]->m_iConstructionPoint = DEF_MAXCONSTRUCTIONPOINT;

                        //testcode
                        copy_string(G_cTxt, "Enemy Player Killed by Npc! Construction +%d", (m_pClientList[iClientH]->m_iLevel / 2));
                        log->info(G_cTxt);
                        // ÁöÈÖ°ü¿¡°Ô ¹Ù·Î Åëº¸.
                        SendNotifyMsg(0, i, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[i]->m_iConstructionPoint, m_pClientList[i]->m_iWarContribution, 0, 0);
                        return;
                    }

                // ÇöÀç ¼­¹ö¿¡ ¾ø´Ù. ´Ù¸¥ ¼­¹öÀÇ ÁöÈÖ°ü¿¡°Ô ¾Ë·Á¾ß ÇÑ´Ù.
                memset(cData, 0, sizeof(cData));
                cp = (char *)cData;
                *cp = GSM_CONSTRUCTIONPOINT;
                cp++;
                ip = (int *)cp;
                *ip = m_pNpcList[iAttackerH]->m_iGuildGUID;
                cp += 4;
                ip = (int *)cp;
                *ip = (m_pClientList[iClientH]->m_iLevel / 2);
                cp += 4;
                bStockMsgToGateServer(cData, 9);
            }
        }
    }
    else if (cAttackerType == DEF_OWNERTYPE_PLAYER_INDIRECT)
    {
        _bPKLog(DEF_PKLOG_BYOTHER, iClientH, 0, 0);
        // ÇÃ·¹ÀÌ¾î°¡ Á×¾úÁö¸¸ °ø°ÝÀÚ°¡ °£Á¢ÀûÀÌ´Ù. ¾Æ¹«·± ¿µÇâÀÌ ¾ø´Ù.
        // m_pClientList[iClientH]->m_iExp -= iDice(1, 50);
        // if (m_pClientList[iClientH]->m_iExp < 0) m_pClientList[iClientH]->m_iExp = 0;

        // SendNotifyMsg(0, iClientH, DEF_NOTIFY_EXP, 0, 0, 0, 0);
    }

    //Player Kill Notice - (C)copyrights to XTra KrazzY 2005+ | All rights reserved.
    //Hb-Zion Team
    char cKillMsg[80];
    ZeroMemory(cKillMsg, sizeof(cKillMsg));


    switch (iDice(1, 4))
    {
        case 1:
            copy_string(cKillMsg, "%s whooped %s's ass!", cAttackerName, m_pClientList[iClientH]->m_cCharName);
            break;

        case 2:
            copy_string(cKillMsg, "%s smashed %s's face into the ground!", cAttackerName, m_pClientList[iClientH]->m_cCharName);
            break;

        case 3:
            copy_string(cKillMsg, "%s was sliced to pieces by %s!", m_pClientList[iClientH]->m_cCharName, cAttackerName);
            break;

        case 4:
            copy_string(cKillMsg, "%s was gutted by %s!", m_pClientList[iClientH]->m_cCharName, cAttackerName);
            break;

        default:
            copy_string(cKillMsg, "%s is now sleeping for good thanks to %s", m_pClientList[iClientH]->m_cCharName, cAttackerName);
            break;

    }
    log->info("{} killed {}", m_pClientList[iAttackerH]->m_cCharName, m_pClientList[iClientH]->m_cCharName);

    for (int iS = 0; iS < DEF_MAXCLIENTS; iS++)
    {
        if ((m_pClientList[iS] != 0))
        {
            EKAnnounce(iS, cKillMsg);
        }
    }
}

void CGame::ReleaseItemHandler(int iClientH, short sItemIndex, bool bNotice)
{
    char cEquipPos, cHeroArmorType;
    short  sTemp;
    int   iTemp;

    if (m_pClientList[iClientH] == nullptr) return;
    if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return;
    if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == 0) return;
    if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType != DEF_ITEMTYPE_EQUIP) return;

    if (m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] == false) return;

    cHeroArmorType = _cCheckHeroItemEquipped(iClientH);
    if (cHeroArmorType != 0x0FFFFFFFF) m_pClientList[iClientH]->m_cHeroArmourBonus = 0;

    cEquipPos = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos;
    if (cEquipPos == DEF_EQUIPPOS_RHAND)
    {
        if (m_pClientList[iClientH]->m_pItemList[sItemIndex] != 0)
        {
            if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 865) || (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 866))
            {
                m_pClientList[iClientH]->m_cMagicMastery[94] = false;
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_SUCCESS, 0, 0, 0, 0);
            }
        }
    }

    // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ Ã‡Ã˜ÃÂ¦Ã€Â§Ã„Â¡Â¿Â¡ Â¸Ã‚Â°Ã” ApprÂºÂ¯Â¼Ã¶Â¸Â¦ ÃÂ¶ÃÂ¤Ã‡Ã‘Â´Ã™.
    switch (cEquipPos)
    {
        case DEF_EQUIPPOS_RHAND:
            // Â¿Ã€Â¸Â¥Â¼Ã•Â¿Â¡ Ã€Ã¥Ã‚Ã¸Ã‡ÃÂ´Ã‚ Â¾Ã†Ã€ÃŒÃ…Ã›. Â¹Â«Â±Ã¢Â·Ã¹Ã€ÃŒÂ´Ã™. 
            sTemp = m_pClientList[iClientH]->m_sAppr2;
            sTemp = sTemp & 0xF00F;	// Â¹Â«Â±Ã¢ Ã†Â¯Â¼ÂºÃ„Â¡ ÂºÃ±Ã†Â®Â¸Â¦ Ã…Â¬Â¸Â®Â¾Ã®.
            m_pClientList[iClientH]->m_sAppr2 = sTemp;

            iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 Ã„ÃƒÂ·Â¯Â°Âª Â¼Â¼Ã†Ãƒ 
            iTemp = iTemp & 0x0FFFFFFF; // Â¹Â«Â±Ã¢ Â»Ã¶ ÂºÃ±Ã†Â®Â¸Â¦ Ã…Â¬Â¸Â®Â¾Ã®.
            m_pClientList[iClientH]->m_iApprColor = iTemp;

            // V1.3 Â¹Â«Â±Ã¢Â¼Ã“ÂµÂµ Ã€Ã§Â°Ã¨Â»Ãª <- Â¸Ã‡Â¼Ã•Ã€ÃŒÂ´ÃÂ±Ã± 0
            iTemp = m_pClientList[iClientH]->m_iStatus;
            iTemp = iTemp & 0xFFFFFFF0;
            m_pClientList[iClientH]->m_iStatus = iTemp;
            break;

        case DEF_EQUIPPOS_LHAND:
            // Â¿ÃžÂ¼Ã•Â¿Â¡ Ã€Ã¥Ã‚Ã¸Ã‡ÃÂ´Ã‚ Â¾Ã†Ã€ÃŒÃ…Ã›. Â¹Ã¦Ã†ÃÂ·Ã¹Ã€ÃŒÂ´Ã™. 
            sTemp = m_pClientList[iClientH]->m_sAppr2;
            sTemp = sTemp & 0xFFF0;	// Â¹Â«Â±Ã¢ Ã†Â¯Â¼ÂºÃ„Â¡ ÂºÃ±Ã†Â®Â¸Â¦ Ã…Â¬Â¸Â®Â¾Ã®.
            m_pClientList[iClientH]->m_sAppr2 = sTemp;

            iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 Ã„ÃƒÂ·Â¯Â°Âª Â¼Â¼Ã†Ãƒ 
            iTemp = iTemp & 0xF0FFFFFF; // Â»Ã¶ ÂºÃ±Ã†Â®Â¸Â¦ Ã…Â¬Â¸Â®Â¾Ã®.
            m_pClientList[iClientH]->m_iApprColor = iTemp;
            break;

        case DEF_EQUIPPOS_TWOHAND:
            // Â¾Ã§Â¼Ã•Â¿Â¡ Ã€Ã¥Ã‚Ã¸Ã‡Ã‘Â´Ã™ÃÃ¶Â¸Â¸ Â»Ã§Â½Ã‡ Â¿Ã€Â¸Â¥Â¼Ã•Â¿Â¡Â¸Â¸ ÂµÃ©Â°Ã­ Ã€Ã–Â´Ã™. Â¿ÃžÂ¼Ã•Ã€Âº ÂºÃ±Â¾Ã®Ã€Ã–Â´Ã‚ Â»Ã³Ã…Ã‚.
            sTemp = m_pClientList[iClientH]->m_sAppr2;
            sTemp = sTemp & 0xF00F;	// Â¹Â«Â±Ã¢ Ã†Â¯Â¼ÂºÃ„Â¡ ÂºÃ±Ã†Â®Â¸Â¦ Ã…Â¬Â¸Â®Â¾Ã®.
            m_pClientList[iClientH]->m_sAppr2 = sTemp;

            iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 Ã„ÃƒÂ·Â¯Â°Âª Â¼Â¼Ã†Ãƒ 
            iTemp = iTemp & 0x0FFFFFFF; // Â¹Â«Â±Ã¢ Â»Ã¶ ÂºÃ±Ã†Â®Â¸Â¦ Ã…Â¬Â¸Â®Â¾Ã®.
            m_pClientList[iClientH]->m_iApprColor = iTemp;
            break;

        case DEF_EQUIPPOS_BODY:
            sTemp = m_pClientList[iClientH]->m_sAppr3;
            sTemp = sTemp & 0x0FFF;	// Â°Ã‘Â¿ÃŠ Ã†Â¯Â¼ÂºÃ„Â¡ ÂºÃ±Ã†Â®Â¸Â¦ Ã…Â¬Â¸Â®Â¾Ã®.
            m_pClientList[iClientH]->m_sAppr3 = sTemp;

            // Â°Ã‘Â¿ÃŠ ÃˆÂ®Ã€Ã¥ Ã†Â¯Â¼ÂºÃ„Â¡ ÂºÃ±Ã†Â®Â¸Â¦ Ã…Â¬Â¸Â®Â¾Ã®.
            sTemp = m_pClientList[iClientH]->m_sAppr4;
            sTemp = sTemp & 0xFF7F;
            m_pClientList[iClientH]->m_sAppr4 = sTemp;

            iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 Ã„ÃƒÂ·Â¯Â°Âª Â¼Â¼Ã†Ãƒ 
            iTemp = iTemp & 0xFF0FFFFF; // Â»Ã¶ ÂºÃ±Ã†Â®Â¸Â¦ Ã…Â¬Â¸Â®Â¾Ã®.
            m_pClientList[iClientH]->m_iApprColor = iTemp;
            break;

        case DEF_EQUIPPOS_BACK:
            sTemp = m_pClientList[iClientH]->m_sAppr4;
            sTemp = sTemp & 0xF0FF;	// Â¸ÃÃ…Ã¤ Ã†Â¯Â¼ÂºÃ„Â¡ ÂºÃ±Ã†Â®Â¸Â¦ Ã…Â¬Â¸Â®Â¾Ã®.
            m_pClientList[iClientH]->m_sAppr4 = sTemp;

            iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 Ã„ÃƒÂ·Â¯Â°Âª Â¼Â¼Ã†Ãƒ 
            iTemp = iTemp & 0xFFF0FFFF; // Â»Ã¶ ÂºÃ±Ã†Â®Â¸Â¦ Ã…Â¬Â¸Â®Â¾Ã®.
            m_pClientList[iClientH]->m_iApprColor = iTemp;
            break;

        case DEF_EQUIPPOS_ARMS:
            sTemp = m_pClientList[iClientH]->m_sAppr3;
            sTemp = sTemp & 0xFFF0;	// Â°Ã‘Â¿ÃŠ Ã†Â¯Â¼ÂºÃ„Â¡ ÂºÃ±Ã†Â®Â¸Â¦ Ã…Â¬Â¸Â®Â¾Ã®.
            m_pClientList[iClientH]->m_sAppr3 = sTemp;

            iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 Ã„ÃƒÂ·Â¯Â°Âª Â¼Â¼Ã†Ãƒ 
            iTemp = iTemp & 0xFFFF0FFF; // Â»Ã¶ ÂºÃ±Ã†Â®Â¸Â¦ Ã…Â¬Â¸Â®Â¾Ã®.
            m_pClientList[iClientH]->m_iApprColor = iTemp;
            break;

        case DEF_EQUIPPOS_PANTS:
            sTemp = m_pClientList[iClientH]->m_sAppr3;
            sTemp = sTemp & 0xF0FF;	// Â°Ã‘Â¿ÃŠ Ã†Â¯Â¼ÂºÃ„Â¡ ÂºÃ±Ã†Â®Â¸Â¦ Ã…Â¬Â¸Â®Â¾Ã®.
            m_pClientList[iClientH]->m_sAppr3 = sTemp;

            iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 Ã„ÃƒÂ·Â¯Â°Âª Â¼Â¼Ã†Ãƒ 
            iTemp = iTemp & 0xFFFFF0FF; // Â»Ã¶ ÂºÃ±Ã†Â®Â¸Â¦ Ã…Â¬Â¸Â®Â¾Ã®.
            m_pClientList[iClientH]->m_iApprColor = iTemp;
            break;

        case DEF_EQUIPPOS_LEGGINGS:
            sTemp = m_pClientList[iClientH]->m_sAppr4;
            sTemp = sTemp & 0x0FFF;	// Â°Ã‘Â¿ÃŠ Ã†Â¯Â¼ÂºÃ„Â¡ ÂºÃ±Ã†Â®Â¸Â¦ Ã…Â¬Â¸Â®Â¾Ã®.
            m_pClientList[iClientH]->m_sAppr4 = sTemp;

            iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 Ã„ÃƒÂ·Â¯Â°Âª Â¼Â¼Ã†Ãƒ 
            iTemp = iTemp & 0xFFFFFF0F; // Â»Ã¶ ÂºÃ±Ã†Â®Â¸Â¦ Ã…Â¬Â¸Â®Â¾Ã®.
            m_pClientList[iClientH]->m_iApprColor = iTemp;
            break;

        case DEF_EQUIPPOS_HEAD:
            sTemp = m_pClientList[iClientH]->m_sAppr3;
            sTemp = sTemp & 0xFF0F;	// Ã…ÃµÂ±Â¸ Ã†Â¯Â¼ÂºÃ„Â¡ ÂºÃ±Ã†Â®Â¸Â¦ Ã…Â¬Â¸Â®Â¾Ã®.
            m_pClientList[iClientH]->m_sAppr3 = sTemp;

            iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 Ã„ÃƒÂ·Â¯Â°Âª Â¼Â¼Ã†Ãƒ 
            iTemp = iTemp & 0xFFFFFFF0; // Â»Ã¶ ÂºÃ±Ã†Â®Â¸Â¦ Ã…Â¬Â¸Â®Â¾Ã®.
            m_pClientList[iClientH]->m_iApprColor = iTemp;
            break;

        case DEF_EQUIPPOS_RELEASEALL:
            sTemp = m_pClientList[iClientH]->m_sAppr3;
            sTemp = sTemp & 0x0FFF;	// Â¸ÃÃ…Ã¤ Ã†Â¯Â¼ÂºÃ„Â¡ ÂºÃ±Ã†Â®Â¸Â¦ Ã…Â¬Â¸Â®Â¾Ã®.
            m_pClientList[iClientH]->m_sAppr3 = sTemp;

            iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 Ã„ÃƒÂ·Â¯Â°Âª Â¼Â¼Ã†Ãƒ 
            iTemp = iTemp & 0xFFF0FFFF; // Â¸ÃÃ…Ã¤ Â»Ã¶ ÂºÃ±Ã†Â®Â¸Â¦ Ã…Â¬Â¸Â®Â¾Ã®.
            m_pClientList[iClientH]->m_iApprColor = iTemp;
            break;
    }

    //v1.432 Ã†Â¯Â¼Ã¶ Â´Ã‰Â·Ã‚Ã€ÃŒ ÂºÃŽÂ¿Â©ÂµÃˆ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒÂ¶Ã³Â¸Ã© Ã‡ÃƒÂ·Â¡Â±Ã— Â¼Â³ÃÂ¤ 
    if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY)
    {
        // Â°Ã¸Â°Ã Ã†Â¯Â¼Ã¶ Â´Ã‰Â·Ã‚ ÂºÃ±Ã†Â® Ã…Â¬Â¸Â®Â¾Ã®
        m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 & 0xFFF3;
    }

    if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY)
    {
        // Â¹Ã¦Â¾Ã®Â±Â¸ Ã†Â¯Â¼Ã¶ Â´Ã‰Â·Ã‚ ÂºÃ±Ã†Â® Ã…Â¬Â¸Â®Â¾Ã®
        m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 & 0xFFFC;
    }

    m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] = false;
    m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos] = -1;

    // Â¿ÃœÃ‡Ã¼Ã€ÃŒ Â¹Ã™Â²Ã¯Â°ÃÃ€Â» Â¾Ã‹Â¸Â°Â´Ã™.
    if (bNotice == true)
        SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);

    // Ã€ÃŒ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Ã‡Ã˜ÃÂ¦Ã‡ÃÂ´Ã‚ Â°ÃÂ¿Â¡ ÂµÃ»Â¸Â£Â´Ã‚ Ã†Â¯Â¼ÂºÃ„Â¡Ã€Ã‡ ÂºÂ¯ÃˆÂ­Â¸Â¦ Â°Ã¨Â»ÃªÃ‡Ã‘Â´Ã™. 
    CalcTotalItemEffect(iClientH, sItemIndex, true);
}

void CGame::GuildNotifyHandler(char * pData, uint32_t dwMsgSize)
{
    // Â´Ã™Â¸Â¥ Â°Ã”Ã€Ã“Â¼Â­Â¹Ã¶Â·ÃŽÂºÃŽÃ…Ã Â±Ã¦ÂµÃ¥ Ã€ÃŒÂºÂ¥Ã†Â®Â°Â¡ ÂµÂµÃ‚Ã¸Ã‡ÃŸÂ´Ã™. 
    char * cp, cCharName[11], cGuildName[21];

    memset(cCharName, 0, sizeof(cCharName));
    memset(cGuildName, 0, sizeof(cGuildName));

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

    memcpy(cCharName, cp, 10);
    cp += 10;

    memcpy(cGuildName, cp, 20);
    cp += 20;

    // Â¾Ã†ÃÃ· Â±Â¸Ã‡Ã¶ÂµÃ‡ÃÃ¶ Â¾ÃŠÂ¾Ã’Â´Ã™. 
}

void CGame::ToggleCombatModeHandler(int iClientH)
{
    short sAppr2;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    if (m_pClientList[iClientH]->m_bIsKilled == true) return;
    // ÃÃ—Ã€ÂºÃƒÂ´Ã‡ÃÂ±Ã¢Â³Âª ÃÃ—Ã€Âº Â»Ã³Ã…Ã‚Â¿Â¡Â¼Â­Â´Ã‚ ÂºÂ¯ÃˆÂ¯Ã‡Ã’ Â¼Ã¶ Â¾Ã¸Â´Ã™.
    if (m_pClientList[iClientH]->m_bSkillUsingStatus[19] == true) return;

    sAppr2 = (short)((m_pClientList[iClientH]->m_sAppr2 & 0xF000) >> 12);

    m_pClientList[iClientH]->m_bIsAttackModeChange = true; // v2.172


    if (sAppr2 == 0)
    {
        // ÂºÃ±Ã€Ã¼Ã…Ãµ Â¸Ã°ÂµÃ¥Â¿Â´Â´Ã™. Ã€Ã¼Ã…ÃµÂ¸Ã°ÂµÃ¥Â·ÃŽ Â¹Ã™Â²Ã›Â´Ã™.
        m_pClientList[iClientH]->m_sAppr2 = (0xF000 | m_pClientList[iClientH]->m_sAppr2);
    }
    else
    {
        // Ã€Ã¼Ã…Ãµ Â¸Ã°ÂµÃ¥Â¿Â´Â´Ã™. ÂºÃ±Ã€Ã¼Ã…ÃµÂ¸Ã°ÂµÃ¥Â·ÃŽ Â¹Ã™Â²Ã›Â´Ã™.
        m_pClientList[iClientH]->m_sAppr2 = (0x0FFF & m_pClientList[iClientH]->m_sAppr2);
    }

    // Ã„Â³Â¸Â¯Ã…ÃÃ€Ã‡ Â¿ÃœÃ‡Ã¼Ã€ÃŒ Â¹Ã™Â²Ã®Â¾ÃºÃ€Â¸Â¹Ã‡Â·ÃŽ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ Ã€Ã¼Â´ÃžÃ‡Ã‘Â´Ã™.
    SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);

}

/////////////////////////////////////////////////////////////////////////////////////
//  int CGame::iClientMotion_Magic_Handler(int iClientH, short sX, short sY, char cDir)
//  description			:: checks if player is casting magic
//  last updated		:: October 29, 2004; 6:51 PM; Hypnotoad
//	return value		:: int
/////////////////////////////////////////////////////////////////////////////////////
int CGame::iClientMotion_Magic_Handler(int iClientH, short sX, short sY, char cDir)
{
    char  cData[100];
    uint32_t * dwp;
    uint16_t * wp;
    int     iRet;

    if (m_pClientList[iClientH] == nullptr) return 0;
    if (m_pClientList[iClientH]->m_bIsKilled == true) return 0;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return 0;

    if ((sX != m_pClientList[iClientH]->m_sX) || (sY != m_pClientList[iClientH]->m_sY)) return 2;

    int iStX, iStY;
    if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex] != 0)
    {
        iStX = m_pClientList[iClientH]->m_sX / 20;
        iStY = m_pClientList[iClientH]->m_sY / 20;
        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iPlayerActivity++;

        switch (m_pClientList[iClientH]->m_cSide)
        {
            case 0: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iNeutralActivity++; break;
            case 1: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iAresdenActivity++; break;
            case 2: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iElvineActivity++;  break;
        }
    }

    ClearSkillUsingStatus(iClientH);

    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->ClearOwner(0, iClientH, DEF_OWNERTYPE_PLAYER, sX, sY);
    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetOwner(iClientH, DEF_OWNERTYPE_PLAYER, sX, sY);

    if ((m_pClientList[iClientH]->m_iStatus & 0x10) != 0)
    {
        SetInvisibilityFlag(iClientH, DEF_OWNERTYPE_PLAYER, false);
        bRemoveFromDelayEventList(iClientH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
        m_pClientList[iClientH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = 0;
    }

    m_pClientList[iClientH]->m_cDir = cDir;

    dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
    *dwp = MSGID_RESPONSE_MOTION;
    wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
    *wp = DEF_OBJECTMOTION_CONFIRM;

    iRet = m_pClientList[iClientH]->send_msg(cData, 6);
    switch (iRet)
    {
        case DEF_XSOCKEVENT_QUENEFULL:
        case DEF_XSOCKEVENT_SOCKETERROR:
        case DEF_XSOCKEVENT_CRITICALERROR:
        case DEF_XSOCKEVENT_SOCKETCLOSED:
            DeleteClient(iClientH, true, true);
            return 0;
    }

    return 1;
}

/*********************************************************************************************************************
**  void CGame::PlayerMagicHandler(int iClientH, int dX, int dY, short sType, bool bItemEffect, int iV1)			**
**  description			:: handles all magic related items/spells													**
**  last updated		:: November 22, 2004; 5:45 PM; Hypnotoad													**
**	return value		:: void																						**
**  commentary			::	-	added 3.51 casting detection														**
**							-	updated it so civilians can only cast certain spells on players and vice versa		**
**							-	fixed bug causing spell to be cast when mana is below required amount				**
**********************************************************************************************************************/
int  _tmp_iMCProb[] = { 0, 300, 250, 200, 150, 100, 80, 70, 60, 50, 40 };
int  _tmp_iMLevelPenalty[] = { 0,   5,   5,   8,   8,  10, 14, 28, 32, 36, 40 };
void CGame::PlayerMagicHandler(int iClientH, int dX, int dY, short sType, bool bItemEffect, int iV1)
{
    short * sp, sX, sY, sOwnerH, sMagicCircle, rx, ry, sRemainItemSprite, sRemainItemSpriteFrame, sLevelMagic, sTemp;
    char * cp, cData[120], cDir, cOwnerType, cName[11], cItemName[21], cNpcWaypoint[11], cName_Master[11], cNpcName[21], cRemainItemColor, cScanMessage[256];
    double dV1, dV2, dV3, dV4;
    int    i, iErr, iRet, ix, iy, iResult, iDiceRes, iNamingValue, iFollowersNum, iEraseReq, iWhetherBonus;
    int    tX, tY, iManaCost, iMagicAttr;
    class  CItem * pItem;
    uint32_t * dwp, dwTime;
    uint16_t * wp, wWeaponType;
    short sEqStatus;
    int iMapSide = 0;

    dwTime = timeGetTime();
    m_pClientList[iClientH]->m_bMagicConfirm = true;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

    if ((dX < 0) || (dX >= m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_sSizeX) ||
        (dY < 0) || (dY >= m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_sSizeY)) return;

    if (((dwTime - m_pClientList[iClientH]->m_dwRecentAttackTime) < 1000) && (bItemEffect == 0))
    {
        try
        {
            copy_string(G_cTxt, "3.51 Detection: (%s) Player: (%s) - Magic casting speed is too fast! Hack?", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
            PutHackLogFileList(G_cTxt);
            DeleteClient(iClientH, true, true);
        }
        catch (...)
        {
        }
        return;
    }
    m_pClientList[iClientH]->m_dwRecentAttackTime = dwTime;
    m_pClientList[iClientH]->m_dwLastActionTime = dwTime;

    if (m_pClientList[iClientH]->m_cMapIndex < 0) return;
    if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex] == 0) return;

    if ((sType < 0) || (sType >= 100))     return;
    if (m_pMagicConfigList[sType] == 0) return;

    if ((bItemEffect == false) && (m_pClientList[iClientH]->m_cMagicMastery[sType] != 1)) return;

    if ((m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsAttackEnabled == false) && (m_pClientList[iClientH]->m_iAdminUserLevel == 0)) return;
    //if ((var_874 == true) && (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsHeldenianMap == true) && (m_pMagicConfigList[sType]->m_sType != 8)) return;

    if (((m_pClientList[iClientH]->m_iStatus & 0x100000) != 0) && (bItemEffect != true))
    {
        SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, 0, -1, 0);
        return;
    }

    if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND] != -1)
    {
        wWeaponType = ((m_pClientList[iClientH]->m_sAppr2 & 0x0FF0) >> 4);
        if ((wWeaponType >= 34) && (wWeaponType <= 39))
        {
        }
        else return;
    }

    if ((m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LHAND] != -1) ||
        (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1)) return;

    if ((m_pClientList[iClientH]->m_iSpellCount > 1) && (bItemEffect == false))
    {
        try
        {
            copy_string(G_cTxt, "TSearch Spell Hack: (%s) Player: (%s) - casting magic without precasting.", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
            PutHackLogFileList(G_cTxt);
            DeleteClient(iClientH, true, true);
        }
        catch (...)
        {
        }
        return;
    }

    if (m_pClientList[iClientH]->m_bInhibition == true)
    {
        SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, 0, -1, 0);
        return;
    }

    /*if (((m_pClientList[iClientH]->m_iUninteruptibleCheck - (iGetMaxHP(iClientH)/10)) > (m_pClientList[iClientH]->m_iHP)) && (m_pClientList[iClientH]->m_bMagicItem == false)) {
        SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_MAGIC, 0,
            0, 0, 0, 0, 0, 0);
        return;
    }*/

    if (m_pMagicConfigList[sType]->m_sType == 32)
    { // Invisiblity
        sEqStatus = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
        if ((sEqStatus != -1) && (m_pClientList[iClientH]->m_pItemList[sEqStatus] != 0))
        {
            if ((m_pClientList[iClientH]->m_pItemList[sEqStatus]->m_sIDnum == 865) || (m_pClientList[iClientH]->m_pItemList[sEqStatus]->m_sIDnum == 866))
            {
                bItemEffect = true;
            }
        }
    }

    sX = m_pClientList[iClientH]->m_sX;
    sY = m_pClientList[iClientH]->m_sY;

    sMagicCircle = (sType / 10) + 1;
    if (m_pClientList[iClientH]->m_cSkillMastery[4] == 0)
        dV1 = 1.0f;
    else dV1 = (double)m_pClientList[iClientH]->m_cSkillMastery[4];

    if (bItemEffect == true) dV1 = (double)100.0f;
    dV2 = (double)(dV1 / 100.0f);
    dV3 = (double)_tmp_iMCProb[sMagicCircle];
    dV1 = dV2 * dV3;
    iResult = (int)dV1;

    if (m_pClientList[iClientH]->m_iInt > 50)
        iResult += (m_pClientList[iClientH]->m_iInt - 50) / 2;

    sLevelMagic = (m_pClientList[iClientH]->m_iLevel / 10);
    if (sMagicCircle != sLevelMagic)
    {
        if (sMagicCircle > sLevelMagic)
        {
            dV1 = (double)(m_pClientList[iClientH]->m_iLevel - sLevelMagic * 10);
            dV2 = (double)abs(sMagicCircle - sLevelMagic) * _tmp_iMLevelPenalty[sMagicCircle];
            dV3 = (double)abs(sMagicCircle - sLevelMagic) * 10;
            dV4 = (dV1 / dV3) * dV2;
            iResult -= abs(abs(sMagicCircle - sLevelMagic) * _tmp_iMLevelPenalty[sMagicCircle] - (int)dV4);
        }
        else
        {
            iResult += 5 * abs(sMagicCircle - sLevelMagic);
        }
    }

    switch (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cWhetherStatus)
    {
        case 0: break;
        case 1: iResult = iResult - (iResult / 24); break;
        case 2:	iResult = iResult - (iResult / 12); break;
        case 3: iResult = iResult - (iResult / 5);  break;
    }

    if (m_pClientList[iClientH]->m_iSpecialWeaponEffectType == 10)
    {
        dV1 = (double)iResult;
        dV2 = (double)(m_pClientList[iClientH]->m_iSpecialWeaponEffectValue * 3);
        dV3 = dV1 + dV2;
        iResult = (int)dV3;
    }

    if (iResult <= 0) iResult = 1;

    iWhetherBonus = iGetWhetherMagicBonusEffect(sType, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cWhetherStatus);

    iManaCost = m_pMagicConfigList[sType]->m_sValue1;
    if ((m_pClientList[iClientH]->m_bIsSafeAttackMode == true) &&
        (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFightZone == false))
    {
        iManaCost += (iManaCost / 2) - (iManaCost / 10);
    }

    if (m_pClientList[iClientH]->m_iManaSaveRatio > 0)
    {
        dV1 = (double)m_pClientList[iClientH]->m_iManaSaveRatio;
        dV2 = (double)(dV1 / 100.0f);
        dV3 = (double)iManaCost;
        dV1 = dV2 * dV3;
        dV2 = dV3 - dV1;
        iManaCost = (int)dV2;

        if (iManaCost <= 0) iManaCost = 1;
    }

    wWeaponType = ((m_pClientList[iClientH]->m_sAppr2 & 0x0FF0) >> 4);
    if (wWeaponType == 34)
    {
        iManaCost += 20;
    }

    if (iResult < 100)
    {
        iDiceRes = iDice(1, 100);
        if (iResult < iDiceRes)
        {
            SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, 0, -1, 0);
            return;
        }
    }

    if (((m_pClientList[iClientH]->m_iHungerStatus <= 10) || (m_pClientList[iClientH]->m_iSP <= 0)) && (iDice(1, 1000) <= 100))
    {
        SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, 0, -1, 0);
        return;
    }

    if (m_pClientList[iClientH]->m_iMP < iManaCost)
    {
        return;
    }

    iResult = m_pClientList[iClientH]->m_cSkillMastery[4];
    if (m_pClientList[iClientH]->m_iMag > 50) iResult += (m_pClientList[iClientH]->m_iMag - 50);

    sLevelMagic = (m_pClientList[iClientH]->m_iLevel / 10);
    if (sMagicCircle != sLevelMagic)
    {
        if (sMagicCircle > sLevelMagic)
        {
            dV1 = (double)(m_pClientList[iClientH]->m_iLevel - sLevelMagic * 10);
            dV2 = (double)abs(sMagicCircle - sLevelMagic) * _tmp_iMLevelPenalty[sMagicCircle];
            dV3 = (double)abs(sMagicCircle - sLevelMagic) * 10;
            dV4 = (dV1 / dV3) * dV2;

            iResult -= abs(abs(sMagicCircle - sLevelMagic) * _tmp_iMLevelPenalty[sMagicCircle] - (int)dV4);
        }
        else
        {
            iResult += 5 * abs(sMagicCircle - sLevelMagic);
        }
    }

    iResult += m_pClientList[iClientH]->m_iAddAR;
    if (iResult <= 0) iResult = 1;

    if (sType >= 80) iResult += 10000;

    if (m_pMagicConfigList[sType]->m_sType == 28)
    {
        iResult += 10000;
    }

    if (m_pMagicConfigList[sType]->m_cCategory == 1)
    {
        if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->iGetAttribute(sX, sY, 0x00000005) != 0) return;
    }

    iMagicAttr = m_pMagicConfigList[sType]->m_iAttribute;
    if ((m_pClientList[iClientH]->m_iStatus & 0x10) != 0)
    {
        SetInvisibilityFlag(iClientH, DEF_OWNERTYPE_PLAYER, false);
        bRemoveFromDelayEventList(iClientH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
        m_pClientList[iClientH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = 0;
    }

    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
    if ((m_bIsCrusadeMode == false) && (cOwnerType == DEF_OWNERTYPE_PLAYER))
    {
        if ((m_pClientList[iClientH]->m_bIsPlayerCivil != true) && (m_pClientList[sOwnerH]->m_bIsPlayerCivil == true))
        {
            if (m_pClientList[iClientH]->m_cSide != m_pClientList[sOwnerH]->m_cSide) return;
        }
        else if ((m_pClientList[iClientH]->m_bIsPlayerCivil == true) && (m_pClientList[sOwnerH]->m_bIsPlayerCivil == false))
        {
            switch (m_pMagicConfigList[sType]->m_sType)
            {
                case 1:  // DEF_MAGICTYPE_DAMAGE_SPOT
                case 4:  // DEF_MAGICTYPE_SPDOWN_SPOT 4
                case 8:  // DEF_MAGICTYPE_TELEPORT 8
                case 10: // DEF_MAGICTYPE_CREATE 10
                case 11: // DEF_MAGICTYPE_PROTECT 11
                case 12: // DEF_MAGICTYPE_HOLDOBJECT 12
                case 16: // DEF_MAGICTYPE_CONFUSE
                case 17: // DEF_MAGICTYPE_POISON
                case 32: // DEF_MAGICTYPE_RESURRECTION
                    return;
            }
        }
    }

    if (m_pMagicConfigList[sType]->m_dwDelayTime == 0)
    {
        switch (m_pMagicConfigList[sType]->m_sType)
        {

            case DEF_MAGICTYPE_DAMAGE_SPOT:
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
                if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                    Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);

                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
                if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0))
                {
                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                }
                break;

            case DEF_MAGICTYPE_HPUP_SPOT:
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
                Effect_HpUp_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6);
                break;

            case DEF_MAGICTYPE_DAMAGE_AREA:
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
                if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                    Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);

                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
                if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0))
                {
                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                }

                for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
                    for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++)
                    {
                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0))
                        {
                            if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                        }
                    }
                break;

            case DEF_MAGICTYPE_SPDOWN_SPOT:
                break;

            case DEF_MAGICTYPE_SPDOWN_AREA:
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
                if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                    Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6);
                for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
                    for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++)
                    {
                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                    }
                break;

            case DEF_MAGICTYPE_POLYMORPH:
                // Ã‚ÂºÃ‚Â¯Ã‚Â½Ãƒâ€¦ Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½. 
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
                if (1)
                { // bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) {
                    switch (cOwnerType)
                    {
                        case DEF_OWNERTYPE_PLAYER:
                            if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                            if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_POLYMORPH] != 0) goto MAGIC_NOEFFECT;
                            m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_POLYMORPH] = (char)m_pMagicConfigList[sType]->m_sValue4;
                            // Ã‚Â¿ÃƒÂ¸Ã‚Â·Ã‚Â¡ Ãƒâ€¦Ã‚Â¸Ãƒâ‚¬Ãƒâ€Ãƒâ‚¬Ã‚Â» Ãƒâ‚¬ÃƒÂºÃƒâ‚¬ÃƒÂ¥Ãƒâ€¡ÃƒËœ Ã‚Â³ÃƒÂµÃ‚Â´Ãƒâ€šÃ‚Â´Ãƒâ„¢.
                            m_pClientList[sOwnerH]->m_sOriginalType = m_pClientList[sOwnerH]->m_sType;
                            // Ã‚Â¹Ãƒâ„¢Ã‚Â²ÃƒÂ¯ Ã‚Â¿ÃƒÅ“Ãƒâ€¡ÃƒÂ¼Ãƒâ‚¬Ã‚Â» Ãƒâ€¦ÃƒÂ«Ã‚ÂºÃ‚Â¸Ãƒâ€¡ÃƒËœ ÃƒÂÃƒËœÃ‚Â´Ãƒâ„¢.
                            m_pClientList[sOwnerH]->m_sType = 18;
                            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
                            break;

                        case DEF_OWNERTYPE_NPC:
                            if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                            if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_POLYMORPH] != 0) goto MAGIC_NOEFFECT;
                            m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_POLYMORPH] = (char)m_pMagicConfigList[sType]->m_sValue4;
                            // Ã‚Â¿ÃƒÂ¸Ã‚Â·Ã‚Â¡ Ãƒâ€¦Ã‚Â¸Ãƒâ‚¬Ãƒâ€Ãƒâ‚¬Ã‚Â» Ãƒâ‚¬ÃƒÂºÃƒâ‚¬ÃƒÂ¥Ãƒâ€¡ÃƒËœ Ã‚Â³ÃƒÂµÃ‚Â´Ãƒâ€šÃ‚Â´Ãƒâ„¢.
                            m_pNpcList[sOwnerH]->m_sOriginalType = m_pNpcList[sOwnerH]->m_sType;
                            // Ã‚Â¹Ãƒâ„¢Ã‚Â²ÃƒÂ¯ Ã‚Â¿ÃƒÅ“Ãƒâ€¡ÃƒÂ¼Ãƒâ‚¬Ã‚Â» Ãƒâ€¦ÃƒÂ«Ã‚ÂºÃ‚Â¸Ãƒâ€¡ÃƒËœ ÃƒÂÃƒËœÃ‚Â´Ãƒâ„¢.
                            m_pNpcList[sOwnerH]->m_sType = 18;
                            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
                            break;
                    }

                    // Ã‚ÂºÃ‚Â¯Ã‚Â½Ãƒâ€¦ ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                    bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_POLYMORPH, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
                        sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

                    // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ã‚Â»ÃƒÂ½Ã‚Â°ÃƒÂ¥Ãƒâ‚¬Ã‚Â½Ãƒâ‚¬Ã‚Â» Ã‚Â¾Ãƒâ€¹Ã‚Â·ÃƒÂÃƒÂÃƒËœÃ‚Â´Ãƒâ„¢.
                    if (cOwnerType == DEF_OWNERTYPE_PLAYER)
                        SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_POLYMORPH, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
                }
                break;

                // 05/20/2004 - Hypnotoad - Cancellation
            case DEF_MAGICTYPE_CANCELLATION:
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
                if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) && (m_pClientList[sOwnerH]->m_iHP > 0) && (m_pClientList[sOwnerH]->m_iAdminUserLevel == 0))
                {

                    // Removes Invisibility Flag 0x0010
                    SetInvisibilityFlag(sOwnerH, cOwnerType, false);

                    // Removes Illusion Flag 0x01000000
                    SetIllusionFlag(sOwnerH, cOwnerType, false);

                    // Removes Defense Shield Flag 0x02000000
                    // Removes Great Defense Shield Flag 0x02000000
                    SetDefenseShieldFlag(sOwnerH, cOwnerType, false);

                    // Removes Absolute Magic Protection Flag 0x04000000	
                    // Removes Protection From Magic Flag 0x04000000
                    SetMagicProtectionFlag(sOwnerH, cOwnerType, false);

                    // Removes Protection From Arrow Flag 0x08000000
                    SetProtectionFromArrowFlag(sOwnerH, cOwnerType, false);

                    // Removes Illusion Movement Flag 0x00200000
                    SetIllusionMovementFlag(sOwnerH, cOwnerType, false);

                    // Removes Berserk Flag 0x0020
                    SetBerserkFlag(sOwnerH, cOwnerType, false);

                    //Removes ice-added 
                    SetIceFlag(sOwnerH, cOwnerType, false);

                    //Remove paralyse

                    bRemoveFromDelayEventList(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_ICE);
                    bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime),
                        sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

                    bRemoveFromDelayEventList(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_HOLDOBJECT);
                    bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_HOLDOBJECT, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime),
                        sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

                    bRemoveFromDelayEventList(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INHIBITION);
                    bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_INHIBITION, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime),
                        sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

                    bRemoveFromDelayEventList(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
                    bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_INVISIBILITY, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime),
                        sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

                    bRemoveFromDelayEventList(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_BERSERK);
                    bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_BERSERK, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime),
                        sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

                    bRemoveFromDelayEventList(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_PROTECT);
                    bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_PROTECT, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime),
                        sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

                    bRemoveFromDelayEventList(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_CONFUSE);
                    bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_CONFUSE, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime),
                        sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

                    // Update Client
                    SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
                }
                break;

            case DEF_MAGICTYPE_DAMAGE_AREA_NOSPOT_SPDOWN:
                // Á÷°ÝÀº Ã³¸®ÇÏÁö ¾Ê´Â´Ù.
                // ÁÖº¯ °ø°Ý È¿°ú 
                for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
                    for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++)
                    {
                        // ÀÚ½Åµµ ÇÇÆøµÉ ¼ö ÀÖÀ¸´Ï ÁÖÀÇ.
                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        {
                            Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
                            Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                        }

                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                            (m_pClientList[sOwnerH]->m_iHP > 0))
                        {
                            // Á×Àº Ã´ÇÏ°í ÀÖ´Â ÇÃ·¹ÀÌ¾î´Ù.
                            if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            {
                                Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
                                Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                            }
                        }
                    }
                break;

            case DEF_MAGICTYPE_DAMAGE_LINEAR:
                // Ãƒâ‚¬ÃƒÂÃƒÂÃƒÂ·Ã‚Â¼Ã‚Â± Ã‚Â»ÃƒÂ³Ã‚Â¿Ã‚Â¡ Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€š Ã‚Â¸ÃƒÂ±Ãƒâ€¡Ã‚Â¥Ã‚Â¸Ã‚Â¦ Ã‚Â¸ÃƒÂ°Ã‚ÂµÃƒÅ½ Ã‚Â°ÃƒÂ¸Ã‚Â°ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                sX = m_pClientList[iClientH]->m_sX;
                sY = m_pClientList[iClientH]->m_sY;

                for (i = 2; i < 10; i++)
                {
                    iErr = 0;
                    m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);

                    // tx, ty
                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY);
                    if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                        (m_pClientList[sOwnerH]->m_iHP > 0))
                    {
                        // ÃƒÂÃƒâ€”Ãƒâ‚¬Ã‚Âº ÃƒÆ’Ã‚Â´Ãƒâ€¡ÃƒÂÃ‚Â°ÃƒÂ­ Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€š Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ã‚Â´Ãƒâ„¢.
                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                    }

                    // tx-1, ty
                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
                    if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                        (m_pClientList[sOwnerH]->m_iHP > 0))
                    {
                        // ÃƒÂÃƒâ€”Ãƒâ‚¬Ã‚Âº ÃƒÆ’Ã‚Â´Ãƒâ€¡ÃƒÂÃ‚Â°ÃƒÂ­ Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€š Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ã‚Â´Ãƒâ„¢.
                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                    }

                    // tx+1, ty
                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
                    if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                        (m_pClientList[sOwnerH]->m_iHP > 0))
                    {
                        // ÃƒÂÃƒâ€”Ãƒâ‚¬Ã‚Âº ÃƒÆ’Ã‚Â´Ãƒâ€¡ÃƒÂÃ‚Â°ÃƒÂ­ Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€š Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ã‚Â´Ãƒâ„¢.
                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                    }

                    // tx, ty-1
                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
                    if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                        (m_pClientList[sOwnerH]->m_iHP > 0))
                    {
                        // ÃƒÂÃƒâ€”Ãƒâ‚¬Ã‚Âº ÃƒÆ’Ã‚Â´Ãƒâ€¡ÃƒÂÃ‚Â°ÃƒÂ­ Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€š Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ã‚Â´Ãƒâ„¢.
                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                    }

                    // tx, ty+1
                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
                    if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                        (m_pClientList[sOwnerH]->m_iHP > 0))
                    {
                        // ÃƒÂÃƒâ€”Ãƒâ‚¬Ã‚Âº ÃƒÆ’Ã‚Â´Ãƒâ€¡ÃƒÂÃ‚Â°ÃƒÂ­ Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€š Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ã‚Â´Ãƒâ„¢.
                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                    }

                    if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
                }

                // ÃƒÂÃƒâ€“Ã‚ÂºÃ‚Â¯ Ã‚Â°ÃƒÂ¸Ã‚Â°ÃƒÂ ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂº 
                for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
                    for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++)
                    {
                        // Ãƒâ‚¬ÃƒÅ¡Ã‚Â½Ãƒâ€¦Ã‚ÂµÃ‚Âµ Ãƒâ€¡Ãƒâ€¡Ãƒâ€ ÃƒÂ¸Ã‚ÂµÃƒâ€° Ã‚Â¼ÃƒÂ¶ Ãƒâ‚¬Ãƒâ€“Ãƒâ‚¬Ã‚Â¸Ã‚Â´ÃƒÂ ÃƒÂÃƒâ€“Ãƒâ‚¬Ãƒâ€¡.
                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                            (m_pClientList[sOwnerH]->m_iHP > 0))
                        {
                            // ÃƒÂÃƒâ€”Ãƒâ‚¬Ã‚Âº ÃƒÆ’Ã‚Â´Ãƒâ€¡ÃƒÂÃ‚Â°ÃƒÂ­ Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€š Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ã‚Â´Ãƒâ„¢.
                            if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                        }
                    }

                // dX, dY
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
                if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                    Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false

                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
                if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                    (m_pClientList[sOwnerH]->m_iHP > 0))
                {
                    // ÃƒÂÃƒâ€”Ãƒâ‚¬Ã‚Âº ÃƒÆ’Ã‚Â´Ãƒâ€¡ÃƒÂÃ‚Â°ÃƒÂ­ Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€š Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ã‚Â´Ãƒâ„¢.
                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
                }
                break;

                // v2.16 2002-5-23 Ã‚Â°ÃƒÂ­Ã‚Â±Ã‚Â¤Ãƒâ€¡ÃƒÂ¶ 
            case DEF_MAGICTYPE_ICE_LINEAR:
                // Ãƒâ‚¬ÃƒÂÃƒÂÃƒÂ·Ã‚Â¼Ã‚Â± Ã‚Â»ÃƒÂ³Ã‚Â¿Ã‚Â¡ Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€š Ã‚Â¸ÃƒÂ±Ãƒâ€¡Ã‚Â¥Ã‚Â¸Ã‚Â¦ Ã‚Â¸ÃƒÂ°Ã‚ÂµÃƒÅ½ Ã‚Â¾ÃƒÂ³Ã‚Â¸Ã‚Â®Ã‚Â¸ÃƒÂ§ Ã‚Â°ÃƒÂ¸Ã‚Â°ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                sX = m_pClientList[iClientH]->m_sX;
                sY = m_pClientList[iClientH]->m_sY;

                for (i = 2; i < 10; i++)
                {
                    iErr = 0;
                    m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);

                    // tx, ty
                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                    {
                        Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                        // Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚Â¼Ã‚Â­ Ã‚ÂµÃ‚Â¿Ãƒâ‚¬Ãƒâ€ºÃƒâ‚¬ÃƒÅ’ Ã‚Â´ÃƒÅ Ã‚Â¾ÃƒÂ®ÃƒÂÃƒÂ¶Ã‚Â´Ãƒâ€š ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂº
                        switch (cOwnerType)
                        {
                            case DEF_OWNERTYPE_PLAYER:
                                if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                if (m_pClientList[sOwnerH]->m_iHP < 0) goto MAGIC_NOEFFECT;
                                // Ãƒâ€¦Ã‚Â¸Ã‚Â°Ãƒâ„¢Ãƒâ‚¬ÃƒÅ’ Ã‚Â»ÃƒÂ¬Ã‚Â¾Ãƒâ€ Ãƒâ‚¬Ãƒâ€“Ã‚Â°ÃƒÂ­ Ã‚Â¾ÃƒÂ³Ãƒâ‚¬Ã‚Â½ Ãƒâ‚¬ÃƒÂºÃƒâ€¡Ãƒâ€”Ã‚Â¿Ã‚Â¡ Ã‚Â½Ãƒâ€¡Ãƒâ€ ÃƒÂÃƒâ€¡ÃƒÅ¸Ã‚Â´Ãƒâ„¢Ã‚Â¸ÃƒÂ© Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚ÂºÃƒâ„¢Ã‚Â´Ãƒâ€šÃ‚Â´Ãƒâ„¢.
                                if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                {
                                    if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                    {
                                        m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                        SetIceFlag(sOwnerH, cOwnerType, true);
                                        // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                        bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                            sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                        // Ã‚Â´ÃƒÂ«Ã‚Â»ÃƒÂ³Ãƒâ‚¬ÃƒÅ’ Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ãƒâ‚¬ÃƒÅ½ Ã‚Â°ÃƒÂ¦Ã‚Â¿ÃƒÂ¬ Ã‚Â¾Ãƒâ€¹Ã‚Â·ÃƒÂÃƒÂÃƒËœÃ‚Â´Ãƒâ„¢.
                                        SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
                                    }
                                }
                                break;

                            case DEF_OWNERTYPE_NPC:
                                if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                {
                                    if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                    {
                                        m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                        SetIceFlag(sOwnerH, cOwnerType, true);
                                        // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                        bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                            sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                    }
                                }
                                break;
                        }
                    }

                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY);
                    if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                        (m_pClientList[sOwnerH]->m_iHP > 0))
                    {
                        // ÃƒÂÃƒâ€”Ãƒâ‚¬Ã‚Âº ÃƒÆ’Ã‚Â´Ãƒâ€¡ÃƒÂÃ‚Â°ÃƒÂ­ Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€š Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ã‚Â´Ãƒâ„¢.
                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        {
                            Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                            // Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚Â¼Ã‚Â­ Ã‚ÂµÃ‚Â¿Ãƒâ‚¬Ãƒâ€ºÃƒâ‚¬ÃƒÅ’ Ã‚Â´ÃƒÅ Ã‚Â¾ÃƒÂ®ÃƒÂÃƒÂ¶Ã‚Â´Ãƒâ€š ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂº
                            switch (cOwnerType)
                            {
                                case DEF_OWNERTYPE_PLAYER:
                                    if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    // Ãƒâ€¦Ã‚Â¸Ã‚Â°Ãƒâ„¢Ãƒâ‚¬ÃƒÅ’ Ã‚Â»ÃƒÂ¬Ã‚Â¾Ãƒâ€ Ãƒâ‚¬Ãƒâ€“Ã‚Â°ÃƒÂ­ Ã‚Â¾ÃƒÂ³Ãƒâ‚¬Ã‚Â½ Ãƒâ‚¬ÃƒÂºÃƒâ€¡Ãƒâ€”Ã‚Â¿Ã‚Â¡ Ã‚Â½Ãƒâ€¡Ãƒâ€ ÃƒÂÃƒâ€¡ÃƒÅ¸Ã‚Â´Ãƒâ„¢Ã‚Â¸ÃƒÂ© Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚ÂºÃƒâ„¢Ã‚Â´Ãƒâ€šÃ‚Â´Ãƒâ„¢.
                                    if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                    {
                                        if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                        {
                                            m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                            SetIceFlag(sOwnerH, cOwnerType, true);
                                            // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                            bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                                sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                            // Ã‚Â´ÃƒÂ«Ã‚Â»ÃƒÂ³Ãƒâ‚¬ÃƒÅ’ Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ãƒâ‚¬ÃƒÅ½ Ã‚Â°ÃƒÂ¦Ã‚Â¿ÃƒÂ¬ Ã‚Â¾Ãƒâ€¹Ã‚Â·ÃƒÂÃƒÂÃƒËœÃ‚Â´Ãƒâ„¢.
                                            SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
                                        }
                                    }
                                    break;

                                case DEF_OWNERTYPE_NPC:
                                    if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                    {
                                        if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                        {
                                            m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                            SetIceFlag(sOwnerH, cOwnerType, true);
                                            // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                            bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                                sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                        }
                                    }
                                    break;
                            }
                            //
                        }
                    }

                    // tx-1, ty
                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                    {
                        Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                        // Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚Â¼Ã‚Â­ Ã‚ÂµÃ‚Â¿Ãƒâ‚¬Ãƒâ€ºÃƒâ‚¬ÃƒÅ’ Ã‚Â´ÃƒÅ Ã‚Â¾ÃƒÂ®ÃƒÂÃƒÂ¶Ã‚Â´Ãƒâ€š ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂº
                        switch (cOwnerType)
                        {
                            case DEF_OWNERTYPE_PLAYER:
                                if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                // Ãƒâ€¦Ã‚Â¸Ã‚Â°Ãƒâ„¢Ãƒâ‚¬ÃƒÅ’ Ã‚Â»ÃƒÂ¬Ã‚Â¾Ãƒâ€ Ãƒâ‚¬Ãƒâ€“Ã‚Â°ÃƒÂ­ Ã‚Â¾ÃƒÂ³Ãƒâ‚¬Ã‚Â½ Ãƒâ‚¬ÃƒÂºÃƒâ€¡Ãƒâ€”Ã‚Â¿Ã‚Â¡ Ã‚Â½Ãƒâ€¡Ãƒâ€ ÃƒÂÃƒâ€¡ÃƒÅ¸Ã‚Â´Ãƒâ„¢Ã‚Â¸ÃƒÂ© Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚ÂºÃƒâ„¢Ã‚Â´Ãƒâ€šÃ‚Â´Ãƒâ„¢.
                                if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                {
                                    if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                    {
                                        m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                        SetIceFlag(sOwnerH, cOwnerType, true);
                                        // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                        bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                            sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                        // Ã‚Â´ÃƒÂ«Ã‚Â»ÃƒÂ³Ãƒâ‚¬ÃƒÅ’ Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ãƒâ‚¬ÃƒÅ½ Ã‚Â°ÃƒÂ¦Ã‚Â¿ÃƒÂ¬ Ã‚Â¾Ãƒâ€¹Ã‚Â·ÃƒÂÃƒÂÃƒËœÃ‚Â´Ãƒâ„¢.
                                        SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
                                    }
                                }
                                break;

                            case DEF_OWNERTYPE_NPC:
                                if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                {
                                    if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                    {
                                        m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                        SetIceFlag(sOwnerH, cOwnerType, true);
                                        // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                        bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                            sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                    }
                                }
                                break;
                        }
                        //
                    }

                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
                    if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                        (m_pClientList[sOwnerH]->m_iHP > 0))
                    {
                        // ÃƒÂÃƒâ€”Ãƒâ‚¬Ã‚Âº ÃƒÆ’Ã‚Â´Ãƒâ€¡ÃƒÂÃ‚Â°ÃƒÂ­ Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€š Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ã‚Â´Ãƒâ„¢.
                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        {
                            Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                            // Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚Â¼Ã‚Â­ Ã‚ÂµÃ‚Â¿Ãƒâ‚¬Ãƒâ€ºÃƒâ‚¬ÃƒÅ’ Ã‚Â´ÃƒÅ Ã‚Â¾ÃƒÂ®ÃƒÂÃƒÂ¶Ã‚Â´Ãƒâ€š ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂº
                            switch (cOwnerType)
                            {
                                case DEF_OWNERTYPE_PLAYER:
                                    if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    // Ãƒâ€¦Ã‚Â¸Ã‚Â°Ãƒâ„¢Ãƒâ‚¬ÃƒÅ’ Ã‚Â»ÃƒÂ¬Ã‚Â¾Ãƒâ€ Ãƒâ‚¬Ãƒâ€“Ã‚Â°ÃƒÂ­ Ã‚Â¾ÃƒÂ³Ãƒâ‚¬Ã‚Â½ Ãƒâ‚¬ÃƒÂºÃƒâ€¡Ãƒâ€”Ã‚Â¿Ã‚Â¡ Ã‚Â½Ãƒâ€¡Ãƒâ€ ÃƒÂÃƒâ€¡ÃƒÅ¸Ã‚Â´Ãƒâ„¢Ã‚Â¸ÃƒÂ© Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚ÂºÃƒâ„¢Ã‚Â´Ãƒâ€šÃ‚Â´Ãƒâ„¢.
                                    if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                    {
                                        if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                        {
                                            m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                            SetIceFlag(sOwnerH, cOwnerType, true);
                                            // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                            bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                                sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                            // Ã‚Â´ÃƒÂ«Ã‚Â»ÃƒÂ³Ãƒâ‚¬ÃƒÅ’ Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ãƒâ‚¬ÃƒÅ½ Ã‚Â°ÃƒÂ¦Ã‚Â¿ÃƒÂ¬ Ã‚Â¾Ãƒâ€¹Ã‚Â·ÃƒÂÃƒÂÃƒËœÃ‚Â´Ãƒâ„¢.
                                            SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
                                        }
                                    }
                                    break;

                                case DEF_OWNERTYPE_NPC:
                                    if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                    {
                                        if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                        {
                                            m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                            SetIceFlag(sOwnerH, cOwnerType, true);
                                            // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                            bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                                sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                        }
                                    }
                                    break;
                            }
                            //
                        }
                    }

                    // tx+1, ty
                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                    {
                        Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                        // Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚Â¼Ã‚Â­ Ã‚ÂµÃ‚Â¿Ãƒâ‚¬Ãƒâ€ºÃƒâ‚¬ÃƒÅ’ Ã‚Â´ÃƒÅ Ã‚Â¾ÃƒÂ®ÃƒÂÃƒÂ¶Ã‚Â´Ãƒâ€š ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂº
                        switch (cOwnerType)
                        {
                            case DEF_OWNERTYPE_PLAYER:
                                if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                // Ãƒâ€¦Ã‚Â¸Ã‚Â°Ãƒâ„¢Ãƒâ‚¬ÃƒÅ’ Ã‚Â»ÃƒÂ¬Ã‚Â¾Ãƒâ€ Ãƒâ‚¬Ãƒâ€“Ã‚Â°ÃƒÂ­ Ã‚Â¾ÃƒÂ³Ãƒâ‚¬Ã‚Â½ Ãƒâ‚¬ÃƒÂºÃƒâ€¡Ãƒâ€”Ã‚Â¿Ã‚Â¡ Ã‚Â½Ãƒâ€¡Ãƒâ€ ÃƒÂÃƒâ€¡ÃƒÅ¸Ã‚Â´Ãƒâ„¢Ã‚Â¸ÃƒÂ© Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚ÂºÃƒâ„¢Ã‚Â´Ãƒâ€šÃ‚Â´Ãƒâ„¢.
                                if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                {
                                    if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                    {
                                        m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                        SetIceFlag(sOwnerH, cOwnerType, true);
                                        // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                        bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                            sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                        // Ã‚Â´ÃƒÂ«Ã‚Â»ÃƒÂ³Ãƒâ‚¬ÃƒÅ’ Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ãƒâ‚¬ÃƒÅ½ Ã‚Â°ÃƒÂ¦Ã‚Â¿ÃƒÂ¬ Ã‚Â¾Ãƒâ€¹Ã‚Â·ÃƒÂÃƒÂÃƒËœÃ‚Â´Ãƒâ„¢.
                                        SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
                                    }
                                }
                                break;

                            case DEF_OWNERTYPE_NPC:
                                if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                {
                                    if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                    {
                                        m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                        SetIceFlag(sOwnerH, cOwnerType, true);
                                        // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                        bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                            sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                    }
                                }
                                break;
                        }
                        //
                    }

                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
                    if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                        (m_pClientList[sOwnerH]->m_iHP > 0))
                    {
                        // ÃƒÂÃƒâ€”Ãƒâ‚¬Ã‚Âº ÃƒÆ’Ã‚Â´Ãƒâ€¡ÃƒÂÃ‚Â°ÃƒÂ­ Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€š Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ã‚Â´Ãƒâ„¢.
                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        {
                            Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                            // Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚Â¼Ã‚Â­ Ã‚ÂµÃ‚Â¿Ãƒâ‚¬Ãƒâ€ºÃƒâ‚¬ÃƒÅ’ Ã‚Â´ÃƒÅ Ã‚Â¾ÃƒÂ®ÃƒÂÃƒÂ¶Ã‚Â´Ãƒâ€š ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂº
                            switch (cOwnerType)
                            {
                                case DEF_OWNERTYPE_PLAYER:
                                    if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    // Ãƒâ€¦Ã‚Â¸Ã‚Â°Ãƒâ„¢Ãƒâ‚¬ÃƒÅ’ Ã‚Â»ÃƒÂ¬Ã‚Â¾Ãƒâ€ Ãƒâ‚¬Ãƒâ€“Ã‚Â°ÃƒÂ­ Ã‚Â¾ÃƒÂ³Ãƒâ‚¬Ã‚Â½ Ãƒâ‚¬ÃƒÂºÃƒâ€¡Ãƒâ€”Ã‚Â¿Ã‚Â¡ Ã‚Â½Ãƒâ€¡Ãƒâ€ ÃƒÂÃƒâ€¡ÃƒÅ¸Ã‚Â´Ãƒâ„¢Ã‚Â¸ÃƒÂ© Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚ÂºÃƒâ„¢Ã‚Â´Ãƒâ€šÃ‚Â´Ãƒâ„¢.
                                    if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                    {
                                        if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                        {
                                            m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                            SetIceFlag(sOwnerH, cOwnerType, true);
                                            // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                            bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                                sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                            // Ã‚Â´ÃƒÂ«Ã‚Â»ÃƒÂ³Ãƒâ‚¬ÃƒÅ’ Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ãƒâ‚¬ÃƒÅ½ Ã‚Â°ÃƒÂ¦Ã‚Â¿ÃƒÂ¬ Ã‚Â¾Ãƒâ€¹Ã‚Â·ÃƒÂÃƒÂÃƒËœÃ‚Â´Ãƒâ„¢.
                                            SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
                                        }
                                    }
                                    break;

                                case DEF_OWNERTYPE_NPC:
                                    if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                    {
                                        if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                        {
                                            m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                            SetIceFlag(sOwnerH, cOwnerType, true);
                                            // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                            bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                                sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                        }
                                    }
                                    break;
                            }
                            //
                        }
                    }

                    // tx, ty-1
                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                    {
                        Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                        // Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚Â¼Ã‚Â­ Ã‚ÂµÃ‚Â¿Ãƒâ‚¬Ãƒâ€ºÃƒâ‚¬ÃƒÅ’ Ã‚Â´ÃƒÅ Ã‚Â¾ÃƒÂ®ÃƒÂÃƒÂ¶Ã‚Â´Ãƒâ€š ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂº
                        switch (cOwnerType)
                        {
                            case DEF_OWNERTYPE_PLAYER:
                                if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                // Ãƒâ€¦Ã‚Â¸Ã‚Â°Ãƒâ„¢Ãƒâ‚¬ÃƒÅ’ Ã‚Â»ÃƒÂ¬Ã‚Â¾Ãƒâ€ Ãƒâ‚¬Ãƒâ€“Ã‚Â°ÃƒÂ­ Ã‚Â¾ÃƒÂ³Ãƒâ‚¬Ã‚Â½ Ãƒâ‚¬ÃƒÂºÃƒâ€¡Ãƒâ€”Ã‚Â¿Ã‚Â¡ Ã‚Â½Ãƒâ€¡Ãƒâ€ ÃƒÂÃƒâ€¡ÃƒÅ¸Ã‚Â´Ãƒâ„¢Ã‚Â¸ÃƒÂ© Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚ÂºÃƒâ„¢Ã‚Â´Ãƒâ€šÃ‚Â´Ãƒâ„¢.
                                if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                {
                                    if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                    {
                                        m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                        SetIceFlag(sOwnerH, cOwnerType, true);
                                        // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                        bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                            sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                        // Ã‚Â´ÃƒÂ«Ã‚Â»ÃƒÂ³Ãƒâ‚¬ÃƒÅ’ Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ãƒâ‚¬ÃƒÅ½ Ã‚Â°ÃƒÂ¦Ã‚Â¿ÃƒÂ¬ Ã‚Â¾Ãƒâ€¹Ã‚Â·ÃƒÂÃƒÂÃƒËœÃ‚Â´Ãƒâ„¢.
                                        SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
                                    }
                                }
                                break;

                            case DEF_OWNERTYPE_NPC:
                                if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                {
                                    if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                    {
                                        m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                        SetIceFlag(sOwnerH, cOwnerType, true);
                                        // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                        bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                            sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                    }
                                }
                                break;
                        }
                        //
                    }

                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
                    if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                        (m_pClientList[sOwnerH]->m_iHP > 0))
                    {
                        // ÃƒÂÃƒâ€”Ãƒâ‚¬Ã‚Âº ÃƒÆ’Ã‚Â´Ãƒâ€¡ÃƒÂÃ‚Â°ÃƒÂ­ Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€š Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ã‚Â´Ãƒâ„¢.
                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        {
                            Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                            // Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚Â¼Ã‚Â­ Ã‚ÂµÃ‚Â¿Ãƒâ‚¬Ãƒâ€ºÃƒâ‚¬ÃƒÅ’ Ã‚Â´ÃƒÅ Ã‚Â¾ÃƒÂ®ÃƒÂÃƒÂ¶Ã‚Â´Ãƒâ€š ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂº
                            switch (cOwnerType)
                            {
                                case DEF_OWNERTYPE_PLAYER:
                                    if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    // Ãƒâ€¦Ã‚Â¸Ã‚Â°Ãƒâ„¢Ãƒâ‚¬ÃƒÅ’ Ã‚Â»ÃƒÂ¬Ã‚Â¾Ãƒâ€ Ãƒâ‚¬Ãƒâ€“Ã‚Â°ÃƒÂ­ Ã‚Â¾ÃƒÂ³Ãƒâ‚¬Ã‚Â½ Ãƒâ‚¬ÃƒÂºÃƒâ€¡Ãƒâ€”Ã‚Â¿Ã‚Â¡ Ã‚Â½Ãƒâ€¡Ãƒâ€ ÃƒÂÃƒâ€¡ÃƒÅ¸Ã‚Â´Ãƒâ„¢Ã‚Â¸ÃƒÂ© Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚ÂºÃƒâ„¢Ã‚Â´Ãƒâ€šÃ‚Â´Ãƒâ„¢.
                                    if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                    {
                                        if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                        {
                                            m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                            SetIceFlag(sOwnerH, cOwnerType, true);
                                            // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                            bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                                sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                            // Ã‚Â´ÃƒÂ«Ã‚Â»ÃƒÂ³Ãƒâ‚¬ÃƒÅ’ Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ãƒâ‚¬ÃƒÅ½ Ã‚Â°ÃƒÂ¦Ã‚Â¿ÃƒÂ¬ Ã‚Â¾Ãƒâ€¹Ã‚Â·ÃƒÂÃƒÂÃƒËœÃ‚Â´Ãƒâ„¢.
                                            SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
                                        }
                                    }
                                    break;

                                case DEF_OWNERTYPE_NPC:
                                    if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                    {
                                        if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                        {
                                            m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                            SetIceFlag(sOwnerH, cOwnerType, true);
                                            // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                            bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                                sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                        }
                                    }
                                    break;
                            }
                            //
                        }
                    }

                    // tx, ty+1
                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                    {
                        Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                        // Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚Â¼Ã‚Â­ Ã‚ÂµÃ‚Â¿Ãƒâ‚¬Ãƒâ€ºÃƒâ‚¬ÃƒÅ’ Ã‚Â´ÃƒÅ Ã‚Â¾ÃƒÂ®ÃƒÂÃƒÂ¶Ã‚Â´Ãƒâ€š ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂº
                        switch (cOwnerType)
                        {
                            case DEF_OWNERTYPE_PLAYER:
                                if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                // Ãƒâ€¦Ã‚Â¸Ã‚Â°Ãƒâ„¢Ãƒâ‚¬ÃƒÅ’ Ã‚Â»ÃƒÂ¬Ã‚Â¾Ãƒâ€ Ãƒâ‚¬Ãƒâ€“Ã‚Â°ÃƒÂ­ Ã‚Â¾ÃƒÂ³Ãƒâ‚¬Ã‚Â½ Ãƒâ‚¬ÃƒÂºÃƒâ€¡Ãƒâ€”Ã‚Â¿Ã‚Â¡ Ã‚Â½Ãƒâ€¡Ãƒâ€ ÃƒÂÃƒâ€¡ÃƒÅ¸Ã‚Â´Ãƒâ„¢Ã‚Â¸ÃƒÂ© Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚ÂºÃƒâ„¢Ã‚Â´Ãƒâ€šÃ‚Â´Ãƒâ„¢.
                                if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                {
                                    if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                    {
                                        m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                        SetIceFlag(sOwnerH, cOwnerType, true);
                                        // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                        bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                            sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                        // Ã‚Â´ÃƒÂ«Ã‚Â»ÃƒÂ³Ãƒâ‚¬ÃƒÅ’ Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ãƒâ‚¬ÃƒÅ½ Ã‚Â°ÃƒÂ¦Ã‚Â¿ÃƒÂ¬ Ã‚Â¾Ãƒâ€¹Ã‚Â·ÃƒÂÃƒÂÃƒËœÃ‚Â´Ãƒâ„¢.
                                        SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
                                    }
                                }
                                break;

                            case DEF_OWNERTYPE_NPC:
                                if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                {
                                    if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                    {
                                        m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                        SetIceFlag(sOwnerH, cOwnerType, true);
                                        // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                        bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                            sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                    }
                                }
                                break;
                        }
                        //
                    }

                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
                    if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                        (m_pClientList[sOwnerH]->m_iHP > 0))
                    {
                        // ÃƒÂÃƒâ€”Ãƒâ‚¬Ã‚Âº ÃƒÆ’Ã‚Â´Ãƒâ€¡ÃƒÂÃ‚Â°ÃƒÂ­ Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€š Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ã‚Â´Ãƒâ„¢.
                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        {
                            Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                            // Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚Â¼Ã‚Â­ Ã‚ÂµÃ‚Â¿Ãƒâ‚¬Ãƒâ€ºÃƒâ‚¬ÃƒÅ’ Ã‚Â´ÃƒÅ Ã‚Â¾ÃƒÂ®ÃƒÂÃƒÂ¶Ã‚Â´Ãƒâ€š ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂº
                            switch (cOwnerType)
                            {
                                case DEF_OWNERTYPE_PLAYER:
                                    if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    // Ãƒâ€¦Ã‚Â¸Ã‚Â°Ãƒâ„¢Ãƒâ‚¬ÃƒÅ’ Ã‚Â»ÃƒÂ¬Ã‚Â¾Ãƒâ€ Ãƒâ‚¬Ãƒâ€“Ã‚Â°ÃƒÂ­ Ã‚Â¾ÃƒÂ³Ãƒâ‚¬Ã‚Â½ Ãƒâ‚¬ÃƒÂºÃƒâ€¡Ãƒâ€”Ã‚Â¿Ã‚Â¡ Ã‚Â½Ãƒâ€¡Ãƒâ€ ÃƒÂÃƒâ€¡ÃƒÅ¸Ã‚Â´Ãƒâ„¢Ã‚Â¸ÃƒÂ© Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚ÂºÃƒâ„¢Ã‚Â´Ãƒâ€šÃ‚Â´Ãƒâ„¢.
                                    if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                    {
                                        if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                        {
                                            m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                            SetIceFlag(sOwnerH, cOwnerType, true);
                                            // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                            bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                                sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                            // Ã‚Â´ÃƒÂ«Ã‚Â»ÃƒÂ³Ãƒâ‚¬ÃƒÅ’ Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ãƒâ‚¬ÃƒÅ½ Ã‚Â°ÃƒÂ¦Ã‚Â¿ÃƒÂ¬ Ã‚Â¾Ãƒâ€¹Ã‚Â·ÃƒÂÃƒÂÃƒËœÃ‚Â´Ãƒâ„¢.
                                            SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
                                        }
                                    }
                                    break;

                                case DEF_OWNERTYPE_NPC:
                                    if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                    {
                                        if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                        {
                                            m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                            SetIceFlag(sOwnerH, cOwnerType, true);
                                            // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                            bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                                sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                        }
                                    }
                                    break;
                            }
                            //
                        }
                    }

                    if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
                }

                // ÃƒÂÃƒâ€“Ã‚ÂºÃ‚Â¯ Ã‚Â°ÃƒÂ¸Ã‚Â°ÃƒÂ ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂº 
                for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
                    for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++)
                    {
                        // Ãƒâ‚¬ÃƒÅ¡Ã‚Â½Ãƒâ€¦Ã‚ÂµÃ‚Âµ Ãƒâ€¡Ãƒâ€¡Ãƒâ€ ÃƒÂ¸Ã‚ÂµÃƒâ€° Ã‚Â¼ÃƒÂ¶ Ãƒâ‚¬Ãƒâ€“Ãƒâ‚¬Ã‚Â¸Ã‚Â´ÃƒÂ ÃƒÂÃƒâ€“Ãƒâ‚¬Ãƒâ€¡.
                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        {
                            Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                            // Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚Â¼Ã‚Â­ Ã‚ÂµÃ‚Â¿Ãƒâ‚¬Ãƒâ€ºÃƒâ‚¬ÃƒÅ’ Ã‚Â´ÃƒÅ Ã‚Â¾ÃƒÂ®ÃƒÂÃƒÂ¶Ã‚Â´Ãƒâ€š ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂº
                            switch (cOwnerType)
                            {
                                case DEF_OWNERTYPE_PLAYER:
                                    if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    // Ãƒâ€¦Ã‚Â¸Ã‚Â°Ãƒâ„¢Ãƒâ‚¬ÃƒÅ’ Ã‚Â»ÃƒÂ¬Ã‚Â¾Ãƒâ€ Ãƒâ‚¬Ãƒâ€“Ã‚Â°ÃƒÂ­ Ã‚Â¾ÃƒÂ³Ãƒâ‚¬Ã‚Â½ Ãƒâ‚¬ÃƒÂºÃƒâ€¡Ãƒâ€”Ã‚Â¿Ã‚Â¡ Ã‚Â½Ãƒâ€¡Ãƒâ€ ÃƒÂÃƒâ€¡ÃƒÅ¸Ã‚Â´Ãƒâ„¢Ã‚Â¸ÃƒÂ© Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚ÂºÃƒâ„¢Ã‚Â´Ãƒâ€šÃ‚Â´Ãƒâ„¢.
                                    if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                    {
                                        if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                        {
                                            m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                            SetIceFlag(sOwnerH, cOwnerType, true);
                                            // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                            bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                                sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                            // Ã‚Â´ÃƒÂ«Ã‚Â»ÃƒÂ³Ãƒâ‚¬ÃƒÅ’ Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ãƒâ‚¬ÃƒÅ½ Ã‚Â°ÃƒÂ¦Ã‚Â¿ÃƒÂ¬ Ã‚Â¾Ãƒâ€¹Ã‚Â·ÃƒÂÃƒÂÃƒËœÃ‚Â´Ãƒâ„¢.
                                            SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
                                        }
                                    }
                                    break;

                                case DEF_OWNERTYPE_NPC:
                                    if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                    {
                                        if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                        {
                                            m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                            SetIceFlag(sOwnerH, cOwnerType, true);
                                            // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                            bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                                sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                        }
                                    }
                                    break;
                            }
                            //
                        }

                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                            (m_pClientList[sOwnerH]->m_iHP > 0))
                        {
                            // ÃƒÂÃƒâ€”Ãƒâ‚¬Ã‚Âº ÃƒÆ’Ã‚Â´Ãƒâ€¡ÃƒÂÃ‚Â°ÃƒÂ­ Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€š Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ã‚Â´Ãƒâ„¢.
                            if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            {
                                Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                                // Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚Â¼Ã‚Â­ Ã‚ÂµÃ‚Â¿Ãƒâ‚¬Ãƒâ€ºÃƒâ‚¬ÃƒÅ’ Ã‚Â´ÃƒÅ Ã‚Â¾ÃƒÂ®ÃƒÂÃƒÂ¶Ã‚Â´Ãƒâ€š ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂº
                                switch (cOwnerType)
                                {
                                    case DEF_OWNERTYPE_PLAYER:
                                        if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                        // Ãƒâ€¦Ã‚Â¸Ã‚Â°Ãƒâ„¢Ãƒâ‚¬ÃƒÅ’ Ã‚Â»ÃƒÂ¬Ã‚Â¾Ãƒâ€ Ãƒâ‚¬Ãƒâ€“Ã‚Â°ÃƒÂ­ Ã‚Â¾ÃƒÂ³Ãƒâ‚¬Ã‚Â½ Ãƒâ‚¬ÃƒÂºÃƒâ€¡Ãƒâ€”Ã‚Â¿Ã‚Â¡ Ã‚Â½Ãƒâ€¡Ãƒâ€ ÃƒÂÃƒâ€¡ÃƒÅ¸Ã‚Â´Ãƒâ„¢Ã‚Â¸ÃƒÂ© Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚ÂºÃƒâ„¢Ã‚Â´Ãƒâ€šÃ‚Â´Ãƒâ„¢.
                                        if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                        {
                                            if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                            {
                                                m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                                SetIceFlag(sOwnerH, cOwnerType, true);
                                                // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                                bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                                    sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                                // Ã‚Â´ÃƒÂ«Ã‚Â»ÃƒÂ³Ãƒâ‚¬ÃƒÅ’ Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ãƒâ‚¬ÃƒÅ½ Ã‚Â°ÃƒÂ¦Ã‚Â¿ÃƒÂ¬ Ã‚Â¾Ãƒâ€¹Ã‚Â·ÃƒÂÃƒÂÃƒËœÃ‚Â´Ãƒâ„¢.
                                                SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
                                            }
                                        }
                                        break;

                                    case DEF_OWNERTYPE_NPC:
                                        if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                        if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                        {
                                            if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                            {
                                                m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                                SetIceFlag(sOwnerH, cOwnerType, true);
                                                // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                                bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                                    sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                            }
                                        }
                                        break;
                                }
                                //
                            }
                        }
                    }

                // dX, dY
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
                if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                {
                    Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
                    // Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚Â¼Ã‚Â­ Ã‚ÂµÃ‚Â¿Ãƒâ‚¬Ãƒâ€ºÃƒâ‚¬ÃƒÅ’ Ã‚Â´ÃƒÅ Ã‚Â¾ÃƒÂ®ÃƒÂÃƒÂ¶Ã‚Â´Ãƒâ€š ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂº
                    switch (cOwnerType)
                    {
                        case DEF_OWNERTYPE_PLAYER:
                            if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                            // Ãƒâ€¦Ã‚Â¸Ã‚Â°Ãƒâ„¢Ãƒâ‚¬ÃƒÅ’ Ã‚Â»ÃƒÂ¬Ã‚Â¾Ãƒâ€ Ãƒâ‚¬Ãƒâ€“Ã‚Â°ÃƒÂ­ Ã‚Â¾ÃƒÂ³Ãƒâ‚¬Ã‚Â½ Ãƒâ‚¬ÃƒÂºÃƒâ€¡Ãƒâ€”Ã‚Â¿Ã‚Â¡ Ã‚Â½Ãƒâ€¡Ãƒâ€ ÃƒÂÃƒâ€¡ÃƒÅ¸Ã‚Â´Ãƒâ„¢Ã‚Â¸ÃƒÂ© Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚ÂºÃƒâ„¢Ã‚Â´Ãƒâ€šÃ‚Â´Ãƒâ„¢.
                            if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                            {
                                if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                {
                                    m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                    SetIceFlag(sOwnerH, cOwnerType, true);
                                    // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                    bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                        sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                    // Ã‚Â´ÃƒÂ«Ã‚Â»ÃƒÂ³Ãƒâ‚¬ÃƒÅ’ Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ãƒâ‚¬ÃƒÅ½ Ã‚Â°ÃƒÂ¦Ã‚Â¿ÃƒÂ¬ Ã‚Â¾Ãƒâ€¹Ã‚Â·ÃƒÂÃƒÂÃƒËœÃ‚Â´Ãƒâ„¢.
                                    SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
                                }
                            }
                            break;

                        case DEF_OWNERTYPE_NPC:
                            if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                            if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                            {
                                if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                {
                                    m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                    SetIceFlag(sOwnerH, cOwnerType, true);
                                    // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                    bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                        sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                }
                            }
                            break;
                    }
                    //
                }

                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
                if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                    (m_pClientList[sOwnerH]->m_iHP > 0))
                {
                    // ÃƒÂÃƒâ€”Ãƒâ‚¬Ã‚Âº ÃƒÆ’Ã‚Â´Ãƒâ€¡ÃƒÂÃ‚Â°ÃƒÂ­ Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€š Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ã‚Â´Ãƒâ„¢.
                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                    {
                        Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
                        // Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚Â¼Ã‚Â­ Ã‚ÂµÃ‚Â¿Ãƒâ‚¬Ãƒâ€ºÃƒâ‚¬ÃƒÅ’ Ã‚Â´ÃƒÅ Ã‚Â¾ÃƒÂ®ÃƒÂÃƒÂ¶Ã‚Â´Ãƒâ€š ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂº
                        switch (cOwnerType)
                        {
                            case DEF_OWNERTYPE_PLAYER:
                                if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                // Ãƒâ€¦Ã‚Â¸Ã‚Â°Ãƒâ„¢Ãƒâ‚¬ÃƒÅ’ Ã‚Â»ÃƒÂ¬Ã‚Â¾Ãƒâ€ Ãƒâ‚¬Ãƒâ€“Ã‚Â°ÃƒÂ­ Ã‚Â¾ÃƒÂ³Ãƒâ‚¬Ã‚Â½ Ãƒâ‚¬ÃƒÂºÃƒâ€¡Ãƒâ€”Ã‚Â¿Ã‚Â¡ Ã‚Â½Ãƒâ€¡Ãƒâ€ ÃƒÂÃƒâ€¡ÃƒÅ¸Ã‚Â´Ãƒâ„¢Ã‚Â¸ÃƒÂ© Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚ÂºÃƒâ„¢Ã‚Â´Ãƒâ€šÃ‚Â´Ãƒâ„¢.
                                if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                {
                                    if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                    {
                                        m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                        SetIceFlag(sOwnerH, cOwnerType, true);
                                        // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                        bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                            sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                        // Ã‚Â´ÃƒÂ«Ã‚Â»ÃƒÂ³Ãƒâ‚¬ÃƒÅ’ Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ãƒâ‚¬ÃƒÅ½ Ã‚Â°ÃƒÂ¦Ã‚Â¿ÃƒÂ¬ Ã‚Â¾Ãƒâ€¹Ã‚Â·ÃƒÂÃƒÂÃƒËœÃ‚Â´Ãƒâ„¢.
                                        SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
                                    }
                                }
                                break;

                            case DEF_OWNERTYPE_NPC:
                                if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                {
                                    if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                    {
                                        m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                        SetIceFlag(sOwnerH, cOwnerType, true);
                                        // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                        bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                            sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                    }
                                }
                                break;
                        }
                        //
                    }
                }
                break;


            case DEF_MAGICTYPE_INHIBITION:
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
                switch (cOwnerType)
                {
                    case DEF_OWNERTYPE_PLAYER:
                        if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                        if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INHIBITION] != 0) goto MAGIC_NOEFFECT;
                        if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 5) goto MAGIC_NOEFFECT;
                        if (m_pClientList[iClientH]->m_cSide == m_pClientList[sOwnerH]->m_cSide) goto MAGIC_NOEFFECT;
                        if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;
                        //if (m_pClientList[sOwnerH]->m_iAdminUserLevel != 0) goto MAGIC_NOEFFECT;
                        m_pClientList[sOwnerH]->m_bInhibition = true;
                        bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_INHIBITION, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
                            sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
                        break;
                }
                break;


            case DEF_MAGICTYPE_TREMOR: // v1.4 ÃƒÂÃƒâ€“Ã‚ÂºÃ‚Â¯ Ã‚Â°ÃƒÂ¸Ã‚Â°ÃƒÂ ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â¿Ã‚Â¡ Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ® Ãƒâ‚¬ÃƒÅ“Ã‚Â·ÃƒÂ¹ ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ„¢.
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
                if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                    Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);

                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
                if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                    (m_pClientList[sOwnerH]->m_iHP > 0))
                {
                    // ÃƒÂÃƒâ€”Ãƒâ‚¬Ã‚Âº ÃƒÆ’Ã‚Â´Ãƒâ€¡ÃƒÂÃ‚Â°ÃƒÂ­ Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€š Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ã‚Â´Ãƒâ„¢.
                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                }

                // ÃƒÂÃƒâ€“Ã‚ÂºÃ‚Â¯ Ã‚Â°ÃƒÂ¸Ã‚Â°ÃƒÂ ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂº 
                for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
                    for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++)
                    {
                        // Ãƒâ‚¬ÃƒÅ¡Ã‚Â½Ãƒâ€¦Ã‚ÂµÃ‚Âµ Ãƒâ€¡Ãƒâ€¡Ãƒâ€ ÃƒÂ¸Ã‚ÂµÃƒâ€° Ã‚Â¼ÃƒÂ¶ Ãƒâ‚¬Ãƒâ€“Ãƒâ‚¬Ã‚Â¸Ã‚Â´ÃƒÂ ÃƒÂÃƒâ€“Ãƒâ‚¬Ãƒâ€¡.
                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                            (m_pClientList[sOwnerH]->m_iHP > 0))
                        {
                            // ÃƒÂÃƒâ€”Ãƒâ‚¬Ã‚Âº ÃƒÆ’Ã‚Â´Ãƒâ€¡ÃƒÂÃ‚Â°ÃƒÂ­ Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€š Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ã‚Â´Ãƒâ„¢.
                            if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                        }
                    }
                break;

            case DEF_MAGICTYPE_DAMAGE_AREA_NOSPOT:
                // ÃƒÂÃƒÂ·Ã‚Â°ÃƒÂÃƒâ‚¬Ã‚Âº ÃƒÆ’Ã‚Â³Ã‚Â¸Ã‚Â®Ãƒâ€¡ÃƒÂÃƒÂÃƒÂ¶ Ã‚Â¾ÃƒÅ Ã‚Â´Ãƒâ€šÃ‚Â´Ãƒâ„¢.
                // ÃƒÂÃƒâ€“Ã‚ÂºÃ‚Â¯ Ã‚Â°ÃƒÂ¸Ã‚Â°ÃƒÂ ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂº 
                for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
                    for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++)
                    {
                        // Ãƒâ‚¬ÃƒÅ¡Ã‚Â½Ãƒâ€¦Ã‚ÂµÃ‚Âµ Ãƒâ€¡Ãƒâ€¡Ãƒâ€ ÃƒÂ¸Ã‚ÂµÃƒâ€° Ã‚Â¼ÃƒÂ¶ Ãƒâ‚¬Ãƒâ€“Ãƒâ‚¬Ã‚Â¸Ã‚Â´ÃƒÂ ÃƒÂÃƒâ€“Ãƒâ‚¬Ãƒâ€¡.
                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                            (m_pClientList[sOwnerH]->m_iHP > 0))
                        {
                            // ÃƒÂÃƒâ€”Ãƒâ‚¬Ã‚Âº ÃƒÆ’Ã‚Â´Ãƒâ€¡ÃƒÂÃ‚Â°ÃƒÂ­ Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€š Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ã‚Â´Ãƒâ„¢.
                            if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                        }
                    }
                break;

            case DEF_MAGICTYPE_SPUP_AREA:
                // SpÃ‚Â°Ã‚Â¡ Ã‚Â»ÃƒÂ³Ã‚Â½Ãƒâ€šÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
                // Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½ Ãƒâ‚¬ÃƒÂºÃƒâ€¡Ãƒâ€”Ãƒâ‚¬ÃƒÅ’ Ãƒâ€¡ÃƒÅ Ã‚Â¿ÃƒÂ¤Ã‚Â¾ÃƒÂ¸Ã‚Â´Ãƒâ„¢. 
                Effect_SpUp_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6);
                // ÃƒÂÃƒâ€“Ã‚ÂºÃ‚Â¯ Ã‚Â°ÃƒÂ¸Ã‚Â°ÃƒÂ ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂº 
                for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
                    for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++)
                    {
                        // Ãƒâ‚¬ÃƒÅ¡Ã‚Â½Ãƒâ€¦Ã‚ÂµÃ‚Âµ Ãƒâ€¡Ãƒâ€¡Ãƒâ€ ÃƒÂ¸Ã‚ÂµÃƒâ€° Ã‚Â¼ÃƒÂ¶ Ãƒâ‚¬Ãƒâ€“Ãƒâ‚¬Ã‚Â¸Ã‚Â´ÃƒÂ ÃƒÂÃƒâ€“Ãƒâ‚¬Ãƒâ€¡.
                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                        // Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½Ãƒâ‚¬ÃƒÂºÃƒâ€¡Ãƒâ€”Ãƒâ‚¬ÃƒÅ’ Ãƒâ€¡ÃƒÅ Ã‚Â¿ÃƒÂ¤ Ã‚Â¾ÃƒÂ¸Ã‚Â´Ãƒâ„¢.
                        Effect_SpUp_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                    }
                break;

                // v2.16 2002-5-23 Â°Ã­Â±Â¤Ã‡Ã¶ 
            case DEF_MAGICTYPE_DAMAGE_LINEAR_SPDOWN:
                // Ã€ÃÃÃ·Â¼Â± Â»Ã³Â¿Â¡ Ã€Ã–Â´Ã‚ Â¸Ã±Ã‡Â¥Â¸Â¦ Â¸Ã°ÂµÃŽ Â¾Ã³Â¸Â®Â¸Ã§ Â°Ã¸Â°ÃÃ‡Ã‘Â´Ã™.
                sX = m_pClientList[iClientH]->m_sX;
                sY = m_pClientList[iClientH]->m_sY;

                for (i = 2; i < 10; i++)
                {
                    iErr = 0;
                    m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);

                    // tx, ty
                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                    {
                        Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                        // Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
                        switch (cOwnerType)
                        {
                            case DEF_OWNERTYPE_PLAYER:
                                if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                // Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
                                if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                {
                                    Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                    Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                    ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                                }
                                break;

                            case DEF_OWNERTYPE_NPC:
                                if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                {
                                    Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                    Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                    ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                                }
                                break;
                        }
                    }

                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY);
                    if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                        (m_pClientList[sOwnerH]->m_iHP > 0))
                    {
                        // ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        {
                            Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                            // Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
                            switch (cOwnerType)
                            {
                                case DEF_OWNERTYPE_PLAYER:
                                    if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    // Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
                                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                    {
                                        Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                        Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                        ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                                    }
                                    break;

                                case DEF_OWNERTYPE_NPC:
                                    if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                    {
                                        Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                        Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                        ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                                    }
                                    break;
                            }
                            //
                        }
                    }

                    // tx-1, ty
                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                    {
                        Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                        // Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
                        switch (cOwnerType)
                        {
                            case DEF_OWNERTYPE_PLAYER:
                                if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                // Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
                                if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                {
                                    Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                    Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                    ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                                }
                                break;

                            case DEF_OWNERTYPE_NPC:
                                if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                {

                                    Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                    Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                    ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                                }
                                break;
                        }
                        //
                    }

                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
                    if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                        (m_pClientList[sOwnerH]->m_iHP > 0))
                    {
                        // ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        {
                            Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                            // Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
                            switch (cOwnerType)
                            {
                                case DEF_OWNERTYPE_PLAYER:
                                    if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    // Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
                                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                    {
                                        Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                        Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                        ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                                    }
                                    break;

                                case DEF_OWNERTYPE_NPC:
                                    if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                    {
                                        Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                        Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                        ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                                    }
                                    break;
                            }
                            //
                        }
                    }

                    // tx+1, ty
                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                    {
                        Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                        // Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
                        switch (cOwnerType)
                        {
                            case DEF_OWNERTYPE_PLAYER:
                                if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                // Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
                                if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                {
                                    Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                    Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                    ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                                }
                                break;

                            case DEF_OWNERTYPE_NPC:
                                if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                {
                                    Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                    Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                    ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                                }
                                break;
                        }
                        //
                    }

                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
                    if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                        (m_pClientList[sOwnerH]->m_iHP > 0))
                    {
                        // ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        {
                            Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                            // Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
                            switch (cOwnerType)
                            {
                                case DEF_OWNERTYPE_PLAYER:
                                    if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    // Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
                                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                    {
                                        Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                        Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                        ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                                    }
                                    break;

                                case DEF_OWNERTYPE_NPC:
                                    if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                    {
                                        Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                        Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                        ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                                    }
                                    break;
                            }
                            //
                        }
                    }

                    // tx, ty-1
                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                    {
                        Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                        // Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
                        switch (cOwnerType)
                        {
                            case DEF_OWNERTYPE_PLAYER:
                                if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                // Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
                                if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                {
                                    Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                    Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                    ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                                }
                                break;

                            case DEF_OWNERTYPE_NPC:
                                if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                {
                                    Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                    Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                    ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                                }
                                break;
                        }
                        //
                    }

                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
                    if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                        (m_pClientList[sOwnerH]->m_iHP > 0))
                    {
                        // ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        {
                            Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                            // Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
                            switch (cOwnerType)
                            {
                                case DEF_OWNERTYPE_PLAYER:
                                    if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    // Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
                                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                    {
                                        Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                        Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                        ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                                    }
                                    break;

                                case DEF_OWNERTYPE_NPC:
                                    if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                    {
                                        Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                        Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                        ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                                    }
                                    break;
                            }
                            //
                        }
                    }

                    // tx, ty+1
                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                    {
                        Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, sX, sY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                        // Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
                        switch (cOwnerType)
                        {
                            case DEF_OWNERTYPE_PLAYER:
                                if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                // Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
                                if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                {
                                    Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                    Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                    ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                                }
                                break;

                            case DEF_OWNERTYPE_NPC:
                                if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                {
                                    Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                    Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                    ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                                }
                                break;
                        }
                        //
                    }

                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
                    if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                        (m_pClientList[sOwnerH]->m_iHP > 0))
                    {
                        // ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        {
                            Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                            // Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
                            switch (cOwnerType)
                            {
                                case DEF_OWNERTYPE_PLAYER:
                                    if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    // Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
                                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                    {
                                        Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                        Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                        ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                                    }
                                    break;

                                case DEF_OWNERTYPE_NPC:
                                    if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                    {
                                        Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                        Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                        ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                                    }
                                    break;
                            }
                            //
                        }
                    }

                    if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
                }

                // ÃÃ–ÂºÂ¯ Â°Ã¸Â°Ã ÃˆÂ¿Â°Ãº 
                for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
                    for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++)
                    {
                        // Ã€ÃšÂ½Ã…ÂµÂµ Ã‡Ã‡Ã†Ã¸ÂµÃ‰ Â¼Ã¶ Ã€Ã–Ã€Â¸Â´Ã ÃÃ–Ã€Ã‡.
                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        {
                            Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                            // Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
                            switch (cOwnerType)
                            {
                                case DEF_OWNERTYPE_PLAYER:
                                    if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    // Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
                                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                    {
                                        Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                        Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                        ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                                    }
                                    break;

                                case DEF_OWNERTYPE_NPC:
                                    if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                    {
                                        Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                        Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                        ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                                    }
                                    break;
                            }
                            //
                        }

                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                            (m_pClientList[sOwnerH]->m_iHP > 0))
                        {
                            // ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
                            if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            {
                                Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                                // Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
                                switch (cOwnerType)
                                {
                                    case DEF_OWNERTYPE_PLAYER:
                                        if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                        // Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
                                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                        {
                                            Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                            Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                            ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                                        }
                                        break;

                                    case DEF_OWNERTYPE_NPC:
                                        if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                        {
                                            Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                            Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                            ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                                        }
                                        break;
                                }
                                //
                            }
                        }
                    }

                // dX, dY
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
                if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                {
                    Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
                    // Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
                    switch (cOwnerType)
                    {
                        case DEF_OWNERTYPE_PLAYER:
                            if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                            // Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
                            if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            {
                                Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                            }
                            break;

                        case DEF_OWNERTYPE_NPC:
                            if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                            if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            {
                                Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                            }
                            break;
                    }
                    //
                }

                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
                if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                    (m_pClientList[sOwnerH]->m_iHP > 0))
                {
                    // ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
                    if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                    {
                        Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr); // v1.41 false
                        // Â¾Ã³Â¾Ã®Â¼Â­ ÂµÂ¿Ã€Ã›Ã€ÃŒ Â´ÃŠÂ¾Ã®ÃÃ¶Â´Ã‚ ÃˆÂ¿Â°Ãº
                        switch (cOwnerType)
                        {
                            case DEF_OWNERTYPE_PLAYER:
                                if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                // Ã…Â¸Â°Ã™Ã€ÃŒ Â»Ã¬Â¾Ã†Ã€Ã–Â°Ã­ Â¾Ã³Ã€Â½ Ã€ÃºÃ‡Ã—Â¿Â¡ Â½Ã‡Ã†ÃÃ‡ÃŸÂ´Ã™Â¸Ã© Â¾Ã³Â¾Ã®ÂºÃ™Â´Ã‚Â´Ã™.
                                if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                {
                                    Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                    Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                    ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                                }
                                break;

                            case DEF_OWNERTYPE_NPC:
                                if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                {
                                    Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                    Effect_SpDown_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                                    ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);

                                }
                                break;
                        }
                        //
                    }
                }
                break;

            case DEF_MAGICTYPE_TELEPORT:
                // Ãƒâ€¦ÃƒÅ¡Ã‚Â·Ã‚Â¹Ãƒâ€ ÃƒÂ·Ãƒâ€ Ã‚Â® Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½. sValue 4Ã‚Â¿Ã‚Â¡ Ã‚ÂµÃƒÂ»Ã‚Â¶ÃƒÂ³Ã‚Â¼Ã‚Â­ Ãƒâ€¦ÃƒÅ¡Ã‚Â·Ã‚Â¹Ãƒâ€ ÃƒÂ·Ãƒâ€ Ã‚Â® Ã‚Â¸ÃƒÂ±Ãƒâ‚¬ÃƒÂ»ÃƒÂÃƒÂ¶Ã‚Â°Ã‚Â¡ Ã‚Â°ÃƒÂ¡ÃƒÂÃ‚Â¤Ã‚ÂµÃƒË†Ã‚Â´Ãƒâ„¢.
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);

                switch (m_pMagicConfigList[sType]->m_sValue4)
                {
                    case 1:
                        // Ãƒâ‚¬ÃƒÅ¡Ã‚Â½Ãƒâ€¦Ãƒâ‚¬ÃƒÅ’ Ã‚Â¼Ãƒâ€™Ã‚Â¼Ãƒâ€œÃ‚ÂµÃƒË† Ã‚Â¸Ã‚Â¶Ãƒâ‚¬Ã‚Â»Ã‚Â·ÃƒÅ½ Ãƒâ€¦ÃƒÅ¡Ã‚Â·Ã‚Â¹Ãƒâ€ ÃƒÂ·Ãƒâ€ Ã‚Â®. RecallÃƒâ‚¬ÃƒÅ’Ã‚Â´Ãƒâ„¢.
                        if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (sOwnerH == iClientH))
                        {
                            // Ãƒâ‚¬ÃƒÅ¡Ã‚Â½Ãƒâ€¦Ã‚Â¿ÃƒÅ“Ã‚Â¿Ã‚Â¡Ã‚Â´Ãƒâ€š RecallÃƒâ€¡Ãƒâ€™ Ã‚Â¼ÃƒÂ¶ Ã‚Â¾ÃƒÂ¸Ã‚Â´Ãƒâ„¢.
                            RequestTeleportHandler(iClientH, "1   ");
                        }
                        break;
                }
                break;

            case DEF_MAGICTYPE_SUMMON:
                // Ã‚Â¼Ãƒâ€™ÃƒË†Ã‚Â¯Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½ 

                // Ã‚Â»ÃƒÂ§Ãƒâ€¦ÃƒÂµÃƒâ‚¬ÃƒÂ¥ Ã‚Â³Ã‚Â»Ã‚Â¿Ã‚Â¡Ã‚Â¼Ã‚Â­Ã‚Â´Ãƒâ€š Ã‚Â¼Ãƒâ€™ÃƒË†Ã‚Â¯Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½Ãƒâ‚¬ÃƒÅ’ Ã‚ÂºÃƒâ€™Ã‚Â°Ã‚Â¡Ã‚Â´Ãƒâ€°.
                if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFightZone == true) return;

                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
                // ÃƒÂÃƒÂ¶ÃƒÂÃ‚Â¤Ã‚ÂµÃƒË† OwnerÃ‚Â°Ã‚Â¡ MasterÃ‚Â°Ã‚Â¡ Ã‚ÂµÃƒË†Ã‚Â´Ãƒâ„¢. 
                if ((sOwnerH != 0) && (cOwnerType == DEF_OWNERTYPE_PLAYER))
                {
                    // MasterÃ‚Â·ÃƒÅ½ ÃƒÂÃƒÂ¶ÃƒÂÃ‚Â¤Ã‚ÂµÃƒË† Ã‚Â´ÃƒÂ«Ã‚Â»ÃƒÂ³Ãƒâ‚¬Ã‚Â» Ã‚ÂµÃƒÂ»Ã‚Â¶ÃƒÂ³Ã‚Â´Ãƒâ„¢Ã‚Â´ÃƒÂÃ‚Â°ÃƒÂ­ Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€š Ã‚Â°Ã‚Â´ÃƒÆ’Ã‚Â¼ Ã‚Â¼ÃƒÂ¶Ã‚Â¸Ã‚Â¦ Ã‚Â°ÃƒÂ¨Ã‚Â»ÃƒÂªÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢. 
                    iFollowersNum = iGetFollowerNumber(sOwnerH, cOwnerType);

                    // Ã‚Â¼Ãƒâ€™ÃƒË†Ã‚Â¯Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½Ãƒâ‚¬Ã‚Â» CastingÃƒâ€¡Ãƒâ€˜ Ãƒâ‚¬ÃƒÅ¡Ãƒâ‚¬Ãƒâ€¡ Magery/20 Ã‚Â¸Ã‚Â¸Ãƒâ€¦Ã‚Â­Ãƒâ‚¬Ãƒâ€¡ Ã‚Â¸ÃƒÂ³Ã‚Â½Ã‚ÂºÃƒâ€¦ÃƒÂÃ‚Â¸Ã‚Â¦ Ã‚Â¼Ãƒâ€™ÃƒË†Ã‚Â¯Ãƒâ€¡Ãƒâ€™ Ã‚Â¼ÃƒÂ¶ Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ„¢.
                    if (iFollowersNum >= (m_pClientList[iClientH]->m_cSkillMastery[4] / 20)) break;

                    iNamingValue = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->iGetEmptyNamingValue();
                    if (iNamingValue == -1)
                    {
                        // Ã‚Â´ÃƒÂµÃƒâ‚¬ÃƒÅ’Ã‚Â»ÃƒÂ³ Ãƒâ‚¬ÃƒÅ’ Ã‚Â¸ÃƒÅ Ã‚Â¿Ã‚Â¡ NPCÃ‚Â¸Ã‚Â¦ Ã‚Â¸Ã‚Â¸Ã‚ÂµÃƒÂ©Ã‚Â¼ÃƒÂ¶ Ã‚Â¾ÃƒÂ¸Ã‚Â´Ãƒâ„¢. Ãƒâ‚¬ÃƒÅ’Ã‚Â¸Ã‚Â§Ãƒâ‚¬Ã‚Â» Ãƒâ€¡Ãƒâ€™Ã‚Â´ÃƒÂ§Ãƒâ€¡Ãƒâ€™ Ã‚Â¼ÃƒÂ¶ Ã‚Â¾ÃƒÂ¸Ã‚Â±ÃƒÂ¢ Ã‚Â¶Ã‚Â§Ã‚Â¹Ã‚Â®.
                    }
                    else
                    {
                        // NPCÃ‚Â¸Ã‚Â¦ Ã‚Â»ÃƒÂ½Ã‚Â¼Ã‚ÂºÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                        memset(cName, 0, sizeof(cName));
                        copy_string(cName, "XX%d", iNamingValue);
                        cName[0] = '_';
                        cName[1] = m_pClientList[iClientH]->m_cMapIndex + 65;

                        // MageryÃ‚Â¿Ã‚Â¡ Ã‚ÂµÃƒÂ»Ã‚Â¶ÃƒÂ³ Ã‚Â¼Ãƒâ€™ÃƒË†Ã‚Â¯Ã‚ÂµÃƒâ€¡Ã‚Â´Ãƒâ€š Ã‚Â¸ÃƒÂ³Ã‚Â½Ã‚ÂºÃƒâ€¦ÃƒÂÃƒâ‚¬Ãƒâ€¡ Ã‚ÂµÃƒÂ®Ã‚Â±ÃƒÅ¾Ãƒâ‚¬ÃƒÅ’ Ã‚Â´ÃƒÅ¾Ã‚Â¶ÃƒÂ³ÃƒÂÃƒÂ¸Ã‚Â´Ãƒâ„¢.
                        memset(cNpcName, 0, sizeof(cNpcName));

                        switch (iV1)
                        {
                            case 0: // Ãƒâ‚¬ÃƒÂÃ‚Â¹ÃƒÂÃƒâ‚¬ÃƒÂ»Ãƒâ‚¬ÃƒÅ½ Ã‚Â°ÃƒÂ¦Ã‚Â¿ÃƒÂ¬ 
                                iResult = iDice(1, m_pClientList[iClientH]->m_cSkillMastery[4] / 10);

                                // v1.42 ÃƒÆ’Ãƒâ€“Ãƒâ‚¬ÃƒÂº Ã‚Â¸ÃƒÂ· Ã‚Â·Ã‚Â¹Ã‚ÂºÃ‚Â§Ãƒâ‚¬Ã‚Â» Ãƒâ‚¬Ãƒâ€Ã‚Â·Ãƒâ€š 
                                if (iResult < m_pClientList[iClientH]->m_cSkillMastery[4] / 20)
                                    iResult = m_pClientList[iClientH]->m_cSkillMastery[4] / 20;

                                switch (iResult)
                                {
                                    case 1: strcpy(cNpcName, "Slime"); break;
                                    case 2: strcpy(cNpcName, "Giant-Ant"); break;
                                    case 3: strcpy(cNpcName, "Amphis"); break;
                                    case 4: strcpy(cNpcName, "Orc"); break;
                                    case 5: strcpy(cNpcName, "Skeleton"); break;
                                    case 6:	strcpy(cNpcName, "Clay-Golem"); break;
                                    case 7:	strcpy(cNpcName, "Stone-Golem"); break;
                                    case 8: strcpy(cNpcName, "Orc-Mage"); break;
                                    case 9:	strcpy(cNpcName, "Hellbound"); break;
                                    case 10:strcpy(cNpcName, "Cyclops"); break;
                                }
                                break;

                            case 1:	strcpy(cNpcName, "Orc"); break;
                            case 2: strcpy(cNpcName, "Skeleton"); break;
                            case 3: strcpy(cNpcName, "Clay-Golem"); break;
                            case 4: strcpy(cNpcName, "Stone-Golem"); break;
                            case 5: strcpy(cNpcName, "Hellbound"); break;
                            case 6: strcpy(cNpcName, "Cyclops"); break;
                            case 7: strcpy(cNpcName, "Troll"); break;
                            case 8: strcpy(cNpcName, "Orge"); break;
                        }

                        if (bCreateNewNpc(cNpcName, cName, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, 0, 0, DEF_MOVETYPE_RANDOM, &dX, &dY, cNpcWaypoint, 0, 0, m_pClientList[iClientH]->m_cSide, false, true) == false)
                        {
                            // Ã‚Â½Ãƒâ€¡Ãƒâ€ ÃƒÂÃƒâ€¡ÃƒÅ¸Ãƒâ‚¬Ã‚Â¸Ã‚Â¹Ãƒâ€¡Ã‚Â·ÃƒÅ½ Ã‚Â¿Ã‚Â¹Ã‚Â¾ÃƒÂ Ã‚ÂµÃƒË† NameValueÃ‚Â¸Ã‚Â¦ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚Â½ÃƒÆ’Ãƒâ€¦Ã‚Â²Ã‚Â´Ãƒâ„¢.
                            m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetNamingValueEmpty(iNamingValue);
                        }
                        else
                        {
                            memset(cName_Master, 0, sizeof(cName_Master));
                            switch (cOwnerType)
                            {
                                case DEF_OWNERTYPE_PLAYER:
                                    memcpy(cName_Master, m_pClientList[sOwnerH]->m_cCharName, 10);
                                    break;
                                case DEF_OWNERTYPE_NPC:
                                    memcpy(cName_Master, m_pNpcList[sOwnerH]->m_cName, 5);
                                    break;
                            }
                            bSetNpcFollowMode(cName, cName_Master, cOwnerType);
                        }
                    }
                }
                break;

            case DEF_MAGICTYPE_CREATE:
                // Ã‚Â¹Ã‚Â«Ã‚Â¾ÃƒÂ°Ã‚Â°Ã‚Â¡Ã‚Â¸Ã‚Â¦ Ã‚Â»ÃƒÂ½Ã‚Â¼Ã‚ÂºÃ‚Â½ÃƒÆ’Ãƒâ€¦Ã‚Â°Ã‚Â´Ãƒâ€š Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½ 

                // Ãƒâ‚¬Ã‚Â§Ãƒâ€žÃ‚Â¡Ãƒâ€¡Ãƒâ€™ Ã‚Â¼ÃƒÂ¶ Ã‚Â¾ÃƒÂ¸Ã‚Â´Ãƒâ€š Ã‚Â°ÃƒÂ·Ã‚Â¿Ã‚Â¡Ã‚Â´Ãƒâ€š Ã‚Â»ÃƒÂ½Ã‚Â±ÃƒÂ¢ÃƒÂÃƒÂ¶ Ã‚Â¾ÃƒÅ Ã‚Â´Ãƒâ€šÃ‚Â´Ãƒâ„¢. 
                if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bGetIsMoveAllowedTile(dX, dY) == false)
                    goto MAGIC_NOEFFECT;

                pItem = new class CItem;

                switch (m_pMagicConfigList[sType]->m_sValue4)
                {
                    case 1:
                        // Food
                        if (iDice(1, 2) == 1)
                            copy_string(cItemName, "Meat");
                        else copy_string(cItemName, "Baguette");
                        break;
                }

                _bInitItemAttr(pItem, cItemName);

                // v2.15 Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½Ãƒâ‚¬Ã‚Â¸Ã‚Â·ÃƒÅ½ Ã‚Â»ÃƒÂ½Ã‚Â±ÃƒÂ¤ Ã‚Â¾Ãƒâ€ Ãƒâ‚¬ÃƒÅ’Ãƒâ€¦Ãƒâ€ºÃ‚Â¿Ã‚Â¡ Ã‚Â°ÃƒÂ­Ãƒâ‚¬Ã‚Â¯Ã‚Â¹ÃƒÂ¸ÃƒË†Ã‚Â£ Ãƒâ‚¬ÃƒÂºÃƒâ‚¬ÃƒÂ¥ 
                pItem->m_sTouchEffectType = DEF_ITET_ID;
                pItem->m_sTouchEffectValue1 = iDice(1, 100000);
                pItem->m_sTouchEffectValue2 = iDice(1, 100000);
                pItem->m_sTouchEffectValue3 = (short)timeGetTime();

                // Ã‚Â¾Ãƒâ€ Ãƒâ‚¬ÃƒÅ’Ãƒâ€¦Ãƒâ€ºÃƒâ‚¬Ã‚Â» Ã‚Â¼Ã‚Â­Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€š Ãƒâ‚¬Ã‚Â§Ãƒâ€žÃ‚Â¡Ã‚Â¿Ã‚Â¡ Ã‚Â¹ÃƒÂ¶Ã‚Â¸Ã‚Â°Ã‚Â´Ãƒâ„¢. 
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(dX, dY, pItem);

                // v1.41 ÃƒË†ÃƒÂ±Ã‚Â±ÃƒÂ Ã‚Â¾Ãƒâ€ Ãƒâ‚¬ÃƒÅ’Ãƒâ€¦Ãƒâ€ºÃƒâ‚¬Ã‚Â» Ã‚Â¶Ã‚Â³Ã‚Â¾ÃƒÂ®Ã‚Â¶ÃƒÅ¸Ã‚Â¸Ã‚Â° Ã‚Â°ÃƒÂÃƒâ‚¬ÃƒÅ’Ã‚Â¶ÃƒÂ³Ã‚Â¸ÃƒÂ© Ã‚Â·ÃƒÅ½Ã‚Â±Ãƒâ€”Ã‚Â¸Ã‚Â¦ Ã‚Â³Ã‚Â²Ã‚Â±ÃƒÂ¤Ã‚Â´Ãƒâ„¢. 
                _bItemLog(DEF_ITEMLOG_DROP, iClientH, (int)-1, pItem);

                // Ã‚Â´Ãƒâ„¢Ã‚Â¸Ã‚Â¥ Ãƒâ€¦Ã‚Â¬Ã‚Â¶ÃƒÂ³Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ°Ãƒâ€ Ã‚Â®Ã‚Â¿Ã‚Â¡Ã‚Â°Ãƒâ€ Ã‚Â¾Ãƒâ€ Ãƒâ‚¬ÃƒÅ’Ãƒâ€¦Ãƒâ€ºÃƒâ‚¬ÃƒÅ’ Ã‚Â¶Ã‚Â³Ã‚Â¾ÃƒÂ®ÃƒÂÃƒÂ¸ Ã‚Â°ÃƒÂÃƒâ‚¬Ã‚Â» Ã‚Â¾Ãƒâ€¹Ã‚Â¸Ã‚Â°Ã‚Â´Ãƒâ„¢. 
                SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
                    dX, dY, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4 color
                break;

            case DEF_MAGICTYPE_PROTECT:
                // Ã‚ÂºÃ‚Â¸ÃƒË†Ã‚Â£ Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½ 
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);

                // Ã‚ÂºÃ‚Â¸ÃƒË†Ã‚Â£ Ã‚Â»ÃƒÂ³Ãƒâ€¦Ãƒâ€šÃ‚Â¶ÃƒÂ³Ã‚Â´Ãƒâ€š Ã‚Â°ÃƒÂÃƒâ‚¬Ã‚Â» Ã‚Â¼Ã‚Â³ÃƒÂÃ‚Â¤Ãƒâ€¡ÃƒÂÃ‚Â±ÃƒÂ¢ Ãƒâ‚¬ÃƒÂ¼Ã‚Â¿Ã‚Â¡ Ãƒâ‚¬ÃƒÅ’Ã‚Â¹ÃƒÅ’ Ãƒâ€¡ÃƒËœÃ‚Â´ÃƒÂ§ Ã‚ÂºÃ‚Â¸ÃƒË†Ã‚Â£Ã‚Â°Ã‚Â¡ Ã‚Â°Ãƒâ€°Ã‚Â·ÃƒÂÃƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€šÃƒÂÃƒÂ¶ ÃƒË†Ã‚Â®Ãƒâ‚¬ÃƒÅ½Ãƒâ€¡ÃƒÂÃ‚Â°ÃƒÂ­ Ã‚Â°Ãƒâ€°Ã‚Â·ÃƒÂÃƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ„¢Ã‚Â¸ÃƒÂ© Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½Ãƒâ‚¬Ã‚Âº Ã‚Â¹Ã‚Â«Ã‚Â½ÃƒÆ’Ã‚ÂµÃƒË†Ã‚Â´Ãƒâ„¢. 
                switch (cOwnerType)
                {
                    case DEF_OWNERTYPE_PLAYER:
                        if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                        if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] != 0) goto MAGIC_NOEFFECT;
                        // v1.4334 ÃƒÂÃƒÅ¸Ã‚Â¸Ã‚Â³Ãƒâ‚¬Ã‚Âº Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ã‚Â¿Ã‚Â¡Ã‚Â°Ãƒâ€ Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ¦Ãƒâ‚¬Ã‚Â» Ã‚Â¸ÃƒÂ¸Ãƒâ€¡ÃƒÂÃ‚Â°Ãƒâ€ Ã‚Â¼ÃƒÂ¶ÃƒÂÃ‚Â¤
                        if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;

                        m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] = (char)m_pMagicConfigList[sType]->m_sValue4;
                        switch (m_pMagicConfigList[sType]->m_sValue4)
                        {
                            case 1:
                                SetProtectionFromArrowFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
                                break;
                            case 2:
                            case 5:
                                SetMagicProtectionFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
                                break;
                            case 3:
                            case 4:
                                SetDefenseShieldFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
                                break;
                        }
                        break;

                    case DEF_OWNERTYPE_NPC:
                        if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                        if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] != 0) goto MAGIC_NOEFFECT;
                        // ÃƒÂÃ‚Â¤Ãƒâ‚¬ÃƒÂ» NPCÃ‚ÂµÃƒÂ©Ãƒâ‚¬Ã‚Âº Ã‚ÂºÃ‚Â¸ÃƒË†Ã‚Â£ Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½ Ãƒâ‚¬ÃƒÂ»Ã‚Â¿ÃƒÂ« Ã‚Â¾ÃƒË†Ã‚ÂµÃƒÅ .				
                        if (m_pNpcList[sOwnerH]->m_cActionLimit != 0) goto MAGIC_NOEFFECT;
                        m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] = (char)m_pMagicConfigList[sType]->m_sValue4;

                        switch (m_pMagicConfigList[sType]->m_sValue4)
                        {
                            case 1:
                                SetProtectionFromArrowFlag(sOwnerH, DEF_OWNERTYPE_NPC, true);
                                break;
                            case 2:
                            case 5:
                                SetMagicProtectionFlag(sOwnerH, DEF_OWNERTYPE_NPC, true);
                                break;
                            case 3:
                            case 4:
                                SetDefenseShieldFlag(sOwnerH, DEF_OWNERTYPE_NPC, true);
                                break;
                        }
                        break;
                }

                // Ã‚ÂºÃ‚Â¸ÃƒË†Ã‚Â£ ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_PROTECT, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
                    sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

                // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ã‚Â»ÃƒÂ½Ã‚Â°ÃƒÂ¥Ãƒâ‚¬Ã‚Â½Ãƒâ‚¬Ã‚Â» Ã‚Â¾Ãƒâ€¹Ã‚Â·ÃƒÂÃƒÂÃƒËœÃ‚Â´Ãƒâ„¢.
                if (cOwnerType == DEF_OWNERTYPE_PLAYER)
                    SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_PROTECT, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
                break;

            case DEF_MAGICTYPE_SCAN:
                memset(cScanMessage, 0, sizeof(cScanMessage));
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
                if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                {
                    switch (cOwnerType)
                    {
                        case DEF_OWNERTYPE_PLAYER:
                            if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                            copy_string(cScanMessage, " Player: %s HP:%d MP:%d.", m_pClientList[sOwnerH]->m_cCharName, m_pClientList[sOwnerH]->m_iHP, m_pClientList[sOwnerH]->m_iMP);
                            ShowClientMsg(iClientH, cScanMessage);
                            break;

                        case DEF_OWNERTYPE_NPC:
                            if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                            copy_string(cScanMessage, " NPC: %s HP:%d MP:%d", m_pNpcList[sOwnerH]->m_cNpcName, m_pNpcList[sOwnerH]->m_iHP, m_pNpcList[sOwnerH]->m_iMana);
                            ShowClientMsg(iClientH, cScanMessage);
                            break;
                    }
                    SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_MAGIC, m_pClientList[iClientH]->m_cMapIndex,
                        m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, dX, dY, 10, 10);
                }
                break;

            case DEF_MAGICTYPE_HOLDOBJECT:
                // Ã‚Â¿Ãƒâ‚¬Ã‚ÂºÃƒÂªÃƒÂÃ‚Â§Ãƒâ€ Ã‚Â®Ãƒâ‚¬Ãƒâ€¡ Ã‚Â¿ÃƒÂ²ÃƒÂÃƒÂ·Ãƒâ‚¬Ãƒâ€œÃƒâ‚¬Ã‚Â» Ã‚ÂºÃƒâ‚¬Ã‚Â¼ÃƒÂ¢Ãƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢. 
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
                if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                {

                    switch (cOwnerType)
                    {
                        case DEF_OWNERTYPE_PLAYER:
                            if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                            if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0) goto MAGIC_NOEFFECT;
                            if (m_pClientList[sOwnerH]->m_iAddPR >= 500) goto MAGIC_NOEFFECT;
                            // v1.4334 ÃƒÂÃƒÅ¸Ã‚Â¸Ã‚Â³Ãƒâ‚¬Ã‚Âº Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ã‚Â¿Ã‚Â¡Ã‚Â°Ãƒâ€ Ãƒâ€ ÃƒÂÃ‚Â·Ã‚Â²Ãƒâ‚¬Ã‚Â» Ã‚Â¸ÃƒÂ¸Ãƒâ€¡ÃƒÂÃ‚Â°Ãƒâ€ Ã‚Â¼ÃƒÂ¶ÃƒÂÃ‚Â¤
                            if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;
                            // 2002-09-10 #2 Ã‚Â¾ÃƒË†Ãƒâ‚¬ÃƒÂ¼ÃƒÂÃƒÂ¶Ã‚Â´ÃƒÂ«(No-Attack-Area) Ãƒâ€ ÃƒÂÃ‚Â·Ã‚Â² Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½ Ã‚Â¾ÃƒË†Ã‚ÂµÃƒâ€¡Ã‚Â°Ãƒâ€ Ãƒâ€¡Ãƒâ€
                            if (cOwnerType == DEF_OWNERTYPE_PLAYER)
                            {

                                if (m_pMapList[m_pClientList[sOwnerH]->m_cMapIndex]->iGetAttribute(sX, sY, 0x00000006) != 0) goto MAGIC_NOEFFECT;
                                if (m_pMapList[m_pClientList[sOwnerH]->m_cMapIndex]->iGetAttribute(dX, dY, 0x00000006) != 0) goto MAGIC_NOEFFECT;
                            }

                            // 2002-09-10 #3 Ãƒâ‚¬ÃƒÂ¼Ã‚Â¸ÃƒÂ©Ãƒâ‚¬ÃƒÂ¼Ã‚Â½ÃƒÆ’Ã‚Â¿ÃƒÂ Ã‚Â¹ÃƒÅ’Ã‚ÂµÃƒÂ©Ã‚Â·Ã‚Â£Ã‚ÂµÃƒÂ¥Ã‚Â¿Ã‚Â¡Ã‚Â¼Ã‚Â­Ã‚Â¸Ã‚Â¦ ÃƒÂÃ‚Â¦Ã‚Â¿ÃƒÅ“Ãƒâ€¡ÃƒÂÃ‚Â°ÃƒÂ­ Ã‚Â°Ã‚Â°Ãƒâ‚¬Ã‚Âº Ãƒâ€ ÃƒÂ­Ã‚Â¿Ã‚Â¡Ã‚Â°Ãƒâ€Ã‚Â´Ãƒâ€š Ãƒâ€ ÃƒÂÃ‚Â·Ã‚Â² Ã‚Â¾ÃƒË†Ã‚ÂµÃƒÅ 
                            if (strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "middleland") != 0 &&
                                m_bIsCrusadeMode == false &&
                                m_pClientList[iClientH]->m_cSide == m_pClientList[sOwnerH]->m_cSide)
                                goto MAGIC_NOEFFECT;

                            m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = (char)m_pMagicConfigList[sType]->m_sValue4;
                            break;

                        case DEF_OWNERTYPE_NPC:
                            if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                            if (m_pNpcList[sOwnerH]->m_cMagicLevel >= 6) goto MAGIC_NOEFFECT; // v1.4 Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½ Ã‚Â·Ã‚Â¹Ã‚ÂºÃ‚Â§ 6Ãƒâ‚¬ÃƒÅ’Ã‚Â»ÃƒÂ³Ãƒâ‚¬ÃƒÅ½ Ã‚Â¸Ã‚Â®Ãƒâ€žÃ‚Â¡Ã‚Â±ÃƒÅ¾ Ãƒâ‚¬ÃƒÅ’Ã‚Â»ÃƒÂ³Ãƒâ‚¬Ãƒâ€¡ Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½ Ã‚Â¸ÃƒÂ³Ã‚Â½Ã‚ÂºÃƒâ€¦ÃƒÂÃ‚Â¿Ã‚Â¡Ã‚Â°Ãƒâ€Ã‚Â´Ãƒâ€š Ã‚Â¸Ã‚Â¶Ã‚ÂºÃƒÂ± Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½ Ãƒâ€¦ÃƒÂ«Ãƒâ€¡ÃƒÂÃƒÂÃƒÂ¶ Ã‚Â¾ÃƒÅ Ã‚Â´Ãƒâ€šÃ‚Â´Ãƒâ„¢.
                            if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0) goto MAGIC_NOEFFECT;
                            m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = (char)m_pMagicConfigList[sType]->m_sValue4;
                            break;
                    }

                    // Ã‚ÂºÃ‚Â¸ÃƒË†Ã‚Â£ ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                    bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_HOLDOBJECT, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
                        sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

                    // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ã‚Â»ÃƒÂ½Ã‚Â°ÃƒÂ¥Ãƒâ‚¬Ã‚Â½Ãƒâ‚¬Ã‚Â» Ã‚Â¾Ãƒâ€¹Ã‚Â·ÃƒÂÃƒÂÃƒËœÃ‚Â´Ãƒâ„¢.
                    if (cOwnerType == DEF_OWNERTYPE_PLAYER)
                        SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_HOLDOBJECT, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
                }
                break;

            case DEF_MAGICTYPE_INVISIBILITY:
                switch (m_pMagicConfigList[sType]->m_sValue4)
                {
                    case 1:
                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);

                        switch (cOwnerType)
                        {
                            case DEF_OWNERTYPE_PLAYER:
                                if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                if ((sOwnerH != iClientH) && ((memcmp(m_pClientList[iClientH]->m_cLocation, "elvhunter", 9) == 0) || (memcmp(m_pClientList[iClientH]->m_cLocation, "arehunter", 9) == 0)) && ((memcmp(m_pClientList[sOwnerH]->m_cLocation, "elvhunter", 9) != 0) || (memcmp(m_pClientList[sOwnerH]->m_cLocation, "arehunter", 9) != 0))) goto MAGIC_NOEFFECT;
                                if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0) goto MAGIC_NOEFFECT;
                                if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;

                                m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = (char)m_pMagicConfigList[sType]->m_sValue4;
                                SetInvisibilityFlag(sOwnerH, cOwnerType, true);
                                RemoveFromTarget(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
                                break;

                            case DEF_OWNERTYPE_NPC:
                                if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0) goto MAGIC_NOEFFECT;

                                if (m_pNpcList[sOwnerH]->m_cActionLimit == 0)
                                {
                                    // Ãƒâ‚¬ÃƒÅ’Ã‚ÂµÃ‚Â¿Ãƒâ€¡ÃƒÂÃƒÂÃƒÂ¶ Ã‚Â¾ÃƒÅ Ã‚Â´Ãƒâ€š NPCÃ‚Â´Ãƒâ€š Ãƒâ€¦ÃƒÂµÃ‚Â¸ÃƒÂ­ Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½Ãƒâ‚¬Ã‚Â» Ã‚Â°Ãƒâ€° Ã‚Â¼ÃƒÂ¶ Ã‚Â¾ÃƒÂ¸Ã‚Â´Ãƒâ„¢.
                                    m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = (char)m_pMagicConfigList[sType]->m_sValue4;
                                    SetInvisibilityFlag(sOwnerH, cOwnerType, true);
                                    // Ãƒâ‚¬ÃƒÅ’ NPCÃ‚Â¸Ã‚Â¦ ÃƒÆ’ÃƒÅ¸Ãƒâ‚¬ÃƒÂ»Ãƒâ€¡ÃƒÂÃ‚Â°ÃƒÂ­ Ãƒâ‚¬Ãƒâ€“Ã‚Â´ÃƒÂ¸ Ã‚Â¸ÃƒÂ³Ã‚Â½Ã‚ÂºÃƒâ€¦ÃƒÂÃ‚Â¸Ã‚Â¦ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚Â½ÃƒÆ’Ãƒâ€¦Ã‚Â²Ã‚Â´Ãƒâ„¢.
                                    RemoveFromTarget(sOwnerH, DEF_OWNERTYPE_NPC, DEF_MAGICTYPE_INVISIBILITY);
                                }
                                break;
                        }

                        // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                        bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_INVISIBILITY, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
                            sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

                        if (cOwnerType == DEF_OWNERTYPE_PLAYER)
                            SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_INVISIBILITY, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
                        break;

                    case 2:
                        // v1.4334 ÃƒÂÃƒÅ¸Ã‚Â¸Ã‚Â³Ãƒâ‚¬Ã‚Âº Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ã‚Â¿Ã‚Â¡Ã‚Â°Ãƒâ€ ÃƒÂÃƒÅ¸Ã‚ÂµÃ‚Â¶Ãƒâ‚¬Ã‚Â» Ã‚Â¸ÃƒÂ¸Ãƒâ€¡ÃƒÂÃ‚Â°Ãƒâ€ Ã‚Â¼ÃƒÂ¶ÃƒÂÃ‚Â¤
                        if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;
                        if ((memcmp(m_pClientList[iClientH]->m_cLocation, "elvhunter", 9) == 0) || (memcmp(m_pClientList[iClientH]->m_cLocation, "arehunter", 9) == 0)) goto MAGIC_NOEFFECT;

                        // dX, dY Ã‚Â¹ÃƒÂÃ‚Â°ÃƒÂ¦ 8 ÃƒÂÃƒâ€“Ã‚ÂºÃ‚Â¯Ãƒâ‚¬Ãƒâ€¡ Invisibility Ã‚Â»ÃƒÂ³Ãƒâ€¦Ãƒâ€šÃƒâ‚¬ÃƒÅ½ ObjectÃ‚Â°Ã‚Â¡ Ãƒâ‚¬Ãƒâ€“Ãƒâ‚¬Ã‚Â¸Ã‚Â¸ÃƒÂ© Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦ Ã‚Â½ÃƒÆ’Ãƒâ€¦Ã‚Â²Ã‚Â´Ãƒâ„¢.
                        for (ix = dX - 8; ix <= dX + 8; ix++)
                            for (iy = dY - 8; iy <= dY + 8; iy++)
                            {
                                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                                if (sOwnerH != 0)
                                {
                                    switch (cOwnerType)
                                    {
                                        case DEF_OWNERTYPE_PLAYER:
                                            if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                            if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0)
                                            {
                                                m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = 0;
                                                SetInvisibilityFlag(sOwnerH, cOwnerType, false);
                                                bRemoveFromDelayEventList(sOwnerH, cOwnerType, DEF_MAGICTYPE_INVISIBILITY);
                                            }
                                            break;

                                        case DEF_OWNERTYPE_NPC:
                                            if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                            if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0)
                                            {
                                                m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = 0;
                                                SetInvisibilityFlag(sOwnerH, cOwnerType, false);
                                                bRemoveFromDelayEventList(sOwnerH, cOwnerType, DEF_MAGICTYPE_INVISIBILITY);
                                            }
                                            break;
                                    }
                                }
                            }
                        break;
                }
                break;

            case DEF_MAGICTYPE_CREATE_DYNAMIC:
                // Dynamic ObjectÃ‚Â¸Ã‚Â¦ Ã‚Â»ÃƒÂ½Ã‚Â¼Ã‚ÂºÃƒâ€¡ÃƒÂÃ‚Â°ÃƒÂ­ ÃƒË†Ã‚Â¿Ã‚Â·Ãƒâ€šÃƒâ‚¬ÃƒÅ’ ÃƒÂÃƒÂ¶Ã‚Â¼Ãƒâ€œÃ‚ÂµÃƒâ€¡Ã‚Â´Ãƒâ€š Ãƒâ€¦Ã‚Â¸Ãƒâ‚¬Ãƒâ€.

                // v2.1 Ã‚Â¸Ã‚Â¶Ãƒâ‚¬Ã‚Â» Ã‚Â³Ã‚Â»Ã‚Â¿Ã‚Â¡Ã‚Â¼Ã‚Â­Ã‚Â´Ãƒâ€š Ãƒâ€¡ÃƒÅ Ã‚ÂµÃƒÂ¥ Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½ Ã‚Â±ÃƒÂÃƒÂÃƒÂ¶(Ãƒâ‚¬ÃƒÂ¼Ã‚Â¸ÃƒÂ©Ãƒâ‚¬ÃƒÂ¼Ã‚Â½ÃƒÆ’ ÃƒÂÃ‚Â¦Ã‚Â¿ÃƒÅ“)
                if (m_bIsCrusadeMode == false)
                {
                    if (strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "aresden") == 0) return;
                    if (strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "elvine") == 0) return;
                    // v2.14
                    if (strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "arefarm") == 0) return;
                    if (strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "elvfarm") == 0) return;
                }

                switch (m_pMagicConfigList[sType]->m_sValue10)
                {
                    case DEF_DYNAMICOBJECT_PCLOUD_BEGIN: // Ã‚ÂµÃ‚Â¶Ã‚Â±Ã‚Â¸Ã‚Â¸Ã‚Â§

                    case DEF_DYNAMICOBJECT_FIRE:   // Fire Ãƒâ‚¬ÃƒÅ’Ã‚Â´Ãƒâ„¢.
                    case DEF_DYNAMICOBJECT_SPIKE:  // Spike
                        switch (m_pMagicConfigList[sType]->m_sValue11)
                        {
                            case 1:
                                // wall - type
                                cDir = m_Misc.cGetNextMoveDir(m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, dX, dY);
                                switch (cDir)
                                {
                                    case 1:	rx = 1; ry = 0;   break;
                                    case 2: rx = 1; ry = 1;   break;
                                    case 3: rx = 0; ry = 1;   break;
                                    case 4: rx = -1; ry = 1;  break;
                                    case 5: rx = 1; ry = 0;   break;
                                    case 6: rx = -1; ry = -1; break;
                                    case 7: rx = 0; ry = -1;  break;
                                    case 8: rx = 1; ry = -1;  break;
                                }

                                iAddDynamicObjectList(iClientH, DEF_OWNERTYPE_PLAYER_INDIRECT, m_pMagicConfigList[sType]->m_sValue10, m_pClientList[iClientH]->m_cMapIndex,
                                    dX, dY, m_pMagicConfigList[sType]->m_dwLastTime * 1000);

                                bAnalyzeCriminalAction(iClientH, dX, dY);

                                for (i = 1; i <= m_pMagicConfigList[sType]->m_sValue12; i++)
                                {
                                    iAddDynamicObjectList(iClientH, DEF_OWNERTYPE_PLAYER_INDIRECT, m_pMagicConfigList[sType]->m_sValue10, m_pClientList[iClientH]->m_cMapIndex,
                                        dX + i * rx, dY + i * ry, m_pMagicConfigList[sType]->m_dwLastTime * 1000);
                                    bAnalyzeCriminalAction(iClientH, dX + i * rx, dY + i * ry);

                                    iAddDynamicObjectList(iClientH, DEF_OWNERTYPE_PLAYER_INDIRECT, m_pMagicConfigList[sType]->m_sValue10, m_pClientList[iClientH]->m_cMapIndex,
                                        dX - i * rx, dY - i * ry, m_pMagicConfigList[sType]->m_dwLastTime * 1000);
                                    bAnalyzeCriminalAction(iClientH, dX - i * rx, dY - i * ry);
                                }
                                break;

                            case 2:
                                // Field - Type
                                bool bFlag = false;
                                int cx, cy;
                                for (ix = dX - m_pMagicConfigList[sType]->m_sValue12; ix <= dX + m_pMagicConfigList[sType]->m_sValue12; ix++)
                                    for (iy = dY - m_pMagicConfigList[sType]->m_sValue12; iy <= dY + m_pMagicConfigList[sType]->m_sValue12; iy++)
                                    {
                                        iAddDynamicObjectList(iClientH, DEF_OWNERTYPE_PLAYER_INDIRECT, m_pMagicConfigList[sType]->m_sValue10, m_pClientList[iClientH]->m_cMapIndex,
                                            ix, iy, m_pMagicConfigList[sType]->m_dwLastTime * 1000, m_pMagicConfigList[sType]->m_sValue5);

                                        // Ã‚Â¸Ã‚Â¸Ã‚Â¾ÃƒÂ  Ã‚Â¸Ã‚Â¶Ãƒâ‚¬Ã‚Â»Ã‚Â¿Ã‚Â¡Ã‚Â¼Ã‚Â­ Ãƒâ€¡ÃƒÅ Ã‚ÂµÃƒÂ¥Ã‚Â¸Ã‚Â¦ Ã‚Â±ÃƒÂ± Ã‚Â°ÃƒÂ·Ã‚Â¿Ã‚Â¡ Ã‚Â¹Ã‚Â«Ã‚Â°ÃƒÂ­Ãƒâ€¡Ãƒâ€˜ Ãƒâ‚¬ÃƒÅ¡Ã‚Â°Ã‚Â¡ Ãƒâ‚¬Ãƒâ€“Ã‚Â¾ÃƒÂºÃ‚Â´Ãƒâ„¢Ã‚Â¸ÃƒÂ© Ã‚Â°ÃƒÂ¸Ã‚Â°ÃƒÂÃƒâ‚¬ÃƒÅ¡Ã‚Â´Ãƒâ€š Ã‚Â°Ã‚Â¡Ã‚ÂµÃƒÂ¥Ãƒâ‚¬Ãƒâ€¡ Ã‚Â°ÃƒÂ¸Ã‚Â°ÃƒÂÃƒâ‚¬Ã‚Â» Ã‚Â¹ÃƒÅ¾Ã‚Â°Ãƒâ€ Ã‚ÂµÃƒË†Ã‚Â´Ãƒâ„¢. 
                                        if (bAnalyzeCriminalAction(iClientH, ix, iy, true) == true)
                                        {
                                            bFlag = true;
                                            cx = ix;
                                            cy = iy;
                                        }
                                    }
                                // Ãƒâ€¡ÃƒÅ Ã‚ÂµÃƒÂ¥Ãƒâ‚¬ÃƒÅ½ Ã‚Â°ÃƒÂ¦Ã‚Â¿ÃƒÂ¬ Ã‚Â°ÃƒÂ¦Ã‚ÂºÃƒÂ±Ã‚Â¸Ã‚Â¦ 1Ã‚Â¸ÃƒÂ­Ã‚Â¸Ã‚Â¸ Ã‚Â¼Ãƒâ€™ÃƒË†Ã‚Â¯Ãƒâ€¡ÃƒÂÃ‚Â±ÃƒÂ¢ Ãƒâ‚¬Ã‚Â§Ãƒâ€¡Ãƒâ€.
                                if (bFlag == true) bAnalyzeCriminalAction(iClientH, cx, cy);
                                break;
                        }
                        //
                        break;

                    case DEF_DYNAMICOBJECT_ICESTORM:
                        // Ice-Storm Dynamic Object 
                        iAddDynamicObjectList(iClientH, DEF_OWNERTYPE_PLAYER_INDIRECT, m_pMagicConfigList[sType]->m_sValue10, m_pClientList[iClientH]->m_cMapIndex,
                            dX, dY, m_pMagicConfigList[sType]->m_dwLastTime * 1000,
                            m_pClientList[iClientH]->m_cSkillMastery[4]);
                        break;

                    default:
                        break;
                }
                break;

            case DEF_MAGICTYPE_POSSESSION:
                // Ã‚Â¿ÃƒÂ¸Ã‚Â°Ãƒâ€¦Ã‚Â¸Ã‚Â®Ã‚Â¿Ã‚Â¡ Ã‚Â¶Ã‚Â³Ã‚Â¾ÃƒÂ®ÃƒÂÃ‚Â® Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€š Ã‚Â¹Ã‚Â°Ã‚Â°Ãƒâ€¡Ãƒâ‚¬Ã‚Â» ÃƒÂÃƒÂ½Ã‚Â¾ÃƒÂ®Ã‚Â¿Ãƒâ‚¬Ã‚Â´Ãƒâ€š Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½Ãƒâ‚¬ÃƒÅ’Ã‚Â´Ãƒâ„¢. 
                // v2.12 ÃƒÂÃƒÅ¸Ã‚Â¸Ã‚Â³Ãƒâ‚¬Ã‚Âº Ãƒâ€ ÃƒÂ·ÃƒÂÃ‚Â¦Ã‚Â¼Ãƒâ€¡ Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½ Ã‚Â»ÃƒÂ§Ã‚Â¿ÃƒÂ« Ã‚ÂºÃƒâ€™Ã‚Â°Ã‚Â¡ 
                if (m_pClientList[iClientH]->m_cSide == 0) goto MAGIC_NOEFFECT;

                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
                if (sOwnerH != 0) break; // v1.41 Ãƒâ€ ÃƒÂ·ÃƒÂÃ‚Â¦Ã‚Â¼Ãƒâ€¡ Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½Ãƒâ‚¬Ã‚Âº Ã‚Â»ÃƒÂ§Ã‚Â¶ÃƒÂ·Ãƒâ‚¬ÃƒÅ’ Ã‚Â¼Ã‚Â­ Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€š Ãƒâ‚¬Ã‚Â§Ã‚Â¿Ã‚Â¡Ã‚Â´Ãƒâ€š ÃƒË†Ã‚Â¿Ã‚Â·Ãƒâ€šÃƒâ‚¬ÃƒÅ’ Ã‚Â¾ÃƒÂ¸Ã‚Â´Ãƒâ„¢. 

                pItem = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->pGetItem(dX, dY, &sRemainItemSprite, &sRemainItemSpriteFrame, &cRemainItemColor);
                if (pItem != 0)
                {
                    // Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ã‚Â°Ã‚Â¡ Ã‚Â¾Ãƒâ€ Ãƒâ‚¬ÃƒÅ’Ãƒâ€¦Ãƒâ€ºÃƒâ‚¬Ã‚Â» ÃƒË†Ã‚Â¹Ã‚ÂµÃƒÂ¦Ãƒâ€¡ÃƒÂÃ‚Â¿Ã‚Â´Ã‚Â´Ãƒâ„¢. 
                    if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true)
                    {
                        // Ã‚Â¾Ãƒâ€ Ãƒâ‚¬ÃƒÅ’Ãƒâ€¦Ãƒâ€ºÃƒâ‚¬Ã‚Â» ÃƒË†Ã‚Â¹Ã‚ÂµÃƒÂ¦Ãƒâ€¡ÃƒÅ¸Ã‚Â´Ãƒâ„¢.

                        // v1.411 Ã‚Â·ÃƒÅ½Ã‚Â±Ãƒâ€” Ã‚Â³Ã‚Â²Ã‚Â±ÃƒÂ¤Ã‚Â´Ãƒâ„¢.
                        _bItemLog(DEF_ITEMLOG_GET, iClientH, (int)-1, pItem);

                        dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
                        *dwp = MSGID_NOTIFY;
                        wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
                        *wp = DEF_NOTIFY_ITEMOBTAINED;

                        cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);

                        // 1Ã‚Â°Ã‚Â³ ÃƒË†Ã‚Â¹Ã‚ÂµÃƒÂ¦Ãƒâ€¡ÃƒÅ¸Ã‚Â´Ãƒâ„¢. <- Ã‚Â¿Ã‚Â©Ã‚Â±ÃƒÂ¢Ã‚Â¼Ã‚Â­ 1Ã‚Â°Ã‚Â³Ã‚Â¶ÃƒÂµ Ãƒâ€žÃ‚Â«Ã‚Â¿ÃƒÂ®Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚Â¸Ã‚Â»Ãƒâ€¡ÃƒÂÃ‚Â´Ãƒâ€š Ã‚Â°ÃƒÂÃƒâ‚¬ÃƒÅ’ Ã‚Â¾Ãƒâ€ Ã‚Â´ÃƒÂÃ‚Â´Ãƒâ„¢
                        *cp = 1;
                        cp++;

                        memcpy(cp, pItem->m_cName, 20);
                        cp += 20;

                        dwp = (uint32_t *)cp;
                        *dwp = pItem->m_dwCount;
                        cp += 4;

                        *cp = pItem->m_cItemType;
                        cp++;

                        *cp = pItem->m_cEquipPos;
                        cp++;

                        *cp = (char)0; // Ã‚Â¾ÃƒÂ²Ãƒâ‚¬Ã‚Âº Ã‚Â¾Ãƒâ€ Ãƒâ‚¬ÃƒÅ’Ãƒâ€¦Ãƒâ€ºÃƒâ‚¬ÃƒÅ’Ã‚Â¹Ãƒâ€¡Ã‚Â·ÃƒÅ½ Ãƒâ‚¬ÃƒÂ¥Ãƒâ€šÃƒÂ¸Ã‚ÂµÃƒâ€¡ÃƒÂÃƒÂ¶ Ã‚Â¾ÃƒÅ Ã‚Â¾Ãƒâ€™Ã‚Â´Ãƒâ„¢.
                        cp++;

                        sp = (short *)cp;
                        *sp = pItem->m_sLevelLimit;
                        cp += 2;

                        *cp = pItem->m_cGenderLimit;
                        cp++;

                        wp = (uint16_t *)cp;
                        *wp = pItem->m_wCurLifeSpan;
                        cp += 2;

                        wp = (uint16_t *)cp;
                        *wp = pItem->m_wWeight;
                        cp += 2;

                        sp = (short *)cp;
                        *sp = pItem->m_sSprite;
                        cp += 2;

                        sp = (short *)cp;
                        *sp = pItem->m_sSpriteFrame;
                        cp += 2;

                        *cp = pItem->m_cItemColor;
                        cp++;

                        *cp = (char)pItem->m_sItemSpecEffectValue2; // v1.41 
                        cp++;

                        dwp = (uint32_t *)cp;
                        *dwp = pItem->m_dwAttribute;
                        cp += 4;
                        /*
                        *cp = (char)(pItem->m_dwAttribute & 0x00000001); // Custom-ItemÃƒâ‚¬ÃƒÅ½ÃƒÂÃƒÂ¶Ãƒâ‚¬Ãƒâ€¡ Ã‚Â¿Ã‚Â©Ã‚ÂºÃƒÅ½
                        cp++;
                        */

                        if (iEraseReq == 1) delete pItem;

                        // Ã‚Â¾Ãƒâ€ Ãƒâ‚¬ÃƒÅ’Ãƒâ€¦Ãƒâ€ºÃƒâ‚¬Ã‚Â» ÃƒÂÃƒÂÃ‚Â°ÃƒÂ­Ã‚Â³Ã‚Â­ ÃƒË†Ãƒâ€ž Ã‚Â³Ã‚Â²Ãƒâ‚¬Ã‚Âº Ã‚Â¾Ãƒâ€ Ãƒâ‚¬ÃƒÅ’Ãƒâ€¦Ãƒâ€ºÃƒâ‚¬Ã‚Â» Ã‚Â´Ãƒâ„¢Ã‚Â¸Ã‚Â¥ Ãƒâ€¦Ã‚Â¬Ã‚Â¶ÃƒÂ³Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ°Ãƒâ€ Ã‚Â®Ã‚Â¿Ã‚Â¡Ã‚Â°Ãƒâ€ Ã‚Â¾Ãƒâ€¹Ã‚Â¸Ã‚Â°Ã‚Â´Ãƒâ„¢. 
                        SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_SETITEM, m_pClientList[iClientH]->m_cMapIndex,
                            dX, dY, sRemainItemSprite, sRemainItemSpriteFrame, cRemainItemColor); // v1.4

                        // Ã‚Â¾Ãƒâ€ Ãƒâ‚¬ÃƒÅ’Ãƒâ€¦Ãƒâ€º ÃƒÂÃ‚Â¤Ã‚ÂºÃ‚Â¸ Ãƒâ‚¬ÃƒÂ¼Ã‚Â¼Ãƒâ€º 
                        iRet = m_pClientList[iClientH]->send_msg(cData, 53);

                        switch (iRet)
                        {
                            case DEF_XSOCKEVENT_QUENEFULL:
                            case DEF_XSOCKEVENT_SOCKETERROR:
                            case DEF_XSOCKEVENT_CRITICALERROR:
                            case DEF_XSOCKEVENT_SOCKETCLOSED:
                                // Ã‚Â¸ÃƒÅ¾Ã‚Â½ÃƒÆ’ÃƒÂÃƒÂ¶Ã‚Â¸Ã‚Â¦ Ã‚ÂºÃ‚Â¸Ã‚Â³Ã‚Â¾Ã‚Â¶Ã‚Â§ Ã‚Â¿Ã‚Â¡Ã‚Â·Ã‚Â¯Ã‚Â°Ã‚Â¡ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡ÃƒÅ¸Ã‚Â´Ãƒâ„¢Ã‚Â¸ÃƒÂ© ÃƒÂÃ‚Â¦Ã‚Â°Ãƒâ€¦Ãƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                DeleteClient(iClientH, true, true);
                                return;
                        }
                    }
                    else
                    {
                        // Ã‚Â°ÃƒÂ¸Ã‚Â°Ã‚Â£Ãƒâ‚¬ÃƒÅ’ Ã‚ÂºÃƒÅ½ÃƒÂÃ‚Â·Ãƒâ€¡ÃƒÂÃ‚Â°Ãƒâ€¦Ã‚Â³Ã‚Âª Ãƒâ€¡Ãƒâ€˜Ã‚Â°ÃƒÂ¨ÃƒÂÃƒÅ¸Ã‚Â·Ã‚Â®Ãƒâ‚¬Ã‚Â» ÃƒÆ’ÃƒÅ Ã‚Â°ÃƒÂºÃƒâ€¡ÃƒÅ¸Ã‚Â´Ãƒâ„¢. Ã‚Â¾Ãƒâ€ Ãƒâ‚¬ÃƒÅ’Ãƒâ€¦Ãƒâ€ºÃƒâ‚¬Ã‚Â» Ã‚Â¾ÃƒÂ²Ãƒâ‚¬Ã‚Â» Ã‚Â¼ÃƒÂ¶ Ã‚Â¾ÃƒÂ¸Ã‚Â´Ãƒâ„¢.

                        // Ã‚Â°Ã‚Â¡ÃƒÂÃ‚Â®Ã‚Â¿Ãƒâ€Ã‚Â´ÃƒÂ¸ Ã‚Â¾Ãƒâ€ Ãƒâ‚¬ÃƒÅ’Ãƒâ€¦Ãƒâ€ºÃƒâ‚¬Ã‚Â» Ã‚Â¿ÃƒÂ¸Ã‚Â»ÃƒÂ³ÃƒË†Ã‚Â¸Ã‚ÂºÃ‚Â¹Ã‚Â½ÃƒÆ’Ãƒâ€¦Ã‚Â²Ã‚Â´Ãƒâ„¢. 
                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(dX, dY, pItem);

                        dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
                        *dwp = MSGID_NOTIFY;
                        wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
                        *wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;

                        iRet = m_pClientList[iClientH]->send_msg(cData, 6);
                        switch (iRet)
                        {
                            case DEF_XSOCKEVENT_QUENEFULL:
                            case DEF_XSOCKEVENT_SOCKETERROR:
                            case DEF_XSOCKEVENT_CRITICALERROR:
                            case DEF_XSOCKEVENT_SOCKETCLOSED:
                                // Ã‚Â¸ÃƒÅ¾Ã‚Â½ÃƒÆ’ÃƒÂÃƒÂ¶Ã‚Â¸Ã‚Â¦ Ã‚ÂºÃ‚Â¸Ã‚Â³Ã‚Â¾Ã‚Â¶Ã‚Â§ Ã‚Â¿Ã‚Â¡Ã‚Â·Ã‚Â¯Ã‚Â°Ã‚Â¡ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡ÃƒÅ¸Ã‚Â´Ãƒâ„¢Ã‚Â¸ÃƒÂ© ÃƒÂÃ‚Â¦Ã‚Â°Ãƒâ€¦Ãƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                DeleteClient(iClientH, true, true);
                                return;
                        }
                    }
                }
                //
                break;

            case DEF_MAGICTYPE_CONFUSE:
                // if the caster side is the same as the targets side, no effect occurs
                switch (m_pMagicConfigList[sType]->m_sValue4)
                {
                    case 1: // confuse LanguageÃƒÂ¬Ã‚ÂÃ‚Â´ÃƒÂ«Ã¢â‚¬Â¹Ã‚Â¤. 
                    case 2: // Confusion, Mass Confusion 	
                        for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
                            for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++)
                            {
                                // ÃƒÂ¬Ã…Â¾Ã‚ÂÃƒÂ¬Ã¢â‚¬Â¹Ã‚Â ÃƒÂ«Ã‚ÂÃ¢â‚¬Å¾ ÃƒÂ­Ã¢â‚¬ÂÃ‚Â¼ÃƒÂ­Ã‚ÂÃ‚Â­ÃƒÂ«Ã‚ÂÃ‚Â  ÃƒÂ¬Ã‹â€ Ã‹Å“ ÃƒÂ¬Ã…Â¾Ã‹â€ ÃƒÂ¬Ã…â€œÃ‚Â¼ÃƒÂ«Ã¢â‚¬Â¹Ã‹â€  ÃƒÂ¬Ã‚Â£Ã‚Â¼ÃƒÂ¬Ã‚ÂÃ‹Å“.
                                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                                if (cOwnerType == DEF_OWNERTYPE_PLAYER)
                                {
                                    // ÃƒÂ­Ã¢â‚¬Â¢Ã‚Â´ÃƒÂ«Ã¢â‚¬Â¹Ã‚Â¹ ÃƒÂ¬Ã…â€œÃ¢â‚¬Å¾ÃƒÂ¬Ã‚Â¹Ã‹Å“ÃƒÂ¬Ã¢â‚¬â€Ã‚Â ÃƒÂ¬Ã‚ÂºÃ‚ÂÃƒÂ«Ã‚Â¦Ã‚Â­ÃƒÂ­Ã¢â‚¬Å¾Ã‚Â°ÃƒÂªÃ‚Â°Ã¢â€šÂ¬ ÃƒÂ¬Ã…Â¾Ã‹â€ ÃƒÂ«Ã¢â‚¬Â¹Ã‚Â¤.
                                    if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    if ((bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) && (m_pClientList[iClientH]->m_cSide != m_pClientList[sOwnerH]->m_cSide))
                                    {
                                        // ÃƒÂ¬Ã†â€™Ã‚ÂÃƒÂ«Ã…â€™Ã¢â€šÂ¬ÃƒÂ«Ã‚Â°Ã‚Â©ÃƒÂ¬Ã‚ÂÃ‚Â´ ÃƒÂ«Ã‚Â§Ã‹â€ ÃƒÂ«Ã‚Â²Ã¢â‚¬Â¢ ÃƒÂ¬Ã‚Â Ã¢â€šÂ¬ÃƒÂ­Ã¢â‚¬Â¢Ã‚Â­ÃƒÂ¬Ã¢â‚¬â€Ã‚Â ÃƒÂ¬Ã¢â‚¬Â¹Ã‚Â¤ÃƒÂ­Ã…â€™Ã‚Â¨ÃƒÂ­Ã¢â‚¬â€œÃ‹â€ ÃƒÂ«Ã¢â‚¬Â¹Ã‚Â¤.
                                        if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_CONFUSE] != 0) break; //ÃƒÂ¬Ã‚ÂÃ‚Â´ÃƒÂ«Ã‚Â¯Ã‚Â¸ ÃƒÂ«Ã¢â‚¬Â¹Ã‚Â¤ÃƒÂ«Ã‚Â¥Ã‚Â¸ ConfuseÃƒÂ­Ã…Â¡Ã‚Â¨ÃƒÂªÃ‚Â³Ã‚Â¼ÃƒÂªÃ‚Â°Ã¢â€šÂ¬ ÃƒÂ¬Ã…Â¾Ã‹â€ ÃƒÂ«Ã¢â‚¬Â¹Ã‚Â¤ÃƒÂ«Ã‚Â©Ã‚Â´ ÃƒÂ«Ã‚Â¬Ã‚Â´ÃƒÂ¬Ã¢â‚¬Â¹Ã…â€œÃƒÂ«Ã‚ÂÃ…â€œÃƒÂ«Ã¢â‚¬Â¹Ã‚Â¤.
                                        m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_CONFUSE] = (char)m_pMagicConfigList[sType]->m_sValue4;

                                        // ÃƒÂ­Ã…Â¡Ã‚Â¨ÃƒÂªÃ‚Â³Ã‚Â¼ÃƒÂªÃ‚Â°Ã¢â€šÂ¬ ÃƒÂ­Ã¢â‚¬Â¢Ã‚Â´ÃƒÂ¬Ã‚Â Ã…â€œÃƒÂ«Ã‚ÂÃ‚Â  ÃƒÂ«Ã¢â‚¬Â¢Ã…â€™ ÃƒÂ«Ã‚Â°Ã…â€œÃƒÂ¬Ã†â€™Ã‚ÂÃƒÂ­Ã¢â‚¬Â¢Ã‚Â  ÃƒÂ«Ã¢â‚¬ÂÃ…â€œÃƒÂ«Ã‚Â Ã‹â€ ÃƒÂ¬Ã‚ÂÃ‚Â´ ÃƒÂ¬Ã‚ÂÃ‚Â´ÃƒÂ«Ã‚Â²Ã‚Â¤ÃƒÂ­Ã…Â Ã‚Â¸ÃƒÂ«Ã‚Â¥Ã‚Â¼ ÃƒÂ«Ã¢â‚¬Å“Ã‚Â±ÃƒÂ«Ã‚Â¡Ã‚ÂÃƒÂ­Ã¢â‚¬Â¢Ã…â€œÃƒÂ«Ã¢â‚¬Â¹Ã‚Â¤.
                                        bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_CONFUSE, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
                                            sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

                                        // ÃƒÂ«Ã‚Â§Ã‹â€ ÃƒÂ«Ã‚Â²Ã¢â‚¬Â¢ÃƒÂ¬Ã¢â‚¬â€Ã‚Â ÃƒÂªÃ‚Â±Ã‚Â¸ÃƒÂ«Ã‚Â Ã‚Â¸ÃƒÂ¬Ã‚ÂÃ…â€™ÃƒÂ¬Ã‚ÂÃ¢â‚¬Å¾ ÃƒÂ¬Ã¢â‚¬Â¢Ã…â€™ÃƒÂ«Ã‚Â¦Ã‚Â°ÃƒÂ«Ã¢â‚¬Â¹Ã‚Â¤.
                                        SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_CONFUSE, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
                                    }
                                }
                            }
                        break;

                    case 3: // Ilusion, Mass-Ilusion
                        for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
                            for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++)
                            {
                                // ÃƒÂ¬Ã…Â¾Ã‚ÂÃƒÂ¬Ã¢â‚¬Â¹Ã‚Â ÃƒÂ«Ã‚ÂÃ¢â‚¬Å¾ ÃƒÂ­Ã¢â‚¬ÂÃ‚Â¼ÃƒÂ­Ã‚ÂÃ‚Â­ÃƒÂ«Ã‚ÂÃ‚Â  ÃƒÂ¬Ã‹â€ Ã‹Å“ ÃƒÂ¬Ã…Â¾Ã‹â€ ÃƒÂ¬Ã…â€œÃ‚Â¼ÃƒÂ«Ã¢â‚¬Â¹Ã‹â€  ÃƒÂ¬Ã‚Â£Ã‚Â¼ÃƒÂ¬Ã‚ÂÃ‹Å“.
                                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                                if (cOwnerType == DEF_OWNERTYPE_PLAYER)
                                {
                                    // ÃƒÂ­Ã¢â‚¬Â¢Ã‚Â´ÃƒÂ«Ã¢â‚¬Â¹Ã‚Â¹ ÃƒÂ¬Ã…â€œÃ¢â‚¬Å¾ÃƒÂ¬Ã‚Â¹Ã‹Å“ÃƒÂ¬Ã¢â‚¬â€Ã‚Â ÃƒÂ¬Ã‚ÂºÃ‚ÂÃƒÂ«Ã‚Â¦Ã‚Â­ÃƒÂ­Ã¢â‚¬Å¾Ã‚Â°ÃƒÂªÃ‚Â°Ã¢â€šÂ¬ ÃƒÂ¬Ã…Â¾Ã‹â€ ÃƒÂ«Ã¢â‚¬Â¹Ã‚Â¤.
                                    if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    if ((bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) && (m_pClientList[iClientH]->m_cSide != m_pClientList[sOwnerH]->m_cSide))
                                    {
                                        // ÃƒÂ¬Ã†â€™Ã‚ÂÃƒÂ«Ã…â€™Ã¢â€šÂ¬ÃƒÂ«Ã‚Â°Ã‚Â©ÃƒÂ¬Ã‚ÂÃ‚Â´ ÃƒÂ«Ã‚Â§Ã‹â€ ÃƒÂ«Ã‚Â²Ã¢â‚¬Â¢ ÃƒÂ¬Ã‚Â Ã¢â€šÂ¬ÃƒÂ­Ã¢â‚¬Â¢Ã‚Â­ÃƒÂ¬Ã¢â‚¬â€Ã‚Â ÃƒÂ¬Ã¢â‚¬Â¹Ã‚Â¤ÃƒÂ­Ã…â€™Ã‚Â¨ÃƒÂ­Ã¢â‚¬â€œÃ‹â€ ÃƒÂ«Ã¢â‚¬Â¹Ã‚Â¤.
                                        if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_CONFUSE] != 0) break; //ÃƒÂ¬Ã‚ÂÃ‚Â´ÃƒÂ«Ã‚Â¯Ã‚Â¸ ÃƒÂ«Ã¢â‚¬Â¹Ã‚Â¤ÃƒÂ«Ã‚Â¥Ã‚Â¸ ConfuseÃƒÂ­Ã…Â¡Ã‚Â¨ÃƒÂªÃ‚Â³Ã‚Â¼ÃƒÂªÃ‚Â°Ã¢â€šÂ¬ ÃƒÂ¬Ã…Â¾Ã‹â€ ÃƒÂ«Ã¢â‚¬Â¹Ã‚Â¤ÃƒÂ«Ã‚Â©Ã‚Â´ ÃƒÂ«Ã‚Â¬Ã‚Â´ÃƒÂ¬Ã¢â‚¬Â¹Ã…â€œÃƒÂ«Ã‚ÂÃ…â€œÃƒÂ«Ã¢â‚¬Â¹Ã‚Â¤.
                                        m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_CONFUSE] = (char)m_pMagicConfigList[sType]->m_sValue4;

                                        switch (m_pMagicConfigList[sType]->m_sValue4)
                                        {
                                            case 3:
                                                SetIllusionFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
                                                break;
                                        }

                                        // ÃƒÂ­Ã…Â¡Ã‚Â¨ÃƒÂªÃ‚Â³Ã‚Â¼ÃƒÂªÃ‚Â°Ã¢â€šÂ¬ ÃƒÂ­Ã¢â‚¬Â¢Ã‚Â´ÃƒÂ¬Ã‚Â Ã…â€œÃƒÂ«Ã‚ÂÃ‚Â  ÃƒÂ«Ã¢â‚¬Â¢Ã…â€™ ÃƒÂ«Ã‚Â°Ã…â€œÃƒÂ¬Ã†â€™Ã‚ÂÃƒÂ­Ã¢â‚¬Â¢Ã‚Â  ÃƒÂ«Ã¢â‚¬ÂÃ…â€œÃƒÂ«Ã‚Â Ã‹â€ ÃƒÂ¬Ã‚ÂÃ‚Â´ ÃƒÂ¬Ã‚ÂÃ‚Â´ÃƒÂ«Ã‚Â²Ã‚Â¤ÃƒÂ­Ã…Â Ã‚Â¸ÃƒÂ«Ã‚Â¥Ã‚Â¼ ÃƒÂ«Ã¢â‚¬Å“Ã‚Â±ÃƒÂ«Ã‚Â¡Ã‚ÂÃƒÂ­Ã¢â‚¬Â¢Ã…â€œÃƒÂ«Ã¢â‚¬Â¹Ã‚Â¤.
                                        bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_CONFUSE, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
                                            sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

                                        // ÃƒÂ«Ã‚Â§Ã‹â€ ÃƒÂ«Ã‚Â²Ã¢â‚¬Â¢ÃƒÂ¬Ã¢â‚¬â€Ã‚Â ÃƒÂªÃ‚Â±Ã‚Â¸ÃƒÂ«Ã‚Â Ã‚Â¸ÃƒÂ¬Ã‚ÂÃ…â€™ÃƒÂ¬Ã‚ÂÃ¢â‚¬Å¾ ÃƒÂ¬Ã¢â‚¬Â¢Ã…â€™ÃƒÂ«Ã‚Â¦Ã‚Â°ÃƒÂ«Ã¢â‚¬Â¹Ã‚Â¤.
                                        SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_CONFUSE, m_pMagicConfigList[sType]->m_sValue4, iClientH, 0);
                                    }
                                }
                            }
                        break;

                    case 4: // Ilusion Movement
                        if (m_pClientList[iClientH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0) break;
                        for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
                            for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++)
                            {
                                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                                if (cOwnerType == DEF_OWNERTYPE_PLAYER)
                                {
                                    if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    if ((bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false) && (m_pClientList[iClientH]->m_cSide != m_pClientList[sOwnerH]->m_cSide))
                                    {
                                        if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_CONFUSE] != 0) break;
                                        m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_CONFUSE] = (char)m_pMagicConfigList[sType]->m_sValue4;
                                        switch (m_pMagicConfigList[sType]->m_sValue4)
                                        {
                                            case 4:
                                                SetIllusionMovementFlag(sOwnerH, DEF_OWNERTYPE_PLAYER, true);
                                                break;
                                        }

                                        bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_CONFUSE, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
                                            sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

                                        SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_CONFUSE, m_pMagicConfigList[sType]->m_sValue4, iClientH, 0);
                                    }
                                }
                            }
                }
                break;


            case DEF_MAGICTYPE_POISON:
                // ÃƒÂÃƒÅ¸Ã‚ÂµÃ‚Â¶ Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½. Ã‚Â¸Ãƒâ€¢Ãƒâ‚¬ÃƒÂº Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½ Ãƒâ‚¬ÃƒÂºÃƒâ€¡Ãƒâ€”Ãƒâ‚¬Ã‚Â» Ã‚Â±Ã‚Â¼Ã‚Â¸Ã‚Â®Ã‚Â°ÃƒÂ­ Ã‚Â´Ãƒâ„¢Ãƒâ‚¬Ã‚Â½Ãƒâ‚¬Ã‚Â¸Ã‚Â·ÃƒÅ½ Ã‚ÂµÃ‚Â¶Ã‚Â¼Ã‚ÂºÃƒâ‚¬ÃƒÂºÃƒâ€¡Ãƒâ€”Ãƒâ‚¬Ã‚Â» Ãƒâ€¡Ãƒâ€˜Ã‚Â¹ÃƒÂ¸ Ã‚Â´ÃƒÂµ Ã‚Â±Ã‚Â¼Ã‚Â¸Ã‚Â°Ã‚Â´Ãƒâ„¢. 
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);

                if (m_pMagicConfigList[sType]->m_sValue4 == 1)
                {
                    // ÃƒÂÃƒÅ¸Ã‚ÂµÃ‚Â¶Ãƒâ‚¬Ã‚Â» Ã‚Â°Ãƒâ€¦Ã‚Â´Ãƒâ€š Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½ 
                    switch (cOwnerType)
                    {
                        case DEF_OWNERTYPE_PLAYER:
                            if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                            // v1.4334 ÃƒÂÃƒÅ¸Ã‚Â¸Ã‚Â³Ãƒâ‚¬Ã‚Âº Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ã‚Â¿Ã‚Â¡Ã‚Â°Ãƒâ€ ÃƒÂÃƒÅ¸Ã‚ÂµÃ‚Â¶Ãƒâ‚¬Ã‚Â» Ã‚Â¸ÃƒÂ¸Ãƒâ€¡ÃƒÂÃ‚Â°Ãƒâ€ Ã‚Â¼ÃƒÂ¶ÃƒÂÃ‚Â¤
                            if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) goto MAGIC_NOEFFECT;

                            // Ã‚Â¹ÃƒÂ¼ÃƒÂÃƒâ€¹Ãƒâ€¡ÃƒÂ Ãƒâ‚¬Ã‚Â§Ã‚Â¶ÃƒÂ³Ã‚Â¸ÃƒÂ© 
                            bAnalyzeCriminalAction(iClientH, dX, dY);

                            if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            {
                                // Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½ Ãƒâ‚¬ÃƒÂºÃƒâ€¡Ãƒâ€”Ã‚Â¿Ã‚Â¡ Ã‚Â½Ãƒâ€¡Ãƒâ€ ÃƒÂÃƒâ€¡ÃƒÅ¸Ã‚Â´Ãƒâ„¢. Ã‚ÂµÃ‚Â¶Ã‚Â¼Ã‚ÂºÃƒâ‚¬ÃƒÂºÃƒâ€¡Ãƒâ€”Ãƒâ‚¬Ã‚Â» Ã‚Â°ÃƒÂ¨Ã‚Â»ÃƒÂªÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                if (bCheckResistingPoisonSuccess(sOwnerH, cOwnerType) == false)
                                {
                                    // ÃƒÂÃƒÅ¸Ã‚ÂµÃ‚Â¶Ã‚ÂµÃƒâ€¡Ã‚Â¾ÃƒÂºÃ‚Â´Ãƒâ„¢.
                                    m_pClientList[sOwnerH]->m_bIsPoisoned = true;
                                    m_pClientList[sOwnerH]->m_iPoisonLevel = m_pMagicConfigList[sType]->m_sValue5;
                                    m_pClientList[sOwnerH]->m_dwPoisonTime = dwTime;
                                    // 05/06/2004 - Hypnotoad - poison aura appears when cast Poison
                                    SetPoisonFlag(sOwnerH, cOwnerType, true);
                                    // ÃƒÂÃƒÅ¸Ã‚ÂµÃ‚Â¶Ã‚ÂµÃƒâ€¡Ã‚Â¾ÃƒÂºÃƒâ‚¬Ã‚Â½Ãƒâ‚¬Ã‚Â» Ã‚Â¾Ãƒâ€¹Ã‚Â¸Ã‚Â°Ã‚Â´Ãƒâ„¢. 
                                    SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_POISON, m_pMagicConfigList[sType]->m_sValue5, 0, 0);
                                }
                            }
                            break;

                        case DEF_OWNERTYPE_NPC:
                            if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                            if (m_pNpcList[sOwnerH]->m_iHP > 0) goto MAGIC_NOEFFECT;
                            if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            {
                                // Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½ Ãƒâ‚¬ÃƒÂºÃƒâ€¡Ãƒâ€”Ã‚Â¿Ã‚Â¡ Ã‚Â½Ãƒâ€¡Ãƒâ€ ÃƒÂÃƒâ€¡ÃƒÅ¸Ã‚Â´Ãƒâ„¢. Ã‚ÂµÃ‚Â¶Ã‚Â¼Ã‚ÂºÃƒâ‚¬ÃƒÂºÃƒâ€¡Ãƒâ€”Ãƒâ‚¬Ã‚Â» Ã‚Â°ÃƒÂ¨Ã‚Â»ÃƒÂªÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                if (bCheckResistingPoisonSuccess(sOwnerH, cOwnerType) == false)
                                {
                                    // ÃƒÂÃƒÅ¸Ã‚ÂµÃ‚Â¶Ã‚ÂµÃƒâ€¡Ã‚Â¾ÃƒÂºÃ‚Â´Ãƒâ„¢.

                                }
                            }
                            break;
                    }
                }
                else if (m_pMagicConfigList[sType]->m_sValue4 == 0)
                {
                    // ÃƒÂÃƒÅ¸Ã‚ÂµÃ‚Â¶Ãƒâ‚¬Ã‚Â» Ãƒâ€¡Ã‚ÂªÃ‚Â´Ãƒâ€š Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½ 
                    switch (cOwnerType)
                    {
                        case DEF_OWNERTYPE_PLAYER:
                            if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;

                            if (m_pClientList[sOwnerH]->m_bIsPoisoned == true)
                            {
                                // ÃƒÂÃƒÅ¸Ã‚ÂµÃ‚Â¶Ã‚ÂµÃƒË† Ã‚Â»ÃƒÂ³Ãƒâ€¦Ãƒâ€šÃ‚Â¿Ã‚Â´Ã‚Â´Ãƒâ„¢Ã‚Â¸ÃƒÂ© ÃƒÂÃƒÅ¸Ã‚ÂµÃ‚Â¶Ãƒâ‚¬Ã‚Â» Ãƒâ€¡Ã‚Â¬Ã‚Â´Ãƒâ„¢.
                                m_pClientList[sOwnerH]->m_bIsPoisoned = false;
                                // 05/06/2004 - Hypnotoad - poison aura removed when cure cast
                                SetPoisonFlag(sOwnerH, cOwnerType, false);
                                // ÃƒÂÃƒÅ¸Ã‚ÂµÃ‚Â¶Ãƒâ‚¬ÃƒÅ’ Ãƒâ€¡Ã‚Â®Ã‚Â·ÃƒË†Ãƒâ‚¬Ã‚Â½Ãƒâ‚¬Ã‚Â» Ã‚Â¾Ãƒâ€¹Ã‚Â¸Ã‚Â°Ã‚Â´Ãƒâ„¢. 
                                SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_POISON, 0, 0, 0);
                            }
                            break;

                        case DEF_OWNERTYPE_NPC:
                            if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                            break;
                    }
                }
                break;

            case DEF_MAGICTYPE_BERSERK:
                switch (m_pMagicConfigList[sType]->m_sValue4)
                {
                    case 1:
                        // Ã‚Â¹ÃƒÂ¶Ã‚Â¼Ã‚Â­Ãƒâ€žÃ‚Â¿ Ã‚Â¸ÃƒÂ°Ã‚ÂµÃƒÂ¥Ã‚Â·ÃƒÅ½ Ãƒâ‚¬ÃƒÂ¼ÃƒË†Ã‚Â¯Ã‚ÂµÃƒË†Ã‚Â´Ãƒâ„¢.
                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);

                        switch (cOwnerType)
                        {
                            case DEF_OWNERTYPE_PLAYER:
                                if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] != 0) goto MAGIC_NOEFFECT;
                                m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] = (char)m_pMagicConfigList[sType]->m_sValue4;
                                SetBerserkFlag(sOwnerH, cOwnerType, true);
                                break;

                            case DEF_OWNERTYPE_NPC:
                                if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] != 0) goto MAGIC_NOEFFECT;
                                // ÃƒÂÃ‚Â¤Ãƒâ‚¬ÃƒÂ» NPCÃ‚ÂµÃƒÂ©Ãƒâ‚¬Ã‚Âº Ã‚Â±Ã‚Â¤Ã‚ÂºÃƒÂ Ã‚Â¾ÃƒË†Ã‚ÂµÃƒÅ 				
                                if (m_pNpcList[sOwnerH]->m_cActionLimit != 0) goto MAGIC_NOEFFECT;
                                // 2002-09-11 #3 Ãƒâ‚¬ÃƒÂ» Ã‚Â¸ÃƒÂ³Ã‚Â½Ã‚ÂºÃƒâ€¦ÃƒÂÃ‚Â´Ãƒâ€š Ã‚Â±Ã‚Â¤Ã‚ÂºÃƒÂÃ‚ÂµÃƒâ€¡ÃƒÂÃƒÂ¶ Ã‚Â¾ÃƒÅ Ãƒâ‚¬Ã‚Â½
                                if (m_pClientList[iClientH]->m_cSide != m_pNpcList[sOwnerH]->m_cSide) goto MAGIC_NOEFFECT;

                                m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] = (char)m_pMagicConfigList[sType]->m_sValue4;
                                SetBerserkFlag(sOwnerH, cOwnerType, true);
                                break;
                        }

                        // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                        bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_BERSERK, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
                            sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

                        if (cOwnerType == DEF_OWNERTYPE_PLAYER)
                            SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_BERSERK, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
                        break;
                }
                break;

                // v2.16 2002-5-23 Ã‚Â°ÃƒÂ­Ã‚Â±Ã‚Â¤Ãƒâ€¡ÃƒÂ¶ Ã‚Â¼ÃƒÂ¶ÃƒÂÃ‚Â¤
            case DEF_MAGICTYPE_DAMAGE_AREA_ARMOR_BREAK:
                // ÃƒÂÃƒâ€“Ã‚ÂºÃ‚Â¯ Ã‚Â°ÃƒÂ¸Ã‚Â°ÃƒÂ ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂº 
                for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
                    for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++)
                    {
                        // Ãƒâ‚¬ÃƒÅ¡Ã‚Â½Ãƒâ€¦Ã‚ÂµÃ‚Âµ Ãƒâ€¡Ãƒâ€¡Ãƒâ€ ÃƒÂ¸Ã‚ÂµÃƒâ€° Ã‚Â¼ÃƒÂ¶ Ãƒâ‚¬Ãƒâ€“Ãƒâ‚¬Ã‚Â¸Ã‚Â´ÃƒÂ ÃƒÂÃƒâ€“Ãƒâ‚¬Ãƒâ€¡.
                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        {
                            Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                            // Ã‚Â¹ÃƒÂ¦Ã‚Â¾ÃƒÂ®Ã‚Â±Ã‚Â¸Ãƒâ‚¬Ãƒâ€¡ Ã‚Â¼ÃƒÂ¶Ã‚Â¸ÃƒÂ­Ãƒâ‚¬Ã‚Â» ÃƒÂÃƒâ„¢Ãƒâ‚¬ÃƒÅ½Ã‚Â´Ãƒâ„¢.
                            ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                        }

                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                            (m_pClientList[sOwnerH]->m_iHP > 0))
                        {
                            // ÃƒÂÃƒâ€”Ãƒâ‚¬Ã‚Âº ÃƒÆ’Ã‚Â´Ãƒâ€¡ÃƒÂÃ‚Â°ÃƒÂ­ Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€š Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ã‚Â´Ãƒâ„¢.
                            if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            {
                                Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                                // Ã‚Â¹ÃƒÂ¦Ã‚Â¾ÃƒÂ®Ã‚Â±Ã‚Â¸Ãƒâ‚¬Ãƒâ€¡ Ã‚Â¼ÃƒÂ¶Ã‚Â¸ÃƒÂ­Ãƒâ‚¬Ã‚Â» ÃƒÂÃƒâ„¢Ãƒâ‚¬ÃƒÅ½Ã‚Â´Ãƒâ„¢.
                                ArmorLifeDecrement(iClientH, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue10);
                            }
                        }
                    }
                break;

                // Resurrection Magic. 
            case DEF_MAGICTYPE_RESURRECTION:
                // 10 Mins once
                if (m_pClientList[iClientH]->m_iSpecialAbilityTime != 0) goto MAGIC_NOEFFECT;
                m_pClientList[iClientH]->m_iSpecialAbilityTime = DEF_SPECABLTYTIMESEC / 2;
                // Get the ID of the dead Player/NPC on coords dX, dY. 
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
                switch (cOwnerType)
                {
                    // For Player. 
                    case DEF_OWNERTYPE_PLAYER:
                        // The Player has to exist. 
                        if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                        // Resurrection is not for alive Players. 
                        if (m_pClientList[sOwnerH]->m_bIsKilled == false) goto MAGIC_NOEFFECT;
                        // Set Deadflag to Alive. 
                        m_pClientList[sOwnerH]->m_bIsKilled = false;
                        // Player's HP becomes half of the Max HP. 
                        m_pClientList[sOwnerH]->m_iHP = ((m_pClientList[sOwnerH]->m_iLevel * 2) + (m_pClientList[sOwnerH]->m_iVit * 3) + (m_pClientList[sOwnerH]->m_iStr / 2)) / 2;
                        // Send new HP to Player. 
                        SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_HP, 0, 0, 0, 0);
                        // Make Player stand up. (Currently, by a fake damage). 
                        m_pMapList[m_pClientList[sOwnerH]->m_cMapIndex]->ClearDeadOwner(dX, dY);
                        m_pMapList[m_pClientList[sOwnerH]->m_cMapIndex]->SetOwner(sOwnerH, DEF_OWNERTYPE_PLAYER, dX, dY);
                        SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, 0, 0, 0);
                        SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_HP, 0, 0, 0, 0);
                        break;
                        // Resurrection is not for NPC's. 
                    case DEF_OWNERTYPE_NPC:
                        goto MAGIC_NOEFFECT;
                        break;
                }
                break;

            case DEF_MAGICTYPE_ICE:
                for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
                    for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++)
                    {

                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        {
                            // Ã‚Â´ÃƒÂ«Ã‚Â¹ÃƒÅ’ÃƒÂÃƒÂ¶Ã‚Â¿ÃƒÂ Ãƒâ€¡Ãƒâ€Ã‚Â²Ã‚Â²
                            //Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                            Effect_Damage_Spot_DamageMove(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                            // Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚Â¼Ã‚Â­ Ã‚ÂµÃ‚Â¿Ãƒâ‚¬Ãƒâ€ºÃƒâ‚¬ÃƒÅ’ Ã‚Â´ÃƒÅ Ã‚Â¾ÃƒÂ®ÃƒÂÃƒÂ¶Ã‚Â´Ãƒâ€š ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂº
                            switch (cOwnerType)
                            {
                                case DEF_OWNERTYPE_PLAYER:
                                    if (m_pClientList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    // Ãƒâ€¦Ã‚Â¸Ã‚Â°Ãƒâ„¢Ãƒâ‚¬ÃƒÅ’ Ã‚Â»ÃƒÂ¬Ã‚Â¾Ãƒâ€ Ãƒâ‚¬Ãƒâ€“Ã‚Â°ÃƒÂ­ Ã‚Â¾ÃƒÂ³Ãƒâ‚¬Ã‚Â½ Ãƒâ‚¬ÃƒÂºÃƒâ€¡Ãƒâ€”Ã‚Â¿Ã‚Â¡ Ã‚Â½Ãƒâ€¡Ãƒâ€ ÃƒÂÃƒâ€¡ÃƒÅ¸Ã‚Â´Ãƒâ„¢Ã‚Â¸ÃƒÂ© Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚ÂºÃƒâ„¢Ã‚Â´Ãƒâ€šÃ‚Â´Ãƒâ„¢.
                                    if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                    {
                                        if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                        {
                                            m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                            SetIceFlag(sOwnerH, cOwnerType, true);
                                            // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                            bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                                sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                            // Ã‚Â´ÃƒÂ«Ã‚Â»ÃƒÂ³Ãƒâ‚¬ÃƒÅ’ Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ãƒâ‚¬ÃƒÅ½ Ã‚Â°ÃƒÂ¦Ã‚Â¿ÃƒÂ¬ Ã‚Â¾Ãƒâ€¹Ã‚Â·ÃƒÂÃƒÂÃƒËœÃ‚Â´Ãƒâ„¢.
                                            SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
                                        }
                                    }
                                    break;

                                case DEF_OWNERTYPE_NPC:
                                    if (m_pNpcList[sOwnerH] == 0) goto MAGIC_NOEFFECT;
                                    if ((m_pNpcList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                    {
                                        if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                        {
                                            m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                            SetIceFlag(sOwnerH, cOwnerType, true);
                                            // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                            bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                                sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                        }
                                    }
                                    break;
                            }

                        }

                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                            (m_pClientList[sOwnerH]->m_iHP > 0))
                        {
                            // ÃƒÂÃƒâ€”Ãƒâ‚¬Ã‚Âº ÃƒÆ’Ã‚Â´Ãƒâ€¡ÃƒÂÃ‚Â°ÃƒÂ­ Ãƒâ‚¬Ãƒâ€“Ã‚Â´Ãƒâ€š Ãƒâ€¡ÃƒÆ’Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ®Ã‚Â´Ãƒâ„¢.
                            if (bCheckResistingMagicSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            {

                                //Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                Effect_Damage_Spot(iClientH, DEF_OWNERTYPE_PLAYER, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                                // Ã‚Â¾ÃƒÂ³Ã‚Â¾ÃƒÂ®Ã‚Â¼Ã‚Â­ Ã‚ÂµÃ‚Â¿Ãƒâ‚¬Ãƒâ€ºÃƒâ‚¬ÃƒÅ’ Ã‚Â´ÃƒÅ Ã‚Â¾ÃƒÂ®ÃƒÂÃƒÂ¶Ã‚Â´Ãƒâ€š ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂº. ÃƒÂÃƒâ€”ÃƒÂÃƒÂ¶ Ã‚Â¾ÃƒÅ Ã‚Â¾Ãƒâ€™Ãƒâ‚¬Ã‚Â¸Ã‚Â¸ÃƒÂ© Ãƒâ‚¬ÃƒÂ»Ã‚Â¿ÃƒÂ« 
                                if ((m_pClientList[sOwnerH]->m_iHP > 0) && (bCheckResistingIceSuccess(m_pClientList[iClientH]->m_cDir, sOwnerH, cOwnerType, iResult) == false))
                                {
                                    if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                    {
                                        m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                        SetIceFlag(sOwnerH, cOwnerType, true);
                                        // ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â°Ã‚Â¡ Ãƒâ€¡ÃƒËœÃƒÂÃ‚Â¦Ã‚ÂµÃƒâ€° Ã‚Â¶Ã‚Â§ Ã‚Â¹ÃƒÅ¸Ã‚Â»ÃƒÂ½Ãƒâ€¡Ãƒâ€™ Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’ Ãƒâ‚¬ÃƒÅ’Ã‚ÂºÃ‚Â¥Ãƒâ€ Ã‚Â®Ã‚Â¸Ã‚Â¦ Ã‚ÂµÃƒÂ®Ã‚Â·ÃƒÂÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
                                        bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (m_pMagicConfigList[sType]->m_sValue10 * 1000),
                                            sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);

                                        SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
                                    }
                                }
                            }
                        }
                    }
                break;

            default:
                break;
        }
    }
    else
    {
        // Casting ÃƒË†Ãƒâ€ž Ã‚ÂµÃƒÂ´Ã‚Â·Ã‚Â¹Ãƒâ‚¬ÃƒÅ’Ã‚Â°Ã‚Â¡ Ã‚Â°Ãƒâ€°Ã‚Â¸Ã‚Â®Ã‚Â´Ãƒâ€š Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½
        // Resurrection wand(MS.10) or Resurrection wand(MS.20)

        if (m_pMagicConfigList[sType]->m_sType == DEF_MAGICTYPE_RESURRECTION)
        {
            //Check if player has resurrection wand
            if (m_pClientList[iClientH] != 0 && m_pClientList[iClientH]->m_iSpecialAbilityTime == 0 &&
                m_pClientList[iClientH]->m_bIsSpecialAbilityEnabled == false)
            {
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
                if (m_pClientList[sOwnerH] != 0)
                {
                    // GM's can ressurect ne1, and players must be on same side to ressurect

                    if ((m_pClientList[iClientH]->m_iAdminUserLevel < 1) &&
                        (m_pClientList[sOwnerH]->m_cSide != m_pClientList[iClientH]->m_cSide))
                    {
                        return;
                    }
                    if (cOwnerType == DEF_OWNERTYPE_PLAYER && m_pClientList[sOwnerH] != 0 &&
                        m_pClientList[sOwnerH]->m_iHP <= 0)
                    {
                        m_pClientList[sOwnerH]->m_bIsBeingResurrected = true;
                        SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_RESURRECTPLAYER, 0, 0, 0, 0);
                        if (m_pClientList[iClientH]->m_iAdminUserLevel < 2)
                        {
                            m_pClientList[iClientH]->m_bIsSpecialAbilityEnabled = true;
                            m_pClientList[iClientH]->m_dwSpecialAbilityStartTime = dwTime;
                            m_pClientList[iClientH]->m_iSpecialAbilityLastSec = 0;
                            m_pClientList[iClientH]->m_iSpecialAbilityTime = m_pMagicConfigList[sType]->m_dwDelayTime;

                            sTemp = m_pClientList[iClientH]->m_sAppr4;
                            sTemp = 0xFF0F & sTemp;
                            sTemp = sTemp | 0x40;
                            m_pClientList[iClientH]->m_sAppr4 = sTemp;
                        }
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SPECIALABILITYSTATUS, 1, m_pClientList[iClientH]->m_iSpecialAbilityType, m_pClientList[iClientH]->m_iSpecialAbilityLastSec, 0);
                        SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
                    }
                }
            }
        }
    }

    MAGIC_NOEFFECT:;

    if (m_pClientList[iClientH] == nullptr) return;

    //Mana Slate
    if ((m_pClientList[iClientH]->m_iStatus & 0x800000) != 0)
    {
        iManaCost = 0;
    }

    // ManaÃ‚Â¸Ã‚Â¦ Ã‚Â°Ã‚Â¨Ã‚Â¼Ãƒâ€™Ã‚Â½ÃƒÆ’Ãƒâ€¦Ã‚Â°Ã‚Â°ÃƒÂ­ Ãƒâ€¦ÃƒÂ«Ã‚ÂºÃ‚Â¸Ãƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢.
    m_pClientList[iClientH]->m_iMP -= iManaCost; // sValue1Ãƒâ‚¬ÃƒÅ’ Mana Cost
    if (m_pClientList[iClientH]->m_iMP < 0)
        m_pClientList[iClientH]->m_iMP = 0;

    CalculateSSN_SkillIndex(iClientH, 4, 1);

    SendNotifyMsg(0, iClientH, DEF_NOTIFY_MP, 0, 0, 0, 0);

    // Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½ ÃƒË†Ã‚Â¿Ã‚Â°ÃƒÂºÃ‚Â¸Ã‚Â¦ Ã‚Â´Ãƒâ„¢Ã‚Â¸Ã‚Â¥ Ãƒâ€¦Ã‚Â¬Ã‚Â¶ÃƒÂ³Ãƒâ‚¬ÃƒÅ’Ã‚Â¾ÃƒÂ°Ãƒâ€ Ã‚Â®Ã‚Â¿Ã‚Â¡Ã‚Â°Ãƒâ€ Ãƒâ‚¬ÃƒÂ¼Ã‚Â¼Ãƒâ€ºÃƒâ€¡Ãƒâ€˜Ã‚Â´Ãƒâ„¢. Ã‚Â¸Ã‚Â¶Ã‚Â¹ÃƒÂ½Ã‚Â¹ÃƒÂ¸ÃƒË†Ã‚Â£ + 100Ãƒâ‚¬ÃƒÅ’ Ã‚Â¿Ã‚Â¡Ãƒâ€ ÃƒÂ¥Ãƒâ€ Ã‚Â® Ã‚Â¹ÃƒÂ¸ÃƒË†Ã‚Â£ 
    SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_MAGIC, m_pClientList[iClientH]->m_cMapIndex,
        m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, dX, dY, (sType + 100), m_pClientList[iClientH]->m_sType);

}

void CGame::NpcMagicHandler(int iNpcH, short dX, short dY, short sType)
{
    short  sOwnerH;
    char   cOwnerType;
    int i, iErr, ix, iy, sX, sY, tX, tY, iResult, iWhetherBonus, iMagicAttr;
    uint32_t  dwTime = timeGetTime();

    if (m_pNpcList[iNpcH] == 0) return;
    if ((dX < 0) || (dX >= m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_sSizeX) ||
        (dY < 0) || (dY >= m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_sSizeY)) return;

    if ((sType < 0) || (sType >= 100))     return;
    if (m_pMagicConfigList[sType] == 0) return;

    // °ø°ÝÀ§Ä¡°¡ °ø°Ý ºÒ°¡´É ¸ÊÀÌ¶ó¸é °ø°Ý ºÒ°¡´É 
    if (m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_bIsAttackEnabled == false) return;

    // ¸ó½ºÅÍ º° ¸¶¹ý ¸íÁß·ü ÀÔ·Â 
    iResult = m_pNpcList[iNpcH]->m_iMagicHitRatio;

    // ³¯¾¾¿¡ ÀÇÇÑ ¸¶¹ý °ø°Ý·Â Á¶Á¤  
    iWhetherBonus = iGetWhetherMagicBonusEffect(sType, m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_cWhetherStatus);

    // v1.41 ¸¶¹ý ¼Ó¼º 
    iMagicAttr = m_pMagicConfigList[sType]->m_iAttribute;

    if (m_pMagicConfigList[sType]->m_dwDelayTime == 0)
    {
        // Áï½Ã È¿°ú¸¦ º¸´Â ¸¶¹ý 
        switch (m_pMagicConfigList[sType]->m_sType)
        {
            case DEF_MAGICTYPE_INVISIBILITY:
                switch (m_pMagicConfigList[sType]->m_sValue4)
                {
                    case 1:
                        // º¸ÀÌÁö ¾Ê´Â »óÅÂ·Î ¸¸µç´Ù. 
                        m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);

                        switch (cOwnerType)
                        {
                            case DEF_OWNERTYPE_PLAYER:
                                if (m_pClientList[sOwnerH] == 0) goto NMH_NOEFFECT;
                                if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0) goto NMH_NOEFFECT;
                                m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = (char)m_pMagicConfigList[sType]->m_sValue4;
                                SetInvisibilityFlag(sOwnerH, cOwnerType, true);
                                // ÀÌ Ä³¸¯ÅÍ¸¦ ÃßÀûÇÏ°í ÀÖ´ø ¸ó½ºÅÍ¸¦ ÇØÁ¦½ÃÅ²´Ù.
                                RemoveFromTarget(sOwnerH, DEF_OWNERTYPE_PLAYER);
                                break;

                            case DEF_OWNERTYPE_NPC:
                                if (m_pNpcList[sOwnerH] == 0) goto NMH_NOEFFECT;
                                if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0) goto NMH_NOEFFECT;
                                m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = (char)m_pMagicConfigList[sType]->m_sValue4;
                                SetInvisibilityFlag(sOwnerH, cOwnerType, true);
                                // ÀÌ NPC¸¦ ÃßÀûÇÏ°í ÀÖ´ø ¸ó½ºÅÍ¸¦ ÇØÁ¦½ÃÅ²´Ù.
                                RemoveFromTarget(sOwnerH, DEF_OWNERTYPE_NPC);
                                break;
                        }

                        // È¿°ú°¡ ÇØÁ¦µÉ ¶§ ¹ß»ýÇÒ µô·¹ÀÌ ÀÌº¥Æ®¸¦ µî·ÏÇÑ´Ù.
                        bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_INVISIBILITY, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
                            sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

                        if (cOwnerType == DEF_OWNERTYPE_PLAYER)
                            SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_INVISIBILITY, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
                        break;

                    case 2:
                        // dX, dY ¹Ý°æ 8 ÁÖº¯ÀÇ Invisibility »óÅÂÀÎ Object°¡ ÀÖÀ¸¸é ÇØÁ¦ ½ÃÅ²´Ù.
                        for (ix = dX - 8; ix <= dX + 8; ix++)
                            for (iy = dY - 8; iy <= dY + 8; iy++)
                            {
                                m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                                if (sOwnerH != 0)
                                {
                                    switch (cOwnerType)
                                    {
                                        case DEF_OWNERTYPE_PLAYER:
                                            if (m_pClientList[sOwnerH] == 0) goto NMH_NOEFFECT;
                                            if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0)
                                            {
                                                if (m_pClientList[sOwnerH]->m_sType != 66)
                                                {
                                                    m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = 0;
                                                    SetInvisibilityFlag(sOwnerH, cOwnerType, false);
                                                    bRemoveFromDelayEventList(sOwnerH, cOwnerType, DEF_MAGICTYPE_INVISIBILITY);
                                                }
                                            }
                                            break;

                                        case DEF_OWNERTYPE_NPC:
                                            if (m_pNpcList[sOwnerH] == 0) goto NMH_NOEFFECT;
                                            if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0)
                                            {
                                                if (m_pClientList[sOwnerH]->m_sType != 66)
                                                {
                                                    m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = 0;
                                                    SetInvisibilityFlag(sOwnerH, cOwnerType, false);
                                                    bRemoveFromDelayEventList(sOwnerH, cOwnerType, DEF_MAGICTYPE_INVISIBILITY);
                                                }
                                            }
                                            break;
                                    }
                                }
                            }
                        break;
                }
                break;

            case DEF_MAGICTYPE_HOLDOBJECT:
                // ¿ÀºêÁ§Æ®ÀÇ ¿òÁ÷ÀÓÀ» ºÀ¼âÇÑ´Ù. 
                m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
                if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                {

                    switch (cOwnerType)
                    {
                        case DEF_OWNERTYPE_PLAYER:
                            if (m_pClientList[sOwnerH] == 0) goto NMH_NOEFFECT;
                            if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0) goto NMH_NOEFFECT;
                            if (m_pClientList[sOwnerH]->m_iAddPR >= 500) goto NMH_NOEFFECT;
                            m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = (char)m_pMagicConfigList[sType]->m_sValue4;
                            break;

                        case DEF_OWNERTYPE_NPC:
                            if (m_pNpcList[sOwnerH] == 0) goto NMH_NOEFFECT;
                            if (m_pNpcList[sOwnerH]->m_cMagicLevel >= 6) goto NMH_NOEFFECT; // v1.4 ¸¶¹ý ·¹º§ 6ÀÌ»óÀÎ ¸®Ä¡±Þ ÀÌ»óÀÇ ¸¶¹ý ¸ó½ºÅÍ¿¡°Ô´Â ¸¶ºñ ¸¶¹ý ÅëÇÏÁö ¾Ê´Â´Ù.
                            if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0) goto NMH_NOEFFECT;
                            m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = (char)m_pMagicConfigList[sType]->m_sValue4;
                            break;
                    }

                    // ºÀ¼â È¿°ú°¡ ÇØÁ¦µÉ ¶§ ¹ß»ýÇÒ µô·¹ÀÌ ÀÌº¥Æ®¸¦ µî·ÏÇÑ´Ù.
                    bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_HOLDOBJECT, dwTime + (m_pMagicConfigList[sType]->m_dwLastTime * 1000),
                        sOwnerH, cOwnerType, 0, 0, 0, m_pMagicConfigList[sType]->m_sValue4, 0, 0);

                    // È¿°ú°¡ »ý°åÀ½À» ¾Ë·ÁÁØ´Ù.
                    if (cOwnerType == DEF_OWNERTYPE_PLAYER)
                        SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_HOLDOBJECT, m_pMagicConfigList[sType]->m_sValue4, 0, 0);
                }
                break;


            case DEF_MAGICTYPE_DAMAGE_LINEAR:
                // ÀÏÁ÷¼± »ó¿¡ ÀÖ´Â ¸ñÇ¥¸¦ ¸ðµÎ °ø°ÝÇÑ´Ù.
                sX = m_pNpcList[iNpcH]->m_sX;
                sY = m_pNpcList[iNpcH]->m_sY;

                for (i = 2; i < 10; i++)
                {
                    iErr = 0;
                    m_Misc.GetPoint2(sX, sY, dX, dY, &tX, &tY, &iErr, i);

                    // tx, ty
                    m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
                    if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

                    m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY);
                    if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                        (m_pClientList[sOwnerH]->m_iHP > 0))
                    {
                        // Á×Àº Ã´ÇÏ°í ÀÖ´Â ÇÃ·¹ÀÌ¾î´Ù.
                        if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                    }

                    // tx-1, ty
                    m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
                    if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

                    m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX - 1, tY);
                    if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                        (m_pClientList[sOwnerH]->m_iHP > 0))
                    {
                        // Á×Àº Ã´ÇÏ°í ÀÖ´Â ÇÃ·¹ÀÌ¾î´Ù.
                        if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                    }

                    // tx+1, ty
                    m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
                    if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

                    m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX + 1, tY);
                    if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                        (m_pClientList[sOwnerH]->m_iHP > 0))
                    {
                        // Á×Àº Ã´ÇÏ°í ÀÖ´Â ÇÃ·¹ÀÌ¾î´Ù.
                        if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                    }

                    // tx, ty-1
                    m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
                    if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

                    m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY - 1);
                    if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                        (m_pClientList[sOwnerH]->m_iHP > 0))
                    {
                        // Á×Àº Ã´ÇÏ°í ÀÖ´Â ÇÃ·¹ÀÌ¾î´Ù.
                        if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                    }

                    // tx, ty+1
                    m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
                    if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

                    m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, tX, tY + 1);
                    if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                        (m_pClientList[sOwnerH]->m_iHP > 0))
                    {
                        // Á×Àº Ã´ÇÏ°í ÀÖ´Â ÇÃ·¹ÀÌ¾î´Ù.
                        if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                    }

                    if ((abs(tX - dX) <= 1) && (abs(tY - dY) <= 1)) break;
                }

                // ÁÖº¯ °ø°Ý È¿°ú 
                for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
                    for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++)
                    {
                        // ÀÚ½Åµµ ÇÇÆøµÉ ¼ö ÀÖÀ¸´Ï ÁÖÀÇ.
                        m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

                        m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                            (m_pClientList[sOwnerH]->m_iHP > 0))
                        {
                            // Á×Àº Ã´ÇÏ°í ÀÖ´Â ÇÃ·¹ÀÌ¾î´Ù.
                            if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                        }
                    }

                // dX, dY
                m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
                if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                    Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);

                m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
                if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                    (m_pClientList[sOwnerH]->m_iHP > 0))
                {
                    // Á×Àº Ã´ÇÏ°í ÀÖ´Â ÇÃ·¹ÀÌ¾î´Ù.
                    if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, false, iMagicAttr);
                }
                break;

            case DEF_MAGICTYPE_DAMAGE_SPOT:
                m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
                if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                    Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);

                m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
                if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                    (m_pClientList[sOwnerH]->m_iHP > 0))
                {
                    // Á×Àº Ã´ÇÏ°í ÀÖ´Â ÇÃ·¹ÀÌ¾î´Ù.
                    if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                }
                break;

            case DEF_MAGICTYPE_HPUP_SPOT:
                // ÀÌ ¸¶¹ýÀº ¸íÁß·ü°ú´Â »ó°üÀÌ ¾ø´Ù.
                m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
                Effect_HpUp_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6);
                break;

            case DEF_MAGICTYPE_DAMAGE_AREA:
                m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
                if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                    Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);

                m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, dX, dY);
                if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                    (m_pClientList[sOwnerH]->m_iHP > 0))
                {
                    // Á×Àº Ã´ÇÏ°í ÀÖ´Â ÇÃ·¹ÀÌ¾î´Ù.
                    if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                        Effect_Damage_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6 + iWhetherBonus, true, iMagicAttr);
                }

                // ÁÖº¯ °ø°Ý È¿°ú 
                for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
                    for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++)
                    {
                        // ÀÚ½Åµµ ÇÇÆøµÉ ¼ö ÀÖÀ¸´Ï ÁÖÀÇ.
                        m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

                        m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                            (m_pClientList[sOwnerH]->m_iHP > 0))
                        {
                            // Á×Àº Ã´ÇÏ°í ÀÖ´Â ÇÃ·¹ÀÌ¾î´Ù.
                            if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                        }
                    }
                break;

            case DEF_MAGICTYPE_DAMAGE_AREA_NOSPOT:
                // Á÷°ÝÀº °è»êÇÏÁö ¾Ê´Â´Ù.			
                // ÁÖº¯ °ø°Ý È¿°ú 
                for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
                    for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++)
                    {
                        // ÀÚ½Åµµ ÇÇÆøµÉ ¼ö ÀÖÀ¸´Ï ÁÖÀÇ.
                        m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);

                        m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                            (m_pClientList[sOwnerH]->m_iHP > 0))
                        {
                            // Á×Àº Ã´ÇÏ°í ÀÖ´Â ÇÃ·¹ÀÌ¾î´Ù.
                            if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                                Effect_Damage_Spot_DamageMove(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, dX, dY, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9 + iWhetherBonus, false, iMagicAttr);
                        }
                    }
                break;

            case DEF_MAGICTYPE_SPDOWN_AREA:
                // Sp°¡ ÁÙ¾îµç´Ù.
                m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
                if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                    Effect_SpDown_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6);
                // ÁÖº¯ °ø°Ý È¿°ú 
                for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
                    for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++)
                    {
                        // ÀÚ½Åµµ ÇÇÆøµÉ ¼ö ÀÖÀ¸´Ï ÁÖÀÇ.
                        m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                        if (bCheckResistingMagicSuccess(m_pNpcList[iNpcH]->m_cDir, sOwnerH, cOwnerType, iResult) == false)
                            Effect_SpDown_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                    }
                break;

            case DEF_MAGICTYPE_SPUP_AREA:
                // Sp°¡ »ó½ÂÇÑ´Ù.
                m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
                // ¸¶¹ý ÀúÇ×ÀÌ ÇÊ¿ä¾ø´Ù. 
                Effect_SpUp_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue4, m_pMagicConfigList[sType]->m_sValue5, m_pMagicConfigList[sType]->m_sValue6);
                // ÁÖº¯ °ø°Ý È¿°ú 
                for (iy = dY - m_pMagicConfigList[sType]->m_sValue3; iy <= dY + m_pMagicConfigList[sType]->m_sValue3; iy++)
                    for (ix = dX - m_pMagicConfigList[sType]->m_sValue2; ix <= dX + m_pMagicConfigList[sType]->m_sValue2; ix++)
                    {
                        // ÀÚ½Åµµ ÇÇÆøµÉ ¼ö ÀÖÀ¸´Ï ÁÖÀÇ.
                        m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                        // ¸¶¹ýÀúÇ×ÀÌ ÇÊ¿ä ¾ø´Ù.
                        Effect_SpUp_Spot(iNpcH, DEF_OWNERTYPE_NPC, sOwnerH, cOwnerType, m_pMagicConfigList[sType]->m_sValue7, m_pMagicConfigList[sType]->m_sValue8, m_pMagicConfigList[sType]->m_sValue9);
                    }
                break;

        }
    }
    else
    {
        // Casting ÈÄ µô·¹ÀÌ°¡ °É¸®´Â ¸¶¹ý

    }

    NMH_NOEFFECT:;

    // Mana¸¦ °¨¼Ò½ÃÅ²´Ù.
    m_pNpcList[iNpcH]->m_iMana -= m_pMagicConfigList[sType]->m_sValue1; // sValue1ÀÌ Mana Cost
    if (m_pNpcList[iNpcH]->m_iMana < 0)
        m_pNpcList[iNpcH]->m_iMana = 0;

    // ¸¶¹ý È¿°ú¸¦ ´Ù¸¥ Å¬¶óÀÌ¾ðÆ®¿¡°Ô Àü¼ÛÇÑ´Ù. ¸¶¹ý¹øÈ£ + 100ÀÌ ¿¡ÆåÆ® ¹øÈ£ 
    SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_MAGIC, m_pNpcList[iNpcH]->m_cMapIndex,
        m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, dX, dY, (sType + 100), m_pNpcList[iNpcH]->m_sType);

}

void CGame::RequestTeleportHandler(int iClientH, char * pData, char * cMapName, int dX, int dY)
{
    char * pBuffer, cTempMapName[21];
    uint32_t * dwp;
    uint16_t * wp;
    char * cp, cDestMapName[11], cDir, cMapIndex, cQuestRemain;
    short * sp, sX, sY, sSummonPoints;
    int * ip, i, iRet, iSize, iDestX, iDestY, iExH, iMapSide;
    bool    bRet, bIsLockedMapNotify;
    SYSTEMTIME SysTime;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    if (m_pClientList[iClientH]->m_bIsKilled == true) return;
    if (m_pClientList[iClientH]->m_bIsOnWaitingProcess == true) return;
    if ((m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsRecallImpossible == true) && (m_pClientList[iClientH]->m_iAdminUserLevel == 0) &&
        (m_pClientList[iClientH]->m_bIsKilled == false) && (m_bIsApocalypseMode == true) && (m_pClientList[iClientH]->m_iHP > 0))
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_NORECALL, 0, 0, 0, 0);
        return;
    }
    // v2.16 2002-6-2 »ó´ë¹æ ¸¶À»¿¡¼­´Â ¸®ÄÝÀÌ µÇÁö ¾Ê´Â´Ù.
    if ((memcmp(m_pClientList[iClientH]->m_cLocation, "elvine", 6) == 0)
        && (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall > 0)
        && (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "aresden", 7) == 0)
        && ((pData[0] == '1') || (pData[0] == '3'))
        && (m_pClientList[iClientH]->m_iAdminUserLevel == 0)
        && (m_bIsCrusadeMode == false)) return;

    // v2.16 2002-6-2 »ó´ë¹æ ¸¶À»¿¡¼­´Â ¸®ÄÝÀÌ µÇÁö ¾Ê´Â´Ù.
    if ((memcmp(m_pClientList[iClientH]->m_cLocation, "aresden", 7) == 0)
        && (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall > 0)
        && (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "elvine", 6) == 0)
        && ((pData[0] == '1') || (pData[0] == '3'))
        && (m_pClientList[iClientH]->m_iAdminUserLevel == 0)
        && (m_bIsCrusadeMode == false)) return;

    bIsLockedMapNotify = false;

    // Ã«Â§Å’Ã¬â€¢Â½ ÃªÂµÂÃ­â„¢Ëœ Ã«ÂªÂ¨Ã«â€œÅ“Ã«ÂÂ¼Ã«Â©Â´ ÃªÂµÂÃ­â„¢ËœÃ¬Ââ€ž Ã¬Â·Â¨Ã¬â€ Å’Ã­â€¢Å“Ã«â€¹Â¤.
    if (m_pClientList[iClientH]->m_bIsExchangeMode == true)
    {
        iExH = m_pClientList[iClientH]->m_iExchangeH;
        _ClearExchangeStatus(iExH);
        _ClearExchangeStatus(iClientH);
    }

    // Ã¬Â¤â€˜Ã«Â¦Â½Ã¬ÂÂ´ Ã«Â¦Â¬Ã¬Â½Å“ Ã«Â§Ë†Ã«Â²â€¢Ã¬Ââ€ž Ã¬â€šÂ¬Ã¬Å¡Â©Ã­â€¢ËœÃ«Â©Â´ Ã­Ë†Â¬Ã«Âªâ€¦Ã­â€¢ËœÃªÂ²Å’ Ã«ÂËœÃ«Å â€ Ã«Â²â€žÃªÂ·Â¸ Ã¬Ë†ËœÃ¬Â â€¢ 
    // Ã¬Â¤â€˜Ã«Â¦Â½Ã¬ÂÂ´Ã«Â©Â´ Ã«Â¦Â¬Ã¬Â½Å“ Ã«Â§Ë†Ã«Â²â€¢Ã¬Ââ€ž Ã¬â€šÂ¬Ã¬Å¡Â©Ã­â€¢ËœÃ¬Â§â‚¬ Ã«ÂªÂ»Ã­â€¢ËœÃªÂ²Å’ Ã«ÂÅ“Ã«â€¹Â¤. 
    if ((memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) && (pData[0] == '1'))
        return;

    // Ã­Ëœâ€žÃ¬Å¾Â¬ Ã¬ÂÂ´ Ã­â€Å’Ã«Â Ë†Ã¬ÂÂ´Ã¬â€“Â´Ã«Â¥Â¼ Ã«ÂªÂ©Ã­â€˜Å“Ã«Â¡Å“ Ã¬â€šÂ¼ÃªÂ³Â  Ã¬Å¾Ë†Ã«Å â€ Ã«ÂªÂ¹Ã«â€œÂ¤Ã¬ÂËœ ÃªÂ³ÂµÃªÂ²Â©Ã­â€“â€°Ã«Ââ„¢Ã¬Ââ€ž Ã«Â©Ë†Ã¬Â¶â€ÃªÂ²Å’ Ã­â€¢Å“Ã«â€¹Â¤. 
    RemoveFromTarget(iClientH, DEF_OWNERTYPE_PLAYER);

    // Ã­Ëœâ€žÃ¬Å¾Â¬ Ã«Â§Âµ Ã¬Å“â€žÃ¬Â¹ËœÃ¬â€”ÂÃ¬â€žÅ“ Ã¬Â§â‚¬Ã¬Å¡Â´Ã«â€¹Â¤.
    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->ClearOwner(13, iClientH, DEF_OWNERTYPE_PLAYER,
        m_pClientList[iClientH]->m_sX,
        m_pClientList[iClientH]->m_sY);

    // Ã«â€¹Â¤Ã«Â¥Â¸ Ã­ÂÂ´Ã«ÂÂ¼Ã¬ÂÂ´Ã¬â€“Â¸Ã­Å Â¸Ã«â€œÂ¤Ã¬â€”ÂÃªÂ²Å’ Ã­â€Å’Ã«Â Ë†Ã¬ÂÂ´Ã¬â€“Â´ÃªÂ°â‚¬ Ã­Ëœâ€ž Ã¬Å“â€žÃ¬Â¹ËœÃ¬â€”ÂÃ¬â€žÅ“ Ã¬â€šÂ¬Ã«ÂÂ¼Ã¬Â§ÂÃ¬Ââ€ž Ã¬â€¢Å’Ã«Â¦Â°Ã«â€¹Â¤. 
    SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_LOG, DEF_MSGTYPE_REJECT, 0, 0, 0);

    // Ã­Ëœâ€žÃ¬Å¾Â¬ Ã­â€¦â€Ã«Â Ë†Ã­ÂÂ¬Ã­Å Â¸ÃªÂ°â‚¬ Ã«Â§Ë†Ã«Â²â€¢Ã¬â€”Â Ã¬ÂËœÃ­â€¢Å“ ÃªÂ²Æ’Ã¬ÂÂ¸Ã¬Â§â‚¬, Ã¬â€¢â€žÃ«â€¹Ë†Ã«Â©Â´ Ã¬ÂÂ¼Ã«Â°ËœÃ¬Â ÂÃ¬ÂÂ¸ Ã­â€¦â€Ã«Â Ë†Ã­ÂÂ¬Ã­Å Â¸ Ã­Æ’â‚¬Ã¬ÂÂ¼Ã¬â€”Â Ã¬ÂËœÃ­â€¢Å“ ÃªÂ²Æ’Ã¬ÂÂ¸Ã¬Â§â‚¬Ã«Â¥Â¼ Ã¬Â²Â´Ã­ÂÂ¬Ã­â€¢Å“Ã«â€¹Â¤. 
    // Ã«Â§Å’Ã¬â€¢Â½ Ã¬ÂÂ¼Ã«Â°ËœÃ¬Â ÂÃ¬ÂÂ¸ Ã­â€¦â€Ã«Â Ë†Ã­ÂÂ¬Ã­Å Â¸ Ã­Æ’â‚¬Ã¬ÂÂ¼Ã¬â€”Â Ã¬ÂËœÃ­â€¢Å“ ÃªÂ²Æ’Ã¬ÂÂ´Ã«ÂÂ¼Ã«Â©Â´ Ã­â€¦â€Ã«Â Ë†Ã­ÂÂ¬Ã­Å Â¸Ã­â€¢Â  Ã¬Å“â€žÃ¬Â¹ËœÃ¬ÂËœ Ã«Â§ÂµÃ¬ÂÂ´Ã«Â¦â€ž, Ã¬Â¢Å’Ã­â€˜Å“Ã«Â¥Â¼ ÃªÂµÂ¬Ã­â€¢Â´Ã¬â„¢â‚¬Ã¬â€žÅ“ 
    // ÃªÂ·Â¸ Ã«Â§ÂµÃ¬ÂÂ´ Ã­Ëœâ€žÃ¬Å¾Â¬ Ã¬â€žÅ“Ã«Â²â€žÃ¬â€”Â Ã¬Â¡Â´Ã¬Å¾Â¬Ã­â€¢ËœÃ«Å â€Ã¬Â§â‚¬Ã«Â¥Â¼ ÃªÂ²â‚¬Ã¬Æ’â€°Ã­â€¢Å“Ã«â€¹Â¤. Ã«Â§Å’Ã¬â€¢Â½ Ã­Ëœâ€žÃ¬Å¾Â¬ Ã¬â€žÅ“Ã«Â²â€žÃ¬â€”Â Ã¬Â¡Â´Ã¬Å¾Â¬Ã­â€¢ËœÃ¬Â§â‚¬ Ã¬â€¢Å Ã«Å â€Ã«â€¹Â¤Ã«Â©Â´ Ã­ÂÂ´Ã«ÂÂ¼Ã¬ÂÂ´Ã¬â€“Â¸Ã­Å Â¸Ã¬â€”ÂÃªÂ²Å’ 
    // Ã«â€¹Â¤Ã«Â¥Â¸ Ã¬â€žÅ“Ã«Â²â€žÃ«Â¡Å“Ã¬ÂËœ Ã¬Â â€˜Ã¬â€ ÂÃ¬Ââ€ž Ã¬â€¹Å“Ã«Ââ€žÃ­â€¢ËœÃ«ÂÂ¼Ã«Å â€ Ã«Â©â€Ã¬â€¹Å“Ã¬Â§â‚¬Ã«Â¥Â¼ Ã«Â³Â´Ã«â€šÂ¸Ã«â€¹Â¤. 

    // Ã«Â¨Â¼Ã¬Â â‚¬ Ã­â€Å’Ã«Â Ë†Ã¬ÂÂ´Ã¬â€“Â´ÃªÂ°â‚¬ Ã¬â€žÅ“Ã¬Å¾Ë†Ã«Å â€ Ã¬Å“â€žÃ¬Â¹ËœÃªÂ°â‚¬ Ã­â€¦â€Ã«Â Ë†Ã­ÂÂ¬Ã­Å Â¸ Ã­Æ’â‚¬Ã¬ÂÂ¼Ã¬ÂÂ¸Ã¬Â§â‚¬ Ã¬â€¢Å’Ã¬â€¢â€žÃ«â€šÂ¸Ã«â€¹Â¤. 
    sX = m_pClientList[iClientH]->m_sX;
    sY = m_pClientList[iClientH]->m_sY;

    memset(cDestMapName, 0, sizeof(cDestMapName));
    bRet = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSearchTeleportDest(sX, sY, cDestMapName, &iDestX, &iDestY, &cDir);

    // Crusade
    if ((strcmp(m_pClientList[iClientH]->m_cLockedMapName, "NONE") != 0) && (m_pClientList[iClientH]->m_iLockedMapTime > 0))
    {
        // Ã­Å Â¹Ã¬Â â€¢ Ã«Â§ÂµÃ¬â€”Â ÃªÂ°â€“Ã­Å¾Å’ Ã¬Æ’ÂÃ­Æ’Å“Ã¬ÂÂ´Ã«â€¹Â¤. Ã­â€¦â€Ã«Â Ë†Ã­ÂÂ¬Ã­Å Â¸ Ã­Æ’â‚¬Ã¬ÂÂ¼Ã¬ÂËœ Ã¬Å“â€žÃ¬Â¹ËœÃ«Â¥Â¼ ÃªÂ°â€“Ã­Å¾Å’ Ã«Â§ÂµÃ¬Å“Â¼Ã«Â¡Å“ Ã«Â³â‚¬Ã­Ëœâ€¢Ã­â€¢Å“Ã«â€¹Â¤.
        // Ã«Â§Å’Ã¬â€¢Â½ ÃªÂ°Ë† Ã«Â§ÂµÃ¬ÂÂ´ ÃªÂ±Â´Ã«Â¬Â¼ Ã«â€šÂ´Ã«Â¶â‚¬Ã«ÂÂ¼Ã«Â©Â´  
        iMapSide = iGetMapLocationSide(cDestMapName);
        if (iMapSide > 3) iMapSide -= 2; // New 18/05/2004
        if ((iMapSide != 0) && (m_pClientList[iClientH]->m_cSide == iMapSide))
        {
            // Ã«â€œÂ¤Ã¬â€“Â´ÃªÂ°â‚¬ÃªÂ³Â Ã¬Å¾Â Ã­â€¢ËœÃ«Å â€ Ã«Â§ÂµÃ¬ÂÂ´ ÃªÂ°â„¢Ã¬Ââ‚¬ Ã­Å½Â¸Ã¬ÂËœ ÃªÂ±Â´Ã«Â¬Â¼ Ã«â€šÂ´Ã«Â¶â‚¬Ã«ÂÂ¼Ã«Â©Â´ Ã«â€œÂ¤Ã¬â€“Â´ÃªÂ°Ë† Ã¬Ë†Ëœ Ã¬Å¾Ë†Ã«â€¹Â¤.
        }
        else
        {
            iDestX = -1;
            iDestY = -1;
            bIsLockedMapNotify = true;
            memset(cDestMapName, 0, sizeof(cDestMapName));
            strcpy(cDestMapName, m_pClientList[iClientH]->m_cLockedMapName);
        }
    }

    if ((bRet == true) && (cMapName == 0))
    {
        // Ã­â€Å’Ã«Â Ë†Ã¬ÂÂ´Ã¬â€“Â´ÃªÂ°â‚¬ Ã­â€¦â€Ã«Â Ë†Ã­ÂÂ¬Ã­Å Â¸ Ã­Æ’â‚¬Ã¬ÂÂ¼ Ã¬Å“â€žÃ¬â€”Â Ã¬â€žÅ“ Ã¬Å¾Ë†Ã«â€¹Â¤. Ã­â€¦â€Ã«Â Ë†Ã­ÂÂ¬Ã­Å Â¸ Ã­â€¢Â  Ã«Â§ÂµÃ¬ÂÂ´ Ã­Ëœâ€žÃ¬Å¾Â¬ Ã¬â€žÅ“Ã«Â²â€žÃ¬â€”Â Ã¬Â¡Â´Ã¬Å¾Â¬Ã­â€¢ËœÃ«Å â€Ã¬Â§â‚¬ ÃªÂ²â‚¬Ã¬Æ’â€°Ã­â€¢Å“Ã«â€¹Â¤. Ã«Â¬Â¼Ã«Â¡Â  Ã«ÂªÂ©Ã¬Â ÂÃ¬Â§â‚¬ÃªÂ°â‚¬ Ã¬Â§â‚¬Ã¬Â â€¢Ã«ÂËœÃ¬Â§â‚¬ Ã¬â€¢Å Ã¬Ââ‚¬ Ã­â€¦â€Ã«Â Ë†Ã­ÂÂ¬Ã­Å Â¸Ã¬â€”Â¬Ã¬â€¢Â¼Ã­â€¢Å“Ã«â€¹Â¤.
        for (i = 0; i < DEF_MAXMAPS; i++)
            if (m_pMapList[i] != 0)
            {
                if (memcmp(m_pMapList[i]->m_cName, cDestMapName, 10) == 0)
                {
                    // Ã­Ëœâ€žÃ¬Å¾Â¬ Ã¬â€žÅ“Ã«Â²â€žÃ¬â€”Â Ã­â€¦â€Ã«Â Ë†Ã­ÂÂ¬Ã­Å Â¸Ã­â€¢Â  Ã«Â§ÂµÃ¬ÂÂ´ Ã¬Â¡Â´Ã¬Å¾Â¬Ã­â€¢Å“Ã«â€¹Â¤.
                    m_pClientList[iClientH]->m_sX = iDestX;	  // Ã­â€¦â€Ã«Â Ë†Ã­ÂÂ¬Ã­Å Â¸ Ã¬Â¢Å’Ã­â€˜Å“Ã«Â¥Â¼ Ã¬Å¾â€¦Ã«Â Â¥Ã­â€¢Å“Ã«â€¹Â¤.
                    m_pClientList[iClientH]->m_sY = iDestY;
                    m_pClientList[iClientH]->m_cDir = cDir;
                    m_pClientList[iClientH]->m_cMapIndex = i; // Ã«Â§Âµ Ã¬ÂÂ¸Ã«ÂÂ±Ã¬Å Â¤Ã«Â¥Â¼ Ã«Â°â€ÃªÂ¾Â¼Ã«â€¹Â¤.
                    memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof(m_pClientList[iClientH]->m_cMapName));
                    memcpy(m_pClientList[iClientH]->m_cMapName, m_pMapList[i]->m_cName, 10);  // Ã«Â§Âµ Ã¬ÂÂ´Ã«Â¦â€žÃ¬Ââ€ž Ã«Â°â€ÃªÂ¾Â¼Ã«â€¹Â¤.
                    goto RTH_NEXTSTEP;
                }
            }

        // Ã­Ëœâ€žÃ¬Å¾Â¬ Ã¬â€žÅ“Ã«Â²â€žÃ¬â€”Â Ã­â€¦â€Ã«Â Ë†Ã­ÂÂ¬Ã­Å Â¸ Ã­â€¢Â  Ã«Â§ÂµÃ¬ÂÂ´ Ã¬â€”â€ Ã«â€¹Â¤. Ã­ÂÂ´Ã«ÂÂ¼Ã¬ÂÂ´Ã¬â€“Â¸Ã­Å Â¸Ã«Å â€ Ã­Ëœâ€žÃ¬Å¾Â¬ Ã¬Â â€˜Ã¬â€ ÂÃ¬Ââ€ž Ã¬Â¢â€¦Ã«Â£Å’Ã­â€¢ËœÃªÂ³Â  Ã«â€¹Â¤Ã«Â¥Â¸ Ã¬â€žÅ“Ã«Â²â€žÃ«Â¡Å“ Ã¬Â â€˜Ã¬â€ ÂÃ¬Ââ€ž Ã¬â€¹Å“Ã«Ââ€žÃ­â€¢Â´Ã¬â€¢Â¼ Ã­â€¢Å“Ã«â€¹Â¤.
        m_pClientList[iClientH]->m_sX = iDestX;	  // Ã­â€¦â€Ã«Â Ë†Ã­ÂÂ¬Ã­Å Â¸ Ã¬Â¢Å’Ã­â€˜Å“Ã«Â¥Â¼ Ã¬Å¾â€¦Ã«Â Â¥Ã­â€¢Å“Ã«â€¹Â¤.
        m_pClientList[iClientH]->m_sY = iDestY;
        m_pClientList[iClientH]->m_cDir = cDir;
        memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof(m_pClientList[iClientH]->m_cMapName));
        memcpy(m_pClientList[iClientH]->m_cMapName, cDestMapName, 10);  // Ã«Â§Âµ Ã¬ÂÂ´Ã«Â¦â€žÃ¬Ââ€ž Ã«Â°â€ÃªÂ¾Â¼Ã«â€¹Â¤.

        // New 18/05/2004
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_CONFUSE,
            m_pClientList[iClientH]->m_cMagicEffectStatus[DEF_MAGICTYPE_CONFUSE], 0, 0);
        SetSlateFlag(iClientH, DEF_NOTIFY_SLATECLEAR, false);

        // Ã­â€Å’Ã«Â Ë†Ã¬ÂÂ´Ã¬â€“Â´Ã¬ÂËœ Ã«ÂÂ°Ã¬ÂÂ´Ã­â€žÂ°Ã«Â¥Â¼ Ã¬Â â‚¬Ã¬Å¾Â¥Ã­â€¢ËœÃªÂ³Â  Ã¬Â â‚¬Ã¬Å¾Â¥Ã­â€“Ë†Ã«â€¹Â¤Ã«Å â€ Ã¬Ââ€˜Ã«â€¹ÂµÃ¬ÂÂ´ Ã¬ËœÂ¤Ã«Â©Â´ Ã­ÂÂ´Ã«ÂÂ¼Ã¬ÂÂ´Ã¬â€“Â¸Ã­Å Â¸Ã¬â€”ÂÃªÂ²Å’ Ã¬Â â€˜Ã¬â€ ÂÃ¬Ââ€ž Ã«â€¹Â¤Ã¬â€¹Å“ Ã­â€¢Â ÃªÂ²Æ’Ã¬Ââ€ž Ã¬â€¢Å’Ã«Â Â¤Ã¬Â¤â‚¬Ã«â€¹Â¤.
        bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA_REPLY, iClientH, false);  // ! Ã¬Â¹Â´Ã¬Å¡Â´Ã­Å’â€¦ Ã­â€¢ËœÃ¬Â§â‚¬ Ã¬â€¢Å Ã«Å â€Ã«â€¹Â¤.
        // !!!!
        m_pClientList[iClientH]->m_bIsOnServerChange = true;
        m_pClientList[iClientH]->m_bIsOnWaitingProcess = true;
        return;
    }
    else
    {
        // Ã«Â§Ë†Ã«Â²â€¢Ã¬â€”Â Ã¬ÂËœÃ­â€¢Å“ Ã­â€¦â€Ã«Â Ë†Ã­ÂÂ¬Ã­Å Â¸Ã¬ÂÂ´Ã«â€¹Â¤. 
        switch (pData[0])
        {
            case '0':
                // Forced Recall. 
                // Ã«Â Ë†Ã«Â²Â¨ Ã¬Â Å“Ã­â€¢Å“Ã¬ÂÂ´ Ã¬Å¾Ë†Ã«Å â€ Ã«Â§ÂµÃ¬â€”Â Ã«â€œÂ¤Ã¬â€“Â´ÃªÂ°â€Ã¬Ââ€žÃ«â€¢Å’ Ã¬ÂÂ¼Ã¬â€“Â´Ã«â€šÅ“Ã«â€¹Â¤. 
                memset(cTempMapName, 0, sizeof(cTempMapName));
                if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0)
                {
                    strcpy(cTempMapName, "default");
                }
                else if (memcmp(m_pClientList[iClientH]->m_cLocation, "arehunter", 9) == 0)
                {
                    strcpy(cTempMapName, "arefarm");
                }
                else if (memcmp(m_pClientList[iClientH]->m_cLocation, "elvhunter", 9) == 0)
                {
                    strcpy(cTempMapName, "elvfarm");
                }
                else strcpy(cTempMapName, m_pClientList[iClientH]->m_cLocation);

                // Crusade
                if ((strcmp(m_pClientList[iClientH]->m_cLockedMapName, "NONE") != 0) && (m_pClientList[iClientH]->m_iLockedMapTime > 0))
                {
                    // Ã­Å Â¹Ã¬Â â€¢ Ã«Â§ÂµÃ¬â€”Â ÃªÂ°â€“Ã­Å¾Å’ Ã¬Æ’ÂÃ­Æ’Å“Ã¬ÂÂ´Ã«â€¹Â¤. Ã­â€¦â€Ã«Â Ë†Ã­ÂÂ¬Ã­Å Â¸ Ã­Æ’â‚¬Ã¬ÂÂ¼Ã¬ÂËœ Ã¬Å“â€žÃ¬Â¹ËœÃ«Â¥Â¼ ÃªÂ°â€“Ã­Å¾Å’ Ã«Â§ÂµÃ¬Å“Â¼Ã«Â¡Å“ Ã«Â³â‚¬Ã­Ëœâ€¢Ã­â€¢Å“Ã«â€¹Â¤.
                    bIsLockedMapNotify = true;
                    memset(cTempMapName, 0, sizeof(cTempMapName));
                    strcpy(cTempMapName, m_pClientList[iClientH]->m_cLockedMapName);
                }

                for (i = 0; i < DEF_MAXMAPS; i++)
                    if (m_pMapList[i] != 0)
                    {
                        if (memcmp(m_pMapList[i]->m_cName, cTempMapName, 10) == 0)
                        {
                            // Ã­Ëœâ€žÃ¬Å¾Â¬ Ã¬â€žÅ“Ã«Â²â€žÃ¬â€”Â Ã­â€¦â€Ã«Â Ë†Ã­ÂÂ¬Ã­Å Â¸Ã­â€¢Â  Ã«Â§ÂµÃ¬ÂÂ´ Ã¬Â¡Â´Ã¬Å¾Â¬Ã­â€¢Å“Ã«â€¹Â¤.
                            GetMapInitialPoint(i, &m_pClientList[iClientH]->m_sX, &m_pClientList[iClientH]->m_sY, m_pClientList[iClientH]->m_cLocation);

                            m_pClientList[iClientH]->m_cMapIndex = i; // Ã«Â§Âµ Ã¬ÂÂ¸Ã«ÂÂ±Ã¬Å Â¤Ã«Â¥Â¼ Ã«Â°â€ÃªÂ¾Â¼Ã«â€¹Â¤.
                            memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof(m_pClientList[iClientH]->m_cMapName));
                            memcpy(m_pClientList[iClientH]->m_cMapName, cTempMapName, 10);  // Ã«Â§Âµ Ã¬ÂÂ´Ã«Â¦â€žÃ¬Ââ€ž Ã«Â°â€ÃªÂ¾Â¼Ã«â€¹Â¤.
                            goto RTH_NEXTSTEP;
                        }
                    }

                // Ã­Ëœâ€žÃ¬Å¾Â¬ Ã¬â€žÅ“Ã«Â²â€žÃ¬â€”Â Ã­â€¦â€Ã«Â Ë†Ã­ÂÂ¬Ã­Å Â¸ Ã­â€¢Â  Ã«Â§ÂµÃ¬ÂÂ´ Ã¬â€”â€ Ã«â€¹Â¤. Ã­ÂÂ´Ã«ÂÂ¼Ã¬ÂÂ´Ã¬â€“Â¸Ã­Å Â¸Ã«Å â€ Ã­Ëœâ€žÃ¬Å¾Â¬ Ã¬Â â€˜Ã¬â€ ÂÃ¬Ââ€ž Ã¬Â¢â€¦Ã«Â£Å’Ã­â€¢ËœÃªÂ³Â  Ã«â€¹Â¤Ã«Â¥Â¸ Ã¬â€žÅ“Ã«Â²â€žÃ«Â¡Å“ Ã¬Â â€˜Ã¬â€ ÂÃ¬Ââ€ž Ã¬â€¹Å“Ã«Ââ€žÃ­â€¢Â´Ã¬â€¢Â¼ Ã­â€¢Å“Ã«â€¹Â¤.
                m_pClientList[iClientH]->m_sX = -1;	  // Ã­â€¦â€Ã«Â Ë†Ã­ÂÂ¬Ã­Å Â¸ Ã¬Â¢Å’Ã­â€˜Å“Ã«Â¥Â¼ Ã¬Å¾â€¦Ã«Â Â¥Ã­â€¢Å“Ã«â€¹Â¤.
                m_pClientList[iClientH]->m_sY = -1;	  // -1Ã¬Ââ‚¬ InitialPointÃ«Â¥Â¼ Ã«Â§ÂÃ­â€¢Å“Ã«â€¹Â¤.

                memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof(m_pClientList[iClientH]->m_cMapName));
                memcpy(m_pClientList[iClientH]->m_cMapName, cTempMapName, 10);  // Ã«Â§Âµ Ã¬ÂÂ´Ã«Â¦â€žÃ¬Ââ€ž Ã«Â°â€ÃªÂ¾Â¼Ã«â€¹Â¤.

                // New 18/05/2004
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_CONFUSE,
                    m_pClientList[iClientH]->m_cMagicEffectStatus[DEF_MAGICTYPE_CONFUSE], 0, 0);
                SetSlateFlag(iClientH, DEF_NOTIFY_SLATECLEAR, false);

                // Ã­â€Å’Ã«Â Ë†Ã¬ÂÂ´Ã¬â€“Â´Ã¬ÂËœ Ã«ÂÂ°Ã¬ÂÂ´Ã­â€žÂ°Ã«Â¥Â¼ Ã¬Â â‚¬Ã¬Å¾Â¥Ã­â€¢ËœÃªÂ³Â  Ã¬Ââ€˜Ã«â€¹ÂµÃ¬Ââ€ž Ã«Â°â€ºÃ¬Ââ‚¬ Ã­â€ºâ€ž Ã¬Å¾Â¬Ã¬Â â€˜Ã¬â€ ÂÃ¬Ââ€ž Ã¬â€¢Å’Ã«Â Â¤Ã¬â€¢Â¼ Ã­â€¢Å“Ã«â€¹Â¤.
                bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA_REPLY, iClientH, false); // ! Ã¬Â¹Â´Ã¬Å¡Â´Ã­Å’â€¦ Ã­â€¢ËœÃ¬Â§â‚¬ Ã¬â€¢Å Ã«Å â€Ã«â€¹Â¤.

                m_pClientList[iClientH]->m_bIsOnServerChange = true;
                m_pClientList[iClientH]->m_bIsOnWaitingProcess = true;
                return;

            case '1':
                // Recall.Ã­â€Å’Ã«Â Ë†Ã¬ÂÂ´Ã¬â€“Â´ÃªÂ°â‚¬ Ã¬â€ Å’Ã¬â€ ÂÃ«ÂÅ“ Ã«Â§Ë†Ã¬Ââ€žÃ¬ÂËœ Ã¬Â§â€žÃ¬Å¾â€¦ Ã­ÂÂ¬Ã¬ÂÂ¸Ã­Å Â¸Ã«Â¡Å“ ÃªÂ°â€žÃ«â€¹Â¤.
                // Ã«Â§Å’Ã¬â€¢Â½ Ã«Â¶â‚¬Ã­â„¢Å“Ã¬Â¡Â´Ã¬ÂÂ´Ã«ÂÂ¼Ã«Â©Â´ Ã«Â¦Â¬Ã¬Â½Å“Ã«ÂËœÃ¬Â§â‚¬ Ã¬â€¢Å Ã«Å â€Ã«â€¹Â¤.
                // if (memcmp(m_pMapList[ m_pClientList[iClientH]->m_cMapIndex ]->m_cName, "resurr", 6) == 0) return;

                memset(cTempMapName, 0, sizeof(cTempMapName));
                if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0)
                {
                    strcpy(cTempMapName, "default");
                }
                else
                {
                    // v2.14 Â·Â¹ÂºÂ§ 80 Ã€ÃŒÃ‡ÃÂ´Ã‚ Â¸Â®Ã„ÃÂ½Ãƒ Â³Ã³Â°Ã¦ÃÃ¶Â·ÃŽ Â°Â£Â´Ã™.
                    if (m_pClientList[iClientH]->m_iLevel > 80)
                        if (memcmp(m_pClientList[iClientH]->m_cLocation, "are", 3) == 0)
                            strcpy(cTempMapName, "aresden");
                        else strcpy(cTempMapName, "elvine");
                    else
                    {
                        if (memcmp(m_pClientList[iClientH]->m_cLocation, "are", 3) == 0)
                            strcpy(cTempMapName, "arefarm");
                        else strcpy(cTempMapName, "elvfarm");
                    }
                }
                // Crusade
                if ((strcmp(m_pClientList[iClientH]->m_cLockedMapName, "NONE") != 0) && (m_pClientList[iClientH]->m_iLockedMapTime > 0))
                {
                    // Ã­Å Â¹Ã¬Â â€¢ Ã«Â§ÂµÃ¬â€”Â ÃªÂ°â€“Ã­Å¾Å’ Ã¬Æ’ÂÃ­Æ’Å“Ã¬ÂÂ´Ã«â€¹Â¤. Ã­â€¦â€Ã«Â Ë†Ã­ÂÂ¬Ã­Å Â¸ Ã­Æ’â‚¬Ã¬ÂÂ¼Ã¬ÂËœ Ã¬Å“â€žÃ¬Â¹ËœÃ«Â¥Â¼ ÃªÂ°â€“Ã­Å¾Å’ Ã«Â§ÂµÃ¬Å“Â¼Ã«Â¡Å“ Ã«Â³â‚¬Ã­Ëœâ€¢Ã­â€¢Å“Ã«â€¹Â¤.
                    bIsLockedMapNotify = true;
                    memset(cTempMapName, 0, sizeof(cTempMapName));
                    strcpy(cTempMapName, m_pClientList[iClientH]->m_cLockedMapName);
                }

                for (i = 0; i < DEF_MAXMAPS; i++)
                    if (m_pMapList[i] != 0)
                    {
                        if (memcmp(m_pMapList[i]->m_cName, cTempMapName, 10) == 0)
                        {
                            GetMapInitialPoint(i, &m_pClientList[iClientH]->m_sX, &m_pClientList[iClientH]->m_sY, m_pClientList[iClientH]->m_cLocation);

                            m_pClientList[iClientH]->m_cMapIndex = i;
                            memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof(m_pClientList[iClientH]->m_cMapName));
                            memcpy(m_pClientList[iClientH]->m_cMapName, m_pMapList[i]->m_cName, 10);
                            goto RTH_NEXTSTEP;
                        }
                    }

                m_pClientList[iClientH]->m_sX = -1;
                m_pClientList[iClientH]->m_sY = -1;

                memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof(m_pClientList[iClientH]->m_cMapName));
                memcpy(m_pClientList[iClientH]->m_cMapName, cTempMapName, 10);

                SendNotifyMsg(0, iClientH, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_CONFUSE,
                    m_pClientList[iClientH]->m_cMagicEffectStatus[DEF_MAGICTYPE_CONFUSE], 0, 0);
                SetSlateFlag(iClientH, DEF_NOTIFY_SLATECLEAR, false);

                bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA_REPLY, iClientH, false);

                m_pClientList[iClientH]->m_bIsOnServerChange = true;
                m_pClientList[iClientH]->m_bIsOnWaitingProcess = true;
                return;

            case '2':
                if ((strcmp(m_pClientList[iClientH]->m_cLockedMapName, "NONE") != 0) && (m_pClientList[iClientH]->m_iLockedMapTime > 0))
                {
                    dX = -1;
                    dY = -1;
                    bIsLockedMapNotify = true;
                    memset(cTempMapName, 0, sizeof(cTempMapName));
                    strcpy(cTempMapName, m_pClientList[iClientH]->m_cLockedMapName);
                }
                else
                {
                    memset(cTempMapName, 0, sizeof(cTempMapName));
                    strcpy(cTempMapName, cMapName);
                }

                cMapIndex = iGetMapIndex(cTempMapName);
                if (cMapIndex == -1)
                {
                    m_pClientList[iClientH]->m_sX = dX;
                    m_pClientList[iClientH]->m_sY = dY;

                    memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof(m_pClientList[iClientH]->m_cMapName));
                    memcpy(m_pClientList[iClientH]->m_cMapName, cTempMapName, 10);

                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_CONFUSE,
                        m_pClientList[iClientH]->m_cMagicEffectStatus[DEF_MAGICTYPE_CONFUSE], 0, 0);
                    SetSlateFlag(iClientH, DEF_NOTIFY_SLATECLEAR, false);

                    bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA_REPLY, iClientH, false);

                    m_pClientList[iClientH]->m_bIsOnServerChange = true;
                    m_pClientList[iClientH]->m_bIsOnWaitingProcess = true;
                    return;
                }

                m_pClientList[iClientH]->m_sX = dX;
                m_pClientList[iClientH]->m_sY = dY;
                m_pClientList[iClientH]->m_cMapIndex = cMapIndex;

                memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof(m_pClientList[iClientH]->m_cMapName));
                memcpy(m_pClientList[iClientH]->m_cMapName, m_pMapList[cMapIndex]->m_cName, 10);
                break;
        }
    }

    RTH_NEXTSTEP:;

    SetPlayingStatus(iClientH);
    int iTemp, iTemp2;
    iTemp = m_pClientList[iClientH]->m_iStatus;
    iTemp = 0x0FFFFFFF & iTemp;
    iTemp2 = iGetPlayerABSStatus(iClientH);
    iTemp = iTemp | (iTemp2 << 28);
    m_pClientList[iClientH]->m_iStatus = iTemp;

    if (bIsLockedMapNotify == true) SendNotifyMsg(0, iClientH, DEF_NOTIFY_LOCKEDMAP, m_pClientList[iClientH]->m_iLockedMapTime, 0, 0, m_pClientList[iClientH]->m_cLockedMapName);

    pBuffer = new char[DEF_MSGBUFFERSIZE + 1];
    memset(pBuffer, 0, DEF_MSGBUFFERSIZE + 1);

    dwp = (uint32_t *)(pBuffer + DEF_INDEX4_MSGID);
    *dwp = MSGID_RESPONSE_INITDATA;
    wp = (uint16_t *)(pBuffer + DEF_INDEX2_MSGTYPE);
    *wp = DEF_MSGTYPE_CONFIRM;

    cp = (char *)(pBuffer + DEF_INDEX2_MSGTYPE + 2);

    if (m_pClientList[iClientH]->m_bIsObserverMode == false)
        bGetEmptyPosition(&m_pClientList[iClientH]->m_sX, &m_pClientList[iClientH]->m_sY, m_pClientList[iClientH]->m_cMapIndex);
    else GetMapInitialPoint(m_pClientList[iClientH]->m_cMapIndex, &m_pClientList[iClientH]->m_sX, &m_pClientList[iClientH]->m_sY);

    sp = (short *)cp;
    *sp = iClientH;
    cp += 2;

    sp = (short *)cp;
    *sp = m_pClientList[iClientH]->m_sX - 14 - 5;
    cp += 2;

    sp = (short *)cp;
    *sp = m_pClientList[iClientH]->m_sY - 12 - 5;
    cp += 2;

    sp = (short *)cp;
    *sp = m_pClientList[iClientH]->m_sType;
    cp += 2;

    sp = (short *)cp;
    *sp = m_pClientList[iClientH]->m_sAppr1;
    cp += 2;

    sp = (short *)cp;
    *sp = m_pClientList[iClientH]->m_sAppr2;
    cp += 2;

    sp = (short *)cp;
    *sp = m_pClientList[iClientH]->m_sAppr3;
    cp += 2;

    sp = (short *)cp;
    *sp = m_pClientList[iClientH]->m_sAppr4;
    cp += 2;

    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iApprColor;
    cp += 4;

    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iStatus;
    cp += 4;

    memcpy(cp, m_pClientList[iClientH]->m_cMapName, 10);
    cp += 10;

    memcpy(cp, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, 10);
    cp += 10;

    if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFixedDayMode == true)
        *cp = 1;
    else *cp = m_cDayOrNight;
    cp++;

    if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFixedDayMode == true)
        *cp = 0;
    else *cp = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cWhetherStatus;
    cp++;

    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iContribution;
    cp += 4;

    if (m_pClientList[iClientH]->m_bIsObserverMode == false)
    {
        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetOwner(iClientH,
            DEF_OWNERTYPE_PLAYER,
            m_pClientList[iClientH]->m_sX,
            m_pClientList[iClientH]->m_sY);
    }

    *cp = (char)m_pClientList[iClientH]->m_bIsObserverMode;
    cp++;

    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iRating;
    cp += 4;

    ip = (int *)cp;
    *ip = m_pClientList[iClientH]->m_iHP;
    cp += 4;

    *cp = 0;
    cp++;

    iSize = iComposeInitMapData(m_pClientList[iClientH]->m_sX - 10, m_pClientList[iClientH]->m_sY - 7, iClientH, cp);
    iRet = m_pClientList[iClientH]->send_msg(pBuffer, 46 + iSize + 4 + 4 + 1 + 4 + 4 + 3);
    switch (iRet)
    {
        case DEF_XSOCKEVENT_QUENEFULL:
        case DEF_XSOCKEVENT_SOCKETERROR:
        case DEF_XSOCKEVENT_CRITICALERROR:
        case DEF_XSOCKEVENT_SOCKETCLOSED:
            // Ã«Â©â€Ã¬â€¹Å“Ã¬Â§â‚¬Ã«Â¥Â¼ Ã«Â³Â´Ã«â€šÂ¼Ã«â€¢Å’ Ã¬â€”ÂÃ«Å¸Â¬ÃªÂ°â‚¬ Ã«Â°Å“Ã¬Æ’ÂÃ­â€“Ë†Ã«â€¹Â¤Ã«Â©Â´ Ã¬Â Å“ÃªÂ±Â°Ã­â€¢Å“Ã«â€¹Â¤.
            DeleteClient(iClientH, true, true);
            if (pBuffer != 0) delete pBuffer;
            return;
    }

    if (pBuffer != 0) delete pBuffer;

    // Ã«â€¹Â¤Ã«Â¥Â¸ Ã­ÂÂ´Ã«ÂÂ¼Ã¬ÂÂ´Ã¬â€“Â¸Ã­Å Â¸Ã«â€œÂ¤Ã¬â€”ÂÃªÂ²Å’ Ã­â€Å’Ã«Â Ë†Ã¬ÂÂ´Ã¬â€“Â´ÃªÂ°â‚¬ Ã¬Æ’Ë†Ã«Â¡Å“Ã¬Å¡Â´ Ã¬Å“â€žÃ¬Â¹ËœÃ¬â€”Â Ã«â€šËœÃ­Æ’â‚¬Ã«â€šÂ¬Ã¬ÂÅ’Ã¬Ââ€ž Ã¬â€¢Å’Ã«Â¦Â°Ã«â€¹Â¤. 
    SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_LOG, DEF_MSGTYPE_CONFIRM, 0, 0, 0);

    if ((memcmp(m_pClientList[iClientH]->m_cLocation, "are", 3) == 0) &&
        (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "elvine", 6) == 0) &&
        (m_pClientList[iClientH]->m_iAdminUserLevel == 0))
    {

        m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();
        m_pClientList[iClientH]->m_bIsWarLocation = true;
        // New 17/05/2004
        CheckForceRecallTime(iClientH);
    }
    else if ((memcmp(m_pClientList[iClientH]->m_cLocation, "elv", 3) == 0) &&
        (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "aresden", 7) == 0) &&
        (m_pClientList[iClientH]->m_iAdminUserLevel == 0))
    {

        m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();
        m_pClientList[iClientH]->m_bIsWarLocation = true;

        // New 17/05/2004
        CheckForceRecallTime(iClientH);
    }
    else if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFightZone == true)
    {
        m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();
        m_pClientList[iClientH]->m_bIsWarLocation = true;
        SetForceRecallTime(iClientH);

        GetLocalTime(&SysTime);
        m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 2 * 20 * 60 - ((SysTime.wHour % 2) * 20 * 60 + SysTime.wMinute * 20) - 2 * 20;

    }
    else
    {
        m_pClientList[iClientH]->m_bIsWarLocation = false;
        m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 0;
        SetForceRecallTime(iClientH);
    }

    // No entering enemy shops
    int iMapside, iMapside2;

    iMapside = iGetMapLocationSide(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName);
    if (iMapside > 3) iMapside2 = iMapside - 2;
    else iMapside2 = iMapside;
    m_pClientList[iClientH]->m_bIsInsideOwnTown = false;
    if ((m_pClientList[iClientH]->m_cSide != iMapside2) && (iMapside != 0))
    {
        if ((iMapside <= 2) && (m_pClientList[iClientH]->m_iAdminUserLevel < 1))
        {
            if (m_pClientList[iClientH]->m_cSide != 0)
            {
                m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();
                m_pClientList[iClientH]->m_bIsWarLocation = true;
                m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 1;
                m_pClientList[iClientH]->m_bIsInsideOwnTown = true;
            }
        }
    }
    else
    {
        if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFightZone == true &&
            m_iFightzoneNoForceRecall == false &&
            m_pClientList[iClientH]->m_iAdminUserLevel == 0)
        {
            m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();
            m_pClientList[iClientH]->m_bIsWarLocation = true;
            GetLocalTime(&SysTime);
            m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 2 * 60 * 20 - ((SysTime.wHour % 2) * 20 * 60 + SysTime.wMinute * 20) - 2 * 20;
        }
        else
        {
            if (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "arejail", 7) == 0 ||
                memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "elvjail", 7) == 0)
            {
                if (m_pClientList[iClientH]->m_iAdminUserLevel == 0)
                {
                    m_pClientList[iClientH]->m_bIsWarLocation = true;
                    m_pClientList[iClientH]->m_dwWarBeginTime = timeGetTime();
                    if (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall == 0)
                        m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 100;
                    else if (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall > 100)
                        m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 100;
                }
            }
        }
    }

    // Ã¬Â¶â€ÃªÂ°â‚¬Ã«Â¡Å“ Ã«Â³Â´Ã«â€šÂ´Ã¬Â¤ËœÃ¬â€¢Â¼ Ã­â€¢Â  Ã¬Â â€¢Ã«Â³Â´Ã«Â¥Â¼ Ã«Â³Â´Ã«â€šÂ¸Ã«â€¹Â¤. v1.1
    SendNotifyMsg(0, iClientH, DEF_NOTIFY_SAFEATTACKMODE, 0, 0, 0, 0);
    // v1.3
    SendNotifyMsg(0, iClientH, DEF_NOTIFY_DOWNSKILLINDEXSET, m_pClientList[iClientH]->m_iDownSkillIndex, 0, 0, 0);
    // V1.3
    SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMPOSLIST, 0, 0, 0, 0);
    // v1.4 
    _SendQuestContents(iClientH);
    _CheckQuestEnvironment(iClientH);

    // v1.432
    if (m_pClientList[iClientH]->m_iSpecialAbilityTime == 0)
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SPECIALABILITYENABLED, 0, 0, 0, 0);

    // Crusade Ã­Å¡â€°Ã¬Â²Â ?Ã¬Â±Å’ Ã­Å¡â€¡Ã¬Â§Â¤Ã¬Â¨Å’Ã¬Â±Å’Ã¬Â©Å’Ã¬Â©Å’?Ã­Å¡â€˜Ã¬Â¨â€°Ã¬Â±â€¦ Ã¬Â¨ÂÃ¬Â±Â¨Ã¬Â¨â€°Ã¬Â±â€¦?Ã­Å¡â€˜Ã¬Â§Â¸Ã¬Â±Â  Ã­Å¡â€°Ã­Å¡â€žÃ¬Â¨Å’Ã¬Â¨â€?Ã­Å¡â€˜Ã¬Â©ÂÃ¬Â±Â¤?Ã­Å¡â€° CrusadeGUIDÃ¬Â§Â¸Ã¬Â§â€¢ 0 Ã­Å¡Å Ã¬Â§Å¡?Ã¬Â¨Ëœ Ã­Å¡â€°Ã¬Â²Â ?Ã¬Â±Å’Ã¬Â©â€Ã­Å¡â€™ Ã¬Â¨Ë†Ã­Å¡Â¢Ã¬Â¨ÂÃ¬Â§â„¢Ã¬Â¨Ë†Ã­Å¡Â¢Ã¬Â¨ÂÃ¬Â±â€¢ Ã­Å¡â€žÃ¬Â¨â‚¬?Ã¬Â©Â Ã­Å¡â€°Ã­Å¡ËœÃ¬Â¨Ë†Ã¬Â±Å’Ã¬Â¨â€°Ã­Å¡â€°Ã¬Â¨Ë†Ã­Å¡â€š Ã¬Â§Â¸Ã­Å¡â€™?Ã­Å¡â€˜Ã¬Â¨Ë†Ã­Å¡Â¢. Ã­Å¡â€¡Ã¬Â§Â¤Ã¬Â¨Å’Ã¬Â±Å’Ã¬Â©Å’Ã¬Â©Å’?Ã­Å¡â€˜Ã¬Â¨â€°Ã¬Â±â€¦ Ã¬Â©â€Ã¬Â§Â§Ã­Å¡â€°Ã­Å¡Ëœ Ã­Å¡â€žÃ­Å¡Å½Ã¬Â§Â¹Ã¬Â°Â½Ã­Å¡Å Ã¬Â§Â¯.
    if (m_bIsCrusadeMode == true)
    {
        if (m_pClientList[iClientH]->m_dwCrusadeGUID == 0)
        {
            // Ã­Å¡â€¡Ã¬Â§Â¤Ã¬Â¨Å’Ã¬Â±Å’Ã¬Â©Å’Ã¬Â©Å’?Ã­Å¡â€˜Ã¬Â¨â€°Ã¬Â±â€¦Ã¬Â¨ÂÃ¬Â±Â¨Ã¬Â¨â€°Ã¬Â±â€¦?Ã­Å¡â€˜Ã¬Â§Â¸Ã¬Â±Â  Ã­Å¡â€°Ã­Å¡â€žÃ¬Â¨Å’Ã¬Â¨â€?Ã­Å¡â€˜Ã¬Â©ÂÃ¬Â±Â¤?Ã­Å¡â€° GUIDÃ¬Â§Â¸Ã¬Â§â€¢ 0?Ã­Å¡â€˜Ã¬Â¨â€¹Ã¬Â²Å“ Ã¬Â§Â¸Ã­Å¡â€™?Ã¬Â¨Ëœ Ã­Å¡â€žÃ¬Â¨â‚¬?Ã¬Â©Â Ã­Å¡â€¡Ã¬Â§Â¤Ã¬Â¨Å’Ã¬Â±Å’Ã¬Â©Å’Ã¬Â©Å’?Ã­Å¡â€˜Ã¬Â¨â€°Ã¬Â±â€¦ Ã¬Â¨ÂÃ¬Â±Â¨Ã¬Â¨â€°Ã¬Â±â€¦Ã¬Â©â€Ã¬Â§â€¢ Ã¬Â¨â€°Ã¬Â±â€¢Ã¬Â©ÂÃ¬Â±Â¤Ã¬Â©â€Ã­Å¡Å¡Ã¬Â¨Ë†Ã­Å¡Â¢Ã¬Â¨Ë†Ã­Å¡â€š ?Ã­Å¡â€°Ã¬Â¨â€Ã­Å¡â€˜. Ã¬Â©â€Ã¬Â§Â§Ã­Å¡â€°Ã­Å¡Ëœ Ã­Å¡â€žÃ­Å¡Å½Ã¬Â§Â¹Ã¬Â°Â½Ã­Å¡Å Ã¬Â§Â¯.
            m_pClientList[iClientH]->m_iCrusadeDuty = 0;
            m_pClientList[iClientH]->m_iConstructionPoint = 0;
            m_pClientList[iClientH]->m_dwCrusadeGUID = m_dwCrusadeGUID;
        }
        else if (m_pClientList[iClientH]->m_dwCrusadeGUID != m_dwCrusadeGUID)
        {
            // Ã­Å¡â€¡Ã¬Â§Â¤Ã¬Â¨Å’Ã¬Â±Å’Ã¬Â©Å’Ã¬Â©Å’?Ã­Å¡â€˜Ã¬Â¨â€°Ã¬Â±â€¦Ã¬Â¨ÂÃ¬Â±Â¨Ã¬Â¨â€°Ã¬Â±â€¦?Ã­Å¡â€˜Ã¬Â§Â¸Ã¬Â±Â  Ã­Å¡â€°Ã­Å¡â€žÃ¬Â¨Å’Ã¬Â¨â€?Ã­Å¡â€˜Ã¬Â©ÂÃ¬Â±Â¤?Ã­Å¡â€° GUIDÃ¬Â§Â¸Ã¬Â§â€¢ Ã­Å¡â€°Ã¬Â²Â ?Ã¬Â±Å’ Ã­Å¡â€¡Ã¬Â§Â¤Ã¬Â¨Å’Ã¬Â±Å’Ã¬Â©Å’Ã¬Â©Å’?Ã­Å¡â€˜Ã¬Â¨â€°Ã¬Â±â€¦ Ã¬Â©ÂÃ­Å¡Ë†?Ã­Å¡â€˜Ã¬Â¨â€°Ã¬Â±Â¨Ã¬Â©â€Ã­Å¡â€™ Ã¬Â¨Ë†Ã­Å¡Â¢Ã¬Â¨ÂÃ¬Â§â„¢Ã¬Â¨Ë†Ã­Å¡Â¢Ã¬Â¨Ë†Ã­Å¡â€š Ã¬Â§Â¸Ã­Å¡â€™?Ã¬Â¨Ëœ ?Ã¬Â²Â¬Ã¬Â¨â€Ã¬Â²Â©Ã¬Â©â€Ã¬Â§â€¢ Ã¬Â¨â€Ã¬Â²Â¬Ã¬Â©ÂÃ¬Â±Â¤Ã­Å¡ÂÃ¬Â¨â‚¬Ã¬Â¨Ë†Ã¬Â²Â© Ã­Å¡â€¡Ã¬Â§Â¤Ã¬Â¨Å’Ã¬Â±Å’Ã¬Â©Å’Ã¬Â©Å’?Ã­Å¡â€˜Ã¬Â¨â€°Ã¬Â±â€¦?Ã­Å¡â€° Ã¬Â§Â¸Ã¬Â°Â¼Ã¬Â§Â¸Ã¬Â²Â¬?Ã­Å¡â€˜Ã¬Â¨Ë†Ã­Å¡Â¢.
            // ?Ã­Å¡â€˜Ã¬Â¨Å’Ã¬Â§Â¹ Ã¬Â§Â¸Ã¬Â±Ë†Ã¬Â©â€Ã¬Â±â„¢ ?Ã¬Â²Â´Ã¬Â§Â¸Ã¬Â²Â©Ã¬Â©â€Ã¬Â§â€¢ Ã¬Â¨â€°Ã¬Â²Â­Ã¬Â¨ÂÃ¬Â§Å“ Ã­Å¡Ë†Ã¬Â²Â¨Ã¬Â¨Â©Ã¬Â²Ëœ?Ã¬Â¨Â© Ã­Å¡â€°Ã­Å¡Ëœ Ã¬Â©Å’Ã¬Â²Â  Ã¬Â©ÂÃ¬Â²Â©Ã¬Â¨Ë†Ã­Å¡Â¢. Ã­Å¡â€žÃ­Å¡Å“Ã¬Â©Å’Ã­Å¡ËœÃ­Å¡â€°Ã­Å¡â€” Ã­Å¡â€¡Ã¬Â§Â¤Ã¬Â¨Å’Ã¬Â±Å’Ã¬Â©Å’Ã¬Â©Å’?Ã­Å¡â€˜Ã¬Â¨â€°Ã¬Â±â€¦ Ã¬Â¨ÂÃ¬Â±Â¨Ã¬Â¨â€°Ã¬Â±â€¦Ã¬Â§Â¸Ã¬Â§â€¢ Ã­Å¡ÂÃ¬Â©ÂÃ¬Â¨Å’Ã¬Â°Â¼Ã¬Â¨â€°Ã­Å¡Å  Ã¬Â¨Ë†Ã­Å¡Â¢?Ã¬Â©Â Ã¬Â¨Ë†Ã­Å¡Â¢?Ã¬Â©Â ?Ã¬Â²Â´Ã¬Â¨ÂÃ¬Â±â€¢?Ã¬Â²Â´?Ã­Å¡â€˜ Ã¬Â©ÂÃ­Å¡â€ž?Ã­Å¡Â¤Ã¬Â¨â€°Ã­Å¡â€°Ã¬Â§Â¹Ã¬Â°Â½ ?Ã¬Â²Â´Ã¬Â©â€Ã¬Â§â€¢ Ã­Å¡ÂÃ¬Â§â€“Ã¬Â©Å’Ã­Å¡â„¢?Ã¬Â¨Â© Ã­Å¡â€°Ã­Å¡Â Ã¬Â©ÂÃ­Å¡Â© Ã­Å¡Ë†Ã¬Â²Â¨Ã¬Â¨Â©Ã¬Â²Ëœ?Ã¬Â¨Â© Ã¬Â©ÂÃ¬Â±Âµ?Ã¬Â¨Â© Ã¬Â©Å’Ã¬Â²Â  ?Ã­Å¡Å“Ã¬Â¨Ë†Ã­Å¡Â¢.
            // ?Ã­Å¡â€˜?Ã¬Â²Â´Ã¬Â©â€Ã¬Â§â€¢ Ã­Å¡â€°Ã­Å¡ËœÃ¬Â¨Ë†Ã¬Â±Å’Ã¬Â¨â€°Ã­Å¡â€°Ã¬Â©ÂÃ¬Â²Â¬Ã¬Â¨Ë†Ã¬Â²Â© Ã¬Â©â€Ã¬Â§Â§Ã­Å¡â€°Ã­Å¡Ëœ, Ã¬Â§Â¸Ã­Å¡â€°Ã¬Â©Å’Ã¬Â¨â‚¬ Ã­Å¡Ë†Ã¬Â²Â¨?Ã­Å¡â€œÃ­Å¡Ë†Ã¬Â§Â°, ?Ã¬Â²Â´?Ã¬Â±Â¦ Ã¬Â§Â¸Ã¬Â²Â©Ã­Å¡â€°Ã¬Â±â€¦Ã¬Â¨â€°Ã¬Â¨â€° Ã­Å¡â€žÃ­Å¡Å½Ã¬Â§Â¹Ã¬Â°Â½Ã­Å¡Å Ã¬Â§Â¯.
            m_pClientList[iClientH]->m_iCrusadeDuty = 0;
            m_pClientList[iClientH]->m_iConstructionPoint = 0;
            m_pClientList[iClientH]->m_iWarContribution = 0;
            m_pClientList[iClientH]->m_dwCrusadeGUID = m_dwCrusadeGUID;
            // Ã­Å¡â€¡Ã¬Â§Â¤Ã¬Â¨Å’Ã¬Â±Å’Ã¬Â©Å’Ã¬Â©Å’?Ã­Å¡â€˜Ã¬Â¨â€°Ã¬Â±â€¦ GUIDÃ¬Â§Â¸Ã¬Â§â€¢ Ã¬Â¨Ë†Ã­Å¡Â¢Ã¬Â¨ÂÃ¬Â§â„¢Ã¬Â¨Ë†Ã­Å¡Â¢. Ã­Å¡Ë†Ã¬Â²Â¨Ã¬Â¨Â©Ã¬Â²Ëœ Ã¬Â¨ËœÃ­Å¡ËœÃ¬Â§Â¸Ã¬Â§â€¢.
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_CRUSADE, (uint32_t)m_bIsCrusadeMode, 0, 0, 0, -1);
        }
        m_pClientList[iClientH]->m_cVar = 1;
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_CRUSADE, (uint32_t)m_bIsCrusadeMode, m_pClientList[iClientH]->m_iCrusadeDuty, 0, 0);
    }
    else if (m_bIsHeldenianMode == true)
    {
        sSummonPoints = m_pClientList[iClientH]->m_iCharisma * 300;
        if (sSummonPoints > DEF_MAXSUMMONPOINTS) sSummonPoints = DEF_MAXSUMMONPOINTS;
        if (m_pClientList[iClientH]->m_dwHeldenianGUID == 0)
        {
            m_pClientList[iClientH]->m_dwHeldenianGUID = m_dwHeldenianGUID;
            m_pClientList[iClientH]->m_iConstructionPoint = sSummonPoints;
        }
        else if (m_pClientList[iClientH]->m_dwHeldenianGUID != m_dwHeldenianGUID)
        {
            m_pClientList[iClientH]->m_iConstructionPoint = sSummonPoints;
            m_pClientList[iClientH]->m_iWarContribution = 0;
            m_pClientList[iClientH]->m_dwHeldenianGUID = m_dwHeldenianGUID;
        }
        m_pClientList[iClientH]->m_cVar = 2;
        if (m_bIsHeldenianMode == true)
        {
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_HELDENIANTELEPORT, 0, 0, 0, 0);
        }
        if (m_bHeldenianInitiated == true)
        {
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_HELDENIANSTART, 0, 0, 0, 0);
        }
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[iClientH]->m_iConstructionPoint, m_pClientList[iClientH]->m_iWarContribution, 0, 0);
        UpdateHeldenianStatus();
    }
    else if ((m_pClientList[iClientH]->m_cVar == 1) && (m_pClientList[iClientH]->m_dwCrusadeGUID == m_dwCrusadeGUID))
    {
        m_pClientList[iClientH]->m_iCrusadeDuty = 0;
        m_pClientList[iClientH]->m_iConstructionPoint = 0;
    }
    else
    {
        if (m_pClientList[iClientH]->m_dwCrusadeGUID == m_dwCrusadeGUID)
        {
            if (m_pClientList[iClientH]->m_cVar == 1)
            {
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_CRUSADE, (uint32_t)m_bIsCrusadeMode, 0, 0, 0, -1);
            }
        }
        else
        {
            m_pClientList[iClientH]->m_dwCrusadeGUID = 0;
            m_pClientList[iClientH]->m_iWarContribution = 0;
            m_pClientList[iClientH]->m_dwCrusadeGUID = 0;
        }
    }

    // v1.42
    // 2002-7-4 Ã‚Â»ÃƒÂ§Ãƒâ€¦ÃƒÂµÃƒâ‚¬ÃƒÂ¥Ãƒâ‚¬Ãƒâ€¡ Ã‚Â°Ã‚Â¹Ã‚Â¼ÃƒÂ¶Ã‚Â¸Ã‚Â¦ Ã‚Â´ÃƒÆ’Ã‚Â¸Ã‚Â± Ã‚Â¼ÃƒÂ¶ Ãƒâ‚¬Ãƒâ€“Ã‚ÂµÃ‚ÂµÃ‚Â·ÃƒÂ 
    if (memcmp(m_pClientList[iClientH]->m_cMapName, "fight", 5) == 0)
    {
        copy_string(G_cTxt, "Char(%s)-Enter(%s) Observer(%d)", m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_cMapName, m_pClientList[iClientH]->m_bIsObserverMode);
        PutLogEventFileList(G_cTxt);
    }

    // Crusade
    SendNotifyMsg(0, iClientH, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[iClientH]->m_iConstructionPoint, m_pClientList[iClientH]->m_iWarContribution, 1, 0);

    // v2.15
    SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIZONITEMUPGRADELEFT, m_pClientList[iClientH]->m_iGizonItemUpgradeLeft, 0, 0, 0);

    if (m_bIsHeldenianMode == true)
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_HELDENIANTELEPORT, 0, 0, 0, 0);
        if (m_bHeldenianInitiated == true)
        {
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_HELDENIANSTART, 0, 0, 0, 0);
        }
        else
        {
            UpdateHeldenianStatus();
        }
    }

    if (m_pClientList[iClientH]->m_iQuest != 0)
    {
        cQuestRemain = (m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest]->m_iMaxCount - m_pClientList[iClientH]->m_iCurQuestCount);
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTCOUNTER, cQuestRemain, 0, 0, 0);
        _bCheckIsQuestCompleted(iClientH);
    }

}

void CGame::StateChangeHandler(int iClientH, char * pData, uint32_t dwMsgSize)
{
    char * cp, cStateChange1, cStateChange2, cStateChange3;
    char cStr, cVit, cDex, cInt, cMag, cChar;
    char cStateTxt[512];
    int iOldStr, iOldVit, iOldDex, iOldInt, iOldMag, iOldChar;
    int iTotalSetting = 0;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    if (m_pClientList[iClientH]->m_iGizonItemUpgradeLeft <= 0) return;

    cStr = cVit = cDex = cInt = cMag = cChar = 0;

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

    cStateChange1 = *cp;
    cp++;

    cStateChange2 = *cp;
    cp++;

    cStateChange3 = *cp;
    cp++;

    iOldStr = m_pClientList[iClientH]->m_iStr;
    iOldVit = m_pClientList[iClientH]->m_iVit;
    iOldDex = m_pClientList[iClientH]->m_iDex;
    iOldInt = m_pClientList[iClientH]->m_iInt;
    iOldMag = m_pClientList[iClientH]->m_iMag;
    iOldChar = m_pClientList[iClientH]->m_iCharisma;

    try
    {
        copy_string(G_cTxt, "(*) Char(%s) Str(%d) Vit(%d) Dex(%d) Int(%d) Mag(%d) Chr(%d) ", m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_iStr, m_pClientList[iClientH]->m_iVit, m_pClientList[iClientH]->m_iDex, m_pClientList[iClientH]->m_iInt, m_pClientList[iClientH]->m_iMag, m_pClientList[iClientH]->m_iCharisma);
        log->info(G_cTxt);
    }
    catch (...)
    {
    }

    if (!bChangeState(cStateChange1, &cStr, &cVit, &cDex, &cInt, &cMag, &cChar))
    {
        //Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¿Â¡Â¼Â­ Â°ÂªÃ€Â» Ã€ÃŸÂ¸Ã¸ ÂºÂ¸Â³Â»Â¿Ã‚ÂµÃ­...
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, 0);
        return;
    }
    if (!bChangeState(cStateChange2, &cStr, &cVit, &cDex, &cInt, &cMag, &cChar))
    {
        //Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¿Â¡Â¼Â­ Â°ÂªÃ€Â» Ã€ÃŸÂ¸Ã¸ ÂºÂ¸Â³Â»Â¿Ã‚ÂµÃ­...
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, 0);
        return;
    }
    if (!bChangeState(cStateChange3, &cStr, &cVit, &cDex, &cInt, &cMag, &cChar))
    {
        //Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¿Â¡Â¼Â­ Â°ÂªÃ€Â» Ã€ÃŸÂ¸Ã¸ ÂºÂ¸Â³Â»Â¿Ã‚ÂµÃ­...
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, 0);
        return;
    }

    try
    {
        copy_string(G_cTxt, "(*) Char(%s) St1(%d) St2(%d) St3(%d) Str(%d) Vit(%d) Dex(%d) Int(%d) Mag(%d) Chr(%d) ", m_pClientList[iClientH]->m_cCharName, cStateChange1, cStateChange2, cStateChange3, cStr, cVit, cDex, cInt, cMag, cChar);
        log->info(G_cTxt);
    }
    catch (...)
    {
    }

    //Â±Ã¦ÂµÃ¥ Â¸Â¶Â½ÂºÃ…ÃÂ¸Ã© cCharÂ¸Â¦ Â¸Ã¸ Â³Â»Â¸Â°Â´Ã™..
    if (m_pClientList[iClientH]->m_iGuildRank == 0)
    {
        if (m_pClientList[iClientH]->m_iCharisma - cChar < 20)
        {
            //Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¿Â¡Â¼Â­ Â°ÂªÃ€Â» Ã€ÃŸÂ¸Ã¸ ÂºÂ¸Â³Â»Â¿Ã‚ÂµÃ­...
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, 0);
            return;
        }
    }

    //Ã†Â¯Â¼ÂºÃ„Â¡ Â°ÂªÃ€ÃŒ Â·Â¹ÂºÂ§Â°Ãº Â¸Ã‚ÃÃ¶ Â¾ÃŠÂ´Ã™Â¸Ã© Â³Â»Â¸Â±Â¼Ã¶ Â¾Ã¸Â´Ã™..
    if (iOldStr + iOldVit + iOldDex + iOldInt + iOldMag + iOldChar != (179 * 3 + 70))
    {
        //Ã‡Ã˜Ã…Â·Ã€ÃŒÂ´Ã™... Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¿Â¡Â¼Â­ Â¸Ã¸ÂºÂ¸Â³Â»Â°Ã” ÂµÃ‡Â¾Ã® Ã€Ã–Â´Ã™ Â±Ã™ÂµÂ¥ Â¿Ã”Â´Ã™Â¸Ã© Ã‡Ã˜Ã…Â·...
        return;
    }


    if (cStr < 0 || cVit < 0 || cDex < 0 || cInt < 0 || cMag < 0 || cChar < 0
        || cStr + cVit + cDex + cInt + cMag + cChar != 3)
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, 0);
        return;
    }

    // Ã†Â¯Â¼ÂºÃ„Â¡Â°ÂªÃ€ÃŒ 10Ã€ÃŒÃ‡ÃÂ·ÃŽ Â¸Ã¸Â³Â»Â·Ã Â°Â£Â´Ã™... ÃÂ¦Ã‡Ã‘Ã€Â» Â³Ã‘Ã€Â»Â¼Ã¶ÂµÂµ Â¾Ã¸Â´Ã™...
    if ((m_pClientList[iClientH]->m_iStr - cStr > DEF_CHARPOINTLIMIT)
        || (m_pClientList[iClientH]->m_iStr - cStr < 10))
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, 0);
        return;
    }

    if ((m_pClientList[iClientH]->m_iDex - cDex > DEF_CHARPOINTLIMIT)
        || (m_pClientList[iClientH]->m_iDex - cDex < 10))
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, 0);
        return;
    }

    if ((m_pClientList[iClientH]->m_iInt - cInt > DEF_CHARPOINTLIMIT)
        || (m_pClientList[iClientH]->m_iInt - cInt < 10))
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, 0);
        return;
    }

    if ((m_pClientList[iClientH]->m_iVit - cVit > DEF_CHARPOINTLIMIT)
        || (m_pClientList[iClientH]->m_iVit - cVit < 10))
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, 0);
        return;
    }

    if ((m_pClientList[iClientH]->m_iMag - cMag > DEF_CHARPOINTLIMIT)
        || (m_pClientList[iClientH]->m_iMag - cMag < 10))
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, 0);
        return;
    }

    if ((m_pClientList[iClientH]->m_iCharisma - cChar > DEF_CHARPOINTLIMIT)
        || (m_pClientList[iClientH]->m_iCharisma - cChar < 10))
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_FAILED, 0, 0, 0, 0);
        return;
    }

    if (m_pClientList[iClientH]->m_iLU_Pool < 3) m_pClientList[iClientH]->m_iLU_Pool = 3;

    // Â¿Ã€Â·Ã¹Â°Â¡ Â¾Ã¸Ã€Â¸Â¸Ã© Â°ÂªÃ€Â» Ã‡Ã’Â´Ã§Ã‡Ã‘Â´Ã™.
    //m_pClientList[iClientH]->m_iLU_Pool += 3;

    m_pClientList[iClientH]->m_iGizonItemUpgradeLeft--;

    //HBest uuendus, hp,mp,sp lowering, skillcheck
    m_pClientList[iClientH]->m_iHP -= (cStr + 3 * cVit);
    m_pClientList[iClientH]->m_iMP -= (cInt + 2 * cMag);
    m_pClientList[iClientH]->m_iSP -= (2 * cStr);
    m_pClientList[iClientH]->m_iStr -= cStr;
    m_pClientList[iClientH]->m_iVit -= cVit;
    m_pClientList[iClientH]->m_iDex -= cDex;
    m_pClientList[iClientH]->m_iInt -= cInt;
    //2003-04-22Ã€Ã Â¸Â¶Â¹Ã½Ã€Â» Â»Ã¨ÃÂ¦ Â½ÃƒÃ…Â²Â´Ã™.... Ã€ÃŽÃ†Â®Â°Â¡ Â³Â»Â·ÃÂ°Â¡Â°Ã­ Â³ÂªÂ¼Â­ ÂµÂ¹Â¾Ã†Â°Â¡Â¾ÃŸ Ã‡Ã‘Â´Ã™...
    if (cInt > 0)
        bCheckMagicInt(iClientH);
    m_pClientList[iClientH]->m_iMag -= cMag;
    m_pClientList[iClientH]->m_iCharisma -= cChar;
    //SkillCheck(iClientH);

    try
    {
        ZeroMemory(cStateTxt, sizeof(cStateTxt));

        copy_string(cStateTxt, "STR(%d->%d)VIT(%d->%d)DEX(%d->%d)INT(%d->%d)MAG(%d->%d)CHARISMA(%d->%d)",
            iOldStr, m_pClientList[iClientH]->m_iStr,
            iOldVit, m_pClientList[iClientH]->m_iVit,
            iOldDex, m_pClientList[iClientH]->m_iDex,
            iOldInt, m_pClientList[iClientH]->m_iInt,
            iOldMag, m_pClientList[iClientH]->m_iMag,
            iOldChar, m_pClientList[iClientH]->m_iCharisma
        );
    }
    catch (...)
    {

    }

    //_bCustomLog(DEF_ITEMLOG_CUSTOM,iClientH,0,cStateTxt);

    //2003-04-22Ã€Ã Â½ÂºÃ…Â³Ã€Â» Â³Â»Â·ÃÂ°Â¡Â°Ã” Ã‡Ã‘Â´Ã™... 
    //bCheckSkillState(iClientH);

    //Â¼ÂºÂ°Ã¸..!!!
    SendNotifyMsg(0, iClientH, DEF_NOTIFY_STATECHANGE_SUCCESS, 0, 0, 0, 0);
}

void CGame::LevelUpSettingsHandler(int iClientH, char * pData, uint32_t dwMsgSize)
{
    char * cp, cStr, cVit, cDex, cInt, cMag, cChar;
    int iTotalSetting = 0;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    if (m_pClientList[iClientH]->m_iLU_Pool <= 0)
    {
        //Ã‡Ã˜Ã„Â¿Ã€ÃŽÂ°Â¡??
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SETTING_FAILED, 0, 0, 0, 0);
        return;
    }

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

    cStr = *cp;
    cp++;

    cVit = *cp;
    cp++;

    cDex = *cp;
    cp++;

    cInt = *cp;
    cp++;

    cMag = *cp;
    cp++;

    cChar = *cp;
    cp++;

    try
    {
        copy_string(G_cTxt, "(*) Char(%s) , STR (%s), DEX (%s), INT (%s), MAG (%s), VIT(%s), CHR (%s)", m_pClientList[iClientH]->m_cCharName, cStr, cDex, cInt, cMag, cVit, cChar);
        log->info(G_cTxt);
    }
    catch (...)
    {
    }

    //	if(m_pClientList[iClientH]->m_iLU_Pool < 3) {
    //		m_pClientList[iClientH]->m_iLU_Pool = 3;
    //	}


    if ((cStr + cVit + cDex + cInt + cMag + cChar) > m_pClientList[iClientH]->m_iLU_Pool)
    { // -3
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SETTING_FAILED, 0, 0, 0, 0);
        return;
    }

    // Level-Up SettingÂ°ÂªÂ¿Â¡ Â¿Ã€Â·Ã¹Â°Â¡ Ã€Ã–Â´Ã‚ÃÃ¶ Â°Ã‹Â»Ã§Ã‡Ã‘Â´Ã™.
    if ((m_pClientList[iClientH]->m_iStr + cStr > DEF_CHARPOINTLIMIT) || (cStr < 0))
        return;

    if ((m_pClientList[iClientH]->m_iDex + cDex > DEF_CHARPOINTLIMIT) || (cDex < 0))
        return;

    if ((m_pClientList[iClientH]->m_iInt + cInt > DEF_CHARPOINTLIMIT) || (cInt < 0))
        return;

    if ((m_pClientList[iClientH]->m_iVit + cVit > DEF_CHARPOINTLIMIT) || (cVit < 0))
        return;

    if ((m_pClientList[iClientH]->m_iMag + cMag > DEF_CHARPOINTLIMIT) || (cMag < 0))
        return;

    if ((m_pClientList[iClientH]->m_iCharisma + cChar > DEF_CHARPOINTLIMIT) || (cChar < 0))
        return;

    iTotalSetting = m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iDex + m_pClientList[iClientH]->m_iVit +
        m_pClientList[iClientH]->m_iInt + m_pClientList[iClientH]->m_iMag + m_pClientList[iClientH]->m_iCharisma;

    //(Â·Â¹ÂºÂ§ Ã†Â¯Â¼ÂºÂ°Âª + Â·Â¹ÂºÂ§Â¾Ã· Ã†Ã·Ã€ÃŽÃ†Â® > Â·Â¹ÂºÂ§Â¾Ã· Ã†Â¯Â¼ÂºÂ°Âª ÃÂ¤Â»Ã³Ã„Â¡)Â¸Ã© ÂºÃ±ÃÂ¤Â»Ã³Ã€ÃŒÂ´Ã™.. ÃƒÂ³Â¸Â® ÂºÃ’Â°Â¡.. Â·Â¹ÂºÂ§Â¾Ã· Ã†Ã·Ã€ÃŽÃ†Â®Â¸Â¦ ÃÂ¤Â»Ã³Ã„Â¡Â·ÃŽ Â¸Â¶ÃƒÃŸÂ°Ã­ ÃƒÂ³Â¸Â® ÂºÃ’Â°Â¡..
    if (iTotalSetting + m_pClientList[iClientH]->m_iLU_Pool - 3 > ((m_pClientList[iClientH]->m_iLevel - 1) * 3 + 70))
    {
        int bobdole;
        bobdole = 3;
        m_pClientList[iClientH]->m_iLU_Pool = /*m_cLU_Str ÃƒÃŠÂ±Ã¢Â°Âª*/3 + (m_pClientList[iClientH]->m_iLevel - 1) * 3 + 70 - iTotalSetting;

        //iTotalSettingÂ°ÂªÃ€ÃŒ Ã€ÃŸÂ¸Ã¸ÂµÃˆ Â°Ã¦Â¿Ã¬Â´Ã™...
        if (m_pClientList[iClientH]->m_iLU_Pool < 3)
            m_pClientList[iClientH]->m_iLU_Pool = 3;
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SETTING_FAILED, 0, 0, 0, 0);
        return;
    }

    //(Â·Â¹ÂºÂ§ Ã†Â¯Â¼ÂºÂ°Âª + Â·Â¹ÂºÂ§Â¾Ã· Â½ÃƒÃ…Â³ Ã†Ã·Ã€ÃŽÃ†Â® ÃD > Â·Â¹ÂºÂ§Â¾Ã· Ã†Â¯Â¼ÂºÂ°Âª ÃÂ¤Â»Ã³Ã„Â¡)Ã€ÃŒÂ¸Ã© ÃƒÂ³Â¸Â® ÂºÃ’Â°Â¡..
    if (iTotalSetting + (cStr + cVit + cDex + cInt + cMag + cChar)
        > ((m_pClientList[iClientH]->m_iLevel - 1) * 3 + 70))
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SETTING_FAILED, 0, 0, 0, 0);
        return;
    }

    m_pClientList[iClientH]->m_iLU_Pool = m_pClientList[iClientH]->m_iLU_Pool - (cStr + cVit + cDex + cInt + cMag + cChar);

    m_pClientList[iClientH]->m_iStr += cStr;
    m_pClientList[iClientH]->m_iVit += cVit;
    m_pClientList[iClientH]->m_iDex += cDex;
    m_pClientList[iClientH]->m_iInt += cInt;
    m_pClientList[iClientH]->m_iMag += cMag;
    m_pClientList[iClientH]->m_iCharisma += cChar;

    SendNotifyMsg(0, iClientH, DEF_NOTIFY_SETTING_SUCCESS, 0, 0, 0, 0);

}

void CGame::FightzoneReserveHandler(int iClientH, char * pData, uint32_t dwMsgSize)
{
    char cData[100];
    int iFightzoneNum, * ip, iEnableReserveTime;
    uint32_t * dwp, dwGoldCount;
    uint16_t * wp, wResult;
    int     iRet, iResult = 1, iCannotReserveDay;
    SYSTEMTIME SysTime;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

    GetLocalTime(&SysTime);

    // Â¿Â¹Â¾Ã  Â°Â¡Â´Ã‰Ã‡Ã‘ Â½ÃƒÂ°Â£ : ÂµÃŽÂ½ÃƒÂ°Â£ Â°Â£Â°ÃÃ€Â¸Â·ÃŽ Â¿Â¹Â¾Ã Ã€ÃŒ Â°Â¡Â´Ã‰Ã‡ÃÂ¸Ã§ Â»Ã§Â¿Ã«Â¿ÃÂ·Ã¡ 5ÂºÃÃ€Ã¼Â¿Â¡Â´Ã‚ Â¿Â¹Â¾Ã Ã€ÃŒ ÂºÃ’Â°Â¡Â´Ã‰Ã‡ÃÂ´Ã™.
    iEnableReserveTime = 2 * 20 * 60 - ((SysTime.wHour % 2) * 20 * 60 + SysTime.wMinute * 20) - 5 * 20;

    dwGoldCount = dwGetItemCount(iClientH, "Gold");

    ip = (int *)(pData + DEF_INDEX2_MSGTYPE + 2);
    // Â¿Â¹Â¾Ã Ã€Â» Â¿Ã¸Ã‡ÃÂ´Ã‚ Â»Ã§Ã…ÃµÃ€Ã¥ Â¹Ã¸ÃˆÂ£Â¸Â¦ Â¹ÃžÂ´Ã‚Â´Ã™.
    iFightzoneNum = *ip;

    // Ã€ÃŸÂ¸Ã¸ÂµÃˆ fightzone Â¹Ã¸ÃˆÂ£Â¸Â¦ Â°Ã‰Â·Â¯Â³Â½Â´Ã™.
    if ((iFightzoneNum < 1) || (iFightzoneNum > DEF_MAXFIGHTZONE)) return;

    // Â»Ã§Ã…ÃµÃ€Ã¥Ã€ÃŒ ÃÃŸÂºÂ¹ÂµÃ‡ÃÃ¶ Â¾ÃŠÂ°Ã” Ã‡ÃÂ±Ã¢ Ã€Â§Ã‡Ã˜ Â¿Ã¤Ã€ÃÂ¿Â¡ ÂµÃ»Â¶Ã³ Â»Ã§Â¿Ã«Ã‡Ã’Â¼Ã¶ Ã€Ã–Â´Ã‚ Â»Ã§Ã…ÃµÃ€Ã¥Ã€ÃŒ Â´Ã™Â¸Â£Â°Ã” Ã‡ÃÂ±Ã¢ Ã€Â§Ã‡Ã‘ ÂºÂ¯Â¼Ã¶Â´Ã™.
    // ÃˆÂ¦Â¼Ã¶Â³Â¯Â¿Â¡Â´Ã‚ Â¾Ã†Â·Â¹Â½ÂºÂµÂ§ 2 4 6 8 Â¿Â¤Â¹Ã™Ã€ÃŽÃ€ÃŒ 1 3 5 7  Â»Ã§Ã…ÃµÃ€Ã¥Ã€ÃŒ Â»Ã§Â¿Ã«Â°Â¡Â´Ã‰Ã‡ÃÂ´Ã™ 
    //             ex) 1Ã€ÃÃ€ÃŽÂ°Ã¦Â¿Ã¬ => {1 + 1 (Â¾Ã†Â·Â¹Â½ÂºÂµÂ§) + 1 (Â»Ã§Ã…ÃµÃ€Ã¥ Â¹Ã¸ÃˆÂ£ )} %2 == 1 Ã€ÃŒÂ¹Ã‡Â·ÃŽ 
    //                            Â¾Ã†Â·Â¹Â½ÂºÂµÂ§Ã€Âº ÃˆÂ¦Â¼Ã¶Â³Â¯ ÃˆÂ¦Â¼Ã¶ Â»Ã§Ã…ÃµÃ€Ã¥Ã€Â»  Â¿Â¹Â¾Ã  Ã‡Ã’Â¼Ã¶ Â¾Ã¸Â´Ã™. 

    iCannotReserveDay = (SysTime.wDay + m_pClientList[iClientH]->m_cSide + iFightzoneNum) % 2;
    if (iEnableReserveTime <= 0)
    {
        wResult = DEF_MSGTYPE_REJECT;
        iResult = 0;
    }
    else if (m_iFightZoneReserve[iFightzoneNum - 1] != 0)
    {
        wResult = DEF_MSGTYPE_REJECT;
        iResult = -1;
    }
    else if (dwGoldCount < 1500)
    {
        wResult = DEF_MSGTYPE_REJECT;
        iResult = -2;
    }
    else if (iCannotReserveDay)
    {
        wResult = DEF_MSGTYPE_REJECT;
        iResult = -3;
    }
    else if (m_pClientList[iClientH]->m_iFightzoneNumber != 0)
    {
        wResult = DEF_MSGTYPE_REJECT;
        iResult = -4;
    }
    else
    {
        wResult = DEF_MSGTYPE_CONFIRM;

        SetItemCount(iClientH, "Gold", dwGoldCount - 1500);
        iCalcTotalWeight(iClientH);

        m_iFightZoneReserve[iFightzoneNum - 1] = iClientH;

        m_pClientList[iClientH]->m_iFightzoneNumber = iFightzoneNum;
        m_pClientList[iClientH]->m_iReserveTime = SysTime.wMonth * 10000 + SysTime.wDay * 100 + SysTime.wHour;

        if (SysTime.wHour % 2)	m_pClientList[iClientH]->m_iReserveTime += 1;
        else					m_pClientList[iClientH]->m_iReserveTime += 2;
        log->info("(*) Reserve FIGHTZONETICKET : Char({}) TICKENUMBER ({})", m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_iReserveTime);

        m_pClientList[iClientH]->m_iFightZoneTicketNumber = 50;
        iResult = 1;
    }

    memset(cData, 0, sizeof(cData));

    dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
    *dwp = MSGID_RESPONSE_FIGHTZONE_RESERVE;

    wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
    *wp = wResult;

    ip = (int *)(cData + DEF_INDEX2_MSGTYPE + 2);
    *ip = iResult;
    ip += 4;

    iRet = m_pClientList[iClientH]->send_msg(cData, 10);
}

void CGame::RequestCivilRightHandler(int iClientH, char * pData)
{
    char * cp, cData[100]{};
    uint32_t * dwp;
    uint16_t * wp, wResult;
    int  iRet;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    if ((m_bAdminSecurity == true) && (m_pClientList[iClientH]->m_iAdminUserLevel > 0)) return;

    if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) != 0) wResult = 0;
    else wResult = 1;

    if (m_pClientList[iClientH]->m_iLevel < 5) wResult = 0;

    if (wResult == 1)
    {
        memset(m_pClientList[iClientH]->m_cLocation, 0, sizeof(m_pClientList[iClientH]->m_cLocation));
        strcpy(m_pClientList[iClientH]->m_cLocation, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName);
    }

    if (memcmp(m_pClientList[iClientH]->m_cLocation, "are", 3) == 0)
        m_pClientList[iClientH]->m_cSide = 1;

    if (memcmp(m_pClientList[iClientH]->m_cLocation, "elv", 3) == 0)
        m_pClientList[iClientH]->m_cSide = 2;

    dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
    *dwp = MSGID_RESPONSE_CIVILRIGHT;
    wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
    *wp = wResult;

    cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);
    memcpy(cp, m_pClientList[iClientH]->m_cLocation, 10);
    cp += 10;

    iRet = m_pClientList[iClientH]->send_msg(cData, 16);

    SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
}

void CGame::RequestRetrieveItemHandler(int iClientH, char * pData)
{
    char * cp, cBankItemIndex, cMsg[100];
    int i, j, iRet, iItemWeight;
    uint32_t * dwp;
    uint16_t * wp;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);
    cBankItemIndex = *cp;
    //wh remove
    //if (m_pClientList[iClientH]->m_bIsInsideWarehouse == false) return;

    if ((cBankItemIndex < 0) || (cBankItemIndex >= DEF_MAXBANKITEMS)) return;
    if (m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex] == 0)
    {
        // Â¿Ã€Â·Ã¹Â´Ã™. 
        memset(cMsg, 0, sizeof(cMsg));

        dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
        *dwp = MSGID_RESPONSE_RETRIEVEITEM;
        wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
        *wp = DEF_MSGTYPE_REJECT;

        iRet = m_pClientList[iClientH]->send_msg(cMsg, 8);
    }
    else
    {
        /*
        if ( (m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
             (m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW) ) {
            //iItemWeight = m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex]->m_wWeight * m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex]->m_dwCount;
            iItemWeight = iGetItemWeight(m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex], m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex]->m_dwCount);
        }
        else iItemWeight = iGetItemWeight(m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex], 1); //m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex]->m_wWeight;
        */
        iItemWeight = iGetItemWeight(m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex], m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex]->m_dwCount);

        if ((iItemWeight + m_pClientList[iClientH]->m_iCurWeightLoad) > _iCalcMaxLoad(iClientH))
        {
            memset(cMsg, 0, sizeof(cMsg));
.
            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = MSGID_NOTIFY;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;

            iRet = m_pClientList[iClientH]->send_msg(cMsg, 6);
            return;
        }

        if ((m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
            (m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW))
        {	
            for (i = 0; i < DEF_MAXITEMS; i++)
                if ((m_pClientList[iClientH]->m_pItemList[i] != 0) &&
                    (m_pClientList[iClientH]->m_pItemList[i]->m_cItemType == m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex]->m_cItemType) &&
                    (memcmp(m_pClientList[iClientH]->m_pItemList[i]->m_cName, m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex]->m_cName, 20) == 0))
                {
                    SetItemCount(iClientH, i, m_pClientList[iClientH]->m_pItemList[i]->m_dwCount + m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex]->m_dwCount);
 
                    delete m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex];
                    m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex] = 0;

                    for (j = 0; j <= DEF_MAXBANKITEMS - 2; j++)
                    {
                        if ((m_pClientList[iClientH]->m_pItemInBankList[j + 1] != 0) && (m_pClientList[iClientH]->m_pItemInBankList[j] == 0))
                        {
                            m_pClientList[iClientH]->m_pItemInBankList[j] = m_pClientList[iClientH]->m_pItemInBankList[j + 1];

                            m_pClientList[iClientH]->m_pItemInBankList[j + 1] = 0;
                        }
                    }

                    memset(cMsg, 0, sizeof(cMsg));

                    dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
                    *dwp = MSGID_RESPONSE_RETRIEVEITEM;
                    wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
                    *wp = DEF_MSGTYPE_CONFIRM;

                    cp = (char *)(cMsg + DEF_INDEX2_MSGTYPE + 2);
                    *cp = cBankItemIndex;
                    cp++;
                    *cp = i;
                    cp++;

                    iCalcTotalWeight(iClientH);
                    m_pClientList[iClientH]->m_cArrowIndex = _iGetArrowItemIndex(iClientH);

                    iRet = m_pClientList[iClientH]->send_msg(cMsg, 8);
                    return;
                }

            goto RRIH_NOQUANTITY;
        }
        else
        {
            RRIH_NOQUANTITY:;
            for (i = 0; i < DEF_MAXITEMS; i++)
                if (m_pClientList[iClientH]->m_pItemList[i] == 0)
                {
                    m_pClientList[iClientH]->m_pItemList[i] = m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex];
                    m_pClientList[iClientH]->m_ItemPosList[i].x = 40;
                    m_pClientList[iClientH]->m_ItemPosList[i].y = 30;

                    m_pClientList[iClientH]->m_bIsItemEquipped[i] = false;

                    m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex] = 0;

                    for (j = 0; j <= DEF_MAXBANKITEMS - 2; j++)
                    {
                        if ((m_pClientList[iClientH]->m_pItemInBankList[j + 1] != 0) && (m_pClientList[iClientH]->m_pItemInBankList[j] == 0))
                        {
                            m_pClientList[iClientH]->m_pItemInBankList[j] = m_pClientList[iClientH]->m_pItemInBankList[j + 1];

                            m_pClientList[iClientH]->m_pItemInBankList[j + 1] = 0;
                        }
                    }

                    memset(cMsg, 0, sizeof(cMsg));

                    dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
                    *dwp = MSGID_RESPONSE_RETRIEVEITEM;
                    wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
                    *wp = DEF_MSGTYPE_CONFIRM;

                    cp = (char *)(cMsg + DEF_INDEX2_MSGTYPE + 2);
                    *cp = cBankItemIndex;
                    cp++;
                    *cp = i;
                    cp++;

                    iCalcTotalWeight(iClientH);

                    m_pClientList[iClientH]->m_cArrowIndex = _iGetArrowItemIndex(iClientH);

                    iRet = m_pClientList[iClientH]->send_msg(cMsg, 8);
                    return;
                }
            memset(cMsg, 0, sizeof(cMsg));

            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = MSGID_RESPONSE_RETRIEVEITEM;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = DEF_MSGTYPE_REJECT;

            iRet = m_pClientList[iClientH]->send_msg(cMsg, 8);
        }
    }
}

void CGame::PK_KillRewardHandler(short sAttackerH, short sVictumH)
{
    if (m_pClientList[sAttackerH] == 0) return;
    if (m_pClientList[sVictumH] == 0)   return;

    _bPKLog(DEF_PKLOG_BYPLAYER, sAttackerH, sVictumH, 0);

    if (m_pClientList[sAttackerH]->m_iPKCount != 0)
    {

    }
    else
    {
        m_pClientList[sAttackerH]->m_iRewardGold += iGetExpLevel(m_pClientList[sVictumH]->m_iExp) * 3;


        if (m_pClientList[sAttackerH]->m_iRewardGold > DEF_MAXREWARDGOLD)
            m_pClientList[sAttackerH]->m_iRewardGold = DEF_MAXREWARDGOLD;
        if (m_pClientList[sAttackerH]->m_iRewardGold < 0)
            m_pClientList[sAttackerH]->m_iRewardGold = 0;

        SendNotifyMsg(0, sAttackerH, DEF_NOTIFY_PKCAPTURED, m_pClientList[sVictumH]->m_iPKCount, m_pClientList[sVictumH]->m_iLevel, 0, m_pClientList[sVictumH]->m_cCharName);
    }
}

void CGame::EnemyKillRewardHandler(int iAttackerH, int iClientH)
{
    // enemy-kill-mode = 1 | 0
    // if m_bEnemyKillMode is true than death match mode

    // DEATHMATCH MODE:
    // Aresden kills Elvine in Aresden and gets EK
    // Elvine kills Aresden in Aresden and gets an EK 
    // Elvine kills Aresden in Elvine and gets an EK
    // Aresden kills Elvine in Elvine and gets an EK

    // CLASSIC MODE:
    // Aresden kills Elvine in Aresden and gets EK
    // Elvine kills Aresden in Aresden and doesnt get an EK 
    // Elvine kills Aresden in Elvine and gets an EK
    // Aresden kills Elvine in Elvine and doesnt get an EK

    int iRewardExp, iEK_Level;

    // if the attacker doesnt exist no result
    if (m_pClientList[iAttackerH] == 0) return;
    // if the target doesnt exist no result
    if (m_pClientList[iClientH] == nullptr)   return;

    // Log the EK (killed by enemy)
    _bPKLog(DEF_PKLOG_BYENERMY, iAttackerH, iClientH, 0);

    // set EK level to 30
    iEK_Level = 30;
    // if attacker's level is greater than 80, set ek level to 80
    if (m_pClientList[iAttackerH]->m_iLevel >= 80) iEK_Level = 80;
    // check if attacker level is less than or equal to max level
    if (m_pClientList[iAttackerH]->m_iLevel >= m_iPlayerMaxLevel)
    {
        // if the
        if (iGetExpLevel(m_pClientList[iClientH]->m_iExp) >= iEK_Level)
        {
            // ÃˆÃ±Â»Ã½Ã€ÃšÃ€Ã‡ Â·Â¹ÂºÂ§Ã€ÃŒ 80Ã€ÃŒÂ»Ã³Ã€ÃŒÂ°Ã­
            if ((memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[iClientH]->m_cMapName, 10) != 0)
                && (m_bEnemyKillMode == false))
            {
                // ÃˆÃ±Â»Ã½Ã€ÃšÂ°Â¡ ÃÃ—Ã€Âº Â°Ã·Ã€ÃŒ Ã€ÃšÂ½Ã…Ã€Ã‡ Â¸Â¶Ã€Â»Ã€ÃŒ Â¾Ã†Â´ÃÂ¶Ã³Â¸Ã© EKÂ·ÃŽ Ã€ÃŽÃÂ¤ 
                m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
            }

            if (m_bEnemyKillMode == true)
            {
                // ÃˆÃ±Â»Ã½Ã€ÃšÂ°Â¡ ÃÃ—Ã€Âº Â°Ã·Ã€ÃŒ Ã€ÃšÂ½Ã…Ã€Ã‡ Â¸Â¶Ã€Â»Ã€ÃŒ Â¾Ã†Â´ÃÂ¶Ã³Â¸Ã© EKÂ·ÃŽ Ã€ÃŽÃÂ¤ 
                m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
            }
        }
        // Ã†Ã·Â»Ã³Â±Ã Â´Â©Ã€Ã» 
        m_pClientList[iAttackerH]->m_iRewardGold += iDice(1, (iGetExpLevel(m_pClientList[iClientH]->m_iExp)));
        if (m_pClientList[iAttackerH]->m_iRewardGold > DEF_MAXREWARDGOLD)
            m_pClientList[iAttackerH]->m_iRewardGold = DEF_MAXREWARDGOLD;
        if (m_pClientList[iAttackerH]->m_iRewardGold < 0)
            m_pClientList[iAttackerH]->m_iRewardGold = 0;

        // Ã€Ã»Ã€Â» Ã€Ã¢Â¾Ã’Â´Ã™Â´Ã‚ Â¸ÃžÂ½ÃƒÃÃ¶ ÂºÂ¸Â³Â¿ 
        SendNotifyMsg(0, iAttackerH, DEF_NOTIFY_ENEMYKILLREWARD, iClientH, 0, 0, 0);
        return;
    }

    if (m_pClientList[iAttackerH]->m_iPKCount != 0)
    {
        // Ã€Ã»Ã€Â» Ã€Ã¢Ã€Âº Â»Ã§Â¶Ã·Ã€ÃŒ PKÂ¸Ã© Â¾Ã†Â¹Â«Â·Â± ÂµÃ¦Ã€ÃŒ Â¾Ã¸Â´Ã™.
    }
    else
    {
        // ÃˆÃ±Â»Ã½Ã€ÃšÃ€Ã‡ Â½ÃƒÂ¹ÃŽ, Â±Ã¦ÂµÃ¥Â¿Ã¸ Â¿Â©ÂºÃŽÂ¿Â¡ ÂµÃ»Â¶Ã³ Ã†Ã·Â»Ã³Ã€ÃŒ Â´ÃžÂ¶Ã³ÃÃ¸Â´Ã™. 
        if (m_pClientList[iClientH]->m_iGuildRank == -1)
        {
            // Â±Ã¦ÂµÃ¥Â¿Ã¸Ã€ÃŒ Â¾Ã†Â´ÃÂ¹Ã‡Â·ÃŽ Â½ÃƒÂ¹ÃŽ. (Â¿Â©Ã‡Ã Ã€ÃšÃ€Ã‡ Â°Ã¦Â¿Ã¬Â¿Â¡Â´Ã‚ Ã€ÃŒ Ã‡Ã”Â¼Ã¶Â°Â¡ ÃˆÂ£ÃƒÃ¢ÂµÃ‡ÃÃ¶ Â¾ÃŠÃ€Â¸Â¹Ã‡Â·ÃŽ)
            // v2.15 Ã€Ã»Ã€Â» ÃÃ—Â¿Â´Ã€Â»Â¶Â§ Ã†Ã²Â±Ã•Â°Ã¦Ã‡Ã¨Ã„Â¡Â¸Â¦ Â¾Ã²Â±Ã¢Ã€Â§Ã‡Ã˜ 
            iRewardExp = (iDice(3, (3 * iGetExpLevel(m_pClientList[iClientH]->m_iExp))) + iGetExpLevel(m_pClientList[iClientH]->m_iExp)) / 3;

            if (m_bIsCrusadeMode == true)
            {
                // Ã…Â©Â·Ã§Â¼Â¼Ã€ÃŒÂµÃ¥ Â¸Ã°ÂµÃ¥Â¶Ã³Â¸Ã© Â°Ã¦Ã‡Ã¨Ã„Â¡Ã€Ã‡ (1/3)*3Ã€Â» Â¸Ã•Ã€Ãº ÃÃ–Â°Ã­ Â³ÂªÂ¸Ã“ÃÃ¶Â´Ã‚ 6Â¹Ã¨Â·ÃŽ Ã€Ã¼Ã€Ã¯ Â°Ã¸Ã‡Ã¥ÂµÂµÂ¿Â¡ Â´Â©Ã€Ã» 
                m_pClientList[iAttackerH]->m_iExp += (iRewardExp / 3) * 4;
                m_pClientList[iAttackerH]->m_iWarContribution += (iRewardExp - (iRewardExp / 3)) * 12;

                if (m_pClientList[iAttackerH]->m_iWarContribution > DEF_MAXWARCONTRIBUTION)
                    m_pClientList[iAttackerH]->m_iWarContribution = DEF_MAXWARCONTRIBUTION;

                m_pClientList[iAttackerH]->m_iConstructionPoint += m_pClientList[iClientH]->m_iLevel / 2;

                if (m_pClientList[iAttackerH]->m_iConstructionPoint > DEF_MAXCONSTRUCTIONPOINT)
                    m_pClientList[iAttackerH]->m_iConstructionPoint = DEF_MAXCONSTRUCTIONPOINT;

                //testcode
                copy_string(G_cTxt, "Enemy Player Killed by Player! Construction: +%d WarContribution +%d", m_pClientList[iClientH]->m_iLevel / 2, (iRewardExp - (iRewardExp / 3)) * 6);
                log->info(G_cTxt);

                // Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
                SendNotifyMsg(0, iAttackerH, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[iAttackerH]->m_iConstructionPoint, m_pClientList[iAttackerH]->m_iWarContribution, 0, 0);

                // ÃˆÃ±Â»Ã½Ã€ÃšÃ€Ã‡ Â·Â¹ÂºÂ§Ã€ÃŒ 80 Ã€ÃŒÂ»Ã³Ã€ÃŽ Â°Ã¦Â¿Ã¬ Enemy Kill countÂ¸Â¦ Â¿ÃƒÂ¸Â°Â´Ã™.
                if (iGetExpLevel(m_pClientList[iClientH]->m_iExp) >= iEK_Level)
                {
                    // ÃˆÃ±Â»Ã½Ã€ÃšÃ€Ã‡ Â·Â¹ÂºÂ§Ã€ÃŒ 80Ã€ÃŒÂ»Ã³Ã€ÃŒÂ°Ã­
                    if (memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[iClientH]->m_cMapName, 10) != 0)
                    {
                        // ÃˆÃ±Â»Ã½Ã€ÃšÂ°Â¡ ÃÃ—Ã€Âº Â°Ã·Ã€ÃŒ Ã€ÃšÂ½Ã…Ã€Ã‡ Â¸Â¶Ã€Â»Ã€ÃŒ Â¾Ã†Â´ÃÂ¶Ã³Â¸Ã© EKÂ·ÃŽ Ã€ÃŽÃÂ¤ 
                        m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
                    }
                    if (m_bEnemyKillMode == true)
                    {
                        // ÃˆÃ±Â»Ã½Ã€ÃšÂ°Â¡ ÃÃ—Ã€Âº Â°Ã·Ã€ÃŒ Ã€ÃšÂ½Ã…Ã€Ã‡ Â¸Â¶Ã€Â»Ã€ÃŒ Â¾Ã†Â´ÃÂ¶Ã³Â¸Ã© EKÂ·ÃŽ Ã€ÃŽÃÂ¤ 
                        m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
                    }
                }
                // Ã†Ã·Â»Ã³Â±Ã Â´Â©Ã€Ã» 
                m_pClientList[iAttackerH]->m_iRewardGold += iDice(1, (iGetExpLevel(m_pClientList[iClientH]->m_iExp)));
                if (m_pClientList[iAttackerH]->m_iRewardGold > DEF_MAXREWARDGOLD)
                    m_pClientList[iAttackerH]->m_iRewardGold = DEF_MAXREWARDGOLD;
                if (m_pClientList[iAttackerH]->m_iRewardGold < 0)
                    m_pClientList[iAttackerH]->m_iRewardGold = 0;
            }
            else
            {
                // Ã€ÃÂ¹Ã Â¸Ã°ÂµÃ¥.
                m_pClientList[iAttackerH]->m_iExp += iRewardExp;
                // ÃˆÃ±Â»Ã½Ã€ÃšÃ€Ã‡ Â·Â¹ÂºÂ§Ã€ÃŒ 80 Ã€ÃŒÂ»Ã³Ã€ÃŽ Â°Ã¦Â¿Ã¬ Enemy Kill countÂ¸Â¦ Â¿ÃƒÂ¸Â°Â´Ã™.
                if (iGetExpLevel(m_pClientList[iClientH]->m_iExp) >= iEK_Level)
                {
                    // ÃˆÃ±Â»Ã½Ã€ÃšÃ€Ã‡ Â·Â¹ÂºÂ§Ã€ÃŒ 80Ã€ÃŒÂ»Ã³Ã€ÃŒÂ°Ã­
                    if ((memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[iClientH]->m_cMapName, 10) != 0)
                        && (m_bEnemyKillMode == false))
                    {
                        // ÃˆÃ±Â»Ã½Ã€ÃšÂ°Â¡ ÃÃ—Ã€Âº Â°Ã·Ã€ÃŒ Ã€ÃšÂ½Ã…Ã€Ã‡ Â¸Â¶Ã€Â»Ã€ÃŒ Â¾Ã†Â´ÃÂ¶Ã³Â¸Ã© EKÂ·ÃŽ Ã€ÃŽÃÂ¤ 
                        m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
                    }

                    if (m_bEnemyKillMode == true)
                    {
                        // ÃˆÃ±Â»Ã½Ã€ÃšÂ°Â¡ ÃÃ—Ã€Âº Â°Ã·Ã€ÃŒ Ã€ÃšÂ½Ã…Ã€Ã‡ Â¸Â¶Ã€Â»Ã€ÃŒ Â¾Ã†Â´ÃÂ¶Ã³Â¸Ã© EKÂ·ÃŽ Ã€ÃŽÃÂ¤ 
                        m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
                    }
                }
                // Ã†Ã·Â»Ã³Â±Ã Â´Â©Ã€Ã» 
                m_pClientList[iAttackerH]->m_iRewardGold += iDice(1, (iGetExpLevel(m_pClientList[iClientH]->m_iExp)));
                if (m_pClientList[iAttackerH]->m_iRewardGold > DEF_MAXREWARDGOLD)
                    m_pClientList[iAttackerH]->m_iRewardGold = DEF_MAXREWARDGOLD;
                if (m_pClientList[iAttackerH]->m_iRewardGold < 0)
                    m_pClientList[iAttackerH]->m_iRewardGold = 0;
            }
        }
        else
        {
            // v2.15 Ã€Ã»Ã€Â» ÃÃ—Â¿Â´Ã€Â»Â¶Â§ Ã†Ã²Â±Ã•Â°Ã¦Ã‡Ã¨Ã„Â¡Â¸Â¦ Â¾Ã²Â±Ã¢Ã€Â§Ã‡Ã˜ 
            iRewardExp = (iDice(3, (3 * iGetExpLevel(m_pClientList[iClientH]->m_iExp))) + iGetExpLevel(m_pClientList[iClientH]->m_iExp)) / 3;

            // Â±Ã¦ÂµÃ¥Â¿Ã¸Ã€ÃŒÂ´Ã™.
            if (m_bIsCrusadeMode == true)
            {
                // Ã…Â©Â·Ã§Â¼Â¼Ã€ÃŒÂµÃ¥ Â¸Ã°ÂµÃ¥.
                // Ã…Â©Â·Ã§Â¼Â¼Ã€ÃŒÂµÃ¥ Â¸Ã°ÂµÃ¥Â¶Ã³Â¸Ã© Â°Ã¦Ã‡Ã¨Ã„Â¡Ã€Ã‡ 1/3Ã€Â» Â¸Ã•Ã€Ãº ÃÃ–Â°Ã­ Â³ÂªÂ¸Ã“ÃÃ¶Â´Ã‚ 2Â¹Ã¨Â·ÃŽ Ã€Ã¼Ã€Ã¯ Â°Ã¸Ã‡Ã¥ÂµÂµÂ¿Â¡ Â´Â©Ã€Ã» 
                m_pClientList[iAttackerH]->m_iExp += (iRewardExp / 3) * 4;
                m_pClientList[iAttackerH]->m_iWarContribution += (iRewardExp - (iRewardExp / 3)) * 12;

                if (m_pClientList[iAttackerH]->m_iWarContribution > DEF_MAXWARCONTRIBUTION)
                    m_pClientList[iAttackerH]->m_iWarContribution = DEF_MAXWARCONTRIBUTION;

                m_pClientList[iAttackerH]->m_iConstructionPoint += m_pClientList[iClientH]->m_iLevel / 2;

                if (m_pClientList[iAttackerH]->m_iConstructionPoint > DEF_MAXCONSTRUCTIONPOINT)
                    m_pClientList[iAttackerH]->m_iConstructionPoint = DEF_MAXCONSTRUCTIONPOINT;

                //testcode
                copy_string(G_cTxt, "Enemy Player Killed by Player! Construction: +%d WarContribution +%d", m_pClientList[iClientH]->m_iLevel / 2, (iRewardExp - (iRewardExp / 3)) * 6);
                log->info(G_cTxt);

                // Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
                SendNotifyMsg(0, iAttackerH, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[iAttackerH]->m_iConstructionPoint, m_pClientList[iAttackerH]->m_iWarContribution, 0, 0);

                // ÃˆÃ±Â»Ã½Ã€ÃšÃ€Ã‡ Â·Â¹ÂºÂ§Ã€ÃŒ 80 Ã€ÃŒÂ»Ã³Ã€ÃŽ Â°Ã¦Â¿Ã¬ Enemy Kill countÂ¸Â¦ Â¿ÃƒÂ¸Â°Â´Ã™.
                if (iGetExpLevel(m_pClientList[iClientH]->m_iExp) >= iEK_Level)
                {
                    // ÃˆÃ±Â»Ã½Ã€ÃšÃ€Ã‡ Â·Â¹ÂºÂ§Ã€ÃŒ 80Ã€ÃŒÂ»Ã³Ã€ÃŒÂ°Ã­
                    if ((memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[iClientH]->m_cMapName, 10) != 0)
                        && (m_bEnemyKillMode == false))
                    {
                        // ÃˆÃ±Â»Ã½Ã€ÃšÂ°Â¡ ÃÃ—Ã€Âº Â°Ã·Ã€ÃŒ Ã€ÃšÂ½Ã…Ã€Ã‡ Â¸Â¶Ã€Â»Ã€ÃŒ Â¾Ã†Â´ÃÂ¶Ã³Â¸Ã© EKÂ·ÃŽ Ã€ÃŽÃÂ¤ 
                        m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
                    }

                    if (m_bEnemyKillMode == true)
                    {
                        // ÃˆÃ±Â»Ã½Ã€ÃšÂ°Â¡ ÃÃ—Ã€Âº Â°Ã·Ã€ÃŒ Ã€ÃšÂ½Ã…Ã€Ã‡ Â¸Â¶Ã€Â»Ã€ÃŒ Â¾Ã†Â´ÃÂ¶Ã³Â¸Ã© EKÂ·ÃŽ Ã€ÃŽÃÂ¤ 
                        m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
                    }
                }
                // Ã†Ã·Â»Ã³Â±Ã Â´Â©Ã€Ã» 
                m_pClientList[iAttackerH]->m_iRewardGold += iDice(1, (iGetExpLevel(m_pClientList[iClientH]->m_iExp)));
                if (m_pClientList[iAttackerH]->m_iRewardGold > DEF_MAXREWARDGOLD)
                    m_pClientList[iAttackerH]->m_iRewardGold = DEF_MAXREWARDGOLD;
                if (m_pClientList[iAttackerH]->m_iRewardGold < 0)
                    m_pClientList[iAttackerH]->m_iRewardGold = 0;
            }
            else
            {
                // Ã€ÃÂ¹Ã Â¸Ã°ÂµÃ¥.
                m_pClientList[iAttackerH]->m_iExp += iRewardExp;
                // Enemy Kill countÂ¸Â¦ Â¿ÃƒÂ¸Â°Â´Ã™.
                if (iGetExpLevel(m_pClientList[iClientH]->m_iExp) >= iEK_Level)
                {
                    // ÃˆÃ±Â»Ã½Ã€ÃšÃ€Ã‡ Â·Â¹ÂºÂ§Ã€ÃŒ 80Ã€ÃŒÂ»Ã³Ã€ÃŒÂ°Ã­
                    if ((memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[iClientH]->m_cMapName, 10) != 0)
                        && (m_bEnemyKillMode == false))
                    {
                        // ÃˆÃ±Â»Ã½Ã€ÃšÂ°Â¡ ÃÃ—Ã€Âº Â°Ã·Ã€ÃŒ Ã€ÃšÂ½Ã…Ã€Ã‡ Â¸Â¶Ã€Â»Ã€ÃŒ Â¾Ã†Â´ÃÂ¶Ã³Â¸Ã© EKÂ·ÃŽ Ã€ÃŽÃÂ¤ 
                        m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
                    }

                    if (m_bEnemyKillMode == true)
                    {
                        // ÃˆÃ±Â»Ã½Ã€ÃšÂ°Â¡ ÃÃ—Ã€Âº Â°Ã·Ã€ÃŒ Ã€ÃšÂ½Ã…Ã€Ã‡ Â¸Â¶Ã€Â»Ã€ÃŒ Â¾Ã†Â´ÃÂ¶Ã³Â¸Ã© EKÂ·ÃŽ Ã€ÃŽÃÂ¤ 
                        m_pClientList[iAttackerH]->m_iEnemyKillCount += m_iEnemyKillAdjust;
                    }
                }
                // Ã†Ã·Â»Ã³Â±Ã Â´Â©Ã€Ã» 
                m_pClientList[iAttackerH]->m_iRewardGold += iDice(1, (iGetExpLevel(m_pClientList[iClientH]->m_iExp)));
                if (m_pClientList[iAttackerH]->m_iRewardGold > DEF_MAXREWARDGOLD)
                    m_pClientList[iAttackerH]->m_iRewardGold = DEF_MAXREWARDGOLD;
                if (m_pClientList[iAttackerH]->m_iRewardGold < 0)
                    m_pClientList[iAttackerH]->m_iRewardGold = 0;
            }
        }

        // Ã€Ã»Ã€Â» Ã€Ã¢Â¾Ã’Â´Ã™Â´Ã‚ Â¸ÃžÂ½ÃƒÃÃ¶ ÂºÂ¸Â³Â¿ 
        SendNotifyMsg(0, iAttackerH, DEF_NOTIFY_ENEMYKILLREWARD, iClientH, 0, 0, 0);

        if (bCheckLimitedUser(iAttackerH) == false)
        {
            // ÃƒÂ¼Ã‡Ã¨Ã†Ã‡ Â»Ã§Â¿Ã«Ã€Ãš ÃÂ¦Ã‡Ã‘Â¿Â¡ Ã‡Ã˜Â´Ã§ÂµÃ‡ÃÃ¶ Â¾ÃŠÃ€Â¸Â¸Ã© Â°Ã¦Ã‡Ã¨Ã„Â¡Â°Â¡ Â¿ÃƒÂ¶ÃºÂ´Ã™Â´Ã‚ Ã…Ã«ÂºÂ¸Â¸Â¦ Ã‡Ã‘Â´Ã™.
            SendNotifyMsg(0, iAttackerH, DEF_NOTIFY_EXP, 0, 0, 0, 0);
        }
        // Â·Â¹ÂºÂ§Ã€ÃŒ Â¿ÃƒÂ¶ÃºÂ´Ã‚ÃÃ¶Â¸Â¦ Â°Ã‹Â»Ã§Ã‡Ã‘Â´Ã™.
        bCheckLevelUp(iAttackerH);

        //v1.4 Â±Â³Ã€Ã¼Â¿Â¡Â¼Â­ Ã€ÃŒÂ±Ã¤ Ã„Â«Â¿Ã®Ã†Â®Â¸Â¦ Â¿ÃƒÂ¸Â°Â´Ã™.
        m_stCityStatus[m_pClientList[iAttackerH]->m_cSide].iWins++;
    }
}

void CGame::GetRewardMoneyHandler(int iClientH)
{
    int iRet, iEraseReq, iWeightLeft, iRewardGoldLeft;
    uint32_t * dwp;
    uint16_t * wp;
    char * cp, cData[100], cItemName[21];
    class CItem * pItem;
    short * sp;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;


    // Â³Â²Ã€Âº ÃÃŸÂ·Â®Ã€Â» Â°Ã¨Â»ÃªÃ‡Ã‘Â´Ã™. 
    iWeightLeft = _iCalcMaxLoad(iClientH) - iCalcTotalWeight(iClientH);

    if (iWeightLeft <= 0) return;
    // ÃÃŸÂ·Â®Ã€Â» Â¹ÃÃ€Â¸Â·ÃŽ Â³ÂªÂ´Â«Â´Ã™. <- ÃƒÂ£Ã€Âº ÂµÂ·Ã€Â¸Â·ÃŽ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â»Ã¬ Â°Ã¸Â°Â£Ã€Âº Â¸Â¶Â·ÃƒÃ‡Ã˜ ÂµÃ–Â¾ÃŸ Ã‡ÃÂ¹Ã‡Â·ÃŽ.
    iWeightLeft = iWeightLeft / 2;
    if (iWeightLeft <= 0) return;

    pItem = new class CItem;
    memset(cItemName, 0, sizeof(cItemName));
    copy_string(cItemName, "Gold");
    _bInitItemAttr(pItem, cItemName);
    //pItem->m_dwCount = m_pClientList[iClientH]->m_iRewardGold;

    // (iWeightLeft / pItem->m_wWeight)Â°Â¡ ÃƒÃ–Â´Ã« Â¹ÃžÃ€Â» Â¼Ã¶ Ã€Ã–Â´Ã‚ GoldÂ°Â¹Â¼Ã¶. Â°Â®Â°Ã­Ã€Ã–Â´Ã‚ Ã†Ã·Â»Ã³Â±ÃÂ°Ãº ÂºÃ±Â±Â³Ã‡Ã‘Â´Ã™. 
    if ((iWeightLeft / iGetItemWeight(pItem, 1)) >= m_pClientList[iClientH]->m_iRewardGold)
    {
        // Ã†Ã·Â»Ã³Â±ÃÃ€Â» Â¸Ã°ÂµÃŽ Â¹ÃžÃ€Â» Â¼Ã¶ Ã€Ã–Â´Ã™. 
        pItem->m_dwCount = m_pClientList[iClientH]->m_iRewardGold;
        iRewardGoldLeft = 0;
    }
    else
    {
        // (iWeightLeft / pItem->m_wWeight)Â¸Â¸ Â¹ÃžÂ´Ã‚Â´Ã™.
        pItem->m_dwCount = (iWeightLeft / iGetItemWeight(pItem, 1));
        iRewardGoldLeft = m_pClientList[iClientH]->m_iRewardGold - (iWeightLeft / iGetItemWeight(pItem, 1));
    }

    if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true)
    {
        // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» ÃˆÂ¹ÂµÃ¦Ã‡ÃŸÂ´Ã™.

        // Â³Â²Ã€Âº Ã†Ã·Â»Ã³Â±Ã Â³Â»Â¿Âª Â°Ã¨Â»Ãª.
        m_pClientList[iClientH]->m_iRewardGold = iRewardGoldLeft;

        dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
        *dwp = MSGID_NOTIFY;
        wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
        *wp = DEF_NOTIFY_ITEMOBTAINED;

        cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);

        // 1Â°Â³ ÃˆÂ¹ÂµÃ¦Ã‡ÃŸÂ´Ã™. <- Â¿Â©Â±Ã¢Â¼Â­ 1Â°Â³Â¶Ãµ Ã„Â«Â¿Ã®Ã†Â®Â¸Â¦ Â¸Â»Ã‡ÃÂ´Ã‚ Â°ÃÃ€ÃŒ Â¾Ã†Â´ÃÂ´Ã™
        *cp = 1;
        cp++;

        memcpy(cp, pItem->m_cName, 20);
        cp += 20;

        dwp = (uint32_t *)cp;
        *dwp = pItem->m_dwCount;
        cp += 4;

        *cp = pItem->m_cItemType;
        cp++;

        *cp = pItem->m_cEquipPos;
        cp++;

        *cp = (char)0; // Â¾Ã²Ã€Âº Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒÂ¹Ã‡Â·ÃŽ Ã€Ã¥Ã‚Ã¸ÂµÃ‡ÃÃ¶ Â¾ÃŠÂ¾Ã’Â´Ã™.
        cp++;

        sp = (short *)cp;
        *sp = pItem->m_sLevelLimit;
        cp += 2;

        *cp = pItem->m_cGenderLimit;
        cp++;

        wp = (uint16_t *)cp;
        *wp = pItem->m_wCurLifeSpan;
        cp += 2;

        wp = (uint16_t *)cp;
        *wp = pItem->m_wWeight;
        cp += 2;

        sp = (short *)cp;
        *sp = pItem->m_sSprite;
        cp += 2;

        sp = (short *)cp;
        *sp = pItem->m_sSpriteFrame;
        cp += 2;

        *cp = pItem->m_cItemColor;
        cp++;

        *cp = (char)pItem->m_sItemSpecEffectValue2; // v1.41 
        cp++;

        dwp = (uint32_t *)cp;
        *dwp = pItem->m_dwAttribute;
        cp += 4;
        /*
        *cp = (char)(pItem->m_dwAttribute & 0x00000001); // Custom-ItemÃ€ÃŽÃÃ¶Ã€Ã‡ Â¿Â©ÂºÃŽ
        cp++;
        */

        if (iEraseReq == 1) delete pItem;

        // Â¾Ã†Ã€ÃŒÃ…Ã› ÃÂ¤ÂºÂ¸ Ã€Ã¼Â¼Ã› 
        iRet = m_pClientList[iClientH]->send_msg(cData, 53);

        switch (iRet)
        {
            case DEF_XSOCKEVENT_QUENEFULL:
            case DEF_XSOCKEVENT_SOCKETERROR:
            case DEF_XSOCKEVENT_CRITICALERROR:
                // Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ ÂºÂ¸Â³Â¾Â¶Â§ Â¿Â¡Â·Â¯Â°Â¡ Â¹ÃŸÂ»Ã½Ã‡ÃŸÂ´Ã™Â¸Ã© ÃÂ¦Â°Ã…Ã‡Ã‘Â´Ã™.
                DeleteClient(iClientH, true, true);
                return;
        }

        // Â±Ã— Â´Ã™Ã€Â½ Â³Â²Ã€Âº Ã†Ã·Â»Ã³Â±ÃÃ€Â» Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_REWARDGOLD, 0, 0, 0, 0);
    }
    else
    {
        // Â¹ÃžÃ€Â» Â¼Ã¶ Â¾Ã¸Â´Ã‚ Â°Ã¦Â¿Ã¬Â´Ã‚ Â¾Ã†Â¹Â«Â·Â± ÃƒÂ³Â¸Â®Â¸Â¦ Ã‡ÃÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™. 

    }
}

void CGame::ItemDepleteHandler(int iClientH, short sItemIndex, bool bIsUseItemResult)
{
    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return;
    if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == 0) return;

    _bItemLog(DEF_ITEMLOG_DEPLETE, iClientH, 0, m_pClientList[iClientH]->m_pItemList[sItemIndex]);

    ReleaseItemHandler(iClientH, sItemIndex, true);

    SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMDEPLETED_ERASEITEM, sItemIndex, (int)bIsUseItemResult, 0, 0);

    delete m_pClientList[iClientH]->m_pItemList[sItemIndex];
    m_pClientList[iClientH]->m_pItemList[sItemIndex] = 0;

    m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] = false;

    m_pClientList[iClientH]->m_cArrowIndex = _iGetArrowItemIndex(iClientH);

    iCalcTotalWeight(iClientH);
}

void CGame::UseSkillHandler(int iClientH, int iV1, int iV2, int iV3)
{
    char  cOwnerType;
    short sAttackerWeapon, sOwnerH;
    int   iResult, iPlayerSkillLevel;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

    if ((iV1 < 0) || (iV1 >= DEF_MAXSKILLTYPE)) return;
    if (m_pSkillConfigList[iV1] == 0) return;
    // Ã€ÃŒÂ¹ÃŒ Â±Ã¢Â¼ÃºÃ€Â» Â»Ã§Â¿Ã«ÃÃŸÃ€ÃŒÂ¶Ã³ÂµÂµ Â¸Â®Ã…Ã.
    if (m_pClientList[iClientH]->m_bSkillUsingStatus[iV1] == true) return;

    // v1.3 !!1Ã‡Ã˜Ã…Â· Â°Ã‰Â·Â¯Â³Â»Â±Ã¢Â¿Ã«! 
    /*
    if (iV1 != 19) {
        m_pClientList[iClientH]->m_iAbuseCount++;
        if ((m_pClientList[iClientH]->m_iAbuseCount % 30) == 0) {
            copy_string(G_cTxt, "(!) Ã‡Ã˜Ã…Â· Â¿Ã«Ã€Ã‡Ã€Ãš(%s) Skill(%d) Tries(%d)",m_pClientList[iClientH]->m_cCharName,
                                                                       iV1, m_pClientList[iClientH]->m_iAbuseCount);
            PutLogFileList(G_cTxt);
        }
    }
    */

    // Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Ã€Ã‡ Â±Ã¢Â¼ÃºÂ¼Ã¶ÃÃ˜Â¿Â¡ ÂµÃ»Â¶Ã³ Â¼ÂºÂ°Ã¸Â¿Â©ÂºÃŽÂ¸Â¦ Â°Ã¨Â»ÃªÃ‡Ã‘Â´Ã™. 
    iPlayerSkillLevel = m_pClientList[iClientH]->m_cSkillMastery[iV1];
    iResult = iDice(1, 100);

    if (iResult > iPlayerSkillLevel)
    {
        // Â½Ã‡Ã†ÃÂ´Ã™. 
        // Â±Ã¢Â¼Ãº Â»Ã§Â¿Ã«Ã€ÃŒ ÃÃŸÃÃ¶ ÂµÃ‡Â¾ÃºÃ€Â½Ã€Â» Â¾Ã‹Â¸Â°Â´Ã™.
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILLUSINGEND, 0, 0, 0, 0);
        return;
    }

    // iV1Ã€ÃŒ Â±Ã¢Â¼Ãº Â¹Ã¸ÃˆÂ£ 
    switch (m_pSkillConfigList[iV1]->m_sType)
    {
        case DEF_SKILLEFFECTTYPE_PRETEND:
            switch (m_pSkillConfigList[iV1]->m_sValue1)
            {
                case 1:
                    // ÃÃ—Ã€ÂºÃƒÂ´Ã‡ÃÂ±Ã¢ Â±Ã¢Â¼ÃºÃ€ÃŒÂ´Ã™.	

                    // v1.44 Â»Ã§Ã…ÃµÃ€Ã¥Ã€ÃŒÂ¸Ã© ÃÃ—Ã€ÂºÃƒÂ´Ã‡ÃÂ±Ã¢ Â¸Ã¸Ã‡Ã‘Â´Ã™.
                    if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFightZone == true)
                    {
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILLUSINGEND, 0, 0, 0, 0);
                        return;
                    }

                    //Â¸Â¸Â¾Ã  Â¹Ã™Â´ÃšÂ¿Â¡ Â½ÃƒÃƒÂ¼Â°Â¡ Ã€Ã–Â´Ã™Â¸Ã© ÃÃ—Ã€ÂºÃƒÂ´ Ã‡ÃÂ±Ã¢Â¸Â¦ Ã‡Ã’ Â¼Ã¶ Â¾Ã¸Â´Ã™. 
                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY);
                    if (sOwnerH != 0)
                    {
                        // ÃÃ—Ã€ÂºÃƒÂ´ Ã‡ÃÂ±Ã¢Â¸Â¦ Ã‡Ã’ Ã€ÃšÂ¸Â®Â¿Â¡ Â½ÃƒÃƒÂ¼Â°Â¡ Ã€Ã–Â¾Ã® Â±Ã¢Â¼Ãº Â»Ã§Â¿Ã«Ã€ÃŒ ÃÃŸÃÃ¶ ÂµÃ‡Â¾ÃºÃ€Â½Ã€Â» Â¾Ã‹Â¸Â°Â´Ã™.
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILLUSINGEND, 0, 0, 0, 0);
                        return;
                    }

                    // ÃÃ—Ã€ÂºÃƒÂ´ Ã‡ÃÂ°Ã­Ã€Ãš Ã‡ÃÂ´Ã‚ ÃÃ–ÂºÂ¯Â¿Â¡ Â¹Â°ÃƒÂ¼Â°Â¡ Ã€Ã–Â¾Ã®ÂµÂµ Ã‡Ã’ Â¼Ã¶ Â¾Ã¸Â´Ã™. 
                    iResult = 0;
                    if (m_pClientList[iClientH]->m_iAdminUserLevel <= 0)
                    {
                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY - 1);
                        iResult += sOwnerH;
                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY + 1);
                        iResult += sOwnerH;
                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX - 1, m_pClientList[iClientH]->m_sY);
                        iResult += sOwnerH;
                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX + 1, m_pClientList[iClientH]->m_sY);
                        iResult += sOwnerH;

                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX - 1, m_pClientList[iClientH]->m_sY - 1);
                        iResult += sOwnerH;
                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX + 1, m_pClientList[iClientH]->m_sY - 1);
                        iResult += sOwnerH;
                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX - 1, m_pClientList[iClientH]->m_sY + 1);
                        iResult += sOwnerH;
                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX + 1, m_pClientList[iClientH]->m_sY + 1);
                        iResult += sOwnerH;
                    }

                    if (iResult != 0)
                    {
                        // ÃÃ—Ã€ÂºÃƒÂ´ Ã‡ÃÂ±Ã¢Â¸Â¦ Ã‡Ã’ Ã€ÃšÂ¸Â®Â¿Â¡ Â½ÃƒÃƒÂ¼Â°Â¡ Ã€Ã–Â¾Ã® Â±Ã¢Â¼Ãº Â»Ã§Â¿Ã«Ã€ÃŒ ÃÃŸÃÃ¶ ÂµÃ‡Â¾ÃºÃ€Â½Ã€Â» Â¾Ã‹Â¸Â°Â´Ã™.
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILLUSINGEND, 0, 0, 0, 0);
                        return;
                    }

                    // Â½ÂºÃ…Â³ Ã„Â«Â¿Ã®Ã†Â® Â¿ÃƒÂ¸Â°Â´Ã™. <-- Â³Â»ÂºÃŽÂ¿Â¡Â¼Â­ Ã„Â«Â¿Ã®Ã†ÃƒÃ€Â» Ã‡Ã˜Â¾ÃŸ Â¾Ã»Â¶Ã—Ã‡Ã‘ Â½ÂºÃ…Â³Ã€ÃŒ Â¿Ã€Â¸Â£Â´Ã‚ Â°Ã¦Â¿Ã¬Â°Â¡ Â¾Ã¸Â´Ã™. 
                    CalculateSSN_SkillIndex(iClientH, iV1, 1);

                    // Â´Ã™Â¸Â¥ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¿Â¡Â°Ã” ÃÃ—Â´Ã‚ ÂµÂ¿Ã€Ã› Ã€Ã¼Â¼Ã›.
                    sAttackerWeapon = 1;
                    SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDYING, 0, sAttackerWeapon, 0);
                    // ÃÂ¤Â»Ã³ Ã€Â§Ã„Â¡Â¿Â¡Â¼Â­ ÃÃ¶Â¿Ã®Â´Ã™.
                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->ClearOwner(14, iClientH, DEF_OWNERTYPE_PLAYER, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY);
                    // ÃÃ—Ã€Âº Ã€Â§Ã„Â¡ Ã‡Â¥Â½ÃƒÂ¸Â¦ Ã‡Ã‘Â´Ã™.
                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetDeadOwner(iClientH, DEF_OWNERTYPE_PLAYER, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY);
                    break;
            }
            break;

    }

    m_pClientList[iClientH]->m_bSkillUsingStatus[iV1] = true;
}

void CGame::ReqSellItemHandler(int iClientH, char cItemID, char cSellToWhom, int iNum, char * pItemName)
{
    char cItemCategory, cItemName[21];
    short sRemainLife;
    int   iPrice;
    double d1, d2, d3;
    bool   bNeutral;
    uint32_t  dwSWEType, dwSWEValue, dwAddPrice1, dwAddPrice2, dwMul1, dwMul2;
    CItem * m_pGold;

    // Â»Ã§Â¿Ã«Ã€ÃšÃ€Ã‡ Â¾Ã†Ã€ÃŒÃ…Ã› Ã†ÃˆÂ±Ã¢ Â¿Ã¤Â±Â¸.
    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    if ((cItemID < 0) || (cItemID >= 50)) return;
    if (m_pClientList[iClientH]->m_pItemList[cItemID] == 0) return;
    if (iNum <= 0) return;
    if (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwCount < iNum) return;

    iCalcTotalWeight(iClientH);

    m_pGold = new class CItem;
    memset(cItemName, 0, sizeof(cItemName));
    copy_string(cItemName, "Gold");
    _bInitItemAttr(m_pGold, cItemName);

    // v1.42
    bNeutral = false;
    if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) bNeutral = true;
    // v2.13 Â¼ÂºÃˆÃ„Â´Ã Â¼Ã¶ÃÂ¤ Â¹Â°Â°Ã‡Ã€Â» Â¾Ã®ÂµÃ°Â¼Â­Â³Âª Ã†ÃˆÂ°Ã” Â¼Ã¶ÃÂ¤Ã‡ÃÂ¿Â©Â¼Â­ Ã†ÃˆÂ¶Â§Â´Ã‚ NPC Â±Â¸ÂºÃÃ€ÃŒ Ã‡ÃŠÂ¿Ã¤Â¾Ã¸Â´Ã™. 
    // Â´Ãœ Ã„Â«Ã…Ã—Â°Ã­Â¸Â®Â¸Â¦ Â±Ã¢ÃÃ˜Ã€Â¸Â·ÃŽ Â¾Ã†Ã€ÃŒÃ…Ã› Â°Â¡Â°ÃÃ€Â» Â°Ã¡ÃÂ¤Ã‡Ã‘Â´Ã™.
    switch (cSellToWhom)
    {
        case 15: 		// Â»Ã³ÃÂ¡ Â¾Ã†ÃÃœÂ¸Â¶ 
        case 24:        // Â´Ã«Ã€Ã¥Â°Â£ ÃÃ–Ã€ÃŽ 
            cItemCategory = m_pClientList[iClientH]->m_pItemList[cItemID]->m_cCategory;
            // 12-22 Â¼ÂºÃˆÃ„Â´Ã Â¼Ã¶ÃÂ¤ Â¾Ã®ÂµÃ°Â¼Â­ÂµÃ§ Ã†ÃˆÂ¼Ã¶ Ã€Ã–Â°Ã” Â¼Ã¶ÃÂ¤ 
            // Â»Ã³ÃÂ¡Â¾Ã†Ã€ÃŒÃ…Ã› 
            if ((cItemCategory >= 11) && (cItemCategory <= 50))
            {

                // Ã€Ã»Ã‡Ã•Ã‡ÃÂ´Ã™. Â¹Â«ÃÂ¶Â°Ã‡ Â¹ÃÂ°Âª 
                iPrice = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice / 2) * iNum;
                sRemainLife = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wCurLifeSpan;


                //v1.42 ÃÃŸÂ¸Â³Ã€ÃŽ Â°Ã¦Â¿Ã¬ Â¹ÃÃ€Ã‡ Â¹ÃÂ°Âª.
                if (bNeutral == true) iPrice = iPrice / 2;
                if (iPrice <= 0)    iPrice = 1;
                if (iPrice > 1000000) iPrice = 1000000;

                if (m_pClientList[iClientH]->m_iCurWeightLoad + iGetItemWeight(m_pGold, iPrice) > (uint32_t)_iCalcMaxLoad(iClientH))
                {
                    // v2.12 Ã†Ãˆ Â°Ã¦Â¿Ã¬ Â¹Â«Â°Ã”Â°Â¡ ÃƒÃŠÂ°ÃºÂµÃ‡Â¾Ã®Â¼Â­ Ã†Ãˆ Â¼Ã¶ Â¾Ã¸Â´Ã™.
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_CANNOTSELLITEM, cItemID, 4, 0, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
                }
                else SendNotifyMsg(0, iClientH, DEF_NOTIFY_SELLITEMPRICE, cItemID, sRemainLife, iPrice, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName, iNum);
            }
            // Â´Ã«Ã€Ã¥Â°Â£ Â¾Ã†Ã€ÃŒÃ…Ã›
            else if ((cItemCategory >= 1) && (cItemCategory <= 10))
            {
                // Â¿Ã¸Â·Â¡ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ Â¼Ã¶Â¸Ã­Â°Ãº ÂºÃ±Â±Â³Ã‡Ã˜Â¼Â­ Â°Â¨Â°Â¡ Â»Ã³Â°Â¢Ã€Â» Â°Ã¨Â»Ãª, Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ Â°Â¡Â°ÃÃ€Â» Â¸Ã…Â±Ã¤Â´Ã™.
                sRemainLife = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wCurLifeSpan;

                if (sRemainLife == 0)
                {
                    // Â°Ã­Ã€Ã¥Â³Â­ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Âº Ã†Ãˆ Â¼Ã¶ Â¾Ã¸Â´Ã™
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_CANNOTSELLITEM, cItemID, 2, 0, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
                }
                else
                {
                    d1 = (double)sRemainLife;
                    if (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan != 0)
                        d2 = (double)m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan;
                    else d2 = 1.0f;
                    d3 = (d1 / d2) * 0.5f;
                    d2 = (double)m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice; // Â¿Ã¸Â·Â¡ Â°Â¡Â°Ã 
                    d3 = d3 * d2; // ÃƒÃŸÂ»ÃªÂµÃˆ Â°Â¡Â°Ã 

                    iPrice = (int)d3;
                    iPrice = iPrice * iNum;

                    dwAddPrice1 = 0;
                    dwAddPrice2 = 0;
                    // Â¾Ã†Ã€ÃŒÃ…Ã› Ã†Â¯Â¼ÂºÃ„Â¡Â¿Â¡ ÂµÃ»Â¸Â¥ Â°Â¡Â°Ã Â»Ã³Â½Ã‚ 
                    if ((m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x00F00000) != 0)
                    {
                        dwSWEType = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x00F00000) >> 20;
                        dwSWEValue = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x000F0000) >> 16;

                        switch (dwSWEType)
                        {
                            case 6: dwMul1 = 2; break;  // Â°Â¡ÂºÂ­Â¿Ã® 
                            case 8: dwMul1 = 2; break;  // Â°Â­ÃˆÂ­ÂµÃˆ
                            case 5: dwMul1 = 3; break;  // Â¹ÃŽÃƒÂ¸Ã€Ã‡
                            case 1: dwMul1 = 4; break;  // Ã‡ÃŠÂ»Ã¬Ã€Ã‡ 
                            case 7: dwMul1 = 5; break;  // Â¿Â¹Â¸Â®Ã‡Ã‘
                            case 2: dwMul1 = 6; break;  // ÃÃŸÂµÂ¶Ã€Ã‡
                            case 3: dwMul1 = 15; break; // ÃÂ¤Ã€Ã‡Ã€Ã‡ 
                            case 9: dwMul1 = 20; break; // Â°Ã­Â´Ã«Â¹Â®Â¸Ã­ 
                            default: dwMul1 = 1; break;
                        }

                        d1 = (double)iPrice * dwMul1;
                        switch (dwSWEValue)
                        {
                            case 1: d2 = 10.0f; break;
                            case 2: d2 = 20.0f; break;
                            case 3: d2 = 30.0f; break;
                            case 4: d2 = 35.0f; break;
                            case 5: d2 = 40.0f; break;
                            case 6: d2 = 50.0f; break;
                            case 7: d2 = 100.0f; break;
                            case 8: d2 = 200.0f; break;
                            case 9: d2 = 300.0f; break;
                            case 10: d2 = 400.0f; break;
                            case 11: d2 = 500.0f; break;
                            case 12: d2 = 700.0f; break;
                            case 13: d2 = 900.0f; break;
                            default: d2 = 0.0f; break;
                        }
                        d3 = d1 * (d2 / 100.0f);

                        dwAddPrice1 = (int)(d1 + d3);
                    }

                    // v1.42 ÃˆÃ±Â±Ã Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒÂ¶Ã³Â¸Ã© Sub ÃˆÂ¿Â°ÃºÂ¸Â¦ Â¼Â³ÃÂ¤Ã‡Ã‘Â´Ã™. Â°Ã¸Â°ÃÂ¹Â«Â±Ã¢Â´Ã‚ 1Â°Â³Â¸Â¸ Ã€Ã¥Ã‚Ã¸ÂµÃˆÂ´Ã™Â°Ã­ Ã‡ÃŸÃ€Â»Â¶Â§Â¸Â¸ Ã€Â¯ÃˆÂ¿Ã‡Ã”.
                    if ((m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x0000F000) != 0)
                    {
                        dwSWEType = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x0000F000) >> 12;
                        dwSWEValue = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x00000F00) >> 8;

                        switch (dwSWEType)
                        {
                            case 1:
                            case 12: dwMul2 = 2; break;

                            case 2:
                            case 3:
                            case 4:
                            case 5:
                            case 6:
                            case 7: dwMul2 = 4; break;

                            case 8:
                            case 9:
                            case 10:
                            case 11: dwMul2 = 6; break;
                        }

                        d1 = (double)iPrice * dwMul2;
                        switch (dwSWEValue)
                        {
                            case 1: d2 = 10.0f; break;
                            case 2: d2 = 20.0f; break;
                            case 3: d2 = 30.0f; break;
                            case 4: d2 = 35.0f; break;
                            case 5: d2 = 40.0f; break;
                            case 6: d2 = 50.0f; break;
                            case 7: d2 = 100.0f; break;
                            case 8: d2 = 200.0f; break;
                            case 9: d2 = 300.0f; break;
                            case 10: d2 = 400.0f; break;
                            case 11: d2 = 500.0f; break;
                            case 12: d2 = 700.0f; break;
                            case 13: d2 = 900.0f; break;
                            default: d2 = 0.0f; break;
                        }
                        d3 = d1 * (d2 / 100.0f);

                        dwAddPrice2 = (int)(d1 + d3);
                    }

                    // v2.03 925 Ã†Â¯Â¼Ã¶ Â¾Ã†Ã€ÃŒÃ…Ã› Â°Â¡Â°Ã Â°Â¡ÃÃŸÃ„Â¡Â¸Â¦ 77%Â¼Ã¶ÃÃ˜Ã€Â¸Â·ÃŽ Â´Ã™Â¿Ã® 
                    iPrice = iPrice + (dwAddPrice1 - (dwAddPrice1 / 3)) + (dwAddPrice2 - (dwAddPrice2 / 3));

                    //v1.42 ÃÃŸÂ¸Â³Ã€ÃŽ Â°Ã¦Â¿Ã¬ Â¹ÃÃ€Ã‡ Â¹ÃÂ°Âª.
                    if (bNeutral == true) iPrice = iPrice / 2;
                    if (iPrice <= 0)    iPrice = 1;
                    if (iPrice > 1000000) iPrice = 1000000;

                    if (m_pClientList[iClientH]->m_iCurWeightLoad + iGetItemWeight(m_pGold, iPrice) > (uint32_t)_iCalcMaxLoad(iClientH))
                    {
                        // v2.12 Ã†Ãˆ Â°Ã¦Â¿Ã¬ Â¹Â«Â°Ã”Â°Â¡ ÃƒÃŠÂ°ÃºÂµÃ‡Â¾Ã®Â¼Â­ Ã†Ãˆ Â¼Ã¶ Â¾Ã¸Â´Ã™.
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_CANNOTSELLITEM, cItemID, 4, 0, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
                    }
                    else SendNotifyMsg(0, iClientH, DEF_NOTIFY_SELLITEMPRICE, cItemID, sRemainLife, iPrice, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName, iNum);
                }
            }
            else SendNotifyMsg(0, iClientH, DEF_NOTIFY_CANNOTSELLITEM, cItemID, 1, 0, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
            break;


        default:
            break;
    }
    if (m_pGold != 0) delete m_pGold;
}

void CGame::ReqSellItemConfirmHandler(int iClientH, char cItemID, int iNum, char * pString)
{
    class CItem * pItemGold;
    short sRemainLife;
    int   iPrice;
    double d1, d2, d3;
    char * cp, cItemName[21], cData[120], cItemCategory;
    uint32_t * dwp, dwMul1, dwMul2, dwSWEType, dwSWEValue, dwAddPrice1, dwAddPrice2;
    uint16_t * wp;
    int    iEraseReq, iRet;
    short * sp;
    bool   bNeutral;


    // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Ã†ÃˆÂ°ÃšÂ´Ã™Â´Ã‚ Â°ÃÃ€ÃŒ Â°Ã¡ÃÂ¤ÂµÃ‡Â¾ÃºÂ´Ã™.
    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    if ((cItemID < 0) || (cItemID >= 50)) return;
    if (m_pClientList[iClientH]->m_pItemList[cItemID] == 0) return;
    if (iNum <= 0) return;
    if (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwCount < iNum) return;

    // New 18/05/2004
    if (m_pClientList[iClientH]->m_pIsProcessingAllowed == false) return;

    iCalcTotalWeight(iClientH);
    cItemCategory = m_pClientList[iClientH]->m_pItemList[cItemID]->m_cCategory;

    // v1.42
    bNeutral = false;
    if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) bNeutral = true;

    iPrice = 0;
    // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ ÃÂ¾Â·Ã¹Â¿Â¡ ÂµÃ»Â¶Ã³ Â°Â¡Â°Ã Â°Ã¨Â»Ãª.
    if ((cItemCategory >= 1) && (cItemCategory <= 10))
    {
        // Â¹Â«Â±Ã¢Â·Ã¹Â´Ã™
        // Â¿Ã¸Â·Â¡ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ Â¼Ã¶Â¸Ã­Â°Ãº ÂºÃ±Â±Â³Ã‡Ã˜Â¼Â­ Â°Â¨Â°Â¡ Â»Ã³Â°Â¢Ã€Â» Â°Ã¨Â»Ãª, Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ Â°Â¡Â°ÃÃ€Â» Â¸Ã…Â±Ã¤Â´Ã™.
        sRemainLife = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wCurLifeSpan;

        if (sRemainLife <= 0)
        {
            // Â¸ÃÂ°Â¡ÃÃ¸ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Âº Ã†ÃˆÃÃ¶ Â¸Ã¸Ã‡Ã‘Â´Ã™.	
            return;
        }
        else
        {
            d1 = (double)sRemainLife;
            if (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan != 0)
                d2 = (double)m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan;
            else d2 = 1.0f;
            d3 = (d1 / d2) * 0.5f;
            d2 = (double)m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice; // Â¿Ã¸Â·Â¡ Â°Â¡Â°Ã 
            d3 = d3 * d2; // ÃƒÃŸÂ»ÃªÂµÃˆ Â°Â¡Â°Ã 

            iPrice = (short)d3;
            iPrice = iPrice * iNum;

            dwAddPrice1 = 0;
            dwAddPrice2 = 0;
            // Â¾Ã†Ã€ÃŒÃ…Ã› Ã†Â¯Â¼ÂºÃ„Â¡Â¿Â¡ ÂµÃ»Â¸Â¥ Â°Â¡Â°Ã Â»Ã³Â½Ã‚ 
            if ((m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x00F00000) != 0)
            {
                dwSWEType = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x00F00000) >> 20;
                dwSWEValue = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x000F0000) >> 16;

                // ÃˆÃ±Â±Ã Â¾Ã†Ã€ÃŒÃ…Ã› ÃˆÂ¿Â°Ãº ÃÂ¾Â·Ã¹: 
                // 0-None 1-Ã‡ÃŠÂ»Ã¬Â±Ã¢Â´Ã«Â¹ÃŒÃÃ¶ÃƒÃŸÂ°Â¡ 2-ÃÃŸÂµÂ¶ÃˆÂ¿Â°Ãº 3-ÃÂ¤Ã€Ã‡Ã€Ã‡ 4-Ã€ÃºÃÃ–Ã€Ã‡ 
                // 5-Â¹ÃŽÃƒÂ¸Ã€Ã‡ 6-Â°Â¡ÂºÂ­Â¿Ã® 7-Â¿Â¹Â¸Â®Ã‡Ã‘ 8-Â°Â­ÃˆÂ­ÂµÃˆ 9-Â°Ã­Â´Ã«Â¹Â®Â¸Ã­Ã€Ã‡
                switch (dwSWEType)
                {
                    case 6: dwMul1 = 2; break;  // Â°Â¡ÂºÂ­Â¿Ã® 
                    case 8: dwMul1 = 2; break;  // Â°Â­ÃˆÂ­ÂµÃˆ
                    case 5: dwMul1 = 3; break;  // Â¹ÃŽÃƒÂ¸Ã€Ã‡
                    case 1: dwMul1 = 4; break;  // Ã‡ÃŠÂ»Ã¬Ã€Ã‡ 
                    case 7: dwMul1 = 5; break;  // Â¿Â¹Â¸Â®Ã‡Ã‘
                    case 2: dwMul1 = 6; break;  // ÃÃŸÂµÂ¶Ã€Ã‡
                    case 3: dwMul1 = 15; break; // ÃÂ¤Ã€Ã‡Ã€Ã‡ 
                    case 9: dwMul1 = 20; break; // Â°Ã­Â´Ã«Â¹Â®Â¸Ã­ 
                    default: dwMul1 = 1; break;
                }

                d1 = (double)iPrice * dwMul1;
                switch (dwSWEValue)
                {
                    case 1: d2 = 10.0f; break;
                    case 2: d2 = 20.0f; break;
                    case 3: d2 = 30.0f; break;
                    case 4: d2 = 35.0f; break;
                    case 5: d2 = 40.0f; break;
                    case 6: d2 = 50.0f; break;
                    case 7: d2 = 100.0f; break;
                    case 8: d2 = 200.0f; break;
                    case 9: d2 = 300.0f; break;
                    case 10: d2 = 400.0f; break;
                    case 11: d2 = 500.0f; break;
                    case 12: d2 = 700.0f; break;
                    case 13: d2 = 900.0f; break;
                    default: d2 = 0.0f; break;
                }
                d3 = d1 * (d2 / 100.0f);
                dwAddPrice1 = (int)(d1 + d3);
            }

            // v1.42 ÃˆÃ±Â±Ã Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒÂ¶Ã³Â¸Ã© Sub ÃˆÂ¿Â°ÃºÂ¸Â¦ Â¼Â³ÃÂ¤Ã‡Ã‘Â´Ã™. Â°Ã¸Â°ÃÂ¹Â«Â±Ã¢Â´Ã‚ 1Â°Â³Â¸Â¸ Ã€Ã¥Ã‚Ã¸ÂµÃˆÂ´Ã™Â°Ã­ Ã‡ÃŸÃ€Â»Â¶Â§Â¸Â¸ Ã€Â¯ÃˆÂ¿Ã‡Ã”.
            if ((m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x0000F000) != 0)
            {
                dwSWEType = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x0000F000) >> 12;
                dwSWEValue = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x00000F00) >> 8;

                // ÃˆÃ±Â±Ã Â¾Ã†Ã€ÃŒÃ…Ã› ÃˆÂ¿Â°Ãº ÃÂ¾Â·Ã¹: 
                //ÃƒÃŸÂ°Â¡ ÂµÂ¶Â¼ÂºÃ€ÃºÃ‡Ã—(1), ÃƒÃŸÂ°Â¡ Â¸Ã­ÃÃŸÂ°Âª(2), ÃƒÃŸÂ°Â¡ Â¹Ã¦Â¾Ã®Â°Âª(3), HP ÃˆÂ¸ÂºÂ¹Â·Â® ÃƒÃŸÂ°Â¡(4), SP ÃˆÂ¸ÂºÂ¹Â·Â® ÃƒÃŸÂ°Â¡(5)
                //MP ÃˆÂ¸ÂºÂ¹Â·Â® ÃƒÃŸÂ°Â¡(6), ÃƒÃŸÂ°Â¡ Â¸Â¶Â¹Ã½Ã€ÃºÃ‡Ã—(7), Â¹Â°Â¸Â® Â´Ã«Â¹ÃŒÃÃ¶ ÃˆÃ­Â¼Ã¶(8), Â¸Â¶Â¹Ã½ Â´Ã«Â¹ÃŒÃÃ¶ ÃˆÃ­Â¼Ã¶(9)
                //Â¿Â¬Ã…Â¸ Â´Ã«Â¹ÃŒÃÃ¶ ÃƒÃŸÂ°Â¡(10), Â´Ãµ Â¸Â¹Ã€Âº Â°Ã¦Ã‡Ã¨Ã„Â¡(11), Â´ÃµÂ¸Â¹Ã€Âº Gold(12)
                switch (dwSWEType)
                {
                    case 1:
                    case 12: dwMul2 = 2; break;

                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7: dwMul2 = 4; break;

                    case 8:
                    case 9:
                    case 10:
                    case 11: dwMul2 = 6; break;
                }

                d1 = (double)iPrice * dwMul2;
                switch (dwSWEValue)
                {
                    case 1: d2 = 10.0f; break;
                    case 2: d2 = 20.0f; break;
                    case 3: d2 = 30.0f; break;
                    case 4: d2 = 35.0f; break;
                    case 5: d2 = 40.0f; break;
                    case 6: d2 = 50.0f; break;
                    case 7: d2 = 100.0f; break;
                    case 8: d2 = 200.0f; break;
                    case 9: d2 = 300.0f; break;
                    case 10: d2 = 400.0f; break;
                    case 11: d2 = 500.0f; break;
                    case 12: d2 = 700.0f; break;
                    case 13: d2 = 900.0f; break;
                    default: d2 = 0.0f; break;
                }
                d3 = d1 * (d2 / 100.0f);
                dwAddPrice2 = (int)(d1 + d3);
            }

            iPrice = iPrice + (dwAddPrice1 - (dwAddPrice1 / 3)) + (dwAddPrice2 - (dwAddPrice2 / 3));

            if (bNeutral == true) iPrice = iPrice / 2;
            if (iPrice <= 0) iPrice = 1;
            if (iPrice > 1000000) iPrice = 1000000; // New 06/05/2004

            SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMSOLD, cItemID, 0, 0, 0);

            _bItemLog(DEF_ITEMLOG_SELL, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[cItemID]);

            if ((m_pClientList[iClientH]->m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
                (m_pClientList[iClientH]->m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_ARROW))
            {
                SetItemCount(iClientH, cItemID, m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwCount - iNum);
            }
            else ItemDepleteHandler(iClientH, cItemID, false);
        }
    }
    else
        if ((cItemCategory >= 11) && (cItemCategory <= 50))
        {
            iPrice = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice / 2;
            iPrice = iPrice * iNum;

            if (bNeutral == true) iPrice = iPrice / 2;
            if (iPrice <= 0) iPrice = 1;
            if (iPrice > 1000000) iPrice = 1000000;

            SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMSOLD, cItemID, 0, 0, 0);

            _bItemLog(DEF_ITEMLOG_SELL, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[cItemID]);

            if ((m_pClientList[iClientH]->m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
                (m_pClientList[iClientH]->m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_ARROW))
            {
                SetItemCount(iClientH, cItemID, m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwCount - iNum);
            }
            else ItemDepleteHandler(iClientH, cItemID, false);
        }

    if (iPrice <= 0) return;

    pItemGold = new class CItem;
    memset(cItemName, 0, sizeof(cItemName));
    copy_string(cItemName, "Gold");
    _bInitItemAttr(pItemGold, cItemName);

    pItemGold->m_dwCount = iPrice;

    if (_bAddClientItemList(iClientH, pItemGold, &iEraseReq) == true)
    {
        dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
        *dwp = MSGID_NOTIFY;
        wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
        *wp = DEF_NOTIFY_ITEMOBTAINED;

        cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);

        *cp = 1;
        cp++;

        memcpy(cp, pItemGold->m_cName, 20);
        cp += 20;

        dwp = (uint32_t *)cp;
        *dwp = pItemGold->m_dwCount;
        cp += 4;

        *cp = pItemGold->m_cItemType;
        cp++;

        *cp = pItemGold->m_cEquipPos;
        cp++;

        *cp = (char)0;
        cp++;

        sp = (short *)cp;
        *sp = pItemGold->m_sLevelLimit;
        cp += 2;

        *cp = pItemGold->m_cGenderLimit;
        cp++;

        wp = (uint16_t *)cp;
        *wp = pItemGold->m_wCurLifeSpan;
        cp += 2;

        wp = (uint16_t *)cp;
        *wp = pItemGold->m_wWeight;
        cp += 2;

        sp = (short *)cp;
        *sp = pItemGold->m_sSprite;
        cp += 2;

        sp = (short *)cp;
        *sp = pItemGold->m_sSpriteFrame;
        cp += 2;

        *cp = pItemGold->m_cItemColor;
        cp++;

        *cp = (char)pItemGold->m_sItemSpecEffectValue2;
        cp++;

        dwp = (uint32_t *)cp;
        *dwp = pItemGold->m_dwAttribute;
        cp += 4;

        if (iEraseReq == 1)
            delete pItemGold;

        iRet = m_pClientList[iClientH]->send_msg(cData, 53);

        iCalcTotalWeight(iClientH);

        switch (iRet)
        {
            case DEF_XSOCKEVENT_QUENEFULL:
            case DEF_XSOCKEVENT_SOCKETERROR:
            case DEF_XSOCKEVENT_CRITICALERROR:
            case DEF_XSOCKEVENT_SOCKETCLOSED:
                DeleteClient(iClientH, true, true);
                break;
        }
    }
    else
    {
        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
            m_pClientList[iClientH]->m_sY, pItemGold);

        SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
            m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
            pItemGold->m_sSprite, pItemGold->m_sSpriteFrame, pItemGold->m_cItemColor);

        iCalcTotalWeight(iClientH);

        dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
        *dwp = MSGID_NOTIFY;
        wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
        *wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;

        iRet = m_pClientList[iClientH]->send_msg(cData, 6);
        switch (iRet)
        {
            case DEF_XSOCKEVENT_QUENEFULL:
            case DEF_XSOCKEVENT_SOCKETERROR:
            case DEF_XSOCKEVENT_CRITICALERROR:
            case DEF_XSOCKEVENT_SOCKETCLOSED:
                DeleteClient(iClientH, true, true);
                return;
        }
    }
}

void CGame::ReqRepairItemHandler(int iClientH, char cItemID, char cRepairWhom, char * pString)
{
    char cItemCategory;
    short sRemainLife, sPrice;
    double d1, d2, d3;

    // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â°Ã­Ã„Â¡Â°ÃšÂ´Ã™Â´Ã‚ Â¿Ã¤Â±Â¸.
    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    if ((cItemID < 0) || (cItemID >= 50)) return;
    if (m_pClientList[iClientH]->m_pItemList[cItemID] == 0) return;

    cItemCategory = m_pClientList[iClientH]->m_pItemList[cItemID]->m_cCategory;

    // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ ÃÂ¾Â·Ã¹Â¿Â¡ ÂµÃ»Â¶Ã³ Â°Â¡Â°Ã Â°Ã¨Â»Ãª.
    if ((cItemCategory >= 1) && (cItemCategory <= 10))
    {
        // Â¹Â«Â±Ã¢Â·Ã¹Â´Ã™

        // Â¸Â¸Â¾Ã  Â¹Â«Â±Ã¢Â¸Â¦ Â´Ã«Ã€Ã¥Â°Â£ ÃÃ–Ã€ÃŽÃ€ÃŒ Â¾Ã†Â´Ã‘ Ã€ÃŒÂ¿Â¡Â°Ã” Â°Ã­ÃƒÃ„Â´ÃžÂ¶Ã³Â°Ã­ Ã‡Ã‘Â´Ã™Â¸Ã© Ã‡Ã’ Â¼Ã¶ Â¾Ã¸Â´Ã™. 
        if (cRepairWhom != 24)
        {
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_CANNOTREPAIRITEM, cItemID, 2, 0, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
            return;
        }

        // Â¿Ã¸Â·Â¡ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ Â¼Ã¶Â¸Ã­Â°Ãº ÂºÃ±Â±Â³Ã‡Ã˜Â¼Â­ Â°Â¨Â°Â¡ Â»Ã³Â°Â¢Ã€Â» Â°Ã¨Â»Ãª, Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ Â¼Ã¶Â¸Â® ÂºÃ±Â¿Ã«Ã€Â» Â¸Ã…Â±Ã¤Â´Ã™.
        sRemainLife = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wCurLifeSpan;
        if (sRemainLife == 0)
        {
            // Â¿ÃÃ€Ã¼ÃˆÃ· Â¸ÃÂ°Â¡ÃÃ¸ Â°ÃÃ€ÃŒÂ¶Ã³Â¸Ã© Â¿Ã¸Â·Â¡Â°Â¡Â°ÃÃ€Ã‡ Ã€Ã½Â¹ÃÃ€ÃŒ ÂµÃ§Â´Ã™. 
            sPrice = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice / 2;
        }
        else
        {
            d1 = (double)sRemainLife;
            if (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan != 0)
                d2 = (double)m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan;
            else d2 = 1.0f;
            d3 = (d1 / d2) * 0.5f;
            d2 = (double)m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice; // Â¿Ã¸Â·Â¡ Â°Â¡Â°Ã 
            d3 = d3 * d2; // ÃƒÃŸÂ»ÃªÂµÃˆ Â°Â¡Â°Ã 

            sPrice = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice / 2) - (short)d3;
        }

        SendNotifyMsg(0, iClientH, DEF_NOTIFY_REPAIRITEMPRICE, cItemID, sRemainLife, sPrice, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
    }
    else if (((cItemCategory >= 43) && (cItemCategory <= 50)) || ((cItemCategory >= 11) && (cItemCategory <= 12)))
    {
        // Â³Â¬Â½ÃƒÂ´Ã«, Â°Ã®Â±ÂªÃ€ÃŒ ÂµÃ®Â°Ãº Â°Â°Ã€Âº ÂºÃ±Â¹Â«Â±Ã¢Â·Ã¹ Â¼Ã¶Â¸Â®Â°Â¡Â´Ã‰ Â¾Ã†Ã€ÃŒÃ…Ã›. Â¿ÃŠ, ÂºÃŽÃƒÃ·Â·Ã¹

        // Â¸Â¸Â¾Ã  Â»Ã³ÃÂ¡ ÃÃ–Ã€ÃŽÃ€ÃŒ Â¾Ã†Â´Ã‘ Ã€ÃŒÂ¿Â¡Â°Ã” Â°Ã­ÃƒÃ„Â´ÃžÂ¶Ã³Â°Ã­ Ã‡Ã‘Â´Ã™Â¸Ã© Ã‡Ã’ Â¼Ã¶ Â¾Ã¸Â´Ã™. 
        if (cRepairWhom != 15)
        {
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_CANNOTREPAIRITEM, cItemID, 2, 0, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
            return;
        }

        // Â¿Ã¸Â·Â¡ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ Â¼Ã¶Â¸Ã­Â°Ãº ÂºÃ±Â±Â³Ã‡Ã˜Â¼Â­ Â°Â¨Â°Â¡ Â»Ã³Â°Â¢Ã€Â» Â°Ã¨Â»Ãª, Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ Â¼Ã¶Â¸Â® ÂºÃ±Â¿Ã«Ã€Â» Â¸Ã…Â±Ã¤Â´Ã™.
        sRemainLife = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wCurLifeSpan;
        if (sRemainLife == 0)
        {
            // Â¿ÃÃ€Ã¼ÃˆÃ· Â¸ÃÂ°Â¡ÃÃ¸ Â°ÃÃ€ÃŒÂ¶Ã³Â¸Ã© Â¿Ã¸Â·Â¡Â°Â¡Â°ÃÃ€Ã‡ Ã€Ã½Â¹ÃÃ€ÃŒ ÂµÃ§Â´Ã™. 
            sPrice = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice / 2;
        }
        else
        {
            d1 = (double)sRemainLife;
            if (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan != 0)
                d2 = (double)m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan;
            else d2 = 1.0f;
            d3 = (d1 / d2) * 0.5f;
            d2 = (double)m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice; // Â¿Ã¸Â·Â¡ Â°Â¡Â°Ã 
            d3 = d3 * d2; // ÃƒÃŸÂ»ÃªÂµÃˆ Â°Â¡Â°Ã 

            sPrice = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice / 2) - (short)d3;
        }

        SendNotifyMsg(0, iClientH, DEF_NOTIFY_REPAIRITEMPRICE, cItemID, sRemainLife, sPrice, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
    }
    else
    {
        // Â°Ã­Ã„Â¥Â¼Ã¶ Â¾Ã¸Â´Ã‚ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒÂ´Ã™.
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_CANNOTREPAIRITEM, cItemID, 1, 0, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
    }
}

void CGame::ReqRepairItemCofirmHandler(int iClientH, char cItemID, char * pString)
{
    short    sRemainLife, sPrice;
    char * cp, cItemCategory, cData[120];
    double   d1, d2, d3;
    uint32_t * dwp, dwGoldCount;
    uint16_t * wp;
    int      iRet, iGoldWeight;

    // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â¼Ã¶Â¸Â®Ã‡ÃÂ°ÃšÂ´Ã™Â´Ã‚ Â°ÃÃ€ÃŒ Â°Ã¡ÃÂ¤ÂµÃ‡Â¾ÃºÂ´Ã™.
    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

    if ((cItemID < 0) || (cItemID >= 50)) return;
    if (m_pClientList[iClientH]->m_pItemList[cItemID] == 0) return;

    // New 18/05/2004
    if (m_pClientList[iClientH]->m_pIsProcessingAllowed == false) return;

    //testcode
    //log->info("Repair!");

    cItemCategory = m_pClientList[iClientH]->m_pItemList[cItemID]->m_cCategory;

    // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ ÃÂ¾Â·Ã¹Â¿Â¡ ÂµÃ»Â¶Ã³ Â°Â¡Â°Ã Â°Ã¨Â»Ãª.
    if (((cItemCategory >= 1) && (cItemCategory <= 10)) || ((cItemCategory >= 43) && (cItemCategory <= 50)) ||
        ((cItemCategory >= 11) && (cItemCategory <= 12)))
    {
        // Â¹Â«Â±Ã¢Â·Ã¹ ÃˆÂ¤Ã€Âº Â³Â¬Â½ÃƒÂ´Ã«, Â°Ã®Â±ÂªÃ€ÃŒÂ¿Ã Â°Â°Ã€Âº Â¾Ã†Ã€ÃŒÃ…Ã›, Â¿ÃŠ, Â½Ã…Â¹ÃŸ 

        // Â¿Ã¸Â·Â¡ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ Â¼Ã¶Â¸Ã­Â°Ãº ÂºÃ±Â±Â³Ã‡Ã˜Â¼Â­ Â°Â¨Â°Â¡ Â»Ã³Â°Â¢Ã€Â» Â°Ã¨Â»Ãª, Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ Â¼Ã¶Â¸Â® ÂºÃ±Â¿Ã«Ã€Â» Â¸Ã…Â±Ã¤Â´Ã™.
        sRemainLife = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wCurLifeSpan;
        if (sRemainLife == 0)
        {
            // Â¿ÃÃ€Ã¼ÃˆÃ· Â¸ÃÂ°Â¡ÃÃ¸ Â°ÃÃ€ÃŒÂ¶Ã³Â¸Ã© Â¿Ã¸Â·Â¡Â°Â¡Â°ÃÃ€Ã‡ Ã€Ã½Â¹ÃÃ€ÃŒ ÂµÃ§Â´Ã™. 
            sPrice = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice / 2;
        }
        else
        {
            d1 = (double)abs(sRemainLife);
            if (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan != 0)
                d2 = (double)abs(m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan);
            else d2 = 1.0f;
            d3 = (d1 / d2) * 0.5f;
            d2 = (double)m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice; // Â¿Ã¸Â·Â¡ Â°Â¡Â°Ã 
            d3 = d3 * d2; // ÃƒÃŸÂ»ÃªÂµÃˆ Â°Â¡Â°Ã 

            sPrice = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice / 2) - (short)d3;
        }

        // sPriceÂ¸Â¸Ã…Â­Ã€Ã‡ ÂµÂ·Ã€ÃŒ ÂµÃ‡Â¸Ã© Â°Ã­Ã„Â¥ Â¼Ã¶ Ã€Ã–Ã€Â¸Â³Âª ÂºÃŽÃÂ·Ã‡ÃÂ¸Ã© Â°Ã­Ã„Â¥ Â¼Ã¶ Â¾Ã¸Â´Ã™. 
        dwGoldCount = dwGetItemCount(iClientH, "Gold");

        if (dwGoldCount < (uint32_t)sPrice)
        {
            // Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â°Â¡ Â°Â®Â°Ã­Ã€Ã–Â´Ã‚ GoldÂ°Â¡ Â¾Ã†Ã€ÃŒÃ…Ã› Â¼Ã¶Â¸Â® ÂºÃ±Â¿Ã«Â¿Â¡ ÂºÃ±Ã‡Ã˜ Ã€Ã»Â´Ã™. Â°Ã­Ã„Â¥ Â¼Ã¶ Â¾Ã¸Ã€Â½.
            dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
            *dwp = MSGID_NOTIFY;
            wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
            *wp = DEF_NOTIFY_NOTENOUGHGOLD;
            cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);
            *cp = cItemID;
            cp++;

            iRet = m_pClientList[iClientH]->send_msg(cData, 7);
            switch (iRet)
            {
                case DEF_XSOCKEVENT_QUENEFULL:
                case DEF_XSOCKEVENT_SOCKETERROR:
                case DEF_XSOCKEVENT_CRITICALERROR:
                case DEF_XSOCKEVENT_SOCKETCLOSED:
                    // Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ ÂºÂ¸Â³Â¾Â¶Â§ Â¿Â¡Â·Â¯Â°Â¡ Â¹ÃŸÂ»Ã½Ã‡ÃŸÂ´Ã™Â¸Ã© ÃÂ¦Â°Ã…Ã‡Ã‘Â´Ã™.
                    DeleteClient(iClientH, true, true);
                    return;
            }
            return;
        }
        else
        {
            //ÂµÂ·Ã€ÃŒ ÃƒÃ¦ÂºÃÃ‡ÃÂ´Ã™. Â°Ã­Ã„Â¥ Â¼Ã¶ Ã€Ã–Â´Ã™. 

            // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ Â¼Ã¶Â¸Ã­Ã€Â» Â´ÃƒÂ¸Â®Â°Ã­ Ã…Ã«ÂºÂ¸Ã‡Ã‘Â´Ã™. !BUG POINT Ã€Â§Ã„Â¡Â°Â¡ ÃÃŸÂ¿Ã¤Ã‡ÃÂ´Ã™. Â¸Ã•Ã€Ãº Â¼Ã¶Â¸Ã­Ã€Â» Â´ÃƒÂ¸Â®Â°Ã­ ÂµÂ·Ã€Ã‡ Ã„Â«Â¿Ã®Ã†Â®Â¸Â¦ Â³Â·ÃƒÃ¡Â´Ã™.
            m_pClientList[iClientH]->m_pItemList[cItemID]->m_wCurLifeSpan = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan;
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMREPAIRED, cItemID, m_pClientList[iClientH]->m_pItemList[cItemID]->m_wCurLifeSpan, 0, 0);

            iGoldWeight = SetItemCount(iClientH, "Gold", dwGoldCount - sPrice);

            // Â¼Ã’ÃÃ¶Ã‡Â° ÃƒÃ‘ ÃÃŸÂ·Â® Ã€Ã§ Â°Ã¨Â»Ãª 
            iCalcTotalWeight(iClientH);

            //v1.4 Â¸Â¶Ã€Â»Ã€Ã‡ Ã€ÃšÂ±ÃÂ¿Â¡ Â´ÃµÃ‡Ã‘Â´Ã™. 
            m_stCityStatus[m_pClientList[iClientH]->m_cSide].iFunds += sPrice;
        }
    }
    else
    {
        // Â°Ã­Ã„Â¥ Ã‡ÃŠÂ¿Ã¤Â°Â¡ Â¾Ã¸Â´Ã‚ Â¾Ã†Ã€ÃŒÃ…Ã› 
        // Ã‡Ã˜Ã…Â·Ã€ÃŒÂ³Âª Â¹Ã¶Â±Ã—Â¿Â¡ Ã€Ã‡Ã‡Ã‘ Â°ÃÃ€ÃÂµÃ­ 
    }
}

void CGame::NoticeHandler()
{
    char  cTemp, cBuffer[1000], cKey;
    uint32_t dwSize, dwTime = timeGetTime();
    int i, iMsgIndex, iTemp;

    // Â°Ã¸ÃÃ¶Â»Ã§Ã‡Ã—Ã€ÃŒ 1Â°Â³ Ã€ÃŒÃ‡ÃÂ¶Ã³Â¸Ã© ÂºÂ¸Â³Â¾ Ã‡ÃŠÂ¿Ã¤Â°Â¡ Â¾Ã¸Â´Ã™.
    if (m_iTotalNoticeMsg <= 1) return;

    if ((dwTime - m_dwNoticeTime) > DEF_NOTICETIME)
    {
        // Â°Ã¸ÃÃ¶Â»Ã§Ã‡Ã—Ã€Â» Ã€Ã¼Â¼Ã›Ã‡Ã’ Â½ÃƒÂ°Â£Ã€ÃŒ ÂµÃ‡Â¾ÃºÂ´Ã™. 
        m_dwNoticeTime = dwTime;
        do
        {
            iMsgIndex = iDice(1, m_iTotalNoticeMsg) - 1;
        } while (iMsgIndex == m_iPrevSendNoticeMsg);

        m_iPrevSendNoticeMsg = iMsgIndex;

        memset(cBuffer, 0, sizeof(cBuffer));
        if (m_pNoticeMsgList[iMsgIndex] != 0)
        {
            m_pNoticeMsgList[iMsgIndex]->Get(&cTemp, cBuffer, &dwSize, &iTemp, &cKey);
        }

        for (i = 1; i < DEF_MAXCLIENTS; i++)
            if (m_pClientList[i] != 0)
            {
                SendNotifyMsg(0, i, DEF_NOTIFY_NOTICEMSG, 0, 0, 0, cBuffer);
            }
    }
}

void CGame::ResponseSavePlayerDataReplyHandler(char * pData, uint32_t dwMsgSize)
{
    char * cp, cCharName[11];
    int i;

    memset(cCharName, 0, sizeof(cCharName));

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);
    memcpy(cCharName, cp, 10);

    // Ã€ÃŒÃÂ¦ Ã€ÃŒ Ã€ÃŒÂ¸Â§Ã€Â» Â°Â®Â´Ã‚ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¸Â¦ ÃƒÂ£Â¾Ã† ÃÂ¢Â¼Ã“Ã€Â» Â²Ã·Â¾Ã®ÂµÂµ ÃÃÂ´Ã™Â´Ã‚ Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ ÂºÂ¸Â³Â½Â´Ã™. 
    for (i = 0; i < DEF_MAXCLIENTS; i++)
        if (m_pClientList[i] != 0)
        {
            if (memcmp(m_pClientList[i]->m_cCharName, cCharName, 10) == 0)
            {
                // Â°Â°Ã€Âº Ã€ÃŒÂ¸Â§Ã€Â» Â°Â¡ÃÃ¸ Ã„Â³Â¸Â¯Ã…ÃÂ¸Â¦ ÃƒÂ£Â¾Ã’Â´Ã™. ÃÂ¢Â¼Ã“Ã€Â» ÃÂ¾Â·Ã¡Ã‡ÃÂ¶Ã³Â´Ã‚ Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ ÂºÂ¸Â³Â½Â´Ã™.
                SendNotifyMsg(0, i, DEF_NOTIFY_SERVERCHANGE, 0, 0, 0, 0);
            }
        }
}

void CGame::ReqGetFishThisTimeHandler(int iClientH)
{
    int iResult, iFishH;
    class CItem * pItem;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    if (m_pClientList[iClientH]->m_iAllocatedFish == 0) return;
    if (m_pFish[m_pClientList[iClientH]->m_iAllocatedFish] == 0) return;

    // ³¬½Ã ½ºÅ³ »ç¿ëÁßÀÓÀ» ÇØÁ¦½ÃÅ²´Ù.
    m_pClientList[iClientH]->m_bSkillUsingStatus[1] = false;

    iResult = iDice(1, 100);
    if (m_pClientList[iClientH]->m_iFishChance >= iResult)
    {
        // ÀÌ ¹°°í±â¸¦ ³¬´Âµ¥ ¼º°øÇÏ¿´´Ù!

        // °æÇèÄ¡ Áõ°¡ 
        GetExp(iClientH, iDice(m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_iDifficulty, 5)); //m_pClientList[iClientH]->m_iExpStock += iDice(m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_iDifficulty, 5);
        // ½ºÅ³ Áõ°¡ 
        CalculateSSN_SkillIndex(iClientH, 1, m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_iDifficulty);

        // ¾ÆÀÌÅÛ Æ÷ÀÎÅÍ¸¦ ¾ò¾î¿Â´Ù.
        pItem = m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_pItem;
        m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_pItem = 0;

        // ³¬Àº ¾ÆÀÌÅÛÀ» ¹Ù´Ú¿¡ ¶³¾î¶ß¸°´Ù.
        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
            m_pClientList[iClientH]->m_sY,
            pItem);

        // ´Ù¸¥ Å¬¶óÀÌ¾ðÆ®¿¡°Ô ¾ÆÀÌÅÛÀÌ ¶³¾îÁø °ÍÀ» ¾Ë¸°´Ù. 
        SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
            m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
            pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4 color

        // ³¬½Ã ¼º°ø ¸Þ½ÃÁö Àü¼Û 
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_FISHSUCCESS, 0, 0, 0, 0);
        iFishH = m_pClientList[iClientH]->m_iAllocatedFish;
        m_pClientList[iClientH]->m_iAllocatedFish = 0;

        // ¹°°í±â¸¦ Áö¿ì°í ¸®ÅÏ 
        bDeleteFish(iFishH, 1); // <- ¿©±â¼­ ´Ù¸¥ ³¬½Ã²Ûµé¿¡°Ô ¸Þ½ÃÁö°¡ Àü¼ÛµÉ °ÍÀÌ´Ù.
        return;
    }

    // ³¬´Âµ¥ ½ÇÆÐ! 
    m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_sEngagingCount--;
    SendNotifyMsg(0, iClientH, DEF_NOTIFY_FISHFAIL, 0, 0, 0, 0);

    m_pClientList[iClientH]->m_iAllocatedFish = 0;
}

void CGame::SpecialEventHandler()
{
    uint32_t dwTime;

    // Ã†Â¯ÂºÂ°Ã‡Ã‘ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ Â»Ã½Â¼ÂºÃ‡Ã‘Â´Ã™. 
    dwTime = timeGetTime();

    if ((dwTime - m_dwSpecialEventTime) < DEF_SPECIALEVENTTIME) return; // DEF_SPECIALEVENTTIME
    m_dwSpecialEventTime = dwTime;
    m_bIsSpecialEventTime = true;

    switch (iDice(1, 180))
    {
        case 98: m_cSpecialEventType = 2; break; // ÂµÂ¥Â¸Ã³ ÃˆÂ¤Ã€Âº Ã€Â¯Â´ÃÃ„ÃœÃ€ÃŒ Â³ÂªÂ¿Ãƒ Â°Â¡Â´Ã‰Â¼ÂºÃ€Âº 30ÂºÃÂ¿Â¡ 1Â¹Ã¸ 1/30
        default: m_cSpecialEventType = 1; break;
    }
}

void CGame::ToggleSafeAttackModeHandler(int iClientH) //v1.1
{
    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    if (m_pClientList[iClientH]->m_bIsKilled == true) return;

    // Â¾ÃˆÃ€Ã¼ Â°Ã¸Â°Ã Â¸Ã°ÂµÃ¥Â¸Â¦ Ã…Ã¤Â±Ã›Ã‡Ã‘Â´Ã™. 
    if (m_pClientList[iClientH]->m_bIsSafeAttackMode == true)
        m_pClientList[iClientH]->m_bIsSafeAttackMode = false;
    else m_pClientList[iClientH]->m_bIsSafeAttackMode = true;

    SendNotifyMsg(0, iClientH, DEF_NOTIFY_SAFEATTACKMODE, 0, 0, 0, 0);
}

void CGame::ReqCreatePortionHandler(int iClientH, char * pData)
{
    uint32_t * dwp;
    uint16_t * wp;
    char * cp, cI[6], cPortionName[21], cData[120];
    int    iRet, i, j, iEraseReq, iSkillLimit, iSkillLevel, iResult, iDifficulty;
    short * sp, sItemIndex[6], sTemp;
    short  sItemNumber[6], sItemArray[12];
    bool   bDup, bFlag;
    class  CItem * pItem;

    if (m_pClientList[iClientH] == nullptr) return;
    m_pClientList[iClientH]->m_iSkillMsgRecvCount++;

    for (i = 0; i < 6; i++)
    {
        cI[i] = -1;
        sItemIndex[i] = -1;
        sItemNumber[i] = 0;
    }

    cp = (char *)(pData + 11);
    cI[0] = *cp;
    cp++;
    cI[1] = *cp;
    cp++;
    cI[2] = *cp;
    cp++;
    cI[3] = *cp;
    cp++;
    cI[4] = *cp;
    cp++;
    cI[5] = *cp;
    cp++;
    // Ã†Ã·Â¼Ã‡Ã€Ã‡ Ã€Ã§Â·Ã¡ Ã€ÃŽÂµÂ¦Â½ÂºÂ¸Â¦ Â¹ÃžÂ¾Ã’Â´Ã™. Ã€ÃŒ Ã€Ã§Â·Ã¡Â°Â¡ Ã†Ã·Â¼Ã‡Ã€Â» Â¸Â¸ÂµÃ© Â¼Ã¶ Ã€Ã–Â´Ã‚ ÃÂ¶Ã‡Ã•Ã€ÃŽÃÃ¶ ÃˆÂ®Ã€ÃŽÃ‡Ã‘Â´Ã™. 

    // ÂµÂ¥Ã€ÃŒÃ…ÃÂ°Â¡ Ã€Â¯ÃˆÂ¿Ã‡Ã‘ Â¾Ã†Ã€ÃŒÃ…Ã› Ã€ÃŽÂµÂ¦Â½ÂºÃ€ÃŽÃÃ¶ ÃƒÂ¼Ã…Â©Ã‡Ã‘Â´Ã™.
    for (i = 0; i < 6; i++)
    {
        if (cI[i] >= DEF_MAXITEMS) return;
        if ((cI[i] >= 0) && (m_pClientList[iClientH]->m_pItemList[cI[i]] == 0)) return;
    }

    for (i = 0; i < 6; i++)
        if (cI[i] >= 0)
        {
            // Â¸Ã•Ã€Ãº Ã€ÃŒÂ¹ÃŒ Ã€Ã–Â´Ã‚ Â¸Â®Â½ÂºÃ†Â®Ã€ÃŽÃÃ¶ Â°Ã‹Â»Ã¶ 
            bDup = false;
            for (j = 0; j < 6; j++)
                if (sItemIndex[j] == cI[i])
                {
                    // Ã€Ã–Â´Ã™. Ã„Â«Â¿Ã®Ã†Â® ÃÃµÂ°Â¡ 
                    sItemNumber[j]++;
                    bDup = true;
                }
            if (bDup == false)
            {
                // Â¾Ã¸Â´Ã™. Â»ÃµÂ·ÃŽ ÃƒÃŸÂ°Â¡Ã‡Ã‘Â´Ã™.
                for (j = 0; j < 6; j++)
                    if (sItemIndex[j] == -1)
                    {
                        sItemIndex[j] = cI[i];
                        sItemNumber[j]++;
                        goto RCPH_LOOPBREAK;
                    }
                RCPH_LOOPBREAK:;
            }
        }

    // Â¾Ã†Ã€ÃŒÃ…Ã› Â¸Â®Â½ÂºÃ†Â®Â°Â¡ Â¸Â¸ÂµÃ©Â¾Ã® ÃÂ³Â´Ã™. Â¼Ã’ÂºÃ±ÂµÃ‡Â´Ã‚ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒÂ¶Ã³Â¸Ã© Â°Â¹Â¼Ã¶Â¸Â¦ ÃˆÂ®Ã€ÃŽÃ‡Ã‘Â´Ã™. 
    for (i = 0; i < 6; i++)
        if (sItemIndex[i] != -1)
        {
            if (sItemIndex[i] < 0) return;
            if ((sItemIndex[i] >= 0) && (sItemIndex[i] >= DEF_MAXITEMS)) return;
            if (m_pClientList[iClientH]->m_pItemList[sItemIndex[i]] == 0) return;
            // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â°Â¹Â¼Ã¶Â°Â¡ Â¿Ã€Â¹Ã¶Ã‡Ã˜ÂµÂµ Â¸Â®Ã…Ã.
            if (m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_dwCount < sItemNumber[i]) return;
        }

    // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â¾Ã†Ã€ÃŒÃ…Ã› Â¾Ã†Ã€ÃŒÂµÃ° Â¹Ã¸ÃˆÂ£Â°Â¡ Ã…Â« Â¼Ã¸Â¼Â­ÂºÃŽÃ…Ã ÃÂ¤Â·Ã„Ã‡Ã‘Â´Ã™. Bubble Sort
    bFlag = true;
    while (bFlag == true)
    {
        bFlag = false;
        for (i = 0; i < 5; i++)
            if ((sItemIndex[i] != -1) && (sItemIndex[i + 1] != -1))
            {
                if ((m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_sIDnum) <
                    (m_pClientList[iClientH]->m_pItemList[sItemIndex[i + 1]]->m_sIDnum))
                {
                    // Â¹Ã™Â²Ã›Â´Ã™.
                    sTemp = sItemIndex[i + 1];
                    sItemIndex[i + 1] = sItemIndex[i];
                    sItemIndex[i] = sTemp;
                    sTemp = sItemNumber[i + 1];
                    sItemNumber[i + 1] = sItemNumber[i];
                    sItemNumber[i] = sTemp;
                    bFlag = true;
                }
            }
    }

    j = 0;
    for (i = 0; i < 6; i++)
    {
        if (sItemIndex[i] != -1)
            sItemArray[j] = m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_sIDnum;
        else sItemArray[j] = sItemIndex[i];
        sItemArray[j + 1] = sItemNumber[i];
        j += 2;
    }

    memset(cPortionName, 0, sizeof(cPortionName));

    for (i = 0; i < DEF_MAXPORTIONTYPES; i++)
        if (m_pPortionConfigList[i] != 0)
        {
            bFlag = false;
            for (j = 0; j < 12; j++)
                if (m_pPortionConfigList[i]->m_sArray[j] != sItemArray[j]) bFlag = true;

            if (bFlag == false)
            {
                memset(cPortionName, 0, sizeof(cPortionName));
                memcpy(cPortionName, m_pPortionConfigList[i]->m_cName, 20);
                iSkillLimit = m_pPortionConfigList[i]->m_iSkillLimit;
                iDifficulty = m_pPortionConfigList[i]->m_iDifficulty;
            }
        }

    if (strlen(cPortionName) == 0)
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_NOMATCHINGPORTION, 0, 0, 0, 0);
        return;
    }

    iSkillLevel = m_pClientList[iClientH]->m_cSkillMastery[12];
    if (iSkillLimit > iSkillLevel)
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_LOWPORTIONSKILL, 0, 0, 0, cPortionName);
        return;
    }

    iSkillLevel -= iDifficulty;
    if (iSkillLevel <= 0) iSkillLevel = 1;

    iResult = iDice(1, 100);
    if (iResult > iSkillLevel)
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_PORTIONFAIL, 0, 0, 0, cPortionName);
        return;
    }

    CalculateSSN_SkillIndex(iClientH, 12, 1);

    if (strlen(cPortionName) != 0)
    {
        pItem = 0;
        pItem = new class CItem;
        if (pItem == 0) return;

        for (i = 0; i < 6; i++)
            if (sItemIndex[i] != -1)
            {
                if (m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_CONSUME)
                    // v1.41 !!!
                    SetItemCount(iClientH, sItemIndex[i], //     m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_cName,
                        m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_dwCount - sItemNumber[i]);
                else ItemDepleteHandler(iClientH, sItemIndex[i], false);
            }

        SendNotifyMsg(0, iClientH, DEF_NOTIFY_PORTIONSUCCESS, 0, 0, 0, cPortionName);
        m_pClientList[iClientH]->m_iExpStock += iDice(1, (iDifficulty / 3));

        if ((_bInitItemAttr(pItem, cPortionName) == true))
        {
            if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true)
            {
                memset(cData, 0, sizeof(cData));
                dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
                *dwp = MSGID_NOTIFY;
                wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
                *wp = DEF_NOTIFY_ITEMOBTAINED;

                cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);

                *cp = 1;
                cp++;

                memcpy(cp, pItem->m_cName, 20);
                cp += 20;

                dwp = (uint32_t *)cp;
                *dwp = pItem->m_dwCount;	// Â¼Ã¶Â·Â®Ã€Â» Ã€Ã”Â·Ã‚ 
                cp += 4;

                *cp = pItem->m_cItemType;
                cp++;

                *cp = pItem->m_cEquipPos;
                cp++;

                *cp = (char)0;
                cp++;

                sp = (short *)cp;
                *sp = pItem->m_sLevelLimit;
                cp += 2;

                *cp = pItem->m_cGenderLimit;
                cp++;

                wp = (uint16_t *)cp;
                *wp = pItem->m_wCurLifeSpan;
                cp += 2;

                wp = (uint16_t *)cp;
                *wp = pItem->m_wWeight;
                cp += 2;

                sp = (short *)cp;
                *sp = pItem->m_sSprite;
                cp += 2;

                sp = (short *)cp;
                *sp = pItem->m_sSpriteFrame;
                cp += 2;

                *cp = pItem->m_cItemColor;
                cp++;

                *cp = (char)pItem->m_sItemSpecEffectValue2; // v1.41 
                cp++;

                dwp = (uint32_t *)cp;
                *dwp = pItem->m_dwAttribute;
                cp += 4;
                /*
                *cp = (char)(pItem->m_dwAttribute & 0x00000001); // Custom-ItemÃ€ÃŽÃÃ¶Ã€Ã‡ Â¿Â©ÂºÃŽ
                cp++;
                */

                if (iEraseReq == 1) delete pItem;

                iRet = m_pClientList[iClientH]->send_msg(cData, 53);
                switch (iRet)
                {
                    case DEF_XSOCKEVENT_QUENEFULL:
                    case DEF_XSOCKEVENT_SOCKETERROR:
                    case DEF_XSOCKEVENT_CRITICALERROR:
                    case DEF_XSOCKEVENT_SOCKETCLOSED:
                        DeleteClient(iClientH, true, true);
                        break;
                }

                //if ((pItem->m_wPrice * pItem->m_dwCount) > 1000) 
                //	SendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA, iClientH);
            }
            else
            {
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
                    m_pClientList[iClientH]->m_sY, pItem);

                SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
                    m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
                    pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4

                dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
                *dwp = MSGID_NOTIFY;
                wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
                *wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;

                iRet = m_pClientList[iClientH]->send_msg(cData, 6);
                switch (iRet)
                {
                    case DEF_XSOCKEVENT_QUENEFULL:
                    case DEF_XSOCKEVENT_SOCKETERROR:
                    case DEF_XSOCKEVENT_CRITICALERROR:
                    case DEF_XSOCKEVENT_SOCKETCLOSED:
                        DeleteClient(iClientH, true, true);
                        break;
                }
            }
        }
        else
        {
            delete pItem;
            pItem = 0;
        }
    }
}

void CGame::SetDownSkillIndexHandler(int iClientH, int iSkillIndex)
{
    if (m_pClientList[iClientH] == nullptr) return;
    if ((iSkillIndex < 0) || (iSkillIndex >= DEF_MAXSKILLTYPE)) return;

    if (m_pClientList[iClientH]->m_cSkillMastery[iSkillIndex] > 0)
        m_pClientList[iClientH]->m_iDownSkillIndex = iSkillIndex;

    // Â´Ã™Â¿Ã® Â½ÂºÃ…Â³Ã€Â» Â¼Â³ÃÂ¤Ã‡ÃŸÃ€Â¸Â´Ã Ã€Ã€Â´Ã¤Ã€Â» ÂºÂ¸Â³Â½Â´Ã™. 
    SendNotifyMsg(0, iClientH, DEF_NOTIFY_DOWNSKILLINDEXSET, m_pClientList[iClientH]->m_iDownSkillIndex, 0, 0, 0);
}

void CGame::GetOccupyFlagHandler(int iClientH)
{
    int   i, iNum, iRet, iEraseReq, iEKNum;
    char * cp, cData[256], cItemName[21];
    class CItem * pItem;
    uint32_t * dwp;
    short * sp;
    uint16_t * wp;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_iEnemyKillCount < 3) return;
    if (m_pClientList[iClientH]->m_cSide == 0) return;

    memset(cItemName, 0, sizeof(cItemName));
    switch (m_pClientList[iClientH]->m_cSide)
    {
        case 1: strcpy(cItemName, "Â¾Ã†Â·Â¹Â½ÂºÂµÂ§Â±ÃªÂ¹ÃŸ"); break;
        case 2: strcpy(cItemName, "Â¿Â¤Â¹Ã™Ã€ÃŽÂ±ÃªÂ¹ÃŸ");   break;
    }

    // ReqPurchaseItemHandlerÂ¿Â¡Â¼Â­ Â°Â¡ÃÂ®Â¿Ã‚ Â·Ã§Ã†Â¾Ã€Â» Â°Ã­ÃƒÃ†Ã€Â½.
    iNum = 1;
    for (i = 1; i <= iNum; i++)
    {

        pItem = new class CItem;
        if (_bInitItemAttr(pItem, cItemName) == false)
        {
            // Â±Â¸Ã€Ã”Ã‡ÃÂ°Ã­Ã€Ãš Ã‡ÃÂ´Ã‚ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¾Ã†Ã€ÃŒÃ…Ã› Â¸Â®Â½ÂºÃ†Â®Â»Ã³Â¿Â¡ Â¾Ã¸Â´Ã™. Â±Â¸Ã€Ã”Ã€ÃŒ ÂºÃ’Â°Â¡Â´Ã‰Ã‡ÃÂ´Ã™.
            delete pItem;
        }
        else
        {

            if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true)
            {
                // Â¿Â¡Â·Â¯ Â¹Ã¦ÃÃ¶Â¿Ã« Ã„ÃšÂµÃ¥
                if (m_pClientList[iClientH]->m_iCurWeightLoad < 0) m_pClientList[iClientH]->m_iCurWeightLoad = 0;

                // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â¹ÃžÃ€Â» Â¼Ã¶ Ã€Ã–Â´Ã™Â´Ã‚ Â°ÃÃ€ÃŒ ÃˆÂ®ÃÂ¤ ÂµÃ‡Â¾ÃºÃ€Â¸Â¹Ã‡Â·ÃŽ EK Â°ÂªÃ€Â» Â»Â©ÂµÂµ ÂµÃˆÂ´Ã™.
                if (m_pClientList[iClientH]->m_iEnemyKillCount > 12)
                {
                    iEKNum = 12;
                    m_pClientList[iClientH]->m_iEnemyKillCount -= 12;
                }
                else
                {
                    iEKNum = m_pClientList[iClientH]->m_iEnemyKillCount;
                    m_pClientList[iClientH]->m_iEnemyKillCount = 0;
                }

                pItem->m_sItemSpecEffectValue1 = iEKNum;

                log->info("(*) Get Flag : Char({}) Flag-EK({}) Player-EK({})", m_pClientList[iClientH]->m_cCharName, iEKNum, m_pClientList[iClientH]->m_iEnemyKillCount);

                dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
                *dwp = MSGID_NOTIFY;
                wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
                *wp = DEF_NOTIFY_ITEMOBTAINED;

                cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);
                *cp = 1;
                cp++;

                memcpy(cp, pItem->m_cName, 20);
                cp += 20;

                dwp = (uint32_t *)cp;
                *dwp = pItem->m_dwCount;
                cp += 4;

                *cp = pItem->m_cItemType;
                cp++;

                *cp = pItem->m_cEquipPos;
                cp++;

                *cp = (char)0;
                cp++;

                sp = (short *)cp;
                *sp = pItem->m_sLevelLimit;
                cp += 2;

                *cp = pItem->m_cGenderLimit;
                cp++;

                wp = (uint16_t *)cp;
                *wp = pItem->m_wCurLifeSpan;
                cp += 2;

                wp = (uint16_t *)cp;
                *wp = pItem->m_wWeight;
                cp += 2;

                sp = (short *)cp;
                *sp = pItem->m_sSprite;
                cp += 2;

                sp = (short *)cp;
                *sp = pItem->m_sSpriteFrame;
                cp += 2;

                *cp = pItem->m_cItemColor;
                cp++;

                *cp = (char)pItem->m_sItemSpecEffectValue2; 
                cp++;

                dwp = (uint32_t *)cp;
                *dwp = pItem->m_dwAttribute;
                cp += 4;

                if (iEraseReq == 1) delete pItem;

                iRet = m_pClientList[iClientH]->send_msg(cData, 53);

                iCalcTotalWeight(iClientH);

                SendNotifyMsg(0, iClientH, DEF_NOTIFY_ENEMYKILLS, m_pClientList[iClientH]->m_iEnemyKillCount, 0, 0, 0);
            }
            else
            {
                delete pItem;

                iCalcTotalWeight(iClientH);

                dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
                *dwp = MSGID_NOTIFY;
                wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
                *wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;

                iRet = m_pClientList[iClientH]->send_msg(cData, 6);
            }
        }
    }
}

void CGame::GetFightzoneTicketHandler(int iClientH)
{
    int   iRet, iEraseReq, iMonth, iDay, iHour;
    char * cp, cData[256], cItemName[21];
    class CItem * pItem;
    uint32_t * dwp;
    short * sp;
    uint16_t * wp;

    if (m_pClientList[iClientH] == nullptr) return;

    if (m_pClientList[iClientH]->m_iFightZoneTicketNumber <= 0)
    {
        // Ã€Ã”Ã€Ã¥Â±Ã‡Ã€Â» Â´Ã™ Â»Ã§Â¿Ã«Ã‡ÃŸÃ€Â½Ã€Â» Â¾Ã‹Â¸Â°Â´Ã™.
        // Â»Ã§Ã…ÃµÃ€Ã¥ Â¹Ã¸ÃˆÂ£Â°Â¡ Ã€Â½Â¼Ã¶Â¸Ã© Â¿Â¹Â¾Ã Ã€Âº Ã‡ÃŸÂ´Ã‚ÂµÂ¥ Ã€Ã”Ã€Ã¥Â±Ã‡Ã€Â» Â´Ã™ Â¹ÃžÃ€ÂºÂ°Ã¦Â¿Ã¬ ..
        m_pClientList[iClientH]->m_iFightzoneNumber *= -1;
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_FIGHTZONERESERVE, -1, 0, 0, 0);
        return;
    }

    memset(cItemName, 0, sizeof(cItemName));

    if (m_pClientList[iClientH]->m_iFightzoneNumber == 1)
        strcpy(cItemName, "ArenaTicket");
    else  copy_string(cItemName, "ArenaTicket(%d)", m_pClientList[iClientH]->m_iFightzoneNumber);

    pItem = new class CItem;
    if (_bInitItemAttr(pItem, cItemName) == false)
    {
        delete pItem;
        return;
    }

    if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true)
    {
        if (m_pClientList[iClientH]->m_iCurWeightLoad < 0) m_pClientList[iClientH]->m_iCurWeightLoad = 0;

        m_pClientList[iClientH]->m_iFightZoneTicketNumber = m_pClientList[iClientH]->m_iFightZoneTicketNumber - 1;

        pItem->m_sTouchEffectType = DEF_ITET_DATE;

        iMonth = m_pClientList[iClientH]->m_iReserveTime / 10000;
        iDay = (m_pClientList[iClientH]->m_iReserveTime - iMonth * 10000) / 100;
        iHour = m_pClientList[iClientH]->m_iReserveTime - iMonth * 10000 - iDay * 100;

        pItem->m_sTouchEffectValue1 = iMonth;
        pItem->m_sTouchEffectValue2 = iDay;
        pItem->m_sTouchEffectValue3 = iHour;

        log->info("(*) Get FIGHTZONETICKET : Char({}) TICKENUMBER ({})({})({})", m_pClientList[iClientH]->m_cCharName, pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3);

        memset(cData, 0, sizeof(cData));

        dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
        *dwp = MSGID_NOTIFY;
        wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
        *wp = DEF_NOTIFY_ITEMOBTAINED;

        cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);
        *cp = 1;
        cp++;

        memcpy(cp, pItem->m_cName, 20);
        cp += 20;

        dwp = (uint32_t *)cp;
        *dwp = pItem->m_dwCount;
        cp += 4;

        *cp = pItem->m_cItemType;
        cp++;

        *cp = pItem->m_cEquipPos;
        cp++;

        *cp = (char)0;
        cp++;

        sp = (short *)cp;
        *sp = pItem->m_sLevelLimit;
        cp += 2;

        *cp = pItem->m_cGenderLimit;
        cp++;

        wp = (uint16_t *)cp;
        *wp = pItem->m_wCurLifeSpan;
        cp += 2;

        wp = (uint16_t *)cp;
        *wp = pItem->m_wWeight;
        cp += 2;

        sp = (short *)cp;
        *sp = pItem->m_sSprite;
        cp += 2;

        sp = (short *)cp;
        *sp = pItem->m_sSpriteFrame;
        cp += 2;

        *cp = pItem->m_cItemColor;
        cp++;

        *cp = (char)pItem->m_sItemSpecEffectValue2;
        cp++;

        dwp = (uint32_t *)cp;
        *dwp = pItem->m_dwAttribute;
        cp += 4;

        if (iEraseReq == 1) delete pItem;

        iRet = m_pClientList[iClientH]->send_msg(cData, 53);

        iCalcTotalWeight(iClientH);
    }
    else
    {
        delete pItem;

        iCalcTotalWeight(iClientH);

        dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
        *dwp = MSGID_NOTIFY;
        wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
        *wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;

        iRet = m_pClientList[iClientH]->send_msg(cData, 6);
    }
}

void CGame::ExchangeItemHandler(int iClientH, short sItemIndex, int iAmount, short dX, short dY, uint16_t wObjectID, char * pItemName)
{
    short sOwnerH;
    char  cOwnerType;

    if (m_pClientList[iClientH] == nullptr) return;
    if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return;
    if ((m_bAdminSecurity == true) && (m_pClientList[iClientH]->m_iAdminUserLevel > 0)) return;
    if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == 0) return;
    if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount < iAmount) return;
    if (m_pClientList[iClientH]->m_bIsOnServerChange == true) return;
    if (m_pClientList[iClientH]->m_bIsExchangeMode == true) return;
    if (wObjectID >= DEF_MAXCLIENTS) return;

    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);


    if ((sOwnerH != 0) && (cOwnerType == DEF_OWNERTYPE_PLAYER))
    {

        if ((m_bAdminSecurity == true) && (m_pClientList[sOwnerH]->m_iAdminUserLevel > 0))
        {
            return;
        }

        if (wObjectID != 0)
        {
            if (wObjectID < 10000)
            {
                if (m_pClientList[wObjectID] != 0)
                {
                    if ((uint16_t)sOwnerH != wObjectID) sOwnerH = 0;
                }
            }
            else sOwnerH = 0;
        }

        if ((sOwnerH == 0) || (m_pClientList[sOwnerH] == 0))
        { 
            _ClearExchangeStatus(iClientH);
        }
        else
        {
            if ((m_pClientList[sOwnerH]->m_bIsExchangeMode == true) || (m_pClientList[sOwnerH]->m_sAppr2 & 0xF000) ||
                (m_pMapList[m_pClientList[sOwnerH]->m_cMapIndex]->m_bIsFightZone == true))
            { 
                _ClearExchangeStatus(iClientH);
            }
            else
            {
                m_pClientList[iClientH]->m_bIsExchangeMode = true;
                m_pClientList[iClientH]->m_iExchangeH = sOwnerH;
                memset(m_pClientList[iClientH]->m_cExchangeName, 0, sizeof(m_pClientList[iClientH]->m_cExchangeName));
                strcpy(m_pClientList[iClientH]->m_cExchangeName, m_pClientList[sOwnerH]->m_cCharName);

                //Clear items in the list
                m_pClientList[iClientH]->iExchangeCount = 0;
                m_pClientList[sOwnerH]->iExchangeCount = 0;
                for (int i = 0; i < 4; i++)
                {
                    //Clear the trader
                    memset(m_pClientList[iClientH]->m_cExchangeItemName[i], 0, sizeof(m_pClientList[iClientH]->m_cExchangeItemName[i]));
                    m_pClientList[iClientH]->m_cExchangeItemIndex[i] = -1;
                    m_pClientList[iClientH]->m_iExchangeItemAmount[i] = 0;
                    //Clear the guy we're trading with
                    memset(m_pClientList[sOwnerH]->m_cExchangeItemName[i], 0, sizeof(m_pClientList[sOwnerH]->m_cExchangeItemName[i]));
                    m_pClientList[sOwnerH]->m_cExchangeItemIndex[i] = -1;
                    m_pClientList[sOwnerH]->m_iExchangeItemAmount[i] = 0;
                }

                m_pClientList[iClientH]->m_cExchangeItemIndex[m_pClientList[iClientH]->iExchangeCount] = (char)sItemIndex;
                m_pClientList[iClientH]->m_iExchangeItemAmount[m_pClientList[iClientH]->iExchangeCount] = iAmount;

                //memset(m_pClientList[iClientH]->m_cExchangeItemName, 0, sizeof(m_pClientList[iClientH]->m_cExchangeItemName));
                memcpy(m_pClientList[iClientH]->m_cExchangeItemName[m_pClientList[iClientH]->iExchangeCount], m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cName, 20);

                m_pClientList[sOwnerH]->m_bIsExchangeMode = true;
                m_pClientList[sOwnerH]->m_iExchangeH = iClientH;
                memset(m_pClientList[sOwnerH]->m_cExchangeName, 0, sizeof(m_pClientList[sOwnerH]->m_cExchangeName));
                strcpy(m_pClientList[sOwnerH]->m_cExchangeName, m_pClientList[iClientH]->m_cCharName);

                m_pClientList[iClientH]->iExchangeCount++;
                SendNotifyMsg(iClientH, iClientH, DEF_NOTIFY_OPENEXCHANGEWINDOW, sItemIndex + 1000, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSprite,
                    m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpriteFrame, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cName, iAmount, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor,
                    m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan,
                    m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wMaxLifeSpan,
                    m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2 + 100,
                    m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute);

                SendNotifyMsg(iClientH, sOwnerH, DEF_NOTIFY_OPENEXCHANGEWINDOW, sItemIndex, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSprite,
                    m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpriteFrame, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cName, iAmount, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor,
                    m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan,
                    m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wMaxLifeSpan,
                    m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2 + 100,
                    m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute);
            }
        }
    }
    else
    {
        _ClearExchangeStatus(iClientH);
    }
}

void CGame::QuestAcceptedHandler(int iClientH)
{
    int iIndex;

    if (m_pClientList[iClientH] == nullptr) return;

    // Does the quest exist ??
    if (m_pQuestConfigList[m_pClientList[iClientH]->m_iAskedQuest] == 0) return;

    if (m_pQuestConfigList[m_pClientList[iClientH]->m_iAskedQuest]->m_iAssignType == 1)
    {
        // Ã€Ã¼Â¸Ã©Ã€Ã¼Â¿Ã« Ã„Ã¹Â½ÂºÃ†Â®Ã€ÃŒÂ´Ã™. 
        switch (m_pQuestConfigList[m_pClientList[iClientH]->m_iAskedQuest]->m_iType)
        {
            case 10: // Ã…ÃšÂ·Â¹Ã†Ã·Ã†Â®Â¿Ã« 1ÃˆÂ¸Â¼Âº Ã„Ã¹Â½ÂºÃ†Â®. Ã…ÃšÂ·Â¹Ã†Ã·Ã†Â®Ã‡ÃÂ°Ã­ Â³ÂªÂ¸Ã© Â³Â¡Ã€ÃŒÂ´Ã™. Â´ÃµÃ€ÃŒÂ»Ã³Ã€Ã‡ Ã„Ã¹Â½ÂºÃ†Â® Â¿Â©ÂºÃŽÂ°Â¡ Â¾Ã¸Ã€Â½.
                _ClearQuestStatus(iClientH);
                RequestTeleportHandler(iClientH, "2   ", m_pQuestConfigList[m_pClientList[iClientH]->m_iAskedQuest]->m_cTargetName,
                    m_pQuestConfigList[m_pClientList[iClientH]->m_iAskedQuest]->m_sX, m_pQuestConfigList[m_pClientList[iClientH]->m_iAskedQuest]->m_sY);
                return;
        }
    }

    // Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â°Â¡ Ã„Ã¹Â½ÂºÃ†Â®Â¸Â¦ Â¼Ã¶Â¶Ã´Ã‡ÃŸÂ´Ã™. Ã„Ã¹Â½ÂºÃ†Â® Â¹Ã¸ÃˆÂ£, IDÂ°Âª Ã‡Ã’Â´Ã§.
    m_pClientList[iClientH]->m_iQuest = m_pClientList[iClientH]->m_iAskedQuest;
    iIndex = m_pClientList[iClientH]->m_iQuest;
    m_pClientList[iClientH]->m_iQuestID = m_pQuestConfigList[iIndex]->m_iQuestID;
    m_pClientList[iClientH]->m_iCurQuestCount = 0;
    m_pClientList[iClientH]->m_bIsQuestCompleted = false;

    _CheckQuestEnvironment(iClientH);
    _SendQuestContents(iClientH);
}

void CGame::GetHeroMantleHandler(int iClientH, int iItemID, char * pString)
{
    int   i, iNum, iRet, iEraseReq;
    char * cp, cData[256], cItemName[21];
    class CItem * pItem;
    uint32_t * dwp;
    short * sp;
    uint16_t * wp;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_iEnemyKillCount < 100) return;
    if (m_pClientList[iClientH]->m_cSide == 0) return;
    if (_iGetItemSpaceLeft(iClientH) == 0)
    {
        SendItemNotifyMsg(iClientH, DEF_NOTIFY_CANNOTCARRYMOREITEM, 0, 0);
        return;
    }

    //Prevents a crash if item dosent exist
    if (m_pItemConfigList[iItemID] == 0)  return;

    switch (iItemID)
    {
        // Hero Cape
        case 400: //Aresden HeroCape
        case 401: //Elvine HeroCape
            if (m_pClientList[iClientH]->m_iEnemyKillCount < 300) return;
            m_pClientList[iClientH]->m_iEnemyKillCount -= 300;
            break;

            // Hero Helm
        case 403: //Aresden HeroHelm(M)
        case 404: //Aresden HeroHelm(W)
        case 405: //Elvine HeroHelm(M)
        case 406: //Elvine HeroHelm(W)
            if (m_pClientList[iClientH]->m_iEnemyKillCount < 150) return;
            m_pClientList[iClientH]->m_iEnemyKillCount -= 150;
            if (m_pClientList[iClientH]->m_iContribution < 20) return;
            m_pClientList[iClientH]->m_iContribution -= 20;
            break;

            // Hero Cap
        case 407: //Aresden HeroCap(M)
        case 408: //Aresden HeroCap(W)
        case 409: //Elvine HeroHelm(M)
        case 410: //Elvine HeroHelm(W)
            if (m_pClientList[iClientH]->m_iEnemyKillCount < 100) return;
            m_pClientList[iClientH]->m_iEnemyKillCount -= 100;
            if (m_pClientList[iClientH]->m_iContribution < 20) return;
            m_pClientList[iClientH]->m_iContribution -= 20;
            break;

            // Hero Armour
        case 411: //Aresden HeroArmour(M)
        case 412: //Aresden HeroArmour(W)
        case 413: //Elvine HeroArmour(M)
        case 414: //Elvine HeroArmour(W)
            if (m_pClientList[iClientH]->m_iEnemyKillCount < 300) return;
            m_pClientList[iClientH]->m_iEnemyKillCount -= 300;
            if (m_pClientList[iClientH]->m_iContribution < 30) return;
            m_pClientList[iClientH]->m_iContribution -= 30;
            break;

            // Hero Robe
        case 415: //Aresden HeroRobe(M)
        case 416: //Aresden HeroRobe(W)
        case 417: //Elvine HeroRobe(M)
        case 418: //Elvine HeroRobe(W)
            if (m_pClientList[iClientH]->m_iEnemyKillCount < 200) return;
            m_pClientList[iClientH]->m_iEnemyKillCount -= 200;
            if (m_pClientList[iClientH]->m_iContribution < 20) return;
            m_pClientList[iClientH]->m_iContribution -= 20;
            break;

            // Hero Hauberk
        case 419: //Aresden HeroHauberk(M)
        case 420: //Aresden HeroHauberk(W)
        case 421: //Elvine HeroHauberk(M)
        case 422: //Elvine HeroHauberk(W)
            if (m_pClientList[iClientH]->m_iEnemyKillCount < 100) return;
            m_pClientList[iClientH]->m_iEnemyKillCount -= 100;
            if (m_pClientList[iClientH]->m_iContribution < 10) return;
            m_pClientList[iClientH]->m_iContribution -= 10;
            break;

            // Hero Leggings
        case 423: //Aresden HeroLeggings(M)
        case 424: //Aresden HeroLeggings(W)
        case 425: //Elvine HeroLeggings(M)
        case 426: //Elvine HeroLeggings(W)
            if (m_pClientList[iClientH]->m_iEnemyKillCount < 150) return;
            m_pClientList[iClientH]->m_iEnemyKillCount -= 150;
            if (m_pClientList[iClientH]->m_iContribution < 15) return;
            m_pClientList[iClientH]->m_iContribution -= 15;
            break;

        default:
            return;
            break;
    }

    memset(cItemName, 0, sizeof(cItemName));
    memcpy(cItemName, m_pItemConfigList[iItemID]->m_cName, 20);
    // ReqPurchaseItemHandler
    iNum = 1;
    for (i = 1; i <= iNum; i++)
    {
        pItem = new class CItem;
        if (_bInitItemAttr(pItem, cItemName) == false)
        {
            delete pItem;
        }
        else
        {

            if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true)
            {
                if (m_pClientList[iClientH]->m_iCurWeightLoad < 0) m_pClientList[iClientH]->m_iCurWeightLoad = 0;

                log->info("(*) Get HeroItem : Char({}) Player-EK({}) Player-Contr({}) Hero Obtained({})", m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_iEnemyKillCount, m_pClientList[iClientH]->m_iContribution, cItemName);

                pItem->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
                pItem->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
                pItem->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
                pItem->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;

                dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
                *dwp = MSGID_NOTIFY;
                wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
                *wp = DEF_NOTIFY_ITEMOBTAINED;
                cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);

                *cp = 1;
                cp++;

                memcpy(cp, pItem->m_cName, 20);
                cp += 20;

                dwp = (uint32_t *)cp;
                *dwp = pItem->m_dwCount;
                cp += 4;

                *cp = pItem->m_cItemType;
                cp++;

                *cp = pItem->m_cEquipPos;
                cp++;

                *cp = (char)0;
                cp++;

                sp = (short *)cp;
                *sp = pItem->m_sLevelLimit;
                cp += 2;

                *cp = pItem->m_cGenderLimit;
                cp++;

                wp = (uint16_t *)cp;
                *wp = pItem->m_wCurLifeSpan;
                cp += 2;

                wp = (uint16_t *)cp;
                *wp = pItem->m_wWeight;
                cp += 2;

                sp = (short *)cp;
                *sp = pItem->m_sSprite;
                cp += 2;

                sp = (short *)cp;
                *sp = pItem->m_sSpriteFrame;
                cp += 2;

                *cp = pItem->m_cItemColor;
                cp++;

                *cp = (char)pItem->m_sItemSpecEffectValue2;
                cp++;

                dwp = (uint32_t *)cp;
                *dwp = pItem->m_dwAttribute;
                cp += 4;

                if (iEraseReq == 1) delete pItem;

                iRet = m_pClientList[iClientH]->send_msg(cData, 53);

                iCalcTotalWeight(iClientH);

                SendNotifyMsg(0, iClientH, DEF_NOTIFY_ENEMYKILLS, m_pClientList[iClientH]->m_iEnemyKillCount, 0, 0, 0);
            }
            else
            {
                delete pItem;

                iCalcTotalWeight(iClientH);

                dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
                *dwp = MSGID_NOTIFY;
                wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
                *wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;

                iRet = m_pClientList[iClientH]->send_msg(cData, 6);
            }
        }
    }
}

void CGame::JoinPartyHandler(int iClientH, int iV1, char * pMemberName)
{
    char * cp, cData[120];
    short sAppr2;
    uint32_t * dwp;
    uint16_t * wp;
    int i;

    if (m_pClientList[iClientH] == nullptr) return;
    if ((m_bAdminSecurity == true) && (m_pClientList[iClientH]->m_iAdminUserLevel > 0)) return;

    switch (iV1)
    {
        case 0: // Ã†Ã„Ã†Â¼ Ã…Â»Ã…Ã° Â½Ã…ÃƒÂ»
            RequestDeletePartyHandler(iClientH);
            break;

        case 1: // Ã†Ã„Ã†Â¼ Â°Â¡Ã€Ã” Â½Ã…ÃƒÂ»
            //testcode
            copy_string(G_cTxt, "Join Party Req: %s(%d) ID(%d) Stat(%d) ReqJoinH(%d) ReqJoinName(%s)", m_pClientList[iClientH]->m_cCharName, iClientH,
                m_pClientList[iClientH]->m_iPartyID, m_pClientList[iClientH]->m_iPartyStatus, m_pClientList[iClientH]->m_iReqJoinPartyClientH,
                m_pClientList[iClientH]->m_cReqJoinPartyName);
            log->info(G_cTxt);

            if ((m_pClientList[iClientH]->m_iPartyID != 0) || (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_NULL))
            {
                // Ã€ÃŒÂ¹ÃŒ Ã†Ã„Ã†Â¼Â¿Â¡ Â°Â¡Ã€Ã”Ã‡Ã˜ Ã€Ã–Â´Ã™Â¸Ã© Ã†Ã„Ã†Â¼ Â°Â¡Ã€Ã” Â½Ã…ÃƒÂ»Ã€Â» Ã‡Ã’ Â¼Ã¶ Â¾Ã¸Â´Ã™.
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_PARTY, 7, 0, 0, 0);
                m_pClientList[iClientH]->m_iReqJoinPartyClientH = 0;
                memset(m_pClientList[iClientH]->m_cReqJoinPartyName, 0, sizeof(m_pClientList[iClientH]->m_cReqJoinPartyName));
                m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
                //testcode
                log->info("Join Party Reject (1)");
                return;
            }

            for (i = 1; i < DEF_MAXCLIENTS; i++)
                if ((m_pClientList[i] != 0) && (strcmp(m_pClientList[i]->m_cCharName, pMemberName) == 0))
                {
                    // Â°Â°Ã€Âº Ã€ÃŒÂ¸Â§Ã€Â» Â°Â¡ÃÃ¸ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â¸Â¦ ÃƒÂ£Â¾Ã’Â´Ã™.
                    sAppr2 = (short)((m_pClientList[i]->m_sAppr2 & 0xF000) >> 12);
                    if (sAppr2 != 0)
                    {
                        // Ã€Ã¼Ã…Ãµ Â¸Ã°ÂµÃ¥ Â»Ã³Ã…Ã‚Ã€Ã‡ Â»Ã³Â´Ã«Â¿Â¡Â°Ã”Â´Ã‚ Ã†Ã„Ã†Â¼ Â°Â¡Ã€Ã” Â½Ã…ÃƒÂ»Ã€Â» Ã‡Ã’ Â¼Ã¶ Â¾Ã¸Â´Ã™.
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_PARTY, 7, 0, 0, 0);
                        //testcode
                        log->info("Join Party Reject (2)");
                    }
                    else if (m_pClientList[i]->m_cSide != m_pClientList[iClientH]->m_cSide)
                    {
                        // Ã†Ã­Ã€ÃŒ Â´Ã™Â¸Â£Â¸Ã© Ã†Ã„Ã†Â¼Â¿Â¡ ÂµÃ© Â¼Ã¶ Â¾Ã¸Â´Ã™.
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_PARTY, 7, 0, 0, 0);
                        //testcode
                        log->info("Join Party Reject (3)");
                    }
                    else if (m_pClientList[i]->m_iPartyStatus == DEF_PARTYSTATUS_PROCESSING)
                    {
                        // Ã†Ã„Ã†Â¼ Â°Â¡Ã€Ã”Ã€Â» Â½Ã…ÃƒÂ»Ã‡Ã‘ Â´Ã«Â»Ã³Ã€ÃŒ Ã€ÃŒÂ¹ÃŒ Â´Ã™Â¸Â¥ Ã†Ã„Ã†Â¼ Â°Â¡Ã€Ã” Â°Ã¼Â·Ãƒ ÃƒÂ³Â¸Â®Â¸Â¦ Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã™. Â½Ã…ÃƒÂ» ÂºÃ’Â°Â¡.
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_PARTY, 7, 0, 0, 0);
                        //testcode
                        log->info("Join Party Reject (4)");
                        //testcode
                        copy_string(G_cTxt, "Party join reject(2) ClientH:%d ID:%d JoinName:%d", i, m_pClientList[i]->m_iPartyID, m_pClientList[i]->m_cReqJoinPartyName);
                        log->info(G_cTxt);

                        m_pClientList[iClientH]->m_iReqJoinPartyClientH = 0;
                        memset(m_pClientList[iClientH]->m_cReqJoinPartyName, 0, sizeof(m_pClientList[iClientH]->m_cReqJoinPartyName));
                        m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
                    }
                    else
                    {
                        // Â°Â¡Ã€Ã” Â½Ã‚Ã€ÃŽ Â¿Â©ÂºÃŽÂ¸Â¦ Â¹Â¯Â´Ã‚Â´Ã™.
                        m_pClientList[i]->m_iReqJoinPartyClientH = iClientH;
                        memset(m_pClientList[i]->m_cReqJoinPartyName, 0, sizeof(m_pClientList[i]->m_cReqJoinPartyName));
                        strcpy(m_pClientList[i]->m_cReqJoinPartyName, m_pClientList[iClientH]->m_cCharName);
                        SendNotifyMsg(0, i, DEF_NOTIFY_QUERY_JOINPARTY, 0, 0, 0, m_pClientList[iClientH]->m_cCharName);

                        // Â½Ã…ÃƒÂ»Ã‡Ã‘ ÃƒÃ¸Â¿Â¡Â´Ã‚ Ã†Ã„Ã†Â¼ Â¸Â¶Â½ÂºÃ…ÃÂ°Â¡ ÂµÃ‰ Ã„Â³Â¸Â¯Ã…ÃÃ€Ã‡ Ã€ÃŽÂµÂ¦Â½ÂºÂ¸Â¦ Â³Ã–Â¾Ã®ÃÃœ. ÃƒÃ«Â¼Ã’Ã‡Ã’Â¶Â§ ÃƒÂ³Â¸Â®Ã‡ÃÂ±Ã¢ Ã€Â§Ã‡Ã”Ã€Ã“.
                        m_pClientList[iClientH]->m_iReqJoinPartyClientH = i;
                        memset(m_pClientList[iClientH]->m_cReqJoinPartyName, 0, sizeof(m_pClientList[iClientH]->m_cReqJoinPartyName));
                        strcpy(m_pClientList[iClientH]->m_cReqJoinPartyName, m_pClientList[i]->m_cCharName);
                        // Ã†Ã„Ã†Â¼ Â»Ã³Ã…Ã‚ Â¼Â¼Ã†Â®
                        m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_PROCESSING;
                    }
                    return;
                }
            break;

        case 2: // Ã†Ã„Ã†Â¼ Â¸Ã¢Â¹Ã¶ ÃˆÂ®Ã€ÃŽ Â¸Ã­Â·Ã‰ 
            if (m_pClientList[iClientH]->m_iPartyStatus == DEF_PARTYSTATUS_CONFIRM)
            {
                memset(cData, 0, sizeof(cData));
                cp = (char *)cData;
                dwp = (uint32_t *)cp;
                *dwp = MSGID_PARTYOPERATION;
                cp += 4;
                wp = (uint16_t *)cp;
                *wp = 6; // Ã†Ã„Ã†Â¼ Â¸Ã¢Â¹Ã¶ Â¸Â®Â½ÂºÃ†Â® Â¿Ã¤ÃƒÂ»
                cp += 2;
                wp = (uint16_t *)cp;
                *wp = iClientH;
                cp += 2;
                memcpy(cp, m_pClientList[iClientH]->m_cCharName, 10);
                cp += 10;
                wp = (uint16_t *)cp;
                *wp = m_pClientList[iClientH]->m_iPartyID;
                cp += 2;
                SendMsgToGateServer(MSGID_PARTYOPERATION, iClientH, cData);
            }
            break;
    }
}

void CGame::LoteryHandler(int iClientH)
{
    class  CItem * pItem;
    int     iItemID;
    if (m_pClientList[iClientH] == nullptr) return;
    switch (iDice(1, 22))
    {
        case 1:iItemID = 656; break; // XelimaStone
        case 2:iItemID = 657; break; // MerienStone
        case 3:iItemID = 650; break; // ZemstoneOfSacrifice
        case 4:iItemID = 652; break; // RedBall
        case 5:iItemID = 654; break; // BlueBall
        case 6:iItemID = 881; break; // ArmorDye(Indigo)
        case 7:iItemID = 882; break; // ArmorDye(CrimsonRed)
        case 8:iItemID = 883; break; // ArmorDye(Gold)
        case 9:iItemID = 884; break; // ArmorDye(Aqua)
        case 10:iItemID = 885; break; // ArmorDye(Pink)
        case 11:iItemID = 886; break; // ArmorDye(Violet)
        case 12:iItemID = 887; break; // ArmorDye(Blue) 
        case 13:iItemID = 888; break; // ArmorDye(Khaki) 
        case 14:iItemID = 889; break; // ArmorDye(Yellow) 
        case 15:iItemID = 890; break; // ArmorDye(Red) 
        case 16:iItemID = 971; break; // ArmorDye(Green)
        case 17:iItemID = 972; break; // ArmorDye(Black) 
        case 18:iItemID = 973; break; // ArmorDye(Knight) 
        case 19:iItemID = 970; break; // CritCandy
        case 20:iItemID = 651; break; // GreenBall
        case 21:iItemID = 653; break; // YellowBall
        case 22:iItemID = 655; break; // PearlBall
    }

    //chance
    if (iDice(1, 120) <= 3) iItemID = 650;//ZemstoneOfSacrifice
    //chance

    pItem = new class CItem;
    if (_bInitItemAttr(pItem, iItemID) == false)
    {
        delete pItem;
    }
    else
    {
        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
            m_pClientList[iClientH]->m_sY, pItem);
        SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
            m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
            pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);
    }


}

void CGame::RequestStudyMagicHandler(int iClientH, char * pName, bool bIsPurchase)
{
    char * cp, cMagicName[31], cData[100];
    uint32_t * dwp, dwGoldCount;
    uint16_t * wp;
    int * ip, iReqInt, iCost, iRet;
    bool bMagic = true;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

    // ¸¶¹ýÀ» ¹è¿î´Ù. 
    memset(cData, 0, sizeof(cData));

    memset(cMagicName, 0, sizeof(cMagicName));
    memcpy(cMagicName, pName, 30);

    iRet = _iGetMagicNumber(cMagicName, &iReqInt, &iCost);
    if (iRet == -1)
    {
        // ÀÌ·± ÀÌ¸§ÀÇ ¸¶¹ýÀº Á¸ÀçÇÏÁö ¾Ê´Â´Ù. ¹«½ÃÇÑ´Ù. 

    }
    else
    {
        if (bIsPurchase == true)
        {
            if (m_pMagicConfigList[iRet]->m_iGoldCost < 0) bMagic = false; // ÀÏ¹ÝÀûÀ¸·Î ¹è¿ï¼ö ¾ø´Â ¸¶¹ýÀÌ¶ó¸é(°¡°ÝÀÌ À½¼ö) ¹è¿ï ¼ö ¾ø´Ù.
            dwGoldCount = dwGetItemCount(iClientH, "Gold");
            if ((uint32_t)iCost > dwGoldCount)  bMagic = false; // µ·ÀÌ ºÎÁ·ÇØµµ ¹è¿ï ¼ö ¾ø´Ù.
        }
        //wizard remove
        //if (m_pClientList[iClientH]->m_bIsInsideWizardTower == false && bIsPurchase) return;
        if (m_pClientList[iClientH]->m_cMagicMastery[iRet] != 0) return;

        if ((iReqInt <= m_pClientList[iClientH]->m_iInt) && (bMagic == true))
        {

            // µ·À» »ç¿ëÇßÀ½À» ¾Ë¸°´Ù.
            if (bIsPurchase == true) SetItemCount(iClientH, "Gold", dwGoldCount - iCost);

            // ¼ÒÁöÇ° ÃÑ Áß·® Àç °è»ê 
            iCalcTotalWeight(iClientH);

            // ¸¶¹ý »ç¿ë ´É·Â Ç¥½Ã .
            m_pClientList[iClientH]->m_cMagicMastery[iRet] = 1;

            // ¸¶¹ýÀ» ¹è¿ü´Ù´Â ¸Þ½ÃÁö¸¦ Àü¼ÛÇÑ´Ù.
            dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
            *dwp = MSGID_NOTIFY;
            wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
            *wp = DEF_NOTIFY_MAGICSTUDYSUCCESS;

            cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);

            // ¸¶¹ý ¹øÈ£ 
            *cp = iRet;
            cp++;

            memcpy(cp, cMagicName, 30);
            cp += 30;

            // Á¤º¸ Àü¼Û 
            iRet = m_pClientList[iClientH]->send_msg(cData, 37);

            switch (iRet)
            {
                case DEF_XSOCKEVENT_QUENEFULL:
                case DEF_XSOCKEVENT_SOCKETERROR:
                case DEF_XSOCKEVENT_CRITICALERROR:
                case DEF_XSOCKEVENT_SOCKETCLOSED:
                    // ¸Þ½ÃÁö¸¦ º¸³¾¶§ ¿¡·¯°¡ ¹ß»ýÇß´Ù¸é Á¦°ÅÇÑ´Ù.
                    DeleteClient(iClientH, true, true);
                    return;
            }
        }
        else
        {
            // Á¶°ÇÀÌ ¸¸Á·µÇÁö ¾Ê¾Æ ¸¶¹ýÀ» ¹è¿ï ¼ö ¾ø´Ù.
            // ¸¶¹ýÀ» ¹è¿ï¼ö ¾ø¾ú´Ù´Â ¸Þ½ÃÁö¸¦ Àü¼ÛÇÑ´Ù.
            dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
            *dwp = MSGID_NOTIFY;
            wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
            *wp = DEF_NOTIFY_MAGICSTUDYFAIL;

            cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);
            // ½ÇÆÐÀÌÀ¯.
            *cp = 1;
            cp++;

            // ¸¶¹ý ¹øÈ£ 
            *cp = iRet;
            cp++;

            memcpy(cp, cMagicName, 30);
            cp += 30;

            ip = (int *)cp;
            *ip = iCost;
            cp += 4;

            ip = (int *)cp;
            *ip = iReqInt;
            cp += 4;

            // Á¤º¸ Àü¼Û 
            iRet = m_pClientList[iClientH]->send_msg(cData, 46);
            switch (iRet)
            {
                case DEF_XSOCKEVENT_QUENEFULL:
                case DEF_XSOCKEVENT_SOCKETERROR:
                case DEF_XSOCKEVENT_CRITICALERROR:
                case DEF_XSOCKEVENT_SOCKETCLOSED:
                    // ¸Þ½ÃÁö¸¦ º¸³¾¶§ ¿¡·¯°¡ ¹ß»ýÇß´Ù¸é Á¦°ÅÇÑ´Ù.
                    DeleteClient(iClientH, true, true);
                    return;
            }
        }
    }
}

void CGame::UseItemHandler(int iClientH, short sItemIndex, short dX, short dY, short sDestItemID)
{
    int iTemp, iMax, iV1, iV2, iV3, iSEV1, iEffectResult = 0;
    uint32_t dwTime;
    short sTemp, sTmpType, sTmpAppr1;
    char cSlateType[20];

    dwTime = timeGetTime();
    ZeroMemory(cSlateType, sizeof(cSlateType));

    //testcode
    //copy_string(G_cTxt, "%d", sDestItemID);
    //log->info(G_cTxt);

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsKilled == true) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

    if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return;
    if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == 0) return;

    if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_USE_DEPLETE) ||
        (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_USE_PERM) ||
        (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW) ||
        (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_EAT) ||
        (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_USE_SKILL) ||
        (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_USE_DEPLETE_DEST))
    {
    }
    else return;

    if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_USE_DEPLETE) ||
        (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_EAT))
    {

        // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ ÃˆÂ¿Â°ÃºÂ¿Â¡ Â¸Ã‚Â´Ã‚ ÃƒÂ³Â¸Â®Â¸Â¦ Ã‡Ã‘Â´Ã™. 
        switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType)
        {
            case DEF_ITEMEFFECTTYPE_WARM: // v2.172 2002-7-5 ÇØµ¿ ½Ã¾à. 

                // ³Ãµ¿ »óÅÂÀÎ °æ¿ì ÇØµ¿ µÇ¾ú´Ù´Â ¸Þ¼¼Áö¸¦ º¸³»ÁØ´Ù. 
                if (m_pClientList[iClientH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 1)
                {
                    //	SetIceFlag(iClientH, DEF_OWNERTYPE_PLAYER, false);

                    bRemoveFromDelayEventList(iClientH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_ICE);

                    // È¿°ú°¡ ÇØÁ¦µÉ ¶§ ¹ß»ýÇÒ µô·¹ÀÌ ÀÌº¥Æ®¸¦ µî·ÏÇÑ´Ù.
                    bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (1 * 1000),
                        iClientH, DEF_OWNERTYPE_PLAYER, 0, 0, 0, 1, 0, 0);


                    //				SendNotifyMsg(0, iClientH, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_ICE, 0, 0, 0);
                }

                m_pClientList[iClientH]->m_dwWarmEffectTime = dwTime;
                break;

            case DEF_ITEMEFFECTTYPE_LOTTERY:
                // ÂºÂ¹Â±Ã‡ Â¾Ã†Ã€ÃŒÃ…Ã› EV1(ÃˆÂ®Â·Ã¼: ÃƒÃ–Ã€Ãº 100) EV2(Â»Ã³Ã‡Â° ÃÂ¾Â·Ã¹) EV3(Â»Ã³Ã‡Â° Â¼Ã¶Â·Â®)
                iTemp = iDice(1, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1);
                if (iTemp == iDice(1, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1))
                {
                    // Â´Ã§ÃƒÂ·!

                }
                else
                {
                    // Â²ÃŽ!

                }
                break;

            case DEF_ITEMEFFECTTYPE_SLATES:
                if (m_pClientList[iClientH]->m_pItemList[sItemIndex] != 0)
                {
                    // Full Ancient Slate ??
                    if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 867)
                    {
                        // Slates dont work on Heldenian Map
                        switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2)
                        {
                            case 2: // Bezerk slate
                                m_pClientList[iClientH]->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] = true;
                                SetBerserkFlag(iClientH, DEF_OWNERTYPE_PLAYER, true);
                                bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_BERSERK, dwTime + (1000 * 600),
                                    iClientH, DEF_OWNERTYPE_PLAYER, 0, 0, 0, 1, 0, 0);
                                SendNotifyMsg(0, iClientH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_BERSERK, 1, 0, 0);
                                strcpy(cSlateType, "Berserk");
                                break;

                            case 1: // Invincible slate
                                if (strlen(cSlateType) == 0)
                                {
                                    strcpy(cSlateType, "Invincible");
                                }
                            case 3: // Mana slate
                                if (strlen(cSlateType) == 0)
                                {
                                    strcpy(cSlateType, "Mana");
                                }
                            case 4: // Exp slate
                                if (strlen(cSlateType) == 0)
                                {
                                    strcpy(cSlateType, "Exp");
                                }
                                SetSlateFlag(iClientH, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2, true);
                                bRegisterDelayEvent(DEF_DELAYEVENTTYPE_ANCIENT_TABLET, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2,
                                    dwTime + (1000 * 600), iClientH, DEF_OWNERTYPE_PLAYER, 0, 0, 0, 1, 0, 0);
                                switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2)
                                {
                                    case 1:
                                        iEffectResult = 4;
                                        break;
                                    case 3:
                                        iEffectResult = 5;
                                        break;
                                    case 4:
                                        iEffectResult = 6;
                                        break;
                                }
                        }
                        if (strlen(cSlateType) > 0)
                            _bItemLog(DEF_ITEMLOG_USE, iClientH, strlen(cSlateType), m_pClientList[iClientH]->m_pItemList[sItemIndex]);
                    }
                }
                break;
            case DEF_ITEMEFFECTTYPE_HP:
                iMax = iGetMaxHP(iClientH);
                if (m_pClientList[iClientH]->m_iHP < iMax)
                {

                    if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1 == 0)
                    {
                        iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
                        iV2 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
                        iV3 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue3;
                    }
                    else
                    {
                        iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1;
                        iV2 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2;
                        iV3 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue3;
                    }

                    m_pClientList[iClientH]->m_iHP += (iDice(iV1, iV2) + iV3);
                    if (m_pClientList[iClientH]->m_iHP > iMax) m_pClientList[iClientH]->m_iHP = iMax;
                    if (m_pClientList[iClientH]->m_iHP <= 0)   m_pClientList[iClientH]->m_iHP = 1;

                    iEffectResult = 1;
                }
                break;

            case DEF_ITEMEFFECTTYPE_MP:
                iMax = (2 * m_pClientList[iClientH]->m_iMag) + (2 * m_pClientList[iClientH]->m_iLevel) + (m_pClientList[iClientH]->m_iInt / 2);

                if (m_pClientList[iClientH]->m_iMP < iMax)
                {

                    if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1 == 0)
                    {
                        iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
                        iV2 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
                        iV3 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue3;
                    }
                    else
                    {
                        iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1;
                        iV2 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2;
                        iV3 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue3;
                    }

                    m_pClientList[iClientH]->m_iMP += (iDice(iV1, iV2) + iV3);
                    if (m_pClientList[iClientH]->m_iMP > iMax)
                        m_pClientList[iClientH]->m_iMP = iMax;

                    iEffectResult = 2;
                }
                break;
            case DEF_ITEMEFFECTTYPE_CRITKOMM:
                CritInc(iClientH);
                break;
            case DEF_ITEMEFFECTTYPE_SP:
                iMax = (2 * m_pClientList[iClientH]->m_iStr) + (2 * m_pClientList[iClientH]->m_iLevel);

                if (m_pClientList[iClientH]->m_iSP < iMax)
                {

                    if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1 == 0)
                    {
                        iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
                        iV2 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
                        iV3 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue3;
                    }
                    else
                    {
                        iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1;
                        iV2 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2;
                        iV3 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue3;
                    }

                    m_pClientList[iClientH]->m_iSP += (iDice(iV1, iV2) + iV3);
                    if (m_pClientList[iClientH]->m_iSP > iMax)
                        m_pClientList[iClientH]->m_iSP = iMax;

                    iEffectResult = 3;
                }

                if (m_pClientList[iClientH]->m_bIsPoisoned == true)
                {
                    // ÃÃŸÂµÂ¶ÂµÃˆ Â»Ã³Ã…Ã‚Â¿Â´Â´Ã™Â¸Ã© ÃÃŸÂµÂ¶Ã€Â» Ã‡Â¬Â´Ã™.
                    m_pClientList[iClientH]->m_bIsPoisoned = false;
                    // ÃÃŸÂµÂ¶Ã€ÃŒ Ã‡Â®Â·ÃˆÃ€Â½Ã€Â» Â¾Ã‹Â¸Â°Â´Ã™. 
                    SetPoisonFlag(iClientH, DEF_OWNERTYPE_PLAYER, false); // removes poison aura when using a revitalizing potion
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_POISON, 0, 0, 0);
                }
                break;

            case DEF_ITEMEFFECTTYPE_HPSTOCK:
                iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
                iV2 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
                iV3 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue3;

                m_pClientList[iClientH]->m_iHPstock += iDice(iV1, iV2) + iV3;
                if (m_pClientList[iClientH]->m_iHPstock < 0)   m_pClientList[iClientH]->m_iHPstock = 0;
                if (m_pClientList[iClientH]->m_iHPstock > 500) m_pClientList[iClientH]->m_iHPstock = 500;

                // Â¹Ã¨Â°Ã­Ã‡Ã„Ã€Â» Ã‡Ã˜Â°Ã¡Ã‡Ã‘Â´Ã™. 
                m_pClientList[iClientH]->m_iHungerStatus += iDice(iV1, iV2) + iV3;
                if (m_pClientList[iClientH]->m_iHungerStatus > 100) m_pClientList[iClientH]->m_iHungerStatus = 100;
                if (m_pClientList[iClientH]->m_iHungerStatus < 0)   m_pClientList[iClientH]->m_iHungerStatus = 0;
                break;

            case DEF_ITEMEFFECTTYPE_STUDYSKILL:
                // Â±Ã¢Â¼ÃºÃ€Â» Â¹Ã¨Â¿Ã®Â´Ã™.	
                iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
                iV2 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
                iSEV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1;
                // iV1Ã€Âº Â¹Ã¨Â¿Ã¯ Skill Â¹Ã¸ÃˆÂ£. iV2Â´Ã‚ Â±Ã¢Â¼Ãº Â¼Ã¶ÃÃ˜, iSEV1Ã€Âº Â»Ã§Â¿Ã«Ã€Ãš ÃÂ¤Ã€Ã‡ Â±Ã¢Â¼Ãº Â¼Ã¶ÃÃ˜(Â¿Ã¬Â¼Â±Â¼Ã¸Ã€Â§) 
                if (iSEV1 == 0)
                {
                    // Â»Ã§Â¿Ã«Ã€Ãš ÃÂ¤Ã€Ã‡ Â±Ã¢Â¼ÃºÂ¼Ã¶ÃÃ˜Ã€ÃŒ 0Ã€ÃŒÂ¶Ã³Â¸Ã© Ã‡Â¥ÃÃ˜ Â±Ã¢Â¼ÃºÂ¼Ã¶ÃÃ˜Â¿Â¡ ÂµÃ»Â¶Ã³ Â±Ã¢Â¼ÃºÃ€Â» Â¹Ã¨Â¿Ã¬Â°Ã” ÂµÃˆÂ´Ã™. 
                    TrainSkillResponse(true, iClientH, iV1, iV2);
                }
                else
                {
                    TrainSkillResponse(true, iClientH, iV1, iSEV1);
                }
                break;

            case DEF_ITEMEFFECTTYPE_STUDYMAGIC:
                // iV1Ã€Âº Â¹Ã¨Â¿Ã¯ Â¸Â¶Â¹Ã½ Â¹Ã¸ÃˆÂ£.
                iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
                if (m_pMagicConfigList[iV1] != 0)
                    RequestStudyMagicHandler(iClientH, m_pMagicConfigList[iV1]->m_cName, false);
                break;

                /*case DEF_ITEMEFFECTTYPE_LOTTERY:
                    iLottery = iDice(1, m_pClientList[iClientH]->m_pItemList[sItemIndex]->
                    break;*/

                    // New 15/05/2004 Changed
            case DEF_ITEMEFFECTTYPE_MAGIC:
                // Ã…ÃµÂ¸Ã­ Â¸Ã°ÂµÃ¥Â¿Â´Â´Ã™Â¸Ã© Â¸Â¶Â¹Ã½ ÃˆÂ¿Â°Ãº Â¾Ã†Ã€ÃŒÃ…Ã› Â»Ã§Â¿Ã«Â½ÃƒÂ¿Â¡ Ã‡Ã˜ÃÂ¦ÂµÃˆÂ´Ã™.
                if ((m_pClientList[iClientH]->m_iStatus & 0x10) != 0)
                {
                    if (m_pClientList[iClientH]->m_iAdminUserLevel == 0)
                    {
                        SetInvisibilityFlag(iClientH, DEF_OWNERTYPE_PLAYER, false);

                        bRemoveFromDelayEventList(iClientH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
                        m_pClientList[iClientH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = 0;
                    }
                }

                switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1)
                {
                    case 1:
                        // Recall Â¸Â¶Â¹Ã½ ÃˆÂ¿Â°ÃºÂ°Â¡ Ã€Ã–Â´Ã‚ Â¾Ã†Ã€ÃŒÃ…Ã›. 
                        // testcode
                        RequestTeleportHandler(iClientH, "1   ");
                        break;

                    case 2:
                        // Ã…ÃµÂ¸Ã­ Â¸Â¶Â¹Ã½ÃˆÂ¿Â°ÃºÂ°Â¡ Ã€Ã–Â´Ã‚ Â¾Ã†Ã€ÃŒÃ…Ã›. 
                        PlayerMagicHandler(iClientH, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, 32, true);
                        break;

                    case 3:
                        // Ã…ÃµÂ¸Ã­ Â¸Â¶Â¹Ã½ ÃƒÂ£Â±Ã¢. Â»Ã§Ã…ÃµÃ€Ã¥ Â³Â»ÂºÃŽÂ¸Ã© Â¼Ã’Â¿Ã«Â¾Ã¸Â´Ã™. 
                        if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFightZone == false)
                            PlayerMagicHandler(iClientH, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, 34, true);
                        break;

                    case 4:
                        // fixed location teleportation: Ã€Ã”Ã€Ã¥Â±Ã‡ ÂµÃ®ÂµÃ®
                        switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2)
                        {
                            case 1:
                                // ÂºÃ­Â¸Â®ÂµÃ¹ Â¾Ã†Ã€ÃÂ·ÃŽ Â°Â£Â´Ã™ 
                                if (memcmp(m_pClientList[iClientH]->m_cMapName, "bisle", 5) != 0)
                                {
                                    //v1.42
                                    ItemDepleteHandler(iClientH, sItemIndex, true);
                                    RequestTeleportHandler(iClientH, "2   ", "bisle", -1, -1);
                                }
                                break;
                            case 2: //lotery
                                ItemDepleteHandler(iClientH, sItemIndex, true);
                                LoteryHandler(iClientH);
                                break;

                            case 11:
                            case 12:
                            case 13:
                            case 14:
                            case 15:
                            case 16:
                            case 17:
                            case 18:
                            case 19:
                                // Â°Ã¡Ã…ÃµÃ€Ã¥Ã€Â¸Â·ÃŽ Â°Â£Â´Ã™. 
                                SYSTEMTIME SysTime;

                                GetLocalTime(&SysTime);
                                // v1.4311-3 ÂºÂ¯Â°Ã¦ Ã€Ã”Ã€Ã¥Â±Ã‡ ÃƒÂ¼Ã…Â© Â´Ãž/Â³Â¯Ã‚Â¥/Â½ÃƒÂ°Â£Ã€Â¸Â·ÃŽ ÃƒÂ¼Ã…Â©Ã‡Ã‘Â´Ã™. 
                                // Ã€Ã”Ã€Ã¥ Â°Â¡Â´Ã‰Ã‡Ã‘ Â½ÃƒÂ°Â£ÂºÂ¸Â´Ã™ Ã€Ã›Â°Ã…Â³Âª Â°Â°Ã€Â¸Â¸Ã© Ã€Ã”Ã€Ã¥Â±Ã‡Ã€ÃŒ Â»Ã§Â¶Ã³ÃÃ¸Â´Ã™.
                                if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sTouchEffectValue1 != SysTime.wMonth) ||
                                    (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sTouchEffectValue2 != SysTime.wDay) ||
                                    (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sTouchEffectValue3 <= SysTime.wHour))
                                {
                                    // Â³Â¯Ã‚Â¥Â°Â¡ ÃÂ¤ÃˆÂ®Ã‡ÃÃÃ¶ Â¾ÃŠÂ´Ã™. Â¾Ã†Â¹Â«Â·Â± ÃˆÂ¿Â°ÃºÂ°Â¡ Â¾Ã¸Â°Ã­ Ã€Ã”Ã€Ã¥Â±Ã‡Ã€Âº Â»Ã§Â¶Ã³ÃÃ¸Â´Ã™.
                                }
                                else
                                {
                                    char cDestMapName[11];
                                    memset(cDestMapName, 0, sizeof(cDestMapName));
                                    copy_string(cDestMapName, "fightzone%d", m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2 - 10);
                                    if (memcmp(m_pClientList[iClientH]->m_cMapName, cDestMapName, 10) != 0)
                                    {
                                        //v1.42
                                        ItemDepleteHandler(iClientH, sItemIndex, true);
                                        RequestTeleportHandler(iClientH, "2   ", cDestMapName, -1, -1);
                                    }
                                }
                                break;
                        }
                        break;

                    case 5:
                        // Â¼Ã’ÃˆÂ¯ÃˆÂ¿Â°ÃºÂ°Â¡ Ã€Ã–Â´Ã‚ Â¾Ã†Ã€ÃŒÃ…Ã› 
                        PlayerMagicHandler(iClientH, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, 31, true,
                            m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2);
                        break;
                }
                break;

            case DEF_ITEMEFFECTTYPE_FIRMSTAMINAR:
                m_pClientList[iClientH]->m_iTimeLeft_FirmStaminar += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
                if (m_pClientList[iClientH]->m_iTimeLeft_FirmStaminar > 20 * 30) m_pClientList[iClientH]->m_iTimeLeft_FirmStaminar = 20 * 30; // ÃƒÃ–Â´Ã« 30ÂºÃÂ°Â£ 
                break;

            case DEF_ITEMEFFECTTYPE_CHANGEATTR:
                switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1)
                {
                    case 1:
                        // Â¸Ã“Â¸Â® Â»Ã¶Ã€Â» Â¹Ã™Â²Ã›Â´Ã™. 
                        m_pClientList[iClientH]->m_cHairColor++;
                        if (m_pClientList[iClientH]->m_cHairColor > 15) m_pClientList[iClientH]->m_cHairColor = 0;

                        sTemp = (m_pClientList[iClientH]->m_cHairStyle << 8) | (m_pClientList[iClientH]->m_cHairColor << 4) | (m_pClientList[iClientH]->m_cUnderwear);
                        m_pClientList[iClientH]->m_sAppr1 = sTemp;
                        break;

                    case 2:
                        // Â¸Ã“Â¸Â® Â½ÂºÃ…Â¸Ã€ÃÃ€Â» Â¹Ã™Â²Ã›Â´Ã™.
                        m_pClientList[iClientH]->m_cHairStyle++;
                        if (m_pClientList[iClientH]->m_cHairStyle > 7) m_pClientList[iClientH]->m_cHairStyle = 0;

                        sTemp = (m_pClientList[iClientH]->m_cHairStyle << 8) | (m_pClientList[iClientH]->m_cHairColor << 4) | (m_pClientList[iClientH]->m_cUnderwear);
                        m_pClientList[iClientH]->m_sAppr1 = sTemp;
                        break;

                    case 3:
                        // Ã‡Ã‡ÂºÃŽÂ»Ã¶Ã€Â» Â¹Ã™Â²Ã›Â´Ã™.
                        // Â³Â»Â¿Ã«Ã€Â» Â¹Ã™Ã…ÃÃ€Â¸Â·ÃŽ AppearanceÂ¸Â¦ Â°Ã¨Â»Ãª, Ã‡Ã’Â´Ã§Ã‡Ã‘Â´Ã™.
                        m_pClientList[iClientH]->m_cSkin++;
                        if (m_pClientList[iClientH]->m_cSkin > 3)
                            m_pClientList[iClientH]->m_cSkin = 1;

                        if (m_pClientList[iClientH]->m_cSex == 1)      sTemp = 1;
                        else if (m_pClientList[iClientH]->m_cSex == 2) sTemp = 4;

                        switch (m_pClientList[iClientH]->m_cSkin)
                        {
                            case 2:	sTemp += 1; break;
                            case 3:	sTemp += 2; break;
                        }
                        m_pClientList[iClientH]->m_sType = sTemp;
                        break;

                    case 4:
                        // Â¼ÂºÃ€Ã¼ÃˆÂ¯ - Â¸Â¸Â¾Ã  Â¿ÃŠÃ€Â» Ã€Ã”Â°Ã­ Ã€Ã–Â´Ã™Â¸Ã© Â½Ã‡Ã†ÃÃ‡Ã‘Â´Ã™. 
                        sTemp = m_pClientList[iClientH]->m_sAppr3 & 0xFF0F;
                        if (sTemp == 0)
                        {
                            // sTempÂ°Â¡ 0Ã€ÃŒ Â¾Ã†Â´ÃÂ¶Ã³Â¸Ã© Â°Ã‘Â¿ÃŠ, Â¼Ã“Â¿ÃŠ, Â¹Ã™ÃÃ¶ÃÃŸ Ã‡Ã‘Â°Â¡ÃÃ¶Â¸Â¦ Ã€Ã”Â°Ã­ Ã€Ã–Â´Ã‚ Â°ÃÃ€ÃŒÂ´Ã™. Â¼ÂºÃ€Ã¼ÃˆÂ¯Ã€Â» Ã‡Ã’ Â¼Ã¶ Â¾Ã¸Â´Ã™. 
                            if (m_pClientList[iClientH]->m_cSex == 1)
                                m_pClientList[iClientH]->m_cSex = 2;
                            else m_pClientList[iClientH]->m_cSex = 1;

                            // Â³Â»Â¿Ã«Ã€Â» Â¹Ã™Ã…ÃÃ€Â¸Â·ÃŽ AppearanceÂ¸Â¦ Â°Ã¨Â»Ãª, Ã‡Ã’Â´Ã§Ã‡Ã‘Â´Ã™.
                            if (m_pClientList[iClientH]->m_cSex == 1)
                            {
                                // Â³Â²Ã€ÃšÃ€ÃŒÂ´Ã™. 
                                sTmpType = 1;
                            }
                            else if (m_pClientList[iClientH]->m_cSex == 2)
                            {
                                // Â¿Â©Ã€ÃšÃ€ÃŒÂ´Ã™.
                                sTmpType = 4;
                            }

                            switch (m_pClientList[iClientH]->m_cSkin)
                            {
                                case 1:
                                    // Â¹Ã©Ã€ÃŽÃ€ÃŒÂ¸Ã© Â±Ã—Â´Ã«Â·ÃŽ.
                                    break;
                                case 2:
                                    sTmpType += 1;
                                    break;
                                case 3:
                                    sTmpType += 2;
                                    break;
                            }

                            sTmpAppr1 = (m_pClientList[iClientH]->m_cHairStyle << 8) | (m_pClientList[iClientH]->m_cHairColor << 4) | (m_pClientList[iClientH]->m_cUnderwear);
                            m_pClientList[iClientH]->m_sType = sTmpType;
                            m_pClientList[iClientH]->m_sAppr1 = sTmpAppr1;
                            //
                        }
                        break;
                }

                SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
                break;
        }
        // *** Request Teleport HandlerÂ°Â¡ Ã€Ã›ÂµÂ¿ÂµÃ‡Â¸Ã© Ã€ÃŒÂ¹ÃŒ ÂµÂ¥Ã€ÃŒÃ…Ã Ã€ÃºÃ€Ã¥Ã€ÃŒ Â¿Ã¤ÃƒÂ»ÂµÃˆ Â»Ã³Ã…Ã‚Ã€ÃŒÂ¹Ã‡Â·ÃŽ Ã€ÃŒÃˆÃ„Â¿Â¡ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â¾Ã¸Â¾Ã–ÂºÃÂ¾ÃŸ Â¼Ã’Â¿Ã«Ã€ÃŒ Â¾Ã¸Â´Ã™. 
        // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â¸Ã•Ã€Ãº Â¾Ã¸Â¾Ã˜Â´Ã™.
        ItemDepleteHandler(iClientH, sItemIndex, true);

        switch (iEffectResult)
        {
            case 1:
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_HP, 0, 0, 0, 0);
                break;
            case 2:
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_MP, 0, 0, 0, 0);
                break;
            case 3:
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_SP, 0, 0, 0, 0);
                break;
            case 4: // Invincible
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_SLATE_INVINCIBLE, 0, 0, 0, 0);
                break;
            case 5: // Mana
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_SLATE_MANA, 0, 0, 0, 0);
                break;
            case 6: // EXP
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_SLATE_EXP, 0, 0, 0, 0);
                break;
            default:
                break;
        }
    }
    else if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_USE_DEPLETE_DEST)
    {
        // Â»Ã§Â¿Ã«Ã‡ÃÂ¸Ã©Â¼Â­ Â¸Ã±Ã‡Â¥ÃÃ¶ÃÂ¡Ã€Â» ÃÃ¶ÃÂ¤Ã‡ÃÂ´Ã‚ Â¾Ã†Ã€ÃŒÃ…Ã›.
        // dX, dYÃ€Ã‡ ÃÃ‚Ã‡Â¥Â°Â¡ Ã€Â¯ÃˆÂ¿ Â¹Ã¼Ã€Â§ Â³Â»Â¿Â¡ Ã€Ã–Â´Ã‚ÃÃ¶ ÃˆÂ®Ã€ÃŽÃ‡Ã˜Â¾ÃŸ Ã‡Ã‘Â´Ã™.
        if (_bDepleteDestTypeItemUseEffect(iClientH, dX, dY, sItemIndex, sDestItemID) == true)
            ItemDepleteHandler(iClientH, sItemIndex, true);
    }
    else if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW)
    {
        // ÃˆÂ­Â»Ã¬Ã€Â» Ã‡Ã’Â´Ã§Ã‡Ã‘Â´Ã™. 
        m_pClientList[iClientH]->m_cArrowIndex = _iGetArrowItemIndex(iClientH);
    }
    else if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_USE_PERM)
    {
        // Â¿ÂµÂ±Â¸ÃˆÃ· Â¾Âµ Â¼Ã¶ Ã€Ã–Â´Ã‚ Â¾Ã†Ã€ÃŒÃ…Ã›. ÃÃ¯ Â¾Â²Â°Ã­Â³ÂªÂµÂµ Â¾Ã¸Â¾Ã®ÃÃ¶ÃÃ¶ Â¾ÃŠÂ´Ã‚ Â¾Ã†Ã€ÃŒÃ…Ã›. (ex: ÃÃ¶ÂµÂµ) 
        switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType)
        {
            case DEF_ITEMEFFECTTYPE_SHOWLOCATION:
                iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
                switch (iV1)
                {
                    case 1:
                        // Ã‡Ã¶Ã€Ã§ Ã€ÃšÂ½Ã…Ã€Ã‡ Ã€Â§Ã„Â¡Â¸Â¦ ÂºÂ¸Â¿Â©ÃÃ˜Â´Ã™. 
                        if (strcmp(m_pClientList[iClientH]->m_cMapName, "aresden") == 0)
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 1, 0, 0);
                        else if (strcmp(m_pClientList[iClientH]->m_cMapName, "elvine") == 0)
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 2, 0, 0);
                        else if (strcmp(m_pClientList[iClientH]->m_cMapName, "middleland") == 0)
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 3, 0, 0);
                        else if (strcmp(m_pClientList[iClientH]->m_cMapName, "default") == 0)
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 4, 0, 0);
                        else if (strcmp(m_pClientList[iClientH]->m_cMapName, "huntzone2") == 0)
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 5, 0, 0);
                        else if (strcmp(m_pClientList[iClientH]->m_cMapName, "huntzone1") == 0)
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 6, 0, 0);
                        else if (strcmp(m_pClientList[iClientH]->m_cMapName, "huntzone4") == 0)
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 7, 0, 0);
                        else if (strcmp(m_pClientList[iClientH]->m_cMapName, "huntzone3") == 0)
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 8, 0, 0);
                        else if (strcmp(m_pClientList[iClientH]->m_cMapName, "arefarm") == 0)
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 9, 0, 0);
                        else if (strcmp(m_pClientList[iClientH]->m_cMapName, "elvfarm") == 0)
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 10, 0, 0);
                        else SendNotifyMsg(0, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 0, 0, 0);
                        break;
                }
                break;
        }
    }
    else if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_USE_SKILL)
    {
        // Â±Ã¢Â¼ÃºÂ°Ãº Â°Ã¼Â·ÃƒÂµÃˆ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â»Ã§Â¿Ã«Ã‡Ã‘Â´Ã™. Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ Â¼Ã¶Â¸Ã­Ã€Â» Â³Â·ÃƒÃŸÂ°Ã­ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¿Â¡ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™. 

        if ((m_pClientList[iClientH]->m_pItemList[sItemIndex] == 0) ||
            (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan <= 0) ||
            (m_pClientList[iClientH]->m_bSkillUsingStatus[m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sRelatedSkill] == true))
        {
            // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ Â¼Ã¶Â¸Ã­Ã€ÃŒ Â´Ã™ Ã‡ÃŸÂ°Ã…Â³Âª Â¾Ã¸Â°Ã…Â³Âª Â°Ã¼Â·Ãƒ Â½ÂºÃ…Â³Ã€Â» Â»Ã§Â¿Ã«ÃÃŸÃ€ÃŒÂ¶Ã³Â¸Ã© Â¹Â«Â½Ãƒ 
            return;
        }
        else
        {
            if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wMaxLifeSpan != 0)
            {
                // ÃƒÃ–Â´Ã« Â¼Ã¶Â¸Ã­Ã€ÃŒ 0Ã€ÃŒÂ¸Ã© Â»Ã§Â¿Ã«Ã‡Ã˜ÂµÂµ Â¼Ã¶Â¸Ã­Ã€ÃŒ ÃÃ™ÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™.
                m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan--;
                if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan <= 0)
                {
                    // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ Â¼Ã¶Â¸Ã­Ã€ÃŒ Â´Ã™ ÂµÃ‡Â¾ÃºÂ´Ã™.
                    // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¸ÃÂ°Â¡ÃÂ³Â´Ã™Â´Ã‚ Â¸ÃžÂ½ÃƒÃÃ¶ <- Ã€ÃŒÂ°Ã‰ Â¹ÃžÃ€Â¸Â¸Ã© Ã€Ã¥Ã‚Ã¸ÃˆÂ­Â¸Ã©Â¿Â¡Â¼Â­ Ã‡Ã˜ÃÂ¦Â½ÃƒÃ„Ã‘Â¾ÃŸ Ã‡Ã‘Â´Ã™.
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMLIFESPANEND, DEF_EQUIPPOS_NONE, sItemIndex, 0, 0);
                }
                else
                {
                    // Â±Ã¢Â¼Ãº Â»Ã§Â¿Ã« Â½ÃƒÂ°Â£ IDÂ°ÂªÃ€Â» Â±Â¸Ã‡Ã‘Â´Ã™. v1.12
                    int iSkillUsingTimeID = (int)timeGetTime();

                    bRegisterDelayEvent(DEF_DELAYEVENTTYPE_USEITEM_SKILL, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sRelatedSkill,
                        dwTime + m_pSkillConfigList[m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sRelatedSkill]->m_sValue2 * 1000,
                        iClientH, DEF_OWNERTYPE_PLAYER, m_pClientList[iClientH]->m_cMapIndex, dX, dY,
                        m_pClientList[iClientH]->m_cSkillMastery[m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sRelatedSkill], iSkillUsingTimeID, 0);

                    // Â±Ã¢Â¼Ãº Â»Ã§Â¿Ã«ÃÃŸ 
                    m_pClientList[iClientH]->m_bSkillUsingStatus[m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sRelatedSkill] = true;
                    m_pClientList[iClientH]->m_iSkillUsingTimeID[m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sRelatedSkill] = iSkillUsingTimeID; //v1.12
                }
            }
        }
    }
}
