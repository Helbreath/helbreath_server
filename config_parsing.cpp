//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include "Game.h"


bool CGame::_bDecodeBuildItemConfigFileContents(char * pData, uint32_t dwMsgSize)
{
    char * pContents, * token;
    char seps[] = "= \t\n";
    char cReadModeA = 0;
    char cReadModeB = 0;
    int  i, iIndex = 0;
    class CStrTok * pStrTok;
    class CItem * pItem;

    pContents = new char[dwMsgSize + 1];
    memset(pContents, 0, dwMsgSize + 1);
    memcpy(pContents, pData, dwMsgSize);

    pStrTok = new class CStrTok(pContents, seps);
    token = pStrTok->pGet();
    while (token != NULL)
    {
        if (cReadModeA != 0)
        {
            switch (cReadModeA)
            {
                case 1:
                    switch (cReadModeB)
                    {
                        case 1:
                            memset(m_pBuildItemList[iIndex]->m_cName, 0, sizeof(m_pBuildItemList[iIndex]->m_cName));
                            memcpy(m_pBuildItemList[iIndex]->m_cName, token, strlen(token));

                            cReadModeB = 2;
                            break;

                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format(1).");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pBuildItemList[iIndex]->m_iSkillLimit = atoi(token);
                            cReadModeB = 3;
                            break;

                        case 3:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pBuildItemList[iIndex]->m_iMaterialItemID[0] = atoi(token);
                            cReadModeB = 4;
                            break;

                        case 4:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pBuildItemList[iIndex]->m_iMaterialItemCount[0] = atoi(token);
                            cReadModeB = 5;
                            break;

                        case 5:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pBuildItemList[iIndex]->m_iMaterialItemValue[0] = atoi(token);
                            cReadModeB = 6;
                            break;

                        case 6:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pBuildItemList[iIndex]->m_iMaterialItemID[1] = atoi(token);
                            cReadModeB = 7;
                            break;

                        case 7:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pBuildItemList[iIndex]->m_iMaterialItemCount[1] = atoi(token);
                            cReadModeB = 8;
                            break;

                        case 8:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pBuildItemList[iIndex]->m_iMaterialItemValue[1] = atoi(token);
                            cReadModeB = 9;
                            break;

                        case 9:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pBuildItemList[iIndex]->m_iMaterialItemID[2] = atoi(token);
                            cReadModeB = 10;
                            break;

                        case 10:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pBuildItemList[iIndex]->m_iMaterialItemCount[2] = atoi(token);
                            cReadModeB = 11;
                            break;

                        case 11:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pBuildItemList[iIndex]->m_iMaterialItemValue[2] = atoi(token);
                            cReadModeB = 12;
                            break;


                        case 12:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pBuildItemList[iIndex]->m_iMaterialItemID[3] = atoi(token);
                            cReadModeB = 13;
                            break;

                        case 13:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pBuildItemList[iIndex]->m_iMaterialItemCount[3] = atoi(token);
                            cReadModeB = 14;
                            break;

                        case 14:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pBuildItemList[iIndex]->m_iMaterialItemValue[3] = atoi(token);
                            cReadModeB = 15;
                            break;

                        case 15:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pBuildItemList[iIndex]->m_iMaterialItemID[4] = atoi(token);
                            cReadModeB = 16;
                            break;

                        case 16:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pBuildItemList[iIndex]->m_iMaterialItemCount[4] = atoi(token);

                            cReadModeB = 17;
                            break;

                        case 17:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pBuildItemList[iIndex]->m_iMaterialItemValue[4] = atoi(token);

                            cReadModeB = 18;
                            break;


                        case 18:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pBuildItemList[iIndex]->m_iMaterialItemID[5] = atoi(token);
                            cReadModeB = 19;
                            break;

                        case 19:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pBuildItemList[iIndex]->m_iMaterialItemCount[5] = atoi(token);

                            cReadModeB = 20;
                            break;

                        case 20:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pBuildItemList[iIndex]->m_iMaterialItemValue[5] = atoi(token);

                            cReadModeB = 21;
                            break;



                        case 21:
                            // m_iAverageValue
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pBuildItemList[iIndex]->m_iAverageValue = atoi(token);

                            cReadModeB = 22;
                            break;

                        case 22:
                            // m_iMaxSkill
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pBuildItemList[iIndex]->m_iMaxSkill = atoi(token);

                            cReadModeB = 23;
                            break;

                        case 23:
                            // m_wAttribute
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            m_pBuildItemList[iIndex]->m_wAttribute = atoi(token);

                            cReadModeA = 0;
                            cReadModeB = 0;

                            pItem = new class CItem;
                            if (_bInitItemAttr(pItem, m_pBuildItemList[iIndex]->m_cName) == true)
                            {
                                m_pBuildItemList[iIndex]->m_sItemID = pItem->m_sIDnum;

                                for (i = 0; i < 6; i++)
                                    m_pBuildItemList[iIndex]->m_iMaxValue += (m_pBuildItemList[iIndex]->m_iMaterialItemValue[i] * 100);

                                iIndex++;
                            }
                            else
                            {
                                log->critical("(!!!) CRITICAL ERROR! BuildItem configuration file error - Not Existing Item({})", m_pBuildItemList[iIndex]->m_cName);

                                delete m_pBuildItemList[iIndex];
                                m_pBuildItemList[iIndex] = NULL;

                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            delete pItem;
                            break;
                    }
                    break;

                default:
                    break;
            }
        }
        else
        {
            if (memcmp(token, "BuildItem", 9) == 0)
            {
                cReadModeA = 1;
                cReadModeB = 1;
                m_pBuildItemList[iIndex] = new class CBuildItem;
            }

        }
        token = pStrTok->pGet();
    }

    delete pStrTok;
    delete pContents;

    if ((cReadModeA != 0) || (cReadModeB != 0))
    {
        log->critical("(!!!) CRITICAL ERROR! BuildItem configuration file contents error!");
        return false;
    }

    log->info("(!) BuildItem(Total:{}) configuration - success!", iIndex);

    return true;
}


bool CGame::_bDecodeDupItemIDFileContents(char * pData, uint32_t dwMsgSize)
{
    char * pContents, * token;
    char seps[] = "= \t\n";
    char cReadModeA = 0;
    char cReadModeB = 0;
    int  iIndex = 0;
    class CStrTok * pStrTok;

    pContents = new char[dwMsgSize + 1];
    memset(pContents, 0, dwMsgSize + 1);
    memcpy(pContents, pData, dwMsgSize);

    pStrTok = new class CStrTok(pContents, seps);
    token = pStrTok->pGet();

    while (token != NULL)
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
                                log->info("(!!!) ERROR! DupItemID configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            if (m_pDupItemIDList[atoi(token)] != NULL)
                            {
                                log->info("(!!!) ERROR! DupItemID configuration file error - Duplicate magic number.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pDupItemIDList[atoi(token)] = new class CItem;
                            iIndex = atoi(token);

                            cReadModeB = 2;
                            break;

                        case 2:
                            // m_sTouchEffectType
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->info("(!!!) ERROR! DupItemID configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pDupItemIDList[iIndex]->m_sTouchEffectType = atoi(token);
                            cReadModeB = 3;
                            break;

                        case 3:
                            // m_sTouchEffectValue1
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->info("(!!!) ERROR! DupItemID configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pDupItemIDList[iIndex]->m_sTouchEffectValue1 = atoi(token);
                            cReadModeB = 4;
                            break;

                        case 4:
                            // m_sTouchEffectValue2
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->info("(!!!) ERROR! DupItemID configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pDupItemIDList[iIndex]->m_sTouchEffectValue2 = atoi(token);
                            cReadModeB = 5;
                            break;

                        case 5:
                            // m_sTouchEffectValue3
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->info("(!!!) ERROR! DupItemID configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pDupItemIDList[iIndex]->m_sTouchEffectValue3 = atoi(token);
                            cReadModeB = 6;
                            break;

                        case 6:
                            // m_wPrice
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->info("(!!!) ERROR! DupItemID configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pDupItemIDList[iIndex]->m_wPrice = (uint16_t)atoi(token);
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
            if (memcmp(token, "DupItemID", 9) == 0)
            {
                cReadModeA = 1;
                cReadModeB = 1;
            }

        }
        token = pStrTok->pGet();
    }

    delete pStrTok;
    delete pContents;

    if ((cReadModeA != 0) || (cReadModeB != 0))
    {
        log->info("(!!!) ERROR! DupItemID configuration file contents error!");
        return false;
    }

    log->info("(!) DupItemID(Total:{}) configuration - success!", iIndex);

    return true;
}

bool CGame::_bDecodeItemConfigFileContents(char * pData, uint32_t dwMsgSize)
{
    char * pContents, * token;
    char seps[] = "= \t\n";
    char cReadModeA = 0;
    char cReadModeB = 0;
    int  iItemConfigListIndex, iTemp;
    class CStrTok * pStrTok;

    pContents = new char[dwMsgSize + 1];
    memset(pContents, 0, dwMsgSize + 1);
    memcpy(pContents, pData, dwMsgSize);

    pStrTok = new class CStrTok(pContents, seps);
    token = pStrTok->pGet();
    while (token != NULL)
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
                                log->critical("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemIDnumber");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            iItemConfigListIndex = atoi(token);

                            if (m_pItemConfigList[iItemConfigListIndex] != NULL)
                            {
                                log->critical("(!!!) CRITICAL ERROR! Duplicate ItemIDnum({})", iItemConfigListIndex);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pItemConfigList[iItemConfigListIndex] = new class CItem;
                            m_pItemConfigList[iItemConfigListIndex]->m_sIDnum = iItemConfigListIndex;
                            cReadModeB = 2;
                            break;
                        case 2:
                            // m_cName 
                            memset(m_pItemConfigList[iItemConfigListIndex]->m_cName, 0, sizeof(m_pItemConfigList[iItemConfigListIndex]->m_cName));
                            memcpy(m_pItemConfigList[iItemConfigListIndex]->m_cName, token, strlen(token));
                            cReadModeB = 3;
                            break;
                        case 3:
                            // m_cItemType
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemType");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pItemConfigList[iItemConfigListIndex]->m_cItemType = atoi(token);
                            cReadModeB = 4;
                            break;
                        case 4:
                            // m_cEquipPos
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! ITEM configuration file error - EquipPos");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pItemConfigList[iItemConfigListIndex]->m_cEquipPos = atoi(token);
                            cReadModeB = 5;
                            break;
                        case 5:
                            // m_sItemEffectType
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemEffectType");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pItemConfigList[iItemConfigListIndex]->m_sItemEffectType = atoi(token);
                            cReadModeB = 6;
                            break;
                        case 6:
                            // m_sItemEffectValue1
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemEffectValue1");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pItemConfigList[iItemConfigListIndex]->m_sItemEffectValue1 = atoi(token);
                            cReadModeB = 7;
                            break;
                        case 7:
                            // m_sItemEffectValue2
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemEffectValue2");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pItemConfigList[iItemConfigListIndex]->m_sItemEffectValue2 = atoi(token);
                            cReadModeB = 8;
                            break;
                        case 8:
                            // m_sItemEffectValue3
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemEffectValue3");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pItemConfigList[iItemConfigListIndex]->m_sItemEffectValue3 = atoi(token);
                            cReadModeB = 9;
                            break;
                        case 9:
                            // m_sItemEffectValue4
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemEffectValue4");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pItemConfigList[iItemConfigListIndex]->m_sItemEffectValue4 = atoi(token);
                            cReadModeB = 10;
                            break;
                        case 10:
                            // m_sItemEffectValue5
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemEffectValue5");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pItemConfigList[iItemConfigListIndex]->m_sItemEffectValue5 = atoi(token);
                            cReadModeB = 11;
                            break;
                        case 11:
                            // m_sItemEffectValue6
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemEffectValue6");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pItemConfigList[iItemConfigListIndex]->m_sItemEffectValue6 = atoi(token);
                            cReadModeB = 12;
                            break;
                        case 12:
                            // m_wMaxLifeSpan
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! ITEM configuration file error - MaxLifeSpan");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pItemConfigList[iItemConfigListIndex]->m_wMaxLifeSpan = (uint16_t)atoi(token);
                            cReadModeB = 13;
                            break;
                        case 13:
                            // m_sSpecialEffect
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! ITEM configuration file error - MaxFixCount");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pItemConfigList[iItemConfigListIndex]->m_sSpecialEffect = atoi(token);
                            cReadModeB = 14;
                            break;
                        case 14:
                            // m_sSprite
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! ITEM configuration file error - Sprite");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pItemConfigList[iItemConfigListIndex]->m_sSprite = atoi(token);
                            cReadModeB = 15;
                            break;
                        case 15:
                            // m_sSpriteFrame
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! ITEM configuration file error - SpriteFrame");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pItemConfigList[iItemConfigListIndex]->m_sSpriteFrame = atoi(token);
                            cReadModeB = 16;
                            break;
                        case 16:
                            // m_wPrice
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! ITEM configuration file error - Price");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            iTemp = atoi(token);
                            if (iTemp < 0)
                                m_pItemConfigList[iItemConfigListIndex]->m_bIsForSale = false;
                            else m_pItemConfigList[iItemConfigListIndex]->m_bIsForSale = true;

                            m_pItemConfigList[iItemConfigListIndex]->m_wPrice = abs(iTemp);
                            cReadModeB = 17;
                            break;
                        case 17:
                            // m_wWeight
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! ITEM configuration file error - Weight");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pItemConfigList[iItemConfigListIndex]->m_wWeight = atoi(token);
                            cReadModeB = 18;
                            break;
                        case 18:
                            // Appr Value
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! ITEM configuration file error - ApprValue");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pItemConfigList[iItemConfigListIndex]->m_cApprValue = atoi(token);
                            cReadModeB = 19;
                            break;
                        case 19:
                            // m_cSpeed
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! ITEM configuration file error - Speed");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pItemConfigList[iItemConfigListIndex]->m_cSpeed = atoi(token);
                            cReadModeB = 20;
                            break;

                        case 20:
                            // m_sLevelLimit
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! ITEM configuration file error - LevelLimit");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pItemConfigList[iItemConfigListIndex]->m_sLevelLimit = atoi(token);
                            cReadModeB = 21;
                            break;

                        case 21:
                            // m_cGederLimit
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! ITEM configuration file error - GenderLimit");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pItemConfigList[iItemConfigListIndex]->m_cGenderLimit = atoi(token);
                            cReadModeB = 22;
                            break;

                        case 22:
                            // m_sSpecialEffectValue1
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! ITEM configuration file error - SM_HitRatio");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pItemConfigList[iItemConfigListIndex]->m_sSpecialEffectValue1 = atoi(token);
                            cReadModeB = 23;
                            break;

                        case 23:
                            // m_sSpecialEffectValue2
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! ITEM configuration file error - L_HitRatio");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pItemConfigList[iItemConfigListIndex]->m_sSpecialEffectValue2 = atoi(token);
                            cReadModeB = 24;
                            break;

                        case 24:
                            // m_sRelatedSkill
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! ITEM configuration file error - RelatedSkill");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pItemConfigList[iItemConfigListIndex]->m_sRelatedSkill = atoi(token);
                            cReadModeB = 25;
                            break;

                        case 25:
                            // m_cCategory
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! ITEM configuration file error - Category");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pItemConfigList[iItemConfigListIndex]->m_cCategory = atoi(token);
                            cReadModeB = 26;
                            break;

                        case 26:
                            // m_cItemColor
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! ITEM configuration file error - Category");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pItemConfigList[iItemConfigListIndex]->m_cItemColor = atoi(token);
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
            if (memcmp(token, "Item", 4) == 0)
            {
                cReadModeA = 1;
                cReadModeB = 1;
            }

            if (memcmp(token, "[ENDITEMLIST]", 13) == 0)
            {
                cReadModeA = 0;
                cReadModeB = 0;
                goto DICFC_STOPDECODING;
            }
        }
        token = pStrTok->pGet();
    }

    DICFC_STOPDECODING:;

    delete pStrTok;
    delete pContents;

    if ((cReadModeA != 0) || (cReadModeB != 0))
    {
        log->critical("(!!!) CRITICAL ERROR! ITEM configuration file contents error!");
        return false;
    }

    log->info("(!) ITEM(Total:{}) configuration - success!", iItemConfigListIndex);

    return true;
}

bool CGame::_bDecodeMagicConfigFileContents(char * pData, uint32_t dwMsgSize)
{
    char * pContents, * token;
    char seps[] = "= \t\n";
    char cReadModeA = 0;
    char cReadModeB = 0;
    int  iMagicConfigListIndex = 0;
    class CStrTok * pStrTok;

    pContents = new char[dwMsgSize + 1];
    memset(pContents, 0, dwMsgSize + 1);
    memcpy(pContents, pData, dwMsgSize);

    pStrTok = new class CStrTok(pContents, seps);
    token = pStrTok->pGet();
    while (token != NULL)
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
                                log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            if (m_pMagicConfigList[atoi(token)] != NULL)
                            {
                                log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file error - Duplicate magic number.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMagicConfigList[atoi(token)] = new class CMagic;
                            iMagicConfigListIndex = atoi(token);

                            cReadModeB = 2;
                            break;

                        case 2:
                            memset(m_pMagicConfigList[iMagicConfigListIndex]->m_cName, 0, sizeof(m_pMagicConfigList[iMagicConfigListIndex]->m_cName));
                            memcpy(m_pMagicConfigList[iMagicConfigListIndex]->m_cName, token, strlen(token));
                            cReadModeB = 3;
                            break;

                        case 3:
                            // m_sType
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMagicConfigList[iMagicConfigListIndex]->m_sType = atoi(token);
                            cReadModeB = 4;
                            break;

                        case 4:
                            // m_dwDelayTime
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMagicConfigList[iMagicConfigListIndex]->m_dwDelayTime = atoi(token);
                            cReadModeB = 5;
                            break;

                        case 5:
                            // m_dwLastTime
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMagicConfigList[iMagicConfigListIndex]->m_dwLastTime = atoi(token);
                            cReadModeB = 6;
                            break;

                        case 6:
                            // m_sValue1
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMagicConfigList[iMagicConfigListIndex]->m_sValue1 = atoi(token);
                            cReadModeB = 7;
                            break;

                        case 7:
                            // m_sValue2
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMagicConfigList[iMagicConfigListIndex]->m_sValue2 = atoi(token);
                            cReadModeB = 8;
                            break;

                        case 8:
                            // m_sValue3
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMagicConfigList[iMagicConfigListIndex]->m_sValue3 = atoi(token);
                            cReadModeB = 9;
                            break;

                        case 9:
                            // m_sValue4
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMagicConfigList[iMagicConfigListIndex]->m_sValue4 = atoi(token);
                            cReadModeB = 10;
                            break;

                        case 10:
                            // m_sValue5
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMagicConfigList[iMagicConfigListIndex]->m_sValue5 = atoi(token);
                            cReadModeB = 11;
                            break;

                        case 11:
                            // m_sValue6
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMagicConfigList[iMagicConfigListIndex]->m_sValue6 = atoi(token);
                            cReadModeB = 12;
                            break;

                        case 12:
                            // m_sValue7
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMagicConfigList[iMagicConfigListIndex]->m_sValue7 = atoi(token);
                            cReadModeB = 13;
                            break;

                        case 13:
                            // m_sValue8
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMagicConfigList[iMagicConfigListIndex]->m_sValue8 = atoi(token);
                            cReadModeB = 14;
                            break;

                        case 14:
                            // m_sValue9
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMagicConfigList[iMagicConfigListIndex]->m_sValue9 = atoi(token);
                            cReadModeB = 15;
                            break;

                        case 15:
                            // m_sValue10
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMagicConfigList[iMagicConfigListIndex]->m_sValue10 = atoi(token);
                            cReadModeB = 16;
                            break;

                        case 16:
                            // m_sValue11
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMagicConfigList[iMagicConfigListIndex]->m_sValue11 = atoi(token);
                            cReadModeB = 17;
                            break;

                        case 17:
                            // m_sValue12
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMagicConfigList[iMagicConfigListIndex]->m_sValue12 = atoi(token);
                            cReadModeB = 18;
                            break;

                        case 18:
                            // m_sIntLimit
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMagicConfigList[iMagicConfigListIndex]->m_sIntLimit = atoi(token);
                            cReadModeB = 19;
                            break;

                        case 19:
                            // m_iGoldCost
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMagicConfigList[iMagicConfigListIndex]->m_iGoldCost = atoi(token);

                            cReadModeB = 20;
                            break;

                        case 20:
                            // m_cCategory
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMagicConfigList[iMagicConfigListIndex]->m_cCategory = atoi(token);
                            cReadModeB = 21;
                            break;

                        case 21:
                            // m_iAttribute
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMagicConfigList[iMagicConfigListIndex]->m_iAttribute = atoi(token);
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
            if (memcmp(token, "magic", 5) == 0)
            {
                cReadModeA = 1;
                cReadModeB = 1;
            }

        }
        token = pStrTok->pGet();
    }

    delete pStrTok;
    delete pContents;

    if ((cReadModeA != 0) || (cReadModeB != 0))
    {
        log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file contents error!");
        return false;
    }

    log->info("(!) MAGIC(Total:{}) configuration - success!", iMagicConfigListIndex);

    return true;

}



bool CGame::_bDecodeSkillConfigFileContents(char * pData, uint32_t dwMsgSize)
{
    char * pContents, * token;
    char seps[] = "= \t\n";
    char cReadModeA = 0;
    char cReadModeB = 0;
    int  iSkillConfigListIndex = 0;
    class CStrTok * pStrTok;

    pContents = new char[dwMsgSize + 1];
    memset(pContents, 0, dwMsgSize + 1);
    memcpy(pContents, pData, dwMsgSize);

    pStrTok = new class CStrTok(pContents, seps);
    token = pStrTok->pGet();
    while (token != NULL)
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
                                log->critical("(!!!) CRITICAL ERROR! SKILL configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            if (m_pSkillConfigList[atoi(token)] != NULL)
                            {
                                log->critical("(!!!) CRITICAL ERROR! SKILL configuration file error - Duplicate magic number.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pSkillConfigList[atoi(token)] = new class CSkill;
                            iSkillConfigListIndex = atoi(token);

                            cReadModeB = 2;
                            break;

                        case 2:
                            memset(m_pSkillConfigList[iSkillConfigListIndex]->m_cName, 0, sizeof(m_pSkillConfigList[iSkillConfigListIndex]->m_cName));
                            memcpy(m_pSkillConfigList[iSkillConfigListIndex]->m_cName, token, strlen(token));
                            cReadModeB = 3;
                            break;

                        case 3:
                            // m_sType
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! SKILL configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pSkillConfigList[iSkillConfigListIndex]->m_sType = atoi(token);
                            cReadModeB = 4;
                            break;

                        case 4:
                            // m_sValue1
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! SKILL configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pSkillConfigList[iSkillConfigListIndex]->m_sValue1 = atoi(token);
                            cReadModeB = 5;
                            break;

                        case 5:
                            // m_sValue2
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! SKILL configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pSkillConfigList[iSkillConfigListIndex]->m_sValue2 = atoi(token);
                            cReadModeB = 6;
                            break;

                        case 6:
                            // m_sValue3
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! SKILL configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pSkillConfigList[iSkillConfigListIndex]->m_sValue3 = atoi(token);
                            cReadModeB = 7;
                            break;

                        case 7:
                            // m_sValue4
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! SKILL configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pSkillConfigList[iSkillConfigListIndex]->m_sValue4 = atoi(token);
                            cReadModeB = 8;
                            break;

                        case 8:
                            // m_sValue5
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! SKILL configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pSkillConfigList[iSkillConfigListIndex]->m_sValue5 = atoi(token);
                            cReadModeB = 9;
                            break;

                        case 9:
                            // m_sValue6
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! SKILL configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pSkillConfigList[iSkillConfigListIndex]->m_sValue6 = atoi(token);
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
            if (memcmp(token, "skill", 5) == 0)
            {
                cReadModeA = 1;
                cReadModeB = 1;
            }

        }
        token = pStrTok->pGet();
    }

    delete pStrTok;
    delete pContents;

    if ((cReadModeA != 0) || (cReadModeB != 0))
    {
        log->critical("(!!!) CRITICAL ERROR! SKILL configuration file contents error!");
        return false;
    }

    log->info("(!) SKILL(Total:{}) configuration - success!", iSkillConfigListIndex);

    return true;
}

void CGame::_bDecodeNoticementFileContents(char * pData, uint32_t dwMsgSize)
{
    char * cp;

    cp = (pData);

    if (m_pNoticementData != NULL)
    {
        delete m_pNoticementData;
        m_pNoticementData = NULL;
    }

    m_pNoticementData = new char[strlen(cp) + 2];
    memset(m_pNoticementData, 0, strlen(cp) + 2);

    memcpy(m_pNoticementData, cp, strlen(cp));
    m_dwNoticementDataSize = strlen(cp);

    log->info("(!) Noticement Data Size: {}", m_dwNoticementDataSize);
}

bool CGame::_bDecodeNpcConfigFileContents(char * pData, uint32_t dwMsgSize)
{
    char * pContents, * token;
    char seps[] = "= \t\n";
    char cReadModeA = 0;
    char cReadModeB = 0;
    int  iNpcConfigListIndex = 0;
    class CStrTok * pStrTok;

    pContents = new char[dwMsgSize + 1];
    memset(pContents, 0, dwMsgSize + 1);
    memcpy(pContents, pData, dwMsgSize);

    pStrTok = new class CStrTok(pContents, seps);
    token = pStrTok->pGet();
    while (token != NULL)
    {
        if (cReadModeA != 0)
        {
            switch (cReadModeA)
            {
                case 1:
                    switch (cReadModeB)
                    {
                        case 1:
                            // NPC
                            if (strlen(token) > 20)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Too long NPC name.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            memset(m_pNpcConfigList[iNpcConfigListIndex]->m_cNpcName, 0, sizeof(m_pNpcConfigList[iNpcConfigListIndex]->m_cNpcName));
                            memcpy(m_pNpcConfigList[iNpcConfigListIndex]->m_cNpcName, token, strlen(token));
                            cReadModeB = 2;
                            break;
                        case 2:
                            // m_sType
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_sType = atoi(token);
                            cReadModeB = 3;
                            break;
                        case 3:
                            // m_iHitDice
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_iHitDice = atoi(token);
                            cReadModeB = 4;
                            break;
                        case 4:
                            // m_iDefenseRatio
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_iDefenseRatio = atoi(token);
                            cReadModeB = 5;
                            break;
                        case 5:
                            // m_iHitRatio
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_iHitRatio = atoi(token);
                            cReadModeB = 6;
                            break;
                        case 6:
                            // m_iMinBravery
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_iMinBravery = atoi(token);
                            cReadModeB = 7;
                            break;
                        case 7:
                            // m_iExpDiceMin
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_iExpDiceMin = atoi(token);
                            cReadModeB = 8;
                            break;

                        case 8:
                            // m_iExpDiceMax
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_iExpDiceMax = atoi(token);
                            cReadModeB = 9;
                            break;

                        case 9:
                            // m_iGoldDiceMin
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_iGoldDiceMin = atoi(token);
                            cReadModeB = 10;
                            break;

                        case 10:
                            // m_iGoldDiceMax
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_iGoldDiceMax = atoi(token);
                            cReadModeB = 11;
                            break;

                        case 11:
                            // m_cAttackDiceThrow
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_cAttackDiceThrow = atoi(token);
                            cReadModeB = 12;
                            break;

                        case 12:
                            // m_cAttackDiceRange
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_cAttackDiceRange = atoi(token);
                            cReadModeB = 13;
                            break;

                        case 13:
                            // m_cSize
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_cSize = atoi(token);
                            cReadModeB = 14;
                            break;

                        case 14:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_cSide = atoi(token);
                            cReadModeB = 15;
                            break;

                        case 15:
                            // ActionLimit 
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_cActionLimit = atoi(token);
                            cReadModeB = 16;
                            break;

                        case 16:
                            // Action Time
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_dwActionTime = atoi(token);
                            cReadModeB = 17;
                            break;

                        case 17:
                            // ResistMagic
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_cResistMagic = atoi(token);
                            cReadModeB = 18;
                            break;

                        case 18:
                            // cMagicLevel
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_cMagicLevel = atoi(token);
                            cReadModeB = 19;
                            break;

                        case 19:
                            // cGenDayWeekLimit
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_cDayOfWeekLimit = atoi(token);
                            cReadModeB = 20;
                            break;

                        case 20:
                            // cChatMsgPresence
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_cChatMsgPresence = atoi(token);

                            if (m_pNpcConfigList[iNpcConfigListIndex]->m_cChatMsgPresence == 1)
                            {

                            }
                            cReadModeB = 21;
                            break;

                        case 21:
                            // m_cTargetSearchRange
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_cTargetSearchRange = atoi(token);

                            cReadModeB = 22;
                            break;

                        case 22:
                            // Npc
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_dwRegenTime = atoi(token);

                            cReadModeB = 23;
                            break;

                        case 23:
                            // Attribute
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_cAttribute = atoi(token);

                            cReadModeB = 24;
                            break;

                        case 24:
                            // Absorb Magic Damage
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_iAbsDamage = atoi(token);

                            cReadModeB = 25;
                            break;

                        case 25:
                            // Maximum Mana Point
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_iMaxMana = atoi(token);

                            cReadModeB = 26;
                            break;

                        case 26:
                            // MagicHitRatio
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_iMagicHitRatio = atoi(token);

                            cReadModeB = 27;
                            break;

                        case 27:
                            // AttackRange
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! NPC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pNpcConfigList[iNpcConfigListIndex]->m_iAttackRange = atoi(token);

                            cReadModeA = 0;
                            cReadModeB = 0;
                            iNpcConfigListIndex++;
                            break;
                    }
                    break;

                case 2:
                    m_iPlayerMaxLevel = atoi(token);
                    cReadModeA = 0;
                    cReadModeB = 0;
                    break;

                default:
                    break;
            }
        }
        else
        {
            if (memcmp(token, "Npc", 3) == 0)
            {
                cReadModeA = 1;
                cReadModeB = 1;
                m_pNpcConfigList[iNpcConfigListIndex] = new class CNpc(" ");
            }

            if (memcmp(token, "world-server-max-level", 22) == 0)
            {
                cReadModeA = 2;
                cReadModeB = 1;
            }
        }
        token = pStrTok->pGet();
    }

    delete pStrTok;
    delete pContents;

    if ((cReadModeA != 0) || (cReadModeB != 0))
    {
        log->critical("(!!!) CRITICAL ERROR! NPC configuration file contents error!");
        return false;
    }

    log->info("(!) NPC(Total:{}) configuration - success!", iNpcConfigListIndex);

    return true;
}

bool CGame::_bDecodeOccupyFlagSaveFileContents(char * pData, uint32_t dwMsgSize)
{
    char * pContents, * token;
    char seps[] = "= \t\n";
    char cReadModeA = 0;
    char cReadModeB = 0;
    int  iFlagIndex = 0;
    int  iSide;
    class CStrTok * pStrTok;
    int dX, dY, iEKNum, iTotalFlags;

    if (m_iMiddlelandMapIndex < 0)
    {
        log->info("(!) This game server does not managing Middleland Map. OcuupyFlag data ignored.");
        return false;
    }

    iTotalFlags = 0;

    pContents = new char[dwMsgSize + 10];
    memset(pContents, 0, dwMsgSize + 10);
    memcpy(pContents, pData, dwMsgSize - 9);

    pStrTok = new CStrTok(pContents, seps);
    token = pStrTok->pGet();

    while (token != NULL)
    {
        if (cReadModeA != 0)
        {
            switch (cReadModeA)
            {
                case 1:
                    switch (cReadModeB)
                    {
                        case 1:
                            // Side
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! OccupyFlag save file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            iSide = atoi(token);
                            cReadModeB = 2;
                            break;

                        case 2:
                            // X
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! OccupyFlag save file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            dX = atoi(token);
                            cReadModeB = 3;
                            break;

                        case 3:
                            // Y
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! OccupyFlag save file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            dY = atoi(token);
                            cReadModeB = 4;
                            break;

                        case 4:
                            // EKNum
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! OccupyFlag save file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            iEKNum = atoi(token);

                            // (!!! Master Flag)
                            if (__bSetOccupyFlag(m_iMiddlelandMapIndex, dX, dY, iSide, iEKNum, -1, true) == true)
                                iTotalFlags++;

                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }

                default:
                    break;
            }
        }
        else
        {
            if (memcmp(token, "flag", 4) == 0)
            {
                cReadModeA = 1;
                cReadModeB = 1;
            }
        }
        token = pStrTok->pGet();
    }

    delete pStrTok;
    delete pContents;

    if ((cReadModeA != 0) || (cReadModeB != 0))
    {
        log->critical("(!!!) CRITICAL ERROR! OccupyFlag save file contents error!");
        return false;
    }

    log->info("(!) OccupyFlag save file decoding success! Total({})", iTotalFlags);

    return true;
}

bool CGame::_bDecodePlayerDatafileContents(int iClientH, char * pData, uint32_t dwSize)
{
    char * pContents, * token, * pOriginContents, cTmpName[11];
    char   seps[] = "= \t\n";
    char   cReadModeA, cReadModeB;
    int    i, iItemIndex, iItemInBankIndex, iTotalGold, iNotUsedItemPrice;
    class  CStrTok * pStrTok;
    short  sTmpType, sTmpAppr1;
    bool   bRet;
    int    iTemp;
    bool   bIsNotUsedItemFound = false;

    if (m_pClientList[iClientH] == NULL) return false;

    iTotalGold = 0;
    iItemIndex = 0;
    iItemInBankIndex = 0;
    iNotUsedItemPrice = 0;

    cReadModeA = 0;
    cReadModeB = 0;

    pContents = new char[dwSize + 2];
    memset(pContents, 0, dwSize + 2);
    memcpy(pContents, pData, dwSize);

    pOriginContents = pContents;

    pStrTok = new class CStrTok(pContents, seps);
    token = pStrTok->pGet();

    while (token != NULL)
    {
        if (cReadModeA != 0)
        {
            switch (cReadModeA)
            {
                case 1:
                    memset(m_pClientList[iClientH]->m_cMapName, 0, sizeof(m_pClientList[iClientH]->m_cMapName));
                    strcpy(m_pClientList[iClientH]->m_cMapName, token);
                    memset(cTmpName, 0, sizeof(cTmpName));
                    strcpy(cTmpName, token);
                    for (i = 0; i < DEF_MAXMAPS; i++)
                        if ((m_pMapList[i] != NULL) && (memcmp(m_pMapList[i]->m_cName, cTmpName, 10) == 0))
                        {
                            m_pClientList[iClientH]->m_cMapIndex = (char)i;
                        }

                    if (m_pClientList[iClientH]->m_cMapIndex == -1)
                    {
                        log->error("(!) Player({}) tries to enter unknown map : {%s}}", m_pClientList[iClientH]->m_cCharName, cTmpName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    cReadModeA = 0;
                    break;

                case 2:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_sX = atoi(token);
                    cReadModeA = 0;
                    break;

                case 3:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_sY = atoi(token);
                    cReadModeA = 0;
                    break;

                case 4:
                    /*
                    if (_bGetIsStringIsNumber(token) == false) {
                    log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                    delete pContents;
                    delete pStrTok;
                    return false;
                    }
                    m_pClientList[iClientH]->m_cAccountStatus = atoi(token);
                    */
                    cReadModeA = 0;
                    break;

                case 5:
                    switch (cReadModeB)
                    {
                        case 1:
                            // New 07/05/2004
                            // v2.12
                            if (iItemIndex >= DEF_MAXITEMS)
                            {
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            // token
                            if (_bInitItemAttr(m_pClientList[iClientH]->m_pItemList[iItemIndex], token) == false)
                            {
                                log->error("(!!!) Client({})-Item({}) is not existing Item! Connection closed.", m_pClientList[iClientH]->m_cCharName, token);

                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            cReadModeB = 2;
                            break;

                        case 2:
                            // m_dwCount
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            iTemp = atoi(token);
                            if (iTemp < 0) iTemp = 1;
                            if (iGetItemWeight(m_pClientList[iClientH]->m_pItemList[iItemIndex], iTemp) > _iCalcMaxLoad(iClientH))
                            {
                                iTemp = 1;
                                log->error("(!) Player({}) Item ({}) too heavy for player to carry", m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName);
                            }

                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwCount = (uint32_t)iTemp;
                            cReadModeB = 3;

                            // v1.3
                            if (memcmp(m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName, "Gold", 4) == 0)
                                iTotalGold += iTemp;
                            break;

                        case 3:
                            // m_sTouchEffectType
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectType = atoi(token);
                            cReadModeB = 4;
                            break;

                        case 4:
                            // m_sTouchEffectValue1
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 = atoi(token);
                            cReadModeB = 5;
                            break;

                        case 5:
                            // m_sTouchEffectValue2
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 = atoi(token);
                            cReadModeB = 6;
                            break;

                        case 6:
                            // m_sTouchEffectValue3
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 = atoi(token);
                            cReadModeB = 7;
                            break;

                        case 7:
                            // m_cItemColor
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor = atoi(token);
                            cReadModeB = 8;
                            break;

                        case 8:
                            // m_sItemSpecEffectValue1
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1 = atoi(token);
                            cReadModeB = 9;
                            break;

                        case 9:
                            // m_sItemSpecEffectValue2
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 = atoi(token);
                            cReadModeB = 10;
                            break;

                        case 10:
                            // m_sItemSpecEffectValue3
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue3 = atoi(token);
                            cReadModeB = 11;
                            break;

                        case 11:
                            // m_wCurLifeSpan
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan = atoi(token);

                            cReadModeB = 12;
                            break;

                        case 12:
                            // m_dwAttribute
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = atoi(token);

                            if (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectType == DEF_ITET_UNIQUE_OWNER)
                            {
                                if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 != m_pClientList[iClientH]->m_sCharIDnum1) ||
                                    (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 != m_pClientList[iClientH]->m_sCharIDnum2) ||
                                    (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 != m_pClientList[iClientH]->m_sCharIDnum3))
                                {
                                    log->error("(!) Unique item does not belong to Player({}) Item({}) {} {} {} - {} {} {}", m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName,
                                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1,
                                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2,
                                        m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3,
                                        m_pClientList[iClientH]->m_sCharIDnum1,
                                        m_pClientList[iClientH]->m_sCharIDnum2,
                                        m_pClientList[iClientH]->m_sCharIDnum3);
                                }
                            }

                            cReadModeA = 0;
                            cReadModeB = 0;

                            if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00000001) != NULL)
                            {
                                m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1;
                            }

                            _AdjustRareItemValue(m_pClientList[iClientH]->m_pItemList[iItemIndex]);

                            if (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan > m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan)
                                m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan;

                            if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan == 0) &&
                                (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ALTERITEMDROP))
                            {
                                log->error("(!) Player({}) Durability of item is 0 for 'alter item drop' (26) item!", m_pClientList[iClientH]->m_cCharName);
                                m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan = 1;
                            }

                            bCheckAndConvertPlusWeaponItem(iClientH, iItemIndex);

                            if (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemType == DEF_ITEMTYPE_NOTUSED)
                            {
                                iNotUsedItemPrice += m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wPrice;
                                delete m_pClientList[iClientH]->m_pItemList[iItemIndex];
                                m_pClientList[iClientH]->m_pItemList[iItemIndex] = NULL;

                                bIsNotUsedItemFound = true;
                            }
                            else
                                if (_bCheckDupItemID(m_pClientList[iClientH]->m_pItemList[iItemIndex]) == true)
                                {
                                    // v1.42
                                    _bItemLog(DEF_ITEMLOG_DUPITEMID, iClientH, nullptr, m_pClientList[iClientH]->m_pItemList[iItemIndex]);

                                    iNotUsedItemPrice += m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wPrice;
                                    delete m_pClientList[iClientH]->m_pItemList[iItemIndex];
                                    m_pClientList[iClientH]->m_pItemList[iItemIndex] = NULL;
                                }
                                else iItemIndex++;
                            break;
                    }
                    break;

                case 6:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_cSex = atoi(token);
                    cReadModeA = 0;
                    break;

                case 7:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_cSkin = atoi(token);
                    cReadModeA = 0;
                    break;

                case 8:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_cHairStyle = atoi(token);
                    cReadModeA = 0;
                    break;

                case 9:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_cHairColor = atoi(token);
                    cReadModeA = 0;
                    break;

                case 10:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_cUnderwear = atoi(token);
                    cReadModeA = 0;
                    break;

                case 11:
                    for (i = 0; i < DEF_MAXITEMEQUIPPOS; i++)
                        m_pClientList[iClientH]->m_sItemEquipmentStatus[i] = -1;

                    for (i = 0; i < DEF_MAXITEMS; i++)
                        m_pClientList[iClientH]->m_bIsItemEquipped[i] = false;

                    if (bIsNotUsedItemFound == false)
                    {
                        for (i = 0; i < DEF_MAXITEMS; i++)
                        {
                            if ((token[i] == '1') && (m_pClientList[iClientH]->m_pItemList[i] != NULL))
                            {
                                if (m_pClientList[iClientH]->m_pItemList[i]->m_cItemType == DEF_ITEMTYPE_EQUIP)
                                    m_pClientList[iClientH]->m_bIsItemEquipped[i] = true;
                                else m_pClientList[iClientH]->m_bIsItemEquipped[i] = false;
                            }
                            else m_pClientList[iClientH]->m_bIsItemEquipped[i] = false;

                            if ((m_pClientList[iClientH] != NULL) && (m_pClientList[iClientH]->m_bIsItemEquipped[i] == true))
                            {
                                if (bEquipItemHandler(iClientH, i) == false)
                                    m_pClientList[iClientH]->m_bIsItemEquipped[i] = false;
                            }
                        }
                    }

                    cReadModeA = 0;
                    break;

                case 12:
                    memset(m_pClientList[iClientH]->m_cGuildName, 0, sizeof(m_pClientList[iClientH]->m_cGuildName));
                    strcpy(m_pClientList[iClientH]->m_cGuildName, token);
                    cReadModeA = 0;
                    break;

                case 13:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_iGuildRank = atoi(token);
                    cReadModeA = 0;
                    break;

                case 14:
                    // HP 
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_iHP = atoi(token);
                    cReadModeA = 0;
                    break;

                case 15:
                    // DefenseRatio
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    //m_pClientList[iClientH]->m_iOriginDefenseRatio = atoi(token);
                    //m_pClientList[iClientH]->m_iCurDefenseRatio = m_pClientList[iClientH]->m_iOriginDefenseRatio;
                    cReadModeA = 0;
                    break;

                case 16:
                    // HitRatio
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    //m_pClientList[iClientH]->m_iOriginHitRatio = atoi(token);
                    //m_pClientList[iClientH]->m_iCurHitRatio = m_pClientList[iClientH]->m_iOriginHitRatio;
                    cReadModeA = 0;
                    break;

                case 17:
                    // Level  
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_iLevel = atoi(token);
                    cReadModeA = 0;
                    break;

                case 18:
                    // Str 
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_iStr = atoi(token);
                    cReadModeA = 0;
                    break;

                case 19:
                    // Int 
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_iInt = atoi(token);
                    cReadModeA = 0;
                    break;

                case 20:
                    // Vit 
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_iVit = atoi(token);
                    cReadModeA = 0;
                    break;

                case 21:
                    // Dex 
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_iDex = atoi(token);
                    cReadModeA = 0;
                    break;

                case 22:
                    // Mag 
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_iMag = atoi(token);
                    cReadModeA = 0;
                    break;

                case 23:
                    // Charisma 
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_iCharisma = atoi(token);
                    cReadModeA = 0;
                    break;

                case 24:
                    // Luck 
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_iLuck = atoi(token);
                    cReadModeA = 0;
                    break;

                case 25:
                    // Exp 
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_iExp = atoi(token);
                    cReadModeA = 0;
                    break;

                case 26:
                    // Magic-Mastery
                    for (i = 0; i < DEF_MAXMAGICTYPE; i++)
                    {
                        m_pClientList[iClientH]->m_cMagicMastery[i] = token[i] - 48;
                    }
                    cReadModeA = 0;
                    break;

                case 27:
                    // Skill-Mastery
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed.", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_cSkillMastery[cReadModeB] = atoi(token);
                    cReadModeB++;

                    if (cReadModeB >= DEF_MAXSKILLTYPE)
                    {
                        cReadModeA = 0;
                        cReadModeB = 0;
                    }
                    break;
                    //
                case 28:
                    // Warehouse
                    switch (cReadModeB)
                    {
                        case 1:
                            if (iItemInBankIndex >= DEF_MAXBANKITEMS)
                            {
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            // token
                            if (_bInitItemAttr(m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex], token) == false)
                            {
                                log->error("(!!!) Client({})-Bank Item({}) is not existing Item! Connection closed.", m_pClientList[iClientH]->m_cCharName, token);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            cReadModeB = 2;
                            break;

                        case 2:
                            // m_dwCount
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            iTemp = atoi(token);
                            if (iTemp < 0) iTemp = 1;

                            if (iGetItemWeight(m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex], iTemp) > _iCalcMaxLoad(iClientH))
                            {
                                iTemp = 1;
                                log->error("(!) Player({}) Item({}) Puts character over weight!", m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_cName);
                            }

                            m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_dwCount = (uint32_t)iTemp;
                            cReadModeB = 3;

                            if (memcmp(m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_cName, "Gold", 4) == 0)
                                iTotalGold += iTemp;
                            break;

                        case 3:
                            // m_sTouchEffectType
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_sTouchEffectType = atoi(token);
                            cReadModeB = 4;
                            break;

                        case 4:
                            // m_sTouchEffectValue1
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_sTouchEffectValue1 = atoi(token);
                            cReadModeB = 5;
                            break;

                        case 5:
                            // m_sTouchEffectValue2
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_sTouchEffectValue2 = atoi(token);
                            cReadModeB = 6;
                            break;

                        case 6:
                            // m_sTouchEffectValue3
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_sTouchEffectValue3 = atoi(token);
                            cReadModeB = 7;
                            break;

                        case 7:
                            // m_cItemColor
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_cItemColor = atoi(token);
                            cReadModeB = 8;
                            break;

                        case 8:
                            // m_sItemSpecEffectValue1
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_sItemSpecEffectValue1 = atoi(token);
                            cReadModeB = 9;
                            break;

                        case 9:
                            // m_sItemSpecEffectValue2
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_sItemSpecEffectValue2 = atoi(token);
                            cReadModeB = 10;
                            break;

                        case 10:
                            // m_sItemSpecEffectValue3
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_sItemSpecEffectValue3 = atoi(token);
                            cReadModeB = 11;
                            break;

                        case 11:
                            // m_wCurLifeSpan
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_wCurLifeSpan = atoi(token);

                            cReadModeB = 12;
                            break;


                        case 12:
                            // m_dwAttribute
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_dwAttribute = atoi(token);
                            cReadModeA = 0;
                            cReadModeB = 0;

                            if ((m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_dwAttribute & 0x00000001) != NULL)
                            {
                                m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_wMaxLifeSpan = m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_sItemSpecEffectValue1;
                            }

                            int iValue = (m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_dwAttribute & 0xF0000000) >> 28;
                            if (iValue > 0)
                            {
                                switch (m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_cCategory)
                                {
                                    case 5:
                                    case 6:
                                        m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_wMaxLifeSpan = m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_sItemSpecEffectValue1;
                                        break;
                                }
                            }

                            _AdjustRareItemValue(m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]);

                            if (m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_wCurLifeSpan > m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_wMaxLifeSpan)
                                m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_wCurLifeSpan = m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_wMaxLifeSpan;


                            if ((m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_wCurLifeSpan == 0) &&
                                (m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ALTERITEMDROP))
                            {
                                log->error("(!) Player({}) Durability of item is 0 for 'alter item drop' (26) item!", m_pClientList[iClientH]->m_cCharName);
                                m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_wCurLifeSpan = 1;
                            }

                            if (m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_cItemType == DEF_ITEMTYPE_NOTUSED)
                            {
                                iNotUsedItemPrice += m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_wPrice;
                                delete m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex];
                                m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex] = NULL;
                            }
                            else
                                if (_bCheckDupItemID(m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]) == true)
                                {
                                    _bItemLog(DEF_ITEMLOG_DUPITEMID, iClientH, nullptr, m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]);

                                    iNotUsedItemPrice += m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex]->m_wPrice;
                                    delete m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex];
                                    m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex] = NULL;
                                }
                                else iItemInBankIndex++;
                            break;
                    }
                    break;

                case 29:
                    memset(m_pClientList[iClientH]->m_cLocation, 0, sizeof(m_pClientList[iClientH]->m_cLocation));
                    strcpy(m_pClientList[iClientH]->m_cLocation, token);
                    if (memcmp(m_pClientList[iClientH]->m_cLocation + 3, "hunter", 6) == 0)
                        m_pClientList[iClientH]->m_bIsPlayerCivil = true;
                    cReadModeA = 0;
                    break;

                    /* 2.03 Code - Fixed by KLKS
                                case 29:
                                    memset(m_pClientList[iClientH]->m_cLocation, 0, sizeof(m_pClientList[iClientH]->m_cLocation));
                                    strcpy(m_pClientList[iClientH]->m_cLocation, token);
                                    cReadModeA = 0;
                                    break;
                    */
                case 30:
                    // m_iMP
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_iMP = atoi(token);
                    cReadModeA = 0;
                    break;

                case 31:
                    // m_iSP
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_iSP = atoi(token);
                    cReadModeA = 0;
                    break;

                case 32:
                    // m_cLU_Pool
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_iLU_Pool = atoi(token);
                    cReadModeA = 0;
                    break;
                    /*
                    case 33:
                        // m_cLU_Vit
                        if (_bGetIsStringIsNumber(token) == false) {
                            log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                            delete pContents;
                            delete pStrTok;
                            return false;
                        }
                        m_pClientList[iClientH]->m_cLU_Vit = atoi(token);
                        cReadModeA = 0;
                        break;

                    case 34:
                        // m_cLU_Dex
                        if (_bGetIsStringIsNumber(token) == false) {
                            log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                            delete pContents;
                            delete pStrTok;
                            return false;
                        }
                        m_pClientList[iClientH]->m_cLU_Dex = atoi(token);
                        cReadModeA = 0;
                        break;

                    case 35:
                        // m_cLU_Int
                        if (_bGetIsStringIsNumber(token) == false) {
                            log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                            delete pContents;
                            delete pStrTok;
                            return false;
                        }
                        m_pClientList[iClientH]->m_cLU_Int = atoi(token);
                        cReadModeA = 0;
                        break;

                    case 36:
                        // m_cLU_Mag
                        if (_bGetIsStringIsNumber(token) == false) {
                            log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                            delete pContents;
                            delete pStrTok;
                            return false;
                        }
                        m_pClientList[iClientH]->m_cLU_Mag = atoi(token);
                        cReadModeA = 0;
                        break;

                    case 37:
                        // m_cLU_Char
                        if (_bGetIsStringIsNumber(token) == false) {
                            log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                            delete pContents;
                            delete pStrTok;
                            return false;
                        }
                        m_pClientList[iClientH]->m_cLU_Char = atoi(token);
                        cReadModeA = 0;
                        break;
        */
                case 38:
                    // m_iEnemyKillCount
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_iEnemyKillCount = atoi(token);
                    cReadModeA = 0;
                    break;

                case 39:
                    // m_iPKCount
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_iPKCount = atoi(token);
                    cReadModeA = 0;
                    break;

                case 40:
                    // m_iRewardGold
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_iRewardGold = atoi(token);
                    cReadModeA = 0;
                    break;

                case 41:
                    // Skill-SSN
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_iSkillSSN[cReadModeB] = atoi(token);
                    cReadModeB++;

                    if (cReadModeB >= DEF_MAXSKILLTYPE)
                    {
                        cReadModeA = 0;
                        cReadModeB = 0;
                    }
                    break;

                case 42:
                    if (token != NULL)
                    {
                        memset(m_pClientList[iClientH]->m_cProfile, 0, sizeof(m_pClientList[iClientH]->m_cProfile));
                        strcpy(m_pClientList[iClientH]->m_cProfile, token);
                    }
                    cReadModeA = 0;
                    break;

                case 43:
                    // Hunger-Status
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_iHungerStatus = atoi(token);
                    cReadModeA = 0;
                    break;

                case 44:
                    // AdminUserLevel
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_iAdminUserLevel = 0; //Default it to 0
                    if (atoi(token) > 0)
                    {
                        for (i = 0; i < DEF_MAXADMINS; i++)
                        {
                            if (strlen(m_stAdminList[i].m_cGMName) == 0) break; //No more GM's on list
                            if ((strlen(m_stAdminList[i].m_cGMName)) == (strlen(m_pClientList[iClientH]->m_cCharName)))
                            {
                                if (memcmp(m_stAdminList[i].m_cGMName, m_pClientList[iClientH]->m_cCharName, strlen(m_pClientList[iClientH]->m_cCharName)) == 0)
                                {
                                    m_pClientList[iClientH]->m_iAdminUserLevel = atoi(token);
                                    break; //Break goes to cReadModeA = 0;, so no need to do it again
                                }
                            }
                        }
                    }
                    cReadModeA = 0;
                    break;

                    /*case 44:
                    // AdminUserLevel
                    if (_bGetIsStringIsNumber(token) == false) {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    for (i = 0; i < DEF_MAXADMINS; i++) {
                        if (atoi(token) > 0) {
                            if ((strlen(m_stAdminList[i].m_cGMName)) == (strlen(m_pClientList[iClientH]->m_cCharName))) {
                                m_pClientList[iClientH]->m_iAdminUserLevel = atoi(token);
                                cReadModeA = 0;
                                break;
                            }
                            else {
                                m_pClientList[iClientH]->m_iAdminUserLevel = 0;
                            }
                        else m_pClientList[iClientH]->m_iAdminUserLevel = 0;
                        }
                    }
                    cReadModeA = 0;
                    break;*/

                case 45:
                    // TimeLeft_ShutUp
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_iTimeLeft_ShutUp = atoi(token);
                    cReadModeA = 0;
                    break;

                case 46:
                    // TimeLeft_Rating
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_iTimeLeft_Rating = atoi(token);
                    cReadModeA = 0;
                    break;

                case 47:
                    // Rating
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_iRating = atoi(token);
                    cReadModeA = 0;
                    break;

                case 48:
                    // Guild GUID
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_iGuildGUID = atoi(token);
                    cReadModeA = 0;
                    break;

                case 49:
                    // Down Skill Index
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    m_pClientList[iClientH]->m_iDownSkillIndex = atoi(token);
                    cReadModeA = 0;
                    break;

                case 50:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pClientList[iClientH]->m_ItemPosList[cReadModeB - 1].x = atoi(token);
                    cReadModeB++;
                    if (cReadModeB > 50)
                    {
                        cReadModeA = 0;
                        cReadModeB = 0;
                    }
                    break;

                case 51:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pClientList[iClientH]->m_ItemPosList[cReadModeB - 1].y = atoi(token);
                    if (m_pClientList[iClientH]->m_ItemPosList[cReadModeB - 1].y < -10) m_pClientList[iClientH]->m_ItemPosList[cReadModeB - 1].y = -10;
                    cReadModeB++;
                    if (cReadModeB > 50)
                    {
                        cReadModeA = 0;
                        cReadModeB = 0;
                    }
                    break;

                case 52:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pClientList[iClientH]->m_sCharIDnum1 = atoi(token);
                    cReadModeA = 0;
                    break;

                case 53:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pClientList[iClientH]->m_sCharIDnum2 = atoi(token);
                    cReadModeA = 0;
                    break;

                case 54:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pClientList[iClientH]->m_sCharIDnum3 = atoi(token);
                    cReadModeA = 0;
                    break;

                case 55:
                    switch (cReadModeB)
                    {
                        case 1:
                            // Penalty Block Year
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_iPenaltyBlockYear = atoi(token);

                            cReadModeB = 2;
                            break;

                        case 2:
                            // Penalty Block Month
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_iPenaltyBlockMonth = atoi(token);

                            cReadModeB = 3;
                            break;

                        case 3:
                            // Penalty Block day
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_iPenaltyBlockDay = atoi(token);

                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                case 56:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pClientList[iClientH]->m_iQuest = atoi(token);
                    cReadModeA = 0;
                    break;

                case 57:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pClientList[iClientH]->m_iCurQuestCount = atoi(token);
                    cReadModeA = 0;
                    break;

                case 58:
                    cReadModeA = 0;
                    break;

                case 59:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pClientList[iClientH]->m_iQuestRewardType = atoi(token);
                    cReadModeA = 0;
                    break;

                case 60:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pClientList[iClientH]->m_iQuestRewardAmount = atoi(token);
                    cReadModeA = 0;
                    break;

                case 61:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pClientList[iClientH]->m_iContribution = atoi(token);
                    cReadModeA = 0;
                    break;

                case 62:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pClientList[iClientH]->m_iQuestID = atoi(token);
                    cReadModeA = 0;
                    break;

                case 63:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pClientList[iClientH]->m_bIsQuestCompleted = (bool)atoi(token);
                    cReadModeA = 0;
                    break;

                case 64:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = (bool)atoi(token);
                    cReadModeA = 0;
                    break;

                case 65:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pClientList[iClientH]->m_iTimeLeft_FirmStaminar = (bool)atoi(token);
                    cReadModeA = 0;
                    break;

                case 66:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pClientList[iClientH]->m_iSpecialEventID = (bool)atoi(token);
                    cReadModeA = 0;
                    break;

                case 67:
                    switch (cReadModeB)
                    {
                        case 1:
                            // FightZone Number
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_iFightzoneNumber = atoi(token);

                            cReadModeB = 2;
                            break;

                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_iReserveTime = atoi(token);


                            cReadModeB = 3;
                            break;
                        case 3:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pClientList[iClientH]->m_iFightZoneTicketNumber = atoi(token);

                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;

                    }
                    break;

                case 70:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pClientList[iClientH]->m_iSuperAttackLeft = (bool)atoi(token);
                    cReadModeA = 0;
                    break;

                case 71:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pClientList[iClientH]->m_iSpecialAbilityTime = atoi(token);
                    cReadModeA = 0;
                    break;

                case 72:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pClientList[iClientH]->m_iWarContribution = atoi(token);
                    cReadModeA = 0;
                    break;

                case 73:
                    if (strlen(token) > 10)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }
                    memset(m_pClientList[iClientH]->m_cLockedMapName, 0, sizeof(m_pClientList[iClientH]->m_cLockedMapName));
                    strcpy(m_pClientList[iClientH]->m_cLockedMapName, token);
                    cReadModeA = 0;
                    break;

                case 74:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pClientList[iClientH]->m_iLockedMapTime = atoi(token);
                    cReadModeA = 0;
                    break;

                case 75:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pClientList[iClientH]->m_iCrusadeDuty = atoi(token);
                    cReadModeA = 0;
                    break;

                case 76:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pClientList[iClientH]->m_iConstructionPoint = atoi(token);
                    cReadModeA = 0;
                    break;

                case 77:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pClientList[iClientH]->m_dwCrusadeGUID = atoi(token);
                    cReadModeA = 0;
                    break;

                case 78:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pClientList[iClientH]->m_iDeadPenaltyTime = atoi(token);
                    cReadModeA = 0;
                    break;

                case 79:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pClientList[iClientH]->m_iPartyID = atoi(token);
                    if (m_pClientList[iClientH]->m_iPartyID != NULL) m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_CONFIRM;
                    cReadModeA = 0;
                    break;

                case 80:
                    if (_bGetIsStringIsNumber(token) == false)
                    {
                        log->error("(!!!) Player({}) data file error! wrong Data format - Connection closed. ", m_pClientList[iClientH]->m_cCharName);
                        delete pContents;
                        delete pStrTok;
                        return false;
                    }

                    m_pClientList[iClientH]->m_iGizonItemUpgradeLeft = atoi(token);
                    cReadModeA = 0;
                    break;
            }
        }
        else
        {
            if (memcmp(token, "character-loc-map", 17) == 0)		 cReadModeA = 1;
            if (memcmp(token, "character-loc-x", 15) == 0)			 cReadModeA = 2;
            if (memcmp(token, "character-loc-y", 15) == 0)			 cReadModeA = 3;
            if (memcmp(token, "character-account-status", 21) == 0)  cReadModeA = 4;
            if (memcmp(token, "character-item", 14) == 0)
            {
                cReadModeA = 5;
                cReadModeB = 1;
                m_pClientList[iClientH]->m_pItemList[iItemIndex] = new class CItem;
            }

            if (memcmp(token, "character-bank-item", 18) == 0)
            {
                cReadModeA = 28;
                cReadModeB = 1;
                m_pClientList[iClientH]->m_pItemInBankList[iItemInBankIndex] = new class CItem;
            }

            if (memcmp(token, "sex-status", 10) == 0)        cReadModeA = 6;
            if (memcmp(token, "skin-status", 11) == 0)       cReadModeA = 7;
            if (memcmp(token, "hairstyle-status", 16) == 0)  cReadModeA = 8;
            if (memcmp(token, "haircolor-status", 16) == 0)  cReadModeA = 9;
            if (memcmp(token, "underwear-status", 16) == 0)  cReadModeA = 10;

            if (memcmp(token, "item-equip-status", 17) == 0)    cReadModeA = 11;
            if (memcmp(token, "character-guild-name", 20) == 0) cReadModeA = 12;
            if (memcmp(token, "character-guild-rank", 20) == 0) cReadModeA = 13;
            if (memcmp(token, "character-HP", 12) == 0)         cReadModeA = 14;
            if (memcmp(token, "character-DefenseRatio", 22) == 0)  cReadModeA = 15;
            if (memcmp(token, "character-HitRatio", 18) == 0)   cReadModeA = 16;
            if (memcmp(token, "character-LEVEL", 15) == 0)      cReadModeA = 17;
            if (memcmp(token, "character-STR", 13) == 0)        cReadModeA = 18;
            if (memcmp(token, "character-INT", 13) == 0)        cReadModeA = 19;
            if (memcmp(token, "character-VIT", 13) == 0)        cReadModeA = 20;
            if (memcmp(token, "character-DEX", 13) == 0)        cReadModeA = 21;
            if (memcmp(token, "character-MAG", 13) == 0)        cReadModeA = 22;
            if (memcmp(token, "character-CHARISMA", 18) == 0)   cReadModeA = 23;
            if (memcmp(token, "character-LUCK", 14) == 0)       cReadModeA = 24;
            if (memcmp(token, "character-EXP", 13) == 0)        cReadModeA = 25;
            if (memcmp(token, "magic-mastery", 13) == 0)        cReadModeA = 26;

            if (memcmp(token, "skill-mastery", 13) == 0)
            {
                cReadModeA = 27;
                cReadModeB = 0;
            }

            if (memcmp(token, "character-location", 18) == 0)   cReadModeA = 29;

            if (memcmp(token, "character-MP", 12) == 0)         cReadModeA = 30;
            if (memcmp(token, "character-SP", 12) == 0)         cReadModeA = 31;

            if (memcmp(token, "character-LU_Pool", 17) == 0)     cReadModeA = 32;

            /*
            if (memcmp(token, "character-LU_Str", 16) == 0)     cReadModeA = 32;
            if (memcmp(token, "character-LU_Vit", 16) == 0)     cReadModeA = 33;
            if (memcmp(token, "character-LU_Dex", 16) == 0)     cReadModeA = 34;
            if (memcmp(token, "character-LU_Int", 16) == 0)     cReadModeA = 35;
            if (memcmp(token, "character-LU_Mag", 16) == 0)     cReadModeA = 36;
            if (memcmp(token, "character-LU_Char",17) == 0)     cReadModeA = 37;
            */

            if (memcmp(token, "character-EK-Count", 18) == 0)    cReadModeA = 38;
            if (memcmp(token, "character-PK-Count", 18) == 0)    cReadModeA = 39;

            if (memcmp(token, "character-reward-gold", 21) == 0) cReadModeA = 40;
            if (memcmp(token, "skill-SSN", 9) == 0) 			cReadModeA = 41;
            if (memcmp(token, "character-profile", 17) == 0)	cReadModeA = 42;

            if (memcmp(token, "hunger-status", 13) == 0) 		cReadModeA = 43;
            if (memcmp(token, "admin-user-level", 16) == 0) 	cReadModeA = 44;
            if (memcmp(token, "timeleft-shutup", 15) == 0) 		cReadModeA = 45;
            if (memcmp(token, "timeleft-rating", 15) == 0) 		cReadModeA = 46;
            if (memcmp(token, "character-RATING", 16) == 0)	    cReadModeA = 47;

            if (memcmp(token, "character-guild-GUID", 20) == 0) cReadModeA = 48;

            if (memcmp(token, "character-downskillindex", 24) == 0) cReadModeA = 49;
            if (memcmp(token, "item-position-x", 16) == 0)
            {
                cReadModeA = 50;
                cReadModeB = 1;
            }
            if (memcmp(token, "item-position-y", 16) == 0)
            {
                cReadModeA = 51;
                cReadModeB = 1;
            }
            if (memcmp(token, "character-IDnum1", 16) == 0)		cReadModeA = 52;
            if (memcmp(token, "character-IDnum2", 16) == 0)		cReadModeA = 53;
            if (memcmp(token, "character-IDnum3", 16) == 0)		cReadModeA = 54;
            if (memcmp(token, "penalty-block-date", 18) == 0)
            {
                cReadModeA = 55;
                cReadModeB = 1;
            }

            if (memcmp(token, "character-quest-number", 22) == 0) cReadModeA = 56;
            if (memcmp(token, "current-quest-count", 19) == 0)	 cReadModeA = 57;

            if (memcmp(token, "quest-reward-type", 17) == 0)    cReadModeA = 59;
            if (memcmp(token, "quest-reward-amount", 19) == 0)  cReadModeA = 60;

            if (memcmp(token, "character-contribution", 22) == 0)   cReadModeA = 61;
            if (memcmp(token, "character-quest-ID", 18) == 0)        cReadModeA = 62;
            if (memcmp(token, "character-quest-completed", 25) == 0) cReadModeA = 63;

            if (memcmp(token, "timeleft-force-recall", 21) == 0)	cReadModeA = 64;
            if (memcmp(token, "timeleft-firm-staminar", 22) == 0)	cReadModeA = 65;
            if (memcmp(token, "special-event-id", 16) == 0)			cReadModeA = 66;
            if (memcmp(token, "super-attack-left", 17) == 0)		cReadModeA = 70;

            if (memcmp(token, "reserved-fightzone-id", 21) == 0)
            {
                cReadModeA = 67;
                cReadModeB = 1;
            }

            if (memcmp(token, "special-ability-time", 20) == 0)       cReadModeA = 71;
            if (memcmp(token, "character-war-contribution", 26) == 0) cReadModeA = 72;

            if (memcmp(token, "locked-map-name", 15) == 0) cReadModeA = 73;
            if (memcmp(token, "locked-map-time", 15) == 0) cReadModeA = 74;
            if (memcmp(token, "crusade-job", 11) == 0)     cReadModeA = 75;
            if (memcmp(token, "construct-point", 15) == 0) cReadModeA = 76;
            if (memcmp(token, "crusade-GUID", 12) == 0)    cReadModeA = 77;

            if (memcmp(token, "dead-penalty-time", 17) == 0) cReadModeA = 78;
            if (memcmp(token, "party-id", 8) == 0)           cReadModeA = 79;
            if (memcmp(token, "gizon-item-upgade-left", 22) == 0) cReadModeA = 80;

            if (memcmp(token, "[EOF]", 5) == 0) goto DPDC_STOP_DECODING;
        }

        token = pStrTok->pGet();							    
    }

    DPDC_STOP_DECODING:;

    delete pStrTok;
    delete pContents;
    if ((cReadModeA != 0) || (cReadModeB != 0))
    {
        log->error("(!!!) Player({}) data file contents error({} {})! Connection closed.", m_pClientList[iClientH]->m_cCharName, cReadModeA, cReadModeB);

        return false;
    }

    bRet = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bIsValidLoc(m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY);
    if (bRet == false)
    {
        if ((m_pClientList[iClientH]->m_sX != -1) || (m_pClientList[iClientH]->m_sY != -1))
        {
            log->error("Invalid location error! {} ({} {})", m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY);
            return false;
        }
    }

    /*	if ((m_pClientList[iClientH]->m_cLU_Str > DEF_TOTALLEVELUPPOINT) || (m_pClientList[iClientH]->m_cLU_Str < 0))
    return false;

    if ((m_pClientList[iClientH]->m_cLU_Vit > DEF_TOTALLEVELUPPOINT) || (m_pClientList[iClientH]->m_cLU_Vit < 0))
    return false;

    if ((m_pClientList[iClientH]->m_cLU_Dex > DEF_TOTALLEVELUPPOINT) || (m_pClientList[iClientH]->m_cLU_Dex < 0))
    return false;

    if ((m_pClientList[iClientH]->m_cLU_Int > DEF_TOTALLEVELUPPOINT) || (m_pClientList[iClientH]->m_cLU_Int < 0))
    return false;

    if ((m_pClientList[iClientH]->m_cLU_Mag > DEF_TOTALLEVELUPPOINT) || (m_pClientList[iClientH]->m_cLU_Mag < 0))
    return false;

    if ((m_pClientList[iClientH]->m_cLU_Char > DEF_TOTALLEVELUPPOINT) || (m_pClientList[iClientH]->m_cLU_Char < 0))
    return false;
    */
    //	if ( (m_pClientList[iClientH]->m_cLU_Str + m_pClientList[iClientH]->m_cLU_Vit + m_pClientList[iClientH]->m_cLU_Dex + 
    //  		  m_pClientList[iClientH]->m_cLU_Int + m_pClientList[iClientH]->m_cLU_Mag + m_pClientList[iClientH]->m_cLU_Char) > DEF_TOTALLEVELUPPOINT) 
    if ((m_pClientList[iClientH]->m_iLU_Pool < 0) || (m_pClientList[iClientH]->m_iLU_Pool > DEF_CHARPOINTLIMIT))
        return false;

    if ((m_pClientList[iClientH]->m_iStr < 10) || (m_pClientList[iClientH]->m_iStr > DEF_CHARPOINTLIMIT))
        return false;

    if ((m_pClientList[iClientH]->m_iDex < 10) || (m_pClientList[iClientH]->m_iDex > DEF_CHARPOINTLIMIT))
        return false;

    if ((m_pClientList[iClientH]->m_iVit < 10) || (m_pClientList[iClientH]->m_iVit > DEF_CHARPOINTLIMIT))
        return false;

    if ((m_pClientList[iClientH]->m_iInt < 10) || (m_pClientList[iClientH]->m_iInt > DEF_CHARPOINTLIMIT))
        return false;

    if ((m_pClientList[iClientH]->m_iMag < 10) || (m_pClientList[iClientH]->m_iMag > DEF_CHARPOINTLIMIT))
        return false;

    if ((m_pClientList[iClientH]->m_iCharisma < 10) || (m_pClientList[iClientH]->m_iCharisma > DEF_CHARPOINTLIMIT))
        return false;

    //	if ((m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iDex + m_pClientList[iClientH]->m_iVit + 
    //		 m_pClientList[iClientH]->m_iInt + m_pClientList[iClientH]->m_iMag + m_pClientList[iClientH]->m_iCharisma) 
    //		 !=	((m_pClientList[iClientH]->m_iLevel-1)*3 + 70)) return false;


    //if ((m_pClientList[iClientH]->m_cAccountStatus != 2) && (m_pClientList[iClientH]->m_iLevel > DEF_LEVELLIMIT)) 
    //	return false;

    if ((m_Misc.bCheckValidName(m_pClientList[iClientH]->m_cCharName) == false) || (m_Misc.bCheckValidName(m_pClientList[iClientH]->m_cAccountName) == false))
        return false;

    if (m_pClientList[iClientH]->m_iPenaltyBlockYear != 0)
    {
//         GetLocalTime(&SysTime);
//         iDateSum1 = (int64_t)(m_pClientList[iClientH]->m_iPenaltyBlockYear * 10000 + m_pClientList[iClientH]->m_iPenaltyBlockMonth * 100 + m_pClientList[iClientH]->m_iPenaltyBlockDay);
//         iDateSum2 = (int64_t)(SysTime.wYear * 10000 + SysTime.wMonth * 100 + SysTime.wDay);
//         if (iDateSum1 >= iDateSum2) return false;
    }

    if (m_pClientList[iClientH]->m_iReserveTime != 0)
    {
//         GetLocalTime(&SysTime);
//         iDateSum1 = (int64_t)m_pClientList[iClientH]->m_iReserveTime;
//         iDateSum2 = (int64_t)(SysTime.wMonth * 10000 + SysTime.wDay * 100 + SysTime.wHour);
//         if (iDateSum2 >= iDateSum1)
//         {
//             SendNotifyMsg(NULL, i, DEF_NOTIFY_FIGHTZONERESERVE, -2, NULL, NULL, NULL);
//             m_pClientList[iClientH]->m_iFightzoneNumber = 0;
//             m_pClientList[iClientH]->m_iReserveTime = 0;
//             m_pClientList[iClientH]->m_iFightZoneTicketNumber = 0;
//         }
    }

    if (m_pClientList[iClientH]->m_iAdminUserLevel < 0) m_pClientList[iClientH]->m_iAdminUserLevel = 0;

    // ============================================================================================================


    if (m_pClientList[iClientH]->m_cSex == 1)
    {
        sTmpType = 1;
    }
    else if (m_pClientList[iClientH]->m_cSex == 2)
    {
        sTmpType = 4;
    }

    switch (m_pClientList[iClientH]->m_cSkin)
    {
        case 1:
            break;
        case 2:
            sTmpType += 1;
            break;
        case 3:
            sTmpType += 2;
            break;
    }

    if (m_pClientList[iClientH]->m_iAdminUserLevel >= 10)
        sTmpType = m_pClientList[iClientH]->m_iAdminUserLevel;

    sTmpAppr1 = (m_pClientList[iClientH]->m_cHairStyle << 8) | (m_pClientList[iClientH]->m_cHairColor << 4) | (m_pClientList[iClientH]->m_cUnderwear);

    m_pClientList[iClientH]->m_sType = sTmpType;
    m_pClientList[iClientH]->m_sAppr1 = sTmpAppr1;

    iCalcTotalWeight(iClientH);

    if (m_pClientList[iClientH]->m_sCharIDnum1 == 0)
    {
        int _i, _iTemp1, _iTemp2;
        short _sID1, _sID2, _sID3;

        _iTemp1 = 1;
        _iTemp2 = 1;
        for (_i = 0; _i < 10; _i++)
        {
            _iTemp1 += m_pClientList[iClientH]->m_cCharName[_i];
            _iTemp2 += abs(m_pClientList[iClientH]->m_cCharName[_i] ^ m_pClientList[iClientH]->m_cCharName[_i]);
        }

        _sID1 = (short)timeGetTime();
        _sID2 = (short)_iTemp1;
        _sID3 = (short)_iTemp2;

        m_pClientList[iClientH]->m_sCharIDnum1 = _sID1;
        m_pClientList[iClientH]->m_sCharIDnum2 = _sID2;
        m_pClientList[iClientH]->m_sCharIDnum3 = _sID3;
    }

    m_pClientList[iClientH]->m_iRewardGold += iNotUsedItemPrice;

    m_pClientList[iClientH]->m_iSpeedHackCheckExp = m_pClientList[iClientH]->m_iExp;

    if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) m_pClientList[iClientH]->m_bIsNeutral = true;

    return true;
}

