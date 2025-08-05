//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include "Game.h"
#include "string_utils.h"

void CGame::MobGenerator()
{
    int i, x, j, iNamingValue, iResult, iTotalMob;
    char cNpcName[21], cName_Master[11], cName_Slave[11], cWaypoint[11];
    char cSA;
    int  pX, pY, iMapLevel, iProbSA, iKindSA, iResultNum, iNpcID;
    bool bMaster, bFirmBerserk, bIsSpecialEvent;

    if (m_bOnExitProcess == true) return;

    for (i = 0; i < DEF_MAXMAPS; i++)
    {
        // Random Mob Generator

        //if ( (m_pMapList[i] != 0) && (m_pMapList[i]->m_bRandomMobGenerator == true) && 
        //	 ((m_pMapList[i]->m_iMaximumObject - 30) > m_pMapList[i]->m_iTotalActiveObject) ) {

        if (m_pMapList[i] != 0)
        {
            //if (m_bIsCrusadeMode == true) 
            //	 iResultNum = (m_pMapList[i]->m_iMaximumObject - 30) / 3;
            //else iResultNum = (m_pMapList[i]->m_iMaximumObject - 30);
            iResultNum = (m_pMapList[i]->m_iMaximumObject - 30);
        }

        if ((m_pMapList[i] != 0) && (m_pMapList[i]->m_bRandomMobGenerator == true) && (iResultNum > m_pMapList[i]->m_iTotalActiveObject))
        {
            if ((m_iMiddlelandMapIndex != -1) && (m_iMiddlelandMapIndex == i) && (m_bIsCrusadeMode == true)) break;

            iNamingValue = m_pMapList[i]->iGetEmptyNamingValue();
            if (iNamingValue != -1)
            {
                // Master Mob????????.
                memset(cName_Master, 0, sizeof(cName_Master));
                copy_string(cName_Master, "XX%d", iNamingValue);
                cName_Master[0] = '_';
                cName_Master[1] = i + 65;

                memset(cNpcName, 0, sizeof(cNpcName));

                bFirmBerserk = false;
                iResult = iDice(1, 100);
                switch (m_pMapList[i]->m_cRandomMobGeneratorLevel)
                {

                    case 1: // arefarm, elvfarm, aresden, elvine
                        if ((iResult >= 1) && (iResult < 20))
                        {
                            iResult = 1; // Slime
                        }
                        else if ((iResult >= 20) && (iResult < 40))
                        {
                            iResult = 2; // Giant-Ant
                        }
                        else if ((iResult >= 40) && (iResult < 85))
                        {
                            iResult = 24; // Rabbit
                        }
                        else if ((iResult >= 85) && (iResult < 95))
                        {
                            iResult = 25; // Cat
                        }
                        else if ((iResult >= 95) && (iResult <= 100))
                        {
                            iResult = 3; // Orc
                        }
                        iMapLevel = 1;
                        break;

                    case 2:
                        if ((iResult >= 1) && (iResult < 40))
                        {
                            iResult = 1;
                        }
                        else if ((iResult >= 40) && (iResult < 80))
                        {
                            iResult = 2;
                        }
                        else iResult = 10;
                        iMapLevel = 1;
                        break;

                    case 3:     // v1.4334 ????????? ????
                        if ((iResult >= 1) && (iResult < 20))
                        {
                            switch (iDice(1, 2))
                            {
                                case 1: iResult = 3;  break;
                                case 2: iResult = 4;  break;
                            }
                        }
                        else if ((iResult >= 20) && (iResult < 25))
                        {
                            iResult = 30;
                        }
                        else if ((iResult >= 25) && (iResult < 50))
                        {
                            switch (iDice(1, 3))
                            {
                                case 1: iResult = 5;  break;
                                case 2: iResult = 6;  break;
                                case 3:	iResult = 7;  break;
                            }
                        }
                        else if ((iResult >= 50) && (iResult < 75))
                        {
                            switch (iDice(1, 7))
                            {
                                case 1:
                                case 2:	iResult = 8;  break;
                                case 3:	iResult = 11; break;
                                case 4: iResult = 12; break;
                                case 5:	iResult = 18; break;
                                case 6:	iResult = 26; break;
                                case 7: iResult = 28; break;
                            }
                        }
                        else if ((iResult >= 75) && (iResult <= 100))
                        {
                            switch (iDice(1, 5))
                            {
                                case 1:
                                case 2: iResult = 9;  break;
                                case 3:	iResult = 13; break;
                                case 4: iResult = 14; break;
                                case 5:	iResult = 27; break;
                            }
                        }
                        iMapLevel = 3;
                        break;

                    case 4:
                        if ((iResult >= 1) && (iResult < 50))
                        {
                            switch (iDice(1, 2))
                            {
                                case 1:	iResult = 2;  break;
                                case 2: iResult = 10; break;
                            }
                        }
                        else if ((iResult >= 50) && (iResult < 80))
                        {
                            switch (iDice(1, 2))
                            {
                                case 1: iResult = 8;  break;
                                case 2: iResult = 11; break;
                            }
                        }
                        else if ((iResult >= 80) && (iResult <= 100))
                        {
                            switch (iDice(1, 2))
                            {
                                case 1: iResult = 14; break;
                                case 2:	iResult = 9;  break;
                            }
                        }
                        iMapLevel = 2;
                        break;

                    case 5:
                        if ((iResult >= 1) && (iResult < 30))
                        {
                            switch (iDice(1, 5))
                            {
                                case 1:
                                case 2:
                                case 3:
                                case 4:
                                case 5: iResult = 2;  break;
                            }
                        }
                        else if ((iResult >= 30) && (iResult < 60))
                        {
                            switch (iDice(1, 2))
                            {
                                case 1: iResult = 3;  break;
                                case 2: iResult = 4;  break;
                            }
                        }
                        else if ((iResult >= 60) && (iResult < 80))
                        {
                            switch (iDice(1, 2))
                            {
                                case 1: iResult = 5;  break;
                                case 2: iResult = 7;  break;
                            }
                        }
                        else if ((iResult >= 80) && (iResult < 95))
                        {
                            switch (iDice(1, 3))
                            {
                                case 1:
                                case 2: iResult = 8;  break;
                                case 3:	iResult = 11; break;
                            }
                        }
                        else if ((iResult >= 95) && (iResult <= 100))
                        {
                            switch (iDice(1, 3))
                            {
                                case 1: iResult = 11; break;
                                case 2: iResult = 14; break;
                                case 3: iResult = 9;  break;
                            }
                        }
                        iMapLevel = 3;
                        break;

                    case 6: // huntzone3, huntzone4
                        if ((iResult >= 1) && (iResult < 60))
                        {
                            switch (iDice(1, 4))
                            {
                                case 1: iResult = 5;  break; // Skeleton
                                case 2:	iResult = 6;  break; // Orc-Mage
                                case 3: iResult = 12; break; // Cyclops
                                case 4: iResult = 11; break; // Troll
                            }
                        }
                        else if ((iResult >= 60) && (iResult < 90))
                        {
                            switch (iDice(1, 5))
                            {
                                case 1:
                                case 2: iResult = 8;  break; // Stone-Golem
                                case 3:	iResult = 11; break; // Troll
                                case 4:	iResult = 12; break; // Cyclops 
                                case 5:	iResult = 43; break; // Tentocle
                            }
                        }
                        else if ((iResult >= 90) && (iResult <= 100))
                        {
                            switch (iDice(1, 9))
                            {
                                case 1:	iResult = 26; break;
                                case 2:	iResult = 9;  break;
                                case 3: iResult = 13; break;
                                case 4: iResult = 14; break;
                                case 5:	iResult = 18; break;
                                case 6:	iResult = 28; break;
                                case 7: iResult = 27; break;
                                case 8: iResult = 29; break;
                            }
                        }
                        iMapLevel = 4;
                        break;

                    case 7: // areuni, elvuni
                        if ((iResult >= 1) && (iResult < 50))
                        {
                            switch (iDice(1, 5))
                            {
                                case 1: iResult = 3;  break; // Orc
                                case 2: iResult = 6;  break; // Orc-Mage
                                case 3: iResult = 10; break; // Amphis
                                case 4: iResult = 3;  break; // Orc
                                case 5: iResult = 50; break; // Giant-Tree
                            }
                        }
                        //else if ((iResult >= 50) && (iResult < 60)) { 
                        //	iResult = 29; // Rudolph
                        //}
                        else if ((iResult >= 50) && (iResult < 85))
                        {
                            switch (iDice(1, 4))
                            {
                                case 1: iResult = 50; break; // Giant-Tree
                                case 2:
                                case 3: iResult = 6;  break; // Orc-Mage
                                case 4: iResult = 12; break; // Troll
                            }
                        }
                        else if ((iResult >= 85) && (iResult <= 100))
                        {
                            switch (iDice(1, 4))
                            {
                                case 1: iResult = 12;  break; // Troll
                                case 2:
                                case 3:
                                    if (iDice(1, 100) < 3)
                                        iResult = 17; // Unicorn
                                    else iResult = 12; // Troll
                                    break;
                                case 4: iResult = 29;  break; // Cannibal-Plant
                            }
                        }
                        iMapLevel = 4;
                        break;

                    case 8:
                        if ((iResult >= 1) && (iResult < 70))
                        {
                            switch (iDice(1, 2))
                            {
                                case 1:	iResult = 4;  break;
                                case 2: iResult = 5;  break;
                            }
                        }
                        else if ((iResult >= 70) && (iResult < 95))
                        {
                            switch (iDice(1, 2))
                            {
                                case 1: iResult = 8;  break;
                                case 2: iResult = 11; break;
                            }
                        }
                        else if ((iResult >= 95) && (iResult <= 100))
                        {
                            iResult = 14; break;
                        }
                        iMapLevel = 4;
                        break;

                    case 9:
                        if ((iResult >= 1) && (iResult < 70))
                        {
                            switch (iDice(1, 2))
                            {
                                case 1:	iResult = 4;  break;
                                case 2: iResult = 5;  break;
                            }
                        }
                        else if ((iResult >= 70) && (iResult < 95))
                        {
                            switch (iDice(1, 3))
                            {
                                case 1: iResult = 8;  break;
                                case 2: iResult = 9;  break;
                                case 3: iResult = 13; break;
                            }
                        }
                        else if ((iResult >= 95) && (iResult <= 100))
                        {
                            switch (iDice(1, 6))
                            {
                                case 1:
                                case 2:
                                case 3: iResult = 9;  break;
                                case 4:
                                case 5: iResult = 14; break;
                                case 6: iResult = 15; break;
                            }
                        }

                        if ((iDice(1, 3) == 1) && (iResult != 16)) bFirmBerserk = true;
                        iMapLevel = 5;
                        break;

                    case 10:
                        if ((iResult >= 1) && (iResult < 70))
                        {
                            switch (iDice(1, 3))
                            {
                                case 1:	iResult = 9; break;
                                case 2: iResult = 5; break;
                                case 3: iResult = 8; break;
                            }
                        }
                        else if ((iResult >= 70) && (iResult < 95))
                        {
                            switch (iDice(1, 3))
                            {
                                case 1:
                                case 2:	iResult = 13; break;
                                case 3: iResult = 14; break;
                            }
                        }
                        else if ((iResult >= 95) && (iResult <= 100))
                        {
                            switch (iDice(1, 3))
                            {
                                case 1:
                                case 2: iResult = 14; break;
                                case 3: iResult = 15; break;
                            }
                        }
                        // Demon?? Berserk ????? ??? ?????
                        if ((iDice(1, 3) == 1) && (iResult != 16)) bFirmBerserk = true;
                        iMapLevel = 5;
                        break;

                    case 11:
                        if ((iResult >= 1) && (iResult < 30))
                        {
                            switch (iDice(1, 5))
                            {
                                case 1:
                                case 2:
                                case 3:
                                case 4:
                                case 5: iResult = 2; break;
                            }
                        }
                        else if ((iResult >= 30) && (iResult < 60))
                        {
                            switch (iDice(1, 2))
                            {
                                case 1: iResult = 3; break;
                                case 2: iResult = 4; break;
                            }
                        }
                        else if ((iResult >= 60) && (iResult < 80))
                        {
                            switch (iDice(1, 2))
                            {
                                case 1: iResult = 5; break;
                                case 2: iResult = 7; break;
                            }
                        }
                        else if ((iResult >= 80) && (iResult < 95))
                        {
                            switch (iDice(1, 3))
                            {
                                case 1:
                                case 2: iResult = 10;  break;
                                case 3:	iResult = 11; break;
                            }
                        }
                        else if ((iResult >= 95) && (iResult <= 100))
                        {
                            switch (iDice(1, 3))
                            {
                                case 1: iResult = 11; break;
                                case 2: iResult = 7; break;
                                case 3: iResult = 8; break;
                            }
                        }
                        iMapLevel = 4;
                        break;

                    case 12:
                        if ((iResult >= 1) && (iResult < 50))
                        {
                            switch (iDice(1, 3))
                            {
                                case 1:	iResult = 1; break;
                                case 2: iResult = 2; break;
                                case 3: iResult = 10; break;
                            }
                        }
                        else if ((iResult >= 50) && (iResult < 85))
                        {
                            switch (iDice(1, 2))
                            {
                                case 1: iResult = 5; break;
                                case 2: iResult = 4; break;
                            }
                        }
                        else if ((iResult >= 85) && (iResult <= 100))
                        {
                            switch (iDice(1, 3))
                            {
                                case 1: iResult = 8; break;
                                case 2: iResult = 11; break;
                                case 3: iResult = 26; break;
                            }
                        }
                        iMapLevel = 4;
                        break;

                    case 13:
                        if ((iResult >= 1) && (iResult < 15))
                        {
                            iResult = 4;
                            bFirmBerserk = true;
                            iTotalMob = 4 - (iDice(1, 2) - 1);
                            break;
                        }
                        else if ((iResult >= 15) && (iResult < 40))
                        {
                            iResult = 14;
                            bFirmBerserk = true;
                            iTotalMob = 4 - (iDice(1, 2) - 1);
                            break;
                        }
                        else if ((iResult >= 40) && (iResult < 60))
                        {
                            iResult = 9;
                            bFirmBerserk = true;
                            iTotalMob = 4 - (iDice(1, 2) - 1);
                            break;
                        }
                        else if ((iResult >= 60) && (iResult < 75))
                        {
                            iResult = 13;
                            bFirmBerserk = true;
                            iTotalMob = 4 - (iDice(1, 2) - 1);
                            break;
                        }
                        else if ((iResult >= 75) && (iResult < 95))
                        {
                            iResult = 23;
                        }
                        else if ((iResult >= 95) && (iResult <= 100))
                        {
                            iResult = 22;
                        }
                        iMapLevel = 5;
                        break;

                    case 14: // icebound
                        if ((iResult >= 1) && (iResult < 30))
                        {
                            iResult = 23; // Dark-Elf
                        }
                        else if ((iResult >= 30) && (iResult < 50))
                        {
                            iResult = 31; // Ice-Golem
                        }
                        else if ((iResult >= 50) && (iResult < 70))
                        {
                            iResult = 22; // Beholder
                            bFirmBerserk = true;
                            iTotalMob = 4 - (iDice(1, 2) - 1);
                        }
                        else if ((iResult >= 70) && (iResult < 90))
                        {
                            iResult = 32; // DireBoar
                        }
                        else if ((iResult >= 90) && (iResult <= 100))
                        {
                            iResult = 33; // Frost
                        }
                        iMapLevel = 5;
                        break;

                    case 15:
                        if ((iResult >= 1) && (iResult < 35))
                        {
                            iResult = 23;
                            bFirmBerserk = true;
                        }
                        else if ((iResult >= 35) && (iResult < 50))
                        {
                            iResult = 22;
                            bFirmBerserk = true;
                        }
                        else if ((iResult >= 50) && (iResult < 80))
                        {
                            iResult = 15;
                        }
                        else if ((iResult >= 80) && (iResult <= 100))
                        {
                            iResult = 21;
                        }
                        iMapLevel = 4;
                        break;

                    case 16: // 2ndmiddle, huntzone1, huntzone2, 
                        if ((iResult >= 1) && (iResult < 40))
                        {
                            switch (iDice(1, 3))
                            {
                                case 1:	iResult = 7;  break; // Scorpion
                                case 2: iResult = 2;  break; // Giant-Ant
                                case 3: iResult = 10; break; // Amphis
                            }
                        }
                        else if ((iResult >= 40) && (iResult < 50))
                        {
                            iResult = 30; // Rudolph
                        }
                        else if ((iResult >= 50) && (iResult < 85))
                        {
                            switch (iDice(1, 2))
                            {
                                case 1: iResult = 5;  break; // Skeleton
                                case 2: iResult = 4;  break; // Zombie
                            }
                        }
                        else if ((iResult >= 85) && (iResult <= 100))
                        {
                            switch (iDice(1, 3))
                            {
                                case 1: iResult = 8;  break; // Stone-Golem
                                case 2: iResult = 11; break; // Clay-Golem
                                case 3: iResult = 7;  break; // Scorpion
                            }
                        }
                        iMapLevel = 1;
                        break;

                    case 17:
                        if ((iResult >= 1) && (iResult < 30))
                        {
                            switch (iDice(1, 4))
                            {
                                case 1:	iResult = 22;  break; // Giant-Frog
                                case 2: iResult = 8;   break; // Stone-Golem
                                case 3: iResult = 24;  break; // Rabbit
                                case 4: iResult = 5;   break;
                            }
                        }
                        else if ((iResult >= 30) && (iResult < 40))
                        {
                            iResult = 30;
                        }
                        else if ((iResult >= 40) && (iResult < 70))
                        {
                            iResult = 32;
                        }
                        else if ((iResult >= 70) && (iResult < 90))
                        {
                            iResult = 31;
                            if (iDice(1, 5) == 1)
                            {
                                bFirmBerserk = true;
                            }
                        }
                        else if ((iResult >= 90) && (iResult <= 100))
                        {
                            iResult = 33;
                        }
                        iMapLevel = 1;
                        break;

                    case 18: // druncncity
                        if ((iResult >= 1) && (iResult < 2))
                        {
                            iResult = 39; // Tentocle
                        }
                        else if ((iResult >= 2) && (iResult < 12))
                        {
                            iResult = 44; // ClawTurtle
                        }
                        else if ((iResult >= 12) && (iResult < 50))
                        {
                            iResult = 48; // Nizie
                        }
                        else if ((iResult >= 50) && (iResult < 80))
                        {
                            iResult = 45; // Giant-Crayfish
                        }
                        else if ((iResult >= 80) && (iResult < 90))
                        {
                            iResult = 34; // Stalker
                        }
                        else if ((iResult >= 90) && (iResult <= 100))
                        {
                            iResult = 26; // Giant-Frog
                        }
                        iMapLevel = 4;
                        break;

                    case 19:
                        if ((iResult >= 1) && (iResult < 15))
                        {
                            iResult = 44;
                        }
                        else if ((iResult >= 15) && (iResult < 25))
                        {
                            iResult = 46;
                        }
                        else if ((iResult >= 25) && (iResult < 35))
                        {
                            iResult = 21;
                        }
                        else if ((iResult >= 35) && (iResult < 60))
                        {
                            iResult = 43;
                        }
                        else if ((iResult >= 60) && (iResult < 85))
                        {
                            iResult = 23;
                        }
                        else if ((iResult >= 85) && (iResult <= 100))
                        {
                            iResult = 22;
                        }
                        iMapLevel = 4;
                        break;

                    case 20:
                        if ((iResult >= 1) && (iResult < 2))
                        {
                            iResult = 41;
                        }
                        else if ((iResult >= 2) && (iResult < 3))
                        {
                            iResult = 40;
                        }
                        else if ((iResult >= 3) && (iResult < 8))
                        {
                            iResult = 53;
                        }
                        else if ((iResult >= 8) && (iResult < 9))
                        {
                            iResult = 39;
                        }
                        else if ((iResult >= 9) && (iResult < 20))
                        {
                            iResult = 21;
                        }
                        else if ((iResult >= 20) && (iResult < 35))
                        {
                            iResult = 16;
                        }
                        else if ((iResult >= 35) && (iResult < 45))
                        {
                            iResult = 44;
                        }
                        else if ((iResult >= 45) && (iResult < 55))
                        {
                            iResult = 45;
                        }
                        else if ((iResult >= 55) && (iResult < 75))
                        {
                            iResult = 28;
                        }
                        else if ((iResult >= 75) && (iResult < 95))
                        {
                            iResult = 43;
                        }
                        else if ((iResult >= 95) && (iResult < 100))
                        {
                            iResult = 22;
                        }
                        iMapLevel = 4;
                        break;

                    case 21:
                        if ((iResult >= 1) && (iResult < 94))
                        {
                            iResult = 17; // Unicorn
                            bFirmBerserk = true;
                        }
                        else if ((iResult >= 94) && (iResult < 95))
                        {
                            iResult = 36; // Wyvern
                        }
                        else if ((iResult >= 95) && (iResult < 96))
                        {
                            iResult = 37; // Fire-Wyvern
                        }
                        else if ((iResult >= 96) && (iResult < 97))
                        {
                            iResult = 47; // MasterMage-Orc
                        }
                        else if ((iResult >= 97) && (iResult < 98))
                        {
                            iResult = 35; // Hellclaw
                        }
                        else if ((iResult >= 98) && (iResult < 99))
                        {
                            iResult = 49; // Tigerworm
                        }
                        else if ((iResult >= 99) && (iResult <= 100))
                        {
                            iResult = 51; // Abaddon
                        }
                        iMapLevel = 4;
                        break;

                }

                pX = 0;
                pY = 0;

                //				bIsSpecialEvent = true;
                if ((m_bIsSpecialEventTime == true) && (iDice(1, 10) == 3)) bIsSpecialEvent = true;

                if (bIsSpecialEvent == true)
                {
                    switch (m_cSpecialEventType)
                    {
                        case 1:
                            if (m_pMapList[i]->m_iMaxPx != 0)
                            {
                                pX = m_pMapList[i]->m_iMaxPx * 20 + 10;
                                pY = m_pMapList[i]->m_iMaxPy * 20 + 10;

                                if (pX < 0) pX = 0;
                                if (pY < 0) pY = 0;

                                if (m_bIsCrusadeMode == true)
                                {
                                    if (strcmp(m_pMapList[i]->m_cName, "aresden") == 0)
                                        switch (iDice(1, 6))
                                        {
                                            case 1: iResult = 20; break;
                                            case 2: iResult = 53; break;
                                            case 3: iResult = 55; break;
                                            case 4: iResult = 57; break;
                                            case 5: iResult = 59; break;
                                            case 6: iResult = 61; break;
                                        }
                                    else if (strcmp(m_pMapList[i]->m_cName, "elvine") == 0)
                                        switch (iDice(1, 6))
                                        {
                                            case 1: iResult = 19; break;
                                            case 2: iResult = 52; break;
                                            case 3: iResult = 54; break;
                                            case 4: iResult = 56; break;
                                            case 5: iResult = 58; break;
                                            case 6: iResult = 60; break;
                                        }
                                }
                                copy_string(G_cTxt, "(!) Mob-Event Map(%s)[%d (%d,%d)]", m_pMapList[i]->m_cName, iResult, pX, pY);
                            }
                            break;

                        case 2:
                            if (iDice(1, 3) == 2)
                            {
                                if ((memcmp(m_pMapList[i]->m_cLocationName, "aresden", 7) == 0) ||
                                    (memcmp(m_pMapList[i]->m_cLocationName, "middled1n", 9) == 0) ||
                                    (memcmp(m_pMapList[i]->m_cLocationName, "arefarm", 7) == 0) ||
                                    (memcmp(m_pMapList[i]->m_cLocationName, "elvfarm", 7) == 0) ||
                                    (memcmp(m_pMapList[i]->m_cLocationName, "elvine", 6) == 0))
                                {
                                    if (iDice(1, 30) == 5)
                                        iResult = 16;
                                    else iResult = 5;
                                }
                                else iResult = 16;
                            }
                            else iResult = 17;

                            m_bIsSpecialEventTime = false;
                            break;
                    }
                }

                memset(cNpcName, 0, sizeof(cNpcName));
                //Random Monster Spawns
                switch (iResult)
                {
                    case 1:  strcpy(cNpcName, "Slime");				iNpcID = 10; iProbSA = 5;  iKindSA = 1; break;
                    case 2:  strcpy(cNpcName, "Giant-Ant");			iNpcID = 16; iProbSA = 10; iKindSA = 2; break;
                    case 3:  strcpy(cNpcName, "Orc");				iNpcID = 14; iProbSA = 15; iKindSA = 1; break;
                    case 4:  strcpy(cNpcName, "Zombie");			iNpcID = 18; iProbSA = 15; iKindSA = 3; break;
                    case 5:  strcpy(cNpcName, "Skeleton");			iNpcID = 11; iProbSA = 35; iKindSA = 8; break;
                    case 6:  strcpy(cNpcName, "Orc-Mage");			iNpcID = 14; iProbSA = 30; iKindSA = 7; break;
                    case 7:  strcpy(cNpcName, "Scorpion");			iNpcID = 17; iProbSA = 15; iKindSA = 3; break;
                    case 8:  strcpy(cNpcName, "Stone-Golem");		iNpcID = 12; iProbSA = 25; iKindSA = 5; break;
                    case 9:  strcpy(cNpcName, "Cyclops");			iNpcID = 13; iProbSA = 35; iKindSA = 8; break;
                    case 10: strcpy(cNpcName, "Amphis");			iNpcID = 22; iProbSA = 20; iKindSA = 3; break;
                    case 11: strcpy(cNpcName, "Clay-Golem");		iNpcID = 23; iProbSA = 20; iKindSA = 5; break;
                    case 12: strcpy(cNpcName, "Troll");				iNpcID = 28; iProbSA = 25; iKindSA = 3; break;
                    case 13: strcpy(cNpcName, "Orge");				iNpcID = 29; iProbSA = 25; iKindSA = 1; break;
                    case 14: strcpy(cNpcName, "Hellbound");			iNpcID = 27; iProbSA = 25; iKindSA = 8; break;
                    case 15: strcpy(cNpcName, "Liche");				iNpcID = 30; iProbSA = 30; iKindSA = 8; break;
                    case 16: strcpy(cNpcName, "Demon");				iNpcID = 31; iProbSA = 20; iKindSA = 8; break;
                    case 17: strcpy(cNpcName, "Unicorn");			iNpcID = 32; iProbSA = 35; iKindSA = 7; break;
                    case 18: strcpy(cNpcName, "WereWolf");			iNpcID = 33; iProbSA = 25; iKindSA = 1; break;
                    case 19: strcpy(cNpcName, "YB-Aresden");		iNpcID = -1;  iProbSA = 15; iKindSA = 1; break;
                    case 20: strcpy(cNpcName, "YB-Elvine");			iNpcID = -1;  iProbSA = 15; iKindSA = 1; break;
                    case 21: strcpy(cNpcName, "Gagoyle");			iNpcID = 52; iProbSA = 20; iKindSA = 8; break;
                    case 22: strcpy(cNpcName, "Beholder");			iNpcID = 53; iProbSA = 20; iKindSA = 5; break;
                    case 23: strcpy(cNpcName, "Dark-Elf");			iNpcID = 54; iProbSA = 20; iKindSA = 3; break;
                    case 24: strcpy(cNpcName, "Rabbit");			iNpcID = -1; iProbSA = 5;  iKindSA = 1; break;
                    case 25: strcpy(cNpcName, "Cat");				iNpcID = -1; iProbSA = 10; iKindSA = 2; break;
                    case 26: strcpy(cNpcName, "Giant-Frog");		iNpcID = 57; iProbSA = 10; iKindSA = 2; break;
                    case 27: strcpy(cNpcName, "Mountain-Giant");	iNpcID = 58; iProbSA = 25; iKindSA = 1; break;
                    case 28: strcpy(cNpcName, "Ettin");				iNpcID = 59; iProbSA = 20; iKindSA = 8; break;
                    case 29: strcpy(cNpcName, "Cannibal-Plant");	iNpcID = 60; iProbSA = 20; iKindSA = 5; break;
                    case 30: strcpy(cNpcName, "Rudolph");			iNpcID = -1; iProbSA = 20; iKindSA = 5; break;
                    case 31: strcpy(cNpcName, "Ice-Golem");			iNpcID = 65; iProbSA = 35; iKindSA = 8; break;
                    case 32: strcpy(cNpcName, "DireBoar");			iNpcID = 62; iProbSA = 20; iKindSA = 5; break;
                    case 33: strcpy(cNpcName, "Frost");				iNpcID = 63; iProbSA = 30; iKindSA = 8; break;
                    case 34: strcpy(cNpcName, "Stalker");           iNpcID = 48; iProbSA = 20; iKindSA = 1; break;
                    case 35: strcpy(cNpcName, "Hellclaw");			iNpcID = 49; iProbSA = 20; iKindSA = 1; break;
                    case 36: strcpy(cNpcName, "Wyvern");			iNpcID = 66; iProbSA = 20; iKindSA = 1; break;
                    case 37: strcpy(cNpcName, "Fire-Wyvern");		iNpcID = -1; iProbSA = 20; iKindSA = 1; break;
                    case 38: strcpy(cNpcName, "Barlog");			iNpcID = -1; iProbSA = 20; iKindSA = 1; break;
                    case 39: strcpy(cNpcName, "Tentocle");			iNpcID = -1; iProbSA = 20; iKindSA = 1; break;
                    case 40: strcpy(cNpcName, "Centaurus");			iNpcID = -1; iProbSA = 20; iKindSA = 1; break;
                    case 41: strcpy(cNpcName, "Giant-Lizard");		iNpcID = -1; iProbSA = 20; iKindSA = 1; break;
                    case 42: strcpy(cNpcName, "Minotaurs");			iNpcID = -1; iProbSA = 20; iKindSA = 1; break;
                    case 43: strcpy(cNpcName, "Tentocle");			iNpcID = -1; iProbSA = 20; iKindSA = 1; break;
                    case 44: strcpy(cNpcName, "Claw-Turtle");		iNpcID = -1; iProbSA = 20; iKindSA = 1; break;
                    case 45: strcpy(cNpcName, "Giant-Crayfish");	iNpcID = -1; iProbSA = 20; iKindSA = 1; break;
                    case 46: strcpy(cNpcName, "Giant-Plant");		iNpcID = -1; iProbSA = 20; iKindSA = 1; break;
                    case 47: strcpy(cNpcName, "MasterMage-Orc");	iNpcID = -1; iProbSA = 20; iKindSA = 1; break;
                    case 48: strcpy(cNpcName, "Nizie");				iNpcID = -1; iProbSA = 20; iKindSA = 1; break;
                    case 49: strcpy(cNpcName, "Tigerworm");			iNpcID = 50; iProbSA = 20; iKindSA = 1; break;
                    case 50: strcpy(cNpcName, "Giant-Plant");		iNpcID = -1; iProbSA = 20; iKindSA = 1; break;
                    case 51: strcpy(cNpcName, "Abaddon");			iNpcID = -1; iProbSA = 20; iKindSA = 1; break;
                    case 52: strcpy(cNpcName, "YW-Aresden");		iNpcID = -1; iProbSA = 15; iKindSA = 1; break;
                    case 53: strcpy(cNpcName, "YW-Elvine");			iNpcID = -1; iProbSA = 15; iKindSA = 1; break;
                    case 54: strcpy(cNpcName, "YY-Aresden");		iNpcID = -1; iProbSA = 15; iKindSA = 1; break;
                    case 55: strcpy(cNpcName, "YY-Elvine");			iNpcID = -1; iProbSA = 15; iKindSA = 1; break;
                    case 56: strcpy(cNpcName, "XB-Aresden");		iNpcID = -1; iProbSA = 15; iKindSA = 1; break;
                    case 57: strcpy(cNpcName, "XB-Elvine");			iNpcID = -1; iProbSA = 15; iKindSA = 1; break;
                    case 58: strcpy(cNpcName, "XW-Aresden");		iNpcID = -1; iProbSA = 15; iKindSA = 1; break;
                    case 59: strcpy(cNpcName, "XW-Elvine");			iNpcID = -1; iProbSA = 15; iKindSA = 1; break;
                    case 60: strcpy(cNpcName, "XY-Aresden");		iNpcID = -1; iProbSA = 15; iKindSA = 1; break;
                    case 61: strcpy(cNpcName, "XY-Elvine");			iNpcID = -1; iProbSA = 15; iKindSA = 1; break;
                    default: strcpy(cNpcName, "Orc");				iNpcID = 14; iProbSA = 15; iKindSA = 1; break;
                }

                cSA = 0;
                if (iDice(1, 100) <= iProbSA)
                {
                    cSA = _cGetSpecialAbility(iKindSA);
                }

                if ((bMaster = bCreateNewNpc(cNpcName, cName_Master, m_pMapList[i]->m_cName, (rand() % 3), cSA, DEF_MOVETYPE_RANDOM, &pX, &pY, cWaypoint, 0, 0, -1, false, false, bFirmBerserk, true)) == false)
                {
                    m_pMapList[i]->SetNamingValueEmpty(iNamingValue);
                }
                else
                {

                }
            }

            switch (iResult)
            {
                case 1:	 iTotalMob = iDice(1, 5) - 1; break;
                case 2:	 iTotalMob = iDice(1, 5) - 1; break;
                case 3:	 iTotalMob = iDice(1, 5) - 1; break;
                case 4:	 iTotalMob = iDice(1, 3) - 1; break;
                case 5:	 iTotalMob = iDice(1, 3) - 1; break;

                case 6:  iTotalMob = iDice(1, 3) - 1; break;
                case 7:  iTotalMob = iDice(1, 3) - 1; break;
                case 8:  iTotalMob = iDice(1, 2) - 1; break;
                case 9:  iTotalMob = iDice(1, 2) - 1; break;
                case 10: iTotalMob = iDice(1, 5) - 1; break;
                case 11: iTotalMob = iDice(1, 3) - 1; break;
                case 12: iTotalMob = iDice(1, 5) - 1; break;
                case 13: iTotalMob = iDice(1, 3) - 1; break;
                case 14: iTotalMob = iDice(1, 2) - 1; break;
                case 15: iTotalMob = iDice(1, 3) - 1; break;
                case 16: iTotalMob = iDice(1, 2) - 1; break;
                case 17: iTotalMob = iDice(1, 2) - 1; break;

                case 18: iTotalMob = iDice(1, 5) - 1; break;
                case 19: iTotalMob = iDice(1, 2) - 1; break;
                case 20: iTotalMob = iDice(1, 2) - 1; break;
                case 21: iTotalMob = iDice(1, 5) - 1; break;
                case 22: iTotalMob = iDice(1, 2) - 1; break;
                case 23: iTotalMob = iDice(1, 2) - 1; break;

                case 24: iTotalMob = iDice(1, 4) - 1; break;
                case 25: iTotalMob = iDice(1, 2) - 1; break;
                case 26: iTotalMob = iDice(1, 3) - 1; break;
                case 27: iTotalMob = iDice(1, 3) - 1; break;

                case 28: iTotalMob = iDice(1, 3) - 1; break;
                case 29: iTotalMob = iDice(1, 5) - 1; break;
                case 30: iTotalMob = iDice(1, 3) - 1; break;
                case 31: iTotalMob = iDice(1, 3) - 1; break;

                case 32: iTotalMob = 1; break;
                case 33: iTotalMob = 1; break;
                case 34: iTotalMob = 1; break;
                case 35: iTotalMob = 1; break;
                case 36: iTotalMob = 1; break;

                case 37: iTotalMob = 1; break;
                case 38: iTotalMob = 1; break;
                case 39: iTotalMob = 1; break;
                case 40: iTotalMob = 1; break;
                case 41: iTotalMob = 1; break;

                case 42: iTotalMob = iDice(1, 3) - 1; break;
                case 43: iTotalMob = 1; break;
                case 44: iTotalMob = iDice(1, 3) - 1; break;
                case 45: iTotalMob = 1; break;
                default: iTotalMob = 0; break;
            }

            if (bMaster == false) iTotalMob = 0;

            if (iTotalMob > 2)
            {
                switch (iResult)
                {
                    case 1:  // Slime 
                    case 2:  // Giant-Ant
                    case 3:  // Orc
                    case 4:  // Zombie
                    case 5:  // Skeleton
                    case 6:  // Orc-Mage
                    case 7:  // Scorpion
                    case 8:  // Stone-Golem
                    case 9:  // Cyclops
                    case 10: // Amphis
                    case 11: // Clay-Golem
                    case 12: // Troll
                    case 13: // Orge
                    case 14: // Hellbound
                    case 15: // Liche
                    case 16: // Demon
                    case 17: // Unicorn
                    case 18: // WereWolf
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                        if (iDice(1, 5) == 1) iTotalMob = 0;  // 75% ???????? ???.
                        break;

                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                        if (iDice(1, 5) != 1) iTotalMob = 0;  // 75% ???????????????
                        break;
                }
            }

            if (bIsSpecialEvent == true)
            {
                switch (m_cSpecialEventType)
                {
                    case 1:
                        if ((iResult != 35) && (iResult != 36) && (iResult != 37) && (iResult != 49)
                            && (iResult != 51) && (iResult != 15) && (iResult != 16) && (iResult != 21)) iTotalMob = 12;
                        for (x = 1; x < DEF_MAXCLIENTS; x++)
                            if ((iNpcID != -1) && (m_pClientList[x] != 0) && (m_pClientList[x]->m_bIsInitComplete == true))
                            {
                                SendNotifyMsg(0, x, DEF_NOTIFY_SPAWNEVENT, pX, pY, iNpcID, 0, 0, 0);
                            }
                        break;

                    case 2:
                        if ((memcmp(m_pMapList[i]->m_cLocationName, "aresden", 7) == 0) ||
                            (memcmp(m_pMapList[i]->m_cLocationName, "elvine", 6) == 0) ||
                            (memcmp(m_pMapList[i]->m_cLocationName, "elvfarm", 7) == 0) ||
                            (memcmp(m_pMapList[i]->m_cLocationName, "arefarm", 7) == 0))
                        {
                            iTotalMob = 0;
                        }
                        break;
                }
                m_bIsSpecialEventTime = false;
            }

            for (j = 0; j < iTotalMob; j++)
            {
                iNamingValue = m_pMapList[i]->iGetEmptyNamingValue();
                if (iNamingValue != -1)
                {
                    memset(cName_Slave, 0, sizeof(cName_Slave));
                    copy_string(cName_Slave, "XX%d", iNamingValue);
                    cName_Slave[0] = 95; // original '_';
                    cName_Slave[1] = i + 65;

                    cSA = 0;

                    if (iDice(1, 100) <= iProbSA)
                    {
                        cSA = _cGetSpecialAbility(iKindSA);
                    }

                    if (bCreateNewNpc(cNpcName, cName_Slave, m_pMapList[i]->m_cName, (rand() % 3), cSA, DEF_MOVETYPE_RANDOM, &pX, &pY, cWaypoint, 0, 0, -1, false, false, bFirmBerserk) == false)
                    {
                        m_pMapList[i]->SetNamingValueEmpty(iNamingValue);
                    }
                    else
                    {
                        bSetNpcFollowMode(cName_Slave, cName_Master, DEF_OWNERTYPE_NPC);
                    }
                }
            }
        }

        if ((m_pMapList[i] != 0) && ((m_pMapList[i]->m_iMaximumObject) > m_pMapList[i]->m_iTotalActiveObject))
        {
            for (j = 1; j < DEF_MAXSPOTMOBGENERATOR; j++)
                if ((iDice(1, 3) == 2) && (m_pMapList[i]->m_stSpotMobGenerator[j].bDefined == true) &&
                    (m_pMapList[i]->m_stSpotMobGenerator[j].iMaxMobs > m_pMapList[i]->m_stSpotMobGenerator[j].iCurMobs))
                {
                    iNamingValue = m_pMapList[i]->iGetEmptyNamingValue();
                    if (iNamingValue != -1)
                    {

                        memset(cNpcName, 0, sizeof(cNpcName));
                        switch (m_pMapList[i]->m_stSpotMobGenerator[j].iMobType)
                        {
                            // spot-mob-generator
                            case 10:  strcpy(cNpcName, "Slime");				iProbSA = 5;  iKindSA = 1;  break;
                            case 16:  strcpy(cNpcName, "Giant-Ant");			iProbSA = 10; iKindSA = 2;  break;
                            case 14:  strcpy(cNpcName, "Orc");				iProbSA = 15; iKindSA = 1;  break;
                            case 18:  strcpy(cNpcName, "Zombie");			iProbSA = 15; iKindSA = 3;  break;
                            case 11:  strcpy(cNpcName, "Skeleton");			iProbSA = 35; iKindSA = 8;  break;
                            case 6:   strcpy(cNpcName, "Orc-Mage");			iProbSA = 30; iKindSA = 7;  break;
                            case 17:  strcpy(cNpcName, "Scorpion");			iProbSA = 15; iKindSA = 3;  break;
                            case 12:  strcpy(cNpcName, "Stone-Golem");		iProbSA = 25; iKindSA = 5;  break;
                            case 13:  strcpy(cNpcName, "Cyclops");			iProbSA = 35; iKindSA = 8;  break;
                            case 22:  strcpy(cNpcName, "Amphis");			iProbSA = 20; iKindSA = 3;  break;
                            case 23:  strcpy(cNpcName, "Clay-Golem");		iProbSA = 20; iKindSA = 5;  break;
                            case 24:  strcpy(cNpcName, "Guard-Aresden");		iProbSA = 20; iKindSA = 1;  break;
                            case 25:  strcpy(cNpcName, "Guard-Elvine");		iProbSA = 20; iKindSA = 1;  break;
                            case 26:  strcpy(cNpcName, "Guard-Neutral");		iProbSA = 20; iKindSA = 1;  break;
                            case 27:  strcpy(cNpcName, "Hellbound");			iProbSA = 20; iKindSA = 1;  break;
                            case 29:  strcpy(cNpcName, "Orge");              iProbSA = 20; iKindSA = 1;  break;
                            case 30:  strcpy(cNpcName, "Liche");				iProbSA = 30; iKindSA = 8;  break;
                            case 31:  strcpy(cNpcName, "Demon");				iProbSA = 20; iKindSA = 8;  break;
                            case 32:  strcpy(cNpcName, "Unicorn");			iProbSA = 35; iKindSA = 7;  break;
                            case 33:  strcpy(cNpcName, "WereWolf");			iProbSA = 25; iKindSA = 1;  break;
                            case 34:  strcpy(cNpcName, "Dummy");				iProbSA = 5;  iKindSA = 1;  break;
                            case 35:  strcpy(cNpcName, "Attack-Dummy");		iProbSA = 5;  iKindSA = 1;  break;
                            case 48:  strcpy(cNpcName, "Stalker");			iProbSA = 20; iKindSA = 3;  break;
                            case 49:  strcpy(cNpcName, "Hellclaw");			iProbSA = 20; iKindSA = 8;  break;
                            case 50:  strcpy(cNpcName, "Tigerworm");			iProbSA = 20; iKindSA = 8;  break;
                            case 54:  strcpy(cNpcName, "Dark-Elf");			iProbSA = 20; iKindSA = 8;  break;
                            case 53:  strcpy(cNpcName, "Beholder");			iProbSA = 20; iKindSA = 8;  break;
                            case 52:  strcpy(cNpcName, "Gagoyle");			iProbSA = 20; iKindSA = 8;  break;
                            case 57:  strcpy(cNpcName, "Giant-Frog");		iProbSA = 10; iKindSA = 2;  break;
                            case 58:  strcpy(cNpcName, "Mountain-Giant");	iProbSA = 25; iKindSA = 1;  break;
                            case 59:  strcpy(cNpcName, "Ettin");				iProbSA = 20; iKindSA = 8;  break;
                            case 60:  strcpy(cNpcName, "Cannibal-Plant");	iProbSA = 20; iKindSA = 5;  break;
                            case 61:  strcpy(cNpcName, "Rudolph");			iProbSA = 20; iKindSA = 1;  break;
                            case 62:  strcpy(cNpcName, "DireBoar");			iProbSA = 20; iKindSA = 1;  break;
                            case 63:  strcpy(cNpcName, "Frost");				iProbSA = 20; iKindSA = 8;  break;
                            case 65:  strcpy(cNpcName, "Ice-Golem");			iProbSA = 20; iKindSA = 8;  break;
                            case 66:  strcpy(cNpcName, "Wyvern");			iProbSA = 20; iKindSA = 1;  break;
                            case 55:  strcpy(cNpcName, "Rabbit");			iProbSA = 20; iKindSA = 1;  break;
                            case 67:  strcpy(cNpcName, "McGaffin");			iProbSA = 20; iKindSA = 1;  break;
                            case 68:  strcpy(cNpcName, "Perry");				iProbSA = 20; iKindSA = 1;  break;
                            case 69:  strcpy(cNpcName, "Devlin");			iProbSA = 20; iKindSA = 1;  break;
                            case 73:  strcpy(cNpcName, "Fire-Wyvern");		iProbSA = 20; iKindSA = 1;  break;
                            case 70:  strcpy(cNpcName, "Barlog");			iProbSA = 20; iKindSA = 1;  break;
                            case 80:  strcpy(cNpcName, "Tentocle");			iProbSA = 20; iKindSA = 1;  break;
                            case 71:  strcpy(cNpcName, "Centaurus");			iProbSA = 20; iKindSA = 1;  break;
                            case 75:  strcpy(cNpcName, "Giant-Lizard");		iProbSA = 20; iKindSA = 1;  break;
                            case 78:  strcpy(cNpcName, "Minotaurs");			iProbSA = 20; iKindSA = 1;  break;
                            case 81:  strcpy(cNpcName, "Abaddon");			iProbSA = 20; iKindSA = 1;  break;
                            case 72:  strcpy(cNpcName, "Claw-Turtle");		iProbSA = 20; iKindSA = 1;  break;
                            case 74:  strcpy(cNpcName, "Giant-Crayfish");	iProbSA = 20; iKindSA = 1;  break;
                            case 76:  strcpy(cNpcName, "Giant-Plant");		iProbSA = 20; iKindSA = 1;  break;
                            case 77:  strcpy(cNpcName, "MasterMage-Orc");	iProbSA = 20; iKindSA = 1;  break;
                            case 79:  strcpy(cNpcName, "Nizie");				iProbSA = 20; iKindSA = 1; break;
                            default:
                                strcpy(cNpcName, "Orc");
                                iProbSA = 15;
                                iKindSA = 1;
                                break;
                        }
                        /* NPCs not spawning in pits:
                        case 56:  strcpy(cNpcName,"Cat");				iProbSA = 15; iKindSA = 6;  break;
                        case 28:  strcpy(cNpcName, "Troll");			iProbSA = 25; iKindSA = 3; break;

                        // 15 ShopKeeper-W
                        // 19 Gandlf
                        // 20 Howard
                        // 36 Arrow Gaurd Tower Kit - Aresden, Elvine
                        // 37 Cannon Gaurd Tower Kit - Aresden, Elvine
                        // 38 Mana Collector Kit - Aresden, Elvine
                        // 39 Detector Constructor Kit - Aresden, Elvine
                        // 40 Energy Shield Generator - Aresden, Elvine
                        // 41 Grand Master Generator - Aresden Elvine
                        // 43 Light War Beetle - Aresden, Elvine
                        // 44 God's Hand Knight
                        // 45 Mounted God's Hand Knight
                        // 46 Temple Knight
                        // 47 Battle Golem
                        // 51 Catapult
                        // 64 Crops
                        */
                        bFirmBerserk = false;
                        if ((iMapLevel == 5) && (iDice(1, 3) == 1)) bFirmBerserk = true;

                        memset(cName_Master, 0, sizeof(cName_Master));
                        copy_string(cName_Master, "XX%d", iNamingValue);
                        cName_Master[0] = 95; // original '_';
                        cName_Master[1] = i + 65;

                        cSA = 0;
                        if ((m_pMapList[i]->m_stSpotMobGenerator[j].iMobType != 34) && (iDice(1, 100) <= iProbSA))
                        {
                            cSA = _cGetSpecialAbility(iKindSA);
                        }

                        switch (m_pMapList[i]->m_stSpotMobGenerator[j].cType)
                        {
                            case 1:
                                if (bCreateNewNpc(cNpcName, cName_Master, m_pMapList[i]->m_cName, (rand() % 3), cSA, DEF_MOVETYPE_RANDOMAREA, &pX, &pY, cWaypoint, &m_pMapList[i]->m_stSpotMobGenerator[j].rcRect, j, -1, false, false, bFirmBerserk) == false)
                                {
                                    m_pMapList[i]->SetNamingValueEmpty(iNamingValue);
                                }
                                else
                                {
                                    m_pMapList[i]->m_stSpotMobGenerator[j].iCurMobs++;
                                }
                                break;

                            case 2:
                                if (bCreateNewNpc(cNpcName, cName_Master, m_pMapList[i]->m_cName, (rand() % 3), cSA, DEF_MOVETYPE_RANDOMWAYPOINT, 0, 0, m_pMapList[i]->m_stSpotMobGenerator[j].cWaypoint, 0, j, -1, false, false, bFirmBerserk) == false)
                                {
                                    m_pMapList[i]->SetNamingValueEmpty(iNamingValue);
                                }
                                else
                                {
                                    m_pMapList[i]->m_stSpotMobGenerator[j].iCurMobs++;
                                }
                                break;
                        }
                    }
                }
        }
    }
}
