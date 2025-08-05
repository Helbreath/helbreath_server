//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include "Game.h"
#include "string_utils.h"

void CGame::DelayEventProcessor()
{
    int i, iSkillNum, iResult;
    uint32_t dwTime = timeGetTime();
    int iTemp;

    for (i = 0; i < DEF_MAXDELAYEVENTS; i++)
        if ((m_pDelayEventList[i] != 0) && (m_pDelayEventList[i]->m_dwTriggerTime < dwTime))
        {

            // Ã€ÃŒÂºÂ¥Ã†Â®Â°Â¡ ÂµÂ¿Ã€Ã›Ã‡Ã’ Â½ÃƒÂ°Â£Ã€ÃŒ ÂµÃ†Â´Ã™. ÂµÂ¿Ã€Ã›ÃˆÃ„ Â»Ã¨ÃÂ¦ÂµÃˆÂ´Ã™.
            switch (m_pDelayEventList[i]->m_iDelayType)
            {

                case DEF_DELAYEVENTTYPE_ANCIENT_TABLET:
                    if ((m_pClientList[m_pDelayEventList[i]->m_iTargetH]->m_iStatus & 0x400000) != 0)
                    {
                        iTemp = 1;
                    }
                    else if ((m_pClientList[m_pDelayEventList[i]->m_iTargetH]->m_iStatus & 0x800000) != 0)
                    {
                        iTemp = 3;
                    }
                    else if ((m_pClientList[m_pDelayEventList[i]->m_iTargetH]->m_iStatus & 0x10000) != 0)
                    {
                        iTemp = 4;
                    }

                    SendNotifyMsg(0, m_pDelayEventList[i]->m_iTargetH, DEF_NOTIFY_SLATE_STATUS, iTemp, 0, 0, 0);
                    SetSlateFlag(m_pDelayEventList[i]->m_iTargetH, iTemp, false);
                    break;

                case DEF_DELAYEVENTTYPE_CALCMETEORSTRIKEEFFECT:
                    CalcMeteorStrikeEffectHandler(m_pDelayEventList[i]->m_cMapIndex);
                    break;

                case DEF_DELAYEVENTTYPE_DOMETEORSTRIKEDAMAGE:
                    DoMeteorStrikeDamageHandler(m_pDelayEventList[i]->m_cMapIndex);
                    break;

                case DEF_DELAYEVENTTYPE_METEORSTRIKE:
                    MeteorStrikeHandler(m_pDelayEventList[i]->m_cMapIndex);
                    break;

                case DEF_DELAYEVENTTYPE_USEITEM_SKILL:
                    // Â¾Ã†Ã€ÃŒÃ…Ã› Â»Ã§Â¿Ã«Â¿Â¡ ÂµÃ»Â¸Â¥ Â°Ã¡Â°Ãº Â°Ã¨Â»Ãª, Ã…Ã«ÂºÂ¸ 
                    switch (m_pDelayEventList[i]->m_cTargetType)
                    {
                        case DEF_OWNERTYPE_PLAYER:
                            iSkillNum = m_pDelayEventList[i]->m_iEffectType;

                            if (m_pClientList[m_pDelayEventList[i]->m_iTargetH] == 0) break;
                            // Â±Ã¢Â¼Ãº Â»Ã§Â¿Ã«Ã€ÃŒ Â¹Â«ÃˆÂ¿ÃˆÂ­ ÂµÃ‡Â¾ÃºÂ´Ã™Â¸Ã© Â¹Â«Â½Ãƒ. 
                            if (m_pClientList[m_pDelayEventList[i]->m_iTargetH]->m_bSkillUsingStatus[iSkillNum] == false) break;
                            // Â±Ã¢Â¼Ãº Â»Ã§Â¿Ã« Â½ÃƒÂ°Â£ IDÂ°Â¡ Â´ÃžÂ¶Ã³ÂµÂµ Â¹Â«Â½Ãƒ v1.12
                            if (m_pClientList[m_pDelayEventList[i]->m_iTargetH]->m_iSkillUsingTimeID[iSkillNum] != m_pDelayEventList[i]->m_iV2) break;

                            // Â±Ã¢Â¼Ãº Â»Ã§Â¿Ã« Â»Ã³Ã…Ã‚ Ã‡Ã˜ÃÂ¦ 
                            m_pClientList[m_pDelayEventList[i]->m_iTargetH]->m_bSkillUsingStatus[iSkillNum] = false;
                            m_pClientList[m_pDelayEventList[i]->m_iTargetH]->m_iSkillUsingTimeID[iSkillNum] = 0;

                            // Ã€ÃŒÃÂ¦ SkillÃ€Ã‡ ÃˆÂ¿Â°ÃºÂ¿Â¡ ÂµÃ»Â¸Â¥ Â°Ã¨Â»ÃªÃ€Â» Ã‡Ã‘Â´Ã™. 
                            iResult = iCalculateUseSkillItemEffect(m_pDelayEventList[i]->m_iTargetH, m_pDelayEventList[i]->m_cTargetType,
                                m_pDelayEventList[i]->m_iV1, iSkillNum, m_pDelayEventList[i]->m_cMapIndex, m_pDelayEventList[i]->m_dX, m_pDelayEventList[i]->m_dY);

                            // Â±Ã¢Â¼Ãº Â»Ã§Â¿Ã«Ã€ÃŒ ÃÃŸÃÃ¶ ÂµÃ‡Â¾ÃºÃ€Â½Ã€Â» Â¾Ã‹Â¸Â°Â´Ã™.
                            SendNotifyMsg(0, m_pDelayEventList[i]->m_iTargetH, DEF_NOTIFY_SKILLUSINGEND, iResult, 0, 0, 0);
                            break;
                    }
                    break;

                case DEF_DELAYEVENTTYPE_DAMAGEOBJECT:
                    break;

                case DEF_DELAYEVENTTYPE_MAGICRELEASE:
                    // Removes the aura after time
                    switch (m_pDelayEventList[i]->m_cTargetType)
                    {
                        case DEF_OWNERTYPE_PLAYER:
                            if (m_pClientList[m_pDelayEventList[i]->m_iTargetH] == 0) break;

                            SendNotifyMsg(0, m_pDelayEventList[i]->m_iTargetH, DEF_NOTIFY_MAGICEFFECTOFF,
                                m_pDelayEventList[i]->m_iEffectType, m_pClientList[m_pDelayEventList[i]->m_iTargetH]->m_cMagicEffectStatus[m_pDelayEventList[i]->m_iEffectType], 0, 0);

                            m_pClientList[m_pDelayEventList[i]->m_iTargetH]->m_cMagicEffectStatus[m_pDelayEventList[i]->m_iEffectType] = 0;

                            // Inbitition casting 
                            if (m_pDelayEventList[i]->m_iEffectType == DEF_MAGICTYPE_INHIBITION)
                                m_pClientList[m_pDelayEventList[i]->m_iTargetH]->m_bInhibition = false;

                            // Invisibility È¿°ú ÇØÁ¦ 
                            if (m_pDelayEventList[i]->m_iEffectType == DEF_MAGICTYPE_INVISIBILITY)
                                SetInvisibilityFlag(m_pDelayEventList[i]->m_iTargetH, DEF_OWNERTYPE_PLAYER, false);

                            // Berserk È¿°ú ÇØÁ¦
                            if (m_pDelayEventList[i]->m_iEffectType == DEF_MAGICTYPE_BERSERK)
                                SetBerserkFlag(m_pDelayEventList[i]->m_iTargetH, DEF_OWNERTYPE_PLAYER, false);

                            // Confusion
                            if (m_pDelayEventList[i]->m_iEffectType == DEF_MAGICTYPE_CONFUSE)
                                switch (m_pDelayEventList[i]->m_iV1)
                                {
                                    case 3: SetIllusionFlag(m_pDelayEventList[i]->m_iTargetH, DEF_OWNERTYPE_PLAYER, false); break;
                                    case 4: SetIllusionMovementFlag(m_pDelayEventList[i]->m_iTargetH, DEF_OWNERTYPE_PLAYER, false); break;
                                }

                            // Protection Magic
                            if (m_pDelayEventList[i]->m_iEffectType == DEF_MAGICTYPE_PROTECT)
                            {
                                switch (m_pDelayEventList[i]->m_iV1)
                                {
                                    case 1:
                                        SetProtectionFromArrowFlag(m_pDelayEventList[i]->m_iTargetH, DEF_OWNERTYPE_PLAYER, false);
                                        break;
                                    case 2:
                                    case 5:
                                        SetMagicProtectionFlag(m_pDelayEventList[i]->m_iTargetH, DEF_OWNERTYPE_PLAYER, false);
                                        break;
                                    case 3:
                                    case 4:
                                        SetDefenseShieldFlag(m_pDelayEventList[i]->m_iTargetH, DEF_OWNERTYPE_PLAYER, false);
                                        break;
                                }
                            }


                            // polymorph È¿°ú ÇØÁ¦ 
                            if (m_pDelayEventList[i]->m_iEffectType == DEF_MAGICTYPE_POLYMORPH)
                            {
                                m_pClientList[m_pDelayEventList[i]->m_iTargetH]->m_sType = m_pClientList[m_pDelayEventList[i]->m_iTargetH]->m_sOriginalType;
                                SendEventToNearClient_TypeA(m_pDelayEventList[i]->m_iTargetH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
                            }

                            // Ice È¿°ú ÇØÁ¦ 
                            if (m_pDelayEventList[i]->m_iEffectType == DEF_MAGICTYPE_ICE)
                                SetIceFlag(m_pDelayEventList[i]->m_iTargetH, DEF_OWNERTYPE_PLAYER, false);
                            break;

                        case DEF_OWNERTYPE_NPC:
                            if (m_pNpcList[m_pDelayEventList[i]->m_iTargetH] == 0) break;

                            m_pNpcList[m_pDelayEventList[i]->m_iTargetH]->m_cMagicEffectStatus[m_pDelayEventList[i]->m_iEffectType] = 0;

                            // Invisibility ÃˆÂ¿Â°Ãº Ã‡Ã˜ÃÂ¦ 
                            if (m_pDelayEventList[i]->m_iEffectType == DEF_MAGICTYPE_INVISIBILITY)
                                SetInvisibilityFlag(m_pDelayEventList[i]->m_iTargetH, DEF_OWNERTYPE_NPC, false);

                            // Berserk ÃˆÂ¿Â°Ãº Ã‡Ã˜ÃÂ¦
                            if (m_pDelayEventList[i]->m_iEffectType == DEF_MAGICTYPE_BERSERK)
                                SetBerserkFlag(m_pDelayEventList[i]->m_iTargetH, DEF_OWNERTYPE_NPC, false);

                            // polymorph ÃˆÂ¿Â°Ãº Ã‡Ã˜ÃÂ¦ 
                            if (m_pDelayEventList[i]->m_iEffectType == DEF_MAGICTYPE_POLYMORPH)
                            {
                                m_pNpcList[m_pDelayEventList[i]->m_iTargetH]->m_sType = m_pNpcList[m_pDelayEventList[i]->m_iTargetH]->m_sOriginalType;
                                SendEventToNearClient_TypeA(m_pDelayEventList[i]->m_iTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
                            }

                            // Ice ÃˆÂ¿Â°Ãº Ã‡Ã˜ÃÂ¦ 
                            if (m_pDelayEventList[i]->m_iEffectType == DEF_MAGICTYPE_ICE)
                                SetIceFlag(m_pDelayEventList[i]->m_iTargetH, DEF_OWNERTYPE_NPC, false);

                            // Illusion
                            if (m_pDelayEventList[i]->m_iEffectType == DEF_MAGICTYPE_CONFUSE)
                                SetIllusionFlag(m_pDelayEventList[i]->m_iTargetH, DEF_OWNERTYPE_NPC, false);

                            // Protection Magic
                            if (m_pDelayEventList[i]->m_iEffectType == DEF_MAGICTYPE_PROTECT)
                            {
                                switch (m_pDelayEventList[i]->m_iV1)
                                {
                                    case 1:
                                        SetProtectionFromArrowFlag(m_pDelayEventList[i]->m_iTargetH, DEF_OWNERTYPE_NPC, false);
                                        break;
                                    case 2:
                                    case 5:
                                        SetMagicProtectionFlag(m_pDelayEventList[i]->m_iTargetH, DEF_OWNERTYPE_NPC, false);
                                        break;
                                    case 3:
                                    case 4:
                                        SetDefenseShieldFlag(m_pDelayEventList[i]->m_iTargetH, DEF_OWNERTYPE_NPC, false);
                                        break;
                                }
                            }
                            //	if (m_pDelayEventList[i]->m_iEffectType == /*notcoded*/)

                            break;
                    }
                    break;
            }

            delete m_pDelayEventList[i];
            m_pDelayEventList[i] = 0;
        }
}

void CGame::DynamicObjectEffectProcessor()
{
    int i, ix, iy, iIndex;
    short sOwnerH, iDamage, sType;
    char  cOwnerType;
    uint32_t dwTime = timeGetTime(), dwRegisterTime;

    // 
    for (i = 0; i < DEF_MAXDYNAMICOBJECTS; i++)
        if (m_pDynamicObjectList[i] != 0)
        {
            switch (m_pDynamicObjectList[i]->m_sType)
            {

                case DEF_DYNAMICOBJECT_PCLOUD_BEGIN:
                    // Ã†Ã·Ã€ÃŒÃÃ° Ã…Â¬Â¶Ã³Â¿Ã¬ÂµÃ¥
                    for (ix = m_pDynamicObjectList[i]->m_sX - 1; ix <= m_pDynamicObjectList[i]->m_sX + 1; ix++)
                        for (iy = m_pDynamicObjectList[i]->m_sY - 1; iy <= m_pDynamicObjectList[i]->m_sY + 1; iy++)
                        {

                            m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                            if (sOwnerH != 0)
                            {
                                // Poison DamageÂ¸Â¦ Ã€Ã”Â´Ã‚Â´Ã™.
                                switch (cOwnerType)
                                {
                                    case DEF_OWNERTYPE_PLAYER:
                                        if (m_pClientList[sOwnerH] == 0) break;
                                        if (m_pClientList[sOwnerH]->m_bIsKilled == true) break;
                                        // v1.41 ÃÃŸÂ¸Â³Ã€ÃŒÂ°Ã­ Ã€Ã¼Ã…ÃµÂ¸Ã°ÂµÃ¥Â°Â¡ Â¾Ã†Â´ÃÂ¸Ã© Ã‡Ã‡Ã‡Ã˜Â¸Â¦ Ã€Ã”ÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™.
                                        //if ((m_pClientList[sOwnerH]->m_bIsNeutral == true) && (m_pClientList[sOwnerH]->m_sAppr2 & 0xF000) == 0) break;

                                        // Ã€ÃŒÂ·Â± Â½Ã„Ã€Â¸Â·ÃŽ Â´Ã«Â¹ÃŒÃÃ¶Ã€Ã‡ Ã…Â©Â±Ã¢Â¸Â¦ Â°Ã¡ÃÂ¤
                                        if (m_pDynamicObjectList[i]->m_iV1 < 20)
                                            iDamage = iDice(1, 6);
                                        else iDamage = iDice(1, 8);

                                        // New 17/05/2004 Changed
                                        if (m_pClientList[sOwnerH]->m_iAdminUserLevel == 0)
                                            m_pClientList[sOwnerH]->m_iHP -= iDamage;

                                        if (m_pClientList[sOwnerH]->m_iHP <= 0)
                                        {
                                            // Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â°Â¡ Â»Ã§Â¸ÃÃ‡ÃŸÂ´Ã™.
                                            ClientKilledHandler(sOwnerH, sOwnerH, cOwnerType, iDamage);
                                        }
                                        else
                                        {
                                            if (iDamage > 0)
                                            {
                                                // Â¹ÃžÃ€Âº Â´Ã«Â¹ÃŒÃÃ¶Â¸Â¦ Ã…Ã«ÂºÂ¸Ã‡Ã‘Â´Ã™. <- HPÂ¸Â¦ Â±Ã—Â´Ã«Â·ÃŽ Â¾Ã‹Â¸Â°Â´Ã™.
                                                SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_HP, 0, 0, 0, 0);

                                                if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0)
                                                {
                                                    // Â¸Â¶ÂºÃ± Â»Ã³Ã…Ã‚Â°Â¡ Ã‡Â®Â¸Â°Â´Ã™.	
                                                    // 1: Hold-Person 
                                                    // 2: Paralize
                                                    SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_HOLDOBJECT, m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT], 0, 0);

                                                    m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
                                                    bRemoveFromDelayEventList(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_HOLDOBJECT);
                                                }
                                            }

                                            // v1.42 Poison ÃˆÂ¿Â°Ãº
                                            if ((bCheckResistingMagicSuccess(1, sOwnerH, DEF_OWNERTYPE_PLAYER, 100) == false) &&
                                                (m_pClientList[sOwnerH]->m_bIsPoisoned == false))
                                            {

                                                m_pClientList[sOwnerH]->m_bIsPoisoned = true;
                                                m_pClientList[sOwnerH]->m_iPoisonLevel = m_pDynamicObjectList[i]->m_iV1;
                                                m_pClientList[sOwnerH]->m_dwPoisonTime = dwTime;
                                                // ÃÃŸÂµÂ¶ÂµÃ‡Â¾ÃºÃ€Â½Ã€Â» Â¾Ã‹Â¸Â°Â´Ã™. 
                                                SetPoisonFlag(sOwnerH, cOwnerType, true);// poison aura appears from dynamic objects
                                                SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_POISON, m_pClientList[sOwnerH]->m_iPoisonLevel, 0, 0);
                                            }
                                        }
                                        break;

                                    case DEF_OWNERTYPE_NPC:
                                        // Â¸Ã³Â½ÂºÃ…ÃÃ€Ã‡ ÃÃŸÂµÂ¶ÃˆÂ¿Â°ÃºÂ´Ã‚ Â¾Ã†ÃÃ· Â±Â¸Ã‡Ã¶ Â¾ÃˆÂµÃŠ
                                        if (m_pNpcList[sOwnerH] == 0) break;

                                        // Ã€ÃŒÂ·Â± Â½Ã„Ã€Â¸Â·ÃŽ Â´Ã«Â¹ÃŒÃÃ¶Ã€Ã‡ Ã…Â©Â±Ã¢Â¸Â¦ Â°Ã¡ÃÂ¤
                                        if (m_pDynamicObjectList[i]->m_iV1 < 20)
                                            iDamage = iDice(1, 6);
                                        else iDamage = iDice(1, 8);

                                        // Ã€Ã¼Ã€Ã¯Â¿Ã« Â±Â¸ÃÂ¶Â¹Â°ÃÃŸ Â±Ã—Â·Â£ÂµÃ¥ Â¸Ã…ÃÃ· ÃÂ¦Â³Ã—Â·Â¹Ã€ÃŒÃ…Ã, Â¿Â¡Â³ÃŠÃÃ¶ Â½Ã‡ÂµÃ¥ ÃÂ¦Â³Ã—Â·Â¹Ã€ÃŒÃ…ÃÂ´Ã‚ Ã‡ÃŠÂµÃ¥Â·Ã¹ Â¸Â¶Â¹Ã½Â¿Â¡ Â´Ã«Â¹ÃŒÃÃ¶Â¸Â¦ Ã€Ã”ÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™.
                                        switch (m_pNpcList[sOwnerH]->m_sType)
                                        {
                                            case 40: // ESG
                                            case 41: // GMG
                                            case 67: // McGaffin
                                            case 68: // Perry
                                            case 69: // Devlin
                                                iDamage = 0;
                                                break;
                                        }

                                        // HPÂ¿Â¡Â¼Â­ Â»Â«Â´Ã™. Action LimitÂ¿Â¡ ÂµÃ»Â¶Ã³ ÃƒÂ³Â¸Â®Ã‡Ã‘Â´Ã™.
                                        switch (m_pNpcList[sOwnerH]->m_cActionLimit)
                                        {
                                            case 0: // Ã€ÃÂ¹Ã
                                            case 3: // Â´ÃµÂ¹ÃŒÂ·Ã¹
                                            case 5: // Â°Ã‡ÃƒÃ Â¹Â° 
                                                m_pNpcList[sOwnerH]->m_iHP -= iDamage;
                                                break;
                                        }
                                        //if (m_pNpcList[sOwnerH]->m_cActionLimit == 0) 
                                        //	m_pNpcList[sOwnerH]->m_iHP -= iDamage;

                                        if (m_pNpcList[sOwnerH]->m_iHP <= 0)
                                        {
                                            // NPCÂ°Â¡ Â»Ã§Â¸ÃÃ‡ÃŸÂ´Ã™.
                                            NpcKilledHandler(sOwnerH, cOwnerType, sOwnerH, 0); //v1.2 ÃÃŸÂµÂ¶Â±Â¸Â¸Â§Â¿Â¡ ÃÃ—Ã€Â¸Â¸Ã© Â¸Â¶ÃÃ¶Â¸Â· Â´Ã«Â¹ÃŒÃÃ¶Â°Â¡ 0. Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â½Â±Â°Ã” Â±Â¸Ã‡ÃÃÃ¶ Â¸Ã¸Ã‡ÃÂ°Ã” Ã‡ÃÂ±Ã¢ Ã€Â§Ã‡Ã”.
                                        }
                                        else
                                        {
                                            // DamageÂ¸Â¦ Ã€Ã”Ã€Âº ÃƒÃ¦Â°ÃÃ€Â¸Â·ÃŽ Ã€ÃŽÃ‡Ã‘ ÃÃ¶Â¿Â¬ÃˆÂ¿Â°Ãº.
                                            if (iDice(1, 3) == 2)
                                                m_pNpcList[sOwnerH]->m_dwTime = dwTime;

                                            if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0)
                                            {
                                                // Hold ÂµÃ‡Â¾Ã® Ã€Ã–Â¾ÃºÂ´Ã¸ Â»Ã³Ã…Ã‚Â¶Ã³Â¸Ã© Ã‡Â®Â¸Â°Â´Ã™. 	
                                                m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
                                            }

                                            // NPCÂ¸Â¦ ÂµÂµÂ¸ÃÂ°Â¡Â´Ã‚ Â¸Ã°ÂµÃ¥Â·ÃŽ Ã€Ã¼ÃˆÂ¯Â½ÃƒÃ…Â²Â´Ã™.
                                            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);
                                        }
                                        break;
                                }
                            }
                        }
                    break;

                case DEF_DYNAMICOBJECT_ICESTORM:
                    for (ix = m_pDynamicObjectList[i]->m_sX - 2; ix <= m_pDynamicObjectList[i]->m_sX + 2; ix++)
                        for (iy = m_pDynamicObjectList[i]->m_sY - 2; iy <= m_pDynamicObjectList[i]->m_sY + 2; iy++)
                        {

                            m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                            if (sOwnerH != 0)
                            {
                                switch (cOwnerType)
                                {
                                    case DEF_OWNERTYPE_PLAYER:
                                        if (m_pClientList[sOwnerH] == 0) break;
                                        if (m_pClientList[sOwnerH]->m_bIsKilled == true) break;

                                        iDamage = iDice(3, 3) + 5;

                                        if (m_pClientList[sOwnerH]->m_iAdminUserLevel == 0)
                                            m_pClientList[sOwnerH]->m_iHP -= iDamage;

                                        if (m_pClientList[sOwnerH]->m_iHP <= 0)
                                        {
                                            // Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â°Â¡ Â»Ã§Â¸ÃÃ‡ÃŸÂ´Ã™.
                                            ClientKilledHandler(sOwnerH, sOwnerH, cOwnerType, iDamage);
                                        }
                                        else
                                        {
                                            if (iDamage > 0)
                                            {

                                                SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_HP, 0, 0, 0, 0);

                                                if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] == 1)
                                                {

                                                    SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_HOLDOBJECT, m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT], 0, 0);

                                                    m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
                                                    bRemoveFromDelayEventList(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_HOLDOBJECT);
                                                }
                                            }

                                            if ((bCheckResistingIceSuccess(1, sOwnerH, DEF_OWNERTYPE_PLAYER, m_pDynamicObjectList[i]->m_iV1) == false) &&
                                                (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0))
                                            {

                                                m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                                SetIceFlag(sOwnerH, cOwnerType, true);
                                                // ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
                                                bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (20 * 1000),
                                                    sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);

                                                SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
                                            }
                                        }
                                        break;

                                    case DEF_OWNERTYPE_NPC:
                                        if (m_pNpcList[sOwnerH] == 0) break;

                                        iDamage = iDice(3, 3) + 5;

                                        switch (m_pNpcList[sOwnerH]->m_sType)
                                        {
                                            case 40: // ESG
                                            case 41: // GMG
                                            case 67: // McGaffin
                                            case 68: // Perry
                                            case 69: // Devlin
                                                iDamage = 0;
                                                break;
                                        }

                                        switch (m_pNpcList[sOwnerH]->m_cActionLimit)
                                        {
                                            case 0:
                                            case 3:
                                            case 5:
                                                m_pNpcList[sOwnerH]->m_iHP -= iDamage;
                                                break;
                                        }

                                        if (m_pNpcList[sOwnerH]->m_iHP <= 0)
                                        {
                                            // NPCÂ°Â¡ Â»Ã§Â¸ÃÃ‡ÃŸÂ´Ã™.
                                            NpcKilledHandler(sOwnerH, cOwnerType, sOwnerH, 0); //v1.2 Ã…Â¸Â¼Â­ ÃÃ—Ã€Â¸Â¸Ã© Â¸Â¶ÃÃ¶Â¸Â· Â´Ã«Â¹ÃŒÃÃ¶Â°Â¡ 0. Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â½Â±Â°Ã” Â±Â¸Ã‡ÃÃÃ¶ Â¸Ã¸Ã‡ÃÂ°Ã” Ã‡ÃÂ±Ã¢ Ã€Â§Ã‡Ã”.
                                        }
                                        else
                                        {
                                            // DamageÂ¸Â¦ Ã€Ã”Ã€Âº ÃƒÃ¦Â°ÃÃ€Â¸Â·ÃŽ Ã€ÃŽÃ‡Ã‘ ÃÃ¶Â¿Â¬ÃˆÂ¿Â°Ãº.
                                            if (iDice(1, 3) == 2)
                                                m_pNpcList[sOwnerH]->m_dwTime = dwTime;

                                            if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0)
                                            {
                                                // Hold ÂµÃ‡Â¾Ã® Ã€Ã–Â¾ÃºÂ´Ã¸ Â»Ã³Ã…Ã‚Â¶Ã³Â¸Ã© Ã‡Â®Â¸Â°Â´Ã™. 	
                                                m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
                                            }

                                            // NPCÂ¸Â¦ ÂµÂµÂ¸ÃÂ°Â¡Â´Ã‚ Â¸Ã°ÂµÃ¥Â·ÃŽ Ã€Ã¼ÃˆÂ¯Â½ÃƒÃ…Â²Â´Ã™.
                                            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);

                                            // v1.42 Ice ÃˆÂ¿Â°Ãº
                                            if ((bCheckResistingIceSuccess(1, sOwnerH, DEF_OWNERTYPE_NPC, m_pDynamicObjectList[i]->m_iV1) == false) &&
                                                (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0))
                                            {

                                                m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                                SetIceFlag(sOwnerH, cOwnerType, true);
                                                // ÃˆÂ¿Â°ÃºÂ°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‰ Â¶Â§ Â¹ÃŸÂ»Ã½Ã‡Ã’ ÂµÃ´Â·Â¹Ã€ÃŒ Ã€ÃŒÂºÂ¥Ã†Â®Â¸Â¦ ÂµÃ®Â·ÃÃ‡Ã‘Â´Ã™.
                                                bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (20 * 1000),
                                                    sOwnerH, cOwnerType, 0, 0, 0, 1, 0, 0);
                                            }
                                        }
                                        break;
                                }
                            }

                            // ÃÃ—Ã€ÂºÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã„Â³Â¸Â¯Ã€ÃŒ Ã€Ã–Â´Ã™Â¸Ã©
                            m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
                            if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                                (m_pClientList[sOwnerH]->m_iHP > 0))
                            {
                                // ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
                                iDamage = iDice(3, 2);
                                m_pClientList[sOwnerH]->m_iHP -= iDamage;

                                if (m_pClientList[sOwnerH]->m_iHP <= 0)
                                {
                                    // Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â°Â¡ Â»Ã§Â¸ÃÃ‡ÃŸÂ´Ã™.
                                    ClientKilledHandler(sOwnerH, sOwnerH, cOwnerType, iDamage);
                                }
                                else
                                {
                                    if (iDamage > 0)
                                    {
                                        // Â¹ÃžÃ€Âº Â´Ã«Â¹ÃŒÃÃ¶Â¸Â¦ Ã…Ã«ÂºÂ¸Ã‡Ã‘Â´Ã™. <- HPÂ¸Â¦ Â±Ã—Â´Ã«Â·ÃŽ Â¾Ã‹Â¸Â°Â´Ã™.
                                        SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_HP, 0, 0, 0, 0);
                                    }
                                }
                            }

                            // Â±Ã™ÃƒÂ³Â¿Â¡ Fire ObjectÂ°Â¡ Ã€Ã–Â´Ã™Â¸Ã© Â¼Ã¶Â¸Ã­Ã€Â» ÃÃ™Ã€ÃŽÂ´Ã™.
                            m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->bGetDynamicObject(ix, iy, &sType, &dwRegisterTime, &iIndex);
                            if (((sType == DEF_DYNAMICOBJECT_FIRE) || (sType == DEF_DYNAMICOBJECT_FIRE3)) && (m_pDynamicObjectList[iIndex] != 0))
                                m_pDynamicObjectList[iIndex]->m_dwLastTime = m_pDynamicObjectList[iIndex]->m_dwLastTime - (m_pDynamicObjectList[iIndex]->m_dwLastTime / 10);
                        }
                    break;

                case DEF_DYNAMICOBJECT_FIRE3:
                case DEF_DYNAMICOBJECT_FIRE:
                    // Fire-WallÂ·Ã¹Ã€Ã‡ Ã…Â¸Â´Ã‚ ÂºÃ’Â²Ã‰
                    if (m_pDynamicObjectList[i]->m_iCount == 1)
                    {
                        // Â±Ã™ÃƒÂ³Â¿Â¡ Ã…Â¸Â´Ã‚ Â¹Â°Â°Ã‡Ã€ÃŒ Ã€Ã–Â´Ã™Â¸Ã© Â¹Ã¸ÃÃ¸Â´Ã™. 
                        CheckFireBluring(m_pDynamicObjectList[i]->m_cMapIndex, m_pDynamicObjectList[i]->m_sX, m_pDynamicObjectList[i]->m_sY);
                    }
                    m_pDynamicObjectList[i]->m_iCount++;
                    if (m_pDynamicObjectList[i]->m_iCount > 10) m_pDynamicObjectList[i]->m_iCount = 10;


                    for (ix = m_pDynamicObjectList[i]->m_sX - 1; ix <= m_pDynamicObjectList[i]->m_sX + 1; ix++)
                        for (iy = m_pDynamicObjectList[i]->m_sY - 1; iy <= m_pDynamicObjectList[i]->m_sY + 1; iy++)
                        {

                            m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                            if (sOwnerH != 0)
                            {
                                // Fire DamageÂ¸Â¦ Ã€Ã”Â´Ã‚Â´Ã™.
                                switch (cOwnerType)
                                {

                                    case DEF_OWNERTYPE_PLAYER:
                                        if (m_pClientList[sOwnerH] == 0) break;
                                        if (m_pClientList[sOwnerH]->m_bIsKilled == true) break;
                                        // v1.41 ÃÃŸÂ¸Â³Ã€ÃŒÂ°Ã­ Ã€Ã¼Ã…ÃµÂ¸Ã°ÂµÃ¥Â°Â¡ Â¾Ã†Â´ÃÂ¸Ã© Ã‡Ã‡Ã‡Ã˜Â¸Â¦ Ã€Ã”ÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™.
                                        //if ((m_pClientList[sOwnerH]->m_bIsNeutral == true) && (m_pClientList[sOwnerH]->m_sAppr2 & 0xF000) == 0) break;

                                        iDamage = iDice(1, 6);
                                        // New 17/05/2004
                                        if (m_pClientList[sOwnerH]->m_iAdminUserLevel == 0)
                                            m_pClientList[sOwnerH]->m_iHP -= iDamage;

                                        if (m_pClientList[sOwnerH]->m_iHP <= 0)
                                        {
                                            // Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â°Â¡ Â»Ã§Â¸ÃÃ‡ÃŸÂ´Ã™.
                                            ClientKilledHandler(sOwnerH, sOwnerH, cOwnerType, iDamage);
                                        }
                                        else
                                        {
                                            if (iDamage > 0)
                                            {
                                                // Â¹ÃžÃ€Âº Â´Ã«Â¹ÃŒÃÃ¶Â¸Â¦ Ã…Ã«ÂºÂ¸Ã‡Ã‘Â´Ã™. <- HPÂ¸Â¦ Â±Ã—Â´Ã«Â·ÃŽ Â¾Ã‹Â¸Â°Â´Ã™.
                                                SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_HP, 0, 0, 0, 0);

                                                if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0)
                                                {
                                                    // Hold-Person ÂµÃ‡Â¾Ã® Ã€Ã–Â¾ÃºÂ´Ã¸ Â»Ã³Ã…Ã‚Â¶Ã³Â¸Ã© Ã‡Â®Â¸Â°Â´Ã™. Fire FieldÂ·ÃŽÂ´Ã‚ Ã†ÃÂ·Â²Â¶Ã³Ã€ÃŒÃÃ® ÂµÃˆÂ°ÃÂµÂµ Ã‡Â®Â¸Â°Â´Ã™. 	
                                                    // 1: Hold-Person 
                                                    // 2: Paralize	
                                                    SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_HOLDOBJECT, m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT], 0, 0);

                                                    m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
                                                    bRemoveFromDelayEventList(sOwnerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_HOLDOBJECT);
                                                }
                                            }
                                        }
                                        break;

                                    case DEF_OWNERTYPE_NPC:
                                        if (m_pNpcList[sOwnerH] == 0) break;

                                        iDamage = iDice(1, 6);

                                        // Ã€Ã¼Ã€Ã¯Â¿Ã« Â±Â¸ÃÂ¶Â¹Â°ÃÃŸ Â±Ã—Â·Â£ÂµÃ¥ Â¸Ã…ÃÃ· ÃÂ¦Â³Ã—Â·Â¹Ã€ÃŒÃ…Ã, Â¿Â¡Â³ÃŠÃÃ¶ Â½Ã‡ÂµÃ¥ ÃÂ¦Â³Ã—Â·Â¹Ã€ÃŒÃ…ÃÂ´Ã‚ Ã‡ÃŠÂµÃ¥Â·Ã¹ Â¸Â¶Â¹Ã½Â¿Â¡ Â´Ã«Â¹ÃŒÃÃ¶Â¸Â¦ Ã€Ã”ÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™.
                                        switch (m_pNpcList[sOwnerH]->m_sType)
                                        {
                                            case 40: // ESG
                                            case 41: // GMG
                                            case 67: // McGaffin
                                            case 68: // Perry
                                            case 69: // Devlin
                                                iDamage = 0;
                                                break;
                                        }

                                        // HPÂ¿Â¡Â¼Â­ Â»Â«Â´Ã™. Action LimitÂ¿Â¡ ÂµÃ»Â¶Ã³ ÃƒÂ³Â¸Â®Ã‡Ã‘Â´Ã™.
                                        switch (m_pNpcList[sOwnerH]->m_cActionLimit)
                                        {
                                            case 0: // Ã€ÃÂ¹Ã
                                            case 3: // Â´ÃµÂ¹ÃŒÂ·Ã¹
                                            case 5: // Â°Ã‡ÃƒÃ Â¹Â° 
                                                m_pNpcList[sOwnerH]->m_iHP -= iDamage;
                                                break;
                                        }
                                        //if (m_pNpcList[sOwnerH]->m_cActionLimit == 0) 
                                        //	m_pNpcList[sOwnerH]->m_iHP -= iDamage;

                                        if (m_pNpcList[sOwnerH]->m_iHP <= 0)
                                        {
                                            // NPCÂ°Â¡ Â»Ã§Â¸ÃÃ‡ÃŸÂ´Ã™.
                                            NpcKilledHandler(sOwnerH, cOwnerType, sOwnerH, 0); //v1.2 Ã…Â¸Â¼Â­ ÃÃ—Ã€Â¸Â¸Ã© Â¸Â¶ÃÃ¶Â¸Â· Â´Ã«Â¹ÃŒÃÃ¶Â°Â¡ 0. Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â½Â±Â°Ã” Â±Â¸Ã‡ÃÃÃ¶ Â¸Ã¸Ã‡ÃÂ°Ã” Ã‡ÃÂ±Ã¢ Ã€Â§Ã‡Ã”.
                                        }
                                        else
                                        {
                                            // DamageÂ¸Â¦ Ã€Ã”Ã€Âº ÃƒÃ¦Â°ÃÃ€Â¸Â·ÃŽ Ã€ÃŽÃ‡Ã‘ ÃÃ¶Â¿Â¬ÃˆÂ¿Â°Ãº.
                                            if (iDice(1, 3) == 2)
                                                m_pNpcList[sOwnerH]->m_dwTime = dwTime;

                                            if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0)
                                            {
                                                // Hold ÂµÃ‡Â¾Ã® Ã€Ã–Â¾ÃºÂ´Ã¸ Â»Ã³Ã…Ã‚Â¶Ã³Â¸Ã© Ã‡Â®Â¸Â°Â´Ã™. 	
                                                m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
                                            }

                                            // NPCÂ¸Â¦ ÂµÂµÂ¸ÃÂ°Â¡Â´Ã‚ Â¸Ã°ÂµÃ¥Â·ÃŽ Ã€Ã¼ÃˆÂ¯Â½ÃƒÃ…Â²Â´Ã™.
                                            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);
                                        }
                                        break;
                                }
                            }

                            // ÃÃ—Ã€ÂºÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã„Â³Â¸Â¯Ã€ÃŒ Ã€Ã–Â´Ã™Â¸Ã©
                            m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, ix, iy);
                            if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0) &&
                                (m_pClientList[sOwnerH]->m_iHP > 0))
                            {
                                // ÃÃ—Ã€Âº ÃƒÂ´Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â´Ã™.
                                iDamage = iDice(1, 6);
                                m_pClientList[sOwnerH]->m_iHP -= iDamage;

                                if (m_pClientList[sOwnerH]->m_iHP <= 0)
                                {
                                    // Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â°Â¡ Â»Ã§Â¸ÃÃ‡ÃŸÂ´Ã™.
                                    ClientKilledHandler(sOwnerH, sOwnerH, cOwnerType, iDamage);
                                }
                                else
                                {
                                    if (iDamage > 0)
                                    {
                                        // Â¹ÃžÃ€Âº Â´Ã«Â¹ÃŒÃÃ¶Â¸Â¦ Ã…Ã«ÂºÂ¸Ã‡Ã‘Â´Ã™. <- HPÂ¸Â¦ Â±Ã—Â´Ã«Â·ÃŽ Â¾Ã‹Â¸Â°Â´Ã™.
                                        SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_HP, 0, 0, 0, 0);
                                    }
                                }
                            }

                            // Â±Ã™ÃƒÂ³Â¿Â¡ Ice ObjectÂ°Â¡ Ã€Ã–Â´Ã™Â¸Ã© Â¼Ã¶Â¸Ã­Ã€Â» ÃÃ™Ã€ÃŽÂ´Ã™.
                            m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->bGetDynamicObject(ix, iy, &sType, &dwRegisterTime, &iIndex);
                            if ((sType == DEF_DYNAMICOBJECT_ICESTORM) && (m_pDynamicObjectList[iIndex] != 0))
                                m_pDynamicObjectList[iIndex]->m_dwLastTime = m_pDynamicObjectList[iIndex]->m_dwLastTime - (m_pDynamicObjectList[iIndex]->m_dwLastTime / 10);
                        }
                    break;
            }
        }
}

