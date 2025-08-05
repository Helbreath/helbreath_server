//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include "Game.h"
#include "string_utils.h"

bool CGame::bReadAdminListConfigFile(char * pFn)
{
    FILE * pFile;
    HANDLE hFile;
    uint32_t  dwFileSize;
    char * cp, * token, cReadModeA, cReadModeB;
    char seps[] = "= \t\n";
    class CStrTok * pStrTok;
    int   iIndex;
    char len;

    cReadModeA = 0;
    cReadModeB = 0;
    iIndex = 0;

    hFile = CreateFile(pFn, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    dwFileSize = GetFileSize(hFile, 0);
    if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

    pFile = fopen(pFn, "rt");
    if (pFile == 0)
    {
        // °ÔÀÓ¼­¹öÀÇ ÃÊ±âÈ­ ÆÄÀÏÀ» ÀÐÀ» ¼ö ¾ø´Ù.
        log->info("(!) Cannot open AdminList.cfg file.");
        return false;
    }
    else
    {
        log->info("(!) Reading AdminList.cfg...");
        cp = new char[dwFileSize + 2];
        memset(cp, 0, dwFileSize + 2);
        fread(cp, dwFileSize, 1, pFile);

        pStrTok = new class CStrTok(cp, seps);
        token = pStrTok->pGet();

        while (token != 0)
        {
            if (cReadModeA != 0)
            {
                switch (cReadModeA)
                {
                    case 1:
                        if (iIndex >= DEF_MAXADMINS)
                        {
                            log->info("(!) WARNING! Too many GMs on the AdminList.cfg!");
                            return true;
                        }
                        len = strlen(token);
                        if (len > 10) len = 10;
                        memset(m_stAdminList[iIndex].m_cGMName, 0, sizeof(m_stAdminList[iIndex].m_cGMName));
                        memcpy(m_stAdminList[iIndex].m_cGMName, token, strlen(token));
                        iIndex++;
                        cReadModeA = 0;
                        cReadModeB = 0;
                        break;
                }
            }
            else
            {
                if (memcmp(token, "verified-admin", 14) == 0)
                {
                    cReadModeA = 1;
                    cReadModeB = 1;
                }
            }

            token = pStrTok->pGet();
        }
        delete pStrTok;
        delete cp;
    }
    if (pFile != 0) fclose(pFile);

    return true;
}

bool CGame::bReadBannedListConfigFile(char * pFn)
{
    FILE * pFile;
    HANDLE hFile;
    uint32_t  dwFileSize;
    char * cp, * token, cReadModeA, cReadModeB;
    char seps[] = "= \t\n";
    class CStrTok * pStrTok;
    int   iIndex;
    char len;

    cReadModeA = 0;
    cReadModeB = 0;
    iIndex = 0;

    hFile = CreateFile(pFn, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    dwFileSize = GetFileSize(hFile, 0);
    if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

    pFile = fopen(pFn, "rt");
    if (pFile == 0)
    {
        // °ÔÀÓ¼­¹öÀÇ ÃÊ±âÈ­ ÆÄÀÏÀ» ÀÐÀ» ¼ö ¾ø´Ù.
        log->info("(!) Cannot open BannedList.cfg file.");
        return false;
    }
    else
    {
        log->info("(!) Reading BannedList.cfg...");
        cp = new char[dwFileSize + 2];
        memset(cp, 0, dwFileSize + 2);
        fread(cp, dwFileSize, 1, pFile);

        pStrTok = new class CStrTok(cp, seps);
        token = pStrTok->pGet();

        while (token != 0)
        {
            if (cReadModeA != 0)
            {
                switch (cReadModeA)
                {
                    case 1:
                        if (iIndex >= DEF_MAXBANNED)
                        {
                            log->info("(!) WARNING! Too many banned on the BannedList.cfg!");
                            return true;
                        }
                        len = strlen(token);
                        if (len > 20) len = 20;
                        memset(m_stBannedList[iIndex].m_cBannedIPaddress, 0, sizeof(m_stBannedList[iIndex].m_cBannedIPaddress));
                        memcpy(m_stBannedList[iIndex].m_cBannedIPaddress, token, strlen(token));
                        iIndex++;
                        cReadModeA = 0;
                        cReadModeB = 0;
                        break;
                }
            }
            else
            {
                if (memcmp(token, "banned-ip", 9) == 0)
                {
                    cReadModeA = 1;
                    cReadModeB = 1;
                }
            }

            token = pStrTok->pGet();
        }
        delete pStrTok;
        delete cp;
    }
    if (pFile != 0) fclose(pFile);

    return true;
}

bool CGame::bReadAdminSetConfigFile(char * cFn)
{
    FILE * pFile;
    HANDLE hFile;
    uint32_t  dwFileSize;
    char * cp, * token, cReadMode, cGSMode[16] = "";
    char seps[] = "= \t\n";
    class CStrTok * pStrTok;

    cReadMode = 0;

    hFile = CreateFile(cFn, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    dwFileSize = GetFileSize(hFile, 0);
    if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

    pFile = fopen(cFn, "rt");
    if (pFile == 0)
    {
        return false;
    }
    else
    {
        log->info("(!) Reading settings file...");
        cp = new char[dwFileSize + 2];
        memset(cp, 0, dwFileSize + 2);
        fread(cp, dwFileSize, 1, pFile);

        pStrTok = new class CStrTok(cp, seps);
        token = pStrTok->pGet();
        //token = strtok( cp, seps );   
        while (token != 0)
        {

            if (cReadMode != 0)
            {
                switch (cReadMode)
                {

                    case 1:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelGMKill = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelGMKill = 3;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------
                    case 2:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelGMRevive = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelGMRevive = 3;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------
                    case 3:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelGMCloseconn = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelGMCloseconn = 3;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------
                    case 4:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelGMCheckRep = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelGMCheckRep = 1;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------
                    case 5:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelWho = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelWho = 1;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------
                    case 6:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelEnergySphere = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelEnergySphere = 2;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------
                    case 7:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelShutdown = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelShutdown = 3;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------
                    case 8:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelObserver = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelObserver = 3;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------
                    case 9:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelShutup = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelShutup = 2;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------
                    case 10:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelCallGaurd = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelCallGaurd = 2;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------
                    case 11:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelSummonDemon = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelSummonDemon = 3;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------		   
                    case 12:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelSummonDeath = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelSummonDeath = 3;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------
                    case 13:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelReserveFightzone = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelReserveFightzone = 2;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------
                    case 14:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelCreateFish = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelCreateFish = 2;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------
                    case 15:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelTeleport = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelTeleport = 2;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------
                    case 16:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelCheckIP = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelCheckIP = 2;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------
                    case 17:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelPolymorph = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelPolymorph = 2;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------
                    case 18:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelSetInvis = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelSetInvis = 2;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------	
                    case 19:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelSetZerk = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelSetZerk = 2;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------	
                    case 20:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelSetIce = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelSetIce = 2;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------	
                    case 21:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelGetNpcStatus = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelGetNpcStatus = 2;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------				
                    case 22:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelSetAttackMode = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelSetAttackMode = 2;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------	
                    case 23:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelUnsummonAll = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelUnsummonAll = 3;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------	
                    case 24:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelUnsummonDemon = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelUnsummonDemon = 3;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------	
                    case 25:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelSummon = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelSummon = 3;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------			
                    case 26:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelSummonAll = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelSummonAll = 4;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------
                    case 27:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelSummonPlayer = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelSummonPlayer = 1;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------	
                    case 28:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelDisconnectAll = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelDisconnectAll = 2;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------	
                    case 29:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelEnableCreateItem = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelEnableCreateItem = 3;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------			
                    case 30:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelCreateItem = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelCreateItem = 4;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------
                    case 31:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelStorm = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelStorm = 3;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------
                    case 32:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelWeather = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelWeather = 2;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------
                    case 33:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelSetStatus = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelSetStatus = 2;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------
                    case 34:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelGoto = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelGoto = 1;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------	
                    case 35:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelMonsterCount = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelMonsterCount = 1;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------			
                    case 36:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelSetRecallTime = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelSetRecallTime = 3;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------	
                    case 37:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelUnsummonBoss = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelUnsummonBoss = 3;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------	
                    case 38:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelClearNpc = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelClearNpc = 3;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------				
                    case 39:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelTime = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelTime = 2;
                        }
                        cReadMode = 0;
                        break;
                        //----------------------------------------------------------------
                    case 40:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelPushPlayer = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelPushPlayer = 2;
                        }
                        cReadMode = 0;
                        break;
                        //-----------------------------------------------------------------
                    case 41:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelSummonGuild = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelSummonGuild = 3;
                        }
                        cReadMode = 0;
                        break;
                        //-----------------------------------------------------------------
                    case 42:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelCheckStatus = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelCheckStatus = 1;
                        }
                        cReadMode = 0;
                        break;
                        //-----------------------------------------------------------------
                    case 43:
                        if ((strlen(token) > 0) && (strlen(token) < 9))
                        {
                            m_iAdminLevelCleanMap = atoi(token);
                        }
                        else
                        {
                            m_iAdminLevelCleanMap = 1;
                        }
                        cReadMode = 0;
                        break;
                        //-----------------------------------------------------------------
                }
            }
            else
            {
                if (memcmp(token, "Admin-Level-/kill", 17) == 0)						cReadMode = 1;
                if (memcmp(token, "Admin-Level-/revive", 19) == 0)						cReadMode = 2;
                if (memcmp(token, "Admin-Level-/closecon", 21) == 0)					cReadMode = 3;
                if (memcmp(token, "Admin-Level-/checkrep", 21) == 0)					cReadMode = 4;
                if (memcmp(token, "Admin-Level-/who", 16) == 0)							cReadMode = 5;
                if (memcmp(token, "Admin-Level-/energysphere", 25) == 0)				cReadMode = 6;
                if (memcmp(token, "Admin-Level-/shutdownthisserverrightnow", 39) == 0)	cReadMode = 7;
                if (memcmp(token, "Admin-Level-/setobservermode", 28) == 0)				cReadMode = 8;
                if (memcmp(token, "Admin-Level-/shutup", 19) == 0)						cReadMode = 9;
                if (memcmp(token, "Admin-Level-/attack", 19) == 0)						cReadMode = 10;
                if (memcmp(token, "Admin-Level-/summondemon", 24) == 0)					cReadMode = 11;
                if (memcmp(token, "Admin-Level-/summondeath", 24) == 0)					cReadMode = 12;
                if (memcmp(token, "Admin-Level-/reservefightzone", 28) == 0)			cReadMode = 13;
                if (memcmp(token, "Admin-Level-/createfish", 23) == 0)					cReadMode = 14;
                if (memcmp(token, "Admin-Level-/teleport", 21) == 0)					cReadMode = 15;
                if (memcmp(token, "Admin-Level-/checkip", 20) == 0)						cReadMode = 16;
                if (memcmp(token, "Admin-Level-/polymorph", 22) == 0)					cReadMode = 17;
                if (memcmp(token, "Admin-Level-/setinvi", 20) == 0)						cReadMode = 18;
                if (memcmp(token, "Admin-Level-/setzerk", 20) == 0)						cReadMode = 19;
                if (memcmp(token, "Admin-Level-/setfreeze", 22) == 0)					cReadMode = 20;
                if (memcmp(token, "Admin-Level-/gns", 16) == 0)							cReadMode = 21;
                if (memcmp(token, "Admin-Level-/setattackmode", 26) == 0)				cReadMode = 22;
                if (memcmp(token, "Admin-Level-/unsummonall", 24) == 0)					cReadMode = 23;
                if (memcmp(token, "Admin-Level-/unsummondemon", 26) == 0)				cReadMode = 24;
                if (memcmp(token, "Admin-Level-/summonnpc", 22) == 0)					cReadMode = 25;
                if (memcmp(token, "Admin-Level-/summonall", 22) == 0)					cReadMode = 26;
                if (memcmp(token, "Admin-Level-/summonplayer", 25) == 0)				cReadMode = 27;
                if (memcmp(token, "Admin-Level-/disconnectall", 26) == 0)				cReadMode = 28;
                if (memcmp(token, "Admin-Level-/enableadmincreateitem", 34) == 0)		cReadMode = 29;
                if (memcmp(token, "Admin-Level-/createitem", 23) == 0)					cReadMode = 30;
                if (memcmp(token, "Admin-Level-/storm", 18) == 0)						cReadMode = 31;
                if (memcmp(token, "Admin-Level-/weather", 20) == 0)						cReadMode = 32;
                if (memcmp(token, "Admin-Level-/setstatus", 22) == 0)					cReadMode = 33;
                if (memcmp(token, "Admin-Level-/goto", 17) == 0)						cReadMode = 34;
                if (memcmp(token, "Admin-Level-/monstercount", 17) == 0)				cReadMode = 35;
                if (memcmp(token, "Admin-Level-/setforcerecalltime", 23) == 0)			cReadMode = 36;
                if (memcmp(token, "Admin-Level-/unsummonboss", 25) == 0)				cReadMode = 37;
                if (memcmp(token, "Admin-Level-/clearnpc", 21) == 0)					cReadMode = 38;
                if (memcmp(token, "Admin-Level-/time", 17) == 0)						cReadMode = 39;
                if (memcmp(token, "Admin-Level-/send", 17) == 0)						cReadMode = 40;
                if (memcmp(token, "Admin-Level-/summonguild", 24) == 0)					cReadMode = 41;
                if (memcmp(token, "Admin-Level-/checkstatus", 24) == 0)					cReadMode = 42;
                if (memcmp(token, "Admin-Level-/clearmap", 21) == 0)					cReadMode = 43;
            }

            token = pStrTok->pGet();
            //token = strtok( 0, seps );
        }
        delete pStrTok;
        delete cp;
    }
    if (pFile != 0) fclose(pFile);

    return true;
}

