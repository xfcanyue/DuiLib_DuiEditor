#ifndef PPX_BASE_BARCODE_H_
#define PPX_BASE_BARCODE_H_
#pragma once

class Barcode39;
class Barcode93;
class BarcodeIof5;
class Barcode128;

static const int ga2_Code128[2][207] =
{
    {
        64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
        80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
        0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
        16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
        32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
        48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
        -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
        -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
        -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
        -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
        -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
        -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
        -1,  -1,  -1,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106,
    },
    {
        -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
        -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
        0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
        16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
        32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
        48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
        64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
        80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  -1,
        -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
        -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
        -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
        -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
        -1,  -1,  -1,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106,
    },
};


class BarcodeBase {
public:
    BarcodeBase() {
        Clear();
        i_Ratio = 3;
    }

    void operator=(const BarcodeBase&bc) {
        i_LenBuf = bc.i_LenBuf;
        i_Ratio = bc.i_Ratio;
        memcpy(ia_Buf, bc.ia_Buf, sizeof(ia_Buf));
    }

    void Clear() {
        memset(ia_Buf, 0, sizeof(ia_Buf));
        i_LenBuf = 0;
    }

    int GetEncodeLength()	const {
        BYTE*pb = (BYTE*)ia_Buf;
        int i, iLen = 0;
        for (i = 0; i < i_LenBuf; i++) {
            //wide is 3
            if (*pb & 2)	iLen += (i_Ratio - 1);
            pb++;
        }
        return iLen + i_LenBuf;
    }
	BYTE *GetBuffer()
	{
		return ia_Buf;
	}
    int GetBufferLength()	const {
        return i_LenBuf;
    }
    const BYTE&GetAt(int i)	const {
        return ia_Buf[i];
    }
    int GetRatio()	const {
        return i_Ratio;
    }
    int SetRatio(int iRatio) {
        i_Ratio = iRatio;
        if (i_Ratio <= 0)	i_Ratio = 1;
    }

	virtual BOOL Encode(const char* pszCode) { return FALSE; }

    void DrawBarcode(HDC hDC, int iX, int iY0, int iY10, int iY11, const COLORREF clrBar, const COLORREF clrSpace, const int iPenW) {
        HPEN hPenBar = ::CreatePen(PS_SOLID, iPenW, clrBar);
        HPEN hPenSpace = ::CreatePen(PS_SOLID, iPenW, clrSpace);

        HPEN hPenOld = (HPEN)::SelectObject(hDC, hPenBar);

        BYTE*pb = ia_Buf;
        int i0, iNum0 = i_LenBuf;

        BYTE bBar;
        int i1, iNum1;
        int iY;
        for (i0 = 0; i0 < iNum0; i0++) {
            bBar = *pb & 0x01;
            iNum1 = (*pb & 0x02) ? i_Ratio : 1;
            iY = (*pb & 0x04) ? iY11 : iY10;
            for (i1 = 0; i1 < iNum1; i1++) {
                if (bBar)	::SelectObject(hDC, hPenBar);
                else		::SelectObject(hDC, hPenSpace);

                ::MoveToEx(hDC, iX, iY0, 0);
                ::LineTo(hDC, iX, iY);
                iX += iPenW;
            }
            pb++;
        }

        ::SelectObject(hDC, hPenOld);

        ::DeleteObject(hPenBar);
        ::DeleteObject(hPenSpace);
    }

protected:
    BYTE	ia_Buf[4096];
    int		i_LenBuf;

    int		i_Ratio;

    struct IntString {
        int ch;
        char*psz;
    };

};

class Barcode39 :public BarcodeBase {
    //[n/a][n/a][n/a][n/a][n/a][n/a][w-n][b-s]
public:
    Barcode39() {
    }
    ~Barcode39() {
    }

    virtual BOOL Encode(const char*pszCodeIn) {
        int iLen = strlen(pszCodeIn);

        char*pszCode = new char[iLen + 3];
        sprintf(pszCode, "*%s*", pszCodeIn);
        strupr(pszCode);

        BYTE*pFst = ia_Buf;
        BYTE*p0 = pFst, *p1;

        iLen += 2;
        int i;
        for (i = 0; i < iLen; i++) {
            p1 = P_GetNarrowWideBarSpace39(pszCode[i], p0);
            if (p1 == 0)	return 0;
            p0 = p1;
        }
        i_LenBuf = p1 - pFst;
        delete[]pszCode;
        return 1;
    }

