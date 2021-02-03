#pragma once
#include "AlphaNode.h"

// AlphaNodeDlg dialog

class AlphaNodeDlg : public CDialog
{
	DECLARE_DYNAMIC(AlphaNodeDlg)

public:
	AlphaNodeDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~AlphaNodeDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AlphaNodeDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	AlphaNode* currNode;
	CEdit m_content_edit;
	CEdit m_output_edit;
	virtual BOOL OnInitDialog();
	CEdit m_input_edit;
};
