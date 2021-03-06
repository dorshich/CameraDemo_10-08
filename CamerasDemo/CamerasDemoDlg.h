
// CamerasDemoDlg.h : header file
//

#pragma once

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <gdiplus.h>
#include <string>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <tisudshl.h>
#include <algorithm>

using namespace Gdiplus;
using namespace _DSHOWLIB_NAMESPACE;
using namespace std;

#pragma comment (lib, "GdiPlus.lib")
// CCamerasDemoDlg dialog
class CCamerasDemoDlg : public CDialog
{
// Construction
public:
	CCamerasDemoDlg(CWnd* pParent = nullptr);	// standard constructor
	~CCamerasDemoDlg();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERASDEMO_DIALOG };
#endif
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void PaintBitmapEx(CDC* pDC, CRect rc, Bitmap	 * pImage, BOOL bCentr /*= TRUE*/);


// Implementation
protected:
	cv::Mat img;
	HICON m_hIcon;
	CStatic m_stFrm;
	CStatic m_stFrm2;
	CStatic m_stBarcode;
	CStatic m_stBarcode2;
	CStatic m_stStrlength;
	CStatic m_stStrlength2;
	CRect rc;
	tesseract::TessBaseAPI *api;
	Bitmap * m_pImageCaptured;
	GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_gdiplusToken;
	Grabber grabber;
	CDC* pDC;
	Pix* image;
	string outText; 
	std::vector<string> words;
	/*CRecognizeBarcode QrBarcode;
	std::vector<CString>barcode;*/
	CWinThread*m_pThread;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint(CStatic &Static);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

protected:
	CString GrabImag(const CString &devStateFilename,const string &buffer,const string &pass);
	CString cleanImage(const string &pass,string &ImagePath, const string &buffer);
	int computeOutput(const int &x, int &r1, int &s1, int &r2, int &s2);
	CString Clahe(const string &pass,string &ImagePath, const string &buffer);
	CString filter2d_proc(const string &pass, string &ImagePath, const string &buffer);
	void TextRecognition(CString &sImagePath);
	string currentDateTime();
public:
	afx_msg void OnBnClickedPhoto2();
	afx_msg void OnBnClickedPhoto();
	void OnBnClickedSendstring2();
};
