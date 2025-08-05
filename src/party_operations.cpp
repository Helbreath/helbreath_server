//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include "Game.h"

void CGame::PartyOperationResult_Create(int iClientH, char * pName, int iResult, int iPartyID)
{
    char * cp, cData[120];
    uint32_t * dwp;
    uint16_t * wp;
    int i;

    if (m_pClientList[iClientH] == nullptr) return;
    if (strcmp(m_pClientList[iClientH]->m_cCharName, pName) != 0) return;

    switch (iResult)
    {
        case 0: // Ã†Ã„Ã†Â¼ Â»Ã½Â¼Âº Â½Ã‡Ã†Ã 
            if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING) return;
            if (strcmp(m_pClientList[iClientH]->m_cCharName, pName) != 0) return;

            m_pClientList[iClientH]->m_iPartyID = 0;
            m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
            m_pClientList[iClientH]->m_iReqJoinPartyClientH = 0;
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_PARTY, 1, 0, 0, 0);
            break;

        case 1: // Ã†Ã„Ã†Â¼ Â»Ã½Â¼Âº Â¼ÂºÂ°Ã¸ 
            if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING) return;
            if (strcmp(m_pClientList[iClientH]->m_cCharName, pName) != 0) return;

            m_pClientList[iClientH]->m_iPartyID = iPartyID;
            m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_CONFIRM;
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_PARTY, 1, 1, 0, 0);

            // Â°Ã”Ã€Ã“ Â¼Â­Â¹Ã¶Ã€Ã‡ Ã†Ã„Ã†Â¼ Â¸Â®Â½ÂºÃ†Â®Â¿Â¡ ÂµÃ®Â·Ã.
            for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
                if (m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] == 0)
                {
                    m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] = iClientH;
                    m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers++;
                    //testcode
                    wsprintf(G_cTxt, "PartyID:%d member:%d New Total:%d", m_pClientList[iClientH]->m_iPartyID, iClientH, m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers);
                    log->info(G_cTxt);
                    goto PORC_LOOPBREAK1;
                }
            PORC_LOOPBREAK1:;

            // Â¸Â¸Â¾Ã  Ã†Ã„Ã†Â¼ Â°Â¡Ã€Ã”Ã€Â» Â½Ã…ÃƒÂ»Ã‡Ã‘ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â°Â¡ Ã€Ã–Â´Ã™Â¸Ã© 
            if ((m_pClientList[iClientH]->m_iReqJoinPartyClientH != 0) && (strlen(m_pClientList[iClientH]->m_cReqJoinPartyName) != 0))
            {
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
                // Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ ÂºÂ¸Â³Ã‚Ã€Â¸Â´Ã Ã…Â¬Â¸Â®Â¾Ã®
                m_pClientList[iClientH]->m_iReqJoinPartyClientH = 0;
                memset(m_pClientList[iClientH]->m_cReqJoinPartyName, 0, sizeof(m_pClientList[iClientH]->m_cReqJoinPartyName));
            }
            break;
    }
}

void CGame::PartyOperationResult_Join(int iClientH, char * pName, int iResult, int iPartyID)
{
    int i;

    if (m_pClientList[iClientH] == nullptr) return;

    switch (iResult)
    {
        case 0:
            if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING) return;
            if (strcmp(m_pClientList[iClientH]->m_cCharName, pName) != 0) return;

            m_pClientList[iClientH]->m_iPartyID = 0;
            m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_PARTY, 4, 0, 0, pName);

            m_pClientList[iClientH]->m_iReqJoinPartyClientH = 0;
            memset(m_pClientList[iClientH]->m_cReqJoinPartyName, 0, sizeof(m_pClientList[iClientH]->m_cReqJoinPartyName));
            break;

        case 1:
            if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING) return;
            if (strcmp(m_pClientList[iClientH]->m_cCharName, pName) != 0) return;

            m_pClientList[iClientH]->m_iPartyID = iPartyID;
            m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_CONFIRM;
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_PARTY, 4, 1, 0, pName);

            m_pClientList[iClientH]->m_iReqJoinPartyClientH = 0;
            memset(m_pClientList[iClientH]->m_cReqJoinPartyName, 0, sizeof(m_pClientList[iClientH]->m_cReqJoinPartyName));

            for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
                if (m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] == 0)
                {
                    m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] = iClientH;
                    m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers++;

                    wsprintf(G_cTxt, "PartyID:%d member:%d In(Join) Total:%d", m_pClientList[iClientH]->m_iPartyID, iClientH, m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers);
                    log->info(G_cTxt);
                    goto PORC_LOOPBREAK1;
                }
            PORC_LOOPBREAK1:;

            for (i = 1; i < DEF_MAXCLIENTS; i++)
                if ((i != iClientH) && (m_pClientList[i] != 0) && (m_pClientList[i]->m_iPartyID != 0) && (m_pClientList[i]->m_iPartyID == iPartyID))
                {
                    SendNotifyMsg(0, i, DEF_NOTIFY_PARTY, 4, 1, 0, pName);
                }
            break;
    }
}