bool CGame::_bDecodePortionConfigFileContents(char * pData, uint32_t dwMsgSize)
{
    char * pContents, * token;
    char seps[] = "= \t\n";
    char cReadModeA = 0;
    char cReadModeB = 0;
    int  iPortionConfigListIndex = 0;
    class CStrTok * pStrTok;

    pContents = new char[dwMsgSize + 1];
    memset(pContents, 0, dwMsgSize + 1);
    memcpy(pContents, pData, dwMsgSize);

    pStrTok = new class CStrTok(pContents, seps);
    token = pStrTok->pGet();
    while (token != NULL)
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
                                log->critical("(!!!) CRITICAL ERROR! POTION configuration file error - Wrong Data format(1).");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            if (m_pPortionConfigList[atoi(token)] != NULL)
                            {
                                log->critical("(!!!) CRITICAL ERROR! POTION configuration file error - Duplicate portion number.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pPortionConfigList[atoi(token)] = new class CPortion;
                            iPortionConfigListIndex = atoi(token);

                            cReadModeB = 2;
                            break;

                        case 2:
                            memset(m_pPortionConfigList[iPortionConfigListIndex]->m_cName, 0, sizeof(m_pPortionConfigList[iPortionConfigListIndex]->m_cName));
                            memcpy(m_pPortionConfigList[iPortionConfigListIndex]->m_cName, token, strlen(token));
                            cReadModeB = 3;
                            break;

                        default:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pPortionConfigList[iPortionConfigListIndex]->m_sArray[cReadModeB - 3] = atoi(token);
                            cReadModeB++;
                            break;

                        case 14:
                            // ¸¶Áö¸· m_sArray[11]
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pPortionConfigList[iPortionConfigListIndex]->m_sArray[11] = atoi(token);
                            cReadModeB = 15;
                            break;

                        case 15:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pPortionConfigList[iPortionConfigListIndex]->m_iSkillLimit = atoi(token);
                            cReadModeB = 16;
                            break;

                        case 16:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pPortionConfigList[iPortionConfigListIndex]->m_iDifficulty = atoi(token);
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
            if (memcmp(token, "potion", 5) == 0)
            {
                cReadModeA = 1;
                cReadModeB = 1;
            }

        }
        token = pStrTok->pGet();
    }

    delete pStrTok;
    delete pContents;

    if ((cReadModeA != 0) || (cReadModeB != 0))
    {
        log->critical("(!!!) CRITICAL ERROR! POTION configuration file contents error!");
        return false;
    }

    log->info("(!) POTION(Total:%d) configuration - success!", iPortionConfigListIndex);

    return true;
}

