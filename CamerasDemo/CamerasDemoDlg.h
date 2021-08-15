
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

using namespace Gdiplus;
using namespace _DSHOWLIB_NAMESPACE;
using namespace std;

#pragma comment (lib, "GdiPlus.lib")
// CCamerasDemoDlg dialog
class CCamerasDemoDlg : public CDialog
{
// Construction
public:
	CCamerasDemoDlg(CWnd* pParent = NULL);	// standard constructor
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
	int i = 0;
	HICON m_hIcon;
	CStatic m_stFrm;
	CStatic m_stFrm2;
	CStatic m_stBarcode;
	CStatic m_stBarcode2;
	CStatic m_stStrlength;
	CStatic m_stStrlength2;
	tesseract::TessBaseAPI *api;
	Bitmap * m_pImageCaptured;
	GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_gdiplusToken;
	Grabber grabber;
	Pix* image;
	string outText; 
	
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnPaint2();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

protected:
	CString GrabImag(const string &devStateFilename,const string &buffer,const string &pass);
	string currentDateTime();
public:
	afx_msg void OnBnClickedPhoto2();
	afx_msg void OnBnClickedPhoto();
};
