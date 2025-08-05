//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include "Game.h"
#include "string_utils.h"

extern int32_t timeGetTime();

void CGame::_CreateHeldenianGUID(uint32_t dwHeldenianGUID, int iWinnerSide)
{
    char * cp, cTxt[256]{}, cFn[256], cTemp[1024];
    FILE * pFile;

    _mkdir("GameData");
    memset(cFn, 0, sizeof(cFn));

    strcat(cFn, "GameData");
    strcat(cFn, "\\");
    strcat(cFn, "\\");
    strcat(cFn, "HeldenianGUID.Txt");

    pFile = fopen(cFn, "wt");
    if (pFile == 0)
    {
        log->info("(!) Cannot create HeldenianGUID({}) file", dwHeldenianGUID);
    }
    else
    {
        memset(cTemp, 0, sizeof(cTemp));

        memset(cTxt, 0, sizeof(cTxt));
        copy_string(cTxt, "HeldenianGUID = %d", dwHeldenianGUID);
        strcat(cTemp, cTxt);

        memset(cTxt, 0, sizeof(cTxt));
        copy_string(cTxt, "winner-side = %d\n", iWinnerSide);
        strcat(cTemp, cTxt);

        cp = (char *)cTemp;
        fwrite(cp, strlen(cp), 1, pFile);

        log->info("(O) HeldenianGUID({}) file created", dwHeldenianGUID);
    }
    if (pFile != 0) fclose(pFile);
}

/*********************************************************************************************************************
**  bool CGame::bCheckHeldenianMap(int sAttackerH, int iMapIndex, char cType)										**
**  description			:: checks if the player is in the specified map												**
**  last updated		:: November 22, 2004; 6:19 PM; Hypnotoad													**
**	return value		:: bool																						**
**  commentary			::	-	translated from scratch using IDA Pro												**
**						::	-	changed pTile->m_cOwner to m_iOccupyStatus											**
**						::	-	added check to prevent access violation if pTile == 0							**
**						::	-	removed 4 return(s) after "iRet = 1;" and placed at end								**
*********************************************************************************************************************/
bool CGame::bCheckHeldenianMap(int sAttackerH, int iMapIndex, char cType)
{
    short tX, tY;
    int iRet;
    class CTile * pTile;

    iRet = 0;
    if (m_pClientList[sAttackerH] == 0) return 0;
    if ((m_bIsHeldenianMode == 1) || (m_cHeldenianType == 1))
    {
        if (cType == DEF_OWNERTYPE_PLAYER)
        {
            if ((m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex] != 0) && (m_pClientList[sAttackerH]->m_cSide > 0))
            {
                tX = m_pClientList[sAttackerH]->m_sX;
                tY = m_pClientList[sAttackerH]->m_sY;
                if ((tX < 0) || (tX >= m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_sSizeX) ||
                    (tY < 0) || (tY >= m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_sSizeY)) return 0;
                pTile = (class CTile *)(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_pTile + tX + tY * m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_sSizeY);
                if (pTile == 0) return 0;
                if (pTile->m_iOccupyStatus != 0)
                {
                    if (pTile->m_iOccupyStatus < 0)
                    {
                        if (m_pClientList[sAttackerH]->m_cSide == 1)
                        {
                            iRet = 1;
                        }
                    }
                    else if (pTile->m_iOccupyStatus > 0)
                    {
                        if (m_pClientList[sAttackerH]->m_cSide == 2)
                        {
                            iRet = 1;
                        }
                    }
                }
            }
        }
        else if (cType == DEF_OWNERTYPE_NPC)
        {
            if ((m_pMapList[m_pNpcList[sAttackerH]->m_cMapIndex] != 0) && (iMapIndex != -1) && (m_pNpcList[sAttackerH]->m_cSide > 0))
            {
                tX = m_pNpcList[sAttackerH]->m_sX;
                tY = m_pNpcList[sAttackerH]->m_sY;
                pTile = (class CTile *)(m_pMapList[m_pNpcList[sAttackerH]->m_cMapIndex]->m_pTile + tX + tY * m_pMapList[m_pNpcList[sAttackerH]->m_cMapIndex]->m_sSizeY);
                if (pTile == 0) return 0;
                if (pTile->m_iOccupyStatus != 0)
                {
                    if (pTile->m_iOccupyStatus < 0)
                    {
                        if (m_pNpcList[sAttackerH]->m_cSide == 1)
                        {
                            iRet = 1;
                        }
                    }
                    else if (pTile->m_iOccupyStatus > 0)
                    {
                        if (m_pNpcList[sAttackerH]->m_cSide == 2)
                        {
                            iRet = 1;
                        }
                    }
                }
            }
        }
    }
    return iRet;
}

