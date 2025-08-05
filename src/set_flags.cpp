//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include "Game.h"

/*********************************************************************************************************************
**  void CGame::SetInvisibilityFlag(short sOwnerH, char cOwnerType, bool bStatus)									**
**  description			:: changes the status of the player to show invisibility aura								**
**  last updated		:: November 20, 2004; 9:30 PM; Hypnotoad													**
**	return value		:: void																						**
*********************************************************************************************************************/
void CGame::SetInvisibilityFlag(short sOwnerH, char cOwnerType, bool bStatus)
{
    switch (cOwnerType)
    {
        case DEF_OWNERTYPE_PLAYER:
            if (m_pClientList[sOwnerH] == 0) return;
            if (bStatus == true)
                m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00000010;
            else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFFFFFEF;
            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
            break;

        case DEF_OWNERTYPE_NPC:
            if (m_pNpcList[sOwnerH] == 0) return;
            if (bStatus == true)
                m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x00000010;
            else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFFFFFFEF;
            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
            break;
    }
}

/*********************************************************************************************************************
**  void CGame::SetInhibitionCastingFlag(short sOwnerH, char cOwnerType, bool bStatus)								**
**  description			:: changes the status of the player to show inhibit casting aura							**
**  last updated		:: November 20, 2004; 9:33 PM; Hypnotoad													**
**	return value		:: void																						**
*********************************************************************************************************************/
void CGame::SetInhibitionCastingFlag(short sOwnerH, char cOwnerType, bool bStatus)
{
    switch (cOwnerType)
    {
        case DEF_OWNERTYPE_PLAYER:
            if (m_pClientList[sOwnerH] == 0) return;
            if (bStatus == true)
                m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00100000;
            else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFEFFFFF;
            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
            break;

        case DEF_OWNERTYPE_NPC:
            if (m_pNpcList[sOwnerH] == 0) return;
            if (bStatus == true)
                m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x00100000;
            else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFFEFFFFF;
            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
            break;
    }
}

/*********************************************************************************************************************
**  void void CGame::SetBerserkFlag(short sOwnerH, char cOwnerType, bool bStatus)									**
**  description			:: changes the status of the player to show berserk aura									**
**  last updated		:: November 20, 2004; 9:35 PM; Hypnotoad													**
**	return value		:: void																						**
*********************************************************************************************************************/
void CGame::SetBerserkFlag(short sOwnerH, char cOwnerType, bool bStatus)
{
    switch (cOwnerType)
    {
        case DEF_OWNERTYPE_PLAYER:
            if (m_pClientList[sOwnerH] == 0) return;
            if (bStatus == true)
                m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00000020;
            else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFFFFFDF;
            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
            break;

        case DEF_OWNERTYPE_NPC:
            if (m_pNpcList[sOwnerH] == 0) return;
            if (bStatus == true)
                m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x00000020;
            else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFFFFFFDF;
            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
            break;
    }
}

/*********************************************************************************************************************
**  void void CGame::SetIceFlag(short sOwnerH, char cOwnerType, bool bStatus)										**
**  description			:: changes the status of the player to show frozen aura										**
**  last updated		:: November 20, 2004; 9:35 PM; Hypnotoad													**
**	return value		:: void																						**
*********************************************************************************************************************/
void CGame::SetIceFlag(short sOwnerH, char cOwnerType, bool bStatus)
{
    switch (cOwnerType)
    {
        case DEF_OWNERTYPE_PLAYER:
            if (m_pClientList[sOwnerH] == 0) return;
            if (bStatus == true)
                m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00000040;
            else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFFFFFBF;
            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
            break;

        case DEF_OWNERTYPE_NPC:
            if (m_pNpcList[sOwnerH] == 0) return;
            if (bStatus == true)
                m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x00000040;
            else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFFFFFFBF;
            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
            break;
    }
}