    void Draw39(HDC hDC, int iX, int iY0, int iY1, const COLORREF clrBar, const COLORREF clrSpace, const int iPenW) {
        DrawBarcode(hDC, iX, iY0, iY1, iY1, clrBar, clrSpace, iPenW);
    }

private:
    BYTE*P_GetNarrowWideBarSpace39(char ch, BYTE*pb) {
        IntString infs[] =
        {
            {'1',	"wnnwnnnnwn"},
            {'2',	"nnwwnnnnwn"},
            {'3',	"wnwwnnnnnn"},
            {'4',	"nnnwwnnnwn"},
            {'5',	"wnnwwnnnnn"},
            {'6',	"nnwwwnnnnn"},
            {'7',	"nnnwnnwnwn"},
            {'8',	"wnnwnnwnnn"},
            {'9',	"nnwwnnwnnn"},
            {'0',	"nnnwwnwnnn"},
            {'A',	"wnnnnwnnwn"},
            {'B',	"nnwnnwnnwn"},
            {'C',	"wnwnnwnnnn"},
            {'D',	"nnnnwwnnwn"},
            {'E',	"wnnnwwnnnn"},
            {'F',	"nnwnwwnnnn"},
            {'G',	"nnnnnwwnwn"},
            {'H',	"wnnnnwwnnn"},
            {'I',	"nnwnnwwnnn"},
            {'J',	"nnnnwwwnnn"},
            {'K',	"wnnnnnnwwn"},
            {'L',	"nnwnnnnwwn"},
            {'M',	"wnwnnnnwnn"},
            {'N',	"nnnnwnnwwn"},
            {'O',	"wnnnwnnwnn"},
            {'P',	"nnwnwnnwnn"},
            {'Q',	"nnnnnnwwwn"},
            {'R',	"wnnnnnwwnn"},
            {'S',	"nnwnnnwwnn"},
            {'T',	"nnnnwnwwnn"},
            {'U',	"wwnnnnnnwn"},
            {'V',	"nwwnnnnnwn"},
            {'W',	"wwwnnnnnnn"},
            {'X',	"nwnnwnnnwn"},
            {'Y',	"wwnnwnnnnn"},
            {'Z',	"nwwnwnnnnn"},
            {'-',	"nwnnnnwnwn"},
            {'.',	"wwnnnnwnnn"},
            {' ',	"nwwnnnwnnn"},
            {'*',	"nwnnwnwnnn"},
            {'$',	"nwnwnwnnnn"},
            {'/',	"nwnwnnnwnn"},
            {'+',	"nwnnnwnwnn"},
            {'%',	"nnnwnwnwnn"},
        };

        int i0, iNum0 = sizeof(infs) / sizeof(infs[0]);
        int i1;
        for (i0 = 0; i0 < iNum0; i0++) {
            IntString&inf = infs[i0];
            if (inf.ch == ch) {
                for (i1 = 0; i1 < 10; i1++) {
                    if (inf.psz[i1] == 'w')	*pb += 2;
                    if (i1 % 2 == 0)			*pb += 1;
                    pb++;
                }
                return pb;
            }
        }
        return 0;
    }
};

class BarcodeI2of5 :public BarcodeBase {
    //[n/a][n/a][n/a][n/a][n/a][n/a][w-n][b-s]
public:
    BarcodeI2of5() {
    }
    ~BarcodeI2of5() {
    }

    virtual BOOL Encode(const char*pszCode) {
        Clear();
        BYTE*pFst = ia_Buf;
        BYTE*pb = pFst;

        const int iNum = strlen(pszCode);

        int i;
        //"nnnn"
        for (i = 0; i < 4; i++) {
            if (i % 2 == 0)	*pb += 1;
            pb++;
        }

        int iV;
        for (i = 0; i < iNum; i += 2) {
            iV = pszCode[i] - '0';
            iV = iV * 10;
            iV += pszCode[i + 1] - '0';
            pb = P_GetNarrorWideBarSpaceI2of5(pb, iV);
            if (pb == 0)	return 0;
        }

        //"wnn"
        *pb += 3;	pb++;
        *pb += 0;	pb++;
        *pb += 1;	pb++;

        i_LenBuf = pb - pFst;
        return 1;
    }

