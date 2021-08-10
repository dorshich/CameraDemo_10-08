
// CamerasDemoDlg.cpp : implementation file
//
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#include "stdafx.h"
#include "CamerasDemo.h"
#include "CamerasDemoDlg.h"
#include "afxdialogex.h"
#include "DynamSoft/DynamsoftCommon.h"
#include "DynamSoft/DynamsoftBarcodeReader.h"
#include "RecognizeBarcode.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <chrono>

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

// CCamerasDemoDlg dialog
using namespace _DSHOWLIB_NAMESPACE;
using namespace std;

CCamerasDemoDlg::CCamerasDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CAMERASDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CCamerasDemoDlg::~CCamerasDemoDlg()
{
	GdiplusShutdown(m_gdiplusToken);
}

void CCamerasDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ST_FRAM, m_stFrm);
	DDX_Control(pDX, IDC_ST_Frame, m_stFrm2);
	DDX_Control(pDX, IDC_EDIT_BARCODEDFK, m_stBarcode);
	DDX_Control(pDX, IDC_EDIT_BARCODEDFK2, m_stBarcode2);
	//DDX_Text(pDX, IDC_BarcodeDFK, m_strbarcode);
}

BEGIN_MESSAGE_MAP(CCamerasDemoDlg, CDialog)
	//ON_WM_PAINT()
	//ON_WM_PAINT2()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Photo2, &CCamerasDemoDlg::OnBnClickedPhoto2)
	ON_BN_CLICKED(IDC_Photo, &CCamerasDemoDlg::OnBnClickedPhoto)
END_MESSAGE_MAP()


// CCamerasDemoDlg message handlers

BOOL CCamerasDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	_DSHOWLIB_NAMESPACE::InitLibrary();
	GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);
	//CWnd *label = GetDlgItem(IDC_BarcodeDFK);
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	//GetDlgItem(IDOK)->SetWindowText(L"Yes");
	m_pImageCaptured = new Gdiplus::Bitmap(L"C:/Users/dor.s/Desktop/snape/New Bitmap Image.jpg");
	//m_pImageCaptured2 = new Gdiplus::Bitmap(L"C:/Users/dor.s/Desktop/snape/Cyc 5_3.jpg");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCamerasDemoDlg::OnPaint()
{
	CDialog::OnPaint();
	CRect rc;
	CDC* pDC = m_stFrm.GetDC();
	m_stFrm.GetWindowRect(rc);
	PaintBitmapEx(pDC, rc, m_pImageCaptured, FALSE);
	ReleaseDC(pDC);
}

void CCamerasDemoDlg::OnPaint2()
{
	CDialog::OnPaint();
	CRect rc2;
	CDC* pDC2 = m_stFrm2.GetDC();
	m_stFrm2.GetWindowRect(rc2);
	PaintBitmapEx(pDC2, rc2, m_pImageCaptured, FALSE);
	ReleaseDC(pDC2);
}

void CCamerasDemoDlg::PaintBitmapEx(CDC* pDC, CRect rc, Bitmap	 * pImage, BOOL bCentr /*= TRUE*/)
{
	Rect destinationRect;
	Gdiplus::Color clBk;
	clBk.SetFromCOLORREF(0x00FF0000);
	SolidBrush brush(clBk);
	Rect Bound(0, 0, rc.Width(), rc.Height());
	Graphics  gr(pDC->m_hDC);

	gr.FillRectangle(&brush, Bound);
	
	if (pImage)
	{
		/*destinationRect = GetDestinationRect(rc, pImage, bCentr);
		destinationRect.Inflate(-2, -2);*/
		destinationRect = Rect(0, 0, rc.Width(), rc.Height());
		//Draw a portion of the image. Scale that portion of the image
		//so that it fills the destination rectangle.
		gr.DrawImage(
			pImage,
			destinationRect,
			0, 0,				// upper-left corner of source rectangle
			pImage->GetWidth(),				// width of source rectangle
			pImage->GetHeight(),				// height of source rectangle
			UnitPixel);

	}
	
	gr.ReleaseHDC(pDC->GetSafeHdc());
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCamerasDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



CString CCamerasDemoDlg::GrabImag(const string &deviceStateFilename, const string &buffer,const string &pass)
{
	CString sRes;
	//DShowLib::InitLibrary();
	
	const std::string &devStateFilename = deviceStateFilename;
	if (!grabber.loadDeviceStateFromFile(devStateFilename))
	{
		sRes.Format(L" either not found, or device could not be opened from it.");
		AfxMessageBox(sRes, MB_ICONERROR);
	}
	if (!grabber.isDevValid())
	{
		return false;
	}
	//grabber.saveDeviceStateToFile(devStateFilename, true, true, false);
	grabber.setOverlayBitmapPathPosition(ePP_NONE);

	//<<setupsink
		// Create a FrameSnapSink with an image buffer format to eY800.
		// eY800 means monochrome, 8 bits (1 byte) per pixel.	
	tFrameSnapSinkPtr pSink = FrameSnapSink::create(eY800);

	// Set the sink.
	grabber.setSinkType(pSink);

	// Prepare the live mode, to get the output size of the sink.
	if (!grabber.prepareLive(false))
	{
		sRes.Format(L"Could not render the VideoFormat into a eY800 sink.");
		AfxMessageBox(sRes, MB_ICONERROR);
	}

	// Retrieve the output type and dimension of the sink
	// The dimension of the sink could be different from the VideoFormat, when
	// you use filters.
	FrameTypeInfo info;
	pSink->getOutputFrameType(info);
	//>>

	BYTE* pBuf;

	tFrameQueueBufferList lst;
		// Create buffer
		pBuf = new BYTE[info.buffersize];

		// Create a FrameQueueBuffer that objects that wraps the memory of our user buffer
		tFrameQueueBufferPtr ptr;
		Error err = createFrameQueueBuffer(ptr, info, pBuf, info.buffersize, NULL);
		lst.push_back(ptr);
	//}
	//>>

		// Start live mode for fast snapping. The live video will not be displayed,
		// because false is passed to startLive().
	grabber.startLive(false);

	// Snap single image
	Error err1 = pSink->snapSingle(ptr, 2000);
	if (err1.isError()) {
		
		sRes.Format(L"Failed to snap into buffers due to ");
		AfxMessageBox(sRes, MB_ICONERROR);
	}
	
	string fileName = pass;
	fileName += buffer;
	fileName += ".bmp";
	saveToFileBMP(*lst[0], fileName);
	CString sResult(fileName.c_str(), fileName.length()); // convert now to string form
	
	// Stop the live video.
	grabber.stopLive();

	/// Close the device.
	grabber.closeDev();

	lst.clear();

	delete pBuf;
	return sResult;
}
string CCamerasDemoDlg::currentDateTime()
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &tstruct);

	return buf;
}

