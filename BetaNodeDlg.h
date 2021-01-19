#pragma once
#include "BetaNode.h"

// BetaNodeDlg dialog

class BetaNodeDlg : public CDialog
{
	DECLARE_DYNAMIC(BetaNodeDlg)

public:
	BetaNodeDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~BetaNodeDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BetaNodeDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BetaNode* currNode;
	CEdit m_left_edit;
	CEdit m_right_edit;
	CEdit m_content_edit;
	CEdit m_output_edit;
	virtual BOOL OnInitDialog();
};
