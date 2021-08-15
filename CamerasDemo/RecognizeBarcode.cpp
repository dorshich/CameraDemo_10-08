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
//#ifdef DYNAMSOFT
//	vector<CString> barcodes;
//	m_pReq = pReq;
//	//Check if need stretch
//	//// convert CString to string
//	CT2CA pszConvertedAnsiString(pReq->m_sSorces[0]);
//	// construct a std::string using the LPCSTR input
//	std::string sPath(pszConvertedAnsiString);
//	
//	if (S_OK != DynamSoft_PerformBarcodeRecognition(pReq->m_sSorces[0], barcodes, TRUE))
//	{
//		pRes->m_sMessage = CString("Barcode reading failed");
//		pRes->m_hr = E_FAIL;
//	}
//	//If NOT found in original image - try stretching
//	else if (0 == barcodes.size()) 
//	{		
//		StretchImage(sPath, pReq->m_iStretchX / 100, pReq->m_iStretchY / 100);
//		if (S_OK != DynamSoft_PerformBarcodeRecognition(pReq->m_sSorces[0], barcodes, TRUE)) 
//		{
//			pRes->m_sMessage = CString("Barcode reading failed");
//			pRes->m_hr = E_FAIL;
//		}
//		//If NOT found in stretched image also
//		else if (0 == barcodes.size()) 
//		{
//			pRes->m_wMessageType = EVENTLOG_ERROR_TYPE;
//			pRes->m_sMessage = CString("Barcode not found");
//			pRes->m_hr = EQ_NEGATIVE_RESULT;
//		}
//		// If barcode recognized (in stretched image)
//		else 
//		{
//			pRes->m_wMessageType = EVENTLOG_INFORMATION_TYPE
//			pRes->m_sMessage = barcodes[0];
//			pRes->m_iLotExpIndex = m_lBarcodeTop;
//			for (size_t i = 0; i < min(int(barcodes.size()), int(pRes->m_arrayBarcodes.GetCount())); i++) 
//			{
//				CResponseDataItem *pItem = pRes->m_arrayBarcodes.GetAt(i);
//				pItem->m_sData = barcodes[i];
//				pItem->m_vrData.intVal = int(i);
//			}
//			pRes->m_hr = S_OK;
//		}					
//	}
//	// If barcode recognized (in original image)
//	else
//	{
//		pRes->m_wMessageType = EVENTLOG_INFORMATION_TYPE;
//		pRes->m_sMessage = barcodes[0];
//		pRes->m_iLotExpIndex = m_lBarcodeTop;
//		for (size_t i = 0; i < min(int(barcodes.size()), int(pRes->m_arrayBarcodes.GetCount())); i++)
//		{
//			CResponseDataItem *pItem = pRes->m_arrayBarcodes.GetAt(i);
//			pItem->m_sData = barcodes[i];
//			pItem->m_vrData.intVal = int(i);
//		}
//		pRes->m_hr = S_OK;
//	}	
//#endif
//}
//
//void CRecognizeBarcode::RecognizeApprovalStickerBarcode(CRequest * pReq, CResponse * pRes)
//{
//	// Actual Function
//	RecognizeStickerBarcode(pReq, pRes);
//}
//
//// Private Area

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
		runtimeSettings.barcodeFormatIds = BF_ONED; // for QR 1140850688 for data metrix BF_DATAMATRIX
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