    void DrawI2of5(HDC hDC, int iX, int iY0, int iY1, const COLORREF clrBar, const COLORREF clrSpace, const int iPenW) {
        DrawBarcode(hDC, iX, iY0, iY1, iY1, clrBar, clrSpace, iPenW);
    }

private:
    BYTE*P_GetNarrorWideBarSpaceI2of5(BYTE*pb, int ch) {
        if (ch < 0)	return 0;
        if (ch > 99)	return 0;

        IntString infs[] =
        {
            {0,		"nnnnwwwwnn"},
            {1,		"nwnnwnwnnw"},
            {2,		"nnnwwnwnnw"},
            {3,		"nwnwwnwnnn"},
            {4,		"nnnnwwwnnw"},
            {5,		"nwnnwwwnnn"},
            {6,		"nnnwwwwnnn"},
            {7,		"nnnnwnwwnw"},
            {8,		"nwnnwnwwnn"},
            {9,		"nnnwwnwwnn"},
            {10,	"wnnnnwnwwn"},
            {11,	"wwnnnnnnww"},
            {12,	"wnnwnnnnww"},
            {13,	"wwnwnnnnwn"},
            {14,	"wnnnnwnnww"},
            {15,	"wwnnnwnnwn"},
            {16,	"wnnwnwnnwn"},
            {17,	"wnnnnnnwww"},
            {18,	"wwnnnnnwwn"},
            {19,	"wnnwnnnwwn"},
            {20,	"nnwnnwnwwn"},
            {21,	"nwwnnnnnww"},
            {22,	"nnwwnnnnww"},
            {23,	"nwwwnnnnwn"},
            {24,	"nnwnnwnnww"},
            {25,	"nwwnnwnnwn"},
            {26,	"nnwwnwnnwn"},
            {27,	"nnwnnnnwww"},
            {28,	"nwwnnnnwwn"},
            {29,	"nnwwnnnwwn"},
            {30,	"wnwnnwnwnn"},
            {31,	"wwwnnnnnnw"},
            {32,	"wnwwnnnnnw"},
            {33,	"wwwwnnnnnn"},
            {34,	"wnwnnwnnnw"},
            {35,	"wwwnnwnnnn"},
            {36,	"wnwwnwnnnn"},
            {37,	"wnwnnnnwnw"},
            {38,	"wwwnnnnwnn"},
            {39,	"wnwwnnnwnn"},
            {40,	"nnnnwwnwwn"},
            {41,	"nwnnwnnnww"},
            {42,	"nnnwwnnnww"},
            {43,	"nwnwwnnnwn"},
            {44,	"nnnnwwnnww"},
            {45,	"nwnnwwnnwn"},
            {46,	"nnnwwwnnwn"},
            {47,	"nnnnwnnwww"},
            {48,	"nwnnwnnwwn"},
            {49,	"nnnwwnnwwn"},
            {50,	"wnnnwwnwnn"},
            {51,	"wwnnwnnnnw"},
            {52,	"wnnwwnnnnw"},
            {53,	"wwnwwnnnnn"},
            {54,	"wnnnwwnnnw"},
            {55,	"wwnnwwnnnn"},
            {56,	"wnnwwwnnnn"},
            {57,	"wnnnwnnwnw"},
            {58,	"wwnnwnnwnn"},
            {59,	"wnnwwnnwnn"},
            {60,	"nnwnwwnwnn"},
            {61,	"nwwnwnnnnw"},
            {62,	"nnwwwnnnnw"},
            {63,	"nwwwwnnnnn"},
            {64,	"nnwnwwnnnw"},
            {65,	"nwwnwwnnnn"},
            {66,	"nnwwwwnnnn"},
            {67,	"nnwnwnnwnw"},
            {68,	"nwwnwnnwnn"},
            {69,	"nnwwwnnwnn"},
            {70,	"nnnnnwwwwn"},
            {71,	"nwnnnnwnww"},
            {72,	"nnnwnnwnww"},
            {73,	"nwnwnnwnwn"},
            {74,	"nnnnnwwnww"},
            {75,	"nwnnnwwnwn"},
            {76,	"nnnwnwwnwn"},
            {77,	"nnnnnnwwww"},
            {78,	"nwnnnnwwwn"},
            {79,	"nnnwnnwwwn"},
            {80,	"wnnnnwwwnn"},
            {81,	"wwnnnnwnnw"},
            {82,	"wnnwnnwnnw"},
            {83,	"wwnwnnwnnn"},
            {84,	"wnnnnwwnnw"},
            {85,	"wwnnnwwnnn"},
            {86,	"wnnwnwwnnn"},
            {87,	"wnnnnnwwnw"},
            {88,	"wwnnnnwwnn"},
            {89,	"wnnwnnwwnn"},
            {90,	"nnwnnwwwnn"},
            {91,	"nwwnnnwnnw"},
            {92,	"nnwwnnwnnw"},
            {93,	"nwwwnnwnnn"},
            {94,	"nnwnnwwnnw"},
            {95,	"nwwnnwwnnn"},
            {96,	"nnwwnwwnnn"},
            {97,	"nnwnnnwwnw"},
            {98,	"nwwnnnwwnn"},
            {99,	"nnwwnnwwnn"},
        };

        IntString&inf = infs[ch];

        int i;
        for (i = 0; i < 10; i++) {
            if (inf.psz[i] == 'w')	*pb += 2;
            if (i % 2 == 0)			*pb += 1;
            pb++;
        }
        return pb;
    }
};

class Barcode93 :public BarcodeBase {
    //[n/a][n/a][n/a][n/a][n/a][n/a][n/a][b-s]
public:
    Barcode93() {
    }
    ~Barcode93() {
    }

    virtual BOOL Encode(const char* pszCode) {
        Clear();
        const int iNum = strlen(pszCode);

        BYTE*pFst = ia_Buf;
        BYTE*pb = pFst;

        pb = P_GetBarSpace93(pb, 47);
        if (pb == 0)	return 0;

        BOOL b;
        int i, iFirst, iSecond;
        for (i = 0; i < iNum; i++) {
            b = P_AscIItoCode93Sequence((int)pszCode[i], iFirst, iSecond);
            if (b == 0)	return 0;

            pb = P_GetBarSpace93(pb, iFirst);
            if (pb == 0)	return 0;

            if (iSecond != -1) {
                pb = P_GetBarSpace93(pb, iSecond);
                if (pb == 0)	return 0;
            }
        }

        pb = P_GetCheckDigits(pb, pszCode);
        if (pb == 0)	return 0;

        pb = P_GetBarSpace93(pb, 48);
        if (pb == 0)	return 0;

        i_LenBuf = pb - pFst;
        return 1;
    }

