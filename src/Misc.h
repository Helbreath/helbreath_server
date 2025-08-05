//
// Copyright (c) Helbreath Team (helbreath at helbreath dot dev)
//
// Distributed under the Apache 2.0 License. (See accompanying file LICENSE)
//

#pragma once

class CMisc  						    
{
public:
    CMisc() = default;
    ~CMisc() = default;
    void Temp();
	bool bCheckValidName(char *pStr);
	bool bDecode(char cKey, char *pStr);
	bool bEncode(char cKey, char *pStr);
	void GetDirPoint(char cDir, int * pX, int * pY);
	void GetPoint2(int x0, int y0, int x1, int y1, int * pX, int * pY, int * pError, int iCount);
	void GetPoint(int x0, int y0, int x1, int y1, int * pX, int * pY, int * pError);
	char cGetNextMoveDir(short sX, short sY, short dX, short dY);
};