/*********************************************************************************************************************
**  void void CGame::SetPoisonFlag(short sOwnerH, char cOwnerType, bool bStatus)									**
**  description			:: changes the status of the player to show poison aura										**
**  last updated		:: November 20, 2004; 9:36 PM; Hypnotoad													**
**	return value		:: void																						**
*********************************************************************************************************************/
void CGame::SetPoisonFlag(short sOwnerH, char cOwnerType, bool bStatus)
{
    switch (cOwnerType)
    {
        case DEF_OWNERTYPE_PLAYER:
            if (m_pClientList[sOwnerH] == 0) return;
            if (bStatus == true)
                m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00000080;
            else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFFFFF7F;
            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
            break;

        case DEF_OWNERTYPE_NPC:
            if (m_pNpcList[sOwnerH] == 0) return;
            if (bStatus == true)
                m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x00000080;
            else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFFFFFF7F;
            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
            break;
    }
}

/*********************************************************************************************************************
**  void void CGame::SetIllusionFlag(short sOwnerH, char cOwnerType, bool bStatus)									**
**  description			:: changes the status of the player to show illusion aura									**
**  last updated		:: November 20, 2004; 9:36 PM; Hypnotoad													**
**	return value		:: void																						**
*********************************************************************************************************************/
void CGame::SetIllusionFlag(short sOwnerH, char cOwnerType, bool bStatus)
{
    switch (cOwnerType)
    {
        case DEF_OWNERTYPE_PLAYER:
            if (m_pClientList[sOwnerH] == 0) return;
            if (bStatus == true)
                m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x01000000;
            else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFEFFFFFF;
            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
            break;

        case DEF_OWNERTYPE_NPC:
            if (m_pNpcList[sOwnerH] == 0) return;
            if (bStatus == true)
                m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x01000000;
            else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFEFFFFFF;
            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
            break;
    }
}

/*********************************************************************************************************************
**  void void CGame::SetHeroFlag(short sOwnerH, char cOwnerType, bool bStatus)										**
**  description			:: changes the status of the player to show hero item aura									**
**  last updated		:: November 20, 2004; 9:37 PM; Hypnotoad													**
**	return value		:: void																						**
*********************************************************************************************************************/
void CGame::SetHeroFlag(short sOwnerH, char cOwnerType, bool bStatus)
{
    switch (cOwnerType)
    {
        case DEF_OWNERTYPE_PLAYER:
            if (m_pClientList[sOwnerH] == 0) return;
            if (bStatus == true)
                m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00020000;
            else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFFDFFFF;
            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
            break;

        case DEF_OWNERTYPE_NPC:
            if (m_pNpcList[sOwnerH] == 0) return;
            if (bStatus == true)
                m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x00020000;
            else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFFFDFFFF;
            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
            break;
    }
}

/*********************************************************************************************************************
**  void void CGame::SetDefenseShieldFlag(short sOwnerH, char cOwnerType, bool bStatus)								**
**  description			:: changes the status of the player to show defense aura									**
**  last updated		:: November 20, 2004; 9:37 PM; Hypnotoad													**
**	return value		:: void																						**
*********************************************************************************************************************/
void CGame::SetDefenseShieldFlag(short sOwnerH, char cOwnerType, bool bStatus)
{
    switch (cOwnerType)
    {
        case DEF_OWNERTYPE_PLAYER:
            if (m_pClientList[sOwnerH] == 0) return;
            if (bStatus == true)
                m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x02000000;
            else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFDFFFFFF;
            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
            break;

        case DEF_OWNERTYPE_NPC:
            if (m_pNpcList[sOwnerH] == 0) return;
            if (bStatus == true)
                m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x02000000;
            else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFDFFFFFF;
            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
            break;
    }
}

