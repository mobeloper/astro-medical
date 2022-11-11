
// DIB.cpp

#include "stdafx.h"
#include "Cdib.h"
#include "math.h"


#include "AstroDoc.h"
#include "AstroView.h"


extern CAstroView *ActiveWnd;

CDib::CDib()
{
	Reset();
}


CDib::~CDib()
{
	DeleteObject();
}



BOOL CDib::SaveDCMfile(LPCTSTR pszFilename)
{

	if (NULL==pwPixelValues)
		return (FALSE);

	char cSaveFileName[128];
	sprintf(&cSaveFileName[0],pszFilename);

	char *pFname = cFileName;
	char *pSFname = cSaveFileName;

	int flgSameName = 1;
	while (*pFname!='\0')
	{
		if(*pSFname++!=*pFname++)
		{
			flgSameName = 0;
			break;
		}
	}


	if(flgSameName==1 && nAppImage==0)
	{
		CWnd msg;
		int nResult = msg.MessageBox("We want to protect the original data. \nYou should save this file with a different name.\n\nAre you sure to CONTINUE?","Wait...",MB_YESNO|MB_ICONQUESTION);
		if(nResult == IDNO)
			return (FALSE);
	}

	BOOL status;

	FILE *dcmfile;
	fopen_s(&dcmfile,pszFilename,"wb");	// open file


	//Create the Extra Tags to be included in the DICOM Header
	nAppHeaderSz = CreateAppProcessingTags(AppProcDataElmnt, ActiveWnd->InvertPixels, nMinPixelValue, nMaxPixelValue,
											ActiveWnd->nBrightness, ActiveWnd->nEdge, ActiveWnd->nSharp, ActiveWnd->nSmooth, ActiveWnd->nFilter);


	if(nImageFormat==1)			//If is DICOM Image
	{
		status = SaveAsDCMfromDCMfile(nFileNBytes, pwDCM, pwPixelValues, nWidth, nHeight, dcmfile, AppProcDataElmnt,
							nMinPixelValue, nMaxPixelValue, ActiveWnd->nWindow, ActiveWnd->nLevel,
							ActiveWnd->InvertPixels, ActiveWnd->nEdge, ActiveWnd->nSmooth, ActiveWnd->nSharp, ActiveWnd->nFilter);
	}
	else	//Convert from RAW to DICOM File
	{

		////Save a RAW file with the Full DICOM Header
		//CollectDICOMHeaderData();
		//status = SaveRAWfileAsFullDCM(pwPixelValues, nWidth, nHeight, dcmfile, FullTagDataElmnt);

		//Save a RAW file with the Basic DICOM Header		
		CreateBasicDCMTags(BasicTagDataElmnt, nWidth, nHeight, nMinPixelValue, nMaxPixelValue,
							ActiveWnd->nWindow, ActiveWnd->nLevel);

		status = SaveRAWfileAsBasicDCM(pwPixelValues, nWidth, nHeight, dcmfile, AppProcDataElmnt, BasicTagDataElmnt);
	}


	fclose(dcmfile);

	nImageSaved = 1;

	return status;
}



BOOL CDib::SaveRAWfile(LPCTSTR pszFilename)
{

	if (NULL==pwPixelValues)
		return (FALSE);

	char cSaveFileName[128];
	sprintf(&cSaveFileName[0],pszFilename);

	char *pFname = cFileName;
	char *pSFname = cSaveFileName;

	int flgSameName = 1;
	while (*pFname!='\0')
	{
		if(*pSFname++!=*pFname++)
		{
			flgSameName = 0;
			break;
		}
	}

	if(flgSameName==1 && nAppImage==0)
	{

		CWnd msg;
		int nResult = msg.MessageBox("We want to protect the original data. \nYou should save this file with a different name.\n\nAre you sure to CONTINUE?","Wait...",MB_YESNO|MB_ICONQUESTION);
		if(nResult == IDNO)
			return (FALSE);
	}
	
	
	if(nImageFormat==1)			//If is DICOM Image
	{
		CWnd msg;
		int nResult = msg.MessageBox("By converting data from DICOM to RAW file the header information will be lost.\n\nAre you sure to CONTINUE?","Wait...",MB_YESNO|MB_ICONQUESTION);
		if(nResult == IDNO)
			return (FALSE);
	}

	BOOL status;

	FILE *pfile;
	fopen_s(&pfile,pszFilename,"wb");	// open file

	WORD *pwImageDCMout = (WORD *)malloc(nWidth*nHeight*sizeof(WORD));
	memcpy(pwImageDCMout,pwPixelValues,nWidth*nHeight*sizeof(WORD));		//Copy the Pixel Data to be saved

	//Save the data
	fwrite(pwImageDCMout,sizeof(WORD),nWidth*nHeight,pfile);		
				
	fclose(pfile);
	free(pwImageDCMout);

	nImageSaved = 1;

	return TRUE;
}






