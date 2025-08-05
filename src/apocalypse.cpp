//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include "Game.h"

void CGame::LocalEndApocalypse()
{
    int i;

    m_bIsApocalypseMode = false;

    for (i = 1; i < DEF_MAXCLIENTS; i++)
    {
        if (m_pClientList[i] != 0)
        {
            SendNotifyMsg(0, i, DEF_NOTIFY_APOCGATEENDMSG, 0, 0, 0, 0);
        }
    }
    wsprintf(G_cTxt, "(!)Apocalypse Mode OFF.");
    log->info(G_cTxt);
}

void CGame::LocalStartApocalypse(uint32_t dwApocalypseGUID)
{
    int i;
    //uint32_t dwApocalypse;

    m_bIsApocalypseMode = true;

    if (dwApocalypseGUID != 0)
    {
        _CreateApocalypseGUID(dwApocalypseGUID);
        //m_dwApocalypseGUID = dwApocalypse;
    }

    for (i = 1; i < DEF_MAXCLIENTS; i++)
    {
        if (m_pClientList[i] != 0)
        {
            SendNotifyMsg(0, i, DEF_NOTIFY_APOCGATESTARTMSG, 0, 0, 0, 0);
            //RequestTeleportHandler(i, "0   ");
            //SendNotifyMsg(0, i, DEF_NOTIFY_APOCFORCERECALLPLAYERS, 0, 0, 0, 0);
        }
    }
    wsprintf(G_cTxt, "(!)Apocalypse Mode ON.");
    log->info(G_cTxt);
}

void CGame::_CreateApocalypseGUID(uint32_t dwApocalypseGUID)
{
    char * cp, cTxt[256], cFn[256], cTemp[1024];
    FILE * pFile;

    _mkdir("GameData");
    memset(cFn, 0, sizeof(cFn));

    strcat(cFn, "GameData");
    strcat(cFn, "\\");
    strcat(cFn, "\\");
    strcat(cFn, "ApocalypseGUID.Txt");

    pFile = fopen(cFn, "wt");
    if (pFile == 0)
    {
        // Ã†Ã„Ã€ÃÃ€Â» Â¸Â¸ÂµÃ© Â¼Ã¶ Â¾Ã¸Â°Ã…Â³Âª Â»Ã§Ã€ÃŒÃÃ®Â°Â¡ ÃÃ¶Â³ÂªÃ„Â¡Â°Ã” Ã€Ã›Ã€Âº Â°Ã¦Â¿Ã¬Â´Ã‚ . 
        wsprintf(cTxt, "(!) Cannot create ApocalypseGUID(%d) file", dwApocalypseGUID);
        log->info(cTxt);
    }
    else
    {
        memset(cTemp, 0, sizeof(cTemp));

        memset(cTxt, 0, sizeof(cTxt));
        wsprintf(cTxt, "ApocalypseGUID = %d\n", dwApocalypseGUID);
        strcat(cTemp, cTxt);

        cp = (char *)cTemp;
        fwrite(cp, strlen(cp), 1, pFile);

        wsprintf(cTxt, "(O) ApocalypseGUID(%d) file created", dwApocalypseGUID);
        log->info(cTxt);
    }
    if (pFile != 0) fclose(pFile);
}

void CGame::ApocalypseEnder()
{
    SYSTEMTIME SysTime;
    int i;

    if (m_bIsApocalypseMode == false) return;
    if (m_bIsApocalypseStarter == false) return;

    GetLocalTime(&SysTime);

    for (i = 0; i < DEF_MAXAPOCALYPSE; i++)
        if ((m_stApocalypseScheduleEnd[i].iDay == SysTime.wDayOfWeek) &&
            (m_stApocalypseScheduleEnd[i].iHour == SysTime.wHour) &&
            (m_stApocalypseScheduleEnd[i].iMinute == SysTime.wMinute))
        {
            log->info("(!) Automated apocalypse is concluded!");
            GlobalEndApocalypseMode();
            return;
        }
}

void CGame::GlobalEndApocalypseMode()
{
    char * cp, cData[120];

    if (m_bIsApocalypseMode == false) return;

    memset(cData, 0, sizeof(cData));
    cp = (char *)cData;
    *cp = GSM_ENDAPOCALYPSE;
    cp++;

    LocalEndApocalypse();

    bStockMsgToGateServer(cData, 5);
}
