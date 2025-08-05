//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include "Game.h"
#include "string_utils.h"

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
    log->info("(!)Apocalypse Mode OFF.");
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
    log->info("(!)Apocalypse Mode ON.");
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
        log->info(cTxt, "(!) Cannot create ApocalypseGUID({}) file", dwApocalypseGUID);
    }
    else
    {
        memset(cTemp, 0, sizeof(cTemp));

        memset(cTxt, 0, sizeof(cTxt));
        copy_string(cTxt, "ApocalypseGUID = %d\n", dwApocalypseGUID);
        strcat(cTemp, cTxt);

        cp = (char *)cTemp;
        fwrite(cp, strlen(cp), 1, pFile);

        copy_string(cTxt, "(O) ApocalypseGUID(%d) file created", dwApocalypseGUID);
        log->info(cTxt);
    }
    if (pFile != 0) fclose(pFile);
}