BOOL CDib::CollectDICOMHeaderData()
{

	//Example 1
	//Media Storage SOP Instance data
	char cMsSOPInstance[]="1.2.001704870017053020100415102334190682875";
	char *pcMsSOPInstance = cMsSOPInstance;

	//Example 2
	//SOP Instance
	CString str2;
	str2.Format((CString)"1.2.001704870017053020100415102334190682875");
	//str.Format((CString)"Example %d",2);
	//str.Format(_T("TEST %d"),1);
	
	char SOPInstance[128];
	char *pSOPInstance = SOPInstance;
	memcpy(pSOPInstance,str2,128);		//Even we copy 128 bytes we will take until the NULL value


	struct tm *newtime;
	__time64_t long_time;

	_time64(&long_time); // Get time as long integer. 
	newtime = _localtime64(&long_time); // Convert to local time. 
	
	//char am_pm[] = "AM";
	//if( newtime->tm_hour > 12 ) // Set up extension. 
	//strcpy( am_pm, "PM" );
	//if( newtime->tm_hour > 12 ) // Convert from 24-hour 
	//newtime->tm_hour -= 12; // to 12-hour clock. 
	//if( newtime->tm_hour == 0 ) // Set hour to 12 if midnight.
	//newtime->tm_hour = 12;

	char cDate_Time[32];
	sprintf(&cDate_Time[0],asctime(newtime));

	char cMonth[] = "000000000";
	char *pMon = cMonth;
	DecimalToChar(pMon,(int)(newtime->tm_mon+1));


	////Study Date
	//CString str3;
	//str3.Format((CString)"20110426");
	char StdyDt[9];
	char *pStdyDt = StdyDt;
	StdyDt[0]=cDate_Time[20];
	StdyDt[1]=cDate_Time[21];
	StdyDt[2]=cDate_Time[22];
	StdyDt[3]=cDate_Time[23];
	StdyDt[4]=cMonth[7];
	StdyDt[5]=cMonth[8];
	StdyDt[6]=cDate_Time[8];
	StdyDt[7]=cDate_Time[9];
	StdyDt[8]='\0';	//insert a NULL value

	////Series Date
	//CString str4;
	//str4.Format((CString)"20110426");	
	char SeriDt[9];
	char *pSeriDt = SeriDt;
	SeriDt[0]=cDate_Time[20];
	SeriDt[1]=cDate_Time[21];
	SeriDt[2]=cDate_Time[22];
	SeriDt[3]=cDate_Time[23];
	SeriDt[4]=cMonth[7];
	SeriDt[5]=cMonth[8];
	SeriDt[6]=cDate_Time[8];
	SeriDt[7]=cDate_Time[9];
	SeriDt[8]='\0';	//insert a NULL value

	////Acquisition Date
	//CString str5;
	//str5.Format((CString)"20110426");	
	char AcqDt[9];
	char *pAcqDt = AcqDt;
	AcqDt[0]=cDate_Time[20];
	AcqDt[1]=cDate_Time[21];
	AcqDt[2]=cDate_Time[22];
	AcqDt[3]=cDate_Time[23];
	AcqDt[4]=cMonth[7];
	AcqDt[5]=cMonth[8];
	AcqDt[6]=cDate_Time[8];
	AcqDt[7]=cDate_Time[9];
	AcqDt[8]='\0';	//insert a NULL value

	////Content Date
	//CString str6;
	//str6.Format((CString)"20110426");	
	char ContDt[9];
	char *pContDt = ContDt;
	ContDt[0]=cDate_Time[20];
	ContDt[1]=cDate_Time[21];
	ContDt[2]=cDate_Time[22];
	ContDt[3]=cDate_Time[23];
	ContDt[4]=cMonth[7];
	ContDt[5]=cMonth[8];
	ContDt[6]=cDate_Time[8];
	ContDt[7]=cDate_Time[9];
	ContDt[8]='\0';	//insert a NULL value


	////Study Time
	//CString str7;
	//str7.Format((CString)"125959");	
	char StdyTm[7];
	char *pStdyTm = StdyTm;
	StdyTm[0]=cDate_Time[11];
	StdyTm[1]=cDate_Time[12];
	StdyTm[2]=cDate_Time[14];
	StdyTm[3]=cDate_Time[15];
	StdyTm[4]=cDate_Time[17];
	StdyTm[5]=cDate_Time[18];
	StdyTm[6]='\0';	//insert a NULL value

	////Series Time
	//CString str8;
	//str8.Format((CString)"125959");	
	char SrieTm[7];
	char *pSrieTm = SrieTm;
	SrieTm[0]=cDate_Time[11];
	SrieTm[1]=cDate_Time[12];
	SrieTm[2]=cDate_Time[14];
	SrieTm[3]=cDate_Time[15];
	SrieTm[4]=cDate_Time[17];
	SrieTm[5]=cDate_Time[18];
	SrieTm[6]='\0';	//insert a NULL value

	////Acquisition Time
	//CString str9;
	//str9.Format((CString)"125959");	
	char AcqTm[7];
	char *pAcqTm = AcqTm;
	AcqTm[0]=cDate_Time[11];
	AcqTm[1]=cDate_Time[12];
	AcqTm[2]=cDate_Time[14];
	AcqTm[3]=cDate_Time[15];
	AcqTm[4]=cDate_Time[17];
	AcqTm[5]=cDate_Time[18];
	AcqTm[6]='\0';	//insert a NULL value

	////Content Time
	//CString str10;
	//str10.Format((CString)"125959");	
	char CntnTm[7];
	char *pCntnTm = CntnTm;
	CntnTm[0]=cDate_Time[11];
	CntnTm[1]=cDate_Time[12];
	CntnTm[2]=cDate_Time[14];
	CntnTm[3]=cDate_Time[15];
	CntnTm[4]=cDate_Time[17];
	CntnTm[5]=cDate_Time[18];
	CntnTm[6]='\0';	//insert a NULL value

	//Accesion Number
	CString str11;
	str11.Format((CString)"00170530");	
	char AccsNum[128];
	char *pAccsNum = AccsNum;
	memcpy(pAccsNum,str11,128);

	//Institution Name
	CString str12;
	str12.Format((CString)"DR Customer");	
	char InstName[128];
	char *pInstName = InstName;
	memcpy(pInstName,str12,128);

	//Station Name
	CString str13;
	str13.Format((CString)"PowerCapture01");	
	char SttionName[128];
	char *pSttionName = SttionName;
	memcpy(pSttionName,str13,128);


	//Study Description
	CString str14;
	str14.Format((CString)"CHEST");	
	char StdyDesc[128];
	char *pStdyDesc = StdyDesc;
	memcpy(pStdyDesc,str14,128);


	//Series Description
	CString str15;
	str15.Format((CString)"CHEST PA");	
	char SerieDesc[128];
	char *pSerieDesc = SerieDesc;
	memcpy(pSerieDesc,str15,128);

	//Operator's Name
	CString str16;
	str16.Format((CString)"Tech");	
	char OperName[128];
	char *pOperName = OperName;
	memcpy(pOperName,str16,128);

	//Patient's Name
	CString str17;
	str17.Format((CString)"Eric Michel");	
	char PatntName[128];
	char *pPatntName = PatntName;
	memcpy(pPatntName,str17,128);

	//Patient ID
	CString str18;
	str18.Format((CString)"001700487");	
	char PatntID[128];
	char *pPatntID = PatntID;
	memcpy(pPatntID,str18,128);

	//Patient Birth Date
	CString str19;
	str19.Format((CString)"19660426");	
	char PatntBD[128];
	char *pPatntBD = PatntBD;
	memcpy(pPatntBD,str19,128);

	//Patient Birth Time
	CString str20;
	str20.Format((CString)"051235");	
	char PatntBT[128];
	char *pPatntBT = PatntBT;
	memcpy(pPatntBT,str20,128);

	//Patient Sex
	CString str21;
	str21.Format((CString)"M");	
	char PatntSx[128];
	char *pPatntSx = PatntSx;
	memcpy(pPatntSx,str21,128);

	//Patient's Age
	CString str22;
	str22.Format((CString)"045Y");	
	char PatntAge[128];
	char *pPatntAge = PatntAge;
	memcpy(pPatntAge,str22,128);

	//Part Examined
	CString str23;
	str23.Format((CString)"CHEST");	
	char BPexam[128];
	char *pBPexam = BPexam;
	memcpy(pBPexam,str23,128);

	//KVP
	CString str24;
	str24.Format((CString)"106");	
	char KVP[128];
	char *pKVP = KVP;
	memcpy(pKVP,str24,128);

	//Exposure Time
	CString str25;
	str25.Format((CString)"20");	
	char ExpTM[128];
	char *pExpTM = ExpTM;
	memcpy(pExpTM,str25,128);

	//Tube Current
	CString str26;
	str26.Format((CString)"630");	
	char TubeCu[128];
	char *pTubeCu = TubeCu;
	memcpy(pTubeCu,str26,128);

	//Exposure
	CString str27;
	str27.Format((CString)"12");	
	char Exposu[128];
	char *pExposu = Exposu;
	memcpy(pExposu,str27,128);

	//View Position
	CString str28;
	str28.Format((CString)"PA");	
	char ViewPos[128];
	char *pViewPos = ViewPos;
	memcpy(pViewPos,str28,128);

	//Study Instance UID
	CString str29;
	str29.Format((CString)"1.2.840.113681.2198837555.647.329223406020100415111051");	
	char StdyInsUID[128];
	char *pStdyInsUID = StdyInsUID;
	memcpy(pStdyInsUID,str29,128);

	//Series Instance UID
	CString str30;
	str30.Format((CString)"1.2.840.113681.2198837555.647.329223406020100415111051.1");	
	char SerInsUID[128];
	char *pSerInsUID = SerInsUID;
	memcpy(pSerInsUID,str30,128);

	//Study ID
	CString str31;
	str31.Format((CString)"00170530");	
	char StdyID[128];
	char *pStdyID = StdyID;
	memcpy(pStdyID,str31,128);

	//Series Numb
	CString str32;
	str32.Format((CString)"1");	
	char SeriNum[128];
	char *pSeriNum = SeriNum;
	memcpy(pSeriNum,str32,128);

	//Acquisition Numb
	CString str33;
	str33.Format((CString)"1");	
	char AcquNum[128];
	char *pAcquNum = AcquNum;
	memcpy(pAcquNum,str33,128);

	//Instance Numb
	CString str34;
	str34.Format((CString)"1");	
	char InstaNum[128];
	char *pInstaNum = InstaNum;
	memcpy(pInstaNum,str34,128);

	//Laterality
	CString str35;
	str35.Format((CString)"R");	
	char Laterality[128];
	char *pLaterality = Laterality;
	memcpy(pLaterality,str35,128);

	//Image Laterality
	CString str36;
	str36.Format((CString)"R");	
	char ImLater[128];
	char *pImLater = ImLater;
	memcpy(pImLater,str36,128);


	//Photometric Interpolation
	CString str38;
	str38.Format((CString)"MONOCHROME2");
	char PHMI[128];
	char *pPHMI = PHMI;
	memcpy(pPHMI,str38,128);


	int nSampPerPix = 1;	//This must be a variable depending of the acquisition characteristics


	CreateFullDCMTags(FullTagDataElmnt, 
								pcMsSOPInstance, pSOPInstance, pStdyDt, pSeriDt, pAcqDt, pContDt, pStdyTm, 
								pSrieTm, pAcqTm, pCntnTm, pAccsNum, pInstName, pSttionName, pStdyDesc, pSerieDesc, 
								pOperName, pPatntName, pPatntID, pPatntBD, pPatntBT, pPatntSx, pPatntAge, 
								pBPexam, pKVP, pExpTM, pTubeCu, pExposu, pViewPos, pStdyInsUID, pSerInsUID, 
								pStdyID, pSeriNum, pAcquNum, pInstaNum, pLaterality, pImLater, pPHMI, 
								nSampPerPix, nWidth, nHeight, nMinPixelValue, nMaxPixelValue,ActiveWnd->nLevel,ActiveWnd->nWindow);


	return TRUE;
}






