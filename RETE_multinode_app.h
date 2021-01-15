
// RETE_multinode_app.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CRETEmultinodeappApp:
// See RETE_multinode_app.cpp for the implementation of this class
//

class CRETEmultinodeappApp : public CWinApp
{
public:
	CRETEmultinodeappApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CRETEmultinodeappApp theApp;
