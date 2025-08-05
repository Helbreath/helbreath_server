//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include "Game.h"
#include "string_utils.h"

extern int32_t timeGetTime();

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

    memset(cTxt, 0, sizeof(cTxt));

    switch (iAction)
    {

        case DEF_CRUSADELOG_ENDCRUSADE:
            if (cName == 0) return false;
            copy_string(cTxt, "\tEnd Crusade\t%s", cName);
            break;

        case DEF_CRUSADELOG_SELECTDUTY:
            if (cName == 0) return false;
            if (m_pClientList[iClientH] == nullptr) return false;
            copy_string(cTxt, "PC(%s)\tSelect Duty\t(%s)\t \t(%s)", m_pClientList[iClientH]->m_cCharName, cName, m_pClientList[iClientH]->m_cGuildName);
            break;

        case DEF_CRUSADELOG_GETEXP:
            if (m_pClientList[iClientH] == nullptr) return false;
            copy_string(cTxt, "PC(%s)\tGet Exp\t(%d)\t \t(%s)", m_pClientList[iClientH]->m_cCharName, iData, m_pClientList[iClientH]->m_cGuildName);
            break;

        case DEF_CRUSADELOG_STARTCRUSADE:
            copy_string(cTxt, "\tStart Crusade");
            break;

        default:
            return false;
    }

    log->info(cTxt);
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
        log->info("(!) Cannot create CrusadeGUID({}) file", dwCrusadeGUID);
    }
    else
    {
        memset(cTemp, 0, sizeof(cTemp));

        memset(cTxt, 0, sizeof(cTxt));
        copy_string(cTxt, "CrusadeGUID = %d\n", dwCrusadeGUID);
        strcat(cTemp, cTxt);

        memset(cTxt, 0, sizeof(cTxt));
        copy_string(cTxt, "winner-side = %d\n", iWinnerSide);
        strcat(cTemp, cTxt);

        cp = (char *)cTemp;
        fwrite(cp, strlen(cp), 1, pFile);

        copy_string(cTxt, "(O) CrusadeGUID(%d) file created", dwCrusadeGUID);
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
                    iNamingValue = m_pMapList[z]->iGetEmptyNamingValue();
                    if (iNamingValue == -1)
                    {
                    }
                    else
                    {
                        copy_string(cName, "XX%d", iNamingValue);
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
                            m_pMapList[z]->SetNamingValueEmpty(iNamingValue);
                        }
                        else
                        {
                            log->info( "(!) Creating Crusade Structure({}) at {}({}, {})", cNpcName, m_stCrusadeStructures[i].cMapName, tX, tY);
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
        _CreateCrusadeGUID(dwCrusadeGUID, 0);
        m_dwCrusadeGUID = dwCrusadeGUID;
    }

    for (i = 1; i < DEF_MAXCLIENTS; i++)
        if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_bIsInitComplete == true))
        {
            m_pClientList[i]->m_iCrusadeDuty = 0;
            m_pClientList[i]->m_iConstructionPoint = 0;
            m_pClientList[i]->m_dwCrusadeGUID = m_dwCrusadeGUID;
            SendNotifyMsg(0, i, DEF_NOTIFY_CRUSADE, (uint32_t)m_bIsCrusadeMode, m_pClientList[i]->m_iCrusadeDuty, 0, 0);
        }

    for (i = 0; i < DEF_MAXMAPS; i++)
        if (m_pMapList[i] != 0) m_pMapList[i]->RestoreStrikePoints();

    CreateCrusadeStructures();

    log->info("(!)Crusade Mode ON.");
    _bCrusadeLog(DEF_CRUSADELOG_STARTCRUSADE, 0, 0, 0);
}

void CGame::LocalEndCrusadeMode(int iWinnerSide)
{
    int i;

    log->info("LocalEndCrusadeMode({})", iWinnerSide);

    if (m_bIsCrusadeMode == false) return;
    m_bIsCrusadeMode = false;

    log->info("(!)Crusade Mode OFF.");

    RemoveCrusadeStructures();

    RemoveCrusadeNpcs();

    _CreateCrusadeGUID(m_dwCrusadeGUID, iWinnerSide);
    m_iCrusadeWinnerSide = iWinnerSide;
    m_iLastCrusadeWinner = iWinnerSide;

    for (i = 1; i < DEF_MAXCLIENTS; i++)
        if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_bIsInitComplete == true))
        {
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

void CGame::ManualEndCrusadeMode(int iWinnerSide)
{
    char * cp, cData[256];
    uint16_t * wp;

    if (m_bIsCrusadeMode == false) return;

    LocalEndCrusadeMode(iWinnerSide);
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