    void Draw93(HDC hDC, int iX, int iY0, int iY1, const COLORREF clrBar, const COLORREF clrSpace, const int iPenW) {
        DrawBarcode(hDC, iX, iY0, iY1, iY1, clrBar, clrSpace, iPenW);
    }

private:
    BYTE*P_GetBarSpace93(BYTE*pb, int ch) {
        if (ch < 0)	return 0;
        if (ch > 48)	return 0;

        IntString infs[] =
        {
            {0,		"bsssbsbss"},
            {1,		"bsbssbsss"},
            {2,		"bsbsssbss"},
            {3,		"bsbssssbs"},
            {4,		"bssbsbsss"},
            {5,		"bssbssbss"},
            {6,		"bssbsssbs"},
            {7,		"bsbsbssss"},
            {8,		"bsssbssbs"},
            {9,		"bssssbsbs"},
            {10,	"bbsbsbsss"},
            {11,	"bbsbssbss"},
            {12,	"bbsbsssbs"},
            {13,	"bbssbsbss"},
            {14,	"bbssbssbs"},
            {15,	"bbsssbsbs"},
            {16,	"bsbbsbsss"},
            {17,	"bsbbssbss"},
            {18,	"bsbbsssbs"},
            {19,	"bssbbsbss"},
            {20,	"bsssbbsbs"},
            {21,	"bsbsbbsss"},
            {22,	"bsbssbbss"},
            {23,	"bsbsssbbs"},
            {24,	"bssbsbbss"},
            {25,	"bsssbsbbs"},
            {26,	"bbsbbsbss"},
            {27,	"bbsbbssbs"},
            {28,	"bbsbsbbss"},
            {29,	"bbsbssbbs"},
            {30,	"bbssbsbbs"},
            {31,	"bbssbbsbs"},
            {32,	"bsbbsbbss"},
            {33,	"bsbbssbbs"},
            {34,	"bssbbsbbs"},
            {35,	"bssbbbsbs"},
            {36,	"bssbsbbbs"},
            {37,	"bbbsbsbss"},
            {38,	"bbbsbssbs"},
            {39,	"bbbssbsbs"},
            {40,	"bsbbsbbbs"},
            {41,	"bsbbbsbbs"},
            {42,	"bbsbsbbbs"},
            {43,	"bssbssbbs"},
            {44,	"bbbsbbsbs"},
            {45,	"bbbsbsbbs"},
            {46,	"bssbbssbs"},
            {47,	"bsbsbbbbs"},
            {48,	"bsbsbbbbsb"},
        };

        IntString&inf = infs[ch];
        int i;
        for (i = 0; i < 9; i++) {
            if (inf.psz[i] == 'b')	*pb += 1;
            pb++;
        }

        if (ch == 48) {
            *pb += 1;
            pb++;
        }
        return pb;
    }

private:
    BYTE*P_GetCheckDigits(BYTE*pb, const char*&pszCode) {
        int i, iSum, iWeight, iFirst, iSecond;

        // "C" check digit character
        iWeight = 1;
        iSum = 0;

        const int iNum = strlen(pszCode);

        for (i = iNum - 1; i > -1; i--) {
            P_AscIItoCode93Sequence((int)pszCode[i], iFirst, iSecond);

            iSum += (iWeight*iFirst);
            iWeight++;
            if (iWeight > 20)	iWeight = 1;

            if (iSecond != -1) {
                iSum += (iWeight*iSecond);
                iWeight++;
                if (iWeight > 20)	iWeight = 1;
            }
        }

        pb = P_GetBarSpace93(pb, iSum % 47);
        if (pb == 0)	return 0;

        iWeight = 2;
        iSum = iSum % 47;
        for (i = iNum - 1; i > -1; i--) {
            P_AscIItoCode93Sequence((int)pszCode[i], iFirst, iSecond);

            iSum += (iWeight * iFirst);
            iWeight++;
            if (iWeight > 15)	iWeight = 1;

            if (iSecond != -1) {
                iSum += (iWeight * iSecond);
                iWeight++;
                if (iWeight > 15)	iWeight = 1;
            }
        }

        pb = P_GetBarSpace93(pb, iSum % 47);
        if (pb == 0)	return 0;

        return pb;
    }

