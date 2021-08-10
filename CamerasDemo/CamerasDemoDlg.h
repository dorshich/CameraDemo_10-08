
// CamerasDemoDlg.h : header file
//

#pragma once
#include <gdiplus.h>
#include <string>

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
	HICON m_hIcon;
	CStatic m_stFrm;
	CStatic m_stFrm2;
	CStatic m_stBarcode;
	CStatic m_stBarcode2;
	Bitmap * m_pImageCaptured;
	GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_gdiplusToken;
	Grabber grabber;

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