void CGame::PartyOperationResult_Dismiss(int iClientH, char * pName, int iResult, int iPartyID)
{
    int i;
    // iClientHÂ´Ã‚ Ã€ÃŒÂ¹ÃŒ Â»Ã§Â¶Ã³ÃÃ¶Â°Ã­ Â¾Ã¸Â´Ã‚ Ã„Â³Â¸Â¯Ã…ÃÃ€ÃÂ¼Ã¶ÂµÂµ Ã€Ã–Â´Ã™.

    switch (iResult)
    {
        case 0: // Ã†Ã„Ã†Â¼ Ã…Â»Ã…Ã° Â½Ã‡Ã†Ã ? Ã€ÃŒÂ·Â± Ã€ÃÃ€ÃŒ?
            break;

        case 1: // Ã†Ã„Ã†Â¼ Ã…Â»Ã…Ã° Â¼ÂºÂ°Ã¸ 
            if (iClientH == 0)
            {
                // iClientH Â°Â¡ NULLÃ€ÃŒÂ¸Ã© Â¼Â­Â¹Ã¶ Ã€ÃŒÂµÂ¿ÃÃŸ ÃÂ¢Â¼Ã“ ÃÂ¾Â·Ã¡ÂµÃ‡Â¾Ã® Â°Â­ÃÂ¦ ÃÂ¦Â°Ã… Â¿Ã¤ÃƒÂ»ÂµÃˆ Â°ÃÃ€Ã“.
                for (i = 1; i < DEF_MAXCLIENTS; i++)
                    if ((m_pClientList[i] != 0) && (strcmp(m_pClientList[i]->m_cCharName, pName) == 0))
                    {
                        iClientH = i;
                        goto PORD_LOOPBREAK;
                    }
                PORD_LOOPBREAK:;

                // Â°Ã”Ã€Ã“ Â¼Â­Â¹Ã¶Ã€Ã‡ Ã†Ã„Ã†Â¼ Â¸Â®Â½ÂºÃ†Â®Â¿Â¡Â¼Â­ Ã‡Ã˜ÃÂ¦.
                for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
                    if (m_stPartyInfo[iPartyID].iIndex[i] == iClientH)
                    {
                        m_stPartyInfo[iPartyID].iIndex[i] = 0;
                        m_stPartyInfo[iPartyID].iTotalMembers--;
                        //testcode
                        wsprintf(G_cTxt, "PartyID:%d member:%d Out Total:%d", iPartyID, iClientH, m_stPartyInfo[iPartyID].iTotalMembers);
                        log->info(G_cTxt);
                        goto PORC_LOOPBREAK1;
                    }
                PORC_LOOPBREAK1:;
                // Â¸Â®Â½ÂºÃ†Â® Ã€ÃŽÂµÂ¦Â½ÂºÃ€Ã‡ ÂºÃ³Â°Ã¸Â°Â£Ã€Â» ÃÂ¦Â°Ã…Ã‡Ã‘Â´Ã™.
                for (i = 0; i < DEF_MAXPARTYMEMBERS - 1; i++)
                    if ((m_stPartyInfo[iPartyID].iIndex[i] == 0) && (m_stPartyInfo[iPartyID].iIndex[i + 1] != 0))
                    {
                        m_stPartyInfo[iPartyID].iIndex[i] = m_stPartyInfo[iPartyID].iIndex[i + 1];
                        m_stPartyInfo[iPartyID].iIndex[i + 1] = 0;
                    }

                if (m_pClientList[iClientH] != 0)
                {
                    m_pClientList[iClientH]->m_iPartyID = 0;
                    m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
                    m_pClientList[iClientH]->m_iReqJoinPartyClientH = 0;
                }

                // Â¸Ã°ÂµÃ§ Ã†Ã„Ã†Â¼Â¿Ã¸ÂµÃ©Â¿Â¡Â°Ã” Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
                for (i = 1; i < DEF_MAXCLIENTS; i++)
                    if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_iPartyID != 0) && (m_pClientList[i]->m_iPartyID == iPartyID))
                    {
                        SendNotifyMsg(0, i, DEF_NOTIFY_PARTY, 6, 1, 0, pName);
                    }
                return;
            }

            if ((m_pClientList[iClientH] != 0) && (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING)) return;
            if ((m_pClientList[iClientH] != 0) && (strcmp(m_pClientList[iClientH]->m_cCharName, pName) != 0)) return;

            // Â¸Ã°ÂµÃ§ Ã†Ã„Ã†Â¼Â¿Ã¸ÂµÃ©Â¿Â¡Â°Ã” Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
            for (i = 1; i < DEF_MAXCLIENTS; i++)
                if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_iPartyID != 0) && (m_pClientList[i]->m_iPartyID == iPartyID))
                {
                    SendNotifyMsg(0, i, DEF_NOTIFY_PARTY, 6, 1, 0, pName);
                }

            // Â°Ã”Ã€Ã“ Â¼Â­Â¹Ã¶Ã€Ã‡ Ã†Ã„Ã†Â¼ Â¸Â®Â½ÂºÃ†Â®Â¿Â¡Â¼Â­ Ã‡Ã˜ÃÂ¦.
            for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
                if (m_stPartyInfo[iPartyID].iIndex[i] == iClientH)
                {
                    m_stPartyInfo[iPartyID].iIndex[i] = 0;
                    m_stPartyInfo[iPartyID].iTotalMembers--;
                    //testcode
                    wsprintf(G_cTxt, "PartyID:%d member:%d Out Total:%d", iPartyID, iClientH, m_stPartyInfo[iPartyID].iTotalMembers);
                    log->info(G_cTxt);
                    goto PORC_LOOPBREAK2;
                }
            PORC_LOOPBREAK2:;
            // Â¸Â®Â½ÂºÃ†Â® Ã€ÃŽÂµÂ¦Â½ÂºÃ€Ã‡ ÂºÃ³Â°Ã¸Â°Â£Ã€Â» ÃÂ¦Â°Ã…Ã‡Ã‘Â´Ã™.
            for (i = 0; i < DEF_MAXPARTYMEMBERS - 1; i++)
                if ((m_stPartyInfo[iPartyID].iIndex[i] == 0) && (m_stPartyInfo[iPartyID].iIndex[i + 1] != 0))
                {
                    m_stPartyInfo[iPartyID].iIndex[i] = m_stPartyInfo[iPartyID].iIndex[i + 1];
                    m_stPartyInfo[iPartyID].iIndex[i + 1] = 0;
                }

            if (m_pClientList[iClientH] != 0)
            {
                m_pClientList[iClientH]->m_iPartyID = 0;
                m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
                m_pClientList[iClientH]->m_iReqJoinPartyClientH = 0;
            }
            break;
    }
}

