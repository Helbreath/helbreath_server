//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include "Game.h"


void CGame::RemoveCrusadeNpcs(void)
{
    for (int i = 0; i < DEF_MAXNPCS; i++)
    {
        if (m_pNpcList[i] != 0)
        {
            if ((m_pNpcList[i]->m_sType >= 43 && m_pNpcList[i]->m_sType <= 47) || m_pNpcList[i]->m_sType == 51)
            {
                NpcKilledHandler(0, 0, i, 0);
            }
        }
    }
}

void CGame::RemoveCrusadeRecallTime(void)
{
    for (int i = 1; i < DEF_MAXCLIENTS; i++)
    {
        if (m_pClientList[i] != 0)
        {
            if (m_pClientList[i]->m_bIsWarLocation == true &&
                m_pClientList[i]->m_bIsPlayerCivil == true &&
                m_pClientList[i]->m_bIsInitComplete == true)
            {
                m_pClientList[i]->m_iTimeLeft_ForceRecall = 0;
            }
        }
    }
}

bool CGame::_bCrusadeLog(int iAction, int iClientH, int iData, char * cName)
{
    char  cTxt[200];

    //  Â·ÃŽÂ±Ã— Â³Â²Â±Ã¤Â´Ã™. 
    memset(cTxt, 0, sizeof(cTxt));

    switch (iAction)
    {

        case DEF_CRUSADELOG_ENDCRUSADE:
            if (cName == 0) return false;
            wsprintf(cTxt, "\tEnd Crusade\t%s", cName);
            break;

        case DEF_CRUSADELOG_SELECTDUTY:
            if (cName == 0) return false;
            if (m_pClientList[iClientH] == nullptr) return false;
            wsprintf(cTxt, "PC(%s)\tSelect Duty\t(%s)\t \t(%s)", m_pClientList[iClientH]->m_cCharName, cName, m_pClientList[iClientH]->m_cGuildName);
            break;

        case DEF_CRUSADELOG_GETEXP:
            if (m_pClientList[iClientH] == nullptr) return false;
            wsprintf(cTxt, "PC(%s)\tGet Exp\t(%d)\t \t(%s)", m_pClientList[iClientH]->m_cCharName, iData, m_pClientList[iClientH]->m_cGuildName);
            break;

        case DEF_CRUSADELOG_STARTCRUSADE:
            wsprintf(cTxt, "\tStart Crusade");
            break;

        default:
            return false;
    }

    bSendMsgToLS(MSGID_GAMECRUSADELOG, iClientH, 0, cTxt);
    return true;
}

void CGame::_CreateCrusadeGUID(uint32_t dwCrusadeGUID, int iWinnerSide)
{
    char * cp, cTxt[256], cFn[256], cTemp[1024];
    FILE * pFile;

    _mkdir("GameData");
    memset(cFn, 0, sizeof(cFn));

    strcat(cFn, "GameData");
    strcat(cFn, "\\");
    strcat(cFn, "\\");
    strcat(cFn, "CrusadeGUID.Txt");

    pFile = fopen(cFn, "wt");
    if (pFile == 0)
    {
        // Ã†Ã„Ã€ÃÃ€Â» Â¸Â¸ÂµÃ© Â¼Ã¶ Â¾Ã¸Â°Ã…Â³Âª Â»Ã§Ã€ÃŒÃÃ®Â°Â¡ ÃÃ¶Â³ÂªÃ„Â¡Â°Ã” Ã€Ã›Ã€Âº Â°Ã¦Â¿Ã¬Â´Ã‚ . 
        wsprintf(cTxt, "(!) Cannot create CrusadeGUID(%d) file", dwCrusadeGUID);
        log->info(cTxt);
    }
    else
    {
        memset(cTemp, 0, sizeof(cTemp));

        memset(cTxt, 0, sizeof(cTxt));
        wsprintf(cTxt, "CrusadeGUID = %d\n", dwCrusadeGUID);
        strcat(cTemp, cTxt);

        memset(cTxt, 0, sizeof(cTxt));
        wsprintf(cTxt, "winner-side = %d\n", iWinnerSide);
        strcat(cTemp, cTxt);

        cp = (char *)cTemp;
        fwrite(cp, strlen(cp), 1, pFile);

        wsprintf(cTxt, "(O) CrusadeGUID(%d) file created", dwCrusadeGUID);
        log->info(cTxt);
    }
    if (pFile != 0) fclose(pFile);
}