bool CGame::bReadApocalypseGUIDFile(char * cFn)
{
    FILE * pFile;
    HANDLE hFile;
    uint32_t  dwFileSize;
    char * cp, * token, cReadMode;
    char seps[] = "= \t\n";
    class CStrTok * pStrTok;

    cReadMode = 0;

    hFile = CreateFile(cFn, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    dwFileSize = GetFileSize(hFile, 0);
    if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

    pFile = fopen(cFn, "rt");
    if (pFile == 0)
    {
        // Â°Ã”Ã€Ã“Â¼Â­Â¹Ã¶Ã€Ã‡ ÃƒÃŠÂ±Ã¢ÃˆÂ­ Ã†Ã„Ã€ÃÃ€Â» Ã€ÃÃ€Â» Â¼Ã¶ Â¾Ã¸Â´Ã™.
        log->info("(!) Cannot open ApocalypseGUID file.");
        return false;
    }
    else
    {
        log->info("(!) Reading ApocalypseGUID file...");
        cp = new char[dwFileSize + 2];
        memset(cp, 0, dwFileSize + 2);
        fread(cp, dwFileSize, 1, pFile);

        pStrTok = new class CStrTok(cp, seps);
        token = pStrTok->pGet();

        while (token != 0)
        {

            if (cReadMode != 0)
            {
                switch (cReadMode)
                {
                    case 1:
                        m_dwApocalypseGUID = atoi(token);
                        copy_string(G_cTxt, "ApocalypseGUID = %d", m_dwApocalypseGUID);
                        log->info(G_cTxt);
                        cReadMode = 0;
                        break;
                }
            }
            else
            {
                if (memcmp(token, "ApocalypseGUID", 14) == 0) cReadMode = 1;
            }

            token = pStrTok->pGet();
        }

        delete pStrTok;
        delete cp;
    }
    if (pFile != 0) fclose(pFile);

    return true;
}

bool CGame::bReadHeldenianGUIDFile(char * cFn)
{
    FILE * pFile;
    HANDLE hFile;
    uint32_t  dwFileSize;
    char * cp, * token, cReadMode;
    char seps[] = "= \t\n";
    class CStrTok * pStrTok;

    cReadMode = 0;

    hFile = CreateFile(cFn, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    dwFileSize = GetFileSize(hFile, 0);
    if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

    pFile = fopen(cFn, "rt");
    if (pFile == 0)
    {
        // Â°Ã”Ã€Ã“Â¼Â­Â¹Ã¶Ã€Ã‡ ÃƒÃŠÂ±Ã¢ÃˆÂ­ Ã†Ã„Ã€ÃÃ€Â» Ã€ÃÃ€Â» Â¼Ã¶ Â¾Ã¸Â´Ã™.
        log->info("(!) Cannot open HeldenianGUID file.");
        return false;
    }
    else
    {
        log->info("(!) Reading HeldenianGUID file...");
        cp = new char[dwFileSize + 2];
        memset(cp, 0, dwFileSize + 2);
        fread(cp, dwFileSize, 1, pFile);

        pStrTok = new class CStrTok(cp, seps);
        token = pStrTok->pGet();

        while (token != 0)
        {

            if (cReadMode != 0)
            {
                switch (cReadMode)
                {
                    case 1:
                        m_dwHeldenianGUID = atoi(token);
                        copy_string(G_cTxt, "HeldenianGUID = %d", m_dwHeldenianGUID);
                        log->info(G_cTxt);
                        cReadMode = 0;
                        break;
                    case 2:
                        m_sLastHeldenianWinner = atoi(token);
                        copy_string(G_cTxt, "HeldenianWinnerSide = %d", m_sLastHeldenianWinner);
                        log->info(G_cTxt);
                        cReadMode = 0;
                        break;
                }
            }
            else
            {
                if (memcmp(token, "HeldenianGUID", 13) == 0) cReadMode = 1;
                if (memcmp(token, "winner-side", 11) == 0) cReadMode = 2;
            }

            token = pStrTok->pGet();
        }

        delete pStrTok;
        delete cp;
    }
    if (pFile != 0) fclose(pFile);

    return true;
}

bool CGame::bReadCrusadeGUIDFile(char * cFn)
{
    FILE * pFile;
    HANDLE hFile;
    uint32_t  dwFileSize;
    char * cp, * token, cReadMode;
    char seps[] = "= \t\n";
    class CStrTok * pStrTok;

    cReadMode = 0;

    hFile = CreateFile(cFn, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    dwFileSize = GetFileSize(hFile, 0);
    if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

    pFile = fopen(cFn, "rt");
    if (pFile == 0)
    {
        // Â°Ã”Ã€Ã“Â¼Â­Â¹Ã¶Ã€Ã‡ ÃƒÃŠÂ±Ã¢ÃˆÂ­ Ã†Ã„Ã€ÃÃ€Â» Ã€ÃÃ€Â» Â¼Ã¶ Â¾Ã¸Â´Ã™.
        log->info("(!) Cannot open CrusadeGUID file.");
        return false;
    }
    else
    {
        log->info("(!) Reading CrusadeGUID file...");
        cp = new char[dwFileSize + 2];
        memset(cp, 0, dwFileSize + 2);
        fread(cp, dwFileSize, 1, pFile);

        pStrTok = new class CStrTok(cp, seps);
        token = pStrTok->pGet();

        while (token != 0)
        {

            if (cReadMode != 0)
            {
                switch (cReadMode)
                {
                    case 1:
                        m_dwCrusadeGUID = atoi(token);
                        copy_string(G_cTxt, "CrusadeGUID = %d", m_dwCrusadeGUID);
                        log->info(G_cTxt);
                        cReadMode = 0;
                        break;

                    case 2:
                        // New 13/05/2004 Changed
                        m_iLastCrusadeWinner = atoi(token);
                        copy_string(G_cTxt, "CrusadeWinnerSide = %d", m_iLastCrusadeWinner);
                        log->info(G_cTxt);
                        cReadMode = 0;
                        break;
                }
            }
            else
            {
                if (memcmp(token, "CrusadeGUID", 11) == 0) cReadMode = 1;
                if (memcmp(token, "winner-side", 11) == 0) cReadMode = 2;
            }

            token = pStrTok->pGet();
        }

        delete pStrTok;
        delete cp;
    }
    if (pFile != 0) fclose(pFile);

    return true;
}

bool CGame::bReadProgramConfigFile(char * cFn)
{
    FILE * pFile;
    HANDLE hFile;
    uint32_t  dwFileSize;
    char * cp, * token, cReadMode, cTxt[120], cGSMode[16] = "";
    char seps[] = "= \t\n";
    class CStrTok * pStrTok;
    bool bGateDNS = false; // bGateDNS added by snaipperi
    bool bLogDNS = false; // bLogDNS added by snaipperi

    cReadMode = 0;

    hFile = CreateFile(cFn, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    dwFileSize = GetFileSize(hFile, 0);
    if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

    pFile = fopen(cFn, "rt");
    if (pFile == 0)
    {
        //	log->error("(!) Cannot open configuration file.");
        return false;
    }
    else
    {
        log->info("(!) Reading configuration file...");
        cp = new char[dwFileSize + 2];
        memset(cp, 0, dwFileSize + 2);
        fread(cp, dwFileSize, 1, pFile);

        pStrTok = new class CStrTok(cp, seps);
        token = pStrTok->pGet();
        //token = strtok( cp, seps );   
        while (token != 0)
        {

            if (cReadMode != 0)
            {
                switch (cReadMode)
                {
                    case 1:
                        memset(m_cServerName, 0, sizeof(m_cServerName));
                        if (strlen(token) > 10)
                        {
                            copy_string(cTxt, "(!!!) Game server name(%s) must within 10 chars!", token);
                            log->info(cTxt);
                            return false;
                        }
                        strcpy(m_cServerName, token);
                        copy_string(cTxt, "(*) Game server name : %s", m_cServerName);
                        log->info(cTxt);
                        cReadMode = 0;
                        break;

                    case 2:
                        // v2.04
                        memset(m_cGameServerAddr, 0, sizeof(m_cGameServerAddr));
                        char ServerAddr[50];
                        ::gethostname(ServerAddr, 50);
                        struct hostent * pHostEnt;
                        pHostEnt = ::gethostbyname(ServerAddr);
                        if (pHostEnt != 0)
                        {
                            copy_string(ServerAddr, "%d.%d.%d.%d",
                                (pHostEnt->h_addr_list[0][0] & 0x00ff),
                                (pHostEnt->h_addr_list[0][1] & 0x00ff),
                                (pHostEnt->h_addr_list[0][2] & 0x00ff),
                                (pHostEnt->h_addr_list[0][3] & 0x00ff));
                        }
                        strcpy(m_cGameServerAddr, ServerAddr);

                        copy_string(cTxt, "(*) Game server address : %s", m_cGameServerAddr);
                        log->info(cTxt);

                        m_iGameServerPort = atoi(token);
                        copy_string(cTxt, "(*) Game server port : %d", m_iGameServerPort);
                        log->info(cTxt);
                        cReadMode = 0;
                        break;

                    case 3:
                        memset(m_cLogServerAddr, 0, sizeof(m_cLogServerAddr));

                        if (bLogDNS == true)
                        { //bLogDNS by Snaipperi
                            log->info(cTxt);
                            char * cAddress = token;
                            char cDnsResult[40];
                            struct hostent * host_entry;

                            host_entry = gethostbyname(cAddress);
                            if (host_entry == 0)
                            {
                                copy_string(cTxt, "(!)DNS (%s) failed", token);
                                log->info(cTxt);
                                return false;
                            }

                            copy_string(cDnsResult, "%d.%d.%d.%d",
                                (pHostEnt->h_addr_list[0][0] & 0x00ff),
                                (pHostEnt->h_addr_list[0][1] & 0x00ff),
                                (pHostEnt->h_addr_list[0][2] & 0x00ff),
                                (pHostEnt->h_addr_list[0][3] & 0x00ff));

                            copy_string(cTxt, "(!)DNS from (%s) to (%s) success!", token, cDnsResult);
                            log->info(cTxt);
                            strcpy(m_cLogServerAddr, cDnsResult);
                            copy_string(cTxt, "(*) Log server address : %s", m_cLogServerAddr);
                            log->info(cTxt);
                            cReadMode = 0;
                            break;
                        }

                        if (strlen(token) > 20)
                        {
                            copy_string(cTxt, "(!!!) Log server address(%s) must within 20 chars!", token);
                            log->info(cTxt);
                            return false;
                        }
                        strcpy(m_cLogServerAddr, token);
                        copy_string(cTxt, "(*) Log server address : %s", m_cLogServerAddr);
                        log->info(cTxt);
                        cReadMode = 0;
                        break;


                    case 4:
                        m_iLogServerPort = atoi(token);
                        copy_string(cTxt, "(*) Log server port : %d", m_iLogServerPort);
                        log->info(cTxt);
                        cReadMode = 0;
                        break;

                    case 5:
                        if (strlen(token) > 10)
                        {
                            copy_string(cTxt, "(!!!) CRITICAL ERROR! Map name(%s) must within 10 chars!", token);
                            log->info(cTxt);
                            return false;
                        }
                        if (_bRegisterMap(token) == false)
                        {
                            return false;
                        }
                        cReadMode = 0;
                        break;

                    case 6:
                        memset(m_cGateServerAddr, 0, sizeof(m_cGateServerAddr));
                        if (bGateDNS == true)
                        { //bGateDNS by Snaipperi
                            log->info(cTxt);
                            char * cAddress = token;
                            char cDnsResult[40];
                            struct hostent * host_entry;

                            host_entry = gethostbyname(cAddress);
                            if (host_entry == 0)
                            {
                                copy_string(cTxt, "(!)DNS (%s) failed", token);
                                log->info(cTxt);
                                return false;
                            }

                            copy_string(cDnsResult, "%d.%d.%d.%d",
                                (host_entry->h_addr_list[0][0] & 0x00ff),
                                (host_entry->h_addr_list[0][1] & 0x00ff),
                                (host_entry->h_addr_list[0][2] & 0x00ff),
                                (host_entry->h_addr_list[0][3] & 0x00ff));

                            copy_string(cTxt, "(!)DNS from (%s) to (%s) success!", token, cDnsResult);
                            log->info(cTxt);
                            strcpy(m_cGateServerAddr, cDnsResult);
                            copy_string(cTxt, "(*) Gate server address : %s", m_cGateServerAddr);
                            log->info(cTxt);
                            cReadMode = 0;
                            break;
                        }

                        if (strlen(token) > 20)
                        {
                            copy_string(cTxt, "(!!!) Gate server address(%s) must within 20 chars!", token);
                            log->info(cTxt);
                            return false;
                        }
                        strcpy(m_cGateServerAddr, token);
                        copy_string(cTxt, "(*) Gate server address : %s", m_cGateServerAddr);
                        log->info(cTxt);
                        cReadMode = 0;
                        break;
                        break;

                    case 7:
                        m_iGateServerPort = atoi(token);
                        copy_string(cTxt, "(*) Gate server port : %d", m_iGateServerPort);
                        log->info(cTxt);
                        cReadMode = 0;
                        break;

                    case 8:
                        memset(m_cGameServerAddrInternal, 0, sizeof(m_cGameServerAddrInternal));
                        if (strlen(token) > 15)
                        {
                            copy_string(cTxt, "(!!!) Internal (LAN) Game server address(%s) must within 15 chars!", token);
                            log->info(cTxt);
                            return false;
                        }
                        strcpy(m_cGameServerAddrInternal, token);
                        copy_string(cTxt, "(*) Internal (LAN) Game server address : %s", m_cGameServerAddrInternal);
                        log->info(cTxt);
                        cReadMode = 0;
                        break;


                    case 9:
                        memset(m_cGameServerAddrExternal, 0, sizeof(m_cGameServerAddrExternal));
                        if (strlen(token) > 15)
                        {
                            copy_string(cTxt, "(!!!) External (Internet) Game server address(%s) must within 15 chars!", token);
                            log->info(cTxt);
                            return false;
                        }
                        strcpy(m_cGameServerAddrExternal, token);
                        copy_string(cTxt, "(*) External (Internet) Game server address : %s", m_cGameServerAddrExternal);
                        log->info(cTxt);
                        cReadMode = 0;
                        break;

                    case 10:
                        memset(m_cGameServerAddr, 0, sizeof(m_cGameServerAddr));
                        if (strlen(token) > 15)
                        {
                            copy_string(cTxt, "(!!!) Game server address(%s) must within 15 chars!", token);
                            log->info(cTxt);
                            return false;
                        }
                        strcpy(m_cGameServerAddr, token);
                        copy_string(cTxt, "(*) Game server address : %s", m_cGameServerAddr);
                        log->info(cTxt);
                        cReadMode = 0;
                        break;
                    case 11:
                        if ((memcmp(token, "lan", 3) == 0) || (memcmp(token, "LAN", 3) == 0))
                        {
                            m_iGameServerMode = 1;
                            memcpy(cGSMode, "LAN", 3);
                        }
                        if ((memcmp(token, "internet", 3) == 0) || (memcmp(token, "INTERNET", 3) == 0))
                        {
                            m_iGameServerMode = 2;
                            memcpy(cGSMode, "INTERNET", 8);
                        }
                        if (m_iGameServerMode == 0)
                        {
                            copy_string(cTxt, "(!!!) Game server mode(%s) must be either LAN/lan/INTERNET/internet", token);
                            log->info(cTxt);
                            return false;
                        }
                        copy_string(cTxt, "(*) Game server mode : %s", cGSMode);
                        log->info(cTxt);
                        cReadMode = 0;
                        break;

                    case 12: //Gate DNS by SNaipperi
                        if (memcmp(token, "true", 4) == 0)
                        {
                            bGateDNS = true;
                        }
                        else bGateDNS = false;
                        copy_string(cTxt, "Gate DNS is set to %i", bGateDNS);
                        log->info(cTxt);
                        cReadMode = 0;
                        break;

                    case 13: //Log DNS by Snaipperi
                        if (memcmp(token, "true", 4) == 0)
                        {
                            bLogDNS = true;
                        }
                        else bLogDNS = false;
                        copy_string(cTxt, "Log DNS is set to %i", bLogDNS);
                        log->info(cTxt);
                        cReadMode = 0;
                        break;
                }
            }
            else
            {
                if (memcmp(token, "game-server-name", 16) == 0)			cReadMode = 1;
                if (memcmp(token, "game-server-port", 16) == 0)			cReadMode = 2;
                if (memcmp(token, "log-server-address", 18) == 0)		cReadMode = 3;
                if (memcmp(token, "internal-log-server-port", 24) == 0) cReadMode = 4;
                if (memcmp(token, "game-server-map", 15) == 0)			cReadMode = 5;
                if (memcmp(token, "gate-server-address", 19) == 0)		cReadMode = 6;
                if (memcmp(token, "gate-server-port", 16) == 0)			cReadMode = 7;
                if (memcmp(token, "game-server-internal-address", 28) == 0)			cReadMode = 8;
                if (memcmp(token, "game-server-external-address", 28) == 0)			cReadMode = 9;
                if (memcmp(token, "game-server-address", 19) == 0)		cReadMode = 10;
                if (memcmp(token, "game-server-mode", 16) == 0)			cReadMode = 11;
                if (memcmp(token, "gate-server-dns", 15) == 0) cReadMode = 12;
                if (memcmp(token, "log-server-dns", 14) == 0) cReadMode = 13;

            }
            token = pStrTok->pGet();
            //token = strtok( 0, seps );
        }
        delete pStrTok;
        delete cp;
    }
    if (pFile != 0) fclose(pFile);

    if (m_iGameServerMode == 0)
    {
        copy_string(cTxt, "(!!!) Game server mode cannot be empty. It must be either LAN/lan/INTERNET/internet", token);
        log->info(cTxt);
        return false;
    }

    return true;
}

bool CGame::bReadSettingsConfigFile(char * cFn)
{
    FILE * pFile;
    HANDLE hFile;
    uint32_t  dwFileSize;
    char * cp, * token, cReadMode, cTxt[120], cGSMode[16] = "", len;
    char seps[] = "= \t\n";
    class CStrTok * pStrTok;

    cReadMode = 0;

    hFile = CreateFile(cFn, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    dwFileSize = GetFileSize(hFile, 0);
    if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

    pFile = fopen(cFn, "rt");
    if (pFile == 0)
    {
        //   log->error("(!) Cannot open configuration file."); 
        return false;
    }
    else
    {
        log->info("(!) Reading settings file...");
        cp = new char[dwFileSize + 2];
        memset(cp, 0, dwFileSize + 2);
        fread(cp, dwFileSize, 1, pFile);

        pStrTok = new class CStrTok(cp, seps);
        token = pStrTok->pGet();
        //token = strtok( cp, seps );    
        while (token != 0)
        {

            if (cReadMode != 0)
            {
                switch (cReadMode)
                {

                    case 1:
                        if ((strlen(token) != 0) && (strlen(token) <= 10000) && (strlen(token) >= 1))
                        {
                            m_iPrimaryDropRate = atoi(token);
                        }
                        else
                        {
                            copy_string(cTxt, "(!!!) Primary drop rate(%s) invalid must be between 1 to 10000", token);
                            log->info(cTxt);
                        }
                        copy_string(cTxt, "(*) Primary drop rate: (%d)", m_iPrimaryDropRate);
                        log->info(cTxt);
                        cReadMode = 0;
                        break;

                    case 2:
                        if ((strlen(token) != 0) && (strlen(token) <= 10000) && (strlen(token) >= 1))
                        {
                            m_iSecondaryDropRate = atoi(token);
                        }
                        else
                        {
                            copy_string(cTxt, "(!!!) Secondary drop rate(%s) invalid must be between 1 to 10000", token);
                            log->info(cTxt);
                        }
                        copy_string(cTxt, "(*) Secondary drop rate: (%d)", m_iSecondaryDropRate);
                        log->info(cTxt);
                        cReadMode = 0;
                        break;

                    case 3:
                        if ((memcmp(token, "classic", 7) == 0) || (memcmp(token, "CLASSIC", 7) == 0))
                        {
                            m_bEnemyKillMode = false;
                            copy_string(cTxt, "(*) Enemy Kill Mode: CLASSIC");
                            log->info(cTxt);
                        }
                        else if ((memcmp(token, "deathmatch", 10) == 0) || (memcmp(token, "DEATHMATCH", 10) == 0))
                        {
                            m_bEnemyKillMode = true;
                            copy_string(cTxt, "(*) Enemy Kill Mode: DEATHMATCH");
                            log->info(cTxt);
                        }
                        else
                        {
                            copy_string(cTxt, "(!!!) ENEMY-KILL-MODE: (%s) must be either DEATHMATCH or CLASSIC", token);
                            log->info(cTxt);
                            return false;
                        }
                        cReadMode = 0;
                        break;

                    case 4:
                        if ((strlen(token) != 0) && (strlen(token) <= 100) && (strlen(token) >= 1))
                        {
                            m_iEnemyKillAdjust = atoi(token);
                        }
                        else
                        {
                            copy_string(cTxt, "(!!!) ENEMY-KILL-ADJUST: (%s) invalid must be between 1 to 100", token);
                            log->info(cTxt);
                        }
                        copy_string(cTxt, "(*) Player gains (%d) EK per enemy kill.", m_iEnemyKillAdjust);
                        log->info(cTxt);
                        cReadMode = 0;
                        break;

                    case 5:
                        if ((memcmp(token, "on", 2) == 0) || (memcmp(token, "ON", 2) == 0))
                        {
                            m_bAdminSecurity = true;
                            copy_string(cTxt, "(*) Administator actions limited through security.");
                            log->info(cTxt);
                        }
                        if ((memcmp(token, "off", 3) == 0) || (memcmp(token, "OFF", 3) == 0))
                        {
                            m_bAdminSecurity = false;
                            copy_string(cTxt, "(*) Administator vulnerability ignored.");
                            log->info(cTxt);
                        }

                        cReadMode = 0;
                        break;

                    case 6:
                        m_sRaidTimeMonday = atoi(token);
                        if (m_sRaidTimeMonday == 0) m_sRaidTimeMonday = 3;
                        copy_string(cTxt, "(*) Monday Raid Time : (%d) minutes", m_sRaidTimeMonday);
                        log->info(cTxt);
                        cReadMode = 0;
                        break;

                    case 7:
                        m_sRaidTimeTuesday = atoi(token);
                        if (m_sRaidTimeTuesday == 0) m_sRaidTimeTuesday = 3;
                        copy_string(cTxt, "(*) Tuesday Raid Time : (%d) minutes", m_sRaidTimeTuesday);
                        log->info(cTxt);
                        cReadMode = 0;
                        break;

                    case 8:
                        m_sRaidTimeWednesday = atoi(token);
                        if (m_sRaidTimeWednesday == 0) m_sRaidTimeWednesday = 3;
                        copy_string(cTxt, "(*) Wednesday Raid Time : (%d) minutes", m_sRaidTimeWednesday);
                        log->info(cTxt);
                        cReadMode = 0;
                        break;

                    case 9:
                        m_sRaidTimeThursday = atoi(token);
                        if (m_sRaidTimeThursday == 0) m_sRaidTimeThursday = 3;
                        copy_string(cTxt, "(*) Thursday Raid Time : (%d) minutes", m_sRaidTimeThursday);
                        log->info(cTxt);
                        cReadMode = 0;
                        break;

                    case 10:
                        m_sRaidTimeFriday = atoi(token);
                        if (m_sRaidTimeFriday == 0) m_sRaidTimeFriday = 10;
                        copy_string(cTxt, "(*) Friday Raid Time : (%d) minutes", m_sRaidTimeFriday);
                        log->info(cTxt);
                        cReadMode = 0;
                        break;

                    case 11:
                        m_sRaidTimeSaturday = atoi(token);
                        if (m_sRaidTimeSaturday == 0) m_sRaidTimeSaturday = 45;
                        copy_string(cTxt, "(*) Saturday Raid Time : (%d) minutes", m_sRaidTimeSaturday);
                        log->info(cTxt);
                        cReadMode = 0;
                        break;

                    case 12:
                        m_sRaidTimeSunday = atoi(token);
                        if (m_sRaidTimeSunday == 0) m_sRaidTimeSunday = 60;
                        copy_string(cTxt, "(*) Sunday Raid Time : (%d) minutes", m_sRaidTimeSunday);
                        log->info(cTxt);
                        cReadMode = 0;
                        break;

                    case 13:
                        if ((memcmp(token, "player", 6) == 0) || (memcmp(token, "PLAYER", 6) == 0))
                        {
                            m_bLogChatOption = 1;
                            copy_string(cTxt, "(*) Logging Player Chat.");
                            log->info(cTxt);
                        }
                        if ((memcmp(token, "gm", 2) == 0) || (memcmp(token, "GM", 2) == 0))
                        {
                            m_bLogChatOption = 2;
                            copy_string(cTxt, "(*) Logging GM Chat.");
                            log->info(cTxt);
                        }
                        if ((memcmp(token, "all", 3) == 0) || (memcmp(token, "ALL", 3) == 0))
                        {
                            m_bLogChatOption = 3;
                            copy_string(cTxt, "(*) Logging All Chat.");
                            log->info(cTxt);
                        }
                        if ((memcmp(token, "none", 4) == 0) || (memcmp(token, "NONE", 4) == 0))
                        {
                            m_bLogChatOption = 4;
                            copy_string(cTxt, "(*) Not Logging Any Chat.");
                            log->info(cTxt);
                        }
                        cReadMode = 0;
                        break;

                    case 14:
                        m_iSummonGuildCost = atoi(token);
                        copy_string(cTxt, "(*) Summoning guild costs (%d) gold", m_iSummonGuildCost);
                        log->info(cTxt);
                        cReadMode = 0;
                        break;

                    case 15:
                        m_sSlateSuccessRate = atoi(token);
                        copy_string(cTxt, "(*) Slate Success Rate (%d) percent", m_sSlateSuccessRate);
                        log->info(cTxt);
                        if (m_sSlateSuccessRate == 0) m_sSlateSuccessRate = 14;
                        cReadMode = 0;
                        break;

                    case 16:
                        m_sCharStatLimit = atoi(token);
                        if (m_sCharStatLimit == 0) m_sCharStatLimit = 200;
                        cReadMode = 0;
                        break;

                    case 17:
                        m_sCharSkillLimit = atoi(token);
                        if (m_sCharSkillLimit == 0) m_sCharSkillLimit = 700;
                        cReadMode = 0;
                        break;

                    case 18:
                        m_cRepDropModifier = atoi(token);
                        copy_string(cTxt, "(*) Rep<->Drop modifier: (%d)", m_cRepDropModifier);
                        log->info(cTxt);
                        if (m_cRepDropModifier < 0) m_cRepDropModifier = 0;
                        cReadMode = 0;
                        break;

                    case 19:
                        memset(m_cSecurityNumber, 0, sizeof(m_cSecurityNumber));
                        len = strlen(token);
                        if (len > 10) len = 10;
                        memcpy(m_cSecurityNumber, token, len);
                        copy_string(cTxt, "(*) Security number memorized!");
                        log->info(cTxt);
                        cReadMode = 0;
                        break;

                    case 20:
                        m_sMaxPlayerLevel = atoi(token);
                        if (m_sMaxPlayerLevel == 0) m_sMaxPlayerLevel = 180;
                        cReadMode = 0;
                        break;

                }
            }
            else
            {
                if (memcmp(token, "primary-drop-rate", 17) == 0)   cReadMode = 1;
                if (memcmp(token, "secondary-drop-rate", 19) == 0)   cReadMode = 2;
                if (memcmp(token, "enemy-kill-mode", 15) == 0)   cReadMode = 3;
                if (memcmp(token, "enemy-kill-adjust", 17) == 0)   cReadMode = 4;
                if (memcmp(token, "admin-security", 14) == 0)   cReadMode = 5;
                if (memcmp(token, "monday-raid-time", 16) == 0)   cReadMode = 6;
                if (memcmp(token, "tuesday-raid-time", 17) == 0)   cReadMode = 7;
                if (memcmp(token, "wednesday-raid-time", 19) == 0)   cReadMode = 8;
                if (memcmp(token, "thursday-raid-time", 18) == 0)   cReadMode = 9;
                if (memcmp(token, "friday-raid-time", 16) == 0)   cReadMode = 10;
                if (memcmp(token, "saturday-raid-time", 18) == 0)   cReadMode = 11;
                if (memcmp(token, "sunday-raid-time", 16) == 0)   cReadMode = 12;
                if (memcmp(token, "log-chat-settings", 17) == 0)		cReadMode = 13;
                if (memcmp(token, "summonguild-cost", 16) == 0)		cReadMode = 14;
                if (memcmp(token, "slate-success-rate", 18) == 0)		cReadMode = 15;
                if (memcmp(token, "character-stat-limit", 20) == 0)		cReadMode = 16;
                if (memcmp(token, "character-skill-limit", 21) == 0)		cReadMode = 17;
                if (memcmp(token, "rep-drop-modifier", 17) == 0)		cReadMode = 18;
                if (memcmp(token, "admin-security-code", 19) == 0)		cReadMode = 19;
                if (memcmp(token, "max-player-level", 16) == 0)		cReadMode = 20;
            }

            token = pStrTok->pGet();
            //token = strtok( 0, seps ); 
        }

        delete pStrTok;
        delete cp;
    }
    if (pFile != 0) fclose(pFile);

    return true;
}

bool CGame::bReadCrusadeStructureConfigFile(char * cFn)
{
    FILE * pFile;
    HANDLE hFile;
    uint32_t  dwFileSize;
    char * cp, * token, cReadModeA, cReadModeB;
    char seps[] = "= \t\n";
    int   iIndex;
    class CStrTok * pStrTok;

    cReadModeA = 0;
    cReadModeB = 0;

    hFile = CreateFile(cFn, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    dwFileSize = GetFileSize(hFile, 0);
    if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

    pFile = fopen(cFn, "rt");
    if (pFile == 0)
    {
        log->info("(!) Cannot open Crusade configuration file.");
        return false;
    }
    else
    {
        log->info("(!) Reading Crusade configuration file...");
        cp = new char[dwFileSize + 2];
        memset(cp, 0, dwFileSize + 2);
        fread(cp, dwFileSize, 1, pFile);

        iIndex = 0;
        pStrTok = new class CStrTok(cp, seps);
        token = pStrTok->pGet();
        while (token != 0)
        {
            if (cReadModeA != 0)
            {
                switch (cReadModeA)
                {
                    case 1:
                        switch (cReadModeB)
                        {
                            case 1:
                                if (_bGetIsStringIsNumber(token) == false)
                                {
                                    log->critical("(!!!) CRITICAL ERROR!  Crusade configuration file error - Wrong Data format(1).");
                                    delete cp;
                                    delete pStrTok;
                                    return false;
                                }
                                iIndex = atoi(token);

                                if (m_stCrusadeStructures[iIndex].cType != 0)
                                {
                                    log->critical("(!!!) CRITICAL ERROR!  Crusade configuration file error - Duplicate portion number.");
                                    delete cp;
                                    delete pStrTok;
                                    return false;
                                }

                                cReadModeB = 2;
                                break;

                            case 2:
                                memset(m_stCrusadeStructures[iIndex].cMapName, 0, sizeof(m_stCrusadeStructures[iIndex].cMapName));
                                memcpy(m_stCrusadeStructures[iIndex].cMapName, token, strlen(token));
                                cReadModeB = 3;
                                break;


                            case 3:
                                if (_bGetIsStringIsNumber(token) == false)
                                {
                                    log->critical("(!!!) CRITICAL ERROR!  Crusade configuration file error - Wrong Data format.");
                                    delete cp;
                                    delete pStrTok;
                                    return false;
                                }
                                m_stCrusadeStructures[iIndex].cType = atoi(token);
                                cReadModeB = 4;
                                break;

                            case 4:
                                if (_bGetIsStringIsNumber(token) == false)
                                {
                                    log->critical("(!!!) CRITICAL ERROR!  Crusade configuration file error - Wrong Data format.");
                                    delete cp;
                                    delete pStrTok;
                                    return false;
                                }
                                m_stCrusadeStructures[iIndex].dX = atoi(token);
                                cReadModeB = 5;
                                break;

                            case 5:
                                if (_bGetIsStringIsNumber(token) == false)
                                {
                                    log->critical("(!!!) CRITICAL ERROR!  Crusade configuration file error - Wrong Data format.");
                                    delete cp;
                                    delete pStrTok;
                                    return false;
                                }
                                m_stCrusadeStructures[iIndex].dY = atoi(token);
                                cReadModeA = 0;
                                cReadModeB = 0;
                                break;
                        }
                        break;

                    default:
                        break;
                }
            }
            else
            {
                if (memcmp(token, "crusade-structure", 17) == 0)
                {
                    cReadModeA = 1;
                    cReadModeB = 1;
                }

            }
            token = pStrTok->pGet();
        }

        delete pStrTok;
        delete cp;

        if ((cReadModeA != 0) || (cReadModeB != 0))
        {
            log->critical("(!!!) CRITICAL ERROR! Crusade Structure configuration file contents error!");
            return false;
        }
    }

    if (pFile != 0) fclose(pFile);
    return true;
}

bool CGame::bReadNotifyMsgListFile(char * cFn)
{
    FILE * pFile;
    HANDLE hFile;
    uint32_t  dwFileSize;
    char * cp, * token, cReadMode;
    char seps[] = "=\t\n;";
    class CStrTok * pStrTok;
    int   i;

    cReadMode = 0;
    m_iTotalNoticeMsg = 0;

    hFile = CreateFile(cFn, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    dwFileSize = GetFileSize(hFile, 0);
    if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

    pFile = fopen(cFn, "rt");
    if (pFile == 0)
    {
        log->info("(!) Notify Message list file not found!...");
        return false;
    }
    else
    {
        log->info("(!) Reading Notify Message list file...");
        cp = new char[dwFileSize + 2];
        memset(cp, 0, dwFileSize + 2);
        fread(cp, dwFileSize, 1, pFile);

        pStrTok = new class CStrTok(cp, seps);
        token = pStrTok->pGet();
        //token = strtok( cp, seps );   
        while (token != 0)
        {

            if (cReadMode != 0)
            {
                switch (cReadMode)
                {
                    case 1:
                        for (i = 0; i < DEF_MAXNOTIFYMSGS; i++)
                            if (m_pNoticeMsgList[i] == 0)
                            {
                                m_pNoticeMsgList[i] = new class CMsg;
                                m_pNoticeMsgList[i]->bPut(0, token, strlen(token), 0, 0);
                                m_iTotalNoticeMsg++;
                                goto LNML_NEXTSTEP1;
                            }
                        LNML_NEXTSTEP1:;
                        cReadMode = 0;
                        break;
                }
            }
            else
            {
                if (memcmp(token, "notify_msg", 10) == 0) cReadMode = 1;
            }

            token = pStrTok->pGet();
            //token = strtok( 0, seps );
        }

        delete pStrTok;
        delete cp;
    }
    if (pFile != 0) fclose(pFile);

    return true;
}

bool CGame::bReadScheduleConfigFile(char * pFn)
{
    FILE * pFile;
    HANDLE hFile;
    uint32_t  dwFileSize;
    char * cp, * token, cReadModeA, cReadModeB;
    char seps[] = "= \t\n";
    class CStrTok * pStrTok;
    int   iIndex;

    cReadModeA = 0;
    cReadModeB = 0;
    iIndex = 0;

    hFile = CreateFile(pFn, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    dwFileSize = GetFileSize(hFile, 0);
    if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

    pFile = fopen(pFn, "rt");
    if (pFile == 0)
    {
        // °ÔÀÓ¼­¹öÀÇ ÃÊ±âÈ­ ÆÄÀÏÀ» ÀÐÀ» ¼ö ¾ø´Ù.
        log->info("(!) Cannot open Schedule file.");
        return false;
    }
    else
    {
        log->info("(!) Reading Schedule file...");
        cp = new char[dwFileSize + 2];
        memset(cp, 0, dwFileSize + 2);
        fread(cp, dwFileSize, 1, pFile);

        pStrTok = new class CStrTok(cp, seps);
        token = pStrTok->pGet();

        while (token != 0)
        {

            if (cReadModeA != 0)
            {
                switch (cReadModeA)
                {
                    case 1:
                        if (strcmp(m_cServerName, token) == 0)
                        {
                            log->info("(!) Success: This server will schedule crusade total war.");
                            m_bIsCrusadeWarStarter = true;
                        }
                        cReadModeA = 0;
                        break;

                    case 2:
                        if (strcmp(m_cServerName, token) == 0)
                        {
                            log->info("(!) Success: This server will schedule apocalypse.");
                            m_bIsApocalypseStarter = true;
                        }
                        cReadModeA = 0;
                        break;

                    case 3:
                        switch (cReadModeB)
                        {
                            case 1:
                                if (iIndex >= DEF_MAXSCHEDULE)
                                {
                                    log->info("(!) WARNING! Too many crusade war schedule!");
                                    return true;
                                }
                                m_stCrusadeWarSchedule[iIndex].iDay = atoi(token);
                                cReadModeB = 2;
                                break;
                            case 2:
                                m_stCrusadeWarSchedule[iIndex].iHour = atoi(token);
                                cReadModeB = 3;
                                break;
                            case 3:
                                m_stCrusadeWarSchedule[iIndex].iMinute = atoi(token);
                                iIndex++;
                                cReadModeA = 0;
                                cReadModeB = 0;
                                break;
                        }
                        break;

                    case 4:
                        switch (cReadModeB)
                        {
                            case 1:
                                if (iIndex >= DEF_MAXAPOCALYPSE)
                                {
                                    log->info("(!) WARNING! Too many Apocalypse schedule!");
                                    return true;
                                }
                                m_stApocalypseScheduleStart[iIndex].iDay = atoi(token);
                                cReadModeB = 2;
                                break;
                            case 2:
                                m_stApocalypseScheduleStart[iIndex].iHour = atoi(token);
                                cReadModeB = 3;
                                break;
                            case 3:
                                m_stApocalypseScheduleStart[iIndex].iMinute = atoi(token);
                                iIndex++;
                                cReadModeA = 0;
                                cReadModeB = 0;
                                break;
                        }
                        break;

                    case 5:
                        switch (cReadModeB)
                        {
                            case 1:
                                if (iIndex >= DEF_MAXAPOCALYPSE)
                                {
                                    log->info("(!) WARNING! Too many Apocalypse schedule!");
                                    return true;
                                }
                                m_stApocalypseScheduleEnd[iIndex].iDay = atoi(token);
                                cReadModeB = 2;
                                break;
                            case 2:
                                m_stApocalypseScheduleEnd[iIndex].iHour = atoi(token);
                                cReadModeB = 3;
                                break;
                            case 3:
                                m_stApocalypseScheduleEnd[iIndex].iMinute = atoi(token);
                                iIndex++;
                                cReadModeA = 0;
                                cReadModeB = 0;
                                break;
                        }
                        break;

                    case 6:
                        switch (cReadModeB)
                        {
                            case 1:
                                if (iIndex >= DEF_MAXHELDENIAN)
                                {
                                    log->info("(!) WARNING! Too many Heldenians scheduled!");
                                    return true;
                                }
                                m_stHeldenianSchedule[iIndex].iDay = atoi(token);
                                cReadModeB = 2;
                                break;
                            case 2:
                                m_stHeldenianSchedule[iIndex].StartiHour = atoi(token);
                                cReadModeB = 3;
                                break;
                            case 3:
                                m_stHeldenianSchedule[iIndex].StartiMinute = atoi(token);
                                cReadModeB = 4;
                                break;
                            case 4:
                                m_stHeldenianSchedule[iIndex].EndiHour = atoi(token);
                                cReadModeB = 5;
                                break;
                            case 5:
                                m_stHeldenianSchedule[iIndex].EndiMinute = atoi(token);
                                iIndex++;
                                cReadModeA = 0;
                                cReadModeB = 0;
                                break;

                        }
                        break;
                }
            }
            else
            {
                if (memcmp(token, "crusade-server-name", 19) == 0)
                {
                    cReadModeA = 1;
                    cReadModeB = 1;
                }
                if (memcmp(token, "apocalypse-server-name", 22) == 0)
                {
                    cReadModeA = 2;
                    cReadModeB = 1;
                }
                if (memcmp(token, "crusade-schedule", 16) == 0)
                {
                    cReadModeA = 3;
                    cReadModeB = 1;
                }
                if (memcmp(token, "apocalypse-schedule-start", 25) == 0)
                {
                    cReadModeA = 4;
                    cReadModeB = 1;
                }
                if (memcmp(token, "apocalypse-schedule-end", 23) == 0)
                {
                    cReadModeA = 5;
                    cReadModeB = 1;
                }
                if (memcmp(token, "Heldenian-schedule", 18) == 0)
                {
                    cReadModeA = 6;
                    cReadModeB = 1;
                }
            }

            token = pStrTok->pGet();
        }
        delete pStrTok;
        delete cp;
    }
    if (pFile != 0) fclose(pFile);

    return true;
}