void CGame::RequestHeldenianTeleport(int iClientH, char * pData, uint32_t dwMsgSize)
{
    char cTmpName[21], * cp, cTxt[512], cMapName[11];
    short tX, tY, cLoc, * sp;
    uint16_t wResult;
    int iRet, iWhyReturn, iProcessed;

    iProcessed = 1;
    if (m_pClientList[iClientH] == nullptr) return;

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);
    memset(cTmpName, 0, sizeof(cTmpName));
    strcpy(cTmpName, cp);
    if (strcmp(cTmpName, "Gail") == 0)
    {
        memset(cTxt, 0, sizeof(cTxt));
        if ((m_bIsHeldenianMode == 1) && (m_pClientList[iClientH]->m_bIsPlayerCivil != true) && (m_pClientList[iClientH]->m_cSide == 2 || m_pClientList[iClientH]->m_cSide == 1))
        {
            if (m_cHeldenianType == 1)
            {
                memset(cMapName, 0, sizeof(cMapName));
                memcpy(cMapName, "BtField", 10);
                if (m_pClientList[iClientH]->m_cSide == 1)
                {
                    tX = 68;
                    tY = 225;
                    cLoc = 1;
                }
                else if (m_pClientList[iClientH]->m_cSide == 2)
                {
                    tX = 202;
                    tY = 70;
                    cLoc = 2;
                }
            }
            else if (m_cHeldenianType == 2)
            {
                memset(cMapName, 0, sizeof(cMapName));
                memcpy(cMapName, "HRampart", 10);
                if (m_pClientList[iClientH]->m_cSide == m_sLastHeldenianWinner)
                {
                    tX = 81;
                    tY = 42;
                    cLoc = 3;
                }
                else
                {
                    tX = 156;
                    tY = 153;
                    cLoc = 4;
                }
            }
            wResult = DEF_MSGTYPE_CONFIRM;
            iProcessed = 1;
            iWhyReturn = 0;
        }
    }

    wResult = DEF_MSGTYPE_REJECT;
    iProcessed = 0;
    iWhyReturn = 0;

    sp = (short *)cp;
    *sp = 4;
    cp += 4;

    sp = (short *)cp;
    *sp = cLoc;
    cp += 4;

    memcpy(cp, cMapName, 10);
    cp += 10;

    sp = (short *)cp;
    *sp = tX;
    cp += 4;

    sp = (short *)cp;
    *sp = tY;
    cp += 4;

    sp = (short *)cp;
    *sp = iWhyReturn;
    cp += 4;

    iRet = m_pClientList[iClientH]->send_msg(cTxt, 36);
}

void CGame::HeldenianWarStarter()
{
    SYSTEMTIME SysTime;
    int i;

    GetLocalTime(&SysTime);
    if (var_89C == true) return;
    if (var_8A0 == false) return;

    for (i = 0; i < DEF_MAXSCHEDULE; i++)
        if (m_bIsApocalypseMode == true) return;
    if (m_bIsCrusadeMode == true) return;
    if ((m_stHeldenianSchedule[i].iDay != SysTime.wDayOfWeek) &&
        (m_stHeldenianSchedule[i].StartiHour != SysTime.wHour) &&
        (m_stHeldenianSchedule[i].StartiMinute != SysTime.wMinute))
    {
        log->info("Heldenian Start : time({} {}:{}), index({})", SysTime.wDayOfWeek, SysTime.wHour, SysTime.wMinute, i);
        var_8A0 = true;
    }
}