void CGame::CheckCrusadeResultCalculation(int iClientH)
{
    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_cVar == 1) return;

    if ((m_bIsCrusadeMode == false) && (m_pClientList[iClientH]->m_dwCrusadeGUID != 0))
    {
        if (m_pClientList[iClientH]->m_iWarContribution > DEF_MAXWARCONTRIBUTION) m_pClientList[iClientH]->m_iWarContribution = DEF_MAXWARCONTRIBUTION;
        if (m_pClientList[iClientH]->m_dwCrusadeGUID == m_dwCrusadeGUID)
        {
            if (m_iCrusadeWinnerSide == 0)
            {
                m_pClientList[iClientH]->m_iExpStock += (m_pClientList[iClientH]->m_iWarContribution / 6);
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_CRUSADE, (uint32_t)m_bIsCrusadeMode, 0, m_pClientList[iClientH]->m_iWarContribution, 0);
                _bCrusadeLog(DEF_CRUSADELOG_GETEXP, iClientH, (m_pClientList[iClientH]->m_iWarContribution / 6), 0);
            }
            else
            {
                if (m_iCrusadeWinnerSide == m_pClientList[iClientH]->m_cSide)
                {
                    if (m_pClientList[iClientH]->m_iLevel <= 80)
                    {
                        m_pClientList[iClientH]->m_iWarContribution += m_pClientList[iClientH]->m_iLevel * 100;
                    }
                    else if (m_pClientList[iClientH]->m_iLevel <= 100)
                    {
                        m_pClientList[iClientH]->m_iWarContribution += m_pClientList[iClientH]->m_iLevel * 40;
                    }
                    else m_pClientList[iClientH]->m_iWarContribution += m_pClientList[iClientH]->m_iLevel;
                    m_pClientList[iClientH]->m_iExpStock += m_pClientList[iClientH]->m_iWarContribution;
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_CRUSADE, (uint32_t)m_bIsCrusadeMode, 0, m_pClientList[iClientH]->m_iWarContribution, 0);
                    _bCrusadeLog(DEF_CRUSADELOG_GETEXP, iClientH, m_pClientList[iClientH]->m_iWarContribution, 0);
                }
                else if (m_iCrusadeWinnerSide != m_pClientList[iClientH]->m_cSide)
                {
                    if (m_pClientList[iClientH]->m_iLevel <= 80)
                    {
                        m_pClientList[iClientH]->m_iWarContribution += m_pClientList[iClientH]->m_iLevel * 100;
                    }
                    else if (m_pClientList[iClientH]->m_iLevel <= 100)
                    {
                        m_pClientList[iClientH]->m_iWarContribution += m_pClientList[iClientH]->m_iLevel * 40;
                    }
                    else m_pClientList[iClientH]->m_iWarContribution += m_pClientList[iClientH]->m_iLevel;
                    m_pClientList[iClientH]->m_iExpStock += m_pClientList[iClientH]->m_iWarContribution / 10;
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_CRUSADE, (uint32_t)m_bIsCrusadeMode, 0, -1 * m_pClientList[iClientH]->m_iWarContribution, 0);
                    _bCrusadeLog(DEF_CRUSADELOG_GETEXP, iClientH, (m_pClientList[iClientH]->m_iWarContribution / 10), 0);
                }
            }
        }
        else
        {
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_CRUSADE, (uint32_t)m_bIsCrusadeMode, 0, 0, 0, -1);
        }
        m_pClientList[iClientH]->m_iCrusadeDuty = 0;
        m_pClientList[iClientH]->m_iWarContribution = 0;
        m_pClientList[iClientH]->m_dwCrusadeGUID = 0;
        m_pClientList[iClientH]->m_dwSpeedHackCheckTime = timeGetTime();
        m_pClientList[iClientH]->m_iSpeedHackCheckExp = m_pClientList[iClientH]->m_iExp;
    }
}

