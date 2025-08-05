//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include "Tile.h"
#include "Item.h"

CTile::CTile()
{
    m_bIsMoveAllowed = true;
    m_bIsTeleport = false;
    m_bIsWater = false;
    m_bIsFarm = false;

    m_sOwner = 0;
    m_cOwnerClass = 0;

    m_sDeadOwner = 0;
    m_cDeadOwnerClass = 0;

    for (auto & item : m_pItem)
        item = nullptr;
    m_cTotalItem = 0;

    m_wDynamicObjectID = 0;
    m_sDynamicObjectType = 0;

    m_bIsTempMoveAllowed = true;

    m_iOccupyStatus = 0;
    m_iOccupyFlagIndex = 0;

    m_iAttribute = 0;
}

CTile::~CTile()
{
    for (auto & item : m_pItem)
        if (item != nullptr) delete item;
}
