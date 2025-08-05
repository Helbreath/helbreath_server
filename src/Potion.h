//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#pragma once

class CPotion  
{
public:
	CPotion();
	~CPotion() = default;

	char m_cName[21];
	short m_sArray[12];

	int m_iSkillLimit, m_iDifficulty;
};
