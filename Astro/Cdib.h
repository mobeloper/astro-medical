

// DIB.h 

#ifndef __DIB_H__
#define __DIB_H__


#include "..\\Astro\\Dicom.h"
#include "..\\Astro\\filter.h"

#define LITTLE_ENDIAN	0x4C45	//LE
#define BIG_ENDIAN		0x4245	//BE

#define	TXT_ROW			1
#define	TXT_COL			1
#define	T_INCREMENT		130

#define NUM_TAGS 240

#define CHAR_TYPE 0
#define UCHAR_TYPE 1
#define SHORT_TYPE 2
#define USHORT_TYPE 3
#define LONG_TYPE 4
#define ULONG_TYPE 5
#define FLOAT_TYPE 6


#define MAX_LUTSIZE		65536

#define OPTIMIZEDRANGE	0.001	//tolerance for Contrast window

#define GAMMA_VALS  {0.04F, 0.10F, 0.20F, 0.40F, 0.67F, 1.0F, 1.5F, 2.5F, 5.0F, 10.0F, 25.0F}

#define PATIENT_NAME	0x00100010
#define PATIENT_ID		0x00100020
#define PATIENT_BDATE	0x00100030
#define PATIENT_SEX		0x00100040
#define BODY_PART		0x00180015
#define SDY_DATE		0x00080020
#define SDY_TIME		0x00080030

#define TAG_LEVEL		0x00281050
#define TAG_WINDOW		0x00281051



#define SV_INVERSE		0x00291200		//SanteView inverse Tag

#define ctrlColBlack RGB(0x00,0x00,0x00)
#define ctrlColWhite RGB(0xFF,0xFF,0xFF)


class CDib
{
public:
	CDib();
	~CDib();

	
	BOOL SaveDCMfile(LPCTSTR pszFilename);
	BOOL SaveRAWfile(LPCTSTR pszFilename);

	BOOL Draw( CDC *, int nX = 0, int nY = 0, int ndWidth = -1, int ndHeight = -1, float fZoom = -1.0F, int mode = SRCCOPY);
	
	BOOL DisplayImgInfo(CDC *pDC);
	BOOL SetPalette( CDC * );
	BOOL Getsize(LONG *,LONG *);

public:


	BOOL CollectDICOMHeaderData();

	/**************************************
	   Colect DICOM Header data From DB
	****************************************/
	BOOL CollectDICOMHeaderDataFromDB();


	void DeleteObject();
	void Reset();
	void Recovery();	
	BOOL FakeColor(byte R,byte G,byte B);
	

	BOOL ComputeHistogram();
	BOOL TransformPixelType();	
	BOOL CreateGammaLUT(float fPrecompensation);
	BOOL CreateLUT(int nWindowVal,int nLevelVal);

	BOOL CreateScreenBits();

	BOOL CreateDIB();
	
	BOOL ImageTransformation(int nInvert, int nEdge, int nSharp, int nSmooth, int nDenoise, int nGamma);

	BOOL GetDicomTags(CHAR_DICOM_ELEMENT *charDCMtags);


	BOOL LoadDCMfile(LPCTSTR pszFilename);


	BOOL LoadRAWfile(LPCTSTR pszFilename,int nPixelType, int nImagWidth, int nImagHeight, int nEndian);

	/*********************************
	          
	****************************************/

	//BOOL LoadBitmap(LPCSTR filename);
	//BOOL CreateDIBIndirect(LPBITMAPINFO pBMI, const void* lpBits);
		

	DWORD  m_ByteCount;
	
	LPBYTE p_DCM;
	
	
	
	DWORD m_dwDibSize;

	CPalette m_Palette;

	RGBQUAD *m_pPalette;
	BITMAPINFOHEADER *m_pBIH;
	unsigned char *m_pDib, *pbScreenBits; // Data bits to be displayed on the screen	

	int m_nPaletteEntries;