void CGame::EnergySphereProcessor(bool bIsAdminCreate, int iClientH)
{
    int i, iNamingValue, iCIndex, iTemp, pX, pY;
    char cSA, cName_Internal[31], cWaypoint[31];

    if (bIsAdminCreate != true)
    {

        if (m_iMiddlelandMapIndex < 0) return;
        if (m_pMapList[m_iMiddlelandMapIndex] == 0) return;
        // Â¿Â¡Â³ÃŠÃÃ¶ Â½ÂºÃ‡Ã‡Â¾Ã®Â°Â¡ Â¸Â¸ÂµÃ©Â¾Ã®ÃÃº ÃˆÂ®Â·Ã¼Ã€Âº 3ÃƒÃŠÂ¸Â¶Â´Ã™ 2000ÂºÃÃ€Ã‡ 1
        if (iDice(1, 2000) != 123) return;
        // Â¿Â¡Â³ÃŠÃÃ¶ Â½ÂºÃ‡Ã‡Â¾Ã®Â´Ã‚ ÃƒÃ‘ Â»Ã§Â¿Ã«Ã€Ãš 500Â¸Ã­ Ã€ÃŒÂ»Ã³Ã€ÃÂ¶Â§Â¸Â¸ Â¸Â¸ÂµÃ©Â¾Ã® ÃÃ¸Â´Ã™.
        if (m_iTotalGameServerClients < 500) return;

        // Â¸Â¸Â¾Ã  Ã€ÃŒÂ¹ÃŒ Â¿Â¡Â³ÃŠÃÃ¶ Â½ÂºÃ‡Ã‡Â¾Ã®Ã€Ã‡ Â°Ã±Ã€ÃŒ Ã‡Ã’Â´Ã§ÂµÃ‡Â¾Ã® Ã€Ã–Ã€Â¸Â¸Ã© Â¸Â¸ÂµÃ©Â¸Ã© Â¾ÃˆÂµÃˆÂ´Ã™.
        if (m_pMapList[m_iMiddlelandMapIndex]->m_iCurEnergySphereGoalPointIndex >= 0) return;

        // Â¿Â¡Â³ÃŠÃÃ¶ Â½ÂºÃ‡Ã‡Â¾Ã®Â¸Â¦ Â¸Â¸ÂµÃ© Ã€Â§Ã„Â¡Â¸Â¦ Â°Ã¡ÃÂ¤ 
        iCIndex = iDice(1, m_pMapList[m_iMiddlelandMapIndex]->m_iTotalEnergySphereCreationPoint);

        // Â¿Â¡Â³ÃŠÃÃ¶ Â½ÂºÃ‡Ã‡Â¾Ã® Â½ÃƒÃ€Ã› Ã€Â§Ã„Â¡Â°Â¡ ÃƒÃŠÂ±Ã¢ÃˆÂ­Â°Â¡ Â¾ÃˆÂµÃˆ Â»Ã³Ã…Ã‚Â¶Ã³Â¸Ã© 
        if (m_pMapList[m_iMiddlelandMapIndex]->m_stEnergySphereCreationList[iCIndex].cType == 0) return;

        // Â¿Â©Â±Ã¢Â¼Â­ Â¿Â¡Â³ÃŠÃÃ¶ Â½ÂºÃ‡Ã‡Â¾Ã® Â»Ã½Â¼Âº 
        cSA = 0;
        pX = m_pMapList[m_iMiddlelandMapIndex]->m_stEnergySphereCreationList[iCIndex].sX;
        pY = m_pMapList[m_iMiddlelandMapIndex]->m_stEnergySphereCreationList[iCIndex].sY;
        memset(cWaypoint, 0, sizeof(cWaypoint));

        iNamingValue = m_pMapList[m_iMiddlelandMapIndex]->iGetEmptyNamingValue();
        if (iNamingValue != -1)
        {
            // 
            memset(cName_Internal, 0, sizeof(cName_Internal));
            copy_string(cName_Internal, "XX%d", iNamingValue);
            cName_Internal[0] = '_';
            cName_Internal[1] = m_iMiddlelandMapIndex + 65;

            if ((bCreateNewNpc("Energy-Sphere", cName_Internal, m_pMapList[m_iMiddlelandMapIndex]->m_cName, (rand() % 5), cSA, DEF_MOVETYPE_RANDOM, &pX, &pY, cWaypoint, 0, 0, -1, false, false, false)) == false)
            {
                m_pMapList[m_iMiddlelandMapIndex]->SetNamingValueEmpty(iNamingValue);
                return;
            }
        }

        // Â¿Â¡Â³ÃŠÃÃ¶ Â½ÂºÃ‡Ã‡Â¾Ã® Â°Ã± Â»Ã½Â¼Âº 
        iTemp = iDice(1, m_pMapList[m_iMiddlelandMapIndex]->m_iTotalEnergySphereGoalPoint);
        if (m_pMapList[m_iMiddlelandMapIndex]->m_stEnergySphereGoalList[iTemp].cResult == 0) return;

        // Ã‡Ã¶Ã€Ã§ Â¿Â¡Â³ÃŠÃÃ¶ Â½ÂºÃ‡Ã‡Â¾Ã® Â°Ã± Ã€ÃŽÂµÂ¦Â½Âº Ã€ÃºÃ€Ã¥ 
        m_pMapList[m_iMiddlelandMapIndex]->m_iCurEnergySphereGoalPointIndex = iTemp;

        // Â¿Â¡Â³ÃŠÃÃ¶ Â½ÂºÃ‡Ã‡Â¾Ã®Â°Â¡ Â¸Â¸ÂµÃ©Â¾Ã® ÃÂ³Ã€Â½Ã€Â» Â¸Ã°ÂµÃ§ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¿Â¡Â°Ã” Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
        for (i = 1; i < DEF_MAXCLIENTS; i++)
            if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_bIsInitComplete == true))
            {
                SendNotifyMsg(0, i, DEF_NOTIFY_ENERGYSPHERECREATED, pX, pY, 0, 0);
            }

        //testcode
        log->info("(!) Energy Sphere Created! ({}, {})", pX, pY);
    }
    else
    {
        // Â¸Â¸Â¾Ã  Ã€ÃŒÂ¹ÃŒ Â¿Â¡Â³ÃŠÃÃ¶ Â½ÂºÃ‡Ã‡Â¾Ã®Ã€Ã‡ Â°Ã±Ã€ÃŒ Ã‡Ã’Â´Ã§ÂµÃ‡Â¾Ã® Ã€Ã–Ã€Â¸Â¸Ã© Â¸Â¸ÂµÃ©Â¸Ã© Â¾ÃˆÂµÃˆÂ´Ã™.
        if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_iCurEnergySphereGoalPointIndex >= 0) return;

        // Â¿Â¡Â³ÃŠÃÃ¶ Â½ÂºÃ‡Ã‡Â¾Ã®Â¸Â¦ Â¸Â¸ÂµÃ© Ã€Â§Ã„Â¡Â¸Â¦ Â°Ã¡ÃÂ¤ 
        iCIndex = iDice(1, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_iTotalEnergySphereCreationPoint);

        // Â¿Â¡Â³ÃŠÃÃ¶ Â½ÂºÃ‡Ã‡Â¾Ã® Â½ÃƒÃ€Ã› Ã€Â§Ã„Â¡Â°Â¡ ÃƒÃŠÂ±Ã¢ÃˆÂ­Â°Â¡ Â¾ÃˆÂµÃˆ Â»Ã³Ã…Ã‚Â¶Ã³Â¸Ã© 
        if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stEnergySphereCreationList[iCIndex].cType == 0) return;

        // Â¿Â©Â±Ã¢Â¼Â­ Â¿Â¡Â³ÃŠÃÃ¶ Â½ÂºÃ‡Ã‡Â¾Ã® Â»Ã½Â¼Âº 
        cSA = 0;
        pX = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stEnergySphereCreationList[iCIndex].sX;
        pY = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stEnergySphereCreationList[iCIndex].sY;
        memset(cWaypoint, 0, sizeof(cWaypoint));

        iNamingValue = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->iGetEmptyNamingValue();
        if (iNamingValue != -1)
        {
            // 
            memset(cName_Internal, 0, sizeof(cName_Internal));
            copy_string(cName_Internal, "XX%d", iNamingValue);
            cName_Internal[0] = '_';
            cName_Internal[1] = m_pClientList[iClientH]->m_cMapIndex + 65;

            if ((bCreateNewNpc("Energy-Sphere", cName_Internal, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, (rand() % 5), cSA, DEF_MOVETYPE_RANDOM, &pX, &pY, cWaypoint, 0, 0, -1, false, false, false)) == false)
            {
                // Â½Ã‡Ã†ÃÃ‡ÃŸÃ€Â¸Â¹Ã‡Â·ÃŽ Â¿Â¹Â¾Ã ÂµÃˆ NameValueÂ¸Â¦ Ã‡Ã˜ÃÂ¦Â½ÃƒÃ…Â²Â´Ã™.
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetNamingValueEmpty(iNamingValue);
                return;
            }
        }

        // Â¿Â¡Â³ÃŠÃÃ¶ Â½ÂºÃ‡Ã‡Â¾Ã® Â°Ã± Â»Ã½Â¼Âº 
        iTemp = iDice(1, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_iTotalEnergySphereGoalPoint);
        if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stEnergySphereGoalList[iTemp].cResult == 0) return;

        // Ã‡Ã¶Ã€Ã§ Â¿Â¡Â³ÃŠÃÃ¶ Â½ÂºÃ‡Ã‡Â¾Ã® Â°Ã± Ã€ÃŽÂµÂ¦Â½Âº Ã€ÃºÃ€Ã¥ 
        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_iCurEnergySphereGoalPointIndex = iTemp;

        // Â¿Â¡Â³ÃŠÃÃ¶ Â½ÂºÃ‡Ã‡Â¾Ã®Â°Â¡ Â¸Â¸ÂµÃ©Â¾Ã® ÃÂ³Ã€Â½Ã€Â» Â¸Ã°ÂµÃ§ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¿Â¡Â°Ã” Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
        for (i = 1; i < DEF_MAXCLIENTS; i++)
            if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_bIsInitComplete == true))
            {
                SendNotifyMsg(0, i, DEF_NOTIFY_ENERGYSPHERECREATED, pX, pY, 0, 0);
            }

        //testcode
        log->info("(!) Admin Energy Sphere Created! ({}, {})", pX, pY);
    }
}

