
// PrinterHub.h : main header file for the PrinterHub application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// PrinterHubApp:
// See PrinterHub.cpp for the implementation of this class
//

class PrinterHubApp : public CWinAppEx
{
public:
	PrinterHubApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	CString PrinterHubApp::GetCSVFilePath();

// Implementation
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern PrinterHubApp theApp;
