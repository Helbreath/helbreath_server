//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#pragma once

class CNpcItem  
{
public:
	CNpcItem() = default;
	~CNpcItem() = default;

    char m_cName[21]{};
    short m_sItemID{};
    short m_sFirstProbability{};
	short m_sSecondProbability{};
	char m_cFirstTargetValue{};
	char m_cSecondTargetValue{};
};