void CGame::CreateCrusadeStructures()
{
    int i, z, tX, tY, iNamingValue;
    char cName[6], cNpcName[21], cNpcWayPoint[11];

    memset(cName, 0, sizeof(cName));
    memset(cNpcName, 0, sizeof(cNpcName));
    memset(cNpcWayPoint, 0, sizeof(cNpcWayPoint));

    for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++)
        if (m_stCrusadeStructures[i].cType != 0)
        {
            for (z = 0; z < DEF_MAXMAPS; z++)
                if ((m_pMapList[z] != 0) && (strcmp(m_pMapList[z]->m_cName, m_stCrusadeStructures[i].cMapName) == 0))
                {
                    // Ã€Â§Ã„Â¡Â°Â¡ Ã€ÃÃ„Â¡Ã‡ÃÂ´Ã‚ Â¸ÃŠÂ¿Â¡ Â°Ã‡ÃƒÃ Â¹Â°Ã€Â» Ã€Â§Ã„Â¡Â½ÃƒÃ…Â²Â´Ã™.
                    iNamingValue = m_pMapList[z]->iGetEmptyNamingValue();
                    if (iNamingValue == -1)
                    {
                        // Â´ÃµÃ€ÃŒÂ»Ã³ Ã€ÃŒ Â¸ÃŠÂ¿Â¡ NPCÂ¸Â¦ Â¸Â¸ÂµÃ©Â¼Ã¶ Â¾Ã¸Â´Ã™. Ã€ÃŒÂ¸Â§Ã€Â» Ã‡Ã’Â´Ã§Ã‡Ã’ Â¼Ã¶ Â¾Ã¸Â±Ã¢ Â¶Â§Â¹Â®.
                        // Ã€ÃŒÂ·Â± Ã€ÃÃ€ÃŒ?
                    }
                    else
                    {
                        // NPCÂ¸Â¦ Â»Ã½Â¼ÂºÃ‡Ã‘Â´Ã™.
                        wsprintf(cName, "XX%d", iNamingValue);
                        cName[0] = '_';
                        cName[1] = z + 65;

                        switch (m_stCrusadeStructures[i].cType)
                        {
                            case 36:
                                if (strcmp(m_pMapList[z]->m_cName, "aresden") == 0)
                                    strcpy(cNpcName, "AGT-Aresden");
                                else if (strcmp(m_pMapList[z]->m_cName, "elvine") == 0)
                                    strcpy(cNpcName, "AGT-Elvine");
                                break;

                            case 37:
                                if (strcmp(m_pMapList[z]->m_cName, "aresden") == 0)
                                    strcpy(cNpcName, "CGT-Aresden");
                                else if (strcmp(m_pMapList[z]->m_cName, "elvine") == 0)
                                    strcpy(cNpcName, "CGT-Elvine");
                                break;

                            case 40:
                                if (strcmp(m_pMapList[z]->m_cName, "aresden") == 0)
                                    strcpy(cNpcName, "ESG-Aresden");
                                else if (strcmp(m_pMapList[z]->m_cName, "elvine") == 0)
                                    strcpy(cNpcName, "ESG-Elvine");
                                break;

                            case 41:
                                if (strcmp(m_pMapList[z]->m_cName, "aresden") == 0)
                                    strcpy(cNpcName, "GMG-Aresden");
                                else if (strcmp(m_pMapList[z]->m_cName, "elvine") == 0)
                                    strcpy(cNpcName, "GMG-Elvine");
                                break;

                            case 42:
                                strcpy(cNpcName, "ManaStone");
                                break;

                            default:
                                strcpy(cNpcName, m_pNpcConfigList[m_stCrusadeStructures[i].cType]->m_cNpcName);
                                break;
                        }

                        tX = (int)m_stCrusadeStructures[i].dX;
                        tY = (int)m_stCrusadeStructures[i].dY;
                        if (bCreateNewNpc(cNpcName, cName, m_pMapList[z]->m_cName, 0, 0, DEF_MOVETYPE_RANDOM,
                            &tX, &tY, cNpcWayPoint, 0, 0, -1, false) == false)
                        {
                            // Â½Ã‡Ã†ÃÃ‡ÃŸÃ€Â¸Â¹Ã‡Â·ÃŽ Â¿Â¹Â¾Ã ÂµÃˆ NameValueÂ¸Â¦ Ã‡Ã˜ÃÂ¦Â½ÃƒÃ…Â²Â´Ã™.
                            m_pMapList[z]->SetNamingValueEmpty(iNamingValue);
                        }
                        else
                        {
                            wsprintf(G_cTxt, "(!) Creating Crusade Structure(%s) at %s(%d, %d)", cNpcName, m_stCrusadeStructures[i].cMapName, tX, tY);
                            log->info(G_cTxt);
                        }
                    }
                }
        }
}