	/******************
	추가변수
	*******************/
// 	unsigned char *m_pDib;
// 	unsigned char *m_pDibBits;


  /******************
	추가함수
	*******************/
    BOOL LoadBMPFile( const char * );
	BOOL Save( const char * );
	BOOL Draw( CDC *, int nX = 0, int nY = 0, int nWidth = -1, int nHeight = -1, int mode = SRCCOPY);
	//BOOL SetPalette( CDC * );
// 	bool GetPixel(unsigned int nX,unsigned int nY,RGBQUAD& rgb);
// 	bool SetPixel(unsigned int nX,unsigned int nY,const RGBQUAD& rgb);
	inline long Height() { return m_pBIH->biHeight; }
	inline long Width()  { return m_pBIH->biWidth; }
	inline unsigned char* GetImageDataPoint() { return pbScreenBits; }
	inline unsigned short GetBitCount() { return m_pBIH->biBitCount; }



	//Look-up table for contrast adjustment
	BYTE bLUT[MAX_LUTSIZE];	//2^B, B =Bits Allocated, if B=12, bLUT[4096].
	
	//Look-up table for Gamma Correction
	WORD wGammaLUT[MAX_LUTSIZE];	//2^B, B =Bits Allocated, if B=12, bLUT[4096].

	int nNumbPixels;		//Indicate the maximum window
	int nMaxPixelValue;		//Indicate the maximum level
	int nMinPixelValue;
	
	int nInvPixelsOr;
	int nHistogram[PIXELMAX];
	int nMaxHistOr;
	int nMinHistOr;
	int nMaxHist;
	int nMinHist;
	int WC;			//Keep the original Window Center contrast
	int WW;			//Keep the original Window Width contrast

	bool hasBackground;

	int CursorPosition;
	int OldCursorPosition;


	///-----------------------------------------------------------------------------------------
	
	DICOM_ELEMENT	FullTagDataElmnt[NUM_FULL_TAGS];			//Full Dicom Tags Structure
	DICOM_ELEMENT	BasicTagDataElmnt[NUM_BASIC_TAGS];			//Basic Dicom Tags Structure
	DICOM_ELEMENT	AppProcDataElmnt[NUM_APP_TAGS];				//Application Processing Dicom Tags Structure
	
	DICOM_ELEMENT	DataElmnt[NUM_TAGS];						//Dicom Tags Structure

	DICOM_ELEMENT	DataElmntAux[1];

	DICOM_ELEMENT	DataElmntBirth[1];
	DICOM_ELEMENT	DataElmntPatientID[1];
	DICOM_ELEMENT	DataElmntPatientSex[1];
	DICOM_ELEMENT	DataElmntPatientName[1];

	DICOM_ELEMENT	DataElmntBPart[1];
	DICOM_ELEMENT	DataElmntStudyD[1];
	DICOM_ELEMENT	DataElmntStudyT[1];

	DICOM_ELEMENT	DataElmntInvSV[1];
	DICOM_ELEMENT	DataElmntInvApp[1];

	DICOM_ELEMENT	DataElmntIdentApp[1];
	
	WORD   *pwDCM;	//16-bits, 2 bytes
	
	int nFileNBytes;
	int nAppHeaderSz;

	int iNumbTags;

	
	char cFileName[128];
	
	int nImageFormat;
	WORD wPixelType;		//0.char, 1.unsigned char, 2.short, 3.unsigned short, 4.long, 5.unsigned long, 6.float
	int nAppImage;

	unsigned short	wSamPPixel;
	unsigned short	wPixelRepre;

	int nWidth;
	int nHeight;	
	unsigned short wHighBit;
	unsigned short wBitsStored;
	unsigned short wBitsAllocated;
	int nWindCent;
	int nWindWidth;	

	int nImageSaved;

	DWORD  dwBytesNumber;

	WORD   *pwOriginalPixelValues;
	WORD   *pwPixelValues;
	float  *pfOriginalPixelValues;
	float  *pfPixelValues;

};

#endif
