
// CamerasDemo.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CCamerasDemoApp:
// See CamerasDemo.cpp for the implementation of this class
//

class CCamerasDemoApp : public CWinApp
{
public:
	CCamerasDemoApp();
	CString m_sCamerasConfig;
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CCamerasDemoApp theApp;