/*********************************************************************************************************************
**  void void CGame::SetMagicProtectionFlag(short sOwnerH, char cOwnerType, bool bStatus)							**
**  description			:: changes the status of the player to show magic protect aura								**
**  last updated		:: November 20, 2004; 9:38 PM; Hypnotoad													**
**	return value		:: void																						**
*********************************************************************************************************************/
void CGame::SetMagicProtectionFlag(short sOwnerH, char cOwnerType, bool bStatus)
{
    switch (cOwnerType)
    {
        case DEF_OWNERTYPE_PLAYER:
            if (m_pClientList[sOwnerH] == 0) return;
            if (bStatus == true)
                m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x04000000;
            else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFBFFFFFF;
            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
            break;

        case DEF_OWNERTYPE_NPC:
            if (m_pNpcList[sOwnerH] == 0) return;
            if (bStatus == true)
                m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x04000000;
            else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xFBFFFFFF;
            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
            break;
    }
}

/*********************************************************************************************************************
**  void void CGame::SetProtectionFromArrowFlag(short sOwnerH, char cOwnerType, bool bStatus)						**
**  description			:: changes the status of the player to show arrow protect aura								**
**  last updated		:: November 20, 2004; 9:39 PM; Hypnotoad													**
**	return value		:: void																						**
*********************************************************************************************************************/
void CGame::SetProtectionFromArrowFlag(short sOwnerH, char cOwnerType, bool bStatus)
{
    switch (cOwnerType)
    {
        case DEF_OWNERTYPE_PLAYER:
            if (m_pClientList[sOwnerH] == 0) return;
            if (bStatus == true)
                m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x08000000;
            else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xF7FFFFFF;
            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
            break;

        case DEF_OWNERTYPE_NPC:
            if (m_pNpcList[sOwnerH] == 0) return;
            if (bStatus == true)
                m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus | 0x08000000;
            else m_pNpcList[sOwnerH]->m_iStatus = m_pNpcList[sOwnerH]->m_iStatus & 0xF7FFFFFF;
            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
            break;
    }
}

/*********************************************************************************************************************
**  void void CGame::SetIllusionMovementFlag(short sOwnerH, char cOwnerType, bool bStatus)							**
**  description			:: changes the status of the player to show illusion movement aura							**
**  last updated		:: November 20, 2004; 9:39 PM; Hypnotoad													**
**	return value		:: void																						**
*********************************************************************************************************************/
void CGame::SetIllusionMovementFlag(short sOwnerH, char cOwnerType, bool bStatus)
{
    switch (cOwnerType)
    {
        case DEF_OWNERTYPE_PLAYER:
            if (m_pClientList[sOwnerH] == 0) return;
            if (bStatus == true)
                m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus | 0x00200000;
            else m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFDFFFFF;
            SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
            break;
    }
}

void CGame::SetSlateFlag(int iClientH, short sType, bool bFlag)
{
    if (m_pClientList[iClientH] == nullptr) return;

    if (sType == DEF_NOTIFY_SLATECLEAR)
    {
        m_pClientList[iClientH]->m_iStatus &= 0xFFBFFFFF;
        m_pClientList[iClientH]->m_iStatus &= 0xFF7FFFFF;
        m_pClientList[iClientH]->m_iStatus &= 0xFFFEFFFF;
        return;
    }

    if (bFlag == true)
    {
        if (sType == 1)
        { // Invincible slate
            m_pClientList[iClientH]->m_iStatus |= 0x400000;
        }
        else if (sType == 3)
        { // Mana slate
            m_pClientList[iClientH]->m_iStatus |= 0x800000;
        }
        else if (sType == 4)
        { // Exp slate
            m_pClientList[iClientH]->m_iStatus |= 0x10000;
        }
    }
    else
    {
        if ((m_pClientList[iClientH]->m_iStatus & 0x400000) != 0)
        {
            m_pClientList[iClientH]->m_iStatus &= 0xFFBFFFFF;
        }
        else if ((m_pClientList[iClientH]->m_iStatus & 0x800000) != 0)
        {
            m_pClientList[iClientH]->m_iStatus &= 0xFF7FFFFF;
        }
        else if ((m_pClientList[iClientH]->m_iStatus & 0x10000) != 0)
        {
            m_pClientList[iClientH]->m_iStatus &= 0xFFFEFFFF;
        }
    }

    SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
}