    BOOL P_AscIItoCode93Sequence(int iValue, int&iFirst, int&iSecond) {
        if (iValue < 0)	return 0;
        if (iValue > 127)	return 0;

        struct	I3 { int iV, iFirst, iSecond; };

        I3 i3s[] =
        {
            {0,	44,	30},
            {1,	43,	10},
            {2,	43,	11},
            {3,	43,	12},
            {4,	43,	13},
            {5,	43,	14},
            {6,	43,	15},
            {7,	43,	16},
            {8,	43,	17},
            {9,	43,	18},
            {10,43,	19},
            {11,43,	20},
            {12,43,	21},
            {13,43,	22},
            {14,43,	23},
            {15,43,	24},
            {16,43,	25},
            {17,43,	26},
            {18,43,	27},
            {19,43,	28},
            {20,43,	29},
            {21,43,	30},
            {22,43,	31},
            {23,43,	32},
            {24,43,	33},
            {25,43,	34},
            {26,43,	35},
            {27,44,	10},
            {28,44,	11},
            {29,44,	12},
            {30,44,	13},
            {31,44,	14},
            {32,38,	-1},
            {33,45,	10},
            {34,45,	11},
            {35,45,	12},
            {36,39,	-1},
            {37,42,	-1},
            {38,45,	15},
            {39,45,	16},
            {40,45,	17},
            {41,45,	18},
            {42,45,	19},
            {43,41,	-1},
            {44,45,	21},
            {45,36,	-1},
            {46,37,	-1},
            {47,40,	-1},
            {48,0,	-1},
            {49,1,	-1},
            {50,2,	-1},
            {51,3,	-1},
            {52,4,	-1},
            {53,5,	-1},
            {54,6,	-1},
            {55,7,	-1},
            {56,8,	-1},
            {57,9,	-1},
            {58,45,	35},
            {59,44,	15},
            {60,44,	16},
            {61,44,	17},
            {62,44,	18},
            {63,44,	19},
            {64,44,	31},
            {65,10,	-1},
            {66,11,	-1},
            {67,12,	-1},
            {68,13,	-1},
            {69,14,	-1},
            {70,15,	-1},
            {71,16,	-1},
            {72,17,	-1},
            {73,18,	-1},
            {74,19,	-1},
            {75,20,	-1},
            {76,21,	-1},
            {77,22,	-1},
            {78,23,	-1},
            {79,24,	-1},
            {80,25,	-1},
            {81,26,	-1},
            {82,27,	-1},
            {83,28,	-1},
            {84,29,	-1},
            {85,30,	-1},
            {86,31,	-1},
            {87,32,	-1},
            {88,33,	-1},
            {89,34,	-1},
            {90,35,	-1},
            {91,44,	20},
            {92,44,	21},
            {93,44,	22},
            {94,44,	23},
            {95,44,	24},
            {96,44,	32},
            {97,46,	10},
            {98,46,	11},
            {99,46,	12},
            {100,46,13},
            {101,46,14},
            {102,46,15},
            {103,46,16},
            {104,46,17},
            {105,46,18},
            {106,46,19},
            {107,46,20},
            {108,46,21},
            {109,46,22},
            {110,46,23},
            {111,46,24},
            {112,46,25},
            {113,46,26},
            {114,46,27},
            {115,46,28},
            {116,46,29},
            {117,46,30},
            {118,46,31},
            {119,46,32},
            {120,46,33},
            {121,46,34},
            {122,46,35},
            {123,44,25},
            {124,44,26},
            {125,44,27},
            {126,44,28},
            {127,44,29},
        };

        I3&i3 = i3s[iValue];
        iFirst = i3.iFirst;
        iSecond = i3.iSecond;
        return 1;
    }
};

class Barcode128 :public BarcodeBase {
public:
    Barcode128() {
    }
    ~Barcode128() {
    }

    BOOL Encode128A(const char* pszCode) { return P_Encode128((char*)pszCode, SUB::SETA); }
    BOOL Encode128B(const char* pszCode) { return P_Encode128((char*)pszCode, SUB::SETB); }
    BOOL Encode128C(const char* pszCode) { return P_Encode128((char*)pszCode, SUB::SETC); }

    void Draw128(HDC hDC, int iX, int iY0, int iY1, const COLORREF clrBar, const COLORREF clrSpace, const int iPenW) {
        DrawBarcode(hDC, iX, iY0, iY1, iY1, clrBar, clrSpace, iPenW);
    }

private:
    struct SUB {
        enum {
            SETA = 0,
            SETB = 1,
            SETC = 2,
        };
    };

    BOOL P_Encode128(char*pszCode, const int iSetIn) {
        Clear();
        BYTE*pFst = ia_Buf;
        BYTE*pb = pFst;


        if (iSetIn == SUB::SETA)	pb = P_GetBarSpace128(pb, 103);
        else
            if (iSetIn == SUB::SETB)	pb = P_GetBarSpace128(pb, 104);
            else					pb = P_GetBarSpace128(pb, 105);
            if (pb == 0)	return 0;

            const int iCheckDigit = GetCheckDigit(iSetIn, pszCode);
            const int iNum = strlen(pszCode);

            int iChar, iCharNext;
            int iPosition = 0;
            int iSet = iSetIn;

            while (iPosition < iNum) {
                if (iSet == SUB::SETC) {
                    if (ga2_Code128[SUB::SETA][pszCode[iPosition]] == 101) {
                        pb = P_GetBarSpace128(pb, 101);
                        iPosition++;
                        iSet = SUB::SETA;
                    }
                    else
                        if (ga2_Code128[SUB::SETA][pszCode[iPosition]] == 100) {
                            pb = P_GetBarSpace128(pb, 100);
                            iPosition++;
                            iSet = SUB::SETB;
                        }
                        else if (ga2_Code128[SUB::SETA][pszCode[iPosition]] == 102) {
                            pb = P_GetBarSpace128(pb, 100);
                            iPosition++;
                        }
                        else {
                            char chT = pszCode[iPosition + 2];
                            pszCode[iPosition + 2] = 0;
                            iChar = atoi(&pszCode[iPosition]);
                            pszCode[iPosition + 2] = chT;

                            pb = P_GetBarSpace128(pb, iChar);
                            if (pb == 0)	return 0;
                            iPosition += 2;
                        }
                }
                else {
                    int iTemp2 = pszCode[iPosition];
                    if (iTemp2 < -1)	iTemp2 = iTemp2 & 255;

                    iChar = ga2_Code128[iSet][iTemp2];

                    pb = P_GetBarSpace128(pb, iChar);
                    if (pb == 0)	return 0;
                    iPosition++;
                    if (iSet == SUB::SETA) {
                        if (iChar == 100)	iSet = SUB::SETB;
                        else
                            if (iChar == 99)	iSet = SUB::SETC;
                    }
                    else if (iSet == SUB::SETB) {
                        if (iChar == 101)	iSet = SUB::SETA;
                        else
                            if (iChar == 99)	iSet = SUB::SETC;
                    }
                    else
                        if (iChar == 98) {
                            if (iSet == SUB::SETA)
                                iCharNext = ga2_Code128[SUB::SETB][pszCode[iPosition]];
                            else
                                iCharNext = ga2_Code128[SUB::SETA][pszCode[iPosition]];

                            pb = P_GetBarSpace128(pb, iChar);
                            if (pb == 0)	return 0;
                            iPosition++;
                        }
                }
            }

            pb = P_GetBarSpace128(pb, iCheckDigit);
            if (pb == 0)	return 0;
            pb = P_GetBarSpace128(pb, 106);
            i_LenBuf = pb - pFst;

            return 1;
    }