void CCamerasDemoDlg::OnBnClickedPhoto2()
{
	// TODO: Add your control notification handler code here
	CRect rc;
	CString sBarcodes;
	POSITION pos;
	tFrameSnapSinkPtr pSink;
	tFrameQueueBufferPtr  m_pLastSnappedBuffer;
	string deviceStateFilename = "C:\\Users\\administrator\\Documents\\IC Imaging Control 3.5\\samples\\VC\\CamerasDemo\\lastSelectedDevice.xml";
	string pass = "C:\\Users\\dor.s\\Desktop\\Cameras\\DMK\\DMK";

	string buf = currentDateTime();
	CString sImagePath = GrabImag(deviceStateFilename, buf, pass);
	m_pImageCaptured = new Gdiplus::Bitmap(sImagePath);
	OnPaint2();
	CRecognizeBarcode QrBarcode;
	std::vector<CString>barcode;
	CString sFile(L"C:\\Users\\dor.s\\Desktop\\Cyc 5_3_2.jpg");
	QrBarcode.DynamSoft_PerformBarcodeRecognition(sFile, barcode, TRUE);
	if (barcode.size() > 0)
	{
		for (int i = 0; i < barcode.size(); i++)
		{
			sBarcodes += barcode[i] + L"\r\n";
		}
		CFont m_Font;
	    m_Font.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Text formatting"));
		m_stBarcode.SetFont(&m_Font, 1);
		m_stBarcode.SetWindowTextW(sBarcodes);
	}
	
}



void CCamerasDemoDlg::OnBnClickedPhoto()
{
	CRect rc;
	CString sBarcodes;
	tFrameSnapSinkPtr pSink; 
	tFrameQueueBufferPtr  m_pLastSnappedBuffer;
	string deviceStateFilename= "C:\\Users\\administrator\\Documents\\IC Imaging Control 3.5\\samples\\VC\\CamerasDemo\\lastSelectedDeviceState.xml";
	string pass ="C:\\Users\\dor.s\\Desktop\\Cameras\\DFK\\DFK";

	string buf = currentDateTime();
	CString sImagePath = GrabImag(deviceStateFilename, buf, pass);
	m_pImageCaptured = new Gdiplus::Bitmap(sImagePath);
	OnPaint();
	CRecognizeBarcode QrBarcode;
	std::vector<CString>barcode;
	CString sFile(L"C:\\Users\\dor.s\\Desktop\\Cyc 5_3.jpg");
	QrBarcode.DynamSoft_PerformBarcodeRecognition(sFile, barcode, TRUE);
	if (barcode.size() > 0)
	{
		for (int i = 0; i < barcode.size(); i++)
		{
			L"\r\n";
			sBarcodes += barcode[i] + L"\r\n";
		}
		CFont m_Font;
		m_Font.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Text formatting"));
		m_stBarcode2.SetFont(&m_Font, TRUE);
		m_stBarcode2.SetWindowTextW(sBarcodes);
	}
}