BOOL CDib::Draw(CDC *pDC, int nX, int nY, int ndWidth, int ndHeight, float fZoom, int mode)
//BOOL CDib::Draw(CDC *pDC, int nX, int nY, int sX, int sY, int ndWidth, int ndHeight, float fZoom, int mode)
{

	// FORMAT TEXT
	//pDC->SetTextColor(RGB(255, 25, 2));
	//pDC->SetBkColor(RGB(0, 0, 128));
	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->SetBkColor(RGB(0, 0, 0));
	pDC->SetBkMode(TRANSPARENT);			// TRANSPARENT OPAQUE BKMODE_LAST  
	
	//Graphics graphics(pDC->m_hDC);

	if (pbScreenBits == NULL)
	//if (pDC == NULL)//by eric
	return (FALSE);

	
	// Modiffy Default values
	if (ndWidth == - 1)
		ndWidth = m_pBIH->biWidth;
	if (ndHeight == - 1)
		ndHeight = m_pBIH->biHeight;
	if (fZoom != -1.0F)
	{
		ndWidth = int(ndWidth*fZoom);
		ndHeight = int(ndHeight*fZoom);
	}


	 //Display Image
	pDC->SetStretchBltMode(STRETCH_DELETESCANS);
	StretchDIBits(pDC->m_hDC, nX, nY,  ndWidth, ndHeight, 0, 0, (m_pBIH->biWidth), (m_pBIH->biHeight), pbScreenBits, (BITMAPINFO*)m_pBIH, BI_RGB, mode);

	
	//if(ActiveWnd->flgWriteFootText==1)
		DisplayImgInfo(pDC);

	return (TRUE);
}




BOOL CDib::DisplayImgInfo(CDC *pDC)
{


	//char str[30];
	//sprintf(str,"x=%d",nX);
	//pDC->TextOut(50,800,str);
	//sprintf(str,"y=%d",nY);
	//pDC->TextOut(50,820,str);


	if(nImageFormat == 1)
	{
			CString str;
			str = "Patient ID:";
			pDC->TextOut(TXT_COL,TXT_ROW,str);
			
			if(DataElmntPatientID[0].VF[0] == -51 || DataElmntPatientID[0].VF[0] == '\0')
				pDC->TextOut(TXT_COL+T_INCREMENT,TXT_ROW,(CString)"Not available");
			else
				pDC->TextOut(TXT_COL+T_INCREMENT,TXT_ROW,(CString)DataElmntPatientID[0].VF);
			

			pDC->TextOut(TXT_COL,TXT_ROW+15,(CString)"Patient's Name:");
			if(DataElmntPatientName[0].VF[0] == -51 || DataElmntPatientName[0].VF[0] == '\0')
				pDC->TextOut(TXT_COL+T_INCREMENT,TXT_ROW+15,(CString)"Not available");
			else
				pDC->TextOut(TXT_COL+T_INCREMENT,TXT_ROW+15,(CString)DataElmntPatientName[0].VF);
			
			
			pDC->TextOut(TXT_COL,TXT_ROW+30,(CString)"Patient's Sex:");			
			if(DataElmntPatientSex[0].VF[0] == 'F')
			{
				str.Format((CString)"Female");
				pDC->TextOut(TXT_COL+T_INCREMENT,TXT_ROW+30,str);
			}
			else if(DataElmntPatientSex[0].VF[0] == 'M')
			{
				str.Format((CString)"Male");
				pDC->TextOut(TXT_COL+T_INCREMENT,TXT_ROW+30,str);
			}
			else
				pDC->TextOut(TXT_COL+T_INCREMENT,TXT_ROW+30,(CString)"Not available");


			pDC->TextOut(TXT_COL,TXT_ROW+45,(CString)"Patient's Birthdate:");
			if(DataElmntBirth[0].VF[0] == -51 || DataElmntBirth[0].VF[0] == '\0')
				pDC->TextOut(TXT_COL+T_INCREMENT,TXT_ROW+45,(CString)"00/00/0000");
			else
			{
				str.Format("%s%s/%s%s/%s%s%s%s",(CString)DataElmntBirth[0].VF[4],(CString)DataElmntBirth[0].VF[5],(CString)DataElmntBirth[0].VF[6],(CString)DataElmntBirth[0].VF[7],(CString)DataElmntBirth[0].VF[0],(CString)DataElmntBirth[0].VF[1],(CString)DataElmntBirth[0].VF[2],(CString)DataElmntBirth[0].VF[3]);					
				pDC->TextOut(TXT_COL+T_INCREMENT,TXT_ROW+45,str);
			}

	}
	else
	{
		CString str;
		str = "RAW file";
		pDC->TextOut(TXT_COL,TXT_ROW,str);
	
	}



	return TRUE;
}