    BYTE*P_GetBarSpace128(BYTE*pb, int iV) {
        if (iV < 0)	return 0;
        if (iV > 106)	return 0;
        IntString infs[] =
        {
            {0,		"bbsbbssbbss"},
            {1,		"bbssbbsbbss"},
            {2,		"bbssbbssbbs"},
            {3,		"bssbssbbsss"},
            {4,		"bssbsssbbss"},
            {5,		"bsssbssbbss"},
            {6,		"bssbbssbsss"},
            {7,		"bssbbsssbss"},
            {8,		"bsssbbssbss"},
            {9,		"bbssbssbsss"},
            {10,	"bbssbsssbss"},
            {11,	"bbsssbssbss"},
            {12,	"bsbbssbbbss"},
            {13,	"bssbbsbbbss"},
            {14,	"bssbbssbbbs"},
            {15,	"bsbbbssbbss"},
            {16,	"bssbbbsbbss"},
            {17,	"bssbbbssbbs"},
            {18,	"bbssbbbssbs"},
            {19,	"bbssbsbbbss"},
            {20,	"bbssbssbbbs"},
            {21,	"bbsbbbssbss"},
            {22,	"bbssbbbsbss"},
            {23,	"bbbsbbsbbbs"},
            {24,	"bbbsbssbbss"},
            {25,	"bbbssbsbbss"},
            {26,	"bbbssbssbbs"},
            {27,	"bbbsbbssbss"},
            {28,	"bbbssbbsbss"},
            {29,	"bbbssbbssbs"},
            {30,	"bbsbbsbbsss"},
            {31,	"bbsbbsssbbs"},
            {32,	"bbsssbbsbbs"},
            {33,	"bsbsssbbsss"},
            {34,	"bsssbsbbsss"},
            {35,	"bsssbsssbbs"},
            {36,	"bsbbsssbsss"},
            {37,	"bsssbbsbsss"},
            {38,	"bsssbbsssbs"},
            {39,	"bbsbsssbsss"},
            {40,	"bbsssbsbsss"},
            {41,	"bbsssbsssbs"},
            {42,	"bsbbsbbbsss"},
            {43,	"bsbbsssbbbs"},
            {44,	"bsssbbsbbbs"},
            {45,	"bsbbbsbbsss"},
            {46,	"bsbbbsssbbs"},
            {47,	"bsssbbbsbbs"},
            {48,	"bbbsbbbsbbs"},
            {49,	"bbsbsssbbbs"},
            {50,	"bbsssbsbbbs"},
            {51,	"bbsbbbsbsss"},
            {52,	"bbsbbbsssbs"},
            {53,	"bbsbbbsbbbs"},
            {54,	"bbbsbsbbsss"},
            {55,	"bbbsbsssbbs"},
            {56,	"bbbsssbsbbs"},
            {57,	"bbbsbbsbsss"},
            {58,	"bbbsbbsssbs"},
            {59,	"bbbsssbbsbs"},
            {60,	"bbbsbbbbsbs"},
            {61,	"bbssbssssbs"},
            {62,	"bbbbsssbsbs"},
            {63,	"bsbssbbssss"},
            {64,	"bsbssssbbss"},
            {65,	"bssbsbbssss"},
            {66,	"bssbssssbbs"},
            {67,	"bssssbsbbss"},
            {68,	"bssssbssbbs"},
            {69,	"bsbbssbssss"},
            {70,	"bsbbssssbss"},
            {71,	"bssbbsbssss"},
            {72,	"bssbbssssbs"},
            {73,	"bssssbbsbss"},
            {74,	"bssssbbssbs"},
            {75,	"bbssssbssbs"},
            {76,	"bbssbsbssss"},
            {77,	"bbbbsbbbsbs"},
            {78,	"bbssssbsbss"},
            {79,	"bsssbbbbsbs"},
            {80,	"bsbssbbbbss"},
            {81,	"bssbsbbbbss"},
            {82,	"bssbssbbbbs"},
            {83,	"bsbbbbssbss"},
            {84,	"bssbbbbsbss"},
            {85,	"bssbbbbssbs"},
            {86,	"bbbbsbssbss"},
            {87,	"bbbbssbsbss"},
            {88,	"bbbbssbssbs"},
            {89,	"bbsbbsbbbbs"},
            {90,	"bbsbbbbsbbs"},
            {91,	"bbbbsbbsbbs"},
            {92,	"bsbsbbbbsss"},
            {93,	"bsbsssbbbbs"},
            {94,	"bsssbsbbbbs"},
            {95,	"bsbbbbsbsss"},
            {96,	"bsbbbbsssbs"},
            {97,	"bbbbsbsbsss"},
            {98,	"bbbbsbsssbs"},
            {99,	"bsbbbsbbbbs"},
            {100,	"bsbbbbsbbbs"},
            {101,	"bbbsbsbbbbs"},
            {102,	"bbbbsbsbbbs"},
            //			{103,	"bbsbsbbbbss"},
                        {103,	"bbsbssssbss"},
                        {104,	"bbsbssbssss"},
                        {105,	"bbsbssbbbss"},
                        {106,	"bbsssbbbsbsbb"},
        };

        int i;
        IntString&inf = infs[iV];
        for (i = 0; i < 11; i++) {
            if (inf.psz[i] == 'b')	*pb += 1;
            pb++;
        }
        if (iV == 106) {
            *pb += 1;	pb++;
            *pb += 1;	pb++;
        }
        return pb;
    }

private:
    int GetCheckDigit(const int iSet, char*pszCode) {
        int	iSum = 0, iCurSet = 0, iChar128, iCharNext, iWeight, iPosition;

        iCurSet = iSet;
        if (iSet == SUB::SETA) {
            iSum = 103;
        }
        else
            if (iSet == SUB::SETB) {
                iSum = 104;
            }
            else
                if (iSet == SUB::SETC) {
                    iSum = 105;
                }

        iPosition = 0;
        iWeight = 1;

        const int iNum = strlen(pszCode);
        while (iPosition < iNum) {
            if (iCurSet == SUB::SETC) {
                if (ga2_Code128[SUB::SETA][pszCode[iPosition]] == 101) {
                    iChar128 = 101;
                    iSum += (iWeight*iChar128);

                    iPosition++;
                    iWeight++;
                    iCurSet = SUB::SETA;
                }
                else if (ga2_Code128[SUB::SETA][pszCode[iPosition]] == 100) {
                    iChar128 = 100;
                    iSum += (iWeight*iChar128);
                    iPosition++;
                    iWeight++;
                    iCurSet = SUB::SETB;
                }
                else
                    if (ga2_Code128[SUB::SETA][pszCode[iPosition]] == 102) {
                        iChar128 = 102;
                        iSum += (iWeight*iChar128);
                        iPosition++;
                        iWeight++;
                    }
                    else {
                        char chT = pszCode[iPosition + 2];
                        pszCode[iPosition + 2] = 0;
                        iChar128 = atol(&pszCode[iPosition]);
                        pszCode[iPosition + 2] = chT;

                        iSum += (iWeight*iChar128);
                        iPosition += 2;
                        iWeight++;
                    }
            }
            else {
                int iTemp2 = pszCode[iPosition];
                if (iTemp2 < -1)	iTemp2 = iTemp2 & 255;

                iChar128 = ga2_Code128[iCurSet][iTemp2];

                iSum += (iWeight*iChar128);

                iPosition++;
                iWeight++;

                if (iCurSet == SUB::SETA) {
                    if (iChar128 == 100)
                        iCurSet = SUB::SETB;
                    else if (iChar128 == 99)
                        iCurSet = SUB::SETC;
                }
                else
                    if (iCurSet == SUB::SETB) {
                        if (iChar128 == 101)		iCurSet = SUB::SETA;
                        else if (iChar128 == 99)	iCurSet = SUB::SETC;
                    }
                    else
                        if (iChar128 == 98) {
                            if (iCurSet == SUB::SETA)
                                iCharNext = ga2_Code128[SUB::SETB][pszCode[iPosition]];
                            else
                                iCharNext = ga2_Code128[SUB::SETA][pszCode[iPosition]];

                            iSum += (iWeight*iCharNext);
                            iPosition++;
                            iWeight++;
                        }
            }
        }
        return iSum % 103;
    }
};