void CGame::WhetherProcessor()
{
    char cPrevMode;
    int i, j;
    uint32_t dwTime;

    dwTime = timeGetTime();

    for (i = 0; i < DEF_MAXMAPS; i++)
    {
        if ((m_pMapList[i] != 0) && (m_pMapList[i]->m_bIsFixedDayMode == false))
        {
            cPrevMode = m_pMapList[i]->m_cWhetherStatus;
            if (m_pMapList[i]->m_cWhetherStatus != 0)
            {
                if ((dwTime - m_pMapList[i]->m_dwWhetherStartTime) > m_pMapList[i]->m_dwWhetherLastTime)
                    m_pMapList[i]->m_cWhetherStatus = 0;
            }
            else
            {
                if (iDice(1, 300) == 13)
                {
                    m_pMapList[i]->m_cWhetherStatus = iDice(1, 3); //This looks better or else we only get snow :(
                    //m_pMapList[i]->m_cWhetherStatus = iDice(1,3)+3; <- This original code looks fucked
                    m_pMapList[i]->m_dwWhetherStartTime = dwTime;
                    m_pMapList[i]->m_dwWhetherLastTime = 60000 * 3 + 60000 * iDice(1, 7);
                }
            }

            if (m_pMapList[i]->m_bIsSnowEnabled == true)
            {
                m_pMapList[i]->m_cWhetherStatus = iDice(1, 3) + 3;
                m_pMapList[i]->m_dwWhetherStartTime = dwTime;
                m_pMapList[i]->m_dwWhetherLastTime = 60000 * 3 + 60000 * iDice(1, 7);
            }

            if (cPrevMode != m_pMapList[i]->m_cWhetherStatus)
            {
                for (j = 1; j < DEF_MAXCLIENTS; j++)
                    if ((m_pClientList[j] != 0) && (m_pClientList[j]->m_bIsInitComplete == true) && (m_pClientList[j]->m_cMapIndex == i))
                        SendNotifyMsg(0, j, DEF_NOTIFY_WHETHERCHANGE, m_pMapList[i]->m_cWhetherStatus, 0, 0, 0);
            }
        } //If
    } //for Loop
}

void CGame::FightzoneReserveProcessor()
{
}
