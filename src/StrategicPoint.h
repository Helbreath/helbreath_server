//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#pragma once

class CStrategicPoint  
{
public:
	CStrategicPoint() = default;
	~CStrategicPoint() = default;

	int	m_iSide{};
	int m_iValue{};
	int m_iX{}, m_iY{};
};
