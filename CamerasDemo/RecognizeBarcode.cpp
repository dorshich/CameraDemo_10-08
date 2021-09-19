// RecognizeBarcode.cpp : implementation file
//


#include "stdafx.h"
#include <iostream>
#include <vector>
#include <atlstr.h>
#include <string>
#include "RecognizeBarcode.h"
#include "CamerasDemoDlg.h"
#include "DynamSoft/DynamsoftCommon.h"
#include "DynamSoft/DynamsoftBarcodeReader.h"

#pragma comment(lib, "DBRx64.lib")   //DynamSoft

using namespace dynamsoft::dbr;
using namespace std;
//#define DEBUG_WINDOW

// CRecognizeBarcode

CRecognizeBarcode::CRecognizeBarcode()
{
	m_sDynamSoftLicense = L"f0068MgAAAHPobcPC7hReQsQt9mR/4pRqDHoSRHNzYrnCC3SppdtxuGSRyX+KCEOupffFn8ToLXJ099RKq83xJANWDe3+BqU=";
}

CRecognizeBarcode::~CRecognizeBarcode()
{
}

#define DYNAMSOFT

HRESULT CRecognizeBarcode::DynamSoft_PerformBarcodeRecognition(CString & fileName, std::vector<CString>& barcodeStrings, BOOL bOnlyQR)
{
	tagTextResult barcodestr;
	int iRet = -1;
	int iLicMsg = -1;
	TextResultArray *paryResult = NULL;
	PublicRuntimeSettings runtimeSettings;
	//// convert CString to string
	CT2CA pszConvertedAnsiString(fileName);
	// construct a std::string using the LPCSTR input
	std::string sPath(pszConvertedAnsiString);
	// Initialize license prior to any decoding
	dynamsoft::dbr::CBarcodeReader reader;
	char sError[512];

	// convert path to DynamSoftBarcode.Settings file from cstring to string
	CString sPathToDynamSoftSettings;
	sPathToDynamSoftSettings.Format(L"%sDynamSoftBarcode.Settings", L"C:\\Users\\administrator\\Documents\\IC Imaging Control 3.5\\samples\\VC\\CamerasDemo\\CamerasDemo\\DynamSoft");
	CT2CA pszConvertedDynamSoftSettings(sPathToDynamSoftSettings);
	// construct a std::string using the LPCSTR input
	std::string PathToDynamSoftSettings(pszConvertedDynamSoftSettings);

	// convert DynamSoft license from cstring to string
	CT2CA pszConvertedDynamSoftLicense(m_sDynamSoftLicense);
	// construct a std::string using the LPCSTR input
	std::string DynamSoftLicense(pszConvertedDynamSoftLicense);

	// Check if license type is 'Handshake'
	if (m_sDynamSoftLicense.GetLength() < 20)
	{
		char szErrorMsg[256];
		DM_LTSConnectionParameters ltspar;
		CBarcodeReader::InitLTSConnectionParameters(&ltspar);
		ltspar.handshakeCode = pszConvertedDynamSoftLicense; // Please replace the handshakeCode with your own
		iLicMsg = CBarcodeReader::InitLicenseFromLTS(&ltspar, szErrorMsg, 256);
	}
	else
		iLicMsg = reader.InitLicense(DynamSoftLicense.c_str());

	//If error occurs to the license initialization
	if (iLicMsg != DBR_OK)
	{
		string err = DBR_GetErrorString(iLicMsg);
		CString sErr;
		sErr = err.c_str();
		TRACE2("Failed to initialize the license successfully: %d\r\n%s\r\n", iLicMsg, sErr);
	}

	reader.InitRuntimeSettingsWithFile(PathToDynamSoftSettings.c_str(), CM_OVERWRITE, sError, 512);
	if (bOnlyQR)
	{
		reader.GetRuntimeSettings(&runtimeSettings);
		runtimeSettings.barcodeFormatIds = -2146433025; // for QR 1140850688 for data metrix BF_DATAMATRIX for zebra BF_ONED
		runtimeSettings.barcodeFormatIds_2 = 0;
		runtimeSettings.resultCoordinateType = RCT_PIXEL;
		iRet = reader.UpdateRuntimeSettings(&runtimeSettings, sError, 256);
	}
	// Start decoding. Leave the template name empty ("") will use the settings from PublicRuntimeSettings.
	iRet = reader.DecodeFile(pszConvertedAnsiString, "");
	const char* errorString = CBarcodeReader::GetErrorString(iRet);
	// If error occurs
	if (iRet != DBR_OK)
	{
		string err(DBR_GetErrorString(iRet));
		CString sErr;
		sErr = err.c_str();
		TRACE2("Failed to read barcode: %d %s\r\n", iRet, sErr);
	}

	// If succeeds
	reader.GetAllTextResults(&paryResult);
	TRACE1("%d total barcodes found. \r\n", paryResult->resultsCount);
	for (int iIndex = 0; iIndex < paryResult->resultsCount; iIndex++)
	{
		barcodeStrings.push_back(CString(paryResult->results[iIndex]->barcodeText));
		//paryResult->results[iIndex]->localizationResult->y1;
		//pItem = pRes->m_arrayBarcodes.GetAt(iIndex);
		//pItem->m_sData = paryResult->results[iIndex]->barcodeText;
		//pItem->m_vrData.intVal = iIndex;
		//pRes->m_sMessage += paryResult->results[iIndex]->barcodeText;
		//pRes->m_sMessage += L"";
		//TRACE1("Result %d\r\n", iIndex + 1);
		//TRACE1("BarcodeFormat: %s\r\n", paryResult->results[iIndex]->barcodeFormatString);
		//TRACE1("Text read: %s\r\n", paryResult->results[iIndex]->barcodeText);
		barcodestr.barcodeText = paryResult->results[iIndex]->barcodeText;
		/*std::cout << barcodestr.barcodeText << std::endl;
		str = paryResult->results[iIndex]->barcodeText;*/
	}
	int iIndex = 0;
	//string str = paryResult->results[iIndex]->barcodeText;
	//CWnd *label = GetDlgItem(IDC_BarcodeDFK);
	//label->SetWindowText("blah");
	//HWND hwndText = GetDlgItem(hwndDialog, IDC_BarcodeDFK);
	// Finally release BarcodeResultArray
	dynamsoft::dbr::CBarcodeReader::FreeTextResults(&paryResult);
	return S_OK;
}
