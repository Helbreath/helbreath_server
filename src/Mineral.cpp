//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#include "Mineral.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMineral::CMineral(char cType, char cMapIndex, int sX, int sY, int iRemain)
{
	m_cType = cType;
	m_cMapIndex = cMapIndex;
	m_sX = sX;
	m_sY = sY;;
    m_iRemain = iRemain;
	m_iDifficulty = 0;
}

CMineral::~CMineral()
{

}