void CGame::LocalStartCrusadeMode(uint32_t dwCrusadeGUID)
{
    int i;

    if (m_bIsCrusadeMode == true) return;
    m_bIsCrusadeMode = true;
    m_iCrusadeWinnerSide = 0;

    if (dwCrusadeGUID != 0)
    {
        // Ã…Â©Â·Ã§Â¼Â¼Ã€ÃŒÂµÃ¥ GUID Ã†Ã„Ã€ÃÃ€Â» Â¸Â¸ÂµÃ§Â´Ã™.
        _CreateCrusadeGUID(dwCrusadeGUID, 0);
        m_dwCrusadeGUID = dwCrusadeGUID;
    }

    for (i = 1; i < DEF_MAXCLIENTS; i++)
        if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_bIsInitComplete == true))
        {
            // Â¸Ã°ÂµÃ§ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¿Â¡Â°Ã” Ã€Ã¼Â¸Ã©Ã€Ã¼ Â¸Ã°ÂµÃ¥Â°Â¡ Â½ÃƒÃ€Ã›ÂµÃ‡Â¾ÃºÃ€Â½Ã€Â» Â¾Ã‹Â·ÃÃÃ˜Â´Ã™. Â¸ÃƒÃ€Âº Ã€Ã“Â¹Â« Ã…Â¬Â¸Â®Â¾Ã® ÃˆÃ„ Ã…Ã«ÂºÂ¸Ã‡Ã”.
            m_pClientList[i]->m_iCrusadeDuty = 0;
            m_pClientList[i]->m_iConstructionPoint = 0;
            m_pClientList[i]->m_dwCrusadeGUID = m_dwCrusadeGUID;
            SendNotifyMsg(0, i, DEF_NOTIFY_CRUSADE, (uint32_t)m_bIsCrusadeMode, m_pClientList[i]->m_iCrusadeDuty, 0, 0);
        }

    // Â½ÂºÃ†Â®Â¶Ã³Ã€ÃŒÃ…Â© Ã†Ã·Ã€ÃŽÃ†Â® HP ÃƒÃŠÂ±Ã¢ÃˆÂ­.
    for (i = 0; i < DEF_MAXMAPS; i++)
        if (m_pMapList[i] != 0) m_pMapList[i]->RestoreStrikePoints();

    // Â°Â¢Â°Â¢Ã€Ã‡ Â¸ÃŠÂ¿Â¡ ÃƒÃŠÂ±Ã¢ Â°Ã‡Â¹Â° Â¼Â³Ã„Â¡. (Â¸Â¶Â³Âª Â½ÂºÃ…Ã¦, Â¿Â¡Â³ÃŠÃÃ¶ Â½Ã‡ÂµÃ¥ ÃÂ¦Â³Ã—Â·Â¹Ã€ÃŒÃ…Ã, Â±Ã—Â·Â¹Ã€ÃŒÃ†Â® Â¸Ã…ÃÃ· ÃÂ¦Â³Ã—Â·Â¹Ã€ÃŒÃ…Ã, Â°Â¡ÂµÃ¥Ã…Â¸Â¿Ã¶ÂµÃ®ÂµÃ®)
    CreateCrusadeStructures();

    log->info("(!)Crusade Mode ON.");
    _bCrusadeLog(DEF_CRUSADELOG_STARTCRUSADE, 0, 0, 0);
}

