
// CamerasDemoDlg.cpp : implementation file
//
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#include "stdafx.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include "CamerasDemo.h"
#include "CamerasDemoDlg.h"
#include "afxdialogex.h"
#include "DynamSoft/DynamsoftCommon.h"
#include "DynamSoft/DynamsoftBarcodeReader.h"
#include "RecognizeBarcode.h"
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <time.h>
#include <chrono>
#include <ctime>
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iphlpapi.h>
#include <future>
#include <thread>
#include <cstring>

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif
#define DEFAULT_BUFLEN 3
#define DEFAULT_PORT "2000"
CString sBarcodes = L"";
int Flag = 0;
//typedef std::Vector<uchar, 3> Vec3b;
//typedef std::vector<uchar> Vec3b;
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
	api->End();
	delete api;	
	delete m_pImageCaptured;
	pixDestroy(&image);
}

void CCamerasDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ST_FRAM, m_stFrm);
	DDX_Control(pDX, IDC_ST_Frame, m_stFrm2);
	DDX_Control(pDX, IDC_EDIT_BARCODEDFK, m_stBarcode);
	DDX_Control(pDX, IDC_EDIT_BARCODEDFK2, m_stBarcode2);
	//DDX_Control(pDX, IDC_ST_Strlength, m_stStrlength);
	//DDX_Control(pDX, IDC_ST_Strlength2, m_stStrlength2);
	//DDX_Text(pDX, IDC_BarcodeDFK, m_strbarcode);
}

BEGIN_MESSAGE_MAP(CCamerasDemoDlg, CDialog)
	//ON_WM_PAINT()
	//ON_WM_PAINT2()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Photo2, &CCamerasDemoDlg::OnBnClickedPhoto2)
	ON_BN_CLICKED(IDC_Photo, &CCamerasDemoDlg::OnBnClickedPhoto)
END_MESSAGE_MAP()

UINT __cdecl SetSocket(LPVOID pParam);
// CCamerasDemoDlg message handlers

