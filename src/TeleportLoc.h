//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#pragma once
#include <cstdint>

class CTeleportLoc  
{
public:
	CTeleportLoc() = default;
	~CTeleportLoc() = default;
												  
    short m_sSrcX{ -1 }, m_sSrcY{ -1 };

    char m_cDestMapName[11]{};
    char m_cDestMapName2[11]{};
    char m_cDir{};
	short m_sDestX{ -1 },  m_sDestY{ -1 };
	short m_sDestX2{ -1 }, m_sDestY2{ -1 };

    int m_iV1{ 0 }, m_iV2{ 0 };
	uint32_t m_dwTime{ 0 }, m_dwTime2{ 0 };
};