//Set Color Palette
BOOL CDib::SetPalette(CDC *pDC)
{
	if (pbScreenBits == NULL)
		return (FALSE);

	
	if (m_Palette.GetSafeHandle() == NULL)
		return (TRUE);

	
	CPalette *pOldPalette;
	pOldPalette = pDC->SelectPalette(&m_Palette, FALSE);
	pDC->RealizePalette();
	pDC->SelectPalette(pOldPalette, FALSE);

	return (TRUE);
}



BOOL CDib::Getsize(LONG * width,LONG * height)
{
	if (pbScreenBits == NULL)
	return (FALSE);

	*width = m_pBIH->biWidth;
	*height = m_pBIH->biHeight;

	return TRUE;

}





//GET THE DICOM TAGS IN PRINTABLE CHARACTERS
BOOL CDib::GetDicomTags(CHAR_DICOM_ELEMENT *charDCMtags)
{
	return DataElmt2String(charDCMtags,DataElmnt,iNumbTags); //Change the DICOM Tags to Printable Characters to be displayed
}




//READ A DICOM IMAGE
BOOL CDib::LoadDCMfile(LPCTSTR pszFilename)
{
	
	FILE *dcmfile;
	fopen_s(&dcmfile,pszFilename,"rb");		// open file

	//Save the name of this file
	sprintf(&cFileName[0],pszFilename);


	//Check the DICOM input file
	int flgEndian;
	

	//if(pwDCM!=NULL)
	if(pwDCM)
	{
		free(pwDCM);
		pwDCM=NULL;
	}


	int initDCMfile = ftell(dcmfile);    //give the initial point of the DICOM file

	char preamble[128];		//These 128 bytes are usually all set to zero
	DWORD *dfType;			//Followed by the letters 'D', 'I', 'C', 'M' to indicate the file type: DICOM
	
	fread(preamble,sizeof(char),128,dcmfile);	// Read the 128 preamble

	dfType=(DWORD *)malloc(sizeof(DWORD));
	fread(dfType,sizeof(DWORD),1,dcmfile);		// Read the 4 char letters of DICM

	// Compare "DICM" or "MCID" according to Endian style
	if(*dfType=='MCID')
		flgEndian = LITTLE_ENDIAN;
	else if (*dfType='DICM')
		flgEndian = BIG_ENDIAN;
	else
	{
		printf("\n\tFAIL: Sorry, this is not a DICOM file!\n\n");
		system("pause");		
		
		fseek(dcmfile,initDCMfile,SEEK_SET); //go back to beginning point of the DICOM file
		return 0;
	}
		
	int offset = (int)(sizeof(preamble) + sizeof(dfType));
	free(dfType);	//free memory space

	nFileNBytes = (int)(Fsize(dcmfile));		// Take the SIZE of the file	


	BYTE *pbDCM;	//8-bits, 1 byte
	pbDCM=(BYTE *)malloc((nFileNBytes-offset)*sizeof(BYTE));
		
	DWORD check;
	check = fread(pbDCM,sizeof(BYTE),(nFileNBytes-offset),dcmfile);		// Address the pointers to the Dicom header
	
	pwDCM = (WORD*)pbDCM;	//Point to the DICOM Elements

	if (check != (nFileNBytes-offset)) //Check if the file is complete
	{
		printf("\n\tFAIL: Broken DICOM file!\n\n");
		system("pause");
		free(pbDCM);
		fseek(dcmfile,initDCMfile,SEEK_SET); //go back to beginning point of the DICOM file
		return 0;
	}

	fseek(dcmfile,initDCMfile,SEEK_SET); //go back to beginning point of the DICOM file



	if(flgEndian!='LE') //Return if is not Little Endian...
	{
		pwDCM=NULL;
		return FALSE;
	}


	GetDataElmntTagP(DataElmntPatientName, pwDCM,PATIENT_NAME);
	GetDataElmntTagP(DataElmntPatientID, pwDCM,PATIENT_ID);
	GetDataElmntTagP(DataElmntBirth, pwDCM,PATIENT_BDATE);
	GetDataElmntTagP(DataElmntPatientSex, pwDCM,PATIENT_SEX);

	GetDataElmntTagP(DataElmntBPart, pwDCM,BODY_PART);
	GetDataElmntTagP(DataElmntStudyD, pwDCM,SDY_DATE);
	GetDataElmntTagP(DataElmntStudyT, pwDCM,SDY_TIME);

	int isLevel = GetDataElmntTagP(DataElmntAux, pwDCM,TAG_LEVEL);
	int isWindow = GetDataElmntTagP(DataElmntAux, pwDCM,TAG_WINDOW);


	//Check if this image has been saved by this Application
	// 0 = Image not created by this application
	// 1 >= Image created by this application	
	DWORD appTag = (DWORD)((GRP_EMG_APP_TAG<<16) | PRIV_CREA1);
	nAppImage = GetDataElmntTagP(DataElmntIdentApp, pwDCM,appTag);
	appTag = (DWORD)((GRP_EMG_APP_TAG<<16) | DENO_STP);
	nAppImage += GetDataElmntTagP(DataElmntIdentApp, pwDCM,appTag);	
	
	//Check If the current image has the pixel values inverted	
	nInvPixelsOr =0;
	int flgInv;
	flgInv = GetDataElmntTagP(DataElmntInvSV, pwDCM,SV_INVERSE);	

	if(flgInv)
		nInvPixelsOr = DataElmntInvSV[0].VF[0]-48;
	
	appTag = (DWORD)((GRP_EMG_APP_TAG<<16) | INV_PIXELS);
	flgInv = GetDataElmntTagP(DataElmntInvApp, pwDCM,appTag);	
	
	if(flgInv)
		nInvPixelsOr = DataElmntInvApp[0].VF[0] - 48;
	

	//Read All DICOM Tags			
	iNumbTags = GetInfoDCM(DataElmnt,dcmfile);

	if(iNumbTags>=(NUM_TAGS-1))
		return FALSE;


	//Get the Image Parameters: width, height and Bit depht... etc.
	WORD *pwSampPPixel		=(WORD *)malloc(sizeof(WORD));
	WORD *pnWidth			=(WORD *)malloc(sizeof(WORD));
	WORD *pnHeight			=(WORD *)malloc(sizeof(WORD));
	WORD *pwBitsAllocated	=(WORD *)malloc(sizeof(WORD));
	WORD *pwBitsStored		=(WORD *)malloc(sizeof(WORD));
	WORD *pwHighBit			=(WORD *)malloc(sizeof(WORD));
	WORD *pwPixelRepre		=(WORD *)malloc(sizeof(WORD));
	DWORD *pdwWindowCenter	=(DWORD *)malloc(sizeof(DWORD));
	DWORD *pdwWindonWidth	=(DWORD *)malloc(sizeof(DWORD));

	GetImageParam(pwSampPPixel,pnHeight,pnWidth,pwBitsAllocated,pwBitsStored,pwHighBit,pwPixelRepre,pdwWindowCenter,pdwWindonWidth,pwDCM);
	
	wSamPPixel = *pwSampPPixel;
	wPixelRepre = *pwPixelRepre;

	nHeight=*pnHeight;
	nWidth=*pnWidth;	
	wBitsAllocated=*pwBitsAllocated;
	wBitsStored=*pwBitsStored;
	wHighBit = *pwHighBit;

	WC = *pdwWindowCenter;
	WW = *pdwWindonWidth;

	free(pwSampPPixel);
	free(pnWidth);
	free(pnHeight);
	free(pwBitsAllocated);
	free(pwBitsStored);
	free(pwHighBit);
	free(pwPixelRepre);
	free(pdwWindowCenter);
	free(pdwWindonWidth);
	
	//This must me modified for another cases, now this cases are not considered
	if(wBitsAllocated<=8)
		wPixelType =0;
	else if(wBitsAllocated<=16)
		wPixelType =3;


	//Get Pixel Data Information
	WORD *pwPixelFormat=(WORD *)malloc(sizeof(WORD));	//Pixel Format
	WORD *pwPixelData=pwDCM;	//Pointer to the Start of Pixel Values

	dwBytesNumber = GetPixelDataInfo(pwPixelFormat, &pwPixelData, pwDCM);	// Get the number of bytes, pixel format (OW or OF) and
																			// Point to the Pixel Data values (using: pwPixelData)
	free(pwPixelFormat);

	//Backup original pixels
	pwOriginalPixelValues=(USHORT*)malloc(nWidth*nHeight*sizeof(USHORT));
	memcpy(pwOriginalPixelValues,pwPixelData,nWidth*nHeight*sizeof(USHORT));
	//Pixel values to display
	pwPixelValues=(USHORT*)malloc(nWidth*nHeight*sizeof(USHORT));
	memcpy(pwPixelValues,pwPixelData,nWidth*nHeight*sizeof(USHORT));
		

	TransformPixelType();		//get float version types to use the Filter.dll library

	//Find the maximum and minimumvalue of the image
	nMaxPixelValue = (int)FindMax(pfOriginalPixelValues,nWidth*nHeight);
	nMinPixelValue = (int)FindMin(pfOriginalPixelValues,nWidth*nHeight);

	nNumbPixels = (1<<wBitsAllocated);	//Get the maximum posible pixel value

	CreateDIB();            //Convert from DICOM (Device Independent Bitmap) header to DIB (Device Independent Bitmap) header
	
	ComputeHistogram();		//Create the image histogram 

	nMaxHistOr = nMaxHist;
	nMinHistOr = nMinHist;                      

	if(isWindow == 0)
		WW =  (int)(nMaxHist-nMinHist);   
	if(isLevel == 0)
		WC = (int)((nMaxHist+nMinHist)*0.5F);	

                                                        
	CreateLUT(WW,WC);			//Create the look-up table for contrast
	CreateScreenBits();			//Transform to Display bits
	
	nImageFormat = 1;			//Indicate that a DICOM file was loaded

	fclose(dcmfile);

	return TRUE;
}