void CGame::PartyOperationResult_Delete(int iPartyID)
{
    int i;

    for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
    {
        m_stPartyInfo[iPartyID].iIndex[i] = 0;
        m_stPartyInfo[iPartyID].iTotalMembers = 0;
    }

    // Ã†Ã„Ã†Â¼ Ã‡Ã˜Â»ÃªÂµÃŠ 
    for (i = 1; i < DEF_MAXCLIENTS; i++)
        if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_iPartyID == iPartyID))
        {
            SendNotifyMsg(0, i, DEF_NOTIFY_PARTY, 2, 0, 0, 0);
            m_pClientList[i]->m_iPartyID = 0;
            m_pClientList[i]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
            m_pClientList[i]->m_iReqJoinPartyClientH = 0;
            //testcode
            wsprintf(G_cTxt, "Notify delete party: %d", i);
            log->info(G_cTxt);
        }
}

void CGame::PartyOperationResult_Info(int iClientH, char * pName, int iTotal, char * pNameList)
{
    if (m_pClientList[iClientH] == nullptr) return;
    if (strcmp(m_pClientList[iClientH]->m_cCharName, pName) != 0) return;
    if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_CONFIRM) return;

    SendNotifyMsg(0, iClientH, DEF_NOTIFY_PARTY, 5, 1, iTotal, pNameList);
}
