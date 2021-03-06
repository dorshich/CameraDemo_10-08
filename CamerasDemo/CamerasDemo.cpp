
// CamerasDemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CamerasDemo.h"
#include "CamerasDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//using namespace _DSHOWLIB_NAMESPACE;
// CCamerasDemoApp

BEGIN_MESSAGE_MAP(CCamerasDemoApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCamerasDemoApp construction

CCamerasDemoApp::CCamerasDemoApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CCamerasDemoApp object

CCamerasDemoApp theApp;


// CCamerasDemoApp initialization

BOOL CCamerasDemoApp::InitInstance()
{
	CString sRes;
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("CamerasDemo"));

	// Read/Write config file path from/to registry
	m_sCamerasConfig = GetProfileStringW(L"Settings", L"Config", L"");
	if (m_sCamerasConfig.IsEmpty())
	{
		const TCHAR szFilter[] = _T("XML Files (*.xml)|*.xml|All Files (*.*)|*.*||");
		CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
		//CFileDialog dlg(FALSE, _T("csv"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
		if (dlg.DoModal() == IDOK)
		{
			CString m_sCamerasConfig = dlg.GetPathName();
			// Wrong file
			CString sFileName = dlg.GetFileName();
			if (dlg.GetFileName() != L"DMK_GM.xml")
			{
				sRes.Format(L"Incorrect file selected! ");
				AfxMessageBox(sRes, MB_ICONERROR);
			}
			else
			{
				theApp.WriteProfileStringW(L"Settings", L"Config", m_sCamerasConfig);
			}
		}
	}
	CCamerasDemoDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