//READ THE RAW IMAGE
BOOL CDib::LoadRAWfile(LPCTSTR pszFilename,int nPixelType, int nImagWidth, int nImagHeight, int nEndian)
//BOOL CDib::LoadRAWfile(FILE *fp,int nPixelType, WORD wImagWidth, WORD wImagHeight)
{

	FILE *fp;
	fopen_s(&fp,pszFilename,"rb");	// open file

	
	//Save the name of this file
	sprintf(&cFileName[0],pszFilename);

	long tmp = Fsize(fp);	//get the file size
	
	//Initialize the global variables
	nWidth = nImagWidth;
	nHeight = nImagHeight;
	wPixelType = (WORD)nPixelType;
	

	//pwPixelValues=(float*)malloc(nWidth*nHeight*sizeof(float));

	pwOriginalPixelValues = (USHORT*)malloc(nWidth*nHeight*sizeof(USHORT));		//store originals
	pwPixelValues = (USHORT*)malloc(nWidth*nHeight*sizeof(USHORT));				//backup originals
	

	if (wPixelType != 3)
	{	
		
		if (tmp != (nWidth*nHeight*sizeof(float)))	// If the user parameters are incorrect...
		{
			fclose(fp);
			return FALSE;
		}
		
		//If we will work with floats
		//fread(pRawData,sizeof(float),nImageWidth*nImageHeight,fp);

		//If we will work with Unsigned Shorts
		float	*ssf;
		ssf=(float*)malloc(nWidth*nHeight*sizeof(float));
		fread(ssf,sizeof(float),nWidth*nHeight,fp);

		
		if(nEndian==1)	//If the image is Big Endian...
		{
			AfxMessageBox(_T("This option is not implemented!"));	
			fclose(fp);
			return FALSE;
		}
		

		//To avoid posible negative values
		float *ptmp = ssf;
	
		for(int idx=0;idx<nWidth*nHeight;idx++)
		{
			if(*ptmp<0)
				*ptmp=0;
			ptmp++;
		}


		float tmpMax, tmpMin;
		
		//FloatToU16(pwOriginalPixelValues,ssf,nWidth*nHeight,tmpMin,tmpMax);
		FloatToU16(pwOriginalPixelValues,ssf,nWidth*nHeight,0,PIXELMAX);
		
		memcpy(pwPixelValues,pwOriginalPixelValues,nWidth*nHeight*sizeof(USHORT));		//Keep originals
		//pwPixelValues = pwOriginalPixelValues;	//backup the original pixels

		//Compute Maximum and Minimum Values
		U16ToFloat(ssf,pwOriginalPixelValues,nWidth*nHeight);
		tmpMax = FindMax(ssf,nWidth*nHeight);
		tmpMin = FindMin(ssf,nWidth*nHeight);

		nMaxPixelValue = (int)tmpMax;
		nMinPixelValue = (int)tmpMin;

		//Convert Backup to float
		pfOriginalPixelValues = (float*)malloc(nWidth*nHeight*sizeof(float));
		memcpy(pfOriginalPixelValues,ssf,nWidth*nHeight*sizeof(float));		 //Keep originals
		//Pixels to work with
		pfPixelValues=(float*)malloc(nWidth*nHeight*sizeof(float));		
		memcpy(pfPixelValues,pfOriginalPixelValues,nWidth*nHeight*sizeof(float));		//Keep originals

		free(ssf);

		wPixelType = 3;	//Now we work with unsigned short variable type
	}
	else
	{ 
		
		if (tmp != (nWidth*nHeight*sizeof(USHORT)))	// If the user parameters are incorrect...
		{
			fclose(fp);
			return FALSE;
		}


		//we will work with Unsigned Shorts
		fread(pwOriginalPixelValues,sizeof(USHORT),nWidth*nHeight,fp);

		if(nEndian==1)	//If the image is Big Endian...
		{
			BYTE *pbVal = (BYTE *)pwOriginalPixelValues;
			USHORT bLowByte;
			USHORT bHighByte;

			for(int idx=0;idx<nWidth*nHeight;idx++)
			{
				bLowByte = *pbVal++;
				bHighByte = *pbVal++;

				pwOriginalPixelValues[idx] = (USHORT)((bLowByte<<8)|bHighByte);
			}
		}

		memcpy(pwPixelValues,pwOriginalPixelValues,nWidth*nHeight*sizeof(USHORT));		//Keep originals

		TransformPixelType();	//transform the image from U16 to float, to use the Filter.dll library
	
		//Find the maximum and minimumvalue of the image
		nMaxPixelValue = (int)FindMax(pfOriginalPixelValues,nWidth*nHeight);
		nMinPixelValue = (int)FindMin(pfOriginalPixelValues,nWidth*nHeight);

		
		wPixelType = 3;	//3.unsigned short
	}

	fclose(fp);
	
	nImageFormat = 0;	//Indicate that a RAW file was loaded
	nAppImage=0;		//Indicates is not an application saved image

	//This may need to be modified
	wBitsAllocated = 16;
	nNumbPixels = (1<<wBitsAllocated);
		
	CreateDIB();	//Create the Bitmap Image
	
	ComputeHistogram();		//Create the image histogram
	
	nMaxHistOr = nMaxHist;
	nMinHistOr = nMinHist;

	nInvPixelsOr = 0;	//Default value for all RAW files

	WW =  (int)(nMaxHist-nMinHist);
	WC = (int)((nMaxHist+nMinHist)*0.5F);
	
	CreateLUT(WW,WC);	//Create the look-up table for contrast
	CreateScreenBits();	//Adjust the image contrast
		
	return (TRUE);
}

