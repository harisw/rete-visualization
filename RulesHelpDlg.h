#pragma once


// RulesHelpDlg dialog

class RulesHelpDlg : public CDialog
{
	DECLARE_DYNAMIC(RulesHelpDlg)

public:
	RulesHelpDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~RulesHelpDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RulesHelpDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
