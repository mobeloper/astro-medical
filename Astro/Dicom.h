#ifndef GRP_EMG_APP_TAG


#ifdef ___EXPORTS
#define _EXPORT __declspec(dllexport)
#else
#define _EXPORT __declspec(dllimport)
#endif

#ifndef PIXELMAX
#define PIXELMAX	65535
#endif


#define NUM_FULL_TAGS		63			//	Number of Full Tags
#define NUM_BASIC_TAGS		26			//	Number of Basic Tags


//Number of Application Specific (Private) Tags
#define NUM_APP_TAGS		19			

//Aplication Specific (Private) Tags
#define GRP_EMG_APP_TAG		0x0029
#define PRIV_CREA1			0xE011
#define PRIV_CREA2			0xE012
#define PRIV_CREA3			0xE013
#define VIEW_ANN			0xE100
#define INV_PIXELS			0xE200
#define MINPIX				0xE201
#define MAXPIX				0xE202
#define BRGNESS				0xE203
#define TRANS_TYPE			0xE204
#define TRANS_VAL			0xE205
#define BRGNESS_STP			0xE206
#define EDGE_ENH			0xE300
#define EDGE_STP			0xE301
#define SHARP_ENH			0xE302
#define SHARP_STP			0xE303
#define SMTH_ENH			0xE304
#define SMTH_STP			0xE305
#define DENO_ENH			0xE306
#define DENO_STP			0xE307


//Maximum size of the Value of field
#define SIZE_VF		128					


//DICOM DATA ELEMENT STRUCTURE WITH: EXPLICIT VALUE REPRESENTATION 
typedef struct 					//DICOM DATA ELEMENT FIELDS
{
	WORD group_tag;				//Group Number, 16-bit unsigned integers
	WORD element_tag;			//Element Number, 16-bit unsigned integers
	char Tname[128];			//Tag Name, **This is not part of the DICOM element structure, just for GUI Display purposes.
	char VR[4];					//Value Representation, 16-bit character string plus 16 bits reserved (set to 0000h) for special explicit VR.
	int VL;						//VL = Value Length In Bytes, 16 or 32-bit (dependent on VR and whether VR is explicit or implicit)
	char VF[SIZE_VF];			//VF = Value Field of this Dicom element, number of bytes must be according to VR.
} DICOM_ELEMENT;	//DICOM DATA ELEMENT


//DICOM DATA ELEMENT STRUCTURE WITH: EXPLICIT VALUE REPRESENTATION CHAR TYPE
typedef struct 							//DICOM DATA ELEMENT FIELDS
{
	char group_tag[5];			//Group Number, 16-bit unsigned integers
	char element_tag[5];		//Element Number, 16-bit unsigned integers
	char Tname[128];			//Tag Name, **This is not part of the DICOM element structure, just for GUI Display purposes.
	char VR[4];					//Value Representation, 16-bit character string plus 16 bits reserved (set to 0000h) for special explicit VR.
	char VL[10];				//VL = Value Length In Bytes, 16 or 32-bit (dependent on VR and whether VR is explicit or implicit)
	char VF[SIZE_VF];			//VF = Value Field of this Dicom element, number of bytes must be according to VR.
} CHAR_DICOM_ELEMENT;	//DICOM DATA ELEMENT



_EXPORT void SetNumThread(int n);
_EXPORT int GetNumThread();

_EXPORT void FloatToU16(unsigned short *dst,float *src,int n,float min,float max);
_EXPORT void U16ToFloat(float *dst,unsigned short *src,int n);
_EXPORT void S16ToFloat(float *dst,short *src,int n);


_EXPORT int Fsize(FILE *fp);

//_EXPORT int CheckDCMfile(WORD **pwDCMH, DWORD *pNBytes, FILE *dcmfile);
_EXPORT int CheckDCMfile(WORD *pwDCMH, DWORD *pNBytes, FILE *dcmfile);
_EXPORT int GetInfoDCM(DICOM_ELEMENT *sDataElmtDCM, FILE *dcmfile);
_EXPORT BOOL GetDataElmntTag(DICOM_ELEMENT *sDataElmtDCM, FILE *dcmfile,DWORD TagNumber);
_EXPORT BOOL GetDataElmntTagP(DICOM_ELEMENT *sDataElmtDCM, WORD *pwDCMIn,DWORD TagNumber);
_EXPORT void GetImageParam(WORD *pwSampPerPix, WORD *pwHeight, WORD *pwWidth, WORD *pwBitsAllocated, WORD *pwBitsStored, WORD *pwHighBit, WORD *pwPixelRepre, DWORD *pdwWindowCenter, DWORD *pdwWindowWidth, WORD *pwInDCM);
_EXPORT BOOL DataElmt2String(CHAR_DICOM_ELEMENT *sCharDataElmt, DICOM_ELEMENT *sDataElmtDCM, int nElements);
_EXPORT DWORD GetPixelDataInfo(WORD *pwFormat, WORD **pwPixData, WORD *pwInDCM);

_EXPORT int DecimalToChar(char *pbCharOut,int inumber);


_EXPORT int CreateFullDCMTags(DICOM_ELEMENT *FullDCMTags, 
							char *pcMsSOPInstance, char *pSOPInstance, char *pStdyDt, char *pSeriDt, char *pAcqDt, 
							char *pContDt, char *pStdyTm, char *pSrieTm, char *pAcqTm, char *pCntnTm, char *pAccsNum, 
							char *pInstName, char *pSttionName, char *pStdyDesc, char *pSerieDesc, char *pOperName, 
							char *pPatntName, char *pPatntID, char *pPatntBD, char *pPatntBT, char *pPatntSx, 
							char *pPatntAge, char *pBPexam, char *pKVP, char *pExpTM, char *pTubeCu, char *pExposu, 
							char *pViewPos, char *pStdyInsUID, char *pSerInsUID, char *pStdyID, char *pSeriNum, 
							char *pAcquNum, char *pInstaNum, char *pLaterality, char *pImLater, char *pPHMI, 
							int nSPP, int nWidth, int nHeight, int nMinPixelValue, int nMaxPixelValue, int nLevel, int nWindow);

_EXPORT int CreateBasicDCMTags(DICOM_ELEMENT *BasicDCMTags, int nWidth, int nHeight,
								int nMinPixelValue=0, int nMaxPixelValue=1,
								int nWindow=1000, int nLevel=1);

_EXPORT int CreateAppProcessingTags(DICOM_ELEMENT *AppProcTags,
									int nInvertPixels=0, int nMinPixelValue=0, int nMaxPixelValue=1,
									int nBrightness=0, int nEdge=0, int nSharp=0, int nSmooth=0, int nFilter=0);

_EXPORT BOOL SaveAsDCMfromDCMfile(	int nFileNBytes, WORD *pwDCMTags, WORD *pwPixelData, int nWidth, int nHeight, 
									FILE *pDCMfile, DICOM_ELEMENT *AppProcessingTags,
									int nMinPixelValue=0, int nMaxPixelValue=0, int nWindow=0, int nLevel=0,
									int InvertPixels=0, int nEdge=0, int nSmooth=0, int nSharp=0, int nFilter=0);

_EXPORT BOOL SaveRAWfileAsBasicDCM(	WORD *pwPixelData, int nWidth, int nHeight, FILE *pDCMfile, 
									DICOM_ELEMENT *AppProcessingTags,
									DICOM_ELEMENT *BasicDCMTags);

_EXPORT BOOL SaveRAWfileAsFullDCM(	WORD *pwPixelData, int nWidth, int nHeight, FILE *pDCMfile, 
									DICOM_ELEMENT *FullDCMTags);


#endif