// BOOL CDib::LoadBitmap(LPCSTR filename)
// {
// 	CFile file(filename,CFile::modeRead|CFile::shareDenyNone);
// 
// 	WORD bfType;
// 	DWORD   bfSize; 
// 
// 	
// 	file.Read(&bfType,sizeof(WORD));
// 	file.Read(&bfSize,sizeof(DWORD));
// 	if(bfSize <= 0)
// 	{
// 		file.Close();
// 		return -1;
// 	}
// 
// 	//BMP ??
// 	if (bfType != (((WORD)'M'<<8) + 'B'))
// 	{
// 		file.Close();
// 		return -1;
// 	}
// 
// 	DWORD   bfOffBits; 
// 	
// 	file.Seek(2*sizeof(WORD),CFile::current);
// 	file.Read(&bfOffBits,sizeof(DWORD));
// 
// 	LPVOID DibBuf;
// 
// 	DibBuf = (LPVOID)new char[bfSize];
// 
// 	if(DibBuf == NULL)
// 	{
// 		file.Close();
// 		return -1;
// 	}
// 
// 	
// 	file.Read(DibBuf,bfSize);
// 	file.Close();
// 
// 	LPBITMAPINFO pBMI;
// 
// 	int size = bfOffBits - 14;
// 	pBMI = (LPBITMAPINFO)new char[sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD)];
// 
// 	
// 	memcpy(pBMI,DibBuf,size);
// 	LPVOID lpBits = (LPVOID)((LPSTR)DibBuf + size);
// 
// 	
// 	CreateDIBIndirect(pBMI, lpBits);
// 
// 	delete pBMI;
// 	delete DibBuf;
// 	return 1;
// }

// BOOL CDib::CreateDIBIndirect(LPBITMAPINFO pBMI, const void* lpBits)
// {
// 	if (m_hObject != NULL)
// 	{
// 		DeleteObject();
// 	}
// 
// 	// Create the DIB section.
// 	
// 	CDC *pDC = new CDC;
// 	pDC->CreateCompatibleDC(NULL);
// 	LPVOID pBits;
// 	HBITMAP hDIB = ::CreateDIBSection(pDC->GetSafeHdc(),
// 		pBMI,
// 		DIB_RGB_COLORS,
// 		&pBits,
// 		NULL,
// 		0);
// 	delete pDC;
// 
// 	ASSERT(hDIB);
// 	ASSERT(pBits);
// 	Attach(hDIB);
// 
// 	
// 	SetDIBBits(GetWidthBytes() * GetHeight(), lpBits);
// 
// 	return TRUE;
// }