BOOL CCamerasDemoDlg::OnInitDialog()
{
	char* outText=NULL;
	CString sRes;
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
    api = new tesseract::TessBaseAPI();
	//if (api->Init("C:\\vcpkg\\vcpkg\\tessdata", "eng", tesseract::OEM_LSTM_ONLY)) {
		//sRes.Format(L"Could not initialize tesseract.\n");
		//AfxMessageBox(sRes, MB_ICONERROR);
		//exit(1);
	//}
	m_pThread = AfxBeginThread(SetSocket, this);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCamerasDemoDlg::OnPaint(CStatic &Static)
{
	//CDialog::OnPaint();
    pDC = Static.GetDC();
	Static.GetWindowRect(rc);
	PaintBitmapEx(pDC, rc, m_pImageCaptured, FALSE);
	ReleaseDC(pDC);
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



CString CCamerasDemoDlg::GrabImag(const CString &deviceStateFilename, const string &buffer,const string &pass)
{
	CString sRes;
	//DShowLib::InitLibrary();
	
	//cap >> save_img;
	char *outText;
	CT2CA pszConvertedAnsiString(deviceStateFilename);
	// construct a std::string using the LPCSTR input
	std::string devStateFile(pszConvertedAnsiString);
	//const std::string &devStateFile = deviceStateFilename;

	if (!grabber.loadDeviceStateFromFile(devStateFile))
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
	//save_img = fileName;
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


void CCamerasDemoDlg::OnBnClickedPhoto()
{
	int len=0;
	CRect rc;
	tFrameSnapSinkPtr pSink;
	tFrameQueueBufferPtr  m_pLastSnappedBuffer;
	//CString deviceStateFilename = theApp.m_sCamerasConfig; 
	CString deviceStateFilename = L"C:\\Users\\HM_USER\\source\\repos\\Greasing machine\\Greasing machine\\DMK_Right.xml";
	string pass = "C:\\Users\\HM_USER\\source\\repos\\Greasing machine\\Greasing machine\\DMK\\DMK_Right\\DMK_Right";

	string buf = currentDateTime();
	CString sImagePath = GrabImag(deviceStateFilename, buf, pass);
	m_pImageCaptured = new Gdiplus::Bitmap(sImagePath);
	OnPaint(m_stFrm);

	CT2CA pszConvertedAnsiString(sImagePath);
	std::string strpath(pszConvertedAnsiString);

	CRecognizeBarcode QrBarcode;
	std::vector<CString>barcode;
	QrBarcode.DynamSoft_PerformBarcodeRecognition(sImagePath, barcode, TRUE);

	if (barcode.size() > 0)
	{
		sBarcodes = barcode[0];
		m_stBarcode2.SetWindowTextW(sBarcodes);
	}
	else
	{
		m_stBarcode2.SetWindowTextW(NULL);
		CString Spath_Clahe = Clahe(pass, strpath, buf);
		m_pImageCaptured = new Gdiplus::Bitmap(Spath_Clahe);
		OnPaint(m_stFrm);
		barcode.clear();
		QrBarcode.DynamSoft_PerformBarcodeRecognition(Spath_Clahe, barcode, TRUE);

		if (barcode.size() > 0)
		{
			sBarcodes = barcode[0];
			m_stBarcode2.SetWindowTextW(sBarcodes);
		}
		else
		{
			m_stBarcode2.SetWindowTextW(NULL);
			CT2CA pszConvertedAnsiString(Spath_Clahe);
			std::string strpath(pszConvertedAnsiString);
			CString Spath_cleanImage = cleanImage(pass, strpath, buf);
			m_pImageCaptured = new Gdiplus::Bitmap(Spath_cleanImage);
			OnPaint(m_stFrm);
			barcode.clear();
			QrBarcode.DynamSoft_PerformBarcodeRecognition(Spath_cleanImage, barcode, TRUE);

			if (barcode.size() > 0)
			{
				sBarcodes = barcode[0];
				m_stBarcode2.SetWindowTextW(sBarcodes);
			}
			else
			{
				m_stBarcode2.SetWindowTextW(L"<Null>");
			}
		}
	}
}

void CCamerasDemoDlg::OnBnClickedPhoto2()
{
	//char* outText;
	CString sRes;
	int counter = 0;
	int len = 0;
	CRect rc;
	POSITION pos;
	tFrameSnapSinkPtr pSink;
	tFrameQueueBufferPtr  m_pLastSnappedBuffer;
	//CString deviceStateFilename = theApp.m_sCamerasConfig;
	CString deviceStateFilename = L"C:\\Users\\HM_USER\\source\\repos\\Greasing machine\\Greasing machine\\DMK_Left.xml";
	string pass = "C:\\Users\\HM_USER\\source\\repos\\Greasing machine\\Greasing machine\\DMK\\DMK_Left\\DMK_Left";
	//string pass = "C:\\Users\\dor.s\\Desktop\\Cameras\\DMK\\DMK_Left_Try";
	
	string buf = currentDateTime();
	CString sImagePath = GrabImag(deviceStateFilename, buf, pass);
	//CString sImagePath = L"C:\\Users\\dor.s\\Desktop\\Cameras\\DMK_Left2021-09-23_10-53-19.bmp";
	m_pImageCaptured = new Gdiplus::Bitmap(sImagePath);
	OnPaint(m_stFrm2);
	
	CT2CA pszConvertedAnsiString(sImagePath);
	std::string strpath(pszConvertedAnsiString);

	CRecognizeBarcode QrBarcode;
	std::vector<CString>barcode;
	QrBarcode.DynamSoft_PerformBarcodeRecognition(sImagePath, barcode, TRUE);
	//int const size = barcode.size();

	if (barcode.size() > 0)
	{
		sBarcodes = barcode[0];
		/*CFont m_Font;
		m_Font.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Text formatting"));
		m_stBarcode.SetFont(&m_Font, 1);*/
		m_stBarcode.SetWindowTextW(sBarcodes);
	}
	else
	{
		m_stBarcode.SetWindowTextW(NULL);
		CString Spath_Clahe = Clahe(pass,strpath, buf);
		m_pImageCaptured = new Gdiplus::Bitmap(Spath_Clahe);
		OnPaint(m_stFrm2);
		barcode.clear();
		QrBarcode.DynamSoft_PerformBarcodeRecognition(Spath_Clahe, barcode, TRUE);

		if (barcode.size() > 0)
		{
			sBarcodes = barcode[0];
			m_stBarcode.SetWindowTextW(sBarcodes);
		}
		else
		{
			m_stBarcode.SetWindowTextW(NULL);
			CT2CA pszConvertedAnsiString(Spath_Clahe);
			std::string strpath(pszConvertedAnsiString);

			CString Spath_cleanImage = cleanImage(pass, strpath, buf);
			m_pImageCaptured = new Gdiplus::Bitmap(Spath_cleanImage);
			OnPaint(m_stFrm2);
			barcode.clear();
			QrBarcode.DynamSoft_PerformBarcodeRecognition(Spath_cleanImage, barcode, TRUE);

			if (barcode.size() > 0)
			{

				sBarcodes = barcode[0];
				m_stBarcode.SetWindowTextW(sBarcodes);
			}
			else
			{
				m_stBarcode.SetWindowTextW(L"<Null>");				
			}
		}
	}
}
void CCamerasDemoDlg::TextRecognition(CString &sImagePath)
{
	int counter = 0;
	cv::Mat im;
	std::vector<cv::Mat> images;
	cv::Mat save_img;
	CT2A ascii(sImagePath, CP_UTF8); 
	image = pixRead(ascii.m_psz);
	api->SetPageSegMode(tesseract::PSM_AUTO);
	//api->SetImage(image);
	api->SetVariable("tessedit_char_whitelist", "-ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
	api->SetImage(im.data, im.cols, im.rows, im.channels(), im.step);
	if (api->GetUTF8Text() != nullptr)
	{
		outText = string(api->GetUTF8Text());
		std::ofstream file("C:\\Users\\dor.s\\Desktop\\Cameras\\intext.txt");
		file << outText;
	}
	//string file_contents;
	fstream graphic_text;
	graphic_text.open("C:\\Users\\dor.s\\Desktop\\Cameras\\Text Document.txt", ios::in);//Text Document.txt
	//if (graphic_text.is_open()) { //checking whether the file is open
		string file_content;
	//	while (getline(graphic_text, file_contents)) {}
	//	for (string::size_type j = 0; j < outText.size(); ++j)
	//	{

	//		if (j <= file_contents.size()) {
	//			if (outText[j] == file_contents[j])
	//				counter++;
	//		}
	//	}
	//}
	for (string::size_type j = 0; j < outText.size(); ++j) {
		file_content += outText[j];
		if (outText[j] ==' ')
		{
			words.push_back(file_content);		
			file_content.clear();
		}
	}
	for (int i = 0; i < words.size(); i++) {
		if (words[i] == "Equashield " || words[i]=="Medical ")
		{
			counter++;
		}
	}
	vector<string>().swap(words);

}


CString CCamerasDemoDlg::Clahe(const string &pass,string &ImagePath, const string &buffer) {
	std::vector<cv::Mat> images;
	cv::Mat save_img;
	img = cv::imread(ImagePath, 1);
	//detailEnhance(img, save_img, sigma_s, sigma_r);
	cv::split(img, images);
	cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
	clahe->setClipLimit(4);
	clahe->apply(images[0], save_img);
	medianBlur(img, img, 3);
	ImagePath = pass + "_Clahe" + buffer + ".bmp";
	cv::imwrite(ImagePath, save_img); 
	string strStd = "C:\\Users\\dor.s\\Desktop\\enhancecv.bmp";
	//cv::imwrite(strStd, save_img);
	CString sResult(ImagePath.c_str(), ImagePath.length());
	images.clear();
	return sResult;
}


CString CCamerasDemoDlg::cleanImage(const string &pass, string &ImagePath, const string &buffer) {
	cv::Mat new_image;
	img = cv::imread(ImagePath);
	cvtColor(img, img, cv::COLOR_BGR2GRAY);
	//cv::normalize(img, img, 0, 255, cv::NORM_MINMAX);
	//erode(img, img, cv::Mat());
	//cv::Mat src_ROI = img(cv::Rect(33, 19, 250, 110));
	//dilate(img, img, cv::Mat(), cv::Point(0, 0), 4);
	//cv::GaussianBlur(src_ROI, src_ROI, cv::Size(3, 3), 0);
	//cv::cvtColor(img, im, cv::COLOR_RGB2GRAY);

	// calculate the no of pixels for each intensity values
	
	/*threshold(img, img, 0, 255, cv::THRESH_OTSU);

	dilate(img, img, cv::Mat(), cv::Point(0, 0), 2);*/
	//cv::Canny(src_ROI, src_ROI, 140, 40);
	cv::equalizeHist(img, new_image);
	ImagePath = pass + "_cleanImage" + buffer + ".bmp";
	cv::imwrite(ImagePath, new_image);
	CString sResult(ImagePath.c_str(), ImagePath.length());
	return sResult;
	
}

	
CString CCamerasDemoDlg::filter2d_proc(const string &pass, string &ImagePath, const string &buffer)
{
	/*float image_data[9] = { 0, 255, 0, 0, 255, 0, 0, 255, 0 };
	cv::Mat image = cv::Mat(3, 3, CV_32F, image_data);
	std::cout << "image = " << std::endl << image << std::endl;*/
	cv::Mat im;
	img = cv::imread(ImagePath, 0);

	cv::Mat gradientx;
	float sobelx_data[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
	cv::Mat sobelx = cv::Mat(3, 3, CV_32F, sobelx_data);
	std::cout << "sobelx = " << std::endl << sobelx << std::endl;

	cv::filter2D(img, gradientx, -1, sobelx, cv::Point(-1, -1), 0,
		cv::BORDER_DEFAULT);
	std::cout << "gradientx = " << std::endl << gradientx << std::endl;

	ImagePath = pass + "_cleanImage" + buffer + ".bmp";
	cv::imwrite(ImagePath, img);
	CString sResult(ImagePath.c_str(), ImagePath.length());
	return sResult;
}

UINT __cdecl SetSocket(LPVOID pParam)
{
	WSADATA wsaData;
	//CString Barcodes;
	int iResult = 0;
	CString sRes;
	//string ipAddress = "192.168.60.241";

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int iSendResult;
	CString barcodreciv;
	char recvbuf[DEFAULT_BUFLEN];
	//char sendbuf[DEFAULT_BUFLEN];
	char failbuf[6] = "FALSE";
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
	}

	// Setup the TCP listening socket
	iResult = ::bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
	}

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
	}

	// No longer need server socket
	closesocket(ListenSocket);

	// Receive until the peer shuts down the connection
	do {

		iResult = recv(ClientSocket, recvbuf, 3, 0);
		CCamerasDemoDlg *dlg = (CCamerasDemoDlg*)pParam;
		if (iResult > 0) {
			//CWnd* pParent;
			if (recvbuf[0] == 'M') {  //wcslen(sBarcodes) == 46
				dlg->OnBnClickedPhoto2();
				CT2A ascii(sBarcodes);
				iSendResult = send(ClientSocket, ascii, 9, 0);
				if (iSendResult == SOCKET_ERROR) {
					printf("send failed with error: %d\n", WSAGetLastError());
					closesocket(ClientSocket);
					WSACleanup();
				}
				else if (wcslen(sBarcodes) < 5)
				{
					CT2A ascii(L"-1");
					iSendResult = send(ClientSocket, ascii, 2, 0);
					if (iSendResult == SOCKET_ERROR) {
						printf("send failed with error: %d\n", WSAGetLastError());
						closesocket(ClientSocket);
						WSACleanup();
					}
				}
			}
			else if (recvbuf[0] == 'F')
			{
				dlg->OnBnClickedPhoto();
				CT2A ascii(sBarcodes);
				iSendResult = send(ClientSocket, ascii, 9, 0);
				if (iSendResult == SOCKET_ERROR) {
					printf("send failed with error: %d\n", WSAGetLastError());
					closesocket(ClientSocket);
					WSACleanup();
				}
				else if (wcslen(sBarcodes) < 5)
				{
					CT2A ascii(L"-1");
					iSendResult = send(ClientSocket, ascii, 2, 0);
					if (iSendResult == SOCKET_ERROR) {
						printf("send failed with error: %d\n", WSAGetLastError());
						closesocket(ClientSocket);
						WSACleanup();
					}
				}
			}
		}
		else if (iResult == 0)
		{
			sRes.Format(L"Connection closing...\n", iResult);
			AfxMessageBox(sRes, MB_ICONERROR);
		}
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
		}

	} while (iResult > 0);

	// shutdown the connection since we're done
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();
	return 0;
}

void CCamerasDemoDlg::OnBnClickedSendstring2()
{
	Flag = 1;
	CString s;
	m_stBarcode2.SetWindowTextW(NULL);
	m_stStrlength2.GetWindowTextW(s);
	sBarcodes = s;
	m_stStrlength2.SetWindowTextW(NULL);
}