//=============================================
class BarcodeEan13 :public BarcodeBase {
public:
    BarcodeEan13() {
    }
    ~BarcodeEan13() {
    }

    BOOL EncodeEan13(const char*pszCodeIn) {
        Clear();

        //only allow 12 characters as input
        char szCode[14];
        const int iLen = strlen(pszCodeIn);
        if (iLen > 12) {
            strncpy(szCode, pszCodeIn, 12);
        }
        else {
            strcpy(szCode, pszCodeIn);
            while (strlen(szCode) < 12)	strcat(szCode, "0");
        }

        BYTE*pFst = ia_Buf;
        BYTE*pb = pFst;

        //"bsb"-long
        *pb += 5;	pb++;
        *pb += 4;	pb++;
        *pb += 5;	pb++;

        BYTE iaCountryCode[6];
        BOOL b = P_GetCountryCode(szCode[0], iaCountryCode);
        if (b == 0)	return 0;

        pb = P_GetLeftOddParity(pb, szCode[1]);

        int i;
        for (i = 2; i < 7; i++) {
            if (iaCountryCode[i - 2] == 'O') {
                pb = P_GetLeftOddParity(pb, szCode[i]);
            }
            else
                if (iaCountryCode[i - 2] == 'E') {
                    pb = P_GetLeftEvenParity(pb, szCode[i]);
                }
        }

        //"sbsbs"-long
        *pb += 4;	pb++;
        *pb += 5;	pb++;
        *pb += 4;	pb++;
        *pb += 5;	pb++;
        *pb += 4;	pb++;

        for (i = 7; i < 12; i++) {
            pb = P_GetRightPattern(pb, szCode[i]);
        }

        i = P_GetCheckSumDigit(szCode);
        pb = P_GetRightPattern(pb, (char)i);

        //"bsb"-long
        *pb += 5;	pb++;
        *pb += 4;	pb++;
        *pb += 5;	pb++;

        i_LenBuf = pb - pFst;
        return 1;
    }