bool CGame::_bDecodeQuestConfigFileContents(char * pData, uint32_t dwMsgSize)
{
    char * pContents, * token;
    char seps[] = "= \t\n";
    char cReadModeA = 0;
    char cReadModeB = 0;
    int  iQuestConfigListIndex = 0;
    class CStrTok * pStrTok;

    pContents = new char[dwMsgSize + 1];
    memset(pContents, 0, dwMsgSize + 1);
    memcpy(pContents, pData, dwMsgSize);

    pStrTok = new class CStrTok(pContents, seps);
    token = pStrTok->pGet();

    while (token != NULL)
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
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }

                            if (m_pQuestConfigList[atoi(token)] != NULL)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Duplicate quest number.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[atoi(token)] = new class CQuest;
                            iQuestConfigListIndex = atoi(token);

                            cReadModeB = 2;
                            break;

                        case 2:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[iQuestConfigListIndex]->m_cSide = atoi(token);
                            cReadModeB = 3;
                            break;

                        case 3:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[iQuestConfigListIndex]->m_iType = atoi(token);
                            cReadModeB = 4;
                            break;

                        case 4:
                            // TargetType
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[iQuestConfigListIndex]->m_iTargetType = atoi(token);
                            cReadModeB = 5;
                            break;

                        case 5:
                            // MaxCount
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[iQuestConfigListIndex]->m_iMaxCount = atoi(token);
                            cReadModeB = 6;
                            break;

                        case 6:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[iQuestConfigListIndex]->m_iFrom = atoi(token);
                            cReadModeB = 7;
                            break;

                        case 7:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[iQuestConfigListIndex]->m_iMinLevel = atoi(token);
                            cReadModeB = 8;
                            break;

                        case 8:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[iQuestConfigListIndex]->m_iMaxLevel = atoi(token);
                            cReadModeB = 9;
                            break;

                        case 9:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[iQuestConfigListIndex]->m_iRequiredSkillNum = atoi(token);
                            cReadModeB = 10;
                            break;

                        case 10:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[iQuestConfigListIndex]->m_iRequiredSkillLevel = atoi(token);
                            cReadModeB = 11;
                            break;

                        case 11:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[iQuestConfigListIndex]->m_iTimeLimit = atoi(token);
                            cReadModeB = 12;
                            break;

                        case 12:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[iQuestConfigListIndex]->m_iAssignType = atoi(token);
                            cReadModeB = 13;
                            break;

                        case 13:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[iQuestConfigListIndex]->m_iRewardType[1] = atoi(token);
                            cReadModeB = 14;
                            break;

                        case 14:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[iQuestConfigListIndex]->m_iRewardAmount[1] = atoi(token);
                            cReadModeB = 15;
                            break;

                        case 15:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[iQuestConfigListIndex]->m_iRewardType[2] = atoi(token);
                            cReadModeB = 16;
                            break;

                        case 16:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[iQuestConfigListIndex]->m_iRewardAmount[2] = atoi(token);
                            cReadModeB = 17;
                            break;

                        case 17:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[iQuestConfigListIndex]->m_iRewardType[3] = atoi(token);
                            cReadModeB = 18;
                            break;

                        case 18:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[iQuestConfigListIndex]->m_iRewardAmount[3] = atoi(token);
                            cReadModeB = 19;
                            break;

                        case 19:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[iQuestConfigListIndex]->m_iContribution = atoi(token);
                            cReadModeB = 20;
                            break;

                        case 20:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[iQuestConfigListIndex]->m_iContributionLimit = atoi(token);
                            cReadModeB = 21;
                            break;

                        case 21:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[iQuestConfigListIndex]->m_iResponseMode = atoi(token);
                            cReadModeB = 22;
                            break;

                        case 22:
                            memset(m_pQuestConfigList[iQuestConfigListIndex]->m_cTargetName, 0, sizeof(m_pQuestConfigList[iQuestConfigListIndex]->m_cTargetName));
                            strcpy(m_pQuestConfigList[iQuestConfigListIndex]->m_cTargetName, token);
                            cReadModeB = 23;
                            break;

                        case 23:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[iQuestConfigListIndex]->m_sX = atoi(token);
                            cReadModeB = 24;
                            break;

                        case 24:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[iQuestConfigListIndex]->m_sY = atoi(token);
                            cReadModeB = 25;
                            break;

                        case 25:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[iQuestConfigListIndex]->m_iRange = atoi(token);
                            cReadModeB = 26;
                            break;

                        case 26:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[iQuestConfigListIndex]->m_iQuestID = atoi(token);
                            cReadModeB = 27;
                            break;

                        case 27:
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                log->critical("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
                                delete pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pQuestConfigList[iQuestConfigListIndex]->m_iReqContribution = atoi(token);
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
            if (memcmp(token, "quest", 5) == 0)
            {
                cReadModeA = 1;
                cReadModeB = 1;
            }
        }
        token = pStrTok->pGet();
    }

    delete pStrTok;
    delete pContents;

    if ((cReadModeA != 0) || (cReadModeB != 0))
    {
        log->critical("(!!!) CRITICAL ERROR! QUEST configuration file contents error!");
        return false;
    }

    log->info("(!) QUEST(Total:%d) configuration - success!", iQuestConfigListIndex);

    return true;
}
