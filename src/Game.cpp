//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include "Game.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/async.h>
#include <spdlog/fmt/fmt.h>

extern int32_t timeGetTime();

extern char G_cTxt[512];
extern char	G_cData50000[50000];

int _tmp_iMoveLocX[9][37] = {
    // 0
   {0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0},
    // 1
    {0,1,2,3,4,5,6,7,8,9,
    10,11,12,13,14,15,16,17,18,19,
    20,-1,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0},
    //2
    {0,1,2,3,4,5,6,7,8,9,
    10,11,12,13,14,15,16,17,18,19,
    20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,-1},
    //3
    {20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,-1,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0},
    //4
    {20,20,20,20,20,20,20,20,20,20,
    20,20,20,20,20,20,19,18,17,16,
    15,14,13,12,11,10,9,8,7,6,
    5,4,3,2,1,0,-1},
    //5
    {0,1,2,3,4,5,6,7,8,9,
    10,11,12,13,14,15,16,17,18,19,
    20,-1,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0},
    //6
    {0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,1,2,3,4,
    5,6,7,8,9,10,11,12,13,14,
    15,16,17,18,19,20,-1},
    //7
    {0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,-1,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0},
    //8
    {0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,1,2,3,4,
    5,6,7,8,9,10,11,12,13,14,
    15,16,17,18,19,20,-1}
};

int _tmp_iMoveLocY[9][37] = {
    // 0
    {0,0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0},
    //1
    {0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,-1,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0},
    //2
    {0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,1,2,3,4,5,6,7,8,9,
    10,11,12,13,14,15,-1},
    //3
    {0,1,2,3,4,5,6,7,8,9,
    10,11,12,13,14,15,-1,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0},
    //4
    {0,1,2,3,4,5,6,7,8,9,
    10,11,12,13,14,15,15,15,15,15,
    15,15,15,15,15,15,15,15,15,15,
    15,15,15,15,15,15,-1},
    //5
    {15,15,15,15,15,15,15,15,15,15,
    15,15,15,15,15,15,15,15,15,15,
    15,-1,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0},
    //6
    {0,1,2,3,4,5,6,7,8,9,
    10,11,12,13,14,15,15,15,15,15,
    15,15,15,15,15,15,15,15,15,15,
    15,15,15,15,15,15,-1},
    //7
    {0,1,2,3,4,5,6,7,8,9,
    10,11,12,13,14,15,-1,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0},
    // 8
    {15,14,13,12,11,10,9,8,7,6,
    5,4,3,2,1,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,-1}
};

CGame::CGame()
{
    spdlog::init_thread_pool(8192, 2);

    std::vector<spdlog::sink_ptr> sinks;

    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    sinks.push_back(stdout_sink);

    // daily rotation max 30 days
    auto rotating = std::make_shared<spdlog::sinks::daily_file_sink_mt>("logs/helbreath.log", 0, 0, false, 30);
    sinks.push_back(rotating);

    log = std::make_shared<spdlog::async_logger>("helbreath", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    spdlog::register_logger(log);

    log->set_pattern(log_formatting);
    log->set_level(loglevel);
    log->info("Logging initialized");

    int i, x;

    m_bIsGameStarted = false;
    m_bIsLogSockAvailable = false;
    m_bIsGateSockAvailable = false;
    m_bIsItemAvailable = false;
    m_bIsBuildItemAvailable = false;
    m_bIsNpcAvailable = false;
    m_bIsMagicAvailable = false;
    m_bIsSkillAvailable = false;
    m_bIsQuestAvailable = false;
    m_bIsPortionAvailable = false;

    memset(m_cServerName, 0, sizeof(m_cServerName));

    m_iPlayerMaxLevel = DEF_PLAYERMAXLEVEL;

    for (i = 0; i < DEF_MAXCLIENTS; i++)
        m_pClientList[i] = 0;

    for (i = 0; i < DEF_MAXMAPS; i++)
        m_pMapList[i] = 0;

    for (i = 0; i < DEF_MAXITEMTYPES; i++)
        m_pItemConfigList[i] = 0;

    for (i = 0; i < DEF_MAXNPCTYPES; i++)
        m_pNpcConfigList[i] = 0;

    for (i = 0; i < DEF_MAXNPCS; i++)
        m_pNpcList[i] = 0;

    for (i = 0; i < DEF_MSGQUENESIZE; i++)
        m_pMsgQuene[i] = 0;

    for (i = 0; i < DEF_MAXMAGICTYPE; i++)
        m_pMagicConfigList[i] = 0;

    for (i = 0; i < DEF_MAXSKILLTYPE; i++)
        m_pSkillConfigList[i] = 0;

    for (i = 0; i < DEF_MAXQUESTTYPE; i++)
        m_pQuestConfigList[i] = 0;

    for (i = 0; i < DEF_MAXDYNAMICOBJECTS; i++)
        m_pDynamicObjectList[i] = 0;

    for (i = 0; i < DEF_MAXDELAYEVENTS; i++)
        m_pDelayEventList[i] = 0;

    for (i = 0; i < DEF_MAXNOTIFYMSGS; i++)
        m_pNoticeMsgList[i] = 0;

    for (i = 0; i < DEF_MAXFISHS; i++)
        m_pFish[i] = 0;

    for (i = 0; i < DEF_MAXMINERALS; i++)
        m_pMineral[i] = 0;

    for (i = 0; i < DEF_MAXPORTIONTYPES; i++)
        m_pPortionConfigList[i] = 0;

    //	/for (i = 0; i < DEF_MAXTELEPORTTYPE; i++)
    //		m_pTeleportConfigList[i] = 0;

    for (i = 0; i < DEF_MAXSUBLOGSOCK; i++)
    {
        m_pSubLogSock[i] = 0;
        m_bIsSubLogSockAvailable[i] = false;
    }

    for (i = 0; i < DEF_MAXBUILDITEMS; i++)
        m_pBuildItemList[i] = 0;

    for (i = 0; i < DEF_MAXDUPITEMID; i++)
        m_pDupItemIDList[i] = 0;

    // New 06/05/2004
    for (i = 0; i < DEF_MAXCLIENTS; i++)
    {
        m_stPartyInfo[i].iTotalMembers = 0;
        for (x = 0; x < DEF_MAXPARTYMEMBERS; x++)
            m_stPartyInfo[i].iIndex[x] = 0;
    }

    m_iQueneHead = 0;
    m_iQueneTail = 0;

    m_iTotalClients = 0;
    m_iMaxClients = 0;
    m_iTotalMaps = 0;

    m_iTotalGameServerClients = 0;
    m_iTotalGameServerMaxClients = 0;

    m_bIsServerShutdowned = false;
    m_cShutDownCode = 0;

    m_iMiddlelandMapIndex = -1;
    m_iAresdenOccupyTiles = 0;
    m_iElvineOccupyTiles = 0;

    m_iCurMsgs = 0;
    m_iMaxMsgs = 0;

    m_stCityStatus[1].iCrimes = 0;
    m_stCityStatus[1].iFunds = 0;
    m_stCityStatus[1].iWins = 0;

    m_stCityStatus[2].iCrimes = 0;
    m_stCityStatus[2].iFunds = 0;
    m_stCityStatus[2].iWins = 0;

    m_iAutoRebootingCount = 0;
    m_bEnemyKillMode = false;
    m_iEnemyKillAdjust = 1;
    m_bAdminSecurity = true;
    m_sRaidTimeMonday = 0;
    m_sRaidTimeTuesday = 0;
    m_sRaidTimeWednesday = 0;
    m_sRaidTimeThursday = 0;
    m_sRaidTimeFriday = 0;
    m_sRaidTimeSaturday = 0;
    m_sRaidTimeSunday = 0;
    m_sCharPointLimit = 0;
    m_sSlateSuccessRate = 0;
    m_sForceRecallTime = 0;

    //Show Debug Window
    //DbgWnd = new CDebugWindow();
    //DbgWnd->Startup();
    //DbgWnd->AddEventMsg("CGame Startup");
    // 2002-09-09 #1
    m_bReceivedItemList = false;

}

CGame::~CGame()
{
    //DbgWnd->Shutdown();
    //delete DbgWnd;
}

bool CGame::bInit()
{
    char * cp, cTxt[120];
    uint32_t * dwp;
    uint16_t * wp;
    int  i;
    SYSTEMTIME SysTime;
    uint32_t dwTime = timeGetTime();

    //m_Misc.Temp();

    log->info("(!) INITIALIZING GAME SERVER...");
    //

    for (i = 0; i < DEF_MAXCLIENTS + 1; i++)
        m_iClientShortCut[i] = 0;

    for (i = 0; i < DEF_MAXCLIENTS; i++)
        if (m_pClientList[i] != 0) delete m_pClientList[i];

    for (i = 0; i < DEF_MAXNPCS; i++)
        if (m_pNpcList[i] != 0) delete m_pNpcList[i];

    for (i = 0; i < DEF_MAXMAPS; i++)
        if (m_pMapList[i] != 0) delete m_pMapList[i];

    for (i = 0; i < DEF_MAXITEMTYPES; i++)
        if (m_pItemConfigList[i] != 0) delete m_pItemConfigList[i];

    for (i = 0; i < DEF_MAXNPCTYPES; i++)
        if (m_pNpcConfigList[i] != 0) delete m_pNpcConfigList[i];

    for (i = 0; i < DEF_MAXMAGICTYPE; i++)
        if (m_pMagicConfigList[i] != 0) delete m_pMagicConfigList[i];

    for (i = 0; i < DEF_MAXSKILLTYPE; i++)
        if (m_pSkillConfigList[i] != 0) delete m_pSkillConfigList[i];

    for (i = 0; i < DEF_MAXQUESTTYPE; i++)
        if (m_pQuestConfigList[i] != 0) delete m_pQuestConfigList[i];

    for (i = 0; i < DEF_MAXDYNAMICOBJECTS; i++)
        if (m_pDynamicObjectList[i] != 0) delete m_pDynamicObjectList[i];

    for (i = 0; i < DEF_MAXDELAYEVENTS; i++)
        if (m_pDelayEventList[i] != 0) delete m_pDelayEventList[i];

    for (i = 0; i < DEF_MAXNOTIFYMSGS; i++)
        if (m_pNoticeMsgList[i] != 0) delete m_pNoticeMsgList[i];

    for (i = 0; i < DEF_MAXFISHS; i++)
        if (m_pFish[i] != 0) delete m_pFish[i];

    for (i = 0; i < DEF_MAXMINERALS; i++)
        if (m_pMineral[i] != 0) delete m_pMineral[i];

    for (i = 0; i < DEF_MAXPORTIONTYPES; i++)
        if (m_pPortionConfigList[i] != 0) delete m_pPortionConfigList[i];

    //	for (i = 0; i < DEF_MAXTELEPORTTYPE; i++)
    //	if (m_pTeleportConfigList[i] != 0) delete m_pTeleportConfigList[i];

    for (i = 0; i < DEF_MAXBUILDITEMS; i++)
        if (m_pBuildItemList[i] != 0) delete m_pBuildItemList[i];

    for (i = 0; i < DEF_MAXNPCTYPES; i++)
        m_iNpcConstructionPoint[i] = 0;


    for (i = 0; i < DEF_MAXSCHEDULE; i++)
    {
        m_stCrusadeWarSchedule[i].iDay = -1;
        m_stCrusadeWarSchedule[i].iHour = -1;
        m_stCrusadeWarSchedule[i].iMinute = -1;
    }

    for (i = 0; i < DEF_MAXAPOCALYPSE; i++)
    {
        m_stApocalypseScheduleStart[i].iDay = -1;
        m_stApocalypseScheduleStart[i].iHour = -1;
        m_stApocalypseScheduleStart[i].iMinute = -1;
    }

    for (i = 0; i < DEF_MAXHELDENIAN; i++)
    {
        m_stHeldenianSchedule[i].iDay = -1;
        m_stHeldenianSchedule[i].StartiHour = -1;
        m_stHeldenianSchedule[i].StartiMinute = -1;
        m_stHeldenianSchedule[i].EndiHour = -1;
        m_stHeldenianSchedule[i].EndiMinute = -1;
    }

    for (i = 0; i < DEF_MAXAPOCALYPSE; i++)
    {
        m_stApocalypseScheduleEnd[i].iDay = -1;
        m_stApocalypseScheduleEnd[i].iHour = -1;
        m_stApocalypseScheduleEnd[i].iMinute = -1;
    }

    m_iNpcConstructionPoint[1] = 100; // MS
    m_iNpcConstructionPoint[2] = 100; // MS
    m_iNpcConstructionPoint[3] = 100; // MS
    m_iNpcConstructionPoint[4] = 100; // MS
    m_iNpcConstructionPoint[5] = 100; // MS
    m_iNpcConstructionPoint[6] = 100; // MS

    m_iNpcConstructionPoint[43] = 1000; // LWB
    m_iNpcConstructionPoint[44] = 2000; // GHK
    m_iNpcConstructionPoint[45] = 3000; // GHKABS
    m_iNpcConstructionPoint[46] = 2000; // TK
    m_iNpcConstructionPoint[47] = 3000; // BG
    m_iNpcConstructionPoint[51] = 1500; // Catapult
    //

    m_bIsGameStarted = false;

    m_bIsItemAvailable = false;
    m_bIsBuildItemAvailable = false;
    m_bIsNpcAvailable = false;
    m_bIsMagicAvailable = false;
    m_bIsSkillAvailable = false;
    m_bIsQuestAvailable = false;
    m_bIsPortionAvailable = false;

    memset(m_cServerName, 0, sizeof(m_cServerName));

    for (i = 0; i < DEF_MAXCLIENTS; i++)
        m_pClientList[i] = 0;

    for (i = 0; i < DEF_MAXMAPS; i++)
        m_pMapList[i] = 0;

    for (i = 0; i < DEF_MAXITEMTYPES; i++)
        m_pItemConfigList[i] = 0;

    for (i = 0; i < DEF_MAXNPCTYPES; i++)
        m_pNpcConfigList[i] = 0;

    for (i = 0; i < DEF_MAXNPCS; i++)
        m_pNpcList[i] = 0;

    for (i = 0; i < DEF_MSGQUENESIZE; i++)
        m_pMsgQuene[i] = 0;

    for (i = 0; i < DEF_MAXMAGICTYPE; i++)
        m_pMagicConfigList[i] = 0;

    for (i = 0; i < DEF_MAXSKILLTYPE; i++)
        m_pSkillConfigList[i] = 0;

    for (i = 0; i < DEF_MAXQUESTTYPE; i++)
        m_pQuestConfigList[i] = 0;

    for (i = 0; i < DEF_MAXDYNAMICOBJECTS; i++)
        m_pDynamicObjectList[i] = 0;

    for (i = 0; i < DEF_MAXDELAYEVENTS; i++)
        m_pDelayEventList[i] = 0;

    for (i = 0; i < DEF_MAXNOTIFYMSGS; i++)
        m_pNoticeMsgList[i] = 0;

    for (i = 0; i < DEF_MAXFISHS; i++)
        m_pFish[i] = 0;

    for (i = 0; i < DEF_MAXMINERALS; i++)
        m_pMineral[i] = 0;

    for (i = 0; i < DEF_MAXPORTIONTYPES; i++)
        m_pPortionConfigList[i] = 0;

    //	for (i = 0; i < DEF_MAXTELEPORTTYPE; i++)
    //		m_pTeleportConfigList[i] = 0;

    for (i = 0; i < DEF_MAXSUBLOGSOCK; i++)
    {
        m_pSubLogSock[i] = 0;
        m_bIsSubLogSockAvailable[i] = false;
    }

    for (i = 0; i < DEF_MAXBUILDITEMS; i++)
        m_pBuildItemList[i] = 0;

    for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++)
    {
        memset(m_stCrusadeStructures[i].cMapName, 0, sizeof(m_stCrusadeStructures[i].cMapName));
        m_stCrusadeStructures[i].cType = 0;
        m_stCrusadeStructures[i].dX = 0;
        m_stCrusadeStructures[i].dY = 0;
    }

    for (i = 0; i < DEF_MAXADMINS; i++)
    {
        memset(m_stAdminList[i].m_cGMName, 0, sizeof(m_stAdminList[i].m_cGMName));
    }

    for (i = 0; i < DEF_MAXBANNED; i++)
    {
        memset(m_stBannedList[i].m_cBannedIPaddress, 0, sizeof(m_stBannedList[i].m_cBannedIPaddress));
    }

    for (i = 0; i < DEF_MAXGUILDS; i++)
        m_pGuildTeleportLoc[i].m_iV1 = 0;

    for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++)
    {
        m_stMiddleCrusadeStructureInfo[i].cType = 0;
        m_stMiddleCrusadeStructureInfo[i].cSide = 0;
        m_stMiddleCrusadeStructureInfo[i].sX = 0;
        m_stMiddleCrusadeStructureInfo[i].sY = 0;
    }
    m_iTotalMiddleCrusadeStructures = 0;

    m_pNoticementData = 0;

    m_iQueneHead = 0;
    m_iQueneTail = 0;

    m_iTotalClients = 0;
    m_iMaxClients = 0;
    m_iTotalMaps = 0;

    m_iTotalGameServerClients = 0;
    m_iTotalGameServerMaxClients = 0;

    m_MaxUserSysTime.wHour = 0;
    m_MaxUserSysTime.wMinute = 0;

    m_bIsServerShutdowned = false;
    m_cShutDownCode = 0;

    m_iMiddlelandMapIndex = -1;
    m_iAresdenMapIndex = -1;
    m_iElvineMapIndex = -1;
    m_iGodHMapIndex = -1;
    m_iBTFieldMapIndex = -1;

    m_iAresdenOccupyTiles = 0;
    m_iElvineOccupyTiles = 0;

    m_iSubLogSockInitIndex = 0;

    m_iCurMsgs = 0;
    m_iMaxMsgs = 0;

    m_stCityStatus[1].iCrimes = 0;
    m_stCityStatus[1].iFunds = 0;
    m_stCityStatus[1].iWins = 0;

    m_stCityStatus[2].iCrimes = 0;
    m_stCityStatus[2].iFunds = 0;
    m_stCityStatus[2].iWins = 0;

    m_iStrategicStatus = 0;

    m_iCollectedMana[0] = 0;
    m_iCollectedMana[1] = 0;
    m_iCollectedMana[2] = 0;

    m_iAresdenMana = 0;
    m_iElvineMana = 0;

    m_dwSpecialEventTime = m_dwWhetherTime = m_dwGameTime1 =
        m_dwGameTime2 = m_dwGameTime3 = m_dwGameTime4 = m_dwGameTime5 = m_dwGameTime6 = m_dwFishTime = dwTime;

    m_bIsSpecialEventTime = false;

    GetLocalTime(&SysTime);
    m_dwCanFightzoneReserveTime = dwTime - ((SysTime.wHour % 2) * 60 * 60 + SysTime.wMinute * 60) * 1000;

    for (i = 0; i < DEF_MAXFIGHTZONE; i++)
        m_iFightZoneReserve[i] = 0;

    m_iFightzoneNoForceRecall = 0;

    for (i = 1; i < 200; i++)
    {
        m_iLevelExpTable[i] = iGetLevelExp(i);
        //log->info("Level:{} --- Exp:{}", i, m_iLevelExpTable[i]);
    }

    m_iLimitedUserExp = m_iLevelExpTable[DEF_LEVELLIMIT + 1];
    m_iLevelExp20 = m_iLevelExpTable[20];

    m_iGameServerMode = 0;
    if (bReadProgramConfigFile("GServer.cfg") == false)
    {
        log->critical("(!!!) CRITICAL ERROR! Cannot execute server! GServer.cfg file contents error!");
        return false;
    }
    if (bReadSettingsConfigFile("..\\GameConfigs\\Settings.cfg") == false)
    {
        ;
        log->critical("(!!!) CRITICAL ERROR! Cannot execute server! Settings.cfg file contents error!");
        return false;
    }
    if (bReadAdminListConfigFile("..\\GameConfigs\\AdminList.cfg") == false)
    {
        ;
        log->critical("(!!!) CRITICAL ERROR! Cannot execute server! AdminList.cfg file contents error!");
        return false;
    }
    if (bReadBannedListConfigFile("..\\GameConfigs\\BannedList.cfg") == false)
    {
        ;
        log->critical("(!!!) CRITICAL ERROR! Cannot execute server! BannedList.cfg file contents error!");
        return false;
    }
    if (bReadAdminSetConfigFile("..\\GameConfigs\\AdminSettings.cfg") == false)
    {
        ;
        log->critical("(!!!) CRITICAL ERROR! Cannot execute server! AdminSettings.cfg file contents error!");
        return false;
    }
    srand((unsigned)time(0));

    m_bF1pressed = m_bF4pressed = m_bF12pressed = m_bF5pressed = false;

    m_bOnExitProcess = false;

    for (i = 0; i <= 100; i++)
    {
        m_iSkillSSNpoint[i] = _iCalcSkillSSNpoint(i);
    }

    GetLocalTime(&SysTime);
    if (SysTime.wMinute >= DEF_NIGHTTIME)
        m_cDayOrNight = 2;
    else m_cDayOrNight = 1;

    bReadNotifyMsgListFile("notice.txt");
    m_dwNoticeTime = dwTime;

    m_pNoticementData = 0;
    m_dwNoticementDataSize = 0;

    m_dwMapSectorInfoTime = dwTime;
    m_iMapSectorInfoUpdateCount = 0;

    m_iCrusadeCount = 0;
    m_bIsCrusadeMode = false;
    m_bIsApocalypseMode = false;
    m_wServerID_GSS = iDice(1, 65535);

    memset(m_cGateServerStockMsg, 0, sizeof(m_cGateServerStockMsg));
    cp = (char *)m_cGateServerStockMsg;
    dwp = (uint32_t *)cp;
    *dwp = MSGID_SERVERSTOCKMSG;
    cp += 4;
    wp = (uint16_t *)cp;
    *wp = DEF_MSGTYPE_CONFIRM;
    cp += 2;

    m_iIndexGSS = 6;
    m_dwCrusadeGUID = 0;
    m_iCrusadeWinnerSide = 0;
    m_iLastCrusadeWinner = 0;
    m_sLastHeldenianWinner = 0;
    m_sLastCrusadeDate = -1;
    m_iFinalShutdownCount = 0;
    m_bIsCrusadeWarStarter = false;
    m_bIsApocalypseStarter = false;
    m_iLatestCrusadeDayOfWeek = -1;

    m_bHeldenianInitiated = false;
    m_cHeldenianType = false;
    m_bIsHeldenianMode = false;
    m_bHeldenianRunning = false;
    m_iHeldenianAresdenLeftTower = 0;
    m_cHeldenianModeType = -1;
    m_sLastHeldenianWinner = -1;
    m_iHeldenianAresdenLeftTower = 0;
    m_iHeldenianElvineLeftTower = 0;
    m_iHeldenianAresdenDead = 0;
    m_iHeldenianElvineDead = 0;

    return true;
}

void CGame::OnClientRead(int iClientH)
{
    char * pData, cKey;
    uint32_t  dwMsgSize;

    if (m_pClientList[iClientH] == nullptr) return;

    pData = m_pClientList[iClientH]->m_pXSock->pGetRcvDataPointer(&dwMsgSize, &cKey); // v1.4

    if (bPutMsgQuene(DEF_MSGFROM_CLIENT, pData, dwMsgSize, iClientH, cKey) == false)
    {
        log->info("@@@@@@ CRITICAL ERROR in MsgQuene!!! @@@@@@");
    }
}

int CGame::iComposeInitMapData(short sX, short sY, int iClientH, char * pData)
{
    int * ip, ix, iy, iSize, iTileExists;
    class CTile * pTileSrc, * pTile;
    unsigned char ucHeader;
    short * sp, * pTotal;
    int     sTemp, sTemp2;
    uint16_t * wp;
    char * cp;

    if (m_pClientList[iClientH] == nullptr) return 0;

    pTotal = (short *)pData;
    cp = (char *)(pData + 2);

    iSize = 2;
    iTileExists = 0;
    pTileSrc = (class CTile *)(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_pTile +
        (sX)+(sY)*m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_sSizeY);

    for (iy = 0; iy < 16; iy++)
        for (ix = 0; ix < 21; ix++)
        {

            if (((sX + ix) == 100) && ((sY + iy) == 100))
                sX = sX;

            pTile = (class CTile *)(pTileSrc + ix + iy * m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_sSizeY);

            //If player not same side and is invied (Beholder Hack)
            if ((m_pClientList[pTile->m_sOwner] != 0) && (pTile->m_sOwner != iClientH))
                if ((m_pClientList[pTile->m_sOwner]->m_cSide != 0) &&
                    (m_pClientList[pTile->m_sOwner]->m_cSide != m_pClientList[iClientH]->m_cSide) &&
                    ((m_pClientList[pTile->m_sOwner]->m_iStatus & 0x00000010) != 0))
                {
                    continue;
                }

            if ((pTile->m_sOwner != 0) || (pTile->m_sDeadOwner != 0) ||
                (pTile->m_pItem[0] != 0) || (pTile->m_sDynamicObjectType != 0))
            {
                iTileExists++;
                sp = (short *)cp;
                *sp = ix;
                cp += 2;
                sp = (short *)cp;
                *sp = iy;
                cp += 2;
                iSize += 4;

                ucHeader = 0;
                if (pTile->m_sOwner != 0)
                {
                    if (pTile->m_cOwnerClass == DEF_OWNERTYPE_PLAYER)
                    {
                        if (m_pClientList[pTile->m_sOwner] != 0) ucHeader = ucHeader | 0x01;
                        else
                        {
                            log->info("Empty player handle: {}", pTile->m_sOwner);
                            pTile->m_sOwner = 0;
                        }
                    }

                    if (pTile->m_cOwnerClass == DEF_OWNERTYPE_NPC)
                    {
                        if (m_pNpcList[pTile->m_sOwner] != 0) ucHeader = ucHeader | 0x01;
                        else pTile->m_sOwner = 0;
                    }
                }
                if (pTile->m_sDeadOwner != 0)
                {
                    if (pTile->m_cDeadOwnerClass == DEF_OWNERTYPE_PLAYER)
                    {
                        if (m_pClientList[pTile->m_sDeadOwner] != 0) ucHeader = ucHeader | 0x02;
                        else pTile->m_sDeadOwner = 0;
                    }
                    if (pTile->m_cDeadOwnerClass == DEF_OWNERTYPE_NPC)
                    {
                        if (m_pNpcList[pTile->m_sDeadOwner] != 0) ucHeader = ucHeader | 0x02;
                        else pTile->m_sDeadOwner = 0;
                    }
                }
                if (pTile->m_pItem[0] != 0)				ucHeader = ucHeader | 0x04;
                if (pTile->m_sDynamicObjectType != 0)    ucHeader = ucHeader | 0x08;
                //
                *cp = ucHeader;
                cp++;
                iSize++;

                if ((ucHeader & 0x01) != 0)
                {
                    switch (pTile->m_cOwnerClass)
                    {
                        case DEF_OWNERTYPE_PLAYER:
                            // Object ID number(Player) : 1~10000
                            sp = (short *)cp;
                            *sp = pTile->m_sOwner;
                            cp += 2;
                            iSize += 2;
                            // object type
                            sp = (short *)cp;
                            *sp = m_pClientList[pTile->m_sOwner]->m_sType;
                            cp += 2;
                            iSize += 2;
                            // dir
                            *cp = m_pClientList[pTile->m_sOwner]->m_cDir;
                            cp++;
                            iSize++;
                            // Appearance1
                            sp = (short *)cp;
                            *sp = m_pClientList[pTile->m_sOwner]->m_sAppr1;
                            cp += 2;
                            iSize += 2;
                            // Appearance2
                            sp = (short *)cp;
                            *sp = m_pClientList[pTile->m_sOwner]->m_sAppr2;
                            cp += 2;
                            iSize += 2;
                            // Appearance3
                            sp = (short *)cp;
                            *sp = m_pClientList[pTile->m_sOwner]->m_sAppr3;
                            cp += 2;
                            iSize += 2;
                            // Appearance4
                            sp = (short *)cp;
                            *sp = m_pClientList[pTile->m_sOwner]->m_sAppr4;
                            cp += 2;
                            iSize += 2;
                            // v1.4 ApprColor
                            ip = (int *)cp;
                            *ip = m_pClientList[pTile->m_sOwner]->m_iApprColor;
                            cp += 4;
                            iSize += 4;

                            // Status
                            ip = (int *)cp;

                            // sStatus
                            sTemp = m_pClientList[pTile->m_sOwner]->m_iStatus;
                            sTemp = 0x0FFFFFFF & sTemp;//Original : sTemp = 0x0FFF & sTemp;
                            sTemp2 = iGetPlayerABSStatus(pTile->m_sOwner, iClientH); //(short)iGetPlayerRelationship(iClientH, pTile->m_sOwner);
                            sTemp = (sTemp | (sTemp2 << 28));//Original : 12
                            *ip = sTemp;
                            //*sp = DEF_TEST;
                            cp += 4;//Original 2
                            iSize += 4;//Original 2
                            // Name
                            memcpy(cp, m_pClientList[pTile->m_sOwner]->m_cCharName, 10);
                            cp += 10;
                            iSize += 10;
                            break;

                        case DEF_OWNERTYPE_NPC:
                            // Object ID number(NPC) : 10000~
                            sp = (short *)cp;
                            *sp = pTile->m_sOwner + 10000;
                            cp += 2;
                            iSize += 2;
                            // object type
                            sp = (short *)cp;
                            *sp = m_pNpcList[pTile->m_sOwner]->m_sType;
                            cp += 2;
                            iSize += 2;
                            // dir
                            *cp = m_pNpcList[pTile->m_sOwner]->m_cDir;
                            cp++;
                            iSize++;
                            // Appr2
                            sp = (short *)cp;
                            *sp = m_pNpcList[pTile->m_sOwner]->m_sAppr2;
                            cp += 2;
                            iSize += 2;
                            // Status
                            ip = (int *)cp;

                            sTemp = m_pNpcList[pTile->m_sOwner]->m_iStatus;
                            sTemp = 0x0FFFFFFF & sTemp;//Original : sTemp = 0x0FFF & sTemp; // Â»Ã³Ã€Â§ 4ÂºÃ±Ã†Â® Ã…Â¬Â¸Â®Â¾Ã®
                            sTemp2 = iGetNpcRelationship(pTile->m_sOwner, iClientH);
                            sTemp = (sTemp | (sTemp2 << 28));//Original : 12
                            *ip = sTemp;
                            //*sp = DEF_TEST;
                            cp += 4;//Original 2
                            iSize += 4;//Original 2
                            // Name
                            memcpy(cp, m_pNpcList[pTile->m_sOwner]->m_cName, 5);
                            cp += 5;
                            iSize += 5;
                            break;
                    }
                }

                if ((ucHeader & 0x02) != 0)
                {
                    switch (pTile->m_cDeadOwnerClass)
                    {
                        case DEF_OWNERTYPE_PLAYER:
                            // Object ID number : 1~10000
                            sp = (short *)cp;
                            *sp = pTile->m_sDeadOwner;
                            cp += 2;
                            iSize += 2;
                            // object type
                            sp = (short *)cp;
                            *sp = m_pClientList[pTile->m_sDeadOwner]->m_sType;
                            cp += 2;
                            iSize += 2;
                            // dir
                            *cp = m_pClientList[pTile->m_sDeadOwner]->m_cDir;
                            cp++;
                            iSize++;
                            // Appearance1
                            sp = (short *)cp;
                            *sp = m_pClientList[pTile->m_sDeadOwner]->m_sAppr1;
                            cp += 2;
                            iSize += 2;
                            // Appearance2
                            sp = (short *)cp;
                            *sp = m_pClientList[pTile->m_sDeadOwner]->m_sAppr2;
                            cp += 2;
                            iSize += 2;
                            // Appearance3
                            sp = (short *)cp;
                            *sp = m_pClientList[pTile->m_sDeadOwner]->m_sAppr3;
                            cp += 2;
                            iSize += 2;
                            // Appearance4
                            sp = (short *)cp;
                            *sp = m_pClientList[pTile->m_sDeadOwner]->m_sAppr4;
                            cp += 2;
                            iSize += 2;
                            // v1.4 ApprColor
                            ip = (int *)cp;
                            *ip = m_pClientList[pTile->m_sDeadOwner]->m_iApprColor;
                            cp += 4;
                            iSize += 4;

                            // Status
                            ip = (int *)cp;

                            // sStatus
                            sTemp = m_pClientList[pTile->m_sDeadOwner]->m_iStatus;
                            sTemp = 0x0FFFFFFF & sTemp;//Original : sTemp = 0x0FFF & sTemp;
                            sTemp2 = iGetPlayerABSStatus(pTile->m_sDeadOwner, iClientH); //(short)iGetPlayerRelationship(iClientH, pTile->m_sDeadOwner);
                            sTemp = (sTemp | (sTemp2 << 28));//Original : 12
                            *ip = sTemp;
                            //*sp = DEF_TEST;
                            cp += 4;//Original 2
                            iSize += 4;//Original 2
                            // Name
                            memcpy(cp, m_pClientList[pTile->m_sDeadOwner]->m_cCharName, 10);
                            cp += 10;
                            iSize += 10;
                            break;

                        case DEF_OWNERTYPE_NPC:
                            // Object ID number : 10000	~
                            sp = (short *)cp;
                            *sp = pTile->m_sDeadOwner + 10000;
                            cp += 2;
                            iSize += 2;
                            // object type
                            sp = (short *)cp;
                            *sp = m_pNpcList[pTile->m_sDeadOwner]->m_sType;
                            cp += 2;
                            iSize += 2;
                            // dir
                            *cp = m_pNpcList[pTile->m_sDeadOwner]->m_cDir;
                            cp++;
                            iSize++;
                            // Appr2
                            sp = (short *)cp;
                            *sp = m_pNpcList[pTile->m_sDeadOwner]->m_sAppr2;
                            cp += 2;
                            iSize += 2;
                            // Status
                            ip = (int *)cp;

                            sTemp = m_pNpcList[pTile->m_sDeadOwner]->m_iStatus;
                            sTemp = 0x0FFFFFFF & sTemp;//Original : sTemp = 0x0FFF & sTemp; // Â»Ã³Ã€Â§ 4ÂºÃ±Ã†Â® Ã…Â¬Â¸Â®Â¾Ã®
                            sTemp2 = iGetNpcRelationship(pTile->m_sDeadOwner, iClientH);
                            sTemp = (sTemp | (sTemp2 << 28));//Original : 12
                            *ip = sTemp;
                            //*sp = DEF_TEST;
                            cp += 4;//Original 2
                            iSize += 4;//Original 2
                            // Name
                            memcpy(cp, m_pNpcList[pTile->m_sDeadOwner]->m_cName, 5);
                            cp += 5;
                            iSize += 5;
                            break;
                    }
                }

                if (pTile->m_pItem[0] != 0)
                {
                    sp = (short *)cp;
                    *sp = pTile->m_pItem[0]->m_sSprite;
                    cp += 2;
                    iSize += 2;
                    sp = (short *)cp;
                    *sp = pTile->m_pItem[0]->m_sSpriteFrame;
                    cp += 2;
                    iSize += 2;
                    *cp = pTile->m_pItem[0]->m_cItemColor;
                    cp++;
                    iSize++;
                }

                if (pTile->m_sDynamicObjectType != 0)
                {
                    wp = (uint16_t *)cp;
                    *wp = pTile->m_wDynamicObjectID;
                    cp += 2;
                    iSize += 2;

                    sp = (short *)cp;
                    *sp = pTile->m_sDynamicObjectType;
                    cp += 2;
                    iSize += 2;
                }
            } // Big if
        } // while(1)

    *pTotal = iTileExists;
    return iSize;
}

void CGame::DeleteClient(int iClientH, bool bSave, bool bNotify, bool bCountLogout, bool bForceCloseConn)
{
    int i, iExH;
    char * cp, cData[120], cTmpMap[30];
    uint32_t * dwp;
    uint16_t * wp;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == true)
    {
        if (memcmp(m_pClientList[iClientH]->m_cMapName, "fight", 5) == 0)
        {
            wsprintf(G_cTxt, "Char(%s)-Exit(%s)", m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_cMapName);
            PutLogEventFileList(G_cTxt);
        }

        if (m_pClientList[iClientH]->m_bIsExchangeMode == true)
        {
            iExH = m_pClientList[iClientH]->m_iExchangeH;
            _ClearExchangeStatus(iExH);
            _ClearExchangeStatus(iClientH);
        }

        if ((m_pClientList[iClientH]->m_iAllocatedFish != 0) && (m_pFish[m_pClientList[iClientH]->m_iAllocatedFish] != 0))
            m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_sEngagingCount--;

        if (bNotify == true)
            SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_LOG, DEF_MSGTYPE_REJECT, 0, 0, 0);

        RemoveFromTarget(iClientH, DEF_OWNERTYPE_PLAYER);

        for (i = 1; i < DEF_MAXCLIENTS; i++)
            if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_iWhisperPlayerIndex == iClientH))
            {
                m_pClientList[i]->m_iWhisperPlayerIndex = -1;
                SendNotifyMsg(0, i, DEF_NOTIFY_WHISPERMODEOFF, 0, 0, 0, m_pClientList[iClientH]->m_cCharName);
            }

        memset(cData, 0, sizeof(cData));
        cp = (char *)cData;
        *cp = GSM_DISCONNECT;
        cp++;
        memcpy(cp, m_pClientList[iClientH]->m_cCharName, 10);
        cp += 10;
        bStockMsgToGateServer(cData, 11);

        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->ClearOwner(2, iClientH, DEF_OWNERTYPE_PLAYER,
            m_pClientList[iClientH]->m_sX,
            m_pClientList[iClientH]->m_sY);

        bRemoveFromDelayEventList(iClientH, DEF_OWNERTYPE_PLAYER, 0);
    }

    if ((bSave == true) && (m_pClientList[iClientH]->m_bIsOnServerChange == false))
    {

        if (m_pClientList[iClientH]->m_bIsKilled == true)
        {
            m_pClientList[iClientH]->m_sX = -1;
            m_pClientList[iClientH]->m_sY = -1;

            strcpy(cTmpMap, m_pClientList[iClientH]->m_cMapName);

            memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof(m_pClientList[iClientH]->m_cMapName));

            if (m_pClientList[iClientH]->m_cSide == 0)
            {
                strcpy(m_pClientList[iClientH]->m_cMapName, "default");
            }
            else
            {
                if (memcmp(m_pClientList[iClientH]->m_cLocation, "are", 3) == 0)
                {
                    if (m_bIsCrusadeMode == true)
                    {
                        if (m_pClientList[iClientH]->m_iDeadPenaltyTime > 0)
                        {
                            memset(m_pClientList[iClientH]->m_cLockedMapName, 0, sizeof(m_pClientList[iClientH]->m_cLockedMapName));
                            strcpy(m_pClientList[iClientH]->m_cLockedMapName, "aresden");
                            m_pClientList[iClientH]->m_iLockedMapTime = 60 * 5;
                            m_pClientList[iClientH]->m_iDeadPenaltyTime = 60 * 10;
                        }
                        else
                        {
                            m_pClientList[iClientH]->m_iDeadPenaltyTime = 60 * 10;
                        }
                    }

                    if (strcmp(cTmpMap, "elvine") == 0)
                    {
                        strcpy(m_pClientList[iClientH]->m_cLockedMapName, "elvjail");
                        m_pClientList[iClientH]->m_iLockedMapTime = 60 * 3;
                        memcpy(m_pClientList[iClientH]->m_cMapName, "elvjail", 7);
                    }
                    else if (m_pClientList[iClientH]->m_iLevel > 80)
                        memcpy(m_pClientList[iClientH]->m_cMapName, "resurr1", 7);
                    else memcpy(m_pClientList[iClientH]->m_cMapName, "arefarm", 7);
                }
                else
                {
                    if (m_bIsCrusadeMode == true)
                    {
                        if (m_pClientList[iClientH]->m_iDeadPenaltyTime > 0)
                        {
                            memset(m_pClientList[iClientH]->m_cLockedMapName, 0, sizeof(m_pClientList[iClientH]->m_cLockedMapName));
                            strcpy(m_pClientList[iClientH]->m_cLockedMapName, "elvine");
                            m_pClientList[iClientH]->m_iLockedMapTime = 60 * 5;
                            m_pClientList[iClientH]->m_iDeadPenaltyTime = 60 * 10;
                        }
                        else
                        {
                            m_pClientList[iClientH]->m_iDeadPenaltyTime = 60 * 10;
                        }
                    }
                    if (strcmp(cTmpMap, "aresden") == 0)
                    {
                        strcpy(m_pClientList[iClientH]->m_cLockedMapName, "arejail");
                        m_pClientList[iClientH]->m_iLockedMapTime = 60 * 3;
                        memcpy(m_pClientList[iClientH]->m_cMapName, "arejail", 7);

                    }
                    else if (m_pClientList[iClientH]->m_iLevel > 80)
                        memcpy(m_pClientList[iClientH]->m_cMapName, "resurr2", 7);
                    else memcpy(m_pClientList[iClientH]->m_cMapName, "elvfarm", 7);
                }
            }
        }
        else if (bForceCloseConn == true)
        {
            memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof(m_pClientList[iClientH]->m_cMapName));
            memcpy(m_pClientList[iClientH]->m_cMapName, "bisle", 5);
            m_pClientList[iClientH]->m_sX = -1;
            m_pClientList[iClientH]->m_sY = -1;

            memset(m_pClientList[iClientH]->m_cLockedMapName, 0, sizeof(m_pClientList[iClientH]->m_cLockedMapName));
            strcpy(m_pClientList[iClientH]->m_cLockedMapName, "bisle");
            m_pClientList[iClientH]->m_iLockedMapTime = 10 * 60;
        }

        if (m_pClientList[iClientH]->m_bIsObserverMode == true)
        {
            memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof(m_pClientList[iClientH]->m_cMapName));
            if (m_pClientList[iClientH]->m_cSide == 0)
            {
                switch (iDice(1, 2))
                {
                    case 1:
                        memcpy(m_pClientList[iClientH]->m_cMapName, "aresden", 7);
                        break;
                    case 2:
                        memcpy(m_pClientList[iClientH]->m_cMapName, "elvine", 6);
                        break;
                }
            }
            else
            {
                memcpy(m_pClientList[iClientH]->m_cMapName, m_pClientList[iClientH]->m_cLocation, 10);
            }
            m_pClientList[iClientH]->m_sX = -1;
            m_pClientList[iClientH]->m_sY = -1;
        }

        if (memcmp(m_pClientList[iClientH]->m_cMapName, "fight", 5) == 0)
        {
            memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof(m_pClientList[iClientH]->m_cMapName));
            if (m_pClientList[iClientH]->m_cSide == 0)
            {
                switch (iDice(1, 2))
                {
                    case 1:
                        memcpy(m_pClientList[iClientH]->m_cMapName, "aresden", 7);
                        break;
                    case 2:
                        memcpy(m_pClientList[iClientH]->m_cMapName, "elvine", 6);
                        break;
                }
            }
            else
            {
                memcpy(m_pClientList[iClientH]->m_cMapName, m_pClientList[iClientH]->m_cLocation, 10);
            }
            m_pClientList[iClientH]->m_sX = -1;
            m_pClientList[iClientH]->m_sY = -1;
        }

        if (m_pClientList[iClientH]->m_bIsInitComplete == true)
        {
            if (m_pClientList[iClientH]->m_iPartyID != 0)
            {
                memset(cData, 0, sizeof(cData));
                cp = (char *)cData;

                dwp = (uint32_t *)cp;
                *dwp = MSGID_PARTYOPERATION;
                cp += 4;

                wp = (uint16_t *)cp;
                *wp = 4;
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
            if (bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATALOGOUT, iClientH, bCountLogout) == false) LocalSavePlayerData(iClientH);
        }
        else bSendMsgToLS(MSGID_REQUEST_NOSAVELOGOUT, iClientH, bCountLogout);
    }
    else
    {
        if (m_pClientList[iClientH]->m_bIsOnServerChange == false)
        {
            if (m_pClientList[iClientH]->m_iPartyID != 0)
            {
                memset(cData, 0, sizeof(cData));
                cp = (char *)cData;

                dwp = (uint32_t *)cp;
                *dwp = MSGID_PARTYOPERATION;
                cp += 4;

                wp = (uint16_t *)cp;
                *wp = 4;
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

            bSendMsgToLS(MSGID_REQUEST_NOSAVELOGOUT, iClientH, bCountLogout);
        }
        else
        {
            if (m_pClientList[iClientH]->m_iPartyID != 0)
            {
                memset(cData, 0, sizeof(cData));
                cp = (char *)cData;

                dwp = (uint32_t *)cp;
                *dwp = MSGID_PARTYOPERATION;
                cp += 4;

                wp = (uint16_t *)cp;
                *wp = 7;
                cp += 2;

                wp = (uint16_t *)cp;
                *wp = 0;
                cp += 2;

                memcpy(cp, m_pClientList[iClientH]->m_cCharName, 10);
                cp += 10;

                wp = (uint16_t *)cp;
                *wp = m_pClientList[iClientH]->m_iPartyID;
                cp += 2;

                SendMsgToGateServer(MSGID_PARTYOPERATION, iClientH, cData);
            }

            bSendMsgToLS(MSGID_REQUEST_SETACCOUNTWAITSTATUS, iClientH, false);
        }
    }

    if (m_pClientList[iClientH]->m_iPartyID != 0)
    {
        for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
            if (m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] == iClientH)
            {
                m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] = 0;
                m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers--;
                m_pClientList[iClientH]->m_iPartyID = 0;
                m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
                m_pClientList[iClientH]->m_iReqJoinPartyClientH = 0;
                wsprintf(G_cTxt, "PartyID:%d member:%d Out(Delete) Total:%d", m_pClientList[iClientH]->m_iPartyID, iClientH, m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers);
                log->info(G_cTxt);
                goto DC_LOOPBREAK1;
            }
        DC_LOOPBREAK1:;
        for (i = 0; i < DEF_MAXPARTYMEMBERS - 1; i++)
            if ((m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] == 0) && (m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i + 1] != 0))
            {
                m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] = m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i + 1];
                m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i + 1] = 0;
            }
    }

    m_iTotalClients--;

    delete m_pClientList[iClientH];
    m_pClientList[iClientH] = 0;

    RemoveClientShortCut(iClientH);
}

void CGame::SendEventToNearClient_TypeA(short sOwnerH, char cOwnerType, uint32_t dwMsgID, uint16_t wMsgType, short sV1, short sV2, short sV3)
{
    int * ip, i, iRet, iShortCutIndex;
    char * cp_a, * cp_s, * cp_sv, cData_All[200], cData_Srt[200], cData_Srt_Av[200];
    uint32_t * dwp;
    uint16_t * wp;
    int * ipStatus, iDumm;
    short * sp, sRange, sX, sY;
    bool  bFlag, cOwnerSend;
    char cKey;
    int iTemp3, iTemp, iTemp2;

    memset(cData_All, 0, sizeof(cData_All));
    memset(cData_Srt, 0, sizeof(cData_Srt));
    memset(cData_Srt_Av, 0, sizeof(cData_Srt_Av));
    ipStatus = (int *)&iDumm;
    cKey = (rand() % 255) + 1;

    dwp = (uint32_t *)(cData_All + DEF_INDEX4_MSGID);
    *dwp = dwMsgID;
    wp = (uint16_t *)(cData_All + DEF_INDEX2_MSGTYPE);
    *wp = wMsgType;

    dwp = (uint32_t *)(cData_Srt + DEF_INDEX4_MSGID);
    *dwp = dwMsgID;
    wp = (uint16_t *)(cData_Srt + DEF_INDEX2_MSGTYPE);
    *wp = wMsgType;

    dwp = (uint32_t *)(cData_Srt_Av + DEF_INDEX4_MSGID);
    *dwp = dwMsgID;
    wp = (uint16_t *)(cData_Srt_Av + DEF_INDEX2_MSGTYPE);
    *wp = wMsgType;

    cp_a = (char *)(cData_All + DEF_INDEX2_MSGTYPE + 2);
    cp_s = (char *)(cData_Srt + DEF_INDEX2_MSGTYPE + 2);
    cp_sv = (char *)(cData_Srt_Av + DEF_INDEX2_MSGTYPE + 2);


    if ((dwMsgID == MSGID_EVENT_LOG) || (wMsgType == DEF_OBJECTMOVE) || (wMsgType == DEF_OBJECTRUN) ||
        (wMsgType == DEF_OBJECTATTACKMOVE) || (wMsgType == DEF_OBJECTDAMAGEMOVE) || (wMsgType == DEF_OBJECTDYING))
        sRange = 1;
    else sRange = 0;


    if (cOwnerType == DEF_OWNERTYPE_PLAYER)
    {
        if (m_pClientList[sOwnerH] == 0) return;

        switch (wMsgType)
        {
            case DEF_OBJECTNULLACTION:
            case DEF_OBJECTDAMAGE:
            case DEF_OBJECTDYING:
            case DEF_MSGTYPE_CONFIRM:
                cOwnerSend = true;
                break;
            default:
                cOwnerSend = false;
                break;
        }

        wp = (uint16_t *)cp_a;
        *wp = sOwnerH;
        cp_a += 2;

        sp = (short *)cp_a;
        sX = m_pClientList[sOwnerH]->m_sX;
        *sp = sX;
        cp_a += 2;

        sp = (short *)cp_a;
        sY = m_pClientList[sOwnerH]->m_sY;
        *sp = sY;
        cp_a += 2;

        sp = (short *)cp_a;
        *sp = m_pClientList[sOwnerH]->m_sType;
        cp_a += 2;

        *cp_a = m_pClientList[sOwnerH]->m_cDir;
        cp_a++;

        memcpy(cp_a, m_pClientList[sOwnerH]->m_cCharName, 10);
        cp_a += 10;

        sp = (short *)cp_a;
        *sp = m_pClientList[sOwnerH]->m_sAppr1;
        cp_a += 2;

        sp = (short *)cp_a;
        *sp = m_pClientList[sOwnerH]->m_sAppr2;
        cp_a += 2;

        sp = (short *)cp_a;
        *sp = m_pClientList[sOwnerH]->m_sAppr3;
        cp_a += 2;

        sp = (short *)cp_a;
        *sp = m_pClientList[sOwnerH]->m_sAppr4;
        cp_a += 2;

        ip = (int *)cp_a;
        *ip = m_pClientList[sOwnerH]->m_iApprColor;
        cp_a += 4;

        ip = (int *)cp_a;
        ipStatus = ip;
        *ip = m_pClientList[sOwnerH]->m_iStatus;
        cp_a += 4;

        //iTemp += m_pClientList[sOwnerH]->m_iStatus & 0x0F0;
        iTemp3 = m_pClientList[sOwnerH]->m_iStatus & 0x0F0FFFF7F; //0F0FFFF7Fh

        if (wMsgType == DEF_OBJECTNULLACTION)
        {
            if (m_pClientList[sOwnerH]->m_bIsKilled == true)
                *cp_a = 1;
            else *cp_a = 0;
        }
        else *cp_a = 0;
        cp_a++;

        wp = (uint16_t *)cp_s;
        *wp = sOwnerH + 30000;
        cp_s += 2;

        *cp_s = m_pClientList[sOwnerH]->m_cDir;
        cp_s++;

        *cp_s = (unsigned char)sV1;
        cp_s++;

        *cp_s = (unsigned char)sV2;
        cp_s++;

        sp = (short *)cp_s;
        sX = m_pClientList[sOwnerH]->m_sX;
        *sp = sX;
        cp_s += 2;

        sp = (short *)cp_s;
        sY = m_pClientList[sOwnerH]->m_sY;
        *sp = sY;
        cp_s += 2;

        wp = (uint16_t *)cp_sv;
        *wp = sOwnerH + 30000;
        cp_sv += 2;

        *cp_sv = m_pClientList[sOwnerH]->m_cDir;
        cp_sv++;

        *cp_sv = sV1 - sX;
        cp_sv++;

        *cp_sv = sV2 - sY;
        cp_sv++;

        sp = (short *)cp_sv;
        *sp = sV3;
        cp_sv += 2;

        bFlag = true;
        iShortCutIndex = 0;

        while (bFlag)
        {
            i = m_iClientShortCut[iShortCutIndex];
            iShortCutIndex++;
            if (i == 0) bFlag = false;

            if ((bFlag == true) && (m_pClientList[i] != 0) && (m_pClientList[i]->m_bIsInitComplete == true))

                if ((m_pClientList[i]->m_cMapIndex == m_pClientList[sOwnerH]->m_cMapIndex) &&
                    (m_pClientList[i]->m_sX >= m_pClientList[sOwnerH]->m_sX - 10 - sRange) &&
                    (m_pClientList[i]->m_sX <= m_pClientList[sOwnerH]->m_sX + 10 + sRange) &&
                    (m_pClientList[i]->m_sY >= m_pClientList[sOwnerH]->m_sY - 8 - sRange) &&
                    (m_pClientList[i]->m_sY <= m_pClientList[sOwnerH]->m_sY + 8 + sRange))
                {

                    /*//If player not same side and is invied (Beholder Hack)
                    if (m_pClientList[sOwnerH] != 0 && i != sOwnerH)
                        if ((m_pClientList[i]->m_cSide != m_pClientList[sOwnerH]->m_cSide) &&
                            ((m_pClientList[sOwnerH]->m_iStatus & 0x00000010) != 0)) {
                            if(wMsgType == DEF_OBJECTRUN || wMsgType == DEF_OBJECTMOVE || wMsgType == DEF_OBJECTSTOP || wMsgType == DEF_OBJECTGETITEM)
                                if we filter attacking
                                [KLKS] - http://xtremehb.com -> Play Abaddon (PRO ARESDEN) says:
                                the poor guy who's being attacked wont know what hit him
                        continue;
                    }*/

                    /*iTemp = *ipStatus;
                    iTemp = 0x0FFFFFFF & iTemp; // »óÀ§ 4ºñÆ® Å¬¸®¾î
                    iTemp2 = (short)iGetPlayerABSStatus(sOwnerH, i); //(short)iGetPlayerRelationship_SendEvent(sOwnerH, i);
                    iTemp  = (iTemp | (iTemp2 << 28));
                    *ipStatus = iTemp;*/

                    if (m_pClientList[sOwnerH]->m_cSide != m_pClientList[i]->m_cSide)
                    {
                        if (m_pClientList[i]->m_iAdminUserLevel > 0)
                        {	//Â¾Ã®ÂµÃ¥Â¹ÃŽÂ¿Â¡Â°Ã”Â´Ã‚ Â¹Â«ÃÂ¶Â°Ã‡ ÂºÂ¸Â³Â½Â´Ã™.
                            iTemp = m_pClientList[sOwnerH]->m_iStatus;
                        }
                        else if (i != sOwnerH)
                        {
                            iTemp = iTemp3;
                        }
                        else
                        {
                            iTemp = m_pClientList[sOwnerH]->m_iStatus;
                        }
                    }
                    else
                    {
                        iTemp = m_pClientList[sOwnerH]->m_iStatus;
                        //iTemp = 0x0FFFFFFF;
                    }

                    //iTemp = m_pClientList[sOwnerH]->m_iStatus;
                    iTemp = 0x0FFFFFFF & iTemp;
                    iTemp2 = iGetPlayerABSStatus(sOwnerH, i);
                    iTemp = (iTemp | (iTemp2 << 28));
                    *ipStatus = iTemp;

                    if ((m_pClientList[i]->m_sX >= m_pClientList[sOwnerH]->m_sX - 9) &&
                        (m_pClientList[i]->m_sX <= m_pClientList[sOwnerH]->m_sX + 9) &&
                        (m_pClientList[i]->m_sY >= m_pClientList[sOwnerH]->m_sY - 7) &&
                        (m_pClientList[i]->m_sY <= m_pClientList[sOwnerH]->m_sY + 7))
                    {

                        switch (wMsgType)
                        {
                            case DEF_MSGTYPE_CONFIRM:
                            case DEF_MSGTYPE_REJECT:
                            case DEF_OBJECTNULLACTION:
                                if (cOwnerSend == true)
                                    iRet = m_pClientList[i]->send_msg(cData_All, 43, cKey);
                                else
                                    if (i != sOwnerH)
                                        iRet = m_pClientList[i]->send_msg(cData_All, 43, cKey);
                                break;

                            case DEF_OBJECTATTACK:
                            case DEF_OBJECTATTACKMOVE:
                                if (cOwnerSend == true)
                                    iRet = m_pClientList[i]->send_msg(cData_Srt_Av, 13, cKey);
                                else
                                    if (i != sOwnerH)
                                        iRet = m_pClientList[i]->send_msg(cData_Srt_Av, 13, cKey);
                                break;

                            case DEF_OBJECTMAGIC:
                            case DEF_OBJECTDAMAGE:
                            case DEF_OBJECTDAMAGEMOVE:
                                if (cOwnerSend == true)
                                    iRet = m_pClientList[i]->send_msg(cData_Srt, 11, cKey);
                                else
                                    if (i != sOwnerH)
                                        iRet = m_pClientList[i]->send_msg(cData_Srt, 11, cKey);
                                break;

                            case DEF_OBJECTDYING:
                                if (cOwnerSend == true)
                                    iRet = m_pClientList[i]->send_msg(cData_Srt, 15, cKey);
                                else
                                    if (i != sOwnerH)
                                        iRet = m_pClientList[i]->send_msg(cData_Srt, 15, cKey);
                                break;

                            default:
                                if (cOwnerSend == true)
                                    iRet = m_pClientList[i]->send_msg(cData_Srt, 9, cKey);
                                else
                                    if (i != sOwnerH)
                                        iRet = m_pClientList[i]->send_msg(cData_Srt, 9, cKey);
                                break;
                        } //Switch
                    } // If 2
                    else
                    {
                        switch (wMsgType)
                        {
                            case DEF_MSGTYPE_CONFIRM:
                            case DEF_MSGTYPE_REJECT:
                            case DEF_OBJECTNULLACTION:
                                if (cOwnerSend == true)
                                    iRet = m_pClientList[i]->send_msg(cData_All, 43, cKey);
                                else
                                    if (i != sOwnerH)
                                        iRet = m_pClientList[i]->send_msg(cData_All, 43, cKey);
                                break;

                            case DEF_OBJECTATTACK:
                            case DEF_OBJECTATTACKMOVE:
                                if (cOwnerSend == true)
                                    iRet = m_pClientList[i]->send_msg(cData_Srt_Av, 13, cKey);
                                else
                                    if (i != sOwnerH)
                                        iRet = m_pClientList[i]->send_msg(cData_Srt_Av, 13, cKey);
                                break;

                            case DEF_OBJECTMAGIC:
                            case DEF_OBJECTDAMAGE:
                            case DEF_OBJECTDAMAGEMOVE:
                                if (cOwnerSend == true)
                                    iRet = m_pClientList[i]->send_msg(cData_Srt, 11, cKey);
                                else
                                    if (i != sOwnerH)
                                        iRet = m_pClientList[i]->send_msg(cData_Srt, 11, cKey);
                                break;

                            case DEF_OBJECTDYING:
                                if (cOwnerSend == true)
                                    iRet = m_pClientList[i]->send_msg(cData_Srt, 15, cKey);
                                else
                                    if (i != sOwnerH)
                                        iRet = m_pClientList[i]->send_msg(cData_Srt, 15, cKey);
                                break;

                            default:
                                if (cOwnerSend == true)
                                    iRet = m_pClientList[i]->send_msg(cData_All, 43, cKey);
                                else
                                    if (i != sOwnerH)
                                        iRet = m_pClientList[i]->send_msg(cData_All, 43, cKey);
                                break;
                        } //Switch
                    } //else
                } // If 1
        } //While finish
    } //Finish Player
    else
    {

        if (m_pNpcList[sOwnerH] == 0) return;

        wp = (uint16_t *)cp_a;
        *wp = sOwnerH + 10000;
        cp_a += 2;

        sp = (short *)cp_a;
        sX = m_pNpcList[sOwnerH]->m_sX;
        *sp = sX;
        cp_a += 2;

        sp = (short *)cp_a;
        sY = m_pNpcList[sOwnerH]->m_sY;
        *sp = sY;
        cp_a += 2;

        sp = (short *)cp_a;
        *sp = m_pNpcList[sOwnerH]->m_sType;
        cp_a += 2;

        *cp_a = m_pNpcList[sOwnerH]->m_cDir;
        cp_a++;

        memcpy(cp_a, m_pNpcList[sOwnerH]->m_cName, 5);
        cp_a += 5;

        sp = (short *)cp_a;
        *sp = m_pNpcList[sOwnerH]->m_sAppr2;
        cp_a += 2;

        ip = (int *)cp_a;
        ipStatus = ip;
        *ip = m_pNpcList[sOwnerH]->m_iStatus;
        cp_a += 4;

        if (wMsgType == DEF_OBJECTNULLACTION)
        {
            if (m_pNpcList[sOwnerH]->m_bIsKilled == true)
                *cp_a = 1;
            else *cp_a = 0;
        }
        else *cp_a = 0;
        cp_a++;

        wp = (uint16_t *)cp_s;
        *wp = sOwnerH + 40000;
        cp_s += 2;

        *cp_s = m_pNpcList[sOwnerH]->m_cDir;
        cp_s++;

        *cp_s = (unsigned char)sV1;
        cp_s++;
        *cp_s = (unsigned char)sV2;
        cp_s++;

        sp = (short *)cp_s;
        sX = m_pNpcList[sOwnerH]->m_sX;
        *sp = sX;
        cp_s += 2;
        sp = (short *)cp_s;
        sY = m_pNpcList[sOwnerH]->m_sY;
        *sp = sY;
        cp_s += 2;

        wp = (uint16_t *)cp_sv;
        *wp = sOwnerH + 40000;
        cp_sv += 2;
        *cp_sv = m_pNpcList[sOwnerH]->m_cDir;
        cp_sv++;
        *cp_sv = sV1 - sX;
        cp_sv++;
        *cp_sv = sV2 - sY;
        cp_sv++;
        sp = (short *)cp_sv;
        *sp = sV3;
        cp_sv += 2;

        bFlag = true;
        iShortCutIndex = 0;

        while (bFlag)
        {

            i = m_iClientShortCut[iShortCutIndex];
            iShortCutIndex++;
            if (i == 0) bFlag = false;

            if ((bFlag == true) && (m_pClientList[i] != 0))

                if ((m_pClientList[i]->m_cMapIndex == m_pNpcList[sOwnerH]->m_cMapIndex) &&
                    (m_pClientList[i]->m_sX >= m_pNpcList[sOwnerH]->m_sX - 10 - sRange) &&
                    (m_pClientList[i]->m_sX <= m_pNpcList[sOwnerH]->m_sX + 10 + sRange) &&
                    (m_pClientList[i]->m_sY >= m_pNpcList[sOwnerH]->m_sY - 8 - sRange) &&
                    (m_pClientList[i]->m_sY <= m_pNpcList[sOwnerH]->m_sY + 8 + sRange))
                {

                    iTemp = *ipStatus;
                    iTemp = 0x0FFFFFFF & iTemp;
                    iTemp2 = iGetNpcRelationship(sOwnerH, i);
                    iTemp = (iTemp | (iTemp2 << 28));
                    *ipStatus = iTemp;

                    if ((m_pClientList[i]->m_sX >= m_pNpcList[sOwnerH]->m_sX - 9) &&
                        (m_pClientList[i]->m_sX <= m_pNpcList[sOwnerH]->m_sX + 9) &&
                        (m_pClientList[i]->m_sY >= m_pNpcList[sOwnerH]->m_sY - 7) &&
                        (m_pClientList[i]->m_sY <= m_pNpcList[sOwnerH]->m_sY + 7))
                    {
                        switch (wMsgType)
                        {
                            case DEF_MSGTYPE_CONFIRM:
                            case DEF_MSGTYPE_REJECT:
                            case DEF_OBJECTNULLACTION:
                                iRet = m_pClientList[i]->send_msg(cData_All, 27, cKey);
                                break;

                            case DEF_OBJECTDYING:
                                iRet = m_pClientList[i]->send_msg(cData_Srt, 15, cKey);
                                break;

                            case DEF_OBJECTDAMAGE:
                            case DEF_OBJECTDAMAGEMOVE:
                                iRet = m_pClientList[i]->send_msg(cData_Srt, 11, cKey);
                                break;

                            case DEF_OBJECTATTACK:
                            case DEF_OBJECTATTACKMOVE:
                                iRet = m_pClientList[i]->send_msg(cData_Srt_Av, 13, cKey);
                                break;

                            default:
                                iRet = m_pClientList[i]->send_msg(cData_Srt, 9, cKey);
                                break;

                        } //Switch
                    }
                    else
                    {
                        switch (wMsgType)
                        {
                            case DEF_MSGTYPE_CONFIRM:
                            case DEF_MSGTYPE_REJECT:
                            case DEF_OBJECTNULLACTION:
                                iRet = m_pClientList[i]->send_msg(cData_All, 27, cKey);
                                break;

                            case DEF_OBJECTDYING:
                                iRet = m_pClientList[i]->send_msg(cData_Srt, 15, cKey);
                                break;

                            case DEF_OBJECTDAMAGE:
                            case DEF_OBJECTDAMAGEMOVE:
                                iRet = m_pClientList[i]->send_msg(cData_Srt, 11, cKey);
                                break;

                            case DEF_OBJECTATTACK:
                            case DEF_OBJECTATTACKMOVE:
                                iRet = m_pClientList[i]->send_msg(cData_Srt_Av, 13, cKey);
                                break;

                            default:
                                iRet = m_pClientList[i]->send_msg(cData_All, 27, cKey);
                                break;

                        } //Switch
                    }
                }
        }
    } // else - NPC
}

int CGame::iComposeMoveMapData(short sX, short sY, int iClientH, char cDir, char * pData)
{
    int * ip, ix, iy, iSize, iTileExists, iIndex;
    class CTile * pTileSrc, * pTile;
    unsigned char ucHeader;
    short * sp, * pTotal;
    int iTemp, iTemp2;
    uint16_t * wp;
    char * cp;

    if (m_pClientList[iClientH] == nullptr) return 0;

    pTotal = (short *)pData;
    cp = (char *)(pData + 2);

    iSize = 2;
    iTileExists = 0;

    pTileSrc = (class CTile *)(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_pTile +
        (sX)+(sY)*m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_sSizeY);

    iIndex = 0;

    while (1)
    {
        ix = _tmp_iMoveLocX[cDir][iIndex];
        iy = _tmp_iMoveLocY[cDir][iIndex];
        if ((ix == -1) || (iy == -1)) break;

        iIndex++;

        pTile = (class CTile *)(pTileSrc + ix + iy * m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_sSizeY);

        //If player not same side and is invied (Beholder Hack)
        // there is another person on the tiles, and the owner is not the player
//xxxxxx
        if ((m_pClientList[pTile->m_sOwner] != 0) && (pTile->m_sOwner != iClientH))
            if ((m_pClientList[pTile->m_sOwner]->m_cSide != 0) &&
                (m_pClientList[pTile->m_sOwner]->m_cSide != m_pClientList[iClientH]->m_cSide) &&
                ((m_pClientList[pTile->m_sOwner]->m_iStatus & 0x00000010) != 0))
            {
                continue;
            }

        if ((pTile->m_sOwner != 0) || (pTile->m_sDeadOwner != 0) ||
            (pTile->m_pItem[0] != 0) || (pTile->m_sDynamicObjectType != 0))
        {

            iTileExists++;

            sp = (short *)cp;
            *sp = ix;
            cp += 2;
            sp = (short *)cp;
            *sp = iy;
            cp += 2;
            iSize += 4;

            ucHeader = 0;

            if (pTile->m_sOwner != 0)
            {
                if (pTile->m_cOwnerClass == DEF_OWNERTYPE_PLAYER)
                {
                    if (m_pClientList[pTile->m_sOwner] != 0) ucHeader = ucHeader | 0x01;
                    else pTile->m_sOwner = 0;
                }
                if (pTile->m_cOwnerClass == DEF_OWNERTYPE_NPC)
                {
                    if (m_pNpcList[pTile->m_sOwner] != 0) ucHeader = ucHeader | 0x01;
                    else pTile->m_sOwner = 0;
                }
            }
            if (pTile->m_sDeadOwner != 0)
            {
                if (pTile->m_cDeadOwnerClass == DEF_OWNERTYPE_PLAYER)
                {
                    if (m_pClientList[pTile->m_sDeadOwner] != 0)	ucHeader = ucHeader | 0x02;
                    else pTile->m_sDeadOwner = 0;
                }
                if (pTile->m_cDeadOwnerClass == DEF_OWNERTYPE_NPC)
                {
                    if (m_pNpcList[pTile->m_sDeadOwner] != 0) ucHeader = ucHeader | 0x02;
                    else pTile->m_sDeadOwner = 0;
                }
            }

            if (pTile->m_pItem[0] != 0)				ucHeader = ucHeader | 0x04;
            if (pTile->m_sDynamicObjectType != 0)    ucHeader = ucHeader | 0x08;

            *cp = ucHeader;
            cp++;
            iSize++;

            if ((ucHeader & 0x01) != 0)
            {
                switch (pTile->m_cOwnerClass)
                {
                    case DEF_OWNERTYPE_PLAYER:
                        sp = (short *)cp;
                        *sp = pTile->m_sOwner;
                        cp += 2;
                        iSize += 2;

                        sp = (short *)cp;
                        *sp = m_pClientList[pTile->m_sOwner]->m_sType;
                        cp += 2;
                        iSize += 2;

                        *cp = m_pClientList[pTile->m_sOwner]->m_cDir;
                        cp++;
                        iSize++;

                        sp = (short *)cp;
                        *sp = m_pClientList[pTile->m_sOwner]->m_sAppr1;
                        cp += 2;
                        iSize += 2;

                        sp = (short *)cp;
                        *sp = m_pClientList[pTile->m_sOwner]->m_sAppr2;
                        cp += 2;
                        iSize += 2;

                        sp = (short *)cp;
                        *sp = m_pClientList[pTile->m_sOwner]->m_sAppr3;
                        cp += 2;
                        iSize += 2;

                        sp = (short *)cp;
                        *sp = m_pClientList[pTile->m_sOwner]->m_sAppr4;
                        cp += 2;
                        iSize += 2;

                        ip = (int *)cp;
                        *ip = m_pClientList[pTile->m_sOwner]->m_iApprColor;
                        cp += 4;
                        iSize += 4;

                        ip = (int *)cp;

                        if (m_pClientList[iClientH]->m_cSide != m_pClientList[pTile->m_sOwner]->m_cSide)
                        {
                            if (iClientH != pTile->m_sOwner)
                            {
                                iTemp = m_pClientList[pTile->m_sOwner]->m_iStatus & 0x0F0FFFF7F;
                            }
                            else
                            {
                                iTemp = m_pClientList[pTile->m_sOwner]->m_iStatus;
                            }
                        }
                        else
                        {
                            iTemp = m_pClientList[pTile->m_sOwner]->m_iStatus;
                        }

                        //iTemp = m_pClientList[pTile->m_sOwner]->m_iStatus;
                        iTemp = 0x0FFFFFFF & iTemp;
                        iTemp2 = iGetPlayerABSStatus(pTile->m_sOwner, iClientH);
                        iTemp = (iTemp | (iTemp2 << 28));
                        *ip = iTemp;
                        cp += 4;
                        iSize += 4;

                        memcpy(cp, m_pClientList[pTile->m_sOwner]->m_cCharName, 10);
                        cp += 10;
                        iSize += 10;
                        break;

                    case DEF_OWNERTYPE_NPC:
                        sp = (short *)cp;
                        *sp = pTile->m_sOwner + 10000;
                        cp += 2;
                        iSize += 2;

                        sp = (short *)cp;
                        *sp = m_pNpcList[pTile->m_sOwner]->m_sType;
                        cp += 2;
                        iSize += 2;

                        *cp = m_pNpcList[pTile->m_sOwner]->m_cDir;
                        cp++;
                        iSize++;

                        sp = (short *)cp;
                        *sp = m_pNpcList[pTile->m_sOwner]->m_sAppr2;
                        cp += 2;
                        iSize += 2;

                        ip = (int *)cp;
                        iTemp = m_pNpcList[pTile->m_sOwner]->m_iStatus;
                        iTemp = 0x0FFFFFFF & iTemp;
                        iTemp2 = iGetNpcRelationship(pTile->m_sOwner, iClientH);
                        iTemp = (iTemp | (iTemp2 << 28));
                        *ip = iTemp;
                        cp += 4;
                        iSize += 4;

                        memcpy(cp, m_pNpcList[pTile->m_sOwner]->m_cName, 5);
                        cp += 5;
                        iSize += 5;
                }//end switch
            }// if ((ucHeader & 0x01) != 0)

            if ((ucHeader & 0x02) != 0)
            {
                switch (pTile->m_cDeadOwnerClass)
                {
                    case DEF_OWNERTYPE_PLAYER:

                        sp = (short *)cp;
                        *sp = pTile->m_sDeadOwner;
                        cp += 2;
                        iSize += 2;

                        sp = (short *)cp;
                        *sp = m_pClientList[pTile->m_sDeadOwner]->m_sType;
                        cp += 2;
                        iSize += 2;

                        *cp = m_pClientList[pTile->m_sDeadOwner]->m_cDir;
                        cp++;
                        iSize++;

                        sp = (short *)cp;
                        *sp = m_pClientList[pTile->m_sDeadOwner]->m_sAppr1;
                        cp += 2;
                        iSize += 2;

                        sp = (short *)cp;
                        *sp = m_pClientList[pTile->m_sDeadOwner]->m_sAppr2;
                        cp += 2;
                        iSize += 2;

                        sp = (short *)cp;
                        *sp = m_pClientList[pTile->m_sDeadOwner]->m_sAppr3;
                        cp += 2;
                        iSize += 2;

                        sp = (short *)cp;
                        *sp = m_pClientList[pTile->m_sDeadOwner]->m_sAppr4;
                        cp += 2;
                        iSize += 2;

                        ip = (int *)cp;
                        *ip = m_pClientList[pTile->m_sDeadOwner]->m_iApprColor;
                        cp += 4;
                        iSize += 4;

                        ip = (int *)cp;

                        if (m_pClientList[iClientH]->m_cSide != m_pClientList[pTile->m_sDeadOwner]->m_cSide)
                        {
                            if (iClientH != pTile->m_sDeadOwner)
                            {
                                iTemp = m_pClientList[pTile->m_sDeadOwner]->m_iStatus & 0x0F0FFFF7F;
                            }
                            else
                            {
                                iTemp = m_pClientList[pTile->m_sDeadOwner]->m_iStatus;
                            }
                        }
                        else
                        {
                            iTemp = m_pClientList[pTile->m_sDeadOwner]->m_iStatus;
                        }

                        //iTemp = m_pClientList[pTile->m_sDeadOwner]->m_iStatus;
                        iTemp = 0x0FFFFFFF & iTemp;

                        iTemp2 = iGetPlayerABSStatus(pTile->m_sDeadOwner, iClientH);
                        iTemp = (iTemp | (iTemp2 << 28));
                        *ip = iTemp;
                        cp += 4;
                        iSize += 4;

                        memcpy(cp, m_pClientList[pTile->m_sDeadOwner]->m_cCharName, 10);
                        cp += 10;
                        iSize += 10;
                        break;

                    case DEF_OWNERTYPE_NPC:
                        sp = (short *)cp;
                        *sp = pTile->m_sDeadOwner + 10000;
                        cp += 2;
                        iSize += 2;

                        sp = (short *)cp;
                        *sp = m_pNpcList[pTile->m_sDeadOwner]->m_sType;
                        cp += 2;
                        iSize += 2;

                        *cp = m_pNpcList[pTile->m_sDeadOwner]->m_cDir;
                        cp++;
                        iSize++;

                        sp = (short *)cp;
                        *sp = m_pNpcList[pTile->m_sDeadOwner]->m_sAppr2;
                        cp += 2;
                        iSize += 2;

                        ip = (int *)cp;

                        iTemp = m_pNpcList[pTile->m_sDeadOwner]->m_iStatus;
                        iTemp = 0x0FFFFFFF & iTemp;
                        iTemp2 = iGetNpcRelationship(pTile->m_sDeadOwner, iClientH);
                        iTemp = (iTemp | (iTemp2 << 28));
                        *ip = iTemp;

                        cp += 4;
                        iSize += 4;

                        memcpy(cp, m_pNpcList[pTile->m_sDeadOwner]->m_cName, 5);
                        cp += 5;
                        iSize += 5;
                        break;
                }//End Switch
            }// if ((ucHeader & 0x02) != 0)

            if (pTile->m_pItem[0] != 0)
            {
                sp = (short *)cp;
                *sp = pTile->m_pItem[0]->m_sSprite;
                cp += 2;
                iSize += 2;

                sp = (short *)cp;
                *sp = pTile->m_pItem[0]->m_sSpriteFrame;
                cp += 2;
                iSize += 2;

                *cp = pTile->m_pItem[0]->m_cItemColor;
                cp++;
                iSize++;
            }

            if (pTile->m_sDynamicObjectType != 0)
            {

                wp = (uint16_t *)cp;
                *wp = pTile->m_wDynamicObjectID;
                cp += 2;
                iSize += 2;

                sp = (short *)cp;
                *sp = pTile->m_sDynamicObjectType;
                cp += 2;
                iSize += 2;
            } //(pTile->m_sDynamicObjectType != 0)

        } //(pTile->m_sOwner != 0)
    } // end While(1)
    *pTotal = iTileExists;
    return iSize;
}

void CGame::CheckClientResponseTime()
{
    int i, iPlusTime, iMaxSuperAttack, iValue;
    short sTemp;
    uint32_t dwTime;
    short sItemIndex;
    //locobans
    //int iMapside, iMapside2;
    //SYSTEMTIME SysTime;

       /*
       GetLocalTime(&SysTime);
       switch (SysTime.wDayOfWeek) {
       case 1:	iWarPeriod = 30; break; //Â¿Ã¹Â¿Ã¤Ã€Ã
       case 2:	iWarPeriod = 30; break; //ÃˆÂ­Â¿Ã¤Ã€Ã
       case 3:	iWarPeriod = 60; break; //Â¼Ã¶Â¿Ã¤Ã€Ã
       case 4:	iWarPeriod = 60*2;  break; //Â¸Ã±Â¿Ã¤Ã€Ã
       case 5:	iWarPeriod = 60*5;  break; //Â±ÃÂ¿Ã¤Ã€Ã
       case 6:	iWarPeriod = 60*10; break; //Ã…Ã¤Â¿Ã¤Ã€Ã
       case 0:	iWarPeriod = 60*20; break; //Ã€ÃÂ¿Ã¤Ã€Ã
       }
       */

    dwTime = timeGetTime();

    for (i = 1; i < DEF_MAXCLIENTS; i++)
    {
        if (m_pClientList[i] != 0)
        {

            if ((dwTime - m_pClientList[i]->m_dwTime) > DEF_CLIENTTIMEOUT)
            {
                if (m_pClientList[i]->m_bIsInitComplete == true)
                {
                    //Testcode 
                    wsprintf(G_cTxt, "Client Timeout: %s", m_pClientList[i]->m_cIPaddress);
                    log->info(G_cTxt);

                    DeleteClient(i, true, true);
                }
                else if ((dwTime - m_pClientList[i]->m_dwTime) > DEF_CLIENTTIMEOUT)
                {
                    DeleteClient(i, false, false);
                }
            }
            else if (m_pClientList[i]->m_bIsInitComplete == true)
            {
                m_pClientList[i]->m_iTimeLeft_ShutUp--;
                if (m_pClientList[i]->m_iTimeLeft_ShutUp < 0) m_pClientList[i]->m_iTimeLeft_ShutUp = 0;

                m_pClientList[i]->m_iTimeLeft_Rating--;
                if (m_pClientList[i]->m_iTimeLeft_Rating < 0) m_pClientList[i]->m_iTimeLeft_Rating = 0;

                if (((dwTime - m_pClientList[i]->m_dwHungerTime) > DEF_HUNGERTIME) && (m_pClientList[i]->m_bIsKilled == false))
                {
                    // v2.03
                    if ((m_pClientList[i]->m_iLevel < DEF_LEVELLIMIT) || (m_pClientList[i]->m_iAdminUserLevel >= 1))
                    {
                    }
                    else m_pClientList[i]->m_iHungerStatus--;
                    if (m_pClientList[i]->m_iHungerStatus <= 0) m_pClientList[i]->m_iHungerStatus = 0;
                    m_pClientList[i]->m_dwHungerTime = dwTime;

                    if ((m_pClientList[i]->m_iHP > 0) && (m_pClientList[i]->m_iHungerStatus < 30))
                    {
                        SendNotifyMsg(0, i, DEF_NOTIFY_HUNGER, m_pClientList[i]->m_iHungerStatus, 0, 0, 0);
                    }
                }

                if (_bCheckCharacterData(i) == false)
                {
                    DeleteClient(i, true, true);
                    break;
                }

                if ((m_pClientList[i]->m_iHungerStatus <= 30) && (m_pClientList[i]->m_iHungerStatus >= 0))
                    iPlusTime = (30 - m_pClientList[i]->m_iHungerStatus) * 1000;
                else iPlusTime = 0;

                iPlusTime = abs(iPlusTime);

                // HP
                if ((dwTime - m_pClientList[i]->m_dwHPTime) > (uint32_t)(DEF_HPUPTIME + iPlusTime))
                {
                    TimeHitPointsUp(i);
                    m_pClientList[i]->m_dwHPTime = dwTime;
                }

                // MP
                if ((dwTime - m_pClientList[i]->m_dwMPTime) > (uint32_t)(DEF_MPUPTIME + iPlusTime))
                {
                    TimeManaPointsUp(i);
                    m_pClientList[i]->m_dwMPTime = dwTime;
                }

                // SP
                if ((dwTime - m_pClientList[i]->m_dwSPTime) > (uint32_t)(DEF_SPUPTIME + iPlusTime))
                {
                    TimeStaminarPointsUp(i);
                    m_pClientList[i]->m_dwSPTime = dwTime;
                }

                if ((m_pClientList[i]->m_bIsPoisoned == true) && ((dwTime - m_pClientList[i]->m_dwPoisonTime) > DEF_POISONTIME))
                {
                    PoisonEffect(i, 0);
                    m_pClientList[i]->m_dwPoisonTime = dwTime;
                }

                if ((m_pMapList[m_pClientList[i]->m_cMapIndex]->m_bIsFightZone == false) &&
                    ((dwTime - m_pClientList[i]->m_dwAutoSaveTime) > (uint32_t)DEF_AUTOSAVETIME))
                {
                    bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA, i);
                    m_pClientList[i]->m_dwAutoSaveTime = dwTime;
                }

                // ExpStock
                if ((dwTime - m_pClientList[i]->m_dwExpStockTime) > (uint32_t)DEF_EXPSTOCKTIME)
                {
                    m_pClientList[i]->m_dwExpStockTime = dwTime;
                    CalcExpStock(i);
                    CheckUniqueItemEquipment(i);
                    CheckCrusadeResultCalculation(i);
                    //CheckHeldenianResultCalculation(i);
                }

                // AutoExe
                if ((dwTime - m_pClientList[i]->m_dwAutoExpTime) > (uint32_t)DEF_AUTOEXPTIME)
                {
                    iValue = (m_pClientList[i]->m_iLevel / 2);
                    if (iValue <= 0) iValue = 1;
                    if (m_pClientList[i]->m_iAutoExpAmount < iValue)
                    {
                        if ((m_pClientList[i]->m_iExp + iValue) < m_iLevelExpTable[m_pClientList[i]->m_iLevel + 1])
                        {
                            //m_pClientList[i]->m_iExpStock += iValue;
                            GetExp(i, iValue, false);
                            CalcExpStock(i);
                        }
                    }

                    m_pClientList[i]->m_iAutoExpAmount = 0;
                    m_pClientList[i]->m_dwAutoExpTime = dwTime;
                }

                // v1.432
                if (m_pClientList[i]->m_iSpecialAbilityTime == 3)
                {
                    SendNotifyMsg(0, i, DEF_NOTIFY_SPECIALABILITYENABLED, 0, 0, 0, 0);
                    // New 25/05/2004
                    // After the time up, add magic back
                    sItemIndex = m_pClientList[i]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
                    if (sItemIndex != -1)
                    {
                        if ((m_pClientList[i]->m_pItemList[sItemIndex]->m_sIDnum == 865) || (m_pClientList[i]->m_pItemList[sItemIndex]->m_sIDnum == 866))
                        {
                            if (m_pClientList[i]->m_iInt > 99 && m_pClientList[i]->m_iMag > 99)
                            {
                                m_pClientList[i]->m_cMagicMastery[94] = true;
                                SendNotifyMsg(0, i, DEF_NOTIFY_STATECHANGE_SUCCESS, 0, 0, 0, 0);
                            }
                        }
                    }
                }
                m_pClientList[i]->m_iSpecialAbilityTime -= 3;
                if (m_pClientList[i]->m_iSpecialAbilityTime < 0) m_pClientList[i]->m_iSpecialAbilityTime = 0;

                // v1.432
                if (m_pClientList[i]->m_bIsSpecialAbilityEnabled == true)
                {
                    if (((dwTime - m_pClientList[i]->m_dwSpecialAbilityStartTime) / 1000) > m_pClientList[i]->m_iSpecialAbilityLastSec)
                    {
                        SendNotifyMsg(0, i, DEF_NOTIFY_SPECIALABILITYSTATUS, 3, 0, 0, 0);
                        m_pClientList[i]->m_bIsSpecialAbilityEnabled = false;
                        m_pClientList[i]->m_iSpecialAbilityTime = DEF_SPECABLTYTIMESEC;
                        sTemp = m_pClientList[i]->m_sAppr4;
                        sTemp = sTemp & 0xFF0F;
                        m_pClientList[i]->m_sAppr4 = sTemp;
                        SendEventToNearClient_TypeA(i, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
                    }
                }

                //Crusade
                m_pClientList[i]->m_iLockedMapTime -= 3;
                if (m_pClientList[i]->m_iLockedMapTime < 0)
                {
                    m_pClientList[i]->m_iLockedMapTime = 0;
                    memset(m_pClientList[i]->m_cLockedMapName, 0, sizeof(m_pClientList[i]->m_cLockedMapName));
                    strcpy(m_pClientList[i]->m_cLockedMapName, "NONE");
                }

                // v2.04
                m_pClientList[i]->m_iDeadPenaltyTime -= 3;
                if (m_pClientList[i]->m_iDeadPenaltyTime < 0) m_pClientList[i]->m_iDeadPenaltyTime = 0;

                if ((m_pClientList[i]->m_bIsWarLocation == true) && IsEnemyZone(i) && (m_pClientList[i]->m_iAdminUserLevel < 1))
                {
                    // Crusade
                    if (m_bIsCrusadeMode == false)
                        if (m_pClientList[i]->m_bIsInsideOwnTown == false)
                            m_pClientList[i]->m_iTimeLeft_ForceRecall--;

                    if (m_pClientList[i]->m_iTimeLeft_ForceRecall <= 0)
                    {
                        m_pClientList[i]->m_iTimeLeft_ForceRecall = 0;
                        m_pClientList[i]->m_dwWarBeginTime = dwTime;
                        m_pClientList[i]->m_bIsWarLocation = false;

                        SendNotifyMsg(0, i, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, 0);
                        RequestTeleportHandler(i, "1   ");
                    }
                }

                if ((m_bIsHeldenianMode == true) && (m_pMapList[m_pClientList[i]->m_cMapIndex] != 0))
                {
                    if (bCheckHeldenianMap(i, m_iBTFieldMapIndex, DEF_OWNERTYPE_PLAYER) == 1)
                    {
                        SetHeroFlag(i, DEF_OWNERTYPE_PLAYER, true);
                    }
                    else
                    {
                        SetHeroFlag(i, DEF_OWNERTYPE_PLAYER, false);
                    }
                }

                if (m_pClientList[i] == 0) break;
                if (m_pClientList[i]->m_iSkillMsgRecvCount >= 2)
                {
                    DeleteClient(i, true, true);
                }
                else
                {
                    m_pClientList[i]->m_iSkillMsgRecvCount = 0;
                }

                if (m_pClientList[i] == 0) break;
                //if (m_pClientList[i]->m_iLevel < m_pMapList[m_pClientList[i]->m_cMapIndex]->m_iLevelLimit) {
                if ((m_pClientList[i]->m_iLevel < m_pMapList[m_pClientList[i]->m_cMapIndex]->m_iLevelLimit) && (m_pClientList[i]->m_iAdminUserLevel < 1))
                {
                    SendNotifyMsg(0, i, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, 0);
                    RequestTeleportHandler(i, "0   ");
                }

                if (m_pClientList[i] == 0) break;
                //if ( (m_pMapList[m_pClientList[i]->m_cMapIndex]->m_iUpperLevelLimit != 0) &&
                //	 (m_pClientList[i]->m_iLevel > m_pMapList[m_pClientList[i]->m_cMapIndex]->m_iUpperLevelLimit) ) {
                if ((m_pMapList[m_pClientList[i]->m_cMapIndex]->m_iUpperLevelLimit != 0) &&
                    (m_pClientList[i]->m_iLevel > m_pMapList[m_pClientList[i]->m_cMapIndex]->m_iUpperLevelLimit) && (m_pClientList[i]->m_iAdminUserLevel < 1))
                {
                    SendNotifyMsg(0, i, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, 0);
                    if ((m_pClientList[i]->m_cSide == 1) &&
                        (m_pClientList[i]->m_iAdminUserLevel == 0))
                    {
                        RequestTeleportHandler(i, "2   ", "aresden", -1, -1);
                    }
                    else if ((m_pClientList[i]->m_cSide == 2) &&
                        (m_pClientList[i]->m_iAdminUserLevel == 0))
                    {
                        RequestTeleportHandler(i, "2   ", "elvine", -1, -1);
                    }
                }

                if (m_pClientList[i] == 0) break;
                if ((strcmp(m_pClientList[i]->m_cLocation, "elvine") != 0) &&
                    (strcmp(m_pClientList[i]->m_cLocation, "elvhunter") != 0) &&
                    (strcmp(m_pClientList[i]->m_cLocation, "arehunter") != 0) &&
                    (strcmp(m_pClientList[i]->m_cLocation, "aresden") != 0) &&
                    (m_pClientList[i]->m_iLevel >= 20) &&
                    (m_pClientList[i]->m_iAdminUserLevel == 0))
                {
                    wsprintf(G_cTxt, "Traveller Hack: (%s) Player: (%s) is a traveller and is greater than level 19.", m_pClientList[i]->m_cIPaddress, m_pClientList[i]->m_cCharName);
                    PutHackLogFileList(G_cTxt);
                    DeleteClient(i, true, true);
                }

                if (m_pClientList[i] == 0) break;
                if ((m_pMapList[m_pClientList[i]->m_cMapIndex]->m_bIsApocalypseMap == true) &&
                    (m_bIsApocalypseMode == false) &&
                    (m_pClientList[i]->m_iAdminUserLevel == 0))
                {
                    RequestTeleportHandler(i, "1   ");
                }

                //(m_bIsCrusadeMode == true)
                /*
                if (m_pClientList[i] == 0) break;
                if (((memcmp(m_pClientList[i]->m_cLocation, "aresden",7) == 0) || (memcmp(m_pClientList[i]->m_cLocation, "elvine",6) == 0)) ) {
                    iMapside = iGetMapLocationSide(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName);
                    if (iMapside > 3) iMapside2 = iMapside - 2;
                    else iMapside2 = iMapside;

                    if ((m_pClientList[i]->m_cSide != iMapside2) && (iMapside != 0)) {
                        if ((iMapside <= 2) && (m_pClientList[i]->m_iAdminUserLevel < 1)) {
                            if (m_pClientList[i]->m_cSide != 0) {
                                m_pClientList[i]->m_dwWarBeginTime = timeGetTime();
                                m_pClientList[i]->m_bIsWarLocation = true;
                                m_pClientList[i]->m_iTimeLeft_ForceRecall = 1;

                                RequestTeleportHandler(i, "1   ");
                                SendNotifyMsg(0, i, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, 0);
                            }
                        }
                    }
                }*/

                if (m_pClientList[i] == 0) break;
                if (((memcmp(m_pClientList[i]->m_cLocation, "arehunter", 9) == 0) || (memcmp(m_pClientList[i]->m_cLocation, "elvhunter", 9) == 0)) &&
                    ((strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "2ndmiddle") == 0) || (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "middleland") == 0)))
                {
                    SendNotifyMsg(0, i, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, 0);
                    RequestTeleportHandler(i, "1   ");
                }

                if (m_bIsApocalypseMode == true)
                {
                    if (memcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "abaddon", 7) == 0)
                        SendNotifyMsg(0, i, DEF_NOTIFY_APOCGATEOPEN, 167, 169, 0, m_pClientList[i]->m_cMapName);
                    else if (memcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "icebound", 8) == 0)
                        SendNotifyMsg(0, i, DEF_NOTIFY_APOCGATEOPEN, 89, 31, 0, m_pClientList[i]->m_cMapName);
                }

                if (m_pClientList[i] == 0) break;
                if ((m_bIsApocalypseMode == true) &&
                    (memcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "icebound", 8) == 0) &&
                    ((m_pClientList[i]->m_sX == 89 && m_pClientList[i]->m_sY == 31) ||
                        (m_pClientList[i]->m_sX == 89 && m_pClientList[i]->m_sY == 32) ||
                        (m_pClientList[i]->m_sX == 90 && m_pClientList[i]->m_sY == 31) ||
                        (m_pClientList[i]->m_sX == 90 && m_pClientList[i]->m_sY == 32)))
                {
                    RequestTeleportHandler(i, "2   ", "druncncity", -1, -1);
                }

                if (m_pClientList[i] == 0) break;
                if ((memcmp(m_pClientList[i]->m_cLocation, "are", 3) == 0) &&
                    (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "elvfarm") == 0) && (m_pClientList[i]->m_iAdminUserLevel < 1))
                {
                    SendNotifyMsg(0, i, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, 0);
                    RequestTeleportHandler(i, "0   ");
                }

                if (m_pClientList[i] == 0) break;
                if ((memcmp(m_pClientList[i]->m_cLocation, "elv", 3) == 0) &&
                    (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "arefarm") == 0) && (m_pClientList[i]->m_iAdminUserLevel < 1))
                {
                    SendNotifyMsg(0, i, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, 0);
                    RequestTeleportHandler(i, "0   ");
                }

                if (m_pClientList[i] == 0) break;
                if ((strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "middleland") == 0)
                    && (strcmp(m_pClientList[i]->m_cLocation, "NONE") == 0) &&
                    (m_pClientList[i]->m_iAdminUserLevel < 1))
                {
                    SendNotifyMsg(0, i, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, 0);
                    RequestTeleportHandler(i, "0   ");
                }

                if ((m_pClientList[i]->m_bInRecallImpossibleMap == true) && (m_pClientList[i]->m_iAdminUserLevel < 1))
                {
                    m_pClientList[i]->m_iTimeLeft_ForceRecall--;
                    if (m_pClientList[i]->m_iTimeLeft_ForceRecall <= 0)
                    {
                        m_pClientList[i]->m_iTimeLeft_ForceRecall = 0;
                        m_pClientList[i]->m_bInRecallImpossibleMap = false;
                        SendNotifyMsg(0, i, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, 0);
                        RequestTeleportHandler(i, "0   ");
                    }
                }

                if (m_pClientList[i] == 0) break;
                m_pClientList[i]->m_iSuperAttackCount++;
                if (m_pClientList[i]->m_iSuperAttackCount > 12)
                { //39

                    m_pClientList[i]->m_iSuperAttackCount = 0;
                    iMaxSuperAttack = (m_pClientList[i]->m_iLevel / 10);
                    if (m_pClientList[i]->m_iSuperAttackLeft < iMaxSuperAttack) m_pClientList[i]->m_iSuperAttackLeft++;

                    // v1.12
                    SendNotifyMsg(0, i, DEF_NOTIFY_SUPERATTACKLEFT, 0, 0, 0, 0);
                }

                // v1.42
                m_pClientList[i]->m_iTimeLeft_FirmStaminar--;
                if (m_pClientList[i]->m_iTimeLeft_FirmStaminar < 0) m_pClientList[i]->m_iTimeLeft_FirmStaminar = 0;

                // Crusade
                if (m_pClientList[i] == 0) break;
                if (m_pClientList[i]->m_bIsSendingMapStatus == true) _SendMapStatus(i);

                if (m_pClientList[i]->m_iConstructionPoint > 0)
                {
                    CheckCommanderConstructionPoint(i);
                }
            }
        }
    }
}

// TODO
bool CGame::bSendMsgToLS(uint32_t dwMsg, int iClientH, bool bFlag, char * pData)
{
    uint32_t * dwp;
    uint16_t * wp;
    int     iRet, i, iSize;
    char    cCharName[11], cAccountName[11], cAccountPassword[11], cAddress[16], cGuildName[21], cTxt[120], * cp;
    char    cGuildLoc[11], cTemp[120];
    int * ip, iSendSize;


    /*
    m_iCurSubLogSockIndex++;
    if (m_iCurSubLogSockIndex >= DEF_MAXSUBLOGSOCK) m_iCurSubLogSockIndex = 0;


    iCnt = 0;
    bLoopFlag = false;
    while (bLoopFlag == false) {
        if (m_pSubLogSock[m_iCurSubLogSockIndex] != 0)
             bLoopFlag = true;
        else m_iCurSubLogSockIndex++;

        iCnt++;
        if (iCnt >= DEF_MAXSUBLOGSOCK) {
            if (m_bOnExitProcess == false) {
                m_cShutDownCode      = 3;
                m_bOnExitProcess     = true;
                m_dwExitProcessTime  = timeGetTime();
                log->info("(!) GAME SERVER SHUTDOWN PROCESS STARTED(by Log-server connection Lost)!!!");
            }
            return false;
        }
    }
    */


    // v1.41
    memset(G_cData50000, 0, sizeof(G_cData50000));
    memset(cCharName, 0, sizeof(cCharName));
    memset(cAccountName, 0, sizeof(cAccountName));
    memset(cAccountPassword, 0, sizeof(cAccountPassword));
    memset(cAddress, 0, sizeof(cAddress));
    memset(cGuildName, 0, sizeof(cGuildName));
    memset(cGuildLoc, 0, sizeof(cGuildLoc));

    switch (dwMsg)
    {
        // New 07/05/2004
        case MSGID_GAMEMASTERLOG:
            // Sub-log-socket
            if (_bCheckSubLogSocketIndex() == false) return false;

            if (m_pClientList[iClientH] == nullptr) return false;
            if (pData == 0) return false;


            dwp = (uint32_t *)(G_cData50000 + DEF_INDEX4_MSGID);
            *dwp = MSGID_GAMEMASTERLOG;
            wp = (uint16_t *)(G_cData50000 + DEF_INDEX2_MSGTYPE);
            *wp = DEF_MSGTYPE_CONFIRM;

            cp = (char *)(G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

            iSize = strlen(pData);
            memcpy((char *)cp, pData, iSize);

            iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 6 + iSize);
            iSendSize = 6 + iSize;
            break;
            // v2.15 

        case MSGID_GAMEITEMLOG:
            // Sub-log-socket
            if (_bCheckSubLogSocketIndex() == false) return false;

            //		if (m_pClientList[iClientH] == nullptr) return false;
            if (pData == 0) return false;

            dwp = (uint32_t *)(G_cData50000 + DEF_INDEX4_MSGID);
            *dwp = MSGID_GAMEITEMLOG;
            wp = (uint16_t *)(G_cData50000 + DEF_INDEX2_MSGTYPE);
            *wp = DEF_MSGTYPE_CONFIRM;

            cp = (char *)(G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

            iSize = strlen(pData);
            memcpy((char *)cp, pData, iSize);

            iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 6 + iSize);
            iSendSize = 6 + iSize;
            break;

        case MSGID_SENDSERVERSHUTDOWNMSG:
            if (m_pMainLogSock == 0) return false;

            dwp = (uint32_t *)(G_cData50000 + DEF_INDEX4_MSGID);
            *dwp = MSGID_SENDSERVERSHUTDOWNMSG;
            wp = (uint16_t *)(G_cData50000 + DEF_INDEX2_MSGTYPE);
            *wp = DEF_MSGTYPE_CONFIRM;

            iRet = m_pMainLogSock->iSendMsg(G_cData50000, 6);
            break;

        case MSGID_GAMESERVERSHUTDOWNED:
            if (m_pMainLogSock == 0) return false;

            dwp = (uint32_t *)(G_cData50000 + DEF_INDEX4_MSGID);
            *dwp = MSGID_GAMESERVERSHUTDOWNED;
            wp = (uint16_t *)(G_cData50000 + DEF_INDEX2_MSGTYPE);
            *wp = DEF_MSGTYPE_CONFIRM;

            iRet = m_pMainLogSock->iSendMsg(G_cData50000, 6);
            return true;

        case MSGID_REQUEST_SETACCOUNTWAITSTATUS:
        case MSGID_REQUEST_SETACCOUNTINITSTATUS:
            // Sub-log-socket
            if (_bCheckSubLogSocketIndex() == false) return false;

            if (m_pClientList[iClientH] == nullptr) return false;
            dwp = (uint32_t *)(G_cData50000 + DEF_INDEX4_MSGID);
            *dwp = dwMsg;
            wp = (uint16_t *)(G_cData50000 + DEF_INDEX2_MSGTYPE);
            *wp = DEF_MSGTYPE_CONFIRM;

            cp = (char *)(G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

            memcpy(cp, m_pClientList[iClientH]->m_cAccountName, 10);
            cp += 10;

            ip = (int *)cp;
            *ip = m_pClientList[iClientH]->m_iLevel;
            cp += 4;

            iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 20);
            iSendSize = 16;
            break;

        case MSGID_ENTERGAMECONFIRM:

            // Sub-log-socket
            if (_bCheckSubLogSocketIndex() == false) return false;

            if (m_pClientList[iClientH] == nullptr) return false;

            dwp = (uint32_t *)(G_cData50000 + DEF_INDEX4_MSGID);
            *dwp = MSGID_ENTERGAMECONFIRM;
            wp = (uint16_t *)(G_cData50000 + DEF_INDEX2_MSGTYPE);
            *wp = DEF_MSGTYPE_CONFIRM;

            cp = (char *)(G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

            memcpy(cp, m_pClientList[iClientH]->m_cAccountName, 10);
            cp += 10;

            memcpy(cp, m_pClientList[iClientH]->m_cAccountPassword, 10);
            cp += 10;

            memcpy(cp, m_cServerName, 10);
            cp += 10;

            memset(cTxt, 0, sizeof(cTxt));
            m_pClientList[iClientH]->m_pXSock->iGetPeerAddress(cTxt);
            memcpy(cp, cTxt, 16);
            cp += 16;

            ip = (int *)cp;
            *ip = m_pClientList[iClientH]->m_iLevel;
            cp += 4;

            //testcode
            wsprintf(G_cTxt, "Confirmed. Account: (%s) Name: (%s) Level: (%d)", m_pClientList[iClientH]->m_cAccountName, m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_iLevel);
            log->info(G_cTxt);



            iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 56);
            iSendSize = 56;
            if ((m_bIsCrusadeMode == false) && (m_pClientList[iClientH]->m_dwCrusadeGUID == m_dwCrusadeGUID))
            {
                break;
            }
            else
                m_pClientList[iClientH]->m_dwCrusadeGUID = m_dwCrusadeGUID;


            break;

        case MSGID_REQUEST_REGISTERGAMESERVER:
            if (m_pMainLogSock == 0) return false;

            wsprintf(cTxt, "(!) Try to register game server(%s)", m_cServerName);
            log->info(cTxt);

            dwp = (uint32_t *)(G_cData50000 + DEF_INDEX4_MSGID);
            *dwp = MSGID_REQUEST_REGISTERGAMESERVER;
            wp = (uint16_t *)(G_cData50000 + DEF_INDEX2_MSGTYPE);
            *wp = DEF_MSGTYPE_CONFIRM;

            cp = (char *)(G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

            memcpy(cAccountName, m_cServerName, 10);
            if (m_iGameServerMode == 1)
            {
                memcpy(cAddress, m_cGameServerAddrExternal, strlen(m_cGameServerAddrExternal));
            }
            if (m_iGameServerMode == 2)
            {
                memcpy(cAddress, m_cGameServerAddr, strlen(m_cGameServerAddr));
            }
            memcpy(cp, cAccountName, 10);
            cp += 10;

            memcpy(cp, cAddress, 16);
            cp += 16;

            wp = (uint16_t *)cp;
            *wp = m_iGameServerPort;
            cp += 2;

            *cp = m_iTotalMaps;
            cp++;

            for (i = 0; i < m_iTotalMaps; i++)
            {
                memcpy(cp, m_pMapList[i]->m_cName, 11);
                cp += 11;
            }

            iRet = m_pMainLogSock->iSendMsg(G_cData50000, 35 + m_iTotalMaps * 11);

            return true;

        case MSGID_REQUEST_PLAYERDATA:

            // Sub-log-socket
            if (_bCheckSubLogSocketIndex() == false) return false;

            if (m_pClientList[iClientH] == nullptr) return false;

            dwp = (uint32_t *)(G_cData50000 + DEF_INDEX4_MSGID);
            *dwp = MSGID_REQUEST_PLAYERDATA;
            wp = (uint16_t *)(G_cData50000 + DEF_INDEX2_MSGTYPE);
            *wp = DEF_MSGTYPE_CONFIRM;

            cp = (char *)(G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

            //testcode
            if (strlen(m_pClientList[iClientH]->m_cCharName) == 0) log->error("(X) CharName 0!");

            memcpy(cCharName, m_pClientList[iClientH]->m_cCharName, 10);
            memcpy(cAccountName, m_pClientList[iClientH]->m_cAccountName, 10);
            memcpy(cAccountPassword, m_pClientList[iClientH]->m_cAccountPassword, 10);

            memcpy((char *)cp, cCharName, 10);
            cp += 10;

            memcpy((char *)cp, cAccountName, 10);
            cp += 10;

            memcpy((char *)cp, cAccountPassword, 10);
            cp += 10;

            memset(cTemp, 0, sizeof(cTemp));
            iRet = m_pClientList[iClientH]->m_pXSock->iGetPeerAddress(cTemp);
            memcpy((char *)cp, cTemp, 15);
            cp += 15;

            iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 52);
            iSendSize = 52;
            break;

        case MSGID_REQUEST_SAVEPLAYERDATA_REPLY:
        case MSGID_REQUEST_SAVEPLAYERDATA:
        case MSGID_REQUEST_SAVEPLAYERDATALOGOUT:

            // Sub-log-socket
            if (_bCheckSubLogSocketIndex() == false) return false;

            if (m_pClientList[iClientH] == nullptr) return false;

            dwp = (uint32_t *)(G_cData50000 + DEF_INDEX4_MSGID);
            *dwp = dwMsg;
            wp = (uint16_t *)(G_cData50000 + DEF_INDEX2_MSGTYPE);
            *wp = DEF_MSGTYPE_CONFIRM;

            cp = (char *)(G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

            memcpy(cCharName, m_pClientList[iClientH]->m_cCharName, 10);
            memcpy(cAccountName, m_pClientList[iClientH]->m_cAccountName, 10);
            memcpy(cAccountPassword, m_pClientList[iClientH]->m_cAccountPassword, 10);

            memcpy((char *)cp, cCharName, 10);
            cp += 10;

            memcpy((char *)cp, cAccountName, 10);
            cp += 10;

            memcpy((char *)cp, cAccountPassword, 10);
            cp += 10;

            *cp = (char)bFlag;
            cp++;

            iSize = _iComposePlayerDataFileContents(iClientH, cp);

            iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 37 + iSize);
            iSendSize = 37 + iSize;
            break;

        case MSGID_REQUEST_NOSAVELOGOUT:

            // Sub-log-socket
            if (_bCheckSubLogSocketIndex() == false) return false;

            if (m_pClientList[iClientH] == nullptr) return false;

            dwp = (uint32_t *)(G_cData50000 + DEF_INDEX4_MSGID);
            *dwp = MSGID_REQUEST_NOSAVELOGOUT;
            wp = (uint16_t *)(G_cData50000 + DEF_INDEX2_MSGTYPE);
            *wp = DEF_MSGTYPE_CONFIRM;

            cp = (char *)(G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

            memcpy(cCharName, m_pClientList[iClientH]->m_cCharName, 10);
            memcpy(cAccountName, m_pClientList[iClientH]->m_cAccountName, 10);
            memcpy(cAccountPassword, m_pClientList[iClientH]->m_cAccountPassword, 10);

            memcpy((char *)cp, cCharName, 10);
            cp += 10;

            memcpy((char *)cp, cAccountName, 10);
            cp += 10;

            memcpy((char *)cp, cAccountPassword, 10);
            cp += 10;

            *cp = (char)bFlag;
            cp++;

            iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 37);
            iSendSize = 37;
            break;

        case MSGID_REQUEST_CREATENEWGUILD:

            // Sub-log-socket
            if (_bCheckSubLogSocketIndex() == false) return false;

            if (m_pClientList[iClientH] == nullptr) return false;

            dwp = (uint32_t *)(G_cData50000 + DEF_INDEX4_MSGID);
            *dwp = MSGID_REQUEST_CREATENEWGUILD;
            wp = (uint16_t *)(G_cData50000 + DEF_INDEX2_MSGTYPE);
            *wp = DEF_MSGTYPE_CONFIRM;

            cp = (char *)(G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

            memcpy(cCharName, m_pClientList[iClientH]->m_cCharName, 10);
            memcpy(cAccountName, m_pClientList[iClientH]->m_cAccountName, 10);
            memcpy(cAccountPassword, m_pClientList[iClientH]->m_cAccountPassword, 10);
            memcpy(cGuildName, m_pClientList[iClientH]->m_cGuildName, 20);
            memcpy(cGuildLoc, m_pClientList[iClientH]->m_cLocation, 10);

            memcpy((char *)cp, cCharName, 10);
            cp += 10;

            memcpy((char *)cp, cAccountName, 10);
            cp += 10;

            memcpy((char *)cp, cAccountPassword, 10);
            cp += 10;

            memcpy((char *)cp, cGuildName, 20);
            cp += 20;
            memcpy((char *)cp, cGuildLoc, 10);
            cp += 10;

            ip = (int *)cp;
            *ip = m_pClientList[iClientH]->m_iGuildGUID;
            cp += 4;

            iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 75);
            iSendSize = 75;
            break;

        case MSGID_REQUEST_DISBANDGUILD:
            // Sub-log-socket
            if (_bCheckSubLogSocketIndex() == false) return false;

            if (m_pClientList[iClientH] == nullptr) return false;

            dwp = (uint32_t *)(G_cData50000 + DEF_INDEX4_MSGID);
            *dwp = MSGID_REQUEST_DISBANDGUILD;
            wp = (uint16_t *)(G_cData50000 + DEF_INDEX2_MSGTYPE);
            *wp = DEF_MSGTYPE_CONFIRM;

            cp = (char *)(G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

            memcpy(cCharName, m_pClientList[iClientH]->m_cCharName, 10);
            memcpy(cAccountName, m_pClientList[iClientH]->m_cAccountName, 10);
            memcpy(cAccountPassword, m_pClientList[iClientH]->m_cAccountPassword, 10);
            memcpy(cGuildName, m_pClientList[iClientH]->m_cGuildName, 20);

            memcpy((char *)cp, cCharName, 10);
            cp += 10;

            memcpy((char *)cp, cAccountName, 10);
            cp += 10;

            memcpy((char *)cp, cAccountPassword, 10);
            cp += 10;

            memcpy((char *)cp, cGuildName, 20);
            cp += 20;

            iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 56);
            iSendSize = 56;
            break;

        case MSGID_REQUEST_HELDENIAN_WINNER:
            if (_bCheckSubLogSocketIndex() == false) return false;

            dwp = (uint32_t *)(G_cData50000 + DEF_INDEX4_MSGID);
            *dwp = MSGID_REQUEST_HELDENIAN_WINNER;
            wp = (uint16_t *)(G_cData50000 + DEF_INDEX2_MSGTYPE);
            *wp = DEF_MSGTYPE_CONFIRM;

            cp = (char *)(G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

            if (m_cHeldenianVictoryType == 1)
                memcpy(cp, "aresden", 7);
            else if (m_cHeldenianVictoryType == 2)
                memcpy(cp, "elvine", 6);
            else
                memcpy(cp, "draw", 4);

            iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 21);
            iSendSize = 21;
            break;

        case MSGID_REQUEST_UPDATEGUILDINFO_NEWGUILDSMAN:

            // Sub-log-socket
            if (_bCheckSubLogSocketIndex() == false) return false;

            if (m_pClientList[iClientH] == nullptr) return false;

            dwp = (uint32_t *)(G_cData50000 + DEF_INDEX4_MSGID);
            *dwp = MSGID_REQUEST_UPDATEGUILDINFO_NEWGUILDSMAN;
            wp = (uint16_t *)(G_cData50000 + DEF_INDEX2_MSGTYPE);
            *wp = DEF_MSGTYPE_CONFIRM;

            cp = (char *)(G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

            memcpy(cCharName, m_pClientList[iClientH]->m_cCharName, 10);
            memcpy(cGuildName, m_pClientList[iClientH]->m_cGuildName, 20);

            memcpy((char *)cp, cCharName, 10);
            cp += 10;

            memcpy((char *)cp, cGuildName, 20);
            cp += 20;

            iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 36);
            iSendSize = 36;
            break;

        case MSGID_REQUEST_UPDATEGUILDINFO_DELGUILDSMAN:

            // Sub-log-socket
            if (_bCheckSubLogSocketIndex() == false) return false;

            if (m_pClientList[iClientH] == nullptr) return false;

            dwp = (uint32_t *)(G_cData50000 + DEF_INDEX4_MSGID);
            *dwp = MSGID_REQUEST_UPDATEGUILDINFO_DELGUILDSMAN;
            wp = (uint16_t *)(G_cData50000 + DEF_INDEX2_MSGTYPE);
            *wp = DEF_MSGTYPE_CONFIRM;

            cp = (char *)(G_cData50000 + DEF_INDEX2_MSGTYPE + 2);

            memcpy(cCharName, m_pClientList[iClientH]->m_cCharName, 10);
            memcpy(cGuildName, m_pClientList[iClientH]->m_cGuildName, 20);

            memcpy((char *)cp, cCharName, 10);
            cp += 10;

            memcpy((char *)cp, cGuildName, 20);
            cp += 20;

            iRet = m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, 36);
            iSendSize = 36;
            break;
    }

    switch (iRet)
    {
        case DEF_XSOCKEVENT_QUENEFULL:
        case DEF_XSOCKEVENT_SOCKETERROR:
        case DEF_XSOCKEVENT_CRITICALERROR:
        case DEF_XSOCKEVENT_SOCKETCLOSED:
            // Sub-log-socket
            wsprintf(G_cTxt, "(!!!) Sub-log-socket(%d) send error!", m_iCurSubLogSockIndex);
            log->info(G_cTxt);
            PutLogFileList(G_cTxt);

            delete m_pSubLogSock[m_iCurSubLogSockIndex];
            m_pSubLogSock[m_iCurSubLogSockIndex] = 0;
            m_bIsSubLogSockAvailable[m_iCurSubLogSockIndex] = false;
            m_iSubLogSockActiveCount--;

            m_pSubLogSock[m_iCurSubLogSockIndex] = new class XSocket(m_hWnd, DEF_SERVERSOCKETBLOCKLIMIT);
            m_pSubLogSock[m_iCurSubLogSockIndex]->bConnect(m_cLogServerAddr, m_iLogServerPort, (WM_ONLOGSOCKETEVENT + m_iCurSubLogSockIndex + 1));
            m_pSubLogSock[m_iCurSubLogSockIndex]->bInitBufferSize(DEF_MSGBUFFERSIZE);

            wsprintf(G_cTxt, "(!) Try to reconnect sub-log-socket(%d)... Addr:%s  Port:%d", m_iCurSubLogSockIndex, m_cLogServerAddr, m_iLogServerPort);
            log->info(G_cTxt);

            // v1.41 sub-log-socket
            m_iSubLogSockFailCount++;

            // v1.41
            if (_bCheckSubLogSocketIndex() == false) return false;
            m_pSubLogSock[m_iCurSubLogSockIndex]->iSendMsg(G_cData50000, iSendSize);

            return false;
    }

    return true;
}

void CGame::InitPlayerData(int iClientH, char * pData, uint32_t dwSize)
{
    char * cp, cName[11], cData[256], cTxt[256], cGuildStatus, cQuestRemain;
    uint32_t * dwp;
    uint16_t * wp;
    int     iRet, i, iTotalPoints;
    bool    bRet;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == true) return; // Ã€ÃŒÂ¹ÃŒ ÃƒÃŠÂ±Ã¢ÃˆÂ­ ÂµÃˆ Ã„Â³Â¸Â¯Ã…ÃÃ€Ã‡ ÂµÂ¥Ã€ÃŒÃ…ÃÃ€ÃŒÂ´Ã™. Ã€ÃŒÂ·Â±Ã€ÃÃ€ÃŒ Ã€Ã–Ã€Â» Â¼Ã¶ Ã€Ã–Ã€Â»Â±Ã®?

    // Log Server
    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

    memset(cName, 0, sizeof(cName));
    memcpy(cName, cp, 10);
    cp += 10;

    //m_pClientList[iClientH]->m_cAccountStatus = *cp;
    cp++;

    cGuildStatus = *cp;
    cp++;

    m_pClientList[iClientH]->m_iHitRatio = 0;
    m_pClientList[iClientH]->m_iDefenseRatio = 0;
    m_pClientList[iClientH]->m_cSide = 0;

    bRet = _bDecodePlayerDatafileContents(iClientH, cp, dwSize - 19);
    if (bRet == false)
    {
        wsprintf(G_cTxt, "(HACK?) Character(%s) data error!", m_pClientList[iClientH]->m_cCharName);
        DeleteClient(iClientH, false, true); //!!!
        return;
    }

    ___RestorePlayerCharacteristics(iClientH);

    ___RestorePlayerRating(iClientH);

    if ((m_pClientList[iClientH]->m_sX == -1) && (m_pClientList[iClientH]->m_sY == -1))
    {
        GetMapInitialPoint(m_pClientList[iClientH]->m_cMapIndex, &m_pClientList[iClientH]->m_sX, &m_pClientList[iClientH]->m_sY, m_pClientList[iClientH]->m_cLocation);
    }

    // New 17/05/2004
    SetPlayingStatus(iClientH);
    int iTemp, iTemp2;
    iTemp = m_pClientList[iClientH]->m_iStatus;
    iTemp = 0x0FFFFFFF & iTemp;
    iTemp2 = iGetPlayerABSStatus(iClientH);
    iTemp = iTemp | (iTemp2 << 28);
    m_pClientList[iClientH]->m_iStatus = iTemp;

    if (m_pClientList[iClientH]->m_iLevel > 100)
        if (m_pClientList[iClientH]->m_bIsPlayerCivil == true)
            ForceChangePlayMode(iClientH, false);

    m_pClientList[iClientH]->m_iNextLevelExp = m_iLevelExpTable[m_pClientList[iClientH]->m_iLevel + 1]; //iGetLevelExp(m_pClientList[iClientH]->m_iLevel + 1);

    CalcTotalItemEffect(iClientH, -1, true); //false
    iCalcTotalWeight(iClientH);

    // New 23/05/2004
    //GM Invi comes here
    if (m_pClientList[iClientH]->m_iAdminUserLevel > 0)
    {
        SetInvisibilityFlag(iClientH, DEF_OWNERTYPE_PLAYER, true);
    }

    if ((m_pClientList[iClientH]->m_iLevel > 2) && (m_pClientList[iClientH]->m_iAdminUserLevel == 0) &&
        (m_pClientList[iClientH]->m_iExp < iGetLevelExp(m_pClientList[iClientH]->m_iLevel - 1) - 3000))
    {
        try
        {
            wsprintf(G_cTxt, "Data Error: (%s) Player: (%s) CurrentExp: %d --- Minimum Exp: %d", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_iExp, (iGetLevelExp(m_pClientList[iClientH]->m_iLevel) - 1));
            PutHackLogFileList(G_cTxt);
            DeleteClient(iClientH, true, true);
        }
        catch (...)
        {
        }
        return;
    }

    iTotalPoints = 0;
    for (i = 0; i < DEF_MAXSKILLTYPE; i++)
        iTotalPoints += m_pClientList[iClientH]->m_cSkillMastery[i];
    if ((iTotalPoints - 21 > m_sCharSkillLimit) && (m_pClientList[iClientH]->m_iAdminUserLevel == 0))
    {
        try
        {
            wsprintf(G_cTxt, "Packet Editing: (%s) Player: (%s) - has more than allowed skill points (%d).", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName, iTotalPoints);
            PutHackLogFileList(G_cTxt);
            DeleteClient(iClientH, true, true);
        }
        catch (...)
        {
        }
        return;
    }

    CheckSpecialEvent(iClientH);
    bCheckMagicInt(iClientH);

    // GuildName
    if ((cGuildStatus == 0) && (memcmp(m_pClientList[iClientH]->m_cGuildName, "NONE", 4) != 0))
    {
        memset(m_pClientList[iClientH]->m_cGuildName, 0, sizeof(m_pClientList[iClientH]->m_cGuildName));
        strcpy(m_pClientList[iClientH]->m_cGuildName, "NONE");
        m_pClientList[iClientH]->m_iGuildRank = -1;
        m_pClientList[iClientH]->m_iGuildGUID = -1;

        SendNotifyMsg(0, iClientH, DEF_NOTIFY_GUILDDISBANDED, 0, 0, 0, m_pClientList[iClientH]->m_cGuildName);
    }

    if (m_pClientList[iClientH]->m_iQuest != 0)
    {
        cQuestRemain = (m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest]->m_iMaxCount - m_pClientList[iClientH]->m_iCurQuestCount);
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTCOUNTER, cQuestRemain, 0, 0, 0);
        _bCheckIsQuestCompleted(iClientH);
    }


    if (m_pClientList[iClientH] == nullptr)
    {
        wsprintf(cTxt, "<%d> InitPlayerData error - Socket error! Disconnected.", iClientH);
        log->info(cTxt);
        return;
    }

    memset(cData, 0, sizeof(cData));
    dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
    *dwp = MSGID_RESPONSE_INITPLAYER;
    wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
    *wp = DEF_MSGTYPE_CONFIRM;

    iRet = m_pClientList[iClientH]->send_msg(cData, 6);
    switch (iRet)
    {
        case DEF_XSOCKEVENT_QUENEFULL:
        case DEF_XSOCKEVENT_SOCKETERROR:
        case DEF_XSOCKEVENT_CRITICALERROR:
        case DEF_XSOCKEVENT_SOCKETCLOSED:
            // ## BUG POINT!!!
            wsprintf(cTxt, "<%d> InitPlayerData - Socket error! Disconnected.", iClientH);
            log->info(cTxt);

            DeleteClient(iClientH, false, true); //!!!!!
            return;
    }

    m_pClientList[iClientH]->m_bIsInitComplete = true;

    bSendMsgToLS(MSGID_ENTERGAMECONFIRM, iClientH);

    if (m_iTotalClients > DEF_MAXONESERVERUSERS)
    {
        switch (iDice(1, 2))
        {
            case 1:
                RequestTeleportHandler(iClientH, "2   ", "bisle", -1, -1);
                break;
            case 2:
                switch (m_pClientList[iClientH]->m_cSide)
                {
                    case 0: RequestTeleportHandler(iClientH, "2   ", "resurr1", -1, -1); break;
                    case 1: RequestTeleportHandler(iClientH, "2   ", "resurr1", -1, -1); break;
                    case 2: RequestTeleportHandler(iClientH, "2   ", "resurr2", -1, -1); break;
                }
                break;
        }
    }

    return;
}

void CGame::GameProcess()
{
    NpcProcess();
    MsgProcess();
    ForceRecallProcess();
    DelayEventProcess();
}

int CGame::_iComposePlayerDataFileContents(int iClientH, char * pData)
{
    SYSTEMTIME SysTime;
    char  cTxt[120], cTmp[21];
    POINT TempItemPosList[DEF_MAXITEMS];
    int   i, iPos;

    if (m_pClientList[iClientH] == nullptr) return 0;

    GetLocalTime(&SysTime);
    strcat(pData, "[FILE-DATE]\n\n");

    wsprintf(cTxt, "file-saved-date: %d %d %d %d %d\n", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    strcat(pData, "[NAME-ACCOUNT]\n\n");
    strcat(pData, "character-name     = ");
    strcat(pData, m_pClientList[iClientH]->m_cCharName);
    strcat(pData, "\n");
    strcat(pData, "account-name       = ");
    strcat(pData, m_pClientList[iClientH]->m_cAccountName);
    strcat(pData, "\n\n");

    strcat(pData, "[STATUS]\n\n");
    strcat(pData, "character-profile 	=");
    if (strlen(m_pClientList[iClientH]->m_cProfile) == 0)
    {
        strcat(pData, "__________");
    }
    else strcat(pData, m_pClientList[iClientH]->m_cProfile);
    strcat(pData, "\n");

    strcat(pData, "character-location   = ");
    strcat(pData, m_pClientList[iClientH]->m_cLocation);
    strcat(pData, "\n");

    strcat(pData, "character-guild-name = ");
    if (m_pClientList[iClientH]->m_iGuildRank != -1)
    {
        // GuildRank
        strcat(pData, m_pClientList[iClientH]->m_cGuildName);
    }
    else strcat(pData, "NONE");
    strcat(pData, "\n");

    // GUID 
    strcat(pData, "character-guild-GUID = ");
    if (m_pClientList[iClientH]->m_iGuildRank != -1)
    {
        // GuildRank
        memset(cTxt, 0, sizeof(cTxt));
        wsprintf(cTxt, "%d", m_pClientList[iClientH]->m_iGuildGUID);
        strcat(pData, cTxt);
    }
    else strcat(pData, "-1");
    strcat(pData, "\n");

    strcat(pData, "character-guild-rank = ");
    itoa(m_pClientList[iClientH]->m_iGuildRank, cTxt, 10);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    strcat(pData, "character-loc-map = ");
    strcat(pData, m_pClientList[iClientH]->m_cMapName);
    strcat(pData, "\n");
    strcat(pData, "character-loc-x   = ");
    itoa(m_pClientList[iClientH]->m_sX, cTxt, 10);
    strcat(pData, cTxt);
    strcat(pData, "\n");
    strcat(pData, "character-loc-y   = ");
    itoa(m_pClientList[iClientH]->m_sY, cTxt, 10);
    strcat(pData, cTxt);
    strcat(pData, "\n\n");
    //
    if (m_pClientList[iClientH]->m_iHP <= 0)
        m_pClientList[iClientH]->m_iHP = 30;

    wsprintf(cTxt, "character-HP       = %d", m_pClientList[iClientH]->m_iHP);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "character-MP       = %d", m_pClientList[iClientH]->m_iMP);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    if (m_pClientList[iClientH]->m_iSP < 0) m_pClientList[iClientH]->m_iSP = 0; // v1.1
    wsprintf(cTxt, "character-SP       = %d", m_pClientList[iClientH]->m_iSP);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "character-LEVEL    = %d", m_pClientList[iClientH]->m_iLevel);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "character-RATING   = %d", m_pClientList[iClientH]->m_iRating);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "character-STR      = %d", m_pClientList[iClientH]->m_iStr);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "character-INT      = %d", m_pClientList[iClientH]->m_iInt);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "character-VIT      = %d", m_pClientList[iClientH]->m_iVit);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "character-DEX      = %d", m_pClientList[iClientH]->m_iDex);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "character-MAG      = %d", m_pClientList[iClientH]->m_iMag);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "character-CHARISMA = %d", m_pClientList[iClientH]->m_iCharisma);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "character-LUCK     = %d", m_pClientList[iClientH]->m_iLuck);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "character-EXP      = %d", m_pClientList[iClientH]->m_iExp);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "character-LU_Pool  = %d", m_pClientList[iClientH]->m_iLU_Pool);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    /*
    wsprintf(cTxt, "character-LU_Str   = %d", m_pClientList[iClientH]->m_cLU_Str);
    strcat(pData, cTxt);
    strcat(pData,"\n");

    wsprintf(cTxt, "character-LU_Vit   = %d", m_pClientList[iClientH]->m_cLU_Vit);
    strcat(pData, cTxt);
    strcat(pData,"\n");

    wsprintf(cTxt, "character-LU_Dex   = %d", m_pClientList[iClientH]->m_cLU_Dex);
    strcat(pData, cTxt);
    strcat(pData,"\n");

    wsprintf(cTxt, "character-LU_Int   = %d", m_pClientList[iClientH]->m_cLU_Int);
    strcat(pData, cTxt);
    strcat(pData,"\n");

    wsprintf(cTxt, "character-LU_Mag   = %d", m_pClientList[iClientH]->m_cLU_Mag);
    strcat(pData, cTxt);
    strcat(pData,"\n");

    wsprintf(cTxt, "character-LU_Char  = %d", m_pClientList[iClientH]->m_cLU_Char);
    strcat(pData, cTxt);
    strcat(pData,"\n");
    */
    wsprintf(cTxt, "character-EK-Count = %d", m_pClientList[iClientH]->m_iEnemyKillCount);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "character-PK-Count = %d", m_pClientList[iClientH]->m_iPKCount);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "character-reward-gold = %d", m_pClientList[iClientH]->m_iRewardGold);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "character-downskillindex = %d", m_pClientList[iClientH]->m_iDownSkillIndex);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "character-IDnum1 = %d", m_pClientList[iClientH]->m_sCharIDnum1);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "character-IDnum2 = %d", m_pClientList[iClientH]->m_sCharIDnum2);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "character-IDnum3 = %d", m_pClientList[iClientH]->m_sCharIDnum3);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    /*
    wsprintf(cTxt, "party-rank = %d", m_pClientList[iClientH]->m_iPartyRank);
    strcat(pData, cTxt);
    strcat(pData,"\n");

    wsprintf(cTxt, "party-IDnum1 = %d", m_pClientList[iClientH]->m_sPartyIDnum1);
    strcat(pData, cTxt);
    strcat(pData,"\n");

    wsprintf(cTxt, "party-IDnum2 = %d", m_pClientList[iClientH]->m_sPartyIDnum2);
    strcat(pData, cTxt);
    strcat(pData,"\n");

    wsprintf(cTxt, "party-IDnum3 = %d", m_pClientList[iClientH]->m_sPartyIDnum3);
    strcat(pData, cTxt);
    strcat(pData,"\n\n");
    */

    strcat(pData, "sex-status       = ");
    itoa(m_pClientList[iClientH]->m_cSex, cTxt, 10);
    strcat(pData, cTxt);
    strcat(pData, "\n");
    strcat(pData, "skin-status      = ");
    itoa(m_pClientList[iClientH]->m_cSkin, cTxt, 10);
    strcat(pData, cTxt);
    strcat(pData, "\n");
    strcat(pData, "hairstyle-status = ");
    itoa(m_pClientList[iClientH]->m_cHairStyle, cTxt, 10);
    strcat(pData, cTxt);
    strcat(pData, "\n");
    strcat(pData, "haircolor-status = ");
    itoa(m_pClientList[iClientH]->m_cHairColor, cTxt, 10);
    strcat(pData, cTxt);
    strcat(pData, "\n");
    strcat(pData, "underwear-status = ");
    itoa(m_pClientList[iClientH]->m_cUnderwear, cTxt, 10);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "hunger-status    = %d", m_pClientList[iClientH]->m_iHungerStatus);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "timeleft-shutup  = %d", m_pClientList[iClientH]->m_iTimeLeft_ShutUp);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "timeleft-rating  = %d", m_pClientList[iClientH]->m_iTimeLeft_Rating);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "timeleft-force-recall  = %d", m_pClientList[iClientH]->m_iTimeLeft_ForceRecall);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "timeleft-firm-staminar = %d", m_pClientList[iClientH]->m_iTimeLeft_FirmStaminar);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "admin-user-level = %d", m_pClientList[iClientH]->m_iAdminUserLevel);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "penalty-block-date = %d %d %d", m_pClientList[iClientH]->m_iPenaltyBlockYear, m_pClientList[iClientH]->m_iPenaltyBlockMonth, m_pClientList[iClientH]->m_iPenaltyBlockDay);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "character-quest-number = %d", m_pClientList[iClientH]->m_iQuest);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "character-quest-ID     = %d", m_pClientList[iClientH]->m_iQuestID);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "current-quest-count    = %d", m_pClientList[iClientH]->m_iCurQuestCount);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "quest-reward-type      = %d", m_pClientList[iClientH]->m_iQuestRewardType);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "quest-reward-amount    = %d", m_pClientList[iClientH]->m_iQuestRewardAmount);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "character-contribution = %d", m_pClientList[iClientH]->m_iContribution);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "character-war-contribution = %d", m_pClientList[iClientH]->m_iWarContribution);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "character-quest-completed = %d", (int)m_pClientList[iClientH]->m_bIsQuestCompleted);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "special-event-id = %d", (int)m_pClientList[iClientH]->m_iSpecialEventID);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "super-attack-left = %d", (int)m_pClientList[iClientH]->m_iSuperAttackLeft);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    // v1.4311-3
    wsprintf(cTxt, "reserved-fightzone-id = %d %d %d", m_pClientList[iClientH]->m_iFightzoneNumber, m_pClientList[iClientH]->m_iReserveTime, m_pClientList[iClientH]->m_iFightZoneTicketNumber);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "special-ability-time = %d", m_pClientList[iClientH]->m_iSpecialAbilityTime);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "locked-map-name = %s", m_pClientList[iClientH]->m_cLockedMapName);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "locked-map-time = %d", m_pClientList[iClientH]->m_iLockedMapTime);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "crusade-job = %d", m_pClientList[iClientH]->m_iCrusadeDuty);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "crusade-GUID = %d", m_pClientList[iClientH]->m_dwCrusadeGUID);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    wsprintf(cTxt, "construct-point = %d", m_pClientList[iClientH]->m_iConstructionPoint);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    // v2.04 
    wsprintf(cTxt, "dead-penalty-time = %d", m_pClientList[iClientH]->m_iDeadPenaltyTime);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    // v2.06 12-4
    wsprintf(cTxt, "party-id = %d", m_pClientList[iClientH]->m_iPartyID);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    // v2.15
    wsprintf(cTxt, "gizon-item-upgade-left = %d", m_pClientList[iClientH]->m_iGizonItemUpgradeLeft);
    strcat(pData, cTxt);
    strcat(pData, "\n");

    strcat(pData, "\n\n");

    strcat(pData, "appr1 = ");
    itoa(m_pClientList[iClientH]->m_sAppr1, cTxt, 10);
    strcat(pData, cTxt);
    strcat(pData, "\n");
    strcat(pData, "appr2 = ");
    //m_pClientList[iClientH]->m_sAppr2 = m_pClientList[iClientH]->m_sAppr2 & 0x0FFF;
    itoa(m_pClientList[iClientH]->m_sAppr2, cTxt, 10);
    strcat(pData, cTxt);
    strcat(pData, "\n");
    strcat(pData, "appr3 = ");
    itoa(m_pClientList[iClientH]->m_sAppr3, cTxt, 10);
    strcat(pData, cTxt);
    strcat(pData, "\n");
    strcat(pData, "appr4 = ");
    itoa(m_pClientList[iClientH]->m_sAppr4, cTxt, 10);
    strcat(pData, cTxt);
    strcat(pData, "\n");
    // v1.4 ApprColor
    strcat(pData, "appr-color = ");
    itoa(m_pClientList[iClientH]->m_iApprColor, cTxt, 10);
    strcat(pData, cTxt);
    strcat(pData, "\n\n");

    strcat(pData, "[ITEMLIST]\n\n");

    for (i = 0; i < DEF_MAXITEMS; i++)
    { // v1.4
        TempItemPosList[i].x = 40;
        TempItemPosList[i].y = 30;
    }
    iPos = 0;

    for (i = 0; i < DEF_MAXITEMS; i++)
        if (m_pClientList[iClientH]->m_pItemList[i] != 0)
        {
            // v1.4
            TempItemPosList[iPos].x = m_pClientList[iClientH]->m_ItemPosList[i].x;
            TempItemPosList[iPos].y = m_pClientList[iClientH]->m_ItemPosList[i].y;
            iPos++;

            strcat(pData, "character-item = ");
            memset(cTmp, ' ', 21);
            strcpy(cTmp, m_pClientList[iClientH]->m_pItemList[i]->m_cName);
            cTmp[strlen(m_pClientList[iClientH]->m_pItemList[i]->m_cName)] = (char)' ';
            cTmp[20] = 0;
            strcat(pData, cTmp);
            strcat(pData, " ");
            itoa(m_pClientList[iClientH]->m_pItemList[i]->m_dwCount, cTxt, 10);
            strcat(pData, cTxt);
            strcat(pData, " ");
            itoa(m_pClientList[iClientH]->m_pItemList[i]->m_sTouchEffectType, cTxt, 10);
            strcat(pData, cTxt);
            strcat(pData, " ");
            itoa(m_pClientList[iClientH]->m_pItemList[i]->m_sTouchEffectValue1, cTxt, 10);
            strcat(pData, cTxt);
            strcat(pData, " ");
            itoa(m_pClientList[iClientH]->m_pItemList[i]->m_sTouchEffectValue2, cTxt, 10);
            strcat(pData, cTxt);
            strcat(pData, " ");
            itoa(m_pClientList[iClientH]->m_pItemList[i]->m_sTouchEffectValue3, cTxt, 10);
            strcat(pData, cTxt);
            strcat(pData, " ");
            itoa(m_pClientList[iClientH]->m_pItemList[i]->m_cItemColor, cTxt, 10);
            strcat(pData, cTxt);
            strcat(pData, " ");
            itoa(m_pClientList[iClientH]->m_pItemList[i]->m_sItemSpecEffectValue1, cTxt, 10);
            strcat(pData, cTxt);
            strcat(pData, " ");
            itoa(m_pClientList[iClientH]->m_pItemList[i]->m_sItemSpecEffectValue2, cTxt, 10);
            strcat(pData, cTxt);
            strcat(pData, " ");
            itoa(m_pClientList[iClientH]->m_pItemList[i]->m_sItemSpecEffectValue3, cTxt, 10);
            strcat(pData, cTxt);
            strcat(pData, " ");
            itoa(m_pClientList[iClientH]->m_pItemList[i]->m_wCurLifeSpan, cTxt, 10);
            strcat(pData, cTxt);
            strcat(pData, " ");
            itoa(m_pClientList[iClientH]->m_pItemList[i]->m_dwAttribute, cTxt, 10);
            strcat(pData, cTxt);
            strcat(pData, "\n");
        }
    strcat(pData, "\n\n");

    // v1.4
    for (i = 0; i < DEF_MAXITEMS; i++)
    {
        m_pClientList[iClientH]->m_ItemPosList[i].x = TempItemPosList[i].x;
        m_pClientList[iClientH]->m_ItemPosList[i].y = TempItemPosList[i].y;
    }

    for (i = 0; i < DEF_MAXBANKITEMS; i++)
        if (m_pClientList[iClientH]->m_pItemInBankList[i] != 0)
        {
            strcat(pData, "character-bank-item = ");
            memset(cTmp, ' ', 21);
            strcpy(cTmp, m_pClientList[iClientH]->m_pItemInBankList[i]->m_cName);
            cTmp[strlen(m_pClientList[iClientH]->m_pItemInBankList[i]->m_cName)] = (char)' ';
            cTmp[20] = 0;
            strcat(pData, cTmp);
            strcat(pData, " ");
            itoa(m_pClientList[iClientH]->m_pItemInBankList[i]->m_dwCount, cTxt, 10);
            strcat(pData, cTxt);
            strcat(pData, " ");
            itoa(m_pClientList[iClientH]->m_pItemInBankList[i]->m_sTouchEffectType, cTxt, 10);
            strcat(pData, cTxt);
            strcat(pData, " ");
            itoa(m_pClientList[iClientH]->m_pItemInBankList[i]->m_sTouchEffectValue1, cTxt, 10);
            strcat(pData, cTxt);
            strcat(pData, " ");
            itoa(m_pClientList[iClientH]->m_pItemInBankList[i]->m_sTouchEffectValue2, cTxt, 10);
            strcat(pData, cTxt);
            strcat(pData, " ");
            itoa(m_pClientList[iClientH]->m_pItemInBankList[i]->m_sTouchEffectValue3, cTxt, 10);
            strcat(pData, cTxt);
            strcat(pData, " ");
            itoa(m_pClientList[iClientH]->m_pItemInBankList[i]->m_cItemColor, cTxt, 10);
            strcat(pData, cTxt);
            strcat(pData, " ");
            itoa(m_pClientList[iClientH]->m_pItemInBankList[i]->m_sItemSpecEffectValue1, cTxt, 10);
            strcat(pData, cTxt);
            strcat(pData, " ");
            itoa(m_pClientList[iClientH]->m_pItemInBankList[i]->m_sItemSpecEffectValue2, cTxt, 10);
            strcat(pData, cTxt);
            strcat(pData, " ");
            itoa(m_pClientList[iClientH]->m_pItemInBankList[i]->m_sItemSpecEffectValue3, cTxt, 10);
            strcat(pData, cTxt);
            strcat(pData, " ");
            itoa(m_pClientList[iClientH]->m_pItemInBankList[i]->m_wCurLifeSpan, cTxt, 10);
            strcat(pData, cTxt);
            strcat(pData, " ");
            itoa(m_pClientList[iClientH]->m_pItemInBankList[i]->m_dwAttribute, cTxt, 10);
            strcat(pData, cTxt);
            strcat(pData, "\n");
        }
    strcat(pData, "\n\n");


    strcat(pData, "[MAGIC-SKILL-MASTERY]\n\n");

    strcat(pData, "//------------------012345678901234567890123456789012345678901234567890");
    strcat(pData, "\n");

    strcat(pData, "magic-mastery     = ");
    for (i = 0; i < DEF_MAXMAGICTYPE; i++)
    {
        wsprintf(cTxt, "%d", (int)m_pClientList[iClientH]->m_cMagicMastery[i]);
        strcat(pData, cTxt);
    }
    strcat(pData, "\n");

    strcat(pData, "skill-mastery     = ");


    for (i = 0; i < 60; i++)
    {
        memset(cTxt, 0, sizeof(cTxt));
        wsprintf(cTxt, "%d ", m_pClientList[iClientH]->m_cSkillMastery[i]);

        strcat(pData, cTxt);
    }
    strcat(pData, "\n");

    strcat(pData, "skill-SSN     = ");
    for (i = 0; i < 60; i++)
    {
        memset(cTxt, 0, sizeof(cTxt));
        wsprintf(cTxt, "%d ", m_pClientList[iClientH]->m_iSkillSSN[i]);

        strcat(pData, cTxt);
    }
    strcat(pData, "\n");

    strcat(pData, "[ITEM-EQUIP-STATUS]\n\n");
    strcat(pData, "item-equip-status = ");

    memset(cTxt, 0, sizeof(cTxt));
    strcpy(cTxt, "00000000000000000000000000000000000000000000000000");

    int iEP = 0;
    for (i = 0; i < DEF_MAXITEMS; i++)
        if (m_pClientList[iClientH]->m_pItemList[i] != 0)
        {
            if ((m_pClientList[iClientH]->m_bIsItemEquipped[i] == true) &&
                (m_pClientList[iClientH]->m_pItemList[i]->m_cItemType == DEF_ITEMTYPE_EQUIP))
            {
                cTxt[iEP] = '1';
            }
            iEP++;
        }
    strcat(pData, cTxt);
    strcat(pData, "\n");


    strcat(pData, "item-position-x = ");
    for (i = 0; i < DEF_MAXITEMS; i++)
    {
        memset(cTxt, 0, sizeof(cTxt));
        wsprintf(cTxt, "%d ", m_pClientList[iClientH]->m_ItemPosList[i].x);
        strcat(pData, cTxt);
    }
    strcat(pData, "\n");

    strcat(pData, "item-position-y = ");
    for (i = 0; i < DEF_MAXITEMS; i++)
    {
        memset(cTxt, 0, sizeof(cTxt));
        wsprintf(cTxt, "%d ", m_pClientList[iClientH]->m_ItemPosList[i].y);
        strcat(pData, cTxt);
    }
    strcat(pData, "\n\n");

    strcat(pData, "[EOF]");
    strcat(pData, "\n\n\n\n");

    return strlen(pData);
}

bool CGame::_bInitItemAttr(class CItem * pItem, char * pItemName)
{
    int i;
    char cTmpName[21];

    memset(cTmpName, 0, sizeof(cTmpName));
    strcpy(cTmpName, pItemName);

    for (i = 0; i < DEF_MAXITEMTYPES; i++)
        if (m_pItemConfigList[i] != 0)
        {
            if (memcmp(cTmpName, m_pItemConfigList[i]->m_cName, 20) == 0)
            {
                memset(pItem->m_cName, 0, sizeof(pItem->m_cName));
                strcpy(pItem->m_cName, m_pItemConfigList[i]->m_cName);
                pItem->m_cItemType = m_pItemConfigList[i]->m_cItemType;
                pItem->m_cEquipPos = m_pItemConfigList[i]->m_cEquipPos;
                pItem->m_sItemEffectType = m_pItemConfigList[i]->m_sItemEffectType;
                pItem->m_sItemEffectValue1 = m_pItemConfigList[i]->m_sItemEffectValue1;
                pItem->m_sItemEffectValue2 = m_pItemConfigList[i]->m_sItemEffectValue2;
                pItem->m_sItemEffectValue3 = m_pItemConfigList[i]->m_sItemEffectValue3;
                pItem->m_sItemEffectValue4 = m_pItemConfigList[i]->m_sItemEffectValue4;
                pItem->m_sItemEffectValue5 = m_pItemConfigList[i]->m_sItemEffectValue5;
                pItem->m_sItemEffectValue6 = m_pItemConfigList[i]->m_sItemEffectValue6;
                pItem->m_wMaxLifeSpan = m_pItemConfigList[i]->m_wMaxLifeSpan;
                pItem->m_wCurLifeSpan = pItem->m_wMaxLifeSpan;					// ÃƒÃ–ÃƒÃŠÂ·ÃŽ Â»Ã½Â¼ÂºÂµÃˆ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ Â¼Ã¶Â¸Ã­Ã€Âº ÃƒÃ–Â´Ã«Ã„Â¡Ã€ÃŒÂ´Ã™.
                pItem->m_sSpecialEffect = m_pItemConfigList[i]->m_sSpecialEffect;

                pItem->m_sSprite = m_pItemConfigList[i]->m_sSprite;
                pItem->m_sSpriteFrame = m_pItemConfigList[i]->m_sSpriteFrame;
                pItem->m_wPrice = m_pItemConfigList[i]->m_wPrice;
                pItem->m_wWeight = m_pItemConfigList[i]->m_wWeight;
                pItem->m_cApprValue = m_pItemConfigList[i]->m_cApprValue;
                pItem->m_cSpeed = m_pItemConfigList[i]->m_cSpeed;
                pItem->m_sLevelLimit = m_pItemConfigList[i]->m_sLevelLimit;
                pItem->m_cGenderLimit = m_pItemConfigList[i]->m_cGenderLimit;

                pItem->m_sSpecialEffectValue1 = m_pItemConfigList[i]->m_sSpecialEffectValue1;
                pItem->m_sSpecialEffectValue2 = m_pItemConfigList[i]->m_sSpecialEffectValue2;

                pItem->m_sRelatedSkill = m_pItemConfigList[i]->m_sRelatedSkill;
                pItem->m_cCategory = m_pItemConfigList[i]->m_cCategory;
                pItem->m_sIDnum = m_pItemConfigList[i]->m_sIDnum;

                pItem->m_bIsForSale = m_pItemConfigList[i]->m_bIsForSale;
                pItem->m_cItemColor = m_pItemConfigList[i]->m_cItemColor;

                return true;
            }
        }

    return false;
}

bool CGame::_bGetIsStringIsNumber(char * pStr)
{
    int i;
    for (i = 0; i < (int)strlen(pStr); i++)
        if ((pStr[i] != '-') && ((pStr[i] < (char)'0') || (pStr[i] > (char)'9'))) return false;

    return true;
}

bool CGame::_bReadMapInfoFiles(int iMapIndex)
{
    if (__bReadMapInfo(iMapIndex) == false)
    {
        return false;
    }

    return true;
}

bool CGame::bSetNpcFollowMode(char * pName, char * pFollowName, char cFollowOwnerType)
{
    int i, iIndex, iMapIndex, iFollowIndex;
    char cTmpName[11], cFollowSide;

    memset(cTmpName, 0, sizeof(cTmpName));
    iMapIndex = -1;
    iFollowIndex = -1;

    for (i = 1; i < DEF_MAXNPCS; i++)
        if ((m_pNpcList[i] != 0) && (memcmp(m_pNpcList[i]->m_cName, pName, 5) == 0))
        {
            iIndex = i;
            iMapIndex = m_pNpcList[i]->m_cMapIndex;
            goto NEXT_STEP_SNFM1;
        }

    NEXT_STEP_SNFM1:;

    switch (cFollowOwnerType)
    {
        case DEF_OWNERTYPE_NPC:
            for (i = 1; i < DEF_MAXNPCS; i++)
                if ((m_pNpcList[i] != 0) && (memcmp(m_pNpcList[i]->m_cName, pFollowName, 5) == 0))
                {
                    if (m_pNpcList[i]->m_cMapIndex != iMapIndex) return false;
                    iFollowIndex = i;
                    cFollowSide = m_pNpcList[i]->m_cSide;
                    goto NEXT_STEP_SNFM2;
                }
            break;

        case DEF_OWNERTYPE_PLAYER:
            for (i = 1; i < DEF_MAXCLIENTS; i++)
                if ((m_pClientList[i] != 0) && (memcmp(m_pClientList[i]->m_cCharName, pFollowName, 10) == 0))
                {
                    if (m_pClientList[i]->m_cMapIndex != iMapIndex) return false;
                    iFollowIndex = i;
                    cFollowSide = m_pClientList[i]->m_cSide;
                    goto NEXT_STEP_SNFM2;
                }
            break;
    }

    NEXT_STEP_SNFM2:;

    if ((iIndex == -1) || (iFollowIndex == -1)) return false;

    m_pNpcList[iIndex]->m_cMoveType = DEF_MOVETYPE_FOLLOW;
    m_pNpcList[iIndex]->m_cFollowOwnerType = cFollowOwnerType;
    m_pNpcList[iIndex]->m_iFollowOwnerIndex = iFollowIndex;
    m_pNpcList[iIndex]->m_cSide = cFollowSide;


    return true;
}

int CGame::bCreateNewNpc(char * pNpcName, char * pName, char * pMapName, short sClass, char cSA, char cMoveType, int * poX, int * poY, char * pWaypointList, RECT * pArea, int iSpotMobIndex, char cChangeSide, bool bHideGenMode, bool bIsSummoned, bool bFirmBerserk, bool bIsMaster, int iGuildGUID)
{
    int i, t, j, k, iMapIndex;
    char  cTmpName[11], cTxt[120];
    short sX, sY, sRange;
    bool  bFlag;
    SYSTEMTIME SysTime;

    if (strlen(pName) == 0)   return false;
    if (strlen(pNpcName) == 0) return false;
    GetLocalTime(&SysTime);
    memset(cTmpName, 0, sizeof(cTmpName));
    strcpy(cTmpName, pMapName);
    iMapIndex = -1;

    for (i = 0; i < DEF_MAXMAPS; i++)
        if (m_pMapList[i] != 0)
        {
            if (memcmp(m_pMapList[i]->m_cName, cTmpName, 10) == 0)
                iMapIndex = i;
        }

    if (iMapIndex == -1) return false;

    for (i = 1; i < DEF_MAXNPCS; i++)
        if (m_pNpcList[i] == 0)
        {
            m_pNpcList[i] = new class CNpc(pName);
            if (_bInitNpcAttr(m_pNpcList[i], pNpcName, sClass, cSA) == false)
            {
                wsprintf(cTxt, "(!) Not existing NPC creation request! (%s) Ignored.", pNpcName);
                log->info(cTxt);

                delete m_pNpcList[i];
                m_pNpcList[i] = 0;
                return false;
            }

            if (m_pNpcList[i]->m_cDayOfWeekLimit < 10)
            {
                if (m_pNpcList[i]->m_cDayOfWeekLimit != SysTime.wDayOfWeek)
                {
                    delete m_pNpcList[i];
                    m_pNpcList[i] = 0;
                    return false;
                }
            }

            switch (cMoveType)
            {
                case DEF_MOVETYPE_GUARD:
                case DEF_MOVETYPE_RANDOM:
                    if ((poX != 0) && (poY != 0) && (*poX != 0) && (*poY != 0))
                    {
                        sX = *poX;
                        sY = *poY;
                    }
                    else
                    {
                        for (j = 0; j <= 30; j++)
                        {
                            sX = (rand() % (m_pMapList[iMapIndex]->m_sSizeX - 50)) + 15;
                            sY = (rand() % (m_pMapList[iMapIndex]->m_sSizeY - 50)) + 15;

                            bFlag = true;
                            for (k = 0; k < DEF_MAXMGAR; k++)
                                if (m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[k].left != -1)
                                {
                                    if ((sX >= m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[k].left) &&
                                        (sX <= m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[k].right) &&
                                        (sY >= m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[k].top) &&
                                        (sY <= m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[k].bottom))
                                    {
                                        // Avoid Rect
                                        bFlag = false;
                                    }
                                }
                            if (bFlag == true) goto GET_VALIDLOC_SUCCESS;

                        }
                        delete m_pNpcList[i];
                        m_pNpcList[i] = 0;
                        return false;

                        GET_VALIDLOC_SUCCESS:;
                        // sX, sYÂ
                    }
                    break;

                case DEF_MOVETYPE_RANDOMAREA:
                    // pArea
                    sRange = (short)(pArea->right - pArea->left);
                    sX = (short)((rand() % sRange) + pArea->left);
                    sRange = (short)(pArea->bottom - pArea->top);
                    sY = (short)((rand() % sRange) + pArea->top);
                    break;

                case DEF_MOVETYPE_RANDOMWAYPOINT:
                    // 10
                    sX = (short)m_pMapList[iMapIndex]->m_WaypointList[pWaypointList[iDice(1, 10) - 1]].x;
                    sY = (short)m_pMapList[iMapIndex]->m_WaypointList[pWaypointList[iDice(1, 10) - 1]].y;
                    break;

                default:
                    // poX, poY
                    if ((poX != 0) && (poY != 0) && (*poX != 0) && (*poY != 0))
                    {
                        sX = *poX;
                        sY = *poY;
                    }
                    else
                    {
                        sX = (short)m_pMapList[iMapIndex]->m_WaypointList[pWaypointList[0]].x;
                        sY = (short)m_pMapList[iMapIndex]->m_WaypointList[pWaypointList[0]].y;
                    }
                    break;
            }

            if (bGetEmptyPosition(&sX, &sY, iMapIndex) == false)
            {

                delete m_pNpcList[i];
                m_pNpcList[i] = 0;
                return false;
            }

            if ((bHideGenMode == true) && (_iGetPlayerNumberOnSpot(sX, sY, iMapIndex, 7) != 0))
            {
                delete m_pNpcList[i];
                m_pNpcList[i] = 0;
                return false;
            }

            if ((poX != 0) && (poY != 0))
            {
                *poX = sX;
                *poY = sY;
            }

            m_pNpcList[i]->m_sX = sX;
            m_pNpcList[i]->m_sY = sY;

            m_pNpcList[i]->m_vX = sX;
            m_pNpcList[i]->m_vY = sY;

            // WaypointIndex
            for (t = 0; t < 10; t++)
                m_pNpcList[i]->m_iWayPointIndex[t] = pWaypointList[t];

            // WaypointIndex
            m_pNpcList[i]->m_cTotalWaypoint = 0;
            for (t = 0; t < 10; t++)
                if (m_pNpcList[i]->m_iWayPointIndex[t] != -1) m_pNpcList[i]->m_cTotalWaypoint++;

            if (pArea != 0)
            {
                // RANDOMAREA Copy
                SetRect(&m_pNpcList[i]->m_rcRandomArea, pArea->left, pArea->top, pArea->right, pArea->bottom);
            }

            switch (cMoveType)
            {
                case DEF_MOVETYPE_GUARD:
                    m_pNpcList[i]->m_dX = m_pNpcList[i]->m_sX;
                    m_pNpcList[i]->m_dY = m_pNpcList[i]->m_sY;
                    break;

                case DEF_MOVETYPE_SEQWAYPOINT:
                    m_pNpcList[i]->m_cCurWaypoint = 1;

                    m_pNpcList[i]->m_dX = (short)m_pMapList[iMapIndex]->m_WaypointList[m_pNpcList[i]->m_iWayPointIndex[m_pNpcList[i]->m_cCurWaypoint]].x;
                    m_pNpcList[i]->m_dY = (short)m_pMapList[iMapIndex]->m_WaypointList[m_pNpcList[i]->m_iWayPointIndex[m_pNpcList[i]->m_cCurWaypoint]].y;
                    break;

                case DEF_MOVETYPE_RANDOMWAYPOINT:
                    m_pNpcList[i]->m_cCurWaypoint = (rand() % (m_pNpcList[i]->m_cTotalWaypoint - 1)) + 1;

                    m_pNpcList[i]->m_dX = (short)m_pMapList[iMapIndex]->m_WaypointList[m_pNpcList[i]->m_iWayPointIndex[m_pNpcList[i]->m_cCurWaypoint]].x;
                    m_pNpcList[i]->m_dY = (short)m_pMapList[iMapIndex]->m_WaypointList[m_pNpcList[i]->m_iWayPointIndex[m_pNpcList[i]->m_cCurWaypoint]].y;
                    break;

                case DEF_MOVETYPE_RANDOMAREA:
                    m_pNpcList[i]->m_cCurWaypoint = 0;

                    sRange = (short)(m_pNpcList[i]->m_rcRandomArea.right - m_pNpcList[i]->m_rcRandomArea.left);
                    m_pNpcList[i]->m_dX = (short)((rand() % sRange) + m_pNpcList[i]->m_rcRandomArea.left);
                    sRange = (short)(m_pNpcList[i]->m_rcRandomArea.bottom - m_pNpcList[i]->m_rcRandomArea.top);
                    m_pNpcList[i]->m_dY = (short)((rand() % sRange) + m_pNpcList[i]->m_rcRandomArea.top);
                    break;

                case DEF_MOVETYPE_RANDOM:
                    m_pNpcList[i]->m_dX = (short)((rand() % (m_pMapList[iMapIndex]->m_sSizeX - 50)) + 15);
                    m_pNpcList[i]->m_dY = (short)((rand() % (m_pMapList[iMapIndex]->m_sSizeY - 50)) + 15);
                    break;
            }

            m_pNpcList[i]->m_tmp_iError = 0;

            m_pNpcList[i]->m_cMoveType = cMoveType;

            switch (m_pNpcList[i]->m_cActionLimit)
            {
                case 2:
                case 3:
                case 5:
                    m_pNpcList[i]->m_cBehavior = DEF_BEHAVIOR_STOP;

                    switch (m_pNpcList[i]->m_sType)
                    {
                        case 15: // ShopKeeper-W
                        case 19: // Gandlf
                        case 20: // Howard
                        case 24: // Tom
                        case 25: // William
                        case 26: // Kennedy
                            m_pNpcList[i]->m_cDir = 4 + iDice(1, 3) - 1;
                            break;

                        default:
                            m_pNpcList[i]->m_cDir = iDice(1, 8);
                            break;
                    }
                    break;

                default:
                    m_pNpcList[i]->m_cBehavior = DEF_BEHAVIOR_MOVE;
                    m_pNpcList[i]->m_cDir = 5;
                    break;
            }

            m_pNpcList[i]->m_iFollowOwnerIndex = 0;
            m_pNpcList[i]->m_iTargetIndex = 0;
            m_pNpcList[i]->m_cTurn = (rand() % 2);

            switch (m_pNpcList[i]->m_sType)
            {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                    m_pNpcList[i]->m_sAppr2 = 0xF000;
                    m_pNpcList[i]->m_sAppr2 = m_pNpcList[i]->m_sAppr2 | ((rand() % 13) << 4); // Â¹Â«Â±Ã¢
                    m_pNpcList[i]->m_sAppr2 = m_pNpcList[i]->m_sAppr2 | (rand() % 9); // Â¹Ã¦Ã†Ã 
                    break;

                case 36: // AGT-Aresden/AGT-Elvine
                case 37: // CGT-Aresden/CGT-Elvine
                case 38: // MS-Aresden/MS-Elvine
                case 39: // DT-Aresden/DT-Elvine
                    m_pNpcList[i]->m_sAppr2 = 3;
                    break;

                    //case 64: // Crop
                    //	m_pNpcList[i]->m_sAppr2 = 1;
                    //	break;
                    // appr2 = 2 seems to be enemy detection for crusade

                    /*case 91: // gate
                        m_pNpcList[i]->m_sAppr2 = 0xF000; // 10 aura no sphere 13 no name movable with magic (crash) test to 29
                        break;*/

                case 64: // Crop
                    m_pNpcList[i]->m_sAppr2 = 1; // 1 bud; 2 grown; 3 large 
                    break;

                    // case 66: // Wyvern
                    //	m_pNpcList[i]->m_iStatus = 0x00000010; 
                    //	// m_pNpcList[i]->m_cSide = 2;
                    //	break;

                default:
                    m_pNpcList[i]->m_sAppr2 = 0;
                    break;
            }

            m_pNpcList[i]->m_cMapIndex = (char)iMapIndex;
            m_pNpcList[i]->m_dwTime = timeGetTime() + (rand() % 10000);
            m_pNpcList[i]->m_dwActionTime += (rand() % 300);
            m_pNpcList[i]->m_dwMPupTime = timeGetTime();
            m_pNpcList[i]->m_dwHPupTime = m_pNpcList[i]->m_dwMPupTime;
            m_pNpcList[i]->m_sBehaviorTurnCount = 0;
            m_pNpcList[i]->m_bIsSummoned = bIsSummoned;
            m_pNpcList[i]->m_bIsMaster = bIsMaster;
            if (bIsSummoned == true)
                m_pNpcList[i]->m_dwSummonedTime = timeGetTime();

            if (bFirmBerserk == true)
            {
                m_pNpcList[i]->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] = 1;
                m_pNpcList[i]->m_iStatus = m_pNpcList[i]->m_iStatus | 0x20;
                //iExpRoll = iDice(m_pNpcList[i]->m_iExpDiceMin, m_pNpcList[i]->m_iExpDiceMax);
                //iExpRoll *= 2;
            }

            // !!!
            if (cChangeSide != -1) m_pNpcList[i]->m_cSide = cChangeSide;

            m_pNpcList[i]->m_cBravery = (rand() % 3) + m_pNpcList[i]->m_iMinBravery;
            m_pNpcList[i]->m_iSpotMobIndex = iSpotMobIndex;
            m_pNpcList[i]->m_iGuildGUID = iGuildGUID;
            //testcode
            if (iGuildGUID != 0)
            {
                wsprintf(G_cTxt, "Summon War Unit(%d) GUID(%d)", m_pNpcList[i]->m_sType, iGuildGUID);
                log->info(G_cTxt);
            }

            m_pMapList[iMapIndex]->SetOwner(i, DEF_OWNERTYPE_NPC, sX, sY);
            m_pMapList[iMapIndex]->m_iTotalActiveObject++;
            m_pMapList[iMapIndex]->m_iTotalAliveObject++;

            switch (m_pNpcList[i]->m_sType)
            {
                case 36: // AGT-Aresden/AGT-Elvine
                case 37: // CGT-Aresden/CGT-Elvine
                case 38: // MS-Aresden/MS-Elvine
                case 39: // DT-Aresden/DT-Elvine
                case 42: // ManaStone
                    m_pMapList[iMapIndex]->bAddCrusadeStructureInfo(m_pNpcList[i]->m_sType, sX, sY, m_pNpcList[i]->m_cSide);
                    break;

                case 64:
                    m_pMapList[iMapIndex]->bAddCropsTotalSum();
                    break;
            }

            SendEventToNearClient_TypeA(i, DEF_OWNERTYPE_NPC, MSGID_EVENT_LOG, DEF_MSGTYPE_CONFIRM, 0, 0, 0);
            return true;
        }

    return false;
}

void CGame::NpcProcess()
{
    int i, iMaxHP;
    uint32_t dwTime, dwActionTime;

    dwTime = timeGetTime();

    for (i = 1; i < DEF_MAXNPCS; i++)
    {

        if (m_pNpcList[i] != 0)
        {
            if (m_pNpcList[i]->m_cBehavior == DEF_BEHAVIOR_ATTACK)
            {
                switch (iDice(1, 7))
                {
                    case 1: dwActionTime = m_pNpcList[i]->m_dwActionTime; break;
                    case 2: dwActionTime = m_pNpcList[i]->m_dwActionTime - 100; break;
                    case 3: dwActionTime = m_pNpcList[i]->m_dwActionTime - 200; break;
                    case 4: dwActionTime = m_pNpcList[i]->m_dwActionTime - 300; break;
                    case 5: dwActionTime = m_pNpcList[i]->m_dwActionTime - 400; break;
                    case 6: dwActionTime = m_pNpcList[i]->m_dwActionTime - 600; break;
                    case 7: dwActionTime = m_pNpcList[i]->m_dwActionTime - 700; break;
                }
                if (dwActionTime < 600) dwActionTime = 600;
            }
            else dwActionTime = m_pNpcList[i]->m_dwActionTime;

            // v1.42
            if (m_pNpcList[i]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] != 0)
                dwActionTime += (dwActionTime / 2);
        }

        if ((m_pNpcList[i] != 0) && ((dwTime - m_pNpcList[i]->m_dwTime) > dwActionTime))
        {
            m_pNpcList[i]->m_dwTime = dwTime;

            if (abs(m_pNpcList[i]->m_cMagicLevel) > 0)
            {
                if ((dwTime - m_pNpcList[i]->m_dwMPupTime) > DEF_MPUPTIME)
                {
                    m_pNpcList[i]->m_dwMPupTime = dwTime;

                    //if (m_pNpcList[i]->m_bIsSummoned == false)
                    m_pNpcList[i]->m_iMana += iDice(1, (m_pNpcList[i]->m_iMaxMana / 5));

                    if (m_pNpcList[i]->m_iMana > m_pNpcList[i]->m_iMaxMana)
                        m_pNpcList[i]->m_iMana = m_pNpcList[i]->m_iMaxMana;
                }
            }

            // HP
            if (((dwTime - m_pNpcList[i]->m_dwHPupTime) > DEF_HPUPTIME) && (m_pNpcList[i]->m_bIsKilled == false))
            {
                m_pNpcList[i]->m_dwHPupTime = dwTime;

                iMaxHP = iDice(m_pNpcList[i]->m_iHitDice, 8) + m_pNpcList[i]->m_iHitDice;
                if (m_pNpcList[i]->m_iHP < iMaxHP)
                {

                    if (m_pNpcList[i]->m_bIsSummoned == false)
                        m_pNpcList[i]->m_iHP += iDice(1, m_pNpcList[i]->m_iHitDice); // Hit Point

                    if (m_pNpcList[i]->m_iHP > iMaxHP) m_pNpcList[i]->m_iHP = iMaxHP;
                    if (m_pNpcList[i]->m_iHP <= 0)     m_pNpcList[i]->m_iHP = 1;
                }
            }

            switch (m_pNpcList[i]->m_cBehavior)
            {
                case DEF_BEHAVIOR_DEAD:
                    NpcBehavior_Dead(i);
                    break;
                case DEF_BEHAVIOR_STOP:
                    NpcBehavior_Stop(i);
                    break;
                case DEF_BEHAVIOR_MOVE:
                    NpcBehavior_Move(i);
                    break;
                case DEF_BEHAVIOR_ATTACK:
                    NpcBehavior_Attack(i);
                    break;
                case DEF_BEHAVIOR_FLEE:
                    NpcBehavior_Flee(i);
                    break;
            }

            // !!! m_pNpcList
            if ((m_pNpcList[i] != 0) && (m_pNpcList[i]->m_iHP != 0) && (m_pNpcList[i]->m_bIsSummoned == true))
            {
                switch (m_pNpcList[i]->m_sType)
                {
                    case 29:
                        if ((dwTime - m_pNpcList[i]->m_dwSummonedTime) > 1000 * 90)
                            NpcKilledHandler(0, 0, i, 0);
                        break;

                    default:
                        if ((dwTime - m_pNpcList[i]->m_dwSummonedTime) > DEF_SUMMONTIME)
                            NpcKilledHandler(0, 0, i, 0);
                        break;
                }
            }
        }
    }
}

char CGame::cGetNextMoveDir(short sX, short sY, short dstX, short dstY, char cMapIndex, char cTurn, int * pError)
{
    char  cDir, cTmpDir;
    int   aX, aY, dX, dY;
    int   i, iResX, iResY;

    if ((sX == dstX) && (sY == dstY)) return 0;

    dX = sX;
    dY = sY;

    if ((abs(dX - dstX) <= 1) && (abs(dY - dstY) <= 1))
    {
        iResX = dstX;
        iResY = dstY;
    }
    else m_Misc.GetPoint(dX, dY, dstX, dstY, &iResX, &iResY, pError);

    cDir = m_Misc.cGetNextMoveDir(dX, dY, iResX, iResY);

    if (cTurn == 0)
        for (i = cDir; i <= cDir + 7; i++)
        {
            cTmpDir = i;
            if (cTmpDir > 8) cTmpDir -= 8;
            aX = _tmp_cTmpDirX[cTmpDir];
            aY = _tmp_cTmpDirY[cTmpDir];
            if (m_pMapList[cMapIndex]->bGetMoveable(dX + aX, dY + aY) == true) return cTmpDir;
        }

    if (cTurn == 1)
        for (i = cDir; i >= cDir - 7; i--)
        {
            cTmpDir = i;
            if (cTmpDir < 1) cTmpDir += 8;
            aX = _tmp_cTmpDirX[cTmpDir];
            aY = _tmp_cTmpDirY[cTmpDir];
            if (m_pMapList[cMapIndex]->bGetMoveable(dX + aX, dY + aY) == true) return cTmpDir;
        }

    return 0;
}

char _tmp_cEmptyPosX[] = { 0, 1, 1, 0, -1, -1, -1, 0 ,1, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2, -1, 0, 1, 2 };
char _tmp_cEmptyPosY[] = { 0, 0, 1, 1, 1, 0, -1, -1, -1, -1, 0, 1, 2, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2 };

bool CGame::bGetEmptyPosition(short * pX, short * pY, char cMapIndex)
{
    int i;
    short sX, sY;

    for (i = 0; i < 25; i++)
        if ((m_pMapList[cMapIndex]->bGetMoveable(*pX + _tmp_cEmptyPosX[i], *pY + _tmp_cEmptyPosY[i]) == true) &&
            (m_pMapList[cMapIndex]->bGetIsTeleport(*pX + _tmp_cEmptyPosX[i], *pY + _tmp_cEmptyPosY[i]) == false))
        {
            sX = *pX + _tmp_cEmptyPosX[i];
            sY = *pY + _tmp_cEmptyPosY[i];
            *pX = sX;
            *pY = sY;
            return true;
        }


    GetMapInitialPoint(cMapIndex, &sX, &sY);
    *pX = sX;
    *pY = sY;

    return false;
}

void CGame::TargetSearch(int iNpcH, short * pTarget, char * pTargetType)
{
    int ix, iy, iPKCount;
    short sX, sY, rX, rY, dX, dY;
    short sOwner, sTargetOwner, sDistance, sTempDistance;
    char  cOwnerType, cTargetType, cTargetSide;
    int   iInv;

    sTargetOwner = 0;
    cTargetType = 0;
    sDistance = 100;

    sX = m_pNpcList[iNpcH]->m_sX;
    sY = m_pNpcList[iNpcH]->m_sY;

    rX = m_pNpcList[iNpcH]->m_sX - m_pNpcList[iNpcH]->m_cTargetSearchRange;
    rY = m_pNpcList[iNpcH]->m_sY - m_pNpcList[iNpcH]->m_cTargetSearchRange;

    for (ix = rX; ix < rX + m_pNpcList[iNpcH]->m_cTargetSearchRange * 2 + 1; ix++)
        for (iy = rY; iy < rY + m_pNpcList[iNpcH]->m_cTargetSearchRange * 2 + 1; iy++)
        {

            m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwner, &cOwnerType, ix, iy);
            if (sOwner != 0)
            {
                if ((sOwner == iNpcH) && (cOwnerType == DEF_OWNERTYPE_NPC)) break; // Ã€ÃšÂ±Ã¢ Ã€ÃšÂ½Ã…Ã€ÃŒÂ¸Ã© Â¹Â«Â½Ãƒ 

                iPKCount = 0;
                switch (cOwnerType)
                {
                    case DEF_OWNERTYPE_PLAYER:
                        if (m_pClientList[sOwner] == 0)
                        {
                            m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearOwner(5, sOwner, DEF_OWNERTYPE_PLAYER, ix, iy);
                        }
                        else
                        {
                            if (m_pClientList[sOwner]->m_iAdminUserLevel > 0) goto SKIP_SEARCH;

                            dX = m_pClientList[sOwner]->m_sX;
                            dY = m_pClientList[sOwner]->m_sY;
                            cTargetSide = m_pClientList[sOwner]->m_cSide;
                            iPKCount = m_pClientList[sOwner]->m_iPKCount;
                            iInv = m_pClientList[sOwner]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY];
                        }
                        break;

                    case DEF_OWNERTYPE_NPC:
                        if (m_pNpcList[sOwner] == 0)
                        {
                            m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearOwner(6, sOwner, DEF_OWNERTYPE_NPC, ix, iy);
                        }
                        else
                        {
                            dX = m_pNpcList[sOwner]->m_sX;
                            dY = m_pNpcList[sOwner]->m_sY;
                            cTargetSide = m_pNpcList[sOwner]->m_cSide;
                            iPKCount = 0;
                            iInv = m_pNpcList[sOwner]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY];

                            if (m_pNpcList[iNpcH]->m_sType == 21)
                            {
                                if (_iCalcPlayerNum(m_pNpcList[sOwner]->m_cMapIndex, dX, dY, 2) != 0)
                                {
                                    sOwner = 0;
                                    cOwnerType = 0;
                                }
                            }
                        }
                        break;
                }

                if (m_pNpcList[iNpcH]->m_cSide < 10)
                {
                    // NPC
                    if (cTargetSide == 0)
                    {
                        if (iPKCount == 0) goto SKIP_SEARCH;
                    }
                    else
                    {
                        if ((iPKCount == 0) && (cTargetSide == m_pNpcList[iNpcH]->m_cSide)) goto SKIP_SEARCH;
                        if (m_pNpcList[iNpcH]->m_cSide == 0) goto SKIP_SEARCH;
                    }
                }
                else
                {
                    if ((cOwnerType == DEF_OWNERTYPE_NPC) && (cTargetSide == 0)) goto SKIP_SEARCH;
                    if (cTargetSide == m_pNpcList[iNpcH]->m_cSide) goto SKIP_SEARCH;
                }

                if ((iInv != 0) && (m_pNpcList[iNpcH]->m_cSpecialAbility != 1)) goto SKIP_SEARCH;

                if (abs(sX - dX) >= abs(sY - dY))
                    sTempDistance = abs(sX - dX);
                else sTempDistance = abs(sY - dY);

                if (sTempDistance < sDistance)
                {
                    sDistance = sTempDistance;
                    sTargetOwner = sOwner;
                    cTargetType = cOwnerType;
                }
                SKIP_SEARCH:;
            }
        }

    *pTarget = sTargetOwner;
    *pTargetType = cTargetType;
    return;
}

void CGame::RemoveFromTarget(short sTargetH, char cTargetType, int iCode)
{
    int i;
    uint32_t dwTime = timeGetTime();

    for (i = 0; i < DEF_MAXNPCS; i++)
        if (m_pNpcList[i] != 0)
        {
            if ((m_pNpcList[i]->m_iGuildGUID != 0) && (cTargetType == DEF_OWNERTYPE_PLAYER) &&
                (m_pClientList[sTargetH]->m_iGuildGUID == m_pNpcList[i]->m_iGuildGUID))
            {

                if (m_pNpcList[i]->m_cActionLimit == 0)
                {
                    m_pNpcList[i]->m_bIsSummoned = true;
                    m_pNpcList[i]->m_dwSummonedTime = dwTime;
                }
            }

            if ((m_pNpcList[i]->m_iTargetIndex == sTargetH) &&
                (m_pNpcList[i]->m_cTargetType == cTargetType))
            {

                switch (iCode)
                {
                    case DEF_MAGICTYPE_INVISIBILITY:
                        if (m_pNpcList[i]->m_cSpecialAbility == 1)
                        {
                        }
                        else
                        {
                            m_pNpcList[i]->m_cBehavior = DEF_BEHAVIOR_MOVE;
                            m_pNpcList[i]->m_iTargetIndex = 0;
                            m_pNpcList[i]->m_cTargetType = 0;
                        }
                        break;

                    default:
                        m_pNpcList[i]->m_cBehavior = DEF_BEHAVIOR_MOVE;
                        m_pNpcList[i]->m_iTargetIndex = 0;
                        m_pNpcList[i]->m_cTargetType = 0;
                        break;
                }
            }
        }
}

int CGame::iGetDangerValue(int iNpcH, short dX, short dY)
{
    int ix, iy, iDangerValue;
    short sOwner, sDOType;
    char  cOwnerType;
    uint32_t dwRegisterTime;

    if (m_pNpcList[iNpcH] == 0) return false;

    iDangerValue = 0;

    for (ix = dX - 2; ix <= dX + 2; ix++)
        for (iy = dY - 2; iy <= dY + 2; iy++)
        {
            m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwner, &cOwnerType, ix, iy);
            m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->bGetDynamicObject(ix, iy, &sDOType, &dwRegisterTime);

            if (sDOType == 1) iDangerValue++;

            switch (cOwnerType)
            {
                case 0:
                    break;
                case DEF_OWNERTYPE_PLAYER:
                    if (m_pClientList[sOwner] == 0) break;
                    if (m_pNpcList[iNpcH]->m_cSide != m_pClientList[sOwner]->m_cSide)
                        iDangerValue++;
                    else iDangerValue--;
                    break;
                case DEF_OWNERTYPE_NPC:
                    if (m_pNpcList[sOwner] == 0) break;
                    if (m_pNpcList[iNpcH]->m_cSide != m_pNpcList[sOwner]->m_cSide)
                        iDangerValue++;
                    else iDangerValue--;
                    break;
            }
        }

    return iDangerValue;
}

void CGame::MsgProcess()
{
    char * pData, cFrom, cKey;
    uint32_t    dwMsgSize, * dwpMsgID;
    uint16_t * wpMsgType, * wp;
    int      i, iClientH;
    char   m_msgBuff[1000];

    if ((m_bF5pressed == true) && (m_bF1pressed == true))
    {
        log->info("(XXX) RELOADING CONFIGS MANUALY...");
        for (i = 1; i < DEF_MAXCLIENTS; i++)
            if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_bIsInitComplete == true))
            {
                bSendMsgToLS(MSGID_REQUEST_SAVEPLAYERDATA, i);
            }
        bInit();
    }

    if ((m_bF1pressed == true) && (m_bF4pressed == true) && (m_bOnExitProcess == false))
    {
        m_cShutDownCode = 2;
        m_bOnExitProcess = true;
        m_dwExitProcessTime = timeGetTime();
        log->info("(!) GAME SERVER SHUTDOWN PROCESS BEGIN(by Local command)!!!");
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

    memset(m_pMsgBuffer, 0, DEF_MSGBUFFERSIZE + 1);
    pData = (char *)m_pMsgBuffer;

    m_iCurMsgs = 0;
    while (bGetMsgQuene(&cFrom, pData, &dwMsgSize, &iClientH, &cKey) == true)
    {

        //v1.31
        m_iCurMsgs++;
        if (m_iCurMsgs > m_iMaxMsgs) m_iMaxMsgs = m_iCurMsgs;

        switch (cFrom)
        {
            case DEF_MSGFROM_GATESERVER:
                dwpMsgID = (uint32_t *)(pData + DEF_INDEX4_MSGID);
                wpMsgType = (uint16_t *)(pData + DEF_INDEX2_MSGTYPE);

                switch (*dwpMsgID)
                {
                    // New 07/05/2004
                    case MSGID_PARTYOPERATION:
                        PartyOperationResultHandler(pData);
                        break;

                    case MSGID_SERVERSTOCKMSG:
                        //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_GATESERVER -> MSGID_SERVERSTOCKMSG");
                        ServerStockMsgHandler(pData);
                        break;

                    case MSGID_SENDSERVERSHUTDOWNMSG:
                        //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_GATESERVER -> MSGID_SENDSERVERSHUTDOWNMSG");
                        wp = (uint16_t *)(pData + DEF_INDEX2_MSGTYPE + 2);
                        switch (*wp)
                        {
                            case 1:
                                log->info("(!) Send server shutdown announcement - 1.");
                                for (i = 1; i < DEF_MAXCLIENTS; i++)
                                    if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_bIsInitComplete == true))
                                    {
                                        SendNotifyMsg(0, i, DEF_NOTIFY_SERVERSHUTDOWN, 1, 0, 0, 0);
                                    }
                                break;

                            case 2:
                                log->info("(!) Send server shutdown announcement - 2.");
                                for (i = 1; i < DEF_MAXCLIENTS; i++)
                                    if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_bIsInitComplete == true))
                                    {
                                        SendNotifyMsg(0, i, DEF_NOTIFY_SERVERSHUTDOWN, 2, 0, 0, 0);
                                    }
                                break;
                        }

                        // WLS
                        bSendMsgToLS(MSGID_SENDSERVERSHUTDOWNMSG, 0, 0);
                        break;

                    case MSGID_GAMESERVERSHUTDOWNED:
                        //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_GATESERVER -> MSGID_GAMESERVERSHUTDOWNED");
                        m_cShutDownCode = 1;
                        m_bOnExitProcess = true;
                        m_dwExitProcessTime = timeGetTime();
                        log->info("(!) GAME SERVER SHUTDOWN PROCESS BEGIN(by Global command)!!!");
                        bSendMsgToLS(MSGID_GAMESERVERSHUTDOWNED, 0);
                        // OccupyFlag
                        if (m_iMiddlelandMapIndex > 0)
                        {
                            // Crusade
                            SaveOccupyFlagData();
                            //bSendMsgToLS(MSGID_REQUEST_SAVEARESDENOCCUPYFLAGDATA, 0, 0);
                            //bSendMsgToLS(MSGID_REQUEST_SAVEELVINEOCCUPYFLAGDATA, 0, 0);
                        }
                        break;

                    case MSGID_TOTALGAMESERVERCLIENTS:
                        wp = (uint16_t *)(pData + DEF_INDEX2_MSGTYPE + 2);
                        m_iTotalGameServerClients = (int)*wp;
                        if (m_iTotalGameServerClients > m_iTotalGameServerMaxClients)
                            m_iTotalGameServerMaxClients = m_iTotalGameServerClients;
                        break;

                    case MSGID_RESPONSE_REGISTERGAMESERVER:
                        //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_GATESERVER -> MSGID_RESPONSE_REGISTERGAMESERVER");
                        switch (*wpMsgType)
                        {
                            case DEF_MSGTYPE_CONFIRM:
                                //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_GATESERVER -> MSGID_RESPONSE_REGISTERGAMESERVER -> DEF_MSGTYPE_CONFIRM");
                                log->info("(!) Game Server registration to Gate Server - Success!");
                                m_bIsGateSockAvailable = true;
                                break;

                            case DEF_MSGTYPE_REJECT:
                                //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_GATESERVER -> MSGID_RESPONSE_REGISTERGAMESERVER -> DEF_MSGTYPE_REJECT");
                                log->info("(!) Game Server registration to Gate Server - Fail!");
                                log->info(" ");
                                log->info("(!!!) STOPPED!");
                                break;
                        }

                        //wsprintf(G_cTxt, "%d %d %d %d %d %d %d", (int)m_bIsGameStarted, (int)m_bIsItemAvailable, m_bIsNpcAvailable, (int)m_bIsGateSockAvailable, (int)m_bIsLogSockAvailable, (int)m_bIsMagicAvailable, (int)m_bIsSkillAvailable);
                        //log->info(G_cTxt);

                        /*
                        if ((m_bIsGameStarted == false) && (m_bIsItemAvailable == true) &&
                            (m_bIsNpcAvailable == true) && (m_bIsGateSockAvailable == true) &&
                            (m_bIsLogSockAvailable == true) && (m_bIsMagicAvailable == true) &&
                            (m_bIsSkillAvailable == true) && (m_bIsPortionAvailable == true) &&
                            (m_bIsQuestAvailable == true) ) {
                            SendMessage(m_hWnd, WM_USER_STARTGAMESIGNAL, 0, 0);
                            m_bIsGameStarted = true;
                        }
                        */
                        break;
                }
                break;

            case DEF_MSGFROM_CLIENT:
                /*m_pClientList[iClientH]->m_cConnectionCheck++;
                if (m_pClientList[iClientH]->m_cConnectionCheck > 3) {
                    wsprintf(G_cTxt, "Client Hex Edit: (%s) Player: (%s) - has removed 3203203 (check connection handler).", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
                    PutHackLogFileList(G_cTxt);
                    DeleteClient(iClientH, true, true);
                    break;
                }*/

                dwpMsgID = (uint32_t *)(pData + DEF_INDEX4_MSGID);
                switch (*dwpMsgID)
                { // 84148741

                    case DEF_REQUEST_RESURRECTPLAYER_YES:
                        RequestResurrectPlayer(iClientH, true);
                        break;

                    case DEF_REQUEST_RESURRECTPLAYER_NO:
                        RequestResurrectPlayer(iClientH, false);
                        break;

                    case MSGID_REQUEST_SELLITEMLIST:
                        RequestSellItemListHandler(iClientH, pData);
                        break;

                    case MSGID_REQUEST_RESTART:
                        RequestRestartHandler(iClientH);
                        break;

                    case MSGID_REQUEST_PANNING:
                        iRequestPanningMapDataRequest(iClientH, pData);
                        break;

                    case MSGID_REQUEST_NOTICEMENT:
                        RequestNoticementHandler(iClientH, pData);
                        break;

                    case MSGID_BWM_COMMAND_SHUTUP:
                        _BWM_Command_Shutup(pData);
                        break;

                    case MSGID_BWM_INIT:
                        _BWM_Init(iClientH, pData);
                        break;

                    case MSGID_REQUEST_SETITEMPOS:
                        _SetItemPos(iClientH, pData);
                        break;

                    case MSGID_ADMINUSER:
                        RequestAdminUserMode(iClientH, pData);
                        break;

                    case MSGID_REQUEST_FULLOBJECTDATA:
                        RequestFullObjectData(iClientH, pData);
                        break;

                    case MSGID_REQUEST_RETRIEVEITEM:
                        RequestRetrieveItemHandler(iClientH, pData);
                        break;

                    case MSGID_REQUEST_CIVILRIGHT:
                        RequestCivilRightHandler(iClientH, pData);
                        break;

                    case MSGID_REQUEST_TELEPORT:
                        RequestTeleportHandler(iClientH, pData);
                        break;

                    case MSGID_REQUEST_INITPLAYER:
                        RequestInitPlayerHandler(iClientH, pData, cKey);
                        break;

                    case MSGID_REQUEST_INITDATA:
                        RequestInitDataHandler(iClientH, pData, cKey);
                        break;

                    case MSGID_COMMAND_COMMON:
                        ClientCommonHandler(iClientH, pData);
                        break;

                    case MSGID_COMMAND_MOTION:
                        ClientMotionHandler(iClientH, pData);
                        break;

                    case MSGID_COMMAND_CHECKCONNECTION:
                        CheckConnectionHandler(iClientH, pData);
                        break;

                    case MSGID_COMMAND_CHATMSG:
                        ChatMsgHandler(iClientH, pData, dwMsgSize);
                        break;

                    case MSGID_REQUEST_CREATENEWGUILD:
                        RequestCreateNewGuildHandler(iClientH, pData, dwMsgSize);
                        break;

                    case MSGID_REQUEST_DISBANDGUILD:
                        RequestDisbandGuildHandler(iClientH, pData, dwMsgSize);
                        break;

                    case MSGID_REQUEST_FIGHTZONE_RESERVE:
                        FightzoneReserveHandler(iClientH, pData, dwMsgSize);
                        break;

                    case MSGID_LEVELUPSETTINGS:
                        LevelUpSettingsHandler(iClientH, pData, dwMsgSize);
                        break;

                    case MSGID_STATECHANGEPOINT:
                        StateChangeHandler(iClientH, pData, dwMsgSize);
                        break;

                    case MSGID_REQUEST_HELDENIANTELEPORT:
                        RequestHeldenianTeleport(iClientH, pData, dwMsgSize);
                        break;


                    case MSGID_REQUEST_CITYHALLTELEPORT:
                        if (memcmp(m_pClientList[iClientH]->m_cLocation, "aresden", 7) == 0)
                        {
                            RequestTeleportHandler(iClientH, "2   ", "dglv2", 263, 258);
                        }
                        else if (memcmp(m_pClientList[iClientH]->m_cLocation, "elvine", 6) == 0)
                        {
                            RequestTeleportHandler(iClientH, "2   ", "dglv2", 209, 258);
                        }
                        break;

                    default:
                        wsprintf(m_msgBuff, "Unknown message received! (0x%.8X) Delete Client", *dwpMsgID);
                        log->info(m_msgBuff);
                        DeleteClient(iClientH, true, true); // v1.4
                        break;
                }
                break;

            case DEF_MSGFROM_LOGSERVER:
                dwpMsgID = (uint32_t *)(pData + DEF_INDEX4_MSGID);
                wpMsgType = (uint16_t *)(pData + DEF_INDEX2_MSGTYPE);

                switch (*dwpMsgID)
                {
                    case MSGID_REQUEST_CHECKACCOUNTPASSWORD:
                        //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_REQUEST_CHECKACCOUNTPASSWORD");
                        RequestCheckAccountPasswordHandler(pData, dwMsgSize);
                        break;

                    case MSGID_REQUEST_FORCEDISCONECTACCOUNT:
                        //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_REQUEST_FORCEDISCONECTACCOUNT");
                        wpMsgType = (uint16_t *)(pData + DEF_INDEX2_MSGTYPE);
                        ForceDisconnectAccount((char *)(pData + DEF_INDEX2_MSGTYPE + 2), *wpMsgType);
                        break;

                    case MSGID_RESPONSE_SAVEPLAYERDATA_REPLY:
                        //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_RESPONSE_SAVEPLAYERDATA_REPLY");
                        ResponseSavePlayerDataReplyHandler(pData, dwMsgSize);
                        break;

                    case MSGID_GUILDNOTIFY:
                        //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_GUILDNOTIFY");
                        GuildNotifyHandler(pData, dwMsgSize);
                        break;

                    case MSGID_RESPONSE_DISBANDGUILD:
                        //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_RESPONSE_DISBANDGUILD");
                        ResponseDisbandGuildHandler(pData, dwMsgSize);
                        break;

                    case MSGID_RESPONSE_CREATENEWGUILD:
                        //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_RESPONSE_CREATENEWGUILD");
                        ResponseCreateNewGuildHandler(pData, dwMsgSize);
                        break;

                    case MSGID_RESPONSE_REGISTERGAMESERVER:
                        switch (*wpMsgType)
                        {
                            case DEF_MSGTYPE_CONFIRM:
                                //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_RESPONSE_REGISTERGAMESERVER -> DEF_MSGTYPE_CONFIRM");
                                log->info("(!) Game Server registration to Log Server - Success!");
                                m_bIsLogSockAvailable = true;
                                break;

                            case DEF_MSGTYPE_REJECT:
                                //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_RESPONSE_REGISTERGAMESERVER -> DEF_MSGTYPE_REJECT");
                                log->critical("(!) Game Server registration to Log Server - Fail!");
                                break;
                        }

                        if (m_bIsBuildItemAvailable == false)
                        {
                            log->critical("(!!!) STOPPED! Build-Item configuration error.");
                        }

                        if (m_bIsItemAvailable == false)
                        {
                            log->critical("(!!!) STOPPED! Item configuration error.");
                        }

                        if (m_bIsNpcAvailable == false)
                        {
                            log->critical("(!!!) STOPPED! Npc configuration error.");
                        }

                        if (m_bIsMagicAvailable == false)
                        {
                            log->critical("(!!!) STOPPED! MAGIC configuration error.");
                        }

                        if (m_bIsSkillAvailable == false)
                        {
                            log->critical("(!!!) STOPPED! SKILL configuration error.");
                        }

                        if (m_bIsQuestAvailable == false)
                        {
                            log->critical("(!!!) STOPPED! QUEST configuration error.");
                        }

                        if (m_bIsPortionAvailable == false)
                        {
                            log->critical("(!!!) STOPPED! POTION configuration error.");
                        }

                        //wsprintf(G_cTxt, "%d %d %d %d %d %d %d", (int)m_bIsGameStarted, (int)m_bIsItemAvailable, m_bIsNpcAvailable, (int)m_bIsGateSockAvailable, (int)m_bIsLogSockAvailable, (int)m_bIsMagicAvailable, (int)m_bIsSkillAvailable);
                        //log->info(G_cTxt);

                        /*
                        if ((m_bIsGameStarted == false) && (m_bIsItemAvailable == true) &&
                            (m_bIsNpcAvailable == true) && (m_bIsGateSockAvailable == true) &&
                            (m_bIsLogSockAvailable == true) && (m_bIsMagicAvailable == true) &&
                            (m_bIsSkillAvailable == true) && (m_bIsPortionAvailable == true) &&
                            (m_bIsQuestAvailable == true) ) {
                            SendMessage(m_hWnd, WM_USER_STARTGAMESIGNAL, 0, 0);
                            m_bIsGameStarted = true;
                        }
                        */
                        break;

                    case MSGID_RESPONSE_PLAYERDATA:
                        //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_RESPONSE_PLAYERDATA");
                        ResponsePlayerDataHandler(pData, dwMsgSize);
                        break;

                    case MSGID_BUILDITEMCONFIGURATIONCONTENTS:
                        //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_BUILDITEMCONFIGURATIONCONTENTS");
                        // Build Item contents
                        log->info("(!) BUILD-ITEM configuration contents received. Now decoding...");
                        m_bIsBuildItemAvailable = _bDecodeBuildItemConfigFileContents((char *)(pData + DEF_INDEX2_MSGTYPE + 2), dwMsgSize);
                        break;

                    case MSGID_ITEMCONFIGURATIONCONTENTS:
                        //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_ITEMCONFIGURATIONCONTENTS");
                        log->info("(!) ITEM configuration contents received. Now decoding...");
                        m_bIsItemAvailable = _bDecodeItemConfigFileContents((char *)(pData + DEF_INDEX2_MSGTYPE + 2), dwMsgSize);
                        break;

                    case MSGID_NPCCONFIGURATIONCONTENTS:
                        //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_NPCCONFIGURATIONCONTENTS");
                        log->info("(!) NPC configuration contents received. Now decoding...");
                        m_bIsNpcAvailable = _bDecodeNpcConfigFileContents((char *)(pData + DEF_INDEX2_MSGTYPE + 2), dwMsgSize);
                        break;

                    case MSGID_MAGICCONFIGURATIONCONTENTS:
                        //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_MAGICCONFIGURATIONCONTENTS");
                        log->info("(!) MAGIC configuration contents received. Now decoding...");
                        m_bIsMagicAvailable = _bDecodeMagicConfigFileContents((char *)(pData + DEF_INDEX2_MSGTYPE + 2), dwMsgSize);
                        break;

                    case MSGID_SKILLCONFIGURATIONCONTENTS:
                        //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_SKILLCONFIGURATIONCONTENTS");
                        log->info("(!) SKILL configuration contents received. Now decoding...");
                        m_bIsSkillAvailable = _bDecodeSkillConfigFileContents((char *)(pData + DEF_INDEX2_MSGTYPE + 2), dwMsgSize);
                        break;

                    case MSGID_QUESTCONFIGURATIONCONTENTS:
                        //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_QUESTCONFIGURATIONCONTENTS");
                        log->info("(!) QUEST configuration contents received. Now decoding...");
                        m_bIsQuestAvailable = _bDecodeQuestConfigFileContents((char *)(pData + DEF_INDEX2_MSGTYPE + 2), dwMsgSize);
                        break;

                    case MSGID_PORTIONCONFIGURATIONCONTENTS:
                        //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_PORTIONCONFIGURATIONCONTENTS");
                        log->info("(!) POTION configuration contents received. Now decoding...");
                        m_bIsPortionAvailable = _bDecodePortionConfigFileContents((char *)(pData + DEF_INDEX2_MSGTYPE + 2), dwMsgSize);
                        break;

                        // Crusade
                        //case MSGID_ARESDENOCCUPYFLAGSAVEFILECONTENTS:
                            //log->info("(!) Aresden OccupyFlag save file contents received. Now decoding...");
                            //_bDecodeOccupyFlagSaveFileContents((char *)(pData + DEF_INDEX2_MSGTYPE + 2), dwMsgSize, 1);
                            //break;

                        // Crusade
                        //case MSGID_ELVINEOCCUPYFLAGSAVEFILECONTENTS:
                            //log->info("(!) Elvine OccupyFlag save file contents received. Now decoding...");
                            //_bDecodeOccupyFlagSaveFileContents((char *)(pData + DEF_INDEX2_MSGTYPE + 2), dwMsgSize, 2);
                            //break;

                    case MSGID_DUPITEMIDFILECONTENTS:
                        //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_DUPITEMIDFILECONTENTS");
                        log->info("(!) DupItemID file contents received. Now decoding...");
                        _bDecodeDupItemIDFileContents((char *)(pData + DEF_INDEX2_MSGTYPE + 2), dwMsgSize);
                        break;

                    case MSGID_NOTICEMENTFILECONTENTS:
                        //DbgWnd->AddEventMsg("RECV -> DEF_MSGFROM_LOGSERVER -> MSGID_NOTICEMENTFILECONTENTS");
                        log->info("(!) Noticement file contents received. Now decoding...");
                        _bDecodeNoticementFileContents((char *)(pData + DEF_INDEX2_MSGTYPE + 2), dwMsgSize);
                        break;

                        // v2.17 2002-8-7 // 2002-09-06 #1
                        /*case MSGID_NPCITEMCONFIGCONTENTS:
                            log->info("(!) NpcItemConfig file contents received. Now decoding...");
                            _bDecodeNpcItemConfigFileContents((char *)(pData + DEF_INDEX2_MSGTYPE + 2), dwMsgSize);
                            break;*/

                }
                break;
        }
    }

}

bool CGame::bPutMsgQuene(char cFrom, char * pData, uint32_t dwMsgSize, int iIndex, char cKey)
{
    /*
    HANDLE hMutex;

    hMutex = OpenMutex(MUTEX_ALL_ACCESS, false, m_cServerName);
    if (hMutex != 0) return false; // Â¹Ã‚Ã…Ã˜Â½ÂºÂ°Â¡ Â»Ã½Â¼ÂºÂµÃ‡Â¾Ã® Ã€Ã–Â´Ã™. Â¾Ã®ÂµÃ°Â¼Â±Â°Â¡ Ã…Â¥Â¸Â¦ ÃÂ¶Ã€Ã›ÃÃŸÃ€ÃŒÂ´Ã™. Â±Ã—Â³Ã‰ Â¸Â®Ã…Ã

    hMutex = CreateMutex(0, false, m_cServerName);
    */
    if (m_pMsgQuene[m_iQueneTail] != 0) return false;

    m_pMsgQuene[m_iQueneTail] = new class CMsg;
    if (m_pMsgQuene[m_iQueneTail] == 0) return false;

    if (m_pMsgQuene[m_iQueneTail]->bPut(cFrom, pData, dwMsgSize, iIndex, cKey) == false) return false;

    m_iQueneTail++;
    if (m_iQueneTail >= DEF_MSGQUENESIZE) m_iQueneTail = 0;

    //ReleaseMutex(hMutex);

    return true;
}

bool CGame::bGetMsgQuene(char * pFrom, char * pData, uint32_t * pMsgSize, int * pIndex, char * pKey)
{
    /*
    HANDLE hMutex;

    hMutex = OpenMutex(MUTEX_ALL_ACCESS, false, m_cServerName);
    if (hMutex != 0) return false;

    hMutex = CreateMutex(0, false, m_cServerName);
    */
    if (m_pMsgQuene[m_iQueneHead] == 0) return false;

    m_pMsgQuene[m_iQueneHead]->Get(pFrom, pData, pMsgSize, pIndex, pKey);

    delete m_pMsgQuene[m_iQueneHead];
    m_pMsgQuene[m_iQueneHead] = 0;

    m_iQueneHead++;
    if (m_iQueneHead >= DEF_MSGQUENESIZE) m_iQueneHead = 0;

    //ReleaseMutex(hMutex);

    return true;
}

bool CGame::_bAddClientItemList(int iClientH, class CItem * pItem, int * pDelReq)
{
    int i;

    if (m_pClientList[iClientH] == nullptr) return false;
    if (pItem == 0) return false;

    if ((pItem->m_cItemType == DEF_ITEMTYPE_CONSUME) || (pItem->m_cItemType == DEF_ITEMTYPE_ARROW))
    {
        if ((m_pClientList[iClientH]->m_iCurWeightLoad + iGetItemWeight(pItem, pItem->m_dwCount)) > (uint32_t)_iCalcMaxLoad(iClientH))
            return false;
    }
    else
    {
        if ((m_pClientList[iClientH]->m_iCurWeightLoad + iGetItemWeight(pItem, 1)) > (uint32_t)_iCalcMaxLoad(iClientH))
            return false;
    }

    if ((pItem->m_cItemType == DEF_ITEMTYPE_CONSUME) || (pItem->m_cItemType == DEF_ITEMTYPE_ARROW))
    {
        for (i = 0; i < DEF_MAXITEMS; i++)
            if ((m_pClientList[iClientH]->m_pItemList[i] != 0) &&
                (memcmp(m_pClientList[iClientH]->m_pItemList[i]->m_cName, pItem->m_cName, 20) == 0))
            {
                m_pClientList[iClientH]->m_pItemList[i]->m_dwCount += pItem->m_dwCount;
                //delete pItem;
                *pDelReq = 1;

                iCalcTotalWeight(iClientH);

                return true;
            }
    }

    for (i = 0; i < DEF_MAXITEMS; i++)
        if (m_pClientList[iClientH]->m_pItemList[i] == 0)
        {

            m_pClientList[iClientH]->m_pItemList[i] = pItem;
            m_pClientList[iClientH]->m_ItemPosList[i].x = 40;
            m_pClientList[iClientH]->m_ItemPosList[i].y = 30;

            *pDelReq = 0;

            if (pItem->m_cItemType == DEF_ITEMTYPE_ARROW)
                m_pClientList[iClientH]->m_cArrowIndex = _iGetArrowItemIndex(iClientH);

            iCalcTotalWeight(iClientH);

            return true;
        }

    return false;
}

void CGame::SendEventToNearClient_TypeB(uint32_t dwMsgID, uint16_t wMsgType, char cMapIndex, short sX, short sY, short sV1, short sV2, short sV3, short sV4)
{
    int i, iRet, iShortCutIndex;
    char * cp, cData[100];
    uint32_t * dwp, dwTime;
    uint16_t * wp;
    short * sp;
    bool bFlag;

    memset(cData, 0, sizeof(cData));

    dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
    *dwp = dwMsgID;
    wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
    *wp = wMsgType;

    cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);

    sp = (short *)cp;
    *sp = sX;
    cp += 2;

    sp = (short *)cp;
    *sp = sY;
    cp += 2;

    sp = (short *)cp;
    *sp = sV1;
    cp += 2;

    sp = (short *)cp;
    *sp = sV2;
    cp += 2;

    sp = (short *)cp;
    *sp = sV3;
    cp += 2;

    sp = (short *)cp;
    *sp = sV4;
    cp += 2;

    dwTime = timeGetTime();

    //for (i = 1; i < DEF_MAXCLIENTS; i++)
    bFlag = true;
    iShortCutIndex = 0;
    while (bFlag == true)
    {
        // DEF_MAXCLIENTS 
        i = m_iClientShortCut[iShortCutIndex];
        iShortCutIndex++;
        if (i == 0) bFlag = false;

        if ((bFlag == true) && (m_pClientList[i] != 0))
        {
            if ((m_pClientList[i]->m_cMapIndex == cMapIndex) &&
                (m_pClientList[i]->m_sX >= sX - 10) &&
                (m_pClientList[i]->m_sX <= sX + 10) &&
                (m_pClientList[i]->m_sY >= sY - 8) &&
                (m_pClientList[i]->m_sY <= sY + 8))
            {

                iRet = m_pClientList[i]->send_msg(cData, 18);
            }
        }
    }
}

void CGame::SendNotifyMsg(int iFromH, int iToH, uint16_t wMsgType, uint32_t sV1, uint32_t sV2, uint32_t sV3, char * pString, uint32_t sV4, uint32_t sV5, uint32_t sV6, uint32_t sV7, uint32_t sV8, uint32_t sV9, char * pString2)
{
    char cData[1000];
    uint32_t * dwp;
    uint16_t * wp;
    char * cp;
    short * sp;
    int * ip, iRet, i;

    if (m_pClientList[iToH] == 0) return;

    memset(cData, 0, sizeof(cData));

    dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
    *dwp = MSGID_NOTIFY;
    wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
    *wp = wMsgType;

    cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);

    // !!! sV1, sV2, sV3Â´Ã‚ DWORDÃ‡Ã¼Ã€Ã“Ã€Â» Â¸Ã­Â½Ã‰Ã‡ÃÂ¶Ã³.
    switch (wMsgType)
    {
        case DEF_NOTIFY_HELDENIANCOUNT:
            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV1;
            cp += 2;

            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV2;
            cp += 2;

            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV3;
            cp += 2;

            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV4;
            cp += 2;

            cp += 14;

            iRet = m_pClientList[iToH]->send_msg(cData, 14);

            break;

        case DEF_NOTIFY_NOMOREAGRICULTURE:
        case DEF_NOTIFY_AGRICULTURESKILLLIMIT:
        case DEF_NOTIFY_AGRICULTURENOAREA:
            iRet = m_pClientList[iToH]->send_msg(cData, 6);
            break;

            // New 18/05/2004
        case DEF_NOTIFY_SPAWNEVENT:
            *cp = (char)sV3;
            cp++;

            sp = (short *)cp;
            *cp = sV1;
            cp += 2;

            sp = (short *)cp;
            *cp = sV2;
            cp += 2;

            iRet = m_pClientList[iToH]->send_msg(cData, 11);
            break;

        case DEF_NOTIFY_QUESTCOUNTER:
            ip = (int *)cp;
            *ip = sV1;
            cp += 4;

            iRet = m_pClientList[iToH]->send_msg(cData, 24);
            break;

        case DEF_NOTIFY_APOCGATECLOSE:
        case DEF_NOTIFY_APOCGATEOPEN:
            ip = (int *)cp;
            *ip = sV1;
            cp += 4;

            ip = (int *)cp;
            *ip = sV2;
            cp += 4;

            memcpy(cp, pString, 10);
            cp += 10;

            iRet = m_pClientList[iToH]->send_msg(cData, 24);
            break;

        case DEF_NOTIFY_ABADDONKILLED:
            memcpy(cp, m_pClientList[iFromH]->m_cCharName, 10);
            cp += 10;

            iRet = m_pClientList[iToH]->send_msg(cData, 24);
            break;

        case DEF_NOTIFY_APOCFORCERECALLPLAYERS:
        case DEF_NOTIFY_APOCGATESTARTMSG:
        case DEF_NOTIFY_APOCGATEENDMSG:
        case DEF_NOTIFY_NORECALL:
            iRet = m_pClientList[iToH]->send_msg(cData, 6);
            break;

        case DEF_NOTIFY_FORCERECALLTIME:
            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV1;
            cp += 2;

            iRet = m_pClientList[iToH]->send_msg(cData, 8);
            break;

            // New 16/05/2004
            //0xB4E2, 0xBEB
        case DEF_NOTIFY_MONSTERCOUNT:
        case DEF_NOTIFY_SLATE_STATUS:
            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV1;
            cp += 2;

            iRet = m_pClientList[iToH]->send_msg(cData, 8);
            break;

            //0x0BE5, 0x0BE7, 0x0BE8, 0x0BEA
        case DEF_NOTIFY_0BE8:
        case DEF_NOTIFY_HELDENIANTELEPORT:
        case DEF_NOTIFY_HELDENIANEND:
        case DEF_NOTIFY_RESURRECTPLAYER:
        case DEF_NOTIFY_SLATE_EXP:
        case DEF_NOTIFY_SLATE_MANA:
        case DEF_NOTIFY_SLATE_INVINCIBLE:
            iRet = m_pClientList[iToH]->send_msg(cData, 6);
            break;

        case DEF_NOTIFY_SLATE_CREATEFAIL:
            iRet = m_pClientList[iToH]->send_msg(cData, 6);
            break;

        case DEF_NOTIFY_SLATE_CREATESUCCESS:
            dwp = (uint32_t *)cp;
            *dwp = sV1;
            cp += 4;

            iRet = m_pClientList[iToH]->send_msg(cData, 10);
            break;

            // New 07/05/2004
            // Party Notify Msg's
        case DEF_NOTIFY_PARTY:
            switch (sV1)
            {
                case 4:
                case 6:
                    wp = (uint16_t *)cp;
                    *wp = (uint16_t)sV1;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    *wp = (uint16_t)sV2;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    *wp = (uint16_t)sV3;
                    cp += 2;

                    memcpy(cp, pString, 10);
                    cp += 10;

                    iRet = m_pClientList[iToH]->send_msg(cData, 22);
                    break;
                case 5:
                    wp = (uint16_t *)cp;
                    *wp = (uint16_t)sV1;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    *wp = (uint16_t)sV2;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    *wp = (uint16_t)sV3;
                    cp += 2;

                    memcpy(cp, pString, sV3 * 11);
                    cp += sV3 * 11;

                    iRet = m_pClientList[iToH]->send_msg(cData, 12 + (sV3 * 11));
                    break;
                default:
                    wp = (uint16_t *)cp;
                    *wp = (uint16_t)sV1;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    *wp = (uint16_t)sV2;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    *wp = (uint16_t)sV3;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    *wp = (uint16_t)sV4;
                    cp += 2;

                    iRet = m_pClientList[iToH]->send_msg(cData, 14);
                    break;
            }
            break;

        case DEF_NOTIFY_REQGUILDNAMEANSWER:
            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV1;
            cp += 2;

            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV2;
            cp += 2;

            memcpy(cp, pString, 20);
            cp += 20;

            iRet = m_pClientList[iToH]->send_msg(cData, 30);
            break;

            // New 06/05/2004
            // Upgrade Notify Msg's
        case DEF_NOTIFY_ITEMUPGRADEFAIL:
            sp = (short *)cp;
            *sp = (short)sV1;
            cp += 2;

            iRet = m_pClientList[iToH]->send_msg(cData, 8);
            break;

        case DEF_NOTIFY_ITEMATTRIBUTECHANGE:
        case DEF_NOTIFY_GIZONITEMUPGRADELEFT:
            sp = (short *)cp;
            *sp = sV1;
            cp += 2;

            dwp = (uint32_t *)cp;
            *dwp = sV2;
            cp += 4;

            dwp = (uint32_t *)cp;
            *dwp = sV3;
            cp += 4;

            dwp = (uint32_t *)cp;
            *dwp = sV4;
            cp += 4;

            iRet = m_pClientList[iToH]->send_msg(cData, 20);
            break;

        case DEF_NOTIFY_GIZONITEMCANGE:
            sp = (short *)cp;
            *sp = (short)sV1;
            cp += 2;

            *cp = (char)sV2;
            cp++;

            sp = (short *)cp;
            *sp = (short)sV3;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV4;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV5;
            cp += 2;

            *cp = (char)sV6;
            cp++;

            *cp = (char)sV7;
            cp++;

            dwp = (uint32_t *)cp;
            *dwp = sV8;
            cp += 4;

            memcpy(cp, pString, 20);
            cp += 20;

            iRet = m_pClientList[iToH]->send_msg(cData, 41);
            break;

            // 2.06 - by KLKS
        case DEF_NOTIFY_CHANGEPLAYMODE:
            memcpy(cp, pString, 10);
            cp += 10;
            iRet = m_pClientList[iToH]->send_msg(cData, 16);
            break;
            //

        case DEF_NOTIFY_TCLOC:
            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV1;
            cp += 2;

            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV2;
            cp += 2;

            memcpy(cp, pString, 10);
            cp += 10;

            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV4;
            cp += 2;

            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV5;
            cp += 2;

            memcpy(cp, pString2, 10);
            cp += 10;

            iRet = m_pClientList[iToH]->send_msg(cData, 34);
            break;

        case DEF_NOTIFY_GRANDMAGICRESULT:
            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV1;
            cp += 2;

            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV2;
            cp += 2;

            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV3;
            cp += 2;

            memcpy(cp, pString, 10);
            cp += 10;

            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV4;
            cp += 2;

            if (sV9 > 0)
            {
                memcpy(cp, pString2, (sV9 * 2) + 2);
                cp += (sV9 * 2) + 2;
            }
            else
            {
                sp = (short *)cp;
                *sp = 0;
                cp += 2;
            }
            iRet = m_pClientList[iToH]->send_msg(cData, (sV9 * 2) + 26);
            break;

        case DEF_NOTIFY_MAPSTATUSNEXT:
            memcpy(cp, pString, sV1);
            cp += sV1;
            iRet = m_pClientList[iToH]->send_msg(cData, 6 + sV1);
            break;

        case DEF_NOTIFY_MAPSTATUSLAST:
            memcpy(cp, pString, sV1);
            cp += sV1;
            iRet = m_pClientList[iToH]->send_msg(cData, 6 + sV1);
            break;

        case DEF_NOTIFY_LOCKEDMAP:
            sp = (short *)cp;
            *sp = (short)sV1;
            cp += 2;

            memcpy(cp, pString, 10);
            cp += 10;

            iRet = m_pClientList[iToH]->send_msg(cData, 18);
            break;

        case DEF_NOTIFY_BUILDITEMSUCCESS:
        case DEF_NOTIFY_BUILDITEMFAIL:

            if (sV1 >= 0)
            {
                sp = (short *)cp;
                *sp = (short)sV1;
                cp += 2;
            }
            else
            {
                sp = (short *)cp;
                *sp = (short)sV1 + 10000;
                cp += 2;
            }

            sp = (short *)cp;
            *sp = (short)sV2;
            cp += 2;

            iRet = m_pClientList[iToH]->send_msg(cData, 10);
            break;

        case DEF_NOTIFY_HELP:
        case DEF_NOTIFY_QUESTREWARD:
            sp = (short *)cp;
            *sp = (short)sV1;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV2;
            cp += 2;

            ip = (int *)cp;
            *ip = (int)sV3;
            cp += 4;

            memcpy(cp, pString, 20);
            cp += 20;

            ip = (int *)cp;
            *ip = (int)sV4;
            cp += 4;

            iRet = m_pClientList[iToH]->send_msg(cData, 38);
            break;

        case DEF_NOTIFY_CANNOTCONSTRUCT:
        case DEF_NOTIFY_METEORSTRIKECOMING:
        case DEF_NOTIFY_METEORSTRIKEHIT:
        case DEF_NOTIFY_HELPFAILED:
        case DEF_NOTIFY_SPECIALABILITYENABLED:
        case DEF_NOTIFY_FORCEDISCONN:
        case DEF_NOTIFY_OBSERVERMODE:
        case DEF_NOTIFY_QUESTCOMPLETED:
        case DEF_NOTIFY_QUESTABORTED:
            sp = (short *)cp;
            *sp = (short)sV1;
            cp += 2;

            iRet = m_pClientList[iToH]->send_msg(cData, 8);
            break;

        case DEF_NOTIFY_QUESTCONTENTS:
            sp = (short *)cp;
            *sp = (short)sV1;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV2;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV3;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV4;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV5;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV6;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV7;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV8;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV9;
            cp += 2;

            if (pString2 != 0) memcpy(cp, pString2, 20);
            cp += 20;

            iRet = m_pClientList[iToH]->send_msg(cData, 44);
            break;

        case DEF_NOTIFY_ENERGYSPHERECREATED:
        case DEF_NOTIFY_ITEMCOLORCHANGE:
            sp = (short *)cp;
            *sp = (short)sV1;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV2;
            cp += 2;

            iRet = m_pClientList[iToH]->send_msg(cData, 10);
            break;

        case DEF_NOTIFY_NOMORECRUSADESTRUCTURE:
        case DEF_NOTIFY_EXCHANGEITEMCOMPLETE:
        case DEF_NOTIFY_CANCELEXCHANGEITEM:
            iRet = m_pClientList[iToH]->send_msg(cData, 6);
            break;

        case DEF_NOTIFY_SETEXCHANGEITEM:
            sp = (short *)cp;
            *sp = (short)sV1;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV2;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV3;
            cp += 2;

            ip = (int *)cp;
            *ip = (int)sV4;
            cp += 4;

            *cp = (char)sV5;
            cp++;

            sp = (short *)cp;
            *sp = (short)sV6;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV7;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV8;
            cp += 2;

            memcpy(cp, pString, 20);
            cp += 20;

            memcpy(cp, m_pClientList[iFromH]->m_cCharName, 10);
            cp += 10;

            // v1.42
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)sV9;
            cp += 4;

            iRet = m_pClientList[iToH]->send_msg(cData, 57);
            break;

        case DEF_NOTIFY_OPENEXCHANGEWINDOW:
            sp = (short *)cp;
            *sp = (short)sV1;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV2;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV3;
            cp += 2;

            ip = (int *)cp;
            *ip = (int)sV4;
            cp += 4;

            *cp = (char)sV5;
            cp++;

            sp = (short *)cp;
            *sp = (short)sV6;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV7;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV8;
            cp += 2;

            memcpy(cp, pString, 20);
            cp += 20;

            memcpy(cp, m_pClientList[iFromH]->m_cCharName, 10);
            cp += 10;

            // v1.42
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)sV9;
            cp += 4;

            iRet = m_pClientList[iToH]->send_msg(cData, 57);
            break;

        case DEF_NOTIFY_NOTFLAGSPOT:
            iRet = m_pClientList[iToH]->send_msg(cData, 6);
            break;

        case DEF_NOTIFY_ITEMPOSLIST:
            for (i = 0; i < DEF_MAXITEMS; i++)
            {
                sp = (short *)cp;
                *sp = (short)m_pClientList[iToH]->m_ItemPosList[i].x;
                cp += 2;
                sp = (short *)cp;
                *sp = (short)m_pClientList[iToH]->m_ItemPosList[i].y;
                cp += 2;
            }
            iRet = m_pClientList[iToH]->send_msg(cData, 6 + DEF_MAXITEMS * 4);
            break;

        case DEF_NOTIFY_ENEMYKILLS:
            ip = (int *)cp;
            *ip = (int)sV1;
            cp += 4;

            iRet = m_pClientList[iToH]->send_msg(cData, 10);
            break;

        case DEF_NOTIFY_CRUSADE:
            ip = (int *)cp;
            *ip = (int)sV1;
            cp += 4;

            ip = (int *)cp;
            *ip = (int)sV2;
            cp += 4;

            ip = (int *)cp;
            *ip = (int)sV3;
            cp += 4;

            ip = (int *)cp;
            *ip = (int)sV4;
            cp += 4;

            iRet = m_pClientList[iToH]->send_msg(cData, 22);
            break;

        case DEF_NOTIFY_CONSTRUCTIONPOINT:
        case DEF_NOTIFY_SPECIALABILITYSTATUS:
        case DEF_NOTIFY_DAMAGEMOVE:
            sp = (short *)cp;
            *sp = (short)sV1;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV2;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV3;
            cp += 2;

            iRet = m_pClientList[iToH]->send_msg(cData, 12);
            break;

        case DEF_NOTIFY_DOWNSKILLINDEXSET:
        case DEF_NOTIFY_RESPONSE_CREATENEWPARTY:
            sp = (short *)cp;
            *sp = (short)sV1;
            cp += 2;

            iRet = m_pClientList[iToH]->send_msg(cData, 8);
            break;

        case DEF_NOTIFY_ADMINIFO:
            switch (sV1)
            {
                case 1:
                    // NPCÃ€Ã‡ ÃÂ¤ÂºÂ¸Â¸Â¦ Â¾Ã²Â¾Ã®Â¿Ã‚Â´Ã™.
                    ip = (int *)cp;
                    *ip = m_pNpcList[sV2]->m_iHP;
                    cp += 4;

                    ip = (int *)cp;
                    *ip = m_pNpcList[sV2]->m_iDefenseRatio;
                    cp += 4;

                    ip = (int *)cp;
                    *ip = m_pNpcList[sV2]->m_bIsSummoned;
                    cp += 4;

                    ip = (int *)cp;
                    *ip = m_pNpcList[sV2]->m_cActionLimit;
                    cp += 4;

                    ip = (int *)cp;
                    *ip = m_pNpcList[sV2]->m_iHitDice;
                    cp += 4;

                    dwp = (uint32_t *)cp; // v1.4
                    *dwp = m_pNpcList[sV2]->m_dwDeadTime;
                    cp += 4;

                    dwp = (uint32_t *)cp;
                    *dwp = m_pNpcList[sV2]->m_dwRegenTime;
                    cp += 4;

                    ip = (int *)cp;
                    *ip = (int)m_pNpcList[sV2]->m_bIsKilled;
                    cp += 4;

                    iRet = m_pClientList[iToH]->send_msg(cData, 26 + 12);
                    break;
            }
            break;

        case DEF_NOTIFY_HELDENIANSTART:
        case DEF_NOTIFY_NPCTALK:
            sp = (short *)cp;
            *sp = (short)sV1;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV2;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV3;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV4;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV5;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV6;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV7;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV8;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)sV9;
            cp += 2;

            if (pString != 0) memcpy(cp, pString, 20);
            cp += 20;

            if (pString2 != 0) memcpy(cp, pString2, 20);
            cp += 20;

            iRet = m_pClientList[iToH]->send_msg(cData, 64);
            break;

        case DEF_NOTIFY_PORTIONSUCCESS:
        case DEF_NOTIFY_LOWPORTIONSKILL:
        case DEF_NOTIFY_PORTIONFAIL:
        case DEF_NOTIFY_NOMATCHINGPORTION:
            // Ã€ÃÃ„Â¡Ã‡ÃÂ´Ã‚ Ã†Ã·Â¼Ã‡ ÃÂ¶Ã‡Ã•Ã€ÃŒ Â¾Ã¸Â´Ã™.
            iRet = m_pClientList[iToH]->send_msg(cData, 6);
            break;

        case DEF_NOTIFY_SUPERATTACKLEFT:
            sp = (short *)cp;
            *sp = m_pClientList[iToH]->m_iSuperAttackLeft;
            cp += 2;

            iRet = m_pClientList[iToH]->send_msg(cData, 8);
            break;

        case DEF_NOTIFY_SAFEATTACKMODE:
            *cp = m_pClientList[iToH]->m_bIsSafeAttackMode;
            cp++;

            iRet = m_pClientList[iToH]->send_msg(cData, 7);
            break;

        case DEF_NOTIFY_QUERY_JOINPARTY:
        case DEF_NOTIFY_IPACCOUNTINFO:
            strcpy(cp, pString);
            cp += strlen(pString);

            iRet = m_pClientList[iToH]->send_msg(cData, 6 + strlen(pString) + 1);
            break;

        case DEF_NOTIFY_REWARDGOLD:
            dwp = (uint32_t *)cp;
            *dwp = m_pClientList[iToH]->m_iRewardGold;
            cp += 4;

            iRet = m_pClientList[iToH]->send_msg(cData, 10);
            break;

        case DEF_NOTIFY_SERVERSHUTDOWN:
            *cp = (char)sV1;
            cp++;

            iRet = m_pClientList[iToH]->send_msg(cData, 7);
            break;

        case DEF_NOTIFY_GLOBALATTACKMODE:
        case DEF_NOTIFY_WHETHERCHANGE:
            *cp = (char)sV1;
            cp++;

            iRet = m_pClientList[iToH]->send_msg(cData, 7);
            break;

        case DEF_NOTIFY_FISHCANCELED:
        case DEF_NOTIFY_FISHSUCCESS:
        case DEF_NOTIFY_FISHFAIL:
            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV1;
            cp += 2;

            iRet = m_pClientList[iToH]->send_msg(cData, 8);
            break;

        case DEF_NOTIFY_DEBUGMSG:
            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV1;
            cp += 2;

            iRet = m_pClientList[iToH]->send_msg(cData, 8);
            break;

        case DEF_NOTIFY_FISHCHANCE:
            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV1;
            cp += 2;

            iRet = m_pClientList[iToH]->send_msg(cData, 8);
            break;

        case DEF_NOTIFY_ENERGYSPHEREGOALIN:
        case DEF_NOTIFY_EVENTFISHMODE:
            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV1;
            cp += 2;

            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV2;
            cp += 2;

            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV3;
            cp += 2;

            memcpy(cp, pString, 20);
            cp += 20;

            iRet = m_pClientList[iToH]->send_msg(cData, 32);
            break;

        case DEF_NOTIFY_NOTICEMSG:
            memcpy(cp, pString, strlen(pString));
            cp += strlen(pString);

            *cp = 0;
            cp++;

            iRet = m_pClientList[iToH]->send_msg(cData, strlen(pString) + 7);
            break;

        case DEF_NOTIFY_CANNOTRATING:
            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV1;
            cp += 2;

            iRet = m_pClientList[iToH]->send_msg(cData, 8);
            break;

        case DEF_NOTIFY_RATINGPLAYER:
            *cp = (char)sV1;
            cp++;

            memcpy(cp, pString, 10);
            cp += 10;

            ip = (int *)cp;
            *ip = m_pClientList[iToH]->m_iRating;
            cp += 4;

            iRet = m_pClientList[iToH]->send_msg(cData, 22);
            break;

        case DEF_NOTIFY_ADMINUSERLEVELLOW:
            iRet = m_pClientList[iToH]->send_msg(cData, 6);
            break;

        case DEF_NOTIFY_PLAYERSHUTUP:
            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV1;
            cp += 2;

            memcpy(cp, pString, 10);
            cp += 10;

            iRet = m_pClientList[iToH]->send_msg(cData, 19);
            break;

        case DEF_NOTIFY_TIMECHANGE:
            *cp = (char)sV1;
            cp++;

            iRet = m_pClientList[iToH]->send_msg(cData, 7);
            break;

        case DEF_NOTIFY_TOBERECALLED:
            iRet = m_pClientList[iToH]->send_msg(cData, 6);
            break;

        case DEF_NOTIFY_HUNGER:
            *cp = (char)sV1;
            cp++;

            iRet = m_pClientList[iToH]->send_msg(cData, 7);
            break;

        case DEF_NOTIFY_PLAYERPROFILE:
            if (strlen(pString) > 100)
            {
                memcpy(cp, pString, 100);
                cp += 100;
            }
            else
            {
                memcpy(cp, pString, strlen(pString));
                cp += strlen(pString);
            }
            *cp = 0;
            cp++;

            iRet = m_pClientList[iToH]->send_msg(cData, 7 + strlen(pString));
            break;

            // New 10/05/2004 Changed
        case DEF_NOTIFY_WHISPERMODEON:
        case DEF_NOTIFY_WHISPERMODEOFF:
        case DEF_NOTIFY_PLAYERNOTONGAME:
            memcpy(cp, pString, 10);
            cp += 10;

            memcpy(cp, "          ", 10);
            cp += 10;

            iRet = m_pClientList[iToH]->send_msg(cData, 27);
            break;

            // New 15/05/2004 Changed
        case DEF_NOTIFY_PLAYERONGAME:
            memcpy(cp, pString, 10);
            cp += 10;

            if (pString[0] != 0)
            {
                memcpy(cp, pString2, 14);
                cp += 14;
            }

            iRet = m_pClientList[iToH]->send_msg(cData, 31);
            break;

            // New 06/05/2004
        case DEF_NOTIFY_ITEMSOLD:
        case DEF_NOTIFY_ITEMREPAIRED:
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)sV1;
            cp += 4;
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)sV2;
            cp += 4;

            iRet = m_pClientList[iToH]->send_msg(cData, 14);
            break;

            // New 06/05/2004
        case DEF_NOTIFY_REPAIRITEMPRICE:
        case DEF_NOTIFY_SELLITEMPRICE:
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)sV1;
            cp += 4;
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)sV2;
            cp += 4;
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)sV3;
            cp += 4;
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)sV4;
            cp += 4;

            memcpy(cp, pString, 20);
            cp += 20;

            iRet = m_pClientList[iToH]->send_msg(cData, 42);
            break;

        case DEF_NOTIFY_CANNOTREPAIRITEM:
        case DEF_NOTIFY_CANNOTSELLITEM:
            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV1;
            cp += 2;

            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV2;
            cp += 2;

            memcpy(cp, pString, 20);
            cp += 20;

            iRet = m_pClientList[iToH]->send_msg(cData, 30);

            break;

        case DEF_NOTIFY_SHOWMAP:
            wp = (uint16_t *)cp;  // ÂºÂ¸Â¿Â©ÃÃ–Â´Ã‚ ÃÂ¾Â·Ã¹ 
            *wp = (uint16_t)sV1;
            cp += 2;

            wp = (uint16_t *)cp;  // Â¸ÃŠ Â¹Ã¸ÃˆÂ£ (0 aresden, 1 elvine, 3 middleland...)
            *wp = (uint16_t)sV2;
            cp += 2;

            iRet = m_pClientList[iToH]->send_msg(cData, 10);
            break;

        case DEF_NOTIFY_SKILLUSINGEND:
            wp = (uint16_t *)cp;  // Â±Ã¢Â¼Ãº Â»Ã§Â¿Ã« Â°Ã¡Â°Ãº 
            *wp = (uint16_t)sV1;
            cp += 2;

            iRet = m_pClientList[iToH]->send_msg(cData, 8);
            break;

        case DEF_NOTIFY_TOTALUSERS:
            wp = (uint16_t *)cp;
            *wp = (uint16_t)m_iTotalGameServerClients; //_iGetTotalClients();
            cp += 2;

            iRet = m_pClientList[iToH]->send_msg(cData, 8);
            break;

        case DEF_NOTIFY_MAGICEFFECTOFF:
        case DEF_NOTIFY_MAGICEFFECTON:
            wp = (uint16_t *)cp;  // Â¸Â¶Â¹Ã½ ÃˆÂ¿Â°Ãº ÃÂ¾Â·Ã¹ 
            *wp = (uint16_t)sV1;
            cp += 2;

            dwp = (uint32_t *)cp;  // Â¸Â¶Â¹Ã½ ÃˆÂ¿Â°Ãº ÃˆÂ¿Â·Ã‚  
            *dwp = (uint32_t)sV2;
            cp += 4;

            dwp = (uint32_t *)cp;  // Â¸Â¶Â¹Ã½ ÃˆÂ¿Â°Ãº ÃˆÂ¿Â·Ã‚  
            *dwp = (uint32_t)sV3;
            cp += 4;

            iRet = m_pClientList[iToH]->send_msg(cData, 16);
            break;

        case DEF_NOTIFY_CANNOTITEMTOBANK:
            iRet = m_pClientList[iToH]->send_msg(cData, 6);
            break;

        case DEF_NOTIFY_SERVERCHANGE:
            memcpy(cp, m_pClientList[iToH]->m_cMapName, 10);
            cp += 10;

            // World ServerÃ€Ã‡ ÃÃ–Â¼Ã’Â¸Â¦ Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
            memcpy(cp, m_cLogServerAddr, 15);
            cp += 15;

            ip = (int *)cp;
            //Filter
            *ip = m_iLogServerPort;
            cp += 4;

            iRet = m_pClientList[iToH]->send_msg(cData, 16 + 19);
            break;

        case DEF_NOTIFY_SKILL:
            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV1;
            cp += 2;

            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV2;
            cp += 2;

            iRet = m_pClientList[iToH]->send_msg(cData, 10);
            break;

        case DEF_NOTIFY_SETITEMCOUNT:
            wp = (uint16_t *)cp;  // Â¾Ã†Ã€ÃŒÃ…Ã› Ã€ÃŽÂµÂ¦Â½Âº Â¹Ã¸ÃˆÂ£ 
            *wp = (uint16_t)sV1;
            cp += 2;

            dwp = (uint32_t *)cp;  // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ Ã‡Ã¶Ã€Ã§ Â¼Ã¶Â·Â® 
            *dwp = (uint32_t)sV2;
            cp += 4;

            *cp = (char)sV3;
            cp++;

            iRet = m_pClientList[iToH]->send_msg(cData, 13);
            break;

        case DEF_NOTIFY_ITEMDEPLETED_ERASEITEM:
            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV1;
            cp += 2;

            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV2;
            cp += 2;

            iRet = m_pClientList[iToH]->send_msg(cData, 10);
            break;

        case DEF_NOTIFY_DROPITEMFIN_COUNTCHANGED:
            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV1;
            cp += 2;

            ip = (int *)cp;
            *ip = (int)sV2;
            cp += 4;

            iRet = m_pClientList[iToH]->send_msg(cData, 12);
            break;

        case DEF_NOTIFY_DROPITEMFIN_ERASEITEM:
            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV1;
            cp += 2;

            ip = (int *)cp;
            *ip = (int)sV2;
            cp += 4;

            iRet = m_pClientList[iToH]->send_msg(cData, 12);
            break;

        case DEF_NOTIFY_CANNOTGIVEITEM:
        case DEF_NOTIFY_GIVEITEMFIN_COUNTCHANGED:
            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV1;
            cp += 2;

            ip = (int *)cp;
            *ip = (int)sV2;
            cp += 4;

            memcpy(cp, pString, 20);
            cp += 20;

            iRet = m_pClientList[iToH]->send_msg(cData, 32);
            break;

        case DEF_NOTIFY_GIVEITEMFIN_ERASEITEM:
            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV1;
            cp += 2;

            ip = (int *)cp;
            *ip = (int)sV2;
            cp += 4;

            memcpy(cp, pString, 20);
            cp += 20;

            iRet = m_pClientList[iToH]->send_msg(cData, 32);
            break;

        case DEF_NOTIFY_ENEMYKILLREWARD:
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)m_pClientList[iToH]->m_iExp;
            cp += 4;
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)m_pClientList[iToH]->m_iEnemyKillCount;
            cp += 4;
            memcpy(cp, m_pClientList[sV1]->m_cCharName, 10);
            cp += 10;
            memcpy(cp, m_pClientList[sV1]->m_cGuildName, 20);
            cp += 20;
            sp = (short *)cp;
            *sp = (short)m_pClientList[sV1]->m_iGuildRank;
            cp += 2;
            sp = (short *)cp;
            *sp = (short)m_pClientList[iToH]->m_iWarContribution;
            cp += 2;

            iRet = m_pClientList[iToH]->send_msg(cData, 48);
            break;

        case DEF_NOTIFY_PKCAPTURED:
            // PKÂ¸Â¦ Ã€Ã¢Â¾Ã’Â´Ã™.
            // PKÃ€Ã‡ PKcount
            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV1;
            cp += 2;
            wp = (uint16_t *)cp;
            *wp = (uint16_t)sV2;
            cp += 2;
            memcpy(cp, pString, 10);
            cp += 10;
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)m_pClientList[iToH]->m_iRewardGold;
            cp += 4;
            dwp = (uint32_t *)cp;
            *dwp = m_pClientList[iToH]->m_iExp;
            cp += 4;

            iRet = m_pClientList[iToH]->send_msg(cData, 28);
            break;

        case DEF_NOTIFY_PKPENALTY:
            // PK Ã†Ã¤Â³ÃŽÃ†Â¼Â¸Â¦ Â¸Ã”Â¾ÃºÂ´Ã™.
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)m_pClientList[iToH]->m_iExp;
            cp += 4;
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)m_pClientList[iToH]->m_iStr;
            cp += 4;
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)m_pClientList[iToH]->m_iVit;
            cp += 4;
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)m_pClientList[iToH]->m_iDex;
            cp += 4;
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)m_pClientList[iToH]->m_iInt;
            cp += 4;
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)m_pClientList[iToH]->m_iMag;
            cp += 4;
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)m_pClientList[iToH]->m_iCharisma;
            cp += 4;
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)m_pClientList[iToH]->m_iPKCount;
            cp += 4;

            iRet = m_pClientList[iToH]->send_msg(cData, 38);
            break;

        case DEF_NOTIFY_TRAVELERLIMITEDLEVEL:
        case DEF_NOTIFY_LIMITEDLEVEL:
            // ÃƒÂ¼Ã‡Ã¨Ã†Ã‡ Â»Ã§Â¿Ã«Ã€ÃšÂ´Ã‚ Â´ÃµÃ€ÃŒÂ»Ã³ Â·Â¹ÂºÂ§Ã€Â» Â¿ÃƒÂ¸Â± Â¼Ã¶ Â¾Ã¸Ã€Â½Ã€Â» Â¾Ã‹Â¸Â°Â´Ã™.
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)m_pClientList[iToH]->m_iExp;
            cp += 4;
            iRet = m_pClientList[iToH]->send_msg(cData, 10);
            break;

        case DEF_NOTIFY_ITEMRELEASED:
        case DEF_NOTIFY_ITEMLIFESPANEND:
            // Ã€Ã¼Ã…ÃµÃÃŸ Â¹Â«Â±Ã¢, ÃˆÂ¤Ã€Âº Â¹Ã¦Â¾Ã®Â±Â¸ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ Â¼Ã¶Â¸Ã­Ã€ÃŒ Â´Ã™Ã‡Ã˜ Â¸ÃÂ°Â¡ÃÂ³Ã€Â½Ã€Â» Â¾Ã‹Â¸Â°Â´Ã™. 
            sp = (short *)cp;
            *sp = (short)sV1;	// Ã€Ã¥Ã‚Ã¸ Ã€Â§Ã„Â¡ 
            cp += 2;
            sp = (short *)cp;
            *sp = (short)sV2;	// Â¾Ã†Ã€ÃŒÃ…Ã› Â¹Ã¸ÃˆÂ£  
            cp += 2;

            iRet = m_pClientList[iToH]->send_msg(cData, 10);
            break;

        case DEF_NOTIFY_KILLED:
            // Â»Ã§Â¸Ã Ã…Ã«ÂºÂ¸ : ÃÃ—Ã€ÃŽ Ã„Â³Â¸Â¯Ã…Ã Ã€ÃŒÂ¸Â§ÂµÂµ Ã‡Ã”Â²Â² ÂºÂ¸Â³Â»ÃÃ˜Â´Ã™. 
            memcpy(cp, pString, 20);
            cp += 20;

            iRet = m_pClientList[iToH]->send_msg(cData, 26);
            break;

        case DEF_NOTIFY_EXP:
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)m_pClientList[iToH]->m_iExp;
            cp += 4;

            ip = (int *)cp;
            *ip = m_pClientList[iToH]->m_iRating;
            cp += 4;

            iRet = m_pClientList[iToH]->send_msg(cData, 14);
            break;

        case DEF_NOTIFY_HP:
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)m_pClientList[iToH]->m_iHP;
            cp += 4;
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)m_pClientList[iToH]->m_iMP; // v2.04 0926 HPÃ€Ã‡ ÂµÃšÂ¿Â¡ MPÂ¸Â¦ Â°Â°Ã€ÃŒ Â¾Ã‹Â·ÃÃÃ˜Â´Ã™. Â¸Â¶Â³ÂªÂºÂ¯ÃˆÂ¯Ã€Ã‡ Ã†Â¯Â¼ÂºÃ„Â¡ Â¶Â§Â¹Â® 
            cp += 4;

            iRet = m_pClientList[iToH]->send_msg(cData, 14);
            break;

        case DEF_NOTIFY_MP:
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)m_pClientList[iToH]->m_iMP;
            cp += 4;

            iRet = m_pClientList[iToH]->send_msg(cData, 10);
            break;

        case DEF_NOTIFY_SP:
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)m_pClientList[iToH]->m_iSP;
            cp += 4;

            iRet = m_pClientList[iToH]->send_msg(cData, 10);
            break;

        case DEF_NOTIFY_CHARISMA:
            dwp = (uint32_t *)cp;
            *dwp = (uint32_t)m_pClientList[iToH]->m_iCharisma;
            cp += 4;

            iRet = m_pClientList[iToH]->send_msg(cData, 10);
            break;

            //MOG Fixes
        case DEF_NOTIFY_STATECHANGE_FAILED:		// 2003-04-14 ÃÃ¶ÃÂ¸ Ã†Ã·Ã€ÃŽÃ†Â®Â¸Â¦ Â·Â¹ÂºÂ§ Â¼Ã¶ÃÂ¤Â¿Â¡ Â½Ã‡Ã†Ã..korean buttplugs
        case DEF_NOTIFY_SETTING_FAILED:
            iRet = m_pClientList[iToH]->send_msg(cData, 6);
            break;

        case DEF_NOTIFY_STATECHANGE_SUCCESS:	// 2003-04-14 ÃÃ¶ÃÂ¸ Ã†Ã·Ã€ÃŽÃ†Â®Â¸Â¦ Â·Â¹ÂºÂ§ Â¼Ã¶ÃÂ¤Â¿Â¡ Â¼ÂºÂ°Ã¸.. wtf korean junk
        {
            int i;

            for (i = 0; i < DEF_MAXMAGICTYPE; i++)
            {
                *cp = m_pClientList[iToH]->m_cMagicMastery[i];
                cp++;
            }

            for (i = 0; i < DEF_MAXSKILLTYPE; i++)
            {
                *cp = m_pClientList[iToH]->m_cSkillMastery[i];
                cp++;
            }

            iRet = m_pClientList[iToH]->send_msg(cData, 6 + DEF_MAXMAGICTYPE + DEF_MAXSKILLTYPE);
        }
        break;

        case DEF_NOTIFY_SETTING_SUCCESS:
        case DEF_NOTIFY_LEVELUP:
            ip = (int *)cp;
            *ip = m_pClientList[iToH]->m_iLevel;
            cp += 4;

            ip = (int *)cp;
            *ip = m_pClientList[iToH]->m_iStr;
            cp += 4;

            ip = (int *)cp;
            *ip = m_pClientList[iToH]->m_iVit;
            cp += 4;

            ip = (int *)cp;
            *ip = m_pClientList[iToH]->m_iDex;
            cp += 4;

            ip = (int *)cp;
            *ip = m_pClientList[iToH]->m_iInt;
            cp += 4;

            ip = (int *)cp;
            *ip = m_pClientList[iToH]->m_iMag;
            cp += 4;

            ip = (int *)cp;
            *ip = m_pClientList[iToH]->m_iCharisma;
            cp += 4;

            iRet = m_pClientList[iToH]->send_msg(cData, 34);
            break;

        case DEF_NOTIFY_QUERY_DISMISSGUILDREQPERMISSION:
        case DEF_NOTIFY_QUERY_JOINGUILDREQPERMISSION:
        case DEF_NOTIFY_CANNOTJOINMOREGUILDSMAN:

            memcpy(cp, m_pClientList[iFromH]->m_cCharName, 10);
            cp += 10;

            iRet = m_pClientList[iToH]->send_msg(cData, 16);
            break;

        case DEF_COMMONTYPE_JOINGUILDAPPROVE:
            if (m_pClientList[iFromH] != 0)
                memcpy(cp, m_pClientList[iFromH]->m_cGuildName, 20);
            else memcpy(cp, "?", 1);
            cp += 20;

            sp = (short *)cp;
            *sp = DEF_GUILDSTARTRANK;
            cp += 2;

            iRet = m_pClientList[iToH]->send_msg(cData, 28);
            break;

        case DEF_COMMONTYPE_JOINGUILDREJECT:
        case DEF_COMMONTYPE_DISMISSGUILDAPPROVE:
        case DEF_COMMONTYPE_DISMISSGUILDREJECT:
            if (m_pClientList[iFromH] != 0)
                memcpy(cp, m_pClientList[iFromH]->m_cGuildName, 20);
            else memcpy(cp, "?", 1);
            cp += 20;

            sp = (short *)cp;
            *sp = DEF_GUILDSTARTRANK;
            cp += 2;

            memcpy(cp, m_pClientList[iToH]->m_cLocation, 10);
            cp += 10;

            iRet = m_pClientList[iToH]->send_msg(cData, 38);
            break;

        case DEF_NOTIFY_GUILDDISBANDED:

            memcpy(cp, pString, 20);
            cp += 20;

            memcpy(cp, m_pClientList[iToH]->m_cLocation, 10);
            cp += 10;

            iRet = m_pClientList[iToH]->send_msg(cData, 36);
            break;

            // v1.4311-3 ÃƒÃŸÂ°Â¡ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¿Â¡Â°Ã” Â»Ã§Ã…ÃµÃ€Ã¥ Â¿Â¹Â¾Ã Ã€ÃŒ ÃƒÃ«Â¼Ã’ÂµÃ‡Â¾ÃºÂ´Ã™Â°Ã­ Â¾Ã‹Â¸Â² ..
        case DEF_NOTIFY_FIGHTZONERESERVE:
            ip = (int *)cp;
            *ip = (int)sV1;
            cp += 4;

            iRet = m_pClientList[iToH]->send_msg(cData, 10);
            break;

            // v1.4311-3 ÃƒÃŸÂ°Â¡ Â±Ã¦ÂµÃ¥ Â¸Â¶Â½ÂºÃ…ÃÂ°Â¡ Â¾Ã†Â´Ã‘Â°Ã¦Â¿Ã¬ .
        case DEF_NOTIFY_NOGUILDMASTERLEVEL:
            iRet = m_pClientList[iToH]->send_msg(cData, 6);
            break;

            // v1.4311-3 ÃƒÃŸÂ°Â¡ Ã€ÃšÂ½Ã…Ã€Ã‡ Â±Ã¦ÂµÃ¥Â¿Ã¸Ã€ÃŒ  Â¾Ã†Â´Ã‘Â°Ã¦Â¿Ã¬ 
        case DEF_NOTIFY_CANNOTBANGUILDMAN:
            iRet = m_pClientList[iToH]->send_msg(cData, 6);
            break;
    }

    switch (iRet)
    {
        case DEF_XSOCKEVENT_QUENEFULL:
        case DEF_XSOCKEVENT_SOCKETERROR:
        case DEF_XSOCKEVENT_CRITICALERROR:
        case DEF_XSOCKEVENT_SOCKETCLOSED:
            // Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ ÂºÂ¸Â³Â¾Â¶Â§ Â¿Â¡Â·Â¯Â°Â¡ Â¹ÃŸÂ»Ã½Ã‡ÃŸÂ´Ã™Â¸Ã© ÃÂ¦Â°Ã…Ã‡Ã‘Â´Ã™.
            // ÃƒÂ³Â¸Â® ÂµÂµÃÃŸ Â¿Ã€Â·Ã¹Â°Â¡ Â¹ÃŸÂ»Ã½Ã‡ÃÂ´Ã‚ Â°ÃÃ€Â» Â¸Â·Â±Ã¢ Ã€Â§Ã‡Ã˜ ÃÃ¶Â¿Ã¬ÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™. Time OutÃ€Â¸Â·ÃŽ Â»Ã¨ÃÂ¦ÂµÃ‰ Â°ÃÃ€Ã“. 
            //DeleteClient(iToH, true, true);
            return;
    }
}

uint32_t CGame::dwGetItemCount(int iClientH, char * pName)
{
    int i;
    char cTmpName[21];

    if (m_pClientList[iClientH] == nullptr) return 0;

    memset(cTmpName, 0, sizeof(cTmpName));
    strcpy(cTmpName, pName);

    for (i = 0; i < DEF_MAXITEMS; i++)
        if ((m_pClientList[iClientH]->m_pItemList[i] != 0) && (memcmp(m_pClientList[iClientH]->m_pItemList[i]->m_cName, cTmpName, 20) == 0))
        {

            return m_pClientList[iClientH]->m_pItemList[i]->m_dwCount;
        }

    return 0;
}

int CGame::SetItemCount(int iClientH, char * pItemName, uint32_t dwCount)
{
    int i;
    char cTmpName[21];
    uint16_t wWeight;

    if (m_pClientList[iClientH] == nullptr) return -1;

    memset(cTmpName, 0, sizeof(cTmpName));
    strcpy(cTmpName, pItemName);

    for (i = 0; i < DEF_MAXITEMS; i++)
        if ((m_pClientList[iClientH]->m_pItemList[i] != 0) && (memcmp(m_pClientList[iClientH]->m_pItemList[i]->m_cName, cTmpName, 20) == 0))
        {

            wWeight = iGetItemWeight(m_pClientList[iClientH]->m_pItemList[i], 1);// m_pClientList[iClientH]->m_pItemList[i]->m_wWeight;

            // Ã„Â«Â¿Ã®Ã†Â®Â°Â¡ 0Ã€ÃŒÂ¸Ã© Â¸Ã°ÂµÃŽ Â¼Ã’Â¸Ã°ÂµÃˆ Â°ÃÃ€ÃŒÂ¹Ã‡Â·ÃŽ Â¸Â®Â½ÂºÃ†Â®Â¿Â¡Â¼Â­ Â»Ã¨ÃÂ¦Ã‡Ã‘Â´Ã™.
            if (dwCount == 0)
            {
                ItemDepleteHandler(iClientH, i, false);
            }
            else
            {
                // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ Â¼Ã¶Â·Â®Ã€ÃŒ ÂºÂ¯Â°Ã¦ÂµÃ‡Â¾ÃºÃ€Â½Ã€Â» Â¾Ã‹Â¸Â°Â´Ã™. 
                m_pClientList[iClientH]->m_pItemList[i]->m_dwCount = dwCount;
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_SETITEMCOUNT, i, dwCount, (char)true, 0);
            }

            return wWeight;
        }

    return -1;
}

int CGame::SetItemCount(int iClientH, int iItemIndex, uint32_t dwCount)
{
    uint16_t wWeight;

    if (m_pClientList[iClientH] == nullptr) return -1;
    if (m_pClientList[iClientH]->m_pItemList[iItemIndex] == 0) return -1;

    wWeight = iGetItemWeight(m_pClientList[iClientH]->m_pItemList[iItemIndex], 1);//m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wWeight;

    // Ã„Â«Â¿Ã®Ã†Â®Â°Â¡ 0Ã€ÃŒÂ¸Ã© Â¸Ã°ÂµÃŽ Â¼Ã’Â¸Ã°ÂµÃˆ Â°ÃÃ€ÃŒÂ¹Ã‡Â·ÃŽ Â¸Â®Â½ÂºÃ†Â®Â¿Â¡Â¼Â­ Â»Ã¨ÃÂ¦Ã‡Ã‘Â´Ã™.
    if (dwCount == 0)
    {
        ItemDepleteHandler(iClientH, iItemIndex, false);
    }
    else
    {
        // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ Â¼Ã¶Â·Â®Ã€ÃŒ ÂºÂ¯Â°Ã¦ÂµÃ‡Â¾ÃºÃ€Â½Ã€Â» Â¾Ã‹Â¸Â°Â´Ã™. 
        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwCount = dwCount;
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SETITEMCOUNT, iItemIndex, dwCount, (char)true, 0);
    }

    return wWeight;
}

void CGame::EKAnnounce(int iClientH, char * pMsg)
{
    char * cp, cTemp[256];
    uint32_t * dwp, dwMsgSize;
    uint16_t * wp;
    short * sp;

    memset(cTemp, 0, sizeof(cTemp));

    dwp = (uint32_t *)cTemp;
    *dwp = MSGID_COMMAND_CHATMSG;


    wp = (uint16_t *)(cTemp + DEF_INDEX2_MSGTYPE);
    *wp = 0;

    cp = (char *)(cTemp + DEF_INDEX2_MSGTYPE + 2);
    sp = (short *)cp;
    *sp = 0;
    cp += 2;

    sp = (short *)cp;
    *sp = 0;
    cp += 2;

    memcpy(cp, "Announcer", 9);
    cp += 10;

    *cp = 10;
    cp++;

    dwMsgSize = strlen(pMsg);
    memcpy(cp, pMsg, dwMsgSize);
    cp += dwMsgSize;

    m_pClientList[iClientH]->send_msg(cTemp, dwMsgSize + 22);
}

bool CGame::_bInitNpcAttr(class CNpc * pNpc, char * pNpcName, short sClass, char cSA)
{
    int i, iTemp;
    char cTmpName[21];
    int sTemp;
    double dV1, dV2, dV3;

    memset(cTmpName, 0, sizeof(cTmpName));
    strcpy(cTmpName, pNpcName);

    for (i = 0; i < DEF_MAXNPCTYPES; i++)
        if (m_pNpcConfigList[i] != 0)
        {
            if (memcmp(cTmpName, m_pNpcConfigList[i]->m_cNpcName, 20) == 0)
            {
                // Â°Â°Ã€Âº Ã€ÃŒÂ¸Â§Ã€Â» Â°Â¡ÃÃ¸ NPC Â¼Â³ÃÂ¤Ã€Â» ÃƒÂ£Â¾Ã’Â´Ã™. Â¼Â³ÃÂ¤Â°ÂªÃ€Â¸Â·ÃŽ ÃƒÃŠÂ±Ã¢ÃˆÂ­Ã‡Ã‘Â´Ã™.
                memset(pNpc->m_cNpcName, 0, sizeof(pNpc->m_cNpcName));
                memcpy(pNpc->m_cNpcName, m_pNpcConfigList[i]->m_cNpcName, 20);

                pNpc->m_sType = m_pNpcConfigList[i]->m_sType;

                // HitDiceÂ¿Â¡ ÂµÃ»Â¶Ã³ Â°Ã¸Â½Ã„Ã€ÃŒ Â´Ã™Â¸Â£Â´Ã™. Â¸Ã·Â°Â£Ã€Ã‡ Ã‚Ã·ÂºÂ°Ã€Â» ÂµÃŽÂ±Ã¢ Ã€Â§Ã‡Ã”.
                if (m_pNpcConfigList[i]->m_iHitDice <= 5)
                    pNpc->m_iHP = abs(iDice(m_pNpcConfigList[i]->m_iHitDice, 4) + m_pNpcConfigList[i]->m_iHitDice);
                else pNpc->m_iHP = abs((m_pNpcConfigList[i]->m_iHitDice * 4) + m_pNpcConfigList[i]->m_iHitDice + iDice(1, m_pNpcConfigList[i]->m_iHitDice));
                // v1.4 ÃˆÂ®Ã€ÃŽÃ„ÃšÂµÃ¥
                if (pNpc->m_iHP == 0) pNpc->m_iHP = 1;

                pNpc->m_iExpDiceMin = m_pNpcConfigList[i]->m_iExpDiceMin;
                pNpc->m_iExpDiceMax = m_pNpcConfigList[i]->m_iExpDiceMax;
                pNpc->m_iGoldDiceMin = m_pNpcConfigList[i]->m_iGoldDiceMin;
                pNpc->m_iGoldDiceMax = m_pNpcConfigList[i]->m_iGoldDiceMax;
                pNpc->m_iExp = (iDice(1, (m_pNpcConfigList[i]->m_iExpDiceMax - m_pNpcConfigList[i]->m_iExpDiceMin)) + m_pNpcConfigList[i]->m_iExpDiceMin);

                pNpc->m_iHitDice = m_pNpcConfigList[i]->m_iHitDice;
                pNpc->m_iDefenseRatio = m_pNpcConfigList[i]->m_iDefenseRatio;
                pNpc->m_iHitRatio = m_pNpcConfigList[i]->m_iHitRatio;
                pNpc->m_iMinBravery = m_pNpcConfigList[i]->m_iMinBravery;
                pNpc->m_cAttackDiceThrow = m_pNpcConfigList[i]->m_cAttackDiceThrow;
                pNpc->m_cAttackDiceRange = m_pNpcConfigList[i]->m_cAttackDiceRange;
                pNpc->m_cSize = m_pNpcConfigList[i]->m_cSize;
                pNpc->m_cSide = m_pNpcConfigList[i]->m_cSide;
                pNpc->m_cActionLimit = m_pNpcConfigList[i]->m_cActionLimit;
                pNpc->m_dwActionTime = m_pNpcConfigList[i]->m_dwActionTime;
                pNpc->m_dwRegenTime = m_pNpcConfigList[i]->m_dwRegenTime;
                pNpc->m_cResistMagic = m_pNpcConfigList[i]->m_cResistMagic;
                pNpc->m_cMagicLevel = m_pNpcConfigList[i]->m_cMagicLevel;
                pNpc->m_iMaxMana = m_pNpcConfigList[i]->m_iMaxMana; // v1.4
                pNpc->m_iMana = m_pNpcConfigList[i]->m_iMaxMana;
                pNpc->m_cChatMsgPresence = m_pNpcConfigList[i]->m_cChatMsgPresence;
                pNpc->m_cDayOfWeekLimit = m_pNpcConfigList[i]->m_cDayOfWeekLimit;
                pNpc->m_cTargetSearchRange = m_pNpcConfigList[i]->m_cTargetSearchRange;

                switch (sClass)
                {
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47: // Ã€Ã¼Ã€Ã¯Â¿Ã« Ã€Â¯Â´Ã–Ã€Ã‡ Â°Ã¦Â¿Ã¬ Â¹Â«ÃÂ¶Â°Ã‡ Â°Ã¸Â°ÃÂ¸Â¸ Ã‡ÃÂ´Ã‚ Ã€Ã¼Â·Â«Ã€Â» Â±Â¸Â»Ã§.
                        pNpc->m_iAttackStrategy = DEF_ATTACKAI_NORMAL;
                        break;

                    default:
                        pNpc->m_iAttackStrategy = iDice(1, 10);
                        break;
                }

                pNpc->m_iAILevel = iDice(1, 3);
                pNpc->m_iAbsDamage = m_pNpcConfigList[i]->m_iAbsDamage;
                pNpc->m_iMagicHitRatio = m_pNpcConfigList[i]->m_iMagicHitRatio;
                pNpc->m_iAttackRange = m_pNpcConfigList[i]->m_iAttackRange;
                pNpc->m_cSpecialAbility = cSA;
                pNpc->m_iBuildCount = m_pNpcConfigList[i]->m_iMinBravery;

                // v1.411 NPCÃ€Ã‡ Ã†Â¯Â¼Ã¶ ÃˆÂ¿Â°Ãº Â°Ã¨Â»Ãª. Â°Ã¦Ã‡Ã¨Ã„Â¡ Â°Â¡ÃÃŸ 
                switch (pNpc->m_cSpecialAbility)
                {
                    case 1:
                        dV2 = (double)pNpc->m_iExp;
                        dV3 = 25.0f / 100.0f;
                        dV1 = dV2 * dV3;
                        pNpc->m_iExp += (int)dV1;
                        break;

                    case 2:
                        dV2 = (double)pNpc->m_iExp;
                        dV3 = 30.0f / 100.0f;
                        dV1 = dV2 * dV3;
                        pNpc->m_iExp += (int)dV1;
                        break;

                    case 3: // Absorbing Physical Damage
                        if (pNpc->m_iAbsDamage > 0)
                        {
                            // Â±Ã¢ÂºÂ»Ã€Ã»Ã€Â¸Â·ÃŽ Â¸Â¶Â¹Ã½ Â´Ã«Â¹ÃŒÃÃ¶ Ã€Ã½Â°Â¨ Â´Ã‰Â·Ã‚Ã€ÃŒ Ã€Ã–Â´Ã‚ Â°Ã¦Â¿Ã¬ Â¹Â°Â¸Â® Â´Ã«Â¹ÃŒÃÃ¶ Ã€Ã½Â°Â¨Ã€Âº Ã€Ã»Â¿Ã«ÂµÃ‡ÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™. 
                            pNpc->m_cSpecialAbility = 0;
                            cSA = 0;
                        }
                        else
                        {
                            iTemp = 20 + iDice(1, 60);
                            pNpc->m_iAbsDamage -= iTemp;
                            if (pNpc->m_iAbsDamage < -90) pNpc->m_iAbsDamage = -90;
                        }

                        dV2 = (double)pNpc->m_iExp;
                        dV3 = (double)abs(pNpc->m_iAbsDamage) / 100.0f;
                        dV1 = dV2 * dV3;
                        pNpc->m_iExp += (int)dV1;
                        break;

                    case 4: // Absorbing Magical Damage
                        if (pNpc->m_iAbsDamage < 0)
                        {
                            // Â±Ã¢ÂºÂ»Ã€Ã»Ã€Â¸Â·ÃŽ Â¹Â°Â¸Â® Â´Ã«Â¹ÃŒÃÃ¶ Ã€Ã½Â°Â¨ Â´Ã‰Â·Ã‚Ã€ÃŒ Ã€Ã–Â´Ã‚ Â°Ã¦Â¿Ã¬ Â¸Â¶Â¹Ã½ Â´Ã«Â¹ÃŒÃÃ¶ Ã€Ã½Â°Â¨Ã€Âº Ã€Ã»Â¿Ã«ÂµÃ‡ÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™. 
                            pNpc->m_cSpecialAbility = 0;
                            cSA = 0;
                        }
                        else
                        {
                            iTemp = 20 + iDice(1, 60);
                            pNpc->m_iAbsDamage += iTemp;
                            if (pNpc->m_iAbsDamage > 90) pNpc->m_iAbsDamage = 90;
                        }

                        dV2 = (double)pNpc->m_iExp;
                        dV3 = (double)(pNpc->m_iAbsDamage) / 100.0f;
                        dV1 = dV2 * dV3;
                        pNpc->m_iExp += (int)dV1;
                        break;

                    case 5:
                        dV2 = (double)pNpc->m_iExp;
                        dV3 = 15.0f / 100.0f;
                        dV1 = dV2 * dV3;
                        pNpc->m_iExp += (int)dV1;
                        break;

                    case 6:
                    case 7:
                        dV2 = (double)pNpc->m_iExp;
                        dV3 = 20.0f / 100.0f;
                        dV1 = dV2 * dV3;
                        pNpc->m_iExp += (int)dV1;
                        break;

                    case 8:
                        dV2 = (double)pNpc->m_iExp;
                        dV3 = 25.0f / 100.0f;
                        dV1 = dV2 * dV3;
                        pNpc->m_iExp += (int)dV1;
                        break;
                }

                // v1.411 Ã€Â§Ã„Â¡ Â¿Ã…Â±Ã¨ 
                pNpc->m_iNoDieRemainExp = (pNpc->m_iExp) - (pNpc->m_iExp / 3);

                // v1.411 NPCÃ€Ã‡ Ã†Â¯Â¼Ã¶ Â´Ã‰Â·Ã‚ Â»Ã°Ã€Ã” 
                pNpc->m_iStatus = pNpc->m_iStatus & 0xFFFFF0FF;
                sTemp = cSA;
                sTemp = sTemp << 8;
                pNpc->m_iStatus = pNpc->m_iStatus | sTemp;

                // v1.41 NPCÃ€Ã‡ ClassÂ»Ã°Ã€Ã”.
                pNpc->m_iStatus = pNpc->m_iStatus & 0xFFFFFFF0;
                pNpc->m_iStatus = pNpc->m_iStatus | (sClass);

                return true;
            }
        }

    // NPCÂ¸Â®Â½ÂºÃ†Â®Â¸Â¦ Â¸Ã°ÂµÃŽ Â°Ã‹Â»Ã¶Ã‡ÃŸÃ€Â½Â¿Â¡ÂµÂµ Â¹ÃŸÂ°ÃŸÂµÃ‡ÃÃ¶ Â¾ÃŠÂ¾Ã’Â´Ã™. 
    return false;
}

/*********************************************************************************************************************
**  int CGame::iDice(int iThrow, int iRange)																		**
**  description			:: produces a random number between the throw and range										**
**  last updated		:: November 20, 2004; 10:24 PM; Hypnotoad													**
**	return value		:: int																						**
**********************************************************************************************************************/
int CGame::iDice(int iThrow, int iRange)
{
    int i, iRet;

    if (iRange <= 0) return 0;
    iRet = 0;
    for (i = 1; i <= iThrow; i++)
    {
        iRet += (rand() % iRange) + 1;
    }
    return iRet;
}

void CGame::CalculateGuildEffect(int iVictimH, char cVictimType, short sAttackerH)
{
    int ix, iy, iExp;
    short sOwnerH;
    char cOwnerType;

    // ### ERROR POINT! Ã€Ã“Â½ÃƒÂ·ÃŽ Â¸Â·Â¾Ã† Â³Ã­Â´Ã™.
    return;

    if (m_pClientList[sAttackerH] == 0) return;

    switch (cVictimType)
    {

        case DEF_OWNERTYPE_NPC:
            if (m_pNpcList[iVictimH] == 0) return;

            for (ix = m_pNpcList[iVictimH]->m_sX - 2; ix <= m_pNpcList[iVictimH]->m_sX + 2; ix++)
                for (iy = m_pNpcList[iVictimH]->m_sY - 2; iy <= m_pNpcList[iVictimH]->m_sY + 2; iy++)
                {

                    m_pMapList[m_pNpcList[iVictimH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);

                    if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (sOwnerH != 0) && (sOwnerH != sAttackerH) &&
                        (memcmp(m_pClientList[sAttackerH]->m_cGuildName, m_pClientList[sOwnerH]->m_cGuildName, 20) == 0) &&
                        (m_pClientList[sAttackerH]->m_iLevel >= m_pClientList[sOwnerH]->m_iLevel))
                    {
                        // Â±Ã¦ÂµÃ¥Â¿Ã¸Â°ÃºÃ€Ã‡ Ã‡Ã¹ÂµÂ¿Ã€Ã¼Ã…ÃµÂ¿Â¡Â¼Â­ Â°Ã¦Ã‡Ã¨Ã„Â¡Â¸Â¦ Â¾Ã²Â±Ã¢ Ã€Â§Ã‡Ã˜Â¼Â­Â´Ã‚ Â¸Ã³Â½ÂºÃ…ÃÂ¸Â¦ ÃÃ—Ã€ÃŽ Â±Ã¦ÂµÃ¥Â¿Ã¸Ã€Ã‡ Â·Â¹ÂºÂ§ÂºÂ¸Â´Ã™ Â³Â·Ã€Âº Â±Ã¦ÂµÃ¥Â¿Ã¸Ã€ÃŒÂ¾Ã®Â¾ÃŸ Ã‡Ã‘Â´Ã™.
                        // 1D3Ã€Ã‡ Â°ÂªÃ€ÃŒ 2Ã€ÃÂ¶Â§ Â¸Ã³Â½ÂºÃ…Ã Â°Ã¦Ã‡Ã¨Ã„Â¡Ã€Ã‡ 3ÂºÃÃ€Ã‡ 1Ã€Â» Â¾Ã²Â´Ã‚Â´Ã™. 
                        if (iDice(1, 3) == 2)
                        {
                            iExp = (m_pNpcList[iVictimH]->m_iExp / 3);
                            if (m_pClientList[sAttackerH]->m_iLevel >= m_iPlayerMaxLevel) iExp = 0;

                            if (iExp > 0)
                            {

                                m_pClientList[sOwnerH]->m_iExp += iExp;
                                if (bCheckLimitedUser(sOwnerH) == false)
                                {
                                    // ÃƒÂ¼Ã‡Ã¨Ã†Ã‡ Â»Ã§Â¿Ã«Ã€Ãš ÃÂ¦Ã‡Ã‘Â¿Â¡ Â°Ã‰Â¸Â®ÃÃ¶ Â¾ÃŠÂ¾Ã’Â´Ã™. Â°Ã¦Ã‡Ã¨Ã„Â¡Â°Â¡ Â¿ÃƒÂ¶ÃºÂ´Ã™Â´Ã‚ Ã…Ã«ÂºÂ¸Â¸Â¦ Ã‡Ã‘Â´Ã™.
                                    SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_EXP, 0, 0, 0, 0);
                                }
                                // Â·Â¹ÂºÂ§Ã€ÃŒ Â¿ÃƒÂ¶ÃºÂ´Ã‚ÃÃ¶Â¸Â¦ Â°Ã‹Â»Ã§Ã‡Ã‘Â´Ã™.
                                bCheckLevelUp(sOwnerH);
                            }
                        }
                    }
                }
            break;
    }
}

void CGame::TimeManaPointsUp(int iClientH)
{
    int iMaxMP, iTotal;
    double dV1, dV2, dV3;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsKilled == true) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    if (m_pClientList[iClientH]->m_iHungerStatus <= 0) return;
    if (m_pClientList[iClientH]->m_bSkillUsingStatus[19] == true) return;

    iMaxMP = (2 * m_pClientList[iClientH]->m_iMag) + (2 * m_pClientList[iClientH]->m_iLevel) + (m_pClientList[iClientH]->m_iInt / 2); // v1.4
    if (m_pClientList[iClientH]->m_iMP < iMaxMP)
    {
        iTotal = iDice(1, (m_pClientList[iClientH]->m_iMag));
        if (m_pClientList[iClientH]->m_iAddMP != 0)
        {
            dV2 = (double)iTotal;
            dV3 = (double)m_pClientList[iClientH]->m_iAddMP;
            dV1 = (dV3 / 100.0f) * dV2;
            iTotal += (int)dV1;
        }

        m_pClientList[iClientH]->m_iMP += iTotal;

        if (m_pClientList[iClientH]->m_iMP > iMaxMP)
            m_pClientList[iClientH]->m_iMP = iMaxMP;

        SendNotifyMsg(0, iClientH, DEF_NOTIFY_MP, 0, 0, 0, 0);
    }
}

void CGame::TimeStaminarPointsUp(int iClientH)
{
    int iMaxSP, iTotal = 0;
    double dV1, dV2, dV3;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsKilled == true) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    if (m_pClientList[iClientH]->m_iHungerStatus <= 0) return;
    if (m_pClientList[iClientH]->m_bSkillUsingStatus[19] == true) return; // v1.4 ÃƒÂÃƒâ€”Ãƒâ‚¬Ã‚ÂºÃƒÆ’Ã‚Â´Ãƒâ€¡ÃƒÂÃ‚Â±ÃƒÂ¢ Ã‚Â±ÃƒÂ¢Ã‚Â¼ÃƒÂº Ã‚Â»ÃƒÂ§Ã‚Â¿ÃƒÂ«ÃƒÂÃƒÅ¸Ã‚Â¿Ã‚Â¡Ã‚Â´Ãƒâ€š Ã‚Â½Ã‚ÂºÃƒâ€¦Ãƒâ€”Ã‚Â¹ÃƒÅ’Ã‚Â³ÃƒÅ  Ã‚Â¿Ãƒâ‚¬Ã‚Â¸Ã‚Â£ÃƒÂÃƒÂ¶ Ã‚Â¾ÃƒÅ Ãƒâ‚¬Ã‚Â½.

    iMaxSP = (2 * m_pClientList[iClientH]->m_iStr) + (2 * m_pClientList[iClientH]->m_iLevel);
    if (m_pClientList[iClientH]->m_iSP < iMaxSP)
    {

        iTotal = iDice(1, (m_pClientList[iClientH]->m_iVit / 3)); // Staminar PointÂ´Ã‚ 10ÃƒÃŠÂ¸Â¶Â´Ã™ 1D(Vit/3)Â¾Â¿ Â¿ÃƒÂ¶Ã³Â°Â£Â´Ã™.
        if (m_pClientList[iClientH]->m_iAddSP != 0)
        {
            dV2 = (double)iTotal;
            dV3 = (double)m_pClientList[iClientH]->m_iAddSP;
            dV1 = (dV3 / 100.0f) * dV2;
            iTotal += (int)dV1;
        }

        // v2.03 Â·Â¹ÂºÂ§ 60 Ã€ÃŒÃ‡ÃÂ´Ã‚ ÃÂ¤Â±Ã¢Ã€Ã»Ã€Â¸Â·ÃŽ Â½ÂºÃ…Ã—Â¹ÃŒÂ³ÂªÂ°Â¡ Â¸Â¹Ã€ÃŒ Ã‚Ã¹Â´Ã™.
        if (m_pClientList[iClientH]->m_iLevel <= 20)
        {
            iTotal += 15;
        }
        else if (m_pClientList[iClientH]->m_iLevel <= 40)
        {
            iTotal += 10;
        }
        else if (m_pClientList[iClientH]->m_iLevel <= 60)
        {
            iTotal += 5;
        }

        m_pClientList[iClientH]->m_iSP += iTotal;
        if (m_pClientList[iClientH]->m_iSP > iMaxSP)
            m_pClientList[iClientH]->m_iSP = iMaxSP;

        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SP, 0, 0, 0, 0);
    }
}

void CGame::DelayEventProcess()
{

}

void CGame::SendGuildMsg(int iClientH, uint16_t wNotifyMsgType, short sV1, short sV2, char * pString)
{
    char cData[500];
    uint32_t * dwp;
    uint16_t * wp;
    char * cp;
    int i, iRet;

    // Â°Â°Ã€Âº Â±Ã¦ÂµÃ¥Â¿Ã¸ÂµÃ©Â¿Â¡Â°Ã”Â¸Â¸ ÂºÂ¸Â³Â»Â´Ã‚ Â¸ÃžÂ½ÃƒÃÃ¶ÂµÃ©
    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

    for (i = 0; i < DEF_MAXCLIENTS; i++)
        if ((m_pClientList[i] != 0) &&
            (memcmp(m_pClientList[i]->m_cGuildName, m_pClientList[iClientH]->m_cGuildName, 20) == 0))
        {

            // ### BUG POINT Ã€Â§Ã„Â¡Â°Â¡ Ã€ÃŸÂ¸Ã¸ÂµÃ‡Â¾Ã® Ã†Ã·Ã€ÃŽÃ…Ã Â¿Â¬Â»ÃªÃ€ÃŒ Ã€ÃŸÂ¸Ã¸ÂµÃ‡Â¾ÃºÂ´Ã™. 
            memset(cData, 0, sizeof(cData));

            dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
            *dwp = MSGID_NOTIFY;
            wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
            *wp = wNotifyMsgType;

            cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);

            // Â°Â°Ã€Âº Â±Ã¦ÂµÃ¥ Ã€ÃŒÂ¸Â§Ã€Â» Â°Â®Â°Ã­ Ã€Ã–Â´Ã‚ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¸Â¦ ÃƒÂ£Â¾Ã’Â´Ã™.
            switch (wNotifyMsgType)
            {
                case DEF_NOTIFY_GUILDDISBANDED:
                    if (i == iClientH) break; // <-- Â±Ã¦ÂµÃ¥ Â¸Â¶Â½ÂºÃ…Ã Ã€ÃšÂ½Ã…Â¿Â¡Â°Ã”Â´Ã‚ Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ ÂºÂ¸Â³Â»ÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™.
                    // Â±Ã¦ÂµÃ¥ Ã‡Ã˜Â»Ãª Ã…Ã«ÂºÂ¸Â¿Â¡ Ã‡Ã˜Â´Ã§ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Ã€Ã‡ Â±Ã¦ÂµÃ¥ Â·Â©Ã…Â©Â¸Â¦ Ã…Â¬Â¸Â®Â¾Ã®Ã‡Ã‘Â´Ã™. 
                    memcpy(cp, m_pClientList[iClientH]->m_cGuildName, 20);
                    cp += 20;

                    iRet = m_pClientList[i]->send_msg(cData, 26);
                    // Ã‡Ã˜Â´Ã§ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Ã€Ã‡ Â±Ã¦ÂµÃ¥Â³Â»Â¿Ã«Ã€Â» Ã…Â¬Â¸Â®Â¾Ã®Ã‡Ã‘Â´Ã™. @@@@@@@
                    memset(m_pClientList[i]->m_cGuildName, 0, sizeof(m_pClientList[i]->m_cGuildName));
                    strcpy(m_pClientList[i]->m_cGuildName, "NONE");
                    m_pClientList[i]->m_iGuildRank = -1;
                    m_pClientList[i]->m_iGuildGUID = -1;
                    break;

                case DEF_NOTIFY_EVENTMSGSTRING:
                    // Â±Ã¦ÂµÃ¥Â¿Ã¸ÂµÃ©Â¿Â¡Â°Ã” Ã€Ã¼Â´ÃžÂµÃ‡Â´Ã‚ Ã€ÃŒÂºÂ¥Ã†Â® Â¸ÃžÂ½ÃƒÃÃ¶ Â½ÂºÃ†Â®Â¸Âµ 
                    strcpy(cp, pString);
                    cp += strlen(pString);

                    iRet = m_pClientList[i]->send_msg(cData, 6 + strlen(pString) + 1);
                    break;

                case DEF_NOTIFY_NEWGUILDSMAN:
                    memcpy(cp, m_pClientList[iClientH]->m_cCharName, 10);
                    cp += 10;

                    iRet = m_pClientList[i]->send_msg(cData, 6 + 10 + 1);
                    break;

                case DEF_NOTIFY_DISMISSGUILDSMAN:
                    memcpy(cp, m_pClientList[iClientH]->m_cCharName, 10);
                    cp += 10;

                    iRet = m_pClientList[i]->send_msg(cData, 6 + 10 + 1);
                    break;
            }

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
        }

}

void CGame::ReleaseFollowMode(short sOwnerH, char cOwnerType)
{
    int i;

    for (i = 0; i < DEF_MAXNPCS; i++)
        if ((i != sOwnerH) && (m_pNpcList[i] != 0))
        {
            if ((m_pNpcList[i]->m_cMoveType == DEF_MOVETYPE_FOLLOW) &&
                (m_pNpcList[i]->m_iFollowOwnerIndex == sOwnerH) &&
                (m_pNpcList[i]->m_cFollowOwnerType == cOwnerType))
            {

                m_pNpcList[i]->m_cMoveType = DEF_MOVETYPE_RANDOMWAYPOINT;
            }
        }
}

int CGame::_iGetMagicNumber(char * pMagicName, int * pReqInt, int * pCost)
{
    int i;
    char cTmpName[31];

    memset(cTmpName, 0, sizeof(cTmpName));
    strcpy(cTmpName, pMagicName);

    for (i = 0; i < DEF_MAXMAGICTYPE; i++)
        if (m_pMagicConfigList[i] != 0)
        {
            if (memcmp(cTmpName, m_pMagicConfigList[i]->m_cName, 30) == 0)
            {
                // Â°Â°Ã€Âº Ã€ÃŒÂ¸Â§Ã€Â» Â°Â¡ÃÃ¸ Â¸Â¶Â¹Ã½ Â¼Â³ÃÂ¤Ã€Â» ÃƒÂ£Â¾Ã’Â´Ã™. Â¸Â¶Â¹Ã½ Â¹Ã¸ÃˆÂ£Â¸Â¦ Â¹ÃÃˆÂ¯Ã‡Ã‘Â´Ã™.
                *pReqInt = (int)m_pMagicConfigList[i]->m_sIntLimit;
                *pCost = (int)m_pMagicConfigList[i]->m_iGoldCost;

                return i;
            }
        }

    return -1;
}

void CGame::TrainSkillResponse(bool bSuccess, int iClientH, int iSkillNum, int iSkillLevel)
{
    char * cp, cData[100];
    uint32_t * dwp;
    uint16_t * wp;
    int   iRet;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    if ((iSkillNum < 0) || (iSkillNum > 100)) return;
    if ((iSkillLevel < 0) || (iSkillLevel > 100)) return;

    if (bSuccess == true)
    {
        // ÀÌ¹Ì ±â¼úÀ» ÀÍÈù »óÅÂ¶ó¸é ¼Ò¿ëÀÌ ¾ø´Ù.
        if (m_pClientList[iClientH]->m_cSkillMastery[iSkillNum] != 0) return;

        m_pClientList[iClientH]->m_cSkillMastery[iSkillNum] = iSkillLevel;
        // ±â¼ú ÃÑ ÇÕÀ» Àç °è»êÇÑ´Ù.
        bCheckTotalSkillMasteryPoints(iClientH, iSkillNum);

        // ±â¼úÀ» ¹è¿ü´Ù´Â ¸Þ½ÃÁö¸¦ Àü¼ÛÇÑ´Ù.
        dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
        *dwp = MSGID_NOTIFY;
        wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
        *wp = DEF_NOTIFY_SKILLTRAINSUCCESS;

        cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);

        // ±â¼ú¹øÈ£
        *cp = iSkillNum;
        cp++;

        // ±â¼ú ·¹º§ .
        *cp = iSkillLevel;
        cp++;


        if (m_pSkillConfigList[iSkillNum]->m_cName != 0)
            _bItemLog(DEF_ITEMLOG_SKILLLEARN, iClientH, m_pSkillConfigList[iSkillNum]->m_cName, 0);

        // Á¤º¸ Àü¼Û 
        iRet = m_pClientList[iClientH]->send_msg(cData, 8);
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


    }

}

int CGame::_iGetSkillNumber(char * pSkillName)
{
    int i;
    char cTmpName[21];

    memset(cTmpName, 0, sizeof(cTmpName));
    strcpy(cTmpName, pSkillName);

    for (i = 1; i < DEF_MAXSKILLTYPE; i++)
        if (m_pSkillConfigList[i] != 0)
        {
            if (memcmp(cTmpName, m_pSkillConfigList[i]->m_cName, 20) == 0)
            {
                // Â°Â°Ã€Âº Ã€ÃŒÂ¸Â§Ã€Â» Â°Â¡ÃÃ¸ Â±Ã¢Â¼Ãº Â¼Â³ÃÂ¤Ã€Â» ÃƒÂ£Â¾Ã’Â´Ã™. Â±Ã¢Â¼Ãº Â¹Ã¸ÃˆÂ£Â¸Â¦ Â¹ÃÃˆÂ¯Ã‡Ã‘Â´Ã™.
                return i;
            }
        }

    return 0;
}

bool CGame::bPlayerItemToBank(int iClientH, short sItemIndex)
{
    int i, iIndex;

    if (m_pClientList[iClientH] == nullptr) return false;

    for (i = 0; i < DEF_MAXBANKITEMS; i++)
        if (m_pClientList[iClientH]->m_pItemInBankList[i] == 0)
        {
            iIndex = i;
            goto NEXT_STEP_PLTB;
        }
    // Â´ÃµÃ€ÃŒÂ»Ã³ Ã€ÃºÃ€Ã¥Ã‡Ã’ Â°Ã¸Â°Â£Ã€ÃŒ Â¾Ã¸Â´Ã™. 
    return false;

    NEXT_STEP_PLTB:;

    // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Ã€ÃºÃ€Ã¥Ã‡Ã’ Â°Ã¸Â°Â£Ã€ÃŒ Â³Â²Â¾Ã†Ã€Ã–Â´Ã™. 
    // Â¸Ã•Ã€Ãº Ã€Ã¥Ã‚Ã¸ÂµÃ‡Â¾Ã® Ã€Ã–Â´Ã™Â¸Ã© Ã‡Ã˜ÃÂ¦Â½ÃƒÃ…Â²Â´Ã™.
    ReleaseItemHandler(iClientH, sItemIndex, true);

    // Â¾Ã†Ã€ÃŒÃ…Ã› Ã…Â¬Â·Â¡Â½ÂºÃ€Ã‡ ÃÃ–Â¼Ã’Â¸Â¦ Â¹Ã™Â²Ã›Â´Ã™. 
    m_pClientList[iClientH]->m_pItemInBankList[iIndex] = m_pClientList[iClientH]->m_pItemList[sItemIndex];
    // Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã® Â¾Ã†Ã€ÃŒÃ…Ã› Â¸Â®Â½ÂºÃ†Â®Â¸Â¦ Ã…Â¬Â¸Â®Â¾Ã®Ã‡ÃÂ°Ã­ 
    m_pClientList[iClientH]->m_pItemList[sItemIndex] = 0;
    m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] = false;

    // Â¾Ã†Ã€ÃŒÃ…Ã› Â¸Â®Â½ÂºÃ†Â®Ã€Ã‡ ÂºÃ³ Â°Ã¸Â°Â£Ã€Â» Â»Ã¨ÃÂ¦Ã‡Ã‘Â´Ã™.
    for (i = 1; i < DEF_MAXITEMS; i++)
        if ((m_pClientList[iClientH]->m_pItemList[i - 1] == 0) && (m_pClientList[iClientH]->m_pItemList[i] != 0))
        {
            m_pClientList[iClientH]->m_pItemList[i - 1] = m_pClientList[iClientH]->m_pItemList[i];
            m_pClientList[iClientH]->m_bIsItemEquipped[i - 1] = m_pClientList[iClientH]->m_bIsItemEquipped[i];
            m_pClientList[iClientH]->m_pItemList[i] = 0;
            m_pClientList[iClientH]->m_bIsItemEquipped[i] = false;
        }

    return true;
}

bool CGame::bBankItemToPlayer(int iClientH, short sItemIndex)
{
    int i, iIndex;

    if (m_pClientList[iClientH] == nullptr) return false;

    if (m_pClientList[iClientH]->m_pItemInBankList[sItemIndex] == 0) return false;

    for (i = 0; i < DEF_MAXITEMS; i++)
        if (m_pClientList[iClientH]->m_pItemList[i] == 0)
        {
            iIndex = i;
            goto NEXT_STEP_PLTB;
        }
    // Â´ÃµÃ€ÃŒÂ»Ã³ Â°Â®Â°Ã­Ã€Ã–Ã€Â» Â°Ã¸Â°Â£Ã€ÃŒ Â¾Ã¸Â´Ã™. 
    return false;

    NEXT_STEP_PLTB:;

    // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â¼Ã’ÃÃ¶Ã‡Ã’ Â°Ã¸Â°Â£Ã€ÃŒ Â³Â²Â¾Ã†Ã€Ã–Â´Ã™. 

    // Â¾Ã†Ã€ÃŒÃ…Ã› Ã…Â¬Â·Â¡Â½ÂºÃ€Ã‡ ÃÃ–Â¼Ã’Â¸Â¦ Â¹Ã™Â²Ã›Â´Ã™. 
    m_pClientList[iClientH]->m_pItemList[iIndex] = m_pClientList[iClientH]->m_pItemInBankList[sItemIndex];

    m_pClientList[iClientH]->m_pItemInBankList[sItemIndex] = 0;

    return true;
}

bool CGame::__bReadMapInfo(int iMapIndex)
{
    char * pContents, * token, * pTile, cTxt[250], cFn[255];
    char seps[] = "= \t\n";
    char cReadModeA = 0;
    char cReadModeB = 0;
    int  iTeleportLocIndex = 0;
    int  iWayPointCfgIndex = 0;
    int  iTotalNpcSetting = 0;
    int  iMGARCfgIndex = 0;
    int  iSMGRCfgIndex = 0;
    int  iNMRCfgIndex = 0;
    int  iFishPointIndex = 0;
    int  iMineralPointIndex = 0;
    int  iStrategicPointIndex = 0;
    int  iIndex = 0;

    int  iNamingValue;
    class CStrTok * pStrTok;
    HANDLE hFile;
    uint32_t  dwFileSize, dwReadSize;
    FILE * pFile;

    char cName[6], cNpcName[21], cNpcMoveType, cNpcWaypointIndex[10], cNamePrefix;
    short sIPindex, dX, dY;

    if (memcmp(m_pMapList[iMapIndex]->m_cName, "fightzone", 9) == 0)
        m_pMapList[iMapIndex]->m_bIsFightZone = true;

    if (memcmp(m_pMapList[iMapIndex]->m_cName, "icebound", 8) == 0)
        m_pMapList[iMapIndex]->m_bIsSnowEnabled = true;

    memset(cFn, 0, sizeof(cFn));
    strcat(cFn, "mapdata\\");
    strcat(cFn, m_pMapList[iMapIndex]->m_cName);
    strcat(cFn, ".txt");

    hFile = CreateFile(cFn, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    if (hFile == INVALID_HANDLE_VALUE) return false;
    dwFileSize = GetFileSize(hFile, 0);
    CloseHandle(hFile);


    pContents = new char[dwFileSize + 1];
    memset(pContents, 0, dwFileSize + 1);

    pFile = fopen(cFn, "rt");
    if (pFile == 0)
    {
        wsprintf(cTxt, "(!) Cannot open file : %s", cFn);
        log->critical(cTxt);
        return false;
    }
    else
    {
        wsprintf(cTxt, "(!) Reading Map info file : %s", cFn);
        log->info(cTxt);
        dwReadSize = fread(pContents, dwFileSize, 1, pFile);
        fclose(pFile);
    }

    pStrTok = new class CStrTok(pContents, seps);
    token = pStrTok->pGet();
    //token = strtok(pContents, seps);
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
                            // Ã…ÃšÂ·Â¹Ã†Ã·Ã†Â® Â¼Ã’Â½Âº ÃÃ‚Ã‡Â¥ X  
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 1 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pMapList[iMapIndex]->m_pTeleportLoc[iTeleportLocIndex]->m_sSrcX = atoi(token);
                            cReadModeB = 2;
                            break;

                        case 2:
                            // Ã…ÃšÂ·Â¹Ã†Ã·Ã†Â® Â¼Ã’Â½Âº ÃÃ‚Ã‡Â¥ Y 
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 2 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pMapList[iMapIndex]->m_pTeleportLoc[iTeleportLocIndex]->m_sSrcY = atoi(token);
                            cReadModeB = 3;
                            break;

                        case 3:
                            // Ã…ÃšÂ·Â¹Ã†Ã·Ã†Â® Â¸Ã±Ã€Ã»ÃÃ¶ Â¸ÃŠ Ã€ÃŒÂ¸Â§ 
                            ZeroMemory(m_pMapList[iMapIndex]->m_pTeleportLoc[iTeleportLocIndex]->m_cDestMapName,
                                sizeof(m_pMapList[iMapIndex]->m_pTeleportLoc[iTeleportLocIndex]->m_cDestMapName));
                            strcpy(m_pMapList[iMapIndex]->m_pTeleportLoc[iTeleportLocIndex]->m_cDestMapName, token);
                            cReadModeB = 4;
                            break;

                        case 4:
                            // Ã…ÃšÂ·Â¹Ã†Ã·Ã†Â® Â¸Ã±Ã€Ã»ÃÃ¶ Ã€Â§Ã„Â¡ X 
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 3 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pMapList[iMapIndex]->m_pTeleportLoc[iTeleportLocIndex]->m_sDestX = atoi(token);
                            cReadModeB = 5;
                            break;

                        case 5:
                            // Ã…ÃšÂ·Â¹Ã†Ã·Ã†Â® Â¸Ã±Ã€Ã»ÃÃ¶ Ã€Â§Ã„Â¡ Y 
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 4 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pMapList[iMapIndex]->m_pTeleportLoc[iTeleportLocIndex]->m_sDestY = atoi(token);
                            cReadModeB = 6;
                            break;

                        case 6:
                            // Ã…ÃšÂ·Â¹Ã†Ã·Ã†Â® ÃˆÃ„ Â¹Ã¦Ã‡Ã¢  
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 5 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pMapList[iMapIndex]->m_pTeleportLoc[iTeleportLocIndex]->m_cDir = atoi(token);
                            iTeleportLocIndex++;
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 2:
                    switch (cReadModeB)
                    {
                        case 1:
                            // waypoint Â¹Ã¸ÃˆÂ£   
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 6 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            iWayPointCfgIndex = atoi(token);

                            if (m_pMapList[iMapIndex]->m_WaypointList[iWayPointCfgIndex].x != -1)
                            {
                                // Ã€ÃŒÂ¹ÃŒ Ã‡Ã’Â´Ã§ÂµÃ‡Â¾Ã®Ã€Ã–Â´Ã‚ Waypoint Â¹Ã¸ÃˆÂ£Ã€ÃŒÂ´Ã™.
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 7 - Duplicated waypoint");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            cReadModeB = 2;
                            break;

                        case 2:
                            // waypoint ÃÃ½Ã‡Ã• ÃÂ¤Ã€Ã‡ X  
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 8 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_WaypointList[iWayPointCfgIndex].x = atoi(token);
                            cReadModeB = 3;
                            break;

                        case 3:
                            // waypoint ÃÃ½Ã‡Ã• ÃÂ¤Ã€Ã‡ Y  
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 9 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_WaypointList[iWayPointCfgIndex].y = atoi(token);
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 3:
                    // NpcÂ¸Â¦ Ã†Â¯ÃÂ¤Ã€Â§Ã„Â¡Â¿Â¡ Ã€Â§Ã„Â¡Â½ÃƒÃ…Â²Â´Ã™.
                    switch (cReadModeB)
                    {
                        case 1:
                            // NPCÃ€Ã‡ Ã€ÃŒÂ¸Â§. 
                            memset(cNpcName, 0, sizeof(cNpcName));
                            strcpy(cNpcName, token);
                            cReadModeB = 2;
                            break;
                        case 2:
                            // NpcMoveType
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 10 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            cNpcMoveType = atoi(token);
                            cReadModeB = 3;
                            break;
                        default:
                            // WayPoint0~waypoint9
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 11 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            cNpcWaypointIndex[cReadModeB - 3] = atoi(token);
                            cReadModeB++;
                            break;
                        case 13:
                            // cNamePrefix
                            cNamePrefix = token[0];
                            // Ã€ÃŒÃÂ¦ Ã€ÃŒÂ°Ã·Â¿Â¡Â¼Â­ NPCÂ¸Â¦ Â»Ã½Â¼ÂºÃ‡Ã‘Â´Ã™. 

                            iNamingValue = m_pMapList[iMapIndex]->iGetEmptyNamingValue();
                            if (iNamingValue == -1)
                            {
                                // Â´ÃµÃ€ÃŒÂ»Ã³ Ã€ÃŒ Â¸ÃŠÂ¿Â¡ NPCÂ¸Â¦ Â¸Â¸ÂµÃ©Â¼Ã¶ Â¾Ã¸Â´Ã™. Ã€ÃŒÂ¸Â§Ã€Â» Ã‡Ã’Â´Ã§Ã‡Ã’ Â¼Ã¶ Â¾Ã¸Â±Ã¢ Â¶Â§Â¹Â®.
                            }
                            else
                            {
                                // NPCÂ¸Â¦ Â»Ã½Â¼ÂºÃ‡Ã‘Â´Ã™.
                                memset(cName, 0, sizeof(cName));
                                wsprintf(cName, "XX%d", iNamingValue);
                                cName[0] = cNamePrefix;
                                cName[1] = iMapIndex + 65;

                                if (bCreateNewNpc(cNpcName, cName, m_pMapList[iMapIndex]->m_cName, 0, 0, cNpcMoveType, 0, 0, cNpcWaypointIndex, 0, 0, -1, false) == false)
                                {
                                    // Â½Ã‡Ã†ÃÃ‡ÃŸÃ€Â¸Â¹Ã‡Â·ÃŽ Â¿Â¹Â¾Ã ÂµÃˆ NameValueÂ¸Â¦ Ã‡Ã˜ÃÂ¦Â½ÃƒÃ…Â²Â´Ã™.
                                    m_pMapList[iMapIndex]->SetNamingValueEmpty(iNamingValue);
                                }
                            }
                            cReadModeA = 0;
                            cReadModeB = 0;
                            iTotalNpcSetting++;
                            break;
                    }
                    break;

                case 4:
                    switch (cReadModeB)
                    {
                        case 1:
                            // Random-Mob-Generator Â»Ã§Â¿Ã« Â¿Â©ÂºÃŽ 
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 12 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_bRandomMobGenerator = (bool)atoi(token);
                            cReadModeB = 2;
                            break;

                        case 2:
                            // Mob- Level
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 13 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_cRandomMobGeneratorLevel = atoi(token);
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 5:
                    // Maximum object
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->critical("(!!!) CRITICAL ERROR! Map Info file error 14 - Wrong Data format.");
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pMapList[iMapIndex]->m_iMaximumObject = atoi(token);
                    cReadModeA = 0;
                    cReadModeB = 0;
                    break;

                case 6:
                    // Ã„Â³Â¸Â¯Ã…Ã Â·Â£Â´Ã½ Â»Ã½Â¼Âº Â±ÃÃÃ¶ Â±Â¸Â¿Âª : Â¸Â¶Ã€Â» ÃÃŸÂ½Ã‰ÂºÃŽ Â°Â°Ã€ÂºÂµÂ¥Â¼Â­ Â¸Ã·Ã€ÃŒ Â¹ÃŸÂ»Ã½ÂµÃ‡Â¸Ã© Â°Ã¯Â¶ÃµÃ‡ÃÂ¹Ã‡Â·ÃŽ 
                    switch (cReadModeB)
                    {
                        case 1:
                            // Rect Â¹Ã¸ÃˆÂ£ 
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 15 - Wrong Data format(MGAR num).");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            iMGARCfgIndex = atoi(token);

                            if (m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[iMGARCfgIndex].left != -1)
                            {
                                // Ã€ÃŒÂ¹ÃŒ Ã‡Ã’Â´Ã§ÂµÃ‡Â¾Ã®Ã€Ã–Â´Ã‚ Waypoint Â¹Ã¸ÃˆÂ£Ã€ÃŒÂ´Ã™.
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 16 - Duplicated Mob Gen Rect Number!");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            cReadModeB = 2;
                            break;

                        case 2:
                            // left
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 17 - Wrong Data format(MGAR num).");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[iMGARCfgIndex].left = atoi(token);
                            cReadModeB = 3;
                            break;

                        case 3:
                            // top
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 18 - Wrong Data format(MGAR num).");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[iMGARCfgIndex].top = atoi(token);
                            cReadModeB = 4;
                            break;

                        case 4:
                            // right
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 19 - Wrong Data format(MGAR num).");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[iMGARCfgIndex].right = atoi(token);
                            cReadModeB = 5;
                            break;

                        case 5:
                            // bottom
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 20 - Wrong Data format(MGAR num).");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[iMGARCfgIndex].bottom = atoi(token);
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 7:
                    // Ã†Â¯ÃÂ¤ÃÃ¶Â¿Âª Â¸Ã· Â»Ã½Â¼Âº 
                    switch (cReadModeB)
                    {
                        case 1:
                            // Rect Â¹Ã¸ÃˆÂ£ m_stSpotMobGenerator[]
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 21 - Wrong Data format(MGAR num).");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            iSMGRCfgIndex = atoi(token);

                            if (m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].bDefined == true)
                            {
                                // Ã€ÃŒÂ¹ÃŒ Ã‡Ã’Â´Ã§ÂµÃ‡Â¾Ã®Ã€Ã–Â´Ã‚ Â¸Ã· ÃÂ¦Â³ÃŠÂ·Â¹Ã€ÃŒÃ…Ã Â¹Ã¸ÃˆÂ£Ã€ÃŒÂ´Ã™.
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error - ");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            cReadModeB = 2;
                            m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].bDefined = true;
                            break;

                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 22 - Wrong Data format(SMGAR num).");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].cType = atoi(token);

                            if (m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].cType == 1)
                                cReadModeB = 3;
                            else if (m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].cType == 2)
                                cReadModeB = 9;  // RECTÂ°Â¡ Â¾Ã†Â´ÃÂ¶Ã³ WaypointÃÃ½Ã‡Ã•Ã€Â» Ã€ÃÂ¾Ã®Â¾ÃŸ Ã‡Ã‘Â´Ã™.
                            break;

                        case 3:
                            // left
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 23 - Wrong Data format(SMGAR num).");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].rcRect.left = atoi(token);
                            cReadModeB = 4;
                            break;

                        case 4:
                            // top
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 24 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].rcRect.top = atoi(token);
                            cReadModeB = 5;
                            break;

                        case 5:
                            // right
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 25 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].rcRect.right = atoi(token);
                            cReadModeB = 6;
                            break;

                        case 6:
                            // bottom
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 26 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].rcRect.bottom = atoi(token);
                            cReadModeB = 7;
                            break;

                        case 7:
                            // spot mob type
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 27 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].iMobType = atoi(token);
                            cReadModeB = 8;
                            break;

                        case 8:
                            // Max Mobs
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 28 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].iMaxMobs = atoi(token);
                            m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].iCurMobs = 0;
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;

                        default:
                            // WayPoint0~waypoint9
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 29 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].cWaypoint[cReadModeB - 9] = atoi(token);
                            cReadModeB++;
                            break;

                        case 19:
                            // spot mob type
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 30 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].iMobType = atoi(token);
                            cReadModeB = 20;
                            break;

                        case 20:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 31 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].iMaxMobs = atoi(token);
                            m_pMapList[iMapIndex]->m_stSpotMobGenerator[iSMGRCfgIndex].iCurMobs = 0;
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 8:
                    // Â¸ÃŠÃ€ÃŒ Â¼Ã“Ã‡Ã‘ Ã€Ã¥Â¼Ã’ Ã€ÃŒÂ¸Â§ 
                    memset(m_pMapList[iMapIndex]->m_cLocationName, 0, sizeof(m_pMapList[iMapIndex]->m_cLocationName));
                    memcpy(m_pMapList[iMapIndex]->m_cLocationName, token, 10);
                    cReadModeA = 0;
                    cReadModeB = 0;
                    break;

                case 9:
                    switch (cReadModeB)
                    {
                        case 1:
                            // Initial-Point Index
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 32:1 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            sIPindex = atoi(token);
                            if (m_pMapList[iMapIndex]->m_pInitialPoint[sIPindex].x != -1)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Duplicate Initial Point Index!");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            cReadModeB = 2;
                            break;

                        case 2:
                            // Initial-Point X
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 32 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_pInitialPoint[sIPindex].x = atoi(token);
                            cReadModeB = 3;
                            break;

                        case 3:
                            // Initial-Point Y
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 33 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_pInitialPoint[sIPindex].y = atoi(token);
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 10:
                    // Â°Ã¸Â°Ã Â¹Â«ÃˆÂ¿ÃˆÂ­ Â¿ÂµÂ¿Âª RECT
                    switch (cReadModeB)
                    {
                        case 1:
                            // 
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 34 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            iNMRCfgIndex = atoi(token);

                            if (m_pMapList[iMapIndex]->m_rcNoAttackRect[iNMRCfgIndex].top != -1)
                            {
                                // Ã€ÃŒÂ¹ÃŒ Ã‡Ã’Â´Ã§ÂµÃ‡Â¾Ã®Ã€Ã–Â´Ã‚ No-Magic-Rect Â¹Ã¸ÃˆÂ£Ã€ÃŒÂ´Ã™.
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 35 - Duplicate No-Magic-Rect number");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            cReadModeB = 2;
                            break;

                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 36 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_rcNoAttackRect[iNMRCfgIndex].left = atoi(token);
                            cReadModeB = 3;
                            break;

                        case 3:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 37 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_rcNoAttackRect[iNMRCfgIndex].top = atoi(token);
                            cReadModeB = 4;
                            break;

                        case 4:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 38 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_rcNoAttackRect[iNMRCfgIndex].right = atoi(token);
                            cReadModeB = 5;
                            break;

                        case 5:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 39 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_rcNoAttackRect[iNMRCfgIndex].bottom = atoi(token);
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 11:
                    m_pMapList[iMapIndex]->m_bIsFixedDayMode = (bool)atoi(token);
                    if (m_pMapList[iMapIndex]->m_bIsFixedDayMode == true)
                        m_pMapList[iMapIndex]->m_bIsSnowEnabled = false;
                    cReadModeA = 0;
                    break;

                case 12:
                    switch (cReadModeB)
                    {
                        case 1:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 40 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            iFishPointIndex = atoi(token);

                            if (m_pMapList[iMapIndex]->m_FishPointList[iFishPointIndex].x != -1)
                            {
                                // Ã€ÃŒÂ¹ÃŒ Ã‡Ã’Â´Ã§ÂµÃ‡Â¾Ã®Ã€Ã–Â´Ã‚ Fish Point Â¹Ã¸ÃˆÂ£Ã€ÃŒÂ´Ã™.
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 41 - Duplicate FishPoint number");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pMapList[iMapIndex]->m_iTotalFishPoint++;
                            cReadModeB = 2;
                            break;

                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 42 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_FishPointList[iFishPointIndex].x = atoi(token);

                            cReadModeB = 3;
                            break;

                        case 3:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 43 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_FishPointList[iFishPointIndex].y = atoi(token);

                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 13:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->critical("(!!!) CRITICAL ERROR! Map Info file error 44 - Wrong Data format.");
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pMapList[iMapIndex]->m_iMaxFish = atoi(token);

                    cReadModeA = 0;
                    break;

                case 14:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->critical("(!!!) CRITICAL ERROR! Map Info file error 45 - Wrong Data format.");
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pMapList[iMapIndex]->m_cType = atoi(token);

                    cReadModeA = 0;
                    break;

                case 15:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->critical("(!!!) CRITICAL ERROR! Map Info file error 46 - Wrong Data format.");
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pMapList[iMapIndex]->m_iLevelLimit = atoi(token);

                    cReadModeA = 0;
                    break;

                case 16:
                    // Â±Â¤Â¹Â° ÃÂ¦Â³Ã—Â·Â¹Ã€ÃŒÃ…ÃÃ€Ã‡ ÃÂ¸Ã€Ã§Ã€Â¯Â¹Â«Â¿Ã ÂµÃ®Â±Ãž 
                    switch (cReadModeB)
                    {
                        case 1:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 47 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_bMineralGenerator = (bool)atoi(token);
                            cReadModeB = 2;
                            break;

                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 48 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_cMineralGeneratorLevel = atoi(token);
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 17:
                    switch (cReadModeB)
                    {
                        case 1:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 49 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            iMineralPointIndex = atoi(token);

                            if (m_pMapList[iMapIndex]->m_MineralPointList[iMineralPointIndex].x != -1)
                            {
                                // Ã€ÃŒÂ¹ÃŒ Ã‡Ã’Â´Ã§ÂµÃ‡Â¾Ã®Ã€Ã–Â´Ã‚ Mineral Point Â¹Ã¸ÃˆÂ£Ã€ÃŒÂ´Ã™.
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 50 - Duplicate MineralPoint number");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pMapList[iMapIndex]->m_iTotalMineralPoint++;
                            cReadModeB = 2;
                            break;

                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 51 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_MineralPointList[iMineralPointIndex].x = atoi(token);

                            cReadModeB = 3;
                            break;

                        case 3:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 52 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_MineralPointList[iMineralPointIndex].y = atoi(token);

                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 18:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->critical("(!!!) CRITICAL ERROR! Map Info file error 53 - Wrong Data format.");
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pMapList[iMapIndex]->m_iMaxMineral = atoi(token);

                    cReadModeA = 0;
                    break;

                case 19:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->critical("(!!!) CRITICAL ERROR! Map Info file error 54 - Wrong Data format.");
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pMapList[iMapIndex]->m_iUpperLevelLimit = atoi(token);

                    cReadModeA = 0;
                    break;

                case 20:
                    switch (cReadModeB)
                    {
                        case 1:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 55 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            iStrategicPointIndex = atoi(token);

                            if (m_pMapList[iMapIndex]->m_pStrategicPointList[iStrategicPointIndex] != 0)
                            {
                                // Ã€ÃŒÂ¹ÃŒ Ã‡Ã’Â´Ã§ÂµÃ‡Â¾Ã®Ã€Ã–Â´Ã‚ Strategic Point Â¹Ã¸ÃˆÂ£Ã€ÃŒÂ´Ã™.
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 56 - Duplicate Strategic Point number");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pMapList[iMapIndex]->m_pStrategicPointList[iStrategicPointIndex] = new class CStrategicPoint;
                            cReadModeB = 2;
                            break;

                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 57 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_pStrategicPointList[iStrategicPointIndex]->m_iSide = atoi(token);

                            cReadModeB = 3;
                            break;

                        case 3:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 58 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_pStrategicPointList[iStrategicPointIndex]->m_iValue = atoi(token);

                            cReadModeB = 4;
                            break;

                        case 4:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 59 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_pStrategicPointList[iStrategicPointIndex]->m_iX = atoi(token);

                            cReadModeB = 5;
                            break;

                        case 5:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 60 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_pStrategicPointList[iStrategicPointIndex]->m_iY = atoi(token);

                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 21:
                    switch (cReadModeB)
                    {
                        case 1:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 61 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            iIndex = atoi(token);

                            if (m_pMapList[iMapIndex]->m_stEnergySphereCreationList[iIndex].cType != 0)
                            {
                                // Ã€ÃŒÂ¹ÃŒ Ã‡Ã’Â´Ã§ÂµÃ‡Â¾Ã®Ã€Ã–Â´Ã‚ Energy-Sphere-Creation Point Â¹Ã¸ÃˆÂ£Ã€ÃŒÂ´Ã™.
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 62 - Duplicate EnergySphereCreation number");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pMapList[iMapIndex]->m_iTotalEnergySphereCreationPoint++;
                            cReadModeB = 2;
                            break;

                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 63 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stEnergySphereCreationList[iIndex].cType = atoi(token);

                            cReadModeB = 3;
                            break;

                        case 3:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 64 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stEnergySphereCreationList[iIndex].sX = atoi(token);

                            cReadModeB = 4;
                            break;

                        case 4:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 65 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stEnergySphereCreationList[iIndex].sY = atoi(token);

                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 22:
                    switch (cReadModeB)
                    {
                        case 1:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 66 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            iIndex = atoi(token);

                            if (m_pMapList[iMapIndex]->m_stEnergySphereGoalList[iIndex].cResult != 0)
                            {
                                // Ã€ÃŒÂ¹ÃŒ Ã‡Ã’Â´Ã§ÂµÃ‡Â¾Ã®Ã€Ã–Â´Ã‚ Energy-Sphere-Goal Point Â¹Ã¸ÃˆÂ£Ã€ÃŒÂ´Ã™.
                                wsprintf(G_cTxt, "(!!!) CRITICAL ERROR! Map Info file error 67 - Duplicate EnergySphereGoal number(%d:%d)", iIndex, m_pMapList[iMapIndex]->m_stEnergySphereGoalList[iIndex].cResult);
                                log->critical(G_cTxt);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pMapList[iMapIndex]->m_iTotalEnergySphereGoalPoint++;
                            cReadModeB = 2;
                            break;

                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 68 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stEnergySphereGoalList[iIndex].cResult = atoi(token);

                            cReadModeB = 3;
                            break;

                        case 3:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 69 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stEnergySphereGoalList[iIndex].aresdenX = atoi(token);

                            cReadModeB = 4;
                            break;

                        case 4:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 70 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stEnergySphereGoalList[iIndex].aresdenY = atoi(token);

                            cReadModeB = 5;
                            break;

                        case 5:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 71 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stEnergySphereGoalList[iIndex].elvineX = atoi(token);

                            cReadModeB = 6;
                            break;

                        case 6:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 72 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stEnergySphereGoalList[iIndex].elvineY = atoi(token);

                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 23:
                    switch (cReadModeB)
                    {
                        case 1:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 73 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            iIndex = atoi(token);

                            if (strlen(m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].cRelatedMapName) != 0)
                            {
                                // Ã€ÃŒÂ¹ÃŒ Ã‡Ã’Â´Ã§ÂµÃ‡Â¾Ã®Ã€Ã–Â´Ã‚  Point Â¹Ã¸ÃˆÂ£Ã€ÃŒÂ´Ã™.
                                wsprintf(G_cTxt, "(!!!) CRITICAL ERROR! Map Info file error 74 - Duplicate Strike Point number(%d)", iIndex);
                                log->critical(G_cTxt);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            cReadModeB = 2;
                            break;

                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 75 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].dX = atoi(token);

                            cReadModeB = 3;
                            break;

                        case 3:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 76 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].dY = atoi(token);

                            cReadModeB = 4;
                            break;

                        case 4:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].iHP = atoi(token);
                            m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].iInitHP = atoi(token);
                            cReadModeB = 5;
                            break;


                        case 5:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].iEffectX[0] = atoi(token);

                            cReadModeB = 6;
                            break;

                        case 6:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].iEffectY[0] = atoi(token);

                            cReadModeB = 7;
                            break;

                        case 7:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].iEffectX[1] = atoi(token);

                            cReadModeB = 8;
                            break;

                        case 8:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].iEffectY[1] = atoi(token);

                            cReadModeB = 9;
                            break;

                        case 9:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].iEffectX[2] = atoi(token);

                            cReadModeB = 10;
                            break;

                        case 10:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].iEffectY[2] = atoi(token);

                            cReadModeB = 11;
                            break;

                        case 11:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].iEffectX[3] = atoi(token);

                            cReadModeB = 12;
                            break;

                        case 12:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].iEffectY[3] = atoi(token);

                            cReadModeB = 13;
                            break;

                        case 13:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].iEffectX[4] = atoi(token);

                            cReadModeB = 14;
                            break;

                        case 14:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 77 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].iEffectY[4] = atoi(token);

                            cReadModeB = 15;
                            break;

                        case 15:
                            memset(m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].cRelatedMapName, 0, sizeof(m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].cRelatedMapName));
                            strcpy(m_pMapList[iMapIndex]->m_stStrikePoint[iIndex].cRelatedMapName, token);

                            m_pMapList[iMapIndex]->m_iTotalStrikePoints++;
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;

                    }
                    break; // end 23

                case 24:
                    switch (cReadModeB)
                    {
                        case 1:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 78 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            iIndex = atoi(token);

                            if (strlen(m_pMapList[iMapIndex]->m_stItemEventList[iIndex].cItemName) != 0)
                            {
                                // ÀÌ¹Ì ÇÒ´çµÇ¾îÀÖ´Â Item-Event ¹øÈ£ÀÌ´Ù.
                                wsprintf(G_cTxt, "(!!!) CRITICAL ERROR! Map Info file error 79 - Duplicate Item-Event number(%d:%s)", iIndex, m_pMapList[iMapIndex]->m_stItemEventList[iIndex].cItemName);
                                log->critical(G_cTxt);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pMapList[iMapIndex]->m_iTotalItemEvents++;
                            cReadModeB = 2;
                            break;

                        case 2:
                            strcpy(m_pMapList[iMapIndex]->m_stItemEventList[iIndex].cItemName, token);
                            cReadModeB = 3;
                            break;

                        case 3:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 81 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stItemEventList[iIndex].iAmount = atoi(token);

                            cReadModeB = 4;
                            break;

                        case 4:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 82 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stItemEventList[iIndex].iTotalNum = atoi(token);

                            cReadModeB = 5;
                            break;

                        case 5:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 83 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stItemEventList[iIndex].iMonth = atoi(token);

                            cReadModeB = 6;
                            break;

                        case 6:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 83 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stItemEventList[iIndex].iDay = atoi(token);

                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 25: //mobevent-amount
                    switch (cReadModeB)
                    {
                        case 1:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error 78 - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->sMobEventAmount = atoi(token);
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 26: //ApocalypseMobGenType
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseMobGenType - Wrong Data format.");
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pMapList[iMapIndex]->m_iApocalypseMobGenType = atoi(token);
                    cReadModeA = 0;
                    cReadModeB = 0;
                    break;

                case 27: // ApocalypseBossMob
                    switch (cReadModeB)
                    {
                        case 1: // 3CB6Ch m_pMapList[]->m_ApocalypseBossMobNpcID
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_iApocalypseBossMobNpcID = atoi(token);
                            cReadModeB = 2;
                            break;
                        case 2: // 3CB70h m_pMapList[]->ApocalypseBossMobRectX1
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_sApocalypseBossMobRectX1 = atoi(token);
                            cReadModeB = 3;
                            break;
                        case 3: // 3CB74h m_pMapList[]->ApocalypseBossMobRectY1
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_sApocalypseBossMobRectY1 = atoi(token);
                            cReadModeB = 4;
                            break;
                        case 4: // 3CB78h m_pMapList[]->ApocalypseBossMobRectX2
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_sApocalypseBossMobRectX2 = atoi(token);
                            cReadModeB = 5;
                            break;
                        case 5: // 3CB7Ch m_pMapList[]->ApocalypseBossMobRectY2
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_sApocalypseBossMobRectY2 = atoi(token);
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 28: //DynamicGateType // 28
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->critical("(!!!) CRITICAL ERROR! Map Info file error DynamicGateType - Wrong Data format.");
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pMapList[iMapIndex]->m_cDynamicGateType = atoi(token);
                    cReadModeA = 0;
                    cReadModeB = 0;
                    break;

                case 29: //DynamicGateCoord // 29
                    // DynamicGateCoord	= 59 196 60 197        abaddon	   -1  -1
                    switch (cReadModeB)
                    {
                        case 1: // 3CA20h
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_sDynamicGateCoordRectX1 = atoi(token);
                            cReadModeB = 2;
                            break;

                        case 2: // 3CA24h
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_sDynamicGateCoordRectY1 = atoi(token);
                            cReadModeB = 3;
                            break;

                        case 3: // 3CA28h
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_sDynamicGateCoordRectX2 = atoi(token);
                            cReadModeB = 4;
                            break;

                        case 4: // 3CA2Ch
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_sDynamicGateCoordRectY2 = atoi(token);
                            cReadModeB = 5;
                            break;

                        case 5: // 3CA30h
                            memcpy(m_pMapList[iMapIndex]->m_cDynamicGateCoordDestMap, token, strlen(token));
                            cReadModeB = 6;
                            break;

                        case 6: // 3CA3Ch
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_sDynamicGateCoordTgtX = atoi(token);
                            cReadModeB = 7;
                            break;

                        case 7: // (ty = 3CB60h) unknown (3CA3Eh)
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseBossMob - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_sDynamicGateCoordTgtY = atoi(token);
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 30: // RecallImpossible // 30
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->critical("(!!!) CRITICAL ERROR! Map Info file error RecallImpossible -  Wrong Data format.");
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pMapList[iMapIndex]->m_bIsRecallImpossible = atoi(token);
                    cReadModeA = 0;
                    cReadModeB = 0;
                    break;

                case 31: // ApocalypseMap // 31
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->critical("(!!!) CRITICAL ERROR! Map Info file error ApocalypseMap -  Wrong Data format.");
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pMapList[iMapIndex]->m_bIsApocalypseMap = atoi(token);
                    cReadModeA = 0;
                    cReadModeB = 0;
                    break;

                case 32: // CitizenLimit // 32
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->critical("(!!!) CRITICAL ERROR! Map Info file error CitizenLimit -  Wrong Data format.");
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pMapList[iMapIndex]->m_bIsCitizenLimit = atoi(token);
                    cReadModeA = 0;
                    cReadModeB = 0;
                    break;

                case 33: // HeldenianMap
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->critical("(!!!) CRITICAL ERROR! Map Info file error CitizenLimit -  Wrong Data format.");
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pMapList[iMapIndex]->m_bIsHeldenianMap = atoi(token);
                    cReadModeA = 0;
                    cReadModeB = 0;
                    break;

                case 34: // HeldenianTower
                    switch (cReadModeB)
                    {
                        case 1: // NpcID
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error Hedenian tower type id - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stHeldenianTower[iIndex].sTypeID = atoi(token);
                            cReadModeB = 2;
                            break;
                        case 2: // side 
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error Hedenian Tower Side - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stHeldenianTower[iIndex].cSide = atoi(token);
                            cReadModeB = 3;
                            break;
                        case 3: // sX
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error Hedenian Tower X pos - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stHeldenianTower[iIndex].dX = atoi(token);
                            cReadModeB = 4;
                            break;
                        case 4: // sY
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error Hedenian Tower Y pos - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            iIndex++;
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 35: // HeldenianModeMap
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->critical("(!!!) CRITICAL ERROR! Map Info file error Hedenian Map Mode - Wrong Data format.");
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pMapList[iMapIndex]->m_cHeldenianModeMap = atoi(token);
                    cReadModeA = 0;
                    cReadModeB = 0;
                    break;

                case 36: // HeldenianWinningZone
                    switch (cReadModeB)
                    {
                        case 1:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error Hedenian Map Mode - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            dX = atoi(token);
                            cReadModeB = 2;
                            break;
                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error Hedenian Map Mode - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            dY = atoi(token);
                            pTile = 0;
                            pTile = (char *)(m_pMapList[iMapIndex]->m_pTile + dX + dY * m_pMapList[iMapIndex]->m_sSizeY);
                            if (pTile == 0)
                            {
                                wsprintf(cTxt, "(!!!) CRITICAL ERROR! Map Info file error HeldenianWinningZone - pTile is Null dx(%d), dy(%d).", dX, dY);
                                log->critical(cTxt);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            //pTile->m_iAttribute = 1;
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 37: // HeldenianGateDoor // 37
                    switch (cReadModeB)
                    {
                        case 1:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error Heldenian Door Direction - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stHeldenianGateDoor[iIndex].cDir = atoi(token);
                            cReadModeB = 2;
                            break;
                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error Heldenian Door X pos - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stHeldenianGateDoor[iIndex].dX = atoi(token);
                            cReadModeB = 3;
                            break;
                        case 3:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Map Info file error Heldenian Door Y pos - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMapList[iMapIndex]->m_stHeldenianGateDoor[iIndex].dY = atoi(token);
                            iIndex++;
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
            if (memcmp(token, "teleport-loc", 12) == 0)
            {
                m_pMapList[iMapIndex]->m_pTeleportLoc[iTeleportLocIndex] = new class CTeleportLoc;
                cReadModeA = 1;
                cReadModeB = 1;
            }

            if (memcmp(token, "waypoint", 8) == 0)
            {
                cReadModeA = 2;
                cReadModeB = 1;
            }

            if (memcmp(token, "npc", 3) == 0)
            {
                cReadModeA = 3;
                cReadModeB = 1;
            }

            if (memcmp(token, "random-mob-generator", 20) == 0)
            {
                cReadModeA = 4;
                cReadModeB = 1;
            }

            if (memcmp(token, "maximum-object", 14) == 0)
                cReadModeA = 5;

            if (memcmp(token, "npc-avoidrect", 13) == 0)
            {
                cReadModeA = 6;
                cReadModeB = 1;
            }

            if (memcmp(token, "spot-mob-generator", 18) == 0)
            {
                cReadModeA = 7;
                cReadModeB = 1;
            }

            if (memcmp(token, "map-location", 12) == 0)
                cReadModeA = 8;

            if (memcmp(token, "initial-point", 13) == 0)
            {
                cReadModeA = 9;
                cReadModeB = 1;
            }

            if (memcmp(token, "no-attack-area", 14) == 0)
            {
                cReadModeA = 10;
                cReadModeB = 1;
            }

            if (memcmp(token, "fixed-dayornight-mode", 21) == 0) cReadModeA = 11;

            if (memcmp(token, "fish-point", 10) == 0)
            {
                cReadModeA = 12;
                cReadModeB = 1;
            }

            if (memcmp(token, "max-fish", 8) == 0)		cReadModeA = 13;
            if (memcmp(token, "type", 4) == 0)			cReadModeA = 14;
            if (memcmp(token, "level-limit", 11) == 0)	cReadModeA = 15;

            if (memcmp(token, "mineral-generator", 17) == 0)
            {
                cReadModeA = 16;
                cReadModeB = 1;
            }

            if (memcmp(token, "mineral-point", 13) == 0)
            {
                cReadModeA = 17;
                cReadModeB = 1;
            }

            if (memcmp(token, "max-mineral", 11) == 0)			cReadModeA = 18;
            if (memcmp(token, "upper-level-limit", 17) == 0)	cReadModeA = 19;	// v1.4
            if (memcmp(token, "strategic-point", 15) == 0)
            {	// v1.41
                cReadModeA = 20;
                cReadModeB = 1;
            }

            if (memcmp(token, "energy-sphere-creation-point", 28) == 0)
            {
                cReadModeA = 21;
                cReadModeB = 1;
            }

            if (memcmp(token, "energy-sphere-goal-point", 24) == 0)
            {
                cReadModeA = 22;
                cReadModeB = 1;
            }

            if (memcmp(token, "strike-point", 12) == 0)
            {
                cReadModeA = 23;
                cReadModeB = 1;
            }

            if (memcmp(token, "item-event", 10) == 0)
            {
                cReadModeA = 24;
                cReadModeB = 1;
            }

            if (memcmp(token, "energy-sphere-auto-creation", 27) == 0)
            {
                cReadModeA = 0;
                cReadModeB = 0;
                m_pMapList[iMapIndex]->m_bIsEnergySphereAutoCreation = true;
            }

            if (memcmp(token, "mobevent-amount", 15) == 0)
            {
                cReadModeA = 25;
                cReadModeB = 1;
            }

            if (memcmp(token, "ApocalypseMobGenType", 20) == 0)
            {
                cReadModeA = 26;
                cReadModeB = 1;
            }

            if (memcmp(token, "ApocalypseBossMob", 17) == 0)
            {
                cReadModeA = 27;
                cReadModeB = 1;
            }

            if (memcmp(token, "DynamicGateType", 15) == 0)
            {
                cReadModeA = 28;
                cReadModeB = 1;
            }

            if (memcmp(token, "DynamicGateCoord", 16) == 0)
            {
                cReadModeA = 29;
                cReadModeB = 1;
            }

            if (memcmp(token, "RecallImpossible", 16) == 0)
            {
                cReadModeA = 30;
                cReadModeB = 1;
            }

            if (memcmp(token, "ApocalypseMap", 13) == 0)
            {
                cReadModeA = 31;
                cReadModeB = 1;
            }

            if (memcmp(token, "CitizenLimit", 12) == 0)
            {
                cReadModeA = 32;
                cReadModeB = 1;
            }

            if (memcmp(token, "HeldenianMap", 12) == 0)
            {
                cReadModeA = 33;
                cReadModeB = 1;
            }

            if (memcmp(token, "HeldenianTower", 14) == 0)
            {
                cReadModeA = 34;
                cReadModeB = 1;
            }

            if (memcmp(token, "HeldenianModeMap", 16) == 0)
            {
                cReadModeA = 35;
                cReadModeB = 1;
            }

            if (memcmp(token, "HeldenianWinningZone", 20) == 0)
            {
                cReadModeA = 36;
                cReadModeB = 1;
            }

            if (memcmp(token, "HeldenianGateDoor", 17) == 0)
            {
                cReadModeA = 37;
                cReadModeB = 1;
            }

            if (memcmp(token, "[END-MAP-INFO]", 14) == 0)
            {
                cReadModeA = 0;
                cReadModeB = 0;
                goto RMI_SKIPDECODING;
            }
        }
        token = pStrTok->pGet();
    }

    RMI_SKIPDECODING:;

    delete pStrTok;
    delete pContents;

    if ((cReadModeA != 0) || (cReadModeB != 0))
    {
        log->info("(!!!) CRITICAL ERROR! map info file contents error!");
        return false;
    }

    wsprintf(cTxt, "(!) Map info file decoding(%s) - success! TL(%d) WP(%d) LNPC(%d) MXO(%d) RMG(%d / %d)", cFn, iTeleportLocIndex, iWayPointCfgIndex, iTotalNpcSetting, m_pMapList[iMapIndex]->m_iMaximumObject, m_pMapList[iMapIndex]->m_bRandomMobGenerator, m_pMapList[iMapIndex]->m_cRandomMobGeneratorLevel);
    log->info(cTxt);

    // Crusade Â°Ã¸Â°Ã ÂºÃ’Â°Â¡Â´Ã‰ Â¿ÂµÂ¿ÂªÃ€Â» Ã…Â¸Ã€ÃÂ¿Â¡ Ã‡Â¥Â½ÃƒÃ‡Ã‘Â´Ã™.
    m_pMapList[iMapIndex]->_SetupNoAttackArea();

    return true;

}

void CGame::Quit()
{
    int i;

    for (i = 0; i < DEF_MAXCLIENTS; i++)
        if (m_pClientList[i] != nullptr) delete m_pClientList[i];

    for (i = 0; i < DEF_MAXNPCS; i++)
        if (m_pNpcList[i] != nullptr) delete m_pNpcList[i];

    for (i = 0; i < DEF_MAXMAPS; i++)
        if (m_pMapList[i] != nullptr) delete m_pMapList[i];

    for (i = 0; i < DEF_MAXITEMTYPES; i++)
        if (m_pItemConfigList[i] != nullptr) delete m_pItemConfigList[i];

    for (i = 0; i < DEF_MAXNPCTYPES; i++)
        if (m_pNpcConfigList[i] != nullptr) delete m_pNpcConfigList[i];

    for (i = 0; i < DEF_MAXMAGICTYPE; i++)
        if (m_pMagicConfigList[i] != nullptr) delete m_pMagicConfigList[i];

    for (i = 0; i < DEF_MAXSKILLTYPE; i++)
        if (m_pSkillConfigList[i] != nullptr) delete m_pSkillConfigList[i];

    for (i = 0; i < DEF_MAXQUESTTYPE; i++)
        if (m_pQuestConfigList[i] != nullptr) delete m_pQuestConfigList[i];

    for (i = 0; i < DEF_MAXDYNAMICOBJECTS; i++)
        if (m_pDynamicObjectList[i] != nullptr) delete m_pDynamicObjectList[i];

    for (i = 0; i < DEF_MAXDELAYEVENTS; i++)
        if (m_pDelayEventList[i] != nullptr) delete m_pDelayEventList[i];

    for (i = 0; i < DEF_MAXNOTIFYMSGS; i++)
        if (m_pNoticeMsgList[i] != nullptr) delete m_pNoticeMsgList[i];

    for (i = 0; i < DEF_MAXFISHS; i++)
        if (m_pFish[i] != nullptr) delete m_pFish[i];

    for (i = 0; i < DEF_MAXMINERALS; i++)
        if (m_pMineral[i] != nullptr) delete m_pMineral[i];

    for (i = 0; i < DEF_MAXPORTIONTYPES; i++)
        if (m_pPortionConfigList[i] != nullptr) delete m_pPortionConfigList[i];

    //	for (i = 0; i < DEF_MAXTELEPORTTYPE; i++)
    //	if (m_pTeleportConfigList[i] != nullptr) delete m_pTeleportConfigList[i];

    for (i = 0; i < DEF_MAXBUILDITEMS; i++)
        if (m_pBuildItemList[i] != nullptr) delete m_pBuildItemList[i];

    for (i = 0; i < DEF_MAXDUPITEMID; i++)
        if (m_pDupItemIDList[i] != nullptr) delete m_pDupItemIDList[i];

    if (m_pNoticementData != nullptr) delete m_pNoticementData;

}

int CGame::iGetLevelExp(int iLevel)
{
    int iRet;

    if (iLevel == 0) return 0;

    iRet = iGetLevelExp(iLevel - 1) + iLevel * (50 + (iLevel * (iLevel / 17) * (iLevel / 17)));

    return iRet;
}

int CGame::_iCalcSkillSSNpoint(int iLevel)
{
    int iRet;

    if (iLevel < 1) return 1;

    if (iLevel <= 50)
        iRet = iLevel;
    else if (iLevel > 50)
    {
        iRet = (iLevel * 2);
    }

    return iRet;
}

bool CGame::bCheckLevelUp(int iClientH)
{
    bool bStr, bVit, bDex, bInt, bMag, bChr;
    char cLoopCnt;
    int bobdole;

    if (m_pClientList[iClientH] == nullptr) return false;
    if (m_pClientList[iClientH]->m_iLevel >= 180)
    {
        if (m_pClientList[iClientH]->m_iExp >= m_pClientList[iClientH]->m_iNextLevelExp)
        {
            AddGizon(iClientH);
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIZONITEMUPGRADELEFT, m_pClientList[iClientH]->m_iGizonItemUpgradeLeft, 1, 0, 0);
        }
        return false;
    }

    if (m_pClientList[iClientH]->m_iExp < m_pClientList[iClientH]->m_iNextLevelExp) return false;

    bStr = bVit = bDex = bInt = bMag = bChr = false;
    cLoopCnt = 0;
    while (1)
    {
        if (cLoopCnt++ > 100) return false;
        if (m_pClientList[iClientH]->m_iExp >= m_pClientList[iClientH]->m_iNextLevelExp)
        {
            // Â·Â¹ÂºÂ§Ã€ÃŒ Â¿ÃƒÂ¶ÃºÂ´Ã™.
            m_pClientList[iClientH]->m_iLevel++;
            m_pClientList[iClientH]->m_iLU_Pool += 3;
            // Â·Â¹ÂºÂ§Ã€ÃŒ Â¿Ã€Â¸Â¥Â°ÃÂ¿Â¡ Â´Ã«Ã‡Ã‘ Ã†Â¯Â¼ÂºÃ„Â¡ Ã†Ã·Ã€ÃŽÃ†Â®Â¸Â¦ ÃÃµÂ°Â¡Â½ÃƒÃ„Ã‘Â¾ÃŸ Ã‡Ã‘Â´Ã™. 
//			if ( (m_pClientList[iClientH]->m_cLU_Str + m_pClientList[iClientH]->m_cLU_Vit + m_pClientList[iClientH]->m_cLU_Dex + 
//	  		      m_pClientList[iClientH]->m_cLU_Int + m_pClientList[iClientH]->m_cLU_Mag + m_pClientList[iClientH]->m_cLU_Char) <= DEF_TOTALLEVELUPPOINT) {
                // Â·Â¹ÂºÂ§ Â¾Ã· Â¼Â¼Ã†ÃƒÃ€ÃŒ 3ÂºÂ¸Â´Ã™ Â°Â°Â°Ã…Â³Âª Ã€Ã›Â¾Ã†Â¾ÃŸ Ã€Â¯ÃˆÂ¿Ã‡ÃÂ´Ã™. 

//			}

            // Â¼Â³ÃÂ¤Ã„Â¡Â¸Â¦ ÃˆÂ®Ã€ÃŽÃ‡Ã‘Â´Ã™. 
            if (m_pClientList[iClientH]->m_iStr > DEF_CHARPOINTLIMIT)      m_pClientList[iClientH]->m_iStr = DEF_CHARPOINTLIMIT;
            if (m_pClientList[iClientH]->m_iDex > DEF_CHARPOINTLIMIT)      m_pClientList[iClientH]->m_iDex = DEF_CHARPOINTLIMIT;
            if (m_pClientList[iClientH]->m_iVit > DEF_CHARPOINTLIMIT)      m_pClientList[iClientH]->m_iVit = DEF_CHARPOINTLIMIT;
            if (m_pClientList[iClientH]->m_iInt > DEF_CHARPOINTLIMIT)      m_pClientList[iClientH]->m_iInt = DEF_CHARPOINTLIMIT;
            if (m_pClientList[iClientH]->m_iMag > DEF_CHARPOINTLIMIT)      m_pClientList[iClientH]->m_iMag = DEF_CHARPOINTLIMIT;
            if (m_pClientList[iClientH]->m_iCharisma > DEF_CHARPOINTLIMIT) m_pClientList[iClientH]->m_iCharisma = DEF_CHARPOINTLIMIT;

            // New 17/05/2004
            if (m_pClientList[iClientH]->m_iLevel > 100)
                if (m_pClientList[iClientH]->m_bIsPlayerCivil == true)
                    ForceChangePlayMode(iClientH, true);

            SendNotifyMsg(0, iClientH, DEF_NOTIFY_LEVELUP, 0, 0, 0, 0);

            m_pClientList[iClientH]->m_iNextLevelExp = m_iLevelExpTable[m_pClientList[iClientH]->m_iLevel + 1]; //iGetLevelExp(m_pClientList[iClientH]->m_iLevel + 1);
 
            CalcTotalItemEffect(iClientH, -1, false);

            log->info("(!) Level up: Player ({}) Level ({}) Experience({}) Next Level Experience({})", m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_iLevel, m_pClientList[iClientH]->m_iExp, m_pClientList[iClientH]->m_iNextLevelExp);
        }
        else { return true; }
    }
    bobdole = 3;
    return false;
}

// 2003-04-21 Ã€ÃŽÃ†Â®Â¿Â¡ ÂµÃ»Â¸Â¥ Â¸Â¶Â¹Ã½Ã€Â» Â»Ã¨ÃÂ¦ Â½ÃƒÃ„Ã‘ÃÃ˜Â´Ã™...
/////////////////////////////////////////////////////////////////////////////////////
//  bool CGame::bCheckMagicInt(int iClientH)  //another retarded korean function
//  desc		 :: Ã€ÃŽÃ†Â®Â°Â¡ Â³Â»Â·ÃÂ°Â¬Ã€Â»Â¶Â§ Â»Ã¨ÃÂ¦Ã‡Ã’ Â¸Â¶Â¹Ã½Ã€Â» ÃƒÂ³Â¸Â®Ã‡Ã‘Â´Ã™... ÃƒÂ¼Ã…Â©Â¸Â¸...
//	return value :: Â¹Â«ÃÂ¶Â°Ã‡ true   // ....dumbass koreans
//  date		 :: 2003-04-21
/////////////////////////////////////////////////////////////////////////////////////
bool CGame::bCheckMagicInt(int iClientH)
{

    for (int i = 0; i < DEF_MAXMAGICTYPE; i++)
    {
        if (m_pMagicConfigList[i] != 0)
            if (m_pMagicConfigList[i]->m_sIntLimit > m_pClientList[iClientH]->m_iInt)
            {
                m_pClientList[iClientH]->m_cMagicMastery[i] = 0;
            }
    }

    return true;
}

// 2003-04-14 ÃÃ¶ÃÂ¸ Ã†Ã·Ã€ÃŽÃ†Â®Â¸Â¦ Â·Â¹ÂºÂ§ Â¼Ã¶ÃÂ¤Â¿Â¡ Â¾ÂµÂ¼Ã¶ Ã€Ã–Â´Ã™...
/////////////////////////////////////////////////////////////////////////////////////
//  bChangeState(char cStateChange 
//  		  ,char* cStr, char *cVit,char *cDex,char *cInt,char *cMag,char *cChar)
//  desc		 :: ÃÃ¶ÃÂ¸ Ã†Ã·Ã€ÃŽÃ†Â®Â·ÃŽ Ã†Â¯Â¼ÂºÂ°Âª Â¹Ã™Â²ÃœÆ’â€Ã€Â» Â´ÃµÃ‡Ã˜ ÃÃ˜Â´Ã™...
//	return value :: BOOLÃ‡Ã¼ 0(FASLE) Â¿Â¡Â·Â¯ Â·Â¹ÂºÂ§ Â¼Ã¶ÃÂ¤ ÂºÃ’Â°Â¡...
//  date		 :: [2003-04-14] 
/////////////////////////////////////////////////////////////////////////////////////
bool CGame::bChangeState(char cStateChange, char * cStr, char * cVit, char * cDex, char * cInt, char * cMag, char * cChar)
{
    if (cStateChange == DEF_STR)
    {
        *cStr += 1;
    }
    else if (cStateChange == DEF_VIT)
    {
        *cInt += 1;
        //		if(m_pClientList[iClientH]->m_iStr - cVit < 10 || m_pClientList[iClientH]->cVit - cVit < DEF_CHARPOINTLIMIT )
        //			return 0;
    }
    else if (cStateChange == DEF_DEX)
    {
        *cVit += 1;
    }
    else if (cStateChange == DEF_INT)
    {
        *cDex += 1;
    }
    else if (cStateChange == DEF_MAG)
    {
        *cMag += 1;
    }
    else if (cStateChange == DEF_CHR)
    {
        *cChar += 1;
    }
    else
    {
        //Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â°Â¡ Ã€ÃŸÂ¸Ã¸ÂµÃˆ Â°ÂªÃ€Â» ÂºÂ¸Â³Â» Â¿Ã”Â´Ã™..
//		::MessageBox(0, "Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â® Â¹Ã¶Â±Ã—? - í´ë¼ì´ì–¸íŠ¸ ë²„ê·¸?","debug", MB_ICONEXCLAMATION | MB_YESNO) ;
        return 0;
    }

    return cStateChange;
}

bool CGame::bCheckLimitedUser(int iClientH)
{
    if (m_pClientList[iClientH] == nullptr) return false;

    if ((memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) &&
        (m_pClientList[iClientH]->m_iExp >= m_iLevelExp20))
    {
        // Â¿Â©Ã‡Ã Ã€ÃšÂ°Â¡ Â·Â¹ÂºÂ§ 20 Â°Ã¦Ã‡Ã¨Ã„Â¡Â¸Â¦ Â¾Ã²Â¾ÃºÂ´Ã™Â¸Ã© 19Â¼Ã¶ÃÃ˜Ã€Â¸Â·ÃŽ ÃˆÂ¯Â¿Ã¸. 

        m_pClientList[iClientH]->m_iExp = m_iLevelExp20 - 1;
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_TRAVELERLIMITEDLEVEL, 0, 0, 0, 0);
        return true;
    }


    return false;
}

bool CGame::bSetItemToBankItem(int iClientH, short sItemIndex)
{
    int i, iRet;
    uint32_t * dwp;
    uint16_t * wp;
    char * cp;
    short * sp;
    char cData[100];
    class CItem * pItem;

    // Â¼Ã’ÃÃ¶Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» ÂºÂ¸Â°Ã¼Ã‡Ã‘Â´Ã™.
    if (m_pClientList[iClientH] == nullptr) return false;
    if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return false;
    if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == 0) return false;
    //wh remove
    //if (m_pClientList[iClientH]->m_bIsInsideWarehouse == false) return false;

    for (i = 0; i < DEF_MAXBANKITEMS; i++)
        if (m_pClientList[iClientH]->m_pItemInBankList[i] == 0)
        {
            // ÂºÃ±Â¾Ã®Ã€Ã–Â´Ã‚ Ã€Â§Ã„Â¡Â¸Â¦ ÃƒÂ£Â¾Ã’Â´Ã™.

            m_pClientList[iClientH]->m_pItemInBankList[i] = m_pClientList[iClientH]->m_pItemList[sItemIndex];
            pItem = m_pClientList[iClientH]->m_pItemInBankList[i];
            // !!! Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ Ã†Ã·Ã€ÃŽÃ…ÃÂ¸Â¦ Ã€ÃŒÂµÂ¿Ã‡ÃŸÃ€Â¸Â´Ã Â±Ã¢ÃÂ¸Ã€Ã‡ Ã†Ã·Ã€ÃŽÃ…ÃÂ´Ã‚ NULLÂ°ÂªÃ€Â¸Â·ÃŽ Ã‡Ã’Â´Ã§. 
            m_pClientList[iClientH]->m_pItemList[sItemIndex] = 0;

            // Â¼Ã’ÃÃ¶Ã‡Â° ÃƒÃ‘ ÃÃŸÂ·Â® Ã€Ã§ Â°Ã¨Â»Ãª 
            iCalcTotalWeight(iClientH);

            dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
            *dwp = MSGID_NOTIFY;
            wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
            *wp = DEF_NOTIFY_ITEMTOBANK;

            cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);

            *cp = i; // Ã€Â§Ã„Â¡ Ã€ÃºÃ€Ã¥ 
            cp++;

            // 1Â°Â³.
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

            // v1.432
            sp = (short *)cp;
            *sp = pItem->m_sItemEffectValue2;
            cp += 2;

            // v1.42
            dwp = (uint32_t *)cp;
            *dwp = pItem->m_dwAttribute;
            cp += 4;

            // Â¾Ã†Ã€ÃŒÃ…Ã› ÃÂ¤ÂºÂ¸ Ã€Ã¼Â¼Ã› 
            iRet = m_pClientList[iClientH]->send_msg(cData, 55);
            switch (iRet)
            {
                case DEF_XSOCKEVENT_QUENEFULL:
                case DEF_XSOCKEVENT_SOCKETERROR:
                case DEF_XSOCKEVENT_CRITICALERROR:
                case DEF_XSOCKEVENT_SOCKETCLOSED:
                    // Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ ÂºÂ¸Â³Â¾Â¶Â§ Â¿Â¡Â·Â¯Â°Â¡ Â¹ÃŸÂ»Ã½Ã‡ÃŸÂ´Ã™Â¸Ã© ÃÂ¦Â°Ã…Ã‡Ã‘Â´Ã™. v1.41 ÃÂ¦Â°Ã…Ã‡ÃÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™.
                    // DeleteClient(iClientH, true, true);
                    return true; // v1.41 FALSEÂ¸Â¦ Â¹ÃÃˆÂ¯Ã‡ÃÂ¸Ã© Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¹Ã™Â´ÃšÂ¿Â¡ ÂºÂ¹Â»Ã§ÂµÃˆÂ´Ã™.
            }

            return true;
        }

    // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» ÂºÂ¸Â°Ã¼Ã‡Ã’ Â¿Â©Ã€Â¯Â°Ã¸Â°Â£Ã€ÃŒ Â¾Ã¸Â´Ã™.
    return false;
}

void CGame::ApplyPKpenalty(short sAttackerH, short sVictumH)
{
    int iV1, iV2;

    if (m_pClientList[sAttackerH] == 0) return;
    if (m_pClientList[sVictumH] == 0) return;
    // Â¾ÃˆÃ€Ã¼ Â°Ã¸Â°Ã Â¸Ã°ÂµÃ¥Â°Â¡ Ã„Ã‘ÃÃ¸ Â»Ã³Ã…Ã‚Â¿Â¡Â¼Â­ Â°Ã¸Â°ÃÃ€Â¸Â·ÃŽ Ã€ÃŽÃ‡Ã˜ Â°Â°Ã€Âº Ã†Ã­Ã€ÃŒ ÃÃ—Ã€Â»Â¼Ã¶Â´Ã‚ Â¾Ã¸ÃÃ¶Â¸Â¸ Â¸Â¸Â¾Ã  Ã€Ã–Ã€Â»Â°Ã¦Â¿Ã¬ Â¹Â«Â½Ãƒ 
    // Â´Ãœ Â°Ã¸Â°ÃÃ€ÃšÂ°Â¡ Â¹Ã¼ÃÃ‹Ã€ÃšÂ°Â¡ Â¾Ã†Â´ÃÂ¾Ã®Â¾ÃŸÂ¸Â¸ Ã‡Ã˜Â´Ã§ÂµÃˆÂ´Ã™.
    if ((m_pClientList[sAttackerH]->m_bIsSafeAttackMode == true) && (m_pClientList[sAttackerH]->m_iPKCount == 0)) return;
    if ((strcmp(m_pClientList[sVictumH]->m_cLocation, "aresden") != 0) && (strcmp(m_pClientList[sVictumH]->m_cLocation, "elvine") != 0) && (strcmp(m_pClientList[sVictumH]->m_cLocation, "elvhunter") != 0) && (strcmp(m_pClientList[sVictumH]->m_cLocation, "arehunter") != 0))
    {
        return;
    }

    // PK Count ÃÃµÂ°Â¡  
    m_pClientList[sAttackerH]->m_iPKCount++;

    _bPKLog(DEF_PKLOG_BYPK, sAttackerH, sVictumH, 0);

    iV1 = iDice((m_pClientList[sVictumH]->m_iLevel / 2) + 1, 50);
    iV2 = iDice((m_pClientList[sAttackerH]->m_iLevel / 2) + 1, 50);

    m_pClientList[sAttackerH]->m_iExp -= iV1;
    m_pClientList[sAttackerH]->m_iExp -= iV2;
    if (m_pClientList[sAttackerH]->m_iExp < 0) m_pClientList[sAttackerH]->m_iExp = 0;

    SendNotifyMsg(0, sAttackerH, DEF_NOTIFY_PKPENALTY, 0, 0, 0, 0);

    SendEventToNearClient_TypeA(sAttackerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);

    log->info("(!) PK-penalty: Player({}) Exp Loss({}) Exp({}) ", m_pClientList[sAttackerH]->m_cCharName, iV1+iV2, m_pClientList[sAttackerH]->m_iExp);

    m_stCityStatus[m_pClientList[sAttackerH]->m_cSide].iCrimes++;

    m_pClientList[sAttackerH]->m_iRating -= 10;
    if (m_pClientList[sAttackerH]->m_iRating > 10000)  m_pClientList[sAttackerH]->m_iRating = 10000;
    if (m_pClientList[sAttackerH]->m_iRating < -10000) m_pClientList[sAttackerH]->m_iRating = -10000;


    if (strcmp(m_pClientList[sAttackerH]->m_cLocation, "aresden") == 0)
    {
        if ((strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "arebrk11") == 0) ||
            (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "arebrk12") == 0) ||
            (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "arebrk21") == 0) ||
            (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "arebrk22") == 0) ||
            (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "aresden") == 0) ||
            (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "huntzone2") == 0) ||
            (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "areuni") == 0) ||
            (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "arefarm") == 0))
        {
            memset(m_pClientList[sAttackerH]->m_cLockedMapName, 0, sizeof(m_pClientList[sAttackerH]->m_cLockedMapName));
            strcpy(m_pClientList[sAttackerH]->m_cLockedMapName, "arejail");
            m_pClientList[sAttackerH]->m_iLockedMapTime = 60 * 3;
            RequestTeleportHandler(sAttackerH, "2   ", "arejail", -1, -1);
            return;
        }
    }

    if (strcmp(m_pClientList[sAttackerH]->m_cLocation, "elvine") == 0)
    {
        if ((strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "elvbrk11") == 0) ||
            (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "elvbrk12") == 0) ||
            (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "elvbrk21") == 0) ||
            (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "elvbrk22") == 0) ||
            (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "elvine") == 0) ||
            (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "huntzone1") == 0) ||
            (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "elvuni") == 0) ||
            (strcmp(m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cName, "elvfarm") == 0))
        {
            memset(m_pClientList[sAttackerH]->m_cLockedMapName, 0, sizeof(m_pClientList[sAttackerH]->m_cLockedMapName));
            strcpy(m_pClientList[sAttackerH]->m_cLockedMapName, "elvjail");
            m_pClientList[sAttackerH]->m_iLockedMapTime = 60 * 3;
            RequestTeleportHandler(sAttackerH, "2   ", "elvjail", -1, -1);
            return;
        }
    }
}

void CGame::ApplyCombatKilledPenalty(int iClientH, int cPenaltyLevel, bool bIsSAattacked)
{
    int iExp;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

    if (m_bIsCrusadeMode == true)
    {
        if (m_pClientList[iClientH]->m_iPKCount > 0)
        {
            m_pClientList[iClientH]->m_iPKCount--;
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_PKPENALTY, 0, 0, 0, 0);
            _bPKLog(DEF_PKLOG_REDUCECRIMINAL, 0, iClientH, 0);

        }
        return;
    }

    if (m_pClientList[iClientH]->m_iPKCount > 0)
    {
        m_pClientList[iClientH]->m_iPKCount--;
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_PKPENALTY, 0, 0, 0, 0);
        _bPKLog(DEF_PKLOG_REDUCECRIMINAL, 0, iClientH, 0);
    }

    iExp = iDice(1, (5 * cPenaltyLevel * m_pClientList[iClientH]->m_iLevel));

    if (m_pClientList[iClientH]->m_bIsNeutral == true) iExp = iExp / 3;


    m_pClientList[iClientH]->m_iExp -= iExp;
    if (m_pClientList[iClientH]->m_iExp < 0) m_pClientList[iClientH]->m_iExp = 0;

    SendNotifyMsg(0, iClientH, DEF_NOTIFY_EXP, 0, 0, 0, 0);

    if (m_pClientList[iClientH]->m_bIsNeutral != true)
    {
        if (m_pClientList[iClientH]->m_iLevel < 80)
        {
            cPenaltyLevel--;
            if (cPenaltyLevel <= 0) cPenaltyLevel = 1;
            _PenaltyItemDrop(iClientH, cPenaltyLevel, bIsSAattacked);
        }
        else _PenaltyItemDrop(iClientH, cPenaltyLevel, bIsSAattacked);
    }
}

void CGame::_PenaltyItemDrop(int iClientH, int iTotal, bool bIsSAattacked)
{
    int i, j, iRemainItem;
    char cItemIndexList[DEF_MAXITEMS], cItemIndex;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

    if ((m_pClientList[iClientH]->m_iAlterItemDropIndex != -1) && (m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex] != 0))
    {
        // Testcode
        if (m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ALTERITEMDROP)
        {
            // v2.04 Â½ÂºÃ…Ã¦ Â¿Ã€ÂºÃª Â»ÃµÃ…Â©Â¸Â®Ã†Ã„Ã€ÃŒÂ½ÂºÂ°Â¡ Â¸Ã‚Â´Ã‚ÃÃ¶ ÃˆÂ®Ã€ÃŽ
            // Â´Ã«ÃƒÂ¼Ã€Ã»Ã€Â¸Â·ÃŽ Â¶Â³Â¾Ã®ÃÃ¶Â´Ã‚ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Ã€Ã–Â´Ã™Â¸Ã© Â´Ã™Â¸Â¥ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¶Â³Â¾Ã®ÃÃ¶ÃÃ¶ Â¾ÃŠÂ°Ã­ Ã€ÃŒ Â¾Ã†Ã€ÃŒÃ…Ã›Â¸Â¸ Â¶Â³Â¾Ã®ÃÃ¸Â´Ã™. 
            if (m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex]->m_wCurLifeSpan > 0)
                m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex]->m_wCurLifeSpan--;

            DropItemHandler(iClientH, m_pClientList[iClientH]->m_iAlterItemDropIndex, -1, m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex]->m_cName);

            m_pClientList[iClientH]->m_iAlterItemDropIndex = -1;
        }
        else
        {
            // v2.04 testcode
            log->info("Alter Drop Item Index Error1");
            // Â´Ã™Â½Ãƒ Â°Ã‹Â»Ã¶ 
            for (i = 0; i < DEF_MAXITEMS; i++)
                if ((m_pClientList[iClientH]->m_pItemList[i] != 0) && (m_pClientList[iClientH]->m_pItemList[i]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ALTERITEMDROP))
                {
                    m_pClientList[iClientH]->m_iAlterItemDropIndex = i;
                    if (m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex]->m_wCurLifeSpan > 0)
                        m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex]->m_wCurLifeSpan--;

                    DropItemHandler(iClientH, m_pClientList[iClientH]->m_iAlterItemDropIndex, -1, m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex]->m_cName);
                    m_pClientList[iClientH]->m_iAlterItemDropIndex = -1;
                    return;
                }

            // Â¿Ã€Â·Ã¹Â¹ÃŸÂ»Ã½! ÃˆÃ±Â»Ã½Â¼Â®Ã€ÃŒ Â¾Ã¸Â´Ã™. 
            goto PID_DROP;
        }
        return;
    }

    PID_DROP:;

    for (i = 1; i <= iTotal; i++)
    {
        iRemainItem = 0;
        memset(cItemIndexList, 0, sizeof(cItemIndexList));

        for (j = 0; j < DEF_MAXITEMS; j++)
            if (m_pClientList[iClientH]->m_pItemList[j] != 0)
            {
                cItemIndexList[iRemainItem] = j;
                iRemainItem++;
            }

        if (iRemainItem == 0) return;
        cItemIndex = cItemIndexList[iDice(1, iRemainItem) - 1];


        if ((m_pClientList[iClientH]->m_pItemList[cItemIndex]->m_sTouchEffectType != 0) &&
            (m_pClientList[iClientH]->m_pItemList[cItemIndex]->m_sTouchEffectValue1 == m_pClientList[iClientH]->m_sCharIDnum1) &&
            (m_pClientList[iClientH]->m_pItemList[cItemIndex]->m_sTouchEffectValue2 == m_pClientList[iClientH]->m_sCharIDnum2) &&
            (m_pClientList[iClientH]->m_pItemList[cItemIndex]->m_sTouchEffectValue3 == m_pClientList[iClientH]->m_sCharIDnum3))
        {
            // Â°Â³Ã€ÃŽÂ¿Â¡Â°Ã” Â¼Ã“Ã‡Ã‘ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â¸Â·ÃŽ Â¶Â³Â¾Ã®ÃÃ¶ÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™. Â¸ÃÃ…Ã¤Â°Â°Ã€Âº Â°Â³Ã€ÃŽÂ¿Ã« Â¾Ã†Ã€ÃŒÃ…Ã› 
        }

        else if (
            (m_pClientList[iClientH]->m_pItemList[cItemIndex]->m_sIDnum >= 400) &&
            (m_pClientList[iClientH]->m_pItemList[cItemIndex]->m_sIDnum != 402) &&
            (m_pClientList[iClientH]->m_pItemList[cItemIndex]->m_sIDnum <= 428))
        {
        }

        else if (((m_pClientList[iClientH]->m_pItemList[cItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY) ||
            (m_pClientList[iClientH]->m_pItemList[cItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY)) &&
            (bIsSAattacked == false))
        {
            // Ã†Â¯Â¼Ã¶ Â´Ã‰Â·Ã‚Ã€Â» Â°Â¡ÃÃ¸ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Ã†Â¯Â¼Ã¶ Â°Ã¸Â°ÃÃ€Â» Â¹ÃžÃ€ÂºÂ°Ã” Â¾Ã†Â´ÃÂ¶Ã³Â¸Ã© Â¶Â³Â¾Ã®ÃÃ¶ÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™.
        }

        else if ((m_pClientList[iClientH]->m_bIsLuckyEffect == true) && (iDice(1, 10) == 5))
        {
            // Ã„Â³Â¸Â¯Ã…ÃÂ¿Â¡Â°Ã” Ã‡Ã Â¿Ã®ÃˆÂ¿Â°ÃºÂ°Â¡ Ã€Ã–Ã€Â¸Â¸Ã© 10% ÃˆÂ®Â·Ã¼Â·ÃŽ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¶Â³Â¾Ã®ÃÃ¶ÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™.
        }

        else DropItemHandler(iClientH, cItemIndex, -1, m_pClientList[iClientH]->m_pItemList[cItemIndex]->m_cName);
    }
}

int CGame::_iCalcMaxLoad(int iClientH)
{
    if (m_pClientList[iClientH] == nullptr) return 0;

    return (m_pClientList[iClientH]->m_iStr * 500 + m_pClientList[iClientH]->m_iLevel * 500);
}

int CGame::iAddDynamicObjectList(short sOwner, char cOwnerType, short sType, char cMapIndex, short sX, short sY, uint32_t dwLastTime, int iV1)
{
    int i;
    short sPreType;
    uint32_t dwTime, dwRegisterTime;

    m_pMapList[cMapIndex]->bGetDynamicObject(sX, sY, &sPreType, &dwRegisterTime);
    if (sPreType != 0) return 0;

    switch (sType)
    {
        case DEF_DYNAMICOBJECT_FIRE3:
        case DEF_DYNAMICOBJECT_FIRE:
            if (m_pMapList[cMapIndex]->bGetIsMoveAllowedTile(sX, sY) == false)
                return 0;
            if (dwLastTime != 0)
            {
                switch (m_pMapList[cMapIndex]->m_cWhetherStatus)
                {
                    case 1:	dwLastTime = dwLastTime - (dwLastTime / 2);       break;
                    case 2:	dwLastTime = (dwLastTime / 2) - (dwLastTime / 3); break;
                    case 3:	dwLastTime = (dwLastTime / 3) - (dwLastTime / 4); break;
                }

                if (dwLastTime == 0) dwLastTime = 1000;
            }
            break;

        case DEF_DYNAMICOBJECT_FISHOBJECT:
        case DEF_DYNAMICOBJECT_FISH:
            if (m_pMapList[cMapIndex]->bGetIsWater(sX, sY) == false)
                return 0;
            break;

        case DEF_DYNAMICOBJECT_ARESDENFLAG1:
        case DEF_DYNAMICOBJECT_ELVINEFLAG1:
        case DEF_DYNAMICOBJECT_MINERAL1:
        case DEF_DYNAMICOBJECT_MINERAL2:
            if (m_pMapList[cMapIndex]->bGetMoveable(sX, sY) == false)
                return 0;
            m_pMapList[cMapIndex]->SetTempMoveAllowedFlag(sX, sY, false);
            break;

    }

    for (i = 1; i < DEF_MAXDYNAMICOBJECTS; i++)
        if (m_pDynamicObjectList[i] == 0)
        {
            dwTime = timeGetTime();

            if (dwLastTime != 0)
                dwLastTime += (iDice(1, 4) * 1000);

            m_pDynamicObjectList[i] = new class CDynamicObject(sOwner, cOwnerType, sType, cMapIndex, sX, sY, dwTime, dwLastTime, iV1);
            m_pMapList[cMapIndex]->SetDynamicObject(i, sType, sX, sY, dwTime);
            SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, DEF_MSGTYPE_CONFIRM, cMapIndex, sX, sY, sType, i, 0);

            return i;
        }
    return 0;
}

void CGame::CheckDynamicObjectList()
{
    int i;
    uint32_t dwTime = timeGetTime(), dwRegisterTime;
    short sType;

    // Â³Â¯Â¾Â¾ ÃˆÂ¿Â°ÃºÂ¿Â¡ ÂµÃ»Â¸Â¥ ÂµÂ¿Ã€Ã» Â°Â´ÃƒÂ¼Ã€Ã‡ ÃÃ¶Â¼Ã“Â½ÃƒÂ°Â£ Â´ÃœÃƒÃ Ã€Â» Â°Ã¨Â»ÃªÃ‡Ã‘Â´Ã™. 
    for (i = 1; i < DEF_MAXDYNAMICOBJECTS; i++)
    {
        if ((m_pDynamicObjectList[i] != 0) && (m_pDynamicObjectList[i]->m_dwLastTime != 0))
        {

            switch (m_pDynamicObjectList[i]->m_sType)
            {
                case DEF_DYNAMICOBJECT_FIRE3:
                case DEF_DYNAMICOBJECT_FIRE:
                    // ÂºÃ’Ã€Ã‡ Â°Ã¦Â¿Ã¬ ÂºÃ±Â°Â¡ Â¿ÃˆÃ€Â¸Â·ÃŽ Ã€ÃŽÃ‡Ã˜ Â¼Ã¶Â¸Ã­Ã€ÃŒ Â±ÃžÂ°ÃÃˆÃ· Â´ÃœÃƒÃ ÂµÃˆÂ´Ã™.
                    switch (m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->m_cWhetherStatus)
                    {
                        case 0: break;
                        case 1:
                        case 2:
                        case 3:
                            // (Â³Â²Ã€Âº Â½ÃƒÂ°Â£/10)*ÂºÃ±Â¿Ã€Â´Ã‚ Â»Ã³Ã…Ã‚ Â¸Â¸Ã…Â­ Â½ÃƒÂ°Â£Ã€Â» Â»Â«Â´Ã™.
                            m_pDynamicObjectList[i]->m_dwLastTime = m_pDynamicObjectList[i]->m_dwLastTime -
                                (m_pDynamicObjectList[i]->m_dwLastTime / 10) * m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->m_cWhetherStatus;
                            break;
                    }
                    break;
            }
        }
    }

    // Â½ÃƒÂ°Â£Ã€ÃŒ Â¸Â¸Â·Ã¡ÂµÃˆ Â°Â´ÃƒÂ¼Â¸Â¦ ÃÂ¾Â·Ã¡Â½ÃƒÃ…Â²Â´Ã™. ÃÃ¶Â¼Ã“Â½ÃƒÂ°Â£Ã€ÃŒ NULLÃ€ÃŒÂ¸Ã© Â¿ÂµÂ¿Ã¸ÃˆÃ· Â³Â²Â¾Ã†Ã€Ã–Â´Ã‚ ÂµÂ¿Ã€Ã» Â°Â´ÃƒÂ¼Ã€ÃŒÂ´Ã™.
    for (i = 1; i < DEF_MAXDYNAMICOBJECTS; i++)
    {
        if ((m_pDynamicObjectList[i] != 0) && (m_pDynamicObjectList[i]->m_dwLastTime != 0) &&
            ((dwTime - m_pDynamicObjectList[i]->m_dwRegisterTime) >= m_pDynamicObjectList[i]->m_dwLastTime))
        {

            // ÃÃ¶Â¼Ã“Â½ÃƒÂ°Â£Ã€ÃŒ Â°Ã¦Â°ÃºÃ‡Ã‘ ÂµÂ¿Ã€Ã» Â°Â´ÃƒÂ¼Â¸Â¦ ÃƒÂ£Â¾Ã’Â´Ã™.
            m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->bGetDynamicObject(m_pDynamicObjectList[i]->m_sX, m_pDynamicObjectList[i]->m_sY, &sType, &dwRegisterTime);
            // ÂµÃ®Â·ÃÂ½ÃƒÂ°Â£Ã€ÃŒ Ã€ÃÃ„Â¡Ã‡Ã‘Â´Ã™Â¸Ã© Â°Â´ÃƒÂ¼Â°Â¡ Â»Ã§Â¶Ã³ÃÃ¸Â´Ã™Â´Ã‚ Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ ÂºÂ¸Â³Â»ÃÃ Â¾ÃŸ Ã‡Ã‘Â´Ã™.

            if (dwRegisterTime == m_pDynamicObjectList[i]->m_dwRegisterTime)
            {
                SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, DEF_MSGTYPE_REJECT, m_pDynamicObjectList[i]->m_cMapIndex, m_pDynamicObjectList[i]->m_sX, m_pDynamicObjectList[i]->m_sY, m_pDynamicObjectList[i]->m_sType, i, 0);
                // Â¸ÃŠÂ¿Â¡Â¼Â­ Â»Ã¨ÃÂ¦Ã‡Ã‘Â´Ã™.
                m_pMapList[m_pDynamicObjectList[i]->m_cMapIndex]->SetDynamicObject(0, 0, m_pDynamicObjectList[i]->m_sX, m_pDynamicObjectList[i]->m_sY, dwTime);
            }

            switch (sType)
            {
                case DEF_DYNAMICOBJECT_FISHOBJECT:
                case DEF_DYNAMICOBJECT_FISH:
                    // Â¹Â°Â°Ã­Â±Ã¢ Â¿Ã€ÂºÃªÃÂ§Ã†Â®Â°Â¡ Â»Ã§Â¶Ã³ÃÂ³Ã€Â¸Â¹Ã‡Â·ÃŽ ÃƒÂ³Â¸Â®Ã‡Ã‘Â´Ã™.
                    bDeleteFish(m_pDynamicObjectList[i]->m_sOwner, 2); // Â½ÃƒÂ°Â£Ã€ÃŒ ÃÃ¶Â³ÂªÂ¼Â­ Â¹Â°Â°Ã­Â±Ã¢Â°Â¡ Â»Ã§Â¶Ã³ÃÃ¸Â´Ã™.
                    break;
            }

            // Â¸Â®Â½ÂºÃ†Â®Â¸Â¦ Â»Ã¨ÃÂ¦Ã‡Ã‘Â´Ã™.
            delete m_pDynamicObjectList[i];
            m_pDynamicObjectList[i] = 0;
        }
    }
}

void CGame::CalculateSSN_ItemIndex(int iClientH, short sWeaponIndex, int iValue)
{
    short sSkillIndex;
    int   iOldSSN, iSSNpoint, iWeaponIndex;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    if (m_pClientList[iClientH]->m_pItemList[sWeaponIndex] == 0) return;
    if (m_pClientList[iClientH]->m_bIsKilled == true) return;

    sSkillIndex = m_pClientList[iClientH]->m_pItemList[sWeaponIndex]->m_sRelatedSkill;
    if ((sSkillIndex < 0) || (sSkillIndex >= DEF_MAXSKILLTYPE)) return;
    if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] == 0) return;

    iOldSSN = m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex];
    m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] += iValue;

    iSSNpoint = m_iSkillSSNpoint[m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] + 1];

    if ((m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] < 100) &&
        (m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] > iSSNpoint))
    {

        m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]++;

        switch (sSkillIndex)
        {
            case 0:  // Mining
            case 5:  // Hand-Attack
            case 13: // Manufacturing
                if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iStr * 2))
                {
                    m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
                    m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
                }
                else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
                break;

            case 3: // Magic-Resistance
                if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iLevel * 2))
                {
                    m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
                    m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
                }
                else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
                break;

            case 4:  // Magic
            case 21: // Staff-Attack
                if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iMag * 2))
                {
                    m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
                    m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
                }
                else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
                break;

            case 1:  // Fishing
            case 6:  // Archery
            case 7:  // Short-Sword
            case 8:  // Long-Sword
            case 9:  // Fencing 
            case 10: // Axe-Attack
            case 11: // Shield        	
            case 14: // Hammer 
                if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iDex * 2))
                {
                    m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
                    m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
                }
                else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
                break;

            case 2:	 // Farming
            case 12: // Alchemy
            case 15: // ÀÀ±ÞÃ³Ä¡
            case 19: // Pretend-Corpse
                if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iInt * 2))
                {
                    m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
                    m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
                }
                else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
                break;

            case 23: // Poison-Resistance
                if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iVit * 2))
                {
                    m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
                    m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
                }
                else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
                break;

            default:
                m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
                break;
        }

        // ¸¸¾à ¹«±â¿Í °ü·ÃÀÖ´Â ½ºÅ³ÀÌ ¿Ã¶ú°í ÇöÀç ±× ¹«±â¸¦ »ç¿ë ÁßÀÌ¶ó¸é ÇöÀç ¸íÁß·ü¿¡ 1À» ´õÇÑ´Ù. 
        if (m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] == 0)
        {
            // Ä«¿îÆ®°¡ 0ÀÌ¸é ½ºÅ³ÀÌ Á¤»óÀûÀ¸·Î ¿Ã¶ú´Ù´Â ÀÇ¹Ì. 
            if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1)
            {
                // ¾ç¼Õ¹«±â°¡ ÀåÂøµÇ¾î ÀÖ¾ú´Ù. 
                iWeaponIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
                if (m_pClientList[iClientH]->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sSkillIndex)
                {
                    // È°·ùÀÇ »ç¿ë¿¡ ÀÇÇÑ ½ºÅ³ÀÇ »ó½ÂÀÌ¾ú´Ù. ¸íÁß·üÀ» 1 ¿Ã¸°´Ù. 
                    m_pClientList[iClientH]->m_iHitRatio++;
                }
            }

            if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND] != -1)
            {
                // ¹«±â°¡ ÀåÂøµÇ¾î ÀÖ¾ú´Ù. 
                iWeaponIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
                if (m_pClientList[iClientH]->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sSkillIndex)
                {
                    // °ËÀÌ³ª Mace·ùÀÇ »ç¿ë¿¡ ÀÇÇÑ ½ºÅ³ÀÇ »ó½ÂÀÌ¾ú´Ù. ¸íÁß·üÀ» 1 ¿Ã¸°´Ù. 
                    m_pClientList[iClientH]->m_iHitRatio++;
                }
            }
        }

        if (m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] == 0)
        {
            // SKillÀÇ ÃÑÇÕÀÌ 600À» ³ÑÀ¸¸é ´Ù¸¥ ½ºÅ³Áß ÇÏ³ª¸¦ 1 ³»¸°´Ù. 
            bCheckTotalSkillMasteryPoints(iClientH, sSkillIndex);
            // SkillÀÌ ¿Ã¶ú´Ù´Â °ÍÀ» Å¬¶óÀÌ¾ðÆ®¿¡°Ô ¾Ë·ÁÁØ´Ù.
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILL, sSkillIndex, m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex], 0, 0);
        }
    }
}

void CGame::CalculateSSN_SkillIndex(int iClientH, short sSkillIndex, int iValue)
{
    int   iOldSSN, iSSNpoint, iWeaponIndex;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    if ((sSkillIndex < 0) || (sSkillIndex >= DEF_MAXSKILLTYPE)) return;
    if (m_pClientList[iClientH]->m_bIsKilled == true) return;

    // Â±Ã¢Â¼Ãº Â¼Ã¶ÃÃ˜Ã€ÃŒ 0Ã€ÃŒÂ¶Ã³Â¸Ã© Â½ÂºÃ…Â³Ã€Âº Â¿Ã€Â¸Â£ÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™.
    if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] == 0) return;

#ifdef DEF_TESTSERVER
    iValue = iValue * 100;
#endif

    iOldSSN = m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex];
    m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] += iValue;

    iSSNpoint = m_iSkillSSNpoint[m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] + 1];

    // Â¸Â¸Â¾Ã  SkillSSNÃ€ÃŒ ÃÂ¦Ã‡Ã‘Ã„Â¡Â¸Â¦ Â³Ã‘Â¾Ã®Â¼Â¹Â´Ã™Â¸Ã© SkillÂ°ÂªÃ€ÃŒ ÃÃµÂ°Â¡Ã‡Ã‘Â´Ã™. 
    if ((m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] < 100) &&
        (m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] > iSSNpoint))
    {

        // Â½ÂºÃ…Â³Ã€ÃŒ Â¿ÃƒÂ¶ÃºÂ´Ã™.
        m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]++;
        // Ã†Â¯Â¼ÂºÃ„Â¡ ÃÂ¦Ã‡Ã‘Ã€ÃŒ Ã€Ã–Â´Ã‚ SkillÃ€Â» ÃƒÂ³Â¸Â®Ã‡Ã‘Â´Ã™.
        switch (sSkillIndex)
        {
            case 0:
            case 5:
            case 13:
                if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iStr * 2))
                {
                    // ÃÂ¦Ã‡Ã‘Ã„Â¡ÂºÂ¸Â´Ã™ Ã„Â¿ÃÂ³Â´Ã™. Â¹Â«ÃˆÂ¿Ã€ÃŒÂ¹Ã‡Â·ÃŽ Ã€ÃŒÃ€Ã¼Â»Ã³Ã…Ã‚Â·ÃŽ ÂµÃ‡ÂµÂ¹Â¸Â°Â´Ã™.
                    m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
                    m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
                }
                else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
                break;

            case 3:
                // Â¸Â¶Â¹Ã½ Ã€ÃºÃ‡Ã—Ã€Âº ÃƒÃ–Â´Ã« Level*2Â¸Â¸Ã…Â­ Â¿Ã€Â¸Â¥Â´Ã™.
                if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iLevel * 2))
                {
                    // ÃÂ¦Ã‡Ã‘Ã„Â¡ÂºÂ¸Â´Ã™ Ã„Â¿ÃÂ³Â´Ã™. Â¹Â«ÃˆÂ¿Ã€ÃŒÂ¹Ã‡Â·ÃŽ Ã€ÃŒÃ€Ã¼Â»Ã³Ã…Ã‚Â·ÃŽ ÂµÃ‡ÂµÂ¹Â¸Â°Â´Ã™.
                    m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
                    m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
                }
                else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
                break;

            case 4:
            case 21:
                if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iMag * 2))
                {
                    // ÃÂ¦Ã‡Ã‘Ã„Â¡ÂºÂ¸Â´Ã™ Ã„Â¿ÃÂ³Â´Ã™. Â¹Â«ÃˆÂ¿Ã€ÃŒÂ¹Ã‡Â·ÃŽ Ã€ÃŒÃ€Ã¼Â»Ã³Ã…Ã‚Â·ÃŽ ÂµÃ‡ÂµÂ¹Â¸Â°Â´Ã™.
                    m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
                    m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
                }
                else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
                break;

            case 1:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
                if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iDex * 2))
                {
                    // ÃÂ¦Ã‡Ã‘Ã„Â¡ÂºÂ¸Â´Ã™ Ã„Â¿ÃÂ³Â´Ã™. Â¹Â«ÃˆÂ¿Ã€ÃŒÂ¹Ã‡Â·ÃŽ Ã€ÃŒÃ€Ã¼Â»Ã³Ã…Ã‚Â·ÃŽ ÂµÃ‡ÂµÂ¹Â¸Â°Â´Ã™.
                    m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
                    m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
                }
                else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
                break;

            case 2:
            case 12:
            case 14:
            case 15:
            case 19:
                if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iInt * 2))
                {
                    // ÃÂ¦Ã‡Ã‘Ã„Â¡ÂºÂ¸Â´Ã™ Ã„Â¿ÃÂ³Â´Ã™. Â¹Â«ÃˆÂ¿Ã€ÃŒÂ¹Ã‡Â·ÃŽ Ã€ÃŒÃ€Ã¼Â»Ã³Ã…Ã‚Â·ÃŽ ÂµÃ‡ÂµÂ¹Â¸Â°Â´Ã™.
                    m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
                    m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
                }
                else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
                break;

            case 23: // ÂµÂ¶Â¼Âº Ã€ÃºÃ‡Ã— 
                if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iVit * 2))
                {
                    // ÃÂ¦Ã‡Ã‘Ã„Â¡ÂºÂ¸Â´Ã™ Ã„Â¿ÃÂ³Â´Ã™. Â¹Â«ÃˆÂ¿Ã€ÃŒÂ¹Ã‡Â·ÃŽ Ã€ÃŒÃ€Ã¼Â»Ã³Ã…Ã‚Â·ÃŽ ÂµÃ‡ÂµÂ¹Â¸Â°Â´Ã™.
                    m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
                    m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
                }
                else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
                break;

            default:
                m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
                break;
        }

        // Â¸Â¸Â¾Ã  Â¹Â«Â±Ã¢Â¿Ã Â°Ã¼Â·ÃƒÃ€Ã–Â´Ã‚ Â½ÂºÃ…Â³Ã€ÃŒ Â¿ÃƒÂ¶ÃºÂ°Ã­ Ã‡Ã¶Ã€Ã§ Â±Ã— Â¹Â«Â±Ã¢Â¸Â¦ Â»Ã§Â¿Ã« ÃÃŸÃ€ÃŒÂ¶Ã³Â¸Ã© Ã‡Ã¶Ã€Ã§ Â¸Ã­ÃÃŸÂ·Ã¼Â¿Â¡ 1Ã€Â» Â´ÃµÃ‡Ã‘Â´Ã™. 
        if (m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] == 0)
        {
            // Ã„Â«Â¿Ã®Ã†Â®Â°Â¡ 0Ã€ÃŒÂ¸Ã© Â½ÂºÃ…Â³Ã€ÃŒ ÃÂ¤Â»Ã³Ã€Ã»Ã€Â¸Â·ÃŽ Â¿ÃƒÂ¶ÃºÂ´Ã™Â´Ã‚ Ã€Ã‡Â¹ÃŒ. 
            if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1)
            {
                // Â¾Ã§Â¼Ã•Â¹Â«Â±Ã¢Â°Â¡ Ã€Ã¥Ã‚Ã¸ÂµÃ‡Â¾Ã® Ã€Ã–Â¾ÃºÂ´Ã™. 
                iWeaponIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
                if (m_pClientList[iClientH]->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sSkillIndex)
                {
                    // ÃˆÂ°Â·Ã¹Ã€Ã‡ Â»Ã§Â¿Ã«Â¿Â¡ Ã€Ã‡Ã‡Ã‘ Â½ÂºÃ…Â³Ã€Ã‡ Â»Ã³Â½Ã‚Ã€ÃŒÂ¾ÃºÂ´Ã™. Â¸Ã­ÃÃŸÂ·Ã¼Ã€Â» 1 Â¿ÃƒÂ¸Â°Â´Ã™. 
                    m_pClientList[iClientH]->m_iHitRatio++;
                }
            }

            if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND] != -1)
            {
                // Â¹Â«Â±Ã¢Â°Â¡ Ã€Ã¥Ã‚Ã¸ÂµÃ‡Â¾Ã® Ã€Ã–Â¾ÃºÂ´Ã™. 
                iWeaponIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
                if (m_pClientList[iClientH]->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sSkillIndex)
                {
                    // Â°Ã‹Ã€ÃŒÂ³Âª MaceÂ·Ã¹Ã€Ã‡ Â»Ã§Â¿Ã«Â¿Â¡ Ã€Ã‡Ã‡Ã‘ Â½ÂºÃ…Â³Ã€Ã‡ Â»Ã³Â½Ã‚Ã€ÃŒÂ¾ÃºÂ´Ã™. Â¸Ã­ÃÃŸÂ·Ã¼Ã€Â» 1 Â¿ÃƒÂ¸Â°Â´Ã™. 
                    m_pClientList[iClientH]->m_iHitRatio++;
                }
            }
        }

        if (m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] == 0)
        {
            // SKillÃ€Ã‡ ÃƒÃ‘Ã‡Ã•Ã€ÃŒ 700Ã€Â» Â³Ã‘Ã€Â¸Â¸Ã© Â´Ã™Â¸Â¥ Â½ÂºÃ…Â³ÃÃŸ Ã‡ÃÂ³ÂªÂ¸Â¦ 1 Â³Â»Â¸Â°Â´Ã™. 
            bCheckTotalSkillMasteryPoints(iClientH, sSkillIndex);

            // SkillÃ€ÃŒ Â¿ÃƒÂ¶ÃºÂ´Ã™Â´Ã‚ Â°ÃÃ€Â» Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¿Â¡Â°Ã” Â¾Ã‹Â·ÃÃÃ˜Â´Ã™.
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILL, sSkillIndex, m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex], 0, 0);
        }
    }
}

void CGame::CalcNextWayPointDestination(int iNpcH)
{
    short sRange, sX, sY;
    int i, j, iMapIndex;
    bool bFlag;

    switch (m_pNpcList[iNpcH]->m_cMoveType)
    {
        case DEF_MOVETYPE_GUARD:
            // ÃÃ¶ÃÂ¤ÂµÃˆ Ã€Â§Ã„Â¡Â¸Â¦ Â¹Ã¾Â¾Ã®Â³ÂªÃÃ¶ Â¾ÃŠÃ€Â¸Â¸Ã§ ÃƒÃŸÂ°ÃÃ€ÃŒ Â³Â¡Â³Â­ ÃˆÃ„Â¿Â¡ ÂµÂ¹Â¾Ã†Â¿Ã‚Â´Ã™.
            break;

        case DEF_MOVETYPE_SEQWAYPOINT:

            m_pNpcList[iNpcH]->m_cCurWaypoint++;
            if (m_pNpcList[iNpcH]->m_cCurWaypoint >= m_pNpcList[iNpcH]->m_cTotalWaypoint)
                m_pNpcList[iNpcH]->m_cCurWaypoint = 1;
            m_pNpcList[iNpcH]->m_dX = (short)(m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_WaypointList[m_pNpcList[iNpcH]->m_iWayPointIndex[m_pNpcList[iNpcH]->m_cCurWaypoint]].x);
            m_pNpcList[iNpcH]->m_dY = (short)(m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_WaypointList[m_pNpcList[iNpcH]->m_iWayPointIndex[m_pNpcList[iNpcH]->m_cCurWaypoint]].y);
            break;

        case DEF_MOVETYPE_RANDOMWAYPOINT:

            m_pNpcList[iNpcH]->m_cCurWaypoint = (short)((rand() % (m_pNpcList[iNpcH]->m_cTotalWaypoint - 1)) + 1);
            m_pNpcList[iNpcH]->m_dX = (short)(m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_WaypointList[m_pNpcList[iNpcH]->m_iWayPointIndex[m_pNpcList[iNpcH]->m_cCurWaypoint]].x);
            m_pNpcList[iNpcH]->m_dY = (short)(m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_WaypointList[m_pNpcList[iNpcH]->m_iWayPointIndex[m_pNpcList[iNpcH]->m_cCurWaypoint]].y);
            break;

        case DEF_MOVETYPE_RANDOMAREA:

            sRange = (short)(m_pNpcList[iNpcH]->m_rcRandomArea.right - m_pNpcList[iNpcH]->m_rcRandomArea.left);
            m_pNpcList[iNpcH]->m_dX = (short)((rand() % sRange) + m_pNpcList[iNpcH]->m_rcRandomArea.left);
            sRange = (short)(m_pNpcList[iNpcH]->m_rcRandomArea.bottom - m_pNpcList[iNpcH]->m_rcRandomArea.top);
            m_pNpcList[iNpcH]->m_dY = (short)((rand() % sRange) + m_pNpcList[iNpcH]->m_rcRandomArea.top);
            break;

        case DEF_MOVETYPE_RANDOM:
            // Â¿ÃÃ€Ã¼ Â·Â£Â´Ã½ 
            //m_pNpcList[iNpcH]->m_dX = (rand() % (m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_sSizeX - 50)) + 15;
            //m_pNpcList[iNpcH]->m_dY = (rand() % (m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_sSizeY - 50)) + 15;
            iMapIndex = m_pNpcList[iNpcH]->m_cMapIndex;

            for (i = 0; i <= 30; i++)
            {
                // Â°Â¥ Â¼Ã¶ Ã€Ã–Â´Ã‚ ÃÃ‚Ã‡Â¥Â°Â¡ Â³ÂªÂ¿ÃƒÂ¶Â§ Â±Ã®ÃÃ¶ 30Â¹Ã¸ Â¹ÃÂºÂ¹ 
                sX = (rand() % (m_pMapList[iMapIndex]->m_sSizeX - 50)) + 15;
                sY = (rand() % (m_pMapList[iMapIndex]->m_sSizeY - 50)) + 15;

                bFlag = true;
                for (j = 0; j < DEF_MAXMGAR; j++)
                    if (m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[j].left != -1)
                    {
                        // Ã‡Ã‡Ã‡Ã˜Â¾ÃŸ Ã‡Ã’ ÃÃ‚Ã‡Â¥Â°Â¡ Ã€Ã–Â´Ã™. 
                        if ((sX >= m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[j].left) &&
                            (sX <= m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[j].right) &&
                            (sY >= m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[j].top) &&
                            (sY <= m_pMapList[iMapIndex]->m_rcMobGenAvoidRect[j].bottom))
                        {
                            // Avoid RectÂ¾ÃˆÃ€ÃŒÂ¹Ã‡Â·ÃŽ Ã€ÃŒ Ã€Â§Ã„Â¡Â¿Â¡Â´Ã‚ Â»Ã½Â¼ÂºÂ½ÃƒÃ…Â³ Â¼Ã¶ Â¾Ã¸Â´Ã™.	
                            bFlag = false;
                        }
                    }
                if (bFlag == true) goto CNW_GET_VALIDLOC_SUCCESS;
            }
            // Fail! 
            m_pNpcList[iNpcH]->m_tmp_iError = 0;
            return;

            CNW_GET_VALIDLOC_SUCCESS:;
            m_pNpcList[iNpcH]->m_dX = sX;
            m_pNpcList[iNpcH]->m_dY = sY;
            break;
    }

    m_pNpcList[iNpcH]->m_tmp_iError = 0; // @@@ !!! @@@
}

void CGame::DeleteNpc(int iNpcH)
{
    int  i, iNamingValue, iNumItem, iItemID, iItemIDs[MAX_NPCITEMDROP], iSlateID;
    char cTmp[21], cItemName[21];
    class CItem * pItem, * pItem2;
    uint32_t dwCount, dwTime;
    POINT ItemPositions[MAX_NPCITEMDROP];
    char cTemp[256];
    SYSTEMTIME SysTime;
    if (m_pNpcList[iNpcH] == 0) return;

    dwTime = timeGetTime();

    //Init number of items to 1 unless its a multidrop;
    iNumItem = 0;
    iItemID = 0; // No current item

    //Ã¬Â£Â½Ã¬â€“Â´Ã¬Å¾Ë†Ã«ÂËœ Ã¬Å“â€žÃ¬Â¹ËœÃ¬â€”ÂÃ¬â€žÅ“ Ã¬â€šÂ­Ã¬Â Å“ 
    SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_LOG, DEF_MSGTYPE_REJECT, 0, 0, 0);
    m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->ClearOwner(11, iNpcH, DEF_OWNERTYPE_NPC, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY);

    memset(cTmp, 0, sizeof(cTmp));
    strcpy(cTmp, (char *)(m_pNpcList[iNpcH]->m_cName + 2));
    // NPCÃ¬ÂËœ NamigValueÃ«Â¥Â¼ Ã¬â€“Â»Ã¬â€“Â´Ã¬â„¢â‚¬ Ã¬â€šÂ¬Ã¬Å¡Â©Ã¬Â¤â€˜Ã¬ÂÂ¸ Ã­â€˜Å“Ã¬â€¹Å“Ã«Â¥Â¼ Ã­â€¢Â´Ã¬Â§â‚¬Ã­â€¢Å“Ã«â€¹Â¤.
    iNamingValue = atoi(cTmp);

    // NamingValueÃ«Â¥Â¼ Ã«Â¹â€žÃ¬Å¡Â°ÃªÂ³Â  Ã«Ââ„¢Ã¬Å¾â€˜Ã¬Â¤â€˜Ã¬ÂÂ¸ ÃªÂ°Å“Ã¬Â²Â´ Ã¬Ë†ËœÃ«Â¥Â¼ ÃªÂ°ÂÃ¬â€ Å’Ã¬â€¹Å“Ã­â€šÂ¨Ã«â€¹Â¤.
    m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->SetNamingValueEmpty(iNamingValue);
    m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_iTotalActiveObject--;

    // Spot-mob-generatorÃ¬Â â€¢Ã«Â³Â´ 
    if (m_pNpcList[iNpcH]->m_iSpotMobIndex != 0)
        m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_stSpotMobGenerator[m_pNpcList[iNpcH]->m_iSpotMobIndex].iCurMobs--;

    RemoveFromTarget(iNpcH, DEF_OWNERTYPE_NPC);

    // Ã­ÂÂ¬Ã«Â£Â¨Ã¬â€žÂ¸Ã¬ÂÂ´Ã«â€œÅ“Ã¬Å¡Â© ÃªÂ±Â´Ã¬Â¶â€¢Ã«Â¬Â¼Ã¬ÂÂ´Ã«ÂÂ¼Ã«Â©Â´ Ã¬Â§â‚¬Ã­Å“ËœÃªÂ´â‚¬ Ã­â€ ÂµÃ«Â³Â´Ã¬Å¡Â© ÃªÂ±Â´Ã¬Â¶â€¢Ã«Â¬Â¼ Ã«Â¦Â¬Ã¬Å Â¤Ã­Å Â¸Ã«Â¥Â¼ Ã­â€¢Â´Ã¬Â Å“
    switch (m_pNpcList[iNpcH]->m_sType)
    {
        case 36:
        case 37:
        case 38:
        case 39:
        case 42:
            m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->bRemoveCrusadeStructureInfo(m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY);
            // ÃªÂ¸Â¸Ã«â€œÅ“ ÃªÂ±Â´Ã¬Â¶â€¢Ã«Â¬Â¼ ÃªÂ°Å“Ã¬Ë†ËœÃ«Ââ€ž ÃªÂ°ÂÃ¬â€ Å’Ã¬â€¹Å“Ã­â€šÂ´ 
            for (i = 0; i < DEF_MAXGUILDS; i++)
                if (m_pGuildTeleportLoc[i].m_iV1 == m_pNpcList[iNpcH]->m_iGuildGUID)
                {
                    m_pGuildTeleportLoc[i].m_dwTime = dwTime;
                    m_pGuildTeleportLoc[i].m_iV2--;
                    if (m_pGuildTeleportLoc[i].m_iV2 < 0) m_pGuildTeleportLoc[i].m_iV2 = 0;
                    break;
                }
            break;
        case 64: m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->bRemoveCropsTotalSum(); break;

    }

    // DelayEventÃ«Â¥Â¼ Ã¬â€šÂ­Ã¬Â Å“ 
    bRemoveFromDelayEventList(iNpcH, DEF_OWNERTYPE_NPC, 0);
    if ((m_pNpcList[iNpcH]->m_bIsSummoned == false) && (m_pNpcList[iNpcH]->m_bIsUnsummoned == false))
    {
        // Ã¬â€ Å’Ã­â„¢ËœÃ«ÂªÂ¹Ã¬ÂÂ´ Ã¬â€¢â€žÃ«â€¹Ë†Ã«ÂÂ¼Ã«Â©Â´ Ã¬â€”Â°ÃªÂ¸Ë† Ã¬Å¾Â¬Ã«Â£Å’ÃªÂ°â‚¬ Ã«â€šËœÃ¬ËœÂ¨Ã«â€¹Â¤.
        pItem = new class CItem;
        memset(cItemName, 0, sizeof(cItemName));
        switch (m_pNpcList[iNpcH]->m_sType)
        {

            case 10: // Slime
                if (iDice(1, 25) == 1) iItemID = 220; break; // SlimeJelly 
                break;

            case 11: // Skeleton
                switch (iDice(1, 2))
                {
                    case 1:if (iDice(1, 20) == 1) iItemID = 219; break; // SkeletonBones
                    case 2: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType); break;
                    default: break;
                }
                break;

            case 12: // Stone-Golem
                switch (iDice(1, 2))
                {
                    case 1:	if (iDice(1, 30) == 1) iItemID = 221; break; // StoneGolemPiece
                    case 2: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType); break;
                    default: break;
                }
                break;

            case 13: // Cyclops
                switch (iDice(1, 6))
                {
                    case 1:	if (iDice(1, 36) == 1) iItemID = 194; break; // CyclopsEye
                    case 2:	if (iDice(1, 40) == 1) iItemID = 195; break; // CyclopsHandEdge
                    case 3:	if (iDice(1, 30) == 1) iItemID = 196; break; // CyclopsHeart
                    case 4:	if (iDice(1, 22) == 1) iItemID = 197; break; // CyclopsMeat
                    case 5:	if (iDice(1, 40) == 1) iItemID = 198; break; // CyclopsLeather
                    case 6:	bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType); break;
                    default: break;
                }
                break;

            case 14: // Orc, Orc-Mage
                switch (iDice(1, 4))
                {
                    case 1:	if (iDice(1, 11) == 1) iItemID = 206; break; // OrcMeat
                    case 2:	if (iDice(1, 20) == 1) iItemID = 207; break; // OrcLeather
                    case 3:	if (iDice(1, 21) == 1) iItemID = 208; break; // OrcTeeth
                    case 4: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType); break;
                    default: break;
                }
                break;

            case 16: //Giant-Ant
                switch (iDice(1, 3))
                {
                    case 1:	if (iDice(1, 9) == 1) iItemID = 192; break; // AntLeg
                    case 2:	if (iDice(1, 10) == 1) iItemID = 193; break; // AntFeeler
                    case 3:	bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType); break;
                    default: break;
                }
                break;

            case 17: //Scorpion
                switch (iDice(1, 5))
                {
                    case 1:	if (iDice(1, 50) == 1) iItemID = 215; break; // ScorpionPincers
                    case 2:	if (iDice(1, 20) == 1) iItemID = 216; break; // ScorpionMeat
                    case 3: if (iDice(1, 50) == 1) iItemID = 217; break; // ScorpionSting
                    case 4: if (iDice(1, 40) == 1) iItemID = 218; break; // ScorpionSkin
                    case 5: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType); break;
                    default: break;
                }
                break;

            case 18: //Zombie
                bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType); break;
                break;

            case 22: //Amphis
                switch (iDice(1, 5))
                {
                    case 1: if (iDice(1, 15) == 1) iItemID = 188; break; // SnakeMeat
                    case 2:	if (iDice(1, 16) == 1) iItemID = 189; break; // SnakeSkin
                    case 3:	if (iDice(1, 16) == 1) iItemID = 190; break; // SnakeTeeth
                    case 4:	if (iDice(1, 17) == 1) iItemID = 191; break; // SnakeTongue
                    case 5: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType); break;
                    default: break;
                }
                break;

            case 23: //Clay-Golem
                switch (iDice(1, 2))
                {
                    case 1: if (iDice(1, 30) == 1) iItemID = 205; break; // LumpofClay
                    case 2: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType); break;
                    default: break;
                }
                break;


            case 27: //Hellbound
                switch (iDice(1, 7))
                {
                    case 1:	if (iDice(1, 40) == 1) iItemID = 199; break; // HelboundHeart
                    case 2:	if (iDice(1, 38) == 1) iItemID = 200; break; // HelboundLeather
                    case 3:	if (iDice(1, 38) == 1) iItemID = 201; break; // HelboundTail
                    case 4:	if (iDice(1, 36) == 1) iItemID = 202; break; // HelboundTeeth
                    case 5:	if (iDice(1, 36) == 1) iItemID = 203; break; // HelboundClaw
                    case 6:	if (iDice(1, 50) == 1) iItemID = 204; break; // HelboundTongue
                    case 7: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType); break;
                    default: break;
                }
                break;

            case 28: //Troll
                switch (iDice(1, 5))
                {
                    case 1:	if (iDice(1, 35) == 1) iItemID = 222; break; // TrollHeart
                    case 2:	if (iDice(1, 23) == 1) iItemID = 223; break; // TrollMeat
                    case 3:	if (iDice(1, 25) == 1) iItemID = 224; break; // TrollLeather
                    case 4:	if (iDice(1, 27) == 1) iItemID = 225; break; // TrollClaw
                    case 5: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType); break;
                    default: break;
                }
                break;

            case 29: //Orge
                switch (iDice(1, 7))
                {
                    case 1:	if (iDice(1, 20) == 1) iItemID = 209; break; // OgreHair
                    case 2:	if (iDice(1, 22) == 1) iItemID = 210; break; // OgreHeart
                    case 3:	if (iDice(1, 25) == 1) iItemID = 211; break; // OgreMeat
                    case 4:	if (iDice(1, 25) == 1) iItemID = 212; break; // OgreLeather
                    case 5:	if (iDice(1, 28) == 1) iItemID = 213; break; // OgreTeeth
                    case 6:	if (iDice(1, 28) == 1) iItemID = 214; break; // OgreClaw
                    case 7: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType); break;
                    default: break;
                }
                break;

            case 30: //Liche
                bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType); break;
                break;

            case 31: //Demon
                switch (iDice(1, 5))
                {
                    case 1:	if (iDice(1, 400) == 123) iItemID = 541; break; // DemonHeart
                    case 2:	if (iDice(1, 1000) == 123) iItemID = 542; break; // DemonMeat
                    case 3:	if (iDice(1, 200) == 123) iItemID = 543; break; // DemonLeather
                    case 4:	if (iDice(1, 300) == 123) iItemID = 540; break; // DemonEye
                    case 5: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType); break;
                    default: break;
                }
                break;

            case 32: //Unicorn
                switch (iDice(1, 5))
                {
                    case 1:	if (iDice(1, 3000) == 396) iItemID = 544; break; // UnicornHeart
                    case 2:	if (iDice(1, 500) == 3) iItemID = 545; break; // UnicornHorn
                    case 3:	if (iDice(1, 100) == 3) iItemID = 546; break; // UnicornMeat
                    case 4:	if (iDice(1, 200) == 3) iItemID = 547; break; // UnicornLeather
                    case 5: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType); break;
                    default: break;
                }
                break;

            case 33: //WereWolf
                switch (iDice(1, 8))
                {
                    case 1: if (iDice(1, 30) == 3) iItemID = 551; break; // WerewolfTail
                    case 2:	if (iDice(1, 28) == 3) iItemID = 548; break; // WerewolfHeart
                    case 3:	if (iDice(1, 25) == 3) iItemID = 550; break; // WerewolfMeat
                    case 4:	if (iDice(1, 35) == 3) iItemID = 553; break; // WerewolfLeather
                    case 5:	if (iDice(1, 28) == 3) iItemID = 552; break; // WerewolfTeeth
                    case 6:	if (iDice(1, 28) == 3) iItemID = 554; break; // WerewolfClaw
                    case 7:	if (iDice(1, 38) == 3) iItemID = 549; break; // WerewolfNail
                    case 8: bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType); break;
                    default: break;
                }
                break;

            case 48: //Stalker
            case 49: //Hellclaw
            case 50: //Tigerworm
            case 52: //Gagoyle
            case 53: //Beholder
            case 54: //Dark-Elf
            case 55: //Rabbit
            case 56: //Cat
            case 57: //Giant-Frog
            case 58: //Mountain-Giant
            case 59: //Ettin
            case 60: //Cannibal-Plant
            case 61: //Rudolph
            case 62: //DireBoar
            case 63: //Frost
            case 65: //Ice
            case 70: //Barlog
                bGetItemNameWhenDeleteNpc(iItemID, m_pNpcList[iNpcH]->m_sType);
                break;

                // new 05/10/2004
            case 66: // Wyvern
                bGetMultipleItemNamesWhenDeleteNpc(m_pNpcList[iNpcH]->m_sType,	// NPC Type
                    50,				// ÃˆÂ®Â·Ã¼( ÃƒÃ–Â¼Ã’ ~ ÃƒÃ–Â´Ã« Â»Ã§Ã€ÃŒÃ€Ã‡ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â³ÂªÂ¿Ãƒ ÃˆÂ®Â·Ã¼ , 100 Ã€ÃŒÂ¸Ã© ÃƒÃ–Â´Ã« Â°Â¹Â¼Ã¶ Â¸Â¸Ã…Â­ Â³ÂªÂ¿Ã‚Â´Ã™.)
                    5,				// Â³ÂªÂ¿ÃÂ¾ÃŸ Ã‡ÃÂ´Ã‚ Item ÃƒÃ–Â¼Ã’ Â°Â³Â¼Ã¶
                    15,				// Â³ÂªÂ¿ÃƒÂ¼Ã¶ Ã€Ã–Â´Ã‚ Item ÃƒÃ–Â´Ã« Â°Â³Â¼Ã¶
                    m_pNpcList[iNpcH]->m_sX,	// Â±Ã¢ÃÃ˜ ÃÃ‚Ã‡Â¥ X
                    m_pNpcList[iNpcH]->m_sY,	// Â±Ã¢ÃÃ˜ ÃÃ‚Ã‡Â¥ Y
                    DEF_ITEMSPREAD_FIXED,		// Â¾Ã†Ã€ÃŒÃ…Ã› Â»Ã‘Â¸Â®Â´Ã‚ Â¹Ã¦Â¹Ã½ (RANDOM, FIXED)
                    4,				// Â¾Ã†Ã€ÃŒÃ…Ã› Â»Ã‘Â¸Â± Â¹Ã¼Ã€Â§, DEF_ITEMSPREAD_RANDOM Ã€ÃÂ¶Â§Â¸Â¸ Â»Ã§Â¿Ã«
                    iItemIDs,			// Â¹ÃžÂ¾Ã†Â¿Ãƒ Â¾Ã†Ã€ÃŒÃ…Ã› IdÂµÃ©
                    ItemPositions,			// Â¾Ã†Ã€ÃŒÃ…Ã›ÂµÃ©Ã€Ã‡ Ã€Â§Ã„Â¡
                    &iNumItem);			// Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ ÃƒÃ‘ Â°Â¹Â¼Ã¶
                break;

            case 73: // Fire-Wyvern
                bGetMultipleItemNamesWhenDeleteNpc(m_pNpcList[iNpcH]->m_sType,	// NPC Type
                    50,				// ÃˆÂ®Â·Ã¼( ÃƒÃ–Â¼Ã’ ~ ÃƒÃ–Â´Ã« Â»Ã§Ã€ÃŒÃ€Ã‡ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â³ÂªÂ¿Ãƒ ÃˆÂ®Â·Ã¼ , 100 Ã€ÃŒÂ¸Ã© ÃƒÃ–Â´Ã« Â°Â¹Â¼Ã¶ Â¸Â¸Ã…Â­ Â³ÂªÂ¿Ã‚Â´Ã™.)
                    5,				// Â³ÂªÂ¿ÃÂ¾ÃŸ Ã‡ÃÂ´Ã‚ Item ÃƒÃ–Â¼Ã’ Â°Â³Â¼Ã¶
                    15,				// Â³ÂªÂ¿ÃƒÂ¼Ã¶ Ã€Ã–Â´Ã‚ Item ÃƒÃ–Â´Ã« Â°Â³Â¼Ã¶
                    m_pNpcList[iNpcH]->m_sX,	// Â±Ã¢ÃÃ˜ ÃÃ‚Ã‡Â¥ X
                    m_pNpcList[iNpcH]->m_sY,	// Â±Ã¢ÃÃ˜ ÃÃ‚Ã‡Â¥ Y
                    2,				// Â¾Ã†Ã€ÃŒÃ…Ã› Â»Ã‘Â¸Â®Â´Ã‚ Â¹Ã¦Â¹Ã½ (RANDOM, FIXED)
                    4,				// Â¾Ã†Ã€ÃŒÃ…Ã› Â»Ã‘Â¸Â± Â¹Ã¼Ã€Â§, DEF_ITEMSPREAD_RANDOM Ã€ÃÂ¶Â§Â¸Â¸ Â»Ã§Â¿Ã«
                    iItemIDs,			// Â¹ÃžÂ¾Ã†Â¿Ãƒ Â¾Ã†Ã€ÃŒÃ…Ã› IdÂµÃ©
                    ItemPositions,			// Â¾Ã†Ã€ÃŒÃ…Ã›ÂµÃ©Ã€Ã‡ Ã€Â§Ã„Â¡
                    &iNumItem);			// Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ ÃƒÃ‘ Â°Â¹Â¼Ã¶
                break;
            case 81:
                bGetMultipleItemNamesWhenDeleteNpc(m_pNpcList[iNpcH]->m_sType,	// NPC Type
                    50,				// ÃˆÂ®Â·Ã¼( ÃƒÃ–Â¼Ã’ ~ ÃƒÃ–Â´Ã« Â»Ã§Ã€ÃŒÃ€Ã‡ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â³ÂªÂ¿Ãƒ ÃˆÂ®Â·Ã¼ , 100 Ã€ÃŒÂ¸Ã© ÃƒÃ–Â´Ã« Â°Â¹Â¼Ã¶ Â¸Â¸Ã…Â­ Â³ÂªÂ¿Ã‚Â´Ã™.)
                    12,				// Â³ÂªÂ¿ÃÂ¾ÃŸ Ã‡ÃÂ´Ã‚ Item ÃƒÃ–Â¼Ã’ Â°Â³Â¼Ã¶
                    20,				// Â³ÂªÂ¿ÃƒÂ¼Ã¶ Ã€Ã–Â´Ã‚ Item ÃƒÃ–Â´Ã« Â°Â³Â¼Ã¶
                    m_pNpcList[iNpcH]->m_sX,	// Â±Ã¢ÃÃ˜ ÃÃ‚Ã‡Â¥ X
                    m_pNpcList[iNpcH]->m_sY,	// Â±Ã¢ÃÃ˜ ÃÃ‚Ã‡Â¥ Y
                    DEF_ITEMSPREAD_FIXED,		// Â¾Ã†Ã€ÃŒÃ…Ã› Â»Ã‘Â¸Â®Â´Ã‚ Â¹Ã¦Â¹Ã½ (RANDOM, FIXED)
                    65,				// Â¾Ã†Ã€ÃŒÃ…Ã› Â»Ã‘Â¸Â± Â¹Ã¼Ã€Â§, DEF_ITEMSPREAD_RANDOM Ã€ÃÂ¶Â§Â¸Â¸ Â»Ã§Â¿Ã«
                    iItemIDs,			// Â¹ÃžÂ¾Ã†Â¿Ãƒ Â¾Ã†Ã€ÃŒÃ…Ã› IdÂµÃ©
                    ItemPositions,			// Â¾Ã†Ã€ÃŒÃ…Ã›ÂµÃ©Ã€Ã‡ Ã€Â§Ã„Â¡
                    &iNumItem);
                break;
        }

        dwCount = 1;
        if (iNumItem > 0)
        {
            GetLocalTime(&SysTime);
            wsprintf(cTemp, "%d%02d%", SysTime.wMonth, SysTime.wDay);
            for (int j = 0; j < iNumItem; j++)
            {
                if (pItem == 0)
                {
                    pItem = new class CItem;
                }
                if (_bInitItemAttr(pItem, iItemIDs[j]) == false ||
                    m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->bGetIsMoveAllowedTile(ItemPositions[j].x, ItemPositions[j].y) == false)
                {
                    delete pItem;
                    pItem = 0;
                }
                else
                {
                    if (iItemIDs[j] == 90) // Gold
                        pItem->m_dwCount = iDice(10, 15000);
                    else
                        pItem->m_dwCount = dwCount;

                    pItem->m_sTouchEffectType = DEF_ITET_ID;
                    pItem->m_sTouchEffectValue1 = iDice(1, 100000);
                    pItem->m_sTouchEffectValue2 = iDice(1, 100000);
                    pItem->m_sTouchEffectValue3 = (short)timeGetTime();
                    m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->bSetItem(ItemPositions[j].x, ItemPositions[j].y, pItem);
                    SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pNpcList[iNpcH]->m_cMapIndex,
                        ItemPositions[j].x, ItemPositions[j].y, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);
                    _bItemLog(DEF_ITEMLOG_NEWGENDROP, 0, m_pNpcList[iNpcH]->m_cNpcName, pItem);
                    pItem = 0;
                }
            }
        }
        else
        {
            if (_bInitItemAttr(pItem, iItemID) == false)
            {
                delete pItem;
                pItem = 0;
            }
            else
            {
                pItem->m_dwCount = dwCount;

                pItem->m_sTouchEffectType = DEF_ITET_ID;
                pItem->m_sTouchEffectValue1 = iDice(1, 100000);
                pItem->m_sTouchEffectValue2 = iDice(1, 100000);
                pItem->m_sTouchEffectValue3 = (short)timeGetTime();
                m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->bSetItem(m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, pItem);
                SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pNpcList[iNpcH]->m_cMapIndex,
                    m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);
                _bItemLog(DEF_ITEMLOG_NEWGENDROP, 0, m_pNpcList[iNpcH]->m_cNpcName, pItem);
            }
        }

        if (iDice(1, 100000) < 10)
        {
            pItem2 = new class CItem;
            switch (iDice(1, 4))
            {
                case 1:	iSlateID = 868; break;
                case 2: iSlateID = 869; break;
                case 3: iSlateID = 870; break;
                case 4: iSlateID = 871; break;
            }
            if (_bInitItemAttr(pItem2, iSlateID) == false)
            {
                delete pItem2;
                pItem2 = 0;
            }
            else
            {
                pItem2->m_dwCount = 1;

                pItem2->m_sTouchEffectType = DEF_ITET_ID;
                pItem2->m_sTouchEffectValue1 = iDice(1, 100000);
                pItem2->m_sTouchEffectValue2 = iDice(1, 100000);
                pItem2->m_sTouchEffectValue3 = (short)timeGetTime();

                m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->bSetItem(m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, pItem2);
                SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pNpcList[iNpcH]->m_cMapIndex,
                    m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, pItem2->m_sSprite, pItem2->m_sSpriteFrame, pItem2->m_cItemColor);
                _bItemLog(DEF_ITEMLOG_NEWGENDROP, 0, m_pNpcList[iNpcH]->m_cNpcName, pItem2);
            }
        }
    }

    delete m_pNpcList[iNpcH];
    m_pNpcList[iNpcH] = 0;
}

void CGame::RequestFullObjectData(int iClientH, char * pData)
{
    uint32_t * dwp;
    uint16_t * wp, wObjectID;
    char * cp, cData[100];
    short * sp, sX, sY;
    int     sTemp, sTemp2;
    int * ip, iRet;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

    wp = (uint16_t *)(pData + DEF_INDEX2_MSGTYPE);
    wObjectID = *wp;

    memset(cData, 0, sizeof(cData));
    dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
    *dwp = MSGID_EVENT_MOTION;
    wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
    *wp = DEF_OBJECTSTOP;

    cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);

    if (wObjectID < 10000)
    {
        // Ã„Â³Â¸Â¯Ã…ÃÃ€Ã‡ ÃÂ¤ÂºÂ¸Â¸Â¦ Â¿Ã¸Ã‡Ã‘Â´Ã™. 
        // Ã€ÃŸÂ¸Ã¸ÂµÃˆ Ã€ÃŽÂµÂ¦Â½ÂºÂ°ÂªÃ€ÃŒÂ°Ã…Â³Âª ÃÂ¸Ã€Ã§Ã‡ÃÃÃ¶ Â¾ÃŠÂ´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â¶Ã³Â¸Ã© Â¹Â«Â½Ãƒ.
        if ((wObjectID == 0) || (wObjectID >= DEF_MAXCLIENTS)) return;
        if (m_pClientList[wObjectID] == 0) return;

        wp = (uint16_t *)cp;
        *wp = wObjectID;			// ObjectID
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
        //v1.4 ApprColor
        ip = (int *)cp;
        *ip = m_pClientList[wObjectID]->m_iApprColor;
        cp += 4;

        ip = (int *)cp;

        // m_pClientList[i]Â¿Ã m_pClientList[sOwnerH]Ã€Ã‡ Â°Ã¼Â°Ã¨Â¸Â¦ Ã€Ã”Â·Ã‚Ã‡Ã‘Â´Ã™.
        // sStatusÃ€Ã‡ Â»Ã³Ã€Â§ 4ÂºÃ±Ã†Â®Â°Â¡ FOE Â°Ã¼Â°Ã¨Â¸Â¦ Â³ÂªÃ…Â¸Â³Â½Â´Ã™. 
        sTemp = m_pClientList[wObjectID]->m_iStatus;
        sTemp = 0x0FFFFFFF & sTemp;//Original : sTemp = 0x0FFF & sTemp; // Â»Ã³Ã€Â§ 4ÂºÃ±Ã†Â® Ã…Â¬Â¸Â®Â¾Ã®
        sTemp2 = iGetPlayerABSStatus(wObjectID, iClientH); //(short)iGetPlayerRelationship(iClientH, wObjectID);
        sTemp = (sTemp | (sTemp2 << 28));//Original : 12

        *ip = sTemp;
        //*sp = DEF_TEST;
        cp += 4;//Original 2

        if (m_pClientList[wObjectID]->m_bIsKilled == true)
            *cp = 1;
        else *cp = 0;
        cp++;

        iRet = m_pClientList[iClientH]->send_msg(cData, 41);
    }
    else
    {
        // NPCÃ€Ã‡ ÃÂ¤ÂºÂ¸Â¸Â¦ Â¿Ã¸Ã‡Ã‘Â´Ã™.
        // Ã€ÃŸÂ¸Ã¸ÂµÃˆ Ã€ÃŽÂµÂ¦Â½Âº Â°ÂªÃ€ÃŒÂ°Ã…Â³Âª Â»Ã½Â¼ÂºÂµÃ‡ÃÃ¶ Â¾ÃŠÃ€Âº NPCÂ¶Ã³Â¸Ã© Â¹Â«Â½Ãƒ 
        if (((wObjectID - 10000) == 0) || ((wObjectID - 10000) >= DEF_MAXNPCS)) return;
        if (m_pNpcList[wObjectID - 10000] == 0) return;

        wp = (uint16_t *)cp;
        *wp = wObjectID;
        cp += 2;

        wObjectID -= 10000;

        sp = (short *)cp;
        sX = m_pNpcList[wObjectID]->m_sX;
        *sp = sX;
        cp += 2;
        sp = (short *)cp;
        sY = m_pNpcList[wObjectID]->m_sY;
        *sp = sY;
        cp += 2;
        sp = (short *)cp;
        *sp = m_pNpcList[wObjectID]->m_sType;
        cp += 2;
        *cp = m_pNpcList[wObjectID]->m_cDir;
        cp++;
        memcpy(cp, m_pNpcList[wObjectID]->m_cName, 5);
        cp += 5;
        sp = (short *)cp;
        *sp = m_pNpcList[wObjectID]->m_sAppr2;
        cp += 2;

        ip = (int *)cp;

        sTemp = m_pNpcList[wObjectID]->m_iStatus;
        sTemp = 0x0FFFFFFF & sTemp;

        sTemp2 = iGetNpcRelationship(wObjectID, iClientH);
        sTemp = (sTemp | (sTemp2 << 28));
        *ip = sTemp;
        cp += 4;

        if (m_pNpcList[wObjectID]->m_bIsKilled == true)
            *cp = 1;
        else *cp = 0;
        cp++;

        iRet = m_pClientList[iClientH]->send_msg(cData, 27);
    }

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

int CGame::_iGetArrowItemIndex(int iClientH)
{
    int i;

    if (m_pClientList[iClientH] == nullptr) return -1;

    for (i = 0; i < DEF_MAXITEMS; i++)
        if (m_pClientList[iClientH]->m_pItemList[i] != 0)
        {
            if ((m_pClientList[iClientH]->m_pItemList[i]->m_cItemType == DEF_ITEMTYPE_ARROW) &&
                (m_pClientList[iClientH]->m_pItemList[i]->m_dwCount > 0))
                return i;
        }

    return -1;
}

/*********************************************************************************************************************
**  int bool CGame::bCheckResistingMagicSuccess(char cAttackerDir, short sTargetH, char cTargetType, int iHitRatio) **
**  description			:: calculates if a player resists magic														**
**  last updated		:: November 20, 2004; 8:42 PM; Hypnotoad													**
**	return value		:: bool																						**
**  commentary			::	-	hero armor for target mages adds 50 magic resistance								**
**							-	10000 or more it ratio will deduct 10000 hit ratio									**
**							-	invincible tablet is 100% magic resistance											**
**********************************************************************************************************************/
bool CGame::bCheckResistingMagicSuccess(char cAttackerDir, short sTargetH, char cTargetType, int iHitRatio)
{
    double dTmp1, dTmp2, dTmp3;
    int    iTargetMagicResistRatio, iDestHitRatio, iResult;
    char   cTargetDir, cProtect;

    switch (cTargetType)
    {
        case DEF_OWNERTYPE_PLAYER:
            if (m_pClientList[sTargetH] == 0) return false;
            if (m_pMapList[m_pClientList[sTargetH]->m_cMapIndex]->m_bIsAttackEnabled == false) return false;
            if (m_pClientList[sTargetH]->m_iAdminUserLevel > 0) return true;
            if ((m_pClientList[sTargetH]->m_iStatus & 0x400000) != 0) return true;
            cTargetDir = m_pClientList[sTargetH]->m_cDir;
            iTargetMagicResistRatio = m_pClientList[sTargetH]->m_cSkillMastery[3] + m_pClientList[sTargetH]->m_iAddMR;
            if (m_pClientList[sTargetH]->m_iMag > 50)
                iTargetMagicResistRatio += (m_pClientList[sTargetH]->m_iMag - 50);
            iTargetMagicResistRatio += m_pClientList[sTargetH]->m_iAddResistMagic;
            cProtect = m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT];
            break;

        case DEF_OWNERTYPE_NPC:
            if (m_pNpcList[sTargetH] == 0) return false;
            cTargetDir = m_pNpcList[sTargetH]->m_cDir;
            iTargetMagicResistRatio = m_pNpcList[sTargetH]->m_cResistMagic;
            cProtect = m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT];
            break;
    }

    if (cProtect == 5) return true;

    if ((iHitRatio < 1000) && (cProtect == 2)) return true;
    if (iHitRatio >= 10000) iHitRatio -= 10000;
    if (iTargetMagicResistRatio < 1) iTargetMagicResistRatio = 1;
    if ((cAttackerDir != 0) && (m_pClientList[sTargetH] != 0) && (m_pClientList[sTargetH]->m_cHeroArmourBonus == 2))
    {
        iHitRatio += 50;
    }

    dTmp1 = (double)(iHitRatio);
    dTmp2 = (double)(iTargetMagicResistRatio);
    dTmp3 = (dTmp1 / dTmp2) * 50.0f;
    iDestHitRatio = (int)(dTmp3);

    if (iDestHitRatio < DEF_MINIMUMHITRATIO) iDestHitRatio = DEF_MINIMUMHITRATIO;
    if (iDestHitRatio > DEF_MAXIMUMHITRATIO) iDestHitRatio = DEF_MAXIMUMHITRATIO;
    if (iDestHitRatio >= 100) return false;

    iResult = iDice(1, 100);
    if (iResult <= iDestHitRatio) return false;

    if (cTargetType == DEF_OWNERTYPE_PLAYER)
        CalculateSSN_SkillIndex(sTargetH, 3, 1);
    return true;
}

bool CGame::bCheckResistingIceSuccess(char cAttackerDir, short sTargetH, char cTargetType, int iHitRatio)
{
    // ³Ãµ¿µÉ °ÍÀÎ°¡ÀÇ È®·ü °è»ê.
    int    iTargetIceResistRatio, iResult;

    switch (cTargetType)
    {
        case DEF_OWNERTYPE_PLAYER:
            if (m_pClientList[sTargetH] == 0) return false;
            // v1.4 ¿î¿µÀÚ¿¡ ´ëÇÑ ³Ãµ¿°ø°ÝÀº ¹«ÀÇ¹Ì 
            if (m_pClientList[sTargetH]->m_iAdminUserLevel > 0) return true;
            iTargetIceResistRatio = m_pClientList[sTargetH]->m_iAddAbsWater * 2;
            // v2.172 ÇØµ¿ Æ÷¼ÇÀ» ¸Ô°í 30ÃÊ°£Àº ¾óÁö ¾Ê´Â´Ù.
            if (m_pClientList[sTargetH]->m_dwWarmEffectTime == 0)
            {
            }
            else if ((timeGetTime() - m_pClientList[sTargetH]->m_dwWarmEffectTime) < 1000 * 30) return true;
            break;

        case DEF_OWNERTYPE_NPC:
            if (m_pNpcList[sTargetH] == 0) return false;
            iTargetIceResistRatio = (m_pNpcList[sTargetH]->m_cResistMagic) - (m_pNpcList[sTargetH]->m_cResistMagic / 3); // ¿©±â¿¡ ¾óÀ½ ¹æ¾î ¼öÄ¡ ÀÔ·Â. NPCÀÇ °æ¿ì ¸¶¹ý ÀúÇ×ÀÇ 70% ¼öÁØ 
            break;
    }

    if (iTargetIceResistRatio < 1) iTargetIceResistRatio = 1;

    iResult = iDice(1, 100);
    if (iResult <= iTargetIceResistRatio) return true;

    return false;
}

bool CGame::bSetItemToBankItem(int iClientH, class CItem * pItem)
{
    int i, iRet;
    uint32_t * dwp;
    uint16_t * wp;
    char * cp;
    short * sp;
    char cData[100];

    if (m_pClientList[iClientH] == nullptr) return false;
    if (pItem == 0) return false;
    //wh remove
    //if (m_pClientList[iClientH]->m_bIsInsideWarehouse == false) return false;

    for (i = 0; i < DEF_MAXBANKITEMS; i++)
        if (m_pClientList[iClientH]->m_pItemInBankList[i] == 0)
        {

            m_pClientList[iClientH]->m_pItemInBankList[i] = pItem;

            dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
            *dwp = MSGID_NOTIFY;
            wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
            *wp = DEF_NOTIFY_ITEMTOBANK;

            cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);

            *cp = i; // Ã€Â§Ã„Â¡ Ã€ÃºÃ€Ã¥ 
            cp++;

            // 1Â°Â³.
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

            // v1.432
            sp = (short *)cp;
            *sp = pItem->m_sItemEffectValue2;
            cp += 2;

            // v1.42
            dwp = (uint32_t *)cp;
            *dwp = pItem->m_dwAttribute;
            cp += 4;

            // Â¾Ã†Ã€ÃŒÃ…Ã› ÃÂ¤ÂºÂ¸ Ã€Ã¼Â¼Ã› 
            iRet = m_pClientList[iClientH]->send_msg(cData, 55);
            switch (iRet)
            {
                case DEF_XSOCKEVENT_QUENEFULL:
                case DEF_XSOCKEVENT_SOCKETERROR:
                case DEF_XSOCKEVENT_CRITICALERROR:
                case DEF_XSOCKEVENT_SOCKETCLOSED:
                    // Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ ÂºÂ¸Â³Â¾Â¶Â§ Â¿Â¡Â·Â¯Â°Â¡ Â¹ÃŸÂ»Ã½Ã‡ÃŸÂ´Ã™Â¸Ã© ÃÂ¦Â°Ã…Ã‡Ã‘Â´Ã™. v1.41 ÃÂ¦Â°Ã…Ã‡ÃÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™.
                    // DeleteClient(iClientH, true, true);
                    return true; // v1.41 FALSEÂ¸Â¦ Â¹ÃÃˆÂ¯Ã‡ÃÂ¸Ã© Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¹Ã™Â´ÃšÂ¿Â¡ ÂºÂ¹Â»Ã§ÂµÃˆÂ´Ã™.
            }

            return true;
        }

    // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» ÂºÂ¸Â°Ã¼Ã‡Ã’ Â¿Â©Ã€Â¯Â°Ã¸Â°Â£Ã€ÃŒ Â¾Ã¸Â´Ã™.
    return false;
}

bool CGame::bCheckTotalSkillMasteryPoints(int iClientH, int iSkill)
{
    int i;
    int iRemainPoint, iTotalPoints, iWeaponIndex, iDownSkillSSN, iDownPoint;
    short sDownSkillIndex;

    if (m_pClientList[iClientH] == nullptr) return false;

    iTotalPoints = 0;
    for (i = 0; i < DEF_MAXSKILLTYPE; i++)
        iTotalPoints += m_pClientList[iClientH]->m_cSkillMastery[i];

    iRemainPoint = iTotalPoints - DEF_MAXSKILLPOINTS;

    if (iRemainPoint > 0)
    {
        // Â½ÂºÃ…Â³Ã€Ã‡ Ã†Ã·Ã€ÃŽÃ†Â®Â°Â¡ ÃƒÃŠÂ°ÃºÂµÃ‡Â¾ÃºÂ´Ã™. ÃƒÃŠÂ°ÃºÂµÃˆ Ã†Ã·Ã€ÃŽÃ†Â® Â¸Â¸Ã…Â­ Â·Ã§Ã‡ÃÂ¸Â¦ ÂµÂ¹Â¸Ã§ SSNÃ€ÃŒ Â°Â¡Ã€Ã¥ Â³Â·Ã€Âº Â½ÂºÃ…Â³Ã€Â» Â³Â»Â¸Â°Â´Ã™.	
        while (iRemainPoint > 0)
        {

            sDownSkillIndex = -1; // v1.4
            if (m_pClientList[iClientH]->m_iDownSkillIndex != -1)
            {
                // ÃÃ¶ÃÂ¤ÂµÃˆ Â½ÂºÃ…Â³Ã€ÃŒ Ã€Ã–Â´Ã™. 
                switch (m_pClientList[iClientH]->m_iDownSkillIndex)
                {
                    case 3: // Â¸Â¶Â¹Ã½ Ã€ÃºÃ‡Ã—

                    default:
                        // Â½ÂºÃ…Â³Ã€ÃŒ 20 Ã€ÃŒÃ‡ÃÃ€Ã‡ Â»Ã³Ã…Ã‚Â¶Ã³Â¸Ã© Â±Ã—Â´Ã«Â·ÃŽ 0Ã€Â¸Â·ÃŽ Â¶Â³Â¾Ã® Â¶ÃŸÂ¸Â°Â´Ã™.
                        if (m_pClientList[iClientH]->m_cSkillMastery[m_pClientList[iClientH]->m_iDownSkillIndex] > 0)
                        {
                            sDownSkillIndex = m_pClientList[iClientH]->m_iDownSkillIndex;
                        }
                        else
                        {
                            // Â´Ã™Â¸Â¥ Â½ÂºÃ…Â³Ã€Â» Â°Ã‹Â»Ã¶Ã‡Ã‘Â´Ã™.
                            // Â´Ã™Â¸Â¥ Â½ÂºÃ…Â³Ã€Â» Â°Ã‹Â»Ã¶Ã‡Ã‘Â´Ã™.
                            iDownSkillSSN = 99999999;
                            for (i = 0; i < DEF_MAXSKILLTYPE; i++)
                                if ((m_pClientList[iClientH]->m_cSkillMastery[i] >= 21) && (i != iSkill) &&
                                    (m_pClientList[iClientH]->m_iSkillSSN[i] <= iDownSkillSSN))
                                {
                                    // V1.22 Â´Ã™Â¿Ã® Â½ÃƒÃ…Â³ Â½ÂºÃ…Â³Ã€Âº ÃƒÃ–Â¼Ã’ 20 Ã€ÃŒÃ‡ÃÂ·ÃŽÂ´Ã‚ Â¶Â³Â¾Ã®ÃÃº Â¼Ã¶ Â¾Ã¸Â´Ã™. 
                                    iDownSkillSSN = m_pClientList[iClientH]->m_iSkillSSN[i];
                                    sDownSkillIndex = i;
                                }
                        }
                        break;
                }
            }
            // Ã‡Ã¶Ã€Ã§ 1ÂºÂ¸Â´Ã™ Ã…Â« Â½ÂºÃ…Â³ ÃÃŸÂ¿Â¡Â¼Â­ Â°Â¡Ã€Ã¥ Ã€Ã›Ã€Âº SSNÃ€Â» Â°Â®Â´Ã‚ Â½ÂºÃ…Â³Ã€Âº sDownSkillIndex 

            if (sDownSkillIndex != -1)
            {

                if (m_pClientList[iClientH]->m_cSkillMastery[sDownSkillIndex] <= 20) // v1.4
                    iDownPoint = m_pClientList[iClientH]->m_cSkillMastery[sDownSkillIndex];
                else iDownPoint = 1;

                m_pClientList[iClientH]->m_cSkillMastery[sDownSkillIndex] -= iDownPoint; // v1.4
                m_pClientList[iClientH]->m_iSkillSSN[sDownSkillIndex] = m_iSkillSSNpoint[m_pClientList[iClientH]->m_cSkillMastery[sDownSkillIndex] + 1] - 1;
                iRemainPoint -= iDownPoint; // v1.4

                // Â¸Â¸Â¾Ã  Â³Â·Â¾Ã†ÃÃ¸ Â½ÂºÃ…Â³Ã€ÃŒ Ã‡Ã¶Ã€Ã§ Â»Ã§Â¿Ã«ÃÃŸÃ€ÃŽ Â¹Â«Â±Ã¢Â¿Ã Â°Ã¼Â·ÃƒÃ€ÃŒ Ã€Ã–Â´Ã™Â¸Ã© Â¸Ã­ÃÃŸÂ·Ã¼Ã€Â» Â³Â·ÃƒÃŸÂ¾Ã®Â¾ÃŸ Ã‡Ã‘Â´Ã™. 
                if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1)
                {
                    // Â¾Ã§Â¼Ã•Â¹Â«Â±Ã¢Â°Â¡ Ã€Ã¥Ã‚Ã¸ÂµÃ‡Â¾Ã® Ã€Ã–Â¾ÃºÂ´Ã™. 
                    iWeaponIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
                    if (m_pClientList[iClientH]->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sDownSkillIndex)
                    {
                        // ÃˆÂ°Â·Ã¹Ã€Ã‡ Â»Ã§Â¿Ã«Â¿Â¡ Ã€Ã‡Ã‡Ã‘ Â½ÂºÃ…Â³Ã€Ã‡ Ã‡ÃÂ¶Ã´Ã€ÃŒÂ¾ÃºÂ´Ã™. Â¸Ã­ÃÃŸÂ·Ã¼Ã€Â» Â³Â·ÃƒÃ¡Â´Ã™. 
                        m_pClientList[iClientH]->m_iHitRatio -= iDownPoint; // v1.4
                        if (m_pClientList[iClientH]->m_iHitRatio < 0) m_pClientList[iClientH]->m_iHitRatio = 0;
                    }
                }

                if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND] != -1)
                {
                    iWeaponIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
                    if (m_pClientList[iClientH]->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sDownSkillIndex)
                    {
                        m_pClientList[iClientH]->m_iHitRatio -= iDownPoint; // v1.4
                        if (m_pClientList[iClientH]->m_iHitRatio < 0) m_pClientList[iClientH]->m_iHitRatio = 0;
                    }
                }
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILL, sDownSkillIndex, m_pClientList[iClientH]->m_cSkillMastery[sDownSkillIndex], 0, 0);
            }
            else
            {
                return false;
            }
        }
        return true;
    }

    return false;
}

int CGame::iGetFollowerNumber(short sOwnerH, char cOwnerType)
{
    int i, iTotal;

    iTotal = 0;

    for (i = 1; i < DEF_MAXNPCS; i++)
        if ((m_pNpcList[i] != 0) && (m_pNpcList[i]->m_cMoveType == DEF_MOVETYPE_FOLLOW))
        {

            if ((m_pNpcList[i]->m_iFollowOwnerIndex == sOwnerH) && (m_pNpcList[i]->m_cFollowOwnerType == cOwnerType))
                iTotal++;
        }

    return iTotal;
}

/*********************************************************************************************************************
**  bool CGame::bRegisterDelayEvent(int iDelayType, int iEffectType, uint32_t dwLastTime, int iTargetH,				**
**									   char cTargetType, char cMapIndex, int dX, int dY, int iV1, int iV2, int iV3) **
**  description			:: initiates the delayed event process														**
**  last updated		:: November 20, 2004; 9:30 PM; Hypnotoad													**
**	return value		:: bool																						**
**********************************************************************************************************************/
bool CGame::bRegisterDelayEvent(int iDelayType, int iEffectType, uint32_t dwLastTime, int iTargetH, char cTargetType, char cMapIndex, int dX, int dY, int iV1, int iV2, int iV3)
{
    int i;

    for (i = 0; i < DEF_MAXDELAYEVENTS; i++)
        if (m_pDelayEventList[i] == 0)
        {
            m_pDelayEventList[i] = new class CDelayEvent;
            m_pDelayEventList[i]->m_iDelayType = iDelayType;
            m_pDelayEventList[i]->m_iEffectType = iEffectType;
            m_pDelayEventList[i]->m_cMapIndex = cMapIndex;
            m_pDelayEventList[i]->m_dX = dX;
            m_pDelayEventList[i]->m_dY = dY;
            m_pDelayEventList[i]->m_iTargetH = iTargetH;
            m_pDelayEventList[i]->m_cTargetType = cTargetType;
            m_pDelayEventList[i]->m_iV1 = iV1;
            m_pDelayEventList[i]->m_iV2 = iV2;
            m_pDelayEventList[i]->m_iV3 = iV3;
            m_pDelayEventList[i]->m_dwTriggerTime = dwLastTime;
            return true;
        }
    return false;
}

bool CGame::bRemoveFromDelayEventList(int iH, char cType, int iEffectType)
{
    int i;

    for (i = 0; i < DEF_MAXDELAYEVENTS; i++)
        if (m_pDelayEventList[i] != 0)
        {

            if (iEffectType == 0)
            {
                // Effect ÃÂ¾Â·Ã¹Â¿Â¡ Â»Ã³Â°Ã¼Â¾Ã¸Ã€ÃŒ Â¸Ã°ÂµÃŽ Â»Ã¨ÃÂ¦ 	
                if ((m_pDelayEventList[i]->m_iTargetH == iH) && (m_pDelayEventList[i]->m_cTargetType == cType))
                {
                    delete m_pDelayEventList[i];
                    m_pDelayEventList[i] = 0;
                }
            }
            else
            {
                // Ã‡Ã˜Â´Ã§ EffectÂ¸Â¸ Â»Ã¨ÃÂ¦.
                if ((m_pDelayEventList[i]->m_iTargetH == iH) && (m_pDelayEventList[i]->m_cTargetType == cType) &&
                    (m_pDelayEventList[i]->m_iEffectType == iEffectType))
                {
                    delete m_pDelayEventList[i];
                    m_pDelayEventList[i] = 0;
                }
            }
        }

    return true;
}

void CGame::SendObjectMotionRejectMsg(int iClientH)
{
    char * cp, cData[30];
    uint32_t * dwp;
    uint16_t * wp;
    short * sp;
    int     iRet;

    m_pClientList[iClientH]->m_bIsMoveBlocked = true; // v2.171

    // Ã€ÃŒÂµÂ¿Ã€ÃŒ ÂºÃ’Â°Â¡Â´Ã‰Ã‡ÃÂ´Ã™. 
    dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
    *dwp = MSGID_RESPONSE_MOTION;
    wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
    *wp = DEF_OBJECTMOTION_REJECT;

    cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);
    sp = (short *)cp;
    *sp = m_pClientList[iClientH]->m_sX;
    cp += 2;
    sp = (short *)cp;
    *sp = m_pClientList[iClientH]->m_sY;
    cp += 2;

    iRet = m_pClientList[iClientH]->send_msg(cData, 10);
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

int CGame::_iGetTotalClients()
{
    int i, iTotal;

    iTotal = 0;
    for (i = 1; i < DEF_MAXCLIENTS; i++)
        if (m_pClientList[i] != 0) iTotal++;

    return iTotal;
}

char _tmp_cCorpseX[] = { 0,  1, 1, 1, 0, -1, -1, -1, 0, 0, 0, 0 };
char _tmp_cCorpseY[] = { -1, -1, 0, 1, 1,  1,  0, -1, 0, 0, 0 };

void CGame::ClearSkillUsingStatus(int iClientH)
{
    int i;
    short tX, fX, tY, fY;

    if (m_pClientList[iClientH] == nullptr) return;

    if (m_pClientList[iClientH]->m_bSkillUsingStatus[19] == true)
    {
        tX = m_pClientList[iClientH]->m_sX;
        tY = m_pClientList[iClientH]->m_sY;
        if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bGetMoveable(tX, tY, 0) == false)
        {
            fX = m_pClientList[iClientH]->m_sX + _tmp_cCorpseX[m_pClientList[iClientH]->m_cDir];
            fY = m_pClientList[iClientH]->m_sY + _tmp_cCorpseY[m_pClientList[iClientH]->m_cDir];
            if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bGetMoveable(fX, fY, 0) == false)
            {
                m_pClientList[iClientH]->m_cDir = iDice(1, 8);
                fX = m_pClientList[iClientH]->m_sX + _tmp_cCorpseX[m_pClientList[iClientH]->m_cDir];
                fY = m_pClientList[iClientH]->m_sY + _tmp_cCorpseY[m_pClientList[iClientH]->m_cDir];
                if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bGetMoveable(fX, fY, 0) == false)
                {
                    return;
                }
            }
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_DAMAGEMOVE, m_pClientList[iClientH]->m_cDir, 0, 0, 0);
        }
    }
    for (i = 0; i < DEF_MAXSKILLTYPE; i++)
    {
        m_pClientList[iClientH]->m_bSkillUsingStatus[i] = false;
        m_pClientList[iClientH]->m_iSkillUsingTimeID[i] = 0;
    }

    if (m_pClientList[iClientH]->m_iAllocatedFish != 0)
    {
        if (m_pFish[m_pClientList[iClientH]->m_iAllocatedFish] != 0)
            m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_sEngagingCount--;

        m_pClientList[iClientH]->m_iAllocatedFish = 0;
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_FISHCANCELED, 0, 0, 0, 0);
    }

}

int CGame::iCalculateUseSkillItemEffect(int iOwnerH, char cOwnerType, char cOwnerSkill, int iSkillNum, char cMapIndex, int dX, int dY)
{
    class CItem * pItem;
    char  cItemName[21];
    short lX, lY;
    int   iResult, iFish;

    switch (cOwnerType)
    {
        case DEF_OWNERTYPE_PLAYER:
            if (m_pClientList[iOwnerH] == 0) return 0;
            if (m_pClientList[iOwnerH]->m_cMapIndex != cMapIndex) return 0;
            lX = m_pClientList[iOwnerH]->m_sX;
            lY = m_pClientList[iOwnerH]->m_sY;
            break;

        case DEF_OWNERTYPE_NPC:
            if (m_pNpcList[iOwnerH] == 0) return 0;
            if (m_pNpcList[iOwnerH]->m_cMapIndex != cMapIndex) return 0;
            lX = m_pNpcList[iOwnerH]->m_sX;
            lY = m_pNpcList[iOwnerH]->m_sY;
            break;
    }

    // Â½ÂºÃ…Â³ Â»Ã§Â¿Ã« Â¿Â©ÂºÃŽ ÃÃ–Â»Ã§Ã€Â§Â¸Â¦ Â±Â¼Â¸Â°Â´Ã™. 
    if (cOwnerSkill == 0) return 0;

    // Â½ÂºÃ…Â³Ã€ÃŒ 100Ã€ÃŒÂ¶Ã³Â°Ã­ Ã‡Ã˜ÂµÂµ Â°Â¡Â²Ã» Â³Â¬Â½ÃƒÂ¸Â¦ Â½Ã‡Ã†ÃÃ‡ÃÂ°Ã” Ã‡ÃÂ±Ã¢ Ã€Â§Ã‡Ã˜Â¼Â­ 1D105 
    iResult = iDice(1, 105);
    if (cOwnerSkill <= iResult)	return 0;  // Â½Ã‡Ã†ÃÂ´Ã™.

    // Â¶Â¥Â¿Â¡Â¼Â­Â´Ã‚ Â³Â¬Â½ÃƒÂ°Â¡ ÂºÃ’Â°Â¡Â´Ã‰ 
    if (m_pMapList[cMapIndex]->bGetIsWater(dX, dY) == false) return 0;

    // Â¼ÂºÂ°Ã¸Ã‡ÃŸÃ€Â¸Â¹Ã‡Â·ÃŽ Â½ÂºÃ…Â³ Ã„Â«Â¿Ã®Ã†Â®Â¸Â¦ Â¿ÃƒÂ¸Â°Â´Ã™.
    if (cOwnerType == DEF_OWNERTYPE_PLAYER)
        CalculateSSN_SkillIndex(iOwnerH, iSkillNum, 1);

    switch (m_pSkillConfigList[iSkillNum]->m_sType)
    {
        case DEF_SKILLEFFECTTYPE_TAMING:
            // Â±Ã¦ÂµÃ©Ã€ÃŒÂ±Ã¢ Â±Ã¢Â¼Ãº: dX, dY ÂºÃŽÂ±Ã™Ã€Ã‡ Â¸Ã³Â½ÂºÃ…ÃÂ¸Â¦ Â±Ã¦ÂµÃ©Ã€ÃŽÂ´Ã™.
            _TamingHandler(iOwnerH, iSkillNum, cMapIndex, dX, dY);
            break;

        case DEF_SKILLEFFECTTYPE_GET:
            // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â¾Ã²Â´Ã‚ Â±Ã¢Â¼ÃºÃ€ÃŒÂ¾ÃºÂ´Ã™. 
            memset(cItemName, 0, sizeof(cItemName));
            switch (m_pSkillConfigList[iSkillNum]->m_sValue1)
            {
                case 1:
                    // Â±Â¤Â¹Â° 
                    wsprintf(cItemName, "Meat");
                    break;

                case 2:
                    // Â¹Â°Â°Ã­Â±Ã¢ 
                    // Â³Â¬Â½ÃƒÃ€Ã‡ Â°Ã¦Â¿Ã¬ Ã€Â§Ã„Â¡Â¿Ã Â½ÃƒÂ°Â£Â´Ã«Ã€Ã‡ Â¿ÂµÃ‡Ã¢Â¿Â¡ ÂµÃ»Â¶Ã³ Â¶Ã‡ Â¼ÂºÂ°Ã¸Â·Ã¼Ã€ÃŒ Â´ÃžÂ¶Ã³ÃÃ¸Â´Ã™. 
                    //if (m_pMapList[cMapIndex]->bGetIsWater(dX, dY) == false) return 0; 

                    // Â±Ã™ÃƒÂ³Â¿Â¡ Â´Ã™Ã€ÃŒÂ³ÂªÂ¹Ã Â¿Ã€ÂºÃªÃÂ§Ã†Â® Â¹Â°Â°Ã­Â±Ã¢Â°Â¡ ÃÂ¸Ã€Ã§Ã‡Ã‘Â´Ã™Â¸Ã© ÂºÂ»Â°ÃÂ³Â¬Â½Ãƒ Â¸Ã°ÂµÃ¥Â·ÃŽ ÂµÃ©Â¾Ã®Â°Â£Â´Ã™.
                    if (cOwnerType == DEF_OWNERTYPE_PLAYER)
                    {
                        iFish = iCheckFish(iOwnerH, cMapIndex, dX, dY);
                        if (iFish == 0) wsprintf(cItemName, "Fish");
                    }
                    else wsprintf(cItemName, "Fish");
                    break;
            }

            if (strlen(cItemName) != 0)
            {

                // Â³Â¬Â½ÃƒÂ¿Â¡ Â¼ÂºÂ°Ã¸Ã‡ÃŸÂ´Ã™Â¸Ã© Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ Ã€Ã¼Â¼Ã›.
                if (memcmp(cItemName, "Fish", 6) == 0)
                {
                    SendNotifyMsg(0, iOwnerH, DEF_NOTIFY_FISHSUCCESS, 0, 0, 0, 0);
                    // v1.41 Â¾Ã Â°Â£Ã€Ã‡ Â°Ã¦Ã‡Ã¨Ã„Â¡ Â»Ã³Â½Ã‚ 
                    m_pClientList[iOwnerH]->m_iExpStock += iDice(1, 2);
                }

                pItem = new class CItem;
                if (pItem == 0) return 0;
                if (_bInitItemAttr(pItem, cItemName) == true)
                {
                    // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â³ÃµÂ´Ã‚Â´Ã™. 
                    m_pMapList[cMapIndex]->bSetItem(lX, lY, pItem);

                    // Â´Ã™Â¸Â¥ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¿Â¡Â°Ã” Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¶Â³Â¾Ã®ÃÃ¸ Â°ÃÃ€Â» Â¾Ã‹Â¸Â°Â´Ã™. 
                    SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, cMapIndex,
                        lX, lY, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); //v1.4
                }
            }
            break;
    }

    return 1;
}

int CGame::iCalcTotalWeight(int iClientH)
{
    int i, iWeight;
    short sItemIndex;

    if (m_pClientList[iClientH] == nullptr) return 0;

    m_pClientList[iClientH]->m_iAlterItemDropIndex = -1;
    // Ã‚Ã¸Â¿Ã«Ã‡ÃÃÃ¶ Â¾ÃŠÂ¾Ã†ÂµÂµ ÃˆÂ¿Â°ÃºÂ°Â¡ Ã€Ã–Â´Ã‚ Â¾Ã†Ã€ÃŒÃ…Ã› ÃˆÂ¿Â°Ãº. Â¼Ã’ÃÃ¶Ã‡ÃÂ°Ã­ Ã€Ã–Â¾Ã®ÂµÂµ ÃˆÂ¿Â°ÃºÂ°Â¡ Ã€Ã–Â±Ã¢ Â¶Â§Â¹Â®Â¿Â¡ Â¿Â©Â±Ã¢Â¼Â­ Â°Ã‹Â»Ã§.
    for (sItemIndex = 0; sItemIndex < DEF_MAXITEMS; sItemIndex++)
        if (m_pClientList[iClientH]->m_pItemList[sItemIndex] != 0)
        {
            switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType)
            {
                case DEF_ITEMEFFECTTYPE_ALTERITEMDROP:
                    if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan > 0)
                    {
                        // Â¼Ã¶Â¸Ã­Ã€ÃŒ Ã€Ã–Â¾Ã®Â¾ÃŸ ÃˆÂ¿Â°ÃºÂ°Â¡ Ã€Ã–Â´Ã™.
                        m_pClientList[iClientH]->m_iAlterItemDropIndex = sItemIndex;
                    }
                    break;
            }
        }

    iWeight = 0;
    for (i = 0; i < DEF_MAXITEMS; i++)
        if (m_pClientList[iClientH]->m_pItemList[i] != 0)
        {

            iWeight += iGetItemWeight(m_pClientList[iClientH]->m_pItemList[i], m_pClientList[iClientH]->m_pItemList[i]->m_dwCount);
        }

    m_pClientList[iClientH]->m_iCurWeightLoad = iWeight;

    return iWeight;
}

void CGame::CheckAndNotifyPlayerConnection(int iClientH, char * pMsg, uint32_t dwSize)
{
    char   seps[] = "= \t\n";
    char * token, * cp, cName[11], cBuff[256], cPlayerLocation[120];
    class  CStrTok * pStrTok;
    int i;
    uint16_t * wp;

    if (m_pClientList[iClientH] == nullptr) return;
    if (dwSize <= 0) return;

    ZeroMemory(cPlayerLocation, sizeof(cPlayerLocation));
    memset(cName, 0, sizeof(cName));
    memset(cBuff, 0, sizeof(cBuff));
    memcpy(cBuff, pMsg, dwSize);

    pStrTok = new class CStrTok(cBuff, seps);
    token = pStrTok->pGet();
    token = pStrTok->pGet();

    if (token == 0)
    {
        delete pStrTok;
        return;
    }

    if (strlen(token) > 10)
        memcpy(cName, token, 10);
    else memcpy(cName, token, strlen(token));

    // cNameÃ€Ã‡ Ã€ÃŒÂ¸Â§Ã€Â» Â°Â¡ÃÃ¸ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â°Â¡ ÃÂ¢Â¼Ã“ÃÃŸÃ€ÃŽÃÃ¶ ÃƒÂ£Â´Ã‚Â´Ã™. 
    for (i = 1; i < DEF_MAXCLIENTS; i++)
        if ((m_pClientList[i] != 0) && (memcmp(cName, m_pClientList[i]->m_cCharName, 10) == 0))
        {
            // Â°Â°Ã€Âº Ã€ÃŒÂ¸Â§Ã€Â» Â°Â¡ÃÃ¸ Ã„Â³Â¸Â¯Ã…ÃÂ¸Â¦ ÃƒÂ£Â¾Ã’Â´Ã™. 
            if (m_pClientList[iClientH]->m_iAdminUserLevel > 0)
            { // GM's get more info
                cp = (char *)cPlayerLocation;

                memcpy(cp, m_pClientList[i]->m_cMapName, 10);
                cp += 10;

                wp = (uint16_t *)cp;
                *wp = m_pClientList[i]->m_sX;
                cp += 2;

                wp = (uint16_t *)cp;
                *wp = m_pClientList[i]->m_sY;
                cp += 2;
            }
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_PLAYERONGAME, 0, 0, 0, m_pClientList[i]->m_cCharName, 0, 0, 0, 0, 0, 0, cPlayerLocation);

            delete pStrTok;
            return;
        }

    memset(cBuff, 0, sizeof(cBuff));
    cp = (char *)cBuff;
    *cp = GSM_REQUEST_FINDCHARACTER;
    cp++;

    wp = (uint16_t *)cp;
    *wp = m_wServerID_GSS;
    cp += 2;

    wp = (uint16_t *)cp;
    *wp = iClientH;
    cp += 2;

    memcpy(cp, cName, 10);
    cp += 10;

    memcpy(cp, m_pClientList[iClientH]->m_cCharName, 10);
    cp += 10;

    bStockMsgToGateServer(cBuff, 25);

    delete pStrTok;
}

void CGame::ToggleWhisperPlayer(int iClientH, char * pMsg, uint32_t dwMsgSize)
{
    char   seps[] = "= \t\n";
    char * token, cName[11], cBuff[256];
    class  CStrTok * pStrTok;
    int i;
    char * cp;
    uint16_t * wp;

    if (m_pClientList[iClientH] == nullptr) return;
    if (dwMsgSize <= 0) return;

    memset(cName, 0, sizeof(cName));
    memset(cBuff, 0, sizeof(cBuff));
    memcpy(cBuff, pMsg, dwMsgSize);

    pStrTok = new class CStrTok(cBuff, seps);
    token = pStrTok->pGet();
    token = pStrTok->pGet();

    if (token == 0)
    {
        // Â±Ã“Â¼Ã“Â¸Â» Â»Ã³Â´Ã«Â°Â¡ ÃÃ¶ÃÂ¤ÂµÃ‡ÃÃ¶ Â¾ÃŠÂ¾Ã’Â´Ã™. Â±Ã“Â¼Ã“Â¸Â» Â¸Ã°ÂµÃ¥Â¸Â¦ Ã‡Ã˜ÃÂ¦Ã‡Ã‘Â´Ã™. 
        m_pClientList[iClientH]->m_iWhisperPlayerIndex = -1;
        memset(m_pClientList[iClientH]->m_cWhisperPlayerName, 0, sizeof(m_pClientList[iClientH]->m_cWhisperPlayerName));
        m_pClientList[iClientH]->m_bIsCheckingWhisperPlayer = false;
        // Â±Ã“Â¼Ã“Â¸Â» Â»Ã³Ã…Ã‚Â°Â¡ Ã‡Ã˜ÃÂ¦ÂµÃ‡Â¾ÃºÃ€Â½Ã€Â» Ã…Ã«ÂºÂ¸. 
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_WHISPERMODEOFF, 0, 0, 0, cName);
    }
    else
    {
        if (strlen(token) > 10)
            memcpy(cName, token, 10);
        else memcpy(cName, token, strlen(token));

        m_pClientList[iClientH]->m_iWhisperPlayerIndex = -1;

        for (i = 1; i < DEF_MAXCLIENTS; i++)
            if ((m_pClientList[i] != 0) && (memcmp(m_pClientList[i]->m_cCharName, cName, 10) == 0))
            {
                // Â°Â°Ã€Âº Ã€ÃŒÂ¸Â§Ã€Â» Â°Â¡ÃÃ¸ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â¸Â¦ ÃƒÂ£Â¾Ã’Â´Ã™. 
                // Ã€ÃšÂ±Ã¢ Ã€ÃšÂ½Ã…Ã€ÃŒÂ¶Ã³Â¸Ã© Ã‡Ã’Â´Ã§Ã‡ÃÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™.
                if (i == iClientH)
                {
                    delete pStrTok;
                    return;
                }
                //Ã€ÃŽÂµÂ¦Â½ÂºÂ¸Â¦ Ã‡Ã’Â´Ã§ 
                m_pClientList[iClientH]->m_iWhisperPlayerIndex = i;
                memset(m_pClientList[iClientH]->m_cWhisperPlayerName, 0, sizeof(m_pClientList[iClientH]->m_cWhisperPlayerName));
                strcpy(m_pClientList[iClientH]->m_cWhisperPlayerName, cName);
                break;
            }

        if (m_pClientList[iClientH]->m_iWhisperPlayerIndex == -1)
        {
            memset(cBuff, 0, sizeof(cBuff));
            cp = (char *)cBuff;
            *cp = GSM_REQUEST_FINDCHARACTER;
            cp++;

            wp = (uint16_t *)cp;
            *wp = m_wServerID_GSS;
            cp += 2;

            wp = (uint16_t *)cp;
            *wp = iClientH;
            cp += 2;

            memcpy(cp, cName, 10);
            cp += 10;

            memcpy(cp, m_pClientList[iClientH]->m_cCharName, 10);
            cp += 10;

            bStockMsgToGateServer(cBuff, 25);

            memset(m_pClientList[iClientH]->m_cWhisperPlayerName, 0, sizeof(m_pClientList[iClientH]->m_cWhisperPlayerName));
            strcpy(m_pClientList[iClientH]->m_cWhisperPlayerName, cName);
            m_pClientList[iClientH]->m_bIsCheckingWhisperPlayer = true;
        }
        else
        {
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_WHISPERMODEON, 0, 0, 0, m_pClientList[iClientH]->m_cWhisperPlayerName);
        }
    }

    delete pStrTok;
}

void CGame::SetPlayerProfile(int iClientH, char * pMsg, uint32_t dwMsgSize)
{
    char cTemp[256];
    int i;


    if (m_pClientList[iClientH] == nullptr) return;
    if ((dwMsgSize - 7) <= 0) return;

    memset(cTemp, 0, sizeof(cTemp));
    memcpy(cTemp, (pMsg + 7), dwMsgSize - 7);

    // Â°Ã¸Â¹Ã©Â¶ÃµÃ€Â» Â¾Ã°Â´ÃµÂ¹Ã™Â·ÃŽ Â¹Ã™Â²Ã›Â´Ã™. 
    for (i = 0; i < 256; i++)
        if (cTemp[i] == ' ') cTemp[i] = '_';

    // Â¿Â¡Â·Â¯ Â¹Ã¦ÃÃ¶ Ã„ÃšÂµÃ¥ 
    cTemp[255] = 0;

    memset(m_pClientList[iClientH]->m_cProfile, 0, sizeof(m_pClientList[iClientH]->m_cProfile));
    strcpy(m_pClientList[iClientH]->m_cProfile, cTemp);
}

void CGame::GetPlayerProfile(int iClientH, char * pMsg, uint32_t dwMsgSize)
{
    char   seps[] = "= \t\n";
    char * token, cName[11], cBuff[256], cBuff2[500];
    class  CStrTok * pStrTok;
    int i;

    if (m_pClientList[iClientH] == nullptr) return;
    if ((dwMsgSize) <= 0) return;

    memset(cName, 0, sizeof(cName));
    memset(cBuff, 0, sizeof(cBuff));
    memcpy(cBuff, pMsg, dwMsgSize);

    pStrTok = new class CStrTok(cBuff, seps);
    token = pStrTok->pGet();
    token = pStrTok->pGet();

    if (token != 0)
    {
        // tokenÃ€ÃŒ Â°Ã° Ã‡ÃÂ·ÃŽÃ‡ÃŠÃ€Â» Â¾Ã²Â°Ã­Ã€Ãš Ã‡ÃÂ´Ã‚ Â»Ã§Â¿Ã«Ã€Ãš Ã€ÃŒÂ¸Â§ 
        if (strlen(token) > 10)
            memcpy(cName, token, 10);
        else memcpy(cName, token, strlen(token));

        for (i = 1; i < DEF_MAXCLIENTS; i++)
            if ((m_pClientList[i] != 0) && (memcmp(m_pClientList[i]->m_cCharName, cName, 10) == 0))
            {

                memset(cBuff2, 0, sizeof(cBuff2));
                wsprintf(cBuff2, "%s Profile: %s", cName, m_pClientList[i]->m_cProfile);
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_PLAYERPROFILE, 0, 0, 0, cBuff2);

                delete pStrTok;
                return;
            }
        // Ã‡Ã¶Ã€Ã§ ÃÂ¢Â¼Ã“ÃÃŸÃ€ÃŒ Â¾Ã†Â´ÃÂ´Ã™.
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_PLAYERNOTONGAME, 0, 0, 0, cName);
    }

    delete pStrTok;
    return;
}

void CGame::___RestorePlayerCharacteristics(int iClientH)
{
    int iStr, iDex, iInt, iVit, iMag, iCharisma;
    int iOriginalPoint, iCurPoint, iVerifyPoint, iToBeRestoredPoint;
    int iMax, iA, iB;
    bool bFlag;
    char cTxt[120];
    return;
    if (m_pClientList[iClientH] == nullptr) return;

    // Â¸Ã•Ã€Ãº Â°ÂªÃ€Â» Â¹Ã©Â¾Ã·Ã‡Ã‘Â´Ã™. 
    iStr = m_pClientList[iClientH]->m_iStr;
    iDex = m_pClientList[iClientH]->m_iDex;
    iInt = m_pClientList[iClientH]->m_iInt;
    iVit = m_pClientList[iClientH]->m_iVit;
    iMag = m_pClientList[iClientH]->m_iMag;
    iCharisma = m_pClientList[iClientH]->m_iCharisma;


    iCurPoint = m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iInt +
        m_pClientList[iClientH]->m_iVit + m_pClientList[iClientH]->m_iDex +
        m_pClientList[iClientH]->m_iMag + m_pClientList[iClientH]->m_iCharisma;

    iOriginalPoint = (m_pClientList[iClientH]->m_iLevel - 1) * 3 + 70;

    iToBeRestoredPoint = iOriginalPoint - iCurPoint;

    // ÂºÂ¹Â±Â¸Ã‡Ã’ Ã‡ÃŠÂ¿Ã¤Â°Â¡ Â¾Ã¸Â´Ã™Â¸Ã© Â±ÃÃˆÂ¯.
    if (iToBeRestoredPoint == 0) return;

    if (iToBeRestoredPoint > 0)
    {
        // Ã€ÃŒÃÂ¦ iToBeRestoredPoint Â¸Â¸Ã…Â­Ã€Ã‡ Ã†Ã·Ã€ÃŽÃ†Â®Â¸Â¦ ÂºÂ¹Â±Â¸Ã‡Ã‘Â´Ã™. 
        // Â¸Ã•Ã€Ãº 10 Ã€ÃŒÃ‡ÃÃ€Ã‡ Ã†Ã·Ã€ÃŽÃ†Â®Â°Â¡ Ã€Ã–Â´Ã™Â¸Ã© Â¿Ã¬Â¼Â±Ã€Ã»Ã€Â¸Â·ÃŽ ÃƒÂ¤Â¿Ã®Â´Ã™. 
        while (1)
        {
            bFlag = false;

            if ((iToBeRestoredPoint > 0) && (m_pClientList[iClientH]->m_iStr < 10))
            {
                m_pClientList[iClientH]->m_iStr++;
                iToBeRestoredPoint--;
                bFlag = true;
            }
            if ((iToBeRestoredPoint > 0) && (m_pClientList[iClientH]->m_iMag < 10))
            {
                m_pClientList[iClientH]->m_iMag++;
                iToBeRestoredPoint--;
                bFlag = true;
            }
            if ((iToBeRestoredPoint > 0) && (m_pClientList[iClientH]->m_iInt < 10))
            {
                m_pClientList[iClientH]->m_iInt++;
                iToBeRestoredPoint--;
                bFlag = true;
            }
            if ((iToBeRestoredPoint > 0) && (m_pClientList[iClientH]->m_iDex < 10))
            {
                m_pClientList[iClientH]->m_iDex++;
                iToBeRestoredPoint--;
                bFlag = true;
            }
            if ((iToBeRestoredPoint > 0) && (m_pClientList[iClientH]->m_iVit < 10))
            {
                m_pClientList[iClientH]->m_iVit++;
                iToBeRestoredPoint--;
                bFlag = true;
            }
            if ((iToBeRestoredPoint > 0) && (m_pClientList[iClientH]->m_iCharisma < 10))
            {
                m_pClientList[iClientH]->m_iCharisma++;
                iToBeRestoredPoint--;
                bFlag = true;
            }

            if (bFlag == false)          break;
            if (iToBeRestoredPoint <= 0) break;
        }

        // Â¸Ã‡Â¼Ã• Â°ÃÃ…Ãµ Â½ÂºÃ…Â³Ã€Ã‡ ÃƒÃ–Â´Ã«Ã„Â¡Â´Ã‚ iMax, StrÃ€ÃŒ iMax/2ÂºÂ¸Â´Ã™ Â³Â·Â´Ã™Â¸Ã© Â±Ã—Â¸Â¸Ã…Â­ Â¿ÃƒÂ¸Â°Â´Ã™. 
        iMax = m_pClientList[iClientH]->m_cSkillMastery[5];

        if (m_pClientList[iClientH]->m_iStr < (iMax / 2))
        {

            while (1)
            {
                if ((iToBeRestoredPoint > 0) && (m_pClientList[iClientH]->m_iStr < (iMax / 2)))
                {
                    m_pClientList[iClientH]->m_iStr++;
                    iToBeRestoredPoint--;
                }

                if (m_pClientList[iClientH]->m_iStr == (iMax / 2)) break;
                if (iToBeRestoredPoint <= 0) break;
            }
        }

        // Â¹Â«Â±Ã¢ Â»Ã§Â¿Ã« Â½ÂºÃ…Â³Ã€Ã‡ ÃƒÃ–Â´Ã«Ã„Â¡Â´Ã‚ iMax, DexÂ°Â¡ iMax/2ÂºÂ¸Â´Ã™ Â³Â·Â´Ã™Â¸Ã© Â±Ã—Â¸Â¸Ã…Â­ Â¿ÃƒÂ¸Â°Â´Ã™. 
        iA = m_pClientList[iClientH]->m_cSkillMastery[7];
        iB = m_pClientList[iClientH]->m_cSkillMastery[8];
        if (iA > iB)
            iMax = iA;
        else iMax = iB;
        iA = m_pClientList[iClientH]->m_cSkillMastery[9];
        if (iA > iMax) iMax = iA;
        iA = m_pClientList[iClientH]->m_cSkillMastery[6];
        if (iA > iMax) iMax = iA;


        if (m_pClientList[iClientH]->m_iDex < (iMax / 2))
        {

            while (1)
            {
                if ((iToBeRestoredPoint > 0) && (m_pClientList[iClientH]->m_iDex < (iMax / 2)))
                {
                    m_pClientList[iClientH]->m_iDex++;
                    iToBeRestoredPoint--;
                }

                if (m_pClientList[iClientH]->m_iDex == (iMax / 2)) break;
                if (iToBeRestoredPoint <= 0) break;
            }
        }

        // ÃÃ—Ã€ÂºÃƒÂ´Ã‡ÃÂ±Ã¢ Â½ÂºÃ…Â³Ã€Ã‡ ÃƒÃ–Â´Ã«Ã„Â¡Â´Ã‚ iMax, IntÃ€ÃŒ iMax/2ÂºÂ¸Â´Ã™ Â³Â·Â´Ã™Â¸Ã© Â±Ã—Â¸Â¸Ã…Â­ Â¿ÃƒÂ¸Â°Â´Ã™. 
        iMax = m_pClientList[iClientH]->m_cSkillMastery[19];

        if (m_pClientList[iClientH]->m_iInt < (iMax / 2))
        {

            while (1)
            {
                if ((iToBeRestoredPoint > 0) && (m_pClientList[iClientH]->m_iInt < (iMax / 2)))
                {
                    m_pClientList[iClientH]->m_iInt++;
                    iToBeRestoredPoint--;
                }

                if (m_pClientList[iClientH]->m_iInt == (iMax / 2)) break;
                if (iToBeRestoredPoint <= 0) break;
            }
        }

        // Â¸Â¶Â¹Ã½ Â½ÂºÃ…Â³Ã€Ã‡ ÃƒÃ–Â´Ã«Ã„Â¡Â´Ã‚ iMax, MagÃ€ÃŒ iMax/2ÂºÂ¸Â´Ã™ Â³Â·Â´Ã™Â¸Ã© Â±Ã—Â¸Â¸Ã…Â­ Â¿ÃƒÂ¸Â°Â´Ã™. 
        iA = m_pClientList[iClientH]->m_cSkillMastery[3];
        iB = m_pClientList[iClientH]->m_cSkillMastery[4];
        if (iA > iB)
            iMax = iA;
        else iMax = iB;

        if (m_pClientList[iClientH]->m_iMag < (iMax / 2))
        {

            while (1)
            {
                if ((iToBeRestoredPoint > 0) && (m_pClientList[iClientH]->m_iMag < (iMax / 2)))
                {
                    m_pClientList[iClientH]->m_iMag++;
                    iToBeRestoredPoint--;
                }

                if (m_pClientList[iClientH]->m_iMag == (iMax / 2)) break;
                if (iToBeRestoredPoint <= 0) break;
            }
        }

        // Â³Â²Ã€Âº Ã†Ã·Ã€ÃŽÃ†Â®Â¸Â¦ Â·Â£Â´Ã½Ã‡ÃÂ°Ã” Â¿ÃƒÂ¸Â°Â´Ã™.
        while (iToBeRestoredPoint != 0)
        {
            switch (iDice(1, 6))
            {
                case 1:
                    if (m_pClientList[iClientH]->m_iStr < DEF_CHARPOINTLIMIT)
                    {
                        m_pClientList[iClientH]->m_iStr++;
                        iToBeRestoredPoint--;
                    }
                    break;
                case 2:
                    if (m_pClientList[iClientH]->m_iVit < DEF_CHARPOINTLIMIT)
                    {
                        m_pClientList[iClientH]->m_iVit++;
                        iToBeRestoredPoint--;
                    }
                    break;
                case 3:
                    if (m_pClientList[iClientH]->m_iDex < DEF_CHARPOINTLIMIT)
                    {
                        m_pClientList[iClientH]->m_iDex++;
                        iToBeRestoredPoint--;
                    }
                    break;
                case 4:
                    if (m_pClientList[iClientH]->m_iMag < DEF_CHARPOINTLIMIT)
                    {
                        m_pClientList[iClientH]->m_iMag++;
                        iToBeRestoredPoint--;
                    }
                    break;
                case 5:
                    if (m_pClientList[iClientH]->m_iInt < DEF_CHARPOINTLIMIT)
                    {
                        m_pClientList[iClientH]->m_iInt++;
                        iToBeRestoredPoint--;
                    }
                    break;
                case 6:
                    if (m_pClientList[iClientH]->m_iCharisma < DEF_CHARPOINTLIMIT)
                    {
                        m_pClientList[iClientH]->m_iCharisma++;
                        iToBeRestoredPoint--;
                    }
                    break;
            }
        }

        // ÂºÂ¹Â±Â¸Â°Â¡ Â¼ÂºÂ°Ã¸Ã€Ã»Ã€Â¸Â·ÃŽ ÂµÃ‡Â¾ÃºÂ´Ã‚ÃÃ¶ ÃˆÂ®Ã€ÃŽÃ‡Ã‘Â´Ã™. 
        iVerifyPoint = m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iInt +
            m_pClientList[iClientH]->m_iVit + m_pClientList[iClientH]->m_iDex +
            m_pClientList[iClientH]->m_iMag + m_pClientList[iClientH]->m_iCharisma;

        if (iVerifyPoint != iOriginalPoint)
        {
            wsprintf(cTxt, "(T_T) RestorePlayerCharacteristics(Minor) FAIL! Player(%s)-(%d/%d)", m_pClientList[iClientH]->m_cCharName, iVerifyPoint, iOriginalPoint);
            log->info(cTxt);

            // Â¿Â¡Â·Â¯Â´Ã™. Ã€ÃŒÃ€Ã¼ Â°ÂªÃ€Â¸Â·ÃŽ ÂºÂ¹Â±Â¸.
            m_pClientList[iClientH]->m_iStr = iStr;
            m_pClientList[iClientH]->m_iDex = iDex;
            m_pClientList[iClientH]->m_iInt = iInt;
            m_pClientList[iClientH]->m_iVit = iVit;
            m_pClientList[iClientH]->m_iMag = iMag;
            m_pClientList[iClientH]->m_iCharisma = iCharisma;
        }
        else
        {
            wsprintf(cTxt, "(^o^) RestorePlayerCharacteristics(Minor) SUCCESS! Player(%s)-(%d/%d)", m_pClientList[iClientH]->m_cCharName, iVerifyPoint, iOriginalPoint);
            log->info(cTxt);
        }
    }
    else
    {
        // Ã‡Ã˜Ã…Â·Ã€ÃŒÂ³Âª Â¿Â¡Â·Â¯Â¿Â¡ Ã€Ã‡Ã‡Ã˜Â¼Â­ Ã†Â¯Â¼ÂºÃ„Â¡Â°Â¡ Â¿Ã€Â¹Ã¶ÂµÃˆ Â°Ã¦Â¿Ã¬Ã€ÃŒÂ´Ã™. Â¿Ã€Â¹Ã¶ÂµÃˆ Ã†Ã·Ã€ÃŽÃ†Â®Â¸Â¸Ã…Â­ Â»Â«Â´Ã™. iToBeRestoredPointÂ°Â¡ Â¸Â¶Ã€ÃŒÂ³ÃŠÂ½ÂºÂ»Ã³Ã…Ã‚! 

        // Â¸Ã•Ã€Ãº Ã†Â¯Â¼ÂºÃ„Â¡ ÃÂ¦Ã‡Ã‘Ã„Â¡Â¸Â¦ Â¿Ã€Â¹Ã¶Ã‡Ã‘ Â°ÂªÃ€Â» Â»Â«Â´Ã™.
        while (1)
        {
            bFlag = false;
            if (m_pClientList[iClientH]->m_iStr > DEF_CHARPOINTLIMIT)
            {
                bFlag = true;
                m_pClientList[iClientH]->m_iStr--;
                iToBeRestoredPoint++;
            }

            if (m_pClientList[iClientH]->m_iDex > DEF_CHARPOINTLIMIT)
            {
                bFlag = true;
                m_pClientList[iClientH]->m_iDex--;
                iToBeRestoredPoint++;
            }

            if (m_pClientList[iClientH]->m_iVit > DEF_CHARPOINTLIMIT)
            {
                bFlag = true;
                m_pClientList[iClientH]->m_iVit--;
                iToBeRestoredPoint++;
            }

            if (m_pClientList[iClientH]->m_iInt > DEF_CHARPOINTLIMIT)
            {
                bFlag = true;
                m_pClientList[iClientH]->m_iInt--;
                iToBeRestoredPoint++;
            }

            if (m_pClientList[iClientH]->m_iMag > DEF_CHARPOINTLIMIT)
            {
                bFlag = true;
                m_pClientList[iClientH]->m_iMag--;
                iToBeRestoredPoint++;
            }

            if (m_pClientList[iClientH]->m_iCharisma > DEF_CHARPOINTLIMIT)
            {
                bFlag = true;
                m_pClientList[iClientH]->m_iCharisma--;
                iToBeRestoredPoint++;
            }

            if (bFlag == false)	break;
            if (iToBeRestoredPoint >= 0) break;
        }

        if (iToBeRestoredPoint < 0)
        {
            // Â³Â²Ã€Âº Ã†Ã·Ã€ÃŽÃ†Â® Â¸Â¸Ã…Â­ Â·Â£Â´Ã½Ã‡ÃÂ°Ã” Â¼Â±Ã…ÃƒÂµÃˆ Ã†Â¯Â¼ÂºÃ„Â¡Ã€Ã‡ Â°ÂªÃ€Â» Â³Â»Â¸Â°Â´Ã™.
            while (iToBeRestoredPoint != 0)
            {
                switch (iDice(1, 6))
                {
                    case 1:
                        if (m_pClientList[iClientH]->m_iStr > 10)
                        {
                            m_pClientList[iClientH]->m_iStr--;
                            iToBeRestoredPoint++;
                        }
                        break;
                    case 2:
                        if (m_pClientList[iClientH]->m_iVit > 10)
                        {
                            m_pClientList[iClientH]->m_iVit--;
                            iToBeRestoredPoint++;
                        }
                        break;
                    case 3:
                        if (m_pClientList[iClientH]->m_iDex > 10)
                        {
                            m_pClientList[iClientH]->m_iDex--;
                            iToBeRestoredPoint++;
                        }
                        break;
                    case 4:
                        if (m_pClientList[iClientH]->m_iMag > 10)
                        {
                            m_pClientList[iClientH]->m_iMag--;
                            iToBeRestoredPoint++;
                        }
                        break;
                    case 5:
                        if (m_pClientList[iClientH]->m_iInt > 10)
                        {
                            m_pClientList[iClientH]->m_iInt--;
                            iToBeRestoredPoint++;
                        }
                        break;
                    case 6:
                        if (m_pClientList[iClientH]->m_iCharisma > 10)
                        {
                            m_pClientList[iClientH]->m_iCharisma--;
                            iToBeRestoredPoint++;
                        }
                        break;
                }
            }
        }
        else
        {
            // Â°Ã¨Â»ÃªÃ€Â» Ã‡ÃŸÂ´ÃµÂ´Ã Â´Ã™Â½Ãƒ Â°ÂªÃ€ÃŒ Â¿Ã€Â¹Ã¶ÂµÃ‡Â¾ÃºÂ´Ã™. Ã€ÃŒÂ·Â²Â¼Ã¶Â°Â¡ Ã€Ã–Ã€Â»Â±Ã®?
            while (iToBeRestoredPoint != 0)
            {
                switch (iDice(1, 6))
                {
                    case 1:
                        if (m_pClientList[iClientH]->m_iStr < DEF_CHARPOINTLIMIT)
                        {
                            m_pClientList[iClientH]->m_iStr++;
                            iToBeRestoredPoint--;
                        }
                        break;
                    case 2:
                        if (m_pClientList[iClientH]->m_iVit < DEF_CHARPOINTLIMIT)
                        {
                            m_pClientList[iClientH]->m_iVit++;
                            iToBeRestoredPoint--;
                        }
                        break;
                    case 3:
                        if (m_pClientList[iClientH]->m_iDex < DEF_CHARPOINTLIMIT)
                        {
                            m_pClientList[iClientH]->m_iDex++;
                            iToBeRestoredPoint--;
                        }
                        break;
                    case 4:
                        if (m_pClientList[iClientH]->m_iMag < DEF_CHARPOINTLIMIT)
                        {
                            m_pClientList[iClientH]->m_iMag++;
                            iToBeRestoredPoint--;
                        }
                        break;
                    case 5:
                        if (m_pClientList[iClientH]->m_iInt < DEF_CHARPOINTLIMIT)
                        {
                            m_pClientList[iClientH]->m_iInt++;
                            iToBeRestoredPoint--;
                        }
                        break;
                    case 6:
                        if (m_pClientList[iClientH]->m_iCharisma < DEF_CHARPOINTLIMIT)
                        {
                            m_pClientList[iClientH]->m_iCharisma++;
                            iToBeRestoredPoint--;
                        }
                        break;
                }
            }
        }

        // ÂºÂ¹Â±Â¸Â°Â¡ Â¼ÂºÂ°Ã¸Ã€Ã»Ã€Â¸Â·ÃŽ ÂµÃ‡Â¾ÃºÂ´Ã‚ÃÃ¶ ÃˆÂ®Ã€ÃŽÃ‡Ã‘Â´Ã™. 
        iVerifyPoint = m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iInt +
            m_pClientList[iClientH]->m_iVit + m_pClientList[iClientH]->m_iDex +
            m_pClientList[iClientH]->m_iMag + m_pClientList[iClientH]->m_iCharisma;

        if (iVerifyPoint != iOriginalPoint)
        {
            wsprintf(cTxt, "(T_T) RestorePlayerCharacteristics(Over) FAIL! Player(%s)-(%d/%d)", m_pClientList[iClientH]->m_cCharName, iVerifyPoint, iOriginalPoint);
            log->info(cTxt);

        }
        else
        {
            wsprintf(cTxt, "(^o^) RestorePlayerCharacteristics(Over) SUCCESS! Player(%s)-(%d/%d)", m_pClientList[iClientH]->m_cCharName, iVerifyPoint, iOriginalPoint);
            log->info(cTxt);
        }
    }
}

void CGame::CalcTotalItemEffect(int iClientH, int iEquipItemID, bool bNotify)
{
    short sItemIndex;
    int  i, iArrowIndex, iPrevSAType, iTemp;
    char cEquipPos;
    double dV1, dV2, dV3;
    uint32_t  dwSWEType, dwSWEValue;
    short  sTemp;

    if (m_pClientList[iClientH] == nullptr) return;

    if ((m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND] != -1) &&
        (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1))
    {

        // Àß¸øµÈ ¹«±â ÀåÂø Á¶ÇÕÀÌ´Ù. µÑ Áß ÇÏ³ª¸¦ ³»·Á ³õ´Â´Ù. 
        if (m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND]] != 0)
        {
            // ÇÑ¼Õ °ËÀÇ ÀåÂø »óÅÂ¸¦ ÇØÁ¦ÇÑ´Ù. 
            m_pClientList[iClientH]->m_bIsItemEquipped[m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND]] = false;
            m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND] = -1;
        }
    }

    m_pClientList[iClientH]->m_cAttackDiceThrow_SM = 0;
    m_pClientList[iClientH]->m_cAttackDiceRange_SM = 0;
    m_pClientList[iClientH]->m_cAttackBonus_SM = 0;

    m_pClientList[iClientH]->m_cAttackDiceThrow_L = 0;
    m_pClientList[iClientH]->m_cAttackDiceRange_L = 0;
    m_pClientList[iClientH]->m_cAttackBonus_L = 0;

    m_pClientList[iClientH]->m_iHitRatio = 0;
    m_pClientList[iClientH]->m_iDefenseRatio = m_pClientList[iClientH]->m_iDex * 2;
    m_pClientList[iClientH]->m_iDamageAbsorption_Shield = 0;

    for (i = 0; i < DEF_MAXITEMEQUIPPOS; i++)
        m_pClientList[iClientH]->m_iDamageAbsorption_Armor[i] = 0;

    m_pClientList[iClientH]->m_iManaSaveRatio = 0;
    m_pClientList[iClientH]->m_iAddResistMagic = 0;

    m_pClientList[iClientH]->m_iAddPhysicalDamage = 0;
    m_pClientList[iClientH]->m_iAddMagicalDamage = 0;

    m_pClientList[iClientH]->m_bIsLuckyEffect = false;
    m_pClientList[iClientH]->m_iMagicDamageSaveItemIndex = -1;
    m_pClientList[iClientH]->m_iSideEffect_MaxHPdown = 0;

    m_pClientList[iClientH]->m_iAddAbsAir = 0;	// ¼Ó¼ºº° ´ë¹ÌÁö Èí¼ö
    m_pClientList[iClientH]->m_iAddAbsEarth = 0;
    m_pClientList[iClientH]->m_iAddAbsFire = 0;
    m_pClientList[iClientH]->m_iAddAbsWater = 0;

    m_pClientList[iClientH]->m_iCustomItemValue_Attack = 0;
    m_pClientList[iClientH]->m_iCustomItemValue_Defense = 0;

    m_pClientList[iClientH]->m_iMinAP_SM = 0;
    m_pClientList[iClientH]->m_iMinAP_L = 0;

    m_pClientList[iClientH]->m_iMaxAP_SM = 0;
    m_pClientList[iClientH]->m_iMaxAP_L = 0;

    m_pClientList[iClientH]->m_iSpecialWeaponEffectType = 0;	// Èñ±Í ¾ÆÀÌÅÛ È¿°ú Á¾·ù: 0-None 1-ÇÊ»ì±â´ë¹ÌÁöÃß°¡ 2-Áßµ¶È¿°ú 3-Á¤ÀÇÀÇ 4-ÀúÁÖÀÇ
    m_pClientList[iClientH]->m_iSpecialWeaponEffectValue = 0;	// Èñ±Í ¾ÆÀÌÅÛ È¿°ú °ª

    m_pClientList[iClientH]->m_iAddHP = m_pClientList[iClientH]->m_iAddSP = m_pClientList[iClientH]->m_iAddMP = 0;
    m_pClientList[iClientH]->m_iAddAR = m_pClientList[iClientH]->m_iAddPR = m_pClientList[iClientH]->m_iAddDR = 0;
    m_pClientList[iClientH]->m_iAddMR = m_pClientList[iClientH]->m_iAddAbsPD = m_pClientList[iClientH]->m_iAddAbsMD = 0;
    m_pClientList[iClientH]->m_iAddCD = m_pClientList[iClientH]->m_iAddExp = m_pClientList[iClientH]->m_iAddGold = 0;

    iPrevSAType = m_pClientList[iClientH]->m_iSpecialAbilityType;

    m_pClientList[iClientH]->m_iSpecialAbilityType = 0;
    m_pClientList[iClientH]->m_iSpecialAbilityLastSec = 0;
    m_pClientList[iClientH]->m_iSpecialAbilityEquipPos = 0;

    m_pClientList[iClientH]->m_iAddTransMana = 0;
    m_pClientList[iClientH]->m_iAddChargeCritical = 0;

    m_pClientList[iClientH]->m_iAlterItemDropIndex = -1;
    // Âø¿ëÇÏÁö ¾Ê¾Æµµ È¿°ú°¡ ÀÖ´Â ¾ÆÀÌÅÛ.
    for (sItemIndex = 0; sItemIndex < DEF_MAXITEMS; sItemIndex++)
        if (m_pClientList[iClientH]->m_pItemList[sItemIndex] != 0)
        {
            switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType)
            {
                case DEF_ITEMEFFECTTYPE_ALTERITEMDROP:
                    if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan > 0)
                    {
                        // ¼ö¸íÀÌ ÀÖ¾î¾ß È¿°ú°¡ ÀÖ´Ù.
                        m_pClientList[iClientH]->m_iAlterItemDropIndex = sItemIndex;
                    }
                    break;
            }
        }

    // Âø¿ëÀ» ÇØ¾ß È¿°ú°¡ ÀÖ´Â ¾ÆÀÌÅÛ 
    for (sItemIndex = 0; sItemIndex < DEF_MAXITEMS; sItemIndex++)
        if ((m_pClientList[iClientH]->m_pItemList[sItemIndex] != 0) &&
            (m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] == true))
        {

            cEquipPos = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos;

            switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType)
            {

                case DEF_ITEMEFFECTTYPE_MAGICDAMAGESAVE:
                    // ¸¶¹ý µ¥¹ÌÁö Àý°¨ ¾ÆÀÌÅÛ. ÀÎµ¦½º¸¦ ÀúÀåÇÑ´Ù.
                    m_pClientList[iClientH]->m_iMagicDamageSaveItemIndex = sItemIndex;
                    break;

                case DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY:
                case DEF_ITEMEFFECTTYPE_ATTACK_DEFENSE:
                case DEF_ITEMEFFECTTYPE_ATTACK_MANASAVE:
                case DEF_ITEMEFFECTTYPE_ATTACK_MAXHPDOWN:
                case DEF_ITEMEFFECTTYPE_ATTACK:
                    // ¹«±â ÀåÂø È¿°ú
                    m_pClientList[iClientH]->m_cAttackDiceThrow_SM = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
                    m_pClientList[iClientH]->m_cAttackDiceRange_SM = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
                    m_pClientList[iClientH]->m_cAttackBonus_SM = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue3;
                    m_pClientList[iClientH]->m_cAttackDiceThrow_L = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue4;
                    m_pClientList[iClientH]->m_cAttackDiceRange_L = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue5;
                    m_pClientList[iClientH]->m_cAttackBonus_L = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue6;

                    // v2.05 ¹«±âÀÇ Æ¯¼ºÄ¡ Ãß°¡ 
                    iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
                    //testcode
                    //wsprintf(G_cTxt, "Add Damage: %d", iTemp);
                    //log->info(G_cTxt);

                    // °íÁ¤ ´ë¹ÌÁö Ãß°¡
                    m_pClientList[iClientH]->m_iAddPhysicalDamage += iTemp;
                    m_pClientList[iClientH]->m_iAddMagicalDamage += iTemp;

                    // °ø°Ý¹«±âÀÇ ÇØ´ç ½ºÅ³¸¸Å­ÀÇ ¸íÁß·üÀ» ´õÇÑ´Ù. 
                    m_pClientList[iClientH]->m_iHitRatio += m_pClientList[iClientH]->m_cSkillMastery[m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sRelatedSkill];

                    // v1.432 »ç¿ëÇÏÁö ¾Ê´Â´Ù. ¹«±âÀÇ °ø°Ý´ë»óº° ¸íÁß·ü °¡°¨Ä¡¸¦ ´õÇÑ´Ù.
                    //m_pClientList[iClientH]->m_iHitRatio_ItemEffect_SM += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSM_HitRatio;
                    //m_pClientList[iClientH]->m_iHitRatio_ItemEffect_L  += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sL_HitRatio;
                    // ÇÃ·¹ÀÌ¾î°¡ »ç¿ëÇÏ´Â ¹«±âÀÇ ½ºÅ³À» ÀúÀåÇÑ´Ù. 
                    m_pClientList[iClientH]->m_sUsingWeaponSkill = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sRelatedSkill;

                    // v1.41 Custom-Made ¾ÆÀÌÅÛÀÌ¶ó¸é È¿°ú¸¦ ¼³Á¤ÇÑ´Ù. °ø°Ý¹«±â´Â 1°³¸¸ ÀåÂøµÈ´Ù°í ÇßÀ»¶§¸¸ À¯È¿ÇÔ.
                    if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x00000001) != 0)
                    {
                        m_pClientList[iClientH]->m_iCustomItemValue_Attack += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2;
                        // ¿¡·¯ º¸Á¤¿ë 
                        if (m_pClientList[iClientH]->m_iCustomItemValue_Attack > 100)
                            m_pClientList[iClientH]->m_iCustomItemValue_Attack = 100;

                        if (m_pClientList[iClientH]->m_iCustomItemValue_Attack < -100)
                            m_pClientList[iClientH]->m_iCustomItemValue_Attack = -100;

                        if (m_pClientList[iClientH]->m_iCustomItemValue_Attack > 0)
                        {
                            // ¾ç¼ö¸é ¹«±âÀÇ Å¸°ÝÄ¡ ÃÖ¼Ò°ªÀÌ ÃÖ´ë 5±îÁö ¿Ã¶ó°£´Ù.
                            dV2 = (double)m_pClientList[iClientH]->m_iCustomItemValue_Attack;
                            dV1 = (dV2 / 100.0f) * (5.0f);
                            m_pClientList[iClientH]->m_iMinAP_SM = m_pClientList[iClientH]->m_cAttackDiceThrow_SM +
                                m_pClientList[iClientH]->m_cAttackBonus_SM + (int)dV1;

                            m_pClientList[iClientH]->m_iMinAP_L = m_pClientList[iClientH]->m_cAttackDiceThrow_L +
                                m_pClientList[iClientH]->m_cAttackBonus_L + (int)dV1;

                            // ¼öÄ¡ Á¶Á¤ 
                            if (m_pClientList[iClientH]->m_iMinAP_SM < 1) m_pClientList[iClientH]->m_iMinAP_SM = 1;
                            if (m_pClientList[iClientH]->m_iMinAP_L < 1)  m_pClientList[iClientH]->m_iMinAP_L = 1;

                            // ÃÖ¼Ò°ªÀÌ ÃÖ´ë°ªº¸´Ù Å©¸é ÃÖ´ë°ª¸¸Å­ 
                            if (m_pClientList[iClientH]->m_iMinAP_SM > (m_pClientList[iClientH]->m_cAttackDiceThrow_SM * m_pClientList[iClientH]->m_cAttackDiceRange_SM + m_pClientList[iClientH]->m_cAttackBonus_SM))
                                m_pClientList[iClientH]->m_iMinAP_SM = (m_pClientList[iClientH]->m_cAttackDiceThrow_SM * m_pClientList[iClientH]->m_cAttackDiceRange_SM + m_pClientList[iClientH]->m_cAttackBonus_SM);

                            if (m_pClientList[iClientH]->m_iMinAP_L > (m_pClientList[iClientH]->m_cAttackDiceThrow_L * m_pClientList[iClientH]->m_cAttackDiceRange_L + m_pClientList[iClientH]->m_cAttackBonus_L))
                                m_pClientList[iClientH]->m_iMinAP_L = (m_pClientList[iClientH]->m_cAttackDiceThrow_L * m_pClientList[iClientH]->m_cAttackDiceRange_L + m_pClientList[iClientH]->m_cAttackBonus_L);

                            //testcode
                            //wsprintf(G_cTxt, "MinAP: %d %d +(%d)", m_pClientList[iClientH]->m_iMinAP_SM, m_pClientList[iClientH]->m_iMinAP_L, (int)dV1);
                            //log->info(G_cTxt);
                        }
                        else if (m_pClientList[iClientH]->m_iCustomItemValue_Attack < 0)
                        {
                            // À½¼ö¸é ¹«±âÀÇ Å¸°ÝÄ¡ ÃÖ´ë°ªÀÌ ÃÖ´ë 5±îÁö ³»·Á°£´Ù.
                            dV2 = (double)m_pClientList[iClientH]->m_iCustomItemValue_Attack;
                            dV1 = (dV2 / 100.0f) * (5.0f);
                            m_pClientList[iClientH]->m_iMaxAP_SM = m_pClientList[iClientH]->m_cAttackDiceThrow_SM * m_pClientList[iClientH]->m_cAttackDiceRange_SM
                                + m_pClientList[iClientH]->m_cAttackBonus_SM + (int)dV1;

                            m_pClientList[iClientH]->m_iMaxAP_L = m_pClientList[iClientH]->m_cAttackDiceThrow_L * m_pClientList[iClientH]->m_cAttackDiceRange_L
                                + m_pClientList[iClientH]->m_cAttackBonus_L + (int)dV1;

                            // ¼öÄ¡ Á¶Á¤ 
                            if (m_pClientList[iClientH]->m_iMaxAP_SM < 1) m_pClientList[iClientH]->m_iMaxAP_SM = 1;
                            if (m_pClientList[iClientH]->m_iMaxAP_L < 1)  m_pClientList[iClientH]->m_iMaxAP_L = 1;

                            // ÃÖ´ë°ªÀÌ ÃÖ¼Ò°ªº¸´Ù ÀÛÀ¸¸é ÃÖ¼Ò°ª¸¸Å­ 
                            if (m_pClientList[iClientH]->m_iMaxAP_SM < (m_pClientList[iClientH]->m_cAttackDiceThrow_SM * m_pClientList[iClientH]->m_cAttackDiceRange_SM + m_pClientList[iClientH]->m_cAttackBonus_SM))
                                m_pClientList[iClientH]->m_iMaxAP_SM = (m_pClientList[iClientH]->m_cAttackDiceThrow_SM * m_pClientList[iClientH]->m_cAttackDiceRange_SM + m_pClientList[iClientH]->m_cAttackBonus_SM);

                            if (m_pClientList[iClientH]->m_iMaxAP_L < (m_pClientList[iClientH]->m_cAttackDiceThrow_L * m_pClientList[iClientH]->m_cAttackDiceRange_L + m_pClientList[iClientH]->m_cAttackBonus_L))
                                m_pClientList[iClientH]->m_iMaxAP_L = (m_pClientList[iClientH]->m_cAttackDiceThrow_L * m_pClientList[iClientH]->m_cAttackDiceRange_L + m_pClientList[iClientH]->m_cAttackBonus_L);

                            //testcode
                            //wsprintf(G_cTxt, "MaxAP: %d %d +(%d)", m_pClientList[iClientH]->m_iMaxAP_SM, m_pClientList[iClientH]->m_iMaxAP_L, (int)dV1);
                            //log->info(G_cTxt);
                        }
                    }

                    // v1.42 Èñ±Í ¾ÆÀÌÅÛÀÌ¶ó¸é Main È¿°ú¸¦ ¼³Á¤ÇÑ´Ù. °ø°Ý¹«±â´Â 1°³¸¸ ÀåÂøµÈ´Ù°í ÇßÀ»¶§¸¸ À¯È¿ÇÔ.
                    if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x00F00000) != 0)
                    {
                        dwSWEType = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x00F00000) >> 20;
                        dwSWEValue = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x000F0000) >> 16;

                        // Èñ±Í ¾ÆÀÌÅÛ È¿°ú Á¾·ù: 
                        // 0-None 1-ÇÊ»ì±â´ë¹ÌÁöÃß°¡ 2-Áßµ¶È¿°ú 3-Á¤ÀÇÀÇ 4-ÀúÁÖÀÇ 
                        // 5-¹ÎÃ¸ÀÇ 6-°¡º­¿î 7-¿¹¸®ÇÑ 8-°­È­µÈ 9-°í´ë¹®¸íÀÇ 10-¸¶¹ý ¼º°øÀÇ
                        m_pClientList[iClientH]->m_iSpecialWeaponEffectType = (int)dwSWEType;
                        m_pClientList[iClientH]->m_iSpecialWeaponEffectValue = (int)dwSWEValue;

                        switch (dwSWEType)
                        {
                            case 7: // ¿¹¸®ÇÑ 
                                m_pClientList[iClientH]->m_cAttackDiceRange_SM++;
                                m_pClientList[iClientH]->m_cAttackDiceRange_L++;
                                break;

                            case 9: // °í´ë¹®¸íÀÇ
                                m_pClientList[iClientH]->m_cAttackDiceRange_SM += 2;
                                m_pClientList[iClientH]->m_cAttackDiceRange_L += 2;
                                break;
                        }
                    }

                    // v1.42 Èñ±Í ¾ÆÀÌÅÛÀÌ¶ó¸é Sub È¿°ú¸¦ ¼³Á¤ÇÑ´Ù. °ø°Ý¹«±â´Â 1°³¸¸ ÀåÂøµÈ´Ù°í ÇßÀ»¶§¸¸ À¯È¿ÇÔ.
                    if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x0000F000) != 0)
                    {
                        dwSWEType = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x0000F000) >> 12;
                        dwSWEValue = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x00000F00) >> 8;

                        // Èñ±Í ¾ÆÀÌÅÛ È¿°ú Á¾·ù: 
                        //Ãß°¡ µ¶¼ºÀúÇ×(1), Ãß°¡ ¸íÁß°ª(2), Ãß°¡ ¹æ¾î°ª(3), HP È¸º¹·® Ãß°¡(4), SP È¸º¹·® Ãß°¡(5)
                        //MP È¸º¹·® Ãß°¡(6), Ãß°¡ ¸¶¹ýÀúÇ×(7), ¹°¸® ´ë¹ÌÁö Èí¼ö(8), ¸¶¹ý ´ë¹ÌÁö Èí¼ö(9)
                        //¿¬Å¸ ´ë¹ÌÁö Ãß°¡(10), ´õ ¸¹Àº °æÇèÄ¡(11), ´õ¸¹Àº Gold(12)

                        switch (dwSWEType)
                        {
                            case 0:  break;
                            case 1:  m_pClientList[iClientH]->m_iAddPR += (int)dwSWEValue * 7; break;
                            case 2:  m_pClientList[iClientH]->m_iAddAR += (int)dwSWEValue * 7; break;
                            case 3:  m_pClientList[iClientH]->m_iAddDR += (int)dwSWEValue * 7; break;
                            case 4:  m_pClientList[iClientH]->m_iAddHP += (int)dwSWEValue * 7; break;
                            case 5:  m_pClientList[iClientH]->m_iAddSP += (int)dwSWEValue * 7; break;
                            case 6:  m_pClientList[iClientH]->m_iAddMP += (int)dwSWEValue * 7; break;
                            case 7:  m_pClientList[iClientH]->m_iAddMR += (int)dwSWEValue * 7; break;
                            case 8:  m_pClientList[iClientH]->m_iDamageAbsorption_Armor[m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos] += (int)dwSWEValue * 3; break;
                            case 9:  m_pClientList[iClientH]->m_iAddAbsMD += (int)dwSWEValue * 3; break;
                            case 10: m_pClientList[iClientH]->m_iAddCD += (int)dwSWEValue; break;
                            case 11: m_pClientList[iClientH]->m_iAddExp += (int)dwSWEValue * 10; break;
                            case 12: m_pClientList[iClientH]->m_iAddGold += (int)dwSWEValue * 10; break;
                        }

                        // v2.04 Æ¯¼ºÄ¡ Á¦ÇÑÀ» ºÙÀÓ.
                        switch (dwSWEType)
                        {
                            case 9: if (m_pClientList[iClientH]->m_iAddAbsMD > 80) m_pClientList[iClientH]->m_iAddAbsMD = 80; break;
                        }
                    }

                    // ÀÏ¹Ý °ø°Ý ÀÌ¿ÜÀÇ È¿°ú¸¦ ¼³Á¤.
                    switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType)
                    {
                        case DEF_ITEMEFFECTTYPE_ATTACK_MAXHPDOWN:
                            // ÃÖ´ë HP °¨¼Ò È¿°ú
                            m_pClientList[iClientH]->m_iSideEffect_MaxHPdown = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpecialEffect;
                            break;

                        case DEF_ITEMEFFECTTYPE_ATTACK_MANASAVE:
                            // ¸¶³ª Àý¾à È¿°ú: ¸¶³ª Àý°¨·üÀº ÃÖ´ë 80%
                            m_pClientList[iClientH]->m_iManaSaveRatio += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue4;
                            if (m_pClientList[iClientH]->m_iManaSaveRatio > 80) m_pClientList[iClientH]->m_iManaSaveRatio = 80;
                            break;

                        case DEF_ITEMEFFECTTYPE_ATTACK_DEFENSE:
                            // Ãß°¡ ¹°¸® ¹æ¾î ´É·Â È¿°ú 
                            m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_BODY] += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpecialEffect;
                            break;

                        case DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY:
                            // Æ¯¼ö ´É·Â Á¾·ù
                            m_pClientList[iClientH]->m_iSpecialAbilityType = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpecialEffect;
                            // Æ¯¼ö ´É·Â Áö¼Ó ½Ã°£
                            m_pClientList[iClientH]->m_iSpecialAbilityLastSec = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpecialEffectValue1;
                            // ÀåÂø À§Ä¡ ±â¾ïÇØ ³õ´Â´Ù.
                            m_pClientList[iClientH]->m_iSpecialAbilityEquipPos = (int)cEquipPos;

                            // Æ¯¼ö ´É·Â ¼³Á¤µÊÀ» ¾Ë·ÁÁØ´Ù.
                            if ((bNotify == true) && (iEquipItemID == (int)sItemIndex))
                                SendNotifyMsg(0, iClientH, DEF_NOTIFY_SPECIALABILITYSTATUS, 2, m_pClientList[iClientH]->m_iSpecialAbilityType, m_pClientList[iClientH]->m_iSpecialAbilityTime, 0);
                            break;
                    }
                    break;

                case DEF_ITEMEFFECTTYPE_ADDEFFECT:
                    switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1)
                    {
                        case 1:
                            // Ãß°¡ ¸¶¹ý ÀúÇ× Áõ°¡ 
                            m_pClientList[iClientH]->m_iAddResistMagic += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
                            break;

                        case 2:
                            // ¸¶³ª Àý¾à È¿°ú 
                            m_pClientList[iClientH]->m_iManaSaveRatio += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
                            // ¸¶³ª Àý°¨·üÀº ÃÖ´ë 80%
                            if (m_pClientList[iClientH]->m_iManaSaveRatio > 80) m_pClientList[iClientH]->m_iManaSaveRatio = 80;
                            break;

                        case 3:
                            // °íÁ¤ ¹°¸® ´ë¹ÌÁö È¿°ú. ¸ðµç ¹°¸® °ø°Ý¿¡ ´ëÇØ¼­ °íÁ¤ ´ë¹ÌÁö°¡ ºÙ´Â´Ù. 
                            m_pClientList[iClientH]->m_iAddPhysicalDamage += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
                            break;

                        case 4:
                            // Ãß°¡ ¹°¸® ¹æ¾î·Â 
                            m_pClientList[iClientH]->m_iDefenseRatio += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
                            break;

                        case 5:
                            // Çà¿î È¿°ú?
                            if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2 != 0)
                                m_pClientList[iClientH]->m_bIsLuckyEffect = true;
                            else m_pClientList[iClientH]->m_bIsLuckyEffect = false;
                            break;

                        case 6:
                            // °íÁ¤ ¸¶¹ý ´ë¹ÌÁö È¿°ú. ¸ðµç ¸¶¹ý °ø°Ý¿¡ ´ëÇØ¼­ °íÁ¤ ´ë¹ÌÁö°¡ ºÙ´Â´Ù. 
                            m_pClientList[iClientH]->m_iAddMagicalDamage += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
                            break;

                        case 7:
                            m_pClientList[iClientH]->m_iAddAbsAir += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
                            break;

                        case 8:
                            m_pClientList[iClientH]->m_iAddAbsEarth += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
                            break;

                        case 9:
                            m_pClientList[iClientH]->m_iAddAbsFire += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
                            break;

                        case 10:
                            // ¹°°è¿­ °ø°Ý ´ë¹ÌÁö °¨¼Ò´Â ¾ó¾îºÙÀ» È®·üÀÇ °¨¼ÒÈ¿°ú°¡ ÀÖ´Ù. (2¹è·Î È®·ü °è»êµÊ)
                            m_pClientList[iClientH]->m_iAddAbsWater += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
                            break;

                        case 11:
                            // µ¶¼º ÀúÇ×.
                            m_pClientList[iClientH]->m_iAddPR += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
                            break;

                        case 12:
                            m_pClientList[iClientH]->m_iHitRatio += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
                            break;
                    }
                    break;

                case DEF_ITEMEFFECTTYPE_ATTACK_ARROW:
                    // È­»ìÀ» ÇÊ¿ä·Î ÇÏ´Â ¹«±â(È°·ù)
                    // ¹«±âÀÇ °ø°ÝÈ¿°ú´Â ¾î¶² È­»ìÀÌ ¼±ÅÃµÇ´À³Ä¿¡ µû¶ó ´Þ¶óÁø´Ù. ¾ÆÀÌÅÛÁß È­»ìÀ» Ã£¾Æ ÇÒ´çÇÑ´Ù.
                    if ((m_pClientList[iClientH]->m_cArrowIndex != -1) &&
                        (m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cArrowIndex] == 0))
                    {
                        // ArrowIndex¿¡ È­»ìÀÌ ¾ø´Ù. (ºñ¾îÀÖ´Â ¾ÆÀÌÅÛ) 
                        m_pClientList[iClientH]->m_cArrowIndex = _iGetArrowItemIndex(iClientH);
                    }
                    else if (m_pClientList[iClientH]->m_cArrowIndex == -1)
                        m_pClientList[iClientH]->m_cArrowIndex = _iGetArrowItemIndex(iClientH);

                    if (m_pClientList[iClientH]->m_cArrowIndex == -1)
                    {
                        // È­»ìÀÌ ¾ø´Ù.
                        m_pClientList[iClientH]->m_cAttackDiceThrow_SM = 0;
                        m_pClientList[iClientH]->m_cAttackDiceRange_SM = 0;
                        m_pClientList[iClientH]->m_cAttackBonus_SM = 0;
                        m_pClientList[iClientH]->m_cAttackDiceThrow_L = 0;
                        m_pClientList[iClientH]->m_cAttackDiceRange_L = 0;
                        m_pClientList[iClientH]->m_cAttackBonus_L = 0;
                    }
                    else
                    {
                        iArrowIndex = m_pClientList[iClientH]->m_cArrowIndex;
                        m_pClientList[iClientH]->m_cAttackDiceThrow_SM = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
                        m_pClientList[iClientH]->m_cAttackDiceRange_SM = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
                        m_pClientList[iClientH]->m_cAttackBonus_SM = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue3;
                        m_pClientList[iClientH]->m_cAttackDiceThrow_L = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue4;
                        m_pClientList[iClientH]->m_cAttackDiceRange_L = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue5;
                        m_pClientList[iClientH]->m_cAttackBonus_L = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue6;
                    }

                    m_pClientList[iClientH]->m_iHitRatio += m_pClientList[iClientH]->m_cSkillMastery[m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sRelatedSkill];
                    break;

                case DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY:
                case DEF_ITEMEFFECTTYPE_DEFENSE:
                    m_pClientList[iClientH]->m_iDefenseRatio += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;

                    if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x00000001) != 0)
                    {
                        m_pClientList[iClientH]->m_iCustomItemValue_Defense += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2;

                        dV2 = (double)m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2;
                        dV3 = (double)m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
                        dV1 = (double)(dV2 / 100.0f) * dV3;

                        dV1 = dV1 / 2.0f;
                        m_pClientList[iClientH]->m_iDefenseRatio += (int)dV1;
                        if (m_pClientList[iClientH]->m_iDefenseRatio <= 0) m_pClientList[iClientH]->m_iDefenseRatio = 1;

                        //testcode
                        //wsprintf(G_cTxt, "Custom-Defense: %d", (int)dV1);
                        //log->info(G_cTxt);
                    }

                    if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x00F00000) != 0)
                    {
                        dwSWEType = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x00F00000) >> 20;
                        dwSWEValue = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x000F0000) >> 16;

                        // Èñ±Í ¾ÆÀÌÅÛ È¿°ú Á¾·ù: 
                        // 0-None 1-ÇÊ»ì±â´ë¹ÌÁöÃß°¡ 2-Áßµ¶È¿°ú 3-Á¤ÀÇÀÇ 4-ÀúÁÖÀÇ 
                        // 5-¹ÎÃ¸ÀÇ 6-°¡º­¿î 7-¿¹¸®ÇÑ 8-°­È­µÈ 9-°í´ë¹®¸íÀÇ 10-¸¶¹ý¼º°øÀÇ 11-¸¶³ªº¯È¯ÀÇ 12-ÇÊ»ìÃæÀüÀÇ

                        switch (dwSWEType)
                        {
                            case 7: // ¿¹¸®ÇÑ 
                                m_pClientList[iClientH]->m_cAttackDiceRange_SM++;
                                m_pClientList[iClientH]->m_cAttackDiceRange_L++;
                                break;

                            case 9: // °í´ë¹®¸íÀÇ
                                m_pClientList[iClientH]->m_cAttackDiceRange_SM += 2;
                                m_pClientList[iClientH]->m_cAttackDiceRange_L += 2;
                                break;

                                // v2.04 
                            case 11: // ¸¶³ª º¯È¯ÀÇ 
                                m_pClientList[iClientH]->m_iAddTransMana += dwSWEValue;
                                if (m_pClientList[iClientH]->m_iAddTransMana > 13) m_pClientList[iClientH]->m_iAddTransMana = 13;
                                break;

                            case 12: // ÇÊ»ì ÃæÀüÀÇ 
                                m_pClientList[iClientH]->m_iAddChargeCritical += dwSWEValue;
                                if (m_pClientList[iClientH]->m_iAddChargeCritical > 20) m_pClientList[iClientH]->m_iAddChargeCritical = 20;
                                break;
                        }
                    }

                    // v1.42 Èñ±Í ¾ÆÀÌÅÛÀÌ¶ó¸é Sub È¿°ú¸¦ ¼³Á¤ÇÑ´Ù. °ø°Ý¹«±â´Â 1°³¸¸ ÀåÂøµÈ´Ù°í ÇßÀ»¶§¸¸ À¯È¿ÇÔ.
                    if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x0000F000) != 0)
                    {
                        dwSWEType = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x0000F000) >> 12;
                        dwSWEValue = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x00000F00) >> 8;

                        // Èñ±Í ¾ÆÀÌÅÛ È¿°ú Á¾·ù: 
                        //Ãß°¡ µ¶¼ºÀúÇ×(1), Ãß°¡ ¸íÁß°ª(2), Ãß°¡ ¹æ¾î°ª(3), HP È¸º¹·® Ãß°¡(4), SP È¸º¹·® Ãß°¡(5)
                        //MP È¸º¹·® Ãß°¡(6), Ãß°¡ ¸¶¹ýÀúÇ×(7), ¹°¸® ´ë¹ÌÁö Èí¼ö(8), ¸¶¹ý ´ë¹ÌÁö Èí¼ö(9)
                        //¿¬Å¸ ´ë¹ÌÁö Ãß°¡(10), ´õ ¸¹Àº °æÇèÄ¡(11), ´õ¸¹Àº Gold(12)

                        switch (dwSWEType)
                        {
                            case 0:  break;
                            case 1:  m_pClientList[iClientH]->m_iAddPR += (int)dwSWEValue * 7; break;
                            case 2:  m_pClientList[iClientH]->m_iAddAR += (int)dwSWEValue * 7; break;
                            case 3:  m_pClientList[iClientH]->m_iAddDR += (int)dwSWEValue * 7; break;
                            case 4:  m_pClientList[iClientH]->m_iAddHP += (int)dwSWEValue * 7; break;
                            case 5:  m_pClientList[iClientH]->m_iAddSP += (int)dwSWEValue * 7; break;
                            case 6:  m_pClientList[iClientH]->m_iAddMP += (int)dwSWEValue * 7; break;
                            case 7:  m_pClientList[iClientH]->m_iAddMR += (int)dwSWEValue * 7; break;
                            case 8:  m_pClientList[iClientH]->m_iDamageAbsorption_Armor[m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos] += (int)dwSWEValue * 3; break;
                            case 9:  m_pClientList[iClientH]->m_iAddAbsMD += (int)dwSWEValue * 3; break;
                            case 10: m_pClientList[iClientH]->m_iAddCD += (int)dwSWEValue; break;
                            case 11: m_pClientList[iClientH]->m_iAddExp += (int)dwSWEValue * 10; break;
                            case 12: m_pClientList[iClientH]->m_iAddGold += (int)dwSWEValue * 10; break;
                        }

                        // v2.04 Æ¯¼ºÄ¡ Á¦ÇÑÀ» ºÙÀÓ.
                        switch (dwSWEType)
                        {
                            case 9: if (m_pClientList[iClientH]->m_iAddAbsMD > 80) m_pClientList[iClientH]->m_iAddAbsMD = 80; break; // ¸¶¹ý ´ë¹ÌÁö Èí¼ö ÃÖ´ë 80%
                        }
                    }

                    switch (cEquipPos)
                    {
                        case DEF_EQUIPPOS_LHAND:
                            // ¹æ¾î±¸ Áß ¹æÆÐ°¡ ÀåÂø µÇ¾ú´Ù. 
                            // ¹æÆÐ¿¡ ÀÇÇÑ Ãæ°Ý Èí¼ö È¿°ú ÇÒ´çÇÑ´Ù. ¹æ¾î°ªÀÇ 70% ¼öÁØ 
                            m_pClientList[iClientH]->m_iDamageAbsorption_Shield = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1) - (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1) / 3;
                            break;
                        default:
                            // °©¿Ê¿¡ ÀÇÇÑ Ãæ°Ý Èí¼ö È¿°ú¸¦ ´õÇÑ´Ù. ¹æ¾î°ªÀÇ 70% ¼öÁØ <- v1.43 100%·Î º¯°æµÇ¾ú´Ù. V2!
                            m_pClientList[iClientH]->m_iDamageAbsorption_Armor[m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos] += (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2);
                            break;
                    }

                    // ÀÏ¹Ý °ø°Ý ÀÌ¿ÜÀÇ È¿°ú¸¦ ¼³Á¤.
                    switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType)
                    {
                        case DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY:
                            // Æ¯¼ö ´É·Â Á¾·ù
                            m_pClientList[iClientH]->m_iSpecialAbilityType = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpecialEffect;
                            // Æ¯¼ö ´É·Â Áö¼Ó ½Ã°£
                            m_pClientList[iClientH]->m_iSpecialAbilityLastSec = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpecialEffectValue1;
                            // ÀåÂø À§Ä¡ ±â¾ïÇØ ³õ´Â´Ù.
                            m_pClientList[iClientH]->m_iSpecialAbilityEquipPos = (int)cEquipPos;

                            // Æ¯¼ö ´É·Â ¼³Á¤µÊÀ» ¾Ë·ÁÁØ´Ù.
                            if ((bNotify == true) && (iEquipItemID == (int)sItemIndex))
                                SendNotifyMsg(0, iClientH, DEF_NOTIFY_SPECIALABILITYSTATUS, 2, m_pClientList[iClientH]->m_iSpecialAbilityType, m_pClientList[iClientH]->m_iSpecialAbilityTime, 0);
                            break;
                    }
                    break;
            }
        }

    //v1.432
    if ((iPrevSAType != 0) && (m_pClientList[iClientH]->m_iSpecialAbilityType == 0) && (bNotify == true))
    {
        // Æ¯¼ö ´É·ÂÀÌ ÇØÁ¦µÇ¾ú´Ù.
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SPECIALABILITYSTATUS, 4, 0, 0, 0);
        // ¸¸¾à Æ¯¼ö ´É·Â »ç¿ë ÁßÀÌ¾ú´Ù¸é ¿Ü¸ðµµ º¯È­ÇÏ¸ç »ç¿ëµµ Á¾·á.
        if (m_pClientList[iClientH]->m_bIsSpecialAbilityEnabled == true)
        {
            m_pClientList[iClientH]->m_bIsSpecialAbilityEnabled = false;
            // ´ÙÀ½ »ç¿ë °¡´É ½Ã°£À» ÀÔ·ÂÇÑ´Ù.
            m_pClientList[iClientH]->m_iSpecialAbilityTime = DEF_SPECABLTYTIMESEC;
            // È¿°ú ºñÆ® Å¬¸®¾î
            sTemp = m_pClientList[iClientH]->m_sAppr4;
            sTemp = sTemp & 0xFF0F;
            m_pClientList[iClientH]->m_sAppr4 = sTemp;
            // ¿ÜÇüÀÌ º¯ÇßÀ½À» ¾Ë¸°´Ù.
            SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
        }
    }

    if ((iPrevSAType != 0) && (m_pClientList[iClientH]->m_iSpecialAbilityType != 0) &&
        (iPrevSAType != m_pClientList[iClientH]->m_iSpecialAbilityType) && (bNotify == true))
    {
        // Æ¯¼ö ´É·Â »ç¿ë µµÁß¿¡ Æ¯¼ö ´É·ÂÀ» ¹Ù²Ù¸é ´É·Â »ç¿ëÀÌ ¸ØÃá´Ù.
        if (m_pClientList[iClientH]->m_bIsSpecialAbilityEnabled == true)
        {
            // »ç¿ë½Ã°£ÀÌ ´Ù µÇ¾ú´Ù.
            SendNotifyMsg(0, i, DEF_NOTIFY_SPECIALABILITYSTATUS, 3, 0, 0, 0);
            m_pClientList[iClientH]->m_bIsSpecialAbilityEnabled = false;
            // ´ÙÀ½ »ç¿ë °¡´É ½Ã°£À» ÀÔ·ÂÇÑ´Ù.
            m_pClientList[iClientH]->m_iSpecialAbilityTime = DEF_SPECABLTYTIMESEC;
            // È¿°ú ºñÆ® Å¬¸®¾î
            sTemp = m_pClientList[iClientH]->m_sAppr4;
            sTemp = sTemp & 0xFF0F;
            m_pClientList[iClientH]->m_sAppr4 = sTemp;
            // ¿ÜÇüÀÌ º¯ÇßÀ½À» ¾Ë¸°´Ù.
            SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
        }
    }
}

int CGame::_iGetPlayerNumberOnSpot(short dX, short dY, char cMapIndex, char cRange)
{
    int ix, iy, iSum = 0;
    short sOwnerH;
    char  cOwnerType;

    for (ix = dX - cRange; ix <= dX + cRange; ix++)
        for (iy = dY - cRange; iy <= dY + cRange; iy++)
        {
            m_pMapList[cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
            if ((sOwnerH != 0) && (cOwnerType == DEF_OWNERTYPE_PLAYER))
                iSum++;
        }

    return iSum;
}

void CGame::RequestAdminUserMode(int iClientH, char * pData)
{
    // Â¿Ã®Â¿ÂµÃ€Ãš Â¸Ã°ÂµÃ¥Â¸Â¦ ÃˆÂ®Ã€ÃŽÃ‡ÃÂ±Ã¢ Ã€Â§Ã‡Ã˜Â¼Â­Â´Ã‚ Ã€ÃÂ´Ãœ Ã€ÃŒ Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ Ã€Ã¼Â¼Ã›Ã‡ÃÂ´Ã‚ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¸Â¦ Â°Â®Â°Ã­ Ã€Ã–Â¾Ã®Â¾ÃŸ Ã‡Ã‘Â´Ã™. 
    // Â¶Ã‡Ã‡Ã‘ Â³Â¯Ã‚Â¥Â¸Â¦ ÃÂ¶Ã‡Ã•Ã‡Ã˜Â¼Â­ Â»Ã½Â¼ÂºÂµÃ‡Â´Ã‚ ÃˆÂ®Ã€ÃŽ Ã„ÃšÂµÃ¥Â°Â¡ Ã€ÃÃ„Â¡Ã‡Ã˜Â¾ÃŸ Ã‡Ã‘Â´Ã™. 

}

bool CGame::bAnalyzeCriminalAction(int iClientH, short dX, short dY, bool bIsCheck)
{
    int   iNamingValue, tX, tY;
    short sOwnerH;
    char  cOwnerType, cName[11], cNpcName[21];
    char  cNpcWaypoint[11];

    if (m_pClientList[iClientH] == nullptr) return false;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return false;
    if (m_bIsCrusadeMode == true) return false;

    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);

    if ((cOwnerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sOwnerH] != 0))
    {
        if (_bGetIsPlayerHostile(iClientH, sOwnerH) != true)
        {
            if (bIsCheck == true) return true;

            memset(cNpcName, 0, sizeof(cNpcName));
            if (strcmp(m_pClientList[iClientH]->m_cMapName, "aresden") == 0)
                strcpy(cNpcName, "Guard-Aresden");
            else if (strcmp(m_pClientList[iClientH]->m_cMapName, "elvine") == 0)
                strcpy(cNpcName, "Guard-Elvine");
            else  if (strcmp(m_pClientList[iClientH]->m_cMapName, "default") == 0)
                strcpy(cNpcName, "Guard-Neutral");

            iNamingValue = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->iGetEmptyNamingValue();
            if (iNamingValue == -1)
            {

            }
            else
            {
                memset(cNpcWaypoint, 0, sizeof(cNpcWaypoint));
                memset(cName, 0, sizeof(cName));
                wsprintf(cName, "XX%d", iNamingValue);
                cName[0] = '_';
                cName[1] = m_pClientList[iClientH]->m_cMapIndex + 65;

                tX = (int)m_pClientList[iClientH]->m_sX;
                tY = (int)m_pClientList[iClientH]->m_sY;
                if (bCreateNewNpc(cNpcName, cName, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, 0, 0, DEF_MOVETYPE_RANDOM,
                    &tX, &tY, cNpcWaypoint, 0, 0, -1, false, true) == false)
                {
                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetNamingValueEmpty(iNamingValue);
                }
                else
                {
                    bSetNpcAttackMode(cName, iClientH, DEF_OWNERTYPE_PLAYER, true);
                }
            }
        }
    }
    return false;
}

bool CGame::_bGetIsPlayerHostile(int iClientH, int sOwnerH)
{
    if (m_pClientList[iClientH] == nullptr) return false;
    if (m_pClientList[sOwnerH] == 0) return false;

    // Ã€ÃšÂ±Ã¢ Ã€ÃšÂ½Ã…Â¿Â¡ Â´Ã«Ã‡Ã‘ Â°Ã¸Â°ÃÃ€Âº Â¹Ã¼ÃÃ‹Â¶Ã³ ÂºÂ¼ Â¼Ã¶ Â¾Ã¸Â´Ã™. 
    if (iClientH == sOwnerH) return true;

    if (m_pClientList[iClientH]->m_cSide == 0)
    {
        // Â°Ã¸Â°ÃÃ€ÃšÂ´Ã‚ Â¿Â©Ã‡Ã Ã€ÃšÃ€ÃŒÂ´Ã™.
        // Â»Ã³Â´Ã«Â¹Ã¦Ã€ÃŒ Â¹Ã¼ÃÃ‹Ã€ÃšÃ€ÃŒÂ¸Ã© Ã€Ã»Â¼ÂºÃ€ÃŒÂ¸Ã§ Â±Ã— Ã€ÃŒÂ¿ÃœÂ¿Â¡Â´Ã‚ Â¾Ã†Â´ÃÂ´Ã™. 
        if (m_pClientList[sOwnerH]->m_iPKCount != 0)
            return true;
        else return false;
    }
    else
    {
        // Â°Ã¸Â°ÃÃ€ÃšÂ´Ã‚ Â¼Ã’Â¼Ã“ Â¸Â¶Ã€Â»Ã€Â» Â°Â®Â°Ã­ Ã€Ã–Â´Ã™. 
        if (m_pClientList[iClientH]->m_cSide != m_pClientList[sOwnerH]->m_cSide)
        {
            // Â¼Â­Â·ÃŽ Â»Ã§Ã€ÃŒÂµÃ¥Â°Â¡ Â´Ã™Â¸Â£Â´Ã™. 
            if (m_pClientList[sOwnerH]->m_cSide == 0)
            {
                if (m_pClientList[sOwnerH]->m_iPKCount != 0)
                    return true;
                else return false;
            }
            else return true;
        }
        else
        {
            if (m_pClientList[sOwnerH]->m_iPKCount != 0)
                return true;
            else return false;
        }
    }

    return false;
}

void CGame::bSetNpcAttackMode(char * cName, int iTargetH, char cTargetType, bool bIsPermAttack)
{
    int i, iIndex;

    for (i = 1; i < DEF_MAXNPCS; i++)
        if ((m_pNpcList[i] != 0) && (memcmp(m_pNpcList[i]->m_cName, cName, 5) == 0))
        {
            iIndex = i;
            goto NEXT_STEP_SNAM1;

            //testcode
            //log->info("bSetNpcAttackMode - Npc found");
        }
    // Â°Â°Ã€Âº Ã€ÃŒÂ¸Â§Ã€Â» Â°Â¡ÃÃ¸ NPCÂ°Â¡ Â¾Ã¸Â´Ã™.
    return;

    NEXT_STEP_SNAM1:;

    switch (cTargetType)
    {
        case DEF_OWNERTYPE_PLAYER:
            if (m_pClientList[iTargetH] == 0) return;
            break;

        case DEF_OWNERTYPE_NPC:
            if (m_pNpcList[iTargetH] == 0) return;
            break;
    }

    // Â¸Ã±Ã‡Â¥Â¹Â°Ã€Â» Ã‡Ã’Â´Ã§Ã‡Ã‘Â´Ã™. 
    m_pNpcList[iIndex]->m_cBehavior = DEF_BEHAVIOR_ATTACK;
    m_pNpcList[iIndex]->m_sBehaviorTurnCount = 0;
    m_pNpcList[iIndex]->m_iTargetIndex = iTargetH;
    m_pNpcList[iIndex]->m_cTargetType = cTargetType;

    // Â¿ÂµÂ±Â¸ Â°Ã¸Â°Ã Â¸Ã°ÂµÃ¥ Â¿Â©ÂºÃŽ Â¼Â³ÃÂ¤ 
    m_pNpcList[iIndex]->m_bIsPermAttackMode = bIsPermAttack;

    //testcode
    //log->info("bSetNpcAttackMode - complete");
}

void CGame::PoisonEffect(int iClientH, int iV1)
{
    int iPoisonLevel, iDamage, iPrevHP, iProb;

    // ÃÃŸÂµÂ¶Ã€Â¸Â·ÃŽ ÃÃ—ÃÃ¶Â´Ã‚ Â¾ÃŠÂ´Ã‚Â´Ã™. Â´Ã™Â¸Â¸ ÃƒÂ¼Â·Ã‚Ã€ÃŒ Â°Ã¨Â¼Ã“ Â±Ã¯Ã€ÃŒÂ°Ã­ ÃƒÃ–Â¼Ã’ 1Â¸Â¸ Â³Â²Â´Ã‚Â´Ã™. 
    if (m_pClientList[iClientH] == nullptr)     return;
    if (m_pClientList[iClientH]->m_bIsKilled == true) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

    iPoisonLevel = m_pClientList[iClientH]->m_iPoisonLevel;

    iDamage = iDice(1, iPoisonLevel);

    iPrevHP = m_pClientList[iClientH]->m_iHP;
    m_pClientList[iClientH]->m_iHP -= iDamage;
    if (m_pClientList[iClientH]->m_iHP <= 0) m_pClientList[iClientH]->m_iHP = 1;

    if (iPrevHP != m_pClientList[iClientH]->m_iHP)
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_HP, 0, 0, 0, 0);


    // ÂµÂ¶Â¼Âº Ã€ÃºÃ‡Ã— ÃˆÂ®Â·Ã¼Â·ÃŽ ÃÃŸÂµÂ¶Ã€ÃŒ Ã‡Â®Â¸Â± Â¼Ã¶ Ã€Ã–Â´Ã™.
    iProb = m_pClientList[iClientH]->m_cSkillMastery[23] - 10 + m_pClientList[iClientH]->m_iAddPR;
    if (iProb <= 10) iProb = 10;
    if (iDice(1, 100) <= iProb)
    {
        m_pClientList[iClientH]->m_bIsPoisoned = false;
        // ÃÃŸÂµÂ¶Ã€ÃŒ Ã‡Â®Â·ÃˆÃ€Â½Ã€Â» Â¾Ã‹Â¸Â°Â´Ã™. 
        SetPoisonFlag(iClientH, DEF_OWNERTYPE_PLAYER, false); // remove poison aura after effect complete
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_POISON, 0, 0, 0);
    }
}

bool CGame::bCheckResistingPoisonSuccess(short sOwnerH, char cOwnerType)
{
    int iResist, iResult;

    // ÂµÂ¶Â¼Âº Ã€ÃºÃ‡Ã—Ã€ÃŒ Â¼ÂºÂ°Ã¸Ã‡ÃŸÂ´Ã‚ÃÃ¶Â¸Â¦ Â°Ã¨Â»ÃªÃ‡Ã‘Â´Ã™. 
    switch (cOwnerType)
    {
        case DEF_OWNERTYPE_PLAYER:
            if (m_pClientList[sOwnerH] == 0) return false;
            iResist = m_pClientList[sOwnerH]->m_cSkillMastery[23] + m_pClientList[sOwnerH]->m_iAddPR;
            break;

        case DEF_OWNERTYPE_NPC:
            if (m_pNpcList[sOwnerH] == 0) return false;
            iResist = 0;
            break;
    }

    iResult = iDice(1, 100);
    if (iResult >= iResist) // ÂµÂ¶Â¼Âº Ã€ÃºÃ‡Ã— Â½Ã‡Ã†Ã. ÃÃŸÂµÂ¶ÂµÃˆÂ´Ã™.
        return false;

    // ÂµÂ¶Â¼Âº Ã€ÃºÃ‡Ã— Â¼ÂºÂ°Ã¸. Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Â¶Ã³Â¸Ã© Â½ÂºÃ…Â³Ã€Â» Â¿ÃƒÂ¸Â°Â´Ã™. 
    if (cOwnerType == DEF_OWNERTYPE_PLAYER)
        CalculateSSN_SkillIndex(sOwnerH, 23, 1);

    return true;
}

bool CGame::bCheckBadWord(char * pString)
{
    char * cp;

    // Â½ÂºÃ†Â®Â¸ÂµÃ€Â» Â°Ã‹Â»Ã¶Ã‡ÃÂ¸Ã§ Â¿Ã¥Â¼Â³Ã€ÃŒ Ã€Ã–Â´Ã‚ÃÃ¶Â¸Â¦ ÃƒÂ£Â´Ã‚Â´Ã™. 
    cp = pString;
    while (*cp != 0)
    {

        cp++;
    }

    return false;
}

void CGame::CheckDayOrNightMode()
{
    SYSTEMTIME SysTime;
    char cPrevMode;
    int  i;

    if (m_bManualTime = true) return;

    // Ã‡Ã¶Ã€Ã§ Â½ÃƒÂ°Â£Â¿Â¡ ÂµÃ»Â¶Ã³ Â³Â· ÃˆÂ¤Ã€Âº Â¹Ã£Ã€Â» Â°Ã¡ÃÂ¤Ã‡Ã‘Â´Ã™. 
    // ÃÃ–, Â¾ÃŸÂ°Â£ Â¸Ã°ÂµÃ¥ Â¼Â³ÃÂ¤ 
    cPrevMode = m_cDayOrNight;

    GetLocalTime(&SysTime);
    if (SysTime.wMinute >= DEF_NIGHTTIME)
        m_cDayOrNight = 2;
    else m_cDayOrNight = 1;

    if (cPrevMode != m_cDayOrNight)
    {
        // ÃÃ–, Â¾ÃŸÂ°Â£ Â¸Ã°ÂµÃ¥Â°Â¡ ÂºÂ¯Â°Ã¦ÂµÃ‡Â¾ÃºÂ´Ã™. ÃÃ–, Â¾ÃŸÂ°Â£ Â¸Ã°ÂµÃ¥Ã€Ã‡ Â¿ÂµÃ‡Ã¢Ã€Â» Â¹ÃžÂ´Ã‚ Â¸ÃŠÂ¿Â¡ ÃÂ¸Ã€Ã§Ã‡ÃÂ´Ã‚ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®ÂµÃ©Â¿Â¡Â°Ã” Â¾Ã‹Â¸Â°Â´Ã™. 
        for (i = 1; i < DEF_MAXCLIENTS; i++)
            if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_bIsInitComplete == true))
            {
                if ((m_pClientList[i]->m_cMapIndex >= 0) &&
                    (m_pMapList[m_pClientList[i]->m_cMapIndex] != 0) &&
                    (m_pMapList[m_pClientList[i]->m_cMapIndex]->m_bIsFixedDayMode == false))
                    SendNotifyMsg(0, i, DEF_NOTIFY_TIMECHANGE, m_cDayOrNight, 0, 0, 0);
            }
    }
}

void CGame::ShutUpPlayer(int iClientH, char * pMsg, uint32_t dwMsgSize)
{
    char   seps[] = "= \t\n";
    char * token, cName[11], cBuff[256];
    class  CStrTok * pStrTok;
    int i, iTime;

    if (m_pClientList[iClientH] == nullptr) return;
    if ((dwMsgSize) <= 0) return;

    if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelShutup)
    {
        // Admin user levelÃ€ÃŒ Â³Â·Â¾Ã†Â¼Â­ Ã€ÃŒ Â±Ã¢Â´Ã‰Ã€Â» Â»Ã§Â¿Ã«Ã‡Ã’ Â¼Ã¶ Â¾Ã¸Â´Ã™.
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, 0);
        return;
    }

    memset(cName, 0, sizeof(cName));
    memset(cBuff, 0, sizeof(cBuff));
    memcpy(cBuff, pMsg, dwMsgSize);

    pStrTok = new class CStrTok(cBuff, seps);
    token = pStrTok->pGet();
    token = pStrTok->pGet();

    if (token != 0)
    {
        // tokenÃ€ÃŒ Â°Ã° ÃƒÂ¤Ã†ÃƒÃ€Â» ÂºÃ’Â°Â¡Â´Ã‰Ã‡ÃÂ°Ã” Â¸Â¸ÂµÃ© Â»Ã§Â¿Ã«Ã€Ãš Ã€ÃŒÂ¸Â§ 
        if (strlen(token) > 10)
            memcpy(cName, token, 10);
        else memcpy(cName, token, strlen(token));

        // Â´Ã™Ã€Â½ Ã…Ã¤Ã…Â«Ã€Âº ÃÂ¤ÃÃ¶ Â½ÃƒÃ…Â³ Â½ÃƒÂ°Â£. 
        token = pStrTok->pGet();
        if (token == 0)
            iTime = 0;
        else iTime = atoi(token);

        if (iTime < 0) iTime = 0;

        for (i = 1; i < DEF_MAXCLIENTS; i++)
            if ((m_pClientList[i] != 0) && (memcmp(m_pClientList[i]->m_cCharName, cName, 10) == 0))
            {

                m_pClientList[i]->m_iTimeLeft_ShutUp = iTime * 20; // 1Ã€ÃŒ 3ÃƒÃŠÂ´Ã™. 20Ã€ÃŒÂ¸Ã© 1ÂºÃ 
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_PLAYERSHUTUP, iTime, 0, 0, cName);
                SendNotifyMsg(0, i, DEF_NOTIFY_PLAYERSHUTUP, iTime, 0, 0, cName);

                // Admin Log
                wsprintf(G_cTxt, "GM Order(%s): Shutup PC(%s) (%d)Min", m_pClientList[iClientH]->m_cCharName,
                    m_pClientList[i]->m_cCharName, iTime);
                PutAdminLogFileList(G_cTxt);

                delete pStrTok;
                return;
            }
        // Ã‡Ã¶Ã€Ã§ ÃÂ¢Â¼Ã“ÃÃŸÃ€ÃŒ Â¾Ã†Â´ÃÂ´Ã™.
        memset(cBuff, 0, sizeof(cBuff));

        char * cp;
        uint16_t * wp;

        cp = (char *)cBuff;

        *cp = GSM_REQUEST_SHUTUPPLAYER;
        cp++;

        wp = (uint16_t *)cp;
        *wp = m_wServerID_GSS;
        cp += 2;

        wp = (uint16_t *)cp;
        *wp = (uint16_t)iClientH;
        cp += 2;

        memcpy(cp, cName, 10);
        cp += 10;

        wp = (uint16_t *)cp;
        *wp = iTime;
        cp += 2;

        memcpy(cp, m_pClientList[iClientH]->m_cCharName, 10);
        cp += 10;

        bStockMsgToGateServer(cBuff, 27);
    }

    delete pStrTok;
    return;
}

void CGame::SetPlayerReputation(int iClientH, char * pMsg, char cValue, uint32_t dwMsgSize)
{
    char   seps[] = "= \t\n";
    char * token, cName[11], cBuff[256];
    class  CStrTok * pStrTok;
    int i;

    if (m_pClientList[iClientH] == nullptr) return;
    if ((dwMsgSize) <= 0) return;
    if (m_pClientList[iClientH]->m_iLevel < 40) return;

    if ((m_pClientList[iClientH]->m_iTimeLeft_Rating != 0) || (m_pClientList[iClientH]->m_iPKCount != 0))
    {
        // Æò°¡¸¦ ³»¸± ½Ã°£ÀÌ µÇÁö ¾Ê¾Ò°Å³ª ¹üÁËÀÚ´Â Æò°¡¸¦ ³»¸± ¼ö ¾ø´Ù. ±â´ÉÀ» »ç¿ëÇÒ ¼ö ¾ø´Ù.
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_CANNOTRATING, m_pClientList[iClientH]->m_iTimeLeft_Rating, 0, 0, 0);
        return;
    }
    else if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0)
    {
        // ¼Ò¼Ó ¸¶À»ÀÌ ¾ø´Â »ç¶÷Àº ÆòÆÇÀ» ³»¸± ¼ö ¾ø´Ù.
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_CANNOTRATING, 0, 0, 0, 0);
        return;
    }

    memset(cName, 0, sizeof(cName));
    memset(cBuff, 0, sizeof(cBuff));
    memcpy(cBuff, pMsg, dwMsgSize);

    pStrTok = new class CStrTok(cBuff, seps);
    token = pStrTok->pGet();
    token = pStrTok->pGet();

    if (token != 0)
    {
        // tokenÀÌ °ð ÆòÆÇÀ» Æò°¡ÇÒ »ç¿ëÀÚ ÀÌ¸§ 
        if (strlen(token) > 10)
            memcpy(cName, token, 10);
        else memcpy(cName, token, strlen(token));

        for (i = 1; i < DEF_MAXCLIENTS; i++)
            if ((m_pClientList[i] != 0) && (memcmp(m_pClientList[i]->m_cCharName, cName, 10) == 0))
            {

                if (i != iClientH)
                {
                    // ÀÚ±â ÀÚ½ÅÀ» Æò°¡ÇÒ ¼ö´Â ¾ø´Ù.
                    if (cValue == 0)
                        m_pClientList[i]->m_iRating--;
                    else if (cValue == 1)
                        m_pClientList[i]->m_iRating++;

                    if (m_pClientList[i]->m_iRating > 500)  m_pClientList[i]->m_iRating = 500;
                    if (m_pClientList[i]->m_iRating < -500) m_pClientList[i]->m_iRating = -500;
                    // Æò°¡¸¦ ³»·ÈÀ¸¹Ç·Î ½Ã°£ °£°ÝÀ» Àç ÇÒ´ç. 
                    m_pClientList[iClientH]->m_iTimeLeft_Rating = 20 * 60;

                    SendNotifyMsg(0, i, DEF_NOTIFY_RATINGPLAYER, cValue, 0, 0, cName);
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_RATINGPLAYER, cValue, 0, 0, cName);

                    delete pStrTok;
                    return;
                }
            }
        // ÇöÀç Á¢¼ÓÁßÀÌ ¾Æ´Ï´Ù.
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_PLAYERNOTONGAME, 0, 0, 0, cName);
    }

    delete pStrTok;
    return;
}

void CGame::CalcExpStock(int iClientH)
{
    bool bIsLevelUp;
    class CItem * pItem;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;
    if (m_pClientList[iClientH]->m_iExpStock <= 0) return;
    // !!!!
    // v2.12 2002-2-6 ÃÃ¶ÃÂ¸ÂµÂµ Ã‡Ã¶Ã€Ã§ Â°Ã¦Ã‡Ã¨Ã„Â¡Â°Â¡ ÃƒÃ–Â´Ã« Â·Â¹ÂºÂ§ Â°Ã¦Ã‡Ã¨Ã„Â¡ÂºÂ¸Â´Ã™ Ã€Ã»Ã€Â¸Â¸Ã© Â°Ã¦Ã‡Ã¨Ã„Â¡Â¸Â¦ Â¾Ã²Ã€Â»Â¼Ã¶ Ã€Ã–Â°Ã” ÂºÂ¯Â°Ã¦ .. v2.15 Â»Ã¨ÃÂ¦. ÃÃ¶ÃÂ¸ÂµÂµ Â°Ã¦Ã‡Ã¨Ã„Â¡ Â¾Ã²Â´Ã‚Â´Ã™.
    //if ((m_pClientList[iClientH]->m_iLevel >= m_iPlayerMaxLevel) && (m_pClientList[iClientH]->m_iExp >= m_iLevelExpTable[m_iPlayerMaxLevel])) return;

    // Ã†Ã·Â»Ã³Â°Ãº Ã†Ã¤Â³ÃŽÃ†Â¼Â°Â¡ Â¾Ã¸Â´Ã‚ Â¸ÃŠ Ã€Â§Â¿Â¡ Â¼Â­ Ã€Ã–Â´Ã™Â¸Ã© Â°Ã¦Ã‡Ã¨Ã„Â¡Â´Ã‚ Â¿ÃƒÂ¶Ã³Â°Â¡ÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™.
    if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cType == DEF_MAPTYPE_NOPENALTY_NOREWARD)
    {
        m_pClientList[iClientH]->m_iExpStock = 0;
        return;
    }

    // Â±Ã—ÂµÂ¿Â¾Ãˆ ÃƒÃ Ã€Ã» ÂµÃ‡Â¾ÃºÂ´Ã¸ Â°Ã¦Ã‡Ã¨Ã„Â¡Â¸Â¦ Â°Ã¨Â»ÃªÃ‡Ã‘Â´Ã™. 
    m_pClientList[iClientH]->m_iExp += m_pClientList[iClientH]->m_iExpStock;
    m_pClientList[iClientH]->m_iAutoExpAmount += m_pClientList[iClientH]->m_iExpStock;
    m_pClientList[iClientH]->m_iExpStock = 0;

    if (bCheckLimitedUser(iClientH) == false)
    {
        // ÃƒÂ¼Ã‡Ã¨Ã†Ã‡ Â»Ã§Â¿Ã«Ã€Ãš ÃÂ¦Ã‡Ã‘Â¿Â¡ Ã‡Ã˜Â´Ã§ÂµÃ‡ÃÃ¶ Â¾ÃŠÃ€Â¸Â¸Ã© Â°Ã¦Ã‡Ã¨Ã„Â¡Â°Â¡ Â¿ÃƒÂ¶ÃºÂ´Ã™Â´Ã‚ Ã…Ã«ÂºÂ¸Â¸Â¦ Ã‡Ã‘Â´Ã™.
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_EXP, 0, 0, 0, 0);
    }
    // Â·Â¹ÂºÂ§Ã€ÃŒ Â¿ÃƒÂ¶ÃºÂ´Ã‚ÃÃ¶Â¸Â¦ Â°Ã‹Â»Ã§Ã‡Ã‘Â´Ã™.
    bIsLevelUp = bCheckLevelUp(iClientH);

    // v2.03 ÂºÂ¯Â°Ã¦ 9/1
    if ((bIsLevelUp == true) && (m_pClientList[iClientH]->m_iLevel <= 5))
    {
        // ÃƒÃŠÂºÂ¸Â¿Ã« Gold ÃÃ¶Â±Ãž. Â·Â¹ÂºÂ§ 1~5Â±Ã®ÃÃ¶ 100 Gold ÃÃ¶Â±Ãž.
        pItem = new class CItem;
        if (_bInitItemAttr(pItem, "Gold") == false)
        {
            delete pItem;
            return;
        }
        else pItem->m_dwCount = (uint32_t)100000;
        bAddItem(iClientH, pItem, 0);
    }

    // v2.03 ÂºÂ¯Â°Ã¦ 9/1
    if ((bIsLevelUp == true) && (m_pClientList[iClientH]->m_iLevel > 5) && (m_pClientList[iClientH]->m_iLevel <= 20))
    {
        // ÃƒÃŠÂºÂ¸Â¿Ã« Gold ÃÃ¶Â±Ãž. Â·Â¹ÂºÂ§ 5~20Â±Ã®ÃÃ¶ 300 Gold ÃÃ¶Â±Ãž.
        pItem = new class CItem;
        if (_bInitItemAttr(pItem, "Gold") == false)
        {
            delete pItem;
            return;
        }
        else pItem->m_dwCount = (uint32_t)100000;
        bAddItem(iClientH, pItem, 0);
    }
}

void CGame::___RestorePlayerRating(int iClientH)
{
    if (m_pClientList[iClientH] == nullptr) return;

    if (m_pClientList[iClientH]->m_iRating < -10000) m_pClientList[iClientH]->m_iRating = 0;
    if (m_pClientList[iClientH]->m_iRating > 10000) m_pClientList[iClientH]->m_iRating = 0;
}

int CGame::iGetExpLevel(int iExp)
{
    int i;

    // Â°Ã¦Ã‡Ã¨Ã„Â¡Â°Â¡ Â¾Ã®Â´Ã€ Â·Â¹ÂºÂ§ Â¼Ã¶ÃÃ˜Â¿Â¡ Â¼Ã“Ã‡ÃÂ´Ã‚ÃÃ¶ Ã†Ã‡Â´ÃœÃ‡Ã‘Â´Ã™.
    for (i = 1; i < 200; i++)
        if ((m_iLevelExpTable[i] <= iExp) && (m_iLevelExpTable[i + 1] > iExp)) return i;

    return 0;
}

void CGame::UserCommand_BanGuildsman(int iClientH, char * pData, uint32_t dwMsgSize)
{
    char   seps[] = "= \t\n";
    char * token, cTargetName[11], cBuff[256];
    class  CStrTok * pStrTok;
    int i;

    if (m_pClientList[iClientH] == nullptr) return;
    if ((dwMsgSize) <= 0) return;

    if (m_pClientList[iClientH]->m_iGuildRank != 0)
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_NOGUILDMASTERLEVEL, 0, 0, 0, 0);
        return;
    }

    memset(cTargetName, 0, sizeof(cTargetName));
    memset(cBuff, 0, sizeof(cBuff));
    memcpy(cBuff, pData, dwMsgSize);

    pStrTok = new class CStrTok(cBuff, seps);
    token = pStrTok->pGet();
    token = pStrTok->pGet();

    if (token != 0)
    {
        if (strlen(token) > 10)
            memcpy(cTargetName, token, 10);
        else memcpy(cTargetName, token, strlen(token));

        for (i = 1; i < DEF_MAXCLIENTS; i++)
            if ((m_pClientList[i] != 0) && (memcmp(m_pClientList[i]->m_cCharName, cTargetName, 10) == 0))
            {

                if (memcmp(m_pClientList[iClientH]->m_cGuildName, m_pClientList[i]->m_cGuildName, 20) != 0)
                {

                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_CANNOTBANGUILDMAN, 0, 0, 0, 0);
                    delete pStrTok;
                    return;
                }
                bSendMsgToLS(MSGID_REQUEST_UPDATEGUILDINFO_DELGUILDSMAN, i);

                SendGuildMsg(i, DEF_NOTIFY_DISMISSGUILDSMAN, 0, 0, 0);

                memset(m_pClientList[i]->m_cGuildName, 0, sizeof(m_pClientList[i]->m_cGuildName));
                strcpy(m_pClientList[i]->m_cGuildName, "NONE");
                m_pClientList[i]->m_iGuildRank = -1;
                m_pClientList[i]->m_iGuildGUID = -1;

                SendNotifyMsg(0, iClientH, DEF_NOTIFY_SUCCESSBANGUILDMAN, 0, 0, 0, 0);

                SendNotifyMsg(iClientH, i, DEF_COMMONTYPE_BANGUILD, 0, 0, 0, 0);

                SendEventToNearClient_TypeA(i, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);

                delete pStrTok;
                return;
            }
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_PLAYERNOTONGAME, 0, 0, 0, cTargetName);
    }

    delete pStrTok;
    return;
}

void CGame::UserCommand_DissmissGuild(int iClientH, char * pData, uint32_t dwMsgSize)
{

}

int CGame::iCreateFish(char cMapIndex, short sX, short sY, short sType, class CItem * pItem, int iDifficulty, uint32_t dwLastTime)
{
    int i, iDynamicHandle;

    // Â¸ÃŠÃ€Ã‡ Ã€Â§Ã„Â¡Â°Â¡ Â¹Â°Ã€ÃŽÃÃ¶ ÃˆÂ®Ã€ÃŽÃ‡Ã‘Â´Ã™. 
    if ((cMapIndex < 0) || (cMapIndex >= DEF_MAXMAPS)) return 0;
    if (m_pMapList[cMapIndex] == 0) return 0;
    if (m_pMapList[cMapIndex]->bGetIsWater(sX, sY) == false) return 0;

    for (i = 1; i < DEF_MAXFISHS; i++)
        if (m_pFish[i] == 0)
        {
            // ÂºÃ³ Â°Ã¸Â°Â£Â¿Â¡ Â¹Â°Â°Ã­Â±Ã¢Â¸Â¦ Â¸Â¸ÂµÃ§Â´Ã™.
            m_pFish[i] = new class CFish(cMapIndex, sX, sY, sType, pItem, iDifficulty);
            if (m_pFish[i] == 0) return 0;

            // Dynamic ObjectÂ¸Â¦ Â¹ÃŸÂ»Ã½Â½ÃƒÃ…Â²Â´Ã™. OwnerÂ¿Â¡ Fish Ã€ÃŽÂµÂ¦Â½ÂºÂ¸Â¦ Â³Ã–Â´Ã‚Â´Ã™.
            switch (pItem->m_sIDnum)
            {
                case 101: // Â»Â¡Â°Â­Â¹Â°Â°Ã­Â±Ã¢
                case 102: // ÃƒÃŠÂ·ÃÂ¹Â°Â°Ã­Â±Ã¢
                case 103: // Â³Ã«Â¶Ã»Â¹Â°Â°Ã­Â±Ã¢
                case 570:
                case 571:
                case 572:
                case 573:
                case 574:
                case 575:
                case 576:
                case 577:
                    iDynamicHandle = iAddDynamicObjectList(i, 0, DEF_DYNAMICOBJECT_FISH, cMapIndex, sX, sY, dwLastTime);
                    break;
                default:
                    // Â¹Â°Â°Ã­Â±Ã¢Â°Â¡ Â¾Ã†Â´Ã‘ Â´Ã™Â¸Â¥ Â¾Ã†Ã€ÃŒÃ…Ã› 
                    iDynamicHandle = iAddDynamicObjectList(i, 0, DEF_DYNAMICOBJECT_FISHOBJECT, cMapIndex, sX, sY, dwLastTime);
                    break;
            }

            if (iDynamicHandle == 0)
            {
                delete m_pFish[i];
                m_pFish[i] = 0;
                return 0;
            }
            m_pFish[i]->m_sDynamicObjectHandle = iDynamicHandle;
            m_pMapList[cMapIndex]->m_iCurFish++;

            return i;
        }

    return 0;
}

bool CGame::bDeleteFish(int iHandle, int iDelMode)
{
    int i, iH;
    uint32_t dwTime;

    if (m_pFish[iHandle] == 0) return false;

    dwTime = timeGetTime();

    // Â¿Â¬Â°Ã¼ÂµÃ‡Â¾Ã® Ã€Ã–Â´Ã‚ DynamicObjectÂ¸Â¦ Â»Ã¨ÃÂ¦Ã‡Ã‘Â´Ã™.
    iH = m_pFish[iHandle]->m_sDynamicObjectHandle;

    if (m_pDynamicObjectList[iH] != 0)
    {
        SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, DEF_MSGTYPE_REJECT, m_pDynamicObjectList[iH]->m_cMapIndex, m_pDynamicObjectList[iH]->m_sX, m_pDynamicObjectList[iH]->m_sY, m_pDynamicObjectList[iH]->m_sType, iH, 0);
        // Â¸ÃŠÂ¿Â¡Â¼Â­ Â»Ã¨ÃÂ¦Ã‡Ã‘Â´Ã™.
        m_pMapList[m_pDynamicObjectList[iH]->m_cMapIndex]->SetDynamicObject(0, 0, m_pDynamicObjectList[iH]->m_sX, m_pDynamicObjectList[iH]->m_sY, dwTime);
        m_pMapList[m_pDynamicObjectList[iH]->m_cMapIndex]->m_iCurFish--;

        delete m_pDynamicObjectList[iH];
        m_pDynamicObjectList[iH] = 0;
    }

    // Ã€ÃŒ Â¹Â°Â°Ã­Â±Ã¢Â¿Ã Â¿Â¬Â°Ã¡ÂµÃ‡Â¾Ã® Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®ÂµÃ©Â¿Â¡Â°Ã” Â¹Â°Â°Ã­Â±Ã¢Â°Â¡ Â»Ã§Â¶Ã³ÃÂ® Â³Â¬Â½ÃƒÂ°Â¡ ÃƒÃ«Â¼Ã’ÂµÃ‡Â¾ÃºÃ€Â½Ã€Â» Â¾Ã‹Â·ÃÃÃ˜Â´Ã™. 
    for (i = 1; i < DEF_MAXCLIENTS; i++)
    {
        if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_bIsInitComplete == true) &&
            (m_pClientList[i]->m_iAllocatedFish == iHandle))
        {
            // Â¸ÃžÂ½ÃƒÃÃ¶ Ã€Ã¼Â¼Ã› 
            SendNotifyMsg(0, i, DEF_NOTIFY_FISHCANCELED, iDelMode, 0, 0, 0);
            ClearSkillUsingStatus(i); // v1.4 Â³Â¬Â½Ãƒ Â½ÂºÃ…Â³Ã€Â» Ã…Â¬Â¸Â®Â¾Ã®Ã‡Ã‘Â´Ã™.
        }
    }

    // Â»Ã¨ÃÂ¦Ã‡Ã‘Â´Ã™.
    delete m_pFish[iHandle];
    m_pFish[iHandle] = 0;

    return true;
}

int CGame::iCheckFish(int iClientH, char cMapIndex, short dX, short dY)
{
    int i;
    short sDistX, sDistY;

    if (m_pClientList[iClientH] == nullptr) return 0;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return 0;

    if ((cMapIndex < 0) || (cMapIndex >= DEF_MAXMAPS)) return 0;

    // Â¸ÃŠÃ€Ã‡ Ã†Â¯ÃÂ¤ Ã€Â§Ã„Â¡ Â³Â»Â¿Â¡ Â¹Â°Â°Ã­Â±Ã¢ Â´Ã™Ã€ÃŒÂ³ÂªÂ¹Ã Â¿Ã€ÂºÃªÃÂ§Ã†Â®Â°Â¡ Ã€Ã–Â´Ã‚ÃÃ¶ Ã†Ã‡Â´ÃœÃ‡Ã‘Â´Ã™. 
    for (i = 1; i < DEF_MAXDYNAMICOBJECTS; i++)
        if (m_pDynamicObjectList[i] != 0)
        {
            sDistX = abs(m_pDynamicObjectList[i]->m_sX - dX);
            sDistY = abs(m_pDynamicObjectList[i]->m_sY - dY);

            if ((m_pDynamicObjectList[i]->m_cMapIndex == cMapIndex) &&
                ((m_pDynamicObjectList[i]->m_sType == DEF_DYNAMICOBJECT_FISH) || (m_pDynamicObjectList[i]->m_sType == DEF_DYNAMICOBJECT_FISHOBJECT)) &&
                (sDistX <= 2) && (sDistY <= 2))
            {
                // Â¹Â°Â°Ã­Â±Ã¢ Â´Ã™Ã€ÃŒÂ³ÂªÂ¹Ã Â¿Ã€ÂºÃªÃÂ§Ã†Â®Â¸Â¦ ÃƒÂ£Â¾Ã’Â´Ã™. Ã€ÃŒÃÂ¦ Ã€ÃŒ Â´Ã™Ã€ÃŒÂ³ÂªÂ¹Ã Â¿Ã€ÂºÃªÃÂ§Ã†Â® Ã€ÃŽÂµÂ¦Â½ÂºÂ¸Â¦ Â¼Ã’Ã€Â¯Ã‡Ã‘ FishÃ€Ã‡ Ã€ÃŽÂµÂ¦Â½ÂºÂ¸Â¦ Â¹ÃÃˆÂ¯Ã‡Ã‘Â´Ã™. 

                if (m_pFish[m_pDynamicObjectList[i]->m_sOwner] == 0) return 0;
                if (m_pFish[m_pDynamicObjectList[i]->m_sOwner]->m_sEngagingCount >= DEF_MAXENGAGINGFISH) return 0;

                // Ã€ÃŒÂ¹ÃŒ Â³Â¬Â½ÃƒÂ¸Ã°ÂµÃ¥Â¿Â¡ ÂµÃ©Â¾Ã®Â°Â¡ Ã€Ã–Â´Ã‚ Ã„Â³Â¸Â¯Ã…ÃÂ´Ã‚ ÃÃŸÂºÂ¹ ÂµÂ¿Ã€Ã›Ã€ÃŒ ÂºÃ’Â°Â¡Â´Ã‰.
                if (m_pClientList[iClientH]->m_iAllocatedFish != 0) return 0;
                if (m_pClientList[iClientH]->m_cMapIndex != cMapIndex) return 0;
                // Ã€ÃŒÃÂ¦ Ã„Â³Â¸Â¯Ã…ÃÂ¿Â¡Â°Ã” Â³Â¬Â½Ãƒ Â¸Ã°ÂµÃ¥Â¸Â¦ Â½ÃƒÃ€Ã›Ã‡Ã’ Â°ÃÃ€Â» Ã‡Ã’Â´Ã§Ã‡Ã‘Â´Ã™.
                m_pClientList[iClientH]->m_iAllocatedFish = m_pDynamicObjectList[i]->m_sOwner;
                m_pClientList[iClientH]->m_iFishChance = 1;
                // Ã€ÃŒ Ã„Â³Â¸Â¯Ã…ÃÂ´Ã‚ Â³Â¬Â½Ãƒ Â½ÂºÃ…Â³Ã€Â» Â»Ã§Â¿Ã«ÃÃŸÃ€Ã“Ã€Â» Â¼Â³ÃÂ¤.
                m_pClientList[iClientH]->m_bSkillUsingStatus[1] = true;

                SendNotifyMsg(0, iClientH, DEF_NOTIFY_EVENTFISHMODE, (m_pFish[m_pDynamicObjectList[i]->m_sOwner]->m_pItem->m_wPrice / 2), m_pFish[m_pDynamicObjectList[i]->m_sOwner]->m_pItem->m_sSprite,
                    m_pFish[m_pDynamicObjectList[i]->m_sOwner]->m_pItem->m_sSpriteFrame, m_pFish[m_pDynamicObjectList[i]->m_sOwner]->m_pItem->m_cName);

                // Ã„Â«Â¿Ã®Ã†Â® ÃÃµÂ°Â¡ 
                m_pFish[m_pDynamicObjectList[i]->m_sOwner]->m_sEngagingCount++;

                return i;
            }
        }

    return 0;
}

void CGame::FishProcessor()
{
    int i, iSkillLevel, iResult, iChangeValue;

    // ÀÌº¥Æ® ³¬½Ã ¸ðµå°¡ ÇÒ´çµÈ ÇÃ·¹ÀÌ¾îµéÀ» Ã³¸®ÇÑ´Ù.
    for (i = 1; i < DEF_MAXCLIENTS; i++)
    {
        if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_bIsInitComplete == true) &&
            (m_pClientList[i]->m_iAllocatedFish != 0))
        {

            if (m_pFish[m_pClientList[i]->m_iAllocatedFish] == 0) break;

            // ÀÌ ÇÃ·¹ÀÌ¾î¿¡°Ô ÇÒ´çµÈ ¹°°í±â°¡ ÀÖ´Ù. ³¬À» È®·üÀ» Àç°è»êÇÏ¿© Åëº¸ÇØ ÁØ´Ù. 
            // ³¬½Ã ±â¼úÀÇ ½ºÅ³ 
            iSkillLevel = m_pClientList[i]->m_cSkillMastery[1];
            // ¹°°í±âÀÇ ³­ÀÌµµ¿¡ µû¶ó ½ºÅ³ ·¹º§ÀÌ ¶³¾îÁø´Ù. 
            iSkillLevel -= m_pFish[m_pClientList[i]->m_iAllocatedFish]->m_iDifficulty;
            if (iSkillLevel <= 0) iSkillLevel = 1;

            iChangeValue = iSkillLevel / 10;
            if (iChangeValue <= 0) iChangeValue = 1;
            iChangeValue = iDice(1, iChangeValue);

            iResult = iDice(1, 100);
            if (iSkillLevel > iResult)
            {
                // ½ºÅ³ ÁÖ»çÀ§ ±¼¸² ¼º°ø. ³¬À» È®·ü Áõ°¡.
                m_pClientList[i]->m_iFishChance += iChangeValue;
                if (m_pClientList[i]->m_iFishChance > 99) m_pClientList[i]->m_iFishChance = 99;

                SendNotifyMsg(0, i, DEF_NOTIFY_FISHCHANCE, m_pClientList[i]->m_iFishChance, 0, 0, 0);
            }
            else if (iSkillLevel < iResult)
            {
                // ½ºÅ³ ÁÖ»çÀ§ ±¼¸² ½ÇÆÐ. ³¬À» È®·ü °¨¼Ò.	
                m_pClientList[i]->m_iFishChance -= iChangeValue;
                if (m_pClientList[i]->m_iFishChance < 1) m_pClientList[i]->m_iFishChance = 1;

                SendNotifyMsg(0, i, DEF_NOTIFY_FISHCHANCE, m_pClientList[i]->m_iFishChance, 0, 0, 0);
            }
        }
    }
}

void CGame::FishGenerator()
{
    int i, iP, tX, tY, iRet;
    char  cItemName[21];
    short sDifficulty;
    uint32_t dwLastTime;
    class CItem * pItem;

    for (i = 0; i < DEF_MAXMAPS; i++)
    {
        if ((iDice(1, 10) == 5) && (m_pMapList[i] != 0) &&
            (m_pMapList[i]->m_iCurFish < m_pMapList[i]->m_iMaxFish))
        {

            iP = iDice(1, m_pMapList[i]->m_iTotalFishPoint) - 1;
            if ((m_pMapList[i]->m_FishPointList[iP].x == -1) || (m_pMapList[i]->m_FishPointList[iP].y == -1)) break;

            tX = m_pMapList[i]->m_FishPointList[iP].x + (iDice(1, 3) - 2);
            tY = m_pMapList[i]->m_FishPointList[iP].y + (iDice(1, 3) - 2);

            pItem = new class CItem;
            if (pItem == 0) break;

            // Â¸Â¸ÂµÃ© Â¹Â°Â°Ã­Â±Ã¢ ÃÂ¾Â·Ã¹Â¿Ã Â³Â­Ã€ÃŒÂµÂµ, ÃÃ¶Â¼Ã“ Â½ÃƒÂ°Â£Ã€Â» Â°Ã¡ÃÂ¤Ã‡Ã‘Â´Ã™. 
            memset(cItemName, 0, sizeof(cItemName));
            switch (iDice(1, 9))
            {
                case 1:   strcpy(cItemName, "RedCarp"); sDifficulty = iDice(1, 10) + 20; break;
                case 2:   strcpy(cItemName, "GreenCarp"); sDifficulty = iDice(1, 5) + 10; break;
                case 3:   strcpy(cItemName, "GoldCarp"); sDifficulty = iDice(1, 10) + 1;  break;
                case 4:   strcpy(cItemName, "CrucianCarp"); sDifficulty = 1;  break;
                case 5:   strcpy(cItemName, "BlueSeaBream"); sDifficulty = iDice(1, 15) + 1;  break;
                case 6:   strcpy(cItemName, "RedSeaBream"); sDifficulty = iDice(1, 18) + 1;  break;
                case 7:   strcpy(cItemName, "Salmon"); sDifficulty = iDice(1, 12) + 1;  break;
                case 8:   strcpy(cItemName, "GrayMullet"); sDifficulty = iDice(1, 10) + 1;  break;
                case 9:
                    // Â°Â¡Â²Ã» Â°Â¡Â´Ã™ Â³Â¬Ã€Â» Â¼Ã¶ Ã€Ã–Â´Ã‚ Ã†Â¯Â¼Ã¶ Â¾Ã†Ã€ÃŒÃ…Ã› 
                    switch (iDice(1, 150))
                    {
                        case 1:
                        case 2:
                        case 3:
                            strcpy(cItemName, "PowerGreenPotion");
                            sDifficulty = iDice(5, 4) + 30;
                            break;

                        case 10:
                        case 11:
                            strcpy(cItemName, "SuperPowerGreenPotion");
                            sDifficulty = iDice(5, 4) + 50;
                            break;

                        case 20:
                            strcpy(cItemName, "Dagger+2");
                            sDifficulty = iDice(5, 4) + 30;
                            break;

                        case 30:
                            strcpy(cItemName, "LongSword+2");
                            sDifficulty = iDice(5, 4) + 40;
                            break;

                        case 40:
                            strcpy(cItemName, "Scimitar+2");
                            sDifficulty = iDice(5, 4) + 50;
                            break;

                        case 50:
                            strcpy(cItemName, "Rapier+2");
                            sDifficulty = iDice(5, 4) + 60;
                            break;

                        case 60:
                            strcpy(cItemName, "Flameberge+2");
                            sDifficulty = iDice(5, 4) + 60;
                            break;

                        case 70:
                            strcpy(cItemName, "WarAxe+2");
                            sDifficulty = iDice(5, 4) + 50;
                            break;

                        case 90:
                            strcpy(cItemName, "Ruby");
                            sDifficulty = iDice(5, 4) + 40;
                            break;

                        case 95:
                            strcpy(cItemName, "Diamond");
                            sDifficulty = iDice(5, 4) + 40;
                            break;
                    }
                    break;
            }
            dwLastTime = (60000 * 10) + (iDice(1, 3) - 1) * (60000 * 10);

            if (_bInitItemAttr(pItem, cItemName) == true)
            {
                iRet = iCreateFish(i, tX, tY, 1, pItem, sDifficulty, dwLastTime);
            }
            else
            {
                delete pItem;
                pItem = 0;
            }
        }
    }
}

int CGame::_iCalcPlayerNum(char cMapIndex, short dX, short dY, char cRadius)
{
    int ix, iy, iRet;
    class CTile * pTile;

    // ÃƒÂ³Â¸Â® Â¼Ã“ÂµÂµÂ¸Â¦ Â³Ã´Ã€ÃŒÂ±Ã¢ Ã€Â§Ã‡Ã˜ Ã‡Ã”Â¼Ã¶Â¸Â¦ Ã„ÃÃ‡ÃÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™.
    if ((cMapIndex < 0) || (cMapIndex > DEF_MAXMAPS)) return 0;
    if (m_pMapList[cMapIndex] == 0) return 0;

    iRet = 0;
    for (ix = dX - cRadius; ix <= dX + cRadius; ix++)
        for (iy = dY - cRadius; iy <= dY + cRadius; iy++)
        {
            if ((ix < 0) || (ix >= m_pMapList[cMapIndex]->m_sSizeX) ||
                (iy < 0) || (iy >= m_pMapList[cMapIndex]->m_sSizeY))
            {
                // ÃÃ‚Ã‡Â¥Â¸Â¦ Â¹Ã¾Â¾Ã®Â³ÂªÂ¹Ã‡Â·ÃŽ ÃƒÂ³Â¸Â®Ã‡ÃÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™.	
            }
            else
            {
                pTile = (class CTile *)(m_pMapList[cMapIndex]->m_pTile + ix + iy * m_pMapList[cMapIndex]->m_sSizeY);
                if ((pTile->m_sOwner != 0) && (pTile->m_cOwnerClass == DEF_OWNERTYPE_PLAYER))
                    iRet++;
            }
        }

    return iRet;
}

/*********************************************************************************************************************
**  int CGame::iGetWhetherMagicBonusEffect(short sType, char cWheatherStatus)										**
**  description			:: checks for a weather bonus when magic is cast											**
**  last updated		:: November 20, 2004; 10:34 PM; Hypnotoad													**
**	return value		:: int																						**
*********************************************************************************************************************/
int CGame::iGetWhetherMagicBonusEffect(short sType, char cWheatherStatus)
{
    int iWheatherBonus;

    iWheatherBonus = 0;
    switch (cWheatherStatus)
    {
        case 0: break;
        case 1:
        case 2:
        case 3:
            switch (sType)
            {
                case 10:
                case 37:
                case 43:
                case 51:
                    iWheatherBonus = 1;
                    break;

                case 20:
                case 30:
                    iWheatherBonus = -1;
                    break;
            }
            break;
    }
    return iWheatherBonus;
}

int CGame::iGetPlayerRelationship(int iClientH, int iOpponentH)
{
    int iRet;

    if (m_pClientList[iClientH] == nullptr) return 0;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return 0;

    if (m_pClientList[iOpponentH] == 0) return 0;
    if (m_pClientList[iOpponentH]->m_bIsInitComplete == false) return 0;

    iRet = 0;

    if (m_pClientList[iClientH]->m_iPKCount != 0)
    {
        // ÂºÂ»Ã€ÃŽÃ€ÃŒ Â¹Ã¼ÃÃ‹Ã€ÃšÃ€ÃŒÃ‡ÃÂ¸Ã© Â»Ã³Â´Ã«Â¹Ã¦Ã€ÃŒ Â¸Â¶Ã€Â»Ã€ÃŒ Â°Â°Ã€Âº Â°Ã¦Â¿Ã¬ Ã€Â§Ã‡Ã¹. Â´Ã™Â¸Â£Â¸Ã© Ã€Ã» 
        if ((memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[iOpponentH]->m_cLocation, 10) == 0) &&
            (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) != 0) && (memcmp(m_pClientList[iOpponentH]->m_cLocation, "NONE", 4) != 0))
        {
            iRet = 7; // Ã€Â§Ã‡Ã¹ 
        }
        else iRet = 2; // Ã€Ã»   
    }
    else if (m_pClientList[iOpponentH]->m_iPKCount != 0)
    {
        // Â»Ã³Â´Ã«Â¹Ã¦Ã€ÃŒ Â¹Ã¼ÃÃ‹Ã€ÃšÃ€ÃŒÃ‡ÃÂ¸Ã©
        if ((memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[iOpponentH]->m_cLocation, 10) == 0) &&
            (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) != 0))
            iRet = 6; // Â¸Â¶Ã€Â»Ã€ÃŒ Â°Â°Ã€Â¸Â¸Ã© PK
        else iRet = 2; // Â´Ã™Â¸Â£Â¸Ã© Â±Ã—Â³Ã‰ Ã€Ã» 
    }
    else
    {
        if (m_pClientList[iClientH]->m_cSide != m_pClientList[iOpponentH]->m_cSide)
        {
            if ((m_pClientList[iClientH]->m_cSide != 0) && (m_pClientList[iOpponentH]->m_cSide != 0))
            {
                // ÂµÃ‘ Â´Ã™ 0(Traveler)Ã€ÃŒ Â¾Ã†Â´ÃÂ¸Ã© Ã€Ã»Ã€ÃŒÂ´Ã™.
                iRet = 2;
            }
            else
            {
                iRet = 0;
            }
        }
        else
        {
            // Â¾Ã†Â±ÂºÃ€ÃŒÂ´Ã™. Â±Ã¦ÂµÃ¥Â¿Ã¸Â¿Â©ÂºÃŽÂ¸Â¦ Ã†Ã‡Â´Ãœ.
            if ((memcmp(m_pClientList[iClientH]->m_cGuildName, m_pClientList[iOpponentH]->m_cGuildName, 20) == 0) &&
                (memcmp(m_pClientList[iClientH]->m_cGuildName, "NONE", 4) != 0))
            {
                // Â±Ã¦ÂµÃ¥ Ã€ÃŒÂ¸Â§Ã€ÃŒ Â°Â°Â´Ã™. 
                if (m_pClientList[iOpponentH]->m_iGuildRank == 0)
                    iRet = 5;	// Â±Ã¦ÂµÃ¥ Â·Â©Ã…Â©Â°Â¡ 0. Â±Ã¦ÂµÃ¥Â¸Â¶Â½ÂºÃ…ÃÃ€ÃŒÂ´Ã™.
                else iRet = 3;	// Â°Â°Ã€Âº Â±Ã¦ÂµÃ¥Â¿Ã¸
            }
            else
                if ((memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[iOpponentH]->m_cLocation, 10) == 0) &&
                    (memcmp(m_pClientList[iClientH]->m_cGuildName, "NONE", 4) != 0) &&
                    (memcmp(m_pClientList[iOpponentH]->m_cGuildName, "NONE", 4) != 0) &&
                    (memcmp(m_pClientList[iClientH]->m_cGuildName, m_pClientList[iOpponentH]->m_cGuildName, 20) != 0))
                {
                    // Â±Ã¦ÂµÃ¥ Ã€Â§Ã„Â¡Â´Ã‚ Â°Â°Â°Ã­ Â±Ã¦ÂµÃ¥ Ã€ÃŒÂ¸Â§Ã€ÃŒ Â´Ã™Â¸Â£Â´Ã™.
                    iRet = 4; // Â´Ã™Â¸Â¥ Â±Ã¦ÂµÃ¥Â¿Ã¸ 
                }
                else iRet = 1; // Â±Ã—Â³Ã‰ Â°Â°Ã€ÂºÃ†Ã­ 
        }
    }

    return iRet;
}

int CGame::iGetPlayerABSStatus(int iWhatH, int iRecvH)
{
    int iRet;

    if (m_pClientList[iWhatH] == 0) return 0;
    if (m_pClientList[iRecvH] == 0) return 0;

    iRet = 0;

    /*if (m_pMapList[m_pClientList[iRecvH]->m_cMapIndex]->m_bIsFightZone == true &&
        m_pClientList[iWhatH]->m_iGuildRank != -1 && m_pClientList[iRecvH]->m_iGuildRank != -1 &&
        memcmp(m_pClientList[iWhatH]->m_cGuildName,m_pClientList[iRecvH]->m_cGuildName,20) != 0) {
            iRet = 8;
        }*/

    if (m_pClientList[iWhatH]->m_iPKCount != 0)
    {
        iRet = 8;
    }

    if (m_pClientList[iWhatH]->m_cSide != 0)
    {
        iRet = iRet | 4;
    }

    if (m_pClientList[iWhatH]->m_cSide == 1)
    {
        iRet = iRet | 2;
    }

    if (m_pClientList[iWhatH]->m_bIsPlayerCivil == true)
        iRet = iRet | 1;

    return iRet;
}

int CGame::iGetNpcRelationship(int iWhatH, int iRecvH)
{
    int iRet;

    if (m_pClientList[iRecvH] == 0) return 0;
    if (m_pNpcList[iWhatH] == 0) return 0;

    iRet = 0;
    switch (m_pNpcList[iWhatH]->m_cSide)
    {
        case 10: iRet |= 8; //Fixed, Original 15 
        case 1:  iRet = (iRet | 4) | 2; //Fixed, Original 1 
        case 2:  iRet |= 4; //Fixed, Orignal 8 
    }

    return iRet;
}

int CGame::iGetNpcRelationship_SendEvent(int iNpcH, int iOpponentH)
{
    int iRet;

    if (m_pClientList[iOpponentH] == 0) return 0;
    if (m_pClientList[iOpponentH]->m_bIsInitComplete == false) return 0;

    if (m_pNpcList[iNpcH] == 0) return 0;

    iRet = 0;

    if (m_pClientList[iOpponentH]->m_iPKCount != 0)
    {
        if (m_pNpcList[iNpcH]->m_cSide == m_pClientList[iOpponentH]->m_cSide)
            iRet = 7;
        else iRet = 2;
    }
    else
    {
        if (m_pNpcList[iNpcH]->m_cSide != m_pClientList[iOpponentH]->m_cSide)
        {
            if (m_pNpcList[iNpcH]->m_cSide == 10)
                iRet = 2;
            else
                if (m_pNpcList[iNpcH]->m_cSide == 0) iRet = 0;
                else
                    if (m_pClientList[iOpponentH]->m_cSide == 0)
                        iRet = 0;
                    else iRet = 2;

        }
        else iRet = 1; // Â°Â°Ã€Âº Ã†Ã­ 
    }

    return iRet;
}

int CGame::iGetMapIndex(char * pMapName)
{
    int i, iMapIndex;
    char cTmpName[256];

    memset(cTmpName, 0, sizeof(cTmpName));
    strcpy(cTmpName, pMapName);

    iMapIndex = -1;
    for (i = 0; i < DEF_MAXMAPS; i++)
        if (m_pMapList[i] != 0)
        {
            if (memcmp(m_pMapList[i]->m_cName, pMapName, 10) == 0)
                iMapIndex = i;
        }

    return iMapIndex;
}

int CGame::_iForcePlayerDisconect(int iNum)
{
    int i, iCnt;

    iCnt = 0;
    for (i = 1; i < DEF_MAXCLIENTS; i++)
        if (m_pClientList[i] != 0)
        {
            if (m_pClientList[i]->m_bIsInitComplete == true)
                DeleteClient(i, true, true);
            else DeleteClient(i, false, false);
            iCnt++;
            if (iCnt >= iNum) break;
        }

    return iCnt;
}

void CGame::NpcRequestAssistance(int iNpcH)
{
    int ix, iy, sX, sY;
    short sOwnerH;
    char  cOwnerType;

    // iNpc Â±Ã™Â¹Ã¦Â¿Â¡ Ã€Ã–Â´Ã‚ Ã€Ã¼Ã…ÃµÃÃŸÃ€ÃŒ Â¾Ã†Â´Ã‘ NPCÂ¿Â¡Â°Ã” ÂµÂµÂ¿Ã²Ã€Â» Â¿Ã¤ÃƒÂ»Ã‡Ã‘Â´Ã™. 
    if (m_pNpcList[iNpcH] == 0) return;

    sX = m_pNpcList[iNpcH]->m_sX;
    sY = m_pNpcList[iNpcH]->m_sY;

    for (ix = sX - 8; ix <= sX + 8; ix++)
        for (iy = sY - 8; iy <= sY + 8; iy++)
        {
            m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
            if ((sOwnerH != 0) && (m_pNpcList[sOwnerH] != 0) && (cOwnerType == DEF_OWNERTYPE_NPC) &&
                (iNpcH != sOwnerH) && (m_pNpcList[sOwnerH]->m_cSide == m_pNpcList[iNpcH]->m_cSide) &&
                (m_pNpcList[sOwnerH]->m_bIsPermAttackMode == false) && (m_pNpcList[sOwnerH]->m_cBehavior == DEF_BEHAVIOR_MOVE))
            {

                // ÃÂ¶Â°Ã‡Â¿Â¡ ÂºÃŽÃ‡Ã•Ã‡ÃÂ´Ã‚ NPCÂ¸Â¦ ÃƒÂ£Â¾Ã’Â´Ã™. 
                m_pNpcList[sOwnerH]->m_cBehavior = DEF_BEHAVIOR_ATTACK;
                m_pNpcList[sOwnerH]->m_sBehaviorTurnCount = 0;
                m_pNpcList[sOwnerH]->m_iTargetIndex = m_pNpcList[iNpcH]->m_iTargetIndex;
                m_pNpcList[sOwnerH]->m_cTargetType = m_pNpcList[iNpcH]->m_cTargetType;

                return;
            }
        }
}

void CGame::ForceDisconnectAccount(char * pAccountName, uint16_t wCount)
{
    int i;

    // ÃÃŸÂºÂ¹ÂµÃˆ Â°Ã¨ÃÂ¤Ã€Â» Â°Â®Â°Ã­Ã€Ã–Â´Ã‚ Ã„Â³Â¸Â¯Ã…ÃÂ¸Â¦ Â»Ã¨ÃÂ¦Ã‡Ã‘Â´Ã™. 
    for (i = 1; i < DEF_MAXCLIENTS; i++)
        if ((m_pClientList[i] != 0) && (memcmp(m_pClientList[i]->m_cAccountName, pAccountName, 10) == 0))
        {
            wsprintf(G_cTxt, "<%d> Force disconnect account: CharName(%s) AccntName(%s) Count(%d)", i, m_pClientList[i]->m_cCharName, m_pClientList[i]->m_cAccountName, wCount);
            log->info(G_cTxt);

            // v1.42 Ã„Â«Â¿Ã®Ã†Ãƒ Ã‡ÃƒÂ·Â¡Â±Ã—Â¸Â¦ ÃˆÂ°Â¼ÂºÃˆÂ­ Ã‡Ã˜Â¾ÃŸ Ã‡Ã‘Â´Ã™. 
            //DeleteClient(i, true, true);

            //v1.4312
            SendNotifyMsg(0, i, DEF_NOTIFY_FORCEDISCONN, wCount, 0, 0, 0);
        }
}

bool CGame::bOnClose()
{
    if (m_bIsServerShutdowned == false)
        if (MessageBox(0, "Player data not saved! Shutdown server now?", m_cServerName, MB_ICONEXCLAMATION | MB_YESNO) == IDYES) return true;
        else return false;
    else return true;

    return false;
}

void CGame::_CheckAttackType(int iClientH, short * spType)
{
    uint16_t wType;

    if (m_pClientList[iClientH] == nullptr) return;
    wType = ((m_pClientList[iClientH]->m_sAppr2 & 0x0FF0) >> 4);

    switch (*spType)
    {
        case 2:
            // ÃƒË†Ã‚Â­Ã‚Â»ÃƒÂ¬Ãƒâ‚¬ÃƒÅ’ Ã‚Â¾ÃƒÂ¸Ã‚Â´Ãƒâ„¢Ã‚Â¸ÃƒÂ© ÃƒË†Ã‚Â­Ã‚Â»ÃƒÂ¬ EffectÃ‚Â´Ãƒâ€š Ã‚Â³Ãƒâ€“ÃƒÂÃƒÂ¶ Ã‚Â¾ÃƒÅ Ã‚Â´Ãƒâ€šÃ‚Â´Ãƒâ„¢. 
            if (m_pClientList[iClientH]->m_cArrowIndex == -1) *spType = 0;
            // ÃƒË†Ã‚Â° Ã‚Â°ÃƒÂ¸Ã‚Â°ÃƒÂÃƒâ‚¬ÃƒÅ½Ã‚ÂµÃ‚Â¥ Ãƒâ‚¬ÃƒÂ¥Ãƒâ€šÃƒÂ¸Ãƒâ€¡Ãƒâ€˜ Ã‚Â¹Ã‚Â«Ã‚Â±ÃƒÂ¢Ã‚Â°Ã‚Â¡ ÃƒË†Ã‚Â°Ãƒâ‚¬ÃƒÅ’ Ã‚Â¾Ãƒâ€ Ã‚Â´ÃƒÂÃ‚Â´Ãƒâ„¢. Ãƒâ‚¬ÃƒÂÃ‚Â¹ÃƒÂ Ã‚Â°ÃƒÂ¸Ã‚Â°ÃƒÂÃƒâ‚¬Ã‚Â¸Ã‚Â·ÃƒÅ½ Ãƒâ‚¬ÃƒÂ¼ÃƒË†Ã‚Â¯.
            if (wType < 40) *spType = 1;
            break;

        case 20:
            // Ã‚Â¸Ãƒâ€¡Ã‚Â¼Ãƒâ€¢ Ãƒâ€¡ÃƒÅ Ã‚Â»ÃƒÂ¬Ã‚Â±ÃƒÂ¢ 
            if (m_pClientList[iClientH]->m_iSuperAttackLeft <= 0)  *spType = 1;
            if (m_pClientList[iClientH]->m_cSkillMastery[5] < 100) *spType = 1;
            break;

        case 21:
            // Ã‚Â´ÃƒÅ“Ã‚Â°Ãƒâ€¹ Ãƒâ€¡ÃƒÅ Ã‚Â»ÃƒÂ¬Ã‚Â±ÃƒÂ¢ 
            if (m_pClientList[iClientH]->m_iSuperAttackLeft <= 0)  *spType = 1;
            if (m_pClientList[iClientH]->m_cSkillMastery[7] < 100) *spType = 1;
            break;

        case 22:
            // Ãƒâ€ ÃƒÂ¦Ã‚Â½ÃƒÅ’ Ãƒâ€¡ÃƒÅ Ã‚Â»ÃƒÂ¬Ã‚Â±ÃƒÂ¢ 
            if (m_pClientList[iClientH]->m_iSuperAttackLeft <= 0)  *spType = 1;
            if (m_pClientList[iClientH]->m_cSkillMastery[9] < 100) *spType = 1;
            break;

        case 23:
            // Ãƒâ‚¬ÃƒÂ¥Ã‚Â°Ãƒâ€¹ Ãƒâ€¡ÃƒÅ Ã‚Â»ÃƒÂ¬Ã‚Â±ÃƒÂ¢ 
            if (m_pClientList[iClientH]->m_iSuperAttackLeft <= 0)   *spType = 1;
            if (m_pClientList[iClientH]->m_cSkillMastery[8] < 100) *spType = 1;
            break;

        case 24:
            // Ã‚ÂµÃ‚ÂµÃ‚Â³Ã‚Â¢ Ãƒâ€¡ÃƒÅ Ã‚Â»ÃƒÂ¬Ã‚Â±ÃƒÂ¢ 
            if (m_pClientList[iClientH]->m_iSuperAttackLeft <= 0)  *spType = 1;
            if (m_pClientList[iClientH]->m_cSkillMastery[10] < 100) *spType = 1;
            break;

        case 25:
            // ÃƒË†Ã‚Â° Ãƒâ€¡ÃƒÅ Ã‚Â»ÃƒÂ¬Ã‚Â±ÃƒÂ¢ 
            if (m_pClientList[iClientH]->m_iSuperAttackLeft <= 0)  *spType = 2;
            if (m_pClientList[iClientH]->m_cSkillMastery[6] < 100) *spType = 2;
            if (m_pClientList[iClientH]->m_cArrowIndex == -1)      *spType = 0;
            // ÃƒË†Ã‚Â° Ã‚Â°ÃƒÂ¸Ã‚Â°ÃƒÂÃƒâ‚¬ÃƒÅ½Ã‚ÂµÃ‚Â¥ Ãƒâ‚¬ÃƒÂ¥Ãƒâ€šÃƒÂ¸Ãƒâ€¡Ãƒâ€˜ Ã‚Â¹Ã‚Â«Ã‚Â±ÃƒÂ¢Ã‚Â°Ã‚Â¡ ÃƒË†Ã‚Â°Ãƒâ‚¬ÃƒÅ’ Ã‚Â¾Ãƒâ€ Ã‚Â´ÃƒÂÃ‚Â´Ãƒâ„¢. Ãƒâ‚¬ÃƒÂÃ‚Â¹ÃƒÂ Ã‚Â°ÃƒÂ¸Ã‚Â°ÃƒÂÃƒâ‚¬Ã‚Â¸Ã‚Â·ÃƒÅ½ Ãƒâ‚¬ÃƒÂ¼ÃƒË†Ã‚Â¯.
            if (wType < 40) *spType = 1;
            break;

        case 26:
            // v2.16 2002-5-27 Ã‡Ã˜Â¸Ã“ Ã‡ÃŠÂ»Ã¬Â±Ã¢ 
            if (m_pClientList[iClientH]->m_iSuperAttackLeft <= 0)  *spType = 1;
            if (m_pClientList[iClientH]->m_cSkillMastery[14] < 100) *spType = 1;
            break;

        case 27:
            // v2.16 2002-5-27 ÃÃ¶Ã†ÃŽÃ€ÃŒ Ã‡ÃŠÂ»Ã¬Â±Ã¢ 
            if (m_pClientList[iClientH]->m_iSuperAttackLeft <= 0)  *spType = 1;
            if (m_pClientList[iClientH]->m_cSkillMastery[21] < 100) *spType = 1;
            break;
    }
}

void CGame::LocalSavePlayerData(int iClientH)
{
    char * pData, * cp, cFn[256], cDir[256], cTxt[256], cCharDir[256];
    int    iSize;
    FILE * pFile;
    SYSTEMTIME SysTime;

    if (m_pClientList[iClientH] == nullptr) return;

    pData = new char[30000];
    if (pData == 0) return;
    memset(pData, 0, 30000);

    cp = (char *)(pData);
    iSize = _iComposePlayerDataFileContents(iClientH, cp);

    GetLocalTime(&SysTime);
    memset(cCharDir, 0, sizeof(cDir));
    wsprintf(cCharDir, "Character_%d_%d_%d_%d", SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);

    memset(cDir, 0, sizeof(cDir));
    memset(cFn, 0, sizeof(cFn));
    strcat(cFn, cCharDir);
    strcat(cFn, "\\");
    strcat(cFn, "\\");
    wsprintf(cTxt, "MeC77%d", (unsigned char)m_pClientList[iClientH]->m_cCharName[0]);
    strcat(cFn, cTxt);
    strcpy(cDir, cFn);
    strcat(cFn, "\\");
    strcat(cFn, "\\");
    strcat(cFn, m_pClientList[iClientH]->m_cCharName);
    strcat(cFn, ".txt");

    _mkdir(cCharDir);
    _mkdir(cDir);

    if (iSize == 0)
    {
        log->critical("(!) Character data body empty: Cannot create & save temporary player data file.");
        delete pData;
        return;
    }

    pFile = fopen(cFn, "wt");
    if (pFile == 0)
    {
        wsprintf(cTxt, "(!) Cannot create temporary player data file : Name(%s)", cFn);
        log->critical(cTxt);
    }
    else
    {
        wsprintf(cTxt, "(!) temporary player data file saved : Name(%s)", cFn);
        log->info(cTxt);
        fwrite(cp, iSize, 1, pFile);
    }

    if (pFile != 0) fclose(pFile);
    delete pData;
}

void CGame::MineralGenerator()
{
    int i, iP, tX, tY, iRet;

    for (i = 0; i < DEF_MAXMAPS; i++)
    {
        if ((iDice(1, 4) == 1) && (m_pMapList[i] != 0) &&
            (m_pMapList[i]->m_bMineralGenerator == true) &&
            (m_pMapList[i]->m_iCurMineral < m_pMapList[i]->m_iMaxMineral))
        {

            iP = iDice(1, m_pMapList[i]->m_iTotalMineralPoint) - 1;
            if ((m_pMapList[i]->m_MineralPointList[iP].x == -1) || (m_pMapList[i]->m_MineralPointList[iP].y == -1)) break;

            tX = m_pMapList[i]->m_MineralPointList[iP].x;
            tY = m_pMapList[i]->m_MineralPointList[iP].y;

            iRet = iCreateMineral(i, tX, tY, m_pMapList[i]->m_cMineralGeneratorLevel);
        }
    }
}

int CGame::iCreateMineral(char cMapIndex, int tX, int tY, char cLevel)
{
    int i, iDynamicHandle, iMineralType;

    if ((cMapIndex < 0) || (cMapIndex >= DEF_MAXMAPS)) return 0;
    if (m_pMapList[cMapIndex] == 0) return 0;

    for (i = 1; i < DEF_MAXMINERALS; i++)
        if (m_pMineral[i] == 0)
        {
            // ÂºÃ³ Â°Ã¸Â°Â£Â¿Â¡ Â±Â¤Â¹Â°ÂµÂ¢Ã€ÃŒÂ¸Â¦ Â¸Â¸ÂµÃ§Â´Ã™.
            iMineralType = iDice(1, cLevel);
            m_pMineral[i] = new class CMineral(iMineralType, cMapIndex, tX, tY, 1);
            if (m_pMineral[i] == 0) return 0;

            iDynamicHandle = 0;
            switch (iMineralType)
            {
                case 1: // Â±Â¤Â¹Â°Â·Ã¹
                case 2:
                case 3:
                case 4:
                    iDynamicHandle = iAddDynamicObjectList(0, 0, DEF_DYNAMICOBJECT_MINERAL1, cMapIndex, tX, tY, 0, i);
                    break;

                case 5: // ÂºÂ¸Â¼Â®Â·Ã¹ 
                case 6:
                    iDynamicHandle = iAddDynamicObjectList(0, 0, DEF_DYNAMICOBJECT_MINERAL2, cMapIndex, tX, tY, 0, i);
                    break;

                default:
                    // Â¿Â¡Â·Â¯ Â¹Ã¦ÃÃ¶Â¿Ã« Ã„ÃšÂµÃ¥ 
                    iDynamicHandle = iAddDynamicObjectList(0, 0, DEF_DYNAMICOBJECT_MINERAL1, cMapIndex, tX, tY, 0, i);
                    break;
            }

            if (iDynamicHandle == 0)
            {
                delete m_pMineral[i];
                m_pMineral[i] = 0;
                return 0;
            }
            m_pMineral[i]->m_sDynamicObjectHandle = iDynamicHandle;
            m_pMineral[i]->m_cMapIndex = cMapIndex;

            // Â³Â­Ã€ÃŒÂµÂµÂ¿Ã Â¼Ã¶Â·Â®Ã€Â» Ã€Ã”Â·Ã‚Ã‡Ã‘Â´Ã™.
            switch (iMineralType)
            {
                case 1: m_pMineral[i]->m_iDifficulty = 10; m_pMineral[i]->m_iRemain = 20; break;
                case 2: m_pMineral[i]->m_iDifficulty = 15; m_pMineral[i]->m_iRemain = 15; break;
                case 3: m_pMineral[i]->m_iDifficulty = 20; m_pMineral[i]->m_iRemain = 10; break;
                case 4: m_pMineral[i]->m_iDifficulty = 50; m_pMineral[i]->m_iRemain = 8; break;
                case 5: m_pMineral[i]->m_iDifficulty = 70; m_pMineral[i]->m_iRemain = 6; break;
                case 6: m_pMineral[i]->m_iDifficulty = 90; m_pMineral[i]->m_iRemain = 4; break;
                default: m_pMineral[i]->m_iDifficulty = 10; m_pMineral[i]->m_iRemain = 20; break;
            }

            // Â¸ÃŠÃ€Ã‡ Â¹ÃŒÂ³Ã—Â¶Ã¶ Â¼Ã¶ ÃÃµÂ°Â¡
            m_pMapList[cMapIndex]->m_iCurMineral++;

            return i;
        }

    return 0;
}

void CGame::_CheckMiningAction(int iClientH, int dX, int dY)
{
    short sType;
    uint32_t dwRegisterTime;
    int   iDynamicIndex, iSkillLevel, iResult, iItemID;
    class CItem * pItem;
    uint16_t  wWeaponType;

    iItemID = 0;

    if (m_pClientList[iClientH] == nullptr)  return;

    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bGetDynamicObject(dX, dY, &sType, &dwRegisterTime, &iDynamicIndex);

    if ((m_pClientList[iClientH]->m_iStatus & 0x10) != 0)
    {
        SetInvisibilityFlag(iClientH, DEF_OWNERTYPE_PLAYER, false);
        bRemoveFromDelayEventList(iClientH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
        m_pClientList[iClientH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = 0;
    }

    switch (sType)
    {
        case DEF_DYNAMICOBJECT_MINERAL1:
        case DEF_DYNAMICOBJECT_MINERAL2:
            // Â±Â¤Â¹Â°ÃƒÂ¤ÃƒÃ«Â¸Â¦ Â½ÃƒÂµÂµÃ‡Ã‘ Ã„Â³Â¸Â¯Ã…ÃÃ€Ã‡ ÃÂ¶Â°Ã‡Ã€Â» Â»Ã¬Ã‡Ã‰Â´Ã™. Â°Ã®Â±ÂªÃ€ÃŒÂ¸Â¦ ÂµÃ©Â°Ã­ Ã€Ã–Â´Ã‚Â°Â¡? Â±Â¤Â¹Â° ÃƒÂ¤ÃƒÃ« Â½ÂºÃ…Â³Ã€Âº? 
            // Â°Ã®Â±ÂªÃ€ÃŒÂ¿Â¡ Ã‡Ã˜Â´Ã§Ã‡ÃÂ´Ã‚ Â¿ÃœÃ‡Ã¼Ã€ÃŒÂ¸Ã© Ã…Ã«Â°Ãº 
            wWeaponType = ((m_pClientList[iClientH]->m_sAppr2 & 0x0FF0) >> 4);
            if (wWeaponType == 25)
            {
                // Ã‡ÃˆÂ¾Ã—Â½ÂºÂ¸Â¦ ÂµÃ©Â°Ã­ Ã€Ã–Â´Ã™. Â±Â¤Â¹Â° ÃƒÂ¤ÃƒÃ« Â°Â¡Â´Ã‰ 
            }
            else return;

            // Ã€Ã¼Ã…Ãµ Â¸Ã°Â¼Ã‡Ã€ÃŒ Â¾Ã†Â´ÃÂ´ÃµÂ¶Ã³ÂµÂµ Â¸Â®Ã…Ã 
            if ((m_pClientList[iClientH]->m_sAppr2 & 0xF000) == 0) return;

            iSkillLevel = m_pClientList[iClientH]->m_cSkillMastery[0];
            if (iSkillLevel == 0) break;

            if (m_pDynamicObjectList[iDynamicIndex] == 0) break;
            // Ã„Â³Â´Ã‚ Â±Â¤Â¹Â°Ã€Ã‡ Â³Â­Ã€ÃŒÂµÂµÂ¸Â¸Ã…Â­ Â½ÂºÃ…Â³Ã€Â» Â³Â·ÃƒÃ¡Â´Ã™.
            iSkillLevel -= m_pMineral[m_pDynamicObjectList[iDynamicIndex]->m_iV1]->m_iDifficulty;
            if (iSkillLevel <= 0) iSkillLevel = 1;

            iResult = iDice(1, 100);
            if (iResult <= iSkillLevel)
            {
                // ÃˆÂ®Â·Ã¼Ã€Âº Â¼ÂºÂ°Ã¸Ã‡ÃŸÂ´Ã™. Â±Â¤Â¹Â°ÃƒÂ¤ÃƒÃ« Â±Ã¢Â¼Ãº ÃÃµÂ°Â¡ 
                CalculateSSN_SkillIndex(iClientH, 0, 1);

                // Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®Ã€Ã‡ Â¹ÃŸ Â¹Ã˜Â¿Â¡ Â±Â¤Â¹Â°Ã€Â» Â¶Â³Â¾Ã®Â¶ÃŸÂ¸Â°Â´Ã™. 
                switch (m_pMineral[m_pDynamicObjectList[iDynamicIndex]->m_iV1]->m_cType)
                {
                    case 1:
                        switch (iDice(1, 5))
                        {
                            case 1:
                            case 2:
                            case 3:
                                iItemID = 355; // Coal
                                m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
                                break;
                            case 4:
                                iItemID = 357; // IronOre
                                m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
                                break;
                            case 5:
                                iItemID = 507; // BlondeStone
                                m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
                                break;
                        }
                        break;

                    case 2:
                        switch (iDice(1, 5))
                        {
                            case 1:
                            case 2:
                                iItemID = 355; // Coal
                                m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
                                break;
                            case 3:
                            case 4:
                                iItemID = 357; // IronOre
                                m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
                                break;
                            case 5:
                                if (iDice(1, 3) == 2)
                                {
                                    iItemID = 356; // SilverNugget
                                    m_pClientList[iClientH]->m_iExpStock += iDice(1, 4);
                                }
                                else
                                {
                                    iItemID = 507; // BlondeStone
                                    m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
                                }
                                break;
                        }
                        break;

                    case 3:
                        switch (iDice(1, 6))
                        {
                            case 1:
                                iItemID = 355; // Coal
                                m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
                                break;
                            case 2:
                            case 3:
                            case 4:
                            case 5:
                                iItemID = 357; // IronOre
                                m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
                                break;
                            case 6:
                                if (iDice(1, 8) == 3)
                                {
                                    if (iDice(1, 2) == 1)
                                    {
                                        iItemID = 356; // SilverNugget
                                        m_pClientList[iClientH]->m_iExpStock += iDice(1, 4);
                                    }
                                    else
                                    {
                                        iItemID = 357; // IronOre
                                        m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
                                    }
                                    break;
                                }
                                else
                                {
                                    iItemID = 357; // IronOre
                                    m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
                                }
                                break;
                        }
                        break;

                    case 4:
                        switch (iDice(1, 6))
                        {
                            case 1:
                                iItemID = 355; // Coal
                                m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
                                break;
                            case 2:
                                if (iDice(1, 3) == 2)
                                {
                                    iItemID = 356; // SilverNugget
                                    m_pClientList[iClientH]->m_iExpStock += iDice(1, 4);
                                }
                                break;
                            case 3:
                            case 4:
                            case 5:
                                iItemID = 357; // IronOre
                                m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
                                break;
                            case 6:
                                if (iDice(1, 8) == 3)
                                {
                                    if (iDice(1, 4) == 3)
                                    {
                                        if (iDice(1, 4) < 3)
                                        {
                                            iItemID = 508; // Mithral
                                            m_pClientList[iClientH]->m_iExpStock += iDice(1, 15);
                                        }
                                        else
                                        {
                                            iItemID = 354; // GoldNugget
                                            m_pClientList[iClientH]->m_iExpStock += iDice(1, 5);
                                        }
                                        break;
                                    }
                                    else
                                    {
                                        iItemID = 356; // SilverNugget
                                        m_pClientList[iClientH]->m_iExpStock += iDice(1, 4);
                                    }
                                    break;
                                }
                                else
                                {
                                    if (iDice(1, 2) == 1)
                                    {
                                        iItemID = 354; // GoldNugget
                                        m_pClientList[iClientH]->m_iExpStock += iDice(1, 5);
                                    }
                                    else
                                    {
                                        iItemID = 357;  // IronOre
                                        m_pClientList[iClientH]->m_iExpStock += iDice(1, 3);
                                    }
                                    break;
                                }
                                break;
                        }
                        break;

                    case 5:
                        switch (iDice(1, 19))
                        {
                            case 3:
                                iItemID = 352; // Sapphire
                                m_pClientList[iClientH]->m_iExpStock += iDice(2, 3);
                                break;
                            default:
                                iItemID = 358; // Crystal
                                m_pClientList[iClientH]->m_iExpStock += iDice(2, 3);
                                break;
                        }
                        break;

                    case 6:
                        switch (iDice(1, 5))
                        {
                            case 1:
                                if (iDice(1, 6) == 3)
                                {
                                    iItemID = 353; // Emerald 
                                    m_pClientList[iClientH]->m_iExpStock += iDice(2, 4);
                                }
                                else
                                {
                                    iItemID = 358; // Crystal
                                    m_pClientList[iClientH]->m_iExpStock += iDice(2, 3);
                                }
                                break;
                            case 2:
                                if (iDice(1, 6) == 3)
                                {
                                    iItemID = 352; // Saphire
                                    m_pClientList[iClientH]->m_iExpStock += iDice(2, 4);
                                }
                                else
                                {
                                    iItemID = 358; // Crystal
                                    m_pClientList[iClientH]->m_iExpStock += iDice(2, 3);
                                }
                                break;
                            case 3:
                                if (iDice(1, 6) == 3)
                                {
                                    iItemID = 351; // Ruby
                                    m_pClientList[iClientH]->m_iExpStock += iDice(2, 4);
                                }
                                else
                                {
                                    iItemID = 358; // Crystal 
                                    m_pClientList[iClientH]->m_iExpStock += iDice(2, 3);
                                }
                                break;
                            case 4:
                                iItemID = 358; // Crystal 
                                m_pClientList[iClientH]->m_iExpStock += iDice(2, 3);
                                break;
                            case 5:
                                if (iDice(1, 12) == 3)
                                {
                                    iItemID = 350; // Diamond
                                    m_pClientList[iClientH]->m_iExpStock += iDice(2, 5);
                                }
                                else
                                {
                                    iItemID = 358; // Crystal
                                    m_pClientList[iClientH]->m_iExpStock += iDice(2, 3);
                                }
                                break;
                        }
                        break;

                }

                pItem = new class CItem;
                if (_bInitItemAttr(pItem, iItemID) == false)
                {
                    delete pItem;
                }
                else
                {
                    // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Â¼Â­Ã€Ã–Â´Ã‚ Ã€Â§Ã„Â¡Â¿Â¡ Â¹Ã¶Â¸Â°Â´Ã™. 
                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
                        m_pClientList[iClientH]->m_sY, pItem);
                    // Â´Ã™Â¸Â¥ Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¿Â¡Â°Ã” Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¶Â³Â¾Ã®ÃÃ¸ Â°ÃÃ€Â» Â¾Ã‹Â¸Â°Â´Ã™. 
                    SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
                        m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
                        pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4
                    // testcode
                    // log->info(G_cTxt, "Mining Success: {}", iItemID);
                }

                // Â±Â¤Â¹Â°Ã€Ã‡ Â³Â²Ã€Âº Â·Â®Ã€Â» ÃÃ™Ã€ÃŒÂ°Ã­ 0Ã€ÃŽÂ°Ã¦Â¿Ã¬ Â»Ã¨ÃÂ¦Ã‡Ã‘Â´Ã™.
                m_pMineral[m_pDynamicObjectList[iDynamicIndex]->m_iV1]->m_iRemain--;
                if (m_pMineral[m_pDynamicObjectList[iDynamicIndex]->m_iV1]->m_iRemain <= 0)
                {
                    // Â±Â¤Â¹Â°Ã€ÃŒ Â¸Ã°ÂµÃŽ Â¼Ã’Â¸Ã°ÂµÃ‡Â¾ÃºÂ´Ã™. Delete Mineral 
                    bDeleteMineral(m_pDynamicObjectList[iDynamicIndex]->m_iV1);

                    // ÂµÂ¿Ã€Ã» Â°Â´ÃƒÂ¼ Â»Ã¨ÃÂ¦
                    delete m_pDynamicObjectList[iDynamicIndex];
                    m_pDynamicObjectList[iDynamicIndex] = 0;
                }
            }
            break;

        default:
            break;
    }
}

bool CGame::bDeleteMineral(int iIndex)
{
    int iDynamicIndex;
    uint32_t dwTime;

    dwTime = timeGetTime();

    if (m_pMineral[iIndex] == 0) return false;
    iDynamicIndex = m_pMineral[iIndex]->m_sDynamicObjectHandle;
    if (m_pDynamicObjectList[iDynamicIndex] == 0) return false;

    SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, DEF_MSGTYPE_REJECT, m_pDynamicObjectList[iDynamicIndex]->m_cMapIndex,
        m_pDynamicObjectList[iDynamicIndex]->m_sX, m_pDynamicObjectList[iDynamicIndex]->m_sY,
        m_pDynamicObjectList[iDynamicIndex]->m_sType, iDynamicIndex, 0);
    // Â¸ÃŠÂ¿Â¡Â¼Â­ Â±Â¤Â¹Â° ÂµÂ¿Ã€Ã» Â°Â´ÃƒÂ¼Â¸Â¦ Â»Ã¨ÃÂ¦Ã‡Ã‘Â´Ã™.
    m_pMapList[m_pDynamicObjectList[iDynamicIndex]->m_cMapIndex]->SetDynamicObject(0, 0, m_pDynamicObjectList[iDynamicIndex]->m_sX, m_pDynamicObjectList[iDynamicIndex]->m_sY, dwTime);
    // Â±Â¤Â¹Â°Ã€ÃŒ Â»Ã§Â¶Ã³ÃÂ³Ã€Â¸Â¹Ã‡Â·ÃŽ Ã€ÃŒÂµÂ¿Ã€ÃŒ Â°Â¡Â´Ã‰Ã‡ÃÂ°Ã” Ã‡Ã‘Â´Ã™. 
    m_pMapList[m_pMineral[iIndex]->m_cMapIndex]->SetTempMoveAllowedFlag(m_pDynamicObjectList[iDynamicIndex]->m_sX, m_pDynamicObjectList[iDynamicIndex]->m_sY, true);

    // Â±Â¤Â¹Â° Â°Â³ÃƒÂ¼ Â¼Ã¶ Â°Â¨Â¼Ã’ 
    m_pMapList[m_pMineral[iIndex]->m_cMapIndex]->m_iCurMineral--;

    // Â±Â¤Â¹Â° Â°Â´ÃƒÂ¼ Â»Ã¨ÃÂ¦ 
    delete m_pMineral[iIndex];
    m_pMineral[iIndex] = 0;

    return true;
}

void CGame::CheckFireBluring(char cMapIndex, int sX, int sY)
{
    int ix, iy, iItemNum;
    short sSpr, sSprFrame;
    char  cItemColor;
    class CItem * pItem;

    for (ix = sX - 1; ix <= sX + 1; ix++)
        for (iy = sY - 1; iy <= sY + 1; iy++)
        {
            // Â¸Â¸Â¾Ã  Ã€ÃŒ Ã€Â§Ã„Â¡Â¿Â¡ Â¹ÃŸÃˆÂ­Â¼Âº Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Ã€Ã–Â´Ã™Â¸Ã© ÃÃ¶Â¿Ã¬Â°Ã­ ÂºÃ’ Â¿Ã€ÂºÃªÃÂ§Ã†Â®Â¸Â¦ Â³ÃµÂ´Ã‚Â´Ã™.	
            iItemNum = m_pMapList[cMapIndex]->iCheckItem(ix, iy);

            switch (iItemNum)
            {
                case 355:
                    // Â¼Â®Ã…ÂºÃ€ÃŒÂ´Ã™. Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» ÃÃ¶Â¿Ã¬Â°Ã­ ÂºÃ’Ã€Â» Â¸Â¸ÂµÃ§Â´Ã™.
                    pItem = m_pMapList[cMapIndex]->pGetItem(ix, iy, &sSpr, &sSprFrame, &cItemColor);
                    if (pItem != 0) delete pItem;
                    iAddDynamicObjectList(0, 0, DEF_DYNAMICOBJECT_FIRE, cMapIndex, ix, iy, 6000);

                    SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_SETITEM, cMapIndex,
                        ix, iy, sSpr, sSprFrame, cItemColor);
                    break;
            }
        }
}

int CGame::_iGetWeaponSkillType(int iClientH)
{
    uint16_t wWeaponType;

    if (m_pClientList[iClientH] == nullptr) return 0;

    wWeaponType = ((m_pClientList[iClientH]->m_sAppr2 & 0x0FF0) >> 4);

    if (wWeaponType == 0)
    {
        return 5;
    }
    else if ((wWeaponType >= 1) && (wWeaponType <= 2))
    {
        return 7;
    }
    else if ((wWeaponType > 2) && (wWeaponType < 20))
    {
        if (wWeaponType == 7)
            return 9;
        else return 8;
    }
    else if ((wWeaponType >= 20) && (wWeaponType < 30))
    {
        return 10;
    }
    else if ((wWeaponType >= 30) && (wWeaponType < 35))
    {
        return 14;
    }
    else if ((wWeaponType >= 35) && (wWeaponType < 40))
    {
        return 21;
    }
    else if (wWeaponType >= 40)
    {
        return 6;
    }

    return 1;
}

static int ___iCAB5[] = { 0,0, 0,1,2 };
static int ___iCAB6[] = { 0,0, 0,0,0 };
static int ___iCAB7[] = { 0,0, 1,2,3 };
static int ___iCAB8[] = { 0,0, 1,3,5 };
static int ___iCAB9[] = { 0,0, 2,4,8 };
static int ___iCAB10[] = { 0,0, 1,2,3 };

int CGame::iGetComboAttackBonus(int iSkill, int iComboCount)
{
    if (iComboCount <= 1) return 0;
    if (iComboCount > 6) return 0;
    switch (iSkill)
    {
        case 5:
            return ___iCAB5[iComboCount];
            break;
        case 6:
            return ___iCAB6[iComboCount];
            break;
        case 7:
            return ___iCAB7[iComboCount];
            break;
        case 8:
            return ___iCAB8[iComboCount];
            break;
        case 9:
            return ___iCAB9[iComboCount];
            break;
        case 10:
            return ___iCAB10[iComboCount];
            break;
        case 14:
            return ___iCAB6[iComboCount];
            break;
        case 21:
            return ___iCAB10[iComboCount];
            break;
    }

    return 0;
}

bool CGame::_bDepleteDestTypeItemUseEffect(int iClientH, int dX, int dY, short sItemIndex, short sDestItemID)
{
    bool bRet;

    // Ã€Â§Ã„Â¡Â¸Â¦ ÃÃ¶ÃÂ¤Ã‡ÃÂ°Ã­ Â»Ã§Â¿Ã« ÃˆÃ„ Â»Ã§Â¶Ã³ÃÃ¶Â´Ã‚ Â¾Ã†Ã€ÃŒÃ…Ã› ÃˆÂ¿Â°Ãº ÃƒÂ³Â¸Â® ÂºÃŽÂºÃ 
    if (m_pClientList[iClientH] == nullptr) return false;
    if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return false;
    if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == 0) return false;

    switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType)
    {
        case DEF_ITEMEFFECTTYPE_OCCUPYFLAG:
            bRet = __bSetOccupyFlag(m_pClientList[iClientH]->m_cMapIndex, dX, dY,
                m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1,
                0, iClientH, 0);
            if (bRet == true)
            {
                GetExp(iClientH, (iDice(m_pClientList[iClientH]->m_iLevel, 10)));
            }
            else
            {
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_NOTFLAGSPOT, 0, 0, 0, 0);
            }
            return bRet;

            // crusade
        case DEF_ITEMEFFECTTYPE_CONSTRUCTIONKIT:
            // Â°Ã‡Â¼Â³ Ã…Â°Ã†Â®Ã€ÃŒÂ´Ã™. Â¹ÃŒÂµÃ©Â·Â£ÂµÃ¥Â¿Â¡Â¼Â­Â¸Â¸ Â»Ã§Â¿Ã« Â°Â¡Â´Ã‰Ã‡Ã”. m_sItemEffectValue1: Â°Ã‡ÃƒÃ Â¹Â° ÃÂ¾Â·Ã¹, m_sItemEffectValue2: Â°Ã‡ÃƒÃ  Â½ÃƒÂ°Â£ 
            bRet = __bSetConstructionKit(m_pClientList[iClientH]->m_cMapIndex, dX, dY,                                 // 
                m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1,        //
                m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2,        //
                iClientH); // Â¼Â³Ã„Â¡Ã€Ãš 
            if (bRet == true)
            {
                // Â°Ã‡Â¼Â³ Â½ÃƒÃ€Ã› 
            }
            else
            {
                // Â°Ã‡Â¼Â³ Â½Ã‡Ã†Ã			
            }
            return bRet;

        case DEF_ITEMEFFECTTYPE_DYE:
            // Â¾Ã†Ã€ÃŒÃ…Ã› Â¿Â°Â»Ã¶: Â¾Ã†Ã€ÃŒÃ…Ã› Â°ÂªÃ€ÃŒ Ã€Â¯ÃˆÂ¿Ã‡Ã‘Â°Â¡ ÃƒÂ¼Ã…Â©.
            if ((sDestItemID >= 0) && (sDestItemID < DEF_MAXITEMS))
            {
                if (m_pClientList[iClientH]->m_pItemList[sDestItemID] != 0)
                {
                    if ((m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cCategory == 11) ||
                        (m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cCategory == 12))
                    {
                        // Â¿Â°Â»Ã¶Ã€ÃŒ Â°Â¡Â´Ã‰Ã‡Ã‘ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒÂ´Ã™.
                        m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cItemColor = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
                        // Â»Ã¶ Â¼Ã“Â¼ÂºÃ€ÃŒ Â¹Ã™Â²Ã®Â¾ÃºÃ€Â½Ã€Â» Â¾Ã‹Â·ÃÃÃ˜Â´Ã™. 
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMCOLORCHANGE, sDestItemID, m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cItemColor, 0, 0);
                        return true;
                    }
                    else
                    {
                        // Â¿Â°Â»Ã¶Ã€ÃŒ ÂºÃ’Â°Â¡Â´Ã‰Ã‡Ã‘ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒÂ´Ã™. 
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMCOLORCHANGE, sDestItemID, -1, 0, 0);
                        return false;
                    }
                }
            }
            break;

        case DEF_ITEMEFFECTTYPE_ARMORDYE:
            if ((sDestItemID >= 0) && (sDestItemID < DEF_MAXITEMS))
            {
                if (m_pClientList[iClientH]->m_pItemList[sDestItemID] != 0)
                {
                    if ((m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cCategory == 6) ||
                        (m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cCategory == 15) ||
                        (m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cCategory == 13))
                    {
                        m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cItemColor = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMCOLORCHANGE, sDestItemID, m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cItemColor, 0, 0);
                        return true;
                    }
                    else
                    {
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMCOLORCHANGE, sDestItemID, -1, 0, 0);
                        return false;
                    }
                }
            }
            break;

        case DEF_ITEMEFFECTTYPE_WEAPONDYE:
            if ((sDestItemID >= 0) && (sDestItemID < DEF_MAXITEMS))
            {
                if (m_pClientList[iClientH]->m_pItemList[sDestItemID] != 0)
                {
                    if ((m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cCategory == 1) ||
                        (m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cCategory == 3) ||
                        (m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cCategory == 8))
                    {
                        m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cItemColor = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMCOLORCHANGE, sDestItemID, m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cItemColor, 0, 0);
                        return true;
                    }
                    else
                    {
                        SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMCOLORCHANGE, sDestItemID, -1, 0, 0);
                        return false;
                    }
                }
            }
            break;

        case DEF_ITEMEFFECTTYPE_FARMING:
            bRet = bPlantSeedBag(m_pClientList[iClientH]->m_cMapIndex, dX, dY,
                m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1,
                m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2,
                iClientH);
            return bRet;

        default:
            break;
    }

    return true;
}

bool CGame::__bSetOccupyFlag(char cMapIndex, int dX, int dY, int iSide, int iEKNum, int iClientH, bool bAdminFlag)
{
    int ix, iy;
    int   iDynamicObjectIndex, iIndex;
    class CTile * pTile;
    uint32_t dwTime;

    dwTime = timeGetTime();

    if (m_pMapList[cMapIndex] == 0) return false;
    if (((m_bIsHeldenianMode == false) || (m_bIsHeldenianMode != m_cHeldenianType)) && (m_bHeldenianInitiated == 1)) return false;
    if ((m_cHeldenianType == 1) && (m_iBTFieldMapIndex == -1)) return false;
    if ((m_cHeldenianType == 2) && (m_iGodHMapIndex == -1)) return false;
    if ((m_pClientList[iClientH]->m_iGuildRank == 0)) return false;

    pTile = (class CTile *)(m_pMapList[cMapIndex]->m_pTile + dX + dY * m_pMapList[cMapIndex]->m_sSizeY);
    if (pTile->m_iAttribute != 0) return false;
    iSide = m_sLastHeldenianWinner;
    if ((dX < 25) || (dX >= m_pMapList[cMapIndex]->m_sSizeX - 25) ||
        (dY < 25) || (dY >= m_pMapList[cMapIndex]->m_sSizeY - 25)) return false;

    if ((iClientH > 0) && (m_pClientList[iClientH] != 0))
    {
        if ((bAdminFlag == false) && (m_pClientList[iClientH]->m_cSide != iSide)) return false;
    }

    pTile = (class CTile *)(m_pMapList[cMapIndex]->m_pTile + dX + dY * m_pMapList[cMapIndex]->m_sSizeY);
    if (pTile->m_iOccupyFlagIndex != 0) return false;
    if (pTile->m_bIsMoveAllowed == false)  return false;

    for (ix = dX - 3; ix <= dX + 3; ix++)
        for (iy = dY - 3; iy <= dY + 3; iy++)
        {
            if ((ix == dX) && (iy == dY))
            {

            }
            else
            {
                pTile = (class CTile *)(m_pMapList[cMapIndex]->m_pTile + ix + iy * m_pMapList[cMapIndex]->m_sSizeY);
                if ((pTile->m_iOccupyFlagIndex != 0) && (pTile->m_iOccupyFlagIndex > 0) &&
                    (pTile->m_iOccupyFlagIndex < DEF_MAXOCCUPYFLAG) && (m_pMapList[cMapIndex]->m_pOccupyFlag[pTile->m_iOccupyFlagIndex] != 0))
                {
                    if (m_pMapList[cMapIndex]->m_pOccupyFlag[pTile->m_iOccupyFlagIndex]->m_cSide == iSide) return false;
                }
            }
        }

    if (m_pMapList[cMapIndex]->m_iTotalOccupyFlags >= DEF_MAXOCCUPYFLAG)
    {
        return false;
    }

    switch (iSide)
    {
        case 1:	iDynamicObjectIndex = iAddDynamicObjectList(0, 0, DEF_DYNAMICOBJECT_ARESDENFLAG1, cMapIndex, dX, dY, 0, 0);	break;
        case 2:	iDynamicObjectIndex = iAddDynamicObjectList(0, 0, DEF_DYNAMICOBJECT_ELVINEFLAG1, cMapIndex, dX, dY, 0, 0);	break;
        default: iDynamicObjectIndex = 0;
    }

    iEKNum = 1;
    iIndex = m_pMapList[cMapIndex]->iRegisterOccupyFlag(dX, dY, iSide, iEKNum, iDynamicObjectIndex);
    if (iIndex < 0)
    {
        if (iDynamicObjectIndex > DEF_MAXGUILDS)
            return true;
    }

    pTile = (class CTile *)(m_pMapList[cMapIndex]->m_pTile + dX + dY * m_pMapList[cMapIndex]->m_sSizeY);
    pTile->m_iOccupyFlagIndex = iIndex;

    m_pMapList[cMapIndex]->m_iTotalOccupyFlags++;

    if (m_cHeldenianType == 1)
    {
        for (ix = dX - 3; ix <= dX + 3; ix++)
            for (iy = dY - 3; iy <= dY + 3; iy++)
            {
                if ((ix < 0) || (ix >= m_pMapList[cMapIndex]->m_sSizeX) ||
                    (iy < 0) || (iy >= m_pMapList[cMapIndex]->m_sSizeY))
                {
                }
                else
                {
                    pTile = (class CTile *)(m_pMapList[cMapIndex]->m_pTile + ix + iy * m_pMapList[cMapIndex]->m_sSizeY);
                    switch (iSide)
                    {
                        case 1:
                            pTile->m_iOccupyStatus -= iEKNum;
                            break;
                        case 2:
                            pTile->m_iOccupyStatus += iEKNum;
                            break;
                    }
                }
            }
    }

    if (m_cHeldenianType == 2)
    {
        if (iSide == m_sLastHeldenianWinner)
        {
            m_cHeldenianVictoryType = iSide;
            //sub_4AB9D0
        }
    }
    return true;
}

void CGame::SetSummonMobAction(int iClientH, int iMode, uint32_t dwMsgSize, char * pData)
{
    int i, iTargetIndex;
    char   seps[] = "= \t\n";
    char * token, cTargetName[11], cBuff[256];
    class  CStrTok * pStrTok;

    if (m_pClientList[iClientH] == nullptr) return;
    // v2.1 Áß¸³Àº ¸÷°ø°Ý ¸í·ÉÀ» ÇÒ ¼ö ¾ø´Ù.
    if (m_pClientList[iClientH]->m_cSide == 0) return;

    switch (iMode)
    {
        case 0: // Free ¸ðµå
        case 1: // Hold ¸ðµå
            //  ¸¶½ºÅÍ°¡ iClientHÀÎ ¸÷µéÀÇ Á¦¾î¸ðµå¸¦ º¯°æÇÑ´Ù.
            for (i = 0; i < DEF_MAXNPCS; i++)
                if (m_pNpcList[i] != 0)
                {
                    if ((m_pNpcList[i]->m_bIsSummoned == true) &&
                        (m_pNpcList[i]->m_iFollowOwnerIndex == iClientH) &&
                        (m_pNpcList[i]->m_cFollowOwnerType == DEF_OWNERTYPE_PLAYER))
                    {

                        m_pNpcList[i]->m_iSummonControlMode = iMode;
                        m_pNpcList[i]->m_bIsPermAttackMode = false;
                        m_pNpcList[i]->m_cBehavior = DEF_BEHAVIOR_MOVE;
                        m_pNpcList[i]->m_sBehaviorTurnCount = 0;
                        m_pNpcList[i]->m_iTargetIndex = 0;
                    }
                }
            break;

        case 2:
            // °ø°Ý ¸ðµå - ¸ñÇ¥¹°À» ¾ò¾î¿Â´Ù. 
            if ((dwMsgSize) <= 0) return;
            memcpy(cBuff, pData, dwMsgSize);

            pStrTok = new class CStrTok(cBuff, seps);
            token = pStrTok->pGet();
            token = pStrTok->pGet();

            iTargetIndex = 0;
            if (token != 0)
            {
                // tokenÀÌ °ð °ø°ÝÇÒ »ç¿ëÀÚ ÀÌ¸§ 
                if (strlen(token) > 10)
                    memcpy(cTargetName, token, 10);
                else memcpy(cTargetName, token, strlen(token));

                // 2002.8.17 ÀåÁøÈ£ ¼öÁ¤
                for (i = 1; i < DEF_MAXCLIENTS; i++)
                {
                    // if ((m_pClientList[i] != 0) && (memcmp(m_pClientList[i]->m_cCharName, cTargetName, 10) == 0)) { // original
                    if ((m_pClientList[i] != 0) &&
                        (memcmp(m_pClientList[i]->m_cCharName, cTargetName, 10) == 0) &&
                        (strcmp(m_pClientList[iClientH]->m_cMapName, m_pClientList[i]->m_cMapName) == 0)) // adamas(mapÀÌ °°¾Æ¾ß ÇÑ´Ù.)
                    {
                        // ¸ñÇ¥ Ä³¸¯ÅÍ¸¦ Ã£¾Ò´Ù. ÀÎµ¦½º ÀúÀå
                        iTargetIndex = i;
                        goto SSMA_SKIPSEARCH;
                    }
                }
            }

            SSMA_SKIPSEARCH:;

            if ((iTargetIndex != 0) && (m_pClientList[iTargetIndex]->m_cSide != 0) &&
                (m_pClientList[iTargetIndex]->m_cSide != m_pClientList[iClientH]->m_cSide))
            {
                // ¸ñÇ¥¹°ÀÌ Á¸ÀçÇÑ´Ù¸é ÇÒ´çÇÑ´Ù. 
                for (i = 0; i < DEF_MAXNPCS; i++)
                    if (m_pNpcList[i] != 0)
                    {
                        if ((m_pNpcList[i]->m_bIsSummoned == true) &&
                            (m_pNpcList[i]->m_iFollowOwnerIndex == iClientH) &&
                            (m_pNpcList[i]->m_cFollowOwnerType == DEF_OWNERTYPE_PLAYER))
                        {

                            m_pNpcList[i]->m_iSummonControlMode = iMode;
                            m_pNpcList[i]->m_cBehavior = DEF_BEHAVIOR_ATTACK;
                            m_pNpcList[i]->m_sBehaviorTurnCount = 0;
                            m_pNpcList[i]->m_iTargetIndex = iTargetIndex;
                            m_pNpcList[i]->m_cTargetType = DEF_OWNERTYPE_PLAYER;
                            m_pNpcList[i]->m_bIsPermAttackMode = true;
                        }
                    }
            }
            break;
    }
}

int CGame::_iComposeFlagStatusContents(char * pData)
{
    SYSTEMTIME SysTime;
    char cTxt[120];
    int i;

    if (m_iMiddlelandMapIndex < 0) return 0;

    GetLocalTime(&SysTime);
    strcat(pData, "[FILE-DATE]\n\n");

    wsprintf(cTxt, "file-saved-date: %d %d %d %d %d\n", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
    strcat(pData, cTxt);
    strcat(pData, "\n\n");

    // 
    for (i = 1; i < DEF_MAXOCCUPYFLAG; i++)
        if (m_pMapList[m_iMiddlelandMapIndex]->m_pOccupyFlag[i] != 0)
        {

            wsprintf(cTxt, "flag = %d %d %d %d", m_pMapList[m_iMiddlelandMapIndex]->m_pOccupyFlag[i]->m_cSide,
                m_pMapList[m_iMiddlelandMapIndex]->m_pOccupyFlag[i]->m_sX,
                m_pMapList[m_iMiddlelandMapIndex]->m_pOccupyFlag[i]->m_sY,
                m_pMapList[m_iMiddlelandMapIndex]->m_pOccupyFlag[i]->m_iEKCount);
            strcat(pData, cTxt);
            strcat(pData, "\n");
        }

    strcat(pData, "\n\n");

    return strlen(pData);
}

void CGame::_SetItemPos(int iClientH, char * pData)
{
    char * cp, cItemIndex;
    short * sp, sX, sY;

    if (m_pClientList[iClientH] == nullptr) return;

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);
    cItemIndex = *cp;
    cp++;

    sp = (short *)cp;
    sX = *sp;
    cp += 2;

    sp = (short *)cp;
    sY = *sp;
    cp += 2;

    // Ã€ÃŸÂ¸Ã¸ÂµÃˆ ÃÃ‚Ã‡Â¥Â°Âª ÂºÂ¸ÃÂ¤ 
    if (sY < -10) sY = -10;

    if ((cItemIndex < 0) || (cItemIndex >= DEF_MAXITEMS)) return;
    if (m_pClientList[iClientH]->m_pItemList[cItemIndex] != 0)
    {
        m_pClientList[iClientH]->m_ItemPosList[cItemIndex].x = sX;
        m_pClientList[iClientH]->m_ItemPosList[cItemIndex].y = sY;
    }
}

void CGame::CheckUniqueItemEquipment(int iClientH)
{
    int i, iDamage;

    if (m_pClientList[iClientH] == nullptr) return;
    // v1.3 Ã€Â¯Â´ÃÃ…Â© Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ Ã€Ã¥Ã‚Ã¸ Â»Ã³Ã…Ã‚Â¸Â¦ Ã†Ã‡Â´ÃœÃ‡Ã‘Â´Ã™. Ã†Â¯ÃÂ¤Ã€ÃŽÂ¸Â¸Ã€ÃŒ Ã‚Ã¸Â¿Ã« Â°Â¡Â´Ã‰Ã‡Ã‘ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Ã€Ã¥Ã‚Ã¸Ã‡Ã‘ Â°Ã¦Â¿Ã¬ Â¹Ã¾Â°ÃœÃÃ¸Â´Ã™.

    for (i = 0; i < DEF_MAXITEMS; i++)
        if (m_pClientList[iClientH]->m_pItemList[i] != 0)
        {
            if ((m_pClientList[iClientH]->m_pItemList[i]->m_sTouchEffectType == DEF_ITET_UNIQUE_OWNER) &&
                (m_pClientList[iClientH]->m_bIsItemEquipped[i] == true))
            {
                // Touch Effect TypeÃ€ÃŒ DEF_ITET_OWNERÃ€ÃŒÂ¸Ã© Touch Effect Value 1, 2, 3Ã€ÃŒ ÃÃ–Ã€ÃŽ Ã„Â³Â¸Â¯Ã…ÃÃ€Ã‡ Â°Ã­Ã€Â¯Â°ÂªÃ€Â» Â°Â®Â´Ã‚Â´Ã™. 

                if ((m_pClientList[iClientH]->m_pItemList[i]->m_sTouchEffectValue1 == m_pClientList[iClientH]->m_sCharIDnum1) &&
                    (m_pClientList[iClientH]->m_pItemList[i]->m_sTouchEffectValue2 == m_pClientList[iClientH]->m_sCharIDnum2) &&
                    (m_pClientList[iClientH]->m_pItemList[i]->m_sTouchEffectValue3 == m_pClientList[iClientH]->m_sCharIDnum3))
                {
                    // ID Â°ÂªÃ€ÃŒ Â¸Ã‚Â´Ã™.
                }
                else
                {
                    // Ã€ÃšÂ½Ã…Ã€Ã‡ Â°ÃÃ€ÃŒ Â¾Ã†Â´ÃÂ¹Ã‡Â·ÃŽ Ã‚Ã¸Â¿Ã«Ã€ÃŒ Ã‡Ã˜ÃÂ¦ÂµÃˆÂ´Ã™.
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_pItemList[i]->m_cEquipPos, i, 0, 0);
                    ReleaseItemHandler(iClientH, i, true);
                    // v1.4 Â´Ã«Â¹ÃŒÃÃ¶Â¸Â¦ Â¾Ã²Â´Ã‚Â´Ã™. 
                    iDamage = iDice(10, 10);
                    m_pClientList[iClientH]->m_iHP -= iDamage;
                    if (m_pClientList[iClientH]->m_iHP <= 0)
                    {
                        ClientKilledHandler(iClientH, 0, 0, iDamage);
                    }
                }
            }
        }
}

void CGame::SetExchangeItem(int iClientH, int iItemIndex, int iAmount)
{
    int iExH;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsOnServerChange == true) return;
    if (m_pClientList[iClientH]->iExchangeCount > 4) return;	//only 4 items trade

    //no admin trade
    if ((m_bAdminSecurity == true) && (m_pClientList[iClientH]->m_iAdminUserLevel > 0))
    {
        _ClearExchangeStatus(m_pClientList[iClientH]->m_iExchangeH);
        _ClearExchangeStatus(iClientH);
    }


    if ((m_pClientList[iClientH]->m_bIsExchangeMode == true) && (m_pClientList[iClientH]->m_iExchangeH != 0))
    {
        iExH = m_pClientList[iClientH]->m_iExchangeH;
        if ((m_pClientList[iExH] == 0) || (memcmp(m_pClientList[iClientH]->m_cExchangeName, m_pClientList[iExH]->m_cCharName, 10) != 0))
        {
            // ±³È¯ÇÒ »ó´ë¹æÀÌ ¾ø°Å³ª ±³È¯ÇÏ°íÀÚ Çß´ø ±× Ä³¸¯ÅÍ°¡ ¾Æ´Ï´Ù. 	

        }
        else
        {
            // ±³È¯ÇÏ°íÀÚ ÇÏ´Â »ó´ë¹æ¿¡°Ô ¾ÆÀÌÅÛÀ» ¾Ë·ÁÁØ´Ù. 
            // ÇØ´ç ¾ÆÀÌÅÛÀÌ Á¸ÀçÇÏ´ÂÁö, ¼ö·®ÀÌ ¸Â´ÂÁö Ã¼Å©ÇÑ´Ù. 
            if ((iItemIndex < 0) || (iItemIndex >= DEF_MAXITEMS)) return;
            if (m_pClientList[iClientH]->m_pItemList[iItemIndex] == 0) return;
            if (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwCount < iAmount) return;

            //No Duplicate items
            for (int i = 0; i < m_pClientList[iClientH]->iExchangeCount; i++)
            {
                if (m_pClientList[iClientH]->m_cExchangeItemIndex[i] == (char)iItemIndex)
                {
                    _ClearExchangeStatus(iExH);
                    _ClearExchangeStatus(iClientH);
                    return;
                }
            }

            // ±³È¯ÇÏ°íÀÚ ÇÏ´Â ¾ÆÀÌÅÛ ÀÎµ¦½º, ¼ö·® ÀúÀå 
            m_pClientList[iClientH]->m_cExchangeItemIndex[m_pClientList[iClientH]->iExchangeCount] = (char)iItemIndex;
            m_pClientList[iClientH]->m_iExchangeItemAmount[m_pClientList[iClientH]->iExchangeCount] = iAmount;

            //memset(m_pClientList[iClientH]->m_cExchangeItemName, 0, sizeof(m_pClientList[iClientH]->m_cExchangeItemName));
            memcpy(m_pClientList[iClientH]->m_cExchangeItemName[m_pClientList[iClientH]->iExchangeCount], m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName, 20);

            //m_pClientList[iClientH]->m_cExchangeItemIndex  = iItemIndex;
            //m_pClientList[iClientH]->m_iExchangeItemAmount = iAmount;
            //memset(m_pClientList[iClientH]->m_cExchangeItemName, 0, sizeof(m_pClientList[iClientH]->m_cExchangeItemName));
            //memcpy(m_pClientList[iClientH]->m_cExchangeItemName, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName, 20);

            m_pClientList[iClientH]->iExchangeCount++;
            SendNotifyMsg(iClientH, iClientH, DEF_NOTIFY_SETEXCHANGEITEM, iItemIndex + 1000, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSprite,
                m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSpriteFrame, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName, iAmount, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor,
                m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan,
                m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan,
                m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 + 100,
                m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute);

            SendNotifyMsg(iClientH, iExH, DEF_NOTIFY_SETEXCHANGEITEM, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSprite,
                m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSpriteFrame, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName, iAmount, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor,
                m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan,
                m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan,
                m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 + 100,
                m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute);
        }
    }
    else
    {
        // ±³È¯ ¸ðµå°¡ ¾Æ´Ï¹Ç·Î 
    }
}

void CGame::ConfirmExchangeItem(int iClientH)
{
    int iExH;
    int iItemWeightA, iItemWeightB, iWeightLeftA, iWeightLeftB, iAmountLeft;
    class CItem * pItemA[4], * pItemB[4], * pItemAcopy[4], * pItemBcopy[4];

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_bIsOnServerChange == true) return;
    if ((m_bAdminSecurity == true) && (m_pClientList[iClientH]->m_iAdminUserLevel > 0)) return;


    if ((m_pClientList[iClientH]->m_bIsExchangeMode == true) && (m_pClientList[iClientH]->m_iExchangeH != 0))
    {
        iExH = m_pClientList[iClientH]->m_iExchangeH;

        // v1.42 ¼³¸¶ ÀÌ·± °æ¿ì°¡?
        if (iClientH == iExH) return;

        if (m_pClientList[iExH] != 0)
        {
            if ((memcmp(m_pClientList[iClientH]->m_cExchangeName, m_pClientList[iExH]->m_cCharName, 10) != 0) ||
                (m_pClientList[iExH]->m_bIsExchangeMode != true) ||
                (memcmp(m_pClientList[iExH]->m_cExchangeName, m_pClientList[iClientH]->m_cCharName, 10) != 0))
            {
                // ±³È¯ÇÏ°íÀÚ Çß´ø ±× Ä³¸¯ÅÍ°¡ ¾Æ´Ï´Ù. ±³È¯ »óÅÂ´Â Ãë¼Ò.
                _ClearExchangeStatus(iClientH);
                _ClearExchangeStatus(iExH);
                return;
            }
            else
            {
                m_pClientList[iClientH]->m_bIsExchangeConfirm = true;
                if (m_pClientList[iExH]->m_bIsExchangeConfirm == true)
                {
                    // »ó´ë¹æµµ ±³È¯ÀÇ»ç¸¦ ¹àÇû´Ù. ±³È¯ÀÌ ¼º¸³ÇÒ ¼ö ÀÖ´ÂÁö °è»êÇÑ´Ù. ¾ÆÀÌÅÛ °¹¼ö, ¹«°Ô Á¦ÇÑÀ» ÆÇ´Ü.

                    //Check all items
                    for (int i = 0; i < m_pClientList[iClientH]->iExchangeCount; i++)
                    {
                        if ((m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]] == 0) ||
                            (memcmp(m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_cName, m_pClientList[iClientH]->m_cExchangeItemName[i], 20) != 0))
                        {
                            _ClearExchangeStatus(iClientH);
                            _ClearExchangeStatus(iExH);
                            return;
                        }
                    }
                    for (i = 0; i < m_pClientList[iExH]->iExchangeCount; i++)
                    {
                        if ((m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]] == 0) ||
                            (memcmp(m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]]->m_cName, m_pClientList[iExH]->m_cExchangeItemName[i], 20) != 0))
                        {
                            _ClearExchangeStatus(iClientH);
                            _ClearExchangeStatus(iExH);
                            return;
                        }
                    }

                    iWeightLeftA = _iCalcMaxLoad(iClientH) - iCalcTotalWeight(iClientH);
                    iWeightLeftB = _iCalcMaxLoad(iExH) - iCalcTotalWeight(iExH);

                    //Calculate weight for items
                    iItemWeightA = 0;
                    for (i = 0; i < m_pClientList[iClientH]->iExchangeCount; i++)
                    {
                        iItemWeightA = iGetItemWeight(m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]],
                            m_pClientList[iClientH]->m_iExchangeItemAmount[i]);
                    }
                    iItemWeightB = 0;
                    for (i = 0; i < m_pClientList[iExH]->iExchangeCount; i++)
                    {
                        iItemWeightB = iGetItemWeight(m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]],
                            m_pClientList[iExH]->m_iExchangeItemAmount[i]);
                    }

                    //See if the other person can take the item weightload
                    if ((iWeightLeftA < iItemWeightB) || (iWeightLeftB < iItemWeightA))
                    {
                        // ±³È¯ÇÏ°íÀÚ ÇÏ´Â ¾ÆÀÌÅÛÀ» ¹ÞÀ» ¹«°Ô°ø°£ÀÌ ¾ø´Ù. ±³È¯ ºÒ°¡´É. 
                        _ClearExchangeStatus(iClientH);
                        _ClearExchangeStatus(iExH);
                        return;
                    }

                    for (i = 0; i < m_pClientList[iClientH]->iExchangeCount; i++)
                    {
                        // ¼ö·®´ÜÀ§°¡ ÀÖ´Â ¾ÆÀÌÅÛÀÇ °æ¿ì´Â »õ·Î »ý¼ºÀ» ½ÃÄÑ¾ß ³ª´­¼ö ÀÖ´Ù.
                        if ((m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
                            (m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_ARROW))
                        {

                            if (m_pClientList[iClientH]->m_iExchangeItemAmount[i] > m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_dwCount)
                            {
                                // ±³È¯ÇÏ°íÀÚ Çß´ø ¼ö·®º¸´Ù ¾ÆÀÌÅÛÀÌ Àû´Ù. ±×µ¿¾È ÁÙ¾îµé¾ú´Ù.
                                _ClearExchangeStatus(iClientH);
                                _ClearExchangeStatus(iExH);
                                return;
                            }
                            pItemA[i] = new class CItem;
                            _bInitItemAttr(pItemA[i], m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_cName);
                            pItemA[i]->m_dwCount = m_pClientList[iClientH]->m_iExchangeItemAmount[i];

                            // ·Î±×¸¦ ³²±â±â À§ÇÑ ¾ÆÀÌÅÛ º¹»ç¹° 
                            pItemAcopy[i] = new class CItem;
                            _bInitItemAttr(pItemAcopy[i], m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_cName);
                            bCopyItemContents(pItemAcopy[i], pItemA[i]);
                            pItemAcopy[i]->m_dwCount = m_pClientList[iClientH]->m_iExchangeItemAmount[i];
                        }
                        else
                        {
                            pItemA[i] = (class CItem *)m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]];
                            pItemA[i]->m_dwCount = m_pClientList[iClientH]->m_iExchangeItemAmount[i];

                            // ·Î±×¸¦ ³²±â±â À§ÇÑ ¾ÆÀÌÅÛ º¹»ç¹° 
                            pItemAcopy[i] = new class CItem;
                            _bInitItemAttr(pItemAcopy[i], m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_cName);
                            bCopyItemContents(pItemAcopy[i], pItemA[i]);
                            pItemAcopy[i]->m_dwCount = m_pClientList[iClientH]->m_iExchangeItemAmount[i];
                        }
                    }

                    for (i = 0; i < m_pClientList[iExH]->iExchangeCount; i++)
                    {
                        if ((m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
                            (m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_ARROW))
                        {

                            if (m_pClientList[iExH]->m_iExchangeItemAmount[i] > m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]]->m_dwCount)
                            {
                                // ±³È¯ÇÏ°íÀÚ Çß´ø ¼ö·®º¸´Ù ¾ÆÀÌÅÛÀÌ Àû´Ù. ±×µ¿¾È ÁÙ¾îµé¾ú´Ù.
                                _ClearExchangeStatus(iClientH);
                                _ClearExchangeStatus(iExH);
                                return;
                            }
                            pItemB[i] = new class CItem;
                            _bInitItemAttr(pItemB[i], m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]]->m_cName);
                            pItemB[i]->m_dwCount = m_pClientList[iExH]->m_iExchangeItemAmount[i];

                            // ·Î±×¸¦ ³²±â±â À§ÇÑ ¾ÆÀÌÅÛ º¹»ç¹° 
                            pItemBcopy[i] = new class CItem;
                            _bInitItemAttr(pItemBcopy[i], m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]]->m_cName);
                            bCopyItemContents(pItemBcopy[i], pItemB[i]);
                            pItemBcopy[i]->m_dwCount = m_pClientList[iExH]->m_iExchangeItemAmount[i];
                        }
                        else
                        {
                            pItemB[i] = (class CItem *)m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]];
                            pItemB[i]->m_dwCount = m_pClientList[iExH]->m_iExchangeItemAmount[i];

                            // ·Î±×¸¦ ³²±â±â À§ÇÑ ¾ÆÀÌÅÛ º¹»ç¹° 
                            pItemBcopy[i] = new class CItem;
                            _bInitItemAttr(pItemBcopy[i], m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]]->m_cName);
                            bCopyItemContents(pItemBcopy[i], pItemB[i]);
                            pItemBcopy[i]->m_dwCount = m_pClientList[iExH]->m_iExchangeItemAmount[i];
                        }
                    }

                    // ¸ÕÀú ¾ÆÀÌÅÛÀ» ³ÖÀº ÈÄ 
                    for (i = 0; i < m_pClientList[iExH]->iExchangeCount; i++)
                    {
                        bAddItem(iClientH, pItemB[i], 0);
                        _bItemLog(DEF_ITEMLOG_EXCHANGE, iExH, iClientH, pItemBcopy[i]);
                        delete pItemBcopy[i];
                        pItemBcopy[i] = 0;
                        if ((m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
                            (m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_ARROW))
                        {
                            //
                            iAmountLeft = (int)m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]]->m_dwCount - m_pClientList[iExH]->m_iExchangeItemAmount[i];
                            if (iAmountLeft < 0) iAmountLeft = 0;
                            // v1.41 !!!
                            SetItemCount(iExH, m_pClientList[iExH]->m_cExchangeItemIndex[i], iAmountLeft);
                            // m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex]->m_cName, iAmountLeft);
                            //
                        }
                        else
                        {
                            // ¸¸¾à ÀåÂøµÈ ¾ÆÀÌÅÛÀÌ¶ó¸é ÇØÁ¦ÇÑ´Ù.
                            ReleaseItemHandler(iExH, m_pClientList[iExH]->m_cExchangeItemIndex[i], true);
                            SendNotifyMsg(0, iExH, DEF_NOTIFY_GIVEITEMFIN_ERASEITEM, m_pClientList[iExH]->m_cExchangeItemIndex[i], m_pClientList[iExH]->m_iExchangeItemAmount[i], 0, m_pClientList[iClientH]->m_cCharName);
                            m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]] = 0;
                        }
                    }

                    for (i = 0; i < m_pClientList[iClientH]->iExchangeCount; i++)
                    {
                        bAddItem(iExH, pItemA[i], 0);
                        _bItemLog(DEF_ITEMLOG_EXCHANGE, iClientH, iExH, pItemAcopy[i]);
                        delete pItemAcopy[i];
                        pItemAcopy[i] = 0;

                        if ((m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
                            (m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_ARROW))
                        {
                            //
                            iAmountLeft = (int)m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_dwCount - m_pClientList[iClientH]->m_iExchangeItemAmount[i];
                            if (iAmountLeft < 0) iAmountLeft = 0;
                            // v1.41 !!!
                            SetItemCount(iClientH, m_pClientList[iClientH]->m_cExchangeItemIndex[i], iAmountLeft);
                            // m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex]->m_cName, iAmountLeft);
                            //
                        }
                        else
                        {
                            // ¸¸¾à ÀåÂøµÈ ¾ÆÀÌÅÛÀÌ¶ó¸é ÇØÁ¦ÇÑ´Ù.
                            ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_cExchangeItemIndex[i], true);
                            SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIVEITEMFIN_ERASEITEM, m_pClientList[iClientH]->m_cExchangeItemIndex[i], m_pClientList[iClientH]->m_iExchangeItemAmount[i], 0, m_pClientList[iExH]->m_cCharName);
                            m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]] = 0;
                        }
                    }

                    m_pClientList[iClientH]->m_bIsExchangeMode = false;
                    m_pClientList[iClientH]->m_bIsExchangeConfirm = false;
                    memset(m_pClientList[iClientH]->m_cExchangeName, 0, sizeof(m_pClientList[iClientH]->m_cExchangeName));
                    m_pClientList[iClientH]->m_iExchangeH = 0;
                    m_pClientList[iClientH]->iExchangeCount = 0;

                    m_pClientList[iExH]->m_bIsExchangeMode = false;
                    m_pClientList[iExH]->m_bIsExchangeConfirm = false;
                    memset(m_pClientList[iExH]->m_cExchangeName, 0, sizeof(m_pClientList[iExH]->m_cExchangeName));
                    m_pClientList[iExH]->m_iExchangeH = 0;
                    m_pClientList[iExH]->iExchangeCount = 0;

                    for (i = 0; i < 4; i++)
                    {
                        m_pClientList[iClientH]->m_cExchangeItemIndex[i] = -1;
                        m_pClientList[iExH]->m_cExchangeItemIndex[i] = -1;
                    }

                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_EXCHANGEITEMCOMPLETE, 0, 0, 0, 0);
                    SendNotifyMsg(0, iExH, DEF_NOTIFY_EXCHANGEITEMCOMPLETE, 0, 0, 0, 0);

                    // ¹«°Ô Àç¼³Á¤
                    iCalcTotalWeight(iClientH);
                    iCalcTotalWeight(iExH);
                    return;
                }
            }
        }
        else
        {
            // ±³È¯ÇÒ »ó´ë¹æÀÌ ¾ø´Ù. ±³È¯Àº Ãë¼Ò µÈ´Ù.
            _ClearExchangeStatus(iClientH);
            return;
        }
    }
}

int CGame::_iGetItemSpaceLeft(int iClientH)
{
    int i, iTotalItem;

    iTotalItem = 0;
    for (i = 0; i < DEF_MAXITEMS; i++)
        if (m_pClientList[iClientH]->m_pItemList[i] != 0) iTotalItem++;

    return (DEF_MAXITEMS - iTotalItem);
}

bool CGame::bAddItem(int iClientH, CItem * pItem, char cMode)
{
    char * cp, cData[256];
    uint32_t * dwp;
    uint16_t * wp;
    short * sp;
    int iRet, iEraseReq;

    memset(cData, 0, sizeof(cData));
    if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true)
    {
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
        /*
        *cp = (char)(pItem->m_dwAttribute & 0x00000001); // Custom-ItemÃ€ÃŽÃÃ¶Ã€Ã‡ Â¿Â©ÂºÃŽ
        cp++;
        */

        if (iEraseReq == 1)
        {
            delete pItem;
            pItem = nullptr;
        }

        iRet = m_pClientList[iClientH]->send_msg(cData, 53);

        return true;
    }

    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
        m_pClientList[iClientH]->m_sY,
        pItem);

    SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
        m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
        pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);

    dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
    *dwp = MSGID_NOTIFY;
    wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
    *wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;

    iRet = m_pClientList[iClientH]->send_msg(cData, 6);

    return true;
}

int CGame::__iSearchForQuest(int iClientH, int iWho, int * pQuestType, int * pMode, int * pRewardType, int * pRewardAmount, int * pContribution, char * pTargetName, int * pTargetType, int * pTargetCount, int * pX, int * pY, int * pRange)
{
    int i, iQuestList[DEF_MAXQUESTTYPE]{}, iIndex, iQuest, iReward, iQuestIndex;

    if (m_pClientList[iClientH] == nullptr) return -1;

    iIndex = 0;
    for (i = 0; i < DEF_MAXQUESTTYPE; i++)
        iQuestList[i] = -1;

    for (i = 1; i < DEF_MAXQUESTTYPE; i++)
        if (m_pQuestConfigList[i] != 0)
        {

            if (m_pQuestConfigList[i]->m_iFrom != iWho) goto SFQ_SKIP;
            if (m_pQuestConfigList[i]->m_cSide != m_pClientList[iClientH]->m_cSide) goto SFQ_SKIP;
            if (m_pQuestConfigList[i]->m_iMinLevel > m_pClientList[iClientH]->m_iLevel) goto SFQ_SKIP;
            if (m_pQuestConfigList[i]->m_iMaxLevel < m_pClientList[iClientH]->m_iLevel) goto SFQ_SKIP;
            if (m_pQuestConfigList[i]->m_iReqContribution > m_pClientList[iClientH]->m_iContribution) goto SFQ_SKIP;

            if (m_pQuestConfigList[i]->m_iRequiredSkillNum != -1)
            {
                if (m_pClientList[iClientH]->m_cSkillMastery[m_pQuestConfigList[i]->m_iRequiredSkillNum] <
                    m_pQuestConfigList[i]->m_iRequiredSkillLevel) goto SFQ_SKIP;
            }

            if ((m_bIsCrusadeMode == true) && (m_pQuestConfigList[i]->m_iAssignType != 1)) goto SFQ_SKIP;
            if ((m_bIsCrusadeMode == false) && (m_pQuestConfigList[i]->m_iAssignType == 1)) goto SFQ_SKIP;

            if (m_pQuestConfigList[i]->m_iContributionLimit < m_pClientList[iClientH]->m_iContribution) goto SFQ_SKIP;

            iQuestList[iIndex] = i;
            iIndex++;

            SFQ_SKIP:;
        }

    if (iIndex == 0) return -1;
    iQuest = (iDice(1, iIndex)) - 1;
    iQuestIndex = iQuestList[iQuest];
    iReward = iDice(1, 3);
    *pMode = m_pQuestConfigList[iQuestIndex]->m_iResponseMode;
    *pRewardType = m_pQuestConfigList[iQuestIndex]->m_iRewardType[iReward];
    *pRewardAmount = m_pQuestConfigList[iQuestIndex]->m_iRewardAmount[iReward];
    *pContribution = m_pQuestConfigList[iQuestIndex]->m_iContribution;

    strcpy(pTargetName, m_pQuestConfigList[iQuestIndex]->m_cTargetName);
    *pX = m_pQuestConfigList[iQuestIndex]->m_sX;
    *pY = m_pQuestConfigList[iQuestIndex]->m_sY;
    *pRange = m_pQuestConfigList[iQuestIndex]->m_iRange;

    *pTargetType = m_pQuestConfigList[iQuestIndex]->m_iTargetType;
    *pTargetCount = m_pQuestConfigList[iQuestIndex]->m_iMaxCount;
    *pQuestType = m_pQuestConfigList[iQuestIndex]->m_iType;

    return iQuestIndex;
}

void CGame::_SendQuestContents(int iClientH)
{
    int iWho, iIndex, iQuestType, iContribution, iTargetType, iTargetCount, iX, iY, iRange, iQuestCompleted;
    char cTargetName[21];

    if (m_pClientList[iClientH] == nullptr) return;

    iIndex = m_pClientList[iClientH]->m_iQuest;
    if (iIndex == 0)
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTCONTENTS, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0);
    }
    else
    {
        iWho = m_pQuestConfigList[iIndex]->m_iFrom;
        iQuestType = m_pQuestConfigList[iIndex]->m_iType;
        iContribution = m_pQuestConfigList[iIndex]->m_iContribution;
        iTargetType = m_pQuestConfigList[iIndex]->m_iTargetType;
        iTargetCount = m_pQuestConfigList[iIndex]->m_iMaxCount;
        iX = m_pQuestConfigList[iIndex]->m_sX;
        iY = m_pQuestConfigList[iIndex]->m_sY;
        iRange = m_pQuestConfigList[iIndex]->m_iRange;
        memset(cTargetName, 0, sizeof(cTargetName));
        memcpy(cTargetName, m_pQuestConfigList[iIndex]->m_cTargetName, 20);
        iQuestCompleted = (int)m_pClientList[iClientH]->m_bIsQuestCompleted;

        SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTCONTENTS, iWho, iQuestType, iContribution, 0,
            iTargetType, iTargetCount, iX, iY, iRange, iQuestCompleted, cTargetName);
    }
}

void CGame::_CheckQuestEnvironment(int iClientH)
{
    int iIndex;
    char cTargetName[21];

    if (m_pClientList[iClientH] == nullptr) return;

    iIndex = m_pClientList[iClientH]->m_iQuest;
    if (iIndex == 0) return;
    if (m_pQuestConfigList[iIndex] == 0) return;

    if (iIndex >= 35 && iIndex <= 40)
    {
        m_pClientList[iClientH]->m_iQuest = 0;
        m_pClientList[iClientH]->m_iQuestID = 0;
        m_pClientList[iClientH]->m_iQuestRewardAmount = 0;
        m_pClientList[iClientH]->m_iQuestRewardType = 0;
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTABORTED, 0, 0, 0, 0);
        return;
    }

    if (m_pQuestConfigList[iIndex]->m_iQuestID != m_pClientList[iClientH]->m_iQuestID)
    {
        m_pClientList[iClientH]->m_iQuest = 0;
        m_pClientList[iClientH]->m_iQuestID = 0;
        m_pClientList[iClientH]->m_iQuestRewardAmount = 0;
        m_pClientList[iClientH]->m_iQuestRewardType = 0;

        SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTABORTED, 0, 0, 0, 0);
        return;
    }

    switch (m_pQuestConfigList[iIndex]->m_iType)
    {
        case DEF_QUESTTYPE_MONSTERHUNT:
        case DEF_QUESTTYPE_GOPLACE:
            memset(cTargetName, 0, sizeof(cTargetName));
            memcpy(cTargetName, m_pQuestConfigList[iIndex]->m_cTargetName, 20);
            if (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, cTargetName, 10) == 0)
                m_pClientList[iClientH]->m_bQuestMatchFlag_Loc = true;
            else m_pClientList[iClientH]->m_bQuestMatchFlag_Loc = false;
            break;
    }

}

bool CGame::_bCheckIsQuestCompleted(int iClientH)
{
    int iQuestIndex;

    if (m_pClientList[iClientH] == nullptr) return false;
    if (m_pClientList[iClientH]->m_bIsQuestCompleted == true) return false;
    iQuestIndex = m_pClientList[iClientH]->m_iQuest;
    if (iQuestIndex == 0) return false;

    if (m_pQuestConfigList[iQuestIndex] != 0)
    {
        switch (m_pQuestConfigList[iQuestIndex]->m_iType)
        {
            case DEF_QUESTTYPE_MONSTERHUNT:
                if ((m_pClientList[iClientH]->m_bQuestMatchFlag_Loc == true) &&
                    (m_pClientList[iClientH]->m_iCurQuestCount >= m_pQuestConfigList[iQuestIndex]->m_iMaxCount))
                {
                    m_pClientList[iClientH]->m_bIsQuestCompleted = true;
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTCOMPLETED, 0, 0, 0, 0);
                    return true;
                }
                break;

            case DEF_QUESTTYPE_GOPLACE:
                if ((m_pClientList[iClientH]->m_bQuestMatchFlag_Loc == true) &&
                    (m_pClientList[iClientH]->m_sX >= m_pQuestConfigList[iQuestIndex]->m_sX - m_pQuestConfigList[iQuestIndex]->m_iRange) &&
                    (m_pClientList[iClientH]->m_sX <= m_pQuestConfigList[iQuestIndex]->m_sX + m_pQuestConfigList[iQuestIndex]->m_iRange) &&
                    (m_pClientList[iClientH]->m_sY >= m_pQuestConfigList[iQuestIndex]->m_sY - m_pQuestConfigList[iQuestIndex]->m_iRange) &&
                    (m_pClientList[iClientH]->m_sY <= m_pQuestConfigList[iQuestIndex]->m_sY + m_pQuestConfigList[iQuestIndex]->m_iRange))
                {
                    m_pClientList[iClientH]->m_bIsQuestCompleted = true;
                    SendNotifyMsg(0, iClientH, DEF_NOTIFY_QUESTCOMPLETED, 0, 0, 0, 0);
                    return true;
                }
                break;
        }
    }

    return false;
}

void CGame::SendItemNotifyMsg(int iClientH, uint16_t wMsgType, CItem * pItem, int iV1)
{
    char * cp, cData[512]{};
    uint32_t * dwp;
    uint16_t * wp;
    short * sp;
    int     iRet;

    if (m_pClientList[iClientH] == nullptr) return;

    dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
    *dwp = MSGID_NOTIFY;
    wp = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
    *wp = wMsgType;
    cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);

    switch (wMsgType)
    {
        case DEF_NOTIFY_ITEMOBTAINED:
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

            iRet = m_pClientList[iClientH]->send_msg(cData, 53);
            break;

        case DEF_NOTIFY_ITEMPURCHASED:
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

            wp = (uint16_t *)cp;
            *wp = iV1; // (iCost - iDiscountCost);

            iRet = m_pClientList[iClientH]->send_msg(cData, 48);
            break;

        case DEF_NOTIFY_CANNOTCARRYMOREITEM:
            iRet = m_pClientList[iClientH]->send_msg(cData, 6);
            break;
    }
}

bool CGame::_bCheckItemReceiveCondition(int iClientH, CItem * pItem)
{
    int i;

    if (m_pClientList[iClientH] == nullptr) return false;

    if (m_pClientList[iClientH]->m_iCurWeightLoad + iGetItemWeight(pItem, pItem->m_dwCount) > (uint32_t)_iCalcMaxLoad(iClientH))
        return false;

    for (i = 0; i < DEF_MAXITEMS; i++)
        if (m_pClientList[iClientH]->m_pItemList[i] == 0) return true;

    return false;
}

void CGame::_ClearQuestStatus(int iClientH)
{
    if (m_pClientList[iClientH] == nullptr) return;

    m_pClientList[iClientH]->m_iQuest = 0;
    m_pClientList[iClientH]->m_iQuestID = 0;
    m_pClientList[iClientH]->m_iQuestRewardType = 0;
    m_pClientList[iClientH]->m_iQuestRewardAmount = 0;
    m_pClientList[iClientH]->m_bIsQuestCompleted = false;
}

int CGame::iGetMaxHP(int iClientH)
{
    int iRet;

    if (m_pClientList[iClientH] == nullptr) return 0;

    iRet = m_pClientList[iClientH]->m_iVit * 3 + m_pClientList[iClientH]->m_iLevel * 2 + m_pClientList[iClientH]->m_iStr / 2;

    if (m_pClientList[iClientH]->m_iSideEffect_MaxHPdown != 0)
        iRet = iRet - (iRet / m_pClientList[iClientH]->m_iSideEffect_MaxHPdown);

    return iRet;
}

int CGame::iGetMaxMP(int iClientH)
{
    int iRet;

    if (m_pClientList[iClientH] == nullptr) return 0;

    iRet = (2 * m_pClientList[iClientH]->m_iMag) + (2 * m_pClientList[iClientH]->m_iLevel) + (m_pClientList[iClientH]->m_iInt / 2);

    return iRet;
}

int CGame::iGetMaxSP(int iClientH)
{
    int iRet;

    if (m_pClientList[iClientH] == nullptr) return 0;

    iRet = (2 * m_pClientList[iClientH]->m_iStr) + (2 * m_pClientList[iClientH]->m_iLevel);

    return iRet;
}

void CGame::GetMapInitialPoint(int iMapIndex, short * pX, short * pY, char * pPlayerLocation)
{
    int i, iTotalPoint;
    POINT  pList[DEF_MAXINITIALPOINT];

    if (m_pMapList[iMapIndex] == 0) return;

    iTotalPoint = 0;
    for (i = 0; i < DEF_MAXINITIALPOINT; i++)
        if (m_pMapList[iMapIndex]->m_pInitialPoint[i].x != -1)
        {
            pList[iTotalPoint].x = m_pMapList[iMapIndex]->m_pInitialPoint[i].x;
            pList[iTotalPoint].y = m_pMapList[iMapIndex]->m_pInitialPoint[i].y;
            iTotalPoint++;
        }

    if (iTotalPoint == 0) return;

    if ((pPlayerLocation != 0) && (memcmp(pPlayerLocation, "NONE", 4) == 0))
        i = 0;
    else i = iDice(1, iTotalPoint) - 1;

    *pX = pList[i].x;
    *pY = pList[i].y;
}

void CGame::_CheckStrategicPointOccupyStatus(char cMapIndex)
{
    class CTile * pTile;
    int i, iX, iY, iSide, iValue;

    m_iStrategicStatus = 0;

    for (i = 0; i < DEF_MAXSTRATEGICPOINTS; i++)
        if (m_pMapList[cMapIndex]->m_pStrategicPointList[i] != 0)
        {

            iSide = m_pMapList[cMapIndex]->m_pStrategicPointList[i]->m_iSide;
            iValue = m_pMapList[cMapIndex]->m_pStrategicPointList[i]->m_iValue;
            iX = m_pMapList[cMapIndex]->m_pStrategicPointList[i]->m_iX;
            iY = m_pMapList[cMapIndex]->m_pStrategicPointList[i]->m_iY;

            pTile = (class CTile *)(m_pMapList[cMapIndex]->m_pTile + iX + iY * m_pMapList[cMapIndex]->m_sSizeY);

            m_iStrategicStatus += pTile->m_iOccupyStatus * iValue;
        }
}

char CGame::_cGetSpecialAbility(int iKindSA)
{
    char cSA;

    switch (iKindSA)
    {
        case 1:
            // Slime, Orc, Orge, WereWolf, YB-, Rabbit, Mountain-Giant, Stalker, Hellclaw, 
            // Wyvern, Fire-Wyvern, Barlog, Tentocle, Centaurus, Giant-Lizard, Minotaurus,
            // Abaddon, Claw-Turtle, Giant-Cray-Fish, Giant-Plant, MasterMage-Orc, Nizie,
            // Tigerworm
            switch (iDice(1, 2))
            {
                case 1: cSA = 3; break; // Anti-Physical Damage
                case 2: cSA = 4; break; // Anti-Magic Damage
            }
            break;

        case 2:
            // Giant-Ant, Cat, Giant-Frog, 
            switch (iDice(1, 3))
            {
                case 1: cSA = 3; break; // Anti-Physical Damage
                case 2: cSA = 4; break; // Anti-Magic Damage
                case 3: cSA = 5; break; // Poisonous
            }
            break;

        case 3:
            // Zombie, Scorpion, Amphis, Troll, Dark-Elf
            switch (iDice(1, 4))
            {
                case 1: cSA = 3; break; // Anti-Physical Damage
                case 2: cSA = 4; break; // Anti-Magic Damage
                case 3: cSA = 5; break; // Poisonous
                case 4: cSA = 6; break; // Critical Poisonous
            }
            break;

        case 4:
            // no linked Npc
            switch (iDice(1, 3))
            {
                case 1: cSA = 3; break; // Anti-Physical Damage
                case 2: cSA = 4; break; // Anti-Magic Damage
                case 3: cSA = 7; break; // Explosive
            }
            break;

        case 5:
            // Stone-Golem, Clay-Golem, Beholder, Cannibal-Plant, Rudolph, DireBoar
            switch (iDice(1, 4))
            {
                case 1: cSA = 3; break; // Anti-Physical Damage
                case 2: cSA = 4; break; // Anti-Magic Damage
                case 3: cSA = 7; break; // Explosive
                case 4: cSA = 8; break; // Critical-Explosive
            }
            break;

        case 6:
            // no linked Npc
            switch (iDice(1, 3))
            {
                case 1: cSA = 3; break; // Anti-Physical Damage
                case 2: cSA = 4; break; // Anti-Magic Damage
                case 3: cSA = 5; break; // Poisonous
            }
            break;

        case 7:
            // Orc-Mage, Unicorn
            switch (iDice(1, 3))
            {
                case 1: cSA = 1; break; // Clairvoyant
                case 2: cSA = 2; break; // Distruction of Magic Protection
                case 3: cSA = 4; break; // Anti-Magic Damage
            }
            break;

        case 8:
            // Frost, Ice-Golem, Ettin, Gagoyle, Demon, Liche, Hellbound, Cyclops, 
            // Skeleton
            switch (iDice(1, 5))
            {
                case 1: cSA = 1; break; // Clairvoyant
                case 2: cSA = 2; break; // Distruction of Magic Protection
                case 3: cSA = 4; break; // Anti-Magic Damage
                case 4: cSA = 3; break; // Anti-Physical Damage
                case 5: cSA = 8; break; // Critical-Explosive
            }
            break;

        case 9:
            // no linked Npc
            cSA = iDice(1, 8); // All abilities available
            break;
    }

    return cSA;
}

void CGame::CheckSpecialEvent(int iClientH)
{
    class CItem * pItem;
    char  cItemName[21];
    int   iEraseReq;

    if (m_pClientList[iClientH] == nullptr) return;

    if (m_pClientList[iClientH]->m_iSpecialEventID == 200081)
    {

        if (m_pClientList[iClientH]->m_iLevel < 11)
        {
            m_pClientList[iClientH]->m_iSpecialEventID = 0;
            return;
        }

        memset(cItemName, 0, sizeof(cItemName));
        strcpy(cItemName, "MemorialRing");

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

                log->info("(*) Get MemorialRing  : Char({})", m_pClientList[iClientH]->m_cCharName);

                pItem->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
                pItem->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
                pItem->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
                pItem->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;
                pItem->m_cItemColor = 9;

                m_pClientList[iClientH]->m_iSpecialEventID = 0;
            }
        }
    }
}

bool CGame::_bCheckDupItemID(CItem * pItem)
{
    int i;

    for (i = 0; i < DEF_MAXDUPITEMID; i++)
        if (m_pDupItemIDList[i] != 0)
        {
            if ((pItem->m_sTouchEffectType == m_pDupItemIDList[i]->m_sTouchEffectType) &&
                (pItem->m_sTouchEffectValue1 == m_pDupItemIDList[i]->m_sTouchEffectValue1) &&
                (pItem->m_sTouchEffectValue2 == m_pDupItemIDList[i]->m_sTouchEffectValue2) &&
                (pItem->m_sTouchEffectValue3 == m_pDupItemIDList[i]->m_sTouchEffectValue3))
            {
                pItem->m_wPrice = m_pDupItemIDList[i]->m_wPrice;
                return true;
            }
        }

    return false;
}

void CGame::_AdjustRareItemValue(CItem * pItem)
{
    uint32_t dwSWEType, dwSWEValue;
    double dV1, dV2, dV3;

    if ((pItem->m_dwAttribute & 0x00F00000) != 0)
    {
        dwSWEType = (pItem->m_dwAttribute & 0x00F00000) >> 20;
        dwSWEValue = (pItem->m_dwAttribute & 0x000F0000) >> 16;
        // ÃˆÃ±Â±Ã Â¾Ã†Ã€ÃŒÃ…Ã› ÃˆÂ¿Â°Ãº ÃÂ¾Â·Ã¹: 
        // 0-None 1-Ã‡ÃŠÂ»Ã¬Â±Ã¢Â´Ã«Â¹ÃŒÃÃ¶ÃƒÃŸÂ°Â¡ 2-ÃÃŸÂµÂ¶ÃˆÂ¿Â°Ãº 3-ÃÂ¤Ã€Ã‡Ã€Ã‡ 
        // 5-Â¹ÃŽÃƒÂ¸Ã€Ã‡ 6-Â°Â¡ÂºÂ­Â¿Ã® 7-Â¿Â¹Â¸Â®Ã‡Ã‘ 8-Â°Â­ÃˆÂ­ÂµÃˆ 9-Â°Ã­Â´Ã«Â¹Â®Â¸Ã­Ã€Ã‡
        switch (dwSWEType)
        {
            case 0: break;

            case 5: // Â¹ÃŽÃƒÂ¸Ã€Ã‡ 
                pItem->m_cSpeed--;
                if (pItem->m_cSpeed < 0) pItem->m_cSpeed = 0;
                break;

            case 6: // Â°Â¡ÂºÂ­Â¿Ã® 
                dV2 = (double)pItem->m_wWeight;
                dV3 = (double)(dwSWEValue * 4);
                dV1 = (dV3 / 100.0f) * dV2;
                pItem->m_wWeight -= (int)dV1;

                if (pItem->m_wWeight < 1) pItem->m_wWeight = 1;
                break;

            case 8: // Â°Â­ÃˆÂ­ÂµÃˆ 
            case 9: // Â°Ã­Â´Ã«Â¹Â®Â¸Ã­Ã€Ã‡ 
                dV2 = (double)pItem->m_wMaxLifeSpan;
                dV3 = (double)(dwSWEValue * 7);
                dV1 = (dV3 / 100.0f) * dV2;
                pItem->m_wMaxLifeSpan += (int)dV1;
                break;
        }
    }
}

int CGame::iRequestPanningMapDataRequest(int iClientH, char * pData)
{
    char * cp, cDir, cData[3000];
    uint32_t * dwp;
    uint16_t * wp;
    short * sp, dX, dY;
    int   iRet, iSize;

    if (m_pClientList[iClientH] == nullptr) return 0;
    if (m_pClientList[iClientH]->m_bIsObserverMode == false) return 0;
    if (m_pClientList[iClientH]->m_bIsKilled == true) return 0;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return 0;

    dX = m_pClientList[iClientH]->m_sX;
    dY = m_pClientList[iClientH]->m_sY;

    cDir = *(pData + DEF_INDEX2_MSGTYPE + 2);
    if ((cDir <= 0) || (cDir > 8)) return 0;

    switch (cDir)
    {
        case 1:	dY--; break; // responding when mouse is placed north
        case 2:	dX++; dY--;	break;
        case 3:	dX++; break;
        case 4:	dX++; dY++;	break;
        case 5: dY++; break;
        case 6:	dX--; dY++;	break;
        case 7:	dX--; break; // responding when mouse placed at west side of screen
        case 8:	dX--; dY--;	break; // responding when mouse is placed north west
            /*
            player is in the center, and is trying to pan,
            directions not responding or causing a break will be kept as X,
            others are the cDir case...
                    8	1	X

                    7	_	X

                    X	X	X
            */
    }

    m_pClientList[iClientH]->m_sX = dX;
    m_pClientList[iClientH]->m_sY = dY;
    m_pClientList[iClientH]->m_cDir = cDir;

    dwp = (uint32_t *)(cData + DEF_INDEX4_MSGID);
    *dwp = MSGID_RESPONSE_PANNING;
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

    iSize = iComposeMoveMapData((short)(dX - 10), (short)(dY - 7), iClientH, cDir, cp);
    iRet = m_pClientList[iClientH]->send_msg(cData, iSize + 12 + 1 + 4);
    switch (iRet)
    {
        case DEF_XSOCKEVENT_QUENEFULL:
        case DEF_XSOCKEVENT_SOCKETERROR:
        case DEF_XSOCKEVENT_CRITICALERROR:
        case DEF_XSOCKEVENT_SOCKETCLOSED:
            // Â¸ÃžÂ½ÃƒÃÃ¶Â¸Â¦ ÂºÂ¸Â³Â¾Â¶Â§ Â¿Â¡Â·Â¯Â°Â¡ Â¹ÃŸÂ»Ã½Ã‡ÃŸÂ´Ã™Â¸Ã© ÃÂ¦Â°Ã…Ã‡Ã‘Â´Ã™.
            DeleteClient(iClientH, true, true);
            return 0;
    }

    return 1;
}

void CGame::_DeleteRandomOccupyFlag(int iMapIndex)
{
    int i, iCount, iTotalFlags, iTargetFlag, iDynamicObjectIndex;
    int tx, ty, fx, fy, iLocalSide, iLocalEKNum, iPrevStatus;
    class CTile * pTile;
    uint32_t dwTime;

    if (m_pMapList[iMapIndex] == 0) return;

    dwTime = timeGetTime();

    // ÃƒÃ‘ Â±ÃªÂ¹ÃŸ Â°Â¹Â¼Ã¶Â¸Â¦ Â±Â¸Ã‡Ã‘Â´Ã™.
    iTotalFlags = 0;
    for (i = 1; i < DEF_MAXOCCUPYFLAG; i++)
        if (m_pMapList[iMapIndex]->m_pOccupyFlag[i] != 0)
        {
            iTotalFlags++;
        }

    // Â·Â£Â´Ã½Ã‡ÃÂ°Ã” Ã‡ÃÂ³Âª ÃÂ¤Ã‡Ã‘Â´Ã™.
    iTargetFlag = iDice(1, iTotalFlags);

    iCount = 0;
    for (i = 1; i < DEF_MAXOCCUPYFLAG; i++)
        if (m_pMapList[iMapIndex]->m_pOccupyFlag[i] != 0)
        {
            iCount++;
            if ((iCount == iTotalFlags) && (m_pMapList[iMapIndex]->m_pOccupyFlag[i] != 0))
            {
                // m_pMapList[iMapIndex]->m_pOccupyFlag[i] : Ã€ÃŒ Â±ÃªÂ¹ÃŸÃ€Â» Â¾Ã¸Â¾Ã˜Â´Ã™.	

                //testcode
                wsprintf(G_cTxt, "(*)Delete OccupyFlag: Side(%d) XY(%d, %d)", m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_cSide, m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_sX, m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_sY);
                log->info(G_cTxt);

                fx = m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_sX;
                fy = m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_sY;
                iLocalSide = m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_cSide;
                iLocalEKNum = m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_iEKCount;

                pTile = (class CTile *)(m_pMapList[iMapIndex]->m_pTile + m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_sX +
                    m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_sY * m_pMapList[iMapIndex]->m_sSizeY);
                // Â±ÃªÂ¹ÃŸ Â°Â´ÃƒÂ¼ Â°Â¨Â¼Ã’ 
                m_pMapList[iMapIndex]->m_iTotalOccupyFlags--;

                iDynamicObjectIndex = m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_iDynamicObjectIndex;
                // Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â¿Â¡Â°Ã” Â±ÃªÂ¹ÃŸÃ€ÃŒ Â»Ã§Â¶Ã³ÃÃ¼Ã€Â» Â¾Ã‹Â¸Â®Â°Ã­ 
                SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, DEF_MSGTYPE_REJECT, m_pDynamicObjectList[iDynamicObjectIndex]->m_cMapIndex,
                    m_pDynamicObjectList[iDynamicObjectIndex]->m_sX, m_pDynamicObjectList[iDynamicObjectIndex]->m_sY,
                    m_pDynamicObjectList[iDynamicObjectIndex]->m_sType, iDynamicObjectIndex, 0);
                // Â¸ÃŠÂ¿Â¡Â¼Â­ Â±ÃªÂ¹ÃŸ ÂµÂ¿Ã€Ã» Â°Â´ÃƒÂ¼Â¸Â¦ Â»Ã¨ÃÂ¦Ã‡Ã‘Â´Ã™.
                m_pMapList[m_pDynamicObjectList[iDynamicObjectIndex]->m_cMapIndex]->SetDynamicObject(0, 0, m_pDynamicObjectList[iDynamicObjectIndex]->m_sX, m_pDynamicObjectList[iDynamicObjectIndex]->m_sY, dwTime);

                // Â±ÃªÂ¹ÃŸ Â°Â´ÃƒÂ¼Â¸Â¦ Â»Ã¨ÃÂ¦ 
                delete m_pMapList[iMapIndex]->m_pOccupyFlag[i];
                m_pMapList[iMapIndex]->m_pOccupyFlag[i] = 0;

                // Ã…Â¸Ã€Ã Â»Ã³Ã€Ã‡ Â±ÃªÂ¹ÃŸ Ã€ÃŽÂµÂ¦Â½ÂºÂ¸Â¦ Â»Ã¨ÃÂ¦
                pTile->m_iOccupyFlagIndex = 0;

                // ÂµÂ¿Ã€Ã» Â°Â´ÃƒÂ¼ Â»Ã¨ÃÂ¦ 
                delete m_pDynamicObjectList[iDynamicObjectIndex];
                m_pDynamicObjectList[iDynamicObjectIndex] = 0;

                // Ã€ÃŒ Â±ÃªÂ¹ÃŸÃ€Â» Â»Ã¨ÃÂ¦Ã‡ÃÂ¹Ã‡Â·ÃŽ Â¿ÂµÃ‡Ã¢Â±Ã‡ Â³Â»Ã€Ã‡ Â°ÂªÃ€Â» ÂºÂ¯Â°Ã¦Â½ÃƒÃ„Ã‘Â¾ÃŸ Ã‡Ã‘Â´Ã™. *** Â¸Ã‡ Â¸Â¶ÃÃ¶Â¸Â·Â¿Â¡ ÃƒÂ³Â¸Â®Ã‡Ã˜Â¾ÃŸ Ã‡Ã‘Â´Ã™.
                for (tx = fx - 10; tx <= fx + 10; tx++)
                    for (ty = fy - 8; ty <= fy + 8; ty++)
                    {
                        if ((tx < 0) || (tx >= m_pMapList[iMapIndex]->m_sSizeX) ||
                            (ty < 0) || (ty >= m_pMapList[iMapIndex]->m_sSizeY))
                        {
                            // Â¹Â«Â½ÃƒÃ‡Ã˜Â¾ÃŸ Ã‡ÃÂ´Ã‚ ÃÃ‚Ã‡Â¥ 
                        }
                        else
                        {
                            pTile = (class CTile *)(m_pMapList[iMapIndex]->m_pTile + tx + ty * m_pMapList[iMapIndex]->m_sSizeY);
                            iPrevStatus = pTile->m_iOccupyStatus;
                            // Side: Â¾Ã†Â·Â¹Â½ÂºÂµÂ§(1)  Â¿Â¤Â¹Ã™Ã€ÃŽ(2)
                            switch (iLocalSide)
                            {
                                case 1:
                                    pTile->m_iOccupyStatus += iLocalEKNum;
                                    if (pTile->m_iOccupyStatus > 0) pTile->m_iOccupyStatus = 0;
                                    break;
                                case 2:
                                    pTile->m_iOccupyStatus -= iLocalEKNum;
                                    if (pTile->m_iOccupyStatus < 0) pTile->m_iOccupyStatus = 0;
                                    break;
                            }
                        }
                    }
                //
                return;
            }
        }
}

bool CGame::bCheckEnergySphereDestination(int iNpcH, short sAttackerH, char cAttackerType)
{
    int i, sX, sY, dX, dY, iGoalMapIndex;
    char cResult;

    if (m_pNpcList[iNpcH] == 0) return false;
    if (m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->m_iCurEnergySphereGoalPointIndex == -1) return false;

    if (m_pNpcList[iNpcH]->m_cMapIndex != m_iMiddlelandMapIndex)
    {
        iGoalMapIndex = m_pNpcList[iNpcH]->m_cMapIndex;

        sX = m_pNpcList[iNpcH]->m_sX;
        sY = m_pNpcList[iNpcH]->m_sY;

        cResult = m_pMapList[iGoalMapIndex]->m_stEnergySphereGoalList[m_pMapList[iGoalMapIndex]->m_iCurEnergySphereGoalPointIndex].cResult;
        dX = m_pMapList[iGoalMapIndex]->m_stEnergySphereGoalList[m_pMapList[iGoalMapIndex]->m_iCurEnergySphereGoalPointIndex].aresdenX;
        dY = m_pMapList[iGoalMapIndex]->m_stEnergySphereGoalList[m_pMapList[iGoalMapIndex]->m_iCurEnergySphereGoalPointIndex].aresdenY;
        if ((sX >= dX - 2) && (sX <= dX + 2) && (sY >= dY - 2) && (sY <= dY + 2))
        {
            m_pMapList[iGoalMapIndex]->m_iCurEnergySphereGoalPointIndex = -1;

            if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH] != 0))
            {
                if (m_pClientList[sAttackerH]->m_cSide == 1)
                {
                    m_pClientList[sAttackerH]->m_iContribution += 5;
                    log->info("(!) EnergySphere Hit By Aresden Player ({})", m_pClientList[sAttackerH]->m_cCharName);
                }
                else
                {
                    m_pClientList[sAttackerH]->m_iContribution -= 10;
                }

                for (i = 1; i < DEF_MAXCLIENTS; i++)
                    if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_bIsInitComplete == true))
                    {
                        SendNotifyMsg(0, i, DEF_NOTIFY_ENERGYSPHEREGOALIN, cResult, m_pClientList[sAttackerH]->m_cSide, 2, m_pClientList[sAttackerH]->m_cCharName);
                    }
            }
            return true;
        }

        dX = m_pMapList[iGoalMapIndex]->m_stEnergySphereGoalList[m_pMapList[iGoalMapIndex]->m_iCurEnergySphereGoalPointIndex].elvineX;
        dY = m_pMapList[iGoalMapIndex]->m_stEnergySphereGoalList[m_pMapList[iGoalMapIndex]->m_iCurEnergySphereGoalPointIndex].elvineY;
        if ((sX >= dX - 2) && (sX <= dX + 2) && (sY >= dY - 2) && (sY <= dY + 2))
        {
            m_pMapList[iGoalMapIndex]->m_iCurEnergySphereGoalPointIndex = -1;

            if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH] != 0))
            {
                if (m_pClientList[sAttackerH]->m_cSide == 2)
                {
                    m_pClientList[sAttackerH]->m_iContribution += 5;
                    log->info("(!) EnergySphere Hit By Elvine Player ({})", m_pClientList[sAttackerH]->m_cCharName);
                }
                else
                {
                    m_pClientList[sAttackerH]->m_iContribution -= 10;
                }

                for (i = 1; i < DEF_MAXCLIENTS; i++)
                    if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_bIsInitComplete == true))
                    {
                        SendNotifyMsg(0, i, DEF_NOTIFY_ENERGYSPHEREGOALIN, cResult, m_pClientList[sAttackerH]->m_cSide, 1, m_pClientList[sAttackerH]->m_cCharName);
                    }
            }
        }
        return false;
    }
    else
    {
        sX = m_pNpcList[iNpcH]->m_sX;
        sY = m_pNpcList[iNpcH]->m_sY;

        cResult = m_pMapList[m_iMiddlelandMapIndex]->m_stEnergySphereGoalList[m_pMapList[m_iMiddlelandMapIndex]->m_iCurEnergySphereGoalPointIndex].cResult;
        dX = m_pMapList[m_iMiddlelandMapIndex]->m_stEnergySphereGoalList[m_pMapList[m_iMiddlelandMapIndex]->m_iCurEnergySphereGoalPointIndex].aresdenX;
        dY = m_pMapList[m_iMiddlelandMapIndex]->m_stEnergySphereGoalList[m_pMapList[m_iMiddlelandMapIndex]->m_iCurEnergySphereGoalPointIndex].aresdenY;
        if ((sX >= dX - 4) && (sX <= dX + 4) && (sY >= dY - 4) && (sY <= dY + 4))
        {
            m_pMapList[m_iMiddlelandMapIndex]->m_iCurEnergySphereGoalPointIndex = -1;

            if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH] != 0))
            {
                if (m_pClientList[sAttackerH]->m_cSide == 1)
                {
                    m_pClientList[sAttackerH]->m_iContribution += 5;
                    log->info("(!) EnergySphere Hit By Aresden Player ({})", m_pClientList[sAttackerH]->m_cCharName);
                }
                else
                {
                    m_pClientList[sAttackerH]->m_iContribution -= 10;
                }

                for (i = 1; i < DEF_MAXCLIENTS; i++)
                    if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_bIsInitComplete == true))
                    {
                        SendNotifyMsg(0, i, DEF_NOTIFY_ENERGYSPHEREGOALIN, cResult, m_pClientList[sAttackerH]->m_cSide, 2, m_pClientList[sAttackerH]->m_cCharName);
                    }
            }
            return true;
        }

        dX = m_pMapList[m_iMiddlelandMapIndex]->m_stEnergySphereGoalList[m_pMapList[m_iMiddlelandMapIndex]->m_iCurEnergySphereGoalPointIndex].elvineX;
        dY = m_pMapList[m_iMiddlelandMapIndex]->m_stEnergySphereGoalList[m_pMapList[m_iMiddlelandMapIndex]->m_iCurEnergySphereGoalPointIndex].elvineY;
        if ((sX >= dX - 4) && (sX <= dX + 4) && (sY >= dY - 4) && (sY <= dY + 4))
        {
            m_pMapList[m_iMiddlelandMapIndex]->m_iCurEnergySphereGoalPointIndex = -1;

            if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH] != 0))
            {
                if (m_pClientList[sAttackerH]->m_cSide == 2)
                {
                    m_pClientList[sAttackerH]->m_iContribution += 5;
                    log->info("(!) EnergySphere Hit By Aresden Player ({})", m_pClientList[sAttackerH]->m_cCharName);
                }
                else
                {
                    m_pClientList[sAttackerH]->m_iContribution -= 10;
                }

                for (i = 1; i < DEF_MAXCLIENTS; i++)
                    if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_bIsInitComplete == true))
                    {
                        SendNotifyMsg(0, i, DEF_NOTIFY_ENERGYSPHEREGOALIN, cResult, m_pClientList[sAttackerH]->m_cSide, 1, m_pClientList[sAttackerH]->m_cCharName);
                    }
            }
            return true;
        }
        return false;
    }
}

int CGame::iGetItemWeight(CItem * pItem, int iCount)
{
    int iWeight;

    iWeight = (pItem->m_wWeight);
    if (iCount < 0) iCount = 1;
    iWeight = iWeight * iCount;
    if (pItem->m_sIDnum == 90) iWeight = iWeight / 20;
    if (iWeight <= 0) iWeight = 1;

    return iWeight;
}

void CGame::UpdateMapSectorInfo()
{
    int i, ix, iy;
    int iMaxNeutralActivity, iMaxAresdenActivity, iMaxElvineActivity, iMaxMonsterActivity, iMaxPlayerActivity;

    for (i = 0; i < DEF_MAXMAPS; i++)
        if (m_pMapList[i] != 0)
        {

            iMaxNeutralActivity = iMaxAresdenActivity = iMaxElvineActivity = iMaxMonsterActivity = iMaxPlayerActivity = 0;
            m_pMapList[i]->m_iMaxNx = m_pMapList[i]->m_iMaxNy = m_pMapList[i]->m_iMaxAx = m_pMapList[i]->m_iMaxAy = 0;
            m_pMapList[i]->m_iMaxEx = m_pMapList[i]->m_iMaxEy = m_pMapList[i]->m_iMaxMx = m_pMapList[i]->m_iMaxMy = 0;
            m_pMapList[i]->m_iMaxPx = m_pMapList[i]->m_iMaxPy = 0;

            for (ix = 0; ix < DEF_MAXSECTORS; ix++)
                for (iy = 0; iy < DEF_MAXSECTORS; iy++)
                {
                    if (m_pMapList[i]->m_stTempSectorInfo[ix][iy].iNeutralActivity > iMaxNeutralActivity)
                    {
                        iMaxNeutralActivity = m_pMapList[i]->m_stTempSectorInfo[ix][iy].iNeutralActivity;
                        m_pMapList[i]->m_iMaxNx = ix;
                        m_pMapList[i]->m_iMaxNy = iy;
                    }

                    if (m_pMapList[i]->m_stTempSectorInfo[ix][iy].iAresdenActivity > iMaxAresdenActivity)
                    {
                        iMaxAresdenActivity = m_pMapList[i]->m_stTempSectorInfo[ix][iy].iAresdenActivity;
                        m_pMapList[i]->m_iMaxAx = ix;
                        m_pMapList[i]->m_iMaxAy = iy;
                    }

                    if (m_pMapList[i]->m_stTempSectorInfo[ix][iy].iElvineActivity > iMaxElvineActivity)
                    {
                        iMaxElvineActivity = m_pMapList[i]->m_stTempSectorInfo[ix][iy].iElvineActivity;
                        m_pMapList[i]->m_iMaxEx = ix;
                        m_pMapList[i]->m_iMaxEy = iy;
                    }

                    if (m_pMapList[i]->m_stTempSectorInfo[ix][iy].iMonsterActivity > iMaxMonsterActivity)
                    {
                        iMaxMonsterActivity = m_pMapList[i]->m_stTempSectorInfo[ix][iy].iMonsterActivity;
                        m_pMapList[i]->m_iMaxMx = ix;
                        m_pMapList[i]->m_iMaxMy = iy;
                    }

                    if (m_pMapList[i]->m_stTempSectorInfo[ix][iy].iPlayerActivity > iMaxPlayerActivity)
                    {
                        iMaxPlayerActivity = m_pMapList[i]->m_stTempSectorInfo[ix][iy].iPlayerActivity;
                        m_pMapList[i]->m_iMaxPx = ix;
                        m_pMapList[i]->m_iMaxPy = iy;
                    }
                }

            m_pMapList[i]->ClearTempSectorInfo();

            if (m_pMapList[i]->m_iMaxNx > 0) m_pMapList[i]->m_stSectorInfo[m_pMapList[i]->m_iMaxNx][m_pMapList[i]->m_iMaxNy].iNeutralActivity++;
            if (m_pMapList[i]->m_iMaxAx > 0) m_pMapList[i]->m_stSectorInfo[m_pMapList[i]->m_iMaxAx][m_pMapList[i]->m_iMaxAy].iAresdenActivity++;
            if (m_pMapList[i]->m_iMaxEx > 0) m_pMapList[i]->m_stSectorInfo[m_pMapList[i]->m_iMaxEx][m_pMapList[i]->m_iMaxEy].iElvineActivity++;
            if (m_pMapList[i]->m_iMaxMx > 0) m_pMapList[i]->m_stSectorInfo[m_pMapList[i]->m_iMaxMx][m_pMapList[i]->m_iMaxMy].iMonsterActivity++;
            if (m_pMapList[i]->m_iMaxPx > 0) m_pMapList[i]->m_stSectorInfo[m_pMapList[i]->m_iMaxPx][m_pMapList[i]->m_iMaxPy].iPlayerActivity++;
        }
}

void CGame::AgingMapSectorInfo()
{
    int i, ix, iy;

    for (i = 0; i < DEF_MAXMAPS; i++)
        if (m_pMapList[i] != 0)
        {
            for (ix = 0; ix < DEF_MAXSECTORS; ix++)
                for (iy = 0; iy < DEF_MAXSECTORS; iy++)
                {
                    m_pMapList[i]->m_stSectorInfo[ix][iy].iNeutralActivity--;
                    m_pMapList[i]->m_stSectorInfo[ix][iy].iAresdenActivity--;
                    m_pMapList[i]->m_stSectorInfo[ix][iy].iElvineActivity--;
                    m_pMapList[i]->m_stSectorInfo[ix][iy].iMonsterActivity--;
                    m_pMapList[i]->m_stSectorInfo[ix][iy].iPlayerActivity--;

                    if (m_pMapList[i]->m_stSectorInfo[ix][iy].iNeutralActivity < 0) m_pMapList[i]->m_stSectorInfo[ix][iy].iNeutralActivity = 0;
                    if (m_pMapList[i]->m_stSectorInfo[ix][iy].iAresdenActivity < 0) m_pMapList[i]->m_stSectorInfo[ix][iy].iAresdenActivity = 0;
                    if (m_pMapList[i]->m_stSectorInfo[ix][iy].iElvineActivity < 0) m_pMapList[i]->m_stSectorInfo[ix][iy].iElvineActivity = 0;
                    if (m_pMapList[i]->m_stSectorInfo[ix][iy].iMonsterActivity < 0) m_pMapList[i]->m_stSectorInfo[ix][iy].iMonsterActivity = 0;
                    if (m_pMapList[i]->m_stSectorInfo[ix][iy].iPlayerActivity < 0) m_pMapList[i]->m_stSectorInfo[ix][iy].iPlayerActivity = 0;
                }
        }
}

bool CGame::__bSetConstructionKit(int iMapIndex, int dX, int dY, int iType, int iTimeCost, int iClientH)
{
    int iNamingValue, ix, iy, tX, tY;
    char cNpcName[21], cName[21], cNpcWaypoint[11], cOwnerType;
    short sOwnerH;

    // Ã…Â©Â·Ã§Â¼Â¼Ã€ÃŒÂµÃ¥ Â¸Ã°ÂµÃ¥Â°Â¡ Â¾Ã†Â´ÃÂ°Ã…Â³Âª Â°Ã‡Â¼Â³Ã‡Ã‘ Â»Ã§Â¶Ã·Ã€ÃŒ Â°Ã¸ÂºÂ´Ã€ÃŒ Â¾Ã†Â´ÃÂ¸Ã© Â¹Â«Â½Ãƒ.
    if ((m_bIsCrusadeMode == false) || (m_pClientList[iClientH]->m_iCrusadeDuty != 2)) return false;
    if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_iTotalCrusadeStructures >= DEF_MAXCRUSADESTRUCTURES)
    {
        // Ã‡Ã˜Â´Ã§ Â¸ÃŠÂ¿Â¡ Ã…Â©Â·Ã§Â¼Â¼Ã€ÃŒÂµÃ¥ Â°Ã‡Â¹Â° Â°Â³Â¼Ã¶ ÃÂ¦Ã‡Ã‘Â¿Â¡ Â°Ã‰Â¸Â®ÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™Â¸Ã©
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_NOMORECRUSADESTRUCTURE, 0, 0, 0, 0);
        return false;
    }

    // Ã‡Ã˜Â´Ã§ Ã€Â§Ã„Â¡Â¿Â¡ Â°Ã‡ÃƒÃ Â¹Â° NPCÂ¸Â¦ Â»Ã½Â¼Âº.
    iNamingValue = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->iGetEmptyNamingValue();
    if (iNamingValue == -1)
    {
        // Â´ÃµÃ€ÃŒÂ»Ã³ Ã€ÃŒ Â¸ÃŠÂ¿Â¡ NPCÂ¸Â¦ Â¸Â¸ÂµÃ©Â¼Ã¶ Â¾Ã¸Â´Ã™. Ã€ÃŒÂ¸Â§Ã€Â» Ã‡Ã’Â´Ã§Ã‡Ã’ Â¼Ã¶ Â¾Ã¸Â±Ã¢ Â¶Â§Â¹Â®.
    }
    else
    {

        // Â¸Ã•Ã€Ãº Â¼Â³Ã„Â¡Ã‡ÃÂ°Ã­Ã€Ãš Ã‡ÃÂ´Ã‚ Â±Ã™ÃƒÂ³Â¿Â¡ Â±Â¸ÃÂ¶Â¹Â°Ã€ÃŒ Â¾Ã¸Â³Âª ÃˆÂ®Ã€ÃŽÃ‡Ã‘Â´Ã™.
        for (ix = dX - 3; ix <= dX + 5; ix++)
            for (iy = dY - 3; iy <= dX + 5; iy++)
            {
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
                if ((sOwnerH != 0) && (cOwnerType == DEF_OWNERTYPE_NPC) && (m_pNpcList[sOwnerH]->m_cActionLimit == 5)) return false;
            }

        // NPCÂ¸Â¦ Â»Ã½Â¼ÂºÃ‡Ã‘Â´Ã™.
        memset(cNpcName, 0, sizeof(cNpcName));
        if (m_pClientList[iClientH]->m_cSide == 1)
        {
            switch (iType)
            {
                case 1: strcpy(cNpcName, "AGT-Aresden"); break;
                case 2: strcpy(cNpcName, "CGT-Aresen"); break;
                case 3: strcpy(cNpcName, "MS-Aresden"); break;
                case 4: strcpy(cNpcName, "DT-Aresden"); break;
            }
        }
        else if (m_pClientList[iClientH]->m_cSide == 2)
        {
            switch (iType)
            {
                case 1: strcpy(cNpcName, "AGT-Elvine"); break;
                case 2: strcpy(cNpcName, "CGT-Elvine"); break;
                case 3: strcpy(cNpcName, "MS-Elvine"); break;
                case 4: strcpy(cNpcName, "DT-Elvine"); break;
            }
        }
        else return false;

        memset(cName, 0, sizeof(cName));
        wsprintf(cName, "XX%d", iNamingValue);
        cName[0] = '_';
        cName[1] = m_pClientList[iClientH]->m_cMapIndex + 65;

        memset(cNpcWaypoint, 0, sizeof(cNpcWaypoint));

        tX = (int)dX;
        tY = (int)dY;
        if (bCreateNewNpc(cNpcName, cName, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, 0, (rand() % 9),
            DEF_MOVETYPE_RANDOM, &tX, &tY, cNpcWaypoint, 0, 0, -1, false, false) == false)
        {
            // Â½Ã‡Ã†ÃÃ‡ÃŸÃ€Â¸Â¹Ã‡Â·ÃŽ Â¿Â¹Â¾Ã ÂµÃˆ NameValueÂ¸Â¦ Ã‡Ã˜ÃÂ¦Â½ÃƒÃ…Â²Â´Ã™.
            m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetNamingValueEmpty(iNamingValue);
        }
        else
        {
            // Â¼ÂºÂ°Ã¸
            wsprintf(G_cTxt, "Structure(%s) construction begin(%d,%d)!", cNpcName, tX, tY);
            log->info(G_cTxt);
            return true;
        }
    }

    return false;
}

void CGame::SaveOccupyFlagData()
{
    char * pData;
    int iSize;
    FILE * pFile;

    log->info("(!) Middleland OccupyFlag data saved.");

    pData = new char[1000000 + 1];
    if (pData == 0) return;
    memset(pData, 0, 1000000);

    iSize = _iComposeFlagStatusContents(pData);

    _mkdir("GameData");

    pFile = fopen("GameData\\OccupyFlag.txt", "wt");
    if (pFile == 0) return;

    fwrite(pData, 1, iSize, pFile);

    delete pData;
    fclose(pFile);
}

void CGame::_SendMapStatus(int iClientH)
{
    int i, iDataSize;
    char * cp, cData[DEF_MAXCRUSADESTRUCTURES * 6];
    short * sp;

    memset(cData, 0, sizeof(cData));
    cp = (char *)(cData);

    memcpy(cp, m_pClientList[iClientH]->m_cSendingMapName, 10);
    cp += 10;

    sp = (short *)cp;
    *sp = (short)m_pClientList[iClientH]->m_iCSIsendPoint;
    cp += 2;

    // ÃƒÃ‘ Â¸Ã®Â°Â³Ã€Ã‡ ÂµÂ¥Ã€ÃŒÃ…ÃÂ°Â¡ Ã€Ã–Â´Ã‚ÃÃ¶ Â¾Ã‹Â¸Â®Â´Ã‚ Â°Ã· Â¶Ã§Â¿Ã².
    cp++;

    if (m_pClientList[iClientH]->m_iCSIsendPoint == 0)
        m_pClientList[iClientH]->m_bIsSendingMapStatus = true;

    // 100Â°Â³Â¾Â¿ ÃÂ¤ÂºÂ¸Â¸Â¦ ÂºÂ¸Â³Â½Â´Ã™.
    iDataSize = 0;
    for (i = 0; i < 100; i++)
    {
        if (m_pClientList[iClientH]->m_iCSIsendPoint >= DEF_MAXCRUSADESTRUCTURES) goto SMS_ENDOFDATA;
        if (m_pClientList[iClientH]->m_stCrusadeStructureInfo[m_pClientList[iClientH]->m_iCSIsendPoint].cType == 0) goto SMS_ENDOFDATA;

        *cp = m_pClientList[iClientH]->m_stCrusadeStructureInfo[m_pClientList[iClientH]->m_iCSIsendPoint].cType;
        cp++;
        sp = (short *)cp;
        *sp = m_pClientList[iClientH]->m_stCrusadeStructureInfo[m_pClientList[iClientH]->m_iCSIsendPoint].sX;
        cp += 2;
        sp = (short *)cp;
        *sp = m_pClientList[iClientH]->m_stCrusadeStructureInfo[m_pClientList[iClientH]->m_iCSIsendPoint].sY;
        cp += 2;
        *cp = m_pClientList[iClientH]->m_stCrusadeStructureInfo[m_pClientList[iClientH]->m_iCSIsendPoint].cSide;
        cp++;

        iDataSize += 6;
        m_pClientList[iClientH]->m_iCSIsendPoint++;
    }

    // Â¿Â©Â±Ã¢Â±Ã®ÃÃ¶ Â¿Ã”Â´Ã™Â¸Ã© Â´Ãµ ÂºÂ¸Â³Â¾ Â¸ÃžÂ½ÃƒÃÃ¶Â°Â¡ Ã€Ã–Â´Ã™Â´Ã‚ Ã€Ã‡Â¹ÃŒ.
    cp = (char *)(cData + 12);
    *cp = (iDataSize / 6);
    SendNotifyMsg(0, iClientH, DEF_NOTIFY_MAPSTATUSNEXT, iDataSize + 13, 0, 0, cData);
    return;

    SMS_ENDOFDATA:;

    // ÂµÂ¥Ã€ÃŒÃ…ÃÂ°Â¡ Â´Ã™ Ã€Ã¼Â¼Ã› ÂµÃ‡Â¾ÃºÃ€Â½Ã€Â» Ã€Ã‡Â¹ÃŒ.
    cp = (char *)(cData + 12);
    *cp = (iDataSize / 6);
    SendNotifyMsg(0, iClientH, DEF_NOTIFY_MAPSTATUSLAST, iDataSize + 13, 0, 0, cData);
    m_pClientList[iClientH]->m_bIsSendingMapStatus = false;

    return;
}

void CGame::SyncMiddlelandMapInfo()
{
    int i;
    char * cp;
    short * sp;

    // ¹Ìµé·£µåÀÇ Á¤º¸¸¦ ¸ðµç ¼­¹ö¿¡°Ô ½ÌÅ©½ÃÅ²´Ù. ¹Ìµé·£µå¸¦ ´ã´çÇÑ ¼­¹ö¶ó¸é ¹Ù·Î ±¸Á¶Ã¼¸¦ ¾÷µ¥ÀÌÆ® ÇÏ°í ³ª¸ÓÁö ¼­¹öµé¿¡°Ô Á¤º¸ Àü¼Û.
    if (m_iMiddlelandMapIndex != -1)
    {
        // ±¸Á¶Ã¼ Å¬¸®¾î
        for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++)
        {
            m_stMiddleCrusadeStructureInfo[i].cType = 0;
            m_stMiddleCrusadeStructureInfo[i].cSide = 0;
            m_stMiddleCrusadeStructureInfo[i].sX = 0;
            m_stMiddleCrusadeStructureInfo[i].sY = 0;
        }
        // ±¸Á¶Ã¼ º¹»ç ¹× Àü¼Û ¸Þ½ÃÁö ÀÛ¼º 
        m_iTotalMiddleCrusadeStructures = m_pMapList[m_iMiddlelandMapIndex]->m_iTotalCrusadeStructures;
        memset(G_cData50000, 0, sizeof(G_cData50000));
        cp = (char *)G_cData50000;
        *cp = GSM_MIDDLEMAPSTATUS;
        cp++;

        sp = (short *)cp;
        *sp = (short)m_iTotalMiddleCrusadeStructures;
        cp += 2;

        for (i = 0; i < m_iTotalMiddleCrusadeStructures; i++)
        {
            m_stMiddleCrusadeStructureInfo[i].cType = m_pMapList[m_iMiddlelandMapIndex]->m_stCrusadeStructureInfo[i].cType;
            m_stMiddleCrusadeStructureInfo[i].cSide = m_pMapList[m_iMiddlelandMapIndex]->m_stCrusadeStructureInfo[i].cSide;
            m_stMiddleCrusadeStructureInfo[i].sX = m_pMapList[m_iMiddlelandMapIndex]->m_stCrusadeStructureInfo[i].sX;
            m_stMiddleCrusadeStructureInfo[i].sY = m_pMapList[m_iMiddlelandMapIndex]->m_stCrusadeStructureInfo[i].sY;

            *cp = m_stMiddleCrusadeStructureInfo[i].cType;
            cp++;
            *cp = m_stMiddleCrusadeStructureInfo[i].cSide;
            cp++;
            sp = (short *)cp;
            *sp = (short)m_stMiddleCrusadeStructureInfo[i].sX;
            cp += 2;
            sp = (short *)cp;
            *sp = (short)m_stMiddleCrusadeStructureInfo[i].sY;
            cp += 2;
        }

        // ¸Þ½ÃÁö ½ºÅå.
        if (m_iTotalMiddleCrusadeStructures != 0)
        {
            //testcode
            //wsprintf(G_cTxt, "m_iTotalMiddleCrusadeStructures: %d", m_iTotalMiddleCrusadeStructures);
            //log->info(G_cTxt);
            bStockMsgToGateServer(G_cData50000, 3 + m_iTotalMiddleCrusadeStructures * 6);
        }
    }
}

void CGame::GSM_SetGuildConstructLoc(int iGuildGUID, int dX, int dY, char * pMapName)
{
    int i, iIndex;
    uint32_t dwTemp, dwTime;

    // Ã…Â¬Â¶Ã³Ã€ÃŒÂ¾Ã°Ã†Â®Â·ÃŽÂºÃŽÃ…ÃÃ€Ã‡ Â¿Ã¤ÃƒÂ»Ã€ÃŒ Â¾Ã†Â´ÃÂ¶Ã³ Â´Ã™Â¸Â¥ Â¼Â­Â¹Ã¶Â·ÃŽÂºÃŽÃ…ÃÃ€Ã‡ Ã…ÃšÂ·Â¹Ã†Ã·Ã†Â® ÃÃ‚Ã‡Â¥ Â¼Â³ÃÂ¤ Â¿Ã¤ÃƒÂ»Ã€Ã“. Ã€Ã€Â´Ã¤Ã‡Ã’ Ã‡ÃŠÂ¿Ã¤ Â¾Ã¸Ã€Â½.

    //testcode
    wsprintf(G_cTxt, "SetGuildConstructLoc: %d %s %d %d", iGuildGUID, pMapName, dX, dY);
    log->info(G_cTxt);

    dwTime = timeGetTime();

    // Â¸Ã•Ã€Ãº Â°Â°Ã€Âº GUIDÂ¸Â¦ Â°Â¡ÃÃ¸ Â±Ã¦ÂµÃ¥ Ã…ÃšÂ·Â¹Ã†Ã·Ã†Â® ÃÃ‚Ã‡Â¥Â°Â¡ Â¼Â³ÃÂ¤ÂµÃ‡Â¾Ã® Ã€Ã–Â´Ã‚ÃÃ¶ ÃƒÂ£Â´Ã‚Â´Ã™.
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
                // Â³Â»Â¿Ã« Â°Â»Â½Ã…
                m_pGuildTeleportLoc[i].m_sDestX2 = dX;
                m_pGuildTeleportLoc[i].m_sDestY2 = dY;
                memset(m_pGuildTeleportLoc[i].m_cDestMapName2, 0, sizeof(m_pGuildTeleportLoc[i].m_cDestMapName2));
                strcpy(m_pGuildTeleportLoc[i].m_cDestMapName2, pMapName);
                m_pGuildTeleportLoc[i].m_dwTime2 = dwTime;
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
            memset(m_pGuildTeleportLoc[i].m_cDestMapName2, 0, sizeof(m_pGuildTeleportLoc[i].m_cDestMapName));
            strcpy(m_pGuildTeleportLoc[i].m_cDestMapName2, pMapName);
            m_pGuildTeleportLoc[i].m_dwTime2 = dwTime;
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
    log->info("(X) No more GuildConstLocuct Space! Replaced.");

    m_pGuildTeleportLoc[i].m_iV1 = iGuildGUID;
    m_pGuildTeleportLoc[i].m_sDestX2 = dX;
    m_pGuildTeleportLoc[i].m_sDestY2 = dY;
    memset(m_pGuildTeleportLoc[i].m_cDestMapName2, 0, sizeof(m_pGuildTeleportLoc[i].m_cDestMapName2));
    strcpy(m_pGuildTeleportLoc[i].m_cDestMapName2, pMapName);
    m_pGuildTeleportLoc[i].m_dwTime2 = dwTime;
}

void CGame::CheckCommanderConstructionPoint(int iClientH)
{
    char * cp, cData[120];
    int * ip, i;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_bIsCrusadeMode == false) return;
    if (m_pClientList[iClientH]->m_iConstructionPoint <= 0) return;

    switch (m_pClientList[iClientH]->m_iCrusadeDuty)
    {
        case 1: // Ã†Ã„Ã€ÃŒÃ…Ã
        case 2: // Â°Ã‡Â¼Â³Ã€Ãš: Â¸Ã°Â¾Ã† Â³ÃµÃ€Âº Ã†Ã·Ã€ÃŽÃ†Â®Â¸Â¦ Â±Ã¦ÂµÃ¥Â¸Â¶Â½ÂºÃ…Ã ÃÃ¶ÃˆÃ–Â°Ã¼Â¿Â¡Â°Ã” Ã€Ã¼Â´ÃžÃ‡Ã‘Â´Ã™.
            for (i = 0; i < DEF_MAXCLIENTS; i++)
                if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_iCrusadeDuty == 3) &&
                    (m_pClientList[i]->m_iGuildGUID == m_pClientList[iClientH]->m_iGuildGUID))
                {
                    // Ã‡Ã¶Ã€Ã§ Â¼Â­Â¹Ã¶ Â³Â»Â¿Â¡ Â±Ã¦ÂµÃ¥Â¸Â¶Â½ÂºÃ…Ã ÃÃ¶ÃˆÃ–Â°Ã¼Ã€ÃŒ Ã€Ã–Â´Ã™. ÃÃ¶ÃˆÃ–Â°Ã¼Ã€Ã‡ Ã†Ã·Ã€ÃŽÃ†Â®Â¸Â¦ Â³Ã´Ã€ÃŽ ÃˆÃ„ 
                    m_pClientList[i]->m_iConstructionPoint += m_pClientList[iClientH]->m_iConstructionPoint;
                    m_pClientList[i]->m_iWarContribution += (m_pClientList[iClientH]->m_iConstructionPoint / 10);

                    if (m_pClientList[i]->m_iConstructionPoint > DEF_MAXCONSTRUCTIONPOINT)
                        m_pClientList[i]->m_iConstructionPoint = DEF_MAXCONSTRUCTIONPOINT;

                    if (m_pClientList[i]->m_iWarContribution > DEF_MAXWARCONTRIBUTION)
                        m_pClientList[i]->m_iWarContribution = DEF_MAXWARCONTRIBUTION;

                    SendNotifyMsg(0, i, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[i]->m_iConstructionPoint, m_pClientList[i]->m_iWarContribution, 0, 0);
                    m_pClientList[iClientH]->m_iConstructionPoint = 0; // Â°Âª ÃƒÃŠÂ±Ã¢ÃˆÂ­ 
                    return;
                }

            // Â´Ã™Â¸Â¥ Â¼Â­Â¹Ã¶Ã€Ã‡ ÃÃ¶ÃˆÃ–Â°Ã¼Â¿Â¡Â°Ã” Â¾Ã‹Â·ÃÂ¾ÃŸ Ã‡Ã‘Â´Ã™.
            memset(cData, 0, sizeof(cData));
            cp = (char *)cData;
            *cp = GSM_CONSTRUCTIONPOINT;
            cp++;
            ip = (int *)cp;
            *ip = m_pClientList[iClientH]->m_iGuildGUID;
            cp += 4;
            ip = (int *)cp;
            *ip = m_pClientList[iClientH]->m_iConstructionPoint;
            cp += 4;
            bStockMsgToGateServer(cData, 9);

            m_pClientList[iClientH]->m_iConstructionPoint = 0; // Â°Âª ÃƒÃŠÂ±Ã¢ÃˆÂ­ 
            break;

        case 3: // ÃÃ¶ÃˆÃ–Â°Ã¼: ÂºÂ¸Â³Â¾ Ã‡ÃŠÂ¿Ã¤ Â¾Ã¸Ã€Â½ 

            break;
    }
}

void CGame::GSM_ConstructionPoint(int iGuildGUID, int iPoint)
{
    int i;

    for (i = 1; i < DEF_MAXCLIENTS; i++)
        if ((m_pClientList[i] != 0) && (m_pClientList[i]->m_iCrusadeDuty == 3) &&
            (m_pClientList[i]->m_iGuildGUID == iGuildGUID))
        {
            // Ã‡Ã˜Â´Ã§ Â±Ã¦ÂµÃ¥Â¸Â¶Â½ÂºÃ…Ã ÃÃ¶ÃˆÃ–Â°Ã¼Ã€Â» ÃƒÂ£Â¾Ã’Â´Ã™.
            m_pClientList[i]->m_iConstructionPoint += iPoint;
            m_pClientList[i]->m_iWarContribution += iPoint / 10;

            if (m_pClientList[i]->m_iConstructionPoint > DEF_MAXCONSTRUCTIONPOINT)
                m_pClientList[i]->m_iConstructionPoint = DEF_MAXCONSTRUCTIONPOINT;

            if (m_pClientList[i]->m_iWarContribution > DEF_MAXWARCONTRIBUTION)
                m_pClientList[i]->m_iWarContribution = DEF_MAXWARCONTRIBUTION;

            SendNotifyMsg(0, i, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[i]->m_iConstructionPoint, m_pClientList[i]->m_iWarContribution, 0, 0);
            //testcode
            wsprintf(G_cTxt, "GSM_ConstructionPoint: %d %d", iGuildGUID, iPoint);
            log->info(G_cTxt);
            return;
        }
}

bool CGame::bAddClientShortCut(int iClientH)
{
    int i;

    // Â¸Ã•Ã€Ãº ÂµÂ¿Ã€ÃÃ‡Ã‘ Â¹Ã¸ÃˆÂ£Â°Â¡ ÃÂ¸Ã€Ã§Ã‡ÃÂ´Ã‚ÃÃ¶ Â°Ã‹Â»Ã¶

    for (i = 0; i < DEF_MAXCLIENTS; i++)
        if (m_iClientShortCut[i] == iClientH) return false;

    for (i = 0; i < DEF_MAXCLIENTS; i++)
        if (m_iClientShortCut[i] == 0)
        {
            m_iClientShortCut[i] = iClientH;
            return true;
        }

    return false;
}

void CGame::RemoveClientShortCut(int iClientH)
{
    int i;

    for (i = 0; i < DEF_MAXCLIENTS + 1; i++)
        if (m_iClientShortCut[i] == iClientH)
        {
            m_iClientShortCut[i] = 0;
            goto RCSC_LOOPBREAK;
        }

    RCSC_LOOPBREAK:;

    // ÂºÃ³ Â¿Â©Â¹Ã©Ã€Â» Â¸ÃžÂ²Ã›Â´Ã™.
    //m_iClientShortCut[i] = m_iClientShortCut[m_iTotalClients+1];
    //m_iClientShortCut[m_iTotalClients+1] = 0;
    for (i = 0; i < DEF_MAXCLIENTS; i++)
        if ((m_iClientShortCut[i] == 0) && (m_iClientShortCut[i + 1] != 0))
        {
            m_iClientShortCut[i] = m_iClientShortCut[i + 1];
            m_iClientShortCut[i + 1] = 0;
        }
}

int CGame::iGetMapLocationSide(char * pMapName)
{

    if (strcmp(pMapName, "aresden") == 0) return 3;
    if (strcmp(pMapName, "elvine") == 0) return 4;
    if (strcmp(pMapName, "arebrk11") == 0) return 3;
    if (strcmp(pMapName, "elvbrk11") == 0) return 4;

    if (strcmp(pMapName, "cityhall_1") == 0) return 1;
    if (strcmp(pMapName, "cityhall_2") == 0) return 2;
    if (strcmp(pMapName, "cath_1") == 0) return 1;
    if (strcmp(pMapName, "cath_2") == 0) return 2;
    if (strcmp(pMapName, "gshop_1") == 0) return 1;
    if (strcmp(pMapName, "gshop_2") == 0) return 2;
    if (strcmp(pMapName, "bsmith_1") == 0) return 1;
    if (strcmp(pMapName, "bsmith_2") == 0) return 2;
    if (strcmp(pMapName, "wrhus_1") == 0) return 1;
    if (strcmp(pMapName, "wrhus_2") == 0) return 2;
    if (strcmp(pMapName, "gldhall_1") == 0) return 1;
    if (strcmp(pMapName, "gldhall_2") == 0) return 2;
    if (strcmp(pMapName, "wzdtwr_1") == 0) return 1;
    if (strcmp(pMapName, "wzdtwr_2") == 0) return 2;
    if (strcmp(pMapName, "arefarm") == 0) return 1;
    if (strcmp(pMapName, "elvfarm") == 0) return 2;
    if (strcmp(pMapName, "arewrhus") == 0) return 1;
    if (strcmp(pMapName, "elvwrhus") == 0) return 2;
    if (strcmp(pMapName, "cmdhall_1") == 0) return 1;
    if (strcmp(pMapName, "Cmdhall_2") == 0) return 2;

    return 0;
}

bool CGame::bCopyItemContents(CItem * pCopy, CItem * pOriginal)
{
    if (pOriginal == 0) return false;
    if (pCopy == 0) return false;

    pCopy->m_sIDnum = pOriginal->m_sIDnum;					// Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ Â°Ã­Ã€Â¯ Â¹Ã¸ÃˆÂ£ 
    pCopy->m_cItemType = pOriginal->m_cItemType;
    pCopy->m_cEquipPos = pOriginal->m_cEquipPos;
    pCopy->m_sItemEffectType = pOriginal->m_sItemEffectType;
    pCopy->m_sItemEffectValue1 = pOriginal->m_sItemEffectValue1;
    pCopy->m_sItemEffectValue2 = pOriginal->m_sItemEffectValue2;
    pCopy->m_sItemEffectValue3 = pOriginal->m_sItemEffectValue3;
    pCopy->m_sItemEffectValue4 = pOriginal->m_sItemEffectValue4;
    pCopy->m_sItemEffectValue5 = pOriginal->m_sItemEffectValue5;
    pCopy->m_sItemEffectValue6 = pOriginal->m_sItemEffectValue6;
    pCopy->m_wMaxLifeSpan = pOriginal->m_wMaxLifeSpan;
    pCopy->m_sSpecialEffect = pOriginal->m_sSpecialEffect;

    //short m_sSM_HitRatio, m_sL_HitRatio;
    //v1.432 Â¸Ã­ÃÃŸÂ·Ã¼ Â°Â¡Â°Â¨ Â»Ã§Â¿Ã« Â¾ÃˆÃ‡Ã‘Â´Ã™. Â´Ã«Â½Ã… Ã†Â¯Â¼Ã¶ Â´Ã‰Â·Ã‚ Â¼Ã¶Ã„Â¡Â°Â¡ ÂµÃ©Â¾Ã®Â°Â£Â´Ã™.
    pCopy->m_sSpecialEffectValue1 = pOriginal->m_sSpecialEffectValue1;
    pCopy->m_sSpecialEffectValue2 = pOriginal->m_sSpecialEffectValue2;

    pCopy->m_sSprite = pOriginal->m_sSprite;
    pCopy->m_sSpriteFrame = pOriginal->m_sSpriteFrame;

    pCopy->m_cApprValue = pOriginal->m_cApprValue;
    pCopy->m_cSpeed = pOriginal->m_cSpeed;

    pCopy->m_wPrice = pOriginal->m_wPrice;
    pCopy->m_wWeight = pOriginal->m_wWeight;
    pCopy->m_sLevelLimit = pOriginal->m_sLevelLimit;
    pCopy->m_cGenderLimit = pOriginal->m_cGenderLimit;

    pCopy->m_sRelatedSkill = pOriginal->m_sRelatedSkill;

    pCopy->m_cCategory = pOriginal->m_cCategory;
    pCopy->m_bIsForSale = pOriginal->m_bIsForSale;
    // 

    pCopy->m_dwCount = pOriginal->m_dwCount;
    pCopy->m_sTouchEffectType = pOriginal->m_sTouchEffectType;
    pCopy->m_sTouchEffectValue1 = pOriginal->m_sTouchEffectValue1;
    pCopy->m_sTouchEffectValue2 = pOriginal->m_sTouchEffectValue2;
    pCopy->m_sTouchEffectValue3 = pOriginal->m_sTouchEffectValue3;
    pCopy->m_cItemColor = pOriginal->m_cItemColor;
    pCopy->m_sItemSpecEffectValue1 = pOriginal->m_sItemSpecEffectValue1;
    pCopy->m_sItemSpecEffectValue2 = pOriginal->m_sItemSpecEffectValue2;
    pCopy->m_sItemSpecEffectValue3 = pOriginal->m_sItemSpecEffectValue3;
    pCopy->m_wCurLifeSpan = pOriginal->m_wCurLifeSpan;
    pCopy->m_dwAttribute = pOriginal->m_dwAttribute;

    return true;
}

void CGame::SendMsg(short sOwnerH, char cOwnerType, bool bStatus, long lPass)
{
    if (m_pClientList[sOwnerH] == 0) return;
    //SendNotifyMsg(0, sOwnerH, DEF_NOTIFY_LEVELUP, 0, 0, 0, 0);

    SendNotifyMsg(0, sOwnerH, lPass, 0, 0, 0, 0);

}

void CGame::RequestChangePlayMode(int iClientH)
{

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_iPKCount > 0) return;
    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

    if (memcmp(m_pClientList[iClientH]->m_cMapName, "cityhall", 8) != 0) return;

    if (m_pClientList[iClientH]->m_iLevel < 100 ||
        m_pClientList[iClientH]->m_bIsPlayerCivil == true)
    {
        if (memcmp(m_pClientList[iClientH]->m_cLocation, "aresden", 7) == 0) strcpy(m_pClientList[iClientH]->m_cLocation, "arehunter");
        else if (memcmp(m_pClientList[iClientH]->m_cLocation, "elvine", 6) == 0) strcpy(m_pClientList[iClientH]->m_cLocation, "elvhunter");
        else if (memcmp(m_pClientList[iClientH]->m_cLocation, "arehunter", 9) == 0) strcpy(m_pClientList[iClientH]->m_cLocation, "aresden");
        else if (memcmp(m_pClientList[iClientH]->m_cLocation, "elvhunter", 9) == 0) strcpy(m_pClientList[iClientH]->m_cLocation, "elvine");

        if (m_pClientList[iClientH]->m_bIsPlayerCivil == true)
            m_pClientList[iClientH]->m_bIsPlayerCivil = false;
        else m_pClientList[iClientH]->m_bIsPlayerCivil = true;

        SendNotifyMsg(0, iClientH, DEF_NOTIFY_CHANGEPLAYMODE, 0, 0, 0, m_pClientList[iClientH]->m_cLocation);
        SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, 100, 0, 0, 0);
    }
}

bool CGame::_bItemLog(int iAction, int iGiveH, int iRecvH, class CItem * pItem, bool bForceItemLog)
{
    char  cTxt[1024], cTemp1[120], cTemp2[120];
    int iItemCount;
    if (pItem == 0) return false;

    // !!ÃÃ–Ã€Ã‡ Ã‡Ã‘Â±Â¹Â¿Â¡ Ã€Ã»Â¿Ã«Ã‡Ã’Â¶Â§  New Item Ã€ÃŒ Â»Ã½Â±Ã¦Â¶Â§Â´Ã‚  iGiveÂ°Â¡ Â³ÃŽÃ€ÃÂ¼Ã¶ Ã€Ã–Â´Ã™.
    if (m_pClientList[iGiveH]->m_cCharName == 0) return false;

    if (iAction == DEF_ITEMLOG_DUPITEMID)
    {
        // ÂºÂ¹Â»Ã§ÂµÃˆ Â¾Ã†Ã€ÃŒÃ…Ã› Ã€ÃºÃ€Ã¥ Â¿Ã¤ÃƒÂ»Ã€ÃŒÂ´Ã™. 
        if (m_pClientList[iGiveH] == 0) return false;
        if (m_pClientList[iGiveH]->m_cCharName == 0) return false;
        wsprintf(G_cTxt, "(!) Delete-DupItem(%s %d %d %d %d) Owner(%s)", pItem->m_cName, pItem->m_dwCount, pItem->m_sTouchEffectValue1,
            pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3,
            m_pClientList[iGiveH]->m_cCharName);
        //	PutItemLogFileList(iGiveH,G_cTxt);
        bSendMsgToLS(MSGID_GAMEITEMLOG, iGiveH, 0, G_cTxt);
        return true;
    }

    memset(cTxt, 0, sizeof(cTxt));
    memset(cTemp1, 0, sizeof(cTemp1));
    memset(cTemp2, 0, sizeof(cTemp2));

    switch (iAction)
    {

        case DEF_ITEMLOG_EXCHANGE:
            if (m_pClientList[iRecvH]->m_cCharName == 0) return false;
            wsprintf(cTxt, "(%s) PC(%s)\tExchange\t%s(%d %d %d %d %x)\t%s(%d %d)\tPC(%s)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
                pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
                m_pClientList[iGiveH]->m_cMapName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY, m_pClientList[iRecvH]->m_cCharName);
            PutItemLogFileList(cTxt);
            break;

        case DEF_ITEMLOG_GIVE:
            if (m_pClientList[iRecvH]->m_cCharName == 0) return false;
            wsprintf(cTxt, "(%s) PC(%s)\tGive\t%s(%d %d %d %d %x)\t%s(%d %d)\tPC(%s)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
                pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
                m_pClientList[iGiveH]->m_cMapName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY, m_pClientList[iRecvH]->m_cCharName);
            PutItemLogFileList(cTxt);
            break;

        case DEF_ITEMLOG_DROP:
            wsprintf(cTxt, "(%s) PC(%s)\tDrop\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
                pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
                m_pClientList[iGiveH]->m_cMapName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
            PutItemLogFileList(cTxt);
            break;

        case DEF_ITEMLOG_GET:
            wsprintf(cTxt, "(%s) PC(%s)\tGet\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
                pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
                m_pClientList[iGiveH]->m_cMapName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
            PutItemLogFileList(cTxt);
            break;

        case DEF_ITEMLOG_MAKE:
            wsprintf(cTxt, "(%s) PC(%s)\tMake\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
                pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
                m_pClientList[iGiveH]->m_cMapName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
            PutItemLogFileList(cTxt);
            break;

        case DEF_ITEMLOG_DEPLETE:
            wsprintf(cTxt, "(%s) PC(%s)\tDeplete\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
                pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
                m_pClientList[iGiveH]->m_cMapName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
            PutItemLogFileList(cTxt);
            break;

        case DEF_ITEMLOG_BUY:
            iItemCount = iRecvH;
            wsprintf(cTxt, "(%s) PC(%s)\tBuy\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, iItemCount,
                pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
                m_pClientList[iGiveH]->m_cMapName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
            PutItemLogFileList(cTxt);
            break;

        case DEF_ITEMLOG_SELL:
            wsprintf(cTxt, "(%s) PC(%s)\tSell\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
                pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
                m_pClientList[iGiveH]->m_cMapName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
            PutItemLogFileList(cTxt);
            break;

        case DEF_ITEMLOG_RETRIEVE:
            wsprintf(cTxt, "(%s) PC(%s)\tRetrieve\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
                pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
                m_pClientList[iGiveH]->m_cMapName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
            PutItemLogFileList(cTxt);
            break;

        case DEF_ITEMLOG_DEPOSIT:
            wsprintf(cTxt, "(%s) PC(%s)\tDeposit\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
                pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
                m_pClientList[iGiveH]->m_cMapName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
            PutItemLogFileList(cTxt);
            break;

        case DEF_ITEMLOG_UPGRADEFAIL:
            wsprintf(cTxt, "(%s) PC(%s)\tUpgrade Fail\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName,
                pItem->m_dwCount, pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
                m_pClientList[iGiveH]->m_cMapName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
            PutItemLogFileList(cTxt);
            break;

        case DEF_ITEMLOG_UPGRADESUCCESS:
            wsprintf(cTxt, "(%s) PC(%s)\tUpgrade Success\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName,
                pItem->m_dwCount, pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
                m_pClientList[iGiveH]->m_cMapName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
            PutItemLogFileList(cTxt);
            break;
        default:
            return false;
    }
    bSendMsgToLS(MSGID_GAMEITEMLOG, iGiveH, 0, cTxt);
    return true;
}

bool CGame::_bItemLog(int iAction, int iClientH, char * cName, class CItem * pItem)
{
    if (pItem == 0) return false;
    if (_bCheckGoodItem(pItem) == false) return false;
    if (iAction != DEF_ITEMLOG_NEWGENDROP)
    {
        if (m_pClientList[iClientH] == nullptr) return false;
    }
    char  cTxt[200], cTemp1[120];
    //  Â·ÃŽÂ±Ã— Â³Â²Â±Ã¤Â´Ã™. 
    memset(cTxt, 0, sizeof(cTxt));
    memset(cTemp1, 0, sizeof(cTemp1));
    if (m_pClientList[iClientH] != 0) m_pClientList[iClientH]->m_pXSock->iGetPeerAddress(cTemp1);

    switch (iAction)
    {

        case DEF_ITEMLOG_NEWGENDROP:
            if (pItem == 0) return false;
            wsprintf(cTxt, "NPC(%s)\tDrop\t%s(%d %d %d %d)", cName, pItem->m_cName, pItem->m_dwCount,
                pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3);
            PutItemLogFileList(cTxt);
            break;
        case DEF_ITEMLOG_SKILLLEARN:
        case DEF_ITEMLOG_MAGICLEARN:
            if (cName == 0) return false;
            if (m_pClientList[iClientH] == nullptr) return false;
            wsprintf(cTxt, "PC(%s)\tLearn\t(%s)\t%s(%d %d)\t \tIP(%s)", m_pClientList[iClientH]->m_cCharName, cName,
                m_pClientList[iClientH]->m_cMapName, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, cTemp1);
            PutItemLogFileList(cTxt);
            break;
        case DEF_ITEMLOG_SUMMONMONSTER:
            if (cName == 0) return false;
            if (m_pClientList[iClientH] == nullptr) return false;
            wsprintf(cTxt, "PC(%s)\tSummon\t(%s)\t%s(%d %d)\t \tIP(%s)", m_pClientList[iClientH]->m_cCharName, cName,
                m_pClientList[iClientH]->m_cMapName, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, cTemp1);
            break;
        case DEF_ITEMLOG_POISONED:
            if (m_pClientList[iClientH] == nullptr) return false;
            wsprintf(cTxt, "PC(%s)\tBe Poisoned\t \t%s(%d %d)\t \tIP(%s)", m_pClientList[iClientH]->m_cCharName,
                m_pClientList[iClientH]->m_cMapName, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, cTemp1);
            break;

        case DEF_ITEMLOG_REPAIR:
            if (cName == 0) return false;
            if (m_pClientList[iClientH] == nullptr) return false;
            wsprintf(cTxt, "PC(%s)\tRepair\t(%s)\t%s(%d %d)\t \tIP(%s)", m_pClientList[iClientH]->m_cCharName, cName,
                m_pClientList[iClientH]->m_cMapName, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, cTemp1);
            break;

        default:
            return false;
    }
    bSendMsgToLS(MSGID_GAMEITEMLOG, iClientH, 0, cTxt);
    return true;
}

bool CGame::_bCheckGoodItem(class CItem * pItem)
{
    if (pItem == 0) return false;

    if (pItem->m_sIDnum == 90)
    {
        if (pItem->m_dwCount > 10000) return true;  //GoldÂ¿Â¡ Ã‡Ã‘Ã‡Ã˜ 10000Â¿Ã¸ Ã€ÃŒÂ»Ã³Â¸Â¸ Â·ÃŽÂ±Ã—Â¿Â¡ Â³Â²Â±Ã¤Â´Ã™.
        else return false;
    }
    switch (pItem->m_sIDnum)
    {
        //	case 90: // Gold ÃƒÃŸÂ°Â¡ 
        case 259:
        case 290:
        case 291:
        case 292:
        case 300:
        case 305:
        case 308:
        case 311:
        case 334:
        case 335:
        case 336:
        case 338:
        case 380:
        case 381:
        case 382:
        case 391:
        case 400:
        case 401:
        case 490:
        case 491:
        case 492:
        case 508:
        case 581:
        case 610:
        case 611:
        case 612:
        case 613:
        case 614:
        case 616:  // ÂµÂ¥Â¸Ã³-Â½Â½Â·Â¹Ã€ÃŒÂ¾Ã®
        case 618:  // Â´Ã™Ã…Â©Â¿Â¤Ã‡Ã-ÂºÂ¸Â¿Ã¬

        case 620:
        case 621:
        case 622:
        case 623:

        case 630:
        case 631:

        case 632:
        case 633:
        case 634:
        case 635:
        case 636:
        case 637:
        case 638:
        case 639:
        case 640:
        case 641:

        case 642:
        case 643:

        case 644:
        case 645:
        case 646:
        case 647:

        case 650:
        case 654:
        case 655:
        case 656:
        case 657:

        case 700: 	// v2.03 Â»Ã³Â¾Ã® Â¾Ã†Ã€ÃŒÃ…Ã› 
        case 701:
        case 702:
        case 703:
        case 704:
        case 705:
        case 706:
        case 707:
        case 708:
        case 709:
        case 710:
        case 711:
        case 712:
        case 713:
        case 714:
        case 715:

        case 720:
        case 721:
        case 722:
        case 723:

        case 724:
        case 725:
        case 726:
        case 727:
        case 728:
        case 729:
        case 730:
        case 731:
        case 732:
        case 733:

        case 734:
        case 735:

        case 736:  // Â»ÃµÂ·ÃŽÂ¿Ã® Â»Ã³Â¾Ã®Ã€Ã‡ Ã€ÃšÃ€ÃŒÂ¾Ã°Ã†Â® Â¼Ã’ÂµÃ¥
        case 737:  // Â»ÃµÂ·ÃŽÂ¿Ã® ÃˆÃ¦Â±Ã¢Â»Ã§Ã€Ã‡ Ã€ÃšÃ€ÃŒÂ¾Ã°Ã†Â® Â¼Ã’ÂµÃ¥
        case 738:  // Â»ÃµÂ·ÃŽÂ¿Ã® ÃˆÃ¦Â¸Â¶Â¹Ã½Â»Ã§Ã€Ã‡Â¸Ã…ÃÃ·Â¿Ã¸ÂµÃ¥
        case 924:

            return true;  //Ã†Â¯ÂºÂ°Ã‡Ã‘ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒÂ±Ã¢ Â¶Â«Â½Ãƒ Â±Ã¢Â·Ã...
            break;
        default:
            // v2.17 2002-7-31 ÃÂ¦Ã€Ã› Â¾Ã†Ã€ÃŒÃ…Ã›ÂµÂµ Â·ÃŽÂ±Ã—Â¿Â¡ Â³Â²Â°Ã” Ã‡Ã‘Â´Ã™.
            if ((pItem->m_dwAttribute & 0xF0F0F001) == 0) return false;  //Ã†Â¯ÂºÂ°Ã‡Ã‘ Â¾Ã†Ã€ÃŒÃ…Ã›ÂµÃŽ Â¾Ã†Â´ÃÂ±Â¸ Ã†Â¯Â¼ÂºÃ„Â¡ÂµÂµ Â¾Ã¸Â´Ã™Â¸Ã© ÂºÂ°Â·Ã§..
            else if (pItem->m_sIDnum > 30) return true;  //Ã†Â¯ÂºÂ°Ã‡Ã‘ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Âº Â¾Ã†Â´ÃÃÃ¶Â¸Â¸ Ã†Â¯Â¼ÂºÃ„Â¡Â°Â¡ Ã€Ã–Â°Ã­ Â´ÃœÂ°Ã‹Â·Ã¹Â°Â¡ Â¾Ã†Â´ÃÂ¶Ã³Â¸Ã© ÃÃÃ€Âº Â¾Ã†Ã…Ã›..
            else return false;  //Ã†Â¯ÂºÂ°Ã‡Ã‘ Â¾Ã†Ã€ÃŒÃ…Ã›ÂµÃŽ Â¾Ã†Â´ÃÂ±Â¸ Ã†Â¯Â¼ÂºÃ„Â¡Â´Ã‚ Ã€Ã–ÃÃ¶Â¸Â¸ Â´ÃœÂ°Ã‹Â·Ã¹Â¶Ã³Â¸Ã© ÂºÂ°Â·Ã§...
    }
}

void CGame::GetExp(int iClientH, int iExp, bool bIsAttackerOwn)
{
    double dV1, dV2, dV3;
    int i, iH, iUnitValue;
    uint32_t dwTime = timeGetTime();
    int iTotalPartyMembers;

    if (m_pClientList[iClientH] == nullptr) return;
    if (iExp <= 0) return;

    if (m_pClientList[iClientH]->m_iLevel <= 80)
    {
        dV1 = (double)(80 - m_pClientList[iClientH]->m_iLevel);
        dV2 = dV1 * 0.025f;
        dV3 = (double)iExp;
        dV1 = (dV2 + 1.025f) * dV3;
        iExp = (int)dV1;
    }
    else
    { //Lower exp
        if ((m_pClientList[iClientH]->m_iLevel >= 80) && ((strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "aresdend1") == 0)
            || (strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "elvined1") == 0)))
        {
            iExp = (iExp / 10);
        }
        else if ((strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "aresdend1") == 0)
            || (strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "elvined1") == 0))
        {
            iExp = (iExp * 1 / 4);
        }
    }

    //Check for party status, else give exp to player
    if ((m_pClientList[iClientH]->m_iPartyID != 0) && (m_pClientList[iClientH]->m_iPartyStatus == DEF_PARTYSTATUS_CONFIRM))
    {
        //Only divide exp if >= 1 person and exp > 10
        if (iExp >= 10 && m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers > 0)
        {

            //Calc total ppl in party
            iTotalPartyMembers = 0;
            for (i = 0; i < m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers; i++)
            {
                iH = m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i];
                if ((m_pClientList[iH] != 0) && (m_pClientList[iH]->m_iHP > 0))
                {
                    //Newly added, Only players on same map get exp :}
                    if ((strlen(m_pMapList[m_pClientList[iH]->m_cMapIndex]->m_cName)) == (strlen(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName)))
                    {
                        if (memcmp(m_pMapList[m_pClientList[iH]->m_cMapIndex]->m_cName,
                            m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName,
                            strlen(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName)) == 0)
                        {
                            iTotalPartyMembers++;
                        }
                    }
                }
            }

            //Check for party bug
            if (iTotalPartyMembers > 8)
            {
                log->info("(X) Party Bug !! partyMember {} XXXXXXXXXX", iTotalPartyMembers);
                iTotalPartyMembers = 8;
            }

            //Figure out how much exp a player gets
            dV1 = (double)iExp;

            switch (iTotalPartyMembers)
            {
                case 1:
                    dV2 = dV1;
                    break;
                case 2:
                    dV2 = (dV1 + (dV1 * 2.0e-2)) / 2.0;
                    break;
                case 3:
                    dV2 = (dV1 + (dV1 * 5.0e-2)) / 3.0;
                    break;
                case 4:
                    dV2 = (dV1 + (dV1 * 7.000000000000001e-2)) / 4.0;
                    break;
                case 5:
                    dV2 = (dV1 + (dV1 * 1.0e-1)) / 5.0;
                    break;
                case 6:
                    dV2 = (dV1 + (dV1 * 1.4e-1)) / 6.0;
                    break;
                case 7:
                    dV2 = (dV1 + (dV1 * 1.7e-1)) / 7.0;
                    break;
                case 8:
                    dV2 = (dV1 + (dV1 * 2.0e-1)) / 8.0;
                    break;
            }

            dV3 = dV2 + 5.0e-1;

            //Divide exp among party members
            for (i = 0; i < iTotalPartyMembers; i++)
            {
                iUnitValue = (int)dV3;
                iH = m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i];
                if ((m_pClientList[iH] != 0) && (m_pClientList[iH]->m_bSkillUsingStatus[19] != 1) && (m_pClientList[iH]->m_iHP > 0))
                { // Is player alive ??
                    if (m_pClientList[iH]->m_iLevel < 81)   iUnitValue *= 2;
                    else if (m_pClientList[iH]->m_iLevel < 101)   iUnitValue *= 2;
                    else if (m_pClientList[iH]->m_iLevel < 151)  iUnitValue *= 2;
                    else if (m_pClientList[iH]->m_iLevel < 500)  iUnitValue *= 2;
                    if ((m_pClientList[iH]->m_iStatus & 0x10000) != 0)  iUnitValue *= 3;
                    m_pClientList[iH]->m_iExpStock += iUnitValue;
                }
            }
        }
    }
    else
    {
        if ((m_pClientList[iClientH] != 0) && (m_pClientList[iClientH]->m_bSkillUsingStatus[19] != 1) && (m_pClientList[iClientH]->m_iHP > 0))
        { // Is player alive ??
            if (m_pClientList[iClientH]->m_iLevel < 81)   iExp *= 1.5;
            else if (m_pClientList[iClientH]->m_iLevel < 101)   iExp *= 1.5;
            else if (m_pClientList[iClientH]->m_iLevel < 151)  iExp *= 1.5;
            else if (m_pClientList[iClientH]->m_iLevel < 500)  iExp *= 1.5;
            if ((m_pClientList[iClientH]->m_iStatus & 0x10000) != 0)  iExp *= 3;
            m_pClientList[iClientH]->m_iExpStock += iExp;
        }
    }
}

bool CGame::bCheckAndConvertPlusWeaponItem(int iClientH, int iItemIndex)
{
    if (m_pClientList[iClientH] == nullptr) return false;
    if (m_pClientList[iClientH]->m_pItemList[iItemIndex] == 0) return false;

    switch (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum)
    {
        case 4:  // Dagger +1
        case 9:  // Short Sword +1
        case 13: // Main Gauge +1
        case 16: // Gradius +1
        case 18: // Long Sword +1
        case 19: // Long Sword +2
        case 21: // Excaliber +1
        case 24: // Sabre +1
        case 26: // Scimitar +1
        case 27: // Scimitar +2
        case 29: // Falchoin +1
        case 30: // Falchion +2
        case 32: // Esterk +1
        case 33: // Esterk +2
        case 35: // Rapier +1
        case 36: // Rapier +2
        case 39: // Broad Sword +1
        case 40: // Broad Sword +2
        case 43: // Bastad Sword +1
        case 44: // Bastad Sword +2
        case 47: // Claymore +1
        case 48: // Claymore +2
        case 51: // Great Sword +1
        case 52: // Great Sword +2
        case 55: // Flameberge +1
        case 56: // Flameberge +2
        case 60: // Light Axe +1
        case 61: // Light Axe +2
        case 63: // Tomahoc +1
        case 64: // Tomohoc +2
        case 66: // Sexon Axe +1
        case 67: // Sexon Axe +2
        case 69: // Double Axe +1
        case 70: // Double Axe +2
        case 72: // War Axe +1
        case 73: // War Axe +2

        case 580: // Battle Axe +1
        case 581: // Battle Axe +2
        case 582: // Sabre +2
            return true;
            break;
    }
    return false;
}

void CGame::ArmorLifeDecrement(int iAttackerH, int iTargetH, char cOwnerType, int iValue)
{
    int iTemp;

    if (m_pClientList[iAttackerH] == 0) return;
    // v1.3 Ã‡Ã‡Â°ÃÂ´Ã§Ã‡ÃŸÃ€Â¸Â¹Ã‡Â·ÃŽ Ã€Ã¥Ã‚Ã¸Ã‡ÃÂ°Ã­ Ã€Ã–Â´Ã‚ Â¸Ã¶Ã…Ã« Â¹Ã¦Â¾Ã®Â±Â¸Ã€Ã‡ Â¼Ã¶Â¸Ã­Ã€Â» ÃÃ™Ã€ÃŽÂ´Ã™. 
    switch (cOwnerType)
    {
        case DEF_OWNERTYPE_PLAYER:
            if (m_pClientList[iTargetH] == 0) return;
            break;

        case DEF_OWNERTYPE_NPC:	return;
        default: return;
    }

    // v2.16 Â°Â°Ã€Âº Ã†Ã­Ã€ÃŒÂ¸Ã© Â¼Ã¶Â¸Ã­ Â´ÃžÃÃ¶ Â¾ÃŠÂ°Ã” 
    if (m_pClientList[iAttackerH]->m_cSide == m_pClientList[iTargetH]->m_cSide) return;

    // Â¸Ã¶Ã…Ã« Â°Â©Â¿ÃŠ 
    iTemp = m_pClientList[iTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_BODY];
    if ((iTemp != -1) && (m_pClientList[iTargetH]->m_pItemList[iTemp] != 0))
    {
        // v1.432 ÃÃŸÂ¸Â³Ã€ÃŽ Â°Ã¦Â¿Ã¬ Â¼Ã¶Â¸Ã­ ÃÃ™ÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™.
        if ((m_pClientList[iTargetH]->m_cSide != 0) && (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0))
            m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan -= iValue;

        if ((m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) || (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 64000))
        {
            m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan = 0;
            // Â¼Ã¶Â¸Ã­Ã€ÃŒ Â´Ã™ ÂµÃ‡Â¾ÃºÃ€Â¸Â¹Ã‡Â·ÃŽ Ã€Ã¥Ã‚Ã¸Ã€Â» Ã‡Ã˜ÃÂ¦Ã‡Ã‘Â´Ã™.
            // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¸ÃÂ°Â¡ÃÂ³Â´Ã™Â´Ã‚ Â¸ÃžÂ½ÃƒÃÃ¶ <- Ã€ÃŒÂ°Ã‰ Â¹ÃžÃ€Â¸Â¸Ã© Ã€Ã¥Ã‚Ã¸ÃˆÂ­Â¸Ã©Â¿Â¡Â¼Â­ Ã‡Ã˜ÃÂ¦Â½ÃƒÃ„Ã‘Â¾ÃŸ Ã‡Ã‘Â´Ã™.
            SendNotifyMsg(0, iTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, 0, 0);
            // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Ã€Ã¥Ã‚Ã¸ Ã‡Ã˜ÃÂ¦ Â½ÃƒÃ…Â²Â´Ã™.
            ReleaseItemHandler(iTargetH, iTemp, true);  // <- Ã€ÃŒ Ã‡Ã”Â¼Ã¶Â´Ã‚ Â¼Â­Â¹Ã¶Â¿Â¡Â¼Â­Ã€Ã‡ ÃˆÂ¿Â°ÃºÂ¸Â¸ ÃƒÂ³Â¸Â® 
        }
    }

    // Â¹Ã™ÃÃ¶ ÃˆÂ¤Ã€Âº Â½Ã…Â¹ÃŸÂ·Ã¹ Â¹Ã¦Â¾Ã®Â±Â¸
    iTemp = m_pClientList[iTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_PANTS];
    if ((iTemp != -1) && (m_pClientList[iTargetH]->m_pItemList[iTemp] != 0))
    {

        // v1.432 ÃÃŸÂ¸Â³Ã€ÃŽ Â°Ã¦Â¿Ã¬ Â¼Ã¶Â¸Ã­Ã€ÃŒ ÃÃ™ÃÃ¶ Â¾ÃŠÃ€Â½
        if ((m_pClientList[iTargetH]->m_cSide != 0) && (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0))
            m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan -= iValue;

        if ((m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) || (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 64000))
        {
            m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan = 0;
            // Â¼Ã¶Â¸Ã­Ã€ÃŒ Â´Ã™ ÂµÃ‡Â¾ÃºÃ€Â¸Â¹Ã‡Â·ÃŽ Ã€Ã¥Ã‚Ã¸Ã€Â» Ã‡Ã˜ÃÂ¦Ã‡Ã‘Â´Ã™.
            // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¸ÃÂ°Â¡ÃÂ³Â´Ã™Â´Ã‚ Â¸ÃžÂ½ÃƒÃÃ¶ <- Ã€ÃŒÂ°Ã‰ Â¹ÃžÃ€Â¸Â¸Ã© Ã€Ã¥Ã‚Ã¸ÃˆÂ­Â¸Ã©Â¿Â¡Â¼Â­ Ã‡Ã˜ÃÂ¦Â½ÃƒÃ„Ã‘Â¾ÃŸ Ã‡Ã‘Â´Ã™.
            SendNotifyMsg(0, iTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, 0, 0);
            // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Ã€Ã¥Ã‚Ã¸ Ã‡Ã˜ÃÂ¦ Â½ÃƒÃ…Â²Â´Ã™.
            ReleaseItemHandler(iTargetH, iTemp, true);  // <- Ã€ÃŒ Ã‡Ã”Â¼Ã¶Â´Ã‚ Â¼Â­Â¹Ã¶Â¿Â¡Â¼Â­Ã€Ã‡ ÃˆÂ¿Â°ÃºÂ¸Â¸ ÃƒÂ³Â¸Â® 
        }
    }

    iTemp = m_pClientList[iTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LEGGINGS];
    if ((iTemp != -1) && (m_pClientList[iTargetH]->m_pItemList[iTemp] != 0))
    {

        // v1.432 ÃÃŸÂ¸Â³Ã€ÃŽ Â°Ã¦Â¿Ã¬ Â¼Ã¶Â¸Ã­Ã€ÃŒ ÃÃ™ÃÃ¶ Â¾ÃŠÃ€Â½
        if ((m_pClientList[iTargetH]->m_cSide != 0) && (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0))
            m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan -= iValue;

        if ((m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) || (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 64000))
        {
            m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan = 0;
            // Â¼Ã¶Â¸Ã­Ã€ÃŒ Â´Ã™ ÂµÃ‡Â¾ÃºÃ€Â¸Â¹Ã‡Â·ÃŽ Ã€Ã¥Ã‚Ã¸Ã€Â» Ã‡Ã˜ÃÂ¦Ã‡Ã‘Â´Ã™.
            // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¸ÃÂ°Â¡ÃÂ³Â´Ã™Â´Ã‚ Â¸ÃžÂ½ÃƒÃÃ¶ <- Ã€ÃŒÂ°Ã‰ Â¹ÃžÃ€Â¸Â¸Ã© Ã€Ã¥Ã‚Ã¸ÃˆÂ­Â¸Ã©Â¿Â¡Â¼Â­ Ã‡Ã˜ÃÂ¦Â½ÃƒÃ„Ã‘Â¾ÃŸ Ã‡Ã‘Â´Ã™.
            SendNotifyMsg(0, iTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, 0, 0);
            // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Ã€Ã¥Ã‚Ã¸ Ã‡Ã˜ÃÂ¦ Â½ÃƒÃ…Â²Â´Ã™.
            ReleaseItemHandler(iTargetH, iTemp, true);  // <- Ã€ÃŒ Ã‡Ã”Â¼Ã¶Â´Ã‚ Â¼Â­Â¹Ã¶Â¿Â¡Â¼Â­Ã€Ã‡ ÃˆÂ¿Â°ÃºÂ¸Â¸ ÃƒÂ³Â¸Â® 
        }
    }

    // Ã†ÃˆÂ°Â©Â¿ÃŠ 
    iTemp = m_pClientList[iTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_ARMS];
    if ((iTemp != -1) && (m_pClientList[iTargetH]->m_pItemList[iTemp] != 0))
    {

        // v1.432 ÃÃŸÂ¸Â³Ã€ÃŽ Â°Ã¦Â¿Ã¬ Â¼Ã¶Â¸Ã­Ã€ÃŒ ÃÃ™ÃÃ¶ Â¾ÃŠÃ€Â½
        if ((m_pClientList[iTargetH]->m_cSide != 0) && (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0))
            m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan -= iValue;

        if ((m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) || (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 64000))
        {
            m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan = 0;
            // Â¼Ã¶Â¸Ã­Ã€ÃŒ Â´Ã™ ÂµÃ‡Â¾ÃºÃ€Â¸Â¹Ã‡Â·ÃŽ Ã€Ã¥Ã‚Ã¸Ã€Â» Ã‡Ã˜ÃÂ¦Ã‡Ã‘Â´Ã™.
            // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¸ÃÂ°Â¡ÃÂ³Â´Ã™Â´Ã‚ Â¸ÃžÂ½ÃƒÃÃ¶ <- Ã€ÃŒÂ°Ã‰ Â¹ÃžÃ€Â¸Â¸Ã© Ã€Ã¥Ã‚Ã¸ÃˆÂ­Â¸Ã©Â¿Â¡Â¼Â­ Ã‡Ã˜ÃÂ¦Â½ÃƒÃ„Ã‘Â¾ÃŸ Ã‡Ã‘Â´Ã™.
            SendNotifyMsg(0, iTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, 0, 0);
            // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Ã€Ã¥Ã‚Ã¸ Ã‡Ã˜ÃÂ¦ Â½ÃƒÃ…Â²Â´Ã™.
            ReleaseItemHandler(iTargetH, iTemp, true);  // <- Ã€ÃŒ Ã‡Ã”Â¼Ã¶Â´Ã‚ Â¼Â­Â¹Ã¶Â¿Â¡Â¼Â­Ã€Ã‡ ÃˆÂ¿Â°ÃºÂ¸Â¸ ÃƒÂ³Â¸Â® 
        }
    }

    // Ã…ÃµÂ±Â¸ 
    iTemp = m_pClientList[iTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_HEAD];
    if ((iTemp != -1) && (m_pClientList[iTargetH]->m_pItemList[iTemp] != 0))
    {

        // v1.432 ÃÃŸÂ¸Â³Ã€ÃŽ Â°Ã¦Â¿Ã¬ Â¼Ã¶Â¸Ã­Ã€ÃŒ ÃÃ™ÃÃ¶ Â¾ÃŠÃ€Â½
        if ((m_pClientList[iTargetH]->m_cSide != 0) && (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0))
            m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan -= iValue;

        if ((m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) || (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 64000))
        {
            m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan = 0;
            // Â¼Ã¶Â¸Ã­Ã€ÃŒ Â´Ã™ ÂµÃ‡Â¾ÃºÃ€Â¸Â¹Ã‡Â·ÃŽ Ã€Ã¥Ã‚Ã¸Ã€Â» Ã‡Ã˜ÃÂ¦Ã‡Ã‘Â´Ã™.
            // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ Â¸ÃÂ°Â¡ÃÂ³Â´Ã™Â´Ã‚ Â¸ÃžÂ½ÃƒÃÃ¶ <- Ã€ÃŒÂ°Ã‰ Â¹ÃžÃ€Â¸Â¸Ã© Ã€Ã¥Ã‚Ã¸ÃˆÂ­Â¸Ã©Â¿Â¡Â¼Â­ Ã‡Ã˜ÃÂ¦Â½ÃƒÃ„Ã‘Â¾ÃŸ Ã‡Ã‘Â´Ã™.
            SendNotifyMsg(0, iTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, 0, 0);
            // Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Â» Ã€Ã¥Ã‚Ã¸ Ã‡Ã˜ÃÂ¦ Â½ÃƒÃ…Â²Â´Ã™.
            ReleaseItemHandler(iTargetH, iTemp, true);  // <- Ã€ÃŒ Ã‡Ã”Â¼Ã¶Â´Ã‚ Â¼Â­Â¹Ã¶Â¿Â¡Â¼Â­Ã€Ã‡ ÃˆÂ¿Â°ÃºÂ¸Â¸ ÃƒÂ³Â¸Â® 
        }
    }
}

int CGame::iGetPlayerABSStatus(int iClientH)
{
    int iRet;

    if (m_pClientList[iClientH] == nullptr) return 0;

    iRet = 0;

    if (m_pClientList[iClientH]->m_iPKCount != 0)
    {
        iRet = 8;
    }

    if (m_pClientList[iClientH]->m_cSide != 0)
    {
        iRet = iRet | 4;
    }

    if (m_pClientList[iClientH]->m_cSide == 1)
    {
        iRet = iRet | 2;
    }

    if (m_pClientList[iClientH]->m_bIsPlayerCivil == true)
        iRet = iRet | 1;

    return iRet;
}

bool CGame::_bInitItemAttr(class CItem * pItem, int iItemID)
{
    int i;

    for (i = 0; i < DEF_MAXITEMTYPES; i++)
        if (m_pItemConfigList[i] != 0)
        {
            if (m_pItemConfigList[i]->m_sIDnum == iItemID)
            {
                // Â°Â°Ã€Âº Ã€ÃŒÂ¸Â§Ã€Â» Â°Â¡ÃÃ¸ Â¾Ã†Ã€ÃŒÃ…Ã› Â¼Â³ÃÂ¤Ã€Â» ÃƒÂ£Â¾Ã’Â´Ã™. Â¼Â³ÃÂ¤Â°ÂªÃ€Â» ÂºÂ¹Â»Ã§Ã‡Ã‘Â´Ã™.
                memset(pItem->m_cName, 0, sizeof(pItem->m_cName));
                strcpy(pItem->m_cName, m_pItemConfigList[i]->m_cName);
                pItem->m_cItemType = m_pItemConfigList[i]->m_cItemType;
                pItem->m_cEquipPos = m_pItemConfigList[i]->m_cEquipPos;
                pItem->m_sItemEffectType = m_pItemConfigList[i]->m_sItemEffectType;
                pItem->m_sItemEffectValue1 = m_pItemConfigList[i]->m_sItemEffectValue1;
                pItem->m_sItemEffectValue2 = m_pItemConfigList[i]->m_sItemEffectValue2;
                pItem->m_sItemEffectValue3 = m_pItemConfigList[i]->m_sItemEffectValue3;
                pItem->m_sItemEffectValue4 = m_pItemConfigList[i]->m_sItemEffectValue4;
                pItem->m_sItemEffectValue5 = m_pItemConfigList[i]->m_sItemEffectValue5;
                pItem->m_sItemEffectValue6 = m_pItemConfigList[i]->m_sItemEffectValue6;
                pItem->m_wMaxLifeSpan = m_pItemConfigList[i]->m_wMaxLifeSpan;
                pItem->m_wCurLifeSpan = pItem->m_wMaxLifeSpan;
                pItem->m_sSpecialEffect = m_pItemConfigList[i]->m_sSpecialEffect;

                pItem->m_sSprite = m_pItemConfigList[i]->m_sSprite;
                pItem->m_sSpriteFrame = m_pItemConfigList[i]->m_sSpriteFrame;
                pItem->m_wPrice = m_pItemConfigList[i]->m_wPrice;
                pItem->m_wWeight = m_pItemConfigList[i]->m_wWeight;
                pItem->m_cApprValue = m_pItemConfigList[i]->m_cApprValue;
                pItem->m_cSpeed = m_pItemConfigList[i]->m_cSpeed;
                pItem->m_sLevelLimit = m_pItemConfigList[i]->m_sLevelLimit;
                pItem->m_cGenderLimit = m_pItemConfigList[i]->m_cGenderLimit;

                pItem->m_sSpecialEffectValue1 = m_pItemConfigList[i]->m_sSpecialEffectValue1;
                pItem->m_sSpecialEffectValue2 = m_pItemConfigList[i]->m_sSpecialEffectValue2;

                pItem->m_sRelatedSkill = m_pItemConfigList[i]->m_sRelatedSkill;
                pItem->m_cCategory = m_pItemConfigList[i]->m_cCategory;
                pItem->m_sIDnum = m_pItemConfigList[i]->m_sIDnum;

                pItem->m_bIsForSale = m_pItemConfigList[i]->m_bIsForSale;
                pItem->m_cItemColor = m_pItemConfigList[i]->m_cItemColor;

                return true;
            }
        }
    return false;
}

void CGame::SetPlayingStatus(int iClientH)
{
    char cMapName[11], cLocation[11];

    if (m_pClientList[iClientH] == nullptr) return;

    memset(cMapName, 0, sizeof(cMapName));
    memset(cLocation, 0, sizeof(cLocation));

    strcpy(cLocation, m_pClientList[iClientH]->m_cLocation);
    strcpy(cMapName, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName);

    m_pClientList[iClientH]->m_cSide = 0;
    m_pClientList[iClientH]->m_bIsOwnLocation = false;
    m_pClientList[iClientH]->m_bIsPlayerCivil = false;

    if (memcmp(cLocation, cMapName, 3) == 0)
    {
        m_pClientList[iClientH]->m_bIsOwnLocation = true;
    }

    if (memcmp(cLocation, "are", 3) == 0)
        m_pClientList[iClientH]->m_cSide = 1;
    else if (memcmp(cLocation, "elv", 3) == 0)
        m_pClientList[iClientH]->m_cSide = 2;
    else
    {
        if (strcmp(cMapName, "elvine") == 0 || strcmp(cMapName, "aresden") == 0)
        {
            m_pClientList[iClientH]->m_bIsOwnLocation = true;
        }
        m_pClientList[iClientH]->m_bIsNeutral = true;
    }

    if (memcmp(cLocation, "arehunter", 9) == 0 || memcmp(cLocation, "elvhunter", 9) == 0)
    {
        m_pClientList[iClientH]->m_bIsPlayerCivil = true;
    }

    if (memcmp(m_pClientList[iClientH]->m_cMapName, "bisle", 5) == 0)
    {
        m_pClientList[iClientH]->m_bIsPlayerCivil = false;
    }

    if (memcmp(m_pClientList[iClientH]->m_cMapName, "bsmith", 6) == 0 ||
        memcmp(m_pClientList[iClientH]->m_cMapName, "gldhall", 7) == 0 ||
        memcmp(m_pClientList[iClientH]->m_cMapName, "gshop", 5) == 0)
        m_pClientList[iClientH]->m_pIsProcessingAllowed = true;
    else
        m_pClientList[iClientH]->m_pIsProcessingAllowed = false;

    if (memcmp(m_pClientList[iClientH]->m_cMapName, "wrhus", 5) == 0 ||
        memcmp(m_pClientList[iClientH]->m_cMapName, "arewrhus", 8) == 0 ||
        memcmp(m_pClientList[iClientH]->m_cMapName, "elvwrhus", 8) == 0)
        m_pClientList[iClientH]->m_bIsInsideWarehouse = true;
    else
        m_pClientList[iClientH]->m_bIsInsideWarehouse = false;

    if (memcmp(m_pClientList[iClientH]->m_cMapName, "wzdtwr", 6) == 0)
        m_pClientList[iClientH]->m_bIsInsideWizardTower = true;
    else
        m_pClientList[iClientH]->m_bIsInsideWizardTower = false;
}

void CGame::ForceChangePlayMode(int iClientH, bool bNotify)
{
    if (m_pClientList[iClientH] == nullptr) return;

    if (memcmp(m_pClientList[iClientH]->m_cLocation, "arehunter", 9) == 0)
        strcpy(m_pClientList[iClientH]->m_cLocation, "aresden");
    else if (memcmp(m_pClientList[iClientH]->m_cLocation, "elvhunter", 9) == 0)
        strcpy(m_pClientList[iClientH]->m_cLocation, "elvine");

    if (m_pClientList[iClientH]->m_bIsPlayerCivil == true)
        m_pClientList[iClientH]->m_bIsPlayerCivil = false;

    if (bNotify)
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_CHANGEPLAYMODE, 0, 0, 0, m_pClientList[iClientH]->m_cLocation);
        SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
    }
}

void CGame::ShowVersion(int iClientH)
{
    char cVerMessage[256];

    memset(cVerMessage, 0, sizeof(cVerMessage));
    wsprintf(cVerMessage, "Helbreath Sources %s.%s - www.xtremehb.com", DEF_UPPERVERSION, DEF_LOWERVERSION);
    ShowClientMsg(iClientH, cVerMessage);

}

bool CGame::_bPKLog(int iAction, int iAttackerH, int iVictumH, char * pNPC)
{
    char  cTxt[1024], cTemp1[120], cTemp2[120];

    //  Â·ÃŽÂ±Ã— Â³Â²Â±Ã¤Â´Ã™. 
    memset(cTxt, 0, sizeof(cTxt));
    memset(cTemp1, 0, sizeof(cTemp1));
    memset(cTemp2, 0, sizeof(cTemp2));

    if (m_pClientList[iVictumH] == 0) return false;

    switch (iAction)
    {

        case DEF_PKLOG_REDUCECRIMINAL:
            wsprintf(cTxt, "(%s) PC(%s)\tReduce\tCC(%d)\t%s(%d %d)\t", m_pClientList[iVictumH]->m_cIPaddress, m_pClientList[iVictumH]->m_cCharName, m_pClientList[iVictumH]->m_iPKCount,
                m_pClientList[iVictumH]->m_cMapName, m_pClientList[iVictumH]->m_sX, m_pClientList[iVictumH]->m_sY);
            break;

        case DEF_PKLOG_BYPLAYER:
            if (m_pClientList[iAttackerH] == 0) return false;
            wsprintf(cTxt, "(%s) PC(%s)\tKilled by PC\t \t%s(%d %d)\t(%s) PC(%s)", m_pClientList[iVictumH]->m_cIPaddress, m_pClientList[iVictumH]->m_cCharName,
                m_pClientList[iVictumH]->m_cMapName, m_pClientList[iVictumH]->m_sX, m_pClientList[iVictumH]->m_sY, m_pClientList[iAttackerH]->m_cIPaddress, m_pClientList[iAttackerH]->m_cCharName);
            break;
        case DEF_PKLOG_BYPK:
            if (m_pClientList[iAttackerH] == 0) return false;
            wsprintf(cTxt, "(%s) PC(%s)\tKilled by PK\tCC(%d)\t%s(%d %d)\t(%s) PC(%s)", m_pClientList[iVictumH]->m_cIPaddress, m_pClientList[iVictumH]->m_cCharName, m_pClientList[iAttackerH]->m_iPKCount,
                m_pClientList[iVictumH]->m_cMapName, m_pClientList[iVictumH]->m_sX, m_pClientList[iVictumH]->m_sY, m_pClientList[iAttackerH]->m_cIPaddress, m_pClientList[iAttackerH]->m_cCharName);
            break;
        case DEF_PKLOG_BYENERMY:
            if (m_pClientList[iAttackerH] == 0) return false;
            wsprintf(cTxt, "(%s) PC(%s)\tKilled by Enemy\t \t%s(%d %d)\t(%s) PC(%s)", m_pClientList[iVictumH]->m_cIPaddress, m_pClientList[iVictumH]->m_cCharName,
                m_pClientList[iVictumH]->m_cMapName, m_pClientList[iVictumH]->m_sX, m_pClientList[iVictumH]->m_sY, m_pClientList[iAttackerH]->m_cIPaddress, m_pClientList[iAttackerH]->m_cCharName);
            break;
        case DEF_PKLOG_BYNPC:
            if (pNPC == 0) return false;
            wsprintf(cTxt, "(%s) PC(%s)\tKilled by NPC\t \t%s(%d %d)\tNPC(%s)", m_pClientList[iVictumH]->m_cIPaddress, m_pClientList[iVictumH]->m_cCharName,
                m_pClientList[iVictumH]->m_cMapName, m_pClientList[iVictumH]->m_sX, m_pClientList[iVictumH]->m_sY, pNPC);
            break;
        case DEF_PKLOG_BYOTHER:
            wsprintf(cTxt, "(%s) PC(%s)\tKilled by Other\t \t%s(%d %d)\tUnknown", m_pClientList[iVictumH]->m_cIPaddress, m_pClientList[iVictumH]->m_cCharName,
                m_pClientList[iVictumH]->m_cMapName, m_pClientList[iVictumH]->m_sX, m_pClientList[iVictumH]->m_sY);
            break;
        default:
            return false;
    }
    PutPvPLogFileList(cTxt);
    return true;
}

void CGame::RequestResurrectPlayer(int iClientH, bool bResurrect)
{
    short sX, sY;
    char buff[100];

    if (m_pClientList[iClientH] == nullptr) return;

    sX = m_pClientList[iClientH]->m_sX;
    sY = m_pClientList[iClientH]->m_sY;

    if (bResurrect == false)
    {
        m_pClientList[iClientH]->m_bIsBeingResurrected = false;
        return;
    }

    if (m_pClientList[iClientH]->m_bIsBeingResurrected == false)
    {
        try
        {
            wsprintf(buff, "(!!!) Player(%s) Tried To Use Resurrection Hack", m_pClientList[iClientH]->m_cCharName);
            PutHackLogFileList(G_cTxt);
            DeleteClient(iClientH, true, true, true, true);
        }
        catch (...)
        {
        }
        return;
    }

    wsprintf(buff, "(*) Resurrect Player! %s", m_pClientList[iClientH]->m_cCharName);
    log->info(buff);


    m_pClientList[iClientH]->m_bIsKilled = false;
    // Player's HP becomes half of the Max HP. 
    m_pClientList[iClientH]->m_iHP = iGetMaxHP(iClientH) / 2;
    // Player's MP
    m_pClientList[iClientH]->m_iMP = ((m_pClientList[iClientH]->m_iMag * 2) + (m_pClientList[iClientH]->m_iLevel / 2)) + m_pClientList[iClientH]->m_iInt / 2;
    // Player's SP
    m_pClientList[iClientH]->m_iSP = (m_pClientList[iClientH]->m_iStr * 2) + (m_pClientList[iClientH]->m_iLevel / 2);
    // Player's Hunger
    m_pClientList[iClientH]->m_iHungerStatus = 100;

    m_pClientList[iClientH]->m_bIsBeingResurrected = false;

    // !!! RequestTeleportHandlerÂ³Â»Â¿Â¡Â¼Â­ m_cMapNameÃ€Â» Â¾Â²Â±Ã¢ Â¶Â§Â¹Â®Â¿Â¡ Â±Ã—Â´Ã«Â·ÃŽ Ã†Ã„Â¶Ã³Â¹ÃŒÃ…ÃÂ·ÃŽ Â³Ã‘Â°ÃœÃÃ–Â¸Ã© Â¿Ã€ÂµÂ¿Ã€Ã›
    RequestTeleportHandler(iClientH, "2   ", m_pClientList[iClientH]->m_cMapName, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY);
}

bool CGame::bCheckClientAttackFrequency(int iClientH, uint32_t dwClientTime)
{
    uint32_t dwTimeGap;

    if (m_pClientList[iClientH] == nullptr) return false;
    if (m_pClientList[iClientH]->m_iAdminUserLevel > 0) return false;

    if (m_pClientList[iClientH]->m_dwAttackFreqTime == 0)
        m_pClientList[iClientH]->m_dwAttackFreqTime = dwClientTime;
    else
    {
        dwTimeGap = dwClientTime - m_pClientList[iClientH]->m_dwAttackFreqTime;
        m_pClientList[iClientH]->m_dwAttackFreqTime = dwClientTime;

        if (dwTimeGap < 450)
        {
            try
            {
                wsprintf(G_cTxt, "Swing Hack: (%s) Player: (%s) - attacking with weapon at irregular rates.", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
                PutHackLogFileList(G_cTxt);
                DeleteClient(iClientH, true, true);
            }
            catch (...)
            {
            }

            return false;
        }

        //testcode
        //wsprintf(G_cTxt, "Attack: %d", dwTimeGap);
        //log->info(G_cTxt);
    }

    return false;
}

bool CGame::bCheckClientMagicFrequency(int iClientH, uint32_t dwClientTime)
{
    uint32_t dwTimeGap;

    if (m_pClientList[iClientH] == nullptr) return false;

    if (m_pClientList[iClientH]->m_dwMagicFreqTime == 0)
        m_pClientList[iClientH]->m_dwMagicFreqTime = dwClientTime;
    else
    {
        dwTimeGap = dwClientTime - m_pClientList[iClientH]->m_dwMagicFreqTime;
        m_pClientList[iClientH]->m_dwMagicFreqTime = dwClientTime;

        if ((dwTimeGap < 1500) && (m_pClientList[iClientH]->m_bMagicConfirm == true))
        {
            try
            {
                wsprintf(G_cTxt, "Speed Cast: (%s) Player: (%s) - casting magic at irregular rates. ", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
                PutHackLogFileList(G_cTxt);
                DeleteClient(iClientH, true, true);
            }
            catch (...)
            {
            }
            return false;
        }

        m_pClientList[iClientH]->m_iSpellCount--;
        m_pClientList[iClientH]->m_bMagicConfirm = false;
        m_pClientList[iClientH]->m_bMagicPauseTime = false;

        //testcode
        //wsprintf(G_cTxt, "Magic: %d", dwTimeGap);
        //log->info(G_cTxt);
    }

    return false;
}

bool CGame::bCheckClientMoveFrequency(int iClientH, uint32_t dwClientTime)
{
    uint32_t dwTimeGap;

    if (m_pClientList[iClientH] == nullptr) return false;
    if (m_pClientList[iClientH]->m_iAdminUserLevel > 0) return false;

    if (m_pClientList[iClientH]->m_dwMoveFreqTime == 0)
        m_pClientList[iClientH]->m_dwMoveFreqTime = dwClientTime;
    else
    {
        if (m_pClientList[iClientH]->m_bIsMoveBlocked == true)
        {
            m_pClientList[iClientH]->m_dwMoveFreqTime = 0;
            m_pClientList[iClientH]->m_bIsMoveBlocked = false;
            return false;
        }

        if (m_pClientList[iClientH]->m_bIsAttackModeChange == true)
        {
            m_pClientList[iClientH]->m_dwMoveFreqTime = 0;
            m_pClientList[iClientH]->m_bIsAttackModeChange = false;
            return false;
        }

        dwTimeGap = dwClientTime - m_pClientList[iClientH]->m_dwMoveFreqTime;
        m_pClientList[iClientH]->m_dwMoveFreqTime = dwClientTime;

        if ((dwTimeGap < 200) && (dwTimeGap >= 0))
        {
            try
            {
                wsprintf(G_cTxt, "Speed Hack: (%s) Player: (%s) - running too fast.", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
                PutHackLogFileList(G_cTxt);
                DeleteClient(iClientH, true, true);
            }
            catch (...)
            {
            }
            return false;
        }

        // testcode
        // wsprintf(G_cTxt, "Move: %d", dwTimeGap);
        // log->info(G_cTxt);
    }

    return false;
}

void CGame::OnTimer(char cType)
{
    uint32_t dwTime;

    dwTime = timeGetTime();

    //if ((dwTime - m_dwGameTime1) > 200) {
    GameProcess();
    // m_dwGameTime1 = dwTime;
    //}


    if ((dwTime - m_dwGameTime2) > 3000)
    {
        CheckClientResponseTime();
        SendMsgToGateServer(MSGID_GAMESERVERALIVE, 0);
        CheckDayOrNightMode();
        // È­¸é °»½Å 
        InvalidateRect(G_hWnd, 0, true);
        m_dwGameTime2 = dwTime;
        // v1.41 
        _CheckGateSockConnection();

        // v1.41
        if ((m_bIsGameStarted == false) && (m_bIsItemAvailable == true) &&
            (m_bIsNpcAvailable == true) && (m_bIsGateSockAvailable == true) &&
            (m_bIsLogSockAvailable == true) && (m_bIsMagicAvailable == true) &&
            (m_bIsSkillAvailable == true) && (m_bIsPortionAvailable == true) &&
            (m_bIsQuestAvailable == true) && (m_bIsBuildItemAvailable == true) &&
            (m_iSubLogSockActiveCount == DEF_MAXSUBLOGSOCK))
        {
            // °ÔÀÓÀ» ½ÃÀÛÇÒ ÁØºñ°¡ µÇ¾ú´Ù. ¸Þ½ÃÁö¸¦ º¸³½´Ù.
            log->info("Sending start message...");
            SendMessage(m_hWnd, WM_USER_STARTGAMESIGNAL, 0, 0);
            m_bIsGameStarted = true;
        }
    }
    if ((dwTime - m_dwGameTime6) > 1000)
    {
        DelayEventProcessor();
        SendStockMsgToGateServer();
        m_dwGameTime6 = dwTime;

        // v2.05
        if (m_iFinalShutdownCount != 0)
        {
            m_iFinalShutdownCount--;
            wsprintf(G_cTxt, "Final Shutdown...%d", m_iFinalShutdownCount);
            log->info(G_cTxt);
            if (m_iFinalShutdownCount <= 1)
            {
                // 2.14 ¼ºÈÄ´Ï ¼Ë´Ù¿î½Ã ¹«Á¶°Ç ·Î±× ÀúÀå 
                SendMessage(m_hWnd, WM_CLOSE, 0, 0);
                return;

            }
        }
    }

    if ((dwTime - m_dwGameTime3) > 3000)
    {
        SyncMiddlelandMapInfo();
        CheckDynamicObjectList();
        DynamicObjectEffectProcessor();
        NoticeHandler();
        SpecialEventHandler();
        EnergySphereProcessor();
        m_dwGameTime3 = dwTime;
    }

    if ((dwTime - m_dwGameTime4) > 600)
    {
        MobGenerator();

        // v1.432-3 Sub-Log-SocketÀ» ÇÑ²¨¹ø¿¡ ¸¸µå´Â °ÍÀÌ ¾Æ´Ï¶ó ³ª´©¾î ¸¸µç´Ù.
        if (m_iSubLogSockInitIndex < DEF_MAXSUBLOGSOCK)
        {
            m_pSubLogSock[m_iSubLogSockInitIndex] = new class XSocket(m_hWnd, DEF_SERVERSOCKETBLOCKLIMIT);
            m_pSubLogSock[m_iSubLogSockInitIndex]->bConnect(m_cLogServerAddr, m_iLogServerPort, (WM_ONLOGSOCKETEVENT + m_iSubLogSockInitIndex + 1));
            m_pSubLogSock[m_iSubLogSockInitIndex]->bInitBufferSize(DEF_MSGBUFFERSIZE);
            wsprintf(G_cTxt, "(!) Try to connect sub-log-socket(%d)... Addr:%s  Port:%d", m_iSubLogSockInitIndex, m_cLogServerAddr, m_iLogServerPort);
            log->info(G_cTxt);
            m_iSubLogSockInitIndex++;
        }

        m_dwGameTime4 = dwTime;
    }



    if ((dwTime - m_dwGameTime5) > 1000 * 60 * 3)
    {

        if (m_iMiddlelandMapIndex >= 0)
        {
            // Crusade
            SaveOccupyFlagData();
            //bSendMsgToLS(MSGID_REQUEST_SAVEARESDENOCCUPYFLAGDATA, 0, 0);
            //bSendMsgToLS(MSGID_REQUEST_SAVEELVINEOCCUPYFLAGDATA, 0, 0);
        }
        m_dwGameTime5 = dwTime;

        // v1.41 ·£´ý ½Ãµå°ª ÃÊ±âÈ­.
        srand((unsigned)time(0));
    }

    if ((dwTime - m_dwFishTime) > 4000)
    {
        FishProcessor();
        FishGenerator();
        SendCollectedMana();
        CrusadeWarStarter();
        //ApocalypseStarter();
        ApocalypseEnder();
        m_dwFishTime = dwTime;
    }

    if ((dwTime - m_dwWhetherTime) > 1000 * 20)
    {
        WhetherProcessor();
        m_dwWhetherTime = dwTime;
    }

    if ((m_bHeldenianRunning == true) && (m_bIsHeldenianMode == true))
    {
        SetHeldenianMode();
    }

    if ((dwTime - m_dwCanFightzoneReserveTime) > 7200000)
    {
        FightzoneReserveProcessor();
        m_dwCanFightzoneReserveTime = dwTime;
    }

    if ((m_bIsServerShutdowned == false) && (m_bOnExitProcess == true) && ((dwTime - m_dwExitProcessTime) > 1000 * 2))
    {
        if (_iForcePlayerDisconect(15) == 0)
        {
            log->info("(!) GAME SERVER SHUTDOWN PROCESS COMPLETED! All players are disconnected.");
            m_bIsServerShutdowned = true;

            if ((m_cShutDownCode == 3) || (m_cShutDownCode == 4))
            {
                log->info("(!!!) AUTO-SERVER-REBOOTING!");
                bInit();
                //m_iAutoRebootingCount++;
            }
            else
            {
                if (m_iFinalShutdownCount == 0)	m_iFinalShutdownCount = 20;
            }
        }
        m_dwExitProcessTime = dwTime;
    }

    if ((dwTime - m_dwMapSectorInfoTime) > 1000 * 10)
    {
        m_dwMapSectorInfoTime = dwTime;
        UpdateMapSectorInfo();

        MineralGenerator();

        m_iMapSectorInfoUpdateCount++;
        if (m_iMapSectorInfoUpdateCount >= 5)
        {
            AgingMapSectorInfo();
            m_iMapSectorInfoUpdateCount = 0;
        }
    }
}

void CGame::OnStartGameSignal()
{
    int i;

    for (i = 0; i < DEF_MAXMAPS; i++)
        if (m_pMapList[i] != 0)
            _bReadMapInfoFiles(i);

    bReadCrusadeStructureConfigFile("..\\GameConfigs\\Crusade.cfg");
    _LinkStrikePointMapIndex();
    bReadScheduleConfigFile("..\\GameConfigs\\Schedule.cfg");

    bReadCrusadeGUIDFile("GameData\\CrusadeGUID.txt");
    bReadApocalypseGUIDFile("GameData\\ApocalypseGUID.txt");
    bReadHeldenianGUIDFile("GameData\\HeldenianGUID.txt");

    log->info("");
    log->info("(!) Game Server Activated.");

}

void CGame::_ClearExchangeStatus(int iToH)
{
    if ((iToH <= 0) || (iToH >= DEF_MAXCLIENTS)) return;
    if (m_pClientList[iToH] == 0) return;

    if (m_pClientList[iToH]->m_cExchangeName != false)
        SendNotifyMsg(0, iToH, DEF_NOTIFY_CANCELEXCHANGEITEM, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0);

    // m_pClientList[iToH]->m_cExchangeName    = false;
    m_pClientList[iToH]->m_dwInitCCTime = false;
    m_pClientList[iToH]->m_iAlterItemDropIndex = 0;
    //m_pClientList[iToH]->m_cExchangeItemIndex = -1;
    m_pClientList[iToH]->m_iExchangeH = 0;

    m_pClientList[iToH]->m_bIsExchangeMode = false;

    memset(m_pClientList[iToH]->m_cExchangeName, 0, sizeof(m_pClientList[iToH]->m_cExchangeName));

}

void CGame::CancelExchangeItem(int iClientH)
{
    int iExH;

    // Ã‡Ã¶Ã€Ã§ Â±Â³ÃˆÂ¯Ã€Â» ÃƒÃ«Â¼Ã’Ã‡Ã‘Â´Ã™. Â»Ã³Â´Ã«Â¹Ã¦ÂµÂµ Ã€ÃšÂµÂ¿Ã€Â¸Â·ÃŽ ÃƒÃ«Â¼Ã’ÂµÃˆÂ´Ã™. 
    iExH = m_pClientList[iClientH]->m_iExchangeH;
    _ClearExchangeStatus(iExH);
    _ClearExchangeStatus(iClientH);
}

void CGame::SetTimeOut(int iClientH)
{
    int forcedIn;
    time_t seconds;
    SYSTEMTIME SysTime;

    seconds = time(0);
    GetLocalTime(&SysTime);

    if (m_pClientList[iClientH] == nullptr) return;

    m_pClientList[iClientH]->m_iForceStart = seconds;
    m_pClientList[iClientH]->isForceSet = true;

    switch (SysTime.wDayOfWeek)
    {
        case 1:	forcedIn = m_sRaidTimeMonday; break;
        case 2:	forcedIn = m_sRaidTimeTuesday; break;
        case 3:	forcedIn = m_sRaidTimeWednesday; break;
        case 4:	forcedIn = m_sRaidTimeThursday; break;
        case 5:	forcedIn = m_sRaidTimeFriday; break;
        case 6:	forcedIn = m_sRaidTimeSaturday; break;
        case 0:	forcedIn = m_sRaidTimeSunday; break;
    }

    SendNotifyMsg(0, iClientH, DEF_NOTIFY_FORCERECALLTIME, forcedIn, 0, 0, 0);

}

void CGame::CheckTimeOut(int iClientH)
{
    int forcedIn;
    time_t seconds;
    SYSTEMTIME SysTime;

    seconds = time(0);
    GetLocalTime(&SysTime);

    if (m_pClientList[iClientH] == nullptr) return;

    switch (SysTime.wDayOfWeek)
    {
        case 1:	forcedIn = m_sRaidTimeMonday; break;
        case 2:	forcedIn = m_sRaidTimeTuesday; break;
        case 3:	forcedIn = m_sRaidTimeWednesday; break;
        case 4:	forcedIn = m_sRaidTimeThursday; break;
        case 5:	forcedIn = m_sRaidTimeFriday; break;
        case 6:	forcedIn = m_sRaidTimeSaturday; break;
        case 0:	forcedIn = m_sRaidTimeSunday; break;
    }

    if ((seconds - m_pClientList[iClientH]->m_iForceStart) > (forcedIn * 60))
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_TOBERECALLED, 0, 0, 0, 0);
        if (memcmp(m_pClientList[iClientH]->m_cLocation, "are", 3) == 0)
        {
            RequestTeleportHandler(iClientH, "2   ", "aresden", -1, -1);
        }
        if (memcmp(m_pClientList[iClientH]->m_cLocation, "elv", 3) == 0)
        {
            RequestTeleportHandler(iClientH, "2   ", "elvine", -1, -1);
        }
    }

}

void CGame::SetForceRecallTime(int iClientH)
{
    int iTL_ = 0;
    SYSTEMTIME SysTime;

    if (m_pClientList[iClientH] == nullptr) return;

    if (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall == 0)
    {
        // ³²¾ÆÀÖÀ» ¼ö ÀÖ´Â ÀÜÁ¸ ½Ã°£À» ¼³Á¤ÇØ ÁØ´Ù. 
        // ½Ã°£À» ±¸ÇÑ´ÙÀ½ iWarPeriod¸¦ ±¸ÇÑ´Ù.

        // ¼­¹ö¿¡ °­ÄÝ ½Ã°£ÀÌ ¼³Á¤µÇ¾î ÀÖÀ¸¸é ¼­¹ö °­ÄÝ ½Ã°£À» °­ÄÝ ½Ã°£À¸·Î ÇÑ´Ù.

        if (m_sForceRecallTime > 0)
        {
            m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 20 * m_sForceRecallTime;
        }
        else
        {
            GetLocalTime(&SysTime);
            switch (SysTime.wDayOfWeek)
            {
                case 1:	m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 20 * m_sRaidTimeMonday; break;  //¿ù¿äÀÏ  3ºÐ 2002-09-10 #1
                case 2:	m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 20 * m_sRaidTimeTuesday; break;  //È­¿äÀÏ  3ºÐ 
                case 3:	m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 20 * m_sRaidTimeWednesday; break;  //¼ö¿äÀÏ  3ºÐ 
                case 4:	m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 20 * m_sRaidTimeThursday; break;  //¸ñ¿äÀÏ  3ºÐ 
                case 5:	m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 20 * m_sRaidTimeFriday; break; //±Ý¿äÀÏ 15ºÐ
                case 6:	m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 20 * m_sRaidTimeSaturday; break; //Åä¿äÀÏ 45ºÐ 
                case 0:	m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 20 * m_sRaidTimeSunday; break; //ÀÏ¿äÀÏ 60ºÐ
            }
        }
    }
    else
    { // if (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall == 0) 
        if (m_sForceRecallTime > 0)
        {
            iTL_ = 20 * m_sForceRecallTime;
        }
        else
        {

            // ÀÜÁ¸½Ã°£ÀÌ ÀÖ´Ù. ¿äÀÏº° ÀÜÁ¸½Ã°£º¸´Ù Å©¸é 1·Î ÃÊ±âÈ­ 
            GetLocalTime(&SysTime);
            switch (SysTime.wDayOfWeek)
            {
                case 1:	iTL_ = 20 * m_sRaidTimeMonday; break;  //¿ù¿äÀÏ  3ºÐ 2002-09-10 #1
                case 2:	iTL_ = 20 * m_sRaidTimeTuesday; break;  //È­¿äÀÏ  3ºÐ
                case 3:	iTL_ = 20 * m_sRaidTimeWednesday; break;  //¼ö¿äÀÏ  3ºÐ
                case 4:	iTL_ = 20 * m_sRaidTimeThursday; break;  //¸ñ¿äÀÏ  3ºÐ
                case 5:	iTL_ = 20 * m_sRaidTimeFriday; break; //±Ý¿äÀÏ 15ºÐ
                case 6:	iTL_ = 20 * m_sRaidTimeSaturday; break; //Åä¿äÀÏ 45ºÐ 
                case 0:	iTL_ = 20 * m_sRaidTimeSunday; break; //ÀÏ¿äÀÏ 60ºÐ
            }
        }

        if (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall > iTL_)
            m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 1;

    }

    m_pClientList[iClientH]->m_bIsWarLocation = true;
    return;
}

void CGame::CheckForceRecallTime(int iClientH)
{
    SYSTEMTIME SysTime;
    int iTL_;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_iAdminUserLevel > 0) return;

    if (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall == 0)
    {
        // has admin set a recall time ??
        if (m_sForceRecallTime > 0)
        {
            m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = m_sForceRecallTime * 20;
        }
        // use standard recall time calculations
        else
        {
            GetLocalTime(&SysTime);
            switch (SysTime.wDayOfWeek)
            {
                case 1:	m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 20 * m_sRaidTimeMonday; break;  //¿ù¿äÀÏ  3ºÐ 2002-09-10 #1
                case 2:	m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 20 * m_sRaidTimeTuesday; break;  //È­¿äÀÏ  3ºÐ 
                case 3:	m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 20 * m_sRaidTimeWednesday; break;  //¼ö¿äÀÏ  3ºÐ 
                case 4:	m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 20 * m_sRaidTimeThursday; break;  //¸ñ¿äÀÏ  3ºÐ 
                case 5:	m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 20 * m_sRaidTimeFriday; break; //±Ý¿äÀÏ 15ºÐ
                case 6:	m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 20 * m_sRaidTimeSaturday; break; //Åä¿äÀÏ 45ºÐ 
                case 0:	m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = 20 * m_sRaidTimeSunday; break; //ÀÏ¿äÀÏ 60ºÐ
            }
        }
    }
    else
    {
        // has admin set a recall time ??
        if (m_sForceRecallTime > 0)
        {
            iTL_ = m_sForceRecallTime * 20;
        }
        // use standard recall time calculations
        else
        {
            GetLocalTime(&SysTime);
            switch (SysTime.wDayOfWeek)
            {
                case 1:	iTL_ = 20 * m_sRaidTimeMonday; break;  //¿ù¿äÀÏ  3ºÐ 2002-09-10 #1
                case 2:	iTL_ = 20 * m_sRaidTimeTuesday; break;  //È­¿äÀÏ  3ºÐ 
                case 3:	iTL_ = 20 * m_sRaidTimeWednesday; break;  //¼ö¿äÀÏ  3ºÐ 
                case 4:	iTL_ = 20 * m_sRaidTimeThursday; break;  //¸ñ¿äÀÏ  3ºÐ 
                case 5:	iTL_ = 20 * m_sRaidTimeFriday; break; //±Ý¿äÀÏ 15ºÐ
                case 6:	iTL_ = 20 * m_sRaidTimeSaturday; break; //Åä¿äÀÏ 45ºÐ 
                case 0:	iTL_ = 20 * m_sRaidTimeSunday; break; //ÀÏ¿äÀÏ 60ºÐ
            }
        }
        if (m_pClientList[iClientH]->m_iTimeLeft_ForceRecall > iTL_)
            m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = iTL_;
    }

    m_pClientList[iClientH]->m_bIsWarLocation = true;
    return;

}

int ITEMSPREAD_FIEXD_COORD[25][2] =
{
    { 0,  0},	//1
    { 1,  0},	//2
    { 1,  1},	//3
    { 0,  1},	//4
    {-1,  1},	//5
    {-1,  0},	//6
    {-1, -1},	//7
    { 0, -1},	//8
    { 1, -1},	//9
    { 2, -1},	//10
    { 2,  0},	//11
    { 2,  1},	//12
    { 2,  2},	//13
    { 1,  2},	//14
    { 0,  2},	//15
    {-1,  2},	//16
    {-2,  2},	//17
    {-2,  1},	//18
    {-2,  0},	//19
    {-2, -1},	//20
    {-2, -2},	//21
    {-1, -2},	//22
    { 0, -2},	//23
    { 1, -2},	//24
    { 2, -2},	//25
};

bool CGame::bGetMultipleItemNamesWhenDeleteNpc(short sNpcType, int iProbability, int iMin, int iMax, short sBaseX, short sBaseY,
    int iItemSpreadType, int iSpreadRange,
    int * iItemIDs, POINT * BasePos, int * iNumItem)
{
    int		iProb = 100;
    float	fProb, fProbA, fProbB, fProbC;
    int		iItemID;
    int		iNum = 0;

    for (int i = 0; i < iMax; i++)
    {
        if (i > iMin) iProb = iProbability;

        fProb = (float)(100 - iProb) / 10.0;	//WyvernÃ€Ã‡ Ã†Ã²Â±Ã• 50
        if (fProb < 1.0) fProb = 1.0;

        fProbA = fProbB = fProbC = fProb;

        fProbA = fProbA * 8.0f;
        fProbB = fProbB * 4.0f;
        fProbC = fProbC;

        iItemID = 0;

        switch (sNpcType)
        {
            case 69: // Wyvern...stupid koreans
                // ÃÖ»ó±Þ
                switch (iDice(1, 4))
                {
                    case 1: if (iDice(1, (6000 * fProbA)) == 3) iItemID = 634; break; // RingofWizard
                    case 2: if (iDice(1, (5000 * fProbA)) == 3) iItemID = 636; break; // RingofGrandMage
                    case 3: if (iDice(1, (3000 * fProbA)) == 2) iItemID = 614; break; // SwordofIceElemental
                    case 4: if (iDice(1, (4500 * fProbA)) == 3) iItemID = 380; break; // IceStormManual
                }

                // »ó±Þ
                if (iItemID == 0)
                {
                    switch (iDice(1, 6))
                    {
                        case  1: if (iDice(1, (500 * fProbB)) == 2) iItemID = 642; break; // KnecklaceOfIcePro
                        case  2: if (iDice(1, (2000 * fProbB)) == 2) iItemID = 643; break; // KnecklaceOfIceEle
                        case  3: if (iDice(1, (1000 * fProbB)) == 3) iItemID = 943; break; // IceAxe
                        case  4: if (iDice(1, (1500 * fProbB)) == 3) iItemID = 734; break; // RingOfArcmage
                        case  5: if (iDice(1, (500 * fProbB)) == 3) iItemID = 942; break; // IceHammer
                        case  6: if (iDice(1, (500 * fProbB)) == 2) iItemID = 738; break; // BerserkWand(MS.20)
                    }
                }
                break;

            case 73: // Fire-Wyvern
                // ÃÖ»ó±Þ
                switch (iDice(1, 7))
                {
                    case  1: if (iDice(1, (5000 * fProbA)) == 3) iItemID = 860; break; // NecklaceOfXelima
                    case  2: if (iDice(1, (3000 * fProbA)) == 2) iItemID = 630; break; // RingoftheXelima
                    case  3: if (iDice(1, (3000 * fProbA)) == 2) iItemID = 738; break; // BerserkWand(MS.20)
                    case  4: if (iDice(1, (3000 * fProbA)) == 2) iItemID = 735; break; // RingOfDragonpower
                    case  5: if (iDice(1, (3000 * fProbA)) == 2) iItemID = 20; break; // Excaliber
                    case  6: if (iDice(1, (3000 * fProbA)) == 3) iItemID = 382; break; // BloodyShockW.Manual
                    case  7: if (iDice(1, (3000 * fProbA)) == 3) iItemID = 381; break; // MassFireStrikeManual
                }

                // »ó±Þ
                if (iItemID == 0)
                {
                    switch (iDice(1, 9))
                    {
                        case  1: if (iDice(1, (1000 * fProbB)) == 2) iItemID = 645; break; // KnecklaceOfEfreet
                        case  2: if (iDice(1, (500 * fProbB)) == 2) iItemID = 638; break; // KnecklaceOfFirePro
                        case  3: if (iDice(1, (1000 * fProbB)) == 3) iItemID = 636; break; // RingofGrandMage
                        case  4: if (iDice(1, (800 * fProbB)) == 3) iItemID = 734; break; // RingOfArcmage
                        case  5: if (iDice(1, (500 * fProbB)) == 3) iItemID = 634; break; // RingofWizard
                        case  6: if (iDice(1, (500 * fProbB)) == 2) iItemID = 290; break; // Flameberge+3(LLF)
                        case  7: if (iDice(1, (500 * fProbB)) == 3) iItemID = 490; break; // BloodSword
                        case  8: if (iDice(1, (500 * fProbB)) == 3) iItemID = 491; break; // BloodAxe
                        case  9: if (iDice(1, (500 * fProbB)) == 3) iItemID = 492; break; // BloodRapier
                    }
                }

                break;

            case 81: // Abaddon

                // ÃÖ»ó±Þ
                switch (iDice(1, 6))
                {
                    case 1: if (iDice(1, (100 * fProbA)) == 3) iItemID = 20;  break; // Excaliber
                    case 2: if (iDice(1, (100 * fProbA)) == 3) iItemID = 647; break; // NecklaceOfStoneGol
                    case 3: if (iDice(1, (100 * fProbA)) == 3) iItemID = 860; break; // NecklaceOfXelima
                    case 4: if (iDice(1, (100 * fProbA)) == 3) iItemID = 936; break; // MerienHat
                    case 5: if (iDice(1, (100 * fProbA)) == 2) iItemID = 631; break; // RingoftheAbaddon
                    case 6: if (iDice(1, (100 * fProbA)) == 2) iItemID = 937; break; // MerienHelm
                }

                // »ó±Þ
                if (iItemID == 0)
                {
                    switch (iDice(1, 15))
                    {
                        case  1: if (iDice(1, (4 * fProbB)) == 3) iItemID = 650; break; // ZemstoneOfSacrifice
                        case  2: if (iDice(1, (4 * fProbB)) == 3) iItemID = 490; break; // BloodSword
                        case  3: if (iDice(1, (4 * fProbB)) == 3) iItemID = 491; break; // BloodAxe
                        case  4: if (iDice(1, (4 * fProbB)) == 3) iItemID = 492; break; // BloodRapier
                        case  5: if (iDice(1, (4 * fProbB)) == 3) iItemID = 611; break; // XelimaAxe
                        case  6: if (iDice(1, (4 * fProbB)) == 3) iItemID = 610; break; // XelimaBlade
                        case  7: if (iDice(1, (4 * fProbB)) == 3) iItemID = 612; break; // XelimaRapier
                        case 10: if (iDice(1, (4 * fProbB)) == 3) iItemID = 645; break; // KnecklaceOfEfreet
                        case 11: if (iDice(1, (4 * fProbB)) == 3) iItemID = 638; break; // BloodKlonessAxe
                        case 12: if (iDice(1, (4 * fProbB)) == 3) iItemID = 382; break; // BloodyShockW.Manual
                        case 13: if (iDice(1, (4 * fProbB)) == 3) iItemID = 381; break; // MassFireStrikeManual
                        case 14: if (iDice(1, (4 * fProbB)) == 3) iItemID = 259; break; // MagicWand(M.Shield)
                        case 15: if (iDice(1, (4 * fProbB)) == 3) iItemID = 947; break; // DragonStaff(MS.40)
                    }
                }
                break;
        } // switch

        // ÀÏ¹Ý ¾ÆÀÌÅÛ ....dumb korean idiots
        if (iItemID == 0)
        {
            switch (iDice(1, 24))
            {
                case  1: if (iDice(1, (2 * fProbC)) == 2) iItemID = 740; break; // BagOfGold-medium
                case  2: if (iDice(1, (2 * fProbC)) == 2) iItemID = 741; break; // BagOfGold-large
                case  3: if (iDice(1, (2 * fProbC)) == 2) iItemID = 742; break; // BagOfGold-largest
                case  4:
                case  5:
                case  6:
                case  7: if (iDice(1, (2 * fProbC)) == 2) iItemID = 650; break; // ZemstoneOfSacrifice
                case  8:
                case  9: if (iDice(1, (2 * fProbC)) == 2) iItemID = 656; break; // StoneOfXelima
                case 10:
                case 11:
                case 12: if (iDice(1, (2 * fProbC)) == 2) iItemID = 657; break; // StoneOfMerien
                case 13:
                case 14:
                case 15: if (iDice(1, (2 * fProbC)) == 2) iItemID = 335; break; // EmeraldRing
                case 16:
                case 17:
                case 18: if (iDice(1, (2 * fProbC)) == 2) iItemID = 290; break; // Flameberge+3(LLF)
                case 19:
                case 20: if (iDice(1, (2 * fProbC)) == 2) iItemID = 259; break; // MagicWand(M.Shield)
                case 21: if (iDice(1, (2 * fProbC)) == 2) iItemID = 300; break; // MagicNecklace(RM10)
                case 22: if (iDice(1, (2 * fProbC)) == 2) iItemID = 311; break; // MagicNecklace(DF+10)
                case 23: if (iDice(1, (2 * fProbC)) == 2) iItemID = 305; break; // MagicNecklace(DM+1)
                case 24: if (iDice(1, (2 * fProbC)) == 2) iItemID = 308; break; // MagicNecklace(MS10)
            }
        }

        // È®·üÀÌ 100 ÀÎµ¥ ¾Æ¹« °Íµµ ³ª¿ÀÁö ¾Ê¾Ò´Ù.
        // Gold ÁØ´Ù. retarded koreans -_-
        if (iItemID == 0 && iProb == 100) iItemID = 90; // Gold

        if (iItemID != 0)
        {
            // item id
            iItemIDs[iNum] = iItemID;

            // item position
            switch (iItemSpreadType)
            {
                case DEF_ITEMSPREAD_RANDOM:
                    BasePos[iNum].x = sBaseX + iSpreadRange - iDice(1, iSpreadRange * 2);
                    BasePos[iNum].y = sBaseY + iSpreadRange - iDice(1, iSpreadRange * 2);
                    break;

                case DEF_ITEMSPREAD_FIXED:
                    BasePos[iNum].x = sBaseX + ITEMSPREAD_FIEXD_COORD[iNum][0];
                    BasePos[iNum].y = sBaseY + ITEMSPREAD_FIEXD_COORD[iNum][1];
                    break;
            }
            iNum++;
        }

    } // for

    *iNumItem = iNum;

    return true;

} // bGetMultipleItemNamesWhenDeleteNpc... duh...korean morons

void CGame::SendCollectedMana()
{
    char * cp, cData[120];
    uint16_t * wp;

    //SendMsgToGateServer(MSGID_COLLECTEDMANA, 0);

    // ¸ðÀÎ ¸¶³ª°¡ ¾ø´Ù¸é ¸Þ½ÃÁö Àü¼ÛÇÏÁö ¾Ê´Â´Ù. ¸¶³ª ½ºÅæÀÌ ¾ø´Â ¸Ê¿¡¼­ ¸Þ½ÃÁö¸¦ Àü¼ÛÇÏ¸é ¾ÈµÈ´Ù.
    if ((m_iCollectedMana[1] == 0) && (m_iCollectedMana[2] == 0)) return;

    //testcode
    wsprintf(G_cTxt, "Sending Collected Mana: %d %d", m_iCollectedMana[1], m_iCollectedMana[2]);
    log->info(G_cTxt);

    memset(cData, 0, sizeof(cData));
    cp = (char *)(cData);
    *cp = GSM_COLLECTEDMANA;
    cp++;

    wp = (uint16_t *)cp;
    *wp = (uint16_t)m_iCollectedMana[1];
    cp += 2;

    wp = (uint16_t *)cp;
    *wp = (uint16_t)m_iCollectedMana[2];
    cp += 2;

    CollectedManaHandler(m_iCollectedMana[1], m_iCollectedMana[2]);

    bStockMsgToGateServer(cData, 5);

    // ±×µ¿¾È ¸ðÀº ¸¶³ª·® º¸³»°í Å¬¸®¾î.
    m_iCollectedMana[0] = 0;
    m_iCollectedMana[1] = 0;
    m_iCollectedMana[2] = 0;
}

void CGame::_LinkStrikePointMapIndex()
{
    int i, z, x;

    // ¸¶À»°ú °Ç¹° ³»ºÎ¸¦ ´Ù¸¥ ¼­¹ö¿¡ ¶ç¿ì¸é ¸µÅ©°¡ ºÒ°¡´ÉÇÏ¹Ç·Î ´Ù¿îµÈ´Ù!
    for (i = 0; i < DEF_MAXMAPS; i++)
        if ((m_pMapList[i] != 0) && (m_pMapList[i]->m_iTotalStrikePoints != 0))
        {
            // ½ºÆ®¶óÀÌÅ© Æ÷ÀÎÆ®°¡ Á¤ÀÇµÈ ¸ÊÀÌ´Ù. °¢°¢ÀÇ ½ºÆ®¶óÀÌÅ© Æ÷ÀÎÆ®¿Í ¿¬°èµÈ ¸ÊÀÇ ÀÎµ¦½º¸¦ ±¸ÇØ ÀúÀåÇÑ´Ù.
            for (z = 0; z < DEF_MAXSTRIKEPOINTS; z++)
                if (strlen(m_pMapList[i]->m_stStrikePoint[z].cRelatedMapName) != 0)
                {
                    for (x = 0; x < DEF_MAXMAPS; x++)
                        if ((m_pMapList[x] != 0) && (strcmp(m_pMapList[x]->m_cName, m_pMapList[i]->m_stStrikePoint[z].cRelatedMapName) == 0))
                        {
                            // ÀÎµ¦½º¸¦ ±¸Çß´Ù. ÀúÀåÇÑ´Ù.
                            m_pMapList[i]->m_stStrikePoint[z].iMapIndex = x;
                            //testcode
                            wsprintf(G_cTxt, "(!) Map(%s) Strike Point(%d) Related Map(%s) Index(%d)", m_pMapList[i]->m_cName, z, m_pMapList[i]->m_stStrikePoint[z].cRelatedMapName, x);
                            log->info(G_cTxt);

                            goto LSPMI_LOOPBREAK;
                        }
                    LSPMI_LOOPBREAK:;
                }
        }
}

bool CGame::_bRegisterMap(char * pName)
{
    int i;
    char cTmpName[11], cTxt[120];

    memset(cTmpName, 0, sizeof(cTmpName));
    strcpy(cTmpName, pName);
    for (i = 0; i < DEF_MAXMAPS; i++)
        if ((m_pMapList[i] != 0) && (memcmp(m_pMapList[i]->m_cName, cTmpName, 10) == 0))
        {
            wsprintf(cTxt, "(!!!) CRITICAL ERROR! Map (%s) is already installed! cannot add.", cTmpName);
            log->info(cTxt);
            return false;
        }

    for (i = 0; i < DEF_MAXMAPS; i++)
        if (m_pMapList[i] == 0)
        {
            m_pMapList[i] = (class CMap *)new class CMap(this);
            wsprintf(cTxt, "(*) Add map (%s)   - Loading map info files...", pName);
            log->info(cTxt);
            if (m_pMapList[i]->bInit(pName) == false)
            {
                wsprintf(cTxt, "(!!!) Data file loading fail!", pName);
                log->info(cTxt);
                return false;
            };

            log->info("(*) Data file loading success.");

            if ((m_iMiddlelandMapIndex == -1) && (strcmp("middleland", pName) == 0))
                m_iMiddlelandMapIndex = i;

            if ((m_iAresdenMapIndex == -1) && (strcmp("aresden", pName) == 0))
                m_iAresdenMapIndex = i;

            if ((m_iElvineMapIndex == -1) && (strcmp("elvine", pName) == 0))
                m_iElvineMapIndex = i;

            if ((m_iBTFieldMapIndex == -1) && (strcmp("BtField", pName) == 0))
                m_iBTFieldMapIndex = i;

            if ((m_iGodHMapIndex == -1) && (strcmp("GodH", pName) == 0))
                m_iGodHMapIndex = i;

            m_iTotalMaps++;
            return true;
        }

    wsprintf(cTxt, "(!!!) CRITICAL ERROR! Map (%s) canot be added - no more map space.", pName);
    log->info(cTxt);
    return false;
}

bool CGame::bCheckIsItemUpgradeSuccess(int iClientH, int iItemIndex, int iSomH, bool bBonus)
{
    int iValue, iProb, iResult;

    if (m_pClientList[iClientH]->m_pItemList[iSomH] == 0) return false;

    iValue = (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x0F0000000) >> 28;

    switch (iValue)
    {
        case 0: iProb = 30; break;  // +1 :90%     +1~+2
        case 1: iProb = 25; break;  // +2 :80%      +3
        case 2: iProb = 20; break;  // +3 :48%      +4 
        case 3: iProb = 15; break;  // +4 :24%      +5
        case 4: iProb = 10; break;  // +5 :9.6%     +6
        case 5: iProb = 10; break;  // +6 :2.8%     +7
        case 6: iProb = 8; break;  // +7 :0.57%    +8
        case 7: iProb = 8; break;  // +8 :0.05%    +9
        case 8: iProb = 5; break;  // +9 :0.004%   +10
        case 9: iProb = 3; break;  // +10:0.00016%
        default: iProb = 1; break;
    }

    if (((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00000001) != 0) && (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 > 100))
    {
        if (iProb > 20)
            iProb += (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 / 10);
        else if (iProb > 7)
            iProb += (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 / 20);
        else
            iProb += (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 / 40);
    }
    if (bBonus == true) iProb *= 2;

    iProb *= 100;
    iResult = iDice(1, 10000);

    if (iProb >= iResult)
    {
        _bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);
        return true;
    }

    _bItemLog(DEF_ITEMLOG_UPGRADEFAIL, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);

    return false;
}

/*void CGame::AdminOrder_CleanMap(int iClientH, char * pData, uint32_t dwMsgSize)
{
    char   seps[] = "= \t\n";
    class  CStrTok * pStrTok;
    char   * token, cMapName[11], cBuff[256];
    bool bFlag = false;	//Used to check if we are on the map we wanna clear
    int i;
    CItem *pItem;
    short sRemainItemSprite, sRemainItemSpriteFrame, dX, dY;
    char cRemainItemColor, len;


    if (m_pClientList[iClientH] == nullptr) return;
    if ((dwMsgSize)	<= 0) return;

    if (m_pClientList[iClientH]->m_iAdminUserLevel < m_iAdminLevelCleanMap) {
        // Admin user levelÀÌ ³·¾Æ¼­ ÀÌ ±â´ÉÀ» »ç¿ëÇÒ ¼ö ¾ø´Ù.
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_ADMINUSERLEVELLOW, 0, 0, 0, 0);
        return;
    }

    memset(cBuff, 0, sizeof(cBuff));
    memcpy(cBuff, pData, dwMsgSize);

    pStrTok = new class CStrTok(cBuff, seps);
    token = pStrTok->pGet();
    token = pStrTok->pGet();

    if (token != 0) {
        memset(cMapName, 0, sizeof(cMapName));
        strcpy(cMapName, token);

        for (i = 0; i < DEF_MAXMAPS; i++)	//Enum all maps
            if (m_pMapList[i] != 0) {	//Is allocated map
                if (memcmp(m_pMapList[i]->m_cName, cMapName, 10) == 0) {	//is map same name
                    bFlag = true; //Set flag
                    //Get X and Y coords
                    int m_x = m_pMapList[i]->m_sSizeX;
                    int m_y = m_pMapList[i]->m_sSizeY;
                    for(int j = 1; j < m_x; j++)
                        for(int k = 1; k < m_y; k++){
                            do {	//Delete all items on current tile
                                pItem = m_pMapList[i]->pGetItem(j, k, &sRemainItemSprite, &sRemainItemSpriteFrame, &cRemainItemColor); // v1.4
                                if (pItem != 0) {
                                    delete pItem;	//Delete item;
                                }
                            } while(pItem != 0);
                        }
                    break;	//Break outo f loop
                }
            }

        if (!bFlag) {	//Notify GM he has to be on the map he clears
        }
        else{	//Notify GM that all items have been cleared
            for(int i = 1; i < DEF_MAXCLIENTS; i++){
                if (m_pClientList[i] != 0) {
                len = strlen(cMapName);
                if(len > 10) len = 10;
                if (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, cMapName, len) != 0) return;
                dX = m_pClientList[i]->m_sX;
                dY = m_pClientList[i]->m_sY;
                ZeroMemory(cMapName,sizeof(cMapName));
                strcpy(cMapName, m_pClientList[i]->m_cMapName);
                RequestTeleportHandler(i,"2   ", cMapName, dX, dY);
                }
            }
        }
    }

    delete pStrTok;
    return;
}*/

void CGame::ShowClientMsg(int iClientH, char * pMsg)
{
    char * cp, cTemp[256];
    uint32_t * dwp, dwMsgSize;
    uint16_t * wp;
    short * sp;

    memset(cTemp, 0, sizeof(cTemp));

    dwp = (uint32_t *)cTemp;
    *dwp = MSGID_COMMAND_CHATMSG;

    wp = (uint16_t *)(cTemp + DEF_INDEX2_MSGTYPE);
    *wp = 0;

    cp = (char *)(cTemp + DEF_INDEX2_MSGTYPE + 2);
    sp = (short *)cp;
    *sp = 0;
    cp += 2;

    sp = (short *)cp;
    *sp = 0;
    cp += 2;

    memcpy(cp, "HGServer", 8); // Player name :P
    cp += 10;

    *cp = 10; // chat type
    cp++;

    dwMsgSize = strlen(pMsg);
    if (dwMsgSize > 50) dwMsgSize = 50;
    memcpy(cp, pMsg, dwMsgSize);
    cp += dwMsgSize;

    m_pClientList[iClientH]->send_msg(cTemp, dwMsgSize + 22);
}

void CGame::Command_YellowBall(int iClientH, char * pData, uint32_t dwMsgSize)
{
    char   seps[] = "= \t\n";
    char * token, cBuff[256], cPlayerName[11], cMapName[32];
    class  CStrTok * pStrTok;
    char * cp;
    uint16_t * wp;
    int iSoxH, iSoX, i;

    if (m_pClientList[iClientH] == nullptr) return;
    if ((dwMsgSize) <= 0) return;

    iSoX = 0;
    for (i = 0; i < DEF_MAXITEMS; i++)
        if (m_pClientList[iClientH]->m_pItemList[i] != 0)
        {
            switch (m_pClientList[iClientH]->m_pItemList[i]->m_sIDnum)
            {
                case 653: iSoX++; iSoxH = i; break; // ½ºÅæ ¿Àºê Á¦¸®¸¶ 
            }
        }
    if (iSoX > 0)
    {

        memset(cPlayerName, 0, sizeof(cPlayerName));
        memset(cBuff, 0, sizeof(cBuff));
        memcpy(cBuff, pData, dwMsgSize);

        pStrTok = new class CStrTok(cBuff, seps);
        token = pStrTok->pGet();

        token = pStrTok->pGet();
        if (token == 0)
        {
            delete pStrTok;
            return;
        }

        if (strlen(token) > 10)
        {
            memcpy(cPlayerName, token, 10);
        }
        else
        {
            memcpy(cPlayerName, token, strlen(token));
        }

        for (int i = 1; i < DEF_MAXCLIENTS; i++)
        {
            if (m_pClientList[i] != 0)
            {
                if (memcmp(cPlayerName, m_pClientList[i]->m_cCharName, 10) == 0)
                {
                    if ((m_pClientList[iClientH]->m_cLocation) != (m_pClientList[i]->m_cLocation)) return;
                    ZeroMemory(cMapName, sizeof(cMapName));
                    strcpy(cMapName, m_pClientList[i]->m_cMapName);
                    wsprintf(G_cTxt, "(%s) Player: (%s) - YellowBall MapName(%s)(%d %d)", m_pClientList[i]->m_cIPaddress, m_pClientList[i]->m_cCharName, m_pClientList[i]->m_cMapName, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY);
                    PutItemLogFileList(G_cTxt);
                    ItemDepleteHandler(iClientH, iSoxH, true);
                    RequestTeleportHandler(iClientH, "2   ", cMapName, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY);
                    delete pStrTok;
                    return;
                }
            }
        }
        m_pClientList[iClientH]->m_bIsAdminOrderGoto = true;

        ZeroMemory(cBuff, sizeof(cBuff));

        cp = (char *)cBuff;
        *cp = GSM_REQUEST_FINDCHARACTER;
        cp++;

        wp = (uint16_t *)cp;
        *wp = m_wServerID_GSS;
        cp += 2;

        wp = (uint16_t *)cp;
        *wp = iClientH;
        cp += 2;

        memcpy(cp, cPlayerName, 10);
        cp += 10;

        memcpy(cp, m_pClientList[iClientH]->m_cCharName, 10);
        cp += 10;

        bStockMsgToGateServer(cBuff, 25);

        delete pStrTok;
    }
}

void CGame::Command_RedBall(int iClientH, char * pData, uint32_t dwMsgSize)
{
    char seps[] = "= \t\n", cName[21], cNpcName[21], cNpcWaypoint[11];
    int iNamingValue, tX, tY, i, x, iNpcID;
    int iSoxH, iSoX;

    if (m_pClientList[iClientH] == nullptr) return;
    if ((memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "huntzone1", 9) != 0) &&
        (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "huntzone2", 9) != 0) &&
        (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "huntzone3", 9) != 0) &&
        (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "huntzone4", 9) != 0)) return;

    iSoX = 0;
    for (i = 0; i < DEF_MAXITEMS; i++)
        if (m_pClientList[iClientH]->m_pItemList[i] != 0)
        {
            switch (m_pClientList[iClientH]->m_pItemList[i]->m_sIDnum)
            {
                case 652: iSoX++; iSoxH = i; break; // ½ºÅæ ¿Àºê Á¦¸®¸¶ 
            }
        }
    if (iSoX > 0)
    {
        iNamingValue = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->iGetEmptyNamingValue();
        if (iNamingValue == -1)
        {

        }
        else
        {

            memset(cNpcName, 0, sizeof(cNpcName));
            switch (iDice(1, 5))
            {
                case 1: strcpy(cNpcName, "Wyvern"); iNpcID = 66; break;
                case 2: strcpy(cNpcName, "Hellclaw"); iNpcID = 49; break;
                case 3: strcpy(cNpcName, "Fire-Wyvern"); iNpcID = 73; break;
                case 4: strcpy(cNpcName, "Tigerworm"); iNpcID = 50; break;
                case 5: strcpy(cNpcName, "Gagoyle"); iNpcID = 52; break;
            }
            memset(cName, 0, sizeof(cName));
            wsprintf(cName, "XX%d", iNamingValue);
            cName[0] = '_';
            cName[1] = m_pClientList[iClientH]->m_cMapIndex + 65;

            memset(cNpcWaypoint, 0, sizeof(cNpcWaypoint));

            tX = (int)m_pClientList[iClientH]->m_sX;
            tY = (int)m_pClientList[iClientH]->m_sY;
            if (bCreateNewNpc(cNpcName, cName, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, 0, (rand() % 9),
                DEF_MOVETYPE_RANDOM, &tX, &tY, cNpcWaypoint, 0, 0, -1, false, false) == false)
            {
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetNamingValueEmpty(iNamingValue);
            }
            else
            {
                wsprintf(G_cTxt, "(%s) Player: (%s) - RedBall used %s [%s(%d, %d)] ", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName, cNpcName, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, tX, tY);
                PutItemLogFileList(G_cTxt);
            }
        }

        for (x = 1; x < DEF_MAXCLIENTS; x++)
            if ((m_pClientList[x] != 0) && (m_pClientList[x]->m_bIsInitComplete == true))
            {
                SendNotifyMsg(0, x, DEF_NOTIFY_SPAWNEVENT, tX, tY, iNpcID, 0, 0, 0);
            }
        ItemDepleteHandler(iClientH, iSoxH, true);
    }
}

void CGame::Command_BlueBall(int iClientH, char * pData, uint32_t dwMsgSize)

{
    char seps[] = "= \t\n";
    char   cName_Master[10], cName_Slave[10], cNpcName[256], cWaypoint[11], cSA;
    int    pX, pY, j, iNum, iNamingValue, iNpcID;
    int i, x;
    bool   bMaster;
    int iSoxH, iSoX;

    if (m_pClientList[iClientH] == nullptr) return;
    if ((memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "huntzone1", 9) != 0) &&
        (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "huntzone2", 9) != 0) &&
        (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "huntzone3", 9) != 0) &&
        (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "huntzone4", 9) != 0)) return;


    iSoX = 0;
    for (i = 0; i < DEF_MAXITEMS; i++)
        if (m_pClientList[iClientH]->m_pItemList[i] != 0)
        {
            switch (m_pClientList[iClientH]->m_pItemList[i]->m_sIDnum)
            {
                case 654: iSoX++; iSoxH = i; break; // ½ºÅæ ¿Àºê Á¦¸®¸¶ 
            }
        }
    if (iSoX > 0)
    {
        iNamingValue = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->iGetEmptyNamingValue();
        if (iNamingValue == -1)
        {

        }
        else
        {

            memset(cNpcName, 0, sizeof(cNpcName));

            switch (iDice(1, 38))
            {
                case 1: strcpy(cNpcName, "Slime");			iNpcID = 10; break;
                case 2: strcpy(cNpcName, "Giant-Ant");		iNpcID = 15; break;
                case 3: strcpy(cNpcName, "Zombie");			iNpcID = 17; break;
                case 4: strcpy(cNpcName, "Scorpion");		iNpcID = 16; break;
                case 5: strcpy(cNpcName, "Skeleton");		iNpcID = 11; break;
                case 6: strcpy(cNpcName, "Orc-Mage");		iNpcID = 14; break;
                case 7: strcpy(cNpcName, "Clay-Golem");		iNpcID = 23; break;
                case 8: strcpy(cNpcName, "Stone-Golem");	iNpcID = 12; break;
                case 9: strcpy(cNpcName, "Hellbound");		iNpcID = 27; break;
                case 10: strcpy(cNpcName, "Giant-Frog");	iNpcID = 57; break;
                case 11: strcpy(cNpcName, "Troll");			iNpcID = 28; break;
                case 12: strcpy(cNpcName, "Cyclops");		iNpcID = 13; break;
                case 13: strcpy(cNpcName, "Ice-Golem");		iNpcID = 65; break;
                case 14: strcpy(cNpcName, "Beholder");		iNpcID = 53; break;
                case 15: strcpy(cNpcName, "Cannibal-Plant"); iNpcID = 60; break;
                case 16: strcpy(cNpcName, "Orge");			iNpcID = 29; break;
                case 17: strcpy(cNpcName, "Mountain-Giant"); iNpcID = 58; break;
                case 18: strcpy(cNpcName, "DireBoar");		iNpcID = 62; break;
                case 19: strcpy(cNpcName, "Liche");			iNpcID = 30; break;
                case 20: strcpy(cNpcName, "Stalker");		iNpcID = 48; break;
                case 21: strcpy(cNpcName, "WereWolf");		iNpcID = 33; break;
                case 22: strcpy(cNpcName, "Dark-Elf");		iNpcID = 54; break;
                case 23: strcpy(cNpcName, "Frost");			iNpcID = 63; break;
                case 24: strcpy(cNpcName, "Orc");			iNpcID = 14; break;
                case 25: strcpy(cNpcName, "Ettin");			iNpcID = 59; break;
                case 26: strcpy(cNpcName, "Tentocle");		iNpcID = 80; break;
                case 27: strcpy(cNpcName, "Giant-Crayfish"); iNpcID = 74; break;
                case 28: strcpy(cNpcName, "Giant-Plant");	iNpcID = 76; break;
                case 29: strcpy(cNpcName, "Rudolph");		iNpcID = 61; break;
                case 30: strcpy(cNpcName, "Claw-Turtle");	iNpcID = 72; break;
                case 31: strcpy(cNpcName, "Centaurus");		iNpcID = 71; break;
                case 32: strcpy(cNpcName, "Barlog");		iNpcID = 70; break;
                case 33: strcpy(cNpcName, "Giant-Lizard");	iNpcID = 75; break;
                case 34: strcpy(cNpcName, "MasterMage-Orc"); iNpcID = 77; break;
                case 35: strcpy(cNpcName, "Minotaurs");		iNpcID = 78; break;
                case 36: strcpy(cNpcName, "Unicorn");		iNpcID = 32; break;
                case 37: strcpy(cNpcName, "Nizie");			iNpcID = 79; break;
            }

            iNum = 10;
            cSA = 0;
            pX = m_pClientList[iClientH]->m_sX;
            pY = m_pClientList[iClientH]->m_sY;

            wsprintf(G_cTxt, "(!) BlueBallEvent: SummonMob (%s)-(%d)", cNpcName, iNum);
            log->info(G_cTxt);

            iNamingValue = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->iGetEmptyNamingValue();
            if (iNamingValue != -1)
            {

                memset(cName_Master, 0, sizeof(cName_Master));
                wsprintf(cName_Master, "XX%d", iNamingValue);
                cName_Master[0] = '_';
                cName_Master[1] = m_pClientList[iClientH]->m_cMapIndex + 65;

                if ((bMaster = bCreateNewNpc(cNpcName, cName_Master, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, (rand() % 3), cSA, DEF_MOVETYPE_RANDOM, &pX, &pY, cWaypoint, 0, 0, -1, false, false, false, true)) == false)
                {

                    m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetNamingValueEmpty(iNamingValue);
                }
            }

            for (j = 0; j < (iNum - 1); j++)
            {
                iNamingValue = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->iGetEmptyNamingValue();
                if (iNamingValue != -1)
                {
                    // Slave Mob
                    memset(cName_Slave, 0, sizeof(cName_Slave));
                    wsprintf(cName_Slave, "XX%d", iNamingValue);
                    cName_Slave[0] = '_';
                    cName_Slave[1] = m_pClientList[iClientH]->m_cMapIndex + 65;



                    if (bCreateNewNpc(cNpcName, cName_Slave, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, (rand() % 3), cSA, DEF_MOVETYPE_RANDOM, &pX, &pY, cWaypoint, 0, 0, -1, false, false, false) == false)
                    {

                        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetNamingValueEmpty(iNamingValue);
                    }
                    else
                    {
                        // Slave
                        bSetNpcFollowMode(cName_Slave, cName_Master, DEF_OWNERTYPE_NPC);
                    }
                }
            }

            //delete pStrTok;

        }
    }

    for (x = 1; x < DEF_MAXCLIENTS; x++)
        if ((m_pClientList[x] != 0) && (m_pClientList[x]->m_bIsInitComplete == true))
        {
            SendNotifyMsg(0, x, DEF_NOTIFY_SPAWNEVENT, pX, pY, iNpcID, 0, 0, 0);
        }
    ItemDepleteHandler(iClientH, iSoxH, true);
}

/*
at the end of client connection have a true switch
at the start of client move handler check if the switch is true
if it is not true add 1 warning, if the warning reaches 3
delete client and log him, if the true switch
*/
//and when a client walks into a map with dynamic portal
//[KLKS] - [Pretty Good Coders] says:
//u gotta inform it
//[KLKS] - [Pretty Good Coders] says:
//or else they wont see it

/*void CGame::OpenApocalypseGate(int iClientH)
{
    if (m_pClientList[iClientH] == nullptr) return;

    //m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_iTotalAliveObject;
    SendNotifyMsg(0, iClientH, DEF_NOTIFY_APOCGATEOPEN, 95, 31, 0, m_pClientList[iClientH]->m_cMapName);
}*/

void CGame::GlobalUpdateConfigs(char cConfigType)
{
    char * cp, cData[120];

    memset(cData, 0, sizeof(cData));
    cp = (char *)cData;
    *cp = GSM_UPDATECONFIGS;
    cp++;

    *cp = (char)cConfigType;
    cp++;

    LocalUpdateConfigs(cConfigType);

    bStockMsgToGateServer(cData, 5);
}

void CGame::LocalUpdateConfigs(char cConfigType)
{
    if (cConfigType == 1)
    {
        bReadSettingsConfigFile("..\\GameConfigs\\Settings.cfg");
        log->info("(!!!) Settings.cfg updated successfully!");
    }
    if (cConfigType == 2)
    {
        bReadAdminListConfigFile("..\\GameConfigs\\AdminList.cfg");
        log->info("(!!!) AdminList.cfg updated successfully!");
    }
    if (cConfigType == 3)
    {
        bReadBannedListConfigFile("..\\GameConfigs\\BannedList.cfg");
        log->info("(!!!) BannedList.cfg updated successfully!");
    }
    if (cConfigType == 4)
    {
        bReadAdminSetConfigFile("..\\GameConfigs\\AdminSettings.cfg");
        log->info("(!!!) AdminSettings.cfg updated successfully!");
    }
}

/*void CGame::ApocalypseStarter()
{
 SYSTEMTIME SysTime;
 int i;

    if (m_bIsApocalypseMode == true) return;
    if (m_bIsApocalypseStarter == false) return;

    GetLocalTime(&SysTime);

    for (i = 0; i < DEF_MAXAPOCALYPSE; i++)
    if	((m_stApocalypseScheduleStart[i].iDay == SysTime.wDayOfWeek) &&
        (m_stApocalypseScheduleStart[i].iHour == SysTime.wHour) &&
        (m_stApocalypseScheduleStart[i].iMinute == SysTime.wMinute)) {
            log->info("(!) Automated apocalypse is initiated!");
            GlobalStartApocalypseMode();
            return;
    }
}*/

void CGame::TimeHitPointsUp(int iClientH)
{
    int iMaxHP, iTemp, iTotal;
    double dV1, dV2, dV3;

    if (m_pClientList[iClientH] == nullptr) return;

    if (m_pClientList[iClientH]->m_bIsInitComplete == false) return;

    if (m_pClientList[iClientH]->m_iHungerStatus <= 0) return;

    if (m_pClientList[iClientH]->m_bIsKilled == true) return;

    if (m_pClientList[iClientH]->m_bSkillUsingStatus[19] == true) return;

    iMaxHP = iGetMaxHP(iClientH);

    if (m_pClientList[iClientH]->m_iHP < iMaxHP)
    {

        iTemp = iDice(1, (m_pClientList[iClientH]->m_iVit));

        if (iTemp < (m_pClientList[iClientH]->m_iVit / 2)) iTemp = (m_pClientList[iClientH]->m_iVit / 2);

        if (m_pClientList[iClientH]->m_iSideEffect_MaxHPdown != 0)

            iTemp -= (iTemp / m_pClientList[iClientH]->m_iSideEffect_MaxHPdown);

        iTotal = iTemp + m_pClientList[iClientH]->m_iHPstock;

        if (m_pClientList[iClientH]->m_iAddHP != 0)
        {
            dV2 = (double)iTotal;
            dV3 = (double)m_pClientList[iClientH]->m_iAddHP;
            dV1 = (dV3 / 100.0f) * dV2;
            iTotal += (int)dV1;
        }

        m_pClientList[iClientH]->m_iHP += iTotal;

        if (m_pClientList[iClientH]->m_iHP > iMaxHP) m_pClientList[iClientH]->m_iHP = iMaxHP;

        if (m_pClientList[iClientH]->m_iHP <= 0) m_pClientList[iClientH]->m_iHP = 0;
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_HP, 0, 0, 0, 0);
    }
    m_pClientList[iClientH]->m_iHPstock = 0;
}

char CGame::_cCheckHeroItemEquipped(int iClientH)
{
    short sHeroLeggings, sHeroHauberk, sHeroArmor, sHeroHelm;

    if (m_pClientList[iClientH] == nullptr) return 0;

    sHeroHelm = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_HEAD];
    sHeroArmor = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_BODY];
    sHeroHauberk = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_ARMS];
    sHeroLeggings = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_PANTS];

    if ((sHeroHelm < 0) || (sHeroLeggings < 0) || (sHeroArmor < 0) || (sHeroHauberk < 0)) return 0;

    if (m_pClientList[iClientH]->m_pItemList[sHeroHelm] == 0) return 0;
    if (m_pClientList[iClientH]->m_pItemList[sHeroLeggings] == 0) return 0;
    if (m_pClientList[iClientH]->m_pItemList[sHeroArmor] == 0) return 0;
    if (m_pClientList[iClientH]->m_pItemList[sHeroHauberk] == 0) return 0;

    if ((m_pClientList[iClientH]->m_pItemList[sHeroHelm]->m_sIDnum == 403) &&
        (m_pClientList[iClientH]->m_pItemList[sHeroArmor]->m_sIDnum == 411) &&
        (m_pClientList[iClientH]->m_pItemList[sHeroHauberk]->m_sIDnum == 419) &&
        (m_pClientList[iClientH]->m_pItemList[sHeroLeggings]->m_sIDnum == 423)) return 1;

    if ((m_pClientList[iClientH]->m_pItemList[sHeroHelm]->m_sIDnum == 407) &&
        (m_pClientList[iClientH]->m_pItemList[sHeroArmor]->m_sIDnum == 415) &&
        (m_pClientList[iClientH]->m_pItemList[sHeroHauberk]->m_sIDnum == 419) &&
        (m_pClientList[iClientH]->m_pItemList[sHeroLeggings]->m_sIDnum == 423)) return 2;

    if ((m_pClientList[iClientH]->m_pItemList[sHeroHelm]->m_sIDnum == 404) &&
        (m_pClientList[iClientH]->m_pItemList[sHeroArmor]->m_sIDnum == 412) &&
        (m_pClientList[iClientH]->m_pItemList[sHeroHauberk]->m_sIDnum == 420) &&
        (m_pClientList[iClientH]->m_pItemList[sHeroLeggings]->m_sIDnum == 424)) return 1;

    if ((m_pClientList[iClientH]->m_pItemList[sHeroHelm]->m_sIDnum == 408) &&
        (m_pClientList[iClientH]->m_pItemList[sHeroArmor]->m_sIDnum == 416) &&
        (m_pClientList[iClientH]->m_pItemList[sHeroHauberk]->m_sIDnum == 420) &&
        (m_pClientList[iClientH]->m_pItemList[sHeroLeggings]->m_sIDnum == 424)) return 2;

    if ((m_pClientList[iClientH]->m_pItemList[sHeroHelm]->m_sIDnum == 405) &&
        (m_pClientList[iClientH]->m_pItemList[sHeroArmor]->m_sIDnum == 413) &&
        (m_pClientList[iClientH]->m_pItemList[sHeroHauberk]->m_sIDnum == 421) &&
        (m_pClientList[iClientH]->m_pItemList[sHeroLeggings]->m_sIDnum == 425)) return 1;

    if ((m_pClientList[iClientH]->m_pItemList[sHeroHelm]->m_sIDnum == 409) &&
        (m_pClientList[iClientH]->m_pItemList[sHeroArmor]->m_sIDnum == 417) &&
        (m_pClientList[iClientH]->m_pItemList[sHeroHauberk]->m_sIDnum == 421) &&
        (m_pClientList[iClientH]->m_pItemList[sHeroLeggings]->m_sIDnum == 425)) return 2;

    if ((m_pClientList[iClientH]->m_pItemList[sHeroHelm]->m_sIDnum == 406) &&
        (m_pClientList[iClientH]->m_pItemList[sHeroArmor]->m_sIDnum == 414) &&
        (m_pClientList[iClientH]->m_pItemList[sHeroHauberk]->m_sIDnum == 422) &&
        (m_pClientList[iClientH]->m_pItemList[sHeroLeggings]->m_sIDnum == 426)) return 1;

    if ((m_pClientList[iClientH]->m_pItemList[sHeroHelm]->m_sIDnum == 410) &&
        (m_pClientList[iClientH]->m_pItemList[sHeroArmor]->m_sIDnum == 418) &&
        (m_pClientList[iClientH]->m_pItemList[sHeroHauberk]->m_sIDnum == 422) &&
        (m_pClientList[iClientH]->m_pItemList[sHeroLeggings]->m_sIDnum == 426)) return 2;

    return 0;
}

bool CGame::bPlantSeedBag(int iMapIndex, int dX, int dY, int iItemEffectValue1, int iItemEffectValue2, int iClientH)
{
    int iNamingValue, tX, tY;
    short sOwnerH;
    char cOwnerType, cNpcName[21], cName[21], cNpcWaypointIndex[11];
    bool bRet;

    if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_iTotalAgriculture >= 200)
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_NOMOREAGRICULTURE, 0, 0, 0, 0);
        return false;
    }

    if (iItemEffectValue2 > m_pClientList[iClientH]->m_cSkillMastery[2])
    {
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_AGRICULTURESKILLLIMIT, 0, 0, 0, 0);
        return false;
    }

    iNamingValue = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->iGetEmptyNamingValue();

    if (iNamingValue == -1)
    {
    }
    else
    {
        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
        if (sOwnerH != 0 && sOwnerH == DEF_OWNERTYPE_NPC && m_pNpcList[sOwnerH]->m_cActionLimit == 5)
        {
            SendNotifyMsg(0, iClientH, DEF_NOTIFY_AGRICULTURENOAREA, 0, 0, 0, 0);
            return false;
        }
        else
        {
            if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bGetIsFarm(dX, dY) == false)
            {
                SendNotifyMsg(0, iClientH, DEF_NOTIFY_AGRICULTURENOAREA, 0, 0, 0, 0);
                return false;
            }

            memset(cNpcName, 0, sizeof(cNpcName));
            strcpy(cNpcName, "Crops");
            memset(cName, 0, sizeof(cName));
            wsprintf(cName, "XX%d", iNamingValue);
            cName[0] = '_';
            cName[1] = iMapIndex + 65;

            ZeroMemory(cNpcWaypointIndex, sizeof(cNpcWaypointIndex));
            tX = dX;
            tY = dY;

            bRet = bCreateNewNpc(cNpcName, cName, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, 0, 0, DEF_MOVETYPE_RANDOM, &tX, &tY, cNpcWaypointIndex, 0, 0, 0, false, true);
            if (bRet == false)
            {
                m_pMapList[iMapIndex]->SetNamingValueEmpty(iNamingValue);
            }
            else
            {
                m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
                if (m_pNpcList[sOwnerH] == 0) return 0;
                m_pNpcList[sOwnerH]->m_cCropType = iItemEffectValue1;
                switch (iItemEffectValue1)
                {
                    case 1: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
                    case 2: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
                    case 3: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
                    case 4: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
                    case 5: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
                    case 6: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
                    case 7: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
                    case 8: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
                    case 9: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
                    case 10: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
                    case 11: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
                    case 12: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
                    case 13: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
                    default: m_pNpcList[sOwnerH]->m_cCropSkill = 100; break;
                }
                m_pNpcList[sOwnerH]->m_sAppr2 = 1;
                SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_LOG, DEF_MSGTYPE_CONFIRM, 0, 0, 0);
                wsprintf(G_cTxt, "(skill:%d type:%d)plant(%s) Agriculture begin(%d,%d) sum(%d)!", m_pNpcList[sOwnerH]->m_cCropSkill, m_pNpcList[sOwnerH]->m_cCropType, cNpcName, tX, tY, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_iTotalAgriculture);
                log->info(G_cTxt);
                return true;
            }
        }
    }
    return false;
}

void CGame::_CheckFarmingAction(short sAttackerH, short sTargetH, bool bType)
{
    char cCropType;
    int iItemID;
    class CItem * pItem;

    iItemID = 0;
    cCropType = 0;

    cCropType = m_pNpcList[sTargetH]->m_cCropType;
    switch (cCropType)
    {
        case 1: GetExp(sAttackerH, iDice(3, 10)); iItemID = 820; break; // WaterMelon
        case 2: GetExp(sAttackerH, iDice(3, 10)); iItemID = 821; break; // Pumpkin
        case 3: GetExp(sAttackerH, iDice(4, 10)); iItemID = 822; break; // Garlic
        case 4: GetExp(sAttackerH, iDice(4, 10)); iItemID = 823; break; // Barley
        case 5: GetExp(sAttackerH, iDice(5, 10)); iItemID = 824; break; // Carrot
        case 6: GetExp(sAttackerH, iDice(5, 10)); iItemID = 825; break; // Radish
        case 7: GetExp(sAttackerH, iDice(6, 10)); iItemID = 826; break; // Corn
        case 8: GetExp(sAttackerH, iDice(6, 10)); iItemID = 827; break; // ChineseBellflower
        case 9: GetExp(sAttackerH, iDice(7, 10)); iItemID = 828; break; // Melone
        case 10: GetExp(sAttackerH, iDice(7, 10)); iItemID = 829; break; // Tommato
        case 11: GetExp(sAttackerH, iDice(8, 10)); iItemID = 830; break; // Grapes
        case 12: GetExp(sAttackerH, iDice(8, 10)); iItemID = 831; break; // BlueGrapes
        case 13: GetExp(sAttackerH, iDice(9, 10)); iItemID = 832; break; // Mushroom
        default: GetExp(sAttackerH, iDice(10, 10)); iItemID = 721; break; // Ginseng

    }

    pItem = new class CItem;
    if (_bInitItemAttr(pItem, iItemID) == false)
    {
        delete pItem;
    }
    if (bType == 0)
    {
        m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->bSetItem(m_pClientList[sAttackerH]->m_sX, m_pClientList[sAttackerH]->m_sY, pItem);
        SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[sAttackerH]->m_cMapIndex,
            m_pClientList[sAttackerH]->m_sX, m_pClientList[sAttackerH]->m_sY, pItem->m_sSprite,
            pItem->m_sSpriteFrame, pItem->m_cItemColor, false);
    }
    else if (bType == 1)
    {
        m_pMapList[m_pNpcList[sTargetH]->m_cMapIndex]->bSetItem(m_pNpcList[sTargetH]->m_sX, m_pNpcList[sTargetH]->m_sY, pItem);
        SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pNpcList[sTargetH]->m_cMapIndex,
            m_pNpcList[sTargetH]->m_sX, m_pNpcList[sTargetH]->m_sY, pItem->m_sSprite,
            pItem->m_sSpriteFrame, pItem->m_cItemColor, false);
    }

}

/*void CGame::CalculateEnduranceDecrement(short sTargetH, short sAttackerH, char cTargetType, int iArmorType)
{
 short sItemIndex;
 int iDownValue = 1, iHammerChance = 100;

    if (m_pClientList[sTargetH] == 0) return;

    if ((cTargetType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH] != 0 )) {
        if ((cTargetType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sTargetH]->m_cSide != m_pClientList[sAttackerH]->m_cSide)) {
            switch (m_pClientList[sAttackerH]->m_sUsingWeaponSkill) {
                case 14:
                    if ((31 == ((m_pClientList[sAttackerH]->m_sAppr2  & 0x0FF0) >> 4)) || (32 == ((m_pClientList[sAttackerH]->m_sAppr2  & 0x0FF0) >> 4))) {
                        sItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
                        if ((sItemIndex != -1) && (m_pClientList[sAttackerH]->m_pItemList[sItemIndex] != 0)) {
                            if (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 761) { // BattleHammer
                                iDownValue = 30;
                                break;
                            }
                            if (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 762) { // GiantBattleHammer
                                iDownValue = 35;
                                break;
                            }
                            if (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 843) { // BarbarianHammer
                                iDownValue = 30;
                                break;
                            }
                            if (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 745) { // BarbarianHammer
                                iDownValue = 30;
                                break;
                            }
                        }
                    }
                    else {
                        iDownValue = 20; break;
                    }
                case 10: iDownValue = 3; break;
                default: iDownValue = 1; break;
                }

                if (m_pClientList[sTargetH]->m_bIsSpecialAbilityEnabled == true) {
                    switch (m_pClientList[sTargetH]->m_iSpecialAbilityType)
                        case 52: iDownValue = 0; iHammerChance = 0;
                }
            }
        }

        if ((m_pClientList[sTargetH]->m_cSide != 0) && (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan > 0)) {
                m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan -= iDownValue;
        }

        if ((m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan <= 0) || (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan > 64000)) {
            m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan = 0;
            SendNotifyMsg(0, sTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_cEquipPos, iArmorType, 0, 0);
            ReleaseItemHandler(sTargetH, iArmorType, true);
            return;
        }

    /*try
    {
        if (m_pClientList[sAttackerH] != 0) {
            if (cTargetType == DEF_OWNERTYPE_PLAYER) {
                sItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
                if ((sItemIndex != -1) && (m_pClientList[sAttackerH]->m_pItemList[sItemIndex] != 0)) {
                    if ((m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 617) || (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 618) || (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 619) || (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 873) || (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 874) || (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 75) || (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 76)) {
                        m_pClientList[sAttackerH]->m_sUsingWeaponSkill = 6;
                        return;
                    }
                }
            }
        }*/

        /*if (m_pClientList[sAttackerH] != 0) {
        if (cTargetType == DEF_OWNERTYPE_PLAYER) {
        if ((m_pClientList[sAttackerH]->m_sUsingWeaponSkill == 14) && (iHammerChance == 100)) {
            if (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wMaxLifeSpan < 2000) {
                iHammerChance = iDice(6, (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wMaxLifeSpan - m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan));
            }
            else {
                iHammerChance = iDice(4, (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wMaxLifeSpan - m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan));
            }

            if ((31 == ((m_pClientList[sAttackerH]->m_sAppr2  & 0x0FF0) >> 4)) || (32 == ((m_pClientList[sAttackerH]->m_sAppr2  & 0x0FF0) >> 4))) {
                sItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
                if ((sItemIndex != -1) && (m_pClientList[sAttackerH]->m_pItemList[sItemIndex] != 0)) {
                    if (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 761) { // BattleHammer
                        iHammerChance = iHammerChance/2;
                    }
                    if (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 762) { // GiantBattleHammer
                        iHammerChance = ((iHammerChance*10)/9);
                    }
                    if (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 843) { // GiantBattleHammer
                        iHammerChance = ((iHammerChance*10)/9);
                    }
                    if (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 745) { // GiantBattleHammer
                        iHammerChance = ((iHammerChance*10)/9);
                    }
                }
            }
            if ((m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_sIDnum == 622) || (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_sIDnum == 621)) {
                iHammerChance = 0;
            }
            if (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan < iHammerChance) {
                wsprintf(G_cTxt, "(iHammerChance (%d), target armor endurance (%d)!", iHammerChance, m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan);
                log->info(G_cTxt);
                ReleaseItemHandler(sTargetH, iArmorType, true);
                SendNotifyMsg(0, sTargetH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_cEquipPos, iArmorType, 0, 0);
                return;
            }
        }
        }
        }
    //}
    //catch(...)
    {

    }
}*/

bool CGame::bCalculateEnduranceDecrement(short sTargetH, short sAttackerH, char cTargetType, int iArmorType)
{
    int iDownValue = 1, iHammerChance = 100, iItemIndex;
    uint16_t wWeaponType;

    if (m_pClientList[sTargetH] == 0) return false;
    if ((cTargetType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH] == 0)) return false;
    wWeaponType = ((m_pClientList[sAttackerH]->m_sAppr2 & 0x0FF0) >> 4);
    if ((cTargetType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sTargetH]->m_cSide != m_pClientList[sAttackerH]->m_cSide))
    {
        switch (m_pClientList[sAttackerH]->m_sUsingWeaponSkill)
        {
            case 14:
                if ((wWeaponType == 31) || (wWeaponType == 32))
                {
                    iItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
                    if ((iItemIndex != -1) && (m_pClientList[sAttackerH]->m_pItemList[iItemIndex] != 0))
                    {
                        if (m_pClientList[sAttackerH]->m_pItemList[iItemIndex]->m_sIDnum == 761)
                        { // BattleHammer 
                            iDownValue = 30;
                        }
                        if (m_pClientList[sAttackerH]->m_pItemList[iItemIndex]->m_sIDnum == 762)
                        { // GiantBattleHammer
                            iDownValue = 35;
                        }
                        if (m_pClientList[sAttackerH]->m_pItemList[iItemIndex]->m_sIDnum == 843)
                        { // BarbarianHammer
                            iDownValue = 30;
                        }
                        if (m_pClientList[sAttackerH]->m_pItemList[iItemIndex]->m_sIDnum == 745)
                        { // MasterBattleHammer
                            iDownValue = 30;
                        }
                        break;
                    }
                }
                else
                {
                    iDownValue = 20;
                }
                break;
            case 10:
                iDownValue = 3;
                break;
            default:
                iDownValue = 1;
        }
    }
    if (m_pClientList[sTargetH]->m_bIsSpecialAbilityEnabled == true)
    {
        switch (m_pClientList[sTargetH]->m_iSpecialAbilityType)
        case 52:
            iDownValue = 0;
            iHammerChance = 0;
    }
    if ((m_pClientList[sTargetH]->m_cSide != 0) && (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan > 0))
    {
        m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan -= iDownValue;
    }
    if (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan <= 0)
    {
        m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan = 0;
        SendNotifyMsg(0, sTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_cEquipPos, iArmorType, 0, 0);
        ReleaseItemHandler(sTargetH, iArmorType, true);
        return true;
    }
    if ((cTargetType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH]->m_sUsingWeaponSkill == 14) && (iHammerChance == 100))
    {
        if (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wMaxLifeSpan < 2000)
        {
            iHammerChance = iDice(6, (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wMaxLifeSpan - m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan));
        }
        else
        {
            iHammerChance = iDice(4, (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wMaxLifeSpan - m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan));
        }
        if ((wWeaponType == 31) || (wWeaponType == 32))
        {
            iItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
            if ((iItemIndex != -1) && (m_pClientList[sAttackerH]->m_pItemList[iItemIndex] != 0))
            {
                if (m_pClientList[sAttackerH]->m_pItemList[iItemIndex]->m_sIDnum == 761)
                { // BattleHammer 
                    iHammerChance -= iHammerChance >> 1;
                }
                if (m_pClientList[sAttackerH]->m_pItemList[iItemIndex]->m_sIDnum == 762)
                { // GiantBattleHammer
                    iHammerChance = (((iHammerChance * 5) + 7) >> 3);
                }
                if (m_pClientList[sAttackerH]->m_pItemList[iItemIndex]->m_sIDnum == 843)
                { // BarbarianHammer
                    iHammerChance = (((iHammerChance * 5) + 7) >> 3);
                }
                if (m_pClientList[sAttackerH]->m_pItemList[iItemIndex]->m_sIDnum == 745)
                { // MasterBattleHammer
                    iHammerChance = (((iHammerChance * 5) + 7) >> 3);
                }
            }
            else
            {
                iHammerChance = ((iHammerChance + 3) >> 2);
            }
            switch (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_sIDnum)
            {
                case 621:
                case 622:
                    iHammerChance = 0;
            }
            if (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan < iHammerChance)
            {
                ReleaseItemHandler(sTargetH, iArmorType, true);
                SendNotifyMsg(0, sTargetH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_cEquipPos, iArmorType, 0, 0);
            }
        }
    }
    return true;
}

int CGame::iCalculateAttackEffect(short sTargetH, char cTargetType, short sAttackerH, char cAttackerType, int tdX, int tdY, int iAttackMode, bool bNearAttack, bool bIsDash, bool bArrowUse)
{
    int    iAP_SM, iAP_L, iAttackerHitRatio, iTargetDefenseRatio, iDestHitRatio, iResult, iAP_Abs_Armor, iAP_Abs_Shield;
    char   cAttackerName[21], cAttackerDir, cAttackerSide, cTargetDir, cProtect, cCropSkill, cFarmingSkill;
    short  sWeaponIndex, sAttackerWeapon, dX, dY, sX, sY, sAtkX, sAtkY, sTgtX, sTgtY;
    uint32_t  dwTime;
    uint16_t   wWeaponType;
    double dTmp1, dTmp2, dTmp3;
    bool   bKilled;
    bool   bNormalMissileAttack;
    bool   bIsAttackerBerserk;
    int    iKilledDice, iDamage, iExp, iWepLifeOff, iSideCondition, iMaxSuperAttack, iWeaponSkill, iComboBonus, iTemp;
    int    iAttackerHP, iMoveDamage, iRepDamage;
    char   cAttackerSA;
    int    iAttackerSAvalue, iHitPoint;
    char   cDamageMoveDir;
    int    iPartyID, iConstructionPoint, iWarContribution, tX, tY, iDst1, iDst2;
    short sItemIndex;
    short sSkillUsed;

    dwTime = timeGetTime();
    bKilled = false;
    iExp = 0;
    iPartyID = 0;
    bNormalMissileAttack = false;
    memset(cAttackerName, 0, sizeof(cAttackerName));
    cAttackerSA = 0;
    iAttackerSAvalue = 0;
    wWeaponType = 0;

    switch (cAttackerType)
    {
        case DEF_OWNERTYPE_PLAYER:

            if (m_pClientList[sAttackerH] == 0) return 0;
            if ((m_bAdminSecurity == true) && (m_pClientList[sAttackerH]->m_iAdminUserLevel > 0)) return 0;
            if ((m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_bIsAttackEnabled == false) && (m_pClientList[sAttackerH]->m_iAdminUserLevel == 0)) return 0;
            if ((m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex] == 0) && (m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_bIsHeldenianMap == true) && (m_bIsHeldenianMode == true)) return 0;
            if ((m_bIsCrusadeMode == false) && (m_pClientList[sAttackerH]->m_bIsPlayerCivil == true) && (cTargetType == DEF_OWNERTYPE_PLAYER)) return 0;

            if ((m_pClientList[sAttackerH]->m_iStatus & 0x10) != 0)
            {
                SetInvisibilityFlag(sAttackerH, DEF_OWNERTYPE_PLAYER, false);
                bRemoveFromDelayEventList(sAttackerH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
                m_pClientList[sAttackerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = 0;
            }

            if ((m_pClientList[sAttackerH]->m_sAppr2 & 0xF000) == 0) return 0;

            iAP_SM = 0;
            iAP_L = 0;

            wWeaponType = ((m_pClientList[sAttackerH]->m_sAppr2 & 0x0FF0) >> 4);

            sSkillUsed = m_pClientList[sAttackerH]->m_sUsingWeaponSkill;
            if ((bIsDash == true) && (m_pClientList[sAttackerH]->m_cSkillMastery[sSkillUsed] != 100) && (wWeaponType != 25) && (wWeaponType != 27))
            {
                try
                {
                    wsprintf(G_cTxt, "TSearch Fullswing Hack: (%s) Player: (%s) - dashing with only (%d) weapon skill.", m_pClientList[sAttackerH]->m_cIPaddress, m_pClientList[sAttackerH]->m_cCharName, m_pClientList[sAttackerH]->m_cSkillMastery[sSkillUsed]);
                    PutHackLogFileList(G_cTxt);
                    DeleteClient(sAttackerH, true, true);
                }
                catch (...)
                {

                }
                return 0;
            }

            cAttackerSide = m_pClientList[sAttackerH]->m_cSide;

            if (wWeaponType == 0)
            {
                iAP_SM = iAP_L = iDice(1, (m_pClientList[sAttackerH]->m_iStr / 12));
                if (iAP_SM <= 0) iAP_SM = 1;
                if (iAP_L <= 0) iAP_L = 1;
                iAttackerHitRatio = m_pClientList[sAttackerH]->m_iHitRatio + m_pClientList[sAttackerH]->m_cSkillMastery[5];
                m_pClientList[sAttackerH]->m_sUsingWeaponSkill = 5;

            }
            else if ((wWeaponType >= 1) && (wWeaponType < 40))
            {
                iAP_SM = iDice(m_pClientList[sAttackerH]->m_cAttackDiceThrow_SM, m_pClientList[sAttackerH]->m_cAttackDiceRange_SM);
                iAP_L = iDice(m_pClientList[sAttackerH]->m_cAttackDiceThrow_L, m_pClientList[sAttackerH]->m_cAttackDiceRange_L);

                iAP_SM += m_pClientList[sAttackerH]->m_cAttackBonus_SM;
                iAP_L += m_pClientList[sAttackerH]->m_cAttackBonus_L;

                iAttackerHitRatio = m_pClientList[sAttackerH]->m_iHitRatio;

                dTmp1 = (double)iAP_SM;
                if (m_pClientList[sAttackerH]->m_iStr <= 0)
                    dTmp2 = 1.0f;
                else dTmp2 = (double)m_pClientList[sAttackerH]->m_iStr;

                dTmp2 = dTmp2 / 5.0f;
                dTmp3 = dTmp1 + (dTmp1 * (dTmp2 / 100.0f));
                iAP_SM = (int)(dTmp3 + 0.5f);

                dTmp1 = (double)iAP_L;
                if (m_pClientList[sAttackerH]->m_iStr <= 0)
                    dTmp2 = 1.0f;
                else dTmp2 = (double)m_pClientList[sAttackerH]->m_iStr;

                dTmp2 = dTmp2 / 5.0f;
                dTmp3 = dTmp1 + (dTmp1 * (dTmp2 / 100.0f));
                iAP_L = (int)(dTmp3 + 0.5f);
            }
            else if (wWeaponType >= 40)
            {
                iAP_SM = iDice(m_pClientList[sAttackerH]->m_cAttackDiceThrow_SM, m_pClientList[sAttackerH]->m_cAttackDiceRange_SM);
                iAP_L = iDice(m_pClientList[sAttackerH]->m_cAttackDiceThrow_L, m_pClientList[sAttackerH]->m_cAttackDiceRange_L);

                iAP_SM += m_pClientList[sAttackerH]->m_cAttackBonus_SM;
                iAP_L += m_pClientList[sAttackerH]->m_cAttackBonus_L;

                iAttackerHitRatio = m_pClientList[sAttackerH]->m_iHitRatio;
                bNormalMissileAttack = true;

                iAP_SM += iDice(1, (m_pClientList[sAttackerH]->m_iStr / 20));
                iAP_L += iDice(1, (m_pClientList[sAttackerH]->m_iStr / 20));
            }

            iAttackerHitRatio += 50;
            if (iAP_SM <= 0) iAP_SM = 1;
            if (iAP_L <= 0) iAP_L = 1;

            if (m_pClientList[sAttackerH]->m_iCustomItemValue_Attack != 0)
            {
                if ((m_pClientList[sAttackerH]->m_iMinAP_SM != 0) && (iAP_SM < m_pClientList[sAttackerH]->m_iMinAP_SM))
                {
                    iAP_SM = m_pClientList[sAttackerH]->m_iMinAP_SM;
                }
                if ((m_pClientList[sAttackerH]->m_iMinAP_L != 0) && (iAP_L < m_pClientList[sAttackerH]->m_iMinAP_L))
                {
                    iAP_L = m_pClientList[sAttackerH]->m_iMinAP_L;
                }
                if ((m_pClientList[sAttackerH]->m_iMaxAP_SM != 0) && (iAP_SM > m_pClientList[sAttackerH]->m_iMaxAP_SM))
                {
                    iAP_SM = m_pClientList[sAttackerH]->m_iMaxAP_SM;
                }
                if ((m_pClientList[sAttackerH]->m_iMaxAP_L != 0) && (iAP_L > m_pClientList[sAttackerH]->m_iMaxAP_L))
                {
                    iAP_L = m_pClientList[sAttackerH]->m_iMaxAP_L;
                }
            }

            if (m_pClientList[sAttackerH]->m_cHeroArmourBonus == 1)
            {
                iAttackerHitRatio += 100;
                iAP_SM += 5;
                iAP_L += 5;
            }

            sItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
            if ((sItemIndex != -1) && (m_pClientList[sAttackerH]->m_pItemList[sItemIndex] != 0))
            {
                if ((m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 851) || // KlonessEsterk 
                    (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 863) || // KlonessWand(MS.20)
                    (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 864))
                { // KlonessWand(MS.10) 
                    if (m_pClientList[sAttackerH]->m_iRating > 0)
                    {
                        iRepDamage = m_pClientList[sAttackerH]->m_iRating / 100;
                        if (iRepDamage < 5) iRepDamage = 5;
                        if (iRepDamage > 15) iRepDamage = 15;
                        iAP_SM += iRepDamage;
                        iAP_L += iRepDamage;
                    }
                    if (cTargetType == DEF_OWNERTYPE_PLAYER)
                    {
                        if (m_pClientList[sTargetH] == 0) return 0;
                        if (m_pClientList[sTargetH]->m_iRating < 0)
                        {
                            iRepDamage = (abs(m_pClientList[sTargetH]->m_iRating) / 10);
                            if (iRepDamage > 10) iRepDamage = 10;
                            iAP_SM += iRepDamage;
                            iAP_L += iRepDamage;
                        }
                    }
                }
                if ((m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 732) || // BerserkWand(MS.20)
                    (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 738))
                { // BerserkWand(MS.10)
                    iAP_SM += 1;
                    iAP_L += 1;
                }
            }

            sItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
            if ((sItemIndex != -1) && (m_pClientList[sAttackerH]->m_pItemList[sItemIndex] != 0))
            {
                if ((m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 847) &&
                    (m_cDayOrNight == 2))
                {
                    iAP_SM += 4;
                    iAP_L += 4;
                }
                if ((m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 848) &&
                    (m_cDayOrNight == 1))
                {
                    iAP_SM += 4;
                    iAP_L += 4;
                }
                if ((m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 849) || // KlonessBlade 
                    (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 850))
                { // KlonessAxe
                    if (m_pClientList[sAttackerH]->m_iRating > 0)
                    {
                        iRepDamage = m_pClientList[sAttackerH]->m_iRating / 100;
                        if (iRepDamage < 5) iRepDamage = 5;
                        if (iRepDamage > 15) iRepDamage = 15;
                        iAP_SM += iRepDamage;
                        iAP_L += iRepDamage;
                    }
                    if (cTargetType == DEF_OWNERTYPE_PLAYER)
                    {
                        if (m_pClientList[sTargetH] == 0) return 0;
                        if (m_pClientList[sTargetH]->m_iRating < 0)
                        {
                            iRepDamage = (abs(m_pClientList[sTargetH]->m_iRating) / 10);
                            if (iRepDamage > 10) iRepDamage = 10;
                            iAP_SM += iRepDamage;
                            iAP_L += iRepDamage;
                        }
                    }
                }
            }

            sItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_NECK];
            if ((sItemIndex != -1) && (m_pClientList[sAttackerH]->m_pItemList[sItemIndex] != 0))
            {
                if (m_pClientList[sAttackerH]->m_pItemList[sItemIndex]->m_sIDnum == 859)
                { // NecklaceOfKloness  
                    if (cTargetType == DEF_OWNERTYPE_PLAYER)
                    {
                        if (m_pClientList[sTargetH] == 0) return 0;
                        iRepDamage = (abs(m_pClientList[sTargetH]->m_iRating) / 20);
                        if (iRepDamage > 5) iRepDamage = 5;
                        iAP_SM += iRepDamage;
                        iAP_L += iRepDamage;
                    }
                }
            }

            cAttackerDir = m_pClientList[sAttackerH]->m_cDir;
            strcpy(cAttackerName, m_pClientList[sAttackerH]->m_cCharName);

            if (m_pClientList[sAttackerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] != 0)
                bIsAttackerBerserk = true;
            else bIsAttackerBerserk = false;

            if ((bArrowUse != true) && (m_pClientList[sAttackerH]->m_iSuperAttackLeft > 0) && (iAttackMode >= 20))
            {

                dTmp1 = (double)iAP_SM;
                dTmp2 = (double)m_pClientList[sAttackerH]->m_iLevel;
                dTmp3 = dTmp2 / 100.0f;
                dTmp2 = dTmp1 * dTmp3;
                iTemp = (int)(dTmp2 + 0.5f);
                iAP_SM += iTemp;

                dTmp1 = (double)iAP_L;
                dTmp2 = (double)m_pClientList[sAttackerH]->m_iLevel;
                dTmp3 = dTmp2 / 100.0f;
                dTmp2 = dTmp1 * dTmp3;
                iTemp = (int)(dTmp2 + 0.5f);
                iAP_L += iTemp;

                switch (m_pClientList[sAttackerH]->m_sUsingWeaponSkill)
                {
                    case 6:  iAP_SM += (iAP_SM / 10); iAP_L += (iAP_L / 10); iAttackerHitRatio += 30; break;

                    case 7:  iAP_SM *= 1; iAP_L *= 1; break;

                    case 8:  iAP_SM += (iAP_SM / 10); iAP_L += (iAP_L / 10); iAttackerHitRatio += 30; break;
                    case 10:  iAP_SM += (iAP_SM / 5); iAP_L += (iAP_L / 5);                           break;
                    case 14:  iAP_SM += (iAP_SM / 5); iAP_L += (iAP_L / 5); iAttackerHitRatio += 20; break;
                    case 21:  iAP_SM += (iAP_SM / 5); iAP_L += (iAP_L / 5); iAttackerHitRatio += 50; break;
                    default: break;
                }
                iAttackerHitRatio += 100;
                iAttackerHitRatio += m_pClientList[sAttackerH]->m_iCustomItemValue_Attack;
            }

            if (bIsDash == true)
            {

                iAttackerHitRatio += 20;

                switch (m_pClientList[sAttackerH]->m_sUsingWeaponSkill)
                {
                    case 8:  iAP_SM += (iAP_SM / 10); iAP_L += (iAP_L / 10); break;
                    case 10: iAP_SM += (iAP_SM / 5); iAP_L += (iAP_L / 5); break;
                    case 14: iAP_SM += (iAP_SM / 5); iAP_L += (iAP_L / 5); break;
                    default: break;
                }
            }

            iAttackerHP = m_pClientList[sAttackerH]->m_iHP;
            iAttackerHitRatio += m_pClientList[sAttackerH]->m_iAddAR;

            sAtkX = m_pClientList[sAttackerH]->m_sX;
            sAtkY = m_pClientList[sAttackerH]->m_sY;
            iPartyID = m_pClientList[sAttackerH]->m_iPartyID;
            break;

        case DEF_OWNERTYPE_NPC:

            if (m_pNpcList[sAttackerH] == 0) return 0;
            if (m_pMapList[m_pNpcList[sAttackerH]->m_cMapIndex]->m_bIsAttackEnabled == false) return 0;

            if ((m_pNpcList[sAttackerH]->m_iStatus & 0x10) != 0)
            {
                SetInvisibilityFlag(sAttackerH, DEF_OWNERTYPE_NPC, false);
                bRemoveFromDelayEventList(sAttackerH, DEF_OWNERTYPE_NPC, DEF_MAGICTYPE_INVISIBILITY);
                m_pNpcList[sAttackerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = 0;
            }

            cAttackerSide = m_pNpcList[sAttackerH]->m_cSide;
            iAP_SM = 0;
            iAP_L = 0;

            if (m_pNpcList[sAttackerH]->m_cAttackDiceThrow != 0)
                iAP_L = iAP_SM = iDice(m_pNpcList[sAttackerH]->m_cAttackDiceThrow, m_pNpcList[sAttackerH]->m_cAttackDiceRange);

            iAttackerHitRatio = m_pNpcList[sAttackerH]->m_iHitRatio;

            cAttackerDir = m_pNpcList[sAttackerH]->m_cDir;
            memcpy(cAttackerName, m_pNpcList[sAttackerH]->m_cNpcName, 20);

            if (m_pNpcList[sAttackerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] != 0)
                bIsAttackerBerserk = true;
            else bIsAttackerBerserk = false;

            iAttackerHP = m_pNpcList[sAttackerH]->m_iHP;
            cAttackerSA = m_pNpcList[sAttackerH]->m_cSpecialAbility;

            sAtkX = m_pNpcList[sAttackerH]->m_sX;
            sAtkY = m_pNpcList[sAttackerH]->m_sY;
            break;
    }

    switch (cTargetType)
    {
        case DEF_OWNERTYPE_PLAYER:

            if (m_pClientList[sTargetH] == 0) return 0;
            if (m_pClientList[sTargetH]->m_bIsKilled == true) return 0;
            if ((m_pClientList[sTargetH]->m_iStatus & 0x400000) != 0) return 0;

            if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_bIsCrusadeMode == false) &&
                (m_pClientList[sTargetH]->m_iPKCount == 0) && (m_pClientList[sTargetH]->m_bIsPlayerCivil == true)) return 0;

            if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sTargetH]->m_bIsNeutral == true) &&
                (m_pClientList[sTargetH]->m_iPKCount == 0) && (m_pClientList[sTargetH]->m_bIsOwnLocation == true)) return 0;

            if ((m_pClientList[sTargetH]->m_sX != tdX) || (m_pClientList[sTargetH]->m_sY != tdY)) return 0;

            if (m_pClientList[sTargetH]->m_iAdminUserLevel > 0) return 0;

            if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH]->m_bIsNeutral == true)
                && (m_pClientList[sTargetH]->m_iPKCount == 0)) return 0;

            if ((m_pClientList[sTargetH]->m_iPartyID != 0) && (iPartyID == m_pClientList[sTargetH]->m_iPartyID)) return 0;

            cTargetDir = m_pClientList[sTargetH]->m_cDir;
            iTargetDefenseRatio = m_pClientList[sTargetH]->m_iDefenseRatio;
            m_pClientList[sTargetH]->m_dwLogoutHackCheck = dwTime;
            if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH]->m_bIsSafeAttackMode == true))
            {
                iSideCondition = iGetPlayerRelationship(sAttackerH, sTargetH);
                if ((iSideCondition == 7) || (iSideCondition == 2) || (iSideCondition == 6))
                {
                    iAP_SM = iAP_SM / 2;
                    iAP_L = iAP_L / 2;
                }
                else
                {
                    if (m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_bIsFightZone == true)
                    {
                        if (m_pClientList[sAttackerH]->m_iGuildGUID == m_pClientList[sTargetH]->m_iGuildGUID) return 0;
                        else
                        {
                            iAP_SM = iAP_SM / 2;
                            iAP_L = iAP_L / 2;
                        }
                    }
                    else return 0;
                }
            }

            iTargetDefenseRatio += m_pClientList[sTargetH]->m_iAddDR;



            sTgtX = m_pClientList[sTargetH]->m_sX;
            sTgtY = m_pClientList[sTargetH]->m_sY;
            break;

        case DEF_OWNERTYPE_NPC:

            if (m_pNpcList[sTargetH] == 0) return 0;
            if (m_pNpcList[sTargetH]->m_iHP <= 0) return 0;

            if ((m_pNpcList[sTargetH]->m_sX != tdX) || (m_pNpcList[sTargetH]->m_sY != tdY)) return 0;

            cTargetDir = m_pNpcList[sTargetH]->m_cDir;
            iTargetDefenseRatio = m_pNpcList[sTargetH]->m_iDefenseRatio;

            if (cAttackerType == DEF_OWNERTYPE_PLAYER)
            {
                switch (m_pNpcList[sTargetH]->m_sType)
                {
                    case 40:
                    case 41:
                        if ((m_pClientList[sAttackerH]->m_cSide == 0) || (m_pNpcList[sTargetH]->m_cSide == m_pClientList[sAttackerH]->m_cSide)) return 0;
                        break;
                }

                if ((wWeaponType == 25) && (m_pNpcList[sTargetH]->m_cActionLimit == 5) && (m_pNpcList[sTargetH]->m_iBuildCount > 0))
                {
                    if ((m_pClientList[sAttackerH]->m_iCrusadeDuty != 2) && (m_pClientList[sAttackerH]->m_iAdminUserLevel == 0)) break;

                    switch (m_pNpcList[sTargetH]->m_sType)
                    {
                        case 36:
                        case 37:
                        case 38:
                        case 39:
                            // administrators instantly build crusade structures
                            if (m_pClientList[sAttackerH]->m_iAdminUserLevel > 0)
                            {
                                m_pNpcList[sTargetH]->m_sAppr2 = 0;
                                SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
                            }

                            switch (m_pNpcList[sTargetH]->m_iBuildCount)
                            {
                                case 1:
                                    m_pNpcList[sTargetH]->m_sAppr2 = 0;
                                    SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
                                    switch (m_pNpcList[sTargetH]->m_sType)
                                    {
                                        case 36: iConstructionPoint = 700; iWarContribution = 700; break;
                                        case 37: iConstructionPoint = 700; iWarContribution = 700; break;
                                        case 38: iConstructionPoint = 500; iWarContribution = 500; break;
                                        case 39: iConstructionPoint = 500; iWarContribution = 500; break;
                                    }

                                    m_pClientList[sAttackerH]->m_iWarContribution += iWarContribution;
                                    if (m_pClientList[sAttackerH]->m_iWarContribution > DEF_MAXWARCONTRIBUTION)
                                        m_pClientList[sAttackerH]->m_iWarContribution = DEF_MAXWARCONTRIBUTION;
                                    wsprintf(G_cTxt, "Construction Complete! WarContribution: +%d", iWarContribution);
                                    log->info(G_cTxt);
                                    SendNotifyMsg(0, sAttackerH, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[sAttackerH]->m_iConstructionPoint, m_pClientList[sAttackerH]->m_iWarContribution, 0, 0);
                                    break;
                                case 5:
                                    m_pNpcList[sTargetH]->m_sAppr2 = 1;
                                    SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
                                    break;
                                case 10:
                                    m_pNpcList[sTargetH]->m_sAppr2 = 2;
                                    SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
                                    break;
                            }
                            break;
                    }

                    m_pNpcList[sTargetH]->m_iBuildCount--;
                    if (m_pNpcList[sTargetH]->m_iBuildCount <= 0)
                    {
                        m_pNpcList[sTargetH]->m_iBuildCount = 0;
                    }
                    return 0;
                }
                if ((wWeaponType == 27) && (m_pNpcList[sTargetH]->m_cCropType != 0) && (m_pNpcList[sTargetH]->m_cActionLimit == 5) && (m_pNpcList[sTargetH]->m_iBuildCount > 0))
                {
                    cFarmingSkill = m_pClientList[sAttackerH]->m_cSkillMastery[2];
                    cCropSkill = m_pNpcList[sTargetH]->m_cCropSkill;
                    if (cFarmingSkill < 20) return 0;
                    if (m_pClientList[sAttackerH]->m_iLevel < 20) return 0;
                    switch (m_pNpcList[sTargetH]->m_sType)
                    {
                        case 64:
                            switch (m_pNpcList[sTargetH]->m_iBuildCount)
                            {
                                case 1:
                                    m_pNpcList[sTargetH]->m_sAppr2 = 3;
                                    SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
                                    //sub_4B67E0
                                    CalculateSSN_SkillIndex(sAttackerH, 2, cFarmingSkill <= cCropSkill + 10);
                                    _CheckFarmingAction(sAttackerH, sTargetH, 1);
                                    DeleteNpc(sTargetH);
                                    return 0;
                                case 8:
                                    m_pNpcList[sTargetH]->m_sAppr2 = 3;
                                    SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
                                    CalculateSSN_SkillIndex(sAttackerH, 2, cFarmingSkill <= cCropSkill + 10);
                                    _CheckFarmingAction(sAttackerH, sTargetH, 0);
                                    break;
                                case 18:
                                    m_pNpcList[sTargetH]->m_sAppr2 = 2;
                                    SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, 0, 0, 0);
                                    CalculateSSN_SkillIndex(sAttackerH, 2, cFarmingSkill <= cCropSkill + 10);
                                    _CheckFarmingAction(sAttackerH, sTargetH, 0);
                                    break;

                            }
                            break;
                    }
                    m_pNpcList[sTargetH]->m_iBuildCount--;
                    if (m_pNpcList[sTargetH]->m_iBuildCount <= 0)
                    {
                        m_pNpcList[sTargetH]->m_iBuildCount = 0;
                    }
                    return 0;
                }
            }

            sTgtX = m_pNpcList[sTargetH]->m_sX;
            sTgtY = m_pNpcList[sTargetH]->m_sY;
            break;
    }

    if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (cTargetType == DEF_OWNERTYPE_PLAYER))
    {

        sX = m_pClientList[sAttackerH]->m_sX;
        sY = m_pClientList[sAttackerH]->m_sY;

        dX = m_pClientList[sTargetH]->m_sX;
        dY = m_pClientList[sTargetH]->m_sY;

        if (m_pMapList[m_pClientList[sTargetH]->m_cMapIndex]->iGetAttribute(sX, sY, 0x00000006) != 0) return 0;
        if (m_pMapList[m_pClientList[sTargetH]->m_cMapIndex]->iGetAttribute(dX, dY, 0x00000006) != 0) return 0;
    }

    if (cAttackerType == DEF_OWNERTYPE_PLAYER)
    {
        if (m_pClientList[sAttackerH]->m_iDex > 50)
        {
            iAttackerHitRatio += (m_pClientList[sAttackerH]->m_iDex - 50);
        }
    }

    if (wWeaponType >= 40)
    {
        switch (m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cWhetherStatus)
        {
            case 0:	break;
            case 1:	iAttackerHitRatio -= (iAttackerHitRatio / 20); break;
            case 2:	iAttackerHitRatio -= (iAttackerHitRatio / 10); break;
            case 3:	iAttackerHitRatio -= (iAttackerHitRatio / 4);  break;
        }
    }

    if (iAttackerHitRatio < 0)   iAttackerHitRatio = 0;
    switch (cTargetType)
    {
        case DEF_OWNERTYPE_PLAYER:
            cProtect = m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT];
            break;

        case DEF_OWNERTYPE_NPC:
            cProtect = m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT];
            break;
    }

    if (cAttackerType == DEF_OWNERTYPE_PLAYER)
    {
        if (m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1)
        {
            if (m_pClientList[sAttackerH]->m_pItemList[m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND]] == 0)
            {
                m_pClientList[sAttackerH]->m_bIsItemEquipped[m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND]] = false;
                DeleteClient(sAttackerH, true, true);
                return 0;
            }

            if (m_pClientList[sAttackerH]->m_pItemList[m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND]]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK_ARROW)
            {
                if (m_pClientList[sAttackerH]->m_cArrowIndex == -1)
                {
                    return 0;
                }
                else
                {
                    if (m_pClientList[sAttackerH]->m_pItemList[m_pClientList[sAttackerH]->m_cArrowIndex] == 0)
                        return 0;

                    if (bArrowUse != true)
                        m_pClientList[sAttackerH]->m_pItemList[m_pClientList[sAttackerH]->m_cArrowIndex]->m_dwCount--;
                    if (m_pClientList[sAttackerH]->m_pItemList[m_pClientList[sAttackerH]->m_cArrowIndex]->m_dwCount <= 0)
                    {

                        ItemDepleteHandler(sAttackerH, m_pClientList[sAttackerH]->m_cArrowIndex, false);
                        m_pClientList[sAttackerH]->m_cArrowIndex = _iGetArrowItemIndex(sAttackerH);
                    }
                    else
                    {
                        SendNotifyMsg(0, sAttackerH, DEF_NOTIFY_SETITEMCOUNT, m_pClientList[sAttackerH]->m_cArrowIndex, m_pClientList[sAttackerH]->m_pItemList[m_pClientList[sAttackerH]->m_cArrowIndex]->m_dwCount, false, 0);
                        iCalcTotalWeight(sAttackerH);
                    }
                }
                if (cProtect == 1) return 0;
            }
            else
            {
                switch (cProtect)
                {
                    case 3: iTargetDefenseRatio += 40;  break;
                    case 4: iTargetDefenseRatio += 100; break;
                }
                if (iTargetDefenseRatio < 0) iTargetDefenseRatio = 1;
            }
        }
    }
    else
    {
        switch (cProtect)
        {
            case 1:
                switch (m_pNpcList[sAttackerH]->m_sType)
                {
                    case 54:
                        if ((abs(sTgtX - m_pNpcList[sAttackerH]->m_sX) >= 1) || (abs(sTgtY - m_pNpcList[sAttackerH]->m_sY) >= 1)) return 0;
                }
                break;
            case 3: iTargetDefenseRatio += 40;  break;
            case 4: iTargetDefenseRatio += 100; break;
        }
        if (iTargetDefenseRatio < 0) iTargetDefenseRatio = 1;
    }

    if (cAttackerDir == cTargetDir) iTargetDefenseRatio = iTargetDefenseRatio / 2;
    if (iTargetDefenseRatio < 1)   iTargetDefenseRatio = 1;

    dTmp1 = (double)(iAttackerHitRatio);
    dTmp2 = (double)(iTargetDefenseRatio);
    dTmp3 = (dTmp1 / dTmp2) * 50.0f;
    iDestHitRatio = (int)(dTmp3);

    if (iDestHitRatio < DEF_MINIMUMHITRATIO) iDestHitRatio = DEF_MINIMUMHITRATIO;
    if (iDestHitRatio > DEF_MAXIMUMHITRATIO) iDestHitRatio = DEF_MAXIMUMHITRATIO;

    if ((bIsAttackerBerserk == true) && (iAttackMode < 20))
    {
        iAP_SM = iAP_SM * 2;
        iAP_L = iAP_L * 2;
    }

    if (cAttackerType == DEF_OWNERTYPE_PLAYER)
    {
        iAP_SM += m_pClientList[sAttackerH]->m_iAddPhysicalDamage;
        iAP_L += m_pClientList[sAttackerH]->m_iAddPhysicalDamage;
    }

    if (bNearAttack == true)
    {
        iAP_SM = iAP_SM / 2;
        iAP_L = iAP_L / 2;
    }

    if (cTargetType == DEF_OWNERTYPE_PLAYER)
    {
        iAP_SM -= (iDice(1, m_pClientList[sTargetH]->m_iVit / 10) - 1);
        iAP_L -= (iDice(1, m_pClientList[sTargetH]->m_iVit / 10) - 1);
    }

    if (cAttackerType == DEF_OWNERTYPE_PLAYER)
    {
        if (iAP_SM <= 1) iAP_SM = 1;
        if (iAP_L <= 1) iAP_L = 1;
    }
    else
    {
        if (iAP_SM <= 0) iAP_SM = 0;
        if (iAP_L <= 0) iAP_L = 0;
    }

    iResult = iDice(1, 100);

    if (iResult <= iDestHitRatio)
    {
        if (cAttackerType == DEF_OWNERTYPE_PLAYER)
        {

            if (((m_pClientList[sAttackerH]->m_iHungerStatus <= 10) || (m_pClientList[sAttackerH]->m_iSP <= 0)) && (iDice(1, 10) == 5)) return false;
            m_pClientList[sAttackerH]->m_iComboAttackCount++;
            if (m_pClientList[sAttackerH]->m_iComboAttackCount < 0) m_pClientList[sAttackerH]->m_iComboAttackCount = 0;
            if (m_pClientList[sAttackerH]->m_iComboAttackCount > 4) m_pClientList[sAttackerH]->m_iComboAttackCount = 1;
            iWeaponSkill = _iGetWeaponSkillType(sAttackerH);
            iComboBonus = iGetComboAttackBonus(iWeaponSkill, m_pClientList[sAttackerH]->m_iComboAttackCount);

            if ((m_pClientList[sAttackerH]->m_iComboAttackCount > 1) && (m_pClientList[sAttackerH]->m_iAddCD != 0))
                iComboBonus += m_pClientList[sAttackerH]->m_iAddCD;

            iAP_SM += iComboBonus;
            iAP_L += iComboBonus;

            switch (m_pClientList[sAttackerH]->m_iSpecialWeaponEffectType)
            {
                case 0: break;
                case 1:
                    if ((m_pClientList[sAttackerH]->m_iSuperAttackLeft > 0) && (iAttackMode >= 20))
                    {
                        iAP_SM += m_pClientList[sAttackerH]->m_iSpecialWeaponEffectValue;
                        iAP_L += m_pClientList[sAttackerH]->m_iSpecialWeaponEffectValue;
                    }
                    break;

                case 2:
                    cAttackerSA = 61;
                    iAttackerSAvalue = m_pClientList[sAttackerH]->m_iSpecialWeaponEffectValue * 5;
                    break;

                case 3:
                    cAttackerSA = 62;
                    break;
            }

            if (m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_bIsFightZone == true)
            {
                iAP_SM += iAP_SM / 3;
                iAP_L += iAP_L / 3;
            }

            if (bCheckHeldenianMap(sAttackerH, m_iBTFieldMapIndex, DEF_OWNERTYPE_PLAYER) == 1)
            {
                iAP_SM += iAP_SM / 3;
                iAP_L += iAP_L / 3;
            }

            if ((cTargetType == DEF_OWNERTYPE_PLAYER) && (m_bIsCrusadeMode == true) && (m_pClientList[sAttackerH]->m_iCrusadeDuty == 1))
            {
                if (m_pClientList[sAttackerH]->m_iLevel <= 80)
                {
                    iAP_SM += iAP_SM;
                    iAP_L += iAP_L;
                }
                else if (m_pClientList[sAttackerH]->m_iLevel <= 100)
                {
                    iAP_SM += (iAP_SM * 7) / 10;
                    iAP_L += (iAP_L * 7) / 10;
                }
                else
                {
                    iAP_SM += iAP_SM / 3;
                    iAP_L += iAP_L / 3;
                }
            }
        }

        switch (cTargetType)
        {
            case DEF_OWNERTYPE_PLAYER:
                ClearSkillUsingStatus(sTargetH);
                if ((dwTime - m_pClientList[sTargetH]->m_dwTime) > DEF_RAGPROTECTIONTIME)
                {
                    return 0;
                }
                else
                {
                    switch (cAttackerSA)
                    {
                        case 62:
                            if (m_pClientList[sTargetH]->m_iRating < 0)
                            {
                                iTemp = abs(m_pClientList[sTargetH]->m_iRating) / 10;
                                if (iTemp > 10) iTemp = 10;
                                iAP_SM += iTemp;
                            }
                            break;
                    }

                    iAP_Abs_Armor = 0;
                    iAP_Abs_Shield = 0;
                    iTemp = iDice(1, 10000);
                    if ((iTemp >= 1) && (iTemp < 5000))           iHitPoint = 1;
                    else if ((iTemp >= 5000) && (iTemp < 7500))   iHitPoint = 2;
                    else if ((iTemp >= 7500) && (iTemp < 9000))   iHitPoint = 3;
                    else if ((iTemp >= 9000) && (iTemp <= 10000)) iHitPoint = 4;

                    switch (iHitPoint)
                    {
                        case 1:
                            if (m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_BODY] > 0)
                            {
                                if (m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_BODY] >= 80)
                                    dTmp1 = 80.0f;
                                else dTmp1 = (double)m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_BODY];
                                dTmp2 = (double)iAP_SM;
                                dTmp3 = (dTmp1 / 100.0f) * dTmp2;
                                iAP_Abs_Armor = (int)dTmp3;
                            }
                            break;
                        case 2:
                            if ((m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_PANTS] +
                                m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_LEGGINGS]) > 0)
                            {
                                if ((m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_PANTS] +
                                    m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_LEGGINGS]) >= 80)
                                    dTmp1 = 80.0f;
                                else dTmp1 = (double)(m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_PANTS] + m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_LEGGINGS]);
                                dTmp2 = (double)iAP_SM;
                                dTmp3 = (dTmp1 / 100.0f) * dTmp2;

                                iAP_Abs_Armor = (int)dTmp3;
                            }
                            break;

                        case 3:
                            if (m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_ARMS] > 0)
                            {
                                if (m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_ARMS] >= 80)
                                    dTmp1 = 80.0f;
                                else dTmp1 = (double)m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_ARMS];
                                dTmp2 = (double)iAP_SM;
                                dTmp3 = (dTmp1 / 100.0f) * dTmp2;

                                iAP_Abs_Armor = (int)dTmp3;
                            }
                            break;

                        case 4:
                            if (m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_HEAD] > 0)
                            {
                                if (m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_HEAD] >= 80)
                                    dTmp1 = 80.0f;
                                else dTmp1 = (double)m_pClientList[sTargetH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_HEAD];
                                dTmp2 = (double)iAP_SM;
                                dTmp3 = (dTmp1 / 100.0f) * dTmp2;

                                iAP_Abs_Armor = (int)dTmp3;
                            }
                            break;
                    }

                    if (m_pClientList[sTargetH]->m_iDamageAbsorption_Shield > 0)
                    {
                        if (iDice(1, 100) <= (m_pClientList[sTargetH]->m_cSkillMastery[11]))
                        {
                            CalculateSSN_SkillIndex(sTargetH, 11, 1);
                            if (m_pClientList[sTargetH]->m_iDamageAbsorption_Shield >= 80)
                                dTmp1 = 80.0f;
                            else dTmp1 = (double)m_pClientList[sTargetH]->m_iDamageAbsorption_Shield;
                            dTmp2 = (double)iAP_SM;
                            dTmp3 = (dTmp1 / 100.0f) * dTmp2;

                            iAP_Abs_Shield = (int)dTmp3;

                            iTemp = m_pClientList[sTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LHAND];
                            if ((iTemp != -1) && (m_pClientList[sTargetH]->m_pItemList[iTemp] != 0))
                            {
                                if ((m_pClientList[sTargetH]->m_cSide != 0) && (m_pClientList[sTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0))
                                    m_pClientList[sTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan--;
                                if (m_pClientList[sTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan == 0)
                                {
                                    SendNotifyMsg(0, sTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[sTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, 0, 0);
                                    ReleaseItemHandler(sTargetH, iTemp, true);
                                }
                            }
                        }
                    }

                    iAP_SM = iAP_SM - (iAP_Abs_Armor + iAP_Abs_Shield);
                    if (iAP_SM <= 0) iAP_SM = 1;

                    if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH] != 0) && (m_pClientList[sAttackerH]->m_bIsSpecialAbilityEnabled == true))
                    {
                        switch (m_pClientList[sAttackerH]->m_iSpecialAbilityType)
                        {
                            case 0: break;
                            case 1:
                                iTemp = (m_pClientList[sTargetH]->m_iHP / 2);
                                if (iTemp > iAP_SM) iAP_SM = iTemp;
                                if (iAP_SM <= 0) iAP_SM = 1;
                                break;
                            case 2:
                                if (m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 0)
                                {
                                    m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = 1;
                                    SetIceFlag(sTargetH, DEF_OWNERTYPE_PLAYER, true);
                                    bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + 30000,
                                        sTargetH, DEF_OWNERTYPE_PLAYER, 0, 0, 0, 1, 0, 0);
                                    SendNotifyMsg(0, sTargetH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_ICE, 1, 0, 0);
                                }
                                break;
                            case 3:
                                if (m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] == 0)
                                {
                                    m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 2;
                                    bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_HOLDOBJECT, dwTime + 10000,
                                        sTargetH, DEF_OWNERTYPE_PLAYER, 0, 0, 0, 10, 0, 0);
                                    SendNotifyMsg(0, sTargetH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_HOLDOBJECT, 10, 0, 0);
                                }
                                break;
                            case 4:
                                iAP_SM = (m_pClientList[sTargetH]->m_iHP);
                                break;
                            case 5:
                                m_pClientList[sAttackerH]->m_iHP += iAP_SM;
                                if (iGetMaxHP(sAttackerH) < m_pClientList[sAttackerH]->m_iHP) m_pClientList[sAttackerH]->m_iHP = iGetMaxHP(sAttackerH);
                                SendNotifyMsg(0, sAttackerH, DEF_NOTIFY_HP, 0, 0, 0, 0);
                                break;
                        }
                    }

                    if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH] != 0) && (m_pClientList[sTargetH]->m_bIsSpecialAbilityEnabled == true))
                    {
                        switch (m_pClientList[sTargetH]->m_iSpecialAbilityType)
                        {
                            case 50:
                                if (m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1)
                                    sWeaponIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
                                else sWeaponIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
                                if (sWeaponIndex != -1)	m_pClientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_wCurLifeSpan = 0;
                                break;
                            case 51:
                                if (iHitPoint == m_pClientList[sTargetH]->m_iSpecialAbilityEquipPos)
                                    iAP_SM = 0;
                                break;
                            case 52:
                                iAP_SM = 0;
                                break;
                        }
                    }

                    if ((m_pClientList[sTargetH]->m_bIsLuckyEffect == true) &&
                        (iDice(1, 10) == 5) && (m_pClientList[sTargetH]->m_iHP <= iAP_SM))
                    {
                        iAP_SM = m_pClientList[sTargetH]->m_iHP - 1;
                    }

                    switch (iHitPoint)
                    {
                        case 1:
                            iTemp = m_pClientList[sTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_BODY];
                            if ((iTemp != -1) && (m_pClientList[sTargetH]->m_pItemList[iTemp] != 0))
                            {
                                bCalculateEnduranceDecrement(sTargetH, sAttackerH, cTargetType, iTemp);
                            }
                            break;

                        case 2:
                            iTemp = m_pClientList[sTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_PANTS];
                            if ((iTemp != -1) && (m_pClientList[sTargetH]->m_pItemList[iTemp] != 0))
                            {
                                bCalculateEnduranceDecrement(sTargetH, sAttackerH, cTargetType, iTemp);
                            }
                            else
                            {
                                iTemp = m_pClientList[sTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LEGGINGS];
                                if ((iTemp != -1) && (m_pClientList[sTargetH]->m_pItemList[iTemp] != 0))
                                {
                                    bCalculateEnduranceDecrement(sTargetH, sAttackerH, cTargetType, iTemp);
                                }
                            }
                            break;

                        case 3:
                            iTemp = m_pClientList[sTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_ARMS];
                            if ((iTemp != -1) && (m_pClientList[sTargetH]->m_pItemList[iTemp] != 0))
                            {
                                bCalculateEnduranceDecrement(sTargetH, sAttackerH, cTargetType, iTemp);
                            }
                            break;

                        case 4:
                            iTemp = m_pClientList[sTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_HEAD];
                            if ((iTemp != -1) && (m_pClientList[sTargetH]->m_pItemList[iTemp] != 0))
                            {
                                bCalculateEnduranceDecrement(sTargetH, sAttackerH, cTargetType, iTemp);
                            }
                            break;
                    }

                    if ((cAttackerSA == 2) && (m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] != 0))
                    {
                        SendNotifyMsg(0, sTargetH, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_PROTECT, m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT], 0, 0);
                        switch (m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT])
                        {
                            case 1:
                                SetProtectionFromArrowFlag(sTargetH, DEF_OWNERTYPE_PLAYER, false);
                                break;
                            case 2:
                            case 5:
                                SetMagicProtectionFlag(sTargetH, DEF_OWNERTYPE_PLAYER, false);
                                break;
                            case 3:
                            case 4:
                                SetDefenseShieldFlag(sTargetH, DEF_OWNERTYPE_PLAYER, false);
                                break;
                        }
                        m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] = 0;
                        bRemoveFromDelayEventList(sTargetH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_PROTECT);
                    }

                    if ((m_pClientList[sTargetH]->m_bIsPoisoned == false) &&
                        ((cAttackerSA == 5) || (cAttackerSA == 6) || (cAttackerSA == 61)))
                    {
                        if (bCheckResistingPoisonSuccess(sTargetH, DEF_OWNERTYPE_PLAYER) == false)
                        {
                            m_pClientList[sTargetH]->m_bIsPoisoned = true;
                            if (cAttackerSA == 5)		m_pClientList[sTargetH]->m_iPoisonLevel = 15;
                            else if (cAttackerSA == 6)  m_pClientList[sTargetH]->m_iPoisonLevel = 40;
                            else if (cAttackerSA == 61) m_pClientList[sTargetH]->m_iPoisonLevel = iAttackerSAvalue;

                            m_pClientList[sTargetH]->m_dwPoisonTime = dwTime;
                            SendNotifyMsg(0, sTargetH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_POISON, m_pClientList[sTargetH]->m_iPoisonLevel, 0, 0);
                            SetPoisonFlag(sTargetH, DEF_OWNERTYPE_PLAYER, true);
                        }
                    }

                    m_pClientList[sTargetH]->m_iHP -= iAP_SM;
                    if (m_pClientList[sTargetH]->m_iHP <= 0)
                    {
                        if (cAttackerType == DEF_OWNERTYPE_PLAYER)
                            bAnalyzeCriminalAction(sAttackerH, m_pClientList[sTargetH]->m_sX, m_pClientList[sTargetH]->m_sY);
                        ClientKilledHandler(sTargetH, sAttackerH, cAttackerType, iAP_SM);
                        bKilled = true;
                        iKilledDice = m_pClientList[sTargetH]->m_iLevel;
                    }
                    else
                    {
                        if (iAP_SM > 0)
                        {
                            if (m_pClientList[sTargetH]->m_iAddTransMana > 0)
                            {
                                dTmp1 = (double)m_pClientList[sTargetH]->m_iAddTransMana;
                                dTmp2 = (double)iAP_SM;
                                dTmp3 = (dTmp1 / 100.0f) * dTmp2;
                                iTemp = (2 * m_pClientList[sTargetH]->m_iMag) + (2 * m_pClientList[sTargetH]->m_iLevel) + (m_pClientList[sTargetH]->m_iInt / 2);
                                m_pClientList[sTargetH]->m_iMP += (int)dTmp3;
                                if (m_pClientList[sTargetH]->m_iMP > iTemp) m_pClientList[sTargetH]->m_iMP = iTemp;
                            }
                            if (m_pClientList[sTargetH]->m_iAddChargeCritical > 0)
                            {
                                if (iDice(1, 100) < (m_pClientList[sTargetH]->m_iAddChargeCritical))
                                {
                                    iMaxSuperAttack = (m_pClientList[sTargetH]->m_iLevel / 10);
                                    if (m_pClientList[sTargetH]->m_iSuperAttackLeft < iMaxSuperAttack) m_pClientList[sTargetH]->m_iSuperAttackLeft++;
                                    SendNotifyMsg(0, sTargetH, DEF_NOTIFY_SUPERATTACKLEFT, 0, 0, 0, 0);
                                }
                            }

                            SendNotifyMsg(0, sTargetH, DEF_NOTIFY_HP, 0, 0, 0, 0);

                            if (cAttackerType == DEF_OWNERTYPE_PLAYER)
                                sAttackerWeapon = ((m_pClientList[sAttackerH]->m_sAppr2 & 0x0FF0) >> 4);
                            else sAttackerWeapon = 1;

                            if ((cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_bIsFightZone == true))
                                iMoveDamage = 60;
                            else iMoveDamage = 40;

                            if (iAP_SM >= iMoveDamage)
                            {
                                if (sTgtX == sAtkX)
                                {
                                    if (sTgtY == sAtkY)     goto CAE_SKIPDAMAGEMOVE;
                                    else if (sTgtY > sAtkY) cDamageMoveDir = 5;
                                    else if (sTgtY < sAtkY) cDamageMoveDir = 1;
                                }
                                else if (sTgtX > sAtkX)
                                {
                                    if (sTgtY == sAtkY)     cDamageMoveDir = 3;
                                    else if (sTgtY > sAtkY) cDamageMoveDir = 4;
                                    else if (sTgtY < sAtkY) cDamageMoveDir = 2;
                                }
                                else if (sTgtX < sAtkX)
                                {
                                    if (sTgtY == sAtkY)     cDamageMoveDir = 7;
                                    else if (sTgtY > sAtkY) cDamageMoveDir = 6;
                                    else if (sTgtY < sAtkY) cDamageMoveDir = 8;
                                }
                                m_pClientList[sTargetH]->m_iLastDamage = iAP_SM;

                                SendNotifyMsg(0, sTargetH, DEF_NOTIFY_DAMAGEMOVE, cDamageMoveDir, iAP_SM, sAttackerWeapon, 0);
                            }
                            else
                            {
                                CAE_SKIPDAMAGEMOVE:;
                                int iProb;
                                if (cAttackerType == DEF_OWNERTYPE_PLAYER)
                                {
                                    switch (m_pClientList[sAttackerH]->m_sUsingWeaponSkill)
                                    {
                                        case 6: iProb = 3500; break;
                                        case 8: iProb = 1000; break;
                                        case 9: iProb = 2900; break;
                                        case 10: iProb = 2500; break;
                                        case 14: iProb = 2000; break;
                                        case 21: iProb = 2000; break;
                                        default: iProb = 1; break;
                                    }
                                }
                                else iProb = 1;

                                if (iDice(1, 10000) >= iProb)
                                    SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iAP_SM, sAttackerWeapon, 0);
                            }

                            if (m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] == 1)
                            {
                                SendNotifyMsg(0, sTargetH, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_HOLDOBJECT, m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT], 0, 0);
                                m_pClientList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
                                bRemoveFromDelayEventList(sTargetH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_HOLDOBJECT);
                            }

                            m_pClientList[sTargetH]->m_iSuperAttackCount++;
                            if (m_pClientList[sTargetH]->m_iSuperAttackCount > 14)
                            {
                                m_pClientList[sTargetH]->m_iSuperAttackCount = 0;
                                iMaxSuperAttack = (m_pClientList[sTargetH]->m_iLevel / 10);
                                if (m_pClientList[sTargetH]->m_iSuperAttackLeft < iMaxSuperAttack) m_pClientList[sTargetH]->m_iSuperAttackLeft++;
                                SendNotifyMsg(0, sTargetH, DEF_NOTIFY_SUPERATTACKLEFT, 0, 0, 0, 0);
                            }
                        }
                    }
                }
                break;

            case DEF_OWNERTYPE_NPC:
                if (m_pNpcList[sTargetH]->m_cBehavior == DEF_BEHAVIOR_DEAD) return 0;
                if (m_pNpcList[sTargetH]->m_bIsKilled == true) return 0;
                if (m_bIsCrusadeMode == true)
                {
                    if (cAttackerSide == m_pNpcList[sTargetH]->m_cSide)
                    {
                        switch (m_pNpcList[sTargetH]->m_sType)
                        {
                            case 40:
                            case 41:
                            case 43:
                            case 44:
                            case 45:
                            case 46:
                            case 47:
                            case 51:
                                return 0;

                            default: break;
                        }
                    }
                    else
                    {
                        switch (m_pNpcList[sTargetH]->m_sType)
                        {
                            case 41:
                                if (cAttackerSide != 0)
                                {
                                    m_pNpcList[sTargetH]->m_iV1 += iAP_L;
                                    if (m_pNpcList[sTargetH]->m_iV1 > 500)
                                    {
                                        m_pNpcList[sTargetH]->m_iV1 = 0;
                                        m_pNpcList[sTargetH]->m_iManaStock--;
                                        if (m_pNpcList[sTargetH]->m_iManaStock <= 0) m_pNpcList[sTargetH]->m_iManaStock = 0;
                                        wsprintf(G_cTxt, "ManaStock down: %d", m_pNpcList[sTargetH]->m_iManaStock);
                                        log->info(G_cTxt);
                                    }
                                }
                                break;
                        }
                    }
                }
                switch (m_pNpcList[sTargetH]->m_cActionLimit)
                {
                    case 1:
                    case 2:
                        return 0;
                }

                if (m_pNpcList[sTargetH]->m_cSize == 0)
                    iDamage = iAP_SM;
                else iDamage = iAP_L;

                if (m_pNpcList[sTargetH]->m_iAbsDamage < 0)
                {
                    dTmp1 = (double)iDamage;
                    dTmp2 = (double)(abs(m_pNpcList[sTargetH]->m_iAbsDamage)) / 100.0f;
                    dTmp3 = dTmp1 * dTmp2;
                    dTmp2 = dTmp1 - dTmp3;
                    iDamage = (int)dTmp2;
                    if (iDamage < 0) iDamage = 1;
                    else if ((m_pNpcList[sTargetH]->m_sType == 31) && (cAttackerType == 1) && (m_pClientList[sAttackerH] != 0) && (m_pClientList[sAttackerH]->m_iSpecialAbilityType == 7))
                        iDamage += iDice(3, 2);
                }


                if ((cAttackerSA == 2) && (m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] != 0))
                {
                    switch (m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT])
                    {
                        case 1:
                            SetProtectionFromArrowFlag(sTargetH, DEF_OWNERTYPE_NPC, false);
                            break;
                        case 2:
                        case 5:
                            SetMagicProtectionFlag(sTargetH, DEF_OWNERTYPE_NPC, false);
                            break;
                        case 3:
                        case 4:
                            SetDefenseShieldFlag(sTargetH, DEF_OWNERTYPE_NPC, false);
                            break;
                    }
                    m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] = 0;
                    bRemoveFromDelayEventList(sTargetH, DEF_OWNERTYPE_NPC, DEF_MAGICTYPE_PROTECT);
                }

                switch (m_pNpcList[sTargetH]->m_cActionLimit)
                {
                    case 0:
                    case 3:
                    case 5:
                        m_pNpcList[sTargetH]->m_iHP -= iDamage;
                        break;
                }

                if (m_pNpcList[sTargetH]->m_iHP <= 0)
                {
                    NpcKilledHandler(sAttackerH, cAttackerType, sTargetH, iDamage);
                    bKilled = true;
                    iKilledDice = m_pNpcList[sTargetH]->m_iHitDice;
                }
                else
                {
                    if ((m_pNpcList[sTargetH]->m_sType != 21) && (m_pNpcList[sTargetH]->m_sType != 55) && (m_pNpcList[sTargetH]->m_sType != 56)
                        && (m_pNpcList[sTargetH]->m_cSide == cAttackerSide)) goto CAE_SKIPCOUNTERATTACK;

                    if (m_pNpcList[sTargetH]->m_cActionLimit != 0) goto CAE_SKIPCOUNTERATTACK;
                    if (m_pNpcList[sTargetH]->m_bIsPermAttackMode == true) goto CAE_SKIPCOUNTERATTACK;
                    if ((m_pNpcList[sTargetH]->m_bIsSummoned == true) && (m_pNpcList[sTargetH]->m_iSummonControlMode == 1)) goto CAE_SKIPCOUNTERATTACK;
                    if (m_pNpcList[sTargetH]->m_sType == 51) goto CAE_SKIPCOUNTERATTACK;

                    if (iDice(1, 3) == 2)
                    {
                        if (m_pNpcList[sTargetH]->m_cBehavior == DEF_BEHAVIOR_ATTACK)
                        {
                            tX = tY = 0;
                            switch (m_pNpcList[sTargetH]->m_cTargetType)
                            {
                                case DEF_OWNERTYPE_PLAYER:
                                    if (m_pClientList[m_pNpcList[sTargetH]->m_iTargetIndex] != 0)
                                    {
                                        tX = m_pClientList[m_pNpcList[sTargetH]->m_iTargetIndex]->m_sX;
                                        tY = m_pClientList[m_pNpcList[sTargetH]->m_iTargetIndex]->m_sY;
                                    }
                                    break;

                                case DEF_OWNERTYPE_NPC:
                                    if (m_pNpcList[m_pNpcList[sTargetH]->m_iTargetIndex] != 0)
                                    {
                                        tX = m_pNpcList[m_pNpcList[sTargetH]->m_iTargetIndex]->m_sX;
                                        tY = m_pNpcList[m_pNpcList[sTargetH]->m_iTargetIndex]->m_sY;
                                    }
                                    break;
                            }

                            iDst1 = (m_pNpcList[sTargetH]->m_sX - tX) * (m_pNpcList[sTargetH]->m_sX - tX) + (m_pNpcList[sTargetH]->m_sY - tY) * (m_pNpcList[sTargetH]->m_sY - tY);

                            tX = tY = 0;
                            switch (cAttackerType)
                            {
                                case DEF_OWNERTYPE_PLAYER:
                                    if (m_pClientList[sAttackerH] != 0)
                                    {
                                        tX = m_pClientList[sAttackerH]->m_sX;
                                        tY = m_pClientList[sAttackerH]->m_sY;
                                    }
                                    break;

                                case DEF_OWNERTYPE_NPC:
                                    if (m_pNpcList[sAttackerH] != 0)
                                    {
                                        tX = m_pNpcList[sAttackerH]->m_sX;
                                        tY = m_pNpcList[sAttackerH]->m_sY;
                                    }
                                    break;
                            }

                            iDst2 = (m_pNpcList[sTargetH]->m_sX - tX) * (m_pNpcList[sTargetH]->m_sX - tX) + (m_pNpcList[sTargetH]->m_sY - tY) * (m_pNpcList[sTargetH]->m_sY - tY);

                            if (iDst2 <= iDst1)
                            {
                                m_pNpcList[sTargetH]->m_cBehavior = DEF_BEHAVIOR_ATTACK;
                                m_pNpcList[sTargetH]->m_sBehaviorTurnCount = 0;
                                m_pNpcList[sTargetH]->m_iTargetIndex = sAttackerH;
                                m_pNpcList[sTargetH]->m_cTargetType = cAttackerType;
                            }
                        }
                        else
                        {
                            m_pNpcList[sTargetH]->m_cBehavior = DEF_BEHAVIOR_ATTACK;
                            m_pNpcList[sTargetH]->m_sBehaviorTurnCount = 0;
                            m_pNpcList[sTargetH]->m_iTargetIndex = sAttackerH;
                            m_pNpcList[sTargetH]->m_cTargetType = cAttackerType;
                        }
                    }

                    CAE_SKIPCOUNTERATTACK:;

                    if ((iDice(1, 3) == 2) && (m_pNpcList[sTargetH]->m_cActionLimit == 0))
                        m_pNpcList[sTargetH]->m_dwTime = dwTime;

                    if (cAttackerType == DEF_OWNERTYPE_PLAYER)
                        sAttackerWeapon = ((m_pClientList[sAttackerH]->m_sAppr2 & 0x0FF0) >> 4);
                    else sAttackerWeapon = 1;

                    if ((wWeaponType < 40) && (m_pNpcList[sTargetH]->m_cActionLimit == 4))
                    {
                        if (sTgtX == sAtkX)
                        {
                            if (sTgtY == sAtkY)     goto CAE_SKIPDAMAGEMOVE2;
                            else if (sTgtY > sAtkY) cDamageMoveDir = 5;
                            else if (sTgtY < sAtkY) cDamageMoveDir = 1;
                        }
                        else if (sTgtX > sAtkX)
                        {
                            if (sTgtY == sAtkY)     cDamageMoveDir = 3;
                            else if (sTgtY > sAtkY) cDamageMoveDir = 4;
                            else if (sTgtY < sAtkY) cDamageMoveDir = 2;
                        }
                        else if (sTgtX < sAtkX)
                        {
                            if (sTgtY == sAtkY)     cDamageMoveDir = 7;
                            else if (sTgtY > sAtkY) cDamageMoveDir = 6;
                            else if (sTgtY < sAtkY) cDamageMoveDir = 8;
                        }

                        dX = m_pNpcList[sTargetH]->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
                        dY = m_pNpcList[sTargetH]->m_sY + _tmp_cTmpDirY[cDamageMoveDir];

                        if (m_pMapList[m_pNpcList[sTargetH]->m_cMapIndex]->bGetMoveable(dX, dY, 0) == false)
                        {
                            cDamageMoveDir = iDice(1, 8);
                            dX = m_pNpcList[sTargetH]->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
                            dY = m_pNpcList[sTargetH]->m_sY + _tmp_cTmpDirY[cDamageMoveDir];

                            if (m_pMapList[m_pNpcList[sTargetH]->m_cMapIndex]->bGetMoveable(dX, dY, 0) == false) goto CAE_SKIPDAMAGEMOVE2;
                        }

                        m_pMapList[m_pNpcList[sTargetH]->m_cMapIndex]->ClearOwner(5, sTargetH, DEF_OWNERTYPE_NPC, m_pNpcList[sTargetH]->m_sX, m_pNpcList[sTargetH]->m_sY);
                        m_pMapList[m_pNpcList[sTargetH]->m_cMapIndex]->SetOwner(sTargetH, DEF_OWNERTYPE_NPC, dX, dY);
                        m_pNpcList[sTargetH]->m_sX = dX;
                        m_pNpcList[sTargetH]->m_sY = dY;
                        m_pNpcList[sTargetH]->m_cDir = cDamageMoveDir;

                        SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTMOVE, 0, 0, 0);

                        dX = m_pNpcList[sTargetH]->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
                        dY = m_pNpcList[sTargetH]->m_sY + _tmp_cTmpDirY[cDamageMoveDir];

                        if (m_pMapList[m_pNpcList[sTargetH]->m_cMapIndex]->bGetMoveable(dX, dY, 0) == false)
                        {
                            cDamageMoveDir = iDice(1, 8);
                            dX = m_pNpcList[sTargetH]->m_sX + _tmp_cTmpDirX[cDamageMoveDir];
                            dY = m_pNpcList[sTargetH]->m_sY + _tmp_cTmpDirY[cDamageMoveDir];
                            if (m_pMapList[m_pNpcList[sTargetH]->m_cMapIndex]->bGetMoveable(dX, dY, 0) == false) goto CAE_SKIPDAMAGEMOVE2;
                        }

                        m_pMapList[m_pNpcList[sTargetH]->m_cMapIndex]->ClearOwner(5, sTargetH, DEF_OWNERTYPE_NPC, m_pNpcList[sTargetH]->m_sX, m_pNpcList[sTargetH]->m_sY);
                        m_pMapList[m_pNpcList[sTargetH]->m_cMapIndex]->SetOwner(sTargetH, DEF_OWNERTYPE_NPC, dX, dY);
                        m_pNpcList[sTargetH]->m_sX = dX;
                        m_pNpcList[sTargetH]->m_sY = dY;
                        m_pNpcList[sTargetH]->m_cDir = cDamageMoveDir;

                        SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTMOVE, 0, 0, 0);

                        if (bCheckEnergySphereDestination(sTargetH, sAttackerH, cAttackerType) == true)
                        {
                            if (cAttackerType == DEF_OWNERTYPE_PLAYER)
                            {
                                iExp = (m_pNpcList[sTargetH]->m_iExp / 3);
                                if (m_pNpcList[sTargetH]->m_iNoDieRemainExp > 0)
                                    iExp += m_pNpcList[sTargetH]->m_iNoDieRemainExp;

                                if (m_pClientList[sAttackerH]->m_iAddExp != 0)
                                {
                                    dTmp1 = (double)m_pClientList[sAttackerH]->m_iAddExp;
                                    dTmp2 = (double)iExp;
                                    dTmp3 = (dTmp1 / 100.0f) * dTmp2;
                                    iExp += (int)dTmp3;
                                }

                                if ((m_bIsCrusadeMode == true) && (iExp > 10)) iExp = 10;

                                GetExp(sAttackerH, iExp);

                                DeleteNpc(sTargetH);
                                return false;
                            }
                        }

                        CAE_SKIPDAMAGEMOVE2:;
                    }
                    else
                    {
                        SendEventToNearClient_TypeA(sTargetH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, sAttackerWeapon, 0);
                    }

                    if (m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] == 1)
                    {
                        m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
                        bRemoveFromDelayEventList(sTargetH, DEF_OWNERTYPE_NPC, DEF_MAGICTYPE_HOLDOBJECT);
                    }
                    else if (m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] == 2)
                    {
                        if ((m_pNpcList[sTargetH]->m_iHitDice > 50) && (iDice(1, 10) == 5))
                        {
                            m_pNpcList[sTargetH]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = 0;
                            bRemoveFromDelayEventList(sTargetH, DEF_OWNERTYPE_NPC, DEF_MAGICTYPE_HOLDOBJECT);
                        }
                    }

                    if ((m_pNpcList[sTargetH]->m_iNoDieRemainExp > 0) && (m_pNpcList[sTargetH]->m_bIsSummoned != true) &&
                        (cAttackerType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH] != 0))
                    {
                        if (m_pNpcList[sTargetH]->m_iNoDieRemainExp > iDamage)
                        {
                            iExp = iDamage;
                            m_pNpcList[sTargetH]->m_iNoDieRemainExp -= iDamage;
                        }
                        else
                        {
                            iExp = m_pNpcList[sTargetH]->m_iNoDieRemainExp;
                            m_pNpcList[sTargetH]->m_iNoDieRemainExp = 0;
                        }

                        if (m_pClientList[sAttackerH]->m_iAddExp != 0)
                        {
                            dTmp1 = (double)m_pClientList[sAttackerH]->m_iAddExp;
                            dTmp2 = (double)iExp;
                            dTmp3 = (dTmp1 / 100.0f) * dTmp2;
                            iExp += (int)dTmp3;
                        }

                        if (m_bIsCrusadeMode == true) iExp = iExp / 3;

                        if (m_pClientList[sAttackerH]->m_iLevel > 100)
                        {
                            switch (m_pNpcList[sTargetH]->m_sType)
                            {
                                case 55:
                                case 56:
                                    iExp = 0;
                                    break;
                                default: break;
                            }
                        }
                    }
                }
                break;
        }

        if (cAttackerType == DEF_OWNERTYPE_PLAYER)
        {
            if (m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1)
                sWeaponIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
            else sWeaponIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];

            if ((sWeaponIndex != -1) && (bArrowUse != true))
            {
                if ((m_pClientList[sAttackerH]->m_pItemList[sWeaponIndex] != 0) &&
                    (m_pClientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_sIDnum != 231))
                {
                    if (bKilled == false)
                        CalculateSSN_ItemIndex(sAttackerH, sWeaponIndex, 1);
                    else
                    {
                        if (m_pClientList[sAttackerH]->m_iHP <= 3)
                            CalculateSSN_ItemIndex(sAttackerH, sWeaponIndex, iDice(1, iKilledDice) * 2);
                        else CalculateSSN_ItemIndex(sAttackerH, sWeaponIndex, iDice(1, iKilledDice));
                    }
                }

                if ((m_pClientList[sAttackerH]->m_pItemList[sWeaponIndex] != 0) &&
                    (m_pClientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_wMaxLifeSpan != 0))
                {
                    iWepLifeOff = 1;
                    if ((wWeaponType >= 1) && (wWeaponType < 40))
                    {
                        switch (m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->m_cWhetherStatus)
                        {
                            case 0:	break;
                            case 1:	if (iDice(1, 3) == 1) iWepLifeOff++; break;
                            case 2:	if (iDice(1, 2) == 1) iWepLifeOff += iDice(1, 2); break;
                            case 3:	if (iDice(1, 2) == 1) iWepLifeOff += iDice(1, 3); break;
                        }
                    }

                    if (m_pClientList[sAttackerH]->m_cSide != 0)
                    {
                        if (m_pClientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_wCurLifeSpan < iWepLifeOff)
                            m_pClientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_wCurLifeSpan = 0;
                        else m_pClientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_wCurLifeSpan -= iWepLifeOff;
                    }

                    if (m_pClientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_wCurLifeSpan == 0)
                    {
                        SendNotifyMsg(0, sAttackerH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[sAttackerH]->m_pItemList[sWeaponIndex]->m_cEquipPos, sWeaponIndex, 0, 0);
                        ReleaseItemHandler(sAttackerH, sWeaponIndex, true);
                    }
                }
            }
            else
            {
                if (wWeaponType == 0)
                {
                    CalculateSSN_SkillIndex(sAttackerH, 5, 1);
                }
            }
        }
    }
    else
    {
        if (cAttackerType == DEF_OWNERTYPE_PLAYER)
        {
            m_pClientList[sAttackerH]->m_iComboAttackCount = 0;
        }
    }

    return iExp;
}

void CGame::_GrandMagicLaunchMsgSend(int iType, char cAttackerSide)
{
    char * cp, cBuff[120];
    uint16_t * wp;

    memset(cBuff, 0, sizeof(cBuff));
    cp = (char *)cBuff;
    *cp = GSM_GRANDMAGICLAUNCH;
    cp++;

    wp = (uint16_t *)cp;
    *wp = (uint16_t)iType;
    cp += 2;

    wp = (uint16_t *)cp;
    *wp = (uint16_t)cAttackerSide;
    cp += 2;

    bStockMsgToGateServer(cBuff, 5);
}

bool CGame::bGetItemNameWhenDeleteNpc(int & iItemID, short sNpcType)
{
    int iResult;

    switch (sNpcType)
    {
        case 49: // Hellclaw 
            iResult = iDice(1, 20000);
            if ((iResult >= 1) && (iResult <= 5000)) iItemID = 308;	    // MagicNecklace(MS10)
            else if ((iResult > 5000) && (iResult <= 10000))  iItemID = 259;  // MagicWand(M.Shield)
            else if ((iResult > 10000) && (iResult <= 13000))  iItemID = 337;  // RubyRing
            else if ((iResult > 13000) && (iResult <= 15000))  iItemID = 335;  // EmeraldRing
            else if ((iResult > 15000) && (iResult <= 17500))  iItemID = 300;  // MagicNecklace(RM10)
            else if ((iResult > 17500) && (iResult <= 18750))  iItemID = 311;  // MagicNecklace(DF+10)
            else if ((iResult > 18750) && (iResult <= 19000))  iItemID = 305;  // MagicNecklace(DM+1)
            else if ((iResult > 19000) && (iResult <= 19700))  iItemID = 634;  // RingofWizard
            else if ((iResult > 19700) && (iResult <= 19844))  iItemID = 635;  // RingofMage
            else if ((iResult > 19844) && (iResult <= 19922))  iItemID = 614;  // SwordofIceElemental
            else if ((iResult > 19922) && (iResult <= 19961))  iItemID = 640;  // KnecklaceOfSufferent
            else if ((iResult > 19961) && (iResult <= 19981))  iItemID = 637;  // KnecklaceOfLightPro
            else if ((iResult > 19981) && (iResult <= 19991))  iItemID = 620;  // MerienShield
            else if ((iResult > 19991) && (iResult <= 19996))  iItemID = 644;  // KnecklaceOfAirEle
            else if ((iResult > 19996) && (iResult <= 19999))  iItemID = 643;  // KnecklaceOfIceEle
            else if ((iResult > 19999) && (iResult <= 20000))  iItemID = 636;  // RingofGrandMage
            return true;

        case 50: // Tigerworm
            iResult = iDice(1, 10000);
            if ((iResult >= 1) && (iResult <= 4999))
            {
                if (iDice(1, 2) == 1)
                    iItemID = 311;  // MagicNecklace(DF+10)
                else iItemID = 305;      // MagicNecklace(DM+1)
            }
            else if ((iResult > 5000) && (iResult <= 7499))  iItemID = 614;  // SwordofIceElemental
            else if ((iResult > 7500) && (iResult <= 8749))  iItemID = 290;  // Flameberge+3(LLF)
            else if ((iResult > 8750) && (iResult <= 9374))  iItemID = 633;  // RingofDemonpower
            else if ((iResult > 9375) && (iResult <= 9687))  iItemID = 492;  // BloodRapier
            else if ((iResult > 9688) && (iResult <= 9843))  iItemID = 490;  // BloodSword
            else if ((iResult > 9844) && (iResult <= 9921))  iItemID = 491;  // BloodAxe
            else if ((iResult > 9922) && (iResult <= 9960))  iItemID = 947;  // DragonWand(MS40)
            else if ((iResult > 9961) && (iResult <= 9980))  iItemID = 643;  // KnecklaceOfIceEle
            else if ((iResult > 9981) && (iResult <= 9990))  iItemID = 612;  // XelimaRapier
            else if ((iResult > 9991) && (iResult <= 9996))  iItemID = 610;  // XelimaBlade
            else if ((iResult > 9996) && (iResult <= 9998))  iItemID = 611;  // XelimaAxe
            else if ((iResult > 9999) && (iResult <= 10000)) iItemID = 631;  // RingoftheAbaddon
            return true;

        default:
            break;
    }

    if (iDice(1, 45) == 13)
    {
        switch (sNpcType)
        {
            case 11: if (iDice(1, 465) != 11) return false; break;	  // Skeleton   2 * 100	
            case 12: if (iDice(1, 340) != 11) return false; break;	  // Stone-Golem 2 * 100	
            case 13: if (iDice(1, 85) != 11) return false; break;	  // Cyclops  6 * 100	
            case 14: if (iDice(1, 595) != 11) return false; break;	  // Orc 4 * 100	
            case 17: if (iDice(1, 510) != 11) return false; break;	  // Scorpoin 5 * 100	
            case 18: if (iDice(1, 720) != 11) return false; break;	  // Zombie 1 * 100	
            case 22: if (iDice(1, 510) != 11) return false; break;	  // Amphis 5 * 100	
            case 23: if (iDice(1, 340) != 11) return false; break;	  // Clay-Golem 2 * 100	
            case 27: if (iDice(1, 85) != 11) return false; break;	  // Hellhound 7 * 100	
            case 28: if (iDice(1, 85) != 11) return false; break;	  // Troll 5 * 100	
            case 29: if (iDice(1, 125) != 11) return false; break;	  // Orge  7 * 100	
            case 30: if (iDice(1, 100) != 11) return false; break;	  // Liche 1 * 100   
            case 31: if (iDice(1, 120) != 11) return false; break;	  // Demon 5 * 100
            case 32: if (iDice(1, 170) != 11) return false; break;	  // Unicorn 5 * 100	
            case 33: if (iDice(1, 255) != 11) return false; break;	  // WereWolf 7 * 100
            case 48: if (iDice(1, 80) != 11) return false; break;	  // Stalker 
            case 52: if (iDice(1, 255) != 11) return false; break;     // Gagoyle
            case 53: if (iDice(1, 425) != 11) return false; break;	  // Beholder
            case 54: if (iDice(1, 200) != 11) return false; break;	  // Dark-Elf
            case 57: if (iDice(1, 400) != 11) return false; break;	  // Giant-Frog
            case 63: if (iDice(1, 300) != 11) return false; break;	  // Frost
            case 79: if (iDice(1, 170) != 11) return false; break;	  // Nizie
            case 70: if (iDice(1, 170) != 11) return false; break;	  // Barlog
            case 71: if (iDice(1, 170) != 11) return false; break;	  // Centaurus
            case 74: if (iDice(1, 170) != 11) return false; break;	  // Giant-Crayfish
            case 72: if (iDice(1, 170) != 11) return false; break;	  // Claw-Turtle
            case 75: if (iDice(1, 170) != 11) return false; break;	  // Giant-Lizard
            case 77: if (iDice(1, 170) != 11) return false; break;	  // MasterMage-Orc
            case 78: if (iDice(1, 170) != 11) return false; break;	  // Minotaurs
            case 59: if (iDice(1, 120) != 11) return false; break;	  // Ettin
            default: return false;
        }
    }
    else return false;
    //http://www.helbreath.com/down/d_patch_v2.htm

    switch (sNpcType)
    {
        case 11: // Skeleton
        case 17: // Scorpoin
        case 14: // Orc
        case 28: // Troll
        case 57: // Giant-Frog
            switch (iDice(1, 7))
            {
                case 1: iItemID = 334; break; // LuckyGoldRing
                case 2: iItemID = 336; break; // SapphireRing
                case 3: if (iDice(1, 15) == 3) iItemID = 335; break; // EmeraldRing
                case 4: iItemID = 337; break; // RubyRing
                case 5: iItemID = 333; break; // PlatinumRing
                case 6: if (iDice(1, 15) == 3) iItemID = 634; break; // RingofWizard
                case 7: if (iDice(1, 25) == 3) iItemID = 635; break; // RingofMage
            }
            break;

        case 13: // Cyclops
        case 27: // Hellhound
        case 29: // Orge
            switch (iDice(1, 7))
            {
                case 1: iItemID = 311; break; // MagicNecklace(DF+10)
                case 2: if (iDice(1, 15) == 13) iItemID = 308; break; // MagicNecklace(MS10)
                case 3: if (iDice(1, 5) == 3) iItemID = 305; break; // MagicNecklace(DM+1)
                case 4: iItemID = 300; break; // MagicNecklace(RM10)
                case 5: if (iDice(1, 25) == 13) iItemID = 632; break; // RingofOgrepower
                case 6: if (iDice(1, 25) == 13) iItemID = 637; break; // KnecklaceOfLightPro
                case 7: if (iDice(1, 25) == 13) iItemID = 638; break; // KnecklaceOfFirePro
            }
            break;

        case 18: // Zombie
        case 22: // Amphis
            switch (iDice(1, 4))
            {
                case 1: if (iDice(1, 65) == 13) iItemID = 613; break; // SwordofMedusa
                case 2: if (iDice(1, 15) == 13) iItemID = 639; break; // KnecklaceOfPoisonPro
                case 3: if (iDice(1, 30) == 13) iItemID = 641; break; // KnecklaceOfMedusa
                case 4: if (iDice(1, 25) == 13) iItemID = 640; break; // KnecklaceOfSufferent
            }
            break;

        case 12: // Stone-Golem
            switch (iDice(1, 5))
            {
                case 1: if (iDice(1, 20) == 13) iItemID = 738; break; // BerserkWand(MS.10)
                case 2: if (iDice(1, 30) == 13) iItemID = 621; break; // MerienPlateMailM
                case 3: if (iDice(1, 30) == 13) iItemID = 622; break; // MerienPlateMailW
                case 4: if (iDice(1, 15) == 13) iItemID = 644; break; // KnecklaceOfAirEle
                case 5: if (iDice(1, 15) == 13) iItemID = 647; break; // KnecklaceOfStoneGolem
            }
            break;

        case 23: // Clay-Golem
            switch (iDice(1, 4))
            {
                case 1: if (iDice(1, 20) == 13) iItemID = 738; break; // BerserkWand(MS.10)	
                case 2: if (iDice(1, 30) == 13) iItemID = 621; break; // MerienPlateMailM
                case 3: if (iDice(1, 30) == 13) iItemID = 622; break; // MerienPlateMailW
                case 4: if (iDice(1, 15) == 13) iItemID = 647; break; // KnecklaceOfStoneGolem
            }
            break;

        case 32: // Unicorn
            switch (iDice(1, 4))
            {
                case 1: if (iDice(1, 30) == 13) iItemID = 620; break; // MerienShield	
                case 2: if (iDice(1, 30) == 13) iItemID = 621; break; // MerienPlateMailM
                case 3: if (iDice(1, 30) == 13) iItemID = 622; break; // MerienPlateMailW
                case 4: if (iDice(1, 15) == 13) iItemID = 644; break; // KnecklaceOfAirEle
            }
            break;

        case 33: // WereWolf
        case 48: // Stalker
            switch (iDice(1, 4))
            {
                case 1: if (iDice(1, 20) == 13) iItemID = 852; break; // CancelManual
                case 2: if (iDice(1, 20) == 13) iItemID = 857; break; // I.M.CManual
                case 3: if (iDice(1, 20) == 13) iItemID = 853; break; // E.S.W Manual
                case 4: iItemID = 620; break; // MerienShield
            }
            break;

        case 30: // Liche
            switch (iDice(1, 6))
            {
                case 1: if (iDice(1, 15) == 13) iItemID = 852; break; // CancelManual
                case 2: iItemID = 380; break; // IceStormManual
                case 3: if (iDice(1, 15) == 13) iItemID = 853; break; // E.S.W Manual
                case 4: if (iDice(1, 30) == 13) iItemID = 643; break; // KnecklaceOfIceEle	
                case 5: if (iDice(1, 15) == 13) iItemID = 648; break; // NecklaceOfLiche
                case 6: if (iDice(1, 20) == 13) iItemID = 734; break; // RingOfArcmage
            }
            break;

        case 31: // Demon 
            switch (iDice(1, 8))
            {
                case 1: if (iDice(1, 5) == 3) iItemID = 382; break; // BloodyShockW.Manual
                case 2: iItemID = 491; break; // BloodAxe
                case 3: if (iDice(1, 5) == 3) iItemID = 490; break; // BloodSword
                case 4: iItemID = 492; break; // BloodRapier
                case 5: if (iDice(1, 5) == 3) iItemID = 381; break; // MassFireStrikeManual
                case 6: if (iDice(1, 15) == 3) iItemID = 633; break; // RingofDemonpower
                case 7: if (iDice(1, 10) == 3) iItemID = 645; break; // KnecklaceOfEfreet
                case 8: if (iDice(1, 15) == 3) iItemID = 616; break; // DemonSlayer
            }
            break;

        case 52: // Gagoyle
            switch (iDice(1, 11))
            {
                case 1: if (iDice(1, 5) == 3) iItemID = 382; break; // BloodyShockW.Manual	
                case 2: if (iDice(1, 15) == 13) iItemID = 610; break; // XelimaBlade	
                case 3: if (iDice(1, 15) == 13) iItemID = 611; break; // XelimaAxe	
                case 4: if (iDice(1, 15) == 13) iItemID = 612; break; // XelimaRapier
                case 5: if (iDice(1, 5) == 3) iItemID = 381; break; // MassFireStrikeManual
                case 6: if (iDice(1, 15) == 13) iItemID = 633; break; // RingofDemonpower
                case 7: if (iDice(1, 10) == 3) iItemID = 645; break; // KnecklaceOfEfreet
                case 8: if (iDice(1, 30) == 13) iItemID = 630; break; // RingoftheXelima	
                case 9: if (iDice(1, 40) == 13) iItemID = 631; break; // RingoftheAbaddon
                case 10: if (iDice(1, 20) == 13) iItemID = 735; break; // RingOfDragonpower
                case 11: if (iDice(1, 30) == 13) iItemID = 20; break; // Excalibur
            }
            break;

        case 53: // Beholder
            if (iDice(1, 10) == 11) iItemID = 380; break; // IceStormManual	
            break;

        case 54: // Dark-Elf
            switch (iDice(1, 4))
            {
                case 1: if (iDice(1, 20) == 13) iItemID = 618; break; // DarkElfBow	
                case 2: if (iDice(1, 15) == 13) iItemID = 958; break; // DrowBoots
                case 3: if (iDice(1, 15) == 11) iItemID = 956; break; // DrowChainM
                case 4: if (iDice(1, 15) == 11) iItemID = 957; break; // DrowChainW
            }
            break;

        case 63: // Frost
            switch (iDice(1, 3))
            {
                case 1: if (iDice(1, 20) == 13) iItemID = 943; break; // IceAxe	
                case 2: if (iDice(1, 20) == 13) iItemID = 942; break; // IceHammer
                case 3: if (iDice(1, 30) == 11) iItemID = 732; break; // BerserkWand(MS.20)
            }
            break;

        case 79: // Nizie
            switch (iDice(1, 3))
            {
                case 1: if (iDice(1, 20) == 13) iItemID = 943; break; // IceAxe	
                case 2: if (iDice(1, 20) == 13) iItemID = 942; break; // IceHammer
                case 3: if (iDice(1, 30) == 11) iItemID = 732; break; // BerserkWand(MS.20)
            }
            break;

        case 70: // Barlog
            switch (iDice(1, 3))
            {
                case 1: if (iDice(1, 5) == 3) iItemID = 382; break; // BloodyShockW.Manual
                case 2: if (iDice(1, 5) == 3) iItemID = 381; break; // MassFireStrikeManual
                case 3: if (iDice(1, 30) == 13) iItemID = 732; break; // BerserkWand(MS.20)
            }
            break;

        case 71: // Centaurus
            switch (iDice(1, 2))
            {
                case 1: if (iDice(1, 20) == 13) iItemID = 735; break; // RingOfDragonpower
                case 2: if (iDice(1, 30) == 13) iItemID = 732; break; // BerserkWand(MS.20)
            }
            break;

        case 59: // Ettin
            switch (iDice(1, 3))
            {
                case 1: if (iDice(1, 20) == 13) iItemID = 735; break; // RingOfDragonpower
                case 2: if (iDice(1, 10) == 13) iItemID = 853; break; // E.S.W.Manual
                case 3: if (iDice(1, 7) == 3) iItemID = 382; break; // BloodyShockW.Manual
            }
            break;

    }

    if (iItemID == 0)
        return false;
    else return true;
}

/*
void CGame::StormBringer(int iClientH, short dX, short dY)
{
    char cOwnerType;
    short sOwner, sAppr2, sAttackerWeapon;
    int  iDamage, iTemp, iV1, iV2, iV3;

    //ArchAngel Fix

    if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND] != -1) {
        m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwner, &cOwnerType, dX, dY);

        iTemp = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
        sAppr2 = (short)((m_pClientList[iClientH]->m_sAppr2 & 0xF000) >> 12);

        if (memcmp(m_pClientList[iClientH]->m_pItemList[iTemp]->m_cName, "StormBringer", 12) == 0){

            switch (cOwnerType) {
            case DEF_OWNERTYPE_PLAYER:
                if (m_pClientList[sOwner]->m_iAdminUserLevel < 3) {
                    if (sAppr2 != 0) {
                        iV1 = m_pClientList[iClientH]->m_cAttackDiceThrow_L;
                        iV2 = m_pClientList[iClientH]->m_cAttackDiceRange_L;
                        iV3 = m_pClientList[iClientH]->m_cAttackBonus_L;

                        if (m_pClientList[iClientH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_BERSERK ] != 0){
                            iDamage = iDice(iV1*2,iV2*2)+iV3;
                        }
                        else{
                            iDamage = iDice(iV1,iV2)+iV3;
                        }

                        m_pClientList[sOwner]->m_iHP -= iDamage;
                        if (m_pClientList[sOwner]->m_iHP <= 0){
                            sAttackerWeapon = 1;
                            m_pClientList[sOwner]->m_iHP = 0;

                            m_pClientList[sOwner]->m_bIsKilled = true;
                            m_pClientList[sOwner]->m_iLastDamage = iDamage;
                            SendNotifyMsg(0, sOwner, DEF_NOTIFY_HP, 0, 0, 0, 0);
                            SendEventToNearClient_TypeA(sOwner, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDYING, iDamage, sAttackerWeapon, 0);
                            m_pMapList[m_pClientList[sOwner]->m_cMapIndex]->ClearOwner(14, sOwner, DEF_OWNERTYPE_PLAYER, m_pClientList[sOwner]->m_sX, m_pClientList[sOwner]->m_sY);
                            m_pMapList[m_pClientList[sOwner]->m_cMapIndex]->SetDeadOwner(sOwner, DEF_OWNERTYPE_PLAYER, m_pClientList[sOwner]->m_sX, m_pClientList[sOwner]->m_sY);
                        }
                        else{
                            SendNotifyMsg(0, sOwner, DEF_NOTIFY_HP, 0, 0, 0, 0);
                            SendEventToNearClient_TypeA(sOwner, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);
                        }
                    }
                }
                break;

            case DEF_OWNERTYPE_NPC:
                if (sAppr2 != 0) {
                    if (m_pNpcList[sOwner]->m_cSize == 0){
                        iV1 = m_pClientList[iClientH]->m_cAttackDiceThrow_SM;
                        iV2 = m_pClientList[iClientH]->m_cAttackDiceRange_SM;
                        iV3 = m_pClientList[iClientH]->m_cAttackBonus_SM;
                    }
                    else if (m_pNpcList[sOwner]->m_cSize == 1){
                        iV1 = m_pClientList[iClientH]->m_cAttackDiceThrow_L;
                        iV2 = m_pClientList[iClientH]->m_cAttackDiceRange_L;
                        iV3 = m_pClientList[iClientH]->m_cAttackBonus_L;
                    }

                    if (m_pClientList[iClientH]->m_cMagicEffectStatus[ DEF_MAGICTYPE_BERSERK ] != 0){
                        iDamage = iDice(iV1*2,iV2*2)+iV3;
                    }
                    else{
                        iDamage = iDice(iV1,iV2)+iV3;
                    }

                    m_pNpcList[sOwner]->m_iHP -= iDamage;
                    if (m_pNpcList[sOwner]->m_iHP <= 0){
                        sAttackerWeapon = 1;
                        m_pNpcList[sOwner]->m_iHP = 0;

                        m_pNpcList[sOwner]->m_sBehaviorTurnCount = 0;
                        m_pNpcList[sOwner]->m_cBehavior = DEF_BEHAVIOR_DEAD;
                        m_pNpcList[sOwner]->m_dwDeadTime = timeGetTime();
                        SendEventToNearClient_TypeA(sOwner, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTDYING, iDamage, sAttackerWeapon, 0);
                        m_pMapList[m_pNpcList[sOwner]->m_cMapIndex]->ClearOwner(10, sOwner, DEF_OWNERTYPE_NPC, m_pNpcList[sOwner]->m_sX, m_pNpcList[sOwner]->m_sY);
                        m_pMapList[m_pNpcList[sOwner]->m_cMapIndex]->SetDeadOwner(sOwner, DEF_OWNERTYPE_NPC, m_pNpcList[sOwner]->m_sX, m_pNpcList[sOwner]->m_sY);
                    }
                    else{
                        SendEventToNearClient_TypeA(sOwner, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, 0, 0);
                    }
                }
                break;
            }
        }
    }
}*/

bool CGame::_bCheckCharacterData(int iClientH)
{
    int i;

    if ((m_pClientList[iClientH]->m_iStr > m_sCharStatLimit) || (m_pClientList[iClientH]->m_iVit > m_sCharStatLimit) || (m_pClientList[iClientH]->m_iDex > m_sCharStatLimit) ||
        (m_pClientList[iClientH]->m_iMag > m_sCharStatLimit) || (m_pClientList[iClientH]->m_iInt > m_sCharStatLimit) || (m_pClientList[iClientH]->m_iCharisma > m_sCharStatLimit))
    {
        try
        {
            wsprintf(G_cTxt, "Packet Editing: (%s) Player: (%s) stat points are greater then server accepts.", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
            PutHackLogFileList(G_cTxt);
            return false;
        }
        catch (...)
        {

        }
    }

    if ((m_pClientList[iClientH]->m_iLevel > m_sMaxPlayerLevel) && (m_pClientList[iClientH]->m_iAdminUserLevel == 0))
    {
        try
        {
            wsprintf(G_cTxt, "Packet Editing: (%s) Player: (%s) level above max server level.", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
            PutHackLogFileList(G_cTxt);
            return false;
        }
        catch (...)
        {

        }
    }

    if (m_pClientList[iClientH]->m_iExp < 0)
    {
        try
        {
            wsprintf(G_cTxt, "Packet Editing: (%s) Player: (%s) experience is below 0 - (Exp:%d).", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_iExp);
            PutHackLogFileList(G_cTxt);
            return false;
        }
        catch (...)
        {

        }
    }

    if ((m_pClientList[iClientH]->m_iHP > iGetMaxHP(iClientH)) && (m_pClientList[iClientH]->m_iAdminUserLevel == 0))
    {
        try
        {
            if (m_pClientList[iClientH]->m_pItemList[(m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND])] != 0)
            {
                if ((m_pClientList[iClientH]->m_pItemList[(m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND])]->m_sIDnum == 492) || (m_pClientList[iClientH]->m_pItemList[(m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND])]->m_sIDnum == 491))
                {
                    if (m_pClientList[iClientH]->m_iHP > (4 * (iGetMaxHP(iClientH) / 5)))
                    {

                    }
                }
            }
            else if (m_pClientList[iClientH]->m_pItemList[(m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND])] != 0)
            {
                if ((m_pClientList[iClientH]->m_pItemList[(m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND])]->m_sIDnum == 490))
                {
                    if (m_pClientList[iClientH]->m_iHP > (4 * (iGetMaxHP(iClientH) / 5)))
                    {

                    }
                }
            }
            else
            {
                wsprintf(G_cTxt, "Packet Editing: (%s) Player: (%s) HP: current/maximum (%d/%d).", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_iHP, iGetMaxHP(iClientH));
                PutHackLogFileList(G_cTxt);
                return false;
            }
        }
        catch (...)
        {

        }
    }

    if ((m_pClientList[iClientH]->m_iMP > iGetMaxMP(iClientH)) && (m_pClientList[iClientH]->m_iAdminUserLevel == 0))
    {
        try
        {
            wsprintf(G_cTxt, "Packet Editing: (%s) Player: (%s) MP: current/maximum (%d/%d).", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_iMP, iGetMaxMP(iClientH));
            PutHackLogFileList(G_cTxt);
            return false;
        }
        catch (...)
        {

        }
    }

    if ((m_pClientList[iClientH]->m_iSP > iGetMaxSP(iClientH)) && (m_pClientList[iClientH]->m_iAdminUserLevel == 0))
    {
        try
        {
            wsprintf(G_cTxt, "Packet Editing: (%s) Player: (%s) SP: current/maximum (%d/%d).", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_iSP, iGetMaxSP(iClientH));
            PutHackLogFileList(G_cTxt);
            return false;
        }
        catch (...)
        {

        }
    }

    try
    {
        for (i = 0; i < DEF_MAXBANNED; i++)
        {
            if (strlen(m_stBannedList[i].m_cBannedIPaddress) == 0) break; //No more GM's on list
            if ((strlen(m_stBannedList[i].m_cBannedIPaddress)) == (strlen(m_pClientList[iClientH]->m_cIPaddress)))
            {
                if (memcmp(m_stBannedList[i].m_cBannedIPaddress, m_pClientList[iClientH]->m_cIPaddress, strlen(m_pClientList[iClientH]->m_cIPaddress)) == 0)
                {
                    wsprintf(G_cTxt, "Client Rejected: Banned: (%s)", m_pClientList[iClientH]->m_cIPaddress);
                    log->info(G_cTxt);
                    return false;
                }
                else
                {

                }
            }
        }
    }
    catch (...)
    {

    }

    return true;
}

void CGame::RemoveOccupyFlags(int iMapIndex)
{
    uint32_t dwTime = timeGetTime();
    int i;
    short dX, dY;
    int iDynamicObjectIndex;
    class COccupyFlag * iOccupyFlagIndex;
    class CTile * pTile;
    int iy, ix;

    if (m_pMapList[iMapIndex] == 0) return;
    for (i = 1; i < DEF_MAXOCCUPYFLAG; i++)
        if (m_pMapList[iMapIndex]->m_pOccupyFlag[i] != false) return;
    if (m_pMapList[iMapIndex]->m_pOccupyFlag[i] != false)
    {
        dX = m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_sX;
        dY = m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_sY;
        pTile = (class CTile *)(m_pMapList[iMapIndex]->m_pTile + dX + dY * m_pMapList[iMapIndex]->m_sSizeY);
        m_pMapList[iMapIndex]->m_iTotalOccupyFlags--;
        iDynamicObjectIndex = m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_iDynamicObjectIndex;
        if (m_pDynamicObjectList[iDynamicObjectIndex] == 0) return;

        SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, DEF_MSGTYPE_REJECT, m_pDynamicObjectList[iDynamicObjectIndex]->m_cMapIndex,
            m_pDynamicObjectList[iDynamicObjectIndex]->m_sX, m_pDynamicObjectList[iDynamicObjectIndex]->m_sY,
            m_pDynamicObjectList[iDynamicObjectIndex]->m_sType, iDynamicObjectIndex, 0);

        m_pMapList[m_pDynamicObjectList[iDynamicObjectIndex]->m_cMapIndex]->SetDynamicObject(0, 0, m_pDynamicObjectList[iDynamicObjectIndex]->m_sX, m_pDynamicObjectList[iDynamicObjectIndex]->m_sY, dwTime);

        iOccupyFlagIndex = m_pMapList[iMapIndex]->m_pOccupyFlag[i];

        if (m_pDynamicObjectList[iDynamicObjectIndex] == 0)
        {
            for (ix = dX - 2; ix <= dX + 2; ix++)
                for (iy = dY - 2; iy <= dY + 2; iy++)
                {
                    pTile = (class CTile *)(m_pMapList[iMapIndex]->m_pTile + ix + iy * m_pMapList[iMapIndex]->m_sSizeY);
                    pTile->m_sOwner = 0;
                }
        }
    }
}

void CGame::CritInc(int iClientH)
{
    int iMaxSuperAttack;
    int DEFAULT_CRIT_INC_VALUE = 5;

    iMaxSuperAttack = (m_pClientList[iClientH]->m_iLevel / 10);

    m_pClientList[iClientH]->m_iSuperAttackLeft += DEFAULT_CRIT_INC_VALUE;
    if (m_pClientList[iClientH]->m_iSuperAttackLeft > iMaxSuperAttack)
    {
        m_pClientList[iClientH]->m_iSuperAttackLeft = iMaxSuperAttack;
    }
    SendNotifyMsg(0, iClientH, DEF_NOTIFY_SUPERATTACKLEFT, 0, 0, 0, 0);
}

void CGame::AddGizon(int iClientH)
{
    m_pClientList[iClientH]->m_iGizonItemUpgradeLeft++;
    SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIZONITEMUPGRADELEFT, m_pClientList[iClientH]->m_iGizonItemUpgradeLeft, 0, 0, 0);

    m_pClientList[iClientH]->m_iNextLevelExp = m_iLevelExpTable[m_pClientList[iClientH]->m_iLevel + 1];
    m_pClientList[iClientH]->m_iExp = m_iLevelExpTable[m_pClientList[iClientH]->m_iLevel] + 1;
}

void CGame::ForceRecallProcess()
{
    int i;
    int iMapSide = 0;

    uint32_t * dwp, dwTime;

    dwTime = timeGetTime();

    for (i = 1; i < DEF_MAXCLIENTS; i++)
    {
        if (m_pClientList[i] != 0)
        {
            if (m_pClientList[i]->m_bIsInitComplete == true)
            {
                //force recall in enemy buidlings at crusade
                iMapSide = iGetMapLocationSide(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName);
                if ((memcmp(m_pClientList[i]->m_cLocation, "are", 3) == 0) && (iMapSide == 2) && (m_pClientList[i]->m_iAdminUserLevel == 0) && (m_bIsCrusadeMode == true))
                {
                    RequestTeleportHandler(i, "2   ", "aresden", -1, -1);
                }
                if ((memcmp(m_pClientList[i]->m_cLocation, "elv", 3) == 0) && (iMapSide == 1) && (m_pClientList[i]->m_iAdminUserLevel == 0) && (m_bIsCrusadeMode == true))
                {
                    RequestTeleportHandler(i, "2   ", "elvine", -1, -1);
                }

                //remove mim in building
                if ((memcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "wrhus", 5) == 0)
                    || (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "gshop_1") == 0)
                    || (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "bsmith_1") == 0)
                    || (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "cath_1") == 0)
                    || (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "CmdHall_1") == 0)
                    || (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "cityhall_1") == 0)
                    || (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "gshop_2") == 0)
                    || (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "bsmith_2") == 0)
                    || (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "cath_2") == 0)
                    || (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "CmdHall_2") == 0)
                    || (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "cityhall_2") == 0)
                    || (memcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "wzdtwr", 6) == 0)
                    || (memcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cName, "gldhall", 7) == 0))
                {
                    //SetIllusionFlag(i, DEF_OWNERTYPE_PLAYER, false);
                    if (m_pClientList[i]->m_iStatus & 0x00200000)
                    {
                        SetIllusionMovementFlag(i, DEF_OWNERTYPE_PLAYER, false);
                        bRemoveFromDelayEventList(i, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_CONFUSE);
                        bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_CONFUSE, dwTime + 2, i, DEF_OWNERTYPE_PLAYER, 0, 0, 0, 4, 0, 0);
                    }
                }
            }
            //check gizon errors
            if (m_pClientList[i]->m_iLevel < 180)
            {
                if (m_pClientList[i]->m_iGizonItemUpgradeLeft > 0)
                {
                    m_pClientList[i]->m_iGizonItemUpgradeLeft = 0;
                }
            }
        }
    }
}

void CGame::SkillCheck(int sTargetH)
{
    //magic
    while ((m_pClientList[sTargetH]->m_iMag * 2) < m_pClientList[sTargetH]->m_cSkillMastery[4])
    {
        m_pClientList[sTargetH]->m_cSkillMastery[4]--;
    }
    //hand attack
    while ((m_pClientList[sTargetH]->m_iStr * 2) < m_pClientList[sTargetH]->m_cSkillMastery[5])
    {
        m_pClientList[sTargetH]->m_cSkillMastery[5]--;
    }
    //hammer
    while ((m_pClientList[sTargetH]->m_iDex * 2) < m_pClientList[sTargetH]->m_cSkillMastery[14])
    {
        m_pClientList[sTargetH]->m_cSkillMastery[14]--;
    }
    //shield
    while ((m_pClientList[sTargetH]->m_iDex * 2) < m_pClientList[sTargetH]->m_cSkillMastery[11])
    {
        m_pClientList[sTargetH]->m_cSkillMastery[11]--;
    }
    //axe
    while ((m_pClientList[sTargetH]->m_iDex * 2) < m_pClientList[sTargetH]->m_cSkillMastery[10])
    {
        m_pClientList[sTargetH]->m_cSkillMastery[10]--;
    }
    //fencing
    while ((m_pClientList[sTargetH]->m_iDex * 2) < m_pClientList[sTargetH]->m_cSkillMastery[9])
    {
        m_pClientList[sTargetH]->m_cSkillMastery[9]--;
    }
    //LS
    while ((m_pClientList[sTargetH]->m_iDex * 2) < m_pClientList[sTargetH]->m_cSkillMastery[8])
    {
        m_pClientList[sTargetH]->m_cSkillMastery[8]--;
    }
    //SS
    while ((m_pClientList[sTargetH]->m_iDex * 2) < m_pClientList[sTargetH]->m_cSkillMastery[7])
    {
        m_pClientList[sTargetH]->m_cSkillMastery[7]--;
    }
    //archery
    while ((m_pClientList[sTargetH]->m_iDex * 2) < m_pClientList[sTargetH]->m_cSkillMastery[6])
    {
        m_pClientList[sTargetH]->m_cSkillMastery[6]--;
    }
    //staff
    while ((m_pClientList[sTargetH]->m_iMag * 2) < m_pClientList[sTargetH]->m_cSkillMastery[21])
    {
        m_pClientList[sTargetH]->m_cSkillMastery[21]--;
    }
    //alc
    while ((m_pClientList[sTargetH]->m_iInt * 2) < m_pClientList[sTargetH]->m_cSkillMastery[12])
    {
        m_pClientList[sTargetH]->m_cSkillMastery[12]--;
    }
    //manu
    while ((m_pClientList[sTargetH]->m_iStr * 2) < m_pClientList[sTargetH]->m_cSkillMastery[13])
    {
        m_pClientList[sTargetH]->m_cSkillMastery[13]--;
    }
    //pr
    while ((m_pClientList[sTargetH]->m_iVit * 2) < m_pClientList[sTargetH]->m_cSkillMastery[23])
    {
        m_pClientList[sTargetH]->m_cSkillMastery[23]--;
    }
    //pc
    while ((m_pClientList[sTargetH]->m_iInt * 2) < m_pClientList[sTargetH]->m_cSkillMastery[19])
    {
        m_pClientList[sTargetH]->m_cSkillMastery[19]--;
    }
    //farming
    while ((m_pClientList[sTargetH]->m_iInt * 2) < m_pClientList[sTargetH]->m_cSkillMastery[2])
    {
        m_pClientList[sTargetH]->m_cSkillMastery[2]--;
    }
    //fishing
    while ((m_pClientList[sTargetH]->m_iDex * 2) < m_pClientList[sTargetH]->m_cSkillMastery[1])
    {
        m_pClientList[sTargetH]->m_cSkillMastery[1]--;
    }
    //mining
    while ((m_pClientList[sTargetH]->m_iStr * 2) < m_pClientList[sTargetH]->m_cSkillMastery[0])
    {
        m_pClientList[sTargetH]->m_cSkillMastery[0]--;
    }
}

bool CGame::IsEnemyZone(int i)
{
    if (memcmp(m_pClientList[i]->m_cLocation, "elv", 3) == 0)
    {
        if ((strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cLocationName, "aresden") == 0) || (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cLocationName, "aresdend1") == 0) || (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cLocationName, "areuni") == 0) || (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cLocationName, "huntzone2") == 0) || (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cLocationName, "huntzone4") == 0))
        {
            return true;
        }
    }
    else if (memcmp(m_pClientList[i]->m_cLocation, "are", 3) == 0)
    {
        if ((strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cLocationName, "elvine") == 0) || (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cLocationName, "elvined1") == 0) || (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cLocationName, "elvuni") == 0) || (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cLocationName, "huntzone1") == 0) || (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cLocationName, "huntzone3") == 0))
        {
            return true;
        }
    }
    return false;
}

void CGame::SetSkillAll(int iClientH, char * pData, uint32_t dwMsgSize)
//SetSkillAll Acidx Command,  Added July 04, 2005 INDEPENDENCE BABY Fuck YEA
{
    if (m_pClientList[iClientH] == nullptr) return;
    //Magic
    if (m_pClientList[iClientH]->m_cSkillMastery[4] < 100)
    {
        // now we add skills
        m_pClientList[iClientH]->m_cSkillMastery[4] = m_pClientList[iClientH]->m_iMag * 2;
        if (m_pClientList[iClientH]->m_cSkillMastery[4] > 100)
        {
            m_pClientList[iClientH]->m_cSkillMastery[4] = 100;
        }
        if (m_pClientList[iClientH]->m_iMag > 50)
        {
            m_pClientList[iClientH]->m_cSkillMastery[4] = 100;
        }
        //Send a notify to update the client
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILL, 4, m_pClientList[iClientH]->m_cSkillMastery[4], 0, 0);

    }
    //LongSword
    if (m_pClientList[iClientH]->m_cSkillMastery[8] < 100)
    {
        // now we add skills
        m_pClientList[iClientH]->m_cSkillMastery[8] = m_pClientList[iClientH]->m_iDex * 2;

        if (m_pClientList[iClientH]->m_cSkillMastery[8] > 100)
        {
            m_pClientList[iClientH]->m_cSkillMastery[8] = 100;
        }
        if (m_pClientList[iClientH]->m_iDex > 50)
        {
            m_pClientList[iClientH]->m_cSkillMastery[8] = 100;
        }
        //Send a notify to update the client
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILL, 8, m_pClientList[iClientH]->m_cSkillMastery[8], 0, 0);

    }
    //Hammer
    if (m_pClientList[iClientH]->m_cSkillMastery[14] < 100)
    {
        // now we add skills
        m_pClientList[iClientH]->m_cSkillMastery[14] = m_pClientList[iClientH]->m_iDex * 2;
        if (m_pClientList[iClientH]->m_cSkillMastery[14] > 100)
        {
            m_pClientList[iClientH]->m_cSkillMastery[14] = 100;
        }
        if (m_pClientList[iClientH]->m_iDex > 50)
        {
            m_pClientList[iClientH]->m_cSkillMastery[14] = 100;
        }
        //Send a notify to update the client
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILL, 14, m_pClientList[iClientH]->m_cSkillMastery[14], 0, 0);

    }
    //Axes
    if (m_pClientList[iClientH]->m_cSkillMastery[10] < 100)
    {
        // now we add skills
        m_pClientList[iClientH]->m_cSkillMastery[10] = m_pClientList[iClientH]->m_iDex * 2;
        if (m_pClientList[iClientH]->m_cSkillMastery[10] > 100)
        {
            m_pClientList[iClientH]->m_cSkillMastery[10] = 100;
        }
        if (m_pClientList[iClientH]->m_iDex > 50)
        {
            m_pClientList[iClientH]->m_cSkillMastery[10] = 100;
        }
        //Send a notify to update the client
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILL, 10, m_pClientList[iClientH]->m_cSkillMastery[10], 0, 0);

    }
    //hand attack
    if (m_pClientList[iClientH]->m_cSkillMastery[5] < 100)
    {
        // now we add skills
        m_pClientList[iClientH]->m_cSkillMastery[5] = m_pClientList[iClientH]->m_iStr * 2;
        if (m_pClientList[iClientH]->m_cSkillMastery[5] > 100)
        {
            m_pClientList[iClientH]->m_cSkillMastery[5] = 100;
        }
        if (m_pClientList[iClientH]->m_iStr > 50)
        {
            m_pClientList[iClientH]->m_cSkillMastery[5] = 100;
        }
        //Send a notify to update the client
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILL, 5, m_pClientList[iClientH]->m_cSkillMastery[5], 0, 0);

    }
    //ShortSword
    if (m_pClientList[iClientH]->m_cSkillMastery[7] < 100)
    {
        // now we add skills
        m_pClientList[iClientH]->m_cSkillMastery[7] = m_pClientList[iClientH]->m_iDex * 2;
        if (m_pClientList[iClientH]->m_cSkillMastery[7] > 100)
        {
            m_pClientList[iClientH]->m_cSkillMastery[7] = 100;
        }
        if (m_pClientList[iClientH]->m_iDex > 50)
        {
            m_pClientList[iClientH]->m_cSkillMastery[7] = 100;
        }
        //Send a notify to update the client
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILL, 7, m_pClientList[iClientH]->m_cSkillMastery[7], 0, 0);

    }
    //archery
    if (m_pClientList[iClientH]->m_cSkillMastery[6] < 100)
    {
        // now we add skills
        m_pClientList[iClientH]->m_cSkillMastery[6] = m_pClientList[iClientH]->m_iDex * 2;
        if (m_pClientList[iClientH]->m_cSkillMastery[6] > 100)
        {
            m_pClientList[iClientH]->m_cSkillMastery[6] = 100;
        }
        if (m_pClientList[iClientH]->m_iDex > 50)
        {
            m_pClientList[iClientH]->m_cSkillMastery[6] = 100;
        }
        //Send a notify to update the client
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILL, 6, m_pClientList[iClientH]->m_cSkillMastery[6], 0, 0);

    }
    //Fencing
    if (m_pClientList[iClientH]->m_cSkillMastery[9] < 100)
    {
        // now we add skills
        m_pClientList[iClientH]->m_cSkillMastery[9] = m_pClientList[iClientH]->m_iDex * 2;
        if (m_pClientList[iClientH]->m_cSkillMastery[9] > 100)
        {
            m_pClientList[iClientH]->m_cSkillMastery[9] = 100;
        }
        if (m_pClientList[iClientH]->m_iDex > 50)
        {
            m_pClientList[iClientH]->m_cSkillMastery[9] = 100;
        }
        //Send a notify to update the client
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILL, 9, m_pClientList[iClientH]->m_cSkillMastery[9], 0, 0);

    }
    //Staff Attack
    if (m_pClientList[iClientH]->m_cSkillMastery[21] < 100)
    {
        // now we add skills
        m_pClientList[iClientH]->m_cSkillMastery[21] = m_pClientList[iClientH]->m_iInt * 2;
        if (m_pClientList[iClientH]->m_cSkillMastery[21] > 100)
        {
            m_pClientList[iClientH]->m_cSkillMastery[21] = 100;
        }
        if (m_pClientList[iClientH]->m_iInt > 50)
        {
            m_pClientList[iClientH]->m_cSkillMastery[21] = 100;
        }
        //Send a notify to update the client
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILL, 21, m_pClientList[iClientH]->m_cSkillMastery[21], 0, 0);

    }
    //shield
    if (m_pClientList[iClientH]->m_cSkillMastery[11] < 100)
    {
        // now we add skills
        m_pClientList[iClientH]->m_cSkillMastery[11] = m_pClientList[iClientH]->m_iDex * 2;
        if (m_pClientList[iClientH]->m_cSkillMastery[11] > 100)
        {
            m_pClientList[iClientH]->m_cSkillMastery[11] = 100;
        }
        if (m_pClientList[iClientH]->m_iDex > 50)
        {
            m_pClientList[iClientH]->m_cSkillMastery[11] = 100;
        }
        //Send a notify to update the client
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILL, 11, m_pClientList[iClientH]->m_cSkillMastery[11], 0, 0);

    }
    //mining
    if (m_pClientList[iClientH]->m_cSkillMastery[0] < 100)
    {
        // now we add skills
        m_pClientList[iClientH]->m_cSkillMastery[0] = 100;
        //Send a notify to update the client
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILL, 0, m_pClientList[iClientH]->m_cSkillMastery[0], 0, 0);

    }
    //fishing
    if (m_pClientList[iClientH]->m_cSkillMastery[1] < 100)
    {
        // now we add skills
        m_pClientList[iClientH]->m_cSkillMastery[1] = 100;
        //Send a notify to update the client
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILL, 1, m_pClientList[iClientH]->m_cSkillMastery[1], 0, 0);

    }
    //farming
    if (m_pClientList[iClientH]->m_cSkillMastery[2] < 100)
    {
        // now we add skills
        m_pClientList[iClientH]->m_cSkillMastery[2] = 100;
        //Send a notify to update the client
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILL, 2, m_pClientList[iClientH]->m_cSkillMastery[2], 0, 0);

    }
    //alchemy
    if (m_pClientList[iClientH]->m_cSkillMastery[12] < 100)
    {
        // now we add skills
        m_pClientList[iClientH]->m_cSkillMastery[12] = 100;
        //Send a notify to update the client
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILL, 12, m_pClientList[iClientH]->m_cSkillMastery[12], 0, 0);

    }
    //manufacturing
    if (m_pClientList[iClientH]->m_cSkillMastery[13] < 100)
    {
        // now we add skills
        m_pClientList[iClientH]->m_cSkillMastery[13] = 100;
        //Send a notify to update the client
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILL, 13, m_pClientList[iClientH]->m_cSkillMastery[13], 0, 0);

    }
    //poison resistance
    if (m_pClientList[iClientH]->m_cSkillMastery[23] < 20)
    {
        // now we add skills
        m_pClientList[iClientH]->m_cSkillMastery[23] = 20;
        //Send a notify to update the client
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILL, 23, m_pClientList[iClientH]->m_cSkillMastery[23], 0, 0);

    }
    //pretend corpse
    if (m_pClientList[iClientH]->m_cSkillMastery[19] < 100)
    {
        // now we add skills
        m_pClientList[iClientH]->m_cSkillMastery[19] = m_pClientList[iClientH]->m_iInt * 2;
        if (m_pClientList[iClientH]->m_cSkillMastery[19] > 100)
        {
            m_pClientList[iClientH]->m_cSkillMastery[19] = 100;
        }
        if (m_pClientList[iClientH]->m_iInt > 50)
        {
            m_pClientList[iClientH]->m_cSkillMastery[19] = 100;
        }
        //Send a notify to update the client
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILL, 19, m_pClientList[iClientH]->m_cSkillMastery[19], 0, 0);

    }
    //magic resistance
    if (m_pClientList[iClientH]->m_cSkillMastery[3] < 20)
    {
        // now we add skills
        m_pClientList[iClientH]->m_cSkillMastery[3] = 20;
        //Send a notify to update the client
        SendNotifyMsg(0, iClientH, DEF_NOTIFY_SKILL, 3, m_pClientList[iClientH]->m_cSkillMastery[3], 0, 0);

    }
}

//Angel Code By SlammeR(I dont know if it works)
/*void CGame::GetAngelMantleHandler(int iClientH,int iItemID,char * pString)
{
 int   i, iNum, iRet, iEraseReq;
 char  * cp, cData[256], cItemName[21];
 class CItem * pItem;
 uint32_t * dwp;
 short * sp;
 uint16_t  * wp;

    if (m_pClientList[iClientH] == nullptr) return;
    if (m_pClientList[iClientH]->m_iGizonItemUpgradeLeft < 5) return;
    if (m_pClientList[iClientH]->m_cSide == 0) return;
    if (_iGetItemSpaceLeft(iClientH) == 0) {
        SendItemNotifyMsg(iClientH,	DEF_NOTIFY_CANNOTCARRYMOREITEM, 0, 0);
        return;
    }

    //Prevents a crash if item dosent exist
    if (m_pItemConfigList[iItemID] == 0)  return;

    switch(iItemID) {
    //Angels
    case 908: //AngelicPandent(STR)
    case 909: //AngelicPandent(DEX)
        case 910: //AngelicPandent(INT)
        case 911: //AngelicPandent(MAG)
        if(m_pClientList[iClientH]->m_iGizonItemUpgradeLeft<5) return;
        m_pClientList[iClientH]->m_iGizonItemUpgradeLeft -= 5;
        break;


  default:
     return;
     break;
  }

  memset(cItemName, 0, sizeof(cItemName));
  memcpy(cItemName,m_pItemConfigList[iItemID]->m_cName,20);

  iNum = 1;
  for (i = 1; i <= iNum; i++)
  {
     pItem = new class CItem;
     if (_bInitItemAttr(pItem, cItemName) == false)
     {
        delete pItem;
     }
     else {

        if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == true) {
           if (m_pClientList[iClientH]->m_iCurWeightLoad < 0) m_pClientList[iClientH]->m_iCurWeightLoad = 0;

           wsprintf(G_cTxt, "(*) Get Angel : Char(%s) Player-Majestic-Points(%d) Angel Obtained(%s)", m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_iGizonItemUpgradeLeft, cItemName);
           PutLogFileList(G_cTxt);

           pItem->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
           pItem->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
           pItem->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
           pItem->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;

           dwp  = (uint32_t *)(cData + DEF_INDEX4_MSGID);
           *dwp = MSGID_NOTIFY;
           wp   = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
           *wp  = DEF_NOTIFY_ITEMOBTAINED;
           cp = (char *)(cData + DEF_INDEX2_MSGTYPE + 2);

           *cp = 1;
           cp++;

           memcpy(cp, pItem->m_cName, 20);
           cp += 20;

           dwp  = (uint32_t *)cp;
           *dwp = pItem->m_dwCount;
           cp += 4;

           *cp = pItem->m_cItemType;
           cp++;

           *cp = pItem->m_cEquipPos;
           cp++;

           *cp = (char)0;
           cp++;

           sp  = (short *)cp;
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

           sp  = (short *)cp;
           *sp = pItem->m_sSprite;
           cp += 2;

           sp  = (short *)cp;
           *sp = pItem->m_sSpriteFrame;
           cp += 2;

           *cp = pItem->m_cItemColor;
           cp++;

           *cp = (char)pItem->m_sItemSpecEffectValue2; // v1.41
           cp++;

           dwp = (uint32_t *)cp;
           *dwp = pItem->m_dwAttribute;
           cp += 4;
           //*cp = (char)(pItem->m_dwAttribute & 0x00000001); // Custom-Item??? ??
           //cp++;

           if (iEraseReq == 1) delete pItem;

           iRet = m_pClientList[iClientH]->send_msg(cData, 53);

           iCalcTotalWeight(iClientH);

           switch (iRet) {
           case DEF_XSOCKEVENT_QUENEFULL:
           case DEF_XSOCKEVENT_SOCKETERROR:
           case DEF_XSOCKEVENT_CRITICALERROR:
           case DEF_XSOCKEVENT_SOCKETCLOSED:
              DeleteClient(iClientH, true, true);
              return;
           }

           SendNotifyMsg(0, iClientH, DEF_NOTIFY_GIZONITEMUPGRADELEFT, m_pClientList[iClientH]->m_iGizonItemUpgradeLeft, 0, 0, 0);
        }
        else
        {
           delete pItem;

           iCalcTotalWeight(iClientH);

           dwp  = (uint32_t *)(cData + DEF_INDEX4_MSGID);
           *dwp = MSGID_NOTIFY;
           wp   = (uint16_t *)(cData + DEF_INDEX2_MSGTYPE);
           *wp  = DEF_NOTIFY_CANNOTCARRYMOREITEM;

           iRet = m_pClientList[iClientH]->send_msg(cData, 6);
           switch (iRet) {
           case DEF_XSOCKEVENT_QUENEFULL:
           case DEF_XSOCKEVENT_SOCKETERROR:
           case DEF_XSOCKEVENT_CRITICALERROR:
           case DEF_XSOCKEVENT_SOCKETCLOSED:

              DeleteClient(iClientH, true, true);
              return;
           }
        }
     }
   }
}*/

/*int CGame::iAngelEquip(int iClientH)
{
 int iTemp;
 CItem * cAngelTemp;
    iTemp = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LFINGER];
    cAngelTemp = m_pClientList[iClientH]->m_pItemList[iTemp];
    if ((iTemp != -1) && (cAngelTemp != 0)) {
        if(cAngelTemp->m_sIDnum >= 908){ //AngelicPandent(STR)
                if(cAngelTemp->m_sIDnum >= 909){ //AngelicPandent(DEX)
                if(cAngelTemp->m_sIDnum >= 910){ //AngelicPandent(INT)
                if(cAngelTemp->m_sIDnum >= 911){ //AngelicPandent(MAG)

                return cAngelTemp->m_sIDnum;
            } else {
                return 0;
                }
                }
                }
        }
    }
}*/

/*void CGame::CheckAngelUnequip(int iClientH,int iAngelID)
{
 int iTemp;
 CItem * cAngelTemp;

    iTemp = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LFINGER];
    cAngelTemp = m_pClientList[iClientH]->m_pItemList[iTemp];
    if ((iTemp != -1) && (cAngelTemp->m_sIDnum != iAngelID)) {
        cAngelTemp->m_sIDnum = iAngelID;
    }

}*/