void CGame::HeldenianWarEnder()
{
    SYSTEMTIME SysTime;
    int i;

    GetLocalTime(&SysTime);

    for (i = 0; i < DEF_MAXSCHEDULE; i++)
        if (var_89C != true) return;
    if (var_8A0 != true) return;
    if ((m_stHeldenianSchedule[i].iDay != SysTime.wDayOfWeek) &&
        (m_stHeldenianSchedule[i].EndiHour != SysTime.wHour) &&
        (m_stHeldenianSchedule[i].EndiMinute != SysTime.wMinute))
    {
        log->info( "Heldenian End : time({} {}:{}), index({})", SysTime.wDayOfWeek, SysTime.wHour, SysTime.wMinute, i);
        if (m_cHeldenianModeType == 2)
        {
            m_cHeldenianVictoryType = m_sLastHeldenianWinner;
        }
    }
}

/*********************************************************************************************************************
**  void CGame::LocalStartHeldenianMode(short sV1, short sV2, uint32_t dwHeldenianGUID)								**
**  description			:: localy performs all required tasks upon recieving heldenian initiate message				**
**  last updated		:: December 10, 2004; 12:02 PM; Hypnotoad													**
**	return value		:: void																						**
**  commentary			::	-	translated from scratch using official 3.51 chinese hgserver						**
*********************************************************************************************************************/
void CGame::LocalStartHeldenianMode(short sV1, short sV2, uint32_t dwHeldenianGUID)
{
    int i, x, z, iNamingValue;
    char cName[11], cTmp[21], cNpcWaypointIndex[10], cSide, cOwnerType;
    short sOwnerH;
    bool bRet;
    int dX, dY;

    if (m_bIsHeldenianMode == true) return;

    if ((m_cHeldenianModeType == -1) || (m_cHeldenianModeType != sV1)) m_cHeldenianModeType = sV1;
    if ((m_sLastHeldenianWinner != -1) && (m_sLastHeldenianWinner == sV2))
    {
        log->info("Heldenian Mode : {}, Heldenian Last Winner : {}", m_cHeldenianModeType, m_sLastHeldenianWinner);
    }

    if (dwHeldenianGUID != 0)
    {
        _CreateHeldenianGUID(dwHeldenianGUID, 0);
        m_dwHeldenianGUID = dwHeldenianGUID;
    }
    m_iHeldenianAresdenLeftTower = 0;
    m_iHeldenianElvineLeftTower = 0;
    m_iHeldenianAresdenDead = 0;
    m_iHeldenianElvineDead = 0;

    for (i = 0; i < DEF_MAXCLIENTS; i++)
    {
        if (m_pClientList[i] != 0)
        {
            if (m_pClientList[i]->m_bIsInitComplete != true) break;
            m_pClientList[i]->m_cVar = 2;
            SendNotifyMsg(0, i, DEF_NOTIFY_HELDENIANTELEPORT, 0, 0, 0, 0);
            m_pClientList[i]->m_iWarContribution = 0;
            m_pClientList[i]->m_iConstructionPoint = (m_pClientList[i]->m_iCharisma * 300);
            if (m_pClientList[i]->m_iConstructionPoint > 12000) m_pClientList[i]->m_iConstructionPoint = 12000;
            SendNotifyMsg(0, i, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[i]->m_iConstructionPoint, m_pClientList[i]->m_iWarContribution, 1, 0);
        }
    }

    for (x = 0; x < DEF_MAXMAPS; x++)
    {
        if (m_pMapList[x] == 0) break;
        if (m_pMapList[x]->m_bIsHeldenianMap == true)
        {
            for (i = 0; i < DEF_MAXCLIENTS; i++)
            {
                if (m_pClientList[i] == 0) break;
                if (m_pClientList[i]->m_bIsInitComplete != true) break;
                if (m_pClientList[i]->m_iAdminUserLevel >= 1) break;
                if (m_pClientList[i]->m_cMapIndex != x) break;
                SendNotifyMsg(0, i, DEF_NOTIFY_0BE8, 0, 0, 0, 0);
                RequestTeleportHandler(i, "1   ", 0, -1, -1);
            }
            for (i = 0; i < DEF_MAXNPCS; i++)
            {
                if (m_pNpcList[i] == 0) break;
                if (m_pNpcList[i]->m_bIsKilled != false) break;
                if (m_pNpcList[i]->m_cMapIndex != x) break;
                m_pNpcList[i]->m_bIsSummoned = true;
                RemoveHeldenianNpc(i);
            }

            if (m_cHeldenianModeType == 1)
            {
                if (strcmp(m_pMapList[x]->m_cName, "BtField") == 0)
                {
                    for (i = 0; i < MAX_HELDENIANTOWER; i++)
                    {
                        iNamingValue = m_pMapList[x]->iGetEmptyNamingValue();
                        if (m_pMapList[x]->m_stHeldenianTower[i].sTypeID < 1)  break;
                        if (m_pMapList[x]->m_stHeldenianTower[i].sTypeID > DEF_MAXNPCTYPES) break;
                        if (iNamingValue != -1)
                        {
                            dX = m_pMapList[x]->m_stHeldenianTower[i].dX;
                            dY = m_pMapList[x]->m_stHeldenianTower[i].dY;
                            cSide = m_pMapList[x]->m_stHeldenianTower[i].cSide;
                            for (z = 0; z < DEF_MAXNPCTYPES; z++)
                            {
                                if (m_pNpcConfigList[z] == 0) break;
                                if (m_pNpcConfigList[z]->m_sType == m_pMapList[x]->m_stHeldenianTower[i].sTypeID)
                                {
                                    memset(cTmp, 0, sizeof(cTmp));
                                    strcpy(cTmp, m_pNpcConfigList[z]->m_cNpcName);
                                }
                            }
                            memset(cName, 0, sizeof(cName));
                            copy_string(cName, "XX%d", iNamingValue);
                            cName[0] = 95;
                            cName[1] = i + 65;
                            bRet = bCreateNewNpc(cTmp, cName, m_pMapList[x]->m_cName, (rand() % 3), 0, DEF_MOVETYPE_RANDOM, &dX, &dY, cNpcWaypointIndex, 0, 0, cSide, false, false, false, true, false);
                            if (bRet == false)
                            {
                                m_pMapList[x]->SetNamingValueEmpty(iNamingValue);
                            }
                            else
                            {
                                m_pMapList[x]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
                                if ((m_pNpcList[sOwnerH] != 0) && (sOwnerH > 0) && (sOwnerH < DEF_MAXNPCS))
                                {
                                    m_pNpcList[sOwnerH]->m_iBuildCount = 0;
                                }
                                if (cSide == 1)	m_iHeldenianAresdenLeftTower += 1;
                                if (cSide == 2) m_iHeldenianElvineLeftTower += 1;
                            }
                        }
                    }
                    log->info("HeldenianAresdenLeftTower : {} , HeldenianElvineLeftTower : {}", m_iHeldenianAresdenLeftTower, m_iHeldenianElvineLeftTower);
                    UpdateHeldenianStatus();
                }
            }
            else if (m_cHeldenianModeType == 2)
            {
                if (strcmp(m_pMapList[x]->m_cName, "HRampart") == 0)
                {
                    for (i = 0; i < DEF_MAXHELDENIANDOOR; i++)
                    {
                        iNamingValue = m_pMapList[x]->iGetEmptyNamingValue();
                        if (iNamingValue != -1)
                        {
                            dX = m_pMapList[x]->m_stHeldenianGateDoor[i].dX;
                            dY = m_pMapList[x]->m_stHeldenianGateDoor[i].dY;
                            cSide = m_sLastHeldenianWinner;
                            for (z = 0; z < DEF_MAXNPCTYPES; z++)
                            {
                                if (m_pNpcConfigList[z] == 0) break;
                                if (m_pNpcConfigList[z]->m_sType == 91)
                                {
                                    memset(cTmp, 0, sizeof(cTmp));
                                    strcpy(cTmp, m_pNpcConfigList[z]->m_cNpcName);
                                }
                            }
                            memset(cName, 0, sizeof(cName));
                            copy_string(cName, "XX%d", iNamingValue);
                            cName[0] = 95;
                            cName[1] = i + 65;
                            bRet = bCreateNewNpc(cTmp, cName, m_pMapList[x]->m_cName, (rand() % 3), 0, DEF_MOVETYPE_RANDOM, &dX, &dY, cNpcWaypointIndex, 0, 0, cSide, false, false, false, true, false);
                            if (bRet == false)
                            {
                                m_pMapList[x]->SetNamingValueEmpty(iNamingValue);
                            }
                            else
                            {
                                //m_pMapList[x]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
                                if ((m_pNpcList[bRet] != 0) && (bRet > 0) && (bRet < DEF_MAXNPCS))
                                {
                                    m_pNpcList[bRet]->m_iBuildCount = 0;
                                    m_pNpcList[bRet]->m_cDir = m_pMapList[x]->m_stHeldenianGateDoor[i].cDir;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    m_bHeldenianInitiated = true;
    m_bIsHeldenianMode = true;
    log->info("(!) HELDENIAN Start.");
    m_dwHeldenianStartTime = time(0);
}

void CGame::ManualStartHeldenianMode(int iClientH, char * pData, uint32_t dwMsgSize)
{
    char cHeldenianType, cBuff[256], * token, seps[] = "= \t\n";
    SYSTEMTIME SysTime;
    class CStrTok * pStrTok;
    int iV1;

    if (m_pClientList[iClientH]->m_iAdminUserLevel < 3)
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, 0);

    if (m_bIsHeldenianMode == true) return;
    if (m_bIsApocalypseMode == true) return;
    if (m_bIsCrusadeMode == true) return;
    if ((dwMsgSize != 0) && (pData != 0))
    {
        m_bHeldenianRunning = true;
        GetLocalTime(&SysTime);

        memset(cBuff, 0, sizeof(cBuff));
        memcpy(cBuff, pData, dwMsgSize);
        pStrTok = new class CStrTok(cBuff, seps);
        token = pStrTok->pGet();
        token = pStrTok->pGet();
        if (token != 0)
        {
            iV1 = atoi(token);
            iV1 += (SysTime.wHour * 24 + SysTime.wMinute * 60);
            m_dwHeldenianStartHour = (iV1 / 24);
            m_dwHeldenianStartMinute = (iV1 / 60);
        }
        token = pStrTok->pGet();
        if (token != 0)
        {
            cHeldenianType = atoi(token);
            if ((cHeldenianType == 1) || (cHeldenianType == 2))
            {
                m_cHeldenianModeType = cHeldenianType;
            }
        }
        delete pStrTok;
    }
    GlobalStartHeldenianMode();
    log->info("GM Order({}): begin Heldenian", m_pClientList[iClientH]->m_cCharName);
}

void CGame::ManualEndHeldenianMode(int iClientH, char * pData, uint32_t dwMsgSize)
{
    if (m_pClientList[iClientH]->m_iAdminUserLevel < 3)
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, 0);
    }

    if (m_bIsHeldenianMode == true)
    {
        GlobalEndHeldenianMode();
        m_bHeldenianRunning = false;
        log->info("GM Order({}): end Heldenian", m_pClientList[iClientH]->m_cCharName);
    }
}

/*void CGame::GlobalStartApocalypseMode(int iClientH, char *pData, uint32_t dwMsgSize)
{
 char * cp, cData[120], cBuff[256];
 uint32_t * dwp, dwApocalypseGUID;
 SYSTEMTIME SysTime;

    if (m_pClientList[iClientH]->m_iAdminUserLevel < 3) {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, 0);
    }
    if (m_bIsApocalypseMode == true) return;
    if (m_bIsHeldenianMode == true) return;
    if (m_bIsCrusadeMode == true) return;
    if (dwMsgSize != 0) && (pData != 0) {
        m_bIsApocalypseGateOpen = true;
        GetLocalTime(&SysTime);
        memset(cBuff, 0, sizeof(cBuff));
        memcpy(cBuff, pData, dwMsgSize);

        pStrTok = new class CStrTok(cBuff, seps);
        token = pStrTok->pGet();
        token = pStrTok->pGet();
        if (token != 0) {
            var_124 = atoi(token);
            m_dwApocalypseStartHour = SysTime.wHour;
            m_dwApocalypseStartMinute = SysTime.wMinute;
        }
        else {
            return;
        }
    }
    sub_4AD0E0();
    copy_string(cTemp, "(%s) GM Order(%s): beginapocalypse", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
    bSendMsgToLS(MSGID_GAMEMASTERLOG, iClientH, false, cTemp);
    delete pStrTok;
}*/

void CGame::AutomatedHeldenianTimer()
{
    uint32_t dwTime;
    int x, i;

    dwTime = time(0);
    if ((dwTime == m_dwHeldenianStartTime - 300) && (m_bHeldenianInitiated != true) && (m_bIsHeldenianMode != true))
    {
        log->info("Heldenian Fight Start");
        m_bIsHeldenianMode = false;
        for (x = 0; x < DEF_MAXMAPS; x++)
        {
            if (m_pMapList[x] == 0) return;
            if (m_pMapList[x]->m_bIsHeldenianMap == true) return;
            for (i = 0; i < DEF_MAXCLIENTS; i++)
            {
                if (m_pClientList[i] == 0) return;
                if (m_pClientList[i]->m_bIsInitComplete != true) return;
                if (m_pClientList[i]->m_cMapIndex == x)
                {
                    SendNotifyMsg(0, i, DEF_NOTIFY_HELDENIANSTART, 0, 0, 0, 0);
                }
            }
        }
    }
    if ((dwTime > (180 - var_A38)) && (m_bHeldenianInitiated == true) && (m_bIsHeldenianMode != false))
    {
        log->info("Heldenian Fight End");
        m_bHeldenianInitiated = false;
        for (x = 0; x < DEF_MAXMAPS; x++)
        {
            if (m_pMapList[x] == 0) return;
            if (m_pMapList[x]->m_bIsHeldenianMap == true) return;
            for (i = 0; i < DEF_MAXCLIENTS; i++)
            {
                if (m_pClientList[i] == 0) return;
                if (m_pClientList[i]->m_bIsInitComplete != true) return;
                if (m_pClientList[i]->m_cMapIndex == x)
                {
                    SendNotifyMsg(0, i, DEF_NOTIFY_0BE8, 0, 0, 0, 0);
                    RequestTeleportHandler(i, "1   ", 0, -1, -1);
                }
            }
        }
    }
}

void CGame::SetHeldenianMode()
{
    SYSTEMTIME SysTime;

    GetLocalTime(&SysTime);
    m_dwHeldenianStartHour = SysTime.wHour;
    m_dwHeldenianStartMinute = SysTime.wMinute;

    if (m_cHeldenianModeType != 2)
    {
        m_cHeldenianVictoryType = m_sLastHeldenianWinner;
    }
}

/*void CGame::CheckHeldenianResultCalculation(int iClientH)
{
    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_cVar == 2) return;
    if ((m_cHeldenianModeType != 0) && (m_pClientList[iClientH]->m_dwCrusadeGUID != 0)) {
    m_pClientList[]->m_dwCrusadeGUID


}*/

void CGame::NotifyStartHeldenianMode()
{
    int i, x;

    if (m_bIsHeldenianMode == true) return;
    if (m_bHeldenianInitiated == true) return;
    if (var_88C != true)
    {
        log->info("Heldenian Fight Start");
        m_bHeldenianInitiated = false;
        for (i = 0; i < DEF_MAXMAPS; i++)
        {
            if (m_pMapList[i] == 0) return;
            if (m_pMapList[i]->m_bIsHeldenianMap != true) return;
            for (x = 0; x < DEF_MAXCLIENTS; x++)
            {
                if (m_pClientList[x] == 0) return;
                if (m_pClientList[x]->m_bIsInitComplete != true) return;
                if (m_pClientList[x]->m_cMapIndex == i)
                {
                    SendNotifyMsg(0, x, DEF_NOTIFY_HELDENIANSTART, 0, 0, 0, 0);
                }
            }
        }
    }
}

bool CGame::UpdateHeldenianStatus()
{
    int i;
    bool bFlag;
    int iShortCutIndex, iClientH;

    if (m_bIsHeldenianMode != true) return false;
    for (i = 0; i < DEF_MAXMAPS; i++)
        if (m_pMapList[i] != 0)
        {
            if (m_pMapList[i]->m_bIsHeldenianMap == true)
            {
                bFlag = true;
                iShortCutIndex = 0;
            }
            if (bFlag == true)
            {
                iClientH = m_iClientShortCut[iShortCutIndex];
                iShortCutIndex++;
                if (iClientH == 0)
                {
                    bFlag = 0;
                }
                else
                {
                    if ((m_pClientList[iClientH] != 0) && (m_pClientList[iClientH]->m_bIsInitComplete == true) && (strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "BtField") == 0))
                    {
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_HELDENIANCOUNT, m_iHeldenianAresdenLeftTower, m_iHeldenianElvineLeftTower, m_iHeldenianAresdenDead, 0, m_iHeldenianElvineDead, 0);
                    }
                }
            }
        }
    return true;
}

void CGame::GlobalEndHeldenianMode()
{
    char * cp, cData[32];

    if (m_bIsHeldenianMode == false) return;

    memset(cData, 0, sizeof(cData));
    cp = (char *)cData;
    *cp = GSM_ENDHELDENIAN; // 22
    cp++;

    bStockMsgToGateServer(cData, 1);
    LocalEndHeldenianMode();

}

void CGame::LocalEndHeldenianMode()
{
    int i, x, n;

    if (m_bIsHeldenianMode == false) return;
    m_bIsHeldenianMode = false;
    m_bHeldenianInitiated = true;

    m_dwHeldenianFinishTime = time(0);
    if (var_88C == 1)
    {
        if (m_cHeldenianModeType == 1)
        {
            if (m_iHeldenianAresdenLeftTower > m_iHeldenianElvineLeftTower)
            {
                m_cHeldenianVictoryType = 1;
            }
            else if (m_iHeldenianAresdenLeftTower < m_iHeldenianElvineLeftTower)
            {
                m_cHeldenianVictoryType = 2;
            }
            else if (m_iHeldenianAresdenDead < m_iHeldenianElvineDead)
            {
                m_cHeldenianVictoryType = 1;
            }
            else if (m_iHeldenianAresdenDead > m_iHeldenianElvineDead)
            {
                m_cHeldenianVictoryType = 2;
            }
            else
            {
                m_sLastHeldenianWinner = m_cHeldenianVictoryType;
            }
        }
        else if (m_cHeldenianModeType == 2)
        {
            m_sLastHeldenianWinner = m_cHeldenianVictoryType;
        }
        m_sLastHeldenianWinner = m_cHeldenianVictoryType;
        if (bNotifyHeldenianWinner() == false)
        {
            copy_string(G_cTxt, "(!) HELDENIAN End. Result Report Failed");
            log->info(G_cTxt);
        }
    }
    copy_string(G_cTxt, "(!) HELDENIAN End. %d", m_sLastHeldenianWinner);
    log->info(G_cTxt);

    for (i = 0; i < DEF_MAXMAPS; i++)
        if ((i < 0) || (i > 100)) break;
    if (m_pMapList[i] != 0)
    {
        for (x = 0; x < DEF_MAXCLIENTS; x++)
            if ((m_pClientList[x] != 0) && (m_pClientList[x]->m_bIsInitComplete == true))
            {
                SendNotifyMsg(0, x, DEF_NOTIFY_HELDENIANEND, 0, 0, 0, 0);
                if (m_pMapList[m_pClientList[x]->m_cMapIndex]->m_bIsHeldenianMap == true)
                {
                    for (n = 0; n < DEF_MAXNPCS; n++)
                        if ((m_pNpcList[n] != 0) && (m_pMapList[m_pNpcList[n]->m_cMapIndex] != 0) && (m_pNpcList[n]->m_bIsSummoned == true))
                        {
                            RemoveHeldenianNpc(n);
                        }
                    RemoveOccupyFlags(x);
                }
            }
    }
    _CreateHeldenianGUID(m_dwHeldenianGUID, m_cHeldenianVictoryType);
}

bool CGame::bNotifyHeldenianWinner()
{
    if (var_88C == 0)
    {
        bSendMsgToLS(MSGID_REQUEST_HELDENIAN_WINNER, 0, 1, 0);
        return true;
    }
    else
    {
        return false;
    }

}

void CGame::RemoveHeldenianNpc(int iNpcH)
{
    if (m_pNpcList[iNpcH] == 0) return;
    if (m_pNpcList[iNpcH]->m_bIsKilled == true) return;

    m_pNpcList[iNpcH]->m_bIsKilled = true;
    m_pNpcList[iNpcH]->m_iHP = 0;
    m_pNpcList[iNpcH]->m_iLastDamage = 0;
    m_pNpcList[iNpcH]->m_dwRegenTime = 0;
    m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_iTotalAliveObject--;

    ReleaseFollowMode(iNpcH, DEF_OWNERTYPE_NPC);
    m_pNpcList[iNpcH]->m_iTargetIndex = 0;
    m_pNpcList[iNpcH]->m_cTargetType = 0;

    SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTDYING, 0, 1, 0);
    m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearOwner(10, iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY);
    m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->SetDeadOwner(iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY);
    m_pNpcList[iNpcH]->m_cBehavior = 4;
    m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
    m_pNpcList[iNpcH]->m_dwDeadTime = timeGetTime();

}