void CGame::LocalEndCrusadeMode(int iWinnerSide)
{
    int i;

    //testcode
    wsprintf(G_cTxt, "LocalEndCrusadeMode(%d)", iWinnerSide);
    log->info(G_cTxt);

    if (m_bIsCrusadeMode == false) return;
    m_bIsCrusadeMode = false;

    log->info("(!)Crusade Mode OFF.");

    // Â¼Â³Ã„Â¡ÂµÃ‡Â¾ÃºÂ´Ã¸ Ã…Â©Â·Ã§Â¼Â¼Ã€ÃŒÂµÃ¥ Â°Ã‡ÃƒÃ Â¹Â° ÃÂ¦Â°Ã….
    RemoveCrusadeStructures();

    RemoveCrusadeNpcs();

    // Ã€ÃŒÂ±Ã¤Ã‚ÃŠ Â»Ã§Ã€ÃŒÂµÃ¥Â¸Â¦ Ã€Ã”Â·Ã‚.
    _CreateCrusadeGUID(m_dwCrusadeGUID, iWinnerSide);
    m_iCrusadeWinnerSide = iWinnerSide;
    m_iLastCrusadeWinner = iWinnerSide;

    for (i = 1; i < DEF_MAXCLIENTS; i++)
        if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_bIsInitComplete == true))
        {
            // Â¸Ã°ÂµÃ§ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¿Â¡Â°Ã” Ã€Ã¼Â¸Ã©Ã€Ã¼ Â¸Ã°ÂµÃ¥Â°Â¡ Â³Â¡Â³ÂµÃ€Â½Ã€Â» Â¾Ã‹Â·ÃÃÃ˜Â´Ã™. Â¸ÃƒÃ€Âº Ã€Ã“Â¹Â« Ã…Â¬Â¸Â®Â¾Ã® ÃˆÃ„ Ã…Ã«ÂºÂ¸Ã‡Ã”.
            m_pClientList[i]->m_iCrusadeDuty = 0;
            m_pClientList[i]->m_iConstructionPoint = 0;
            SendNotifyMsg(0, i, DEF_NOTIFY_CRUSADE, (uint32_t)m_bIsCrusadeMode, 0, 0, 0, m_iCrusadeWinnerSide);
        }
    RemoveCrusadeRecallTime();

    if (iWinnerSide == 2)
    {
        _bCrusadeLog(DEF_CRUSADELOG_ENDCRUSADE, false, false, "Elvine Win!");
    }
    else if (iWinnerSide == 1)
    {
        _bCrusadeLog(DEF_CRUSADELOG_ENDCRUSADE, false, false, "Aresden Win!");
    }
    else
    {
        _bCrusadeLog(DEF_CRUSADELOG_ENDCRUSADE, false, false, "Drawn!");
    }

    if (m_iMiddlelandMapIndex != -1)
    {
        //bSendMsgToLS(0x3D00123C, 0, true, 0);
    }
}

