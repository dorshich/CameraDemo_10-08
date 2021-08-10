#pragma once

#include <string>
#include <vector>

class CRecognizeBarcode
{
public:
	CRecognizeBarcode();
	virtual ~CRecognizeBarcode();

public:
	CString m_sDynamSoftLicense;
	HRESULT DynamSoft_PerformBarcodeRecognition(CString &fileName, std::vector<CString> &barcodeStrings, BOOL bOnlyQR = FALSE);
};



