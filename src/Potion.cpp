//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include "Potion.h"
#include <memory>

CPotion::CPotion()
{
    int i;

    memset(m_cName, 0, sizeof(m_cName));
    m_iSkillLimit = 0;
    m_iDifficulty = 0;

    for (i = 0; i < 12; i++)
        m_sArray[i] = -1;
}