void CGame::CrusadeWarStarter()
{
    SYSTEMTIME SysTime;
    int i;

    if (m_bIsCrusadeMode == true) return;
    if (m_bIsCrusadeWarStarter == false) return;

    // °ÔÀÓ ¼­¹ö 1¹ø¿¡¼­¸¸ 
    GetLocalTime(&SysTime);

    for (i = 0; i < DEF_MAXSCHEDULE; i++)
        if ((m_stCrusadeWarSchedule[i].iDay == SysTime.wDayOfWeek) &&
            (m_stCrusadeWarSchedule[i].iHour == SysTime.wHour) &&
            (m_stCrusadeWarSchedule[i].iMinute == SysTime.wMinute))
        {
            log->info("(!) Automated crusade is being initiated!");
            GlobalStartCrusadeMode();
            return;
        }
}

void CGame::GlobalStartCrusadeMode()
{
    char * cp, cData[120];
    uint32_t * dwp, dwCrusadeGUID;
    SYSTEMTIME SysTime;

    // ¿À·ù·Î ÀÎÇØ ÇÏ·ç¿¡ µÎ¹ø Àü¸éÀüÀÌ ¹ß»ýÇÏ´Â °ÍÀ» ¸·´Â ÄÚµå 
    GetLocalTime(&SysTime);
    if (m_iLatestCrusadeDayOfWeek != -1)
    {
        if (m_iLatestCrusadeDayOfWeek == SysTime.wDayOfWeek) return;
    }
    else m_iLatestCrusadeDayOfWeek = SysTime.wDayOfWeek;

    dwCrusadeGUID = timeGetTime();

    memset(cData, 0, sizeof(cData));
    cp = (char *)cData;
    *cp = GSM_BEGINCRUSADE;
    cp++;
    dwp = (uint32_t *)cp;
    *dwp = dwCrusadeGUID;
    cp += 4;

    bStockMsgToGateServer(cData, 5);

    LocalStartCrusadeMode(dwCrusadeGUID);
}

void CGame::ManualEndCrusadeMode(int iWinnerSide)
{
    char * cp, cData[256];
    uint16_t * wp;

    // Â¸Ã…Â´ÂºÂ¾Ã³Â·ÃŽ Ã…Â©Â·Ã§Â¼Â¼Ã€ÃŒÂµÃ¥ Â¸Ã°ÂµÃ¥Â¸Â¦ ÃÂ¾Â·Ã¡Â½ÃƒÃ…Â²Â´Ã™. ÂºÃ±Â±Ã¤ Â»Ã³Ã…Ã‚Â·ÃŽ ÃÂ¾Â·Ã¡Â½ÃƒÃ…Â²Â´Ã™.
    if (m_bIsCrusadeMode == false) return;

    LocalEndCrusadeMode(iWinnerSide);

    // Â´Ã™Â¸Â¥ Â¼Â­Â¹Ã¶Â¿Â¡ Ã…Â©Â·Ã§Â¼Â¼Ã€ÃŒÂµÃ¥ ÃÂ¾Â·Ã¡Â¸Â¦ Â¾Ã‹Â¸Â².
    memset(cData, 0, sizeof(cData));
    cp = (char *)(cData);
    *cp = GSM_ENDCRUSADE;
    cp++;

    *cp = (char)iWinnerSide;
    cp++;

    wp = (uint16_t *)cp;
    *wp = 0;
    cp += 2;

    wp = (uint16_t *)cp;
    *wp = 0;
    cp += 2;

    wp = (uint16_t *)cp;
    *wp = 0;
    cp += 2;

    cp += 10;

    bStockMsgToGateServer(cData, 18);
}

void CGame::RemoveCrusadeStructures()
{
    int i;

    for (i = 0; i < DEF_MAXNPCS; i++)
        if (m_pNpcList[i] != 0)
        {
            switch (m_pNpcList[i]->m_sType)
            {
                case 36:
                case 37:
                case 38:
                case 39:
                case 40:
                case 41:
                case 42:
                    DeleteNpc(i);
                    break;
            }
        }
}