//Creates the image histogram
BOOL CDib::ComputeHistogram()
{
	
	int *pHisto = nHistogram;
	int nSum_Hist[PIXELMAX];
	
	//Initialize Values
	for( int i=0; i<PIXELMAX; i++ )
	{
		nSum_Hist[i]	 = 0;
		pHisto[i]		 = 0;
	}

	//Get the Histogram
	for( int j = 0; j< (nWidth*nHeight) ; j++ )
	{		
		pHisto[pwPixelValues[j]]++;
	}


	int nSum=0;
	for( int x = 0; x < PIXELMAX ; x++ )
	{
		nSum = nSum + nHistogram[x];
		nSum_Hist[x] = nSum;

		// Calculate Min index value for Contrast Window
		if(	 nSum_Hist[x-1]	 <= (nWidth*nHeight) * OPTIMIZEDRANGE && 
			nSum_Hist[x]	 >= (nWidth*nHeight) * OPTIMIZEDRANGE )	
			nMinHist = x; 

		// Calculate Max value for Contrast Window
		else if( nSum_Hist[x-1]	<= (nWidth*nHeight) * ( 1- OPTIMIZEDRANGE ) && 
				nSum_Hist[x]	>= (nWidth*nHeight) * ( 1- OPTIMIZEDRANGE ) )
		{
			nMaxHist = x;
			if( nMaxHist >= PIXELMAX ) nMaxHist = PIXELMAX;
		}
		
	}

	
	return TRUE;
}


//Creates the DIB image for display purposes
BOOL CDib::CreateDIB()
{
	unsigned char *ppDib;
	ppDib = new unsigned char[sizeof(BITMAPINFOHEADER)+ 256*sizeof(RGBQUAD)+nWidth*nHeight];

	if(ppDib==NULL) 
		return(false);

	if(m_pDib!=NULL) 
		delete m_pDib;

	m_pDib=ppDib;

	m_dwDibSize = (nWidth*nHeight);

	m_pBIH=(BITMAPINFOHEADER*)m_pDib;
	m_pPalette = (RGBQUAD*) &m_pDib[sizeof(BITMAPINFOHEADER)];
	pbScreenBits = &m_pDib[sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)];

	
	for(int i=0;i<256;i++)
	{
		 m_pPalette[i].rgbBlue=(byte)i;
		 m_pPalette[i].rgbGreen=(byte)i;
		 m_pPalette[i].rgbRed=(byte)i;
		 m_pPalette[i].rgbReserved=0;
	}

	m_nPaletteEntries=256;


	m_pBIH->biWidth=nWidth;
	m_pBIH->biHeight=nHeight;
	m_pBIH->biBitCount=8;
	m_pBIH->biPlanes=1;
	m_pBIH->biSizeImage = m_dwDibSize;
	m_pBIH->biClrUsed=256;
	m_pBIH->biClrImportant=256;
	m_pBIH->biXPelsPerMeter=0;
	m_pBIH->biYPelsPerMeter=0;
	m_pBIH->biCompression=0;
	m_pBIH->biSize=40;

	return true;	
}



//Creates a float image type
BOOL CDib::TransformPixelType()
{
	//Convert Backup to float
	pfOriginalPixelValues=(float*)malloc(nWidth*nHeight*sizeof(float));
	U16ToFloat(pfOriginalPixelValues,pwOriginalPixelValues,nWidth*nHeight);	

	//Pixels to work with
	pfPixelValues=(float*)malloc(nWidth*nHeight*sizeof(float));		
	memcpy(pfPixelValues,pfOriginalPixelValues,nWidth*nHeight*sizeof(float));		//Keep originals

	return TRUE;
}



//Image processing tools sequence
BOOL CDib::ImageTransformation(int nInvert, int nEdge, int nSharp, int nSmooth, int nDenoise, int nGamma)
{

	memcpy(pfPixelValues,pfOriginalPixelValues,nWidth*nHeight*sizeof(float));

	float *pfTemp=(float*)malloc(nWidth*nHeight*sizeof(float));
	memcpy(pfTemp,pfPixelValues,nWidth*nHeight*sizeof(float));
	
	

	if (nInvert==1)
	{
		ImInvertPixels(pfPixelValues, pfTemp,(float)nMinPixelValue, (float)nMaxPixelValue,nWidth,nHeight);		
		memcpy(pfTemp,pfPixelValues,nWidth*nHeight*sizeof(float));
	}

	if (nDenoise>0)
	{
		//Chooos one of this methods:

		//Geometric Nonlinear Diffusion Filter
		GeomDiff(pfPixelValues, pfTemp, nWidth, nHeight, nDenoise, 0.25,0);
		
		////Median Filter
		//float *sax;
		//sax =(float*)malloc(nWidth*nHeight*sizeof(float));
		//for(int idx=0; idx<nDenoise; idx++)
		//{	
		//	memcpy(sax,idx?pfPixelValues:pfTemp,nWidth*nHeight*sizeof(float));
		//	Median3x3(pfPixelValues,sax,nWidth,nHeight);
		//}
		//free(sax);

		memcpy(pfTemp,pfPixelValues,nWidth*nHeight*sizeof(float));
	}
	
	if (nSmooth>0)
	{
		//Chooos one of this methods:

		////Option 1 General... but Slower
		//float *sax;
		//sax =(float*)malloc(nWidth*nHeight*sizeof(float));
		//for(int idx=0; idx<nSmooth; idx++)
		//{	
		//	memcpy(sax,idx?pfPixelValues:pfTemp,nWidth*nHeight*sizeof(float));
		//	//Average3x3(pfPixelValues,sax,nWidth,nHeight);
		//	Average9x9(pfPixelValues,sax,nWidth,nHeight);
		//}
		//free(sax);

		//Option 2, Slow... but General
		AverageNxN(pfPixelValues,pfTemp,nWidth,nHeight,(2*nSmooth-1));	//(2*nSmooth-1) to use an odd window

		////Option 3, Faster... but limited to 9x9
		//if (nSmooth==1)
		//	Average3x3(pfPixelValues,pfTemp,nWidth,nHeight);
		//if (nSmooth==2)
		//	Average5x5(pfPixelValues,pfTemp,nWidth,nHeight);
		//if (nSmooth==3)
		//	Average7x7(pfPixelValues,pfTemp,nWidth,nHeight);
		//if (nSmooth==4)
		//	Average9x9(pfPixelValues,pfTemp,nWidth,nHeight);


		memcpy(pfTemp,pfPixelValues,nWidth*nHeight*sizeof(float));
		
	}

	if (nSharp>0)
	{
		//ImSharp(pfPixelValues, pfTemp,nWidth,nHeight, nSharp, 25);	//Sharp edges, unsharp window size = 25
		ImSharp9x9(pfPixelValues, pfTemp,nWidth,nHeight, nSharp);	//Sharp edges, unsharp window size = 25
		memcpy(pfTemp,pfPixelValues,nWidth*nHeight*sizeof(float));
	}


	if (nEdge>0)
	{
		ImEdges(pfPixelValues, pfTemp,nWidth,nHeight, nEdge);	//Edge Enhancement Function
		memcpy(pfTemp,pfPixelValues,nWidth*nHeight*sizeof(float));
	}


	if (nGamma != 5)
	{

		float fGamma[] = GAMMA_VALS;
				
		CreateGammaLUT((float)(1/fGamma[nGamma]));	//Build the Gamma LUT
		//CreateGammaLUT((float)(fGamma[nGamma]));	//Build the Gamma LUT

		float *ptmpOr = pfTemp;
		float *ptmpOut = pfPixelValues;

		for(int idx=0;idx<nWidth*nHeight; idx++)
		{
				ptmpOut[idx] = wGammaLUT[(WORD)ptmpOr[idx]];
		}

		memcpy(pfTemp,pfPixelValues,nWidth*nHeight*sizeof(float));
	}

	//Get the transformed pixels
	memcpy(pfPixelValues,pfTemp,nWidth*nHeight*sizeof(float));
	
	free(pfTemp);

	//To avoid values out of the limit...
	for(int idx=0; idx<nWidth*nHeight; idx++)
	{
		if(pfPixelValues[idx]<0)
			pfPixelValues[idx]=0;

		if(pfPixelValues[idx]>nMaxPixelValue)
			pfPixelValues[idx]=(float)(nMaxPixelValue);
	}
	
	FloatToU16(pwPixelValues,pfPixelValues,nWidth*nHeight,0,(float)(nNumbPixels-1));			//Convert to USHORT	
	
	ComputeHistogram();		//Update the image histogram

	return TRUE;
}