    void DrawEan13(HDC hDC, int iX, int iY0, int iY10, int iY11, const COLORREF clrBar, const COLORREF clrSpace, const int iPenW) {
        DrawBarcode(hDC, iX, iY0, iY10, iY11, clrBar, clrSpace, iPenW);
    }

private:
    BOOL P_GetCountryCode(char ch, BYTE*pbCountryCode) {
        const int iV = ch - '0';
        if (iV < 0)	return 0;
        if (iV > 9)	return 0;

        IntString infs[] =
        {
            {0,	"OOOOO"},
            {1,	"OEOEE"},
            {2,	"OEEOE"},
            {3,	"OEEEO"},
            {4,	"EOOEE"},
            {5,	"EEOOE"},
            {6,	"EEEOO"},
            {7,	"EOEOE"},
            {8,	"EOEEO"},
            {9,	"EEOEO"},
        };
        memcpy(pbCountryCode, infs[iV].psz, 5);
        return 1;
    }

    BYTE*P_GetLeftOddParity(BYTE*pb, char ch) {
        const int iV = ch - '0';
        if (iV < 0)	return 0;
        if (iV > 9)	return 0;


        IntString infs[] =
        {
            {0,	"sssbbsb"},
            {1,	"ssbbssb"},
            {2,	"ssbssbb"},
            {3,	"sbbbbsb"},
            {4,	"sbsssbb"},
            {5,	"sbbsssb"},
            {6,	"sbsbbbb"},
            {7,	"sbbbsbb"},
            {8,	"sbbsbbb"},
            {9,	"sssbsbb"},
        };

        IntString&inf = infs[iV];
        int i;
        for (i = 0; i < 7; i++) {
            if (inf.psz[i] == 'b')	*pb += 1;
            pb++;
        }
        return pb;
    }

    BYTE*P_GetLeftEvenParity(BYTE*pb, char ch) {
        const int iV = ch - '0';
        if (iV < 0)	return 0;
        if (iV > 9)	return 0;

        IntString infs[] =
        {
            {0,	"sbssbbb"},
            {1,	"sbbssbb"},
            {2,	"ssbbsbb"},
            {3,	"sbssssb"},
            {4,	"ssbbbsb"},
            {5,	"sbbbssb"},
            {6,	"ssssbsb"},
            {7,	"ssbsssb"},
            {8,	"sssbssb"},
            {9,	"ssbsbbb"},
        };
        char*psz = infs[iV].psz;
        int i;
        for (i = 0; i < 7; i++) {
            if (psz[i] == 'b')	*pb += 1;
            pb++;
        }
        return pb;
    }

    BYTE*P_GetRightPattern(BYTE*pb, char ch) {
        const int iV = ch - '0';
        if (iV < 0)	return 0;
        if (iV > 9)	return 0;

        IntString infs[] =
        {
            {0,	"bbbssbs"},
            {1,	"bbssbbs"},
            {2,	"bbsbbss"},
            {3,	"bssssbs"},
            {4,	"bsbbbss"},
            {5,	"bssbbbs"},
            {6,	"bsbssss"},
            {7,	"bsssbss"},
            {8,	"bssbsss"},
            {9,	"bbbsbss"},
        };
        char*psz = infs[iV].psz;
        int i;
        for (i = 0; i < 7; i++) {
            if (psz[i] == 'b')	*pb += 1;
            pb++;
        }
        return pb;
    }

    char P_GetCheckSumDigit(const char*pszCode) {
        const int iLen = strlen(pszCode);
        int i, iSum = 0, iItem;

        for (i = iLen; i >= 1; i--) {
            iItem = i % 2 ? (pszCode[i - 1] - '0') * 1 : (pszCode[i - 1] - '0') * 3;
            iSum += iItem;
        }

        iSum %= 10;
        return '0' + (10 - iSum) % 10;
    }

};

#endif