//Create the Look-Up Table for Power-Law (Gamma) Transformation
BOOL CDib::CreateGammaLUT(float fPrecompensation)
{	

	float dValue;
	double auxNorm = (double)(1/(nMaxPixelValue+0.0005F));				//Normalization constant

	for (int idx=0; idx<=(nMaxPixelValue); idx++) 
	{
		dValue = (float)((idx+0.0005F)*auxNorm);						//Normalize the value
		dValue = (float)pow((float)dValue,(float)fPrecompensation);	//Apply Power-Law (Gamma) Transformation
		wGammaLUT[idx] = (WORD)(dValue*nMaxPixelValue-0.005F);			//Scale the value back to the original
	}

	return TRUE;
}




//Create the Look-Up Table for Brightness and Contrast Adjustment
BOOL CDib::CreateLUT(int nWindowVal,int nLevelVal)
{	

	int idx=0;
	int il,ih;
	double high,low;

	BYTE lv,hv;

	int nLutSize = nNumbPixels;

	if (nImageFormat>USHORT_TYPE) return FALSE;

	low = (double)(nLevelVal - nWindowVal*0.5F);
	high = (double)(low + nWindowVal);

	if (high<low) 
		lv=255, hv=0;
	else 
		lv=0, hv=255;


	il=(int)min(0,low);
	ih=(int)min((nLutSize-1),high);

	if(il<0) il=0;
	if(ih<0) ih=0;

	double tmpWnd =  255.0F/float(nWindowVal+0.0000005F);

	for (idx=0; idx<il; idx++) 
		bLUT[idx]=lv;	

	for (; idx<ih; idx++) 
	{
		
		float ftmp = float((idx-low)*tmpWnd);

		//Used to avoid overflow
		if(ftmp>255)ftmp=255;
		if(ftmp<0)ftmp=0;

		bLUT[idx]=(BYTE)(ftmp);
	}

	for (; idx<=(nMaxPixelValue); idx++) 
		bLUT[idx]=hv;


	return true;
}


//Creates the final display pixel values
BOOL CDib::CreateScreenBits()
{	
	
	BYTE *pSb = pbScreenBits;	//pointer to the screen bits	

	char *pt8=(CHAR*)pwPixelValues;
	UCHAR *pt8u=(UCHAR*)pwPixelValues;
	short *pt16=(short*)pwPixelValues;
	USHORT *pt16u =(USHORT*)pwPixelValues;
			
	USHORT temp;

	switch (wPixelType)
	{	case 0:
			//Not tested
			for (int i=nWidth*nHeight; i; i--) *pSb++=bLUT[*pt8++ + 128];
			break;
		case 1:
			//Not tested
			for (int i=nWidth*nHeight; i; i--) *pSb++=bLUT[*pt8u++];
			break;
		case 2:
			//Not tested
			for (int i=nWidth*nHeight; i; i--) *pSb++=bLUT[*pt16++];
			break;
		case 3:
			//Only this is implemented
			for(int rows=0; rows<nHeight;rows++)
			{
				for(int cols=0;cols<nWidth;cols++)
				{
					//Align the correct order pixels
					temp = *(pt16u + (nHeight-rows-1)*nWidth + cols);
					*(pSb+rows*nWidth+cols)=bLUT[temp];
				}
			}
			break;
		case 4:
			//Not tested
			break;
		case 5:
			//Not tested
			break;
		default:
			//Not tested
			for (int i=nWidth*nHeight; i; i--) *pSb++=bLUT[*pt8u++];
			break;
	}


	return true;
}



//Change the image display color palette
BOOL CDib::FakeColor(byte R, byte G, byte B)          //	Put some color to the B&W image
{
	if(pwPixelValues==NULL) 
		return false;
	
	double r,g,b;
	b=double(B/105.0f);                               
	g=double(G/105.0f);
	r=double(R/105.0f);

	for(int i=0;i<256;i++)
	{

		m_pPalette[i].rgbBlue=byte(i*b);
		m_pPalette[i].rgbGreen=byte(i*g);
		m_pPalette[i].rgbRed=byte(i*r);

		m_pPalette[i].rgbReserved=0;
	}

	return true;
}



//Display the original color palette (gray)
void CDib::Recovery()                          
{
	pwPixelValues=pwOriginalPixelValues;

	for(int i=0;i<256;i++)
	{
		m_pPalette[i].rgbBlue=(byte)i;
		m_pPalette[i].rgbGreen=(byte)i;
		m_pPalette[i].rgbRed=(byte)i;
		m_pPalette[i].rgbReserved=0;
	}
}


//Reset all the values to the initial condition
void CDib::Reset()
{

	pwDCM = NULL;

	pwOriginalPixelValues=NULL;
	pfOriginalPixelValues=NULL;
	pfPixelValues=NULL;
	pwPixelValues=NULL;
	
	m_pDib = NULL;
	pbScreenBits = NULL;
	m_pBIH = NULL;
	m_pPalette = NULL;

	m_dwDibSize = 0;
	m_nPaletteEntries = 0;
	WW=0;
	WC=0;

}


//Free memory
void CDib::DeleteObject()
{

	if(pwPixelValues!=NULL)
		free(pwPixelValues);

	if(pfOriginalPixelValues!=NULL)
		delete []pfOriginalPixelValues;

	if(pfPixelValues!=NULL)
		free(pfPixelValues);

	if(pwOriginalPixelValues!=NULL)
		delete []pwOriginalPixelValues;

	if(pwDCM!=NULL)
		free(pwDCM);

	if(m_pDib!=NULL) 
		delete []m_pDib;


	Reset();
}

